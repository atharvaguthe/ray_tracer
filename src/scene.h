

#ifndef SCENE_H
#define SCENE_H

#include "hittable.h"
#include <vector>
#include <memory>
#include <limits>

class Scene : public Hittable {
public:
  std::vector < std::shared_ptr<Hittable>> objects;

  void add(std::shared_ptr<Hittable> object) {
    objects.push_back(object);
  }

  void clear(){
    objects.clear();
  }

  bool hit(
      const Ray& ray, double t_min,
      double t_max, HitRecord& record
      ) const override {
    HitRecord temp_record;
    bool hit_anything = false;
    double closest_so_far = t_max;

    for (const auto& object : objects) {
      if (object -> hit(
            ray,t_min,closest_so_far,temp_record
            )) {
        hit_anything = true;
        closest_so_far = temp_record.t;
        record = temp_record;
      }
    }

    return hit_anything;
  }
};

#endif
