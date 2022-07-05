#ifndef RAYTRACER_CAMERA_H
#define RAYTRACER_CAMERA_H

#include "Vec3f.h"
#include "Vec4f.h"
#include "Ray.h"
#include "parser.h"

class Camera {
private:
    Vec3f position;
    Vec3f gaze;
    Vec3f up;
    Vec3f right;
    Vec4f near_plane;
    float near_distance;

    float plane_left;
    float plane_right;
    float plane_bottom;
    float plane_top;
    float plane_width;
    float plane_height;
    Vec3f plane_center;
    Vec3f plane_topleft;
public:
    explicit Camera(const parser::Camera& camera);
    Ray getRay(float x, float y) const;

    int getImgWidth();
    int getImgHeight();
    const char *getFilename();

    float getPlaneLeft();

    float getPlaneRight();

    float getPlaneBottom();

    float getPlaneTop();

    float getNearDistance();

    Vec3f getPosition();

    Vec3f getCameraUp();

    Vec3f getCameraGaze();

    Vec3f getCameraRight();

    int image_width;
    int image_height;
    std::string image_name;
};

#endif