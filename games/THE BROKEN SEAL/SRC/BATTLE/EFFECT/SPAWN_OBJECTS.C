#include "BATTLE_EFFECT_WORK.H"
#include "HEAP_STATE.H"
#include "ANIMSPR.H"
#include "CANVAS.H"
#include "TYPES.H"
#include "SCENE.H"

/* battle/effects/common/spawn_objects.c */

struct AnimationObject *GetBattleEffectObject(s32);
s32 AnimationObjects_SelectAnimationFar(struct AnimationObject *, s32);

void BattleFx_SpawnObjects(s32 entry_count, s32 kind, u32 variant)
{
    struct BattleEffectWork *work = ((union HeapState *)gWorkSlot)->slots[HEAP_SLOT_BATTLE_EFFECT];
    s32 entry_index = 0;


    if (entry_count == 0) {
        return;
    }
    do {
        struct AnimationObject *object = GetBattleEffectObject(kind);

        work->objects[entry_index] = object;
        if (object != 0) {
            object->flags = 0;
            AnimationObjects_SelectAnimationFar(object, entry_index);
            /* The variant is in the low two attribute bits, written as a byte. */
            ((u8 *)&((struct AnimationObject *)work->objects[entry_index])->part[0])[9] =
                (((u8 *)&((struct AnimationObject *)work->objects[entry_index])->part[0])[9] & ~12) | ((variant & 3) << 2);
        }
        entry_index++;
    } while (entry_index != entry_count);
}

/* battle/effects/common/no_effect.c */
void BattleFx_RunNoEffect(void)
{
}
