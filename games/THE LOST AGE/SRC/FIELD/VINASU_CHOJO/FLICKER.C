#include "TYPES.H"
#include "FIELD_EVENT.H"

void Object_SetPartAttribute(void *object, s32 value);

extern u32 gFrameCount;

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
