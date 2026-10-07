#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "LIB.H"
#include "MENU.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "SCENE.H"
#include "TYPES.H"
#include "OBJECT_LOOKUP.H"
#include "FIXED_MATH.H"
#include "OBJECT_EFFECT.H"
#include "SYSTEM.H"
#include "FIELD_EVENT.H"

s32 Object_GetById(u32);

s32 BattleFx_FindNearestObject(s32 self, s32 effect)
{
    s32 nearest = 9999;
    s32 found = -1;
    s32 i;

    for (i = 0; i <= 80; i++) {
        if (i != self) {
            s32 distance = Func_080cdac0(effect, self, i);

            if (distance != -1 && nearest > distance) {
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
    if (object != 0) {
        Audio_PlayCue(124);
        Object_SetMode(object, 4);
        WaitFrames(12);
        result = BattleFx_StartRandomParticleEmitter(effect, mode);
    }
    return result;
}
