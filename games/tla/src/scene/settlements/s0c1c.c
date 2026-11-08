#include "local.h"

void Func_02000c1c(void) { GameFlag_SetBit(SPLIT_CONSTANT(0x102, 255)); }

void Func_02000c2c(void) { GameFlag_ClearBit(SPLIT_CONSTANT(0x102, 255)); }

void Func_02000c3c(void) { Func_02001770(); }
