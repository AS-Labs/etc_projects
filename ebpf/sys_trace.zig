const std = @import("std");

pub fn main() !void {
    const id: u64 = 123;

    const log_message = "syscall id: {}\n";
    try std.io.getStdOut().print(log_message, .{id});
}
