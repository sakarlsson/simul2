#include <stdio.h>
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

main()
{
  struct Car car;
  struct Circuit circuit;
  struct Lap *lap;
  double t;
  int i;

  ReadCar("data", &car);
  ReadCircuit("circuit", &circuit);
  lap = RunOneLap(&circuit, &car);
  printf("# T: s\n");  
  printf("# C0: vmxrad, m/s, 1, 0, 0, 80, red\n");
  printf("# C1: brake, m/s, 1, 0, 0, 80, blue\n");
  printf("# C2: power, m/s, 1, 0, 0, 80, green\n");
  printf("# C3: speed, m/s, 1, 0, 0, 80, white\n");
  printf("# Data: -\n");
  
  t = 0;
  for (i=0; i<lap->segments; i++) {
    if (feq(lap->vmax_radius[i-1],lap->vmax_radius[i]) ||
	feq(lap->vmax_brake[i-1], lap->vmax_brake[i]) ||
	feq(lap->vmax_power[i-1], lap->vmax_power[i]) ||
	feq(lap->vmax_combined[i-1], lap->vmax_combined[i])) {

      printf("%.4f %.2f %.2f %.2f %.2f\n",
	     t,
	     lap->vmax_radius[i],
	     lap->vmax_brake[i],
	     lap->vmax_power[i],
	     lap->vmax_combined[i]
	     );
    }
    t += lap->deltatime[i];
  }
}

