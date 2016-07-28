// monoserv compute package
// author: davethecust

package main

/*
#cgo CFLAGS: -I.
#cgo LDFLAGS: -L. -lcompute

#include "compute.h"
*/
import "C"

func main() {
	C.printPlayer1()
	//	C.printPlayer2()
}
