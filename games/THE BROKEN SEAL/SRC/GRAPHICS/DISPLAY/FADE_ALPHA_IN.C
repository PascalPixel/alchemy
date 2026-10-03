#include "TYPES.H"

s32 WaitFrames(s32);

void Graphics_FadeAlphaIn(void)
{
    s32 step;

    *(s16 *)0x04000050 = 0x2044;
    step = 1;
    do {
        *(s16 *)0x04000052 = step + 0x1000;
        step += 2;
        WaitFrames(1);
    } while (step <= 16);
}
