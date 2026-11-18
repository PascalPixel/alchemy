/// @file
/// Finding a sprite animation's metadata row.

#include "system/sprite_metadata.h"
#include "graphics/battle_sprite_id.h"

/// Looks up animation metadata by the sprite id in descriptorId's
/// SPRITE_ID_MASK bits: that row of gSprite_AnimationMetadata
/// (graphics/sprite/sprites.s), with no bounds check.
struct AnimationMetadata *Resource_GetMetadataRecord(u32 descriptorId) {
  return &gSprite_AnimationMetadata[descriptorId & SPRITE_ID_MASK];
}
