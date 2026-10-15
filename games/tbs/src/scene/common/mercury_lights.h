#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "mem.h"
#include "dma.h"
#include "ram_buffer.h"
#include "field_event.h"
#include "field_scene.h"
#include "object_runtime.h"
#include "animspr.h"
#include "map.h"
#include "io_reg.h"

struct MercuryLights
{
	s32				   lit;
	s32				   region;
	s32				   pad08[3];
	struct FieldActor *lower;
	struct FieldActor *upper;
};

void Mercury_SpawnLightObjects(s32 region, struct MercuryLights *st)
{
	struct ObjectRuntime *actor;
	struct FieldActor	 *obj;
	struct FieldSprite	 *anim;
	struct MapCell		 *cell;
	s32					  z;
	s32					  flag;

	*(struct MercuryLights **)Runtime_AllocateBlock(35, sizeof(st)) = st;
	flag = Engine_GameFlagIsSet(0x109);
	if (flag == 0)
	{
		Dma_Fill32(flag, st, sizeof(*st));
		st->region = region;
		return;
	}
	actor = Engine_ActorLookup(gGameState.selectedActor);
	z = actor->z;
	cell = (struct MapCell *)Ram_MapCellBuffer + ((z / 0x100000) << 7) + actor->x / 0x100000;
	if (st->lit != 0 && st->lower != 0)
	{
		obj = Engine_ObjectCreate(26, actor->x, actor->y + 0x180000, z);
		if (obj != 0)
		{
			obj->terrainHeight = actor->terrainHeight;
			anim = obj->sprite;
			Engine_ObjectSetScript(obj, gMercury_PillarScript);
			obj->linkedObject = (struct FieldActor *)actor;
			obj->motionFlags = 4;
			obj->y.fixed += -0x8000;
			if (anim != 0)
			{
				AnimationObjects_SelectAnimation(anim, 6 - st->lit);
				anim->flags = 0;
				anim->priority = 1;
			}
			st->lower = obj;
		}
	}
	else
	{
		st->lower = 0;
	}
	if (cell->collisionCode == region && st->upper != 0)
	{
		obj = Engine_ObjectCreate(26, actor->x, actor->y, actor->z);
		if (obj == 0)
			return;
		obj->terrainHeight = actor->terrainHeight;
		anim = obj->sprite;
		Engine_ObjectSetScript(obj, gMercury_RampScript);
		obj->motionFlags = 0;
		obj->timer = 0;
		obj->priorityFlags = 2;
		obj->speed = 0x40000;
		if (anim != 0)
		{
			AnimationObjects_SelectAnimation(anim, 6);
			anim->flags = 0;
		}
		st->upper = obj;
	}
	else
	{
		st->upper = 0;
	}
}
