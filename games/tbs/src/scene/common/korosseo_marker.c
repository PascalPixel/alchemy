#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "field_event.h"
#include "field_scene.h"
#include "io_reg.h"
#include "io_write_queue.h"
#include "types.h"
#include "call.h"
#include "animspr.h"
#include "object_runtime.h"
#include "fieldobj.h"
#include "callback_scheduler.h"
#include "vram_block.h"

struct CompetitorState
{
	u8 unknown_00[6];
	u8 mode;
	u8 stage;
};

void Korosseo_FadeInCompetitor(s32 id, s32 x, s32 z)
{
	struct CompetitorState *state;
	struct FieldActor	   *actor;
	struct AnimationObject *sprite;
	s32						i;

	state = (struct CompetitorState *)gMenuCtrlWork;
	actor = Object_GetById(id);
	state->mode = 1;
	state->stage = 4;
	Korosseo_CompetitorStartX = actor->x.fixed;
	Korosseo_CompetitorStartZ = actor->z.fixed;
	sprite = (struct AnimationObject *)actor->sprite;
	Korosseo_CompetitorStartAngle = actor->facing;
	Engine_ActorSetSpritePriority(id, 2);
	actor->priority_flags |= ACTOR_PRIORITY_AUTOMATIC;
	actor->facing = FACING_SOUTH;
	Engine_ActorSetSpriteFlags(actor, 3);
	Object_SetMode(actor, 0);
	Object_SetMode(actor, 1);
	Engine_ActorSetPosition(id, x << 16, z << 16);
	Engine_ActorFaceActor(0, 0x4000, 0);
	IO_WRITE_QUEUE_PUSH(0x4000050, 0xf00, 0x20000);
	sprite->part[0].object_mode = 1;
	sprite->part[1].object_mode = 1;
	Engine_AudioPlayCue(252);
	for (i = 0; i <= 15; i += 2)
	{
		actor->scale_x = (i << 12) + 0x1000;
		actor->scale_y = 0x1f000 - (i << 12);
		IO_WRITE_QUEUE_PUSH(0x4000052, ((15 - i) << 8) | (i + 1), 0x20000);
		Engine_TaskWait(1);
	}
	IO_WRITE_QUEUE_PUSH(0x4000052, 16, 0x20000);
	actor->scale_x = 0x11000;
	actor->scale_y = 0xf000;
	Engine_EventWait(1);
	actor->scale_x = 0x10000;
	actor->scale_y = 0x10000;
	Engine_EventWait(13);
	sprite->part[0].object_mode = 0;
	sprite->part[1].object_mode = 0;
	Engine_ActorSetAnimationAndWait(id, 3);
	Engine_EventWait(20);
}

void Korosseo_RestoreCompetitor(s32 id)
{
	struct CompetitorState *state;
	struct FieldActor	   *actor;
	s32						zero;
	u8					   *control;

	state = (struct CompetitorState *)gMenuCtrlWork;
	actor = Object_GetById(id);
	if (gGameState.movement_mode == 1)
	{
		gGameState.movement_mode = 0;
		Engine_ActorSetAnimation(id, 1);
	}
	else
	{
		Actor_FaceDirection(id, FACING_SOUTH, 30);
		Engine_ActorSetAnimation(id, 3);
		Engine_EventWait(30);
	}
	zero = 0;
	/* FAKEMATCH: needed to match: byte accesses keep the stage zero before the start-position load. */
	control = (u8 *)state;
	control[(u32) & ((struct CompetitorState *)0)->stage] = zero;
	control[(u32) & ((struct CompetitorState *)0)->mode] = 15;
	actor->x.fixed = Korosseo_CompetitorStartX;
	actor->z.fixed = Korosseo_CompetitorStartZ;
	actor->facing = Korosseo_CompetitorStartAngle;
	actor->target_x = ACTOR_NO_TARGET;
	actor->target_z = ACTOR_NO_TARGET;
	actor->velocity_x = zero;
	actor->velocity_z = zero;
	actor->motion_flags = 3;
	actor->terrain_id = 0;
	actor->y.fixed = zero;
	((struct ObjectRuntime *)actor)->terrain_height = zero;
	Engine_ActorSetSpriteFlags(actor, 1);
	Object_SetMode(actor, 0);
	Object_SetMode(actor, 1);
	Engine_TaskWait(1);
}

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

union MarkerCoordinate
{
	s16 signed_value;
	u16 value;
};

extern s16					  Korosseo_MarkerSteps;
extern s16					  Korosseo_MarkerStep;
extern s16					  Korosseo_MarkerX;
extern union MarkerCoordinate Korosseo_MarkerStartX;
extern s16					  Korosseo_MarkerEndX;
extern s16					  Korosseo_MarkerY;
extern union MarkerCoordinate Korosseo_MarkerStartY;
extern s16					  Korosseo_MarkerEndY;
extern s16					  Korosseo_MarkerBlink;
extern s16					  Korosseo_MarkerPriority;

static void Korosseo_UpdateMarker(void)
{
	s32						tile;
	s32						total;
	s32						step;
	s32					   *oam;
	s16					   *steps;
	s32					   *dst;
	s32						x;
	s32						y;
	s32						diff;
	s32						base;
	s16					   *py;
	union MarkerCoordinate *sy;

	oam = Korosseo_MarkerOam;
	tile = gVramBlockCache[Korosseo_MarkerSlot].offset >> 5;
	steps = &Korosseo_MarkerSteps;
	total = *steps;
	if (total != 0)
	{
		/* FAKEMATCH: needed to match: the signed read of the raw halfword keeps the step out of the interpolation registers. */
		step = (u16)++Korosseo_MarkerStep;
		py = &Korosseo_MarkerX;
		sy = &Korosseo_MarkerStartX;
		diff = Korosseo_MarkerEndX - sy->signed_value;
		base = sy->value;
		*py = base + diff * (s16)step / total;
		Korosseo_MarkerY =
			Korosseo_MarkerStartY.value
			+ (Korosseo_MarkerEndY - Korosseo_MarkerStartY.signed_value) * (s16)step / total;
		if ((s16)step >= total)
			*steps = 0;
		Korosseo_MarkerBlink = 0;
	}
	if (++Korosseo_MarkerBlink <= 13)
	{
		/* FAKEMATCH: needed to match: the size bit in a local puts the priority after it. */
		s32 size = 0x40000000;

		x = Korosseo_MarkerX;
		y = Korosseo_MarkerY;
		dst = oam;
		*dst++ = 0;
		*dst++ = (y - 8) | ((x - 8) << 16) | size | (Korosseo_MarkerPriority << 28);
		*dst = tile | 0x400;
		Runtime_PushSlotEntry(oam, 255);
	}
	else if (Korosseo_MarkerBlink > 19)
	{
		Korosseo_MarkerBlink = 0;
	}
}

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
