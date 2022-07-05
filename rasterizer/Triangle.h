#ifndef __TRIANGLE_H__
#define __TRIANGLE_H__


class Triangle {
public:
    int vertexIds[3];

    Triangle();

    Triangle(int vid1, int vid2, int vid3);

    Triangle(const Triangle &other);

    int getFirstVertexId();

    int getSecondVertexId();

    int getThirdVertexId();

    [[maybe_unused]] void setFirstVertexId(int vid);

    [[maybe_unused]] void setSecondVertexId(int vid);

    [[maybe_unused]] void setThirdVertexId(int vid);
};


#endif