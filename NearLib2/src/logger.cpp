#include "pch.h"
#include <NearLib/logger.h>

#include <spdlog/sinks/stdout_color_sinks.h>

namespace Near{

std::shared_ptr<spdlog::logger> Logger::Create(const std::string& name){
  auto logger = spdlog::stdout_color_mt(name, spdlog::color_mode::always);
  logger->set_pattern("%H:%M:%S.%e [%n] [%^%l%$] %v");
  return logger;
}

}
