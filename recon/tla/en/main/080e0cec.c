/* Near miss, differ 4 (two reordered rows, same instructions): the twin of The
 * Broken Seal's BattleFx_UpdateRadialLaunch; this game stores x and z to the
 * slot's origin and position together and does not write them back to pos.
 * After a store, the listing puts one other instruction before the next load
 * through another base: it copies sp to r0 for the camera call before it
 * loads y, and shifts the 8-pixel constant before it loads pos.z. This source
 * loads first both times. The same row is in 0811bd50. */
#include "types.h"
#include "ram_buffer.h"
#include "effect_slot.h"

struct LaunchVector {
    s32 x;
    s32 y;
    s32 z;
};

struct LaunchObject {
    u8 unknown_00[8];
    s32 x;
    s32 y;
    s32 z;
};

struct LaunchScene {
    u8 unknown_00[0x10];
    struct LaunchObject *main_object;
};

extern u32 gFrameCount;

void Camera_WorldToScreen(struct LaunchVector *position);
void Vector_AddPolarOffset(s32 distance, s32 angle, void *position);
u32 Random16(void);
void Audio_PlayCue(s32 cue);
void BattleFx_ClearOwnedSlot(struct EffectSlot *slot);

void Func_080e0cec(struct EffectSlot *slot)
{
    struct LaunchVector pos;
    struct LaunchObject *src;
    s32 state;
    u32 r;

    src = ((struct LaunchScene *)Ram_HeapSlots->effect_work)->main_object;
    state = slot->state;

    if (state == 0) {
        pos.x = src->x;
        pos.y = src->y;
        pos.z = src->z;
        Camera_WorldToScreen(&pos);

        slot->x = slot->origin_x = pos.x;
        slot->z = slot->origin_z = pos.z + (8 << 16);

        r = Random16();
        Vector_AddPolarOffset(120 << 16, ((r << 13) >> 16) - ((Random16() << 13) >> 16) + 0xc000, &pos);

        slot->target_x = pos.x;
        slot->target_z = pos.z;
        slot->acceleration = 5 << 16;
        slot->max_speed = 5 << 16;
        slot->flag42 = state;
        slot->state++;

        if ((gFrameCount & 2) != 0)
            Audio_PlayCue(246);
    } else if (state == 1) {
        if (BattleFx_HasReachedTarget(slot) == 0)
            slot->state--;
    } else if (state == 2) {
        if (BattleFx_HasReachedTarget(slot) == 0)
            BattleFx_ClearOwnedSlot(slot);
    }
}
