#pragma once

#include <memory>
#include <string>

#include "window.h"
#include "renderer.h"
#include "input.h"

namespace Near{

struct InitParams{
  int width = 1280;
  int height = 720;
  bool resizable = true;
  std::string windowTitle;
};

extern Window window;
extern Renderer renderer;
extern InputManager input;

// NearLibを初期化し、ゲームウィンドウを表示します。
void init(const InitParams& params);

// ゲームウィンドウを閉じてNearLibを解放します。
void uninit();

// ウィンドウイベントを読み取ります。
// 毎フレーム、ゲームの更新前に呼ぶべきです。
void pollEvents();

// `markClose()`が呼ばれた、ウィンドウが閉じた、などで
// NearLibの終了が合図されているかを返します。
// ゲームループの条件式にでもしてください。
bool shouldClose();

// NearLibに終了を合図します。
// これを呼んでからは、`shouldClose()`がtrueを返します。
void markClose();

}
