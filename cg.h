#ifndef CG_H
#define CG_H

typedef struct vect_3d {
  float x;
  float y;
  float z;
} Vect3D;

typedef struct rgb {
  float r;
  float g;
  float b;
} RGB;

enum obj_type { SPHERE, PLAN };

typedef struct ray {
  Vect3D orig;
  Vect3D dest;
} Ray;

typedef struct material {
  RGB ka;
  RGB kd;
  RGB ks;
  float ns;
} Material;

typedef struct obj {
  Vect3D position;
  enum obj_type obj_type;
  float radius;
  Material material;
} Object;

typedef struct camera {
  Vect3D look_from;
  Vect3D look_at;
  Vect3D v_up;
  float aspect_ratio;
  float fov;
  float near_plane;
  float far_plane;
} Camera;

typedef struct light {
  Vect3D pos;
  RGB color;
} Light;

void sub_vet(Vect3D v1, Vect3D v2, Vect3D *res);
void sum_vet(Vect3D v1, Vect3D v2, Vect3D *res);
void scalar_mult(float scalar, Vect3D *res);
int normalize(Vect3D *v);
float dot(Vect3D v1, Vect3D v2);
void cross(Vect3D v1, Vect3D v2, Vect3D *res);

float intersection(Ray ray, Object obj);
int closest_intersection(Ray ray, Object *objs, int obj_qtd, float near, float far, float *out_t);

void render_scene(float *pixel_buffer, int width, int height, Object sphere, Light light, RGB ambient_light, Camera cam);
int save_ppm(const char *filename, float *pixel_buffer, int width, int height);

#endif
