#include "pch.h"
#include <NearLib/window.h>

#include <NearLib/near.h>
#include <NearLib/utils.h>
#include "internal/logger.h"

namespace Near{

LRESULT CALLBACK WindowProc(HWND window, UINT msg, WPARAM wParam, LPARAM lParam){
  if(msg == WM_CREATE){
    LPCREATESTRUCT pcs = reinterpret_cast<LPCREATESTRUCT>(lParam);
    SetWindowLongPtrW(window, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pcs->lpCreateParams));
    return 1;
  }
  Window* nearWindow = reinterpret_cast<Window*>(GetWindowLongPtrW(window, GWLP_USERDATA));
  if(nearWindow != nullptr){
    nearWindow->windowProc.fire(msg, wParam, lParam);
  }
  switch(msg){
    case WM_DESTROY:
      PostQuitMessage(0);
      return 0;
    default:
      return DefWindowProc(window, msg, wParam, lParam);
  }
}

Window::Window(){
}

Window::~Window(){
  uninit();
}

void Window::init(HINSTANCE instance, int width, int height, const std::string_view title){
  NEAR_LOG_INFO("Creating wnidow...");

  constexpr const wchar_t* windowClassName = L"NearGameWindow";
  DWORD windowStyle = WS_OVERLAPPEDWINDOW;

  this->width = width;
  this->height = height;

  // おなじみ クラス登録
  WNDCLASSW cls = {};
  cls.lpfnWndProc   = WindowProc;
  cls.lpszClassName = windowClassName;
  cls.cbWndExtra    = sizeof(LONG_PTR); // ウィンドウにNearLib*を持たせるのでメモリを確保
  cls.hInstance     = instance;
  cls.hCursor       = LoadCursorW(NULL, IDC_ARROW);
  cls.hbrBackground = (HBRUSH)(COLOR_BACKGROUND + 1); // ブラシオブジェクト or (HBRUSH)(COLOR_XXX + 1)
  RegisterClassW(&cls);

  // タイトルバーとかを含めたウィンドウサイズを求める
  RECT windowRect = {0, 0, static_cast<LONG>(width), static_cast<LONG>(height)};
  AdjustWindowRect(&windowRect, windowStyle, false);
  int windowWidth  = windowRect.right - windowRect.left;
  int windowHeight = windowRect.bottom - windowRect.top;

  // work area (タスクバーを除いた画面サイズ) の真ん中に配置
  // デスクトップの真ん中だと画面とウィンドウのサイズが近い
  // (たとえば画面1366x768 & ウィンドウ1280x720)
  // とウィンドウ下端がタスクバーに埋まるため
  RECT workRect;
  SystemParametersInfoW(SPI_GETWORKAREA, 0, &workRect, 0);
  int workWidth  = workRect.right - workRect.left;
  int workHeight = workRect.bottom - workRect.top;
  int windowX =             (workWidth  - windowWidth)  / 2;
  int windowY = std::max(0, (workHeight - windowHeight) / 2);

  window = CreateWindowW(
    windowClassName,
    widen(title).c_str(),
    windowStyle,
    windowX,
    windowY,
    windowWidth,
    windowHeight,
    NULL,
    NULL,
    instance,
    this
  );

  if(window == NULL){
    throw std::exception("CreateWindow failed");
  }

  ShowWindow(window, SW_NORMAL);
  UpdateWindow(window);

  NEAR_LOG_INFO("Window created, initializing DirectX...");

  initDevice();
  initRenderTargetView();
  initDepthStencilView();
  deviceContext->OMSetRenderTargets(1, &renderTargetView, depthStencilView);
  initViewport();

  NEAR_LOG_INFO("DirectX ready!");
}

void Window::initDevice(){
  HRESULT result;

  DXGI_SWAP_CHAIN_DESC swapChainDesc{};
  swapChainDesc.BufferCount = 1;
  swapChainDesc.BufferDesc.Width = width;
  swapChainDesc.BufferDesc.Height = height;
  swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
  swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
  swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
  swapChainDesc.OutputWindow = window;
  swapChainDesc.SampleDesc = {1, 0};
  swapChainDesc.Windowed = true;

  UINT createDeviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
  #ifdef _DEBUG
  createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
  #endif

  result = D3D11CreateDeviceAndSwapChain(
    nullptr,
    D3D_DRIVER_TYPE_HARDWARE,
    NULL,
    createDeviceFlags,
    nullptr,
    0,
    D3D11_SDK_VERSION,
    &swapChainDesc,
    &swapChain,
    &device,
    &featureLevel,
    &deviceContext
  );
  if(FAILED(result)) throwResult("CreateDeviceAndSwapChain failed", result);
}

void Window::initRenderTargetView(){
  HRESULT result;
  
  ID3D11Texture2D* renderTargetTexture = nullptr;
  result = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&renderTargetTexture));
  if(FAILED(result)) throwResult("GetBuffer failed", result);
  result = device->CreateRenderTargetView(renderTargetTexture, nullptr, &renderTargetView);
  renderTargetTexture->Release();
  if(FAILED(result)) throwResult("CreateRenderTargetView failed", result);
}

void Window::initDepthStencilView(){
  HRESULT result;
  
  ID3D11Texture2D* depthStencilTexture = nullptr;
  D3D11_TEXTURE2D_DESC textureDesc{};
  textureDesc.Width = width;
  textureDesc.Height = height;
  textureDesc.MipLevels = 1;
  textureDesc.ArraySize = 1;
  textureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
  textureDesc.SampleDesc = {1, 0};
  textureDesc.Usage = D3D11_USAGE_DEFAULT;
  textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
  textureDesc.CPUAccessFlags = 0;
  textureDesc.MiscFlags = 0;
  result = device->CreateTexture2D(&textureDesc, NULL, &depthStencilTexture);
  if(FAILED(result)) throwResult("CreateTexture2D failed", result);

  D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc{};
  depthStencilViewDesc.Format = textureDesc.Format;
  depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
  depthStencilViewDesc.Flags = 0;
  result = device->CreateDepthStencilView(depthStencilTexture, &depthStencilViewDesc, &depthStencilView);
  depthStencilTexture->Release();
  if(FAILED(result)) throwResult("CreateDepthStencilView failed", result);
}

void Window::initViewport(){
  D3D11_VIEWPORT viewport{};
  viewport.Width = static_cast<FLOAT>(width);
  viewport.Height = static_cast<FLOAT>(height);
  viewport.MinDepth = 0.0f;
  viewport.MaxDepth = 1.0f;
  viewport.TopLeftX = 0;
  viewport.TopLeftY = 0;
  deviceContext->RSSetViewports(1, &viewport);
}

void Window::uninit(){
  safeRelease(swapChain);
  safeRelease(device);
  safeRelease(deviceContext);
  safeRelease(renderTargetView);
  safeRelease(depthStencilView);

  DestroyWindow(window);
  window = NULL;
}

void Window::clear(const Math::Color& color){
  deviceContext->ClearRenderTargetView(renderTargetView, color);
  deviceContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void Window::present(){
  swapChain->Present(1, 0);
}

int Window::getWidth() const{
  return width;
}

int Window::getHeight() const{
  return height;
}

void Window::setResizable(bool resizable){
  constexpr DWORD resizeStyles = WS_THICKFRAME | WS_MAXIMIZEBOX;
  auto style = GetWindowLongW(window, GWL_STYLE);
  SetWindowLongW(window, GWL_STYLE, resizable ? style | resizeStyles : style & ~resizeStyles);
}

HWND Window::getWindow() const{
  return window;
}

IDXGISwapChain* Window::getSwapChain() const{
  return swapChain;
}

ID3D11Device* Window::getDevice() const{
  return device;
}

ID3D11DeviceContext* Window::getDeviceContext() const{
  return deviceContext;
}

}
