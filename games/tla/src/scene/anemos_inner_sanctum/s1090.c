#include "types.h"
#define FIELD_AT(base, type, off) (((struct { u8 pad[off]; type v; } *)(base))->v)

#include "call.h"
s32 GameFlag_ClearBit();

void Func_02001090(void)
{
    Call1(GameFlag_ClearBit, 0x200);
}

