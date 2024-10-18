#include <stdio.h>
#include <stdlib.h>
#define SIG_LENGHT 320
extern double InputSignal_f32_1kHz_15kHz[SIG_LENGHT];

double calc_signal_mean(double *sig_src_arr, int sign_lenght) {
  double _mean = 0.0;
  for (int i = 0; i < sign_lenght; i++) {
    _mean = _mean + sig_src_arr[i];
  }
  _mean = _mean / (double)sign_lenght;
  return _mean;
}

int main(int argc, char *argv[]) {

  double mean = calc_signal_mean(InputSignal_f32_1kHz_15kHz, SIG_LENGHT);
  printf("the mean is %f", mean);
  return EXIT_SUCCESS;
}
