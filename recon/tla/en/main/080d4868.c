/*
 * Draft: Object_IsOutsideAnimation108 does not yet match; the reference
 * computes x != 0x108 branch-free (eors/negs/orrs/lsrs) after an early
 * return of 1 for a null current frame; this version branches.
 * Links as its recon/tla/raw listing.
 */
#include "TYPES.H"
#include "OBJECT_RUNTIME.H"

struct ObjectAnimationState {
    u8 unknown_00[0x28];
    s16 *current;
};

/* Whether an object is not playing animation 0x108; an object without a
   sprite animation counts as not playing it. */
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
