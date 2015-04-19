#include <stdlib.h>
#include <sys/types.h>
#include <regex.h>
#include <stdio.h>
#include "car.h"

#define BUFS 1024
#define PI 3.14159265

int ReadCar(char *file, struct Car *car)
{
  FILE *f;
  int i;
  char buf[BUFS], attr[BUFS], value[BUFS];

  if ((f = fopen(file, "r")) == NULL) {
    perror(file);
    return 0;
  }
  while(fgets(buf, BUFS, f)) {
    if (buf[0] != '#') {
      i = attr_value(attr, value, buf);
      if (i == 2) {
	if (strcmp(attr, "twidth") == 0) {
	  car->twidth = atof(value);

	} else if (strcmp(attr, "tprofile") == 0) {
	  car->tprofile = atof(value);

	} else if (strcmp(attr, "trim") == 0) {
	  car->trim = atof(value);

	} else if (strcmp(attr, "wheelbase") == 0) {
	  car->wheelbase = atof(value);

	} else if (strcmp(attr, "tf") == 0) {
	  car->tf = atof(value);

	} else if (strcmp(attr, "tr") == 0) {
	  car->tr = atof(value);

	} else if (strcmp(attr, "Zf") == 0) {
	  car->Zf = atof(value);

	} else if (strcmp(attr, "Zr") == 0) {
	  car->Zr = atof(value);

	} else if (strcmp(attr, "Kr1") == 0) {
	  car->Kr1 = atof(value);

	} else if (strcmp(attr, "Kr1Inst") == 0) {
	  car->Kr1Inst = atof(value);

	} else if (strcmp(attr, "Kof") == 0) {
	  car->Kof = atof(value);

	} else if (strcmp(attr, "Wt") == 0) {
	  car->Wt = atof(value);

	} else if (strcmp(attr, "Wf") == 0) {
	  car->Wf = atof(value);

	} else if (strcmp(attr, "Wr") == 0) {
	  car->Wr = atof(value);

	} else if (strcmp(attr, "h") == 0) {
	  car->h = atof(value);

	} else if (strcmp(attr, "g_max_lateral") == 0) {
	  car->g_max_lateral = atof(value);

	} else if (strcmp(attr, "g_max_brake") == 0) {
	  car->g_max_brake = atof(value);

	} else if (strcmp(attr, "g_max_accel") == 0) {
	  car->g_max_accel = atof(value);

	} else if (strcmp(attr, "power") == 0) {
	  car->power = atof(value);

	} else if (strcmp(attr, "maxrps") == 0) {
	  car->maxrps = atoi(value);

	} else if (strcmp(attr, "shift_time") == 0) {
	  car->shift_time = atof(value);

	} else if (strcmp(attr, "gears") == 0) {
	  /*	  car->gears = atof(value); */
	  char *p = value;
	  car->ngears = 0;
	  while(*p && isdigit(*p)) {
	    car->gears[car->ngears++] = atof(p);
	    while(*p && *p != ',') p++;
	    while(*p && (*p == ',' || isspace(*p))) p++;
	  }

	} else if (strcmp(attr, "endgear") == 0) {
	  car->endgear = atof(value);

	} else if (strcmp(attr, "circumference") == 0) {
	  car->circumference = atof(value);

	} else if (strcmp(attr, "front_area") == 0) {
	  car->front_area = atof(value);

	} else if (strcmp(attr, "area") == 0) {
	  car->area = atof(value);

	} else if (strcmp(attr, "Cl0") == 0) {
	  car->Cl0 = atof(value);

	} else if (strcmp(attr, "Cl") == 0) {
	  car->Cl = atof(value);
	
	} else if (strcmp(attr, "Cd0") == 0) {
	  car->Cd0 = atof(value);
      
	} else if (strcmp(attr, "Cr") == 0) {
	  car->Cr = atof(value);
	}
      }
    }
  }
  return 0;
}

int attr_value(char *attr, char *value, char *buf) {
  
  int i=0;
  
  while(*buf && (*buf == ' '|| *buf =='\t')) {
    if (*buf == '#') return 0;
    *buf++;
  }

  while(*buf) {
    if (*buf == '#') return 0;
    if(*buf != ' ' && *buf !='\t' && *buf != ':') {
      *attr = *buf;
      *attr++;
      i = 1;
    } else {
      *attr = '\0';
      *buf++;
      break;
    }
    *buf++;
  }
  *attr = '\0';
  while(*buf && (*buf == ' '|| *buf =='\t')) {
    if (*buf == '#') return 0;
    *buf++;
  }

  while(*buf) {
    if (*buf == '#') break;
    if(*buf !='\t' && *buf != ':' && *buf != '\n') {
      i = 2;
      *value = *buf;
      *value++;
    } else {
      break;
    }
    *buf++;
  }
  *value = '\0';
  return i;
}
  

int Curv2WheelLoads(struct Car *ch, struct Curve *cu) {
  double wheelcenter;
  double KorRad, KofRad;
  double H;			/* m CG över rollaxeln */
  double a, b;

  wheelcenter = (ch->twidth * ch->tprofile / 100) +
    (ch->trim * 0.0254 / 2);

  ch->Kor = (ch->tr*ch->tr * ch->Kr1) / 180;
  
  KorRad = ch->Kor * 180 / PI;
  KofRad = ch->Kof * 180 / PI;
  a = ch->Wr/ch->Wt * ch->wheelbase;
  b = ch->Wf/ch->Wt * ch->wheelbase;
  
  H = ch->h - ((ch->Zf-ch->Zr)*b/ch->wheelbase + ch->Zr);   /* m CG över rollaxel */

}

/* Return total propulsion power in wats given a Car and a speed v,
   also fills in resulting rps and gear */
double power(struct Car *car, double v, int *rps, int *gear)
{
  int g;

  for (g=0; g<car->ngears; g++) {
    *rps = (int) ((v/car->circumference) * car->gears[g] * car->endgear);

    if (*rps < car->maxrps) {
      *gear = g;

      return (*rps/(double)car->maxrps) * car->power - road_and_aero_drag(car, v);
    }
  }
  return 0;
}

/* Road load from Heywood p.49 */
double road_and_aero_drag(struct Car *car, double v) {
  return (car->Cr * car->Wt * 9.81 + 0.5 * 1.22 *
			  car->CdTotal * car->front_area * v * v) * v;
}

/* Given a car and a speed v returns downforce in N */
double downforce(struct Car *car, double v) {

    return  v * v * 1.23 * 0.5 * (- car->Cl) * car->area;

}
