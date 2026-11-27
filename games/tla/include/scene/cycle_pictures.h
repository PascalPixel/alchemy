#ifndef ALCHEMY_CYCLE_PICTURES_H
#define ALCHEMY_CYCLE_PICTURES_H

/* @file */
/* A sprite that steps through its pictures and colour modes. */

#include "types.h"

struct SPR_ANIMATION;

/* A field sprite (FIELD_SPR) as the scenes that cycle its pictures */
/* read it: its rotation, the flag that redraws it, its sprite flags and its */
/* animation. */
typedef struct ANIMATED_SPR {
	u8 link[4];
	u8 attr0[2];
	u8 attr1[2];
	u8 attr2[2];
	u8 attr3[2];
	u8 scale[4];
	u8 vramBlock; /* VRAM cache entry holding its tiles */
	u8 cacheFlg;
	s16 rotation; /* 1/65536 turns */
	u8 size[4];	  /* width, height and the x and y offsets */
	u8 lastAnm;	  /* the animation number last selected */
	u8 dirty;	  /* nonzero redraws its pixels */
	u8 flg;		  /* the sprite flags (SPR_FLAG) */
	/* The layer count, then the shadow's link and attributes. */
	u8 shadow[13];
	struct SPR_ANIMATION *anm;
} ANIMATED_SPR;

#endif
