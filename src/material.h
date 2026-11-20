
#ifndef MATERIAL_H
#define MATERIAL_H

#include "vec3.h"

class Material {
public:

  virtual ~Material() = default;
  virtual Vec3 albedo() const = 0;
  virtual double reflectivity() const = 0;
  virtual bool is_dielectric() const {
    return false;
  }
  virtual double refractive_index() const {
    return 0.0;
  }
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

class Dielectric : public Material {
  private: 
    double index;
    
  public:
    explicit Dielectric(double index)
      : index(index){}

    Vec3 albedo() const override {
      return {1.0,1.0,1.0};
    }
    
    double reflectivity() const override {
      return 0.0;
    }

    bool is_dielectric() const override {
      return true;
    }

    double refractive_index() const override {
      return index;
    }
};

#endif
