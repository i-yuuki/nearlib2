#pragma once

#include <d3d11.h>

struct ID3D11Buffer;

namespace Near{

// 32ビットインデックスバッファです
class IndexBuffer{
public:
  ~IndexBuffer();
  // インデックスバッファを作ります。
  // @param dynamic `set()`であとから中身を変えられるかどうか
  // @param indexCount インデックス数
  // @throws `dynamic == false && indices == nullptr`の場合
  void init(bool dynamic, unsigned int indexCount, uint32_t* indices);
  // インデックスバッファを解放します。
  void uninit();
  unsigned int getIndexCount() const;
  // @returns `set()`でバッファの中身を変えられるかどうか
  bool isDynamic() const;
  ID3D11Buffer* getBuffer() const;
  // バッファの中身を変えます。
  // @throws `isDynamic() == false`の場合
  void set(uint32_t* indices);
private:
  bool dynamic = false;
  unsigned int indexCount = 0;
  unsigned int size = 0;
  ID3D11Buffer* buffer = nullptr;
};

}
