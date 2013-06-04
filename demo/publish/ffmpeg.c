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

static struct AVIOContext *g_input;
static struct AVIOContext *g_output;
extern void onPublished();

void ffmpeg_init() {
	g_input = NULL;
	g_output = NULL;
        av_register_all();
	avformat_network_init();
}

void ffmpeg_release() {
	avformat_network_deinit();
}

int ffmpeg_publish(char* from, char* to) {
	int ret;
	char errbuf[50];

	if (g_input != NULL || g_output != NULL) {
		return -1;
	}
	
	// Open input stream
	ret = avio_open2(&g_input, from, AVIO_FLAG_READ, NULL, NULL);
	if (ret) {
		av_strerror(ret, errbuf, sizeof(errbuf));
		fprintf(stderr, "Unable to open input %s: %s\n", from, errbuf);
		return -1;
	}

	// Open output stream
	ret = avio_open2(&g_output, to, AVIO_FLAG_WRITE, NULL, NULL);
        if (ret) {
                av_strerror(ret, errbuf, sizeof(errbuf));
                fprintf(stderr, "Unable to open output %s: %s\n", to, errbuf);
		avio_close(g_input);
                return -1;
        }

	return ret;
}

void ffmpeg_loop() {
	uint8_t buf[1024];
	int n;

	while(1) {
		n = avio_read(g_input, buf, sizeof(buf));
		if (n <= 0)
			break;
		avio_write(g_output, buf, n);
		avio_flush(g_output);
		fprintf(stderr, "send %d bytes\n", n);
		av_usleep(20 * 1000);
	}
	onPublished();
}

