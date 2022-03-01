#include <stdio.h>

int sum(int a, int b) {
	return a + b;
}

int main(void) {
	int x = 2;
	int y = 5;

	int z = sum(x, y);
	printf("%d\n", z);

	return 0;
}
