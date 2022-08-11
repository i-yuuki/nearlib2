#pragma once

#include <d3d11.h>

#include "index-buffer.h"
#include "math.h"
#include "shader.h"
#include "texture.h"
#include "vertex-buffer.h"

namespace Near{

class Window;

class Renderer{
public:
  Renderer();
  Renderer(const Renderer& copyFrom) = delete;
  ~Renderer();
  void init(Window* window);
  void uninit();
  Math::Matrix getWorldTransform();
  Math::Matrix getViewTransform();
  Math::Matrix getProjectionTransform();
  void setWorldTransform(Math::Matrix transform);
  void setViewTransform(Math::Matrix transform);
  void setProjectionTransform(Math::Matrix transform);
  void setVertexShader(const VertexShader& shader);
  void setPixelShader(const PixelShader& shader);
  void setShaders(const VertexShader& vertexShader, const PixelShader& pixelShader);
  void setTexture(const Texture& texture);
  void drawMesh(const VertexBufferBase& vertices);
  void drawMesh(const VertexBufferBase& vertices, const IndexBuffer& indices);
private:
  Window* window;
  ID3D11RasterizerState* rasterizerState = nullptr;
  ID3D11BlendState* blendState = nullptr;
  ID3D11DepthStencilState* depthStencilState = nullptr;
  ID3D11SamplerState* samplerState = nullptr;
  ID3D11Buffer* worldBuffer = nullptr;
  ID3D11Buffer* viewBuffer = nullptr;
  ID3D11Buffer* projectionBuffer = nullptr;
  Math::Matrix worldTransform;
  Math::Matrix viewTransform;
  Math::Matrix projectionTransform;
};

}
