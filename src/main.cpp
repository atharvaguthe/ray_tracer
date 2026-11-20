

#include <iostream>
#include <fstream>
#include <cmath>
#include <algorithm>
#include <memory>

#include "ray.h"
#include "sphere.h"
#include "scene.h"


Vec3 ray_color(const Ray& ray, const Scene& scene,int depth) {

  if (depth <= 0)
    return {0,0,0};

  HitRecord record;

  if (!scene.hit(ray, 0.001, INFINITY, record)) {

    double a = 0.5 * (ray.direction.y + 1.0);

    Vec3 white = {1.0, 1.0, 1.0};
    Vec3 blue = {0.5, 0.7, 1.0};

    return white * (1.0 - a) + blue * a;
  }
  Vec3 light_position = {-2.0, 4.0, -1.0};

  Vec3 to_light = light_position - record.point;

  double light_distance = to_light.length();

  Vec3 light_direction = to_light.normalized();

  double diffuse = std::max(
      0.0,
      record.normal.dot(light_direction)
      );

  Ray shadow_ray;

  shadow_ray.origin = record.point + record.normal * 0.001;

  shadow_ray.direction = light_direction;

  HitRecord shadow_record;

  bool in_shadow = scene.hit(
      shadow_ray,
      0.001,
      light_distance - 0.001,
      shadow_record
      );

  if (in_shadow) {
    diffuse = 0.0;
  }
  double ambient = 0.12;
  double intensity = ambient + (1.0 - ambient) * diffuse;

  Vec3 local_color = record.mat_ptr -> albedo() * intensity;
  double reflection = record.mat_ptr -> reflectivity();

  if (reflection <= 0.0)
    return local_color;

  Vec3 reflected_direction = reflect(ray.direction, record.normal);

  Ray reflected_ray;

  reflected_ray.origin = record.point + record.normal * 0.001;

  reflected_ray.direction = reflected_direction.normalized();

  Vec3 reflected_color = ray_color(reflected_ray,scene,depth - 1);

  return local_color * (1.0 - reflection) + reflected_color * reflection;

}

int main() {
  const int width = 400;
  const int height = 225;

  std::ofstream image("output/first_render.ppm");
  image << "P3\n" << width << " " << height << "\n255\n";

  Scene world;
  
  world.add(std::make_shared<Sphere>(
        Vec3{0,0,-1},0.5, std::make_shared<Lambertian> (Vec3{0.8,0.1,0.1})
        ));

  world.add(std::make_shared<Sphere>(
        Vec3{1,0,-1.5},0.5, std::make_shared<Lambertian> (Vec3{0.1,0.2,0.8})
        ));

  world.add(std::make_shared<Sphere>(
        Vec3{-1,0,-1.5},0.5,std::make_shared<Metal> (Vec3{0.8,0.8,0.8},0.85)
        ));

  world.add(std::make_shared<Sphere>(
        Vec3{0,-100.5,-1},100.0,std::make_shared<Lambertian> (Vec3{0.7,0.7,0.7})
        ));

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

      Vec3 color = ray_color(ray,world,10);

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
