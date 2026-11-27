#ifndef ALCHEMY_SPRITE_METADATA_H
#define ALCHEMY_SPRITE_METADATA_H

/* @file */
/* Animation metadata and its lookup by resource. */

#include "edition.h"
#include "static_assert.h"
#include "types.h"
#include "stubs.h"

/* An animation's metadata: the sprite's size, scale and offsets, how its */
/* pictures are drawn and decoded, its pictures and its animation scripts. */
typedef struct ANIMATION_METADATA {
	u8 width;	 /* pixels */
	u8 height;	 /* pixels */
	u16 scale;	 /* 8.8 fixed point, 256 full size */
	u8 drawKind; /* SPR_ANIMATION.kind: the facing table its pictures follow
				  */
	/* The scripts in animation; an animation number at or past it is not */
	/* played. */
	u8 anmCnt;
	s8 adjustX; /* pixels */
	s8 adjustY; /* pixels */
	/* Pixels from the actor's position up to the top of its sprite */
	/* (fdActorGetScrPos). */
	s8 sprTop;
	/* Pixels; half of it is a battle unit's radius, and can be an */
	/* actor's radius (ACTOR.radius). */
	u8 footprint;
	u8 pictureCodec; /* an SPR_PICTURE_CODEC, copied to */
					 /* SPR_ANIMATION.mode */
	/* The picture pointers; null takes them from the sprite picture cache slot
	 */
	/* holding the animation (fdSprPictureCacheFindByTag). */
	void **pictures;
	u8 **anm; /* the command scripts, one per animation number */
} ANIMATION_METADATA;

STATIC_ASSERT(sizeof(ANIMATION_METADATA) == 20);
STATIC_ASSERT(offsetof(ANIMATION_METADATA, scale) == 2);
STATIC_ASSERT(offsetof(ANIMATION_METADATA, anmCnt) == 5);
STATIC_ASSERT(offsetof(ANIMATION_METADATA, sprTop) == 8);
STATIC_ASSERT(offsetof(ANIMATION_METADATA, footprint) == 9);
STATIC_ASSERT(offsetof(ANIMATION_METADATA, pictureCodec) == 10);
STATIC_ASSERT(offsetof(ANIMATION_METADATA, pictures) == 12);
STATIC_ASSERT(offsetof(ANIMATION_METADATA, anm) == 16);

/* Every sprite animation's metadata, by number (graphics/sprite/sprites.s). */
extern ANIMATION_METADATA Sprite_AnimationMetadata[];

/* The sprite-metadata table's function, its stub table's one entry */
/* (src/system/sprite_stubs.s). */
ANIMATION_METADATA *syResourceGetMetadataRecord(u32 descriptorId);

#endif
