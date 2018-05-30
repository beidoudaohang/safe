#ifndef __LOG_H__
#define __LOG_H__
#include "helios.h"

void log_printf(char* fmt, ...);

#ifdef PRO_DEBUG_ENABLE
#define LOG_DEBUG(fmt,args...)  log_printf(fmt,##args)
#else
#define LOG_DEBUG(fmt,args...)
#endif

#endif //__LOG_H__