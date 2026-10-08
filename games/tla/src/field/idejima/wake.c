#include "types.h"
#include "input.h"
#include "field_event.h"

void FelixWake_BeginStep(void)
{
}

void FelixWake_EndStep(void)
{
}

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
