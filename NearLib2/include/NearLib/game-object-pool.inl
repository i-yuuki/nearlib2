template<class T>
void GameObjectPool<T>::init(Layer* layer, size_t initialCapacity){
  this->layer = layer;
  rawObjects = std::make_unique<T[]>(initialCapacity);
  objects.clear();
  objects.resize(initialCapacity);
  for(size_t i = 0;i < initialCapacity;i ++){
    objects[i] = std::shared_ptr<T>(&rawObjects[i], [](T* ptr){
      // custom deleter
      // rawObjectsがあとでdelete[]してくれるのでここではなにもしない
    });
  }
}

template<class T>
void GameObjectPool<T>::uninit(){
  // プール丸ごと解放するので use-after-free で落ちないように即消去
  for(auto object : objects){
    layer->removeGameObjectImmediate(object);
  }

  objects.clear();
  rawObjects.reset();
}

template<class T>
std::shared_ptr<T> GameObjectPool<T>::take(){
  for(auto& poolObject : objects){
    // layerがuninit()でnullptrになるのを利用して
    // オブジェクトがシーンに入ってないかチェック
    if(poolObject->getLayer() != nullptr) continue;
    initTakenObject(poolObject);
    return poolObject;
  }
  return nullptr;
}

template<class T>
std::shared_ptr<T> GameObjectPool<T>::takeOrCreate(){
  auto object = take();
  if(object == nullptr){
    object = std::make_shared<T>();
    initTakenObject(object);
    objects.push_back(object);
  }
  return object;
}

template<class T>
void GameObjectPool<T>::setOnTakeFromPool(std::function<void(std::shared_ptr<T>)> callback){
  onTakeFromPool = callback;
}

template<class T>
void GameObjectPool<T>::initTakenObject(std::shared_ptr<T> object){
  object->init(layer);
  layer->addGameObject(object);
  if(onTakeFromPool){
    onTakeFromPool(object);
  }
}
