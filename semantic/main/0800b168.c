#include "layout_guard.h"
#include "types.h"

typedef struct SpriteSlot_0800b168 {
    u8 padding00[4];
    u8 oam_y;
    u8 oam_mode;
    u16 oam_attr1;
    u8 padding08[4];
} SpriteSlot_0800b168;

typedef struct Sprite_0800b168 {
    SpriteSlot_0800b168 main_slot;
    SpriteSlot_0800b168 shadow_slot;
    u8 padding18[6];
    u16 rotation;
    u8 width;
    u8 height;
    s8 center_x;
    s8 center_y;
    u8 padding24[2];
    u8 flags;
} Sprite_0800b168;

typedef struct SpriteFrame_0800b168 {
    s32 x;
    s32 main_reference_y;
    s32 baseline_y;
    s32 shadow_reference_y;
} SpriteFrame_0800b168;

typedef struct SpriteScale_0800b168 {
    s32 x;
    s32 y;
} SpriteScale_0800b168;

LAYOUT_SIZE_GUARD(
    SpriteSlot0800b168_Size,
    SpriteSlot_0800b168,
    0x0c);
LAYOUT_OFFSET_GUARD(
    Sprite0800b168_ShadowSlot,
    Sprite_0800b168,
    shadow_slot,
    0x0c);
LAYOUT_OFFSET_GUARD(
    Sprite0800b168_Rotation,
    Sprite_0800b168,
    rotation,
    0x1e);
LAYOUT_OFFSET_GUARD(
    Sprite0800b168_Width,
    Sprite_0800b168,
    width,
    0x20);
LAYOUT_OFFSET_GUARD(
    Sprite0800b168_Flags,
    Sprite_0800b168,
    flags,
    0x26);

s32 Func_08003d28(const u32 *);
void Func_08003dec(void *, s32);
s32 Func_0800aa0c(Sprite_0800b168 *, u16);

static s32 RoundedFixedPixel_0800b168(s32 value)
{
    return (value + 0xffff) >> 16;
}

static void PlaceSlot_0800b168(
    SpriteSlot_0800b168 *slot,
    s32 x,
    s32 y,
    u8 mode,
    s32 affine_slot,
    s32 priority)
{
    slot->oam_mode = (slot->oam_mode & (u8)~3) | mode;
    slot->oam_attr1 =
        (slot->oam_attr1 & 0xfe00) | (x & 0x01ff);
    slot->oam_y = (u8)y;
    slot->oam_attr1 =
        (slot->oam_attr1 & (u16)~0x3e00) |
        ((affine_slot & 0x1f) << 9);
    Func_08003dec(slot, priority);
}

/*
 * Draw an orthographic sprite and its optional ground marker.  Plain unit
 * scale uses ordinary OAM; mirroring, rotation, or scaling allocates affine
 * parameters, and scales above 1.0 select double-size affine bounds.
 */
void Func_0800b168(
    Sprite_0800b168 *sprite,
    const SpriteFrame_0800b168 *frame,
    const SpriteScale_0800b168 *scale,
    u16 heading)
{
    u32 affine[2] = {0, 0};
    s32 half_width = sprite->width >> 1;
    s32 half_height = sprite->height >> 1;
    s32 shadow_x_margin = 8;
    s32 shadow_y_margin = 4;
    s32 mirror = Func_0800aa0c(sprite, heading);
    s32 mode;
    s32 affine_slot;
    s32 main_priority;
    s32 shadow_priority;
    s32 base_x;
    s32 shadow_y;
    s32 main_x;
    s32 main_y;

    if (mirror == 0 &&
        scale->x == 0x10000 &&
        scale->y == 0x10000 &&
        sprite->rotation == 0) {
        mode = 0;
        affine_slot = 0;
    } else {
        u16 scale_x = (u16)(((u32)scale->x << 8) >> 16);
        u16 scale_y = (u16)(((u32)scale->y << 8) >> 16);

        mode = 1;
        if (mirror != 0)
            scale_x = (u16)(0 - scale_x);
        affine[0] = scale_x | ((u32)scale_y << 16);
        affine[1] = sprite->rotation;
        affine_slot = Func_08003d28(affine);
    }

    if (scale->x > 0x10000 || scale->y > 0x10000) {
        mode = 3;
        half_width *= 2;
        half_height *= 2;
        shadow_x_margin = 16;
        shadow_y_margin = 8;
    }

    if (frame->main_reference_y <= (s32)0xff9c0000) {
        main_priority = 1;
        shadow_priority = 0;
    } else {
        main_priority = (frame->baseline_y >> 17) + 10;
        shadow_priority = 2;
    }

    base_x = frame->x >> 16;
    shadow_y =
        ((frame->baseline_y - frame->shadow_reference_y) >> 16) -
        shadow_y_margin;
    if ((sprite->flags & 1) != 0 && shadow_y <= 159) {
        PlaceSlot_0800b168(
            &sprite->shadow_slot,
            base_x - shadow_x_margin,
            shadow_y,
            (u8)mode,
            affine_slot,
            shadow_priority);
    }

    main_x = base_x - (sprite->width >> 1) +
        RoundedFixedPixel_0800b168(scale->x * sprite->center_x);
    main_y =
        ((frame->baseline_y - frame->main_reference_y) >> 16) -
        (sprite->height >> 1) -
        RoundedFixedPixel_0800b168(
            scale->y * ((sprite->height >> 1) - sprite->center_y));
    if (mode == 3) {
        main_x -= sprite->width >> 1;
        main_y -= sprite->height >> 1;
    }

    if (main_x <= 239 && main_y <= 159) {
        PlaceSlot_0800b168(
            &sprite->main_slot,
            main_x,
            main_y,
            (u8)mode,
            affine_slot,
            main_priority);
    }
}
