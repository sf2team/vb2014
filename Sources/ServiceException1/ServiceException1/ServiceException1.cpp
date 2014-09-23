// ServiceException1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "windows.h"
#include <stdio.h>

int _tmain(int argc, _TCHAR* argv[])
{
	printf("Verifying that a pattern left on the stack is not smashed by SMC.\n");

	DWORD  oldProt;
	VirtualProtect(_tmain, 8192, PAGE_EXECUTE_READWRITE, &oldProt);

	unsigned int pattern1, pattern2, word1, word2;

	_asm {
		push 0xdeadbeef
		push 0xbadcafe1
		pop pattern2
		pop pattern1
		mov byte ptr smc, 0x90
smc:
		_emit 0xcc
		sub esp, 8
		pop word2
		pop word1
	}

	printf("* [ESP-4] before SMC: %x\n", pattern1);
	printf("* [ESP-8] before SMC: %x\n", pattern2);
	printf("* [ESP-4] after SMC: %x\n", word1);
	printf("* [ESP-8] after SMC: %x\n", word2);

	if (pattern1 == word1 && pattern2 == word2) {
		printf("Test passed.\n");
		return 0;
	}

	printf("Test failed.\n");
	return 1;
}

