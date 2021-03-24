# pic-test

## プッシュでラジオ体操
### 1. sin波で音楽再生
  - [x] オーディオアンプなし版
    - [x] 020 一定間隔でLEDを点滅させたい（MCCでタイマ設定）
    - [x] 075 MCCでオペアンプを設定して使いたい
    - [x] 101 正弦波を出力したい
  - [ ] オーディオアンプあり版

### 2. SPI通信でフラッシュメモリ読み書き
  - [ ] 071 MCCでUSARTを設定して使いたい
  - [ ] 074 MCCでSPIを設定して使いたい
  - [ ] 051 SPI接続の大容量フラッシュメモリを使いたい

### 3. フラッシュメモリから音声データを再生
  - [ ] WAV形式のフォーマットを理解する
  - [ ] [wav converter](https://online-audio-converter.com/ja/)
  - [ ] 104 WAVファイルをフラッシュメモリに書き込みたい
  - [ ] 103 WAVファイルの音楽を再生したい

### 4. ボタン割り込み制御：ボタンを押したら対応する音声ファイルを割り込み再生

## 音声ファイルのwifiアップロード
### 1. esp-wroom-02でアップロードされたファイルをSPI通信でフラッシュメモリ書き込み
