#ifndef _SCENE_H_
#define _SCENE_H_

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>

#include "Camera.h"
#include "ClippingResult.h"
#include "Color.h"
#include "Mesh.h"
#include "Rotation.h"
#include "Scaling.h"
#include "Translation.h"
#include "Triangle.h"
#include "Vec3.h"
#include "Vec4.h"

class Scene {
private:
    ClippingResult clipLine(Camera &cam, const Vec4 &from, const Vec4 &to);

    void drawLine(const Vec4 &from, const Vec4 &to, const Color &from_color, const Color &to_color);

    void drawLineMidpoint(const Vec4 &from, const Vec4 &to, const Color &from_color, const Color &to_color);

    void drawLineMidpointExtended(const Vec4 &from, const Vec4 &to, const Color &from_color, const Color &to_color);

    void drawPixel(int x, int y, const Color &color);

    void drawTriangle(const Vec4 &v0, const Vec4 &v1, const Vec4 &v2);

    [[nodiscard]] Matrix4 getModelingTransformationMatrix(const Camera &camera, const Mesh &mesh) const;

public:
    Color backgroundColor;
    bool cullingEnabled;

    std::vector<std::vector<Color> > image;
    std::vector<Camera *> cameras;
    std::vector<Vec3 *> vertices;
    std::vector<Color *> colorsOfVertices;
    std::vector<Scaling *> scalings;
    std::vector<Rotation *> rotations;
    std::vector<Translation *> translations;
    std::vector<Mesh *> meshes;

    explicit Scene(const char *xmlPath);

    void initializeImage(Camera *camera);

    void forwardRenderingPipeline(Camera *camera);

    static int makeBetweenZeroAnd255(double value);

    void writeImageToPPMFile(Camera *camera);

    static void convertPPMToPNG(const std::string &ppmFileName, int osType);
};

#endif
