#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "BATTLE_EFFECT_WORK.H"
#include "HEAP_STATE.H"
#include "ANIMSPR.H"
#include "CANVAS.H"
#include "TYPES.H"
#include "SCENE.H"

#define gBattleFxWork (*(struct BattleEffectWork * *)&gBattleFxWork)

void BattleFx_SpawnObjects(s32 entry_count, s32 kind, u32 variant)
{
    struct BattleEffectWork *work = gBattleFxWork;
    s32 entry_index = 0;

    if (entry_count == 0) {
        return;
    }
    do {
        struct AnimationObject *object = ResourceObject_CreateFar(kind);

        work->objects[entry_index] = object;
        if (object != 0) {
            object->flags = 0;
            AnimationObjects_SelectAnimationFar(object, entry_index);
            ((struct AnimationObject *)work->objects[entry_index])->part[0].priority = variant;
        }
        entry_index++;
    } while (entry_index != entry_count);
}

void BattleFx_RunNoEffect(void)
{
}
