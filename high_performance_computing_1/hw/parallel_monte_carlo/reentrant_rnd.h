#define IM1 2147483563
#define IM2 2147483399
#define AM (1.0 / IM1)
#define IMM1 (IM1 - 1)
#define IA1 40014
#define IA2 40692
#define IQ1 53668
#define IQ2 52774
#define IR1 12211
#define IR2 3791
#define NTAB 32
#define NDIV (1 + IMM1 / NTAB)
#define EPS 1.2e-7
#define RNMX (1.0 - EPS)

struct cryptic_data
{
  long idum;
  long idum2;
  long iy;
  long iv[32];
};

typedef struct cryptic_data cryptic_data_t;

float ran2_r(cryptic_data_t *r)
{
  int j;
  long k;

  float temp;

  if (r->idum <= 0)
  {
    if (-(r->idum) < 1)
    {
      r->idum = 1;
    }
    else
    {
      r->idum = -(r->idum);
    }

    r->idum2 = r->idum;

    for (j = NTAB + 7; j >= 0; j--)
    {
      k = (r->idum) / IQ1;
      r->idum = IA1 * (r->idum - k * IQ1) - k * IR1;

      if (r->idum < 0)
      {
        r->idum += IM1;
      }

      if (j < NTAB)
      {
        (r->iv)[j] = r->idum;
      }
    }

    r->iy = (r->iv)[0];
  }

  k = (r->idum) / IQ1;
  r->idum = IA1 * (r->idum - k * IQ1) - k * IR1;

  if (r->idum < 0)
  {
    r->idum += IM1;
  }

  k = r->idum2 / IQ2;
  r->idum2 = IA2 * (r->idum2 - k * IQ2) - k * IR2;

  if (r->idum2 < 0)
  {
    r->idum2 += IM2;
  }

  j = (r->iy) / NDIV;
  r->iy = (r->iv)[j] - r->idum2;
  (r->iv)[j] = r->idum;

  if (r->iy < 1)
  {
    r->iy += IMM1;
  }

  if ((temp = AM * r->iy) > RNMX)
  {
    return RNMX;
  }
  else
  {
    return temp;
  }
}

#undef IM1
#undef IM2
#undef AM
#undef IMM1
#undef IA1
#undef IA2
#undef IQ1
#undef IQ2
#undef IR1
#undef IR2
#undef NTAB
#undef NDIV
#undef EPS
#undef RNMX
