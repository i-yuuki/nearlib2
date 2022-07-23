#pragma once

#include <d3d11.h>

#include "asset.h"

namespace Near{

class Texture : public Asset{
public:
  ~Texture();
  void load(NearLib* lib, const std::string& path) override;
  ID3D11ShaderResourceView* getTextureView() const;
private:
  ID3D11ShaderResourceView* textureView = nullptr;
};

}
