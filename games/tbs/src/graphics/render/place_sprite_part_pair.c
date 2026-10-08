#include "types.h"
#include "animspr.h"

void Render_PlaceSpritePartPair(struct AnimationObject *sprite, s32 screen_x,
    s32 height, s32 screen_y, s32 ground_height, s32 *scale)
{
    u32 half_width = sprite->width >> 1;
    u32 half_height = sprite->height >> 1;
    s32 margin_x = 8;
    s32 margin_y = 4;
    s32 affine = 1;
    s32 scale_x = *scale++;
    s32 scale_y = *scale;
    s32 x;
    s32 y;
    struct AnimationSpritePart *shadow;

    if (scale_x > 0x10000 || scale_y > 0x10000) {
        affine = 3;
        margin_x = 16;
        margin_y = 8;
        half_width <<= 1;
        half_height <<= 1;
    }
    x = (screen_x >> 16) - half_width;
    y = ((screen_y - height) >> 16) - half_height
        - ((((sprite->height >> 1) - sprite->offset_y) * scale_y
            + 0xffff) >> 16);
    sprite->part[0].affine = affine;
    sprite->part[0].x = x;
    sprite->part[0].y = y;
    x = (screen_x >> 16) - margin_x;
    y = ((screen_y - ground_height) >> 16) - margin_y;
    shadow = &sprite->part[1];
    shadow->affine = affine;
    shadow->x = x;
    shadow->y = y;
}
