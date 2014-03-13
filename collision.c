// collisions.c
// runs a collision of two particles, when detected

// adapted from http://www.plasmaphysics.org.uk/programs/coll3d_cpp.htm

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "main.h"

void run_collision(particle p1, particle p2, double m1, double m2, double r1, double r2, double x1, double y1, double z1, double x2, double y2, double z2, double vx1, double vy1, double vz1, double vx2, double vy2, double vz2) {
  
  double r12, m21, d, v, theta2, phi2, st, ct, sp, cp, vx1r, vy1r, vz1r, fvz1r, thetav, phiv, dr, alpha, beta, sbeta, cbeta, a, dvz2, vx2r, vy2r, vz2r, x21, y21, z21, vx21, vy21, vz21;

  //     **** initialize some variables ****
  r12 = r1+r2;
  m21 = m2/m1;
  x21 = x2-x1;
  y21 = y2-y1;
  z21 = z2-z1;
  vx21 = vx2-vx1;
  vy21 = vy2-vy1;
  vz21 = vz2-vz1;

  //     **** calculate relative distance and relative speed ***
  d = sqrt(x21*x21 + y21*y21 + z21*z21);
  v = sqrt(vx21*vx21 +vy21*vy21 +vz21*vz21);

  //     **** shift coordinate system so that ball 1 is at the origin ***
  x2 = x21;
  y2 = y21;
  z2 = z21;
  
  //     **** boost coordinate system so that ball 2 is resting ***
  vx1 = -vx21;
  vy1 = -vy21;
  vz1 = -vz21;
  
  //     **** find the polar coordinates of the location of ball 2 ***
  theta2 = acos(z2/d);
  if (x2 == 0 && y2 == 0) {phi2 = 0;} else {phi2 = atan2(y2,x2);}
  st = sin(theta2);
  ct = cos(theta2);
  sp = sin(phi2);
  cp = cos(phi2);

  //     **** express the velocity vector of ball 1 in a rotated coordinate
  //          system where ball 2 lies on the z-axis ******
  vx1r = ct*cp*vx1+ct*sp*vy1-st*vz1;
  vy1r = cp*vy1-sp*vx1;
  vz1r = st*cp*vx1+st*sp*vy1+ct*vz1;
  fvz1r = vz1r/v ;

  if (fvz1r>1) {fvz1r=1;}   // fix for possible rounding errors
  else if (fvz1r<-1) {fvz1r=-1;} 
  thetav = acos(fvz1r);
  if (vx1r==0 && vy1r==0) {phiv=0;} else {phiv=atan2(vy1r,vx1r);}

  //     **** calculate the normalized impact parameter ***
  dr = d*sin(thetav)/r12;

  //     **** calculate impact angles if balls do collide ***
  alpha = asin(-dr);
  beta = phiv;
  sbeta = sin(beta);
  cbeta = cos(beta);

  //     **** update positions and reverse the coordinate shift ***
  x2 += x1;
  y2 += y1;
  z2 += z1;
  
  //  ***  update velocities ***
  
  a = tan(thetav+alpha);
  
  dvz2 = 2*(vz1r+a*(cbeta*vx1r+sbeta*vy1r))/((1+a*a)*(1+m21));
  
  vz2r = dvz2;
  vx2r = a*cbeta*dvz2;
  vy2r = a*sbeta*dvz2;
  vz1r = vz1r-m21*vz2r;
  vx1r = vx1r-m21*vx2r;
  vy1r = vy1r-m21*vy2r;
  
  
  //     **** rotate the velocity vectors back and add the initial velocity
  //           vector of ball 2 to retrieve the original coordinate system ****
  
  vx1 = ct*cp*vx1r-sp*vy1r+st*cp*vz1r +vx2;
  vy1 = ct*sp*vx1r+cp*vy1r+st*sp*vz1r +vy2;
  vz1 = ct*vz1r-st*vx1r               +vz2;
  vx2 = ct*cp*vx2r-sp*vy2r+st*cp*vz2r +vx2;
  vy2 = ct*sp*vx2r+cp*vy2r+st*sp*vz2r +vy2;
  vz2 = ct*vz2r-st*vx2r               +vz2;

  p1->vel.x = vx1;
  p1->vel.y = vy1;
  p1->vel.z = vz1;
  p2->vel.x = vx2;
  p2->vel.y = vy2;
  p2->vel.z = vz2;

  return;
}
