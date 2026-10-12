#include "types.h"
#include "fieldobj.h"
#include "field_event.h"

void SceneEffect_SeedMarkerAndInstallTask(u32 x, u32 y, u32 style)
{
	Korosseo_EnsureMarkerSlot();

	Korosseo_MarkerX = x;
	Korosseo_MarkerY = y;
	Korosseo_MarkerPriority = (style & 3);
	Korosseo_MarkerBlink = 0;
	Korosseo_MarkerSteps = 0;

	Engine_TaskAddCallback((s32)Korosseo_UpdateMarker, 0xc80);
}

void SceneState_StartMarkerMove(u32 x, u32 y, u32 duration)
{
	Korosseo_MarkerEndX = x;
	Korosseo_MarkerEndY = y;
	*(u16 *)&Korosseo_MarkerStartX = Korosseo_MarkerX;
	*(u16 *)&Korosseo_MarkerStartY = Korosseo_MarkerY;
	Korosseo_MarkerSteps = duration;
	Korosseo_MarkerStep = 0;

	Engine_TaskAddCallback((s32)Korosseo_UpdateMarker, 0xc80);
}

void SceneEffect_RemoveMarkerTask(void)
{

	Engine_TaskRemoveCallback(Korosseo_UpdateMarker);
	Resource_ResetEntry(Korosseo_MarkerSlot);
	Korosseo_MarkerSlot = -1;
}

void SceneActor_StartMode5MoveToTile(s32 a, s32 b, s32 c)
{
	struct FieldActor *o = Engine_ActorLookup(a);

	if (o != 0)
	{
		s32 v = 0x20000;
		s32 z = 0;

		o->speed = v;
		o->acceleration = v >> 1;
		o->paused = z;
		ObjectDispatch_InitFromTable6(o);
		Object_SetMode(o, 5);
		Engine_ObjectSetPosition(o, b << 16, o->y.fixed, c << 16);
	}
}

void OverlayObject_PlaceWithScale14000(s32 a, s32 b, s32 c)
{
	struct FieldActor *o = Engine_ActorLookup(a);

	if (o != 0)
	{
		s32 v = 0x14000;
		s32 z = 0;

		o->speed = v;
		o->acceleration = v >> 1;
		o->paused = z;
		ObjectDispatch_InitFromTable6(o);
		Object_SetMode(o, 5);
		Engine_ObjectSetPosition(o, b << 16, o->y.fixed, c << 16);
		Object_CommitPosition(o);
		Object_SetMode(o, 1);
	}
}
