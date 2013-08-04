#include <time.h>

#ifdef MAC_OS_X
#include <mach/mach_time.h>
uint64_t get_tick_count_start_t = 0;
#endif

long long get_tick_count()
{
	long long tick = 0;
#ifdef MAC_OS_X
	if (get_tick_count_start_t = 0) {
		get_tick_count_start_t = mach_absolute_time();
	}

	mach_timebase_info_data_t tb;
	mach_timebase_info(&tb);

	uint64_t now_t = mach_absolute_time();

	uint64_t diff_t = (now_t - get_tick_count_start_t) * tb.numer / tb.denom; // nano sec

	// millisecond
	tick = diff_t / 1000 / 1000;

#else
	// linux...
	struct timespec ts;
	clock_gettime(CLOCK_MONOTONIC, &ts);
	tick = (long long)ts.tv_sec * 1000 + (long long)ts.tv_nsec / 1000 / 1000;
#endif

	return tick;
}

