// すごく雑にロガー扱うよ

#pragma once

#include <cstdio>
#include <Windows.h>

#include <NearLib/logger.h>

namespace Near::Internal{

class Logger{
public:
#if defined(_DEBUG)
  static void Init(){
    {
      AllocConsole();
      SetConsoleOutputCP(CP_UTF8);
      FILE* ignored = nullptr;
      freopen_s(&ignored, "CONOUT$", "w", stdout);
      freopen_s(&ignored, "CONIN$", "r", stdin);
    }
    Core = Near::Logger::Create("NearLib");
  }
  static void Uninit(){
    Core.reset();
  }
  static inline std::shared_ptr<spdlog::logger> Core;
#else
  static void Init(){}
  static void Uninit(){}
#endif
};

#if defined(_DEBUG)
  #define NEAR_LOG_INFO(...) ::Near::Internal::Logger::Core->info(__VA_ARGS__)
  #define NEAR_LOG_WARN(...) ::Near::Internal::Logger::Core->warn(__VA_ARGS__)
  #define NEAR_LOG_ERROR(...) ::Near::Internal::Logger::Core->error(__VA_ARGS__)
#else
  #define NEAR_LOG_INFO
  #define NEAR_LOG_WARN
  #define NEAR_LOG_ERROR
#endif

}
