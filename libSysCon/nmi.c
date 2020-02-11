#include "libSysCon.h"
#include <stdint.h>
#include <stdbool.h>


void* g_ExtSP;
void* g_ExtRegs[6];
void* g_SysConSP;
void* g_SysConRegs[6];
bool g_bFirstYield = true;

void processInterrupts()
{
}



// NMI handler
void nmi_handler()
{
__asm

	; Save external state
	ld		(_g_ExtSP),SP
	ld		SP,#_g_ExtRegs + 12
	push	AF
	push	BC
	push	DE
	push	HL
	push	IX
	push	IY

	; Restore SysCon state
	ld		SP,(_g_SysConSP)
	pop		IY
	pop		IX
	pop		HL
	pop		DE
	pop		BC
	pop		AF

__endasm;

    // Process interrrupts
	processInterrupts();
}



void YieldNmiProc()
{
__asm
	; Save SysCon state
	push	AF
	push	BC
	push	DE
	push	HL
	push	IX
	push	IY
	ld		(_g_SysConSP),SP

	; Restore External state
	ld		SP,#_g_ExtRegs
	pop		IY
	pop		IX
	pop		HL
	pop		DE
	pop		BC

	; If this is the first yield we need to jump to 0x0000
	ld      a,(_g_bFirstYield)
	or      a
	jr      nz,$1

	pop		AF
	ld		SP,(_g_ExtSP)

	out		(_InterruptControllerPort),A
	retn	; NB: "RETN" not "RET"

$1:
	; Clear flag
	xor		a
	ld      (_g_bFirstYield),A

    ; Boot main machine
    ld      HL,#0
	out		(_InterruptControllerPort),A
    jp      (HL)

__endasm;
}

