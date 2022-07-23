#pragma once

#include <string>

#include "near.h"

namespace Near{

class Asset{
public:
  virtual void load(NearLib* lib, const std::string& path) = 0;
};

}
