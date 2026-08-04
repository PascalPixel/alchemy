#include "layout_guard.h"
#include "types.h"

typedef struct SpriteSlot_0800b074 {
    u8 padding00[4];
    u8 oam_y;
    u8 oam_mode;
    u16 oam_attr1;
    u8 padding08[4];
} SpriteSlot_0800b074;

typedef struct Sprite_0800b074 {
    SpriteSlot_0800b074 main_slot;
    SpriteSlot_0800b074 shadow_slot;
    u8 padding18[8];
    u8 width;
    u8 height;
    s8 center_x;
    s8 center_y;
} Sprite_0800b074;

LAYOUT_SIZE_GUARD(
    SpriteSlot0800b074_Size,
    SpriteSlot_0800b074,
    0x0c);
LAYOUT_OFFSET_GUARD(
    Sprite0800b074_ShadowSlot,
    Sprite_0800b074,
    shadow_slot,
    0x0c);
LAYOUT_OFFSET_GUARD(
    Sprite0800b074_Width,
    Sprite_0800b074,
    width,
    0x20);
LAYOUT_OFFSET_GUARD(
    Sprite0800b074_CenterY,
    Sprite_0800b074,
    center_y,
    0x23);

static void PlaceSlot_0800b074(
    SpriteSlot_0800b074 *slot, s32 x, s32 y, u8 mode)
{
    slot->oam_mode = (slot->oam_mode & (u8)~3) | mode;
    slot->oam_attr1 =
        (slot->oam_attr1 & 0xfe00) | (x & 0x01ff);
    slot->oam_y = (u8)y;
}

/*
 * Place a sprite and its ground marker from already-projected coordinates.
 * Scales above 1.0 select double-size affine OAM and double the bounding-box
 * offsets; the main sprite also honors its vertical anchor byte.
 */
void Func_0800b074(
    Sprite_0800b074 *sprite,
    s32 x,
    s32 main_reference_y,
    s32 baseline_y,
    s32 shadow_reference_y,
    const s32 scale[2])
{
    u32 half_width = sprite->width >> 1;
    u32 half_height = sprite->height >> 1;
    s32 shadow_x_offset = 8;
    s32 shadow_y_offset = 4;
    u8 mode = 1;
    s32 main_x;
    s32 main_y;
    s32 shadow_x;
    s32 shadow_y;

    if (scale[0] > 0x10000 || scale[1] > 0x10000) {
        mode = 3;
        shadow_x_offset = 16;
        shadow_y_offset = 8;
        half_width *= 2;
        half_height *= 2;
    }

    main_x = (x >> 16) - half_width;
    main_y = ((baseline_y - main_reference_y) >> 16) - half_height;
    main_y -=
        (((sprite->height >> 1) - sprite->center_y) * scale[1] +
         0xffff) >> 16;
    PlaceSlot_0800b074(&sprite->main_slot, main_x, main_y, mode);

    shadow_x = (x >> 16) - shadow_x_offset;
    shadow_y =
        ((baseline_y - shadow_reference_y) >> 16) - shadow_y_offset;
    PlaceSlot_0800b074(
        &sprite->shadow_slot, shadow_x, shadow_y, mode);
}
