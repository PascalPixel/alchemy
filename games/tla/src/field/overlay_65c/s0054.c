#include "types.h"

void GameFlag_SetBit(s32 flag);
void Func_020001a0(s32 a, s32 b, s32 c);

void Func_02000054(void)
{
	s32 a1 = 13;
	s32 a2 = 14;

	/* FAKEMATCH: the native loads the later arguments before the first; the plain call loads the first earlier (differ 2). */
	asm volatile("" : "+l"(a1), "+l"(a2));
	Func_020001a0(1, a1, a2);
	GameFlag_SetBit(0x203);
}
