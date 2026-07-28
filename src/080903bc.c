#include "types.h"

struct State_080903bc {
    u8 pad_000[0x52a];
    u16 value_52a;
    u8 pad_52c[14];
    s8 start_53a;
    s8 end_53b;
    s8 duration_53c;
    s8 step_53d;
};

struct Display_080903bc {
    u8 pad_000[0x100];
    u16 first_100;
    u16 second_102;
};

extern void Func_08004278(void (*)(void));
extern void Func_0800307c(s32, s32, s32);
extern s32 Func_080072f0(s32, s32, s32, s32);

void Func_080903bc(void)
{
    struct State_080903bc *state =
        *(struct State_080903bc **)0x03001ecc;
    struct Display_080903bc *display =
        *(struct Display_080903bc **)0x03001e70;
    s8 *duration = &state->duration_53c;
    u32 displayValue;

    if (*duration != 0) {
        s8 *step = &state->step_53d;

        if (*step >= *duration) {
            *duration = 0;
            Func_08004278(Func_080903bc);
            Func_0800307c(1, 0, 0);
            return;
        } else {
            s32 delta = state->end_53b - state->start_53a;
            s32 value;

            (*step)++;
            value = Func_080072f0(
                delta * *step, *duration, delta, 0x03000380);
            state->value_52a = state->start_53a + value;
        }
    }

    displayValue = state->value_52a;
    if (displayValue > 79) {
        display->first_100 = 200;
        display->second_102 = 250;
    } else {
        display->first_100 = displayValue;
        display->second_102 = 159 - displayValue;
    }
}
