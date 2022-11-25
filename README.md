# pdf2eps

PDFファイルをクロップしてEPSファイルに変換する

## インストール

```bash
$ cd $HOME
$ sudo apt install make
$ sudo apt-get install libboost-all-dev
$ git clone https://github.com/y0sh1d4/pdf2eps.git
$ sudo make install
```

## アンインストール
```bash
$ sudo rm /usr/local/bin/pdf2eps
```

## 使い方

使い方の表示
```bash
$ pdf2eps --h
```

バージョンの表示
```bash
$ pdf2eps --v
```

依存関係のインストール
```bash
$ pdf2eps --i
```

出力フォルダを指定 (./src.pdf -> ./out/src.eps)
```bash
$ pdf2eps --e --o ./out src.pdf
```

フォルダ内のファイルをバッチ処理 (./src/\*.pdf -> ./dst/\*.pdf)
```bash
$ pdf2eps --b --o ./dst ./src
```

複数ページあるPDFファイルを分割して変換
```bash
$ pdf2eps --d --o ./dst ./src.pdf
```

PDFcropのmarginsを指定
```bash
$ pdf2eps --margins 10 10 10 10 ./src.pdf
```

## (--dオプション時のみ) 出力ファイル名を指定する

PDFファイルと同じ名前のテキストファイルを用意し，各ページに対して対応する出力ファイル名をページ順に改行して記載する． (拡張子不要)
```bash
$ pdf2eps --d --r test.pdf
```

ページ数よりもテキストファイルの記載数が少なければエラー．

多い分には無視する．
