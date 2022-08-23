#include "pch.h"
#include <NearLib/layer.h>

#include <NearLib/scene.h>

namespace Near{

Layer::ObjectCollecgtion::ObjectCollecgtion(Layer* layer) : layer(layer){
}

Layer::ObjectIterator Layer::ObjectCollecgtion::begin(){
  return layer->objectList.begin();
}

Layer::ObjectIterator Layer::ObjectCollecgtion::end(){
  return layer->objectList.end();
}

Layer::Layer() : objects(this){
}

void Layer::init(Scene* scene){
  this->scene = scene;
}

void Layer::beforeUpdate(float deltaTime){
  for(auto& obj : objectList){
    obj->beforeUpdate(deltaTime);
  }
}

void Layer::update(float deltaTime){
  for(auto& obj : objectList){
    obj->update(deltaTime);
  }

  {
    auto result = std::remove_if(objectList.begin(), objectList.end(), [](std::shared_ptr<GameObject>& obj){
      if(obj->isRemoveMarked()){
        obj->uninit();
        return true;
      }
      return false;
    });
    objectList.erase(result, objectList.end());
  }
}

void Layer::afterUpdate(float deltaTime){
  for(auto& obj : objectList){
    obj->afterUpdate(deltaTime);
  }
}

void Layer::draw(){
  for(auto& obj : objectList){
    obj->draw();
  }
}

void Layer::uninit(){
  for(auto& obj : objectList){
    obj->uninit();
  }
  objectList.clear();
}

Scene* Layer::getScene(){
  return scene;
}

void Layer::addGameObject(std::shared_ptr<GameObject> object){
  if(std::find(objectList.begin(), objectList.end(), object) != objectList.end()) return;
  objectList.push_back(object);
}

void Layer::removeGameObjectImmediate(std::shared_ptr<GameObject> object){
  auto it = std::find(objectList.begin(), objectList.end(), object);
  if(it == objectList.end()) return;
  objectList.erase(it);
}

}
