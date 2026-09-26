/* Draft, not exact (2026-09-26): 542 of 544 bytes, 53 differing halfwords.
   Written from the listing, sharing the sibling Render_PlaceProjectedSprite
   structs. What lined up: every spill slot (declarations with initialisers
   put the sprite+33 address temporary between half_height and size), the
   effect packing, the px >> 16 recomputation on the hidden-shadow path
   (written twice in source), and all control flow.
   Remaining: the ROM copies the sprite into r0 at the start of the main
   part block and stores x, y, affine and matrix through r0, so the sprite
   pointer's costs change and it lands in r7 with ground/y in r6; here CSE
   folds any in-block part pointer into the sprite, the stores go through
   the sprite register and r6/r7 swap. Tried: an in-block part pointer, a
   part pointer declared at the top, an inline placement helper. */
#include "TYPES.H"

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

s32 Func_0800aa0c(struct ProjectedSprite *sprite, u16 mode);
s32 AffineMatrix_BuildForEffect(struct ProjectedEffect *source);
void Runtime_PushSlotEntry(void *entry, s32 slot);

/* Places a sprite and its optional shadow from a four-word screen position
   (x, shadow height, depth, ground) and a two-word scale. */
void Render_ApplyProjectedPlacement(struct ProjectedSprite *sprite, s32 *pos, s32 *scale, s32 mode)
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
    s32 ground[3];
    s32 screen[3];
    struct ProjectedEffect effect;
    s32 flip;
    s32 x;
    s32 y;
    s32 depth;

    flip = Func_0800aa0c(sprite, mode);
    if (flip == 0 && scale_x == 0x10000 && scale_y == scale_x && sprite->rotation == 0) {
        affine = 0;
        matrix = 0;
    } else {
        affine = 1;
        effect.angle = sprite->rotation;
        effect.x = scale_x >> 8;
        effect.y = scale_y >> 8;
        if (flip != 0)
            effect.x = -effect.x;
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
    if (sprite->shadow_flags & 1) {
        if (y < 160) {
            struct ProjectedSpritePart *shadow = &sprite->part[1];

            shadow->affine = affine;
            shadow->affine_index = matrix;
            shadow->x = (px >> 16) - size;
            shadow->y = y;
            Runtime_PushSlotEntry(shadow, depth);
        }
    }
    x = (px >> 16) - half_width + ((sprite->offset_x * scale_x + 0xffff) >> 16);
    y = ((pz - py) >> 16) - half_height
        - ((scale_y * ((sprite->height >> 1) - sprite->offset_y) + 0xffff) >> 16);
    if (x < 240 && y < 160) {
        struct ProjectedSpritePart *part = &sprite->part[0];

        part->x = x;
        part->y = y;
        part->affine = affine;
        part->affine_index = matrix;
        Runtime_PushSlotEntry(part, slot);
    }
}
