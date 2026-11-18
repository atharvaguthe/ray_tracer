

#include <iostream>
#include <fstream>
#include <cmath>
#include <algorithm>

#include "ray.h"
#include "sphere.h"


Vec3 ray_color(const Ray& ray) {

  Sphere sphere({0,0,-1}, 0.5);

  HitRecord record;

  if (sphere.hit(ray, 0.001, INFINITY, record)) {

    Vec3 normal = record.normal;
    return{
      0.5 * (normal.x + 1.0),
      0.5 * (normal.y + 1.0),
      0.5 * (normal.z + 1.0)
    };
  }
  double a = 0.5 * (ray.direction.y + 1.0);

  Vec3 white = {1.0, 1.0, 1.0};
  Vec3 blue = {0.5, 0.7, 1.0};

  return white * (1.0 - a) + blue * a;
}

int main() {
  const int width = 400;
  const int height = 225;

  std::ofstream image("output/first_render.ppm");
  image << "P3\n" << width << " " << height << "\n225\n";

  for(int j = height - 1; j >= 0; --j){
    std::cerr << "\rScanlines remaining: " << j << " " << std::flush;

    for(int i = 0; i < width; ++i) {
      double u = double(i) / (width - 1);
      double v = double(j) / (height - 1);

      Ray ray;
      ray.origin = {0,0,0};
      ray.direction = Vec3{
        (2.0 * u - 1.0) * double(width) / height,
          2.0 * v - 1.0, -1.0
      }.normalized();

      Vec3 color = ray_color(ray);

      int r = int(255.999 * std::clamp(color.x,0.0,1.0));
      int g = int(255.999 * std::clamp(color.y,0.0,1.0));
      int b = int(255.999 * std::clamp(color.z,0.0,1.0));

      image << r << " " << g << " " << b << "\n";
    }
  }
  std::cerr << "\nRendering complete!\n";
  image.close();
  
  return 0;
}
