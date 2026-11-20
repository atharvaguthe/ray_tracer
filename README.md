# ray_tracer

A from-scratch ray tracer written in **C++20**.

The goal of this project is to understand the foundations of computer
graphics by implementing the rendering pipeline directly: vector
mathematics, ray generation, geometric intersections, scene management,
lighting, shadows, and recursive ray interactions.

> This project is actively being developed. Features marked as in
> progress are not yet considered complete.

## Features

### Implemented

-   Custom 3D vector type with arithmetic, dot product, length, and
    normalization
-   Ray representation and point evaluation along a ray
-   Ray--sphere intersection using the quadratic equation
-   Hit records containing intersection position, surface normal, and
    face orientation
-   Polymorphic geometry interface (`Hittable`)
-   Scene containing multiple objects
-   Closest-hit intersection queries
-   Point-light diffuse shading using Lambert's cosine law
-   Hard shadows using shadow rays
-   Ambient illumination
-   PPM image output

### In progress

-   Material abstraction
-   Lambertian and metallic materials
-   Recursive mirror reflections

### Planned

-   Refraction and transparent materials
-   Anti-aliasing and multi-sample rendering
-   More geometry types, including planes and triangles
-   Better camera controls
-   Physically based materials and path tracing
-   Acceleration structures such as a bounding volume hierarchy (BVH)
-   Multithreaded rendering

## Project structure

The source tree is being organized into small, focused components:

``` text
ray_tracer/
├── src/
│   ├── main.cpp
│   ├── vec3.h
│   ├── ray.h
│   ├── hit_record.h
│   ├── hittable.h
│   ├── sphere.h
│   ├── scene.h
│   └── material.h
├── output/             # Generated renders (not tracked by Git)
├── build/              # CMake build directory (not tracked by Git)
├── CMakeLists.txt
├── .gitignore
└── README.md
```

Some files may be added or reorganized as development continues.

## Requirements

-   C++20-compatible compiler (GCC or Clang)
-   CMake 3.20 or newer
-   Make or another CMake-supported build tool

On Arch Linux, install the build tools with:

``` bash
sudo pacman -S --needed gcc cmake make
```

## Build

Clone the repository:

``` bash
git clone git@github.com:atharvaguthe/ray_tracer.git
cd ray_tracer
```

Configure and compile:

``` bash
cmake -B build -S .
cmake --build build -j"$(nproc)"
```

Run the renderer:

``` bash
./build/ray_tracer
```

The current renderer writes its image to:

``` text
output/first_render.ppm
```

Make sure the `output` directory exists before running the program:

``` bash
mkdir -p output
```

## Viewing renders

The output is a PPM image. On Arch Linux with a Wayland compositor,
`swayimg` can be used to view it:

``` bash
sudo pacman -S swayimg
swayimg output/first_render.ppm
```

Other image viewers that support PPM can also be used.

## Concepts explored

This project is intended as a learning exercise in the underlying
mathematics and engineering of a ray tracer.

-   **Rays:** A ray is represented by `P(t) = O + tD`, where `O` is its
    origin and `D` is its direction.
-   **Sphere intersection:** Substituting the ray equation into the
    sphere equation produces a quadratic equation. Its roots determine
    where the ray intersects the sphere.
-   **Surface normals:** The normal describes the orientation of a
    surface at an intersection.
-   **Closest-hit queries:** The scene tests objects while narrowing the
    maximum accepted ray distance to the nearest intersection found so
    far.
-   **Diffuse lighting:** Lambertian shading uses the clamped dot
    product between the surface normal and the direction towards the
    light.
-   **Shadow rays:** A secondary ray checks whether another object
    blocks the path from a surface point to the light.
-   **Reflections:** The ideal reflection direction is calculated using
    `R = V - 2(V · N)N`.

## Development status

The project is being built incrementally. The immediate focus is
completing and testing the material system and recursive reflections
before moving on to refraction, anti-aliasing, and path tracing.

## License

No license has been added yet. Until a license is specified, all rights
are reserved by the author.
