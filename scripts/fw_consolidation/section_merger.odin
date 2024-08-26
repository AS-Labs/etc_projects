package main
import "core:fmt"
import "core:io"
import "core:strings"

Section :: struct {
    name: string,
    items: map[string]bool,
}

find_or_create_section :: proc(sections: ^[]Section, name: string) -> ^Section {
    for i in 0..len(*sections) {
        if strings.equal((*sections)[i].name, name) {
            return &(*sections)[i]
        }
    }

    section := Section{name = name, items = map.make[string, bool](allocator)}
    sections^.append(section)
    return &(*sections)[len(*sections) - 1]
}

main :: proc() {
    sections: []Section;
    current_section: ^Section = nil;

    // Open the input file for reading
    input_file_path := "input.txt";
    file, err := io.open(input_file_path);
    if err != nil {
        fmt.println("Error: Unable to open input file.");
        return;
    }
    defer file.close();

    // Read lines from the input file
    line_buffer: [1024]u8;
    for {
        n, read_err := file.read_line(&line_buffer);
        if read_err != nil || n <= 0 {
            break;
        }

        line := strings.trim_whitespace(string(line_buffer[..n]));

        // Check if the line is a section header (ends with ':')
        if strings.has_suffix(line, ":") {
            current_section = find_or_create_section(&sections, line);
        } else if current_section != nil {
            // Add item to the current section if not already present
            if !current_section.items[line] {
                current_section.items[line] = true;
            }
        }
    }

    // Print consolidated sections
    for section in sections {
        fmt.print("{} ", section.name);
        for item in section.items {
            fmt.print("{} ", item);
        }
        fmt.println("");
    }
}

