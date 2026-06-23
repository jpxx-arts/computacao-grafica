#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "cg.h"

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
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

void cross(Vect3D v1, Vect3D v2, Vect3D *res) {
    res->x = v1.y * v2.z - v2.y * v1.z;
    res->y = v1.z * v2.x - v2.z * v1.x;
    res->z = v1.x * v2.y - v2.y * v1.x;
}

int normalize(Vect3D *v) {
    float norm = sqrtf(v->x * v->x + v->y * v->y + v->z * v->z);
    if (norm == 0.0f) return 0;
    v->x /= norm;
    v->y /= norm;
    v->z /= norm;
    return 1;
}

float intersection(Ray ray, Object obj) {
    if (obj.obj_type == SPHERE) {
        Vect3D oc;
        sub_vet(ray.orig, obj.position, &oc);

        float A = dot(ray.dest, ray.dest);
        float B = 2.0f * dot(ray.dest, oc);
        float C = dot(oc, oc) - obj.radius * obj.radius;

        float delta = B * B - 4.0f * A * C;
        if (delta < 0.0f) return -1.0f;

        float t1 = (-B - sqrtf(delta)) / (2.0f * A);
        float t2 = (-B + sqrtf(delta)) / (2.0f * A);

        if (t1 > 0.0f) return t1;
        if (t2 > 0.0f) return t2;
    }
    return -1.0f;
}

int closest_intersection(Ray ray, Object *objs, int obj_qtd, float near, float far, float *out_t) {
    float closest_t = INFINITY;
    int closest_idx = -1;

    for (int i = 0; i < obj_qtd; i++) {
        float t = intersection(ray, objs[i]);
        if (t > near && t < far && t < closest_t) {
            closest_t = t;
            closest_idx = i;
        }
    }
    *out_t = closest_t;
    return closest_idx;
}

void render_scene(float *pixel_buffer, int width, int height, Object sphere, Light light, RGB ambient_light, Camera cam) {
    Vect3D w, u, v;
    sub_vet(cam.look_from, cam.look_at, &w);
    normalize(&w); // Vetor w aponta na direção contrária ao olhar (sistema destro)

    cross(cam.v_up, w, &u);
    normalize(&u);

    cross(w, u, &v);

    float theta = cam.fov * M_PI / 180.0f;
    float half_height = tanf(theta / 2.0f);
    float half_width = cam.aspect_ratio * half_height;

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {

            // Mapeamento para NDC (Normalized Device Coordinates) [-1, 1]
            // O eixo Y é invertido (height - 1 - y) para coincidir com a escrita top-down do formato PPM
            float px = ((2.0f * ((float)x + 0.5f) / (float)width) - 1.0f) * half_width;
            float py = (1.0f - (2.0f * ((float)y + 0.5f) / (float)height)) * half_height;

            Vect3D dir, dir_u, dir_v, dir_w;
            dir_u = u; scalar_mult(px, &dir_u);
            dir_v = v; scalar_mult(py, &dir_v);
            dir_w = w; scalar_mult(-1.0f, &dir_w);

            sum_vet(dir_u, dir_v, &dir);
            sum_vet(dir, dir_w, &dir);
            normalize(&dir);

            Ray ray = { cam.look_from, dir };

            float t = -1.0f;
            Object scene_objs[1] = { sphere };
            int hit_idx = closest_intersection(ray, scene_objs, 1, cam.near_plane, cam.far_plane, &t);

            // Buffer flat para armazenar matriz 3D (largura x altura x 3 canais)
            int buffer_idx = (y * width + x) * 3;

            if (hit_idx != -1) {
                Vect3D hit_point = ray.dest;
                scalar_mult(t, &hit_point);
                sum_vet(ray.orig, hit_point, &hit_point); // P = O + t*D

                Vect3D N;
                sub_vet(hit_point, sphere.position, &N);
                normalize(&N);

                Vect3D L;
                sub_vet(light.pos, hit_point, &L);
                normalize(&L);

                Vect3D V;
                sub_vet(cam.look_from, hit_point, &V);
                normalize(&V);

                float dot_nl = dot(N, L);
                if (dot_nl < 0.0f) dot_nl = 0.0f; // Ignora contribuição de luz em faces traseiras

                // R = 2 * (N . L) * N - L
                Vect3D R = N;
                scalar_mult(2.0f * dot_nl, &R);
                sub_vet(R, L, &R);
                normalize(&R);

                float dot_rv = dot(R, V);
                if (dot_rv < 0.0f) dot_rv = 0.0f;

                float specular_factor = powf(dot_rv, sphere.material.ns);

                pixel_buffer[buffer_idx]     = (ambient_light.r * sphere.material.ka.r) +
                                               (light.color.r * (sphere.material.kd.r * dot_nl + sphere.material.ks.r * specular_factor));

                pixel_buffer[buffer_idx + 1] = (ambient_light.g * sphere.material.ka.g) +
                                               (light.color.g * (sphere.material.kd.g * dot_nl + sphere.material.ks.g * specular_factor));

                pixel_buffer[buffer_idx + 2] = (ambient_light.b * sphere.material.ka.b) +
                                               (light.color.b * (sphere.material.kd.b * dot_nl + sphere.material.ks.b * specular_factor));

                for (int c = 0; c < 3; c++) {
                    if (pixel_buffer[buffer_idx + c] > 1.0f) pixel_buffer[buffer_idx + c] = 1.0f;
                    if (pixel_buffer[buffer_idx + c] < 0.0f) pixel_buffer[buffer_idx + c] = 0.0f;
                }
            } else {
                // Plano de fundo de céu
                float t_bg = (float)y / (float)(height - 1);
                pixel_buffer[buffer_idx]     = (1.0f - t_bg) * 0.5f + t_bg * 1.0f;
                pixel_buffer[buffer_idx + 1] = (1.0f - t_bg) * 0.7f + t_bg * 1.0f;
                pixel_buffer[buffer_idx + 2] = (1.0f - t_bg) * 1.0f + t_bg * 1.0f;
            }
        }
    }
}

int save_ppm(const char *filename, float *pixel_buffer, int width, int height) {
    FILE *fp = fopen(filename, "wb");
    if (!fp) return 0;

    // Header P3 define o formato de cores RGB em texto ASCII
    fprintf(fp, "P3\n%d %d\n255\n", width, height);

    for (int i = 0; i < width * height * 3; i += 3) {
        int ir = (int)(255.999f * pixel_buffer[i]);
        int ig = (int)(255.999f * pixel_buffer[i+1]);
        int ib = (int)(255.999f * pixel_buffer[i+2]);

        fprintf(fp, "%d %d %d\n", ir, ig, ib);
    }

    fclose(fp);
    return 1;
}
