

#include <iostream>
#include <fstream>
#include <cmath>
#include <algorithm>

struct Vec3 {
  double x, y, z;

  Vec3 operator+(const Vec3& v) const {
    return {x + v.x, y + v.y, z + v.z};
  }

  Vec3 operator-(const Vec3& v) const {
    return {x - v.x, y - v.y, z - v.z};
  }

  Vec3 operator*(double t) const {
    return {x * t, y * t, z * t};
  }

  double dot(const Vec3& v) const {
    return x * v.x + y * v.y + z * v.z;
  }

  double length() const {
    return std::sqrt(dot(*this));
  }

  Vec3 normalized() const {
    return *this * (1.0/length());
  }
};

struct Ray {
  Vec3 origin;
  Vec3 direction;

  Vec3 at (double t) const {
    return origin + direction * t;
  }
};

bool hit_sphere(const Vec3& center, double radius, const Ray& ray, double& t) {
  Vec3 oc = ray.origin - center;

  double a = ray.direction.dot(ray.direction);
  double half_b = oc.dot(ray.direction);
  double c = oc.dot(oc) - radius * radius;

  double discriminant = half_b * half_b - a * c;

  if (discriminant < 0)
    return false;
  double sqrt_d = std::sqrt(discriminant);
  double root = (-half_b - sqrt_d) / a;
  if (root <= 0)
    root = (-half_b + sqrt_d) / a;
  if(root <= 0) 
    return false;
  t = root;
  return true;
}

Vec3 ray_color(const Ray& ray) {
  double t;

  Vec3 sphere_center = {0, 0, -1};

  if (hit_sphere(sphere_center, 0.5, ray, t)) {
    Vec3 point = ray.at(t);
    Vec3 normal = (point - sphere_center).normalized();

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
