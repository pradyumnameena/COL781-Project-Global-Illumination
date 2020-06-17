#include "./../include/Scene.h"
#include "./../include/Math.h"
#include "./../include/Sphere.h"
#include "./../include/Ray.h"
#include "./../include/Snowman.h"
#include <fstream>
#include <iostream>

using namespace std;

int num_spheres = 11;

Sphere sp[] = {
    //Walls
    Sphere(1e5, vec3d(50, -1e5 + 81.6, 81.6), vec3d(), vec3d(.75, .75, .75), DIFFUSE),
    Sphere(1e5, vec3d(50, 40.8, -1e5 + 170), vec3d(), vec3d(), DIFFUSE),               
    Sphere(1e5, vec3d(50, 1e5, 81.6), vec3d(), vec3d(.75, .75, .75), DIFFUSE),            
    Sphere(1e5, vec3d(-1e5 + 99, 40.8, 81.6), vec3d(), vec3d(.25, .25, .75), DIFFUSE), 
    Sphere(1e5, vec3d(50, 40.8, 1e5), vec3d(), vec3d(.75, .75, .75), DIFFUSE),
    Sphere(1e5, vec3d(1e5 + 1, 40.8, 81.6), vec3d(), vec3d(.75, .25, .25), DIFFUSE),          
    Sphere(600, vec3d(50, 681.6 - .27, 81.6), vec3d(12, 12, 12), vec3d(), DIFFUSE), 

    // Surrounding spheres
    // Starting from 3 o clock of snowman in clockwise order
    Sphere(5, vec3d(90, 36, 80), vec3d(), vec3d(1, 1, 1) * .999, REFRACTION),
    Sphere(5, vec3d(70, 24, 100), vec3d(), vec3d(1, 1, 1) * .999, REFRACTION),
    Sphere(5, vec3d(30, 24, 100), vec3d(), vec3d(1, 1, 1) * .999, DIFFUSE),
    Sphere(5, vec3d(10, 36, 80), vec3d(), vec3d(1, 1, 1) * .999, DIFFUSE),
};

Snowman s(10, vec3d(50, 46, 60), vec3d(), vec3d(1, 1, 1) * .999, SPECULAR,
          18, vec3d(50, 18, 60), vec3d(), vec3d(1, 1, 1) * .999, SPECULAR);


void printToFile(const char* file, int w, int h, vec3d *c){
    FILE *f = fopen(file, "w"); // Write image to PPM file.
    fprintf(f, "P3\n%d %d\n%d\n", w, h, 255);
    for (int i = 0; i < w * h; i++){
        fprintf(f, "%d %d %d ", toInt(c[i].x), toInt(c[i].y), toInt(c[i].z));
    }
}

void GImain(int argc, const char *argv[])
{
    int w = 1024, h = 768;
    int samples = 1;
    if (argc == 3)
    {
        samples = atoi(argv[2]) / 4;
    }

    vec3d r;
    Ray camera(vec3d(50, 52, 295.6), vec3d(0, -0.042612, -1).normalize());
    vec3d cx = vec3d(w * 0.5135 / h);
    vec3d cy = (cx.cross(camera.direction)).normalize() * 0.5135;
    vec3d *c = new vec3d[w * h];

    Scene myScene(sp, num_spheres, vec3d(50, 60, 85), vec3d(M_PI * 10000, M_PI * 10000, M_PI * 10000), 0);
    myScene.addSnowman(s);

    // cout << "scene created" << endl;
#pragma omp parallel for schedule(dynamic, 1) private(r)
    for (int y = 0; y < h; y++)
    {
        unsigned short Xi[3] = {0, y * y * y, 0};
        fprintf(stderr, "\rRendered (%d spp) %5.2f%%", samples * 4, 100. * y / (h - 1));
        for (unsigned short x = 0; x < w; x++)
        {
            //Subpixel 2 x 2
            for (int sy = 0; sy < 2; sy++)
            {
                int i = (h - y - 1) * w + x;
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

                        r = r + myScene.ray_tracer(Ray(camera.origin + d * 140, d.normalize()), 0, Xi) * (1.0 / samples);
                        // cout << "Ray tracer done" << endl;
                    }
                    c[i] = (vec3d(clamp(r.x), clamp(r.y), clamp(r.z)) * 0.25) + c[i];
                }
            }
        }
    }
    fprintf(stderr, "\n");
    string fname = "imageGI" + to_string(samples * 4) + "samples.ppm";
    printToFile(fname.c_str(), w, h, c);
}

void PMmain(int argc, const char *argv[])
{
    int w = 1024, h = 768;
    int samples = 1;
    int BKT = 1000;
    int estimate = 10;
    if (argc == 3)
    {
        samples = atoi(argv[2]) / BKT;
        if (samples == 0)
            samples = 1;
    }
    if (argc == 4)
    {
        samples = atoi(argv[2]) / BKT;
        if (samples == 0)
            samples = 1;
        estimate = atoi(argv[3]);
    }

    Scene myScene(sp, num_spheres, vec3d(50, 60, 85), vec3d(M_PI * 10000, M_PI * 10000, M_PI * 10000), estimate);
    myScene.addSnowman(s);

    for (int i = 0; i < samples; i++)
    {
        int m = BKT * i;
        vec3d f;
        Ray r;
        fprintf(stderr, "\rPhoton Pass %5.2f%%", 100. * (i + 1) / samples);
        for (int j = 0; j < BKT; j++)
        {
            myScene.generatePhotonRay(&r, &f, m + j);
            myScene.photon_tracer(r, 0, false, f, m + j + 1);
        }
    }
    fprintf(stderr, "\n#photons:%ld\nBuilding kd-tree\n", myScene.photons.size());
    if (!myScene.photons.empty())
        myScene.tree.build(&myScene.photons[0], myScene.photons.size());

    fprintf(stderr, "Kd-tree build complete\n");

    Ray camera(vec3d(50, 52, 295.6), vec3d(0, -0.042612, -1).normalize());
    vec3d cx = vec3d(w * 0.5135 / h), cy = (cx.cross(camera.direction)).normalize() * 0.5135;
    vec3d *c = new vec3d[w * h];
#pragma omp parallel for schedule(dynamic, 1)
    for (int y = 0; y < h; y++)
    {
        fprintf(stderr, "\rRendering (%d samples) %5.2f%%", samples * BKT, 100. * y / (h - 1));
        for (int x = 0; x < w; x++)
        {
            //Subpixel 2 x 2
            const double s = 1.0 / (BKT * samples * 2 * 2);
            vec3d r(0), dmy(0);
            for (int sy = 0; sy < 2; sy++)
            {
                for (int sx = 0; sx < 2; sx++)
                {
                    vec3d d = cx * ((x + sx * 0.5 + 0.25) / w - 0.5) + cy * (-(y + sy * 0.5 + 0.25) / h + 0.5) + camera.direction;
                    r = r + myScene.photon_tracer(Ray(camera.origin + d * 140, d.normalize()), 0, true, dmy, 0);
                }
                c[x + y * w] = r * s;
            }
        }
    }
    fprintf(stderr, "\n");
    string fname = "imagePM" + to_string(samples * BKT) + "samples" + to_string(estimate) + "photons.ppm";
    printToFile(fname.c_str(), w, h, c);
}

int main(int argc, const char *argv[])
{
    cout << argv[1] << endl;
    string flag = argv[1];
    if (flag == "-p")
    {
        PMmain(argc, argv);
    }
    else if (flag == "-g")
    {
        GImain(argc, argv);
    }
    else
    {
        std::cerr<< "Specify either -p [Samples] [Num Photon] (both optional) \n or -g [Num samples]" << std::endl;
    }
    return 0;
}