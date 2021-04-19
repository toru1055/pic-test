# pic-test

## プッシュでラジオ体操
### 1. sin波で音楽再生
  - [x] オーディオアンプなし版
    - [x] 020 一定間隔でLEDを点滅させたい（MCCでタイマ設定）
    - [x] 075 MCCでオペアンプを設定して使いたい
    - [x] 101 正弦波を出力したい
  - [ ] オーディオアンプあり版

### 2. SPI通信でフラッシュメモリ読み書き
  - [x] 071 MCCでUSARTを設定して使いたい
    - [x] `$ screen /dev/tty.usbserial-0001 9600`
  - [x] 074 MCCでSPIを設定して使いたい
  - [x] 051 SPI接続の大容量フラッシュメモリを使いたい

### 3. フラッシュメモリから音声データを再生
  - [x] WAV形式のフォーマットを理解する
  - [x] `$ xxd 1164_8bit.wav | head`を見れば大体わかるが、データ部は80バイト目くらいからなので80バイトオフセットすれば良さそう
  - [x] [wav converter](https://audio.online-convert.com/convert-to-wav): 8bit モノラルの1000Hzに変換
  - [x] 104 WAVファイルをフラッシュメモリに書き込みたい
    - [x] windowsのteratermが最適。
  - [x] 103 WAVファイルの音楽を再生したい

### 4. ボタン割り込み制御：ボタンを押したら対応する音声ファイルを割り込み再生
- [x] Pin ManagerでIOC対象のPINを設定
- [x] Timer割り込みではなく、通常のDelayメソッドで音楽再生するメソッドを用意
- [x] Interrupt Handlerを実装して設定する
- [x] 1, 2番pinを押したら再生

### 5. 仕上げ
- [ ] ラジオ体操の音源を入れる
- [ ] PCBの設計

### 6. 音源をWAV形式で用意する
#### ffmpegをインストール
```
$ brew install homebrew-ffmpeg/ffmpeg/ffmpeg
```

#### mp3音源を連結する
```
$ for f in *.mp3; do echo "file '$f'" >> mylist.txt; done
$ ffmpeg -f concat -safe 0 -i mylist.txt -c copy output.mp3
```

#### MP3→WAV(8kHz,8bit,mono)へ変換
```
$ ffmpeg -i output.mp3 -ac 1 -ar 8000 -acodec pcm_u8 output.wav
```

## 音声ファイルのwifiアップロード
### 1. esp-wroom-02でアップロードされたファイルをSPI通信でフラッシュメモリ書き込み
