#!/bin/bash

# Initialize variables
output=""
current_section=""

# Read the input file line by line
while IFS= read -r line; do
  # Check if the line ends with ':' indicating a new section
  if [[ "$line" =~ :$ ]]; then
    # If there's a current section being built, append it to the output
    if [[ -n "$current_section" ]]; then
      output+="$current_section"$'\n'
    fi
    # Start a new section
    current_section="$line"
  else
    # Append item to the current section
    current_section+=" $(echo $line | xargs)"
  fi
done < "input.txt"

# Append the last section to the output
if [[ -n "$current_section" ]]; then
  output+="$current_section"
fi

echo "$output"
