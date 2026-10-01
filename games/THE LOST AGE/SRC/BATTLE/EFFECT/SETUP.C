#include "OBJECT_RUNTIME.H"

struct BattleAnimationState {
    u8 unknown_00[0x28];
    s16 *value_28;
};

struct BattleRenderObject {
    u8 unknown_00[0x50];
    struct BattleAnimationState *animation;
    u8 kind;
};

s32 BattleFx_GetAnimationValue(u32 object_id)
{
    struct BattleRenderObject *object =
        (struct BattleRenderObject *)ObjectTable_Get(object_id);

    if (object->kind != 1 ||
        object->animation == NULL ||
        object->animation->value_28 == NULL) {
        return 0;
    }
    return *object->animation->value_28;
}
