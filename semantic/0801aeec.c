#include "layout_guard.h"
#include "types.h"

struct OamEntry_0801aeec {
    struct OamEntry_0801aeec *next;
    u8 y;
    u8 attr0_high;
    u16 attr1;
    u16 attr2;
};

struct SlidingSprite_0801aeec {
    u8 unknown_00[8];
    u16 slide_offset;
    u16 active;
    u16 graphics_handle;
    u16 graphics_offset;
    u16 x;
    u16 y;
    u8 unknown_14[0x14];
    struct OamEntry_0801aeec oam;
};

struct SlidingSpriteState_0801aeec {
    struct SlidingSprite_0801aeec sprites[2];
    u8 unknown_068[0x27a];
    u16 object_mode;
};

LAYOUT_SIZE_GUARD(
    OamEntry0801aeec_Size,
    struct OamEntry_0801aeec,
    0x0c);
LAYOUT_SIZE_GUARD(
    SlidingSprite0801aeec_Size,
    struct SlidingSprite_0801aeec,
    0x34);
LAYOUT_SIZE_GUARD(
    SlidingSpriteState0801aeec_Size,
    struct SlidingSpriteState_0801aeec,
    0x2e4);

extern volatile u32 Data_03001800;
extern const u8 Data_08033ef8[];
extern const u8 Data_080342f8[];

s32 Func_08003fa4(s32 handle, s32 size, const void *source);
void Func_08003dec(struct OamEntry_0801aeec *entry, s32 slot);
s32 Func_080770c0(s32 flag);

/*
 * Update one of the two sliding sprites.  The low bits of attr1/attr2 carry
 * the wrapped screen X and graphics offset; their upper OAM control bits are
 * retained.  A nonzero slide offset moves inward one pixel per update.
 */
void Func_0801aeec(struct SlidingSpriteState_0801aeec *state, s32 side)
{
    u32 animationFrame = (Data_03001800 >> 2) & 7;
    struct SlidingSprite_0801aeec *sprite = &state->sprites[side];
    struct OamEntry_0801aeec *oam = &sprite->oam;
    const u8 *graphics;
    u16 slideOffset;
    u16 x;
    s32 graphicsOffset;

    if (sprite->active == 0)
        return;

    oam->attr1 =
        (oam->attr1 & 0xfe00) | (sprite->x & 0x01ff);
    oam->y = (u8)sprite->y;

    if (side != 0) {
        slideOffset = state->sprites[1].slide_offset;
        graphics = Data_080342f8;
        if (slideOffset != 0) {
            x = (oam->attr1 & 0x01ff) + slideOffset;
            oam->attr1 =
                (oam->attr1 & 0xfe00) | (x & 0x01ff);
        }
    } else {
        slideOffset = state->sprites[0].slide_offset;
        graphics = Data_08033ef8;
        if (slideOffset != 0) {
            x = (oam->attr1 & 0x01ff) - slideOffset;
            oam->attr1 =
                (oam->attr1 & 0xfe00) | (x & 0x01ff);
        }
    }

    graphicsOffset = Func_08003fa4(
        sprite->graphics_handle,
        0x80,
        graphics + animationFrame * 0x80);
    oam->attr2 =
        (oam->attr2 & 0xfc00) | (graphicsOffset & 0x03ff);

    if (Func_080770c0(0x103) != 0) {
        oam->attr0_high &= (u8)~0x0c;
        if (state->object_mode == 1)
            oam->attr0_high |= 4;
    }

    Func_08003dec(oam, 0xee);

    if (sprite->slide_offset != 0)
        sprite->slide_offset--;
}
