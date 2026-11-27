#ifndef ALCHEMY_SPRITE_METADATA_H
#define ALCHEMY_SPRITE_METADATA_H

// Animation metadata and its lookup by resource.

#include "edition.h"
#include "static_assert.h"
#include "types.h"
#include "stubs.h"

// An animation's metadata: the sprite's size, scale and offsets, how its pictures are drawn and
// decoded, its pictures and its animation scripts.
typedef struct ANIMATION_METADATA
{
	Uint8   width;  // pixels
	Uint8   height; // pixels
	Uint16  scale;  // 8.8 fixed point, 256 full size
	// SPR_ANIMATION.kind: the facing table its pictures follow
	Uint8   drawKind;
	// The scripts in animation; an animation number at or past it is not played.
	Uint8   animationCount;
	Sint8   adjustX; // pixels
	Sint8   adjustY; // pixels
	// Pixels from the actor's position up to the top of its sprite (fdActorGetScreenPos).
	Sint8   spriteTop;
	// Pixels; half of it is a battle unit's radius, and can be an actor's radius (ACTOR.radius).
	Uint8   footprint;
	// an SPR_PICTURE_CODEC, copied to SPR_ANIMATION.mode
	Uint8   pictureCodec;
	// The picture pointers; null takes them from the sprite picture cache slot holding the
	// animation (fdSpritePictureCacheFindByTag).
	void  **pictures;
	Uint8 **animation; // the command scripts, one per animation number
} ANIMATION_METADATA;

STATIC_ASSERT(sizeof(ANIMATION_METADATA) == 20);
STATIC_ASSERT(offsetof(ANIMATION_METADATA, scale) == 2);
STATIC_ASSERT(offsetof(ANIMATION_METADATA, animationCount) == 5);
STATIC_ASSERT(offsetof(ANIMATION_METADATA, spriteTop) == 8);
STATIC_ASSERT(offsetof(ANIMATION_METADATA, footprint) == 9);
STATIC_ASSERT(offsetof(ANIMATION_METADATA, pictureCodec) == 10);
STATIC_ASSERT(offsetof(ANIMATION_METADATA, pictures) == 12);
STATIC_ASSERT(offsetof(ANIMATION_METADATA, animation) == 16);

// Every sprite animation's metadata, by number (graphics/sprite/sprites.s).
extern ANIMATION_METADATA SpriteAnimationMetadata[];

// The sprite-metadata table's function, its stub table's one entry (src/system/sprite_stubs.s).
ANIMATION_METADATA       *syResourceGetMetadataRecord(Uint32 descriptorId);

#endif
