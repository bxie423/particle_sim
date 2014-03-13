// parseinput.c
// parses input files and converts them into data structures

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "main.h"

#define MAXLINE 256

point new_point(double x, double y, double z) {
  point new_point;

  new_point.x = x;
  new_point.y = y;
  new_point.z = z;

  return new_point;
}

plist new_part_list(void) {
  plist result = (plist) malloc(sizeof(particle_list));
  assert (result != NULL);

  result->pnum = 0;
  result->buf_size = 1;
  result->lst = malloc(result->buf_size * sizeof(part));

  return result;
}

void add_particle(plist plist, particle p) {
  if (plist->buf_size == plist->pnum) {
    plist->buf_size *= 2;
    plist->lst = realloc(plist->lst, plist->buf_size * sizeof(part));
    assert (plist->lst != NULL);
  }

  (plist->lst)[plist->pnum] = p;
  plist->pnum += 1;
}

particle new_particle(double mass, double charge, double radius, point p1, point p2) {
  if (mass == 0) {
    fprintf(stderr, "ERROR: Particle has zero mass!\n");
    exit(1);
  }
  if (radius == 0) {
    fprintf(stderr, "ERROR: Particle has zero radius!\n");
    exit(1);
  }
  particle result = (particle) malloc(sizeof(part));
  assert (result != NULL);

  result->mass = mass;
  result->charge = charge;
  result->radius = radius;
  result->pos = p1;
  result->vel = p2;

  return result;
}

params new_params(void) {
  params result = (params) malloc(sizeof(parameters));
  assert (result != NULL);

  result->elnum = 0;
  result->blnum = 0;
  result->e_buf_size = 1;
  result->b_buf_size = 1;

  result->e_glob = NULL;
  result->b_glob = NULL;
  result->grav = NULL;
  result->e_loc = malloc(result->e_buf_size * sizeof(local_field));
  result->b_loc = malloc(result->b_buf_size * sizeof(local_field));

  return result;
}

locfield new_locfield(char type, int dir, int sign, double val, point c1, point c2) {
  locfield result = (locfield) malloc(sizeof(local_field));
  assert (result != NULL);

  result->type = type;
  result->direction = dir;
  result->sign = sign;
  result->value = val;
  result->c1 = c1;
  result->c2 = c2;

  return result;
}

void add_locfield(params par, locfield l) {
  switch (l->type) {
  case 'e':
    if (par->e_buf_size + 1 == par->elnum) {
      par->e_buf_size *= 2;
      par->e_loc = realloc(par->e_loc, par->e_buf_size * sizeof(local_field));
      assert (par->e_loc != NULL);
    }
    (par->e_loc)[par->elnum] = l;
    par->elnum += 1;
    break;
  case 'b':
    if (par->b_buf_size + 1 == par->blnum) {
      par->b_buf_size *= 2;
      par->b_loc = realloc(par->b_loc, par->b_buf_size * sizeof(local_field));
      assert (par->b_loc != NULL);
    }
    (par->b_loc)[par->blnum] = l;
    par->blnum += 1;
    break;
  }
}

globfield new_globfield(char type, point strengths) {
  globfield result = (globfield) malloc(sizeof(global_field));
  assert (result != NULL);

  result->type = type;
  result->strengths = strengths;

  return result;
}

void add_globfield(params par, globfield g) {
  switch (g->type) {
  case 'g':
    par->grav = g;
    break;
  case 'e':
    par->e_glob = g;
    break;
  case 'b':
    par->b_glob = g;
    break;
  }
}

// the most important functions

params parseconfig(char *fname) {
  params par = new_params();
  FILE *input;
  input = fopen(fname, "r");

  char buf[MAXLINE];
  double a, b, c;
  char *field_type = malloc(sizeof(char) * 9);

  // parse the first line
  fscanf(input, "%lf %d %d", &simtime, &timestep, &writefreq);

  // parse the global gravity line
  fscanf(input, "%s %lf %lf %lf", field_type, &a, &b, &c);
  add_globfield(par, new_globfield('g', new_point(a, b, c)));

  // parse the global e-field line
  fscanf(input, "%s %lf %lf %lf", field_type, &a, &b, &c);
  add_globfield(par, new_globfield('e', new_point(a, b, c)));

  // parse the global b-field line
  fscanf(input, "%s %lf %lf %lf", field_type, &a, &b, &c);
  add_globfield(par, new_globfield('b', new_point(a, b, c)));

  // parse all of the local lines
  char *dir = malloc(sizeof(char) * 5);
  char *sign = malloc(sizeof(char) * 4);
  double d, e, f, strength;

  // for some odd reason, we end up with an extra empty line here.
  fgets(buf, MAXLINE, input);

  while (fgets(buf, MAXLINE, input) != NULL ) {
    sscanf(buf, "%s %s %s %lf %lf %lf %lf %lf %lf %lf", field_type, dir, sign, &strength, &a, &b, &c, &d, &e, &f);
    if (buf[0] == '\0') {
      printf("empty string\n");
    }
    int dir_int = !strcmp(dir, "XDIR") ? 0 : (!strcmp(dir, "YDIR") ? 1 : 2);
    int sign_int = !strcmp(sign, "POS") ? 1 : -1;
    if (isalpha(field_type[0])) {
      add_locfield(par, new_locfield(tolower(field_type[0]), dir_int, sign_int, strength, new_point(a, b, c), new_point(d, e, f)));
    }
  }

  fclose(input);
  return par;
}

plist parseinit(char *fname) {
  plist parts = new_part_list();
  FILE *input;
  input = fopen(fname, "r");

  char buf[MAXLINE];
  double mass, charge, radius, a, b, c, d, e, f;

  while (fgets(buf, MAXLINE, input) != NULL) {
    sscanf(buf, "%lf %lf %lf %lf %lf %lf %lf %lf %lf", &mass, &charge, &radius, &a, &b, &c, &d, &e, &f);
    add_particle(parts, new_particle(mass, charge, radius, new_point(a, b, c), new_point(d, e, f)));
  }

  fclose(input);
  return parts;
}
