#include "types.h"
#define FIELD_AT(base, type, off) (((struct { u8 pad[off]; type v; } *)(base))->v)

#include "call.h"
s32 Object_GetById();
extern u8 gPartyState[];

s32 Func_02000644(void)
{
    s32 t0;

    t0 = Value1(Object_GetById, FIELD_AT(gPartyState, s32, 532));
    if ((u32)((FIELD_AT(t0, u16, 6) + 0x5fff) << 16) > 0x3ffe0000) {
    }
    return 0;
}

