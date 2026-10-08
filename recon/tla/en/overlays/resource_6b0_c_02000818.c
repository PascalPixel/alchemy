#include "types.h"
#define FIELD_AT(base, type, off) (((struct { u8 pad[off]; type v; } *)(base))->v)

#include "call.h"
extern u8 gPartyState[];
extern u8 MsgHat2[];
extern u8 MsgHat[];
extern u8 MsgCrownBoostsLuck[];
extern u8 MsgHatBoostsAgility[];
extern u8 MsgCrownBoostsCriticalsRestoresPp[];

void Func_02000818(void)
{
    if (FIELD_AT(gPartyState, s16, 480) == (s32)MsgHat2) {
    } else {
        if (FIELD_AT(gPartyState, s16, 480) == (s32)MsgHat) {
        } else {
            if (FIELD_AT(gPartyState, s16, 480) == (s32)MsgCrownBoostsLuck) {
            } else {
                if (FIELD_AT(gPartyState, s16, 480) == (s32)MsgHatBoostsAgility) {
                } else {
                    if (FIELD_AT(gPartyState, s16, 480) == (s32)MsgCrownBoostsCriticalsRestoresPp) {
                    } else {
                    }
                }
            }
        }
    }
}

