#include "types.h"
#include "animspr.h"
#include "object_runtime.h"
#include "frame.h"

void Vector_AddPolarOffset(s32 radius, s32 angle, s32 *position);

struct CompositeAnimation {
    u8 unknown_00[0x11];
    u8 display_flags;
    u8 unknown_12[7];
    u8 dirty;
    u8 unknown_1a;
    u8 count;
    u8 unknown_1c[0x0c];
    struct AnimationEntry *entries[4];
};

void ObjectGroup_ApplyRandomChildValues(struct ObjectRuntime *owner)
{
    s32 state;
    struct CompositeAnimation *target;
    s32 initial_count;
    s32 count;
    struct AnimationEntry **entry;
    struct AnimationEntry *current;
    u32 frame;
    s32 value;

    state = owner->animation_kind;
    if (state == 1) {
        target = owner->animation;
        if (target != 0 && (target->display_flags & state) == 0) {
            initial_count = target->count;
            if (initial_count != 0) {
                entry = target->entries;
                frame = gFrameCount;
                count = initial_count;
                do {
                    value = frame % 6;
                    current = *entry++;
                    count--;
                    current->param = value;
                } while (count != 0);
            }
            target->dirty = 1;
        }
    }
}

