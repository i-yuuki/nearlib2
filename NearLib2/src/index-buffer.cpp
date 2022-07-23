#include "pch.h"
#include <NearLib/index-buffer.h>
#include <NearLib/window.h>
#include <NearLib/utils.h>

#include <d3d11.h>

namespace Near{

IndexBuffer::~IndexBuffer(){
  uninit();
}

void IndexBuffer::init(NearLib* lib, bool dynamic, unsigned int indexCount, uint32_t* indices){
  if(!dynamic && !indices) throw std::invalid_argument("Indices must be non-null in non-dynamic IndexBuffer");

  this->lib = lib;
  this->dynamic = dynamic;
  this->indexCount = indexCount;
  size = sizeof(uint32_t) * indexCount;

  HRESULT result;

  D3D11_BUFFER_DESC bufferDesc{};
  bufferDesc.Usage = dynamic ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_DEFAULT;
  bufferDesc.ByteWidth = size;
  bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
  if(dynamic){
    bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
  }

  D3D11_SUBRESOURCE_DATA subresource = {};
  subresource.pSysMem = indices;

  result = lib->getWindow().getDevice()->CreateBuffer(&bufferDesc, indices ? &subresource : nullptr, &buffer);
  if(FAILED(result)) throwResult("CreateBuffer failed", result);
}

void IndexBuffer::uninit(){
  safeRelease(buffer);
}

unsigned int IndexBuffer::getIndexCount() const{
  return indexCount;
}

bool IndexBuffer::isDynamic() const{
  return dynamic;
}

ID3D11Buffer* IndexBuffer::getBuffer() const{
  return buffer;
}

void IndexBuffer::set(uint32_t* indices){
  if(!dynamic) throw std::exception("Can't call set() on non-dynamic buffer");

  HRESULT res;
  auto* ctx = lib->getWindow().getDeviceContext();

  D3D11_MAPPED_SUBRESOURCE resource;
  res = ctx->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
  if(FAILED(res)) throwResult("VertexBuffer Map failed", res);
  std::memcpy(resource.pData, indices, size);
  ctx->Unmap(buffer, 0);
}

}
