#include <stdio.h>
#include <time.h>
#include <omp.h>

#include "reentrant_rnd.h"

#define NE 1000000000

int main()
{
  size_t i;
  size_t EE = 0;

  double t0, t1;

  t0 = omp_get_wtime();

#pragma omp parallel
  {
    cryptic_data_t r;
    r.idum = -(time(NULL) + omp_get_thread_num());
    r.idum2 = 123456789;
    r.iy = 0;

    float x, y;

#pragma omp for reduction(+ : EE)
    for (i = 0; i < NE; i++)
    {
      x = ran2_r(&r);
      y = ran2_r(&r);

      // printf("x = %f, y = %f\n", x, y);

      if ((x * x + y * y) <= 1.0)
      {
        EE++;
      }
    }
  }
  t1 = omp_get_wtime();

  printf("pi = %f\n", 4.0 * (float)((float)EE / NE));
  printf("EE = %zu\n", EE);
  printf("NE = %d\n", NE);
  printf("dt = %lf\n", t1 - t0);

  return 0;
}
