#include "layout_guard.h"
#include "object_runtime.h"
#include "types.h"

struct MountedDisplay_08093fa0 {
    u8 padding00[0x26];
    u8 heading;
};

LAYOUT_OFFSET_GUARD(
    MountedDisplay08093fa0_Heading,
    struct MountedDisplay_08093fa0,
    heading,
    0x26);

extern volatile u8 Data_02000432;
extern u32 Data_02000434;

void Func_080030f8(u32);
void Func_08009080(struct ObjectRuntime *, s32);
void Func_08009150(struct ObjectRuntime *, s32, s32, s32);
void Func_080091e0(struct ObjectRuntime *, u32);
s8 Func_08009220(const s32 *);
void Func_0809163c(s32);
void Func_080916b0(void);
void Func_08091750(void);
struct ObjectRuntime *Func_08092054(u32);
void Func_08092158(u32, s32, s32);
void Func_080923c4(u32);

static s32 CenterToTile_08093fa0(s32 aligned)
{
    s32 center = aligned + 8;

    if (center < 0)
        center = aligned + 23;
    return center >> 4;
}

/*
 * Toggle the current object's two-phase vertical transition.  The opening
 * phase validates the centered map tile and collision probe before playing
 * the descent sequence.  The closing phase lifts the object into its restored
 * animation state.  Both paths balance the world-update lock.
 */
s32 Func_08093fa0(void)
{
    struct ObjectRuntime *object = Func_08092054(Data_02000434);
    s32 aligned_x = (s16)((u32)object->x >> 16) & 0xfff0;
    s32 aligned_z = (s16)((u32)object->z >> 16) & 0xfff0;
    s32 center_x = aligned_x + 8;
    s32 center_z = aligned_z + 8;
    u8 heading = 1;

    Func_080916b0();

    if (object->animation_kind == 1) {
        struct MountedDisplay_08093fa0 *mount = object->animation;

        heading = mount->heading;
    }

    if (Data_02000432 == 0) {
        u32 tile = (u32)CenterToTile_08093fa0(aligned_x) +
                   ((u32)CenterToTile_08093fa0(aligned_z) << 7);
        u32 tile_offset = tile << 2;

        if (*(u8 *)(0x02010002 + tile_offset) ==
            *(u8 *)(0x0200fe02 + tile_offset)) {
            s32 position[3];
            s8 probe_result;

            position[0] = object->x;
            position[1] = object->y;
            position[2] = object->z;
            probe_result = Func_08009220(position);
            if (probe_result == 0) {
                object->action_flags = 0;
                Func_08092158(Data_02000434, center_x, center_z);
                Func_08009080(object, 6);
                Func_080030f8(4);
                Func_08009080(object, 7);
                object->velocity_y = 0x40000;
                Func_080030f8(4);

                object->flags = 0;
                Func_080091e0(object, heading & 0xfe);
                object->speed_limit = 0x10000;
                object->velocity_y = 0;
                Func_08009080(object, 12);
                Func_080030f8(4);

                Data_02000432 = 1;
                object->action_flags = 1;
                Func_080030f8(8);
                Func_08091750();
                return 0;
            }
        }

        Func_08091750();
        return -1;
    }

    object->flags = 0;
    Func_08009080(object, 11);
    Func_08009150(
        object,
        (s32)((u32)center_x << 16),
        object->y + 0x80000,
        (s32)(((u32)center_z << 16) - 0x100000));
    Func_080923c4(Data_02000434);
    object->flags = 3;
    object->terrain_height = object->y;
    Func_080091e0(object, heading | 1);
    Func_0809163c(4);
    Data_02000432 = 0;
    object->action_flags = 1;

    Func_08091750();
    return 0;
}
