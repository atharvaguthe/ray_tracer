

#ifndef VEC3_H
#define VEC3_H

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

  Vec3 operator*(const Vec3& v) const {
    return{
      x * v.x,
      y * v.y,
      z * v.z
    };
  }
};

inline Vec3 reflect(const Vec3& v, const Vec3& n) {
  return v - n * (2.0 * v.dot(n));
}


#endif
