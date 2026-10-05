#include "TYPES.H"
#include "SYSTEM.H"
#include "INPUT.H"
#include "FRAME.H"
#include "BG_SCROLL.H"

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

/* One step of the title's background scroll: how many frames it runs, the
   amount BG1 and BG2 move down and across each frame, and for each a mask
   of the frames it skips (0 for none). */
struct TitleScrollStep {
    s32 frames;
    s32 speed[4];
    u32 mask[4];
};

/* Runs one scroll step; returns 0 if a button cut it short, else 1. */
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
