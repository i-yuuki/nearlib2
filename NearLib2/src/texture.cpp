#include "pch.h"
#include <NearLib/texture.h>

#include <DirectXTK/WICTextureLoader.h>
#include <NearLib/near.h>
#include <NearLib/utils.h>
#include <NearLib/window.h>

namespace Near{

void Texture::load(const std::string& path){
  HRESULT res = DirectX::CreateWICTextureFromFileEx(window.getDevice(), widen(path).c_str(), 0, D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE, 0, 0, DirectX::WIC_LOADER_IGNORE_SRGB, nullptr, &textureView);
  if(FAILED(res)) throwResult(std::string("Failed to load texture from file ").append(path).c_str(), res);
}

ID3D11ShaderResourceView* Texture::getTextureView() const{
  return textureView;
}

Texture::~Texture(){
  safeRelease(textureView);
}

}
