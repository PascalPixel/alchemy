#ifndef ALCHEMY_TITLE_H
#define ALCHEMY_TITLE_H

/* What the Title (title.c) and the File Screen (file_screen.c) share: the
   Golden Sun logo and the sprites of its lettering. */

#include "types.h"

/* One sprite of the logo's lettering, as syOAMAddSprAtDepth queues it.
   scTitleUpdateSprReveal rewrites every entry each frame. */
typedef struct TITLE_SPR {
	/* [0] the queue link syOAMAddSprAtDepth writes; [1] OAM attribute 0 (y,
	   mode and shape) in the low half and attribute 1 (x) in the high half; [2]
	   attribute 2 (the tile, and the palette). */
	Uint32 attr[3];
} TITLE_SPR;

/* Frames since the lettering's reveal began; a column appears every two. */
extern Sint16 TitleRevealFrame;
/* The lettering's sprites, in the overlay's BSS (scene/title_bss.s): 18 in the
   English and Japanese editions, 120 in the others. */
extern TITLE_SPR TitleSprites[];

/* The Golden Sun logo the Title and the File Screen show: an 8-bit background
   on BG2, its map in screen block 6 and its tiles from tile 416 of character
   block 0. */
#define LOGO_BGCNT		(BGCNT_PRIORITY(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(6))
#define LOGO_MAP		((Uint16 *)BG_SCREEN_BLOCK(6))
#define LOGO_FIRST_TILE 416
#define LOGO_TILES		(BG_CHAR_BLOCK(0) + LOGO_FIRST_TILE * TILE_SIZE_8BPP)

#endif
