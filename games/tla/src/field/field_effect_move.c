// A field effect's motion, growth and spin each tick, with no drag.

#include "system/field_effect.h"
#include "field/sprite.h"

// An effect's update function: moves the effect by its velocity, which keeps its speed, grows it at
// its rates and turns its sprite by its spin.
void fdEffectUpdateMotionScaleAndSpin(FIELD_OBJECT *object)
{
	Sint32 x;
	Sint32 velocityX;
	SPR   *sprite;

	x                      = object->effect.x;
	velocityX              = object->effect.velocity.x;
	sprite                 = object->effect.sprite;

	object->effect.x       = x + velocityX;
	object->effect.y      += object->effect.velocity.y;
	object->effect.z      += object->effect.velocity.z;
	object->effect.scaleX += object->effect.scaleRateX;
	object->effect.scaleY += object->effect.scaleRateY;
	sprite->rotation      += object->effect.spin;
}
