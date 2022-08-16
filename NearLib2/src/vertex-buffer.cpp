#include "pch.h"
#include <NearLib/vertex-buffer.h>
#include <NearLib/near.h>
#include <NearLib/utils.h>

namespace Near{

VertexBufferBase::~VertexBufferBase(){
  uninit();
}

void VertexBufferBase::initRaw(bool dynamic, const void* data, size_t size){
  if(!dynamic && !data) throw std::invalid_argument("Data must be non-null in non-dynamic VertexBuffer");

  this->dynamic = dynamic;

  HRESULT result;

  D3D11_BUFFER_DESC bufferDesc{};
  bufferDesc.Usage = dynamic ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_DEFAULT;
  bufferDesc.ByteWidth = static_cast<unsigned int>(size);
  bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
  if(dynamic){
    bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
  }

  D3D11_SUBRESOURCE_DATA subresource = {};
  subresource.pSysMem = data;

  result = window.getDevice()->CreateBuffer(&bufferDesc, data ? &subresource : nullptr, &buffer);
  if(FAILED(result)) throwResult("CreateBuffer failed", result);
}

void VertexBufferBase::uninit(){
  safeRelease(buffer);
}

bool VertexBufferBase::isDynamic() const{
  return dynamic;
}

unsigned int VertexBufferBase::getStride() const{
  return stride;
}

unsigned int VertexBufferBase::getVertexCount() const{
  return vertexCount;
}

ID3D11Buffer* VertexBufferBase::getBuffer() const{
  return buffer;
}

void VertexBufferBase::setRaw(const void* data, size_t size){
  if(!dynamic) throw std::exception("Can't call set() on non-dynamic buffer");

  HRESULT res;
  auto* ctx = window.getDeviceContext();

  D3D11_MAPPED_SUBRESOURCE resource;
  res = ctx->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
  if(FAILED(res)) throwResult("VertexBuffer Map failed", res);
  std::memcpy(resource.pData, data, size);
  ctx->Unmap(buffer, 0);
}

}
