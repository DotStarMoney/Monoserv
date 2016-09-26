// monoserv
// author: dc

package main

import (
	"encoding/json"
	"fmt"
	"golang.org/x/net/websocket"
	"log"
	"net/http"
)

type Position struct {
	X int `json:"x"`
	Y int `json:"y"`
}

type Projectile struct {
	Owner string   `json:"owner"`
	Pos   Position `json:"pos"`
}

type Frame struct {
	Player1Pos  Position     `json:"player1Pos"`
	Player2Pos  Position     `json:"player2Pos"`
	Projectiles []Projectile `json:"projectiles"`
}

func websocketHandler(ws *websocket.Conn) {
	msg := make([]byte, 512)
	n, err := ws.Read(msg)
	if err != nil {
		log.Fatal(err)
	}

	c := make(chan []Frame)

	go compile(c, msg[:n])
	for {
		frames, working := <-c
		f, err := json.Marshal(frames)
		if err != nil {
			fmt.Printf("Error parsing json: %s\n", err)
			ws.Close()
		}
		ws.Write([]byte(f))

		if !working {
			ws.Close()
		}
	}
}

func compile(c chan []Frame, wsMessage []byte) {
	// call some C functions because why not
	fmt.Printf("Calling C functions")
	PrintFromCFiles()
	// compute bogus frames
	frames := [256]Frame{}
	for i := 0; i < 4; i++ {
		for j := 0; j < 255; j++ {
			frames[j].Player1Pos = Position{99, 0}
			frames[j].Player2Pos = Position{0, 99}
			frames[j].Projectiles = []Projectile{}
		}
		c <- frames[:256]
		frames = [256]Frame{}
	}
	defer close(c)
}

func main() {
	PrintFromCFiles()
	http.Handle("/", websocket.Handler(websocketHandler))
	err := http.ListenAndServe(":8080", nil)
	if err != nil {
		panic("ListenAndServe: " + err.Error())
	}
}
