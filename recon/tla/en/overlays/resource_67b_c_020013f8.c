// differ 6: the listing loads the sprite right after render_mode and its
// attribute byte before lsrs; the draft loads both two instructions later.
#include "../../../../games/tla/src/field/overlay_67b/local.h"

void Func_020013f8(void)
{
    struct ObjectRuntime *object = Object_GetById(Func_02002a68());

    if (object != 0 && (object->animation_kind & 15) == 1) {
        struct FieldSprite *sprite = object->animation;
        s32 priority = gPartyState.render_mode == 2 ? 2 : 1;

        sprite->priority = priority;
        sprite->part_priority = priority;
        object->priority_flags &= 0xfe;
    }
}
