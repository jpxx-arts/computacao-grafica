#include <stdio.h>
#include <stdlib.h>
#include "cg.h"

int main() {
    int width = 800;
    int height = 600;
    const char *output_filename = "esfera_phong.ppm";

    Object sphere;
    sphere.obj_type = SPHERE;
    sphere.position = (Vect3D){ 0.0f, 0.0f, -5.0f };
    sphere.radius = 1.5f;

    sphere.material.ka = (RGB){ 0.1f, 0.0f, 0.0f };
    sphere.material.kd = (RGB){ 0.7f, 0.1f, 0.1f };
    sphere.material.ks = (RGB){ 1.0f, 1.0f, 1.0f };
    sphere.material.ns = 32.0f;

    Light light;
    light.pos = (Vect3D){ 5.0f, 5.0f, -2.0f };
    light.color = (RGB){ 1.0f, 1.0f, 1.0f };

    RGB ambient_light = (RGB){ 1.0f, 1.0f, 1.0f };

    Camera cam;
    cam.look_from    = (Vect3D){ 0.0f, 0.0f, 0.0f };
    cam.look_at      = (Vect3D){ 0.0f, 0.0f, -1.0f };
    cam.v_up         = (Vect3D){ 0.0f, 1.0f, 0.0f };
    cam.aspect_ratio = (float)width / (float)height;
    cam.fov          = 60.0f;
    cam.near_plane   = 0.1f;
    cam.far_plane    = 100.0f;

    float *pixel_buffer = (float *)calloc(width * height * 3, sizeof(float));
    if (!pixel_buffer) return -1;

    render_scene(pixel_buffer, width, height, sphere, light, ambient_light, cam);

    save_ppm(output_filename, pixel_buffer, width, height);

    free(pixel_buffer);
    return 0;
}
