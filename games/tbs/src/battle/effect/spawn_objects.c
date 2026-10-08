#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "battle_effect_work.h"
#include "heap_state.h"
#include "animspr.h"
#include "canvas.h"
#include "types.h"
#include "scene.h"

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
