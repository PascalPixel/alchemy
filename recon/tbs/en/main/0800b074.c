/* Exact C body (2026-09-26): [0800b074,0800b166), 242 bytes including
   its three-word pool. The 244-byte listing span includes separate trailing
   inter-function alignment at b166. Split from aa0c after bx b072; both ROMs
   rebuilt byte-identically. The field veneer at 08009040 targets b075.
   Leaf function: six arguments, two margin spills, two 12-byte OAM parts.
   Reuses the proven ProjectedSprite layout of exact 0800b388.
   Model: unsigned dimensions, signed offset_y, unscaled original half-height
   in the scale product, +0xffff rounding; restore both omitted X stores,
   second-part stride 12, and ground Y = (screen_y - ground_height) >> 16.
   Predict the original pair of margin slots and affine/X/Y store sequences.
   Accept only all 244 bytes exact plus comparison, coverage and verify.
   Read the complete normalized diff; at most two evidence-backed variants.
   Model result: 240/244 bytes, 85 differing halfwords / 76 aligned edits.
   All arithmetic, part stride and packed stores are restored. No frame is
   emitted: margins live in r9/fp; screen_x, updated in place, lives in r4
   from entry instead of moving its narrowed value from r1 to r8. Reference
   spills margins to sp4/sp0 and uses a distinct narrowed-x lifetime.
   H1: split incoming fixed-point X from the narrowed pixel-X local. The
   incoming r1 must die at its shift; predict a new r8 lifetime, freeing r1
   through the scale branch and spilling margins instead of coordinates.
   Same 244-byte exact gate, one trial, complete difference inspected.
   Own-ROM reference audit: b075 occurs only in the 08009040 veneer pool;
   no direct BL to b074/9040 in the first 1 MiB and no pointer to 9041.
   H1 result: every instruction and all three pool words match. Score
   extracts 242/244 bytes with only the trailing zero alignment halfword
   absent. Existing integration rejects that extra inter-function pad;
   follow the established 080a7a32 alignment listing convention, leaving
   those two bytes separate and uncredited. No gate or compiler change. */
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

void Render_PlaceSpritePartPair(struct ProjectedSprite *sprite, s32 screen_x,
    s32 height, s32 screen_y, s32 ground_height, s32 *scale)
{
    u32 half_width = sprite->width >> 1;
    u32 half_height = sprite->height >> 1;
    s32 margin_x = 8;
    s32 margin_y = 4;
    s32 affine = 1;
    s32 scale_x = *scale++;
    s32 scale_y = *scale;
    s32 pixel_x;
    s32 x;
    s32 y;
    struct ProjectedSpritePart *shadow;

    if (scale_x > 0x10000 || scale_y > 0x10000) {
        affine = 3;
        margin_x = 16;
        margin_y = 8;
        half_width <<= 1;
        half_height <<= 1;
    }
    /* FAKEMATCH: separate the pixel-X lifetime from the incoming argument. */
    pixel_x = screen_x >> 16;
    x = pixel_x - half_width;
    y = ((screen_y - height) >> 16) - half_height
        - ((((sprite->height >> 1) - sprite->offset_y) * scale_y
            + 0xffff) >> 16);
    sprite->part[0].affine = affine;
    sprite->part[0].x = x;
    sprite->part[0].y = y;
    x = pixel_x - margin_x;
    y = ((screen_y - ground_height) >> 16) - margin_y;
    shadow = &sprite->part[1];
    shadow->affine = affine;
    shadow->x = x;
    shadow->y = y;
}
