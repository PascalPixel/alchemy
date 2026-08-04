#include "layout_guard.h"
#include "types.h"

typedef struct OamSlot_080912b8 {
    u8 padding00[4];
    s8 screen_y;
    u8 attr0_high;
    u16 attr1;
    u16 attr2;
    u8 padding0a[2];
} OamSlot_080912b8;

typedef struct SceneObject_080912b8 {
    u8 padding00[8];
    s32 x;
    u8 padding0c[4];
    s32 z;
    u8 padding14[2];
    s16 minimum_height;
    u8 padding18[0x0a];
    u8 projection_layer;
} SceneObject_080912b8;

typedef struct MarkerState_080912b8 {
    OamSlot_080912b8 slots[2];
    SceneObject_080912b8 *object;
} MarkerState_080912b8;

typedef struct ViewState_080912b8 {
    u8 padding00[0xe6];
    s16 tile_x;
    u8 paddinge8[2];
    s16 tile_z;
} ViewState_080912b8;

LAYOUT_SIZE_GUARD(OamSlot080912b8_Size, OamSlot_080912b8, 0x0c);
LAYOUT_OFFSET_GUARD(
    SceneObject080912b8_X,
    SceneObject_080912b8,
    x,
    8);
LAYOUT_OFFSET_GUARD(
    SceneObject080912b8_Z,
    SceneObject_080912b8,
    z,
    0x10);
LAYOUT_OFFSET_GUARD(
    SceneObject080912b8_MinimumHeight,
    SceneObject_080912b8,
    minimum_height,
    0x16);
LAYOUT_OFFSET_GUARD(
    SceneObject080912b8_ProjectionLayer,
    SceneObject_080912b8,
    projection_layer,
    0x22);
LAYOUT_OFFSET_GUARD(
    MarkerState080912b8_Object,
    MarkerState_080912b8,
    object,
    0x18);
LAYOUT_OFFSET_GUARD(
    ViewState080912b8_TileX,
    ViewState_080912b8,
    tile_x,
    0xe6);
LAYOUT_OFFSET_GUARD(
    ViewState080912b8_TileZ,
    ViewState_080912b8,
    tile_z,
    0xea);

extern ViewState_080912b8 *Data_03001e70;
extern MarkerState_080912b8 *Data_03001ee0;

void Func_08003dec(void *entry, s32 priority);
s32 Func_080091a8(s32 layer, s32 x, s32 z);

static s32 ProjectMarkerHeight_080912b8(
    u8 layer, s32 world_x, s32 world_z)
{
    s32 near_height =
        Func_080091a8(layer, world_x, world_z + 0x100000) >> 16;
    s32 far_height =
        (Func_080091a8(layer, world_x, world_z + 0x200000) >> 16) -
        16;

    if (far_height > near_height)
        near_height = far_height;
    return near_height;
}

static void DrawMarker_080912b8(
    OamSlot_080912b8 *slot,
    s32 initial_attr2,
    u32 tile_index,
    s32 world_x,
    s32 world_z,
    s32 height,
    const ViewState_080912b8 *view)
{
    *(u32 *)&slot->screen_y = 0x40000800;
    *(u32 *)&slot->attr2 = initial_attr2;
    slot->attr0_high =
        (slot->attr0_high & (u8)~0x0c) | 4;
    slot->attr2 =
        (slot->attr2 & 0xfc00) | (tile_index & 0x03ff);
    slot->attr1 =
        (slot->attr1 & 0xfe00) |
        ((((world_x >> 16) & 0xfff0) - view->tile_x) & 0x01ff);
    slot->screen_y = (s8)(
        (((world_z >> 16) & 0xf0) - view->tile_z - height) + 16);
    Func_08003dec(slot, 0);
}

/*
 * Draw the paired ground markers flanking the tracked scene object. Each
 * marker samples two terrain points, uses the taller projected result, and
 * is suppressed below the object's minimum visible height.
 */
void Func_080912b8(void)
{
    MarkerState_080912b8 *state = Data_03001ee0;
    ViewState_080912b8 *view = Data_03001e70;
    SceneObject_080912b8 *object = state->object;
    u32 tile_index;
    s32 world_x;
    s32 height;

    if (object == 0)
        return;

    tile_index = *(volatile u16 *)0x03001c8a >> 5;
    world_x = object->x - 0x80000;
    height = ProjectMarkerHeight_080912b8(
        object->projection_layer, world_x, object->z);
    if (height > 0 && height > object->minimum_height) {
        DrawMarker_080912b8(
            &state->slots[0],
            0x400,
            tile_index,
            world_x,
            object->z,
            height,
            view);
    }

    world_x += 0x100000;
    height = ProjectMarkerHeight_080912b8(
        object->projection_layer, world_x, object->z);
    if (height > 0 && height > object->minimum_height) {
        DrawMarker_080912b8(
            &state->slots[1],
            0,
            tile_index,
            world_x,
            object->z,
            height,
            view);
    }
}
