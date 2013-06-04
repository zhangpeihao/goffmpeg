package main

import (
	"fmt"
	"time"
	"unsafe"
)

/*
#cgo LDFLAGS: -lavformat -lavcodec -lavutil -lm -lz
extern void ffmpeg_init(int max);
extern int ffmpeg_open(char* url);
extern void ffmpeg_release();
extern void ffmpeg_readloop(int id);
*/
import "C"

func main() {
	C.ffmpeg_init(100)
	id := C.ffmpeg_open(C.CString("rtmp://192.168.20.111/vid3/sample"))
	if id == -1 {
		fmt.Println("Failed")
	} else {
		fmt.Printf("OK\n")
	}
	go C.ffmpeg_readloop(id)
	time.Sleep(20 * time.Second)
	C.ffmpeg_release()
}

//export onReceivedData
func onReceivedData(id C.int, flags C.int,  pts, dts C.long, data unsafe.Pointer, size C.int) {
	fmt.Printf("Received:\n\tid:\t%d\n\tflags:\t%d\n\tpts:\t%d\n\tdts:\t%d\n\tsize:\t%d\n", 
		int(id), int(flags), int64(pts), int64(dts), int(size))
	d := C.GoBytes(data, size)
	fmt.Printf("Data:\n\t% 02X\n", d[:16])
}
