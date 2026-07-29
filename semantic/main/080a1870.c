#include "types.h"

#define FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

extern u8 *Data_03001f2c;

s32 Func_08077158(u16 *out);
s32 Func_0808a288(u16 value);
void *Func_08009030(s32 value);
void Func_08009020(void *object, s32 value);
void Func_080041d8(void (*callback)(void), s32 value);
void Func_080a19a0(void);

void Func_080a1870(void *arg0, s32 arg1, s32 arg2, s32 arg3)
{
    u16 values[14];
    u8 *state = Data_03001f2c;
    s32 count = (u16)Func_08077158(values);
    s32 i;
    s32 mask1;
    s8 mask2;
    u8 flags;

    state[0x1e] = count;
    for (i = 0; i < count; i++) {
        void *object = Func_08009030(Func_0808a288(values[i]));
        if (object != 0) {
            s32 x;
            s32 base;
            FIELD(state, void **, 0x114 + i * 4) = object;
            base = FIELD(arg0, u16 *, 0xc);
            x = arg3;
            x += 16;
            x *= i;
            base = arg1 + base;
            FIELD(state, u16 *, 0x134 + i * 2) = base * 8 + x;
            FIELD(state, u16 *, 0x144 + i * 2) =
                (arg2 + FIELD(arg0, u16 *, 0xe)) * 8 + 16;
            FIELD(state, u32 *, 0x154 + i * 4) = 0x10000;
            mask1 = 13;
            flags = FIELD(object, u8 *, 9);
            mask1 = -mask1;
            mask2 = mask1;
            flags &= mask2;
            FIELD(object, u8 *, 9) = flags;
            FIELD(object, u8 *, 38) = 0;
            Func_08009020(object, 1);
        }
    }
    for (; i < 8; i++) {
        FIELD(state, void **, 0x114 + i * 4) = 0;
    }
    {
        s32 delay = 200;
        delay <<= 4;
        Func_080041d8(Func_080a19a0, delay);
    }
}
