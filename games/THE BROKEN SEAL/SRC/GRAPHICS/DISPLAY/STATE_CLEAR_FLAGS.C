#include "TYPES.H"
#include "IWRAM_CALL.H"
#include "SCENE.H"
#include "GLOBAL_CELLS.H"
#include "RAM_BUFFER.H"
extern u8 Data_03001e70[];

/* display/state/clear_flags.c */
void DisplayState_ClearFlags(s32 clear_0800, s32 clear_0400, s32 clear_0200)
{
    void *state;

    state = *(void **)((u32)&Data_03001e70);
    if (state != NULL) {
        if (clear_0200 != 0) {
            FIELD_AT_OFFSET(state, u16 *, 0x14) &= 0xFDFF;
        }
        if (clear_0400 != 0) {
            FIELD_AT_OFFSET(state, u16 *, 0x14) &= 0xFBFF;
        }
        if (clear_0800 != 0) {
            FIELD_AT_OFFSET(state, u16 *, 0x14) &= 0xF7FF;
        }
    }
}

/* display/transition/update.c */
/* Signed division runs from IWRAM through the register-call veneer. */

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

extern s32 Scheduler_RemoveCallback(void (*)(void));

void DisplayTransition_Update(void)
{
    struct DisplayTransitionState *state =
        *(struct DisplayTransitionState **)Ram_DisplayWork;
    struct DisplayTransitionRegisters *display =
        *(struct DisplayTransitionRegisters **)Ram_MapWork;
    s8 *duration = &state->transition_duration;
    u32 display_value;

    if (*duration != 0) {
        s8 *step = &state->transition_step;

        if (*step >= *duration) {
            *duration = 0;
            Scheduler_RemoveCallback(DisplayTransition_Update);
            Runtime_SetIrqHandler(1, 0, 0);
            return;
        } else {
            s32 delta = state->transition_end - state->transition_start;
            s32 value;

            (*step)++;
            value = Iwram_SignedDivide(delta * *step, *duration);
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
