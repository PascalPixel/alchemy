#ifndef ALCHEMY_MENU_TITLE_H
#define ALCHEMY_MENU_TITLE_H

#include "types.h"

/// The title overlay's sprite records, three OAM words each.
struct TitleSprite {
  u32 attr[3];
};

extern s16 gTitleRevealFrame;
extern struct TitleSprite gTitleSprites[];

/// The Golden Sun logo the title and clear screens show: an 8-bit background
/// on BG2, its map in screen block 6 and its tiles from tile 0x1a0 of
/// character block 0.
#define LOGO_BGCNT (BGCNT_PRIORITY(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(6))
#define LOGO_MAP ((u16 *)BG_SCREEN_BLOCK(6))
#define LOGO_FIRST_TILE 0x1a0
#define LOGO_TILES (BG_CHAR_BLOCK(0) + LOGO_FIRST_TILE * 64)
/// The map entry after each row's 30 visible tiles.
#define LOGO_BLANK_TILE 0x1ff

#endif
