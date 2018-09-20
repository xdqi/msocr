#include <stdio.h>
#include <stdarg.h>

static const char *levels[] = {
    "UNKNOWN",
    "DEFAULT",    /* only for SetMinPriority() */
    "VERBOSE",
    "DEBUG",
    "INFO",
    "WARN",
    "ERROR",
    "FATAL",
    "SILENT",     /* only for SetMinPriority(); must be last */
};

int __android_log_print(int prio, const char *tag,  const char *fmt, ...) {

    va_list ap;
    va_start(ap, fmt);
    printf("[%s] [%s] ", levels[prio], tag);
    int result = vfprintf(stdout, fmt, ap);
    printf("\n");
    va_end(ap);
    return result;
}
