def process_sections(file_path):
    # Dictionary to store sections and their corresponding items
    sections = {}
    current_section = ""

    # Open the input file for reading
    with open(file_path, "r") as file:
        # Process each line in the file
        for line in file:
            line = line.strip()

            # Check if the line is a section header (ends with ':')
            if line.endswith(":"):
                # Set the current section
                current_section = line
                # Ensure the section exists in the dictionary
                if current_section not in sections:
                    sections[current_section] = []
            else:
                # Add item to the current section if not already present
                if current_section and line not in sections[current_section]:
                    sections[current_section].append(line)

    # Print consolidated sections
    for section, items in sections.items():
        print(f"{section} {' '.join(items)}")


if __name__ == "__main__":
    # Specify the path to the input file
    input_file_path = "input.txt"
    process_sections(input_file_path)

