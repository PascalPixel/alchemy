#include "SYSTEM.H"
#include "TYPES.H"
#include "IO_REG.H"

void Graphics_FadeAlphaIn(void)
{
    s32 step;

    REG_BLDCNT = 0x2044;
    for (step = 1; step <= 16; step += 2) {
        REG_BLDALPHA = step + 0x1000;
        WaitFrames(1);
    }
}
