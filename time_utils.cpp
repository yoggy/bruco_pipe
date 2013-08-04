#include <time.h>

long long get_tick_count()
{
	struct timespec ts;
	clock_gettime(CLOCK_MONOTONIC, &ts);
	long long tick = (long long)ts.tv_sec * 1000 + (long long)ts.tv_nsec / 1000 / 1000;

	return tick;
}

