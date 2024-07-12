const std = @import("std");
const Example = @import("example.zig").Example;
const raylib = @import("raylib");

pub const example = Example{
    .initFn = init,
    .updateFn = update,
};

fn init(_: std.mem.Allocator) !void {
    raylib.InitWindow(800, 450, "raylib [core] example - basic window");
    raylib.SetConfigFlags(.{ .FLAG_WINDOW_RESIZABLE = true });
    raylib.SetTargetFPS(60);
}

fn update(_: f32) !void {
    raylib.BeginDrawing();
    raylib.DrawText("Congrats! You created your first window!", 190, 200, 20, raylib.LIGHTGRAY);
    raylib.EndDrawing();
}
