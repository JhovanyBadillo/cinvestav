#include <stdint.h>
#include <immintrin.h>

void hamming_distance_256(
    const uint64_t *Q, /* Arreglo de 4 enteros de 64 bits (256 bits) */
    const uint64_t *B, /* Arreglo contiguo de N * 4 enteros de 64 bits */
    size_t N,          /* Cantidad de vectores N */
    uint16_t *D        /* Arreglo de salida de tamaño N */
)
{
    const uint64_t q0 = Q[0];
    const uint64_t q1 = Q[1];
    const uint64_t q2 = Q[2];
    const uint64_t q3 = Q[3];

    for (size_t i = 0; i < N; ++i)
    {
        const size_t offset = i * 4;

        uint64_t d0 = q0 ^ B[offset + 0];
        uint64_t d1 = q1 ^ B[offset + 1];
        uint64_t d2 = q2 ^ B[offset + 2];
        uint64_t d3 = q3 ^ B[offset + 3];

        /* Conteo de bits activos con intrinsic popcnt */
        uint64_t c0 = (uint64_t)_mm_popcnt_u64(d0);
        uint64_t c1 = (uint64_t)_mm_popcnt_u64(d1);
        uint64_t c2 = (uint64_t)_mm_popcnt_u64(d2);
        uint64_t c3 = (uint64_t)_mm_popcnt_u64(d3);

        D[i] = (uint16_t)(c0 + c1 + c2 + c3);
    }
}