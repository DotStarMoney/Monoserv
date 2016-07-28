// monoserv compute package
// author: davethecust

package compute

/*
 #cgo linux LDFLAGS -L. -lplayer1 -lplayer2
 #cgo windows LDFLAGS -L. -lplayer1.dll -lplayer2.dll

 #include "compute.h"

*/
import "C"

func compute() {
	C.printPlayer1()
	C.printPlayer2()
}
