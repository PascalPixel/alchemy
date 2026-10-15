#include "types.h"
#include "field_obj.h"
#include "field_event.h"

static void SceneState_SetFlagByActorPosition(void)
{
	struct FieldActor *p0 = Object_GetById(0);
	s32				   rx = p0->x.fixed;
	s32				   x;
	s32				   z;

	struct FieldActor *p1 = Object_GetById(0);
	x = rx >> 20;
	z = p1->z.fixed;
	x -= 34;
	z >>= 20;

	if ((u32)x <= 1 && z > 40 && z <= 42)
	{
		Engine_GameFlagSet(148 << 2);
	}
	else
	{
		Engine_GameFlagClear(148 << 2);
	}
}
