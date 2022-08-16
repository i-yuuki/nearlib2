#pragma once

#include "math.h"

namespace Near{

struct Vertex3D{
  Math::Vector3 position;
  Math::Vector3 normal;
  Math::Color   color;
  Math::Vector2 uv;
};

}
