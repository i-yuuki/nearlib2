#include "pch.h"
#include <NearLib/near.h>
#include <NearLib/input.h>
#include <NearLib/window.h>
#include <NearLib/utils.h>

#include "internal/logger.h"

namespace Near{

Window window;
Renderer renderer;
InputManager input;

bool closeMarked;

void init(const InitParams& params){
  closeMarked = false;
  CoInitialize(nullptr);

  Internal::Logger::Init();
  NEAR_LOG_INFO("Initializing NearLib...");

  window.init(GetModuleHandleW(nullptr), params.width, params.height, params.windowTitle);
  window.setResizable(params.resizable);
  window.windowProc.addListener([](UINT msg, WPARAM wParam, LPARAM lParam){
    if(msg == WM_DESTROY){
      markClose();
    }
    input.processMessage(msg, wParam, lParam);
  });

  input.init(&window);
  renderer.init(&window);

  NEAR_LOG_INFO("NearLib initialized!");
}

void uninit(){
  NEAR_LOG_INFO("Uninitializing NearLib...");

  input.uninit();
  window.uninit();

  Internal::Logger::Uninit();

  CoUninitialize();
}

void pollEvents(){
  input.beforePollEvents();
  MSG msg = {};
  while(PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE)){
    TranslateMessage(&msg);
    DispatchMessageW(&msg);
  }
}

bool shouldClose(){
  return closeMarked;
}

void markClose(){
  closeMarked = true;
}

}
