#ifndef ALCHEMY_MENU_TITLE_H
#define ALCHEMY_MENU_TITLE_H

#include "types.h"

/// One sprite of the logo's lettering, as Runtime_PushSlotEntry queues it.
/// Title_UpdateSpriteReveal rewrites every record each frame.
struct TitleSprite {
  /// [0] the queue link Runtime_PushSlotEntry writes; [1] OAM attribute 0
  /// (y, mode and shape) in the low half and attribute 1 (x) in the high
  /// half; [2] attribute 2 (the tile, and the palette).
  u32 attr[3];
};

/// Frames since the lettering's reveal began; a column appears every two.
extern s16 gTitleRevealFrame;
/// The lettering's sprites, in the overlay's BSS (ovl_36f.s): 18 in the
/// English and Japanese editions, 120 in the others.
extern struct TitleSprite gTitleSprites[];

// The Golden Sun logo the title and clear screens show: an 8-bit background
// on BG2, its map in screen block 6 and its tiles from tile 0x1a0 of
// character block 0.
#define LOGO_BGCNT (BGCNT_PRIORITY(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(6))
#define LOGO_MAP ((u16 *)BG_SCREEN_BLOCK(6))
#define LOGO_FIRST_TILE 0x1a0
#define LOGO_TILES (BG_CHAR_BLOCK(0) + LOGO_FIRST_TILE * 64)
/// The map entry after each row's 30 visible tiles.
#define LOGO_BLANK_TILE 0x1ff

#endif
