#include "TYPES.H"
#include "IWRAM_CALL.H"
#include "RAM_BUFFER.H"

struct DisplayTransitionState {
    u8 reserved_000[0x52a];
    u16 value;
    u8 reserved_52c[14];
    s8 start;
    s8 end;
    s8 duration;
    s8 step;
};

struct DisplayTransitionWindow {
    u8 reserved_000[0x100];
    u16 first_line;
    u16 second_line;
};

extern volatile u32 gFrameCount;

s32 Scheduler_RemoveCallback(void (*callback)(void));
void Runtime_SetIrqHandler(s32 irq, s32 flags, s32 handler);


/* Steps the transition value from start to end over duration frames; once
 * done it removes itself as a callback and clears the HBlank handler. Then
 * sets the window's two scanlines: 200 and 250 from a value of 80 up; on odd
 * frames with a nonzero value, 80 + value and 80 - value about the centre
 * row; otherwise 0 and 159, the whole screen. */
void DisplayTransition_UpdateFromCentre(void)
{
    struct DisplayTransitionState *state =
        *(struct DisplayTransitionState **)Ram_DisplayWork;
    struct DisplayTransitionWindow *window =
        *(struct DisplayTransitionWindow **)Ram_MapWork;
    s8 *duration = &state->duration;
    u16 value;

    if (*duration != 0) {
        s8 *step = &state->step;

        if (*step >= *duration) {
            *duration = 0;
            Scheduler_RemoveCallback(DisplayTransition_UpdateFromCentre);
            Runtime_SetIrqHandler(1, 0, 0);
            return;
        } else {
            s32 delta = state->end - state->start;
            s32 offset;

            (*step)++;
            offset = Iwram_SignedDivide(delta * *step, *duration);
            state->value = state->start + offset;
        }
    }

    value = state->value;
    if (value > 79) {
        window->first_line = 200;
        window->second_line = 250;
    } else if (value != 0 && (gFrameCount & 1)) {
        window->first_line = value + 80;
        window->second_line = 80 - value;
    } else {
        window->first_line = 0;
        window->second_line = 159;
    }
}
