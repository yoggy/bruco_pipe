#ifndef __LOG_H__
#define __LOG_H__

#ifdef __cplusplus
extern "C" {
#endif 

#define LOGGING_LEVEL_ERROR 3
#define LOGGING_LEVEL_WARN  2
#define LOGGING_LEVEL_INFO  1
#define LOGGING_LEVEL_DEBUG 0

void set_output_log_level(const int level);
void set_output_string_length(const int level);

void log_message_format_(const char *file, int line, const char *func, int level, const char *fmt, ...);

#define log_e(...) {log_message_format_(__FILE__, __LINE__, __FUNCTION__, LOGGING_LEVEL_ERROR, __VA_ARGS__);}
#define log_w(...) {log_message_format_(__FILE__, __LINE__, __FUNCTION__, LOGGING_LEVEL_WARN , __VA_ARGS__);}
#define log_i(...) {log_message_format_(__FILE__, __LINE__, __FUNCTION__, LOGGING_LEVEL_INFO , __VA_ARGS__);}
#define log_d(...) {log_message_format_(__FILE__, __LINE__, __FUNCTION__, LOGGING_LEVEL_DEBUG, __VA_ARGS__);}

#ifdef __cplusplus
};
#endif

#endif //#define __LOG_H__
