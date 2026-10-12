#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "makyuri.h"
#include "field_effect.h"
#include "iwram_call.h"
#include "ram_buffer.h"
#include "map.h"
#include "fieldrun.h"
#include "heap_state.h"

#include "keystate.h"

struct PushState
{
	s32				   count;
	s32				   level;
	s32				   active;
	s32				   target_x;
	s32				   target_z;
	struct FieldActor *source;
	struct FieldActor *effect;
};

#define MAP_CELLS ((struct MapCell *)Ram_MapCellBuffer)

void SceneState_ClearCurrentRecordAndReleaseTarget(void)
{
	s32 *record = **(s32 ***)&((struct WorkPointers *)&gEventWork)->scene;
	s32 *target;

	if (record[0] == 0)
	{
		return;
	}

	record[0] = 0;
	Engine_GameFlagClear(0x161);

	target = (s32 *)record[5];
	if (target != 0)
	{
		*(short *)((u8 *)target + 0x64) = 0;
		Engine_ObjectSetScript(target, (s32)Makyuri_ScaleCounterScript);
		Object_SetMode(target, 7);
		record[5] = 0;
	}
}

struct SceneTimer
{
	u8	unknown_00[8];
	s32 count;
};

void Makyuri_RunActorMove(void)
{
	struct FieldStepWork *work;
	struct PushState	 *state;
	struct FieldActor	 *actor;
	struct FieldActor	 *source;
	struct FieldActor	 *obj;
	struct FieldSprite	 *sprite;
	struct MapCell		 *from;
	struct MapCell		 *to;
	u8					  saved;
	u8					 *flags;
	s32					  angle;
	u32					  keys;
	s32					  n;
	s32					  i;
	u8					  zero;
	union FieldCoordinate pos[3];

	state = *(struct PushState **)WORK_SLOT(35);
	work = WORK_SLOT(HEAP_SLOT_EVENT);
	actor = (struct FieldActor *)work->actors[gGameState.selected_actor];
	flags = &actor->motion_flags;
	saved = *flags;
	keys = gKeysHeld;
	angle = Makyuri_MoveAngles[(keys >> 4) & 15];
	if ((s16)Makyuri_MoveAngles[(keys >> 4) & 15] == -1)
		return;
	pos[0].fixed = (actor->x.fixed & -0x100000) + 0x80000;
	pos[1].fixed = ((struct ObjectRuntime *)actor)->terrain_height;
	pos[2].fixed = (actor->z.fixed & -0x100000) + 0x80000;
	from = &MAP_CELLS[(pos[2].fixed / 0x100000 << 7) + pos[0].fixed / 0x100000];
	Vector_AddPolarOffset(0x200000, angle, &pos[0].fixed);
	to = &MAP_CELLS[(pos[2].fixed / 0x100000 << 7) + pos[0].fixed / 0x100000];
	if (from->collision_code != state->level && to->collision_code == state->level
		&& state->count == 0)
		return;

	Engine_EventBegin();
	if (Value2(Object_CheckMovementCollision, (s32)actor, (s32)pos) != 0)
		return;
	obj = state->effect;
	if (obj != NULL)
	{
		((union FieldObject *)obj)->effect.spin = 0;
		Engine_ObjectSetScript(obj, Makyuri_ScaleCounterScript);
		Object_SetMode(obj, 7);
		state->effect = NULL;
	}
	if (to->collision_code == state->level && state->count != 0)
	{
		source = state->source;
		obj = Engine_ObjectCreate(26, source->x.fixed, source->y.fixed, source->z.fixed);
		if (obj != NULL)
		{
			sprite = obj->sprite;
			((struct ObjectRuntime *)obj)->terrain_height =
				((struct ObjectRuntime *)source)->terrain_height;
			Engine_ObjectSetScript(obj, Makyuri_RampScript);
			obj->motion_flags = 0;
			((union FieldObject *)obj)->effect.spin = 0;
			obj->priority_flags = 2;
			obj->speed = 0x40000;
			obj->acceleration = 0x20000;
			Engine_ObjectSetPosition(obj, pos[0].fixed, pos[1].fixed, pos[2].fixed);
			if (sprite != NULL)
			{
				AnimationObjects_SelectAnimation(sprite, 6);
				zero = 0;
				sprite->flags = zero;
			}
			state->effect = obj;
		}
		n = state->count - 1;
		state->count = n;
		if (n == 0)
		{
			Engine_ObjectDispatchRelease(state->source);
			state->source = NULL;
			Engine_GameFlagClear(0x161);
		}
		else if (state->source != NULL)
		{
			Object_SetMode(state->source, 6 - n);
		}
	}
	Object_SetMode(actor, 6);
	Engine_TaskWait(3);
	Engine_AudioPlayCue(152);
	Object_SetMode(actor, 7);
	actor->speed = 0x30000;
	actor->acceleration = 0x20000;
	actor->velocity_y = 0x40000;
	*flags &= 0x7e;
	Engine_ActorSetSpriteFlags(actor, 0);
	Engine_ActorMoveToAndWait(0, pos[0].part.pixel, pos[2].part.pixel);
	Object_SetMode(actor, 6);
	Engine_TaskWait(2);
	if (to->collision_code != state->level)
		Engine_ActorSetSpriteFlags(actor, 1);
	else
		Engine_AudioPlayCue(215);
	Engine_TaskWait(1);
	*flags = saved;
	if (to->collision_code == state->level && state->effect == NULL)
	{
		Object_SetMode(actor, 18);
		Engine_AudioPlayCue(241);
		for (i = 0;; i++)
		{
			if ((i & 15) == 0)
				OverlayObject_SpawnKind24AtObject(actor);
			if (i > 31 && gKeyState != 0)
				break;
			Engine_TaskWait(1);
		}
		Engine_AudioPlayCue(0x120);
		Engine_TaskWait(1);
		actor->x.fixed = state->target_x;
		actor->z.fixed = state->target_z;
		Engine_ActorSetSpriteFlags(actor, 1);
	}
	state->active = 0;
	Engine_EventEnd();
	work->encounter_steps += Iwram_MulQ16(work->encounter_rate, 0x200000);
}

void Makyuri_TickSpawnTimer(void)
{
	struct SceneTimer *timer;
	struct FieldActor *point;

	timer = *(struct SceneTimer **)WORK_SLOT(35);
	point = (struct FieldActor *)((struct FieldStepWork *)WORK_SLOT(HEAP_SLOT_EVENT))
				->actors[gGameState.selected_actor];
	if (timer->count != 0)
	{
		timer->count--;
	}
	else
	{
		OverlayObject_SpawnKind24AtObject(point);
		timer->count = ((u32)(Engine_RandomNext() * 30) >> 16) + 10;
	}
}

static void OverlayObject_ReleasePublishedAttachment(void)
{
	u8 **pub = Runtime_AllocateBlock(35, 4);
	u8	*state;
	u8	*obj;

	if (pub == 0)
		return;
	state = *pub;
	obj = *(u8 **)(state + 20);
	if (obj == 0)
		return;
	Engine_ObjectDispatchRelease(obj);
	*(u8 **)(state + 20) = 0;
}
