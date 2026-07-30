#include "types.h"

struct VisualChild_0809b450 {
    u8 unknown_00[0x16];
    u8 state;
};

struct VisualState_0809b450 {
    u8 unknown_00[5];
    u8 flags_05;
    u8 unknown_06;
    u8 flags_07;
    u16 tile;
    u8 unknown_0A[0x12];
    u8 resource_handle;
    u8 state_1D;
    u8 unknown_1E[8];
    u8 state_26;
    u8 unknown_27;
    struct VisualChild_0809b450 *child;
};

struct SceneObject_0809b450 {
    u8 unknown_00[6];
    u16 graphic_id;
    s32 x;
    s32 y;
    s32 z;
    s32 origin_y;
    s32 scale_x;
    s32 scale_y;
    u8 unknown_20[0x30];
    struct VisualState_0809b450 *visual;
    u8 unknown_54;
    u8 state_55;
    u8 unknown_56[0x0E];
    s16 callback_mode;
    u16 callback_counter;
    struct SceneObject_0809b450 *owner;
    void (*callback)(struct SceneObject_0809b450 *);
};

struct SceneSequence_0809b450 {
    u8 unknown_000[0x10];
    struct SceneObject_0809b450 *object;
    u8 unknown_014[0x706];
    u16 graphic_handle;
};

struct ResourceSlot_0809b450 {
    u16 unknown_00;
    u16 size;
};

extern struct SceneSequence_0809b450 *Data_03001f30;

struct SceneObject_0809b450 *Func_080090c8(s32, s32, s32, s32);
void Func_08009020(void *, s32);
s32 Func_08003f3c(u32);
void Func_0809b364(struct SceneObject_0809b450 *);
void Func_0809b3d8(struct SceneObject_0809b450 *);

/*
 * Spawn two rising objects and retarget their visual resources. The second
 * object inherits the original object's two orientation bits.
 */
void Func_0809b450(struct SceneObject_0809b450 *source)
{
    struct SceneSequence_0809b450 *sequence = Data_03001f30;
    struct ResourceSlot_0809b450 *resources =
        (struct ResourceSlot_0809b450 *)0x03001B10;
    struct SceneObject_0809b450 *spawned[2];
    struct SceneObject_0809b450 *original = sequence->object;
    s32 index;

    for (index = 0; index < 2; index++) {
        struct SceneObject_0809b450 *object;
        struct VisualState_0809b450 *visual;

        object = Func_080090c8(
            0x1A, source->x, source->y + 0x400000, source->z);
        spawned[index] = object;
        if (object != 0) {
            object->origin_y = source->origin_y;
            object->state_55 = 0;
            object->callback_mode = 0;
            object->owner = source;
            object->scale_x = 0x6666;
            object->scale_y = 0x6666;

            visual = object->visual;
            if (visual != 0) {
                u16 tile_base;

                Func_08009020(visual, 0);
                visual->state_26 = 0;
                Func_08003f3c(visual->resource_handle);
                visual->resource_handle = (u8)sequence->graphic_handle;
                visual->state_1D |= 1;

                tile_base = resources[visual->resource_handle].size;
                visual->tile =
                    (visual->tile & 0xFC00) |
                    ((tile_base >> 5) & 0x03FF);
                visual->flags_05 = (visual->flags_05 & 0x1F) | 0x40;
                visual->flags_07 = (visual->flags_07 & 0x3F) | 0x80;
                visual->child->state = 0;
            }
        }
    }

    spawned[0]->callback = Func_0809b3d8;
    ((u8 *)&spawned[0]->visual->tile)[1] &= (u8)~0x0C;

    spawned[1]->callback = Func_0809b364;
    ((u8 *)&spawned[1]->visual->tile)[1] =
        (((u8 *)&spawned[1]->visual->tile)[1] & (u8)~0x0C) |
        (((u8 *)&original->visual->tile)[1] & 0x0C);
}
