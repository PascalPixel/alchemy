#include "types.h"
#define FIELD_AT(base, type, off) (((struct { u8 pad[off]; type v; } *)(base))->v)

#include "call.h"
s32 GameFlag_Test();

void Func_0200004c(void)
{
    s32 t0;

    t0 = Value1(GameFlag_Test, 0x97f);
    if (t0 != 0) {
    } else {
    }
}

