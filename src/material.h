
#ifndef MATERIAL_H
#define MATERIAL_H

#include "vec3.h"

class Material {
public:

  virtual ~Material() = default;
  virtual Vec3 albedo() const = 0;
  virtual double reflectivity() const = 0;
};

class Lambertian : public Material {
  private:
    Vec3 color;

  public:
    explicit Lambertian(const Vec3& color)
      : color(color){}

    
    Vec3 albedo() const override {
      return color;
    }

    double reflectivity() const override {
      return 0.0;
    }
};

class Metal : public Material {
  private:
    Vec3 color;
    double reflection;

  public:
    Metal(const Vec3& color, double reflection)
      : color(color), reflection(reflection) {}

    Vec3 albedo() const override {
      return color;
    }

    double reflectivity() const override {
      return reflection;
    }
};

#endif
