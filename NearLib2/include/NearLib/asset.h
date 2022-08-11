#pragma once

#include <string>

namespace Near{

class Asset{
public:
  virtual void load(const std::string& path) = 0;
};

}
