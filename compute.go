// monoserv compute package
// author: davethecust

package main

/*
#cgo CFLAGS: -I./c_src
#cgo LDFLAGS: -L./c_src -lcompute

#include <compute.h>

*/
import "C"

func PrintFromCFiles() {
	C.printPlayer1()
	C.printPlayer2()
}
