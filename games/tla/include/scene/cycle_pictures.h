#ifndef ALCHEMY_CYCLE_PICTURES_H
#define ALCHEMY_CYCLE_PICTURES_H

/* A sprite that steps through its pictures and colour modes. */

#include "types.h"

struct SPR_ANIMATION;

/* A field sprite (FIELD_SPR) as the scenes that cycle its pictures read it: its
   rotation, the flag that redraws it, its sprite flags and its animation. */
typedef struct ANIMATED_SPR {
	Uint8  link[4];
	Uint8  attr0[2];
	Uint8  attr1[2];
	Uint8  attr2[2];
	Uint8  attr3[2];
	Uint8  scale[4];
	Uint8  vramBlock; /* VRAM cache entry holding its tiles */
	Uint8  cacheFlg;
	Sint16 rotation; /* 1/65536 turns */
	Uint8  size[4];	 /* width, height and the x and y offsets */
	Uint8  lastAnm;	 /* the animation number last selected */
	Uint8  dirty;	 /* nonzero redraws its pixels */
	Uint8  flg;		 /* the sprite flags (SPR_FLAG) */
	/* The layer count, then the shadow's link and attributes. */
	Uint8				  shadow[13];
	struct SPR_ANIMATION *anm;
} ANIMATED_SPR;

#endif
