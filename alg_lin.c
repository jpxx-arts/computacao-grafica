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

float dot(Vect3D v1, Vect3D v2) {
    return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
}

void cross(Vect3D v1, Vect3D v2, Vect3D *res) {
    res->x = v1.y*v2.z - v2.y*v1.z;
    res->y = v1.x*v2.z - v2.x*v1.z;
    res->z = v1.x*v2.y - v2.x*v1.y;
}

float normalize(Vect3D v) {
    return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

float intersection(Ray ray, Object obj) {
    switch (obj.obj_type) {
        case SPHERE: {
            // Solve |O + t*D - C|^2 = r^2  =>  A*t^2 + B*t + C = 0
            // where oc = O - C (vector from sphere center to ray origin)
            Vect3D oc;
            sub_vet(ray.look_from, obj.position, &oc);

            // Probably A will be 1 once direction is normalized.
            float A = dot(ray.look_at, ray.look_at);
            float B = 2.0f * dot(ray.look_at, oc);
            float C = dot(oc, oc) - obj.radius * obj.radius;

            float delta = B*B - 4.0f*A*C;

            if (delta < 0.0f) return -1.0f;

            float t1 = (-B + sqrtf(delta)) / (2.0f * A);
            float t2 = (-B - sqrtf(delta)) / (2.0f * A);

            // t2 <= t1; prefer the smaller positive value (nearest hit in front of ray)
            if (t2 > 0.0f) return t2;
            if (t1 > 0.0f) return t1;
            return -1.0f;
        }
        case PLAN:
            return -1.0f;
    }
    return -1.0f;
}

// Returns the index of the closest intersected object, or -1 if none.
int closest_intersection(Ray ray, Scene scene) {
    float closest_t = INFINITY;
    int closest_idx = -1;

    for (int i = 0; i < scene.obj_qtd; i++) {
        float t = intersection(ray, scene.objs[i]);
        if (t > 0.0f && t < closest_t) {
            closest_t = t;
            closest_idx = i;
        }
    }
    return closest_idx;
}
