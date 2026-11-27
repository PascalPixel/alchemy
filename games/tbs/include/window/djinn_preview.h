#ifndef ALCHEMY_DJINN_PREVIEW_H
#define ALCHEMY_DJINN_PREVIEW_H

/* @file */
/* The Djinn preview's sprites and arrow tiles. */

#include "edition.h"
#include "graphics/battle_sprite_id.h"
#include "window/window_sprite.h"

/* The OAM entry of a window sprite (from WINDOW_SPR.link on), as the Djinn */
/* preview places its stat arrows (mnDjinnMenuDrawStatArrow). */
typedef struct PREVIEW_SPR {
	u32 link; /* the output's depth-queue link (WINDOW_SPR.link) */
	/* OAM attributes 0 and 1, written whole first: a 16x16 semi-transparent */
	/* sprite. */
	union {
		u32 word;
		struct {
			unsigned y : 8;	   /* screen pixels */
			unsigned flg : 8;  /* attribute 0's high byte: its mode and shape */
			unsigned x : 9;	   /* screen pixels */
			unsigned high : 7; /* attribute 1's flip and size bits: 16 by 16 */
		} bits;
	} attributes;
	OAM_ATTR2_WORD tile; /* OAM attribute 2: the arrow's VRAM tile */
} PREVIEW_SPR;

/* The stat arrows a Djinni preview shows beside a stat that would rise or */
/* fall, 16 by 16 pixels each (graphics/ui/pointing_hand.s). */
extern u8 DjinnMenu_ArrowUpTiles[];
extern u8 DjinnMenu_ArrowDownTiles[];

#endif
