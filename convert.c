#include <stdio.h>

int main(int argc, char *argv[]) {
	int c;
	while ((c = getchar()) != EOF) {
		if (c == '\r') {
			putchar('\n');
		} else {
			putchar(c);
		}
	}
}
