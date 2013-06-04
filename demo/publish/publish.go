package main

import (
	"fmt"
	"flag"
	"os"
	"syscall"
	"os/signal"
)

/*
#cgo LDFLAGS: -lavformat -lavcodec -lavutil -lm -lz
extern void ffmpeg_init(int max);
extern int ffmpeg_publish(char* from, char* to);
extern void ffmpeg_release();
extern void ffmpeg_loop();
*/
import "C"

const (
	programName = "Publisher"
	version     = "0.0.1"
)

var (
	url        *string = flag.String("URL", "rtmp://192.168.20.111/vid3", "The rtmp url to connect.")
	streamName *string = flag.String("Stream", "camstream", "Stream name to play.")
	flvFile    *string = flag.String("FLV", "./sample.flv", "FLV filename to publish.")
)



func main() {
	flag.Usage = func() {
		fmt.Fprintf(os.Stderr, "%s version[%s]\r\nUsage: %s [OPTIONS]\r\n", programName, version, os.Args[0])
		flag.PrintDefaults()
	}
	flag.Parse()

	C.ffmpeg_init(100)
	defer C.ffmpeg_release()
	id := C.ffmpeg_publish(C.CString(*flvFile), C.CString(fmt.Sprintf("%s/%s", *url, *streamName)))
	if id == -1 {
		fmt.Println("Failed")
		os.Exit(-1)
	} else {
		fmt.Printf("OK\n")
	}
	go C.ffmpeg_loop()

	ch := make(chan os.Signal)
	signal.Notify(ch, syscall.SIGINT)
	<-ch
}

//export onPublished
func onPublished() {
	fmt.Println("Publish finished")
}
