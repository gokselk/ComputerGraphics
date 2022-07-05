//
// Created by goksel on 11/26/21.
//

#include "Camera.h"

Camera::Camera(const parser::Camera& camera){
    position = Vec3f(camera.position);
    gaze = Vec3f(camera.gaze);
    up = Vec3f(camera.up);
    right = gaze.cross(up).normalize();
    near_plane = Vec4f(camera.near_plane);
    near_distance = camera.near_distance;
    image_width = camera.image_width;
    image_height = camera.image_height;
    image_name = camera.image_name;

    plane_left = near_plane.x;
    plane_right = near_plane.y;
    plane_bottom = near_plane.z;
    plane_top = near_plane.w;

    plane_width = plane_right - plane_left;
    plane_height = plane_top - plane_bottom;
    plane_center = position + gaze * near_distance;
    plane_topleft = plane_center + right * near_plane.x + up * near_plane.w;
}

Ray Camera::getRay(float row, float col) const {
    float u = (col + 0.5f) * plane_width / static_cast<float>(image_width);
    float v = (row + 0.5f) * plane_height / static_cast<float>(image_height);

    Vec3f pixel_pos = (plane_topleft + right * u - up * v);
    Vec3f direction = (pixel_pos - position).normalize();
    return {position, direction};
}

const char* Camera::getFilename() {return image_name.c_str();}
int Camera::getImgWidth() {return image_width;}
int Camera::getImgHeight() {return image_height;}
float Camera::getPlaneLeft() {return plane_left;}
float Camera::getPlaneRight() {return plane_right;}
float Camera::getPlaneBottom() {return plane_bottom;}
float Camera::getPlaneTop() {return plane_top;}
float Camera::getNearDistance() {return near_distance;}
Vec3f Camera::getPosition() {return position;}
Vec3f Camera::getCameraUp() {return up;}
Vec3f Camera::getCameraGaze() {return gaze;}
Vec3f Camera::getCameraRight() {return right;}
