#include <chrono>

namespace Near{

class Clock{
public:
  static constexpr double FPS_UPDATE_INTERVAL = 500;
  Clock() = default;
  ~Clock() = default;
  // `frame()`が何回呼ばれたかを返します。
  uint64_t getTotalFrames() const;
  // 前回の`frame()`の、その前の1回からの経過時間を返します。
  double getDeltaTime() const;
  // `frame()`が呼ばれている回数の毎秒換算を返します。
  // `FPS_UPDATE_INTERVAL`msごとに、その間に呼ばれた回数から計算されます。
  double getFPS() const;
  void frame();
private:
  using clock = std::chrono::high_resolution_clock;
  clock::time_point timeLastFrame;
  double time = 0;
  double timeFPSUpdated = 0;
  double deltaTime = 0;
  double fps = 0;
  uint64_t totalFrames = 0;
  uint64_t framesSinceFPSUpdate = 0;
};

}
