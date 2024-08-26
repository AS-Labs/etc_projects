package main

import (
	"bufio"
	"fmt"
	"os"
	"strings"
)

type Section struct {
	name  string
	items map[string]bool
}

// findOrCreateSection checks if a section exists; if not, it creates a new one
func findOrCreateSection(sections map[string]*Section, name string) *Section {
	section, exists := sections[name]
	if !exists {
		section = &Section{
			name:  name,
			items: make(map[string]bool),
		}
		sections[name] = section
	}
	return section
}

func main() {
	// Open the input file for reading
	inputFilePath := "input.txt"
	file, err := os.Open(inputFilePath)
	if err != nil {
		fmt.Println("Error: Unable to open input file.")
		return
	}
	defer file.Close()

	// Initialize the sections map and a pointer to the current section
	sections := make(map[string]*Section)
	var currentSection *Section

	// Read lines from the input file
	scanner := bufio.NewScanner(file)
	for scanner.Scan() {
		line := strings.TrimSpace(scanner.Text())

		// Check if the line is a section header (ends with ':')
		if strings.HasSuffix(line, ":") {
			// Create or get the existing section
			currentSection = findOrCreateSection(sections, line)
		} else if currentSection != nil {
			// Add item to the current section if not already present
			if !currentSection.items[line] {
				currentSection.items[line] = true
			}
		}
	}

	// Check for errors during scanning
	if err := scanner.Err(); err != nil {
		fmt.Println("Error reading file:", err)
		return
	}

	// Print consolidated sections
	for _, section := range sections {
		fmt.Printf("%s ", section.name)
		for item := range section.items {
			fmt.Printf("%s ", item)
		}
		fmt.Println()
	}
}

