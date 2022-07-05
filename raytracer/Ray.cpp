#include <limits>
#include "Ray.h"
#include "RayHit.h"
#include "Scene.h"
#include <iostream>

Ray::Ray(const Ray& ray){
    position = ray.position;
    direction = ray.direction;
}

Ray::Ray(const Vec3f &position, const Vec3f &direction) {
    this->position = position;
    this->direction = direction;
}

Ray& Ray::operator=(const Ray& ray){
    position = ray.position;
    direction = ray.direction;
    return *this;
}

Vec3f Ray::get_color(const Scene& scene, int depth) {
    Vec3f color{0, 0, 0};
    if (depth > scene.max_recursion_depth) {
        return color;
    }
    RayHit hit = get_first_hit(scene);
    if (!hit.material_id) {
        if(depth){
            return color;
        }
        return {
                static_cast<float>(scene.background_color.x),
                static_cast<float>(scene.background_color.y),
                static_cast<float>(scene.background_color.z)
        };
    }
    Material material = scene.materials[hit.material_id - 1];
    if(material.is_mirror){
        Ray reflected_ray = hit.get_reflected_ray(scene.shadow_ray_epsilon);
        color += reflected_ray.get_color(scene, depth + 1) * material.mirror;
    }
    color += material.ambient * scene.ambient_light;
    for (const auto& light: scene.point_lights) {
        Vec3f light_vector = light.position - hit.hit_point;
        Vec3f light_vector_norm = light_vector.normalize();
        float light_dist = light_vector.length();
        Ray light_ray = Ray(hit.hit_point + light_vector_norm * scene.shadow_ray_epsilon, light_vector_norm);
        RayHit light_hit = light_ray.get_shadow_ray(scene, light_dist);
        if (light_hit.material_id) {
            continue;
        }
        Vec3f irradiance = light.get_irradiance(hit.hit_point);
        color += material.diffuse * irradiance * std::max(0.0f, hit.normal.dot(light_vector_norm));
        Vec3f half_vector = (light_vector_norm - direction).normalize();
        color += material.specular * irradiance * std::pow(std::max(0.0f, hit.normal.dot(half_vector)), material.phong_exponent);
    }
    return color;
}

// Get intersection of ray with triangle
// v0, v1, v2 are the vertices of the triangle
RayHit Ray::get_ray_hit(const Scene &scene, const Triangle &triangle) const {
    RayHit ray_hit(*this, 0);
    Vec3f v0 = scene.vertex_data[triangle.indices.v0_id - 1];
    Vec3f v1 = scene.vertex_data[triangle.indices.v1_id - 1];
    Vec3f v2 = scene.vertex_data[triangle.indices.v2_id - 1];

    Vec3f e0 = v0 - v1;
    Vec3f e1 = v0 - v2;
    Vec3f e2 = v0 - position;

    float detA = Matrix3x3f(e0, e1, direction).determinant();
    if (detA == 0)
        return ray_hit;
    float t = Matrix3x3f(e0, e1, e2).determinant() / detA;
    if (t + scene.shadow_ray_epsilon < .0f)
        return ray_hit;
    float gamma = Matrix3x3f(e0, e2, direction).determinant() / detA;
    if (gamma < .0f - scene.shadow_ray_epsilon || gamma > 1.f + scene.shadow_ray_epsilon)
        return ray_hit;
    float beta = Matrix3x3f(e2, e1, direction).determinant() / detA;
    if (beta < .0f - scene.shadow_ray_epsilon || beta > 1.f - gamma + scene.shadow_ray_epsilon)
        return ray_hit;
    ray_hit.setHit(t, position + direction * (t - scene.shadow_ray_epsilon), (v1 - v0).cross(v2 - v0).normalize());
    ray_hit.material_id = triangle.material_id;
    return ray_hit;
}

RayHit Ray::get_ray_hit(const Scene &scene, const Sphere &sphere) const {
    RayHit ray_hit(*this, 0);
    Vec3f center = scene.vertex_data[sphere.center_vertex_id - 1];
    Vec3f center_to_origin = position - center;
    float a = direction.dot(direction);
    float b = 2 * center_to_origin.dot(direction);
    float c = center_to_origin.dot(center_to_origin) - sphere.radius * sphere.radius;
    float discriminant = b * b - 4 * a * c;
    if (discriminant < 0)
        return ray_hit;
    float t1 = (-b - std::sqrt(discriminant)) / (2 * a);
    float t2 = (-b + std::sqrt(discriminant)) / (2 * a);
    float t = std::min(t1, t2);
    if(t < 0)
        return ray_hit;
    Vec3f hit_point = position + direction * (t - scene.shadow_ray_epsilon);
    ray_hit.setHit(t,
                   hit_point,
                   (hit_point - center) / sphere.radius);
    ray_hit.material_id = sphere.material_id;
    return ray_hit;
}

RayHit Ray::get_ray_hit(const Scene &scene, const Vec3f &v0, const Vec3f &v1, const Vec3f &v2, int material_id) const {
    RayHit ray_hit(*this, 0);

    Vec3f e0 = v0 - v1;
    Vec3f e1 = v0 - v2;
    Vec3f e2 = v0 - position;

    float detA = Matrix3x3f(e0, e1, direction).determinant();
    if (detA == 0)
        return ray_hit;
    float t = Matrix3x3f(e0, e1, e2).determinant() / detA;
    if (t < .0f - scene.shadow_ray_epsilon)
        return ray_hit;
    float gamma = Matrix3x3f(e0, e2, direction).determinant() / detA;
    if (gamma < .0f - scene.shadow_ray_epsilon || gamma > 1.f + scene.shadow_ray_epsilon)
        return ray_hit;
    float beta = Matrix3x3f(e2, e1, direction).determinant() / detA;
    if (beta < .0f - scene.shadow_ray_epsilon || beta > 1.f - gamma + scene.shadow_ray_epsilon)
        return ray_hit;
    ray_hit.setHit(t, position + direction * (t - scene.shadow_ray_epsilon), (v1 - v0).cross(v2 - v0).normalize());
    ray_hit.material_id = material_id;
    return ray_hit;
}

RayHit Ray::get_ray_hit(const Scene &scene, const Mesh &mesh) const {
    RayHit ray_hit(*this, 0);
    float min_t = std::numeric_limits<float>::max();
    for (auto face: mesh.faces) {
        Vec3f v0 = scene.vertex_data[face.v0_id - 1];
        Vec3f v1 = scene.vertex_data[face.v1_id - 1];
        Vec3f v2 = scene.vertex_data[face.v2_id - 1];
        RayHit temp_ray_hit = get_ray_hit(scene, v0, v1, v2, mesh.material_id);
        if (temp_ray_hit.material_id && temp_ray_hit.t < min_t) {
            min_t = temp_ray_hit.t;
            ray_hit = temp_ray_hit;
        }
    }
    return ray_hit;
}

RayHit Ray::get_first_hit(const Scene &scene) {
    RayHit ray_hit(*this, 0);
    float min_t = std::numeric_limits<float>::max();
    for (const auto &mesh: scene.meshes) {
        RayHit temp_ray_hit = get_ray_hit(scene, mesh);
        if (temp_ray_hit.material_id && temp_ray_hit.t < min_t) {
            min_t = temp_ray_hit.t;
            ray_hit = temp_ray_hit;
        }
    }
    for (const auto &sphere: scene.spheres) {
        RayHit temp_ray_hit = get_ray_hit(scene, sphere);
        if (temp_ray_hit.material_id && temp_ray_hit.t < min_t) {
            min_t = temp_ray_hit.t;
            ray_hit = temp_ray_hit;
        }
    }
    for (const auto &triangle: scene.triangles) {
        RayHit temp_ray_hit = get_ray_hit(scene, triangle);
        if (temp_ray_hit.material_id && temp_ray_hit.t < min_t) {
            min_t = temp_ray_hit.t;
            ray_hit = temp_ray_hit;
        }
    }
    return ray_hit;
}

RayHit Ray::get_shadow_ray(const Scene &scene, float light_distance) {
    light_distance += scene.shadow_ray_epsilon;
    RayHit ray_hit(*this, 0);
    for (const auto &mesh: scene.meshes) {
        RayHit temp_ray_hit = get_ray_hit(scene, mesh);
        if (temp_ray_hit.material_id && temp_ray_hit.t < light_distance) {
            return temp_ray_hit;
        }
    }
    for (const auto &sphere: scene.spheres) {
        RayHit temp_ray_hit = get_ray_hit(scene, sphere);
        if (temp_ray_hit.material_id && temp_ray_hit.t < light_distance) {
            return temp_ray_hit;
        }
    }
    for (const auto &triangle: scene.triangles) {
        RayHit temp_ray_hit = get_ray_hit(scene, triangle);
        if (temp_ray_hit.material_id && temp_ray_hit.t < light_distance) {
            return temp_ray_hit;
        }
    }
    return ray_hit;
}
