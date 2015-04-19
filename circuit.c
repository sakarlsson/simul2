#include <stdlib.h>
#include <sys/types.h>
#include <regex.h>
#include <stdio.h>
#include "simul2.h"
#include "circuit.h"

#define BUFS 1024
#define PI 3.14159265

int ReadCircuit(char *file, struct Circuit *circuit)
{
  FILE *f;
  int circpos = 0, i, firstline = 1;
  char buf[BUFS], attr[BUFS], value[BUFS], *p;
  double radius, length, correction;

  if ((f = fopen(file, "r")) == NULL) {
    perror(file);
    return 0;
  }

  /* Sum track lenght */
  
  circuit->segments = 0;
  while(fgets(buf, BUFS, f)) {
    if (buf[0] != '#') {
      if (firstline) {
	correction = atof(buf);
	firstline = 0;
      } else {
	parse_one_segment_line(&radius, &length, buf);
	circuit->segments += length / SIM_STEP_LENGTH;
      }
    }
  }
  circuit->radius = malloc(sizeof(double)*circuit->segments);

  /* OK, do it again and fill in data into circuit  */;
  rewind(f);
  firstline = 1;
  while(fgets(buf, BUFS, f)) {
    if (buf[0] != '#') {
      if (firstline) {
	correction = atof(buf);
	firstline = 0;
      } else {
	parse_one_segment_line(&radius, &length, buf);
	for (i = 0; i<(length / SIM_STEP_LENGTH); i++) {
	  circuit->radius[circpos++] = radius;
	  /* printf("%d ", (int)radius); */
	}
      }
    }
  }
  fclose(f);
  circuit->dl = SIM_STEP_LENGTH;

  /* printf("segm %d\n", circuit->segments); */
  return 0;
}

int parse_one_segment_line(double *radius, double *length, char *buf)
{
  int i;
  char *p;
  
  i = strspn(buf, "0123456789-");
  p = buf+i;
  *p = '\0';
  *radius = atof(buf);
  p++;
  i = strspn(p, " \t");
  p = p + i;
  *length = atof(p);
  /* printf("%f %f\n", *radius, *length); */
  return 0;
}

