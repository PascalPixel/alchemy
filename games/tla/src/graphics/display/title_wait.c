#include "types.h"
#include "system.h"
#include "input.h"
#include "frame.h"
#include "bg_scroll.h"

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

struct TitleScrollStep {
    s32 frames;
    s32 speed[4];
    u32 mask[4];
};

s32 Title_RunScrollStep(struct TitleScrollStep *step)
{
    s32 i;

    for (i = 0; i < step->frames; i++) {
        if (step->mask[0] == 0 || (gFrameCount & step->mask[0]) == 0)
            gBgScroll[1].y += step->speed[0];
        if (step->mask[1] == 0 || (gFrameCount & step->mask[1]) == 0)
            gBgScroll[1].x += step->speed[1];
        if (step->mask[2] == 0 || (gFrameCount & step->mask[2]) == 0)
            gBgScroll[2].y += step->speed[2];
        if (step->mask[3] == 0 || (gFrameCount & step->mask[3]) == 0)
            gBgScroll[2].x += step->speed[3];
        if (Title_WaitFramesOrPress(1))
            return 0;
    }
    return 1;
}
