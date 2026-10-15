#include "types.h"
#define FIELD_AT(base, type, off) (((struct { u8 pad[off]; type v; } *)(base))->v)

#include "call.h"
s32 Func_02001958();

void Func_02000248(void)
{
    Call1(Func_02001958, 0);
}

