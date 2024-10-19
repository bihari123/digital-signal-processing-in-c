
// to compile, enter
// $ gcc -o main main.c waveforms.c  -lm
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#define SIG_LENGHT 320
extern double InputSignal_f32_1kHz_15kHz[SIG_LENGHT];
extern double Impulse_response[29];

int main(int argc, char *argv[]) {

  // Opens a pipe to GNUplot using popen().
  FILE *gnuplotPipe = popen("gnuplot -persistent", "w");
  if (gnuplotPipe == NULL) {
    printf("Error opening pipe to GNUplot.\n");
    return 1;
  }

  // Sends commands to set up the plot (title, plot command).
  // set size for a window
  fprintf(gnuplotPipe, "set size 1,1 \n");

  fprintf(gnuplotPipe, "set multiplot layout 2,2 \n");

  // setting size for each quadrant
  fprintf(gnuplotPipe, "set size 0.5,0.5 \n");

  fprintf(gnuplotPipe, "set origin 0,0.5 \n");
  fprintf(gnuplotPipe, "set title 'Impulse function'\n");
  fprintf(gnuplotPipe, "plot  '-' with lines lc rgb \"black\" \n");

  for (int x = 0; x <= 29; x++) {
    fprintf(gnuplotPipe, "%d %f\n", x, Impulse_response[x]);
  }

  fprintf(gnuplotPipe, "e\n");

  fprintf(gnuplotPipe, "set origin 0.5,0.5 \n");
  fprintf(gnuplotPipe, "set title 'Input function'\n");
  fprintf(gnuplotPipe, "plot  '-' with lines lc rgb \"blue\" \n");

  for (int x = 0; x <= SIG_LENGHT; x++) {
    fprintf(gnuplotPipe, "%d %f\n", x, InputSignal_f32_1kHz_15kHz[x]);
  }
  fprintf(gnuplotPipe, "e\n");

  // Second plot (Cosine wave)
  fprintf(gnuplotPipe, "set origin 0.5,0");
  fprintf(gnuplotPipe, "set title 'Cos(x) Graph'\n");
  fprintf(gnuplotPipe, "plot '-' with lines title 'cos(x)'\n");
  for (double x = 0; x <= 2 * M_PI; x += 0.1) {
    fprintf(gnuplotPipe, "%f %f\n", x, cos(x));
  }
  fprintf(gnuplotPipe, "e\n");

  // end multiplot
  fprintf(gnuplotPipe, "unset multiplot\n");
  fflush(gnuplotPipe);
  pclose(gnuplotPipe);

  return EXIT_SUCCESS;
}
