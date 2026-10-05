#include "TYPES.H"

void Object_SetPartAttribute(void *object, s32 value);
void Engine_AudioPlayCue(s32 cue);

extern u32 gFrameCount;

/* Flickers an object's part every fourth frame and plays cue 138 every
   eighth. */
s32 OverlayObject_UpdateEveryFourFrames(void *obj)
{
    if ((gFrameCount & 3) == 0)
        Object_SetPartAttribute(obj, 7);
    else
        Object_SetPartAttribute(obj, 0);

    if ((gFrameCount & 7) == 0)
        Engine_AudioPlayCue(138);
    return 0;
}
