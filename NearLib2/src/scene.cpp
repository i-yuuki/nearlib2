#include "pch.h"
#include <NearLib/scene.h>

namespace Near{

Scene::ObjectIterator::ObjectIterator(Scene* scene, std::map<int, Layer>::iterator layer, Layer::ObjectIterator object) : scene(scene), layer(layer), object(object){
  advance();
}

Scene::ObjectIterator Scene::ObjectIterator::operator++(int){
  ObjectIterator ret = *this;
  this->operator++();
  return ret;
}

Scene::ObjectIterator& Scene::ObjectIterator::operator++(){
  object ++;
  advance();
  return *this;
}

bool Scene::ObjectIterator::operator!=(const ObjectIterator& other){
  if(scene != other.scene) return true;
  if(layer != other.layer) return true;
  if(object != other.object) return true;
  return false;
}

std::shared_ptr<GameObject> Scene::ObjectIterator::operator*(){
  return *object;
}

void Scene::ObjectIterator::advance(){
  // 今オブジェクトを指していなければオブジェクトがあるレイヤーまで進んで
  // 最初のオブジェクトを指す
  if(layer == scene->layers.end()) return;
  while(object == layer->second.objects.end()){
    layer ++;
    if(layer == scene->layers.end()) break;
    object = layer->second.objects.begin();
  }
}



Scene::ObjectCollection::ObjectCollection(Scene* scene) : scene(scene){
}

Scene::ObjectIterator Scene::ObjectCollection::begin(){
  auto itFirstLayer = scene->layers.begin();
  if(scene->layers.empty()) return ObjectIterator(scene, itFirstLayer, {});
  return ObjectIterator(scene, itFirstLayer, itFirstLayer->second.objects.begin());
}
Scene::ObjectIterator Scene::ObjectCollection::end(){
  if(scene->layers.empty()) return ObjectIterator(scene, scene->layers.end(), {});
  auto& lastLayer = scene->layers.rbegin()->second;
  return ObjectIterator(scene, scene->layers.end(), lastLayer.objects.end());
}



Scene::Scene() : objects(this){
}

Scene::~Scene(){
  uninit();
}

void Scene::init(){
  insertLayer(LAYER_MANAGERS);
  insertLayer(LAYER_OBJECTS);
  insertLayer(LAYER_TRANSPARENT_OBJECTS);
  insertLayer(LAYER_OVERLAY);
  onInit();
}

void Scene::update(float deltaTime){
  beforeUpdate(deltaTime);
  for(auto& it : layers){
    it.second.beforeUpdate(deltaTime);
  }
  for(auto& it : layers){
    it.second.update(deltaTime);
  }
  for(auto& it : layers){
    it.second.afterUpdate(deltaTime);
  }
  afterUpdate(deltaTime);
}

void Scene::draw(){
  beforeDraw();
  for(auto& it : layers){
    it.second.draw();
  }
  afterDraw();
}

void Scene::uninit(){
  onUninit();
  for(auto& it : layers){
    it.second.uninit();
  }
}

Layer* Scene::getLayer(int num){
  auto it = layers.find(num);
  return it == layers.end() ? nullptr : &it->second;
}

Layer* Scene::insertLayer(int num){
  // try_emplace:
  // 要素があったら<既存の要素, false>、なかったら挿入して<挿入した要素, true>のpairを返す
  auto [it, inserted] = layers.try_emplace(num);
  if(inserted){
    it->second.init(this);
  }
  return &it->second;
}

void Scene::deleteLayer(int num){
  auto it = layers.find(num);
  if(it == layers.end()) return;
  it->second.uninit();
  layers.erase(it);
}

void Scene::onInit(){
}

void Scene::beforeUpdate(float deltaTime){
}

void Scene::afterUpdate(float deltaTime){
}

void Scene::beforeDraw(){
}

void Scene::afterDraw(){
}

void Scene::onUninit(){
}

}
