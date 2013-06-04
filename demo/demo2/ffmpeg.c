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

typedef struct context {
	int              id;
	AVFormatContext *pFormatCtx;
	int              videoStream;
        
} context;

static struct context* g_contexts;
static int             g_max;
extern void onReceivedData(int id, int flags, int64_t pts, int64_t dts, void* data, int size);

struct context* GetContextByID(int id) {
	if(id <= 0 || id >g_max) {
		return NULL;
	}
	return &(g_contexts[id-1]);
}

struct context* NewContext() {
	int i;
	for(i = 0; i < g_max; i++) {
		if(g_contexts[i].id == 0) {
			g_contexts[i].id = i + 1;
			return &(g_contexts[i]);
		}
	}
	return NULL;
}

void DeleteContext(struct context* c) {
	if(c->pFormatCtx != NULL) {
		avformat_close_input(&(c->pFormatCtx));
	}
	memset(c, 0, sizeof(c));
}

void ffmpeg_init(int max) {
	g_max = max;
	int size = sizeof(struct context) * g_max;
	g_contexts = (struct context*)malloc(size);
	memset(g_contexts, 0, size);
        av_register_all();
	avformat_network_init();
}

void ffmpeg_release() {
	free(g_contexts);
	g_contexts = NULL;
}

int ffmpeg_open(char* url) {
	struct context* c;
	int i;
	c = NewContext();
	if(NULL == c)
		return -1;
	if(avformat_open_input(&(c->pFormatCtx), url, NULL, NULL) != 0) {
		goto err;
	}

	if(avformat_find_stream_info(c->pFormatCtx, NULL) < 0) {
		goto err;
	}

	av_dump_format(c->pFormatCtx, 0, url, 0);

	c->videoStream = -1;
	for(i = 0; i < c->pFormatCtx->nb_streams; i++) {
		if(c->pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) {
			c->videoStream = i;
			break;
		}
	}
	if(c->videoStream == -1) {
		goto err;
	}

	// Todo: codecs
	return c->id;
err:
	DeleteContext(c);
	return -1;
}

void ffmpeg_readloop(int id) {
	int i;
	struct context* c = GetContextByID(id);
	AVPacket packet;

	if(c == NULL){
		return;
	}

	while(av_read_frame(c->pFormatCtx, &packet) >= 0) {
		onReceivedData(id, packet.flags, packet.pts, packet.dts, packet.data, packet.size);
		av_free_packet(&packet);
	}
}

