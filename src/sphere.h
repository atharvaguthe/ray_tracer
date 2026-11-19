

#ifndef SPHERE_H
#define SPHERE_H 

#include "hittable.h"
#include <cmath>


class Sphere : public Hittable {
public:
  Vec3 center;
  double radius;

  Sphere(const Vec3& center, double radius) : center(center), radius(radius) {}

  bool hit(const Ray& ray, double t_min, double t_max, HitRecord& record) const override {
    Vec3 oc = ray.origin - center;

    double a = ray.direction.dot(ray.direction);
    double half_b = oc.dot(ray.direction);
    double c = oc.dot(oc) - radius * radius;

    double discriminant = half_b * half_b - a * c;

    if(discriminant < 0)
      return false;

    double sqrt_d = std::sqrt(discriminant);
    double root = (-half_b - sqrt_d) / a;

    if (root < t_min || root > t_max) {
      root = (-half_b + sqrt_d) / a;

      if (root < t_min || root > t_max)
        return false;
    }

    record.t = root;
    record.point = ray.at(root);

    Vec3 outward_normal = (record.point - center) * (1.0 / radius);

    record.set_face_normal(ray,outward_normal);
    return true;
  }
};

#endif
