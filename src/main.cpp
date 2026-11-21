

#include <iostream>
#include <fstream>
#include <cmath>
#include <algorithm>
#include <memory>

#include "ray.h"
#include "sphere.h"
#include "scene.h"
#include "random.h"


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

  // Dielectric material (glass)
  if (record.mat_ptr -> is_dielectric()) {
    double refraction_ratio = record.front_face ? (1.0 / record.mat_ptr -> refractive_index()) : record.mat_ptr -> refractive_index();

    double cos_theta = std::min((-ray.direction).dot(record.normal), 1.0);

    double sin_theta = std::sqrt(1.0 - cos_theta * cos_theta);

    bool cannot_refract = refraction_ratio * sin_theta > 1.0;

    double reflect_probability = cannot_refract ? 1.0 : schlick(cos_theta, refraction_ratio);

    Vec3 reflected_direction = reflect(ray.direction, record.normal);

    Ray reflected_ray;

    reflected_ray.origin = record.point + record.normal * 0.001;

    reflected_ray.direction = reflected_direction.normalized();

    Vec3 reflected_color = ray_color(reflected_ray, scene, depth - 1);

    Vec3 refracted_color = {0,0,0};

    if(!cannot_refract) {
      Vec3 refracted_direction = refract(ray.direction, record.normal,refraction_ratio);

      Ray refracted_ray;

      refracted_ray.origin = record.point - record.normal * 0.001;

      refracted_ray.direction = refracted_direction.normalized();

      refracted_color = ray_color(refracted_ray, scene, depth - 1);

    }
    return reflected_color * reflect_probability +
      refracted_color * (1.0 - reflect_probability);
  }


  // Shadow ray
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
        Vec3{1,0,-1.5},0.5, std::make_shared<Dielectric> (1.5)
        ));

  world.add(std::make_shared<Sphere>(
        Vec3{-1,0,-1.5},0.5,std::make_shared<Metal> (Vec3{0.8,0.8,0.8},0.85)
        ));

  world.add(std::make_shared<Sphere>(
        Vec3{0,-100.5,-1},100.0,std::make_shared<Lambertian> (Vec3{0.7,0.7,0.7})
        ));

  double viewport_height = 2.0;
  double viewport_width = viewport_height * double(width) / height;

  double focal_length = 1.0;
  double lens_radius = 0.0;

  Vec3 camera_origin = {0,0,0};

  Vec3 horizontal = {viewport_width,0,0};
  Vec3 vertical = {0,viewport_height,0};

  Vec3 lower_left_corner = camera_origin
    - horizontal * 0.5 - vertical * 0.5 - Vec3{0,0,focal_length};

  const int samples_per_pixel = 256;

  for(int j = height - 1; j >= 0; --j){
    for(int i = 0; i < width; ++i) {
      Vec3 pixel_color = {0,0,0};

      for (int s = 0; s < samples_per_pixel; ++s){
        double u = (i + random_double()) / (width - 1);
        double v = (j + random_double()) / (height - 1);

        Vec3 rd = random_in_unit_disk() * lens_radius;
        Vec3 offset = {rd.x,rd.y,0.0};

        Vec3 ray_origin = camera_origin * offset;
        Vec3 ray_direction = lower_left_corner + horizontal * u 
          + vertical * v - camera_origin - offset;
        Ray ray;
        ray.origin = ray_origin;
        ray.direction = ray_direction.normalized();

        pixel_color = pixel_color + ray_color(ray, world, 10);

      }
      pixel_color = pixel_color * (1.0 / samples_per_pixel);

      int r = int(255.999 * std::clamp(pixel_color.x,0.0,1.0));
      int g = int(255.999 * std::clamp(pixel_color.y,0.0,1.0));
      int b = int(255.999 * std::clamp(pixel_color.z,0.0,1.0));

      image << r << " " << g << " " << b << "\n";
    }
  }
  std::cerr << "\nRendering complete!\n";
  image.close();
  
  return 0;
}
