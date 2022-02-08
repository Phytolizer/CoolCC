CC := clang
CFLAGS += -Wall -Wextra -Wpedantic -Wmissing-prototypes -Wswitch-enum -ggdb -fsanitize=address
LDFLAGS += -fsanitize=address

.PHONY: all check clean

all: CoolChibiCC

check: CoolChibiCC CoolChibiTest
	./CoolChibiTest

CoolChibiTest: test.o
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

CoolChibiCC: main.o
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@
%.o: %.c
	$(CC) $(CFLAGS) -c $^ -o $@

clean:
	rm -vf *.o CoolChibiCC
