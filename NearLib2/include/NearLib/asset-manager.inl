#pragma once

template<class T>
std::shared_ptr<T> AssetManager<T>::get(const std::string& path){
  auto it = assets.find(path);
  if(it == assets.end()) return nullptr;
  return it->second;
}

template<class T>
std::shared_ptr<T> AssetManager<T>::getOrLoad(const std::string& path){
  auto asset = get(path);
  if(asset) return asset;
  asset = std::make_shared<T>();
  try{
    asset->load(path);
  // }catch(const std::exception& ex){
  //   printf_s("%s failed to load asset! %s (%s)\n", typeid(*this).name(), ex.what(), path.c_str());
  }catch(...){
    // ロードエラー対応どうしよっか
    // nullptr返す？とりあえず例外投げとく
    throw;
  }
  assets.insert({path, asset});
  return asset;
}

template<class T>
void AssetManager<T>::unloadUnused(){
  for(auto it = assets.begin();it != assets.end();){
    // 参照カウント1 == this->assetsの中にしかいない == 不使用
    if(it->second.use_count() == 1){
      it = assets.erase(it);
    }else{
      it ++;
    }
  }
}
