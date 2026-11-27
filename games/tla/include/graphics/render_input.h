#ifndef ALCHEMY_RENDER_INPUT_H
#define ALCHEMY_RENDER_INPUT_H

/* Sprites as OAM takes them, and the window sprites: the characters, icons and
   portraits a window carries. */

#include "types.h"
#include "static_assert.h"
#include "edition.h"
#include "oam.h"

struct WINDOW;

Sint32 mnWindowSprCreateAbilityIcon(
	struct WINDOW *window, Sint32 x, Sint32 y, Sint32 resource);

/* Attributes zero through two as the hardware lays them out in OAM, plus the
   halfword after them. */
typedef struct OAM_DATA {
	Uint16 y : 8;		   /* screen pixels */
	Uint16 affineMode : 2; /* 0 plain, 1 affine, 3 affine at double size */
	Uint16 objMode : 2;	   /* an SPR_MODE */
	Uint16 mosaic : 1;
	Uint16 fullCol : 1; /* 256 colours instead of a 16-colour palette */
	Uint16 shape : 2;	/* 0 square, 1 wide, 2 tall */
	Uint16 x : 9;		/* screen pixels */
	/* The affine matrix; for a plain sprite, 8 mirrors it left to right and 16
	   top to bottom. */
	Uint16 matrix : 5;
	Uint16 size : 2; /* with shape, the sprite's size */
	Uint16 idx : 10; /* first sprite tile in VRAM */
	/* 0 to 3: drawn over every background of the same or a larger priority
	   number. */
	Uint16 pri : 2;
	Uint16 pal : 4; /* 16-colour palette */
	Uint16 high;
} OAM_DATA;

/* A sprite as the depth queue takes it: syOAMAddSprAtDepth writes the link to
   the next sprite queued at the same depth, and the OAM attributes follow. */
typedef struct QUEUED_SPR {
	struct QUEUED_SPR *next; /* the next sprite queued at its depth */
	OAM_DATA		   oam;
} QUEUED_SPR;

/* A window sprite: a sprite in a window's sprite list (a character, an icon or
   a portrait), with its VRAM cache slot and OAM attributes. */
typedef struct WINDOW_SPR {
	struct WINDOW_SPR *next; /* the next sprite in its list */
	Sint8			   kind; /* an WINDOW_SPR_KIND */
	Sint8  motion; /* its WINDOW_SPRITE_MOTION_ motion; 0 while free */
	Uint16 x;	   /* screen pixels */
	Uint16 y;	   /* screen pixels */
	Uint16 unused;
	Uint16 anmStep; /* step of its motion's animation */
	Sint8  idx;		/* VRAM cache slot, or a glyph's tile after glyphTiles */
	/* The depth slot it is drawn at (syOAMAddSprAtDepth): a higher slot is
	   drawn in front. */
	Uint8	   depth;
	QUEUED_SPR spr; /* queued at its depth each frame */
} WINDOW_SPR;

/* The queued sprite of window sprite `sprite` (WINDOW_SPR.spr). */
static inline QUEUED_SPR *mnWindowSprGetSpr(WINDOW_SPR *sprite)
{
	return &sprite->spr;
}

/* A list of window sprites: its first sprite and the link to append after. */
typedef struct WINDOW_SPR_LIST {
	WINDOW_SPR	*head;
	WINDOW_SPR **tailLink;
} WINDOW_SPR_LIST;

STATIC_ASSERT(sizeof(WINDOW_SPR_LIST) == 8);
STATIC_ASSERT(offsetof(WINDOW_SPR_LIST, tailLink) == 4);

/* Takes a free window sprite showing VRAM cache slot `slot`, places it
   (offsetX, offsetY) pixels from the window's inner corner and appends it to
   the window's list. */
WINDOW_SPR *mnWindowSprCreateStub(Sint32 slot, Sint32 oamFlags,
	struct WINDOW *window, Sint32 offsetX, Sint32 offsetY);

STATIC_ASSERT(offsetof(WINDOW_SPR, anmStep) == 12);

STATIC_ASSERT(offsetof(WINDOW_SPR, depth) == 15);
STATIC_ASSERT(offsetof(WINDOW_SPR, spr) == 16);
STATIC_ASSERT(offsetof(WINDOW_SPR, spr.oam) == 20);
STATIC_ASSERT(sizeof(WINDOW_SPR) == 28);

#endif
