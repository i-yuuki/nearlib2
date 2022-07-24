#include "pch.h"
#include <NearLib/game-object.h>

namespace Near{

GameObject::~GameObject(){
  uninit();
}

void GameObject::init(Layer* layer){
  this->layer = layer;
  removeMarked = false;
  onInit(layer);
}

void GameObject::beforeUpdate(float deltaTime){
  onBeforeUpdate(deltaTime);
}

void GameObject::update(float deltaTime){
  onUpdate(deltaTime);
}

void GameObject::afterUpdate(float deltaTime){
  onAfterUpdate(deltaTime);
}

void GameObject::draw(){
  onDraw();
}

void GameObject::uninit(){
  onUninit();
  layer = nullptr;
}

void GameObject::onInit(Layer* layer){
}

void GameObject::onBeforeUpdate(float deltaTime){
}

void GameObject::onUpdate(float deltaTime){
}

void GameObject::onAfterUpdate(float deltaTime){
}

void GameObject::onDraw(){
}

void GameObject::onUninit(){
}

Layer* GameObject::getLayer() const{
  return layer;
}

bool GameObject::isRemoveMarked() const{
  return removeMarked;
}

void GameObject::markRemove(){
  removeMarked = true;
}

}
