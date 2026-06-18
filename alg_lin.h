#ifndef ALG_LIN_H
#define ALG_LIN_H

#include <stdint.h>

typedef struct vect_3d {
  float x;
  float y;
  float z;
} Vect3D;

typedef struct color {
  float r;
  float g;
  float b;
  float a;
} Color;

enum obj_type { SPHERE, PLAN };

typedef struct ray {
  Vect3D look_from;
  Vect3D look_at;
} Ray;

typedef struct sphere {
  Vect3D center;
  float radius;
} Sphere;

typedef struct obj {
  Vect3D position;
  Color color;
  enum obj_type obj_type;
  float radius;
} Object;

typedef struct scene {
  Object *objs;
  int obj_qtd;
} Scene;

void sub_vet(Vect3D v1, Vect3D v2, Vect3D *res);
void sum_vet(Vect3D v1, Vect3D v2, Vect3D *res);
void scalar_mult(float scalar, Vect3D *res);
void scalar_mult_vet(Vect3D v1, Vect3D v2, Vect3D *res);
void vet_mult_vet(Vect3D v1, Vect3D v2, Vect3D *res);
float normalize(Vect3D v);
float dot(Vect3D v1, Vect3D v2);

int closest_intersection(Ray ray, Scene scene);
float intersection(Ray ray, Object obj);

#endif
