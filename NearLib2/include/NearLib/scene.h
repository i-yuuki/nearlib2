#pragma once

#include <map>

#include "near.h"
#include "game-object-collection.h"
#include "layer.h"
#include "shader.h"
#include "texture.h"

namespace Near{

class Scene{
public:
  class ObjectIterator{
  public:
    ObjectIterator(Scene* scene, std::map<int, Layer>::iterator layer, Layer::ObjectIterator object);
    ObjectIterator operator++(int);
    ObjectIterator& operator++();
    bool operator!=(const ObjectIterator& other);
    std::shared_ptr<GameObject> operator*();
  private:
    Scene* scene;
    std::map<int, Layer>::iterator layer;
    Layer::ObjectIterator object;
    void advance();
  };
  class ObjectCollection : public GameObjectCollection<ObjectIterator>{
  public:
    ObjectCollection(Scene* scene);
    ObjectIterator begin() override;
    ObjectIterator end() override;
  private:
    Scene* scene;
  };
  static constexpr int LAYER_MANAGERS = 0;
  static constexpr int LAYER_OBJECTS = 100;
  static constexpr int LAYER_TRANSPARENT_OBJECTS = 200;
  static constexpr int LAYER_OVERLAY = 300;
  ObjectCollection objects;
  Scene();
  virtual ~Scene();
  void init();
  void update(float deltaTime);
  void draw();
  void uninit();
  // レイヤー番号からレイヤーを返します。
  // @return レイヤー。なかったら`nullptr`
  Layer* getLayer(int num);
  // この番号のレイヤーがなければ作ります。
  // @return 作られたレイヤー。レイヤーが既に存在していたらそれ。
  Layer* insertLayer(int num);
  // この番号のレイヤーがあれば、その中のすべてのオブジェクトとともに削除します。
  void deleteLayer(int num);
protected:
  std::map<int, Layer> layers;
  virtual void onInit();
  // `update()`によって、各レイヤーの一連の更新の前に呼ばれます。
  virtual void beforeUpdate(float deltaTime);
  // `update()`によって、各レイヤーの一連の更新が全て終わった後に呼ばれます。
  virtual void afterUpdate(float deltaTime);
  // `draw()`によって、各レイヤーの描画前に呼ばれます。
  virtual void beforeDraw();
  // `draw()`によって、各レイヤーの描画後に呼ばれます。
  virtual void afterDraw();
  virtual void onUninit();
};

}
