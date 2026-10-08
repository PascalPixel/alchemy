#include "types.h"
#define FIELD_AT(base, type, off) (((struct { u8 pad[off]; type v; } *)(base))->v)

#include "call.h"
extern u8 gPartyState[];
extern u8 MsgErase[];
extern u8 MsgRevive[];

void Func_020007e4(void)
{
    if (FIELD_AT(gPartyState, s16, 480) == (s32)MsgErase) {
    } else {
        if (FIELD_AT(gPartyState, s16, 480) == (s32)MsgRevive) {
        } else {
        }
    }
}

