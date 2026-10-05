/*
 * Draft: ObjectGroup_ApplyRandomChildValues, ☀️'s loop over ⚓️'s composite
 * animation (flags at 0x11, dirty 0x19, count 0x1b, parts 0x28). With the
 * frame count read once (as here) 48 rows differ: ⚓️ keeps the animation in
 * r2 and saves it around each __umodsi3 call, holding the part in r5; with
 * ☀️'s volatile read in the loop 16 differ (count loaded into r1, frame
 * cached in r8). Links as recon/tla/raw/080db91c.s.
 */
#include "TYPES.H"
#include "ANIMSPR.H"
#include "OBJECT_RUNTIME.H"
#include "FRAME.H"

void Vector_AddPolarOffset(s32 radius, s32 angle, s32 *position);

/* ⚓️'s composite animation keeps its flags, dirty mark and part count
   twelve bytes earlier than ☀️'s AnimationObject; its parts follow at 0x28. */
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

/* Gives each part of an object's composite animation the frame count
   modulo 6, then marks the animation for redrawing. */
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

