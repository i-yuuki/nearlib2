#include "pch.h"
#include <NearLib/input.h>

#include <hidusage.h>
#include <windowsx.h>

#include "internal/logger.h"

namespace Near{

void InputManager::init(Window* window){
  NEAR_LOG_INFO("Initializing input manager...");

  this->window = window;
  HWND windowHandle = window->getWindow();
  RAWINPUTDEVICE devices[2] = {};
  devices[0].usUsagePage = HID_USAGE_PAGE_GENERIC;
  devices[0].usUsage = HID_USAGE_GENERIC_KEYBOARD;
  devices[0].hwndTarget = windowHandle;
  devices[1].usUsagePage = HID_USAGE_PAGE_GENERIC;
  devices[1].usUsage = HID_USAGE_GENERIC_MOUSE;
  devices[1].hwndTarget = windowHandle;
  if(!RegisterRawInputDevices(devices, 2, sizeof(devices[0]))){
    throw std::exception("RegisterRawInputDevices failed");
  }

  NEAR_LOG_INFO("Input manager initialized!");
}

void InputManager::uninit(){
}

bool InputManager::isKeyDown(int key){
  return key >= 0 && key < BUTTON_COUNT && (buttons[key] == ButtonState::PRESSED || buttons[key] == ButtonState::DOWN);
}

bool InputManager::isKeyPressedThisFrame(int key){
  return key >= 0 && key < BUTTON_COUNT && buttons[key] == ButtonState::PRESSED;
}

int InputManager::getMouseX(){
  return mouseX;
}

int InputManager::getMouseY(){
  return mouseY;
}

int InputManager::getMouseMovementX(){
  return mouseMovementX;
}

int InputManager::getMouseMovementY(){
  return mouseMovementY;
}

bool InputManager::isMouseLocked(){
  return mouseLocked;
}

void InputManager::lockMouse(bool lock){
  if(mouseLocked == lock) return;
  mouseLocked = lock;
  SetCursor(mouseLocked ? NULL : (HCURSOR)GetClassLongW(window->getWindow(), GCL_HCURSOR));
}

void InputManager::beforePollEvents(){
  for(int i = 0;i < BUTTON_COUNT;i ++){
    if(buttons[i] == ButtonState::PRESSED){
      buttons[i] = ButtonState::DOWN;
    }else if(buttons[i] == ButtonState::RELEASED){
      buttons[i] = ButtonState::NOT_PRESSED;
    }
  }
  mouseMovementX = 0;
  mouseMovementY = 0;
  HWND windowHandle = window->getWindow();
  if(mouseLocked && windowHandle == GetActiveWindow()){
    RECT client = {};
    GetClientRect(windowHandle, &client);
    POINT pos = {client.right / 2, client.bottom / 2};
    ClientToScreen(windowHandle, &pos);
    SetCursorPos(pos.x, pos.y);
  }
}

bool InputManager::processMessage(UINT message, WPARAM wParam, LPARAM lParam){
  HWND windowHandle = window->getWindow();
  if(message == WM_INPUT){
    HRAWINPUT rawInput = reinterpret_cast<HRAWINPUT>(lParam);
    UINT size;
    GetRawInputData(rawInput, RID_INPUT, nullptr, &size, sizeof(RAWINPUTHEADER));
    std::vector<unsigned char> bytes(size, 0);
    GetRawInputData(rawInput, RID_INPUT, bytes.data(), &size, sizeof(RAWINPUTHEADER));
    RAWINPUT& input = *reinterpret_cast<RAWINPUT*>(bytes.data());
    if(input.header.dwType == RIM_TYPEKEYBOARD){
      int vkey = input.data.keyboard.VKey;
      if(input.data.keyboard.Flags & 1){ // LSB 1 = ?????????, 0 = ?????????
        buttons[vkey] = ButtonState::RELEASED;
        onKeyUp.fire({vkey, false});
      }else{
        bool repeat = isKeyDown(vkey); // ?????????????????????????????????????????????????????????????????????????????????
        buttons[vkey] = ButtonState::PRESSED;
        onKeyDown.fire({vkey, repeat});
      }
    }else if(input.header.dwType == RIM_TYPEMOUSE){
      if(input.data.mouse.usFlags & MOUSE_MOVE_ABSOLUTE){
        bool isVirtualDesktop = (input.data.mouse.usFlags & MOUSE_VIRTUAL_DESKTOP) == MOUSE_VIRTUAL_DESKTOP;
        int width = GetSystemMetrics(isVirtualDesktop ? SM_CXVIRTUALSCREEN : SM_CXSCREEN);
        int height = GetSystemMetrics(isVirtualDesktop ? SM_CYVIRTUALSCREEN : SM_CYSCREEN);
        POINT point = {
          static_cast<LONG>((input.data.mouse.lLastX / 65535.0f) * width),
          static_cast<LONG>((input.data.mouse.lLastY / 65535.0f) * height),
        };
        ScreenToClient(windowHandle, &point);
        // printf_s("%d / %d -> %d / %d\n", mouseX, mouseY, absoluteX, absoluteY);
        mouseMovementX += point.x - mouseX;
        mouseMovementY += point.y - mouseY;
        mouseX = point.x;
        mouseY = point.y;
      }else{
        mouseMovementX += input.data.mouse.lLastX;
        mouseMovementY += input.data.mouse.lLastY;
      }
      // ?????????VK_XBUTTON??????
      if(input.data.mouse.usButtonFlags & RI_MOUSE_BUTTON_1_DOWN){
        buttons[VK_LBUTTON] = ButtonState::PRESSED;
        onMouseDown.fire({0, mouseX, mouseY});
      }
      if(input.data.mouse.usButtonFlags & RI_MOUSE_BUTTON_1_UP){
        buttons[VK_LBUTTON] = ButtonState::RELEASED;
        onMouseUp.fire({0, mouseX, mouseY});
      }
      if(input.data.mouse.usButtonFlags & RI_MOUSE_BUTTON_2_DOWN){
        buttons[VK_RBUTTON] = ButtonState::PRESSED;
        onMouseDown.fire({1, mouseX, mouseY});
      }
      if(input.data.mouse.usButtonFlags & RI_MOUSE_BUTTON_2_UP){
        buttons[VK_RBUTTON] = ButtonState::RELEASED;
        onMouseUp.fire({1, mouseX, mouseY});
      }
      if(input.data.mouse.usButtonFlags & RI_MOUSE_BUTTON_3_DOWN){
        buttons[VK_MBUTTON] = ButtonState::PRESSED;
        onMouseDown.fire({2, mouseX, mouseY});
      }
      if(input.data.mouse.usButtonFlags & RI_MOUSE_BUTTON_3_UP){
        buttons[VK_MBUTTON] = ButtonState::RELEASED;
        onMouseUp.fire({2, mouseX, mouseY});
      }
    }
    return true;
  }else if(message == WM_MOUSEMOVE){
    mouseX = GET_X_LPARAM(lParam);
    mouseY = GET_Y_LPARAM(lParam);
    return true;
  }else if(message == WM_SETCURSOR){
    if(mouseLocked && windowHandle == GetActiveWindow()){
      SetCursor(NULL);
      return true;
    }
  }
  return false;
}

}
