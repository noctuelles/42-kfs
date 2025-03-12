#include "VGATerminal.hpp"

static inline uint8_t inb(uint16_t port)
{
	uint8_t data{};

	asm volatile("inb %1, %0" : "=a"(data) : "d"(port) : "memory");

	return data;
}

extern "C" void kernel_main(void)
{
	VGATerminal term{};

	// for (size_t y = 0; y < VGATerminal::HEIGHT; y++) {
	// 	char buff[VGATerminal::WIDTH + 1]{};

	// 	for (size_t i = 0; i < VGATerminal::WIDTH; i++) {
	// 		buff[i] = 'A' + y;
	// 	}

	// 	term.putString(buff);
	// }

	while (1)
	{
		while ((inb(0x64) & 0x1) == 0)
		{
		}

		char scancode_map[128];
		scancode_map[0x1E] = 'A';
		scancode_map[0x30] = 'B';
		scancode_map[0x2E] = 'C';
		scancode_map[0x20] = 'D';
		scancode_map[0x12] = 'E';
		scancode_map[0x21] = 'F';
		scancode_map[0x22] = 'G';
		scancode_map[0x23] = 'H';
		scancode_map[0x17] = 'I';
		scancode_map[0x24] = 'J';
		scancode_map[0x25] = 'K';
		scancode_map[0x26] = 'L';
		scancode_map[0x32] = 'M';
		scancode_map[0x31] = 'N';
		scancode_map[0x18] = 'O';
		scancode_map[0x19] = 'P';
		scancode_map[0x10] = 'Q';
		scancode_map[0x13] = 'R';
		scancode_map[0x14] = 'S';
		scancode_map[0x1F] = 'T';
		scancode_map[0x16] = 'U';
		scancode_map[0x2F] = 'V';
		scancode_map[0x11] = 'W';
		scancode_map[0x2D] = 'X';
		scancode_map[0x15] = 'Y';
		scancode_map[0x2C] = 'Z';
		scancode_map[0x02] = '1';
		scancode_map[0x03] = '2';
		scancode_map[0x04] = '3';
		scancode_map[0x05] = '4';
		scancode_map[0x06] = '5';
		scancode_map[0x07] = '6';
		scancode_map[0x08] = '7';
		scancode_map[0x09] = '8';
		scancode_map[0x0A] = '9';
		scancode_map[0x0B] = '0';
		scancode_map[0x39] = ' '; // Space

		uint8_t scan_code{inb(0x60)};
		char c{scancode_map[scan_code]};

		if ((c >= 'A' && c <= 'Z') || c == ' ') {
			term.putChar(scancode_map[scan_code]);
		}

	}

	return;
}
