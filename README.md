# libsdp

**libsdp** 是基于[sdptransform](https://github.com/ibc/libsdptransform)和[media-server-go](https://github.com/notedit/media-server-go)开发的c++版本。
  并提供了swig转换使用示例（wrapper中有java的转换例子）。
  

## Usage

git clone  https://github.com/shuinan/libsdp
git submodule init
git submodule update

更多请参考 Test和wrapper中的代码。

###wrapper
make
java -classpath ./ test