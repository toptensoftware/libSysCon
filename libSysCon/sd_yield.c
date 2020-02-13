#include "libSysCon.h"
#include <string.h>

void (*sd_yield)() = yield_nop;
