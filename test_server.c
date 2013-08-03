#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	setvbuf(stdout, NULL, _IONBF, 0);

	for (int i = 0; i < 10; ++i) {
		printf("test%d\n", i);
		sleep(1);
	}

	return 0;
}
