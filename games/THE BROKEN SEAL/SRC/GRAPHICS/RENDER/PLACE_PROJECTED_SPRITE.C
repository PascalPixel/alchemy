#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "AFFINE.H"
#include "CALLBACK_SCHEDULER.H"
#include "RESOURCE.H"
#include "TYPES.H"
#include "ANIMSPR.H"
#include "IWRAM_CALL.H"

#define gMenuCtrlWork ((u8 (*))&gMenuCtrlWork)

void Render_ApplyProjectedPlacement(struct AnimationObject *sprite, s32 *pos, s32 *scale, s32 mode)
{
    s32 matrix;
    u32 half_width = sprite->width >> 1;
    u32 half_height = sprite->height >> 1;
    s32 size = 8;
    s32 size_half = 4;
    s32 affine;
    s32 scale_x = *scale++;
    s32 px = *pos++;
    s32 scale_y = *scale;
    s32 py = *pos++;
    s32 pz = *pos++;
    s32 pw = *pos;
    s32 slot;
    /* FAKEMATCH: needed to match: the two unused arrays supply 24 bytes of the 68-byte frame; removing them leaves 44 bytes. */
    s32 ground[3];
    s32 screen[3];
    struct AffineTransform effect;
    s32 flip;
    s32 x;
    s32 y;
    s32 depth;
    struct AnimationSpritePart *part;

    flip = Sprite_ComposeAnimationFrame(sprite, mode);
    if (flip == 0 && scale_x == 0x10000 && scale_y == scale_x && sprite->rotation == 0) {
        affine = 0;
        matrix = 0;
    } else {
        affine = 1;
        effect.angle = sprite->rotation;
        effect.scale_x = scale_x >> 8;
        effect.scale_y = scale_y >> 8;
        if (flip != 0)
            effect.scale_x = -effect.scale_x;
        matrix = AffineMatrix_BuildForEffect(&effect);
    }
    if (scale_x > 0x10000 || scale_y > 0x10000) {
        affine = 3;
        half_width <<= 1;
        half_height <<= 1;
        size = 16;
        size_half = 8;
    }
    if (py <= -0x640000) {
        slot = 1;
        depth = 0;
    } else {
        slot = (pz >> 17) + 10;
        depth = 2;
    }
    y = ((pz - pw) >> 16) - size_half;
    if (sprite->flags & 1) {
        if (y < 160) {
            part = &sprite->part[1];
            part->affine = affine;
            part->affine_index = matrix;
            part->x = (px >> 16) - size;
            part->y = y;
            Runtime_PushSlotEntry(part, depth);
        }
    }
    x = (px >> 16) - half_width + ((sprite->offset_x * scale_x + 0xffff) >> 16);
    y = ((pz - py) >> 16) - half_height
        - ((scale_y * ((sprite->height >> 1) - sprite->offset_y) + 0xffff) >> 16);
    if (x < 240 && y < 160) {
        part = &sprite->part[0];
        part->x = x;
        part->y = y;
        part->affine = affine;
        part->affine_index = matrix;
        Runtime_PushSlotEntry(part, slot);
    }
}

void Render_PlaceProjectedSprite(struct AnimationObject *sprite, s32 *point, s32 *scale, s32 mode, s32 depth)
{
    s32 flip;
    u32 half_height;
    u32 size;
    s32 affine;
    s32 half_width;
    s32 ground[3];
    s32 screen[3];
    struct AffineTransform effect;
    s32 z;
    s32 base;
    s32 scale_x;
    s32 scale_y;
    s32 offset_x;
    s32 offset_y;
    s32 slot;

    affine = 1;
    if ((*(struct ObjectSystemWork **)gMenuCtrlWork)->suspended != 0)
        goto hide;
    z = Render_ProjectPoint(point, screen);
    if (screen[2] == 0)
        goto hide;
    if (screen[0] < -32)
        goto hide;
    if (screen[0] > 272)
        goto hide;
    if (screen[1] < -32)
        goto hide;
    if (screen[1] > 208)
        goto hide;
    if (sprite->display_flags & 2)
        z = sprite->scale;
    else
        z = Iwram_MulQ16(z, sprite->scale);
    half_width = sprite->width >> 1;
    half_height = sprite->height >> 1;
    size = 8;
    flip = Sprite_ComposeAnimationFrame(sprite, mode);
    base = (z + 0x400) & -0x800;
    scale_x = Iwram_MulQ16(base, *scale++);
    scale_y = Iwram_MulQ16(base, *scale);
    if (scale_x > 0x1f7ff)
        scale_x = 0x1f800;
    if (scale_y > 0x1f7ff)
        scale_y = 0x1f800;
    offset_x = Iwram_MulQ16(sprite->offset_x, scale_x);
    offset_y = -Iwram_MulQ16((sprite->height >> 1) - sprite->offset_y, scale_y);
    if (scale_x > 0x10000 || scale_y > 0x10000) {
        affine = 3;
        half_width <<= 1;
        half_height <<= 1;
        size = 16;
    } else if (scale_x == 0x10000 && sprite->rotation == 0 && scale_y == scale_x) {
        affine = 0;
    }
    if (affine != 0) {
        effect.angle = sprite->rotation;
        effect.scale_x = scale_x >> 8;
        if (flip != 0) {
            effect.scale_x = -effect.scale_x;
            offset_x = -offset_x;
        }
        effect.scale_y = scale_y >> 8;
        z = AffineMatrix_BuildForEffect(&effect);
    } else if (flip != 0) {
        z = 8;
        offset_x = -offset_x;
    } else {
        z = 0;
    }
    sprite->part[0].x = screen[0] - half_width + offset_x;
    sprite->part->y = screen[1] - half_height + offset_y;
    sprite->part[0].affine = affine;
    sprite->part[0].affine_index = z;
    if (depth == 0) {
        slot = (0x200 - screen[2]) / 2 + 128;
        if (slot <= 0)
            slot = 1;
        Runtime_PushSlotEntry(sprite, slot);
    } else {
        Runtime_PushSlotEntry(sprite, depth);
    }
    if (sprite->flags & 1) {
        struct AnimationSpritePart *shadow;

        ground[0] = point[0];
        ground[1] = 0;
        ground[2] = point[2];
        Render_ProjectPoint(ground, screen);
        shadow = &sprite->part[1];
        shadow->x = screen[0] - size;
        shadow->y = screen[1] - (size >> 1) + 2;
        shadow->affine = affine;
        shadow->affine_index = z;
        if (depth == 0)
            Runtime_PushSlotEntry(shadow, 0);
        else
            Runtime_PushSlotEntry(shadow, depth);
    }
    return;
hide:
    if (!(sprite->display_flags & 1)) {
        Resource_ActivateEntry(sprite->slot);
        sprite->dirty = 1;
    }
}
