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

pdfcropのマージン(--margins)オプションを指定 (既定値は10)
```bash
$ ./pdf2eps.sh -m 20 -f hoge.pdf
```

出力フォルダを指定 (./hoge.pdf -> ./out/hoge.eps)
```bash
$ ./pdf2eps.sh -o ./out -f hoge.pdf
```

フォルダ内のファイルをバッチ処理 (./src/\*.pdf -> ./dst/\*.eps)
```bash
$ ./pdf2eps.sh -o ./src -b ./dst
```

## オプション使用時の順序

```bash
./pdf2eps [-n] [-m <margin>] [-o <out>] [-b|-f <target>]
```