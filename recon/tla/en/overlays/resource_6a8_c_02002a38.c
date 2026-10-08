#include "types.h"
#define FIELD_AT(base, type, off) (((struct { u8 pad[off]; type v; } *)(base))->v)

#include "call.h"
s32 GameFlag_Test();
s32 Func_02001c24();
s32 Func_020045dc();
s32 Scheduler_AddOrUpdateCallback();
extern u8 gPartyState[];
extern u8 Func_02002a08[];

void Func_02002a38(void)
{
    s32 t0;

    t0 = Value1(GameFlag_Test, 0x109);
    if (t0 == 0) {
        if (FIELD_AT(gPartyState, s16, 482) == 6) {
            Call3(Func_02001c24, 0, 0, FIELD_AT(gPartyState, u16, 482));
        } else {
            if ((u32)((FIELD_AT(gPartyState, s16, 482) - 7) << 16) <= 0x10000) {
                Call1(Func_02001c24, 1);
            }
        }
    }
    Call1(Func_020045dc, 0);
    Scheduler_AddOrUpdateCallback((s32)Func_02002a08, 0x480);
}

