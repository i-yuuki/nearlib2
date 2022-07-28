# NearLib 2

NearLib (にあーりぶ) は Windows ゲーム開発のスタートダッシュを少しだけ助けるかもしれない DirectX 11 グラフィックライブラリです。気づいたら作り直してました。

自分用ですが、謎に公開していたりいなかったり。

NearLib 1 は[ここ](https://github.com/i-yuuki/near/tree/gui)にあったりなかったり。

## この実験ブランチについて

密 (結合) です。

`GameObject` が `NearLib` ポインター、`NearLib` が `Renderer` ポインターを保持します。`Renderer` を自分で作って管理したり、それを `GameObject` に渡す方法で悩んだりする必要がなく、NearLib 1 みたいな使い方ができます。

before:

```cpp
// 頑張ってどっかから取得する場合
void MyGameObject::onDraw() override{
  MyGame::GetInstance()->getRenderer().drawMesh(vertexBuffer);
}
// 全オブジェクトでわざわざ渡す場合
class MyGameObject : public Near::GameObject{
public:
  MyGameObject(Near::NearLib* lib, Near::Renderer* renderer, ...) : Near::GameObject(), lib(lib), renderer(renderer){
  }
private:
  Near::NearLib* lib;
  Near::Renderer* renderer;
};
scene.getLayer(...)->createGameObject<MyGameObject>(&lib, &renderer, ...);
```

after:

```cpp
void MyGameObject::onDraw() override{
  lib->getRenderer().drawMesh(vertexBuffer);
}
```

## 特徴

- バッファ・テクスチャ・シェーダーを大胆にラップ
- イベント (???)
- キー & マウス入力
- [手抜き™](https://github.com/microsoft/DirectXTK)
- More coming soon! (たぶん)

## 材料

- Windows 10 April 2018 Update 以降
- Visual Studio 2017 以降
- C++17

## ビルド

`NearLib2/NearLib2.vcxproj` を Visual Studio で開いてx86ビルド (x64の時代なのに。ワンチャンいけるかも？)

## 下ごしらえ

- `NearLib2/include/` をインクルードパスに追加
- `<Debug|Release>/NearLib2.lib` を追加の依存ファイルに追加

## 使用イメージ

空のウィンドウ

```cpp
#include <Windows.h>
#include <NearLib/near.h>
int APIENTRY WinMain(HINSTANCE, HINSTANCE, LPSTR, int){
  // しょきか
  Near::NearLib lib;
  Near::NearLib::InitParams initParams{};
  initParams.width = 1200;
  initParams.height = 800;
  initParams.windowTitle = u8"🍅";
  lib.init(initParams);
  // げむるぷ
  while (!lib.shouldClose()){
    lib.pollEvents();
    lib.getWindow().clear(Near::Math::Color(0.9f, 0.9f, 0.9f, 1.0f));
    lib.getWindow().present();
  }
  // かたづけ
  lib.uninit();
  return 0;
}
```

## ドキュメント

各ヘッダーファイルにコメントで少し書いています。ドキュメントページは…作らないかもしれないです ¯\\\_(ツ)\_/¯
