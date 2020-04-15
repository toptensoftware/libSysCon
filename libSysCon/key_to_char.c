#include "libSysCon.h"

static const char normal_keys[128] = {
//          0     1     2     3     4     5     6     7     8     9     A     B     C     D     E     F   
/* 0 */ 	'\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '`',  '\0', 
/* 1 */ 	'\0', '\0', '\0', '\0', '\0', 'q',  '1',  '\0', '\0', '\0', 'z',  's',  'a',  'w',  '2',  '\0', 
/* 2 */ 	'\0', 'c',  'x',  'd',  'e',  '4',  '3',  '\0', '\0', ' ',  'v',  'f',  't',  'r',  '5',  '\0', 
/* 3 */ 	'\0', 'n',  'b',  'h',  'g',  'y',  '6',  '\0', '\0', '\0', 'm',  'j',  'u',  '7',  '8',  '\0', 
/* 4 */ 	'\0', ',',  'k',  'i',  'o',  '0',  '9',  '\0', '\0', '.',  '/',  'l',  ';',  'p',  '-',  '\0', 
/* 5 */ 	'\0', '\0', '\'', '\0', '[',  '=',  '\0', '\0', '\0', '\0', '\n', ']',  '\0', '\\', '\0', '\0', 
/* 6 */ 	'\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '1',  '\0', '4',  '7',  '\0', '\0', '\0', 
/* 7 */ 	'0',  '.',  '2',  '5',  '6',  '8',  '\0', '\0', '\0', '+',  '3',  '-',  '*',  '9',  '\0', '\0', 
};

static const char shift_keys[128] = {
//          0     1     2     3     4     5     6     7     8     9     A     B     C     D     E     F   
/* 0 */ 	'\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '~',  '\0', 
/* 1 */ 	'\0', '\0', '\0', '\0', '\0', 'Q',  '!',  '\0', '\0', '\0', 'Z',  'S',  'A',  'W',  '@',  '\0', 
/* 2 */ 	'\0', 'C',  'X',  'D',  'E',  '$',  '#',  '\0', '\0', ' ',  'V',  'F',  'T',  'R',  '%',  '\0', 
/* 3 */ 	'\0', 'N',  'B',  'H',  'G',  'Y',  '^',  '\0', '\0', '\0', 'M',  'J',  'U',  '&',  '*',  '\0', 
/* 4 */ 	'\0', '<',  'K',  'I',  'O',  ')',  '(',  '\0', '\0', '>',  '?',  'L',  ':',  'P',  '_',  '\0', 
/* 5 */ 	'\0', '\0', '\"', '\0', '{',  '+',  '\0', '\0', '\0', '\0', '\n', '}',  '\0', '|',  '\0', '\0', 
/* 6 */ 	'\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '1',  '\0', '4',  '7',  '\0', '\0', '\0', 
/* 7 */ 	'0',  '.',  '2',  '5',  '6',  '8',  '\0', '\0', '\0', '+',  '3',  '-',  '*',  '9',  '\0', '\0', 
};

// Map a VK to a character code
char key_to_char(uint8_t lo, uint8_t hi)
{
	bool shift = hi & 0x01;
	bool ctrl = hi & 0x02;

	// Not if control key
	if (ctrl)
		return 0;

	// Not if extended key
	if (lo & 0x80)
		return 0;

	if (shift)
	{
		return shift_keys[lo & 0x7f];
	}
	else
	{
		return normal_keys[lo & 0x7f];
	}

	return 0;
}
