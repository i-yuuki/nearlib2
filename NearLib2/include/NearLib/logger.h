#pragma once

#include <memory>
#include "spdlog/spdlog.h"

namespace Near{

class Logger{
public:
  static std::shared_ptr<spdlog::logger> Create(const std::string& name);
};

}
