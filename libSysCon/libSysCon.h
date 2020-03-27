#ifndef _LIBSYSCON_H
#define _LIBSYSCON_H

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>



// ------------------------- Linked Lists -------------------------

typedef struct _LISTITEM
{
	struct _LISTITEM* pNext;
} LISTITEM;


void* ll_push(void** pHead, void* pItem);
void* ll_pop(void** pHead);
void* ll_shift(void** pHead);
void* ll_unshift(void** pHead, void* pItem);
void* ll_remove(void** pHead, void* pItem);
bool ll_contains(void* pHead, void* pItem);



// ------------------------- NMI handling -------------------------

// Call this to yield back from the NMI handler
void yield_from_nmi();



// ------------------------- Fibers -------------------------

typedef struct _FIBER
{
    struct _FIBER* pNext;
    void* sp;
    bool sleeping;
} FIBER;

typedef struct _SIGNAL
{
    FIBER* pWaitingFibers;
    bool set;
} SIGNAL;

typedef struct _MUTEX
{
    FIBER* pOwningFiber;
    SIGNAL signal;
	uint8_t depth;
} MUTEX;

// Run all active fibers
void run_fibers();

// Create a new fiber with given proc and stack size
FIBER* create_fiber(void (*fiberProc)(), int stack_size);

// Get the currently running fiber
FIBER* get_current_fiber();

// Signals
void init_signal(SIGNAL* pSignal);
void set_signal(SIGNAL* pSignal);
void wait_signal(SIGNAL* pSignal);

// Mutexes
void init_mutex(MUTEX* pMutex);
void enter_mutex(MUTEX* pMutex);
void leave_mutex(MUTEX* pMutex);



// ------------------------- Address Page Mapping -------------------------

// Address page mapping ports
#define APM_PAGEBANK_PORT     0xA1
#define APM_ENABLE_PORT       0xA2

// Sets the upper bits that addresses 0xFC00 -> 0xFFFF map to
// Actual ram address will be (ApmPageBank << 10) | address (& 0x3ff)
__sfr __at APM_PAGEBANK_PORT ApmPageBank;

// Enables/disables other entries in the memory map (see SYSCON_ENABLE_* flags)
__sfr __at APM_ENABLE_PORT ApmEnable;

// Bit flags for ApmSysConEnable
#define APM_ENABLE_VIDEOBANK  0x01           // 0xFC00 -> 0xFFFF
#define APM_ENABLE_BOOTROM    0x02           // Enable boot rom at 0x0000 -> 0x7FFF
                                             // (from 0x6000 -> 0x7FFF is writable)
#define APM_ENABLE_PAGEBANK   0x04           // Enable page banking from 0xFC00 -> 0xFFFF
#define APM_ENABLE_VIDEOSHOW  0x08           // Enable syscon video display
#define APM_ENABLE_ALLKEYS    0x10           // Forward all keys to syscon
#define APM_ENABLE_RESET      0x80           // Write this to ApmEnable to reset the machine

__at(0xFC00) char banked_page[0x400];



// ------------------------- Options Port -------------------------

#define OPTIONS_PORT 0x00

__sfr __at OPTIONS_PORT OptionsPort;

#define OPTION_TURBO_TAPE 0x01
#define OPTION_TYPING_MODE 0x02
#define OPTION_GREEN_SCREEN 0x04
#define OPTION_NO_SCAN_LINES 0x08
#define OPTION_CAS_AUDIO 0x10
#define OPTION_AUTO_CAS 0x20



// ------------------------- Interrupt Controller -------------------------

#define INTERRUPT_CONTROLLER_PORT    0x1c

__sfr __at INTERRUPT_CONTROLLER_PORT InterruptControllerPort;

// Reading from the interrupt controller port returns a bit mask
// of the raised interrupts.

// Writing to the interrupt controller port indicates a request
// to exit hijack mode.  The actual mode switch will happen on the 
// next RET or JP (HL).


#define IRQ_UART_RX     0x01
#define IRQ_UART_TX     0x02
#define IRQ_SD_CARD     0x04
#define IRQ_KEYBOARD    0x08


// ------------------------- Serial Port -------------------------

// UART Port Numbers
#define UART_TX_STATUS_PORT 0x80
#define UART_TX_DATA_PORT   0x80
#define UART_RX_STATUS_PORT 0x81
#define UART_RX_DATA_PORT   0x82

#define UART_TX_BUFFER_SIZE 255
#define UART_RX_BUFFER_SIZE 255

// Reading this port returns the number of bytes currently 
// in the transmit buffer.
__sfr __at UART_TX_STATUS_PORT UartTxStatusPort;

// Writing to this port enqueues data to the transmit buffer
// (unless it's full)
__sfr __at UART_TX_DATA_PORT UartTxDataPort;

// Reading this port returns the number of bytes currently in 
// the receive buffer.
__sfr __at UART_RX_STATUS_PORT UartRxStatusPort;

// Reading this port returns a single byte from the receive
// buffer (unless it's empty)
__sfr __at UART_RX_DATA_PORT UartRxDataPort;

// UART Write
void uart_write(void* ptr, uint8_t length);
void uart_write_sz(const char* psz);

// UART Read.  Will block until something received
// but may return with a partially filled buffer
uint8_t uart_read(void* ptr, uint8_t length);

// Same as above but doesn't return until length bytes
// received.
uint8_t uart_read_wait(void* ptr, uint8_t length);


extern void (*uart_read_yield)();
extern void (*uart_write_yield)();

// UART ISR
void uart_read_init_isr();
void uart_read_isr();
void uart_write_init_isr();
void uart_write_isr();



// ------------------------- SD Card -------------------------

#define SD_PORT_SET_BLOCK_NUMBER    0x90        // Write-only, 32-bits, LSB first
#define SD_PORT_STATUS              0x91        // Read for status
#define SD_PORT_COMMAND             0x91        // Write to invoke command
#define SD_PORT_DATA                0x92        // Read-Write

#define SD_STATUS_BUSY			    0x01 	    // Disk controller is busy
#define SD_STATUS_READING           0x02        // Dick controller is busy reading
#define SD_STATUS_WRITING           0x04        // Disk controller is busy writing
#define SD_STATUS_ERROR			    0x08        // Disk controller has an DISK_STATUS_BIT_ERROR
#define SD_STATUS_INIT      	    0x40        // Disk controller has initialized
#define SD_STATUS_SDHC			    0x80 	    // Disk controller is busy

#define SD_COMMAND_NOP              0           // Null op (resets the buffer pointer to 0)
#define SD_COMMAND_READ			    1           // Start a read operation
#define SD_COMMAND_WRITE			2           // Start a write operation

// Write to this port 4x times (32-bits) to set the block number (LSB first)
__sfr __at SD_PORT_SET_BLOCK_NUMBER SdSetBlockNumberPort;

// Read this port to get the SD Card status (SD_STATUS_*)
__sfr __at SD_PORT_STATUS SdStatusPort;

// Write to this port to invoke commands (SD_COMMAND_*)
__sfr __at SD_PORT_COMMAND SdCommandPort;

// Read/write this port to drain/fill the SD card buffer
__sfr __at SD_PORT_DATA SdDataPort;

void sd_set_block_number(uint32_t blockNumber);

void sd_read_command(uint32_t blockNumber);
void sd_read_data(void* ptr);
void sd_read(uint32_t block_number, void* ptr);

void sd_write_command(uint32_t blockNumber);
void sd_write_data(void* ptr);
void sd_write(uint32_t block_number, void* ptr);

void sd_init_isr();
void sd_isr();

extern void (*sd_yield)();


// ------------------------- Geometry -------------------------

typedef struct tagRECT
{
	uint8_t left;
	uint8_t top;
	uint8_t width;
	uint8_t height;
} RECT;

typedef struct tagPOINT
{
	uint8_t x;
	uint8_t y;
} POINT;



// ------------------------- Video Overlay -------------------------

#define SCREEN_WIDTH		32
#define SCREEN_HEIGHT		16

// Colors
#define COLOR_BLACK			0
#define COLOR_DARKRED		1
#define COLOR_DARKGREEN		2
#define COLOR_BROWN			3
#define COLOR_DARKBLUE		4
#define COLOR_DARKMAGENTA	5
#define COLOR_DARKCYAN		6
#define COLOR_GRAY			7
#define COLOR_DARKGRAY		8
#define COLOR_RED			9
#define COLOR_GREEN			10
#define COLOR_YELLOW		11
#define COLOR_BLUE			12
#define COLOR_MAGENTA		13
#define COLOR_CYAN			14
#define COLOR_WHITE			15

#define MAKECOLOR(fg, bg) ((uint8_t)(((bg) << 4) | (fg)))


// Character rom is reduced character set.
// These are the box draw character codes
#define BOX_TL	((char)6)
#define BOX_TR	((char)3)
#define BOX_BL	((char)4)
#define BOX_BR	((char)5)
#define BOX_H	((char)1)
#define BOX_V	((char)2)
#define CH_CURSOR	((char)20)

// Flags for video_draw_text
#define DT_LEFT			0
#define DT_RIGHT 		1
#define DT_CENTER 		2
#define DT_ALIGNMASK 	0x03
#define DT_NOFILL 		0x80		// Otherwise fill unused area with spaces

__at(0xFC00) char video_char_ram_hw[SCREEN_WIDTH * SCREEN_HEIGHT];
__at(0xFC00 + SCREEN_WIDTH * SCREEN_HEIGHT) char video_color_ram_hw[SCREEN_WIDTH * SCREEN_HEIGHT];

extern char* video_char_ram;
extern char* video_color_ram;

void video_clear();
void video_write(uint8_t x, uint8_t y, const char* psz, uint8_t length, uint8_t attr);
void video_write_sz(uint8_t x, uint8_t y, const char* psz, uint8_t attr);
void video_set_color(uint8_t left, uint8_t top, uint8_t width, uint8_t height, uint8_t attr);
void video_draw_box_indirect(RECT* prc, uint8_t attr);
void video_draw_box(uint8_t left, uint8_t top, uint8_t width, uint8_t height, uint8_t attr);
void video_text_out(uint8_t x, uint8_t y, const char* psz);
void video_draw_text(uint8_t x, uint8_t y, uint8_t width, const char* psz, uint8_t flags);
void video_measure_multiline_text(const char* psz, POINT* pVal);
void video_draw_multiline_text(const char* psz, POINT* pPos);
void video_scroll(uint8_t left, uint8_t top, uint8_t width, uint8_t height, int dy, bool attr, uint8_t* pRedrawFrom, uint8_t* pRedrawCount);
void video_begin_offscreen();
void video_end_offscreen();
void* video_save(uint8_t left, uint8_t top, uint8_t width, uint8_t height);
void video_restore(void* saveData);


// ------------------------- Keyboard -------------------------

#define KEYBOARD_PORT_LO    0x70       // Read only
#define KEYBOARD_PORT_HI    0x71       // Read only

__sfr __at KEYBOARD_PORT_LO KeyboardPortLo;
__sfr __at KEYBOARD_PORT_HI KeyboardPortHi;

// KeyboardPortLo(7) = extended key flag
// KeyboardPortLo(6 downto 0) = scan code
// KeyboardPortLo == 0 then no key

// KeyboardPortHi(7) = key release flag
// KeyboardPortHi(6 downto 2) = unused
// KeyboardPortHi(1) = Ctrl pressed
// KeyboardPortHi(0) = Shift pressed

// Reading KeyboardPortHi clears the current key

char key_to_char(uint8_t lo, uint8_t hi);


#define KEY_F9 0x01
#define KEY_F5 0x03
#define KEY_F3 0x04
#define KEY_F1 0x05
#define KEY_F2 0x06
#define KEY_F12 0x07
#define KEY_F10 0x09
#define KEY_F8 0x0A
#define KEY_F6 0x0B
#define KEY_F4 0x0C
#define KEY_TAB 0x0D
#define KEY_BACKTICK 0x0E
#define KEY_LMENU 0x11
#define KEY_RMENU 0x91
#define KEY_LSHIFT 0x12
#define KEY_LCTRL 0x14
#define KEY_RCTRL 0x94
#define KEY_Q 0x15
#define KEY_1 0x16
#define KEY_Z 0x1A
#define KEY_S 0x1B
#define KEY_A 0x1C
#define KEY_W 0x1D
#define KEY_2 0x1E
#define KEY_C 0x21
#define KEY_X 0x22
#define KEY_D 0x23
#define KEY_E 0x24
#define KEY_4 0x25
#define KEY_3 0x26
#define KEY_SPACE 0x29
#define KEY_V 0x2A
#define KEY_F 0x2B
#define KEY_T 0x2C
#define KEY_R 0x2D
#define KEY_5 0x2E
#define KEY_N 0x31
#define KEY_B 0x32
#define KEY_H 0x33
#define KEY_G 0x34
#define KEY_Y 0x35
#define KEY_6 0x36
#define KEY_M 0x3A
#define KEY_J 0x3B
#define KEY_U 0x3C
#define KEY_7 0x3D
#define KEY_8 0x3E
#define KEY_COMMA 0x41
#define KEY_K 0x42
#define KEY_I 0x43
#define KEY_O 0x44
#define KEY_0 0x45
#define KEY_9 0x46
#define KEY_PERIOD 0x49
#define KEY_SLASH 0x4A
#define KEY_DIVIDE 0xCA
#define KEY_L 0x4B
#define KEY_SEMICOLON 0x4C
#define KEY_P 0x4D
#define KEY_HYPHEN 0x4E
#define KEY_QUOTE 0x52
#define KEY_LSQUARE 0x54
#define KEY_EQUALS 0x55
#define KEY_CAPITAL 0x58
#define KEY_RSHIFT 0x59
#define KEY_ENTER 0x5A
#define KEY_NUMENTER 0xDA
#define KEY_RSQUARE 0x5B
#define KEY_BACKSLASH 0x5D
#define KEY_BACKSPACE 0x66
#define KEY_NUM1 0x69
#define KEY_END 0xE9
#define KEY_NUM4 0x6B
#define KEY_LEFT 0xEB
#define KEY_NUM7 0x6C
#define KEY_HOME 0xEC
#define KEY_NUM0 0x70
#define KEY_INSERT 0xF0
#define KEY_DECIMAL 0x71
#define KEY_DELETE 0xF1
#define KEY_NUM2 0x72
#define KEY_DOWN 0xF2
#define KEY_NUM5 0x73
#define KEY_NUM6 0x74
#define KEY_RIGHT 0xF4
#define KEY_NUM8 0x75
#define KEY_UP 0xF5
#define KEY_ESCAPE 0x76
#define KEY_F11 0x78
#define KEY_ADD 0x79
#define KEY_NUM3 0x7A
#define KEY_NEXT 0xFA
#define KEY_SUBTRACT 0x7B
#define KEY_MULTIPLY 0x7C
#define KEY_NUM9 0x7D
#define KEY_PRIOR 0xFD
#define KEY_F7 0x83        // NB: Not an extended key code


// ------------------------- No-op Yield -------------------------

void yield_nop();



// ------------------------- Messaging -------------------------


#define MESSAGE_NOP			0
#define MESSAGE_KEYDOWN  	1
#define MESSAGE_CHAR	 	2
#define MESSAGE_KEYUP  	    3
#define MESSAGE_DRAWFRAME	4
#define MESSAGE_DRAWCONTENT	5

typedef struct tagMSG
{
	uint8_t message;
	uint8_t param1;
	uint16_t param2;
} MSG;

void msg_init();
void msg_copy(MSG* dest, MSG* src);
void msg_enqueue(MSG* pMessage);
void msg_post(uint8_t msg, uint8_t param1, uint16_t param2);
bool msg_peek(MSG* pMsg, bool remove);
void msg_get(MSG* pMsg);
void msg_yield();
void msg_isr();



// ------------------------- Windowing -------------------------

typedef struct tagWINDOW
{
// public:
	RECT rcFrame;
	uint8_t attrNormal;
	uint8_t attrSelected;
	void* user;
	const char* title;
	size_t (*wndProc)(struct tagWINDOW* pWindow, MSG* pMessage);

// private/readonly:
	struct tagWINDOW* parent;
	void* screenSave;
	bool running;
	bool modal;
	int retv;
	bool needsDraw;
} WINDOW;


size_t window_send_message(WINDOW* pWindow, uint8_t message, uint8_t param1, uint16_t param2);
void window_get_client_rect(WINDOW* pWindow, RECT* prc);
extern size_t (*window_msg_hook)(WINDOW* pWindow, MSG* pMessage, bool* pbHandled);
size_t window_def_proc(WINDOW* pWindow, MSG* pMsg);
WINDOW* window_get_active();
void window_create(WINDOW* pWindow);
void window_destroy();
void window_run_modeless(WINDOW* pWindow);
bool window_update_modeless(WINDOW* pWindow);
int window_end_modeless(WINDOW* pWindow);
int window_run_modal(WINDOW* pWindow);
void window_end_modal(int retv);
uint8_t window_get_attr_normal(WINDOW* pWindow);
uint8_t window_get_attr_selected(WINDOW* pWindow);
void window_invalidate(WINDOW* pWindow);



// ------------------------- Windowing -------------------------

#define MB_INPROGRESS	0x40	// Green display
#define MB_ERROR		0x80	// Red display

typedef struct tagMESSAGEBOX
{
	WINDOW window;
	const char* pszMessage;
	const char** ppszButtons;
	uint8_t flags;
	uint8_t selectedButton;
} MESSAGEBOX;

// Setup to display a modeless message box
void message_box_modeless(MESSAGEBOX* pMessageBox, const char* pszTitle, const char* pszMessage, const char** ppszButtons, uint8_t flags);

// Returns 0 if last button pressed
// Otherwise 1 based button index
int message_box(const char* pszTitle, const char* pszMessage, const char** ppszButtons, uint8_t flags);



// ------------------------- Prompt Input -------------------------

bool prompt_input(const char* pszTitle, char* buf, int cbBuf);



// ------------------------- List Box -------------------------

typedef struct tagLISTBOX
{
// Window
	WINDOW window;

// Public
	int selectedItem;

// Pull Data source
	int (*getItemCount)(struct tagLISTBOX* pListBox);
	const char* (*getItemText)(struct tagLISTBOX* pListBox, int item);

// Push data source
	int itemCount;
	const char** ppItems;

// Internal
	uint8_t	state;
	int scrollPos;
} LISTBOX;

size_t listbox_wndproc(WINDOW* pWindow, MSG* pMsg);
void listbox_draw(LISTBOX* pListBox);
void listbox_drawitem(LISTBOX* pListBox, int item);
bool listbox_message(LISTBOX* pListBox, MSG* pMsg);
int listbox_ensure_visible(LISTBOX* pListBox, int item);
bool listbox_select(LISTBOX* pListBox, int item);
void listbox_set_data(LISTBOX* pListBox, int itemCount, const char** ppItems);

int listbox_modal(LISTBOX* pListBox);


#endif      // _LIBSYSCON_H
