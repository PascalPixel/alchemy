#ifndef ALCHEMY_WINDOW_SPRITE_H
#define ALCHEMY_WINDOW_SPRITE_H

/* @file */
/* Sprites as the windows draw them: a sprite's OAM attributes, and the lists */
/* of characters, icons and portraits each window shows as sprites. */

#include "edition.h"
#include "static_assert.h"
#include "types.h"
#include "stubs.h"

/* A 16-byte rectangle. Only the Japanese glyph renderer declares one, and no */
/* code reads its fields. */
typedef struct GLYPH_RECT {
	u8 unused[8];
	u16 width;
	u16 height;
	u16 x;
	u16 y;
} GLYPH_RECT;

STATIC_ASSERT(sizeof(GLYPH_RECT) == 16);
STATIC_ASSERT(offsetof(GLYPH_RECT, width) == 8);
STATIC_ASSERT(offsetof(GLYPH_RECT, x) == 12);

/* A sprite's OAM attribute 2 (tile index, priority and palette) and the */
/* halfword after it, as one word or as fields. */
typedef union OAM_ATTR2_WORD {
	u32 value;
	struct {
		unsigned short idx : 10;
		unsigned short pri : 2;
		unsigned short pal : 4;
		u16 high;
	} bits;
	struct {
		u16 low;
		u16 high;
	} half;
} OAM_ATTR2_WORD;

/* Attributes zero through two copied to OAM, plus the halfword after them. */
typedef struct OAM_DATA {
	u16 y : 8; /* screen pixels */
	/* OBJ_AFFINE, and OBJ_DOUBLE_SIZE to turn it without clipping. */
	u16 affineMode : 2;
	u16 objMode : 2; /* an SPR_MODE */
	u16 mosaic : 1;
	u16 fullCol : 1; /* 256 colours instead of a 16-colour palette */
	u16 shape : 2;	 /* square, wide or tall, as the OBJ_SIZE_ macros */
	u16 x : 9;		 /* screen pixels */
	/* The affine matrix; for a plain sprite, the OBJ_HFLIP and OBJ_VFLIP */
	/* bits. */
	u16 matrix : 5;
	u16 size : 2; /* with shape, the sprite's size */
	u16 idx : 10; /* first sprite tile in VRAM */
	/* 0 to 3: drawn over every background of the same or a larger priority */
	/* number. */
	u16 pri : 2;
	u16 pal : 4; /* 16-colour palette */
	u16 high;
} OAM_DATA;

/* A sprite as the depth queue takes it: syOamAddSprAtDepth writes the link */
/* to the next sprite queued at the same depth, and the OAM attributes follow.
 */
typedef struct QUEUED_SPR {
	struct QUEUED_SPR *next; /* the next sprite queued at its depth */
	OAM_DATA oam;
} QUEUED_SPR;

/* WINDOW_SPR.kind: what a window sprite owns besides itself, which */
/* mnWindowSprRelease frees. */
typedef enum WINDOW_SPR_KIND {
	WINDOW_SPRITE_PLAIN = 0,   /* a glyph in the window's glyph tiles */
	WINDOW_SPRITE_ICON = 1,	   /* its own VRAM cache entry */
	WINDOW_SPRITE_PORTRAIT = 2 /* a VRAM cache entry and a palette slot */
} WINDOW_SPR_KIND;

/* A window sprite: a sprite in a window's sprite list (a character, an icon */
/* or a portrait), with its VRAM cache slot and OAM attributes. */
typedef struct WINDOW_SPR {
	struct WINDOW_SPR *next; /* the next sprite in its list */
	u8 kind;				 /* an WINDOW_SPR_KIND */
	u8 motion; /* its WINDOW_SPRITE_MOTION_ motion; 0 while free */
	u16 x;	   /* screen pixels */
	u16 y;	   /* screen pixels */
	u16 unused;
	u16 anmStep; /* step of its motion's animation */
	u8 idx;		 /* VRAM cache slot, or a glyph's tile after glyphTiles */
	/* The depth slot it is drawn at (syOamAddSprAtDepth): a higher slot is */
	/* drawn in front. The windows' glyphs take 254. */
	u8 depth;
	QUEUED_SPR spr; /* queued at its depth each frame */
} WINDOW_SPR;

/* A list of window sprites: its first sprite and the link to append after. */
typedef struct WINDOW_SPR_LIST {
	WINDOW_SPR *head;
	WINDOW_SPR **tailLink;
} WINDOW_SPR_LIST;

STATIC_ASSERT(sizeof(WINDOW_SPR_LIST) == 8);
STATIC_ASSERT(offsetof(WINDOW_SPR_LIST, tailLink) == 4);

STATIC_ASSERT(offsetof(WINDOW_SPR, anmStep) == 12);

STATIC_ASSERT(offsetof(WINDOW_SPR, depth) == 15);
STATIC_ASSERT(offsetof(WINDOW_SPR, spr) == 16);
STATIC_ASSERT(sizeof(WINDOW_SPR) == 28);

#endif
