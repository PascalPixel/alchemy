#include "TYPES.H"

/* Mode entries of the summon canvas effect that follows them, which opens
   a canvas layer and runs one of four variants. */

void SummonFx_RunCanvasEffect(void *action, s32 mode);

void SummonFx_RunCanvasMode0(void *action)
{
    SummonFx_RunCanvasEffect(action, 0);
}

void SummonFx_RunCanvasMode3(void *action)
{
    SummonFx_RunCanvasEffect(action, 3);
}

void SummonFx_RunCanvasMode1(void *action)
{
    SummonFx_RunCanvasEffect(action, 1);
}

void SummonFx_RunCanvasMode2(void *action)
{
    SummonFx_RunCanvasEffect(action, 2);
}
