/* Signed division runs from IWRAM through the register-call veneer. */
#include "types.h"
#include "gs1_edition.h"

#if defined(GS1_EDITION_DE)
#define STATE_CELL_ADDR 0x03001EDC
#define DISPLAY_CELL_ADDR 0x03001E80
#else
#define STATE_CELL_ADDR 0x03001ECC
#define DISPLAY_CELL_ADDR 0x03001E70
#endif

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

extern s32 ScheduleCallback(void (*)(void));
extern void Func_0800307c(s32, s32, s32);
typedef s32 (*SignedDivide)(s32, s32);

void DisplayTransition_Update(void)
{
    struct DisplayTransitionState *state =
        *(struct DisplayTransitionState **)STATE_CELL_ADDR;
    struct DisplayTransitionRegisters *display =
        *(struct DisplayTransitionRegisters **)DISPLAY_CELL_ADDR;
    s8 *duration = &state->transition_duration;
    u32 display_value;

    if (*duration != 0) {
        s8 *step = &state->transition_step;

        if (*step >= *duration) {
            *duration = 0;
            ScheduleCallback(DisplayTransition_Update);
            Func_0800307c(1, 0, 0);
            return;
        } else {
            s32 delta = state->transition_end - state->transition_start;
            s32 value;

            (*step)++;
            value = ((SignedDivide)0x03000380)(delta * *step, *duration);
            state->transition_value = state->transition_start + value;
        }
    }

    display_value = state->transition_value;
    if (display_value > 79) {
        display->primary_value = 200;
        display->secondary_value = 250;
    } else {
        display->primary_value = display_value;
        display->secondary_value = 159 - display_value;
    }
}
