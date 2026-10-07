#include "TYPES.H"
#define FIELD_AT(base, type, off) (((struct { u8 pad[off]; type v; } *)(base))->v)

#include "CALL.H"
extern u8 gPartyState[];
extern u8 MsgShieldResistsFire[];
extern u8 MsgShieldUseToRestoreHp[];
extern u8 MsgBlank[];
extern u8 MsgBlank2[];
extern u8 MsgGloves[];
extern u8 MsgGloves2[];

void Func_02000598(void)
{
    if (FIELD_AT(gPartyState, s16, 480) == (s32)MsgShieldResistsFire) {
    } else {
        if (FIELD_AT(gPartyState, s16, 480) == (s32)MsgShieldUseToRestoreHp) {
        } else {
            if (FIELD_AT(gPartyState, s16, 480) == (s32)MsgBlank) {
            } else {
                if (FIELD_AT(gPartyState, s16, 480) == (s32)MsgBlank2) {
                } else {
                    if (FIELD_AT(gPartyState, s16, 480) == (s32)MsgGloves) {
                    } else {
                        if (FIELD_AT(gPartyState, s16, 480) == (s32)MsgGloves2) {
                        } else {
                        }
                    }
                }
            }
        }
    }
}

