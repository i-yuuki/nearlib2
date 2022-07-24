#pragma once

#include "math.h"

namespace Near{

class Transform{
public:
  // このTransformの前方向を表すベクトル (unit vector) を新たに作ります。
  Math::Vector3 getForward() const;
  // このTransformの右方向を表すベクトル (unit vector) を新たに作ります。
  Math::Vector3 getRight() const;
  // このTransformの上方向を表すベクトル (unit vector) を新たに作ります。
  Math::Vector3 getUp() const;
  // 拡大・回転・移動のMatrixを新たに作ります。
  Math::Matrix createTransform() const;
  Math::Vector3 position = Math::Vector3(0.0f);
  Math::Quaternion rotation = Math::Quaternion::Identity;
  Math::Vector3 scale = Math::Vector3(1.0f);
};

}
