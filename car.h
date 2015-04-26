struct Car {
  /* Input data */

  /* Tire data */
  double	twidth;		/* Tire width m */
  double	tprofile;	/* Profile % */
  double	trim;		/* Rim diameter inch */
  /* Chassis Geometrics */
  double	wheelbase;	/* m */
  double	tf;		/* Trackwidth front m */
  double	tr;		/* Trackwidth rear m */
  double	Zf;		/* Roll center heigth front */
  double	Zr;		/* Roll center heigth rear */
  double	Kr1;		/* One spring rear N/m */
  double	Kr1Inst;	/* Installation ratio rear */

  double	Kof;		/* Roll rate front Nm/deg */

  double	Wt;		/* Weight total kg */
  double	Wf;		/* Weight front kg */
  double	Wr;		/* Weight rear kg */

  double	h;		/* CG over ground m */

  /* Simple car performance model */
  double	g_max_lateral;	/* Maximum cornering */
  double	g_max_brake;
  double	g_max_accel;

  /* Calculated data */
  double	Kor;		/* Roll rate rear Nm/deg */

  /* Engine */
  double	power;		/* Engine power in wats (linear 
				   interpolation = constant tourqe assumed) */
  int		maxrps;		/* Maximum r/s */

  /* Transmission */

  double	shift_time;	/* Time without power between shifts in s */
  int		ngears;		/* Number of gears */
  double	gears[32];	/* E.g 2.05, 1.75, 1.48, 1.00, 0.81  */
  double	endgear;	/* E.g 3.73 */
  double	circumference;   /* m, e.g 1.856, driving wheels circumference */

  /* Aerodynamics */

  double	front_area;	/* m^2 */
  double	area;		/* m^2 */

  double	Cl0;		/* Original body Lift  */
  double	Cl;		/* Lift with aero modifications */

  double	Cd0;		/* Origimnal body drag */

  /* Calculated Aerodynamics */
  double	CdTotal;	/* Calculated */

  /* Rolling resistance */
  double	Cr;		/* E.g 0.013 */

};

struct Curve {
  double	alfadeg;	/* Banking */
  double	radius;		/* in m */
  double	v;		/* Speed i curve */
};

double wheelpower(struct Car *car, double v, int *rps, int *gear, double *force);
double road_and_aero_drag(struct Car *car, double v);
double downforce(struct Car *car, double v);
