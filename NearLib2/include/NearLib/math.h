#pragma once

#include "DirectXTK/SimpleMath.h"

namespace Near::Math{

// MARUNAGE
using namespace DirectX::SimpleMath;

constexpr float PI = DirectX::XM_PI;

constexpr float toRadians(float degrees){
  return DirectX::XMConvertToRadians(degrees);
}

constexpr float toDegrees(float radians){
  return DirectX::XMConvertToDegrees(radians);
}

}
