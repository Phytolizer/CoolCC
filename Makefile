CC ?= clang
CFLAGS += -Wall -Wextra -Wpedantic -Wmissing-prototypes -Wswitch-enum -ggdb -fsanitize=address
LDFLAGS += -fsanitize=address

.PHONY: all clean

all: CoolCC

CoolCC: main.o error.o
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@
%.o: %.c
	$(CC) $(CFLAGS) -c $^ -o $@
