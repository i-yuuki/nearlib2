#include "pch.h"
#include <NearLib/renderer.h>

#include <NearLib/near.h>
#include <NearLib/utils.h>

namespace Near{

Renderer::Renderer(){
}

Renderer::~Renderer(){
  uninit();
}

void Renderer::init(Window* window){
  this->window = window;

  HRESULT res;
  auto* device = window->getDevice();
  auto* deviceContext = window->getDeviceContext();

  D3D11_RASTERIZER_DESC rasterizerDesc{};
  rasterizerDesc.FillMode = D3D11_FILL_SOLID;
  rasterizerDesc.CullMode = D3D11_CULL_BACK;
  rasterizerDesc.FrontCounterClockwise = true;
  rasterizerDesc.DepthClipEnable = true;
  rasterizerDesc.MultisampleEnable = false;

  res = device->CreateRasterizerState(&rasterizerDesc, &rasterizerState);
  if(FAILED(res)) throwResult("CreateRasterizerState failed", res);
  deviceContext->RSSetState(rasterizerState);
  
  D3D11_BLEND_DESC blendDesc{};
  blendDesc.AlphaToCoverageEnable = false;
  blendDesc.IndependentBlendEnable = false;
  blendDesc.RenderTarget[0].BlendEnable = true;
  blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
  blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
  blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
  blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
  blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
  blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
  blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
  res = device->CreateBlendState(&blendDesc, &blendState);
  if(FAILED(res)) throwResult("CreateBlendState failed", res);
  {
    float blendFactor[4] = {0.0f, 0.0f, 0.0f, 0.0f};
    deviceContext->OMSetBlendState(blendState, blendFactor, 0xffffffff);
  }

  D3D11_DEPTH_STENCIL_DESC depthStencilDesc{};
  depthStencilDesc.DepthEnable = true;
  depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
  depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
  depthStencilDesc.StencilEnable = false;
  res = device->CreateDepthStencilState(&depthStencilDesc, &depthStencilState);
  if(FAILED(res)) throwResult("CreateDepthStencilState failed", res);
  deviceContext->OMSetDepthStencilState(depthStencilState, 0);

  D3D11_SAMPLER_DESC samplerDesc{};
  samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
  samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
  samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
  samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
  res = device->CreateSamplerState(&samplerDesc, &samplerStatePoint);
  if(FAILED(res)) throwResult("CreateSamplerState failed", res);

  samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
  res = device->CreateSamplerState(&samplerDesc, &samplerStateLinear);
  if(FAILED(res)) throwResult("CreateSamplerState failed", res);

  setTextureFiltering(textureFiltering); // デフォルトのフィルタリングをデバイスにセット

  // 座標変換をシェーダーに送る枠を作る

  D3D11_BUFFER_DESC bufferDesc{};
  bufferDesc.ByteWidth = sizeof(DirectX::SimpleMath::Matrix);
  bufferDesc.Usage = D3D11_USAGE_DEFAULT;
  bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
  bufferDesc.CPUAccessFlags = 0;
  bufferDesc.MiscFlags = 0;
  bufferDesc.StructureByteStride = sizeof(float);

  res = device->CreateBuffer(&bufferDesc, NULL, &worldBuffer);
  if(FAILED(res)) throwResult("CreateBuffer (world transform) failed", res);
  deviceContext->VSSetConstantBuffers(0, 1, &worldBuffer);

  res = device->CreateBuffer(&bufferDesc, NULL, &viewBuffer);
  if(FAILED(res)) throwResult("CreateBuffer (view transform) failed", res);
  deviceContext->VSSetConstantBuffers(1, 1, &viewBuffer);

  res = device->CreateBuffer(&bufferDesc, NULL, &projectionBuffer);
  if(FAILED(res)) throwResult("CreateBuffer (projection transform) failed", res);
  deviceContext->VSSetConstantBuffers(2, 1, &projectionBuffer);

  deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Renderer::uninit(){
  safeRelease(projectionBuffer);
  safeRelease(viewBuffer);
  safeRelease(worldBuffer);
  safeRelease(samplerStatePoint);
  safeRelease(samplerStateLinear);
  safeRelease(depthStencilState);
  safeRelease(blendState);
  safeRelease(rasterizerState);
}

DirectX::SimpleMath::Matrix Renderer::getWorldTransform(){
  return worldTransform;
}

DirectX::SimpleMath::Matrix Renderer::getViewTransform(){
  return viewTransform;
}

DirectX::SimpleMath::Matrix Renderer::getProjectionTransform(){
  return projectionTransform;
}

void Renderer::setWorldTransform(DirectX::SimpleMath::Matrix transform){
  worldTransform = transform;
  DirectX::SimpleMath::Matrix t = transform.Transpose();
  window->getDeviceContext()->UpdateSubresource(worldBuffer, 0, nullptr, &t, 0, 0);
}

void Renderer::setViewTransform(DirectX::SimpleMath::Matrix transform){
  viewTransform = transform;
  DirectX::SimpleMath::Matrix t = transform.Transpose();
  window->getDeviceContext()->UpdateSubresource(viewBuffer, 0, nullptr, &t, 0, 0);
}

void Renderer::setProjectionTransform(DirectX::SimpleMath::Matrix transform){
  projectionTransform = transform;
  DirectX::SimpleMath::Matrix t = transform.Transpose();
  window->getDeviceContext()->UpdateSubresource(projectionBuffer, 0, nullptr, &t, 0, 0);
}

TextureFiltering Renderer::getTextureFiltering(){
  return textureFiltering;
}

void Renderer::setTextureFiltering(TextureFiltering filtering){
  switch(filtering){
    case TextureFiltering::POINT:  window->getDeviceContext()->PSSetSamplers(0, 1, &samplerStatePoint); break;
    case TextureFiltering::LINEAR: window->getDeviceContext()->PSSetSamplers(0, 1, &samplerStateLinear); break;
    default: throw std::invalid_argument("Unknown texture filtering");
  }
  textureFiltering = filtering;
}

void Renderer::setVertexShader(const VertexShader& shader){
  window->getDeviceContext()->IASetInputLayout(shader.getLayout());
  window->getDeviceContext()->VSSetShader(shader.getShader(), nullptr, 0);
}

void Renderer::setPixelShader(const PixelShader& shader){
  window->getDeviceContext()->PSSetShader(shader.getShader(), nullptr, 0);
}

void Renderer::setShaders(const VertexShader& vertexShader, const PixelShader& pixelShader){
  setVertexShader(vertexShader);
  setPixelShader(pixelShader);
}

void Renderer::setTexture(const Texture& texture){
  auto view = texture.getTextureView();
  window->getDeviceContext()->PSSetShaderResources(0, 1, &view);
}

void Renderer::drawMesh(const VertexBufferBase& vertices, D3D11_PRIMITIVE_TOPOLOGY topology){
  auto buffer = vertices.getBuffer();
  unsigned int stride = vertices.getStride();
  unsigned int offset = 0;
  auto ctx = window->getDeviceContext();
  ctx->IASetVertexBuffers(0, 1, &buffer, &stride, &offset);
  ctx->IASetPrimitiveTopology(topology);
  ctx->Draw(vertices.getVertexCount(), 0);
}

void Renderer::drawMesh(const VertexBufferBase& vertices, const IndexBuffer& indices, D3D11_PRIMITIVE_TOPOLOGY topology){
  auto vertexBuffer = vertices.getBuffer();
  unsigned int stride = vertices.getStride();
  unsigned int offset = 0;
  auto ctx = window->getDeviceContext();
  ctx->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
  ctx->IASetIndexBuffer(indices.getBuffer(), DXGI_FORMAT_R32_UINT, 0);
  ctx->IASetPrimitiveTopology(topology);
  ctx->DrawIndexed(indices.getIndexCount(), 0, 0);
}

}
