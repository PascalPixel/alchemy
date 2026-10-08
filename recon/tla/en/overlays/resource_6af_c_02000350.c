#include "types.h"
#define FIELD_AT(base, type, off) (((struct { u8 pad[off]; type v; } *)(base))->v)

#include "call.h"
extern u8 gPartyState[];
extern u8 MsgScene665No19[];
extern u8 MsgScene682No52[];
extern u8 MsgBlank5[];
extern u8 MsgHat2[];

void Func_02000350(void)
{
    if (FIELD_AT(gPartyState, s16, 480) == (s32)MsgScene665No19) {
    } else {
        if (FIELD_AT(gPartyState, s16, 480) == (s32)MsgScene682No52) {
        } else {
            if (FIELD_AT(gPartyState, s16, 480) == (s32)MsgBlank5) {
            } else {
                if (FIELD_AT(gPartyState, s16, 480) == (s32)MsgHat2) {
                } else {
                }
            }
        }
    }
}

