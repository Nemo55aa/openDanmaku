# openDanmaku
弾幕を簡単に作るためのフレームワーク/ライブラリ
dxlib を使わせていただいています．（ライセンス表記はdxlibライセンス.txtにまとめました）
自分の好みの画像や音源ファイルを用いて弾幕を比較的簡単にかつある程度の自由度をもって作成できるようにしたかったので作りました．
いずれリファレンスのような形で使い方を整理する予定です．

・drawPixel.cpp
名前が変ですがこれがメインファイルです．ここに弾幕に関する主要な処理を書いていきます．

・bullets.h
N-way弾や狙い撃ち弾など，弾幕ゲームにはよくある/よく使いたい弾群についてはここにまとめています．

エラー・不具合，足りない機能など多くあると思いますが生々しく温かい目で見守る/助け舟をだしていただけるとありがたいです．
本職（ゲーム・プログラムなどは関係ない）の合間でやっているのでゆっくりとした反応だと思いますがよろしくお願いいたします．
# 導入
このgithubからDownloadZipすると，以下のようなファイルの入ったものをダウンロードできます．
![readmeImg1](https://github.com/Nemo55aa/openDanmaku/assets/147301350/564ff017-b3af-4a0b-bba0-203d8036ee14)
## danmakuTemplate_YYMMDDn
VisualStudio2019のソリューションフォルダをそのままコピーしたものをアップロードしています．  
プレイヤーや敵の画像，ステージの画像を例として入れています．
## NemoDanmakuTemplate.zip  
VS2019のプロジェクトのテンプレートです．こちらを  
\Documents\Visual Studio 2019\My Exported Templates  
においておけば，新規プロジェクトの作成時にDanmakuのプロジェクトを作成することができます．  
プロジェクトテンプレートから作成すると，/imgや/soundsなどのフォルダは作成されません．  
