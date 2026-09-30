#include "TYPES.H"

s32 Scheduler_EnableCallbacks(u32 callback);
void Object_EffectSpawnCallback(void);

s32 BattleFx_GetAnimationValue(void)
{
    struct BattleRenderObject *object = ObjectTable_Get();

    if (object->kind != 1 ||
        object->animation == NULL ||
        object->animation->value_28 == NULL) {
        return 0;
    }
    return *object->animation->value_28;
}
