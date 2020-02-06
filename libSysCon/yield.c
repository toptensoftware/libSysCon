#include "libSysCon.h"

void yieldNop()
{
}

void (*yield)() = yieldNop;

