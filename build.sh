git submodule init
git submodule sync
git submodule update

cd vo-aacenc
autoreconf -ivfi || exit 1
./configure --prefix=/usr --disable-dependency-tracking --disable-shared --enable-static --with-pic || exit 1
make clean || exit 1
make install || exit 1
cd ..

#wget http://www.tortall.net/projects/yasm/releases/yasm-0.7.0.tar.gz || exit 1
#tar zxvf yasm-0.7.0.tar.gz
#cd yasm-0.7.0
#./configure --prefix=/usr || exit 1
#make clean || exit 1
#make install || exit 1

cd ffmpeg
./configure --prefix=/usr \
	--extra-cflags="-O3 -fpic  -fasm" \
	--disable-shared \
	--disable-everything \
	--disable-encoders \
	--disable-decoders \
	--disable-muxers \
	--disable-avdevice \
	--disable-avfilter \
	--disable-demuxers \
	--disable-debug \
	--disable-logging \
	--disable-doc \
	--disable-ffplay \
	--disable-ffmpeg \
	--disable-ffplay \
	--disable-ffprobe \
	--disable-ffserver \
	--enable-nonfree \
	--enable-version3 \
	--enable-memalign-hack \
	--enable-libvo-aacenc \
	--enable-protocol=tcp \
	--enable-hwaccel=h264_vaapi \
	--enable-hwaccel=h264_vaapi \
	--enable-hwaccel=h264_dxva2 \
	--enable-demuxer=h264 \
	--enable-decoder=h264 \
	--enable-parser=h264 \
	--enable-decoder=aac \
	--enable-decoder=flv \
	--enable-encoder=flv \
	--enable-demuxer=flv \
	--enable-muxer=flv \
	--enable-protocol=rtmp \
	--enable-protocol=rtmpt \
	--enable-decoder=nellymoser \
	--enable-hwaccels \
	--enable-avformat \
	--enable-avcodec \
	--enable-avresample \
	--enable-zlib || exit 1
make clean || exit 1
make install || exit 1

echo -e "\033[32mBuild success\033[0m"
exit 0


