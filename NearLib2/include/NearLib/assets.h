#pragma once

#include <memory>

#include "asset-manager.h"

namespace Near{

class VertexShader;
class PixelShader;
class Texture;

namespace Assets{

extern std::unique_ptr<AssetManager<VertexShader>> vertexShaders;
extern std::unique_ptr<AssetManager<PixelShader>> pixelShaders;
extern std::unique_ptr<AssetManager<Texture>> textures;

}

}
