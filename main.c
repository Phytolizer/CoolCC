#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    if (argc != 2) {
        fprintf(stderr, "usage: %s <integer>\n", argv[0]);
        return 1;
    }

    char* p = argv[1];

    printf("  .globl main\n");
    printf("main:\n");
    printf("  mov $%ld, %%rax\n", strtol(argv[1], &p, 10));

    while (*p != '\0') {
        switch (*p) {
            case '+':
                p += 1;
                printf("  add $%ld, %%rax\n", strtol(p, &p, 10));
                break;

            case '-':
                p += 1;
                printf("  sub $%ld, %%rax\n", strtol(p, &p, 10));
                break;

            default:
                fprintf(stderr, "unexpected character '%c'\n", *p);
                return 1;
        }
    }

    printf("  ret\n");
    return 0;
}
