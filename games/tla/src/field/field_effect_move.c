/* A field effect's motion, growth and spin each tick, with no drag. */

#include "system/field_effect.h"
#include "field/sprite.h"

/* An effect's update function: moves the effect by its velocity, which keeps
   its speed, grows it at its rates and turns its sprite by its spin. */
void fdEffectUpdateMotionScaleAndSpin(FIELD_OBJECT *object)
{
	Sint32 x;
	Sint32 velX;
	SPR	  *spr;

	x = object->eff.x;
	velX = object->eff.vel.x;
	spr = object->eff.spr;

	object->eff.x = x + velX;
	object->eff.y += object->eff.vel.y;
	object->eff.z += object->eff.vel.z;
	object->eff.scaleX += object->eff.scaleRateX;
	object->eff.scaleY += object->eff.scaleRateY;
	spr->rotation += object->eff.spin;
}
