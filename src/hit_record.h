

#ifndef HIT_RECORD_H
#define HIT_RECORD_H

#include "ray.h"

struct HitRecord {
  Vec3 point;
  Vec3 normal;
  double t;
  bool front_face;

  void set_face_normal(
      const Ray& ray, const Vec3& outward_normal
      ) {
        front_face = ray.direction.dot(outward_normal) < 0;

        normal = front_face ? outward_normal : outward_normal * -1.0;
        }
};

#endif
