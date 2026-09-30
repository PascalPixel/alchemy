#include "TYPES.H"
#include "FIXED_MATH.H"

extern volatile u8 gBlendDuration;

void Blend_WaitForTransition(void)
{
    if (gBlendDuration != 0) {
        do {
            WaitFrames(1);
        } while (gBlendDuration != 0);
    }
}
