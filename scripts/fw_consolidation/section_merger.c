#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 10024

typedef struct {
  char *name;
  char **items;
  size_t item_count;
  size_t item_capacity;
} Section;

void add_item(Section *section, const char *item) {
  // Check if we need to expand the items array
  if (section->item_count >= section->item_capacity) {
    section->item_capacity *= 2;
    section->items =
        realloc(section->items, section->item_capacity * sizeof(char *));
    if (section->items == NULL) {
      perror("Failed to allocate memory");
      exit(EXIT_FAILURE);
    }
  }

  section->items[section->item_count] = strdup(item);
  section->item_count++;
}

int main() {
  const char *input_file_path = "input.txt";
  FILE *file = fopen(input_file_path, "r");
  if (file == NULL) {
    perror("Error opening file");
    return EXIT_FAILURE;
  }

  Section *sections = NULL;
  size_t section_count = 0;
  size_t section_capacity = 100;

  // Allocate initial memory for sections
  sections = malloc(section_capacity * sizeof(Section));
  if (sections == NULL) {
    perror("Failed to allocate memory for sections");
    fclose(file);
    return EXIT_FAILURE;
  }

  char line[MAX_LINE_LENGTH];
  Section *current_section = NULL;

  while (fgets(line, sizeof(line), file)) {
    // Remove newline character if present
    line[strcspn(line, "\n")] = '\0';

    // Check if the line is a section header (ends with ':')
    if (strchr(line, ':') && line[strlen(line) - 1] == ':') {
      // Trim the ':' from the section name
      line[strlen(line) - 1] = '\0';

      // Check if we need to expand the sections array
      if (section_count >= section_capacity) {
        section_capacity *= 2;
        sections = realloc(sections, section_capacity * sizeof(Section));
        if (sections == NULL) {
          perror("Failed to allocate memory for sections");
          fclose(file);
          return EXIT_FAILURE;
        }
      }

      // Initialize a new section
      current_section = &sections[section_count++];
      current_section->name = strdup(line);
      current_section->items = NULL;
      current_section->item_count = 0;
      current_section->item_capacity = 10;
    } else if (current_section != NULL) {
      add_item(current_section, line);
    }
  }

  fclose(file);

  // Print consolidated sections
  for (size_t i = 0; i < section_count; i++) {
    Section *section = &sections[i];
    printf("%s ", section->name);
    for (size_t j = 0; j < section->item_count; j++) {
      printf("%s ", section->items[j]);
    }
    printf("\n");

    // Free allocated memory for items
    for (size_t j = 0; j < section->item_count; j++) {
      free(section->items[j]);
    }
    free(section->items);
    free(section->name);
  }

  free(sections);
  return EXIT_SUCCESS;
}
