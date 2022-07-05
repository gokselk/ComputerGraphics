//
// Created by goksel on 1/8/22.
//

#ifndef HW2_CLIPPINGRESULT_H
#define HW2_CLIPPINGRESULT_H

#include "Vec4.h"
#include "Color.h"

class ClippingResult {
public:
    bool visible;
    Vec4 from;
    Vec4 to;
    Color from_color;
    Color to_color;

    ClippingResult(bool visible, const Vec4 &from, const Vec4 &to, const Color &from_color, const Color &to_color);

};


#endif //HW2_CLIPPINGRESULT_H
