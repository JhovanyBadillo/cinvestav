#include <stdio.h>
#include <immintrin.h>

union b128 {
    __m128i _128;
    unsigned long long _64[2];
};

union b256 {
    __m256i _256;
    __m128i _128;
    unsigned long long _64[4];
};

// warmup: product of two unsigned 64-bit integers
void mul64() {
    unsigned long long a, b;
    unsigned long long hi;
    unsigned long long dst;

    a = 18446744073709551615UL;
    b = 18446744073709551614UL;

    dst = _mulx_u64(a, b, &hi);

    printf("a: %llx\n", a);
    printf("b: %llx\n", b);

    printf("hi: %llx\n", hi);
    printf("lo: %llx\n", dst);
}

void mul128() {
    union b128 A;
    A._64[1] = 0xFFFFFFFFFFFFFFFFUL; // AH
    A._64[0] = 0xFFFFFFFFFFFF1A0EUL; // AL

    union b128 B;
    B._64[1] = 0xFFFFFFFFFFFFFFFFUL; // BH
    B._64[0] = 0xFFFFFFFFFFFFFFFFUL; // BL

    union b256 R;
    union b128 ALBL, AHBL, ALBH, AHBH;

    ALBL._64[0] = R._64[0] = _mulx_u64(A._64[0], B._64[0], &ALBL._64[1]);
    AHBL._64[0] = _mulx_u64(A._64[1], B._64[0], &AHBL._64[1]);
    ALBH._64[0] = _mulx_u64(A._64[0], B._64[1], &ALBH._64[1]);
    AHBH._64[0] = _mulx_u64(A._64[1], B._64[1], &AHBH._64[1]);

    unsigned char c1, c2, c3, c;
    unsigned long long out;

    c1 = _addcarryx_u64(0, ALBL._64[1], AHBL._64[0], &out); // (AL*BL)H + (AH*BL)L. c1 in {0, 1}
    c2 = _addcarryx_u64(0, out, ALBH._64[0], &R._64[1]); // R[1] = (AL*BL)H + (AH*BL)L + (AL*BH)L. c2 in {0, 1}
    c = c1 + c2;
    c1 = _addcarryx_u64(0, c, AHBL._64[1], &out); // S1 = acarreo de columna 1 + (AH*BL)H, c1 in {0, 1}
    c2 = _addcarryx_u64(0, out, ALBH._64[1], &out); // S2 = S1 + (AL*BH)H, c2 in {0, 1}
    c3 = _addcarryx_u64(0, out, AHBH._64[0], &R._64[2]); // R[2] = S2 + (AH*BH)L, c3 in {0, 1}
    c = c1 + c2 + c3; // acarreo hacia la columna 3. c in {0, 1, 2, 3}
    _addcarryx_u64(0, c, AHBH._64[1], &R._64[3]); // R[3] = acarreo de columna 2 + (AH*BH)H

    printf("R[3}R[2]R[1]R[0]: %llx %llx %llx %llx\n", R._64[3], R._64[2], R._64[1], R._64[0]);
}

int main(void) {
    mul128();

    return 0;
}
