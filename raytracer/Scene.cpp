//
// Created by goksel on 11/27/21.
//

#include <thread>
#include "Scene.h"
#include "parser.h"
#include "ppm.h"

Scene::Scene(const parser::Scene &scene) {
    background_color = Vec3i(scene.background_color);
    shadow_ray_epsilon = scene.shadow_ray_epsilon;
    max_recursion_depth = scene.max_recursion_depth;
    ambient_light = Vec3f(scene.ambient_light);
    for (const auto &camera : scene.cameras) {
        cameras.emplace_back(Camera(camera));
    }
    for (const auto &point_light : scene.point_lights) {
        point_lights.emplace_back(PointLight(point_light));
    }
    for (const auto &material : scene.materials) {
        materials.emplace_back(Material(material));
    }
    for (const auto &vertex : scene.vertex_data) {
        vertex_data.emplace_back(Vec3f(vertex));
    }
    for (const auto &mesh : scene.meshes) {
        meshes.emplace_back(Mesh(mesh));
    }
    for (const auto &triangle : scene.triangles) {
        triangles.emplace_back(Triangle(triangle));
    }
    for (const auto &sphere : scene.spheres) {
        spheres.emplace_back(Sphere(sphere));
    }
}

void Scene::render_region(const Camera& camera, unsigned char image[], int from, int to) const{
    for(int row = 0; row < camera.image_height; row++){
        for(int col = from; col < to; col++){
            Ray ray = camera.getRay(static_cast<float>(row),
                                    static_cast<float>(col)
                                    );
            Vec3f color = ray.get_color(*this, 0).clamp({0, 0, 0}, {255, 255, 255});
            image[(row*camera.image_width + col) * 3] = static_cast<unsigned int>(color.x);
            image[(row*camera.image_width + col) * 3 + 1] = static_cast<unsigned int>(color.y);
            image[(row*camera.image_width + col) * 3 + 2] = static_cast<unsigned int>(color.z);
        }
    }
}

void Scene::render_multithreaded(){
    for(auto camera: cameras) {
        auto image = new unsigned char[camera.image_width * camera.image_height * 3];
        std::vector<std::thread> threads;
        int thread_count = static_cast<int>(std::thread::hardware_concurrency() * 8);
        if(thread_count == 0) {
            thread_count = 32;
        }
        int thread_size = camera.image_width / thread_count;
        threads.reserve(thread_count);
        for(int i = 0; i < thread_count; i++){
            threads.emplace_back(std::thread(&Scene::render_region, this, camera, image, i * thread_size, (i + 1) * thread_size));
        }
        if(camera.image_width % thread_count != 0){
            threads.emplace_back(std::thread(&Scene::render_region, this, camera, image, thread_count * thread_size, camera.image_width));
        }
        for(auto &thread: threads){
            thread.join();
        }
        write_ppm(camera.image_name.c_str(), image, camera.image_width, camera.image_height);
        delete[] image;
    }
}

Scene::Scene() = default;