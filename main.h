// main.h

// type definitions

typedef struct coordinate { // anything involving 3 values will be a coordinate
  double x;
  double y;
  double z;
} point;

typedef struct global_field {
  char type;
  point strengths;
} global_field, *globfield;

typedef struct local_field {
  char type;
  int direction;
  int sign;
  double value;
  point c1;
  point c2;
} local_field, *locfield;

typedef struct parameters {
  globfield e_glob;
  globfield b_glob;
  globfield grav;
  locfield *e_loc;
  locfield *b_loc;
  int elnum;
  int blnum;
  int e_buf_size;
  int b_buf_size;
} parameters, *params;

typedef struct part {
  double mass;
  double charge;
  double radius;
  point pos;
  point vel;
} part, *particle;

typedef struct particle_list {
  particle *lst;
  int pnum;
  int buf_size;
} particle_list, *plist;

// external variables

double simtime;
int timestep;
int writefreq;

// defined in parseinput.c

point new_point(double, double, double);

plist new_part_list(void);
void add_particle(plist, particle);
particle new_particle(double, double, double, point, point);

params new_params(void);
locfield new_locfield(char, int, int, double, point, point);
void add_locfield(params, locfield);
globfield new_globfield(char, point);
void add_globfield(params, globfield);

params parseconfig(char *);
plist parseinit(char *);

// defined in simulation.c

void external_forces(params, point *, particle);
void particle_forces(plist, point *, int);
void update_particle(point *, plist, int);
void printstate(plist, int, FILE *);
void printpct(int);

void detect_collisions(plist);
double distance(point, point);

point sign(particle, particle);

void runsim(params, plist);

// defined in collision.c

void run_collision(particle, particle, double, double, double, double, double, double, double, double, double, double, double, double, double, double, double, double);
