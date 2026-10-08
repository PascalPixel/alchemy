#include "types.h"
#define FIELD_AT(base, type, off) (((struct { u8 pad[off]; type v; } *)(base))->v)

#include "call.h"
extern u8 gPartyState[];
extern u8 MsgAxe[];
extern u8 MsgAxe2[];
extern u8 MsgAxeUnleashesHeatMirage[];
extern u8 MsgAxeUnleashesMeltdown[];

void Func_020009e8(void)
{
    if (FIELD_AT(gPartyState, u16, 480) == (s32)MsgAxe) {
    } else {
        if (FIELD_AT(gPartyState, s16, 480) == (s32)MsgAxe2) {
        } else {
            if (FIELD_AT(gPartyState, s16, 480) == (s32)MsgAxeUnleashesHeatMirage) {
            } else {
                if (FIELD_AT(gPartyState, s16, 480) == (s32)MsgAxeUnleashesMeltdown) {
                } else {
                    if (FIELD_AT(gPartyState, s16, 482) > 8) {
                    } else {
                    }
                }
            }
        }
    }
}

