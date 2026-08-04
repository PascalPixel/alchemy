#include "layout_guard.h"
#include "types.h"

enum {
    RENDER_SLOT_COUNT_0800BC70 = 64,
    RESOURCE_ID_EXHAUSTED_0800BC70 = 0x60,
};

typedef struct ResourceDimensions_0800bc70 {
    u8 width;
    u8 height;
} ResourceDimensions_0800bc70;

typedef struct RenderSlot_0800bc70 {
    u32 reserved00;
    u32 object_attributes;
    u32 tile_attributes;
    u32 reserved0c;
    u32 coordinate_bias;
    u32 palette_attributes;
    u8 padding18[4];
    u8 resource_id;
    u8 padding1d;
    u16 animation_state;
    u8 active;
    u8 padding21[5];
    u8 visible;
    u8 padding27[0x11];
} RenderSlot_0800bc70;

LAYOUT_OFFSET_GUARD(
    RenderSlot0800bc70_ResourceId,
    RenderSlot_0800bc70,
    resource_id,
    0x1c);
LAYOUT_OFFSET_GUARD(
    RenderSlot0800bc70_AnimationState,
    RenderSlot_0800bc70,
    animation_state,
    0x1e);
LAYOUT_OFFSET_GUARD(
    RenderSlot0800bc70_Active,
    RenderSlot_0800bc70,
    active,
    0x20);
LAYOUT_OFFSET_GUARD(
    RenderSlot0800bc70_Visible,
    RenderSlot_0800bc70,
    visible,
    0x26);
LAYOUT_SIZE_GUARD(
    RenderSlot0800bc70_Size,
    RenderSlot_0800bc70,
    0x38);

extern RenderSlot_0800bc70 *Data_03001e60;
extern u16 Data_03001b10[];

ResourceDimensions_0800bc70 *Func_08185000(s32);
s32 Func_08004080(void);
s32 Func_08003fa4(s32, s32, s32);
s32 Func_0800b8ac(RenderSlot_0800bc70 *, s32);

/* Translate supported sprite dimensions into GBA OBJ shape/size bits. */
static u32 ObjectShapeSize_0800bc70(u16 dimensions)
{
    switch (dimensions) {
    case 0x0808:
        return 0;
    case 0x0810:
        return 0x00008000;
    case 0x1008:
        return 0x00004000;
    case 0x1010:
        return 0x40000000;
    case 0x1020:
        return 0x80008000;
    case 0x2010:
        return 0x80004000;
    case 0x2020:
        return 0x80000000;
    case 0x2040:
        return 0xc0008000;
    case 0x4020:
        return 0xc0004000;
    case 0x4040:
        return 0xc0000000;
    default:
        return 0;
    }
}

/* Reserve and initialize one of the 64 shared sprite-render slots. */
RenderSlot_0800bc70 *Func_0800bc70(s32 resource)
{
    ResourceDimensions_0800bc70 *dimensions = Func_08185000(resource);
    s32 resource_id = Func_08004080();
    RenderSlot_0800bc70 *slot = Data_03001e60;
    s32 tile_attributes;
    s32 index;
    u16 packed_dimensions;

    if (dimensions->width == 0)
        return 0;

    for (index = 0; index < RENDER_SLOT_COUNT_0800BC70; index++, slot++) {
        if (slot->active == 0)
            break;
    }
    if (index == RENDER_SLOT_COUNT_0800BC70)
        return 0;
    if (resource_id == RESOURCE_ID_EXHAUSTED_0800BC70)
        return 0;

    tile_attributes = Func_08003fa4(resource_id, 0, 0);
    if (tile_attributes == 0)
        return 0;

    slot->resource_id = resource_id;
    slot->animation_state = 0;
    slot->visible = 1;

    packed_dimensions =
        ((u16)dimensions->width << 8) | dimensions->height;
    slot->reserved00 = 0;
    slot->object_attributes =
        ObjectShapeSize_0800bc70(packed_dimensions) | 0x2000;
    slot->tile_attributes = tile_attributes | 0x0800;
    slot->reserved0c = 0;
    slot->coordinate_bias = 0x6000;
    slot->palette_attributes =
        (Data_03001b10[187] >> 5) | 0x0800;

    Func_0800b8ac(slot, resource);
    return slot;
}
