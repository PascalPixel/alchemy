#include "types.h"
#define FIELD_AT(base, type, off) (((struct { u8 pad[off]; type v; } *)(base))->v)

#include "call.h"
s32 Scheduler_RemoveCallbackFar();
extern u8 Func_020001f8[];

void Func_02000158(void)
{
    FIELD_AT((FIELD_AT(0x3000000, s32, 108) + 0x1ac), s32, 0) = 0x100;
    Call1(Scheduler_RemoveCallbackFar, (s32)Func_020001f8);
}

