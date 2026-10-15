#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "scene.h"
#include "types.h"
#include "object_lookup.h"
#include "fixed_math.h"
#include "object_effect.h"
#include "system.h"
#include "field_event.h"

s32 Object_GetById(u32);

s32 BattleFx_FindNearestObject(s32 self, s32 effect)
{
    s32 nearest = 9999;
    s32 found = -1;
    s32 i;

    for (i = 0; i <= 80; i++)
    {
        if (i != self)
        {
            s32 distance = Func_080cdac0(effect, self, i);

            if (distance != -1 && nearest > distance)
            {
                found = i;
                nearest = distance;
            }
        }
    }
    return found;
}

s32 BattleFx_PlayCueAndStartEmitterOnTarget(s32 effect, s32 target, s32 mode)
{
    s32 object;
    s32 result;

    object = Object_GetById(target);
    result = 0;
    if (object != 0)
    {
        Audio_PlayCue(0x7C);
        Object_SetMode(object, 4);
        WaitFrames(0xC);
        result = BattleFx_StartRandomParticleEmitter(effect, mode);
    }
    return result;
}
