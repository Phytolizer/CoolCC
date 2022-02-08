CC := clang
CFLAGS += -Wall -Wextra -Wpedantic -Wmissing-prototypes -Wswitch-enum -ggdb -fsanitize=address
LDFLAGS += -fsanitize=address

.PHONY: all clean

all: CoolChibiCC

CoolChibiCC: main.o
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@
%.o: %.c
	$(CC) $(CFLAGS) -c $^ -o $@
