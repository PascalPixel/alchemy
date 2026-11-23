#ifndef ALCHEMY_SPRITE_METADATA_H
#define ALCHEMY_SPRITE_METADATA_H

/// @file
/// Animation metadata and its lookup by resource.

#include "edition.h"
#include "static_assert.h"
#include "types.h"
#include "gamepak/far_call.h"

/// An animation's metadata: the sprite's size, scale and offsets, how its
/// pictures are drawn and decoded, its pictures and its animation scripts.
struct AnimationMetadata {
  u8 width;    ///< pixels
  u8 height;   ///< pixels
  u16 scale;   ///< 8.8 fixed point, 256 full size
  u8 drawKind; ///< SpriteAnimation.kind: the facing table its pictures follow
  /// The scripts in animation; an animation number at or past it is not
  /// played.
  u8 animationCount;
  s8 adjustX; ///< pixels
  s8 adjustY; ///< pixels
  /// Pixels from the actor's position up to the top of its sprite
  /// (Actor_GetScreenPosition).
  s8 boxX;
  /// Pixels; half of it is a battle unit's radius, and can be an
  /// actor's radius (Actor.radius).
  u8 boxY;
  u8 pictureCodec; ///< an enum SpritePictureCodec, copied to
                   ///< SpriteAnimation.mode
  /// The picture pointers; null takes them from the sprite picture cache slot
  /// holding the animation (SpritePictureCache_FindByTag).
  void **pictures;
  u8 **animation; ///< the command scripts, one per animation number
};

STATIC_ASSERT(sizeof(struct AnimationMetadata) == 20);
STATIC_ASSERT(offsetof(struct AnimationMetadata, scale) == 2);
STATIC_ASSERT(offsetof(struct AnimationMetadata, animationCount) == 5);
STATIC_ASSERT(offsetof(struct AnimationMetadata, boxX) == 8);
STATIC_ASSERT(offsetof(struct AnimationMetadata, boxY) == 9);
STATIC_ASSERT(offsetof(struct AnimationMetadata, pictureCodec) == 10);
STATIC_ASSERT(offsetof(struct AnimationMetadata, pictures) == 12);
STATIC_ASSERT(offsetof(struct AnimationMetadata, animation) == 16);

/// Every sprite animation's metadata, by number (graphics/sprite/sprites.s).
extern struct AnimationMetadata gSprite_AnimationMetadata[];

struct AnimationMetadata *Resource_GetMetadataRecord(u32 descriptorId);

#endif
