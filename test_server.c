#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	int i;

	setvbuf(stdout, NULL, _IONBF, 0);

	for (i = 0; i < 10; ++i) {
		printf("test%d\n", i);
		sleep(1);

		if (i == 4) {
			printf("bbbb\n"); // <- break session keywork
		}
	}

	return 0;
}
