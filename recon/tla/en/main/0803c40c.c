#include "types.h"
#include "ram_buffer.h"
#include "window.h"
#include "affine.h"

struct OutputSprite {
    u32 link;
    u8 y;
    u8 affine : 2;
    u8 mode : 2;
    u8 other : 4;
    u16 x : 9;
    u16 affine_index : 5;
    u16 other_x : 2;
    u16 tile;
};
extern u16 Data_0805e9c4[];

void RenderOutput_UpdateScaleAnimation(struct RenderOutput *output)
{
    s32 scale = 256;
    struct OutputSprite *sprite = (struct OutputSprite *)output->unknown_10;
    struct AffineTransform effect;

    switch ((u8)output->active) {
    case 9:
        scale = Data_0805e9c4[output->anim_step++ & 31];
        break;
    case 10:
        scale = Data_0805e9c4[output->anim_step++ & 31] >> 1;
        break;
    case 11:
        if (output->anim_step <= 7)
            scale = Data_0805e9c4[output->anim_step++ * 2 + 16];
        break;
    case 12:
        if (output->anim_step <= 7)
            scale = Data_0805e9c4[output->anim_step++ * 2 + 16] >> 1;
        break;
    }
    if (scale == 256) {
        sprite->affine_index = 0;
        sprite->affine = 0;
        sprite->x = (u16)output->x;
        sprite->y = (u16)output->y;
    } else {
        effect.scale_x = scale;
        effect.scale_y = scale;
        effect.angle = 0;
        sprite->affine_index = AffineMatrix_BuildForEffect(&effect);
        if (scale > 256) {
            sprite->affine = 3;
            /* FAKEMATCH: preserve the original unsigned halfword read at
               the logical x cell. Casting the signed member value swaps the
               x load and -8 pool load at the same 460-byte module extent. */
            sprite->x = *(u16 *)&output->x - 8;
            sprite->y = (u16)output->y - 8;
        } else {
            sprite->affine = 1;
            sprite->x = (u16)output->x;
            sprite->y = (u16)output->y;
        }
    }
}
