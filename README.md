# pdf2eps

PDFファイルをクロップしてEPSファイルに変換する

## 使い方

使い方の表示
```bash
$ ./pdf2eps.sh -h
```

バージョンの表示
```bash
$ ./pdf2eps.sh -v
```

依存関係のインストール
```bash
$ ./pdf2eps.sh -i
```

ファイルを処理 (./hoge.pdf -> ./hoge.eps)
```bash
$ ./pdf2eps.sh -f hoge.pdf
```

上書き禁止で処理 (バッチ処理の場合はそのファイルを無視)
```bash
$ ./pdf2eps.sh -n -f hoge.pdf
```

出力フォルダを指定 (./hoge.pdf -> ./out/hoge.eps)
```bash
$ ./pdf2eps.sh -o ./out -f hoge.pdf
```

フォルダ内のファイルをバッチ処理 (./src/\*.pdf -> ./dst/\*.eps)
```bash
$ ./pdf2eps.sh -o ./dst -b ./src
```

複数ページあるPDFファイルを分割して変換
```bash
$ ./pdf2eps.sh -o ./dst -p ./src.pdf
```

## オプション使用時の順序

```bash
./pdf2eps [-n] [-o <out>] [-b|-f|-p <target>]
```

## pdfcrop/pdftopsのオプションを変更したい場合

`pdf2eps.sh`を開き，`$pdfcrop_args`/`$pdftop_args`を編集

`$pdfcrop`の`--margins`として`10`を設定済み