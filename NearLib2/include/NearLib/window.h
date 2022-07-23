#pragma once

#include <string_view>

#include <Windows.h>
#include <d3d11.h>

#include "math.h"
#include "event.h"

namespace Near{

class Window{
public:
  Window();
  Window(const Window& copyFrom) = delete;
  ~Window();
  void init(HINSTANCE instance, int width, int height, const std::string_view title);
  void uninit();
  void clear(const Math::Color& color);
  void present();
  int getWidth() const;
  int getHeight() const;
  void setResizable(bool resizable);
  HWND getWindow() const;
  IDXGISwapChain* getSwapChain() const;
  ID3D11Device* getDevice() const;
  ID3D11DeviceContext* getDeviceContext() const;
  Event::Signal<UINT, WPARAM, LPARAM> windowProc;
private:
  int width = 0;
  int height = 0;
  HWND window = NULL;
	IDXGISwapChain* swapChain = nullptr;
  ID3D11Device* device = nullptr;
  ID3D11DeviceContext* deviceContext = nullptr;
  ID3D11RenderTargetView* renderTargetView = nullptr;
  ID3D11DepthStencilView* depthStencilView = nullptr;
  D3D_FEATURE_LEVEL featureLevel;
  void initDevice();
  void initRenderTargetView();
  void initDepthStencilView();
  void initViewport();
};

}
