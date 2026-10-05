#include "TYPES.H"
#include "SYSTEM.H"
#include "INPUT.H"

/* Waits up to frames frames, stirring the random number each frame;
   returns 1 as soon as a button is pressed, else 0. */
s32 Title_WaitFramesOrPress(u32 frames)
{
    u32 i;

    for (i = 0; i < frames; i++) {
        WaitFrames(1);
        Random16();
        if (gInput.pressed != 0)
            return 1;
    }
    return 0;
}
