# pdf2eps

PDFファイルをクロップしてEPSファイルに変換する

## インストール

```bash
$ cd $HOME
$ sudo apt install make
$ sudo apt-get install libboost-all-dev
$ git clone https://github.com/ri0044ep/pdf2eps.git
$ sudo make install
```

## アンインストール
```bash
$ sudo rm /usr/local/bin/pdf2eps
```

## 使い方

使い方の表示
```bash
$ pdf2eps.sh --h
```

バージョンの表示
```bash
$ pdf2eps.sh --v
```

依存関係のインストール
```bash
$ pdf2eps.sh --i
```

出力フォルダを指定 (./hoge.pdf -> ./out/hoge.eps)
```bash
$ pdf2eps.sh -e -o ./out hoge.pdf
```

フォルダ内のファイルをバッチ処理 (./src/\*.pdf -> ./dst/\*.pdf)
```bash
$ pdf2eps.sh -b -o ./dst ./src
```

複数ページあるPDFファイルを分割して変換
```bash
$ pdf2eps.sh -d -o ./dst ./src.pdf
```

PDFcropのmarginsを指定
```bash
$ pdf2eps.sh --margins 10 10 10 10 ./src.pdf
```

## (--dオプション時のみ) 出力ファイル名を指定する

PDFファイルと同じ名前のテキストファイルを用意し，各ページに対して対応する出力ファイル名をページ順に改行して記載する． (拡張子不要)

ページ数よりもテキストファイルの記載数が少なければエラー．

多い分には無視する．
