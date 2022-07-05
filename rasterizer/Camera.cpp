#include "Camera.h"
#include <string>
#include <iostream>
#include <iomanip>
#include <utility>

enum ProjectionType {
    ORTHOGRAPHIC = 0, PERSPECTIVE = 1
};

Camera::Camera() = default;

Camera::Camera(int cameraId, int projectionType, const Vec3 &pos, const Vec3 &gaze, const Vec3 &u, const Vec3 &v,
               const Vec3 &w, double left, double right, double bottom, double top, double near, double far, int horRes,
               int verRes, std::string outputFileName) {

    this->cameraId = cameraId;
    this->projectionType = projectionType;
    this->pos = pos;
    this->gaze = gaze;
    this->u = u;
    this->v = v;
    this->w = w;
    this->left = left;
    this->right = right;
    this->bottom = bottom;
    this->top = top;
    this->near = near;
    this->far = far;
    this->horRes = horRes;
    this->verRes = verRes;
    this->outputFileName = std::move(outputFileName);
}

Camera::Camera(const Camera &other) {
    this->cameraId = other.cameraId;
    this->projectionType = other.projectionType;
    this->pos = other.pos;
    this->gaze = other.gaze;
    this->u = other.u;
    this->v = other.v;
    this->w = other.w;
    this->left = other.left;
    this->right = other.right;
    this->bottom = other.bottom;
    this->top = other.top;
    this->near = other.near;
    this->far = other.far;
    this->horRes = other.horRes;
    this->verRes = other.verRes;
    this->outputFileName = other.outputFileName;
}

std::ostream &operator<<(std::ostream &os, const Camera &c) {
    const char *camType = c.projectionType ? "perspective" : "orthographic";

    os << std::fixed << std::setprecision(6) << "Camera " << c.cameraId << " (" << camType << ") => pos: " << c.pos
       << " gaze: " << c.gaze << std::endl << "\tu: " << c.u << " v: " << c.v << " w: " << c.w << std::endl
       << std::fixed << std::setprecision(3) << "\tleft: " << c.left << " right: " << c.right << " bottom: " << c.bottom
       << " top: " << c.top << std::endl << "\tnear: " << c.near << " far: " << c.far << " resolutions: " << c.horRes
       << "x" << c.verRes << " fileName: " << c.outputFileName;

    return os;
}

Matrix4 Camera::getCameraTransformationMatrix() const {
    double cameraTransformation[4][4] = {u.x, u.y, u.z, -u.dot(pos), v.x, v.y, v.z, -v.dot(pos), w.x, w.y, w.z,
                                         -w.dot(pos), 0, 0, 0, 1};
    return {cameraTransformation};
}

Matrix4 Camera::getPerspectiveTransformationMatrix() const {
    double perspectiveTransformation[4][4] = {near * 2 / (right - left), 0, (right + left) / (right - left), 0, 0,
                                              near * 2 / (top - bottom), (top + bottom) / (top - bottom), 0, 0, 0,
                                              -(far + near) / (far - near), -(2 * far * near) / (far - near), 0, 0, -1,
                                              0};
    return {perspectiveTransformation};
}

Matrix4 Camera::getViewportTransformationMatrix() const {
    double viewportTransformation[4][4] = {static_cast<double>(horRes) / 2, 0, 0, (static_cast<double>(horRes) - 1) / 2,
                                           0, static_cast<double>(verRes) / 2, 0, (static_cast<double>(verRes) - 1) / 2,
                                           0, 0, 0.5, 0.5, 0, 0, 0, 0};
    return {viewportTransformation};
}

Matrix4 Camera::getProjectionTransformationMatrix() const {
    switch (projectionType) {
        case ORTHOGRAPHIC:
            return getOrthographicTransformationMatrix();
        case PERSPECTIVE:
            return getPerspectiveTransformationMatrix();
        default:
            return {};
    }
}

Matrix4 Camera::getOrthographicTransformationMatrix() const {
    double orthographicTransformation[4][4] = {2 / (right - left), 0, 0, -(right + left) / (right - left), 0,
                                               2 / (top - bottom), 0, -(top + bottom) / (top - bottom), 0, 0,
                                               -2 / (far - near), -(far + near) / (far - near), 0, 0, 0, 1};
    return {orthographicTransformation};
}