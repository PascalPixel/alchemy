#include "types.h"

struct State_080fa798 {
    u8 padding00[8];
    u8 mode;
    u8 padding09[2];
    u8 scale;
    u8 padding0c[4];
    u32 frequency;
    s32 period;
    s32 half_period;
};

extern u16 Data_080fb914[];

s32 Func_080022ec(s32 numerator, s32 denominator);
void Func_080fa9a4(void);

void Func_080fa798(u32 value)
{
    struct State_080fa798 *state =
        *(struct State_080fa798 **)0x03007ff0;
    u16 frequency;
    s32 period;
    s32 zero;
    volatile u16 *timer;

    value = (value & 0x000f0000) >> 16;
    zero = 0;
    state->mode = value;
    frequency = Data_080fb914[value - 1];
    state->frequency = frequency;
    state->scale = Func_080022ec(0x630, frequency);
    period = Func_080022ec(0x91d1b * frequency + 0x1388, 0x2710);
    state->period = period;
    state->half_period = (Func_080022ec(0x01000000, period) + 1) >> 1;
    *(volatile u16 *)0x04000102 = zero;
    timer = (volatile u16 *)0x04000100;
    *timer = -Func_080022ec(0x44940, frequency);
    Func_080fa9a4();
    while (*(volatile u8 *)0x04000006 == 0x9f) {
    }
    while (*(volatile u8 *)0x04000006 != 0x9f) {
    }
    *(volatile u16 *)0x04000102 = 0x80;
}
