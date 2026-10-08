#include "types.h"
#define FIELD_AT(base, type, off) (((struct { u8 pad[off]; type v; } *)(base))->v)

#include "call.h"
s32 Func_02003cb8();
s32 Func_02000468();
extern u8 Data_02003d94[];

void Func_02001ca4(void)
{
    Call1(Func_02003cb8, (s32)Data_02003d94);
    Call1(Func_02000468, FIELD_AT((FIELD_AT(0x3000000, s32, 108) + 0xcbc), s32, 0));
}

