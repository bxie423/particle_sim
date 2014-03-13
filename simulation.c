// simulation.c
// given a parameters list and a particle list, runs a physics simulation
// and outputs all of the coordinates to stdout

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "main.h"

#define OUTPUTNAME "OUTPUTCOORDS"
#define MICROSECOND 1000000

#define GRAVCONST 6.67384E-11
#define COULCONST 8.98755E9

// note: we are intentially going to avoid collisions of more than 2 particles at once
void detect_collisions(plist lst) {
  int pnum = lst->pnum;
  int collided[pnum];
  for (int i = 0; i < pnum; i++) {
    collided[i] = 0;
  }
  for (int i = 0; i < pnum; i++) {
    for (int j = i + 1; j < pnum; j++) {
      if (!collided[i] && !collided[j]) {
	if (distance(lst->lst[i]->pos, lst->lst[j]->pos) < lst->lst[i]->radius + lst->lst[j]->radius) {
	  particle p1 = lst->lst[i];
	  particle p2 = lst->lst[j];
	  collided[i] = 1;
	  collided[j] = 1;
	  run_collision(p1, p2, p1->mass, p2->mass, p1->radius, p2->radius, p1->pos.x, p1->pos.y, p1->pos.z, p2->pos.x, p2->pos.y, p2->pos.z, p1->vel.x, p1->vel.y, p1->vel.z, p2->vel.x, p2->vel.y, p2->vel.z);
	}
      }
    }
  }
}

double distance(point p, point q) {
  double retval = sqrt(pow(p.x - q.x, 2) + pow(p.y - q.y, 2) + pow(p.z - q.z, 2));
  return retval;
}

void external_forces(params par, point *netacc, particle p) {
  double x = par->grav->strengths.x + (par->e_glob->strengths.x + p->vel.y * par->b_glob->strengths.z - p->vel.z * par->b_glob->strengths.y) * p->charge / p->mass;
  double y = par->grav->strengths.y + (par->e_glob->strengths.y + p->vel.z * par->b_glob->strengths.x - p->vel.x * par->b_glob->strengths.z) * p->charge / p->mass;
  double z = par->grav->strengths.z + (par->e_glob->strengths.z + p->vel.x * par->b_glob->strengths.y - p->vel.y * par->b_glob->strengths.x) * p->charge / p->mass;

  int elnum = par->elnum;
  for (int i = 0; i < elnum; i++) {
    locfield field = par->e_loc[i];
    if ((field->c1.x < p->pos.x) && (field->c2.x > p->pos.x) && (field->c1.y < p->pos.y) && (field->c2.y > p->pos.y) && (field->c1.z < p->pos.z) && (field->c2.z > p->pos.z)) {
      switch (field->direction) {
      case 0:
        x += field->value * field->sign * p->charge / p->mass;
        break;
      case 1:
        y += field->value * field->sign * p->charge / p->mass;
        break;
      case 2:
        z += field->value * field->sign * p->charge / p->mass;
        break;
      }
    }
  }

  int blnum = par->blnum;
  for (int i = 0; i < blnum; i++) {
    locfield field = par->b_loc[i];
    if ((field->c1.x < p->pos.x) && (field->c2.x > p->pos.x) && (field->c1.y < p->pos.y) && (field->c2.y > p->pos.y) && (field->c1.z < p->pos.z) && (field->c2.z > p->pos.z)) {
      switch (field->direction) {
      case 0:
        y += field->value * field->sign * p->charge * p->vel.z / p->mass;
	z -= field->value * field->sign * p->charge * p->vel.y / p->mass;
        break;
      case 1:
        z += field->value * field->sign * p->charge * p->vel.x / p->mass;
	x -= field->value * field->sign * p->charge * p->vel.z / p->mass;
        break;
      case 2:
        x += field->value * field->sign * p->charge * p->vel.y / p->mass;
	y -= field->value * field->sign * p->charge * p->vel.x / p->mass;
        break;
      }
    }
  }

  netacc->x += x;
  netacc->y += y;
  netacc->z += z;
}

void particle_forces(plist lst, point *netacc, int pidx) {
  double x = 0;
  double y = 0;
  double z = 0;
  particle p = lst->lst[pidx];
  int pnum = lst->pnum;

  for (int i = 0; i < pnum; i++) {
    if (i != pidx) {
      particle p2 = lst->lst[i];
      point dirs = sign(p, p2);
      double dist = distance(p->pos, p2->pos);

      // we avoid taking the fourth power of distance to make sure everything fits
      // into a standard double.
      x += -1 * dirs.x * GRAVCONST * p2->mass / pow(dist, 2) * pow(p2->pos.x - p->pos.x, 2) / pow(dist, 2);
      y += -1 * dirs.y * GRAVCONST * p2->mass / pow(dist, 2) * pow(p2->pos.y - p->pos.y, 2) / pow(dist, 2);
      z += -1 * dirs.z * GRAVCONST * p2->mass / pow(dist, 2) * pow(p2->pos.z - p->pos.z, 2) / pow(dist, 2);

      if ((p->charge != 0) && (p2->charge != 0)) {
	x += dirs.x * COULCONST * p->charge * p2->charge / p->mass / pow(dist, 2) * pow(p2->pos.x - p->pos.x, 2) / pow(dist, 2);
	y += dirs.y * COULCONST * p->charge * p2->charge / p->mass / pow(dist, 2) * pow(p2->pos.y - p->pos.y, 2) / pow(dist, 2);
	z += dirs.z * COULCONST * p->charge * p2->charge / p->mass / pow(dist, 2) * pow(p2->pos.z - p->pos.z, 2) / pow(dist, 2);
      }
    }
  }

  netacc->x += x;
  netacc->y += y;
  netacc->z += z;
}

// this assumes that a negative force is attractive, which is reversed for gravity
point sign(particle a, particle b) {
  double x = a->pos.x < b->pos.x ? -1 : 1;
  double y = a->pos.y < b->pos.y ? -1 : 1;
  double z = a->pos.z < b->pos.z ? -1 : 1;
  return new_point(x, y, z);
}

void update_particle(point *netacc, plist lst, int partidx) {
  particle p = lst->lst[partidx];
  p->pos.x += p->vel.x * timestep / MICROSECOND;
  p->pos.y += p->vel.y * timestep / MICROSECOND;
  p->pos.z += p->vel.z * timestep / MICROSECOND;
  p->vel.x += netacc->x * timestep / MICROSECOND;
  p->vel.y += netacc->y * timestep / MICROSECOND;
  p->vel.z += netacc->z * timestep / MICROSECOND;
}

void printstate(plist lst, int step, FILE *out) {
  int pnum = lst->pnum;
  for (int i = 0; i < pnum; i++) {
    point p = lst->lst[i]->pos;
    fprintf(out, "%d\t%d\t%.20f\t%.20f\t%.20f\n", step, i, p.x, p.y, p.z);
  }
}

void printpct(int pct) {
  printf("\r%d%%\t[", pct);
  int i = 0;
  for (; i < pct/2; i++) {
    putchar('#');
  }
  for (; i < 50; i++) {
    putchar('-');
  }
  putchar(']');
  fflush(stdout);
}

void runsim(params par, plist lst) {
  int runtime = simtime * MICROSECOND / timestep; // index will increment by one
  int writestep = floor(writefreq / timestep);
  int iostep = floor(runtime / 100);
  FILE *output;
  output = fopen(OUTPUTNAME, "w");

  fprintf(output, "NUMPARTICLES: %d\n", lst->pnum);
  printf("Running particle simulation...\n0%%\t[--------------------------------------------------]");

  for (int i = 0; i < runtime; i++) {
    detect_collisions(lst);
    int pnum = lst->pnum;
    for (int j = 0; j < pnum; j++) {
      point netacc = new_point(0, 0, 0);
      external_forces(par, &netacc, lst->lst[j]);
      particle_forces(lst, &netacc, j);
      update_particle(&netacc, lst, j);
    }
    if (!(i % writestep)) {
      printstate(lst, i, output);
    }
    if (!(i % iostep)) {
      printpct(i / iostep);
    }
  }

  printf("\r100%%\t[##################################################]\n");
  fclose(output);
}
