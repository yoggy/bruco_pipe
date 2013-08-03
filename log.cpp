#include "log.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include <time.h>

int output_log_level_ = LOGGING_LEVEL_DEBUG;
int output_string_length_ = 1024;

void set_output_log_level(const int level)
{
	output_log_level_ = level;
}

void set_output_string_length(const int level)
{
	output_string_length_= level;
}

void log_message_output_(const char *time_str, const char *level_str, const char *msg)
{
	fprintf(stderr, "%s %s %s\n", time_str, level_str, msg);
	fflush(stderr);
}

void log_message_format_(const char *file, int line, const char *func, int level, const char *fmt, ...)
{
	if (level < output_log_level_) return;

	char time_str[64];

	time_t lt;
	struct tm t;
	lt = time(&lt);
	localtime_r(&lt, &t);
	strftime(time_str, 64, "%Y-%m-%dT%H:%M:%S%z", &t);

	const char *level_str = NULL;
	switch(level) {
		case LOGGING_LEVEL_ERROR:
			level_str = "[E]";
			break;
		case LOGGING_LEVEL_WARN:
			level_str = "[W]";
			break;
		case LOGGING_LEVEL_INFO:
			level_str = "[I]";
			break;
		case LOGGING_LEVEL_DEBUG:
			level_str = "[D]";
			break;
	}

	char *msg = new char[output_string_length_];
	va_list args;
	va_start(args, fmt);
	vsnprintf(msg, output_string_length_, fmt, args);
	va_end(args);

	log_message_output_(time_str, level_str, msg);

	delete msg;
	msg = 0;
}


