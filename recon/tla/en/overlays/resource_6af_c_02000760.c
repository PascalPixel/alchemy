#include "types.h"
#define FIELD_AT(base, type, off) (((struct { u8 pad[off]; type v; } *)(base))->v)

#include "call.h"
s32 Object_GetById();
s32 GameFlag_SetBit();
s32 Func_02000728();
extern u8 gPartyState[];
extern u8 MsgHelmBoostsPp[];
extern u8 MsgScene665No19[];
extern u8 MsgScene682No52[];
extern u8 MsgBlank5[];

void Func_02000760(s32 a0, s32 a1)
{
    s32 t0;

    t0 = Value1(Object_GetById, a1);
    if (FIELD_AT(gPartyState, s16, 480) == (s32)MsgHelmBoostsPp) {
        if (a1 == 8) {
            if ((FIELD_AT(t0, s32, 8) >> 20) == 0x10) {
                Call1(GameFlag_SetBit, 0x300);
            }
        }
        if (a1 == 9) {
            if ((FIELD_AT(t0, s32, 8) >> 20) == 0x14) {
                Call1(GameFlag_SetBit, 0x301);
            }
        }
        if (a1 == 0xa) {
            if ((FIELD_AT(t0, s32, 8) >> 20) == 0x1b) {
                Call1(GameFlag_SetBit, 0x302);
            }
        }
    }
    if (FIELD_AT(gPartyState, s16, 480) == (s32)MsgScene665No19) {
        if (a1 == 0xa) {
            if ((FIELD_AT(t0, s32, 8) >> 19) == 0x21) {
                Call1(GameFlag_SetBit, 0x308);
            }
        }
        if (a1 == 0xb) {
            if ((FIELD_AT(t0, s32, 8) >> 19) == 0x55) {
                if ((FIELD_AT(t0, s32, 16) >> 19) == 0x53) {
                    Func_02000728();
                }
            }
        }
    }
    if (FIELD_AT(gPartyState, s16, 480) == (s32)MsgScene682No52) {
        if (a1 == 0xa) {
            if ((FIELD_AT(t0, s32, 8) >> 19) == 0x53) {
                Call1(GameFlag_SetBit, 0x309);
            }
        }
    }
    if (FIELD_AT(gPartyState, s16, 480) == (s32)MsgBlank5) {
        if (a1 == 8) {
            if ((FIELD_AT(t0, s32, 8) >> 19) == 0x57) {
                Call1(GameFlag_SetBit, 0x30d);
            }
        }
    }
}

