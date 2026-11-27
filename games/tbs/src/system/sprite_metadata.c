/* Finding a sprite animation's metadata row. */

#include "system.h"
#include "system/sprite_metadata.h"
#include "graphics/battle_sprite_id.h"

/* Looks up animation metadata by the sprite id in descriptorId's SPRITE_ID_MASK
   bits: that row of Sprite_AnimationMetadata (graphics/sprite/sprites.s), with
   no bounds check. */
ANIMATION_METADATA *syResourceGetMetadataRecord(Uint32 descriptorId)
{
	return &Sprite_AnimationMetadata[descriptorId & SPRITE_ID_MASK];
}
