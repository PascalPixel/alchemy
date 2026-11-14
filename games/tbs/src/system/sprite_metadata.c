/// @file
/// Finding a sprite animation's metadata row.

#include "system/sprite_metadata.h"

/// Looks up animation metadata using the descriptor ID in the low twelve bits:
/// that row of gSprite_AnimationMetadata (graphics/sprite/sprites.s), with no
/// bounds check.
struct AnimationMetadata *Resource_GetMetadataRecord(u32 descriptorId) {
  return &gSprite_AnimationMetadata[descriptorId & 0xfff];
}
