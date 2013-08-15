#include "log.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include <time.h>

#include <string.h>
#include <string>

#include "udp.hpp"

int output_log_level_ = LOGGING_LEVEL_DEBUG;
int output_string_length_ = 1024;
int udp_socket_ = -1;
std::string udp_host_ = "";

void set_output_log_level(const int level)
{
	output_log_level_ = level;
}

void set_output_string_length(const int level)
{
	output_string_length_= level;
}

void start_udp_log_output(const char *host, const int &port)
{
	stop_udp_log_output();

	log_d("start_udp_log_output() host=%s, port=%d", host, port);
	udp_socket_ = open_udp(host, port);
}

void stop_udp_log_output()
{
	if (udp_socket_ != -1) {
		close(udp_socket_);
		udp_socket_ = -1;
	}
}

void udp_log_message_output_(const char *buf, const int &size)
{
	if (udp_socket_ == -1) return;
	
	::send(udp_socket_, buf, size, 0);
}

void log_message_output_(const char *time_str, const char *level_str, const char *msg)
{
	int buf_size = strlen(time_str) + 1 + strlen(level_str) + 1 + strlen(msg) + 3;
	char *buf = (char*)malloc(buf_size);
	memset(buf, 0, buf_size);

	snprintf(buf, buf_size, "%s %s %s\r\n", time_str, level_str, msg);

	fprintf(stderr, "%s", buf);
	fflush(stderr);

	if (udp_socket_ != -1) {
		udp_log_message_output_(buf, buf_size-1);
	}

	free(buf);
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


