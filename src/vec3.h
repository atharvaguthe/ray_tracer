

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

  Vec3 operator-() const {
    return{-x,-y,-z};
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

inline Vec3 refract(const Vec3& uv, const Vec3& n, double eta_ratio) {
  double cos_theta = std::min((-uv).dot(n),1.0);

  Vec3 perpendicular = (uv + n * cos_theta) * eta_ratio;

  Vec3 parallel = n * (-std::sqrt(std::abs(1.0 - perpendicular.dot(perpendicular))));

  return perpendicular + parallel;
}

inline double schlick(double cosine, double refraction_ratio) {

  double r0 = (1.0 - refraction_ratio) / (1.0 + refraction_ratio);

  r0 = r0 * r0;

  return r0 + (1.0 - r0) * std::pow(1.0 - cosine, 5);
}

#endif
