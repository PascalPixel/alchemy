#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "LIB.H"
#include "MENU.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "TYPES.H"
#include "FIELD_EVENT.H"

#define Object_SetPartAttribute ((void (*)(void*,s32))Object_SetPartAttribute)


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
