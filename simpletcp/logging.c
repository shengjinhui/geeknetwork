#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <string.h>
#include "logging.h"


void Logging(const char* level,const char *format, ...){
    printf(" [+][%s][%s] ",asctimeEx(),level);
	va_list arg;
	va_start(arg, format);
	while (*format){
		char ret = *format;
		if (ret == '%'){
			switch (*++format){
			case 'c': {
                char ch = va_arg(arg, int);
                printf("%c",ch);
                break;
			}
			case 's':{
                char *pc = va_arg(arg, char *);
                printf("%s",pc);
                break;
			}
            case 'd':{
                int d = va_arg(arg,int);
                printf("%d",d);
			}
			default:
				break;
			}
		}else{
			putchar(*format);
		}
		format++;
	}
	va_end(arg);
    printf("\n");
}


// 返回格式化好的asctime 去掉末尾的回车
char *asctimeEx(){
	char* ret;
	time_t timep;
	size_t ascLen;

    time(&timep);
	ret = asctime(gmtime(&timep));
	ascLen = strlen(ret);
	ret[ascLen-1] = '\0';
	return ret;
}