CC="g++"
OPTIONS="-O2 -I/usr/include/X11/ -L/usr/lib/ -lX11 -lm"
VERSION="-std=c++14"

if [ -z $1 ] || [ -z $2 ]; then
  echo "./compile [tspShow.cpp] [comile_file]"
  exit 1
fi

$CC $VERSION $OPTIONS $1 $OPTIONS $2 -o enumTsp

chmod 777 enumTsp
