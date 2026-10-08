#include "types.h"
#define FIELD_AT(base, type, off) (((struct { u8 pad[off]; type v; } *)(base))->v)

#include "call.h"
s32 Scene_SetArrivalFlags();
s32 Func_02000aa4();

s32 Func_0200099c(void)
{
    Call1(Scene_SetArrivalFlags, 0);
    Call1(Func_02000aa4, 0);
    return 0;
}

