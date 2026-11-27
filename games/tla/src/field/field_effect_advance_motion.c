// A field effect's motion each tick.

#include "system/field_effect.h"
#include "field/sprite.h"

// An effect's update function: move by its velocity, which loses an eighteenth of its x and a
// sixteenth of its z each tick, and grow and spin at its rates.
void fdEffectAdvanceMotion(FIELD_OBJECT *object)
{
	FIELD_EFFECT *effect = &object->effect;
	Sint32        velocityX;
	Sint32        velocityZ;
	Sint32        scaleRateX;
	FIELD_SPR    *sprite;

	velocityX           = effect->velocity.x;
	effect->x          += velocityX;
	effect->y          += effect->velocity.y;
	velocityZ           = effect->velocity.z;
	effect->z          += velocityZ;

	effect->velocity.x  = velocityX - velocityX / FIELD_EFFECT_DRAG_X;
	effect->velocity.z  = velocityZ - velocityZ / FIELD_EFFECT_DRAG_Z;

	scaleRateX          = effect->scaleRateX;
	effect->scaleX     += scaleRateX;
	sprite              = effect->sprite;
	effect->scaleY     += effect->scaleRateY;
	sprite->rotation   += effect->spin;
}
