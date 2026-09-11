#include "types.h"
#include "scene.h"
#include "abi/overlays/scene/state/interaction/apply_three_rects_rows_9_and_10.h"

void State_ApplyThreeRectsRows9And10(void)
{
    s32 strip = 17;

    {
        s32 p5 = 80;
        s32 p6 = 9;

        State_SetRect(90, 9, 2, 3, p5, p6);
    }
    State_SetRect2(27, 10, 1, 2, strip, 10);
    State_SetRect3(17, 10, 1, 1, strip, 11);
}
