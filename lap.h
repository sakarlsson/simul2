struct Lap {
  int segments;
  double *vmax_radius;
  double *downforce;
  double *aeromod;
  double *vmax_brake;
  double *vmax_power;
  double *vmax_combined;
  double *deltatime;
  double *rpsdata;
  double *geardata;
  double *drag;
};


struct Lap *RunOneLap(struct Circuit *circuit, struct Car *car);
