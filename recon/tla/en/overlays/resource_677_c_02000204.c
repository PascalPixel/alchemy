#include "types.h"
#define FIELD_AT(base, type, off) (((struct { u8 pad[off]; type v; } *)(base))->v)

#include "call.h"
extern u8 gPartyState[];
extern u8 MsgNoWay[];
extern u8 MsgILlDoIt[];

void Func_02000204(void)
{
    if (FIELD_AT(gPartyState, s16, 480) == (s32)MsgNoWay) {
    } else {
        if (FIELD_AT(gPartyState, s16, 480) == (s32)MsgILlDoIt) {
        } else {
        }
    }
}

