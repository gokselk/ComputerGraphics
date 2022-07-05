#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "Vec3.h"
#include "Matrix4.h"
#include <string>

class Camera {

public:
    int cameraId;
    int projectionType; // 1 for perspective, 0 for orthographic
    Vec3 pos;
    Vec3 gaze;
    Vec3 u;
    Vec3 v;
    Vec3 w;
    double left, right, bottom, top;
    double near;
    double far;
    int horRes;
    int verRes;
    std::string outputFileName;

    Camera();

    Camera(int cameraId, int projectionType, const Vec3& pos, const Vec3& gaze, const Vec3& u, const Vec3& v, const Vec3& w, double left, double right,
           double bottom, double top, double near, double far, int horRes, int verRes, std::string outputFileName);

    Camera(const Camera &other);

    friend std::ostream &operator<<(std::ostream &os, const Camera &c);

    [[nodiscard]] Matrix4 getCameraTransformationMatrix() const;

    [[nodiscard]] Matrix4 getPerspectiveTransformationMatrix() const;

    [[nodiscard]] Matrix4 getProjectionTransformationMatrix() const;

    [[nodiscard]] Matrix4 getOrthographicTransformationMatrix() const;

    [[nodiscard]] Matrix4 getViewportTransformationMatrix() const;
};

#endif