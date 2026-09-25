/* Draft, not exact (2026-09-25): 762 of 764 bytes, one scheduling swap.
   Written from the listing. What lined up: the four spill slots (flip,
   half height, size, affine mode) by declaring them in reverse; the screen
   bounds as four separate tests so GCC does not fold them into one unsigned
   range check; the mode passed through as s32 and narrowed at the call; the
   flip branch sets the matrix index before negating the offset.
   Remaining: at the sprite y store the ROM loads the half height (sp+8)
   before screen y ([sl+4]); sched2 prefers the screen load because it
   depends truly on the preceding mov r2, sl. Tried: operand orders, casts,
   s32/u32/int types, a struct screen point, a u8 y field, temporaries and
   do-while around either store. */
#include "TYPES.H"
#include "IWRAM_CALL.H"

struct ProjectedSpritePart {
    u8 unknown_00[4];
    u16 y : 8;
    u16 affine : 2;
    u16 unknown_5 : 6;
    u16 x : 9;
    u16 affine_index : 5;
    u16 unknown_7 : 2;
    u8 unknown_08[4];
};

struct ProjectedSprite {
    struct ProjectedSpritePart part[2];
    s32 scale;
    u8 vram_block;
    u8 flags;
    u16 rotation;
    u8 width;
    u8 height;
    s8 offset_x;
    s8 offset_y;
    u8 unknown_24;
    u8 hidden;
    u8 shadow_flags;
};

struct ProjectedEffect {
    unsigned x : 16;
    unsigned y : 16;
    unsigned angle : 16;
    unsigned unused : 16;
};

struct RenderState {
    u8 unknown_00[4];
    s16 frozen;
};

extern u8 Value_03000118[];
#define MulQ16(left, right) Iwram_Call2((left), (right), Value_03000118)

s32 Render_ProjectPoint(s32 *point, s32 *screen);
s32 Func_0800aa0c(struct ProjectedSprite *sprite, u16 mode);
s32 AffineMatrix_BuildForEffect(struct ProjectedEffect *source);
void Runtime_PushSlotEntry(void *entry, s32 slot);
s32 Resource_ActivateEntry(u32 resource_index);

void Render_PlaceProjectedSprite(struct ProjectedSprite *sprite, s32 *point, s32 *scale, s32 mode, s32 depth)
{
    s32 flip;
    u32 half_height;
    u32 size;
    s32 affine;
    s32 half_width;
    s32 ground[3];
    s32 screen[3];
    struct ProjectedEffect effect;
    s32 z;
    s32 base;
    s32 scale_x;
    s32 scale_y;
    s32 offset_x;
    s32 offset_y;
    s32 slot;

    affine = 1;
    if ((*(struct RenderState **)0x03001e68)->frozen != 0)
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
    if (sprite->flags & 2)
        z = sprite->scale;
    else
        z = MulQ16(z, sprite->scale);
    half_width = sprite->width >> 1;
    half_height = sprite->height >> 1;
    size = 8;
    flip = Func_0800aa0c(sprite, mode);
    base = (z + 0x400) & -0x800;
    scale_x = MulQ16(base, *scale++);
    scale_y = MulQ16(base, *scale);
    if (scale_x > 0x1f7ff)
        scale_x = 0x1f800;
    if (scale_y > 0x1f7ff)
        scale_y = 0x1f800;
    offset_x = MulQ16(sprite->offset_x, scale_x);
    offset_y = -MulQ16((sprite->height >> 1) - sprite->offset_y, scale_y);
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
        effect.x = scale_x >> 8;
        if (flip != 0) {
            effect.x = -effect.x;
            offset_x = -offset_x;
        }
        effect.y = scale_y >> 8;
        z = AffineMatrix_BuildForEffect(&effect);
    } else if (flip != 0) {
        z = 8;
        offset_x = -offset_x;
    } else {
        z = 0;
    }
    sprite->part[0].x = screen[0] - half_width + offset_x;
    sprite->part[0].y = screen[1] - half_height + offset_y;
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
    if (sprite->shadow_flags & 1) {
        struct ProjectedSpritePart *shadow;

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
    if (!(sprite->flags & 1)) {
        Resource_ActivateEntry(sprite->vram_block);
        sprite->hidden = 1;
    }
}
