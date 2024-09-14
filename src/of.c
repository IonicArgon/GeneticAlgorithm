#include "functions.h"

double rastrigin(int num_vars, double* x) {
  double sum1 = 0.0, sum2 = 0.0;
  for (int i = 0; i < num_vars; ++i) {
    sum1 += x[i] * x[i];
    sum2 += cos(2.0 * M_PI * x[i]);
  }
  return -20.0 * exp(-0.2 * sqrt(sum1 / num_vars)) - exp(sum2 / num_vars) +
         20.0 + M_E;
}