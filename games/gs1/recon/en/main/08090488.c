#include "types.h"

struct DisplayTransitionState {
    u8 pad_000[0x52a];
    u16 transition_value;
    u8 pad_52c[14];
    s8 transition_start;
    s8 transition_end;
    s8 transition_duration;
    s8 transition_step;
};

struct DisplayTransitionRegisters {
    u8 pad_000[0x100];
    u16 primary_value;
    u16 secondary_value;
};

extern u8 Data_00000050[];

extern s32 Func_08004278(void *);
extern void Func_0800307c(s32, s32, s32);
extern s32 Func_080072f0(s32, s32, s32, s32);

#define UpdateMapTransition Func_08090488

void UpdateMapTransition(void)
{
    struct DisplayTransitionState *state =
        *(struct DisplayTransitionState **)0x03001ECC;
    struct DisplayTransitionRegisters *display =
        *(struct DisplayTransitionRegisters **)0x03001E70;
    s8 *duration = &state->transition_duration;
    u32 display_value;

    if (*duration != 0) {
        s8 *step = &state->transition_step;

        if (*step >= *duration) {
            *duration = 0;
            Func_08004278((void *)UpdateMapTransition);
            Func_0800307c(1, 0, 0);
            return;
        } else {
            s32 delta = state->transition_end - state->transition_start;
            s32 value;

            (*step)++;
            value = Func_080072f0(
                delta * *step, *duration, delta, 0x03000380);
            state->transition_value = state->transition_start + value;
        }
    }

    display_value = state->transition_value;
    if (display_value > 79) {
        display->primary_value = 200;
        display->secondary_value = 250;
    } else if (display_value != 0 && (*(u32 *)0x03001E40 & 1)) {
        display->primary_value = display_value + 80;
        display->secondary_value = (u32)Data_00000050 - display_value;
    } else {
        display->primary_value = 0;
        display->secondary_value = 159;
    }
}
