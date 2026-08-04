#include "types.h"

#define M2C_FIELD(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

struct Runtime_080b90f8 {
    u8 padding_00[0x45];
    u8 field_45;
    u8 field_46;
};

struct Creature_080b90f8 {
    u8 padding_00[0x0f];
    u8 field_0f;
};

s32 Func_080b6b40(s32 arg0, u16 *arg1);
struct Creature_080b90f8 *Func_08077008(s32);
s32 Func_080022ec(s32, s32);
u32 Func_08004458(void);
extern u8 Data_02000240[];

s32 Func_080b90f8(void) {
    s32 sp4;
    u8 *sp0;
    s16 sp8[14];
    s32 count;
    s32 total;
    s32 i;
    s32 accum;
    struct Runtime_080b90f8 *runtime;

    sp4 = 0;
    runtime = *(struct Runtime_080b90f8 **)0x03001E74;
    if (runtime->field_45 == 1) {
        sp4 = 1;
    } else {
        sp0 = &runtime->field_46;
        accum = 0x1388 + (runtime->field_46 * 0x7D0);
        count = Func_080b6b40(1, sp8);
        total = 0;
        for (i = sp4; i < count; i++) {
            total += Func_08077008((s32) sp8[i])->field_0f;
        }
        accum += Func_080022ec(total * 0x1F4, count);
        count = Func_080b6b40(2, sp8);
        total = 0;
        for (i = 0; i < count; i++) {
            total += Func_08077008((s32) sp8[i])->field_0f;
        }
        accum -= Func_080022ec(total * 0x1F4, count);
        if ((accum > 0) && ((u32) ((u32) (0x2710 * Func_08004458()) >> 0x10) < (u32) accum)) {
            sp4 = 1;
        }
        *sp0 += 1;
    }
    if (Data_02000240[0x22B] == 2) {
        sp4 = 0;
    }
    return sp4;
}
