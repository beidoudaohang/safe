#include <stdio.h>
#include <stdarg.h>
#include <helios.h>
#if 1
void log_printf(char* fmt, ...)
{
    va_list ap;
    char *p, *sval;
    int ival;

    if (NULL == fmt)
        printf("relay printf's fmt is null \r\n");

    va_start(ap, fmt);

    for (p = fmt; *p; p++) {
        if ('%' != (*p)) {
            putchar(*p);
            continue;
        }
        switch (*(p + 1)) {
        case 'd': {
            printf("%d", va_arg(ap, int));
            break;
        }
        case 'f': {
            printf("%f", va_arg(ap, double));
            break;
        }
        case 's': {
            for (sval = va_arg(ap, char*); *sval; sval++) {
                putchar(*sval);
            }
            break;
        }
        case 'x': {
            ival = (int)va_arg(ap, int);
            printf("%02x", ival);
            break;
        }
        default:
            putchar(*p);
        }
        p++;
    }
    fflush(stdout);
}
#endif

void hexdata_debug(char *f, int len)
{
	int i;

	for(i=0; i<len; i++){
		log_printf("0x%x,", (u8) *f++);
	}
	log_printf("\r\n");
}