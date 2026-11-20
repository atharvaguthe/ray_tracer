

#ifndef HIT_RECORD_H
#define HIT_RECORD_H

#include "ray.h"
#include "material.h"
#include <memory>

struct HitRecord {
  Vec3 point;
  Vec3 normal;
  Vec3 albedo;
  double t;
  bool front_face;

  std::shared_ptr<Material> mat_ptr;

  void set_face_normal(
      const Ray& ray, const Vec3& outward_normal
      ) {
        front_face = ray.direction.dot(outward_normal) < 0;

        normal = front_face ? outward_normal : outward_normal * -1.0;
        }
};

#endif
