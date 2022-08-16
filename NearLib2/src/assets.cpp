#include "pch.h"
#include <NearLib/assets.h>

namespace Near::Assets{

std::unique_ptr<AssetManager<VertexShader>> vertexShaders;
std::unique_ptr<AssetManager<PixelShader>> pixelShaders;
std::unique_ptr<AssetManager<Texture>> textures;

}
