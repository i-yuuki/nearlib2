#include "pch.h"
#include <NearLib/clock.h>

namespace Near{

uint64_t Clock::getTotalFrames() const{
  return totalFrames;
}

double Clock::getDeltaTime() const{
  return deltaTime;
}

double Clock::getFPS() const{
  return fps;
}

void Clock::frame(){
  clock::time_point now = clock::now();
  deltaTime = totalFrames == 0 ? 0 : std::chrono::duration<double, std::milli>(now - timeLastFrame).count();
  time += deltaTime;
  totalFrames ++;
  framesSinceFPSUpdate ++;
  timeLastFrame = now;
  double timeSinceUpdate = time - timeFPSUpdated;
  if(timeSinceUpdate >= FPS_UPDATE_INTERVAL){
    fps = 1000 * framesSinceFPSUpdate / timeSinceUpdate;
    timeFPSUpdated = time;
    framesSinceFPSUpdate = 0;
  }
}

}
