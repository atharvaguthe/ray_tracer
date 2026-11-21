

#pragma once 

#include <random>

inline double random_double() {
  static std::uniform_real_distribution<double> distribution(0.0, 1.0);
  static std::mt19937 generator(std::random_device{}());

  return distribution(generator);

}

inline Vec3 random_in_unit_disk() {
  while (true) {
    Vec3 p = {
      random_double() * 2.0 - 1.0,
      random_double() * 2.0 - 1.0,
      0.0
    };

    if (p.dot(p) < 1.0)
      return p;
    
  }
}
