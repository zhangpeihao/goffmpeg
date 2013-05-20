package main

import (
	"fmt"
)

/*
#cgo LDFLAGS: -lavcodec -lavformat -lavutil -lm
#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include <libavutil/avstring.h>
#include <libavutil/colorspace.h>
#include <libavutil/mathematics.h>
#include <libavutil/pixdesc.h>
#include <libavutil/imgutils.h>
#include <libavutil/dict.h>
#include <libavutil/parseutils.h>
#include <libavutil/samplefmt.h>
#include <libavutil/avassert.h>
#include <libavutil/time.h>
#include <libavformat/avformat.h>
#include <libavdevice/avdevice.h>
#include <libswscale/swscale.h>
#include <libavutil/opt.h>
#include <libavcodec/avfft.h>
#include <libswresample/swresample.h>

void ffmpeg_init() {
        avcodec_register_all();
}
*/
import "C"

func main() {
	C.ffmpeg_init()
	fmt.Printf("OK\n")
}

