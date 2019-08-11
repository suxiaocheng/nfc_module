#ifndef __DEBUG_H__
#define __DEBUG_H__

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#define LOG_ENABLE
#define LOG_OUTPUT_STD

#define LOG_FILE_NAME "/tmp/car_control.log"

#define ERR(str, ...) err("%s(%d)"str, __func__, __LINE__, ##__VA_ARGS__)
#define DEBUG(str, ...) debug("%s(%d)"str, __func__, __LINE__, ##__VA_ARGS__)

void init_log_file(char *path);
int close_log_file(void);
int err(char *str,...);

#ifdef LOG_ENABLE
int debug(char *str,...);
int dump_memory(const char *buf, int count);
#else
__inline static int debug(char *str,...){}
__inline static int dump_memory(const char *buf, int count){}
#endif

#endif
