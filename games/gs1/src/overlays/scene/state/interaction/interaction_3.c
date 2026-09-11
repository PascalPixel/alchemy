#include "scene.h"
#include "types.h"

/* overlays/scene/state/interaction/no_op.c */
/* Deliberate no-op callback. */
void Scene_NoOp(void) {}

/* overlays/scene/state/interaction/apply_three_rects_rows_9_and_10.c */
void State_ApplyThreeRectsRows9And10(void)
{
    s32 strip = 17;

    {
        s32 p5 = 80;
        s32 p6 = 9;

        State_SetRect(90, 9, 2, 3, p5, p6);
    }
    State_unk2_5(27, 10, 1, 2, strip, 10);
    State_unk3_5(17, 10, 1, 1, strip, 11);
}
