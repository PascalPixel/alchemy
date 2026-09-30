#include "TYPES.H"
#include "FIELD_EVENT.H"

/* The steps of the scene where Felix wakes on Idejima. */

/* The controller state the engine refreshes each frame. */
struct InputState {
    u32 held;
    u32 pressed;
};

extern struct InputState gInput;

/* Marks the start of each step of the scene where Felix wakes; does nothing. */
void FelixWake_BeginStep(void)
{
}

/* Marks the end of each step of the scene where Felix wakes; does nothing. */
void FelixWake_EndStep(void)
{
}

/* Holds the scene for up to three seconds, or until a button is pressed. */
void FelixWake_WaitForButton(void)
{
    s32 frames = 180;

    FelixWake_BeginStep();
wait:
    if (--frames != -1) {
        Task_Wait(1);
        if (gInput.pressed == 0) {
            goto wait;
        }
    }
    FelixWake_EndStep();
}
