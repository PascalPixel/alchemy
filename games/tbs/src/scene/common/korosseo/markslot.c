#include "types.h"
#include "fieldobj.h"

void OverlayObject_ResetMotionFields(s32 actor)
{
	struct FieldActor *o = Object_GetById(actor);

	ObjectDispatch_InitFromTable6(o);
	o->velocity_x = 0;
	o->velocity_z = 0;
	o->target_x = ACTOR_NO_TARGET;
	o->target_z = ACTOR_NO_TARGET;
}

static void Korosseo_EnsureMarkerSlot(void)
{

	if (Korosseo_MarkerSlot == -1)
	{
		Korosseo_MarkerSlot = Resource_LoadFixedBlockBIntoFreeSlot();
	}
}
