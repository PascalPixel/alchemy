#include "unit/system/resource/metadata_get_record.h"
#include "metadata_lookup.h"

extern struct AnimationMetadata gCharacter_DescriptorTable[];

/// Looks up animation metadata using the descriptor ID in the low twelve bits:
/// that row of gCharacter_DescriptorTable (graphics/sprite/sprites.s), with no
/// bounds check. The bits above are dropped, so the field's animation code
/// can pass a layer's animationId as it is, even when it carries a resource
/// slot in bits 12 and up (the slot's tag, slot << 12 | number;
/// SpriteFrameCache_FindByTag, field/field/animation.c). Reached through the
/// resource far-call table by the field's animation, objects and sprite
/// viewer, the Psynergy effects and the battle's objects.
struct AnimationMetadata *Resource_GetMetadataRecord(u32 descriptorId) {
  return &gCharacter_DescriptorTable[descriptorId & 0xfff];
}
