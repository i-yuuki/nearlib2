#include "pch.h"
#include <NearLib/shader.h>

#include <fstream>
#include <d3dcompiler.h>

#include <NearLib/near.h>
#include <NearLib/utils.h>

namespace Near{

VertexShader::~VertexShader(){
  safeRelease(shader);
  safeRelease(layout);
}

void VertexShader::load(const std::string& path){
  ID3DBlob* code = nullptr;
  ID3DBlob* errorMessages = nullptr;
  ID3D11ShaderReflection* reflection = nullptr;
  try{
    UINT compileFlags = 0;
    #ifdef _DEBUG
    compileFlags |= D3DCOMPILE_DEBUG;
    #endif
    HRESULT result = D3DCompileFromFile(widen(path).c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "vs_4_0", compileFlags, 0, &code, &errorMessages);
    if(errorMessages != nullptr){
      std::string errorStr(static_cast<char*>(errorMessages->GetBufferPointer()), errorMessages->GetBufferSize());
      OutputDebugStringW(widen(errorStr).c_str());
      // maybe do something with errorStr here
    }
    if(FAILED(result)){
      throwResult("Vertex shader compile failed", result);
    }

    auto* device = window.getDevice();
    result = device->CreateVertexShader(code->GetBufferPointer(), code->GetBufferSize(), nullptr, &shader);
    if(FAILED(result)) throwResult("CreateVertexShader failed", result);

    // シェーダーコードからInputLayoutを自動生成
    // https://takinginitiative.wordpress.com/2011/12/11/directx-1011-basic-shader-reflection-automatic-input-layout-creation/
    result = D3DReflect(code->GetBufferPointer(), code->GetBufferSize(), IID_ID3D11ShaderReflection, reinterpret_cast<void**>(&reflection));
    if(FAILED(result)) throwResult("D3DReflect failed", result);

    D3D11_SHADER_DESC shaderDesc;
    reflection->GetDesc(&shaderDesc);

    std::vector<D3D11_INPUT_ELEMENT_DESC> inputElements;
    for(UINT i = 0;i < shaderDesc.InputParameters;i ++){
      D3D11_SIGNATURE_PARAMETER_DESC paramDesc;
      reflection->GetInputParameterDesc(i, &paramDesc);
      DXGI_FORMAT elemFormat;
      if(paramDesc.Mask == 1){
        switch(paramDesc.ComponentType){
          case D3D_REGISTER_COMPONENT_UINT32:  elemFormat = DXGI_FORMAT_R32_UINT; break;
          case D3D_REGISTER_COMPONENT_SINT32:  elemFormat = DXGI_FORMAT_R32_SINT; break;
          case D3D_REGISTER_COMPONENT_FLOAT32: elemFormat = DXGI_FORMAT_R32_FLOAT; break;
          default: break; // ↑に当てはまらない場合あったらどうしよ
        }
      }else if(paramDesc.Mask <= 3){
        switch(paramDesc.ComponentType){
          case D3D_REGISTER_COMPONENT_UINT32:  elemFormat = DXGI_FORMAT_R32G32_UINT; break;
          case D3D_REGISTER_COMPONENT_SINT32:  elemFormat = DXGI_FORMAT_R32G32_SINT; break;
          case D3D_REGISTER_COMPONENT_FLOAT32: elemFormat = DXGI_FORMAT_R32G32_FLOAT; break;
          default: break;
        }
      }else if(paramDesc.Mask <= 7){
        switch(paramDesc.ComponentType){
          case D3D_REGISTER_COMPONENT_UINT32:  elemFormat = DXGI_FORMAT_R32G32B32_UINT; break;
          case D3D_REGISTER_COMPONENT_SINT32:  elemFormat = DXGI_FORMAT_R32G32B32_SINT; break;
          case D3D_REGISTER_COMPONENT_FLOAT32: elemFormat = DXGI_FORMAT_R32G32B32_FLOAT; break;
          default: break;
        }
      }else if(paramDesc.Mask <= 15){
        switch(paramDesc.ComponentType){
          case D3D_REGISTER_COMPONENT_UINT32:  elemFormat = DXGI_FORMAT_R32G32B32A32_UINT; break;
          case D3D_REGISTER_COMPONENT_SINT32:  elemFormat = DXGI_FORMAT_R32G32B32A32_SINT; break;
          case D3D_REGISTER_COMPONENT_FLOAT32: elemFormat = DXGI_FORMAT_R32G32B32A32_FLOAT; break;
          default: break;
        }
      }

      D3D11_INPUT_ELEMENT_DESC elem = {
        paramDesc.SemanticName,
        paramDesc.SemanticIndex,
        elemFormat,
        0,
        D3D11_APPEND_ALIGNED_ELEMENT,
        D3D11_INPUT_PER_VERTEX_DATA,
        0,
      };
      inputElements.push_back(elem);
    }

    result = device->CreateInputLayout(inputElements.data(), static_cast<UINT>(inputElements.size()), code->GetBufferPointer(), code->GetBufferSize(), &layout);
    if(FAILED(result)) throwResult("CreateInputLayout failed", result);
  }catch(...){
    safeRelease(code);
    safeRelease(errorMessages);
    safeRelease(reflection);
    throw;
  }
  safeRelease(code);
  safeRelease(errorMessages);
  safeRelease(reflection);
}

ID3D11VertexShader* VertexShader::getShader() const{
  return shader;
}

ID3D11InputLayout* VertexShader::getLayout() const{
  return layout;
}



PixelShader::~PixelShader(){
  safeRelease(shader);
}

void PixelShader::load(const std::string& path){
  ID3DBlob* code = nullptr;
  ID3DBlob* errorMessages = nullptr;
  try{
    UINT compileFlags = 0;
    #ifdef _DEBUG
    compileFlags |= D3DCOMPILE_DEBUG;
    #endif
    HRESULT result = D3DCompileFromFile(widen(path).c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "ps_4_0", compileFlags, 0, &code, &errorMessages);
    if(FAILED(result)){
      if(errorMessages != nullptr){
        std::string errorStr(static_cast<char*>(errorMessages->GetBufferPointer()), errorMessages->GetBufferSize());
        // maybe do something with errorStr here
      }
      throwResult("Pixel shader compile failed", result);
    }

    auto* device = window.getDevice();
    result = device->CreatePixelShader(code->GetBufferPointer(), code->GetBufferSize(), nullptr, &shader);
    if(FAILED(result)) throwResult("CreatePixelShader failed", result);
  }catch(...){
    safeRelease(code);
    safeRelease(errorMessages);
    throw;
  }
  safeRelease(code);
  safeRelease(errorMessages);
}

ID3D11PixelShader* PixelShader::getShader() const{
  return shader;
}

}
