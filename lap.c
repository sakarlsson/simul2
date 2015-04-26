#include <stdlib.h>
#include <sys/types.h>
#include <regex.h>
#include <stdio.h>
#include <math.h>
#include "simul2.h"
#include "circuit.h"
#include "car.h"
#include "lap.h"


struct Lap *RunOneLap(struct Circuit *circuit, struct Car *car)
{
  int i, gear, keepgear, prevg, rps;
  struct Lap *lap;
  double prev_v, p, shifttimer;
  double e2, e1, v;
  double power, drag, force;
		
  
  lap = malloc(sizeof(struct Lap));
  lap->segments = circuit->segments;
  lap->vmax_radius = malloc(sizeof(double)*lap->segments);
  lap->downforce = malloc(sizeof(double)*lap->segments);
  lap->aeromod = malloc(sizeof(double)*lap->segments);
  lap->vmax_brake = malloc(sizeof(double)*lap->segments);
  lap->vmax_power = malloc(sizeof(double)*lap->segments);
  lap->vmax_combined = malloc(sizeof(double)*lap->segments);
  lap->deltatime = malloc(sizeof(double)*lap->segments);
  lap->rpsdata = malloc(sizeof(double)*lap->segments);
  lap->geardata = malloc(sizeof(double)*lap->segments);
  lap->drag = malloc(sizeof(double)*lap->segments);
  lap->power = malloc(sizeof(double)*lap->segments);
  lap->force = malloc(sizeof(double)*lap->segments);
  
  
  /* Go through each segment of the circuit and calculate the maximum speed
     we can go with.  If its a straight line max is VBIG. If it has
     a radius we calculate the cars basic maximimum cornering and
     then recalculate for extra speed with the downforce */

  /* This could be improved in several ways, e.g could the calculation 
     be iterative since added speed for downforce gives even more
     downforce and even more speed etc.etc */

  /* Also this asumes we are going at g_max latteral all every part
     of the corner, which probably is a oversimplification */

  for (i=0; i<lap->segments; i++) {
    if (circuit->radius[i] != 0) {
      lap->vmax_radius[i] = sqrt(ABS(circuit->radius[i]) * car->g_max_lateral);
      lap->downforce[i] = downforce(car, lap->vmax_radius[i]);
      lap->aeromod[i] = 1 + lap->downforce[i] / (car->Wt * GRAVITATION);
      lap->vmax_radius[i] = sqrt(abs(circuit->radius[i]) *
				 car->g_max_lateral * lap->aeromod[i]);
    } else {
      lap->vmax_radius[i] = VBIG;
    }
  }

  /* Go backwards through the circuit and make sure that the difference 
     in speed between the current segment and the segment that the car
     comes from is at most what the brakes can handle. */

  prev_v = lap->vmax_radius[0];
  for (i = lap->segments - 1; i >= 0; i--){
      drag = road_and_aero_drag(car, prev_v);
      /* Using energy calculation e = m*v^2/2 */
      e2 = car->Wt * (prev_v * prev_v)/2;

      /* Using energy calculation e = f * l (and f = m * a) */
      e1 = car->Wt * car->g_max_brake * circuit->dl + e2 + drag;

      v = sqrt(e1 * 2 / car->Wt);
      if (lap->vmax_radius[i] > v) {
	  lap->vmax_brake[i] = v;
      } else {
	  lap->vmax_brake[i] = lap->vmax_radius[i];
      }
      prev_v = lap->vmax_brake[i];
  }

  /* Go forward through the circuit and accelerate as much as possible 
     compared to the speed in the segment before.  This means we are
     accelerating out of corners. In and into corners we just reset the 
     current speed to what the corner allows. */
  
  prev_v = lap->vmax_brake[lap->segments - 1];
  
  shifttimer = 0;
  gear = car->ngears - 1;
  for (i= 0; i < lap->segments; i++) {
	prevg = gear;
	lap->power[i] = power = wheelpower(car, prev_v, &rps, &gear, &force);;
	lap->force[i] = force;
	drag = road_and_aero_drag(car, prev_v);

	e1 = car->Wt * (prev_v * prev_v) / 2;
	e2 = e1 + ((power-drag)/prev_v) * circuit->dl;
	v = sqrt(e2 * 2 / car->Wt);

	if (lap->vmax_radius[i] > v) {
	    lap->vmax_power[i] = v;
	} else {
	    lap->vmax_power[i] = lap->vmax_radius[i];
	}
	prev_v = lap->vmax_power[i];
	lap->rpsdata[i] = rps;
	lap->geardata[i] = gear;
    }

  /* This is the final step where we combine the speed if we would be
     accelerating out of the corners all the time with the braking
     constrained speed we need not to go over if we should go into 
     corners without exeeding maximum corner speed */
     
  for (i = 0; i < lap->segments; i++) {
    v = lap->vmax_brake[i];
    if (lap->vmax_power[i] < v) {
      v = lap->vmax_power[i];
    }
    lap->vmax_combined[i] = v;
    lap->drag[i] = road_and_aero_drag(car, v);
    lap->deltatime[i] = circuit->dl/v;
  }
  return lap;
}


