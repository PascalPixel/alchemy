#include "battle_effect_runtime.h"

s32 Func_08091584(void)
{
    struct BattleRenderObject *object = Func_0808ba1c();

    if (object->kind != 1 ||
        object->animation == NULL ||
        object->animation->value_28 == NULL) {
        return 0;
    }
    return *object->animation->value_28;
}
