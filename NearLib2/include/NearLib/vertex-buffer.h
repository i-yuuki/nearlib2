#pragma once

#include <d3d11.h>

namespace Near{

// 任意の頂点型の頂点バッファを表します。
// 型情報いらないからテンプレートやだ、そんな時にどうぞ。
// それ以外の場合はVertexBuffer<T>をお使いください。
class VertexBufferBase{
public:
  virtual ~VertexBufferBase();
  // 頂点バッファを解放します。
  virtual void uninit();
  // @returns バッファの中身を変えられるかどうか
  bool isDynamic() const;
  unsigned int getStride() const;
  // @returns バッファに何頂点入るか
  unsigned int getVertexCount() const;
  ID3D11Buffer* getBuffer() const;
protected:
  bool dynamic = false;
  unsigned int stride = 0;
  unsigned int vertexCount = 0;
  ID3D11Buffer* buffer = nullptr;
  // バイト列を指定してバッファを作ります。
  void initRaw(bool dynamic, const void* data, size_t size);
  // バイト列を指定してバッファの中身を変えます。
  void setRaw(const void* data, size_t size);
private:
};

template<typename T>
class VertexBuffer : public VertexBufferBase{
public:
  virtual ~VertexBuffer();
  // 頂点バッファを作ります。
  // @param dynamic `set()`であとから中身を変えられるかどうか
  // @param data 頂点データ
  // @param count 頂点数
  // @throws `dynamic == false && data == nullptr`の場合
  void init(bool dynamic, const T* data, size_t count);
  // バッファの中身を変えます。
  // @param data 頂点データ
  // @param count 頂点数
  // @throws `isDynamic() == false`の場合
  void set(const T* data, size_t count);
};

#include "vertex-buffer.inl"

}
