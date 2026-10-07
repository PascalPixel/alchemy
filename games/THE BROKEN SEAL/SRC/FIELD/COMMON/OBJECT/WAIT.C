#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "TYPES.H"
#include "SYSTEM.H"
#include "OBJECT_RUNTIME.H"


void Script_WaitForEventTimeout(s32 arg0)
{
    s32 cnt;

    cnt = 0;
    while (cnt <= 0x257 && Object_IsTargetUnset((struct ObjectRuntime *)arg0) == 0) {
        WaitFrames(1);
        cnt++;
    }
}

s32 Object_IsTargetUnset(struct ObjectRuntime *object)
{
    s32 first;
    s32 second;

    if (object->flags == 0) {
        second = object->target_x;
        if (second != 0x80000000)
            return 0;
        first = object->target_y;
    } else {
        first = object->target_x;
        second = 0x80000000;
    }
    if (first != second || object->target_z != first)
        return 0;
    return 1;
}
