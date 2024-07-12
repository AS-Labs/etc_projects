package main

import "core:fmt"

const (
    width  = 20
    height = 10
)

type Point struct {
    x, y int
}

func drawTriangle(points [3]Point) [height][width]rune {
    var canvas [height][width]rune
    for y := 0; y < height; y++ {
        for x := 0; x < width; x++ {
            canvas[y][x] = ' '
        }
    }

    for i := 0; i < 3; i++ {
        p1 := points[i]
        p2 := points[(i+1)%3]
        drawLine(&canvas, p1, p2)
    }

    return canvas
}

func drawLine(canvas *[height][width]rune, p1, p2 Point) {
    dx := p2.x - p1.x
    dy := p2.y - p1.y
    steps := max(abs(dx), abs(dy))
    if steps == 0 {
        return
    }
    xIncrement := float64(dx) / float64(steps)
    yIncrement := float64(dy) / float64(steps)
    x := float64(p1.x)
    y := float64(p1.y)
    for i := 0; i < steps; i++ {
        canvas[int(y)][int(x)] = '*'
        x += xIncrement
        y += yIncrement
    }
}

func max(a, b int) int {
    if a > b {
        return a
    }
    return b
}

func abs(x int) int {
    if x < 0 {
        return -x
    }
    return x
}

func main() {
    points := [3]Point{
        {1, 1},
        {width / 2, height - 2},
        {width - 2, 1},
    }

    canvas := drawTriangle(points)

    for _, row := range canvas {
        for _, char := range row {
            fmt.Printf("%c", char)
        }
        fmt.Println()
    }
}

