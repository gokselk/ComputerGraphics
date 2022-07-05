#include <iostream>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <cmath>

#include "Scene.h"
#include "Camera.h"
#include "Color.h"
#include "Mesh.h"
#include "Rotation.h"
#include "Scaling.h"
#include "Translation.h"
#include "Triangle.h"
#include "Vec3.h"
#include "tinyxml2.h"
#include "Helpers.h"

/*
	Transformations, clipping, culling, rasterization are done here.
	You may define helper functions.
*/

enum ModelType {
    WIREFRAME = 0, SOLID = 1
};

Matrix4 Scene::getModelingTransformationMatrix(const Camera &camera, const Mesh &mesh) const {
    Matrix4 M = getIdentityMatrix();
    for (int i = 0; i < mesh.numberOfTransformations; i++) {
        auto transformationType = mesh.transformationTypes[i];
        switch (transformationType) {
            case 's': {
                Scaling &scaling = *scalings[mesh.transformationIds[i] - 1];
                M = multiplyMatrixWithMatrix(scaling.getMatrix(), M);
                break;
            }
            case 'r': {
                Rotation &rotation = *rotations[mesh.transformationIds[i] - 1];
                M = multiplyMatrixWithMatrix(rotation.getMatrix(), M);
                break;
            }
            case 't': {
                Translation &translation = *translations[mesh.transformationIds[i] - 1];
                M = multiplyMatrixWithMatrix(translation.getMatrix(), M);
                break;
            }
            default:
                break;
        }
    }
    return M;
}

void Scene::drawPixel(int x, int y, const Color &color) {
    int width = static_cast<int>(image.size());
    int height = static_cast<int>(image[0].size());
    if (x < 0 || x >= width || y < 0 || y >= height) {
        return;
    }
    image[x][y] = color.round();
}

void Scene::drawLineMidpoint(const Vec4 &from, const Vec4 &to, const Color &from_color, const Color &to_color) {
    double dx = to.x - from.x;
    double dy = to.y - from.y;
    int yi = 1;
    if (dy < 0) {
        yi = -1;
        dy = -dy;
    }
    double D = 2 * dy - dx;

    Color c = from_color;
    Color ci = (to_color - from_color) / dx;
    int y = static_cast<int>(from.y);

    for (int x = static_cast<int>(from.x); x <= static_cast<int>(to.x); x++) {
        drawPixel(x, y, c);
        c += ci;
        if (D > 0) {
            y += yi;
            D += 2 * (dy - dx);
        } else {
            D += 2 * dy;
        }
    }
}

void Scene::drawLineMidpointExtended(const Vec4 &from, const Vec4 &to, const Color &from_color, const Color &to_color) {
    double dx = to.x - from.x;
    double dy = to.y - from.y;
    int xi = 1;

    if (dx < 0) {
        xi = -1;
        dx = -dx;
    }
    double D = 2 * dx - dy;


    Color c = from_color;
    Color ci = (to_color - from_color) / dy;
    int x = static_cast<int>(from.x);

    for (int y = static_cast<int>(from.y); y <= static_cast<int>(to.y); y++) {
        drawPixel(x, y, c);
        c += ci;
        if (D > 0) {
            x += xi;
            D += 2 * (dx - dy);
        } else {
            D += 2 * dx;
        }
    }
}

void Scene::drawLine(const Vec4 &from, const Vec4 &to, const Color &from_color, const Color &to_color) {
    double dx = to.x - from.x;
    double dy = to.y - from.y;
    if (std::abs(dy) < std::abs(dx)) {
        // Slope is less than 1
        if (from.x <= to.x) {
            drawLineMidpoint(from, to, from_color, to_color);
        } else {
            drawLineMidpoint(to, from, to_color, from_color);
        }
    } else {
        // Slope is greater than 1
        if (from.y <= to.y) {
            drawLineMidpointExtended(from, to, from_color, to_color);
        } else {
            drawLineMidpointExtended(to, from, to_color, from_color);
        }
    }
}

void Scene::drawTriangle(const Vec4 &v0, const Vec4 &v1, const Vec4 &v2) {
    auto nx = static_cast<double>(image.size());
    auto ny = static_cast<double>(image[0].size());
    double xmin = std::min(std::min(v0.x, v1.x), v2.x);
    xmin = std::min(xmin, nx - 1);
    double xmax = std::max(std::max(v0.x, v1.x), v2.x);
    xmax = std::min(xmax, nx - 1);
    double ymin = std::min(std::min(v0.y, v1.y), v2.y);
    ymin = std::min(ymin, ny - 1);
    double ymax = std::max(std::max(v0.y, v1.y), v2.y);
    ymax = std::min(ymax, ny - 1);
    for (int i = static_cast<int>(xmin); i <= static_cast<int>(xmax); i++) {
        for (int j = static_cast<int>(ymin); j <= static_cast<int>(ymax); j++) {
            double x = i;
            double y = j;
            double alpha = f_ab(x, y, v1.x, v1.y, v2.x, v2.y) / f_ab(v0.x, v0.y, v1.x, v1.y, v2.x, v2.y);
            double beta = f_ab(x, y, v2.x, v2.y, v0.x, v0.y) / f_ab(v1.x, v1.y, v2.x, v2.y, v0.x, v0.y);
            double gamma = f_ab(x, y, v0.x, v0.y, v1.x, v1.y) / f_ab(v2.x, v2.y, v0.x, v0.y, v1.x, v1.y);
            if (alpha >= 0 && beta >= 0 && gamma >= 0) {
                Color &c0 = *colorsOfVertices[v0.colorId - 1];
                Color &c1 = *colorsOfVertices[v1.colorId - 1];
                Color &c2 = *colorsOfVertices[v2.colorId - 1];
                Color c = (c0 * alpha + c1 * beta + c2 * gamma);
                drawPixel(i, j, c);
            }
        }
    }
}

ClippingResult Scene::clipLine(Camera &cam, const Vec4 &from, const Vec4 &to) {
    double tE = 0;
    double tL = 1;
    bool visible = false;

    Vec4 v1 = from;
    Vec4 v2 = to;
    Color c1 = *colorsOfVertices[v1.colorId - 1];
    Color c2 = *colorsOfVertices[v2.colorId - 1];

    double dx = v2.x - v1.x;
    double dy = v2.y - v1.y;
    double dz = v2.z - v1.z;
    Color dc = c2 - c1;

    constexpr double x_min = -1, y_min = -1, z_min = -1, x_max = 1, y_max = 1, z_max = 1;

    if (isVisible(dx, x_min - v1.x, tE, tL) && isVisible(-dx, v1.x - x_max, tE, tL) &&
        isVisible(dy, y_min - v1.y, tE, tL) && isVisible(-dy, v1.y - y_max, tE, tL) &&
        isVisible(dz, z_min - v1.z, tE, tL) && isVisible(-dz, v1.z - z_max, tE, tL)) {
        visible = true;
        if (tL < 1) {
            v2.x = v1.x + dx * tL;
            v2.y = v1.y + dy * tL;
            v2.z = v1.z + dz * tL;
            c2 = c1 + dc * tL;
        }
        if (tE > 0) {
            v1.x += dx * tE;
            v1.y += dy * tE;
            v1.z += dz * tE;
            c1 += dc * tE;
        }
    }
    return {visible, v1, v2, c1, c2};
}

bool isSurfaceVisible(const Vec4 &v1, const Vec4 &v2, const Vec4 &v3) {
    Vec3 v1_3d = {v1.x, v1.y, v1.z, -1};
    Vec3 v2_3d = {v2.x, v2.y, v2.z, -1};
    Vec3 v3_3d = {v3.x, v3.y, v3.z, -1};
    Vec3 normal = (v2_3d - v1_3d).cross(v3_3d - v1_3d);
    return normal.dot(v1_3d) > 0;
}

void Scene::forwardRenderingPipeline(Camera *camera) {
    Matrix4 cam_tm = camera->getCameraTransformationMatrix();
    Matrix4 viewport_tm = camera->getViewportTransformationMatrix();
    Matrix4 projection_tm = camera->getProjectionTransformationMatrix();
    for (auto mesh: meshes) {
        Matrix4 modeling_tm = getModelingTransformationMatrix(*camera, *mesh);
        Matrix4 tm = projection_tm * cam_tm * modeling_tm;
        for (auto triangle: mesh->triangles) {
            const Vec3 &v1 = *vertices[triangle.getFirstVertexId() - 1];
            const Vec3 &v2 = *vertices[triangle.getSecondVertexId() - 1];
            const Vec3 &v3 = *vertices[triangle.getThirdVertexId() - 1];
            Vec4 v1p = tm * Vec4(v1.x, v1.y, v1.z, 1., v1.colorId);
            Vec4 v2p = tm * Vec4(v2.x, v2.y, v2.z, 1., v2.colorId);
            Vec4 v3p = tm * Vec4(v3.x, v3.y, v3.z, 1., v3.colorId);

            if (cullingEnabled && !isSurfaceVisible(v1p, v2p, v3p))
                continue;

            v1p.perspectiveDivide();
            v2p.perspectiveDivide();
            v3p.perspectiveDivide();

            switch (mesh->type) {
                case WIREFRAME: {
                    ClippingResult e12 = clipLine(*camera, v1p, v2p);
                    ClippingResult e23 = clipLine(*camera, v2p, v3p);
                    ClippingResult e31 = clipLine(*camera, v3p, v1p);
                    if (e12.visible)
                        drawLine(viewport_tm * e12.from, viewport_tm * e12.to, e12.from_color, e12.to_color);
                    if (e23.visible)
                        drawLine(viewport_tm * e23.from, viewport_tm * e23.to, e23.from_color, e23.to_color);
                    if (e31.visible)
                        drawLine(viewport_tm * e31.from, viewport_tm * e31.to, e31.from_color, e31.to_color);
                    break;
                }
                case SOLID:
                    drawTriangle(viewport_tm * v1p, viewport_tm * v2p, viewport_tm * v3p);
                    break;
                default:
                    break;
            }
        }
    }
}

/*
	Parses XML file
*/
Scene::Scene(const char *xmlPath) {
    const char *str;
    tinyxml2::XMLDocument xmlDoc;
    tinyxml2::XMLElement *pElement;

    xmlDoc.LoadFile(xmlPath);

    tinyxml2::XMLNode *pRoot = xmlDoc.FirstChild();

    // read background color
    pElement = pRoot->FirstChildElement("BackgroundColor");
    str = pElement->GetText();
    sscanf(str, "%lf %lf %lf", &backgroundColor.r, &backgroundColor.g, &backgroundColor.b);

    // read culling
    pElement = pRoot->FirstChildElement("Culling");
    if (pElement != nullptr) {
        str = pElement->GetText();

        if (strcmp(str, "enabled") == 0) {
            cullingEnabled = true;
        } else {
            cullingEnabled = false;
        }
    }

    // read cameras
    pElement = pRoot->FirstChildElement("Cameras");
    tinyxml2::XMLElement *pCamera = pElement->FirstChildElement("Camera");
    tinyxml2::XMLElement *camElement;
    while (pCamera != nullptr) {
        auto *cam = new Camera();

        pCamera->QueryIntAttribute("id", &cam->cameraId);

        // read projection type
        str = pCamera->Attribute("type");

        if (strcmp(str, "orthographic") == 0) {
            cam->projectionType = 0;
        } else {
            cam->projectionType = 1;
        }

        camElement = pCamera->FirstChildElement("Position");
        str = camElement->GetText();
        sscanf(str, "%lf %lf %lf", &cam->pos.x, &cam->pos.y, &cam->pos.z);

        camElement = pCamera->FirstChildElement("Gaze");
        str = camElement->GetText();
        sscanf(str, "%lf %lf %lf", &cam->gaze.x, &cam->gaze.y, &cam->gaze.z);

        camElement = pCamera->FirstChildElement("Up");
        str = camElement->GetText();
        sscanf(str, "%lf %lf %lf", &cam->v.x, &cam->v.y, &cam->v.z);

        cam->gaze = normalizeVec3(cam->gaze);
        cam->u = crossProductVec3(cam->gaze, cam->v);
        cam->u = normalizeVec3(cam->u);

        cam->w = inverseVec3(cam->gaze);
        cam->v = crossProductVec3(cam->u, cam->gaze);
        cam->v = normalizeVec3(cam->v);

        camElement = pCamera->FirstChildElement("ImagePlane");
        str = camElement->GetText();
        sscanf(str, "%lf %lf %lf %lf %lf %lf %d %d", &cam->left, &cam->right, &cam->bottom, &cam->top, &cam->near,
               &cam->far, &cam->horRes, &cam->verRes);

        camElement = pCamera->FirstChildElement("OutputName");
        str = camElement->GetText();
        cam->outputFileName = std::string(str);

        cameras.push_back(cam);

        pCamera = pCamera->NextSiblingElement("Camera");
    }

    // read vertices
    pElement = pRoot->FirstChildElement("Vertices");
    tinyxml2::XMLElement *pVertex = pElement->FirstChildElement("Vertex");
    int vertexId = 1;

    while (pVertex != nullptr) {
        Vec3 *vertex = new Vec3();
        auto *color = new Color();

        vertex->colorId = vertexId;

        str = pVertex->Attribute("position");
        sscanf(str, "%lf %lf %lf", &vertex->x, &vertex->y, &vertex->z);

        str = pVertex->Attribute("color");
        sscanf(str, "%lf %lf %lf", &color->r, &color->g, &color->b);

        vertices.push_back(vertex);
        colorsOfVertices.push_back(color);

        pVertex = pVertex->NextSiblingElement("Vertex");

        vertexId++;
    }

    // read translations
    pElement = pRoot->FirstChildElement("Translations");
    tinyxml2::XMLElement *pTranslation = pElement->FirstChildElement("Translation");
    while (pTranslation != nullptr) {
        auto *translation = new Translation();

        pTranslation->QueryIntAttribute("id", &translation->translationId);

        str = pTranslation->Attribute("value");
        sscanf(str, "%lf %lf %lf", &translation->tx, &translation->ty, &translation->tz);

        translations.push_back(translation);

        pTranslation = pTranslation->NextSiblingElement("Translation");
    }

    // read scalings
    pElement = pRoot->FirstChildElement("Scalings");
    tinyxml2::XMLElement *pScaling = pElement->FirstChildElement("Scaling");
    while (pScaling != nullptr) {
        auto *scaling = new Scaling();

        pScaling->QueryIntAttribute("id", &scaling->scalingId);
        str = pScaling->Attribute("value");
        sscanf(str, "%lf %lf %lf", &scaling->sx, &scaling->sy, &scaling->sz);

        scalings.push_back(scaling);

        pScaling = pScaling->NextSiblingElement("Scaling");
    }

    // read rotations
    pElement = pRoot->FirstChildElement("Rotations");
    tinyxml2::XMLElement *pRotation = pElement->FirstChildElement("Rotation");
    while (pRotation != nullptr) {
        auto *rotation = new Rotation();

        pRotation->QueryIntAttribute("id", &rotation->rotationId);
        str = pRotation->Attribute("value");
        sscanf(str, "%lf %lf %lf %lf", &rotation->angle, &rotation->ux, &rotation->uy, &rotation->uz);

        rotations.push_back(rotation);

        pRotation = pRotation->NextSiblingElement("Rotation");
    }

    // read meshes
    pElement = pRoot->FirstChildElement("Meshes");

    tinyxml2::XMLElement *pMesh = pElement->FirstChildElement("Mesh");
    tinyxml2::XMLElement *meshElement;
    while (pMesh != nullptr) {
        Mesh *mesh = new Mesh();

        pMesh->QueryIntAttribute("id", &mesh->meshId);

        // read projection type
        str = pMesh->Attribute("type");

        if (strcmp(str, "wireframe") == 0) {
            mesh->type = 0;
        } else {
            mesh->type = 1;
        }

        // read mesh transformations
        tinyxml2::XMLElement *pTransformations = pMesh->FirstChildElement("Transformations");
        tinyxml2::XMLElement *pTransformation = pTransformations->FirstChildElement("Transformation");

        while (pTransformation != nullptr) {
            char transformationType;
            int transformationId;

            str = pTransformation->GetText();
            sscanf(str, "%c %d", &transformationType, &transformationId);

            mesh->transformationTypes.push_back(transformationType);
            mesh->transformationIds.push_back(transformationId);

            pTransformation = pTransformation->NextSiblingElement("Transformation");
        }

        mesh->numberOfTransformations = mesh->transformationIds.size();

        // read mesh faces
        char *row;
        char *clone_str;
        int v1, v2, v3;
        tinyxml2::XMLElement *pFaces = pMesh->FirstChildElement("Faces");
        str = pFaces->GetText();
        clone_str = strdup(str);

        row = strtok(clone_str, "\n");
        while (row != nullptr) {
            int result = sscanf(row, "%d %d %d", &v1, &v2, &v3);

            if (result != EOF) {
                mesh->triangles.emplace_back(v1, v2, v3);
            }
            row = strtok(nullptr, "\n");
        }
        mesh->numberOfTriangles = mesh->triangles.size();
        meshes.push_back(mesh);

        pMesh = pMesh->NextSiblingElement("Mesh");
    }
}

/*
	Initializes image with background color
*/
void Scene::initializeImage(Camera *camera) {
    if (this->image.empty()) {
        for (int i = 0; i < camera->horRes; i++) {
            std::vector<Color> rowOfColors;

            rowOfColors.reserve(camera->verRes);
            for (int j = 0; j < camera->verRes; j++) {
                rowOfColors.push_back(this->backgroundColor);
            }

            this->image.push_back(rowOfColors);
        }
    } else {
        for (int i = 0; i < camera->horRes; i++) {
            for (int j = 0; j < camera->verRes; j++) {
                this->image[i][j].r = this->backgroundColor.r;
                this->image[i][j].g = this->backgroundColor.g;
                this->image[i][j].b = this->backgroundColor.b;
            }
        }
    }
}

/*
	If given value is less than 0, converts value to 0.
	If given value is more than 255, converts value to 255.
	Otherwise returns value itself.
*/
int Scene::makeBetweenZeroAnd255(double value) {
    if (value >= 255.0)
        return 255;
    if (value <= 0.0)
        return 0;
    return (int) (value);
}

/*
	Writes contents of image (Color**) into a PPM file.
*/
void Scene::writeImageToPPMFile(Camera *camera) {
    std::ofstream fout;

    fout.open(camera->outputFileName.c_str());

    fout << "P3" << std::endl;
    fout << "# " << camera->outputFileName << std::endl;
    fout << camera->horRes << " " << camera->verRes << std::endl;
    fout << "255" << std::endl;

    for (int j = camera->verRes - 1; j >= 0; j--) {
        for (int i = 0; i < camera->horRes; i++) {
            fout << makeBetweenZeroAnd255(this->image[i][j].r) << " " << makeBetweenZeroAnd255(this->image[i][j].g)
                 << " " << makeBetweenZeroAnd255(this->image[i][j].b) << " ";
        }
        fout << std::endl;
    }
    fout.close();
}

/*
	Converts PPM image in given path to PNG file, by calling ImageMagick's 'convert' command.
	os_type == 1 		-> Ubuntu
	os_type == 2 		-> Windows
	os_type == other	-> No conversion
*/
void Scene::convertPPMToPNG(const std::string &ppmFileName, int osType) {
    std::string command;

    // call command on Ubuntu
    if (osType == 1) {
        command = "convert " + ppmFileName + " " + ppmFileName + ".png";
        system(command.c_str());
    }

        // call command on Windows
    else if (osType == 2) {
        command = "magick convert " + ppmFileName + " " + ppmFileName + ".png";
        system(command.c_str());
    }

        // default action - don't do conversion
    else {
    }
}