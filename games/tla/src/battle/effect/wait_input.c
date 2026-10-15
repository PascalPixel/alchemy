#include "types.h"
#include "input.h"

void WaitFrames(s32 frames);

void Func_080d2084(void)
{
    u32 i;

    for (i = 0; i <= 119; i++)
    {
        WaitFrames(1);
        if (gInput.pressed != 0)
            break;
    }
}
