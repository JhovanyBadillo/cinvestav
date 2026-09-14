#include <stdio.h>
#include <gmp.h>

int is_coprime(mpz_t e, mpz_t phiN) {
  mpz_t gcd;
  mpz_init(gcd);
  mpz_gcd(gcd, e, phiN);

  const int comparison = mpz_cmp_ui(gcd, 1);
  mpz_clear(gcd);
  return comparison == 0 ? 1 : 0;
}


int main(void) {
  mpz_t p, q, some_number, N, phiN, e, d, m, c;

  mpz_inits(p, q, some_number, N, phiN, d, c, NULL);
  mpz_init_set_ui(e, 65537); // 65537 is prime
  mpz_init_set_ui(m, 7); // the message to be ciphered (plaintext)
  gmp_printf("Text to cipher: %Zd\n", m);

  mpz_ui_pow_ui(some_number, 2, 32); // some_number = 2^32
  mpz_nextprime(p, some_number); // we get some prime p, p > 2^32

  mpz_ui_pow_ui(some_number, 2, 64); // some_number = 2^64
  mpz_nextprime(q, some_number); // we get some prime q, q > 2^64

  mpz_mul(N, p, q); // N = p*q

  mpz_sub_ui(p, p, 1); // compute p - 1 and store it in p
  mpz_sub_ui(q, q, 1); // compute q - 1 and store it in q

  gmp_printf("p - 1: %Zd\n", p);
  gmp_printf("q - 1: %Zd\n", q);

  mpz_mul(phiN, p, q); // compute phi(N) = (p - 1) * (q - 1)

  if (!is_coprime(e, phiN)) {
    printf("gcd(e, phi(N)) != 1\n");

    return 1;
  }

  gmp_printf("e: %Zd\n", e);

  mpz_invert(d, e, phiN); // compute the secret key d

  gmp_printf("d: %Zd\n", d);

  mpz_powm(c, m, e, N); // cipher the plaintext m

  gmp_printf("ciphertext: %Zd\n", c);

  mpz_t deciphered;
  mpz_init(deciphered);
  mpz_powm(deciphered, c, d, N);

  gmp_printf("deciphered: %Zd\n", deciphered);

  mpz_clears(
    p, q, some_number, N, phiN, e, d, m, c, deciphered, NULL);

  return 0;
}
