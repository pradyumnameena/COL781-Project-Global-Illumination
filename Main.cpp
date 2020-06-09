#include "Scene.h"
#include "Math.h"
#include "Sphere.h"
#include "Ray.h"
#include <iostream>

using namespace std;

int main(int argc, const char *argv[])
{
    int w = 1024, h = 768;
    int samples = 1;
    if (argc == 2)
    {
        samples = atoi(argv[1]) / 4;
    }

    Ray camera(vec3d(50, 52, 295.6), vec3d(0, -0.042612, -1).normalize());
    vec3d v = vec3d(0, -0.042612, -1).normalize();
    vec3d cx = vec3d(w * 0.5135 / h), cy = (cx.cross(camera.direction)).normalize() * 0.5135;
    vec3d r, *c = new vec3d[w * h];

    Sphere sp[] = {
        //Scene: radius, position, emission, color, material
        Sphere(1e5, vec3d(1e5 + 1, 40.8, 81.6), vec3d(), vec3d(.75, .25, .25), DIFFUSE),   //Left
        Sphere(1e5, vec3d(-1e5 + 99, 40.8, 81.6), vec3d(), vec3d(.25, .25, .75), DIFFUSE), //Rght
        Sphere(1e5, vec3d(50, 40.8, 1e5), vec3d(), vec3d(.75, .75, .75), DIFFUSE),         //Back
        Sphere(1e5, vec3d(50, 40.8, -1e5 + 170), vec3d(), vec3d(), DIFFUSE),               //Frnt
        Sphere(1e5, vec3d(50, 1e5, 81.6), vec3d(), vec3d(.75, .75, .75), DIFFUSE),         //Botm
        Sphere(1e5, vec3d(50, -1e5 + 81.6, 81.6), vec3d(), vec3d(.75, .75, .75), DIFFUSE), //Top
        Sphere(16.5, vec3d(27, 16.5, 47), vec3d(), vec3d(1, 1, 1) * .999, SPECULAR),       //Mirr
        Sphere(16.5, vec3d(73, 16.5, 78), vec3d(), vec3d(1, 1, 1) * .999, REFRACTION),     //Glas
        Sphere(600, vec3d(50, 681.6 - .27, 81.6), vec3d(12, 12, 12), vec3d(), DIFFUSE)     //Lite
    };
    // cout << "D Scene" << endl;
    Scene mySceen(sp, 9);

    // cout << "scene created" << endl;
#pragma omp parallel for schedule(dynamic, 1) private(r)
    for (int y = 0; y < h; y++)
    {
        fprintf(stderr, "\rRendering (%d spp) %5.2f%%", samples * 4, 100. * y / (h - 1));
        for (unsigned short x = 0, Xi[3] = {0, 0, y * y * y}; x < w; x++)
        {
            //Subpixel 2 x 2
            for (int sy = 0, i = (h - y - 1) * w + x; sy < 2; sy++)
            {
                for (int sx = 0; sx < 2; sx++)
                {
                    r = vec3d();
                    for (int s = 0; s < samples; s++)
                    {
                        double r1 = 2 * erand48(Xi);
                        double dx = r1 < 1 ? sqrt(r1) - 1 : 1 - sqrt(2 - r1);
                        double r2 = 2 * erand48(Xi);
                        double dy = r2 < 1 ? sqrt(r2) - 1 : 1 - sqrt(2 - r2);

                        vec3d d = cx * (((sx + 0.5 + dx) / 2 + x) / w - 0.5) + cy * (((sy + 0.5 + dy) / 2 + y) / h - 0.5) + camera.direction;

                        r = r + mySceen.ray_tracer(Ray(camera.origin + d * 140, d.normalize()), 0, Xi) * (1.0 / samples);
                        // cout << "Ray tracer done" << endl;
                    }
                    c[i] = c[i] + vec3d(clamp(r.x), clamp(r.y), clamp(r.z)) * 0.25;
                }
            }
        }
    }
    fprintf(stderr, "\n");
    FILE *f = fopen("image.ppm", "w"); // Write image to PPM file.
    fprintf(f, "P3\n%d %d\n%d\n", w, h, 255);
    for (int i = 0; i < w * h; i++)
        fprintf(f, "%d %d %d ", toInt(c[i].x), toInt(c[i].y), toInt(c[i].z));
}