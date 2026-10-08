#include "types.h"
#define FIELD_AT(base, type, off) (((struct { u8 pad[off]; type v; } *)(base))->v)

#include "call.h"
s32 Func_02002304();
extern u8 gPartyState[];

void Func_020006dc(void)
{
    if (FIELD_AT(gPartyState, u8, 530) == 0xa) {
        Call4(Func_02002304, 0x1500000, 0x2500000, 0, 0xff);
    }
}

