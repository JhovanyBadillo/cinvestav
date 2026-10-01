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
    double R_standard,
    auto duration_horner,
    double R_intrinsic,
    auto duration_horner_intrinsic);

double horner(double h, double *a, long degree)
{
    double Y = 0.0;
    int i;

    for (i = 0; i < degree; i++)
    {
        Y = (Y + a[i]) * h;
    }

    return Y;
}

double horner_intrinsic(double h, double *a, long degree)
{
    // h is the evaluation point
    // a points to the coefficients as doubles
    double *R, P;
    int i;
    __m256d *A, X, Y;

    // A points to the coefficients as vectors of 4 doubles
    A = (__m256d *)a;

    X = _mm256_set1_pd(h * h * h * h);
    Y = _mm256_set1_pd(0.0);

    for (i = 0; i < degree / 4 - 1; i++)
    {
        Y = _mm256_add_pd(Y, A[i]);
        Y = _mm256_mul_pd(Y, X);
    }

    Y = _mm256_add_pd(Y, A[i]); // i = size / 4 - 1
    R = (double *)(&Y);

    P = R[3] * h;
    P += R[2] * h * h;
    P += R[1] * h * h * h;
    P += R[0] * h * h * h * h;

    return P;
}

int main()
{
    double R_intrinsic, R_standard, *coefficients, h = 1.1;
    int i, j;
    long trials = 1000;

    /* when the degree n of the polynomial is not divisible by 4,
    we need to complete with 4*ceil(n/4) - n zeros the array of
    coefficients.
    */
    long degree = 10000;
    int padding = 4 * ceil(degree / 4.0) - degree;
    /* padding in {0, 1, 2, 3}. So this adds, at most, 3 multiplications by zero */

    cout << "degree: " << degree << endl;
    cout << "how much padding: " << padding << endl;

    coefficients = (double *)_mm_malloc((degree + padding) * sizeof(double), 32);

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
            coefficients[i] = (double)distribution(gen);
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
    double R_standard,
    auto duration_horner,
    double R_intrinsic,
    auto duration_horner_intrinsic)
{
    cout << "trial: " << trial << endl;
    cout << "\tvectorised: " << R_intrinsic << " (" << duration_horner_intrinsic << " ns)" << endl;
    cout << "\tstandard: " << R_standard << " (" << duration_horner << " ns)" << endl;
}