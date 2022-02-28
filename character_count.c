#include <ctype.h>
#include <stddef.h>
#include <stdio.h>

#define LINE_BUFFER_SIZE 128

static void usage(void) {
    printf("\n  Usage:\n");
    printf("    character_count [filename]\n\n");
}

int main(int argc, char** argv) {
    if (argc < 2) {
        printf("Filename not specified.\n");
        usage();
        return -1;
    }
    FILE* fp = fopen(argv[1], "r");
    if (!fp) {
        fprintf(stderr, "The file %s cannot be opened.\n", argv[1]);
        return 1;
    }

    size_t num_chars = 0;
    char line_buffer[LINE_BUFFER_SIZE];
    while (fgets(line_buffer, sizeof line_buffer, fp) != NULL) {
        for (size_t i = 0; line_buffer[i] != '\0'; i += 1) {
            if (!isspace(i)) {
                num_chars += 1;
            }
        }

        printf("%zu Characters\n", num_chars);
    }
    fclose(fp);
    return 0;
}
