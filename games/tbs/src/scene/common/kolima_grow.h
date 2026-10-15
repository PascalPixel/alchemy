#include "types.h"
#include "field_obj.h"

static s32 SceneEffect_AdvanceAngleUntilIdle(struct FieldActor *p)
{
	p->scale_x += 0x1eb8;
	if (p->target_x == 0x80000000 && p->target_y == 0x80000000 && p->target_z == 0x80000000)
	{
		Engine_ObjectDispatchRelease(p);
	}
	return 1;
}
