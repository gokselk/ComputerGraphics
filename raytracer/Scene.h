//
// Created by goksel on 11/27/21.
//

#ifndef RAYTRACER_SCENE_H
#define RAYTRACER_SCENE_H

#include "Vec3i.h"
#include "Vec3f.h"
#include "Camera.h"
#include "PointLight.h"
#include "Material.h"
#include "Mesh.h"
#include "Triangle.h"
#include "Sphere.h"

typedef struct {
    unsigned int r, g, b;
} RGB;

class Scene{
public:
    Scene();
    explicit Scene(const parser::Scene &scene);

    int max_recursion_depth{};
    Vec3i background_color;
    float shadow_ray_epsilon{};
    std::vector<Camera> cameras;
    Vec3f ambient_light;
    std::vector<PointLight> point_lights;
    std::vector<Material> materials;
    std::vector<Vec3f> vertex_data;
    std::vector<Mesh> meshes;
    std::vector<Triangle> triangles;
    std::vector<Sphere> spheres;

    void render_multithreaded();

    void render_old();

    void render_region(const Camera &camera, unsigned char image[], int from, int to) const;
};

#endif //RAYTRACER_SCENE_H
