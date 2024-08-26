const std = @import("std");

const Section = struct {
    name: []const u8,
    items: std.StringHashMap(bool),
};

fn findOrCreateSection(sections: *std.ArrayList(Section), name: []const u8) *Section {
    for (sections.items) |*section| {
        if (std.mem.eql(u8, section.name, name)) {
            return section;
        }
    }

    const section = Section{
        .name = name,
        .items = std.StringHashMap(bool).init(std.heap.page_allocator),
    };
    sections.append(section) catch std.debug.panic("Failed to append section.");
    return &sections.items[sections.items.len - 1];
}

pub fn main() !void {
    const allocator = std.heap.page_allocator;
    const stdin = std.io.getStdIn().reader();
    const stdout = std.io.getStdOut().writer();

    var sections = std.ArrayList(Section).init(allocator);
    defer sections.deinit();

    var current_section: ?*Section = null;

    // Buffer for reading lines
    var line_buffer: [1024]u8 = undefined;

    while (true) {
        const line = try stdin.readUntilDelimiterOrEof(&line_buffer, '\n');
        if (line.len == 0) break;

        const trimmed_line = std.mem.trim(u8, line, "\n\r ");

        // Check if the line is a section header (ends with ':')
        if (trimmed_line.len > 0 and trimmed_line[trimmed_line.len - 1] == ':') {
            current_section = findOrCreateSection(&sections, trimmed_line);
        } else if (current_section) |section| {
            if (!section.items.put(trimmed_line, true)) {
                std.debug.panic("Failed to add item to section.");
            }
        }
    }

    // Print consolidated sections
    for (sections.items) |section| {
        try stdout.print("{} ", .{section.name});
        for (section.items.keys()) |item| {
            try stdout.print("{} ", .{item});
        }
        try stdout.print("\n", .{});
    }
}
