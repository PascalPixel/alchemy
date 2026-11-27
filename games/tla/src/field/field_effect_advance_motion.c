/* A field effect's motion each tick. */

#include "system/field_effect.h"
#include "field/sprite.h"

/* An effect's update function: move by its velocity, which loses an eighteenth
   of its x and a sixteenth of its z each tick, and grow and spin at its
   rates. */
void fdEffectAdvanceMotion(FIELD_OBJECT *object)
{
	FIELD_EFFECT *effect = &object->eff;
	Sint32		  velX;
	Sint32		  velZ;
	Sint32		  scaleRateX;
	FIELD_SPR	 *spr;

	velX = effect->vel.x;
	effect->x += velX;
	effect->y += effect->vel.y;
	velZ = effect->vel.z;
	effect->z += velZ;

	effect->vel.x = velX - velX / FIELD_EFFECT_DRAG_X;
	effect->vel.z = velZ - velZ / FIELD_EFFECT_DRAG_Z;

	scaleRateX = effect->scaleRateX;
	effect->scaleX += scaleRateX;
	spr = effect->spr;
	effect->scaleY += effect->scaleRateY;
	spr->rotation += effect->spin;
}
