#ifndef __LOG_H__
#define __LOG_H__
#include "helios.h"

void log_printf(char* fmt, ...);

#ifdef PRO_DEBUG_ENABLE
#define RLDEBUG(fmt,args...)  log_printf(fmt,##args)
#else
#define RLDEBUG(fmt,args...)
#endif

void hexdata_debug(char *f, int len);

#endif //__LOG_H__