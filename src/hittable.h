

#ifndef HITTABLE_H
#define HITTABLE_H

#include "hit_record.h"

class Hittable {
public:
  virtual ~Hittable() = default;

  virtual bool hit(
      const Ray& ray,
      double t_min,
      double t_max,
      HitRecord& record
      ) const = 0;
};

#endif
