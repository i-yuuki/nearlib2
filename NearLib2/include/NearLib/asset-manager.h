#pragma once

#include <string>
#include <memory>

#include "near.h"

namespace Near{

// ファイルパスで素材を管理します。
// 種類ごとに限られた数のAssetManagerをプログラム全体で使うことで
// (各GameObjectに持たせるなどでいっぱい作ると効果が薄くなります)
// 同じファイルを複数回読み込まなくてよくなります。
// 読み込んだ素材はずっとメモリに残りますが、`unloadUnused()`を呼ぶことで
// 使われなくなった素材だけ好きなタイミングで解放できます。
template<class T>
class AssetManager{
public:
  AssetManager() = default;
  ~AssetManager() = default;
  // 読み込まれている素材を取得します。
  // @param path 素材のファイルパス
  // @returns 素材、なければnullptr
  std::shared_ptr<T> get(const std::string& path);
  // 素材を取得します。なければ読み込みます。
  // @param path 素材のファイルパス
  // @returns 素材
  std::shared_ptr<T> getOrLoad(const std::string& path);
  // 使われていない素材を解放します。
  void unloadUnused();
  // 全ての素材の管理をやめます。
  // 使われていない素材はこのタイミングで解放もされます。
  void clear();
private:
  std::unordered_map<std::string, std::shared_ptr<T>> assets;
};

#include "asset-manager.inl"

}
