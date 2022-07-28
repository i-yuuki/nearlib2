#include "pch.h"
#include <NearLib/near.h>
#include <NearLib/input.h>
#include <NearLib/renderer.h>
#include <NearLib/window.h>
#include <NearLib/utils.h>

namespace Near{


NearLib::NearLib(){
}

NearLib::~NearLib(){
}

void NearLib::init(const InitParams& params){
  closeMarked = false;
  CoInitialize(nullptr);

  window = new Window();
  window->init(GetModuleHandleW(nullptr), params.width, params.height, params.windowTitle);
  window->setResizable(params.resizable);
  window->windowProc.addListener([this](UINT msg, WPARAM wParam, LPARAM lParam){
    if(msg == WM_DESTROY){
      markClose();
    }
    input->processMessage(msg, wParam, lParam);
  });

  input = new InputManager();
  input->init(this);

  renderer = new Renderer();
  renderer->init(this);
}

void NearLib::uninit(){
  safeUninitDelete(renderer);
  safeUninitDelete(input);
  safeUninitDelete(window);

  CoUninitialize();
}

void NearLib::pollEvents(){
  input->beforePollEvents();
  MSG msg = {};
  while(PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE)){
    TranslateMessage(&msg);
    DispatchMessageW(&msg);
  }
}

bool NearLib::shouldClose() const{
  return closeMarked;
}

void NearLib::markClose(){
  closeMarked = true;
}

InputManager& NearLib::getInput() const{
  return *input;
}

Window& NearLib::getWindow() const{
  return *window;
}

Renderer& NearLib::getRenderer() const{
  return *renderer;
}

}
