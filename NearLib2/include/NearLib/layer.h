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
  void init(NearLib* lib, Scene* scene);
  void beforeUpdate(float deltaTime);
  void update(float deltaTime);
  void afterUpdate(float deltaTime);
  void draw();
  void uninit();
  Scene* getScene();
  template<class T, typename... Args>
  std::shared_ptr<T> createGameObject(Args&&... args){
    auto obj = std::make_shared<T>(std::forward<Args>(args)...);
    obj->init(lib, this);
    objectList.push_back(obj);
    return obj;
  }
protected:
  NearLib* lib = nullptr;
private:
  Scene* scene = nullptr;
  std::vector<std::shared_ptr<GameObject>> objectList;
};

}
