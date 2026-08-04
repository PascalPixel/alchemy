#include "types.h"

/*
 * Core-drive row 0x0800b388, 762 bytes: the perspective sprite-attribute
 * builder. Same family as semantic/main/0800b168.c (the orthographic
 * variant, whose m2c trace fixes the field meanings used here) and the
 * 0800c62c entry gate (both begin by testing the s16 pause word at
 * Data_03001e68 + 4). The three `mov ip, pc ; bx rN` sequences are the
 * ordinary indirect calls through the pooled 0x03000118 IWRAM
 * fixed-point multiply (returns (a * b) >> 16), spelled per
 * semantic/main/0800447c.c.
 *
 * Flow: unless the runtime is paused, projects the caller's position
 * triple through Func_08005268 into screen space and rejects anything
 * outside x in [-32, 272], y in [-32, 208] or with a zero depth word.
 * The perspective scale (cached in cached_scale_18 when flags_1d bit 2
 * is set, otherwise multiplied fresh) is rounded to 0x800 steps, then
 * multiplied against the caller's two size words to give the x/y zoom
 * pair, each clamped to 0x1f800. The x/y screen offsets come from the
 * signed center bytes times the zoom. Mode selection: zoom above
 * 0x10000 on either axis means double-size affine (mode 3, dimensions
 * and margins doubled); exactly 0x10000 on both with no rotation and
 * no mirror means plain (mode 0); anything else normal affine (mode
 * 1). For affine modes the parameter pair (x zoom | y zoom in 8.8,
 * negated x when Func_0800aa0c reports a mirror, rotation halfword)
 * goes to Func_08003d28, which returns the affine slot written into
 * OAM attribute bits 9-13; a plain mirror uses slot bits 8 (h-flip).
 * OAM y/mode/x fields are written masked, exactly as the reference
 * does. Display priority comes from Func_08003dec -- the caller's
 * override when nonzero, else 128 + (512 - depth) / 2 clamped to at
 * least 1.
 *
 * When sub_flags_26 bit 0 is set the shadow slot at +12 repeats the
 * attribute build at the ground plane: the position re-projects with
 * its y word zeroed, the x margin (8, or 16 in double mode) and half
 * that plus 2 offset the coordinates, and the priority call repeats
 * with 0 in place of the computed depth priority.
 *
 * The offscreen/paused tail releases the object once: unless flags_1d
 * bit 0 is set, Func_08003f78 frees entry_no_1c and released_25 is
 * stamped 1.
 *
 * Complete owner: asm/0800b388.s in full including its interleaved
 * pools. Span: 762 bytes, matching the census row exactly.
 */

/* 0x03000118, the IWRAM-relocated ARM fixed-point multiply. */
typedef s32 (*Multiply_0800b388)(s32 a, s32 b);

struct SpriteSlot_0800b388 {
    u8 unknown_00[4];
    u8 oam_y_04;
    u8 oam_mode_05;     /* low 2 bits: 0 plain, 1 affine, 3 double */
    u16 oam_attr_06;    /* bits 0-8 x, bits 9-13 affine slot / flip */
    u8 unknown_08[4];
};

struct Sprite_0800b388 {
    struct SpriteSlot_0800b388 main_slot;   /* 0 */
    struct SpriteSlot_0800b388 shadow_slot; /* 12 */
    s32 cached_scale_18;    /* 24 */
    u8 entry_no_1c;         /* 28 */
    u8 flags_1d;            /* 29: bit 0 released, bit 1 scale cached */
    u16 rotation_1e;        /* 30 */
    u8 width_20;            /* 32: full width, halved for the anchor */
    u8 height_21;           /* 33 */
    s8 center_x_22;         /* 34 */
    s8 center_y_23;         /* 35 */
    u8 unknown_24;          /* 36 */
    u8 released_25;         /* 37 */
    u8 sub_flags_26;        /* 38: bit 0 = shadow slot in use */
};

extern u8 *Data_03001e68;

s32 Func_08005268(const s32 *source, s32 *projected);
s32 Func_0800aa0c(struct Sprite_0800b388 *sprite, u16 heading);
s32 Func_08003d28();
void Func_08003dec(void *slot, s32 priority);
s32 Func_08003f78(u32 entry_no);

void Func_0800b388(struct Sprite_0800b388 *sprite, const s32 *position,
                   const s32 *size, u16 heading, s32 priority_override)
{
    Multiply_0800b388 multiply = (Multiply_0800b388)0x03000118;
    s32 projected[3];
    u32 affine[2];
    s32 mode = 1;
    s32 scale;
    s32 zoom_x;
    s32 zoom_y;
    s32 offset_x;
    s32 offset_y;
    s32 half_width;
    s32 half_height;
    s32 margin = 8;
    s32 mirror;
    s32 slot;

    if (*(s16 *)(Data_03001e68 + 4) != 0) {
        goto release;
    }

    scale = Func_08005268(position, projected);
    if (projected[2] == 0) {
        goto release;
    }
    if (projected[0] < -32 || projected[0] > 272) {
        goto release;
    }
    if (projected[1] < -32 || projected[1] > 208) {
        goto release;
    }

    if ((sprite->flags_1d & 2) != 0) {
        scale = sprite->cached_scale_18;
    } else {
        scale = multiply(scale, sprite->cached_scale_18);
    }

    half_width = sprite->width_20 >> 1;
    half_height = sprite->height_21 >> 1;
    mirror = Func_0800aa0c(sprite, heading);

    scale = (scale + 0x400) & 0xfffff800;
    zoom_x = multiply(scale, size[0]);
    zoom_y = multiply(scale, size[1]);
    if (zoom_x > 0x1f7ff) {
        zoom_x = 252 << 9;
    }
    if (zoom_y > 0x1f7ff) {
        zoom_y = 252 << 9;
    }

    offset_x = multiply(sprite->center_x_22, zoom_x);
    offset_y = -multiply(half_height - sprite->center_y_23, zoom_y);

    if (zoom_x > 0x10000 || zoom_y > 0x10000) {
        mode = 3;
        half_width <<= 1;
        half_height <<= 1;
        margin = 16;
    } else if (zoom_x == 0x10000 && sprite->rotation_1e == 0 &&
               zoom_y == zoom_x) {
        mode = 0;
    }

    if (mode != 0) {
        affine[1] = (affine[1] & 0xffff0000) | sprite->rotation_1e;
        affine[0] = (affine[0] & 0xffff0000)
            | (u16)(((u32)zoom_x << 8) >> 16);
        if (mirror != 0) {
            affine[0] = (affine[0] & 0xffff0000)
                | (u16)(0 - (u16)affine[0]);
            offset_x = -offset_x;
        }
        affine[0] = (affine[0] & 0x0000ffff)
            | ((((u32)zoom_y << 8) >> 16) << 16);
        slot = Func_08003d28(affine);
    } else if (mirror != 0) {
        offset_x = -offset_x;
        slot = 8;
    } else {
        slot = 0;
    }

    sprite->main_slot.oam_attr_06 =
        (sprite->main_slot.oam_attr_06 & 0xfe00)
        | ((projected[0] - half_width + offset_x) & 0x1ff);
    sprite->main_slot.oam_y_04 =
        projected[1] - half_height + offset_y;
    sprite->main_slot.oam_mode_05 =
        (sprite->main_slot.oam_mode_05 & ~3) | mode;
    sprite->main_slot.oam_attr_06 =
        (sprite->main_slot.oam_attr_06 & ~0x3e00)
        | ((slot & 31) << 9);

    if (priority_override == 0) {
        s32 priority = ((512 - projected[2]) / 2) + 128;

        if (priority <= 0) {
            priority = 1;
        }
        Func_08003dec(sprite, priority);
    } else {
        Func_08003dec(sprite, priority_override);
    }

    if ((sprite->sub_flags_26 & 1) != 0) {
        struct SpriteSlot_0800b388 *shadow = &sprite->shadow_slot;
        s32 ground[3];

        ground[0] = position[0];
        ground[1] = 0;
        ground[2] = position[2];
        Func_08005268(ground, projected);

        shadow->oam_attr_06 = (shadow->oam_attr_06 & 0xfe00)
            | ((projected[0] - margin) & 0x1ff);
        shadow->oam_y_04 = projected[1] - (margin >> 1) + 2;
        shadow->oam_mode_05 = (shadow->oam_mode_05 & ~3) | mode;
        shadow->oam_attr_06 = (shadow->oam_attr_06 & ~0x3e00)
            | ((slot & 31) << 9);

        if (priority_override == 0) {
            Func_08003dec(shadow, 0);
        } else {
            Func_08003dec(shadow, priority_override);
        }
    }
    return;

release:
    if ((sprite->flags_1d & 1) == 0) {
        Func_08003f78(sprite->entry_no_1c);
        sprite->released_25 = 1;
    }
}
