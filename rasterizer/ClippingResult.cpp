//
// Created by goksel on 1/8/22.
//

#include "ClippingResult.h"

ClippingResult::ClippingResult(bool visible, const Vec4 &from, const Vec4 &to, const Color &from_color,
                               const Color &to_color) {
    this->visible = visible;
    this->from = from;
    this->to = to;
    this->from_color = from_color;
    this->to_color = to_color;
}
