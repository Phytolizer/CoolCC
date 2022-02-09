#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum token_kind {
    TK_PUNCTUATION,
    TK_NUMBER,
    TK_EOF,
};

struct token {
    enum token_kind kind;
    struct token* next;
    /// if kind is TK_NUMBER, the value.
    int value;
    char* location;
    int length;
};

static void error(const char* format, ...) {
    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
    fputs("\n", stderr);
    exit(1);
}

static struct token* advance(struct token* tok) {
    struct token* next = tok->next;
    free(tok);
    return next;
}

static bool equal(const struct token* tok, const char* op) {
    int len = strlen(op);
    return len == tok->length && memcmp(tok->location, op, len) == 0;
}

static struct token* skip(struct token* tok, const char* s) {
    if (!equal(tok, s)) {
        error("expected '%s'", s);
    }

    return advance(tok);
}

static int get_number(const struct token* tok) {
    if (tok->kind != TK_NUMBER) {
        error("expected a number");
    }

    return tok->value;
}

static struct token* new_token(enum token_kind kind, char* start, char* end) {
    struct token* tok = calloc(1, sizeof(struct token));
    tok->kind = kind;
    tok->location = start;
    tok->length = end - start;
    return tok;
}

static struct token* tokenize(char* str) {
    struct token head = {0};
    struct token* cur = &head;

    while (*str != '\0') {
        if (isspace(*str)) {
            str += 1;
        } else if (isdigit(*str)) {
            cur->next = new_token(TK_NUMBER, str, str);
            cur = cur->next;
            char* p = str;
            cur->value = strtoul(str, &str, 10);
            cur->length = str - p;
        } else if (*str == '+' || *str == '-') {
            cur->next = new_token(TK_PUNCTUATION, str, str + 1);
            cur = cur->next;
            str += 1;
        } else {
            error("invalid token");
        }
    }

    cur->next = new_token(TK_EOF, str, str);
    return head.next;
}

int main(int argc, char** argv) {
    if (argc != 2) {
        error("%s: invalid number of arguments", argv[0]);
    }

    struct token* tok = tokenize(argv[1]);

    printf("  .globl main\n");

    printf("main:\n");

    printf("  mov $%d, %%rax\n", get_number(tok));

    tok = advance(tok);

    while (tok->kind != TK_EOF) {
        if (equal(tok, "+")) {
            printf("  add $%d, %%rax\n", get_number(tok->next));
            tok = advance(advance(tok));
        } else {
            tok = skip(tok, "-");
            printf("  sub $%d, %%rax\n", get_number(tok));
            tok = advance(tok);
        }
    }

    free(tok);
    printf("  ret\n");
    return 0;
}
