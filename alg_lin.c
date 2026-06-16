#include <math.h>
#include "alg_lin.h"

void sub_vet(Vect3D v1, Vect3D v2, Vect3D *res) {
    res->x = v1.x - v2.x;
    res->y = v1.y - v2.y;
    res->z = v1.z - v2.z;
}

void sum_vet(Vect3D v1, Vect3D v2, Vect3D *res) {
    res->x = v1.x + v2.x;
    res->y = v1.y + v2.y;
    res->z = v1.z + v2.z;
}

void scalar_mult(float scalar, Vect3D *res) {
    res->x *= scalar;
    res->y *= scalar;
    res->z *= scalar;
}

void scalar_mult_vet(Vect3D v1, Vect3D v2, Vect3D *res) {
    res->x = v1.x * v2.x;
    res->y = v1.y * v2.y;
    res->z = v1.z * v2.z;
}

void vet_mult_vet(Vect3D v1, Vect3D v2, Vect3D *res) {
    res->x = v1.y*v2.z - v2.y*v1.z;
    res->y = v1.x*v2.z - v2.x*v1.z;
    res->z = v1.x*v2.y - v2.x*v1.y;
}

float normalize(Vect3D v) {
    return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

int intersection(Ray ray, Object obj) {
    // I think I could make a macro to handle with multiple object types. Maybe in the future...
    switch (obj.obj_type) {
        case SPHERE:
            // Probably A will be 1. So I have to remove this once validated.
            float A = (ray.look_at.x*ray.look_at.x) + (ray.look_at.y*ray.look_at.y) + (ray.look_at.z*ray.look_at.z);
            float B = 2 * (ray.look_from.x*ray.look_at.x +ray.look_from.y*ray.look_at.y + ray.look_from.z*ray.look_at.z);
            float C = ray.look_from.x*ray.look_from.x + ray.look_from.y+ray.look_from.y + ray.look_from.z*ray.look_from.z;

            float delta = B*B - 4*C;

            if (delta < 0) {
                return -1;
            }

            float t1 = (-B + sqrt(delta))/2*A;
            float t2 = (-B - sqrt(delta))/2*A;

            break;
    }
}

int closest_intersection(Ray ray, Scene scene) {
    int closest_point = INFINITY;

    for (int i = 0; i < scene.obj_qtd; i++) {
        if (intersection(ray, scene.objs[i]) < closest_point) {

        }
    }
}