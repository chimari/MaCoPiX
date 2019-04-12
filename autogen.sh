#!/bin/sh
#  Shimaki Matsubara <shimaki@bb.mbn.or.jp> 1998/09/16
#                            Last modified: 1999/08/06
#
# autoconf/automake/gettext 自動判別 autogen.sh 
#
# バグ
#   1つのパッケージ内に複数の configure.ac が含まれるものには
#   対応していません。
#
#   古いバージョンの gettext 用のパッケージには対応していません。
#   gettext 0.10.35 以降が望ましいです。
#============================================================
# conf_flags 変数
#   configure スクリプトに渡したい開発用のフラグを指定します。
#   例:
#     --enable-maintainer-mode
#     --enable-compile-warnings
#     --enable-debug=yes
conf_flags=

# configure.ac が無ければ始まりません。
(test -f configure.ac) || 
{
  #echo "**Error**: You must write 'configure.ac', first."
  echo "**エラー**: 最初に 'configure.ac' を作成してください。"
  exit 1
}

FILE=`sed 's/^.*AC_INIT.*(\(.*\)).*$/\1/;t;d' configure.ac`
PACKAGE=`sed 's/^.*PACKAGE=\(.*\)$/\1/;t;d' configure.ac`
# gettext を使用するか判別します。
ALL_LINGUAS=`sed 's/^.*ALL_LINGUAS="\?\([^"]*\)"\?$/\1/;t;d' configure.ac`
# automake を使用するか判別します。
(test -f Makefile.am) && { ENABLE_AUTOMAKE=yes; }
# aclocal 用の include ディレクトリが指定されているか調べます。
MACRODIRS=`sed 's/^.*AM_ACLOCAL_INCLUDE(\(.*\)).*$/\1/;t;d' configure.ac`
DIE=0

# AC_INIT は必須です。
(test -f $FILE) ||
{
  #echo "**Error**: You must run this script in the top-level '$PACKAGE' directory"
  echo "**エラー**: '$PACKAGE' トップディレクトリでこのスクリプトを実行してください。"
  exit 1
}

# automake では、PACKAGE 名は必須です。
(test "$ENABLE_AUTOMAKE" = "yes") && (test -z "$PACKAGE") && 
{
  PACKAGE=`sed 's/^.*AM_INIT_AUTOMAKE.*(\([^,]*\),[^)]*).*$/\1/;t;d' configure.ac`
  (test -z "$PACKAGE") &&
  {
    #echo "**Error**: You must set PACKAGE variable to your package name in configure.ac"
    echo "**エラー**: 'configure.ac' 内に PACKAGE 変数をあなたのパッケージ名で定義してください。 "
    exit 1
  }
}


# autoconf がインストールされているか調べます。
(autoconf --version) < /dev/null > /dev/null 2>&1 ||
{
  #echo
  #echo "**Error**: You must have autoconf installed to compile '$PACKAGE'."
  #echo "Download the appropriate package for your distribution,"
  #echo "or get the source at ftp://ftp.gnu.org/pub/gnu/"
  echo
  echo "**エラー**: '$PACKAGE' をコンパイルするためには autoconf をインストールする必要があります。"
  echo "            あなたのディストリビューション用の autoconf をインストールするか"
  echo "            ftp://ftp.gnu.org/pub/gnu/ からソースを入手、コンパイルしてください。"
  echo "        例: (Debian GNU/Linux ユーザ用)"
  echo "            $ apt-get -fy install autoconf"
  DIE=1
}
# automake がインストールされているか調べます。
(test "$ENABLE_AUTOMAKE" = "yes") &&
{
  (automake --version) < /dev/null > /dev/null 2>&1 ||
  {
    #echo
    #echo "**Error**: You must have automake installed to compile '$PACKAGE'."
    #echo "Download the appropriate package for your distribution,"
    #echo "or get the source at ftp://ftp.gnu.org/pub/gnu/"
  echo
  echo "**エラー**: '$PACKAGE' をコンパイルするためには automake をインストールする必要があります。"
    echo "          あなたのディストリビューション用の automake をインストールするか"
    echo "          ftp://ftp.gnu.org/pub/gnu/ からソースを入手、コンパイルしてください。"
    echo "      例: (Debian GNU/Linux ユーザ用)"
    echo "          $ apt-get -fy install automake"
    DIE=1
  }
}
# gettext がインストールされているか調べます。
(grep "^AM_GNU_GETTEXT" configure.ac > /dev/null) &&
{
  (gettext --version) < /dev/null > /dev/null 2>&1 ||
  {
    #echo
    #echo "**Error**: You must have gettext installed to compile '$PACKAGE'."
    #echo "           Download the appropriate package for your distribution,"
    #echo "           or get the source at ftp://ftp.gnu.org/pub/gnu/"
    echo
    echo "**エラー**: '$PACKAGE' をコンパイルするためには gettext をインストールする必要があります。"
    echo "            あなたのディストリビューション用の gettext をインストールするか"
    echo "            ftp://ftp.gnu.org/pub/gnu/ からソースを入手、コンパイルしてください。"
    echo "        例: (Debian GNU/Linux ユーザ用)"
    echo "            $ apt-get -fy install gettext"
    DIE=1
  }
}
(grep "^AM_GNOME_GETTEXT" configure.ac > /dev/null) &&
{
  (gettext --version) < /dev/null > /dev/null 2>&1 ||
  {
    #echo
    #echo "**Error**: You must have gettext installed to compile '$PACKAGE'."
    #echo "           Download the appropriate package for your distribution,"
    #echo "           or get the source at ftp://ftp.gnu.org/pub/gnu/"
    echo
    echo "**エラー**: '$PACKAGE' をコンパイルするためには gettext をインストールする必要があります。"
    echo "            あなたのディストリビューション用の gettext をインストールするか"
    echo "            ftp://ftp.gnu.org/pub/gnu/ からソースを入手、コンパイルしてください。"
    echo "        例: (Debian GNU/Linux ユーザ用)"
    echo "            $ apt-get -fy install gettext"
    DIE=1
  }
}

# libtool がインストールされているか調べます。
(grep "^AM_PROG_LIBTOOL" configure.ac > /dev/null) &&
{
  (libtool --version) < /dev/null > /dev/null 2>&1 ||
  {
    #echo
    #echo "**Error**: You must have libtool installed to compile '$PACKAGE'."
    #echo "Download the appropriate package for your distribution,"
    #echo "or get the source at ftp://ftp.gnu.org/pub/gnu/"
    echo
    echo "**エラー**: '$PACKAGE' をコンパイルするためには libtool をインストールする必要があります。"
    echo "            あなたのディストリビューション用の libtool をインストールするか"
    echo "            ftp://ftp.gnu.org/pub/gnu/ からソースを入手、コンパイルしてください。"
    echo "        例: (Debian GNU/Linux ユーザ用)"
    echo "            $ apt-get -fy install libtool"
    DIE=1
  }
}

(test "$DIE" -eq 1) && exit 1

(test -z "$*") &&
{
    #echo "**Warning**: I am going to run ./configure with no arguments."
    #echo "             if you wish to pass any to it, please specify them on the "
    #echo "             '$0' command line."
    echo "**警告**: ./configure を引数無しで実行します。"
    echo "          もし引数を指定したいのであれば、"
    echo "          '$0' の引数に指定してください。"
}

#echo processing...
echo 処理中...

# 依存関係を記述したディレクトリがあるときは削除する。
files=`find -name .deps`
#echo "Cleaning .deps/ directories ..."
echo "削除中... .deps/ ディレクトリ ..."
for d in $files; do
  echo -n "   $d"
  if test -d $d; then
    if (rm -r $d); then
      #echo "...removed...OK"
      echo "...削除...完了"
    else
      #echo "...failed to remove...Why..."
      echo "...削除...失敗...なぜ..."
    fi
  else
    #echo "...not found...Why..."
    echo "...見失いました...なぜ..."
  fi
done

# automake に必要なファイルが無い場合はそれを automake へ伝える。
#
# NEWS README AUTHORS ChangeLog のどれも見つからない場合は、
# --foreign を automake へ渡す。
#
requires="NEWS README AUTHORS ChangeLog"
#echo -n "Checking document files ..."
echo -n "確認中... ドキュメントファイル ..."
req_flag=0
for f in $requires; do
  test -f $f && test -s $f && req_flag=1
done

if test "$req_flag" -eq 1 ; then
  echo ""
  for f in $requires; do
    if test ! -f $f; then
      #echo "Creating $f..."
      echo "作成中... $f..."
      touch $f
    elif test ! -s $f; then
      #echo "Checking $f ...empty...should be written...OK?"
      echo "確認中... $f... 空です。必ず中身を作成してください"
    else
      #echo "Checking $f ...OK"
      echo "確認中... $f...完了"
    fi
  done
else
  #echo " skiped"
  echo " 処理を飛ばします"
  automake_flags="--foreign"
fi

# gettext 用の拡張部分
# 必要な .po ファイルをチェックします。
# po/POTFILES.in が見つからなければサンプルを作成します。
(test -n "$ALL_LINGUAS") &&
{
  ((test -d po) && (test -d intl)) || 
  {
    (test -r aclocal.m4) ||
    {
      #echo "Creating aclocal.m4 ..."
      echo "作成中... aclocal.m4 ..."
      touch aclocal.m4
    }
    #echo "Running gettextize..."    
    echo "実行中... gettextize..."    
    echo "no" | gettextize --force --copy
    (test -r aclocal.m4) && chmod u+w aclocal.m4
  }

  #echo "Checking po files ..."
  echo "確認中... po ファイル ..."
  for l in $ALL_LINGUAS; do
    po=po/$l.po
    if test ! -f $po; then
      #echo "Creating $po ..."
      echo "作成中... $po ..."
      touch $po
    else
      #echo "Checking $po ...OK"
      echo "確認中... $po ...完了"
    fi
  done

  # po/POTFILES.in のサンプルを作成します。
  (test ! -f po/POTFILES.in) && 
  {
    rm -f po/POTFILES.in.example
     # ソースファイルらしき、*.c ファイルを全て列挙します。
    # intl/, po/ ディレクトリ内のソースは除去します。
    src=`find -type f -name '*.c' | sed '/\/intl\//d; /\/po\//d'`
    # ソースファイルの中から "_(" を検索します。
    # この文字列があれば...
    for f in $src; do
      (grep \_\( $f > /dev/null 2>&1) && echo $f >> po/POTFILES.in.example
    done
    # po/POTFILES.in.example が作成されなかったときは、
    # 空のファイルを作成します。
    if test ! -f po/POTFILES.in.example; then
      touch po/POTFILES.in.example
    fi
    cp po/POTFILES.in.example po/POTFILES.in
    #echo "**Warning**: po/POTFILES.in was generated automaticaly."
    echo "**警告**: po/POTFILES.in を自動的に作成しました。"
  }
}

# /usr/share/aclocal と /usr/local/share/aclocal が存在するときは、
# 上手につじつまをあわせます。
(test "$ENABLE_AUTOMAKE" = "yes") && 
  (test -d /usr/share/aclocal) && 
  (test -d /usr/local/share/aclocal) &&
{
  # ディレクトリ名はここで指定しています。
  macros=m4

  # aclocal の path を調べ、.m4 をどこから読むか調べます。
  if test -f /usr/local/bin/aclocal; then
    acl_path1=/usr/local/share/aclocal
    acl_path2=/usr/share/aclocal
  elif test -f /usr/bin/aclocal; then
    acl_path1=/usr/share/aclocal
    acl_path2=/usr/local/share/aclocal
  fi

  (test -n "$acl_path1") &&
  {
    # 他方のディレクトリにしかない .m4 は読みこまれないので、
    # $macros/ を作成し、そこへコピーします。
    echo ""

    MACRODIRS="$MACRODIRS $macros"
    pwd=`pwd`
    cd $acl_path2
    files=`find -type f -name '*.m4'`
    # aclocal がチェックしない .m4 を読みこむようにします。
    for f in $files; do
      # 同じファイル名が $acl_path1 にあれば、それは衝突するので
      # 飛ばします。
      ff=$acl_path1/$f
      if test ! -f $ff; then
        # $f は、aclocal が読みこまれない .m4 です。
	# ユーザが指定する $MACRODIRS の中にあるかどうか調べます。
	FLAG=0
	for d in $MACRODIRS; do
	  if test -f $pwd/$d/`basename $f`; then
	    FLAG=1
	    break
	  fi
	done
	if test $FLAG -eq 0; then
	  # $MACRODIRS の中にも見つからなかったのでコピーします。
	  mkdir $pwd/$macros > /dev/null 2>&1
          cp $f $pwd/$macros
          #echo "  " `basename $f` " ... imported from $acl_path2"
          echo "  " `basename $f` " ... $acl_path2 から取り込みました"
	else
	  #echo "  " `basename $f` " ... already imported"
	  echo "  " `basename $f` " ... 既に取り込まれています"
	fi
      fi
    done
    cd $pwd
  }
}

if grep "^AM_PROG_LIBTOOL" configure.ac > /dev/null; then
  #echo "Running libtoolize..."
  echo "実行中... libtoolize..."
  libtoolize --force --copy
fi

# ここから先、configure script までは、
# コマンドが1つでも失敗したら止まります。
#
# acinclude.m4 を読み込み、aclocal.m4 を作成する。
if test "$ENABLE_AUTOMAKE" = "yes"; then
  # aclocal に引数を指定して読みこませます。
  for d in $MACRODIRS; do
    if test -d $d; then
      aclocal_include="$aclocal_include -I $d"
    fi
  done

  # GNOME_INIT があるときは、gnome 用の m4 を読みこむ
  (grep "^GNOME_INIT" configure.ac > /dev/null) &&
  {
    (echo $ACLOCAL_FLAGS | grep "gnome" > /dev/null) ||
    (echo $aclocal_include | grep "gnome" > /dev/null) || 
    aclocal_include="$aclocal_include -I $acl_path1/gnome"
  }

  #echo "Running aclocal $ACLOCAL_FLAGS $aclocal_include..." &&
  echo "実行中... aclocal $ACLOCAL_FLAGS $aclocal_include..." &&
  aclocal $ACLOCAL_FLAGS $aclocal_include
fi &&
# configure.ac を読み込み、configure を作成する。
#echo "Running autoconf..." &&
echo "実行中... autoconf..." &&
autoconf &&
# acconfig.h を読み込み、config.h.in を作成する。
if grep "^AM_CONFIG_HEADER" configure.ac > /dev/null; then
  #echo "Running autoheader..." &&
  echo "実行中... autoheader..." &&
  autoheader
fi &&
# Makefile.am を読み込み、Makefile.in を作成する。
if test "$ENABLE_AUTOMAKE" = "yes"; then
  #echo "Running automake --add-missing $automake_flags..." &&
  echo "実行中... automake --add-missing $automake_flags..." &&
  automake --add-missing -c $automake_flags
fi &&
# configure の実行。autogen.sh スクリプトの引数がそのまま渡される。
# config.cache があるときは削除する。
rm -f config.cache &&
#echo "Running configure $conf_flags $@..." &&
echo "実行中... configure $conf_flags $@..." &&
./configure $conf_flags "$@" &&

echo &&
#echo "Now type 'make' to compile '$PACKAGE'." &&
echo "さあ、'$PACKAGE' を楽しむために 'make' を実行しよう。" &&

# gettext 用の拡張部分
(test -f po/POTFILES.in.example) &&
{
  #echo
  #echo "**Warning**: 'po/POTFILES.in' is probably generated by autogen.sh."
  #echo "If it is correct, You must check 'po/POTFILES.in'."
  echo
  echo "**警告**: 'po/POTFILES.in' はおそらく '$0' によって"
  echo "          自動的に作成されました。"
  echo "          必ず 'po/POTFILES.in' を確認してください。"
}

(test -f "po/${PACKAGE}.pot") &&
{
  echo
  for l in $ALL_LINGUAS; do
    po=po/$l.po
    if test ! -s $po; then
      #echo "**警告**: '$po' is empty. You have to edit this file from '$PACKAGE.pot'."
      echo "**警告**: '$po' が空です。"
      echo "          $PACKAGE.pot' から作成する必要があります。"
    fi
  done
}

exit 0

#------------------------------------------------------------
#
