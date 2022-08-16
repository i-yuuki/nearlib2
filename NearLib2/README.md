# NearLib 2

NearLib (にあーりぶ) は Windows ゲーム開発のスタートダッシュを少しだけ助けるかもしれない DirectX 11 グラフィックライブラリです。気づいたら作り直してました。

自分用ですが、謎に公開していたりいなかったり。

NearLib 1 は[ここ](https://github.com/i-yuuki/near/tree/gui)にあったりなかったり。

## 特徴

- シングルトンスタイル
- バッファ・テクスチャ・シェーダーを大胆にラップ
- シーンとゲームオブジェクト
- イベント (???)
- キー & マウス入力
- そこら中にUTF-8
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
  Near::InitParams initParams{};
  initParams.width = 1200;
  initParams.height = 800;
  initParams.windowTitle = u8"🍅";
  Near::init(initParams);
  // げむるぷ
  while (!Near::shouldClose()){
    Near::pollEvents();
    Near::window.clear(Near::Math::Color(0.9f, 0.9f, 0.9f, 1.0f));
    Near::window.present();
  }
  // かたづけ
  Near::uninit();
  return 0;
}
```

## ドキュメント

各ヘッダーファイルにコメントで少し書いています。ドキュメントページは…作らないかもしれないです ¯\\\_(ツ)\_/¯
