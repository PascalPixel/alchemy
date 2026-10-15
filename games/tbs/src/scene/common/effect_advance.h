#include "types.h"
#include "field_effect.h"

static void Effect_AdvanceMotion(union FieldObject *object)
{
	struct FieldEffect *effect = &object->effect;

	effect->x += effect->velocity_x;
	effect->y += effect->velocity_y;
	effect->z += effect->velocity_z;
	effect->velocity_x -= effect->velocity_x / 18;
	effect->velocity_z -= effect->velocity_z / 16;
	effect->scale_x += effect->scale_rate_x;
	effect->scale_y += effect->scale_rate_y;
	effect->sprite->rotation += effect->spin;
}
