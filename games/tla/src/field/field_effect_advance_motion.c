/* @file */
/* A field effect's motion each frame. */

#include "system/field_effect.h"
#include "field/sprite.h"

/* An effect's update function: move by its velocity, which loses an eighteenth
 */
/* of its x and a sixteenth of its z each frame, and grow and spin at its */
/* rates. */
void fdEffectAdvanceMotion(FIELD_OBJECT *object) {
	FIELD_EFFECT *effect = &object->eff;
	s32 velocityX;
	s32 velocityZ;
	s32 scaleRateX;
	FIELD_SPR *sprite;

	velocityX = effect->vel.x;
	effect->x += velocityX;
	effect->y += effect->vel.y;
	velocityZ = effect->vel.z;
	effect->z += velocityZ;

	effect->vel.x = velocityX - velocityX / FIELD_EFFECT_DRAG_X;
	effect->vel.z = velocityZ - velocityZ / FIELD_EFFECT_DRAG_Z;

	scaleRateX = effect->scaleRateX;
	effect->scaleX += scaleRateX;
	sprite = effect->spr;
	effect->scaleY += effect->scaleRateY;
	sprite->rotation += effect->spin;
}
