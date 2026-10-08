#include "types.h"
#include "object_runtime.h"

struct ObjectAnimationState {
    u8 unknown_00[0x28];
    s16 *current;
};

s32 Object_IsOutsideAnimation108(struct ObjectRuntime *object)
{
    struct ObjectAnimationState *state;

    if (object != 0 && object->animation_kind == 1) {
        state = object->animation;
        if (state != 0) {
            if (state->current == 0)
                return 1;
            return (*state->current ^ 0x108) != 0;
        }
    }
    return 1;
}
