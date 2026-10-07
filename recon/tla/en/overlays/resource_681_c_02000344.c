#include "TYPES.H"
#define FIELD_AT(base, type, off) (((struct { u8 pad[off]; type v; } *)(base))->v)

#include "CALL.H"
extern u8 gPartyState[];
extern u8 MsgAxeUnleashesMeltdown[];

void Func_02000344(void)
{
    if (FIELD_AT(gPartyState, s16, 480) == (s32)MsgAxeUnleashesMeltdown) {
    }
}

