#include "types.h"
#include "facing_object.h"

static s32 OverlayObject_UpdateFacingTowardTarget(struct FacingObject *obj)
{
	s32					 delta;
	u16					 old;
	s32					 angle;
	struct FacingObject *target;

	target = obj->facing_target;
	if (target != 0)
	{
		obj->facing_flags = (0xFE & obj->facing_flags);
		angle = (u16)ArcTan2(
			target->position_z - obj->position_z, target->position_x - obj->position_x);
		old = obj->facing;
		delta = (s16)(angle - old);
		if (delta != 0)
		{
			if (delta > 0x1000)
			{
				delta = 0x1000;
			}

			if (delta < -0x1000)
			{
				delta = -0x1000;
			}
			obj->facing = (old + delta);
		}
	}
	return 1;
}
