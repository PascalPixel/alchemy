#ifndef ALCHEMY_FIELD_SPRITE_H
#define ALCHEMY_FIELD_SPRITE_H

/* @file */
/* An actor's sprite as the field's scenes read it. */

#include "fixed_math.h"
#include "types.h"

struct SPR;
struct SPR_ANIMATION;

/* SPR (field/sprite.h) as the field's scenes read it: its image's */
/* OAM attributes as bit fields, its retain, own-scale and sunk-rows bits, its
 */
/* layer count and its shadow's first two attributes as bytes, and its */
/* shadow's attribute 2 as bit fields; the first layer is record. */
typedef struct FIELD_SPR {
	/* The link to the next sprite queued at its depth (struct */
	/* ANIMATION_SPR_PART's first word). */
	struct SPR *link;
	/* The image's OAM attributes 0 to 2. */
	/* @{ */
	u16 y : 8; /* screen pixels */
	u16 affine : 2;
	u16 objMode : 2;
	u16 mosaic : 1;
	u16 fullCol : 1; /* 256 colours */
	u16 shape : 2;
	u16 x : 9; /* screen pixels */
	u16 affineIdx : 5;
	u16 size : 2;
	u16 tile : 10; /* first sprite tile in VRAM */
	u16 pri : 2;
	u16 pal : 4;
	/* @} */
	u8 unusedAfterPal[2]; /* no code names it */
	FIXED scale;		  /* 16.16 fixed point, FIXED_ONE full size */
	u8 slot;			  /* VRAM cache entry holding its tiles */
	u8 retainCacheEntry : 1;
	u8 useOwnScale : 1;
	u8 sunkRows : 6; /* rows SPRITE_FLAG_SUNK sinks or SPRITE_FLAG_SHADED shades
					  */
	u16 rotation;	 /* 1/65536 turns */
	u8 width;		 /* pixels; zero marks a free sprite */
	u8 height;		 /* pixels */
	s8 ofsX;		 /* pixels */
	s8 ofsY;		 /* pixels */
	/* The animation number last selected, saved with the actor's snapshot. */
	u8 lastNo;
	u8 dirty; /* nonzero redraws its pixels into VRAM when next drawn */
	/* The sprite flags (SPR_FLAG), saved with the actor's snapshot; */
	/* zeroed for sprites shown without them. */
	u8 flg;
	u8 cnt; /* the layer count (SPR.cnt), cleared with the first layer */
	/* The shadow's link and attributes 0 and 1; no code names them. */
	u8 unusedShadow[8];
	/* The shadow's OAM attribute 2. */
	/* @{ */
	u16 secondTile : 10;
	u16 partPri : 2;
	u16 secondPal : 4;
	/* @} */
	u8 unusedAfterSecondPal[2];	  /* no code names it */
	struct SPR_ANIMATION *record; /* released by fdSprAnimationClearStub */
} FIELD_SPR;

/* The depth a sprite the scene draws itself takes past its screen row, just */
/* behind an actor's image (+ PLACEMENT_IMAGE_DEPTH, field/field.h) and shadow
 */
/* (two slots less) at the same spot: light sprites, a hole's mask, map cell */
/* sprites, fountains and ropes. */
#define SCENE_SPRITE_DEPTH 58

#endif
