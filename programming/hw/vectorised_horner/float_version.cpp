#include <iostream>
#include <random>
#include <chrono>

extern "C"
{
#include <immintrin.h>
}

using namespace std;

void report(
    int trial,
    float R_standard,
    auto duration_horner,
    float R_intrinsic,
    auto duration_horner_intrinsic);

float horner(float h, float *a, long degree)
{
    float Y = 0.0;
    int i;

    for (i = 0; i < degree; i++)
    {
        Y = (Y + a[i]) * h;
    }

    return Y;
}

float horner_intrinsic(float h, float *a, long degree)
{
    // h is the evaluation point
    // a points to the coefficients as floats
    float *R, P;
    int i;
    __m256 *A, X, Y;

    // A points to the coefficients as vectors of 8 floats
    A = (__m256 *)a;

    X = _mm256_set1_ps(h * h * h * h * h * h * h * h);
    Y = _mm256_set1_ps(0.0);

    for (i = 0; i < degree / 8 - 1; i++)
    {
        Y = _mm256_add_ps(Y, A[i]);
        Y = _mm256_mul_ps(Y, X);
    }

    Y = _mm256_add_ps(Y, A[i]); // i = degree / 8 - 1
    R = (float *)(&Y);

    P = R[7] * h;
    P += R[6] * h * h;
    P += R[5] * h * h * h;
    P += R[4] * h * h * h * h;
    P += R[3] * h * h * h * h * h;
    P += R[2] * h * h * h * h * h * h;
    P += R[1] * h * h * h * h * h * h * h;
    P += R[0] * h * h * h * h * h * h * h * h;

    return P;
}

int main()
{
    float R_intrinsic, R_standard, *coefficients, h = 1.1;
    int i, j;
    long trials = 1000;

    /* when the degree n of the polynomial is not divisible by 8,
    we need to complete with 8*ceil(n/8) - n zeros the array of
    coefficients.
    */
    long degree = 10000;
    int padding = 8 * ceil(degree / 8.0) - degree;
    /* padding in {0, 1, 2, 3, 4, 5, 6, 7}. So this adds, at most,
    7 multiplications by zero */

    cout << "degree: " << degree << endl;
    cout << "how much padding: " << padding << endl;

    coefficients = (float *)_mm_malloc((degree + padding) * sizeof(float), 32);

    for (i = 0; i < padding; i++)
    {
        coefficients[i] = 0.0;
    }

    // random number generation
    random_device device;
    mt19937 gen(device());
    uniform_real_distribution<double> distribution(0.0, 0.01);

    for (j = 0; j < trials; j++)
    {
        for (i = padding; i < degree + padding; i++)
        {
            coefficients[i] = (float)distribution(gen);
        }

        auto start_horner_intrinsic = chrono::high_resolution_clock::now();
        R_intrinsic = horner_intrinsic(h, coefficients, degree + padding);
        auto end_horner_intrinsic = chrono::high_resolution_clock::now();

        auto duration_horner_intrinsic =
            chrono::duration_cast<chrono::nanoseconds>(
                end_horner_intrinsic - start_horner_intrinsic)
                .count();

        auto start_horner = chrono::high_resolution_clock::now();
        R_standard = horner(h, coefficients, degree + padding);
        auto end_horner = chrono::high_resolution_clock::now();

        auto duration_horner = chrono::duration_cast<chrono::nanoseconds>(
                                   end_horner - start_horner)
                                   .count();

        if (j >= trials - 15)
        {
            report(j, R_standard, duration_horner, R_intrinsic, duration_horner_intrinsic);
        }
    }

    _mm_free(coefficients);

    return 0;
}

void report(
    int trial,
    float R_standard,
    auto duration_horner,
    float R_intrinsic,
    auto duration_horner_intrinsic)
{
    cout << "trial: " << trial << endl;
    cout << "\tvectorised: " << R_intrinsic << " (" << duration_horner_intrinsic << " ns)" << endl;
    cout << "\tstandard: " << R_standard << " (" << duration_horner << " ns)" << endl;
}