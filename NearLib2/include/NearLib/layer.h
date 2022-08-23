#pragma once

#include "near.h"
#include "game-object.h"
#include "game-object-collection.h"

namespace Near{

class Scene;

class Layer{
public:
  using ObjectIterator = std::vector<std::shared_ptr<GameObject>>::iterator;
  class ObjectCollecgtion : public GameObjectCollection<ObjectIterator>{
  public:
    ObjectCollecgtion(Layer* layer);
    ObjectIterator begin() override;
    ObjectIterator end() override;
  private:
    Layer* layer;
  };
  ObjectCollecgtion objects;
  Layer();
  void init(Scene* scene);
  void beforeUpdate(float deltaTime);
  void update(float deltaTime);
  void afterUpdate(float deltaTime);
  void draw();
  void uninit();
  Scene* getScene();
  template<class T, typename... Args>
  std::shared_ptr<T> createGameObject(Args&&... args){
    auto obj = std::make_shared<T>(std::forward<Args>(args)...);
    obj->init(this);
    objectList.push_back(obj);
    return obj;
  }
  // 初期化済みのオブジェクトをレイヤーに追加します。
  // 同じオブジェクトを複数のレイヤーに追加しないように気を付けなはれや！
  void addGameObject(std::shared_ptr<GameObject> object);
  // レイヤー内のオブジェクトをただちに消します。
  // オブジェクトの解放 (object.uninit) はご自身でおこなってください。
  // GameObject::onUpdate内などこのレイヤーがオブジェクトでループ中に呼ぶと壊れます。
  void removeGameObjectImmediate(std::shared_ptr<GameObject> object);
private:
  Scene* scene = nullptr;
  std::vector<std::shared_ptr<GameObject>> objectList;
};

}
