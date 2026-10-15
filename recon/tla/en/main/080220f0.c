#include "affine.h"
#include "callback_scheduler.h"
#include "resource.h"
#include "types.h"
#include "animspr.h"
#include "iwram_call.h"

void Render_ApplyProjectedPlacement(struct AnimationObject *sprite, s32 *pos, s32 *scale, s32 mode)
{
    s32 matrix;
    u32 half_width = sprite->width >> 1;
    u32 half_height = sprite->height >> 1;
    s32 size = 8;
    s32 size_half = 4;
    s32 affine;
    s32 scaleX = *scale++;
    s32 px = *pos++;
    s32 scaleY = *scale;
    s32 py = *pos++;
    s32 pz = *pos++;
    s32 pw = *pos;
    s32 slot;
    /* FAKEMATCH: unused; they give the ROM's 68-byte frame.
       2026-10-02: deleting both arrays changes only the stack allocation
       and release from 68 to 44 bytes; the complete function then differs. */
    s32 ground[3];
    s32 screen[3];
    struct AffineTransform effect;
    s32 flip;
    s32 x;
    s32 y;
    s32 depth;
    struct AnimationSpritePart *part;

    flip = Sprite_ComposeAnimationFrame(sprite, mode);
    if (flip == 0 && scaleX == 0x10000 && scaleY == scaleX && sprite->rotation == 0) {
        affine = 0;
        matrix = 0;
    } else {
        affine = 1;
        effect.angle = sprite->rotation;
        effect.scaleX = scaleX >> 8;
        effect.scaleY = scaleY >> 8;
        if (flip != 0)
            effect.scaleX = -effect.scaleX;
        matrix = AffineMatrix_BuildForEffect(&effect);
    }
    if (scaleX > 0x10000 || scaleY > 0x10000) {
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
            part->affineIndex = matrix;
            part->x = (px >> 16) - size;
            part->y = y;
            Runtime_PushSlotEntry(part, depth);
        }
    }
    x = (px >> 16) - half_width + ((sprite->offsetX * scaleX + 0xffff) >> 16);
    y = ((pz - py) >> 16) - half_height
        - ((scaleY * ((sprite->height >> 1) - sprite->offsetY) + 0xffff) >> 16);
    if (x < 240 && y < 160) {
        part = &sprite->part[0];
        part->x = x;
        part->y = y;
        part->affine = affine;
        part->affineIndex = matrix;
        Runtime_PushSlotEntry(part, slot);
    }
}
