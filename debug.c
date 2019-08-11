#include "debug.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>

//#define debug(f, a...)	printf("[DEBUG]%s(%d):" f,  __func__, errno , ## a)
//#define err(f, a...)		printf("[ERR]%s(%d):" f,  __func__, errno , ## a)

FILE *fp_log = NULL;

void init_log_file(char *path)
{
	if (fp_log == 0) {
		fp_log = fopen(path, "wb+");
	}
}

int close_log_file(void)
{
	if ((fp_log != NULL)) {
		fclose(fp_log);
	}
	return 0;
}

int err(char *str,...)
{
	char buf[10240];
	time_t timep;
	struct tm *p; 

	time(&timep);
	p = localtime(&timep);

	va_list ap;
	va_start(ap, str);
	vsprintf(buf, str, ap);
	va_end(ap);

	if (fp_log != NULL) {
		fprintf(fp_log, "[%d:%d:%d][Error]%s", p->tm_hour, p->tm_min, p->tm_sec, buf);
	}
	#ifdef LOG_OUTPUT_STD
	printf("[%d:%d:%d][Error]%s", p->tm_hour, p->tm_min, p->tm_sec, buf);
	#endif

	return 0;
}

#ifdef LOG_ENABLE

int debug(char *str,...)
{
	char buf[10240];
	time_t timep;
	struct tm *p; 

	time(&timep);
	p = localtime(&timep);

	va_list ap;
	va_start(ap, str);
	vsprintf(buf, str, ap);
	va_end(ap);

	if (fp_log != NULL) {
		fprintf(fp_log, "[%d:%d:%d][Debug]%s", p->tm_hour, p->tm_min, p->tm_sec, buf);
	}
	#ifdef LOG_OUTPUT_STD
	printf("[%d:%d:%d][Debug]%s", p->tm_hour, p->tm_min, p->tm_sec, buf);
	#endif

	return 0;
}

int dump_memory(const char *buf, int count)
{
	char str[1024];
	int ret = 0; 
	int i, j;	
	for(i=0; i<(count+15)/16; i++){
		sprintf(str, "%-8x: ", i*16);
		for(j=0; j<16; j++){
			if(i*16+j >= count){
				break;
			}
			sprintf(str, "%s%4x", str, buf[i*16+j]);
		}
		sprintf(str, "%s\n", str);
		debug(str);
	}
	
	return ret;
}

#endif
