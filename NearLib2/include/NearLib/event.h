#pragma once

#include <functional>
#include <memory>

namespace Near::Event{

// イベントです。ほかの場所から追加されたコールバックをまとめて呼べます。
template<typename... CallbackArgs>
class Signal{
private:
  struct Handle;
public:
  using Callback = std::function<void(CallbackArgs...)>;
  // `Event`と追加されたコールバックをつなぐオブジェクトです。
  class Listener : public std::enable_shared_from_this<Listener>{
  public:
    Listener(std::weak_ptr<Handle> handle, Callback callback) : handle(handle), callback(callback){
    }
    // リスナーをイベントから削除します。
    void disconnect(){
      if(auto e = handle.lock()){
        e->listeners.erase(std::find(e->listeners.begin(), e->listeners.end(), this->shared_from_this()));
      }
      handle.reset();
    }
    Callback callback;
  private:
    std::weak_ptr<Handle> handle;
  };
  Signal(){
    handle = std::make_shared<Handle>();
  }
  Signal(const Signal& copyFrom) = delete;
  Signal& operator=(const Signal&) = delete;
  Signal& operator=(Signal&& moveFrom) = delete;
  // リスナーを追加します。
  std::shared_ptr<Listener> addListener(Callback callback){
    auto listener = std::make_shared<Listener>(handle, callback);
    handle->listeners.push_back(listener);
    return listener;
  }
  // 追加されたリスナーをすべて呼びます。
  void fire(CallbackArgs... args){
    for(auto listener : handle->listeners){
      listener->callback(args...);
    }
  }
private:
  struct Handle{
    std::vector<std::shared_ptr<Listener>> listeners;
  };
  std::shared_ptr<Handle> handle;
};

template<typename... CallbackArgs>
using ListenerPtr = std::shared_ptr<typename Signal<CallbackArgs...>::Listener>;

}
