#include "pch.h"
#include <NearLib/near.h>
#include <NearLib/input.h>
#include <NearLib/window.h>
#include <NearLib/utils.h>

namespace Near{

Window window;
Renderer renderer;
InputManager input;

bool closeMarked;

void init(const InitParams& params){
  closeMarked = false;
  CoInitialize(nullptr);

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
}

void uninit(){
  input.uninit();
  window.uninit();

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
