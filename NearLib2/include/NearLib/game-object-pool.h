#pragma once

#include <functional>
#include <memory>
#include <vector>

#include "layer.h"

namespace Near{

// ゲームオブジェクトをメモリープール的にまとめて確保し、かつ
// 同じシーンレイヤーで複数回使いまわせるようにするやつです。
// このプールが使えるオブジェクトの条件:
// ・コンストラクタ引数がないこと
// ・uninit後にもう一度initして正しく動作すること
template<class T>
class GameObjectPool{
public:
  GameObjectPool() = default;
  ~GameObjectPool() = default;
  void init(Layer* layer, size_t initialCapacity);
  void uninit();
  // 使えるオブジェクトをプールから探し、見つかれば初期化してレイヤーに追加します。
  // @returns プール内のオブジェクト、見つからなかったらnullptr
  std::shared_ptr<T> take();
  // 使えるオブジェクトをプールから探し、見つかれば初期化してレイヤーに追加します。
  // 見つからなかったらその場で新しく作ってプールに入れます。
  // @returns 取り出したor作ったオブジェクト
  std::shared_ptr<T> takeOrCreate();
  // オブジェクトがプールから取り出されたor作られた後に行う処理を足します。
  void setOnTakeFromPool(std::function<void(std::shared_ptr<T>)> callback);
private:
  void initTakenObject(std::shared_ptr<T> object);
  Layer* layer;
  std::unique_ptr<T[]> rawObjects;
  std::vector<std::shared_ptr<T>> objects;
  std::function<void(std::shared_ptr<T>)> onTakeFromPool;
};

#include "game-object-pool.inl"

}
