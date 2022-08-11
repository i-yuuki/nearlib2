#pragma once

#include <unordered_set>

#include <Windows.h>

#include "window.h"
#include "event.h"

namespace Near{

enum class ButtonState{
  NOT_PRESSED,
  PRESSED,
  DOWN,
  RELEASED,
};

// キーとマウスの入力を管理します。
class InputManager{
public:
  struct KeyEvent{
    int vkey;
    bool isRepeat;
  };
  struct MouseEvent{
    // ボタン番号
    // (0 = 左クリック, 1 = 右クリック, etc)
    int button;
    int x;
    int y;
  };
  void init(Window* window);
  void uninit();
  // キーが押されているかを返します。
  // @param key 仮想キーコード
  // @returns 押されていたらtrue
  bool isKeyDown(int vkey);
  // 現在のフレームでキーがちょうど押されたかを返します。
  // Near::pollEvents()が低頻度で呼ばれていると、
  // キーが連打されたときに実際より少ない回数しか検出できないため、
  // キーイベントハンドラーを使うことも検討してください。
  // @param key 仮想キーコード
  bool isKeyPressedThisFrame(int vkey);
  // ウィンドウの左上を原点として現在のマウス位置を返します。
  int getMouseX();
  // ウィンドウの左上を原点として現在のマウス位置を返します。
  int getMouseY();
  int getMouseMovementX();
  int getMouseMovementY();
  bool isMouseLocked();
  void lockMouse(bool lock);
  // ウィンドウメッセージを見て入力状態を更新します。
  // @returns メッセージが処理されたかどうか (入力と関係ないウィンドウメッセージだったらfalse)
  bool processMessage(UINT message, WPARAM wParam, LPARAM lParam);
  Event::Signal<KeyEvent> onKeyDown;
  Event::Signal<KeyEvent> onKeyUp;
  Event::Signal<MouseEvent> onMouseDown;
  Event::Signal<MouseEvent> onMouseUp;
private:
  Window* window = nullptr;
  
  constexpr static int BUTTON_COUNT = 256;
  ButtonState buttons[BUTTON_COUNT];

  int mouseX;
  int mouseY;
  int mouseMovementX;
  int mouseMovementY;
  bool mouseLocked;

  friend void pollEvents();
  void beforePollEvents();
};

}
