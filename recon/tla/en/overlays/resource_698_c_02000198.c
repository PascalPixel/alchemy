#include "types.h"
#define FIELD_AT(base, type, off) (((struct { u8 pad[off]; type v; } *)(base))->v)

#include "call.h"
s32 GameFlag_Test();
s32 Func_02001b3c();
extern u8 gPartyState[];
extern u8 MsgClothingResistsWater[];

s32 Func_02000198(void)
{
    s32 t0;

    FIELD_AT((FIELD_AT(0x3000000, s32, 108) + 0x1ac), s32, 0) = 0x100;
    t0 = Value1(GameFlag_Test, 0x109);
    if (t0 == 0) {
        if (FIELD_AT(gPartyState, s16, 482) == 4) {
            Call1(Func_02001b3c, FIELD_AT(gPartyState, s32, 532));
        }
    }
    FIELD_AT(gPartyState, u16, 484) = (s32)MsgClothingResistsWater;
    FIELD_AT(gPartyState, u16, 486) = 6;
    return 0;
}

