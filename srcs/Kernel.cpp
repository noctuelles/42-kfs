#include "VGATerminal.hpp"

extern "C" void kernel_main(void) 
{
	VGATerminal term{};

	for (size_t y = 0; y < VGATerminal::HEIGHT; y++) {
		char buff[VGATerminal::WIDTH + 1]{};

		for (size_t i = 0; i < VGATerminal::WIDTH; i++) {
			buff[i] = 'A' + y;
		}

		term.putString(buff);
	}
}
