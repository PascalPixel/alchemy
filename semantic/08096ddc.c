#include "types.h"

struct VisualChild_08096ddc {
    u8 unknown_00[0x16];
    u8 state;
};

struct VisualState_08096ddc {
    u8 unknown_00[5];
    u8 flags_05;
    u8 unknown_06;
    u8 flags_07;
    u16 tile;
    u8 unknown_0a[0x12];
    u8 resource_handle;
    u8 state_1d;
    u8 unknown_1e[8];
    u8 state_26;
    u8 unknown_27;
    struct VisualChild_08096ddc *child;
};

struct SceneObject_08096ddc {
    u8 unknown_00[6];
    u16 graphic_id;
    s32 x;
    s32 y;
    s32 z;
    s32 origin_y;
    s32 scale_x;
    s32 scale_y;
    u8 unknown_20[3];
    u8 role;
    u8 unknown_24[0x2c];
    struct VisualState_08096ddc *visual;
    u8 unknown_54;
    u8 state_55;
    u8 unknown_56[0x0e];
    s16 callback_mode;
    u16 callback_counter;
    struct SceneObject_08096ddc *owner;
    void (*callback)(struct SceneObject_08096ddc *);
};

struct EffectContext_08096ddc {
    u8 unknown_00[0x10];
    struct SceneObject_08096ddc *original;
    u8 unknown_14[0x32];
    u16 graphic_handle;
};

struct ResourceSlot_08096ddc {
    u16 unknown_00;
    u16 tile_offset;
};

struct SceneObject_08096ddc *Func_080090c8(s32, s32, s32, s32);
void Func_08009020(void *, s32);
s32 Func_08003f3c(u32);
void Func_08096d2c(struct SceneObject_08096ddc *);
void Func_08096d84(struct SceneObject_08096ddc *);

/*
 * Spawn the paired arc effects, retarget their visual resource, and give the
 * second effect the original object's orientation bits.
 */
void Func_08096ddc(struct SceneObject_08096ddc *source)
{
    struct EffectContext_08096ddc *context =
        *(struct EffectContext_08096ddc **)0x03001f30;
    struct ResourceSlot_08096ddc *resources =
        (struct ResourceSlot_08096ddc *)0x03001b10;
    struct SceneObject_08096ddc *spawned[2];
    s32 index;

    for (index = 0; index < 2; index++) {
        struct SceneObject_08096ddc *object;
        struct VisualState_08096ddc *visual;

        object = Func_080090c8(0x1a, source->x, source->y, source->z);
        spawned[index] = object;
        if (object != 0) {
            object->origin_y = source->origin_y;
            object->state_55 = 0;
            object->callback_mode = 0;
            object->owner = source;
            object->scale_x = 0x1999;
            object->scale_y = 0x1999;

            visual = object->visual;
            if (visual != 0) {
                u16 tile_base;

                Func_08009020(visual, 0);
                visual->state_26 = 0;
                Func_08003f3c(visual->resource_handle);
                visual->resource_handle = (u8)context->graphic_handle;
                visual->state_1d |= 1;

                tile_base = resources[visual->resource_handle].tile_offset;
                visual->tile =
                    (visual->tile & 0xfc00) |
                    ((tile_base >> 5) & 0x03ff);
                visual->flags_05 = (visual->flags_05 & 0x1f) | 0x40;
                visual->flags_07 = (visual->flags_07 & 0x3f) | 0x80;
                visual->child->state = 0;
            }
        }
    }

    spawned[0]->callback = Func_08096d84;
    ((u8 *)&spawned[0]->visual->tile)[1] &= (u8)~0x0c;

    spawned[1]->callback = Func_08096d2c;
    ((u8 *)&spawned[1]->visual->tile)[1] =
        (((u8 *)&spawned[1]->visual->tile)[1] & (u8)~0x0c) |
        (((u8 *)&context->original->visual->tile)[1] & 0x0c);
    spawned[1]->role = 2;
}
