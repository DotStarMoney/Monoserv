package game

type Position struct {
        x int
        y int
}

type Projectiles struct {
        owner string
        pos Position
}

type Frame struct {
        player1Pos Position
        player2Pos Position
        projectiles []Projectiles
}

