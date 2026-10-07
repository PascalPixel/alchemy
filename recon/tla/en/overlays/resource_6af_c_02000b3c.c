#include "TYPES.H"
#define FIELD_AT(base, type, off) (((struct { u8 pad[off]; type v; } *)(base))->v)

#include "CALL.H"
extern u8 gPartyState[];
extern u8 MsgBlank[];
extern u8 MsgBlank2[];
extern u8 MsgBlank3[];
extern u8 MsgHat2[];

void Func_02000b3c(void)
{
    if (FIELD_AT(gPartyState, s16, 480) == (s32)MsgBlank) {
    } else {
        if (FIELD_AT(gPartyState, s16, 480) == (s32)MsgBlank2) {
        } else {
            if (FIELD_AT(gPartyState, s16, 480) == (s32)MsgBlank3) {
            } else {
                if (FIELD_AT(gPartyState, s16, 480) == (s32)MsgHat2) {
                } else {
                }
            }
        }
    }
}

