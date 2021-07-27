# !/bin/bash

VERSION=2.0.0

#####################
# 設定変更はここから 
#####################
# pdfcropのオプション引数
pdfcrop_args="--margins 10"

# pdftopsのオプション引数
pdftops_args="-eps"

# 出力の拡張子 ($out_eps? .eps : .pdf)
out_eps=true
#####################

# 使い方の表示
function usage(){
cat <<_EOT_
$(basename ${0}) is a tool for cropping PDF and convert to EPS file.

Usage:
    $(basename ${0}) [-v|-h|-i] | [-n|] [-o dir|] [-b|-f|-p target]

Examples:
    Crop & convert fuga.pdf to ./hoge/fuga.eps
        $(basename ${0}) -n -o ./hoge -f fuga.pdf

    Crop & convert ./fuga/*.pdf to ./hoge/*.eps
        $(basename ${0}) -o ./hoge -b ./fuga
    
    Crop & convert for each page ./fuga.pdf to ./hoge/%04d.eps
        $(basename ${0}) -o ./hoge -p fuga.pdf

options:
    -v               print this script's version
    -h               print this  
    -i               install pdfcrop, pdftops, and pdftk
    -n               NOT overwrite existing file(s)
    -o <dir>         output to the directory
    -b <target>      batch process all PDF files in the folder
    -f <target>      process target PDF file
    -p <target>      process target PDF file (with splitting each page)
_EOT_
}

# 依存関係のインストール
function install_commands(){
    echo "Installing texlive-extra-utils, poppler-utils, and pdftk"
    sudo add-apt-repository ppa:malteworld/ppa
    sudo apt update
    sudo apt install -y texlive-extra-utils poppler-utils pdftk
}

# バージョン表示
function print_version(){
    echo $(basename ${0}) $VERSION ", 2021/07/27 - Kota Yoshida" 
}

# コマンドが存在するか
function command_exists(){
    cmd_exist=true
    if !(type "pdfcrop" > /dev/null 2>&1); then
        cmd_exist=false
        echo "Command 'pdfcrop' not found, it can be installed with:"
        echo "pdf2eps -i"
    fi

    if !(type "pdftops" > /dev/null 2>&1); then
        cmd_exist=false
        echo "Command 'pdftops' not found, it can be installed with:"
        echo "pdf2eps -i"
    fi

    if !(type "pdftk" > /dev/null 2>&1); then
        cmd_exist=false
        echo "Command 'pdftk' not found, it can be installed with:"
        echo "pdf2eps -i"
    fi

    if !(${cmd_exist}); then
        exit 1
    fi
}

# 上書きチェック
function check_overwrite(){
    process_continue=true
    if $no_overwrite; then
        if [ -e $target_file ]; then
            process_continue=false
        fi
    fi
}

# バッチ処理
function batch(){
    for target_file in $target/*.pdf; do
        file
    done
}

# ファイル処理
function file(){
    # ファイル名のパース
    bname=`basename $target_file`
    dname=`dirname $target_file`
    fname=`echo $bname | sed 's/\.[^\.]*$//'`

    echo "Processing ${target_file} ..."
    check_overwrite  # 出力ファイルが存在するか，上書きしてもいいか確認

    if !($set_out); then
        out=$dname
    fi

    if $process_continue; then

        pdfcrop $pdfcrop_args $target_file tmp.pdf
        
        if $out_eps; then
            echo "eps output"
            pdftops $pdftops_args tmp.pdf $out/$fname.eps
        else
            echo "pdf output"
            cp tmp.pdf $out/$fname.pdf
        fi

        rm tmp.pdf  # 片付け
    
    else
        echo "${target_file} was not processed due to the file is exists."
    fi
}

function split_pdf(){
    if !($set_out); then
        out=`dirname $target_file`
        set_out=true
    fi

    mkdir -p ./tmp  # 作業フォルダ
    # PDFをページ毎に分割
    pdftk $target_file burst output ./tmp/file_%04d.pdf
    target="./tmp"
    batch
    rm -rf ./tmp  # 片付け
}

# フラグ
no_overwrite=false
set_out=false

# オプション対応
while getopts vhinm:o:b:f:p: opt; do
    case $opt in
        v)  # バージョン表示
            print_version
            exit 1
        ;;

        h)  # ヘルプ表示
            print_version
            usage
            exit 1
        ;;
        
        i)  # 依存関係のインストール
            install_commands
            exit 1
        ;;

        n)  # 上書き禁止
            no_overwrite=true
        ;;

        o)  # 出力先フォルダ
            out=$OPTARG
            set_out=true
            if [ ! -e $out ]; then
                mkdir -p $out
            fi
        ;;

        b)  # バッチ処理
            target=$OPTARG
            command_exists
            batch
            exit 1
        ;;

        f)  # ファイル処理
            target_file=$OPTARG
            command_exists
            file
            exit 1
        ;;

        p) # PDFをページ毎に分割して処理
            target_file=$OPTARG
            command_exists
            split_pdf
            exit 1
        ;;

    esac
done
