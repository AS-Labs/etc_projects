use std::collections::HashMap;
use std::fs::File;
use std::io::{self, BufRead};
use std::path::Path;

fn main() -> io::Result<()> {
    // Define the path to the input file
    let path = "input.txt";

    // Open the input file
    let file = File::open(path)?;
    let reader = io::BufReader::new(file);

    // HashMap to store sections and their corresponding items
    let mut sections: HashMap<String, Vec<String>> = HashMap::new();
    let mut current_section = String::new();

    // Read the file line by line
    for line in reader.lines() {
        let line = line?;
        let trimmed_line = line.trim();

        // Check if the line is a section header (ends with ':')
        if trimmed_line.ends_with(':') {
            // Set the current section
            current_section = trimmed_line.to_string();
            // Ensure the section exists in the hashmap
            sections.entry(current_section.clone()).or_insert(Vec::new());
        } else if !current_section.is_empty() {
            // Add item to the current section if not already present
            let items = sections.get_mut(&current_section).unwrap();
            if !items.contains(&trimmed_line.to_string()) {
                items.push(trimmed_line.to_string());
            }
        }
    }

    // Print consolidated sections
    for (section, items) in &sections {
        print!("{} ", section);
        for item in items {
            print!("{} ", item);
        }
        println!();
    }

    Ok(())
}

