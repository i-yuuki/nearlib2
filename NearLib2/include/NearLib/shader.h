#pragma once

#include <d3d11.h>

#include "asset.h"

namespace Near{

class VertexShader : public Asset{
public:
  ~VertexShader();
  void load(const std::string& path) override;
  ID3D11VertexShader* getShader() const;
  ID3D11InputLayout* getLayout() const;
private:
  ID3D11VertexShader* shader = nullptr;
  ID3D11InputLayout* layout = nullptr;
};

class PixelShader : public Asset{
public:
  ~PixelShader();
  void load(const std::string& path) override;
  ID3D11PixelShader* getShader() const;
private:
  ID3D11PixelShader* shader = nullptr;
};

}
