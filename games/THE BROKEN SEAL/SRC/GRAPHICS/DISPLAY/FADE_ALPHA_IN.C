#include "SYSTEM.H"
#include "TYPES.H"



void Graphics_FadeAlphaIn(void)
{
    s32 step;

    *(s16 *)0x04000050 = 0x2044;
    for (step = 1; step <= 16; step += 2) {
        *(s16 *)0x04000052 = step + 0x1000;
        WaitFrames(1);
    }
}
