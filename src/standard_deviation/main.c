
// to compile, enter
// $ gcc -o main main.c waveforms.c  -lm
#include <math.h>
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

double calc_signal_variance(double *sig_src_arr, double sig_len) {
  double _variance = 0.0;
  double sig_mean = calc_signal_mean(sig_src_arr, sig_len);

  for (int i = 0; i < sig_len; i++) {
    _variance = _variance + pow((sig_src_arr[i] - sig_mean), 2);
  }
  _variance = _variance / (sig_len - 1);
  return _variance;
}

double calc_standard_deviation(double *sig_src_arr, double sig_len) {
  return sqrt(calc_signal_variance(sig_src_arr, sig_len));
}
int main(int argc, char *argv[]) {
  printf("the standard deviation is %f",
         calc_standard_deviation(InputSignal_f32_1kHz_15kHz, SIG_LENGHT));
  return EXIT_SUCCESS;
}
