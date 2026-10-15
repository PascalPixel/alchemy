#include "types.h"
#include "field_obj.h"
#include "game_state.h"

static void SceneActor_PlacePairAtOffset(s32 id, s32 dx, s32 dz)
{
	struct FieldActor *actor;
	struct FieldActor *other;
	s32				   x;
	s32				   z;

	actor = Object_GetById(gGameState.selected_actor);
	other = Object_GetById(id);
	Engine_EventBegin();
	{
		x = ((actor->x.fixed + (dx << 16)) & 0xFFF00000) + 0x80000;
		z = ((actor->z.fixed + (dz << 16)) & 0xFFF00000) + 0x80000;

		actor->speed = 0x10000;
		actor->acceleration = 0x8000;
		Engine_ObjectSetPosition(actor, x, actor->y.fixed, z);
	}
	Object_SetMode(actor, 27);
	{
		x = ((other->x.fixed + (dx << 16)) & 0xFFF00000) + 0x80000;
		z = ((other->z.fixed + (dz << 16)) & 0xFFF00000) + 0x80000;

		other->speed = 0x10000;
		other->acceleration = 0x8000;
		Engine_ObjectSetPosition(other, x, other->y.fixed, z);
	}
	if (dx < 0 || dz < 0)
	{
		Object_SetMode(other, 4);
	}
	else
	{
		Object_SetMode(other, 3);
	}
	Object_CommitPosition(actor);
	Engine_EventEnd();
}
