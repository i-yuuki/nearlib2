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

// 数値をある範囲から別の範囲にリマップします。
// たとえば valueStart～valueEnd の真ん中の値を渡すと
// targetStart～targetEnd の真ん中の値が返ります。
// @param clamp `value`を`valueStart`～`valueEnd`の範囲に収めてからリマップするかどうか
template<typename T>
constexpr T map(T value, T valueStart, T valueEnd, T targetStart, T targetEnd, bool clamp = false){
  if(clamp){
    value = std::clamp(value, std::min(valueStart, valueEnd), std::max(valueStart, valueEnd));
  }
  return targetStart + ((value - valueStart) / (valueEnd - valueStart)) * (targetEnd - targetStart);
}

}
