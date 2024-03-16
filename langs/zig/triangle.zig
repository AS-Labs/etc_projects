const std = @import("std");

const Width = 20;
const Height = 10;

pub fn main() void {
    var canvas = [_]u8{0} ** Height;
    const points = [_]Point{
        Point{1, 1},
        Point{Width / 2, Height - 2},
        Point{Width - 2, 1},
    };
    
    drawTriangle(&canvas, points);
    printCanvas(canvas);
}

fn drawTriangle(canvas: &[_]u8 ** Height, points: [_]Point), void {
    for (points) |p1, p2| {
        drawLine(canvas, p1, p2);
    }
}

fn drawLine(canvas: &[_]u8 ** Height, p1: Point, p2: Point) void {
    var dx = p2.x - p1.x;
    var dy = p2.y - p1.y;
    var steps = max(abs(dx), abs(dy));
    if (steps == 0) {
        return;
    }
    var xIncrement = f32(dx) / f32(steps);
    var yIncrement = f32(dy) / f32(steps);
    var x: f32 = p1.x;
    var y: f32 = p1.y;
    for (0 .. steps) |i| {
        canvas[p1.y + i * dy / steps][p1.x + i * dx / steps] = '*';
        x += xIncrement;
        y += yIncrement;
    }
}

fn max(a: int, b: int) int {
    if (a > b) {
        return a;
    }
    return b;
}

fn abs(x: int) int {
    if (x < 0) {
        return -x;
    }
    return x;
}

fn printCanvas(canvas: [_]u8 ** Height) void {
    const stdout = std.io.getStdOut().writer();
    for (canvas) |row| {
        for (row) |c| {
            try stdout.print(u8(c));
        }
        try stdout.print("\n");
    }
}

