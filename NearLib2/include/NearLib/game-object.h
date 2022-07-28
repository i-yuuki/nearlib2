#pragma once

#include "near.h"
#include "transform.h"

namespace Near{

class Layer;

class GameObject{
public:
  GameObject() = default;
  virtual ~GameObject();
  // オブジェクトを初期化します。
  // オブジェクトによっては素材の読み込みが行われます。
  // @param layer このオブジェクトを持つレイヤー
  void init(NearLib* lib, Layer* layer);
  // LayerまたはSceneによって、各オブジェクトの`update()`の前に呼ばれます。
  // @param deltaTime 前のフレームからの経過時間
  void beforeUpdate(float deltaTime);
  // オブジェクトを1フレーム進めます。
  // @param deltaTime 前のフレームからの経過時間
  void update(float deltaTime);
  // LayerまたはSceneによって、各オブジェクトの`update()`の後に呼ばれます。
  // @param deltaTime 前のフレームからの経過時間
  void afterUpdate(float deltaTime);
  // オブジェクトを描画します。
  void draw();
  // オブジェクトや使った素材などを解放します。
  void uninit();
  // このオブジェクトが追加されているレイヤーを返します。
  Layer* getLayer() const;
  // `markRemove()`で削除が合図されているかを返します。
  bool isRemoveMarked() const;
  // このオブジェクトの削除を合図します。
  // これを呼んでからは、`isRemoveMarked()`がtrueを返します。
  void markRemove();
  Transform transform{};
protected:
  NearLib* lib = nullptr;
  virtual void onInit(Layer* layer);
  virtual void onBeforeUpdate(float deltaTime);
  virtual void onUpdate(float deltaTime);
  virtual void onAfterUpdate(float deltaTime);
  virtual void onDraw();
  virtual void onUninit();
private:
  Layer* layer = nullptr;
  bool removeMarked = false;
};

}