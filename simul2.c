#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "car.h"
#include "circuit.h"
#include "lap.h"
 



int feq(double f1, double f2)
{
  char s1[16], s2[16];
  sprintf(s1, "%0.2f", f1);
  sprintf(s2, "%0.2f", f2);
  return strcmp(s1,s2);
}

int main (int argc, char **argv)
{
  struct Car car;
  struct Circuit circuit;
  struct Lap *lap;
  double t;
  int i;
  char *carfile = NULL;
  char *trackfile = NULL;
  int c;

  opterr = 0;
  while ((c = getopt (argc, argv, "c:t:")) != -1) {
    switch (c)
      {
      case 't':
        trackfile = optarg;
        break;
      case 'c':
        carfile = optarg;
        break;
      default:
        abort ();
      }
  }

  ReadCar(carfile, &car);
  ReadCircuit(trackfile, &circuit);
  lap = RunOneLap(&circuit, &car);
  fprintf(stderr,"# T: s\n");  
  fprintf(stderr,"# C0: vmxrad, m/s, 1, 0, 0, 80, red\n");
  fprintf(stderr,"# C1: brake, m/s, 1, 0, 0, 80, blue\n");
  fprintf(stderr,"# C2: power, m/s, 1, 0, 0, 80, green\n");
  fprintf(stderr,"# C3: speed, m/s, 1, 0, 0, 80, white\n");
  fprintf(stderr,"# Data: -\n");
  
  fprintf(stderr,"lap->segments %d\n", lap->segments);
  
  t = 0;
  for (i=0; i<lap->segments; i++) {
    if (feq(lap->vmax_radius[i-1],lap->vmax_radius[i]) ||
	feq(lap->vmax_brake[i-1], lap->vmax_brake[i]) ||
	feq(lap->vmax_power[i-1], lap->vmax_power[i]) ||
	feq(lap->vmax_combined[i-1], lap->vmax_combined[i])) {

      printf("%.4f %.2f %.2f %.2f %.2f %.2f %.2f %.2f\n",
	     t,
	     lap->vmax_radius[i],
	     lap->vmax_brake[i],
	     lap->vmax_power[i],
	     lap->vmax_combined[i],
	     lap->rpsdata[i],
	     lap->geardata[i]+1,
	     lap->drag[i]
	     );
    }
    t += lap->deltatime[i];
  }
  exit(0);
}

