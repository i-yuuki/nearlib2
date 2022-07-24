#include "pch.h"
#include <NearLib/transform.h>

namespace Near{

Math::Vector3 Transform::getForward() const{
  Math::Matrix rot = Math::Matrix::CreateFromQuaternion(rotation);
  return rot.Forward();
}

Math::Vector3 Transform::getRight() const{
  Math::Matrix rot = Math::Matrix::CreateFromQuaternion(rotation);
  return rot.Right();
}

Math::Vector3 Transform::getUp() const{
  Math::Matrix rot = Math::Matrix::CreateFromQuaternion(rotation);
  return rot.Up();
}

Math::Matrix Transform::createTransform() const{
  return Math::Matrix::CreateScale(scale) * Math::Matrix::CreateFromQuaternion(rotation) * Math::Matrix::CreateTranslation(position);
}

}
