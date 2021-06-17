# !/bin/bash

VERSION=1.0.0

# 使い方の表示
function usage(){
cat <<_EOT_
$(basename ${0}) is a tool for cropping PDF and convert to EPS file.

Usage:
    $(basename ${0}) [-v|-h|-i] | [-n|] [-o dir|] [-m margins|] [-b|-f target]

Examples:
    $(basename ${0}) -n -o ./hoge -f fuga.pdf
    $(basename ${0}) -o ./hoge -b ./fuga

options:
    -v               print this script's version
    -h               print this  
    -i               install pdfcrop and pdftops
    -n               NOT overwrite existing file(s)
    -m <margins>     '--margins' option argument for pdfcrop
    -o <dir>         output to the directory
    -b <target>      batch process all PDF files in the folder
    -f <target>      process target PDF file
_EOT_
}

# 依存関係のインストール
function install_commands(){
    sudo apt install texlive-extra-utils
    sudo apt install poppler-utils
}

# バージョン表示
function print_version(){
    echo $(basename ${0}) $VERSION ", 2021/06/18 - Kota Yoshida" 
}

# コマンドが存在するか
function command_exists(){
    cmd_exist=true
    if !(type "pdfcrop" > /dev/null 2>&1); then
        cmd_exist=false
        echo "Command 'pdfcrop' not found, it can be installed with:"
        echo "sudo apt install texlive-extra-utils"
    fi

    if !(type "pdftops" > /dev/null 2>&1); then
        cmd_exist=false
        echo "Command 'pdftops' not found, it can be installed with:"
        echo "sudo apt install poppler-utils"
    fi

    if !(${cmd_exist}); then
        exit 1
    fi
}

# 上書きチェック
function check_overwrite(){
    process_continue=true
    if ($no_overwrite); then
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

    if ($process_continue); then

        pdfcrop --margins $margins $target_file tmp.pdf
        
        if ($set_out); then
            pdftops tmp.pdf $out/$fname.eps

        else
            pdftops tmp.pdf $dname/$fname.eps

        fi

        rm tmp.pdf  # 片付け
    
    else
        echo "${target_file} was not processed due to the file is exists."
    fi
}

# フラグ
no_overwrite=false
set_out=false
margins=10

# オプション対応
while getopts vhinm:o:b:f: opt; do
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

        m) # pdfcropの--marginsオプションにわたす引数
            margins=$OPTARG
        ;;

        o)  # 出力先フォルダ
            out=$OPTARG
            set_out=true
            if ( ! -e $out); then
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

    esac
done