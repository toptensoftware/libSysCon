#include "libSysCon.h"
#include <string.h>

// Linked list of current active fibers
FIBER* g_pActiveFibers = NULL;

// Linked list of sleeping fibers
FIBER* g_pSleepingFibers = NULL;

// The currently executing fiber
FIBER* g_pCurrentFiber = NULL;

// Used to save the stack pointer of caller to run_fibers()
void* g_run_fibers_sp;

// Forward declarations
void resume_current_fiber();

// Runs all fibers until none are active
void run_fibers()
{
	// Quit if no active fibers
	if (!g_pActiveFibers)
		return;

	// Save caller stack
	__asm
	ld (_g_run_fibers_sp),sp	
	__endasm;

	// Resume the next active fiber
	g_pCurrentFiber = g_pActiveFibers;
	resume_current_fiber();
}

// Run the next fiber
void run_next_fiber()
{
	// Get the next active fiber
	g_pCurrentFiber = g_pActiveFibers;
	if (g_pCurrentFiber)
	{
		// Resume it
		resume_current_fiber();
	}
	else
	{
		// No active fibers, return from run_fibers()
		__asm
		ld sp,(_g_run_fibers_sp)
		__endasm;
	}
}

// Resume execution of g_pCurrentFiber
void resume_current_fiber() __naked
{
	__asm

	// Restore fiber stack frame
	ld ix,(_g_pCurrentFiber)
	ld l,2 (ix)
	ld h,3 (ix)
	ld sp,hl

	// Restore fiber registers
	pop IY
	pop IX

	// Continue fiber execution
	ret

	__endasm;
}


// When a fiber function ends it will return to here. 
// Kill the fiber and run the next one
void end_current_fiber()
{
	ll_remove(&g_pActiveFibers, g_pCurrentFiber);
	free(g_pCurrentFiber);
	run_next_fiber();
}

// Create a new fiber with given fiber proc and stack size
FIBER* create_fiber(void (*fiberProc)(), int stack_size)
{
	// Allocate fiber
	FIBER* pFiber = (FIBER*)malloc(sizeof(FIBER) + stack_size);

	// Make active
	pFiber->sleeping = false;
	ll_unshift(&g_pActiveFibers, pFiber);

	// Setup stack so when activated we return to the start proc
	uint16_t* sp = (uint16_t*)(((char*)pFiber) + sizeof(FIBER) + stack_size);
	sp--; *sp = (uint16_t)end_current_fiber;
	sp--; *sp = (uint16_t)fiberProc;
	sp--; *sp = 0;		// IX
	sp--; *sp = 0;		// IY
	pFiber->sp = sp;
	
	// Return fiber
	return pFiber;
}

// Wake a fiber by putting it in the active fiber list
void wake_fiber(FIBER* pFiber)
{
	if (pFiber->sleeping)
	{
		ll_remove(&g_pSleepingFibers, pFiber);
		ll_push(&g_pActiveFibers, pFiber);
		pFiber->sleeping = false;
	}
}

// Sleep the current fiber
void sleep_fiber()
{
	// Sleep the current fiber
	ll_remove(&g_pActiveFibers, g_pCurrentFiber);
	ll_push(&g_pSleepingFibers, g_pCurrentFiber);
	g_pCurrentFiber->sleeping = true;

	// Save fiber context
	__asm
	push IX
	push IY
	ld	hl,#0
	add hl,sp
	ld ix,(_g_pCurrentFiber)
	ld 2 (ix),l
	ld 3 (ix),h
	__endasm;

	// Run the next fiber (or exit fiber processing)
	run_next_fiber();
}

// Initialize a signal
void init_signal(SIGNAL* pSignal)
{
	pSignal->pWaitingFibers = NULL;
	pSignal->set = false;
}

// Fire a signal
void set_signal(SIGNAL* pSignal)
{
	// Get the next fiber to activate
	FIBER* pActivate = ll_shift(&pSignal->pWaitingFibers);
	if (pActivate)
	{
		wake_fiber(pActivate);
	}
	else
	{
		pSignal->set = true;
	}
}

// Wait for a signal to be set
void wait_signal(SIGNAL* pSignal)
{
	// Signal already set?
	if (pSignal->set)
	{
		pSignal->set = false;
		return;
	}

	// Add self as a waiter
	ll_push(&pSignal->pWaitingFibers, g_pCurrentFiber);

	// Sleep for now...
	sleep_fiber();
}


// Initialize a mutex
void init_mutex(MUTEX* pMutex)
{
	pMutex->pOwningFiber = NULL;
	init_signal(&pMutex->signal);
}

// Enter mutex
void enter_mutex(MUTEX* pMutex)
{
	if (pMutex->pOwningFiber == NULL)
	{
		pMutex->pOwningFiber = g_pCurrentFiber;
		return;
	}

	wait_signal(&pMutex->signal);
}

// Leave mutex
void leave_mutex(MUTEX* pMutex)
{
	pMutex->pOwningFiber = NULL;
	if (pMutex->signal.pWaitingFibers != NULL)
	{
		set_signal(&pMutex->signal);
	}
}
