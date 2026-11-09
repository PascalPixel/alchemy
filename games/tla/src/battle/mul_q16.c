#include "types.h"
#include "io_reg.h"
#include "iwram_call.h"

void Func_081269ac(void) { REG_BLDCNT = 0xbf; }

s32 Func_081269bc(s32 a, s32 b) { return Iwram_MulQ16(a, b); }
