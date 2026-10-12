#include "game_state.h"
#include "vram_tab.h"
#include "unit/import.h"
#include "unit/field/vinasu_chojo/chojo.h"
#include "unit/field/common/effect/move.h"
#include "unit/battle/effect/slots.h"
#include "unit/field/common/script/script.h"
#include "unit/game/character/chara.h"
#include "unit/graphics/window/measure.h"
#include "unit/system/scheduler/callback.h"

#define NULL ((void *)0)
#include "fieldobj.h"
#include "types.h"
#include "vinasu.h"
#include "field_effect.h"
#include "field_event.h"
#include "field_scene.h"
#include "sound_ids.h"
#include "create_configured_overlay_object.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "resource.h"
#include "iwram_call.h"
#include "vram_block.h"
#include "callback_scheduler.h"
#include "call.h"
#include "field_service.h"
#include "b5_context.h"
#include "text/msg_ids.h"
#include "global_cells.h"
#include "scene_ids.h"
#include "staged_actor.h"
#include "map_scroll.h"
#include "tbs_edition.h"
#include "io_reg.h"
#include "window.h"
#include "heap_state.h"
#include "objcmd.h"

typedef struct
{
	u8	 filler0[0x28];
	s16 *unk28;
} Obj_020004bc;

typedef struct
{
	u8			  filler0[0x50];
	Obj_020004bc *obj;
} Spr_020004bc;

s32 Math_RemainderUnsigned(s32, s32);

void SetEffectRecordMode(struct FieldActor *work, s32 mode)
{
	work->sprite->priority = mode;
}

void *OverlayObject_PrepareObject(s32 x, s32 y, s32 z, s32 type)
{
	struct FieldActor *object = Engine_ObjectCreate(type, x, y, z);

	if (object != 0)
	{
		object->sprite->priority = 0;
		object->motion_flags = 0;
		object->collision_flags = 8;
		Engine_ActorSetSpriteFlags(object, 0);
		ObjectGroup_SetChildValue(object, 14);
		Engine_ObjectSetBlendMode(object, 1);
		return object;
	}
	return 0;
}

void *OverlayObject_CreateConfiguredObject(s32 x, s32 y, s32 z, s32 type)
{
	struct FieldActor *object = Engine_ObjectCreate(type, x, y, z);

	if (object != 0)
	{
		object->sprite->priority = 1;
		object->motion_flags = 0;
		object->collision_flags = 8;
		Engine_ActorSetSpriteFlags(object, 0);
		ObjectGroup_SetChildValue(object, 15);
		object->priority_flags = (object->priority_flags & 0xfe) | 2;
		return object;
	}
	return 0;
}

// ここに共通ソースが入る（ROM の並びのとおり）
#include "../common/effect/move.c"
#include "../common/effect/spawnov.c"

struct PairDetail
{
	u8 unknown_00[22];
	u8 field_16;
};

struct PairSprite
{
	struct FieldSprite sprite;
	struct PairDetail *detail;
};

union PairObject
{
	union FieldObject object;
	s32				  words[28];
	struct
	{
		u8				  unknown_00[0x68];
		union PairObject *parent;
	} link;
};

struct PairWork
{
	u8	unknown_00[70];
	u16 vram_block;
};

extern struct PairWork *gEffectWork;

struct WorldMapOam
{
	u8	unknown_00[4];
	u16 attr0;
	u16 x : 9;
	u16 affine_index : 5;
	u16 size : 2;
};

s32 OverlayObject_StepScaleByCounter(struct FieldActor *actor)
{
	s16 *counter = (s16 *)&actor->timer;

	switch (*counter)
	{
	case 6:
		actor->scale_x += -0x2000;
		actor->scale_y += 0x1000;
		break;
	case 4:
		actor->scale_x += 0x1000;
		actor->scale_y += -0x800;
		break;
	case 2:
		actor->scale_x += 0x800;
		actor->scale_y += -0x400;
		break;
	case 0:
		actor->scale_x += 0x800;
		actor->scale_y += -0x400;
		*counter = Math_RemainderUnsigned(Engine_RandomNext(), 80) + 80;
		break;
	}
	{
		s32 t = (u16)actor->timer;
		t--;
		actor->timer = t;
	}
	return 1;
}

void Effect_MoveWithDrag(union FieldObject *object)
{
	s32 velocity_x = object->effect.velocity_x;
	s32 velocity_z;

	object->effect.x += velocity_x;
	object->effect.y += object->effect.velocity_y;
	velocity_z = object->effect.velocity_z;
	object->effect.z += velocity_z;
	object->effect.velocity_x = velocity_x - velocity_x / 22;
	object->effect.velocity_z = velocity_z - velocity_z / 20;
	object->effect.scale_x += object->effect.scale_rate_x;
	object->effect.scale_y += object->effect.scale_rate_y;
	object->effect.sprite->rotation += object->effect.spin;
}

void VinasuChojo_StepSpriteRotation(struct FieldActor *actor)
{
	struct FieldSprite *sprite = actor->sprite;
	u16					rotation = sprite->rotation;

	if (((rotation + 0xffff) << 16) < 0)
	{
		{
			s32 t = rotation - 0xa00;
			sprite->rotation = t;
		}
	}
}

s32 OverlayObject_UpdateHeadingTimer(struct FieldActor *actor)
{
	u16 *q = &actor->work;
	s32	 c = *q;
	s16	 v = *(s16 *)q;

	if (v == 0)
	{
		{
			s32 t = ((u32)(Engine_RandomNext() << 15)) >> 16;
			actor->facing += t;
		}
		{
			s32 n = ((u32)(Engine_RandomNext() * 80)) >> 16;
			*q = n;
			if (n == 0)
			{
				goto out;
			}
			c = n;
		}
	}
	*q = c - 1;
out:
	return 1;
}

const struct SceneEntrance *Scene_GetEntrances(void)
{
	return gVinasuChojoEntrances;
}

const struct SceneRegion *Scene_GetRegions(void)
{
	return 0;
}

const u32 *Scene_GetExits(void)
{
	return gVinasuChojoExits;
}

const struct ScenePlacement *Scene_GetPlacements(void)
{
	return gVinasuChojoPlacements;
}

s32 FixedPoint_Distance(s32 *first_position, s32 *second_position)
{
	s32 delta_x = (*first_position++ - *second_position++) >> 16;
	s32 delta_y = (*first_position++ - *second_position++) >> 16;
	s32 delta_z = (*first_position - *second_position) >> 16;
	s32 delta_x_squared = delta_x * delta_x;
	s32 delta_y_squared = delta_y * delta_y;
	s32 delta_z_squared = delta_z * delta_z;

	return Iwram_Sqrt(delta_x_squared + delta_y_squared + delta_z_squared);
}

s32 SceneActor_FindNearestSlotOfKindF2(void)
{
	struct EventWork   *work = gEventWork;
	struct FieldActor **p;
	s32					best = 0;
	struct FieldActor  *ref;
	s32					limit;
	u32					i;

	limit = 640;
	ref = Object_GetById(ACTOR_PARTY_LEADER);
	i = ACTOR_FIRST_PLACED;
	p = work->placed_actors;
	do
	{
		struct FieldActor *actor = *p++;
		if (actor != 0)
		{
			if (*((Spr_020004bc *)actor)->obj->unk28 == 0xf2)
			{
				s32 dist = FixedPoint_Distance(&ref->x.fixed, &actor->x.fixed);
				if (dist < limit)
				{
					limit = dist;
					best = i;
				}
			}
		}
		i++;
	} while (i <= 65);
	return best;
}

void InitializeActorZeroMotion(void)
{
	struct FieldActor *actor;
	s32				   position[3];
	s32				   angle;
	u8				   flags;

	actor = Object_GetById(ACTOR_PARTY_LEADER);
	angle = (actor->facing + 0x1000) & 0xe000;
	flags = actor->motion_flags;
	position[0] = (actor->x.fixed & 0xfff00000) + 0x80000;
	position[1] = actor->y.fixed;
	position[2] = (actor->z.fixed & 0xfff00000) + 0x80000;
	Vector_AddPolarOffset(0x200000, angle, position);
	if (Object_CheckMovementCollision(actor, position) == 0)
	{
		Engine_GameFlagClear(592);
		VinasuChojo_ConfigureGroupMotion();
		Object_SetMode(actor, 6);
		Engine_TaskWait(6);
		Object_SetMode(actor, 7);
		actor->speed = 0x30000;
		actor->acceleration = 0x20000;
		Engine_AudioPlayCue(152);
		actor->velocity_y = 0x40000;
		actor->motion_flags &= 0x7e;
		Engine_ActorSetSpriteFlags(actor, 0);
		Engine_ActorMoveToAndWait(ACTOR_PARTY_LEADER, position[0] >> 16, position[2] >> 16);
		Object_SetMode(actor, 6);
		Engine_ActorSetSpriteFlags(actor, 1);
		actor->motion_flags = flags;
	}
}

void SceneEffect_SpawnAndBobWithActorZero(void)
{
	struct FieldActor *leader = Object_GetById(ACTOR_PARTY_LEADER);
	struct FieldActor *pillar;
	s32				   id;
	s32				   k;

	Engine_EventBegin();
	id = SceneActor_FindNearestSlotOfKindF2();
	VinasuChojo_NearestActor = id;
	if (id != 0)
	{
		Engine_GameFlagSet(592);
		pillar = Object_GetById(VinasuChojo_NearestActor);
		pillar->motion_flags = 0;
		leader->motion_flags &= 0xfe;
		pillar->y.fixed += -0x30000;
		leader->y.fixed += -0x30000;
		leader->terrain_height += -0x30000;
		Engine_TaskWait(2);
		pillar->y.fixed += -0x20000;
		leader->y.fixed += -0x20000;
		leader->terrain_height += -0x20000;
		Engine_TaskWait(10);
		k = 0x20000;
		pillar->y.fixed += k;
		leader->y.fixed += k;
		leader->terrain_height += k;
		Engine_TaskWait(4);
		pillar->y.fixed += k;
		leader->y.fixed += k;
		leader->terrain_height += k;
		Engine_TaskWait(4);
		pillar->y.fixed += 0x10000;
		leader->y.fixed += 0x10000;
		leader->terrain_height += 0x10000;
	}
	Engine_EventEnd();
}

void VinasuChojo_ConfigureGroupMotion(void)
{
	s32 flags;

	Object_GetById(0)->motion_flags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
	flags = 4;
	Object_GetById(12)->motion_flags = flags;
	Object_GetById(13)->motion_flags = flags;
	Object_GetById(14)->motion_flags = flags;
	Object_GetById(15)->motion_flags = flags;
	Object_GetById(16)->motion_flags = flags;
	Object_GetById(17)->motion_flags = flags;
}

const struct SceneEvent *Scene_GetEvents(void)
{
	return gVinasuChojoEvents;
}

s32 Scene_Initialize(void)
{
	s32				   v;
	u32				   i;
	s32				   z;
	s16				   mode;
	struct FieldActor *obj;

	Engine_GameFlagSet(324);
	Engine_TaskWait(1);
	Engine_GameFlagSet(272);
	Engine_ActorSetSpriteFlags(Object_GetById(8), 0);
	Engine_ActorSetSpriteFlags(Object_GetById(9), 0);
	Engine_ActorSetSpriteFlags(Object_GetById(10), 0);
	Engine_ActorSetSpriteFlags(Object_GetById(11), 0);
	v = 0xffff0000;
	Object_GetById(10)->scale_x = v;
	Object_GetById(11)->scale_x = v;
	i = 12;
	z = 0;
	do
	{
		obj = Object_GetById(i);
		Engine_ActorSetSpriteFlags(Object_GetById(i), z);
		Engine_ActorSetSpritePriority(i, 1);
		obj->motion_flags = 4;
		obj->priority_flags |= 2;
		obj->y.fixed = 0x8000;
		i++;
	} while (i <= 17);

	mode = gGameState.entrance;

	switch (mode)
	{
	case 1:
		if (Engine_GameFlagIsSet(0x109) == 0)
		{
			Scene_RunScriptedActorPresentation();
		}
		break;
	case 2:
		Scene_RunPairedActorEffectSequence();
		break;
	case 3:
		FieldScene_RunThreeStepsInBracket();
		break;
	case 0x5d:
		FieldScene_RunBracketedSceneWithFlag282();
		break;
	case 4:
		VinasuChojo_RunLeapEntrance();
		break;
	case 9:
		Engine_EventBegin();
		if (Engine_GameFlagIsSet(0x345) != 0)
		{
			Inventory_AddItem(0, 65);
		}
		else if (Engine_GameFlagIsSet(0x346) != 0)
		{
			Inventory_AddItem(1, 65);
		}
		else if (Engine_GameFlagIsSet(0x347) != 0)
		{
			Inventory_AddItem(2, 65);
		}
		else
		{
			Inventory_AddItem(3, 65);
		}
		Engine_EventRequestExit(9);
		break;
	}

	if (Engine_GameFlagIsSet(0x109) != 0)
	{
		s32 slot = SceneActor_FindNearestSlotOfKindF2();

		if (slot != 0)
		{
			struct FieldActor *p = Object_GetById(slot);
			if (p != 0)
			{
				p->motion_flags = 0;
			}
		}
	}
	return z;
}

void VinasuChojo_ShowMessage(s32 speaker)
{
	Engine_EventShowMessage(speaker, 0);
	Engine_EventWait(10);
}

void VinasuChojo_FaceActor(s32 actor, s32 facing)
{
	Engine_ActorFaceDirection(actor, facing, 10);
}

void Scene_RunScriptedActorPresentation(void)
{
	struct FieldActor *companion;
	struct FieldActor *leader;

	companion = Object_GetById(18);
	Engine_EventBegin();
	VinasuChojo_ShowPartyMember(1, 0);
	VinasuChojo_ShowPartyMember(2, 0);
	VinasuChojo_ShowPartyMember(3, 0);
	Engine_CameraMoveTo(-1, -1, -1, 0);
	Engine_TaskWait(1);
	companion->motion_flags = 0;
	companion->priority_flags |= ACTOR_PRIORITY_UNDERFOOT;
	Engine_ActorSetSpriteFlags(Object_GetById(18), 0);
	Engine_ActorSetSpritePriority(18, 1);
	Engine_ActorSetPosition(18, PIXELS(580), PIXELS(338));
	leader = Object_GetById(ACTOR_PARTY_LEADER);
	leader->motion_flags = 0;
	Engine_ActorSetSpritePriority(ACTOR_PARTY_LEADER, 1);
	Engine_ActorSetPosition(ACTOR_PARTY_LEADER, PIXELS(581), PIXELS(288));
	Engine_TaskWait(1);
	Engine_EventOpenScreen();
	Engine_EventWaitForScreen();
	Engine_EventWait(20);
	Actor_SetSpeed(18, 0x10000, 0x8000);
	Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
	Object_GetById(ACTOR_PARTY_LEADER)->steering_flags &= ~ACTOR_FACES_MOTION;
	Actor_SetDestination(18, 580, 221);
	Actor_MoveToAndWait(0, 581, 171);
	Actor_SetDestination(18, 530, 211);
	Actor_MoveToAndWait(0, 531, 161);
	Actor_SetDestination(18, 520, 191);
	Engine_ActorMoveToAndWait(0, 521, 141);
	Engine_ActorSetSpriteFlags(Object_GetById(18), 1);
	Actor_SetDestination(18, 515, 171);
	Engine_ActorMoveToAndWait(0, 516, 121);
	Engine_AudioPlayCue(0x120);
	Engine_EventWait(20);
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 6);
	ObjectMotion_WaitForAnimationChange(0);
	leader->x.fixed = PIXELS(516);
	leader->y.fixed = PIXELS(8);
	leader->z.fixed = PIXELS(148);
	leader->facing = FACING_WEST;
	leader->motion_flags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
	Engine_AudioPlayCue(152);
	leader->velocity_y = 0x40000;
	Engine_AudioPlayCue(152);
	Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_PARTY_LEADER), 1);
	Engine_ActorMoveToAndWait(0, 504, 148);
	Engine_EventWait(10);
	Object_GetById(ACTOR_PARTY_LEADER)->steering_flags |= ACTOR_FACES_MOTION;
	{
		u16 shown = FACING_SOUTH;

		leader->y.fixed = -PIXELS(32);
		leader->facing = shown;
	}
	Engine_EventWait(20);
	Engine_AudioPlayCue(0x134);
	Engine_ActorMoveToAndWait(18, 524, 191);
	Engine_ActorSetSpriteFlags(Object_GetById(18), 0);
	Actor_MoveToAndWait(18, 530, 211);
	Actor_MoveToAndWait(18, 580, 221);
	Engine_ActorSetDestination(18, 580, 338);
	Object_GetById(ACTOR_PARTY_LEADER)->priority_flags |= ACTOR_PRIORITY_AUTOMATIC;
	Actor_SetSpeed(ACTOR_PARTY_LEADER, 0xcccc, 0x6666);
	VinasuChojo_ShowPartyMember(1, 1);
	VinasuChojo_ShowPartyMember(2, 1);
	VinasuChojo_ShowPartyMember(3, 1);
	Actor_WalkTo(ACTOR_PARTY_LEADER, 492, 164);
	Engine_ActorWalkTo(1, 514, 164);
	Actor_WalkTo(IWAN, 492, 140);
	Engine_ActorWalkToAndWait(3, 514, 140);
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 1);
	Engine_ActorSetAnimation(GERALD, 1);
	Engine_ActorSetAnimation(IWAN, 1);
	Engine_ActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
	Engine_ActorFaceDirection(GERALD, FACING_SOUTH, 0);
	Engine_ActorFaceDirection(IWAN, FACING_SOUTH, 0);
	Engine_ActorFaceDirection(MEARI, FACING_SOUTH, 0);
	Engine_ActorWaitForMove(18);
	Engine_ActorSetPosition(18, 0, 0);
	Engine_AudioPlayCue(0x121);
	Engine_EventSetMessage((s32)MsgVinasuLongLastTime);
	VinasuChojo_ShowMessage(0x4013);
	Engine_ActorFaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
	Engine_ActorFaceDirection(GERALD, FACING_WEST, 0);
	Engine_ActorFaceDirection(IWAN, FACING_WEST, 0);
	VinasuChojo_FaceActor(MEARI, FACING_WEST);
	Engine_EventGetViewCenter()->motion_flags = 0;
	Engine_CameraSetSpeed(0x4cccc, 0x9999);
	Engine_CameraMoveTo(0x1300000, 0x200000, 0x9e0000, 1);
	Engine_CameraWaitForMove();
	Engine_EventWait(20);
	VinasuChojo_FaceActor(ACTOR_FIRST_OF_PAIR, FACING_NORTH + FACING_STEP);
	Engine_ActorRunRepeatedMotion(ACTOR_FIRST_OF_PAIR, 1);
	Engine_AudioPlayCue(61);
	VinasuChojo_ShowMessage(0x2014);
	Engine_ActorSetAnimation(ACTOR_SECOND_OF_PAIR, 4);
	VinasuChojo_ShowMessage(0x4013);
	VinasuChojo_FaceActor(ACTOR_FIRST_OF_PAIR, FACING_NORTHWEST + FACING_STEP);
	Actor_ShowEmote(ACTOR_FIRST_OF_PAIR, 0x105, 40);
	VinasuChojo_ShowMessage(0x2014);
	VinasuChojo_ShowMessage(21);
	Actor_ShowEmote(ACTOR_SECOND_OF_PAIR, 0x100, 0);
	Engine_ActorShowEmote(20, 0x100, 20);
	Actor_FaceDirection(6, FACING_SOUTHEAST + FACING_STEP, 0);
	Actor_FaceDirection(ACTOR_SECOND_OF_PAIR, FACING_SOUTH + FACING_STEP, 0);
	Actor_FaceDirection(ACTOR_FIRST_OF_PAIR, FACING_SOUTH + FACING_STEP, 20);
	Engine_CameraSetSpeed(0x19999, 0x3333);
	Camera_MoveTo(0x1260000, -1, 0xc20000, 1);
	Engine_EventWait(20);
	Actor_SetSpeed(21, 0xcccc, 0x6666);
	Actor_WalkToAndWait(21, 272, 200);
	Engine_ActorRunRepeatedMotion(ACTOR_FIRST_OF_PAIR, 1);
	Engine_EventWait(20);
	VinasuChojo_ShowMessage(0x2014);
	Actor_ShowEmote(ACTOR_SECOND_OF_PAIR, 0x103, 20);
	VinasuChojo_ShowMessage(ACTOR_SECOND_OF_PAIR);
	Engine_ActorSetAnimationAndWait(21, 3);
	VinasuChojo_ShowMessage(21);
	Actor_ShowEmote(ACTOR_FIRST_OF_PAIR, 0x101, 40);
	VinasuChojo_ShowMessage(0x2014);
	Engine_ActorSetAnimationAndWait(21, 4);
	VinasuChojo_ShowMessage(21);
	Actor_ShowEmote(ACTOR_SECOND_OF_PAIR, 0x101, 60);
	Engine_EventShowMessageAndWait(19, 0, 40);
	Actor_ShowEmote(ACTOR_SECOND_OF_PAIR, 0x106, 40);
	VinasuChojo_FaceActor(ACTOR_SECOND_OF_PAIR, FACING_WEST);
	VinasuChojo_ShowMessage(0x2013);
	Engine_ActorFaceDirection(6, FACING_EAST, 0);
	Actor_ShowEmote(21, 0x103, 40);
	VinasuChojo_ShowMessage(21);
	Actor_FaceDirection(ACTOR_SECOND_OF_PAIR, FACING_SOUTHEAST + FACING_STEP, 0);
	Engine_ActorFaceDirection(ACTOR_FIRST_OF_PAIR, FACING_NORTHWEST + FACING_STEP, 40);
	Engine_ActorRunRepeatedMotion(21, 1);
	VinasuChojo_ShowMessage(21);
	Actor_FaceDirection(ACTOR_FIRST_OF_PAIR, FACING_SOUTH + FACING_STEP, 0);
	VinasuChojo_FaceActor(ACTOR_SECOND_OF_PAIR, FACING_SOUTH + FACING_STEP);
	Actor_ShowEmote(ACTOR_SECOND_OF_PAIR, 0x108, 20);
	VinasuChojo_ShowMessage(0x2013);
	Actor_ShowEmote(21, 0x103, 20);
	Engine_EventShowMessageAndWait(21, 0, 20);
	Engine_ActorFaceDirection(ACTOR_FIRST_OF_PAIR, FACING_WEST, 40);
	Engine_ActorSetAnimationAndWait(ACTOR_FIRST_OF_PAIR, 4);
	Engine_EventShowMessageAndWait(0x2014, 0, 40);
	Engine_ActorSetAnimationAndWait(21, 3);
	Engine_EventShowMessageAndWait(21, 0, 20);
	Engine_ActorFaceDirection(ACTOR_FIRST_OF_PAIR, FACING_SOUTH + FACING_STEP, 20);
	Engine_ActorStartRepeatedMotion(21, 2);
	VinasuChojo_ShowMessage(21);
	Actor_ShowEmote(ACTOR_FIRST_OF_PAIR, 0x105, 0);
	Actor_ShowEmote(ACTOR_SECOND_OF_PAIR, 0x105, 80);
	Engine_ActorStartRepeatedMotion(21, 2);
	VinasuChojo_ShowMessage(21);
	Actor_ShowEmote(ACTOR_SECOND_OF_PAIR, 0x101, 60);
	VinasuChojo_ShowMessage(0x2013);
	Engine_ActorSetAnimationAndWait(21, 3);
	VinasuChojo_ShowMessage(21);
	Engine_ActorStartRepeatedMotion(ACTOR_SECOND_OF_PAIR, 1);
	Engine_ActorRunRepeatedMotion(ACTOR_FIRST_OF_PAIR, 1);
	Engine_EventWait(20);
	Engine_ActorSetAnimationAndWait(21, 4);
	Engine_EventShowMessageAndWait(21, 0, 20);
	Actor_ShowEmote(ACTOR_FIRST_OF_PAIR, 0x105, 60);
	Engine_EventShowMessageAndWait(0x2014, 0, 20);
	VinasuChojo_FaceActor(21, FACING_NORTHWEST + FACING_STEP);
	VinasuChojo_ShowMessage(21);
	VinasuChojo_FaceActor(6, FACING_SOUTHEAST + FACING_STEP);
	Engine_ActorRunRepeatedMotion(6, 2);
	Engine_EventWait(20);
	VinasuChojo_FaceActor(21, FACING_NORTH + FACING_STEP);
	Engine_ActorSetAnimationAndWait(ACTOR_SECOND_OF_PAIR, 4);
	VinasuChojo_ShowMessage(0x2013);
	Engine_ActorFaceDirection(6, FACING_EAST, 0);
	Engine_ActorRunRepeatedMotion(21, 1);
	VinasuChojo_ShowMessage(21);
	Engine_ActorSetAnimationAndWait(ACTOR_FIRST_OF_PAIR, 3);
	VinasuChojo_ShowMessage(0x2014);
	Engine_ActorSetAnimationAndWait(21, 4);
	VinasuChojo_ShowMessage(21);
	Actor_ShowEmote(ACTOR_FIRST_OF_PAIR, 0x101, 0);
	Actor_ShowEmote(ACTOR_SECOND_OF_PAIR, 0x101, 80);
	Engine_ActorFaceDirection(ACTOR_SECOND_OF_PAIR, FACING_WEST, 0);
	Engine_ActorFaceDirection(ACTOR_FIRST_OF_PAIR, FACING_WEST, 0);
	Engine_CameraSetSpeed(0x6666, 0xccc);
	Camera_MoveTo(0x1260000, -1, 0xb40000, 1);
	Actor_WalkToAndWait(21, 262, 176);
	Engine_ActorFaceDirection(21, FACING_WEST, 40);
	Engine_ActorFaceDirection(21, FACING_EAST, 20);
	Engine_ActorStartRepeatedMotion(21, 2);
	VinasuChojo_ShowMessage(0x8015);
	Engine_ActorShowEmote(19, 0x100, 20);
	VinasuChojo_ShowMessage(0x2013);
	Engine_ActorStartRepeatedMotion(21, 2);
	VinasuChojo_ShowMessage(0x8015);
	Actor_ShowEmote(ACTOR_FIRST_OF_PAIR, 0x103, 40);
	Event_ShowMessageAndWait(0xa014, 0, 20);
	Actor_ShowEmote(21, 0x105, 20);
	VinasuChojo_ShowMessage(0x8015);
	Actor_ShowEmote(ACTOR_SECOND_OF_PAIR, 0x103, 20);
	VinasuChojo_ShowMessage(0x2013);
	Actor_ShowEmote(21, 0x101, 40);
	VinasuChojo_ShowMessage(0x8015);
	Engine_ActorSetAnimationAndWait(ACTOR_FIRST_OF_PAIR, 4);
	VinasuChojo_ShowMessage(0xa014);
	Engine_ActorSetAnimationAndWait(ACTOR_SECOND_OF_PAIR, 3);
	VinasuChojo_ShowMessage(0x2013);
	Actor_ShowEmote(21, 0x103, 60);
	Engine_ActorFaceDirection(21, FACING_WEST, 20);
	Engine_EventShowMessageAndWait(0xa015, 0, 40);
	Actor_ShowEmote(6, 0x105, 120);
	Actor_ShowEmote(ACTOR_FIRST_OF_PAIR, 0x105, 60);
	VinasuChojo_ShowMessage(0xa014);
	Engine_ActorFaceDirection(21, FACING_EAST, 40);
	Engine_ActorSetAnimation(ACTOR_SECOND_OF_PAIR, 3);
	VinasuChojo_ShowMessage(0x2013);
	Engine_ActorRunRepeatedMotion(21, 1);
	Engine_EventShowMessageAndWait(0x8015, 0, 20);
	VinasuChojo_ShowMessage(0xa014);
	Engine_ActorShowEmote(21, 0x100, 40);
	Engine_ActorSetAnimation(ACTOR_SECOND_OF_PAIR, 4);
	VinasuChojo_ShowMessage(0x2013);
	Actor_ShowEmote(6, 0x105, 40);
	Engine_ActorShowEmote(20, 0x108, 40);
	VinasuChojo_ShowMessage(0xa014);
	Engine_ActorShowEmote(19, 0x103, 20);
	VinasuChojo_ShowMessage(0x2013);
	Engine_ActorRunRepeatedMotion(21, 1);
	Engine_EventWait(20);
	Engine_ActorStartRepeatedMotion(ACTOR_FIRST_OF_PAIR, 2);
	VinasuChojo_ShowMessage(0xa014);
	Engine_ActorSetAnimation(ACTOR_SECOND_OF_PAIR, 4);
	VinasuChojo_ShowMessage(0x2013);
	gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 2);
	Engine_EventCloseScreen();
	Engine_EventWaitForScreen();
	Engine_CameraMoveTo(0x1f80000, -0x180000, 0xa80000, 0);
	Engine_TaskWait(1);
	Engine_MapRedraw();
	Engine_TaskWait(1);
	Engine_EventOpenScreen();
	Engine_EventWaitForScreen();
	Engine_EventWait(20);
	Engine_ActorRunRepeatedMotion(GERALD, 1);
	VinasuChojo_ShowMessage(0x8001);
	Engine_ActorShowEmote(3, 0x101, 40);
	VinasuChojo_ShowMessage(MEARI);
	Engine_ActorSetAnimationAndWait(IWAN, 3);
	Engine_EventShowMessageAndWait(0x1002, 0, 40);
	Engine_ActorJump(GERALD, 2, 20);
	VinasuChojo_ShowMessage(0x8001);
	Engine_EventOpenMessage(0x8001, 0);
	Engine_ActorFaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
	Engine_ActorFaceDirection(IWAN, FACING_SOUTH, 0);
	Actor_FaceDirection(MEARI, FACING_SOUTHEAST, 0);
	if (Engine_EventChooseYesNo(0, 0) == 1)
	{
		gEventWork->message += 1;
	}
	Engine_EventWait(20);
	VinasuChojo_ShowMessage(GERALD);
	Engine_ActorFaceDirection(IWAN, FACING_EAST, 0);
	Engine_ActorFaceDirection(MEARI, FACING_WEST, 20);
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
	Engine_ActorSetAnimation(GERALD, 3);
	Engine_ActorSetAnimation(IWAN, 3);
	Engine_ActorSetAnimationAndWait(MEARI, 3);
	Engine_ActorEnableActionCallback(GERALD, VinasuChojo_PairActionScript);
	Engine_ActorEnableActionCallback(IWAN, VinasuChojo_PairActionScript);
	Object_SetActionCallbackAndRefreshById(MEARI, VinasuChojo_PairActionScript);
	Engine_EventWait(20);
	Engine_EventEnd();
}

void VinasuChojo_ShowPartyMember(s32 actor, s32 shown)
{
	s32 record;

	if (shown != 0)
	{
		Actor_SetChildValue(actor, 0);
		record = Actor_Get(actor);
		Engine_ActorSetSpriteFlags(record, 1);
		Actor_SetSpeed(actor, 0xcccc, 0x6666);
	}
	else
	{
		Actor_SetChildValue(actor, 15);
		record = Actor_Get(actor);
		Engine_ActorSetSpriteFlags(record, 0);
	}
}

void Scene_RunActorEntrySequence(void)
{
	s32				   owner;
	struct FieldActor *object;
	s32				   pending;
	s32				   frame;
	s32				   message;

	Engine_EventBegin();
	Engine_EventGetViewCenter()->motion_flags = 0;
	Camera_SetSpeed(0xcccc, 0x1999);
	Camera_MoveTo(PIXELS(332), PIXELS(32), PIXELS(180), 1);
	Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
	Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 340, 184);
	VinasuChojo_FaceActor(ACTOR_PARTY_LEADER, FACING_WEST);
	Engine_ActorRunRepeatedMotion(21, 1);
	Engine_EventSetMessage((s32)MsgVinasuRobin);
	VinasuChojo_ShowMessage(0x9015);
	Engine_EventGetViewCenter()->motion_flags = 0;
	Camera_SetSpeed(0xcccc, 0x1999);
	Camera_MoveTo(PIXELS(304), PIXELS(32), PIXELS(180), 1);
	Actor_SetSpeed(GERALD, 0x10000, 0x8000);
	Actor_SetSpeed(IWAN, 0x10000, 0x8000);
	Actor_SetSpeed(MEARI, 0x16666, 0xb333);
	object = Object_GetById(ACTOR_PARTY_LEADER);
	if (object != 0)
	{
		Actor_SetPosition(GERALD, object->x.fixed, object->z.fixed);
	}
	object = Object_GetById(ACTOR_PARTY_LEADER);
	if (object != 0)
	{
		Actor_SetPosition(IWAN, object->x.fixed, object->z.fixed);
	}
	object = Object_GetById(ACTOR_PARTY_LEADER);
	if (object != 0)
	{
		Actor_SetPosition(MEARI, object->x.fixed, object->z.fixed);
	}
	Actor_WalkTo(GERALD, 328, 168);
	Actor_WalkTo(IWAN, 340, 196);
	Actor_WalkToAndWait(MEARI, 326, 204);
	Engine_ActorSetAnimation(GERALD, 1);
	Engine_ActorSetAnimation(IWAN, 1);
	Engine_ActorFaceDirection(GERALD, FACING_WEST, 0);
	Engine_ActorFaceDirection(IWAN, FACING_WEST, 0);
	VinasuChojo_FaceActor(MEARI, FACING_WEST);
	Engine_ActorFaceDirection(ACTOR_FIRST_OF_PAIR, FACING_EAST, 0);
	Engine_ActorFaceDirection(ACTOR_SECOND_OF_PAIR, FACING_EAST, 40);
	Engine_ActorRunRepeatedMotion(ACTOR_FIRST_OF_PAIR, 2);
	VinasuChojo_ShowMessage(ACTOR_FIRST_OF_PAIR);
	VinasuChojo_FaceActor(ACTOR_SECOND_OF_PAIR, FACING_WEST);
	VinasuChojo_ShowMessage(0x2013);
	Actor_ShowEmote(21, 0x103, 20);
	VinasuChojo_ShowMessage(21);
	Engine_ActorStartRepeatedMotion(21, 2);
	Event_ShowMessageAndWait(21, 0, 20);
	Engine_ActorFaceDirection(21, FACING_NORTH + FACING_STEP, 40);
	VinasuChojo_ShowMessage(21);
	Engine_ActorSetAnimationAndWait(21, 4);
	VinasuChojo_FaceActor(21, FACING_EAST);
	Event_ShowMessageAndWait(21, 0, 20);
	Engine_ActorRunRepeatedMotion(ACTOR_FIRST_OF_PAIR, 1);
	VinasuChojo_FaceActor(ACTOR_FIRST_OF_PAIR, FACING_WEST);
	VinasuChojo_ShowMessage(0x2014);
	Actor_SetAttachedEffect(MEARI, 0x102);
	Engine_ActorStartRepeatedMotion(MEARI, 2);
	VinasuChojo_ShowMessage(MEARI);
	Engine_ActorRunRepeatedMotion(IWAN, 1);
	VinasuChojo_ShowMessage(IWAN);
	VinasuChojo_FaceActor(ACTOR_SECOND_OF_PAIR, FACING_EAST);
	VinasuChojo_ShowMessage(ACTOR_SECOND_OF_PAIR);
	Actor_ShowEmote(ACTOR_FIRST_OF_PAIR, 0x106, 40);
	Engine_ActorFaceDirection(ACTOR_FIRST_OF_PAIR, FACING_EAST, 20);
	VinasuChojo_ShowMessage(ACTOR_FIRST_OF_PAIR);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
	Actor_FaceDirection(GERALD, FACING_SOUTHEAST, 0);
	Actor_FaceDirection(IWAN, FACING_SOUTHWEST, 0);
	Actor_FaceDirection(MEARI, FACING_NORTHEAST, 0);
	Engine_ActorFaceDirection(21, FACING_WEST, 0);
	VinasuChojo_FaceActor(ACTOR_SECOND_OF_PAIR, FACING_SOUTHEAST + FACING_STEP);
	Actor_ShowEmote(ACTOR_SECOND_OF_PAIR, 0x102, 40);
	VinasuChojo_ShowMessage(ACTOR_SECOND_OF_PAIR);
	VinasuChojo_FaceActor(ACTOR_FIRST_OF_PAIR, FACING_NORTHWEST + FACING_STEP);
	VinasuChojo_ShowMessage(0x2014);
	Engine_ActorFaceDirection(ACTOR_FIRST_OF_PAIR, FACING_WEST, 20);
	VinasuChojo_ShowMessage(0x2014);
	Engine_ActorFaceDirection(ACTOR_SECOND_OF_PAIR, FACING_WEST, 0);
	Engine_ActorFaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
	Engine_ActorFaceDirection(GERALD, FACING_WEST, 0);
	Engine_ActorFaceDirection(IWAN, FACING_WEST, 0);
	Engine_ActorFaceDirection(MEARI, FACING_WEST, 0);
	Engine_ActorFaceDirection(21, FACING_EAST, 20);
	Actor_ShowEmote(6, 0x101, 40);
	VinasuChojo_ShowMessage(6);
	Actor_ShowEmote(ACTOR_FIRST_OF_PAIR, 0x103, 20);
	VinasuChojo_ShowMessage(0x2014);
	Engine_ActorRunRepeatedMotion(6, 2);
	Engine_EventWait(20);
	Engine_ActorSetAnimation(6, 3);
	VinasuChojo_ShowMessage(6);
	Engine_ActorSetAnimationAndWait(ACTOR_FIRST_OF_PAIR, 3);
	VinasuChojo_ShowMessage(0x2014);
	Actor_SetSpeed(6, 0xcccc, 0x6666);
	Actor_WalkToAndWait(6, 260, 186);
	Engine_ActorFaceDirection(21, FACING_SOUTHEAST + FACING_STEP, 0);
	Actor_WalkToAndWait(6, 276, 192);
	object = Object_GetById(ACTOR_SECOND_OF_PAIR);
	object->facing = FACING_SOUTH + FACING_STEP;
	Engine_TaskWait(1);
	Engine_ActorStartRepeatedMotion(ACTOR_SECOND_OF_PAIR, 2);
	VinasuChojo_ShowMessage(0x2013);
	Engine_ActorJump(6, 2, 20);
	Actor_SetSpeed(6, 0x26666, 0x13333);
	Actor_WalkToAndWait(6, 260, 186);
	Engine_ActorFaceDirection(21, FACING_WEST, 0);
	Actor_WalkToAndWait(6, 248, 172);
	Engine_ActorFaceDirection(ACTOR_SECOND_OF_PAIR, FACING_WEST, 0);
	Engine_ActorFaceDirection(21, FACING_WEST, 0);
	Engine_ActorFaceDirection(6, FACING_EAST, 20);
	Engine_ActorSetAnimationAndWait(6, 3);
	Engine_EventWait(40);
	SceneState_ApplyPair140And0();
	Engine_TaskWait(1);
	frame = 0;
	do
	{
		object = Object_GetById(6);
		SceneEffect_UpdateObjectByFrameParity((s32)object);
		frame = (frame + 1);
		Engine_TaskWait(1);
	} while ((u32)frame <= 39);
	Engine_TaskAddCallback(FieldScene_RunStep6, TASK_PRIORITY_SCENE);
	Engine_EventWait(80);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
	Actor_FaceDirection(GERALD, FACING_SOUTHEAST, 0);
	Actor_FaceDirection(IWAN, FACING_SOUTHWEST, 0);
	Actor_FaceDirection(MEARI, FACING_NORTHEAST, 20);
	Engine_ActorFaceDirection(21, FACING_EAST, 40);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
	Actor_FaceDirection(GERALD, FACING_WEST, 0);
	Actor_FaceDirection(IWAN, FACING_WEST, 0);
	Actor_FaceDirection(MEARI, FACING_WEST, 0);
	VinasuChojo_FaceActor(21, FACING_WEST);
	Actor_ShowEmote(ACTOR_FIRST_OF_PAIR, 0x101, 40);
	Event_ShowMessageAndWait(0x2014, 0, 20);
	Actor_ShowEmote(6, 0x105, 80);
	Engine_ActorStartRepeatedMotion(ACTOR_SECOND_OF_PAIR, 2);
	VinasuChojo_ShowMessage(0x2013);
	Scheduler_RemoveCallback((u32)FieldScene_RunStep6);
	Engine_TaskWait(1);
	Actor_SetChildValue(6, 0);
	Engine_TaskWait(10);
	VinasuChojo_CleanupSceneObjects();
	Engine_ActorJump(6, 2, 40);
	VinasuChojo_ShowMessage(6);
	Actor_ShowEmote(ACTOR_FIRST_OF_PAIR, 0x103, 20);
	VinasuChojo_ShowMessage(0x2014);
	Engine_ActorRunRepeatedMotion(6, 2);
	Object_GetById(6)->steering_flags &= ~ACTOR_FACES_MOTION;
	Actor_WalkToAndWait(6, 250, 176);
	Engine_EventWait(1);
	Object_GetById(6)->steering_flags |= ACTOR_FACES_MOTION;
	Actor_ShowEmote(21, 0x103, 20);
	Engine_ActorFaceDirection(21, FACING_EAST, 20);
	VinasuChojo_ShowMessage(21);
	Engine_ActorSetAnimationAndWait(ACTOR_SECOND_OF_PAIR, 4);
	VinasuChojo_ShowMessage(0x2013);
	Engine_ActorRunRepeatedMotion(6, 2);
	Engine_EventWait(40);
	Actor_SetSpeed(6, 0x9999, 0x4ccc);
	Object_GetById(6)->steering_flags &= ~ACTOR_FACES_MOTION;
	Actor_WalkToAndWait(6, 248, 172);
	Engine_EventWait(1);
	Object_GetById(6)->steering_flags |= ACTOR_FACES_MOTION;
	Engine_EventWait(20);
	Engine_ActorSetAnimationAndWait(6, 3);
	Engine_EventWait(20);
	Engine_ActorSetAnimationAndWait(ACTOR_SECOND_OF_PAIR, 3);
	VinasuChojo_ShowMessage(0x2013);
	Engine_ActorSetAnimationAndWait(6, 3);
	VinasuChojo_ShowMessage(6);
	Actor_FaceDirection(ACTOR_SECOND_OF_PAIR, FACING_SOUTHEAST + FACING_STEP, 0);
	Actor_FaceDirection(ACTOR_FIRST_OF_PAIR, FACING_NORTHWEST + FACING_STEP, 20);
	Actor_ShowEmote(ACTOR_SECOND_OF_PAIR, 0x105, 0);
	Actor_ShowEmote(ACTOR_FIRST_OF_PAIR, 0x105, 60);
	Engine_ActorSetAnimation(ACTOR_FIRST_OF_PAIR, 4);
	VinasuChojo_ShowMessage(0x2014);
	Actor_ShowEmote(ACTOR_SECOND_OF_PAIR, 0x101, 40);
	VinasuChojo_ShowMessage(ACTOR_SECOND_OF_PAIR);
	Actor_ShowEmote(ACTOR_FIRST_OF_PAIR, 0x105, 100);
	Engine_ActorRunRepeatedMotion(ACTOR_FIRST_OF_PAIR, 1);
	Engine_EventWait(20);
	VinasuChojo_FaceActor(ACTOR_FIRST_OF_PAIR, FACING_EAST);
	VinasuChojo_FaceActor(ACTOR_SECOND_OF_PAIR, FACING_EAST);
	Event_OpenMessage(ACTOR_FIRST_OF_PAIR, 0);
	Actor_FaceDirection(GERALD, FACING_SOUTHEAST, 0);
	Actor_FaceDirection(IWAN, FACING_NORTH, 0);
	Actor_FaceDirection(MEARI, FACING_NORTHEAST, 0);
	if (Engine_EventChooseYesNo(0, 0) == 0)
	{
		Engine_EventWait(20);
		Engine_ActorSetAnimationAndWait(ACTOR_FIRST_OF_PAIR, 3);
		pending = 1;
	}
	else
	{
		Engine_EventWait(20);
		Engine_ActorSetAnimationAndWait(ACTOR_FIRST_OF_PAIR, 4);
		gEventWork->message += 1;
		pending = 0;
	}
	VinasuChojo_ShowMessage(ACTOR_FIRST_OF_PAIR);
	if (pending != 0)
	{
		gEventWork->message += 1;
	}
	Actor_FaceDirection(GERALD, FACING_WEST, 0);
	Actor_FaceDirection(IWAN, FACING_WEST, 0);
	Actor_FaceDirection(MEARI, FACING_WEST, 20);
	Actor_ShowEmote(ACTOR_FIRST_OF_PAIR, 0x108, 40);
	Event_OpenMessage(ACTOR_FIRST_OF_PAIR, 0);
	Actor_FaceDirection(GERALD, FACING_SOUTHEAST, 0);
	Actor_FaceDirection(IWAN, FACING_NORTH, 0);
	Actor_FaceDirection(MEARI, FACING_NORTHEAST, 0);
	if (Engine_EventChooseYesNo(0, 0) == 0)
	{
		Engine_EventWait(20);
		Engine_ActorSetAnimation(IWAN, 3);
		pending = 1;
	}
	else
	{
		Engine_EventWait(20);
		Engine_ActorSetAnimation(IWAN, 4);
		gEventWork->message += 1;
		pending = 0;
	}
	VinasuChojo_ShowMessage(IWAN);
	if (pending != 0)
	{
		gEventWork->message += 1;
	}
	Engine_ActorRunRepeatedMotion(GERALD, 2);
	VinasuChojo_FaceActor(GERALD, FACING_SOUTH);
	VinasuChojo_ShowMessage(GERALD);
	Engine_ActorRunRepeatedMotion(ACTOR_SECOND_OF_PAIR, 1);
	VinasuChojo_ShowMessage(ACTOR_SECOND_OF_PAIR);
	Actor_FaceDirection(GERALD, FACING_WEST, 0);
	Actor_FaceDirection(IWAN, FACING_WEST, 0);
	VinasuChojo_FaceActor(MEARI, FACING_WEST);
	Engine_ActorSetAnimationAndWait(GERALD, 4);
	Engine_ActorSetAnimationAndWait(ACTOR_FIRST_OF_PAIR, 3);
	Event_ShowMessageAndWait(ACTOR_FIRST_OF_PAIR, 0, 20);
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
	Engine_ActorSetAnimation(GERALD, 3);
	Engine_ActorSetAnimation(IWAN, 3);
	Engine_ActorSetAnimationAndWait(MEARI, 3);
	VinasuChojo_FaceActor(ACTOR_SECOND_OF_PAIR, FACING_SOUTHEAST + FACING_STEP);
	Engine_ActorSetAnimation(ACTOR_SECOND_OF_PAIR, 3);
	VinasuChojo_ShowMessage(ACTOR_SECOND_OF_PAIR);
	Actor_FaceDirection(ACTOR_FIRST_OF_PAIR, FACING_NORTHWEST + FACING_STEP, 20);
	Engine_ActorSetAnimationAndWait(ACTOR_FIRST_OF_PAIR, 3);
	Engine_EventWait(40);
	VinasuChojo_FaceActor(ACTOR_FIRST_OF_PAIR, FACING_WEST);
	VinasuChojo_ShowMessage(0x2014);
	Engine_ActorRunRepeatedMotion(21, 2);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(21, 0, 40);
	VinasuChojo_ShowMessage(0x2014);
	Actor_ShowEmote(21, 0x103, 60);
	VinasuChojo_FaceActor(ACTOR_SECOND_OF_PAIR, FACING_WEST);
	Engine_ActorRunRepeatedMotion(ACTOR_SECOND_OF_PAIR, 1);
	VinasuChojo_ShowMessage(0x2013);
	Actor_ShowEmote(21, 0x105, 60);
	Engine_ActorSetAnimationAndWait(21, 3);
	Engine_EventWait(20);
	Actor_SetSpeed(21, 0xcccc, 0x6666);
	Actor_WalkToAndWait(21, 288, 192);
	Engine_ActorFaceDirection(ACTOR_SECOND_OF_PAIR, FACING_EAST, 0);
	Engine_ActorFaceDirection(ACTOR_FIRST_OF_PAIR, FACING_EAST, 0);
	Actor_WalkToAndWait(21, 310, 192);
	Actor_WalkToAndWait(21, 328, 186);
	Engine_EventWait(20);
	Engine_ActorRunRepeatedMotion(21, 2);

	message = (s32)MsgVinasuRobinHandedOverShamansRod;
	Engine_MessageShowCentered(message, 1);
	Actor_WalkToAndWait(21, 310, 192);
	Engine_ActorFaceDirection(ACTOR_SECOND_OF_PAIR, FACING_WEST, 0);
	Engine_ActorFaceDirection(ACTOR_FIRST_OF_PAIR, FACING_WEST, 0);
	Actor_WalkToAndWait(21, 288, 192);
	Actor_WalkToAndWait(21, 262, 176);
	Engine_ActorFaceDirection(21, FACING_EAST, 40);
	Engine_EventSetMessage((message + 1));
	VinasuChojo_ShowMessage(21);
	Engine_ActorSetAnimationAndWait(ACTOR_FIRST_OF_PAIR, 3);
	VinasuChojo_ShowMessage(0x2014);
	Engine_ActorSetAnimationAndWait(21, 3);
	VinasuChojo_FaceActor(ACTOR_FIRST_OF_PAIR, FACING_EAST);
	VinasuChojo_FaceActor(21, FACING_WEST);
	Engine_ActorSetAnimationAndWait(21, 3);
	Engine_ActorSetAnimationAndWait(6, 3);
	Actor_SetSpeed(6, 0xcccc, 0x6666);
	Actor_WalkToAndWait(6, 260, 186);
	Engine_ActorFaceDirection(21, FACING_SOUTHEAST + FACING_STEP, 0);
	Actor_WalkToAndWait(6, 276, 192);
	Event_PlayCue(19);
	object = Object_GetById(ACTOR_SECOND_OF_PAIR);
	object->facing = FACING_SOUTH + FACING_STEP;
	Engine_TaskWait(1);
	Engine_ActorRunRepeatedMotion(ACTOR_SECOND_OF_PAIR, 1);
	VinasuChojo_ShowMessage(ACTOR_SECOND_OF_PAIR);
	Engine_ActorRunRepeatedMotion(6, 2);
	Engine_ActorFaceDirection(21, FACING_EAST, 0);
	Engine_ActorFaceDirection(ACTOR_FIRST_OF_PAIR, FACING_SOUTH + FACING_STEP, 0);
	Actor_FaceDirection(6, FACING_NORTH + FACING_STEP, 20);
	Engine_ActorJump(MEARI, 2, 20);
	VinasuChojo_FaceActor(MEARI, FACING_NORTHWEST);
	VinasuChojo_ShowMessage(MEARI);
	Engine_ActorFaceDirection(21, FACING_EAST, 0);
	Engine_ActorFaceDirection(6, FACING_EAST, 0);
	Engine_ActorFaceDirection(ACTOR_SECOND_OF_PAIR, FACING_SOUTHEAST + FACING_STEP, 0);
	Actor_FaceDirection(ACTOR_FIRST_OF_PAIR, FACING_NORTHWEST + FACING_STEP, 80);
	Engine_ActorFaceDirection(ACTOR_SECOND_OF_PAIR, FACING_EAST, 0);
	Engine_ActorFaceDirection(ACTOR_FIRST_OF_PAIR, FACING_EAST, 40);
	Event_PlayCue(29);
	VinasuChojo_ShowMessage(ACTOR_FIRST_OF_PAIR);
	Engine_ActorFaceDirection(21, FACING_SOUTHEAST + FACING_STEP, 0);
	Actor_FaceDirection(6, FACING_NORTHWEST + FACING_STEP, 20);
	Engine_ActorStartRepeatedMotion(IWAN, 2);
	VinasuChojo_ShowMessage(IWAN);
	VinasuChojo_FaceActor(ACTOR_SECOND_OF_PAIR, FACING_SOUTHEAST + FACING_STEP);
	Engine_ActorSetAnimationAndWait(ACTOR_SECOND_OF_PAIR, 3);
	VinasuChojo_ShowMessage(ACTOR_SECOND_OF_PAIR);
	Engine_ActorFaceDirection(21, FACING_EAST, 0);
	VinasuChojo_FaceActor(6, FACING_NORTH + FACING_STEP);
	Engine_ActorSetAnimation(MEARI, 3);
	VinasuChojo_ShowMessage(MEARI);
	Actor_ShowEmote(ACTOR_FIRST_OF_PAIR, 0x100, 20);
	Engine_ActorFaceDirection(ACTOR_FIRST_OF_PAIR, FACING_SOUTHEAST + FACING_STEP, 20);
	VinasuChojo_ShowMessage(ACTOR_FIRST_OF_PAIR);
	Actor_ShowEmote(6, 0x102, 0);
	Actor_ShowEmote(21, 0x102, 60);
	Engine_ActorFaceDirection(6, FACING_EAST, 0);
	VinasuChojo_FaceActor(21, FACING_EAST);
	Actor_ShowEmote(GERALD, 0x103, 40);
	Engine_ActorStartRepeatedMotion(GERALD, 2);
	VinasuChojo_ShowMessage(GERALD);
	VinasuChojo_FaceActor(ACTOR_SECOND_OF_PAIR, FACING_EAST);
	Engine_ActorSetAnimationAndWait(ACTOR_SECOND_OF_PAIR, 4);
	VinasuChojo_ShowMessage(ACTOR_SECOND_OF_PAIR);
	VinasuChojo_FaceActor(ACTOR_FIRST_OF_PAIR, FACING_EAST);
	Engine_ActorSetAnimation(ACTOR_FIRST_OF_PAIR, 4);
	VinasuChojo_ShowMessage(ACTOR_FIRST_OF_PAIR);
	Engine_ActorFaceDirection(6, FACING_NORTH + FACING_STEP, 0);
	Actor_ShowEmote(6, 0x101, 0);
	Actor_ShowEmote(MEARI, 0x101, 60);
	Event_ShowMessageAndWait(MEARI, 0, 20);
	VinasuChojo_FaceActor(ACTOR_SECOND_OF_PAIR, FACING_SOUTHEAST + FACING_STEP);
	Engine_ActorSetAnimation(ACTOR_SECOND_OF_PAIR, 4);
	VinasuChojo_ShowMessage(ACTOR_SECOND_OF_PAIR);
	Actor_SetAttachedEffect(IWAN, 0x102);
	Event_ShowMessageAndWait(IWAN, 0, 20);
	VinasuChojo_FaceActor(ACTOR_SECOND_OF_PAIR, FACING_EAST);
	Engine_ActorSetAnimationAndWait(ACTOR_FIRST_OF_PAIR, 3);
	VinasuChojo_ShowMessage(ACTOR_FIRST_OF_PAIR);
	Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
	Engine_ActorStartRepeatedMotion(GERALD, 2);
	Engine_ActorStartRepeatedMotion(IWAN, 2);
	Engine_ActorRunRepeatedMotion(MEARI, 2);
	Engine_ActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
	Actor_FaceDirection(GERALD, FACING_SOUTHEAST, 0);
	Actor_FaceDirection(IWAN, FACING_SOUTHWEST, 0);
	Actor_FaceDirection(MEARI, FACING_NORTHEAST, 40);
	Engine_ActorRunRepeatedMotion(MEARI, 1);
	VinasuChojo_FaceActor(MEARI, FACING_NORTHWEST);
	VinasuChojo_ShowMessage(MEARI);
	Engine_ActorFaceDirection(GERALD, FACING_WEST, 20);
	Actor_ShowEmote(GERALD, 0x103, 20);
	Engine_ActorStartRepeatedMotion(GERALD, 2);
	VinasuChojo_ShowMessage(GERALD);
	Engine_ActorFaceDirection(6, FACING_EAST, 0);
	Engine_ActorFaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
	Engine_ActorFaceDirection(IWAN, FACING_WEST, 40);
	Actor_ShowEmote(ACTOR_FIRST_OF_PAIR, 0x103, 40);
	Engine_ActorStartRepeatedMotion(ACTOR_FIRST_OF_PAIR, 2);
	VinasuChojo_ShowMessage(ACTOR_FIRST_OF_PAIR);
	Engine_ActorRunRepeatedMotion(ACTOR_SECOND_OF_PAIR, 1);
	VinasuChojo_ShowMessage(ACTOR_SECOND_OF_PAIR);
	owner = PartyInventory_FindOwner(65);
	GameFlag_Set(owner + 0x345);
	PartyInventory_Remove(65);
	Object_GetById(ACTOR_PARTY_LEADER)->steering_flags &= ~ACTOR_FACES_MOTION;
	Object_GetById(GERALD)->steering_flags &= ~ACTOR_FACES_MOTION;
	Object_GetById(IWAN)->steering_flags &= ~ACTOR_FACES_MOTION;
	Object_GetById(MEARI)->steering_flags &= ~ACTOR_FACES_MOTION;
	Object_GetById(ACTOR_SECOND_OF_PAIR)->steering_flags &= ~ACTOR_FACES_MOTION;
	Object_GetById(ACTOR_FIRST_OF_PAIR)->steering_flags &= ~ACTOR_FACES_MOTION;
	Object_GetById(21)->steering_flags &= ~ACTOR_FACES_MOTION;
	Object_GetById(6)->steering_flags &= ~ACTOR_FACES_MOTION;
	Engine_ActorEnableActionCallback(ACTOR_PARTY_LEADER, SceneAction_EntryGroup);
	Engine_ActorEnableActionCallback(GERALD, SceneAction_EntryGroup);
	Engine_ActorEnableActionCallback(IWAN, SceneAction_EntryGroup);
	Engine_ActorEnableActionCallback(MEARI, SceneAction_EntryGroup);
	Engine_ActorEnableActionCallback(ACTOR_SECOND_OF_PAIR, SceneAction_EntryPair);
	Engine_ActorEnableActionCallback(ACTOR_FIRST_OF_PAIR, SceneAction_EntryPair);
	Engine_ActorEnableActionCallback(21, SceneAction_EntryGroup);
	Object_SetActionCallbackAndRefreshById(6, SceneAction_EntryGroup);
	Battle_SetStart(3);
	{
		s32 actor = (s32)&SceneId_VinasuChojo;

		Party_SetFields1ceAnd1d0(actor, 2);
		Event_SetDefeatDestination(actor, 9);
	}
	BattleFx_SetWeightedResult(98, 1);
	GameFlag_Set(0x350);
}

void SceneEffect_UpdateObjectByFrameParity(s32 a)
{
	if (*(s32 *)&gFrameCount & 2)
	{
		Engine_ObjectSetPartPalettes(a, 7);
	}
	else
	{
		Engine_ObjectSetPartPalettes(a, 0);
	}
	if (Math_RemainderUnsigned(*(s32 *)&gFrameCount, 15) == 0)
	{
		VinasuChojo_SpawnLinkedPairEffects(a);
	}
}

void SceneState_ForwardByRuntimeWordBits(s32 a)
{
	if (gFrameCount & 1)
	{
		Engine_ObjectSetPartPalettes(a, Math_RemainderUnsigned(gFrameCount >> 1, 6));
	}
	if (Math_RemainderUnsigned(gFrameCount, 15) == 0)
	{
		VinasuChojo_SpawnLinkedPairEffects(a);
	}
}

void Effect_AnimateVerticalPositive(union FieldObject *object)
{
	struct FieldActor *self = &object->actor;
	struct FieldActor *anchor;
	s32				   frame;
	s32				   amplitude;

	anchor = self->linked_object;
	self->timer = (self->timer + 1);
	frame = self->timer;

	if (frame > 31)
	{
		Engine_ObjectDispatchRelease(self);
		return;
	}

	amplitude = Engine_MathSin(frame << 10);
	self->scale_x = amplitude;
	self->scale_y = amplitude;
	self->x.fixed = anchor->x.fixed;
	self->y.fixed += 0x10000;
	self->z.fixed = anchor->z.fixed + (0x10000 - amplitude) * 5 + 0x80000;
}

void Effect_AnimateVerticalNegative(union FieldObject *object)
{
	struct FieldActor *self = &object->actor;
	struct FieldActor *anchor;
	s32				   frame;
	s32				   amplitude;

	anchor = self->linked_object;
	self->timer = (self->timer + 1);
	frame = self->timer;

	if (frame > 31)
	{
		Engine_ObjectDispatchRelease(self);
		return;
	}

	amplitude = Engine_MathSin(frame << 10);
	self->scale_x = amplitude;
	self->scale_y = -amplitude;
	self->x.fixed = anchor->x.fixed;
	self->y.fixed += 0x10000;
	self->z.fixed = anchor->z.fixed - (0x10000 - amplitude) * 5 + 0x100000;
}

void VinasuChojo_SpawnLinkedPairEffects(union PairObject *parent)
{
	union PairObject   *pair[2];
	union PairObject   *child;
	struct PairSprite  *part;
	struct FieldSprite *sprite;
	struct PairWork	   *work = gEffectWork;
	s32					i;

	Engine_AudioPlayCue(131);
	for (i = 0; i < 2; ++i)
	{
		child = (union PairObject *)Engine_ObjectCreate(26, parent->object.actor.x.fixed,
			parent->object.actor.y.fixed, parent->object.actor.z.fixed);
		pair[i] = child;
		if (child != NULL)
		{
			child->words[5] = parent->words[5];
			part = (struct PairSprite *)child->object.actor.sprite;
			child->object.actor.motion_flags = 0;
			child->object.effect.spin = 0;
			child->link.parent = parent;
			if (part != NULL)
			{
				sprite = &part->sprite;
				AnimationObjects_SelectAnimation(sprite, 0);
				sprite->flags = 0;
				Resource_ResetEntry(sprite->vram_block);
				sprite->vram_block = work->vram_block;
				sprite->unknown_1d_0 = 1;
				sprite->tile = (gVramBlockCache[sprite->vram_block].offset >> 5) & 0x3ff;
				sprite->full_color = 0;
				sprite->shape = 1;
				((struct WorldMapOam *)sprite)->size = 2;
				part->detail->field_16 = 0;
			}
		}
	}
	{
		union PairObject   *p = pair[0];
		struct FieldSprite *sp = p->object.actor.sprite;

		p->object.actor.update = (void (*)(union FieldObject *))Effect_AnimateVerticalNegative;
		sp->priority = parent->object.actor.sprite->priority;
	}
	{
		struct FieldActor  *p = &pair[1]->object.actor;
		struct FieldSprite *sp = p->sprite;

		sp->priority = parent->object.actor.sprite->priority;
		p->update = (void (*)(union FieldObject *))Effect_AnimateVerticalPositive;
		p->priority_flags = 2;
	}
}

void SceneState_ApplyPair140And0(void)
{
	Engine_PsynergyBegin(140, 0);
}

void VinasuChojo_CleanupSceneObjects(void)
{
	BattleEffect_CleanupSceneObjects();
}

void FieldScene_RunStep6(void)
{
	SceneState_ForwardByRuntimeWordBits((s32)Object_GetById(6));
}

static inline void VinasuChojo_AimBurst(s32 velocity[3], s32 angle)
{
	s32 sine;
	s32 cosine;

	velocity[0] = Engine_MathCos(angle);
	velocity[1] = 0;
	sine = Engine_MathSin(angle);
	cosine = velocity[0];
	cosine += cosine * 2;
	sine *= 2;
	velocity[2] = sine;
	velocity[0] = cosine;
}

void Scene_RunPairedActorEffectSequence(void)
{
	struct EffectOptions second_burst;
	s32					 velocity[3];
	struct EffectOptions first_burst;
	struct FieldActor	*actor;
	struct FieldActor	*second;
	struct FieldActor	*first;
	union FieldObject	*star = NULL;
	struct FieldActor	*bird;
	struct FieldSprite	*sprite;
	u8					*buffer;
	u32					 i;

	Event_Begin();
	Map_CopyCellAttributes(17, 10, 4, 2, 17, 8);
	Actor_Get(GERALD)->facing = FACING_WEST;
	Actor_SetPosition(GERALD, PIXELS(328), PIXELS(168));
	Actor_Get(IWAN)->facing = FACING_WEST;
	Actor_SetPosition(IWAN, PIXELS(340), PIXELS(196));
	Actor_Get(MEARI)->facing = FACING_WEST;
	Actor_SetPosition(MEARI, PIXELS(326), PIXELS(204));
	Actor_Get(6)->facing = FACING_SOUTHEAST + FACING_STEP;
	Actor_SetPosition(6, PIXELS(268), PIXELS(154));
	Actor_Get(21)->facing = FACING_SOUTHEAST + FACING_STEP;
	Actor_SetPosition(21, PIXELS(268), PIXELS(164));
	actor = Actor_Get(ACTOR_FIRST_OF_PAIR);
	actor->timer = 10;
	actor->facing = FACING_NORTH + FACING_STEP;
	Actor_SetPosition(ACTOR_FIRST_OF_PAIR, PIXELS(294), PIXELS(212));
	Actor_SetAnimation(ACTOR_FIRST_OF_PAIR, 9);
	Actor_EnableActionCallback(ACTOR_FIRST_OF_PAIR, VinasuChojo_ScaleStepAction);
	Actor_SetSpriteFlags(Actor_Get(ACTOR_FIRST_OF_PAIR), 0);
	actor = Actor_Get(ACTOR_SECOND_OF_PAIR);
	actor->timer = 10;
	actor->facing = FACING_EAST;
	Actor_SetPosition(ACTOR_SECOND_OF_PAIR, PIXELS(286), PIXELS(192));
	Actor_SetAnimation(ACTOR_SECOND_OF_PAIR, 7);
	Actor_EnableActionCallback(ACTOR_SECOND_OF_PAIR, VinasuChojo_ScaleStepAction);
	Actor_SetSpriteFlags(Actor_Get(ACTOR_SECOND_OF_PAIR), 0);
	Event_GetViewCenter()->motion_flags = 0;
	Camera_MoveTo(PIXELS(304), PIXELS(32), PIXELS(180), 0);
	Task_Wait(1);
	Map_Redraw();
	Task_Wait(1);
	Event_OpenScreen();
	Event_WaitForScreen();
	Event_Wait(80);

	Actor_Jump(GERALD, 2, 20);
	VinasuChojo_FaceActor(GERALD, FACING_SOUTHEAST);
	Event_SetMessage((s32)MsgVinasuBeatEm);
	VinasuChojo_ShowMessage(0x1001);
	VinasuChojo_FaceActor(MEARI, FACING_NORTHWEST);
	VinasuChojo_ShowMessage(MEARI);
	Actor_FaceDirection(GERALD, FACING_WEST, 0);
	VinasuChojo_FaceActor(IWAN, FACING_NORTHWEST);
	Actor_RunRepeatedMotion(21, 2);
	Actor_SetSpeed(21, 0xcccc, 0x6666);
	Actor_Get(21)->steering_flags &= ~ACTOR_FACES_MOTION;
	Actor_WalkToAndWait(21, 0x118, 164);
	Event_Wait(1);
	Actor_Get(21)->steering_flags |= ACTOR_FACES_MOTION;
	Event_Wait(20);
	Actor_ShowEmote(IWAN, 0x102, 40);
	VinasuChojo_ShowMessage(IWAN);
	Actor_SetAnimationAndWait(21, 4);
	VinasuChojo_ShowMessage(21);
	Actor_ShowEmote(GERALD, 0x103, 20);
	Actor_RunRepeatedMotion(GERALD, 1);
	VinasuChojo_ShowMessage(GERALD);
	Actor_RunRepeatedMotion(21, 1);
	VinasuChojo_FaceActor(21, FACING_EAST);
	VinasuChojo_ShowMessage(21);
	Actor_StartRepeatedMotion(MEARI, 2);
	VinasuChojo_ShowMessage(MEARI);
	Actor_SetAnimation(MEARI, 4);
	VinasuChojo_ShowMessage(MEARI);
	Actor_ShowEmote(21, 0x105, 40);
	VinasuChojo_FaceActor(21, FACING_SOUTHEAST + FACING_STEP);
	VinasuChojo_ShowMessage(21);
	Actor_Jump(IWAN, 2, 20);
	VinasuChojo_ShowMessage(IWAN);
	Actor_SetAnimationAndWait(21, 3);
	VinasuChojo_ShowMessage(21);
	Actor_RunRepeatedMotion(GERALD, 1);
	VinasuChojo_FaceActor(GERALD, FACING_SOUTHEAST);
	Event_OpenMessage(GERALD, 0);
	Actor_FaceDirection(IWAN, FACING_NORTHEAST, 0);
	Actor_FaceDirection(MEARI, FACING_NORTHEAST, 0);
	if (Event_ChooseYesNo(0, 0) == 0)
	{
		i = TRUE;
	}
	else
	{
		gEventWork->message++;
		i = FALSE;
	}
	Task_Wait(20);
	Actor_FaceDirection(GERALD, FACING_WEST, 0);
	Actor_FaceDirection(IWAN, FACING_NORTHWEST, 0);
	VinasuChojo_FaceActor(MEARI, FACING_NORTHWEST);
	VinasuChojo_FaceActor(21, FACING_EAST);
	Event_ShowMessage(21, 0);
	Event_PlayCue(17);
	Event_Wait(40);
	if (i)
	{
		gEventWork->message++;
	}

	Actor_Stop(ACTOR_FIRST_OF_PAIR);
	Actor_Stop(ACTOR_SECOND_OF_PAIR);
	Task_Wait(1);
	actor = Actor_Get(ACTOR_FIRST_OF_PAIR);
	actor->scale_x = 0x10000;
	actor->scale_y = 0x10000;
	actor = Actor_Get(ACTOR_SECOND_OF_PAIR);
	actor->scale_x = 0x10000;
	actor->scale_y = 0x10000;
	Task_Wait(1);
	VinasuChojo_ShowMessage(ACTOR_FIRST_OF_PAIR);
	Actor_FaceDirection(21, FACING_SOUTHEAST + FACING_STEP, 0);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
	Actor_FaceDirection(GERALD, FACING_SOUTHWEST, 0);
	Actor_FaceDirection(IWAN, FACING_WEST, 0);
	Actor_FaceDirection(MEARI, FACING_WEST, 40);
	Actor_SetAnimation(ACTOR_FIRST_OF_PAIR, 1);
	Actor_SetSpriteFlags(Actor_Get(ACTOR_FIRST_OF_PAIR), 1);
	Event_Wait(20);
	Actor_SetSpeed(ACTOR_FIRST_OF_PAIR, 0x3333, 0x1999);
	Actor_WalkToAndWait(ACTOR_FIRST_OF_PAIR, 0x12c, 206);
	Event_Wait(20);
	Actor_RunRepeatedMotion(ACTOR_FIRST_OF_PAIR, 2);
	Event_Wait(20);
	Actor_Get(ACTOR_FIRST_OF_PAIR)->scale_x = -0x10000;
	Event_PlayCue(161);
	Actor_SetAnimation(ACTOR_FIRST_OF_PAIR, 8);
	Event_Wait(20);
	Actor_SetAnimation(ACTOR_SECOND_OF_PAIR, 1);
	Actor_SetSpriteFlags(Actor_Get(ACTOR_SECOND_OF_PAIR), 1);
	Actor_Jump(ACTOR_SECOND_OF_PAIR, 4, 40);
	Actor_SetSpeed(ACTOR_SECOND_OF_PAIR, 0x3333, 0x1999);
	Actor_Get(ACTOR_SECOND_OF_PAIR)->steering_flags &= ~ACTOR_FACES_MOTION;
	Actor_WalkToAndWait(ACTOR_SECOND_OF_PAIR, 0x128, 186);
	Actor_SetSpeed(ACTOR_SECOND_OF_PAIR, 0x1999, 0xccc);
	Actor_WalkToAndWait(ACTOR_SECOND_OF_PAIR, 0x124, 186);
	Actor_Get(ACTOR_SECOND_OF_PAIR)->steering_flags |= ACTOR_FACES_MOTION;
	Actor_Get(ACTOR_SECOND_OF_PAIR)->scale_x = -0x10000;
	Event_PlayCue(161);
	Actor_SetAnimation(ACTOR_SECOND_OF_PAIR, 5);
	Event_Wait(20);
	Actor_RunRepeatedMotion(ACTOR_SECOND_OF_PAIR, 2);
	Event_Wait(20);
	Actor_RunRepeatedMotion(ACTOR_FIRST_OF_PAIR, 1);
	Event_Wait(80);
	Actor_SetAnimationAndWait(MEARI, 4);
	Event_ShowMessageAndWait(MEARI, 0, 20);
	Actor_RunRepeatedMotion(ACTOR_SECOND_OF_PAIR, 2);
	Event_Wait(40);
	VinasuChojo_ShowMessage(ACTOR_SECOND_OF_PAIR);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
	Actor_FaceDirection(GERALD, FACING_SOUTHEAST, 0);
	Actor_FaceDirection(IWAN, FACING_SOUTHWEST, 0);
	Actor_FaceDirection(MEARI, FACING_NORTHEAST, 40);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
	Actor_FaceDirection(GERALD, FACING_SOUTHWEST, 0);
	Actor_FaceDirection(IWAN, FACING_WEST, 0);
	Actor_FaceDirection(MEARI, FACING_WEST, 20);

	actor = Actor_Get(ACTOR_FIRST_OF_PAIR);
	actor->facing = FACING_SOUTHEAST + FACING_STEP;
	actor->scale_x = 0x10000;
	Actor_SetAnimation(ACTOR_FIRST_OF_PAIR, 1);
	Event_Wait(10);
	VinasuChojo_FaceActor(ACTOR_FIRST_OF_PAIR, FACING_NORTH + FACING_STEP);
	Actor_Jump(ACTOR_FIRST_OF_PAIR, 6, 0);
	Event_Wait(10);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
	Actor_FaceDirection(GERALD, FACING_NORTHWEST, 0);
	Actor_FaceDirection(IWAN, FACING_NORTHWEST, 0);
	Actor_FaceDirection(MEARI, FACING_NORTHWEST, 0);
	Actor_FaceDirection(21, FACING_NORTH + FACING_STEP, 0);
	Actor_FaceDirection(6, FACING_EAST, 0);
	star = (union FieldObject *)Object_Create(
		22, actor->x.fixed, actor->y.fixed + PIXELS(8), actor->z.fixed);
	if (star != NULL)
	{
		sprite = star->actor.sprite;
		sprite->part_count = 0;
		sprite->full_color = 0;
		sprite->palette = 0;
		sprite->priority = 0;
		star->actor.priority_flags &= ~1;
		star->actor.motion_flags = 0;
		star->actor.unknown_5c = 1;
		star->actor.speed = 0x19999;
		star->actor.acceleration = 0xcccc;
		buffer = Heap_Allocate(17, 0x608);
		Item_LoadIcon(220);
		Vram_Load(sprite->vram_block, 128, buffer + 0x400);
		Heap_Release(17);
	}
	Actor_SetSpritePriority(22, 1);
	bird = Actor_Get(22);
	bird->x.fixed = actor->x.fixed;
	bird->y.fixed = PIXELS(32);
	bird->z.fixed = actor->z.fixed;
	bird->motion_flags = 3;
	bird->speed = 0x19999;
	bird->acceleration = 0xcccc;
	bird->scale_x = 0xc000;
	bird->scale_y = 0xc000;
	if (star != NULL)
	{
		star->actor.motion_flags = 3;
		star->effect.velocity_y = 0x9999;
		star->effect.velocity_x = 0xcccc;
		star->actor.velocity_y = PIXELS(8);
		FieldObject_SetPosition(&star->actor, PIXELS(308), PIXELS(32), PIXELS(164));
	}
	Actor_MoveToAndWait(22, 0x134, 164);
	Actor_SetPosition(22, 0, 0);
	Actor_SetSpritePriority(21, 0);
	if (star != NULL)
	{
		Event_PlayCue(0x135);
		Actor_SetSpriteFlags(&star->actor, 0);
		star->actor.velocity_y = PIXELS(4);
		FieldObject_SetPosition(&star->actor, PIXELS(314), PIXELS(32), PIXELS(137));
		FieldObject_CommitPosition(&star->actor);
		Event_PlayCue(0x135);
		star->actor.sprite->priority = 1;
		star->actor.velocity_y = PIXELS(6);
		FieldObject_SetPosition(&star->actor, PIXELS(285), PIXELS(32), PIXELS(146));
		FieldObject_CommitPosition(&star->actor);
		Event_PlayCue(0x135);
		star->actor.velocity_y = PIXELS(5);
		FieldObject_SetPosition(&star->actor, PIXELS(300), PIXELS(32), PIXELS(154));
		FieldObject_CommitPosition(&star->actor);
		Task_Wait(6);
		star->actor.x.fixed = 0;
		star->actor.y.fixed = 0;
		star->actor.z.fixed = 0;
		SceneActor_ParkRecord(&star->actor);
	}
	Actor_ShowEmote(21, 0x100, 0);
	Actor_ShowEmote(6, 0x100, 0);
	Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x100, 0);
	Actor_ShowEmote(GERALD, 0x100, 0);
	Actor_ShowEmote(IWAN, 0x100, 0);
	Actor_ShowEmote(MEARI, 0x100, 30);
	Actor_SetSpritePriority(21, 1);
	Actor_Get(21)->priority_flags |= 1;
	Actor_StartRepeatedMotion(IWAN, 2);
	VinasuChojo_ShowMessage(IWAN);
	Actor_RunRepeatedMotion(MEARI, 1);
	Event_ShowMessageAndWait(MEARI, 0, 40);
	Actor_RunRepeatedMotion(ACTOR_FIRST_OF_PAIR, 1);
	Event_Wait(20);
	VinasuChojo_ShowMessage(ACTOR_FIRST_OF_PAIR);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH + FACING_STEP, 0);
	Actor_FaceDirection(GERALD, FACING_SOUTH + FACING_STEP, 0);
	Actor_FaceDirection(IWAN, FACING_WEST, 0);
	Actor_FaceDirection(MEARI, FACING_WEST, 0);
	Actor_FaceDirection(6, FACING_SOUTHEAST + FACING_STEP, 0);
	VinasuChojo_FaceActor(21, FACING_SOUTHEAST + FACING_STEP);
	Actor_ShowEmote(21, 0x101, 0);
	Actor_ShowEmote(6, 0x101, 0);
	Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 0);
	Actor_ShowEmote(GERALD, 0x101, 0);
	Actor_ShowEmote(IWAN, 0x101, 0);
	Actor_ShowEmote(MEARI, 0x101, 60);
	Actor_ShowEmote(ACTOR_FIRST_OF_PAIR, 0x108, 40);
	VinasuChojo_ShowMessage(ACTOR_FIRST_OF_PAIR);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
	Actor_FaceDirection(GERALD, FACING_SOUTHEAST, 40);
	Event_OpenMessage(GERALD, 0);
	if (Event_ChooseYesNo(0, 0) == 0)
	{
		Actor_SetAnimation(GERALD, ANIM_NOD);
		i = TRUE;
	}
	else
	{
		gEventWork->message++;
		Actor_SetAnimation(GERALD, ANIM_SHAKE_HEAD);
		i = FALSE;
	}
	VinasuChojo_ShowMessage(GERALD);
	if (i)
	{
		gEventWork->message++;
	}
	Event_Wait(20);

	Actor_SetSpriteFlags(Actor_Get(24), 0);
	Actor_SetChildValue(24, 7);
	actor = Actor_Get(24);
	actor->scale_y = -0x10000;
	actor->scale_x = 0x1999;
	actor->motion_flags = 0;
	actor->y.fixed = PIXELS(64);
	actor->x.fixed = PIXELS(304);
	actor->z.fixed = PIXELS(158);
	Actor_SetSpriteFlags(Actor_Get(25), 0);
	Actor_SetChildValue(25, 7);
	actor = Actor_Get(25);
	actor->scale_y = -0x10000;
	actor->scale_x = 0x1999;
	actor->motion_flags = 0;
	actor->y.fixed = PIXELS(96);
	actor->x.fixed = PIXELS(304);
	actor->z.fixed = PIXELS(158);
	Actor_ShowEmote(21, 0x100, 0);
	Actor_ShowEmote(6, 0x100, 0);
	Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x100, 0);
	Actor_ShowEmote(GERALD, 0x100, 0);
	Actor_ShowEmote(IWAN, 0x100, 0);
	Actor_ShowEmote(MEARI, 0x100, 0);
	Actor_FaceDirection(GERALD, FACING_NORTHWEST, 0);
	Actor_FaceDirection(IWAN, FACING_NORTHWEST, 0);
	Actor_FaceDirection(MEARI, FACING_NORTHWEST, 0);
	Actor_FaceDirection(21, FACING_NORTH + FACING_STEP, 0);
	Actor_FaceDirection(6, FACING_EAST, 0);
	Actor_EnableActionCallback(24, VinasuChojo_BeamOpenAction);
	Actor_EnableActionCallback(25, VinasuChojo_BeamOpenAction);
	Event_PlayCue(145);
	Work_SetValuesIfNonNegative(0x60000, 0x60000, 0x10000);
	ColorBuffer_ApplyTarget(0x4063ff, 0);
	ColorBuffer_Interpolate(16);
	Task_Wait(20);
	ColorBuffer_ApplyTarget(0x7fff, 0);
	ColorBuffer_Interpolate(24);
	Task_Wait(60);
	Task_AddCallback(SceneEffect_SpawnParticlesAboveActor, TASK_PRIORITY_SCENE);
	Event_PlayCue(141);
	Work_SetValuesIfNonNegative(0x40000, 0x40000, 0x10000);
	ColorBuffer_ApplyTarget(0x4063ff, 0);
	ColorBuffer_Interpolate(120);
	Actor_EnableActionCallback(24, Data_020060ac);
	Actor_EnableActionCallback(25, Data_020060ac);
	Task_Wait(120);
	Work_SetValuesIfNonNegative(0x30000, 0x30000, 0x10000);
	ColorBuffer_ApplyTarget(0x203210, 0);
	ColorBuffer_Interpolate(120);
	Task_Wait(120);
	Event_PlayCue(63);
	Work_SetValuesIfNonNegative(0x20000, 0x20000, 0x10000);
	ColorBuffer_ApplyTarget(0x10000, 0);
	ColorBuffer_Interpolate(120);
	Task_Wait(120);
	Work_SetValuesIfNonNegative(0x10000, 0x10000, 0x10000);

	Actor_SetAnimation(ACTOR_SECOND_OF_PAIR, 1);
	Actor_FaceDirection(ACTOR_SECOND_OF_PAIR, FACING_EAST, 0);
	Actor_Get(ACTOR_SECOND_OF_PAIR)->scale_x = 0x10000;
	Actor_Jump(ACTOR_SECOND_OF_PAIR, 4, 40);
	Actor_RunRepeatedMotion(ACTOR_SECOND_OF_PAIR, 1);
	Event_ShowMessageAndWait(ACTOR_SECOND_OF_PAIR, 0, 20);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
	Actor_FaceDirection(GERALD, FACING_SOUTHWEST, 0);
	Actor_FaceDirection(IWAN, FACING_WEST, 0);
	Actor_FaceDirection(MEARI, FACING_WEST, 0);
	Actor_FaceDirection(21, FACING_SOUTHEAST + FACING_STEP, 0);
	Actor_FaceDirection(6, FACING_SOUTHEAST + FACING_STEP, 20);
	Actor_RunRepeatedMotion(ACTOR_FIRST_OF_PAIR, 1);
	VinasuChojo_ShowMessage(ACTOR_FIRST_OF_PAIR);
	Actor_RunRepeatedMotion(MEARI, 1);
	VinasuChojo_FaceActor(MEARI, FACING_WEST);
	VinasuChojo_ShowMessage(MEARI);
	VinasuChojo_FaceActor(ACTOR_FIRST_OF_PAIR, FACING_EAST);
	Actor_SetAnimationAndWait(ACTOR_FIRST_OF_PAIR, 4);
	VinasuChojo_ShowMessage(ACTOR_FIRST_OF_PAIR);
	Actor_ShowEmote(ACTOR_SECOND_OF_PAIR, 0x103, 20);
	Actor_StartRepeatedMotion(ACTOR_SECOND_OF_PAIR, 2);
	VinasuChojo_ShowMessage(ACTOR_SECOND_OF_PAIR);
	Actor_ShowEmote(GERALD, 0x100, 20);
	VinasuChojo_FaceActor(GERALD, FACING_SOUTHWEST);
	VinasuChojo_ShowMessage(GERALD);
	VinasuChojo_FaceActor(ACTOR_FIRST_OF_PAIR, FACING_NORTH + FACING_STEP);
	Actor_SetAnimationAndWait(ACTOR_FIRST_OF_PAIR, 3);
	Event_ShowMessageAndWait(ACTOR_FIRST_OF_PAIR, 0, 20);
	Actor_RunRepeatedMotion(GERALD, 1);
	VinasuChojo_ShowMessage(GERALD);
	Actor_FaceDirection(ACTOR_SECOND_OF_PAIR, FACING_NORTHWEST + FACING_STEP, 20);
	Actor_RunRepeatedMotion(ACTOR_SECOND_OF_PAIR, 1);
	VinasuChojo_ShowMessage(0x2013);
	Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
	Actor_StartRepeatedMotion(GERALD, 1);
	Actor_StartRepeatedMotion(IWAN, 1);
	Actor_RunRepeatedMotion(MEARI, 1);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
	Actor_FaceDirection(GERALD, FACING_WEST, 0);
	Actor_FaceDirection(IWAN, FACING_NORTHWEST, 0);
	VinasuChojo_FaceActor(MEARI, FACING_NORTHWEST);
	Actor_ShowEmote(21, 0x101, 80);
	VinasuChojo_ShowMessage(21);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
	Actor_FaceDirection(GERALD, FACING_SOUTHEAST, 0);
	Actor_FaceDirection(IWAN, FACING_SOUTHWEST, 0);
	Actor_FaceDirection(MEARI, FACING_NORTHEAST, 40);
	Actor_RunRepeatedMotion(ACTOR_FIRST_OF_PAIR, 1);
	VinasuChojo_ShowMessage(ACTOR_FIRST_OF_PAIR);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
	Actor_FaceDirection(GERALD, FACING_WEST, 0);
	Actor_FaceDirection(IWAN, FACING_WEST, 0);
	Actor_FaceDirection(MEARI, FACING_WEST, 20);
	VinasuChojo_FaceActor(ACTOR_FIRST_OF_PAIR, FACING_NORTHWEST + FACING_STEP);
	VinasuChojo_ShowMessage(0x2014);
	Actor_SetAnimationAndWait(21, 3);
	Actor_FaceDirection(21, FACING_NORTHWEST + FACING_STEP, 60);
	Actor_FaceDirection(21, FACING_SOUTHEAST + FACING_STEP, 40);
	Actor_RunRepeatedMotion(ACTOR_SECOND_OF_PAIR, 1);
	VinasuChojo_ShowMessage(0x2013);
	Actor_FaceDirection(21, FACING_EAST, 40);
	VinasuChojo_ShowMessage(21);
	Actor_FaceDirection(ACTOR_FIRST_OF_PAIR, FACING_NORTH + FACING_STEP, 40);
	VinasuChojo_FaceActor(ACTOR_FIRST_OF_PAIR, FACING_NORTHWEST + FACING_STEP);
	VinasuChojo_ShowMessage(0x2014);
	Actor_FaceDirection(21, FACING_SOUTHEAST + FACING_STEP, 40);
	Actor_FaceDirection(21, FACING_SOUTHEAST + FACING_STEP, 20);
	Actor_SetAnimationAndWait(21, 3);
	Actor_FaceDirection(ACTOR_SECOND_OF_PAIR, FACING_EAST, 40);
	Actor_FaceDirection(ACTOR_SECOND_OF_PAIR, FACING_NORTHWEST + FACING_STEP, 20);
	Actor_ShowEmote(ACTOR_SECOND_OF_PAIR, 0x101, 40);
	VinasuChojo_ShowMessage(0x2013);
	VinasuChojo_FaceActor(21, FACING_SOUTHEAST + FACING_STEP);
	Actor_ShowEmote(21, 0x101, 20);
	VinasuChojo_ShowMessage(21);
	Actor_FaceDirection(ACTOR_FIRST_OF_PAIR, FACING_NORTHWEST + FACING_STEP, 20);
	Actor_SetAnimation(ACTOR_SECOND_OF_PAIR, 4);
	VinasuChojo_ShowMessage(0x2013);
	Actor_ShowEmote(21, 0x103, 80);
	VinasuChojo_ShowMessage(21);
	Actor_SetAnimationAndWait(ACTOR_SECOND_OF_PAIR, 3);
	VinasuChojo_ShowMessage(0x2013);
	Actor_ShowEmote(21, 0x103, 20);
	Actor_StartRepeatedMotion(21, 2);
	VinasuChojo_ShowMessage(21);
	Actor_SetAnimation(ACTOR_SECOND_OF_PAIR, 4);
	VinasuChojo_ShowMessage(0x2013);
	Actor_ShowEmote(21, 0x102, 60);
	Actor_RunRepeatedMotion(21, 1);
	VinasuChojo_ShowMessage(21);
	Actor_ShowEmote(ACTOR_FIRST_OF_PAIR, 0x108, 40);
	Actor_SetAnimation(ACTOR_FIRST_OF_PAIR, 4);
	Event_ShowMessageAndWait(0x2014, 0, 40);
	Actor_RunRepeatedMotion(21, 1);
	Event_Wait(20);
	VinasuChojo_ShowMessage(21);
	Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x100, 0);
	Actor_ShowEmote(GERALD, 0x100, 0);
	Actor_ShowEmote(IWAN, 0x100, 0);
	Actor_ShowEmote(MEARI, 0x100, 0);
	Actor_FaceDirection(IWAN, FACING_NORTHWEST, 0);
	Actor_FaceDirection(MEARI, FACING_NORTHWEST, 20);
	Actor_FaceDirection(ACTOR_SECOND_OF_PAIR, FACING_SOUTHEAST + FACING_STEP, 20);
	Actor_SetAnimation(ACTOR_SECOND_OF_PAIR, 3);
	Actor_SetAnimationAndWait(ACTOR_FIRST_OF_PAIR, 3);
	Actor_FaceDirection(21, FACING_NORTHWEST + FACING_STEP, 20);
	VinasuChojo_ShowMessage(0xa015);
	Actor_SetSpeed(6, 0x10000, 0x8000);
	Actor_SetSpeed(21, 0x10000, 0x8000);
	Actor_EnableActionCallback(21, Data_0200622c);
	Event_Wait(20);
	Actor_EnableActionCallback(6, Data_0200622c);
	VinasuChojo_FaceActor(GERALD, FACING_SOUTHWEST);
	Actor_StartRepeatedMotion(GERALD, 2);
	VinasuChojo_ShowMessage(GERALD);
	Event_PlayCue(17);
	ColorBuffer_ApplyTarget(0x40250d, 1);
	ColorBuffer_Interpolate(40);
	VinasuChojo_FaceActor(ACTOR_FIRST_OF_PAIR, FACING_NORTH + FACING_STEP);
	VinasuChojo_ShowMessage(ACTOR_FIRST_OF_PAIR);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
	Actor_FaceDirection(IWAN, FACING_WEST, 0);
	Actor_FaceDirection(MEARI, FACING_WEST, 0);
	VinasuChojo_FlashScreen();
	Actor_SetChildValue(ACTOR_FIRST_OF_PAIR, 7);
	Actor_SetChildValue(ACTOR_SECOND_OF_PAIR, 7);
	Event_Wait(20);
	Actor_SetChildValue(ACTOR_FIRST_OF_PAIR, 0x100);
	Actor_SetChildValue(ACTOR_SECOND_OF_PAIR, 0x100);
	Event_Wait(20);
	VinasuChojo_FlashScreen();
	Actor_Jump(MEARI, 2, 20);
	VinasuChojo_ShowMessage(MEARI);
	VinasuChojo_FaceActor(ACTOR_SECOND_OF_PAIR, FACING_EAST);
	VinasuChojo_ShowMessage(ACTOR_SECOND_OF_PAIR);
	VinasuChojo_FlashScreen();

	second = Actor_Get(ACTOR_SECOND_OF_PAIR);
	second_burst.palette = 7;
	second_burst.update = Effect_MoveWithDrag;
	second_burst.start_scale_x = 0x10000;
	second_burst.start_scale_y = 0x10000;
	for (i = 0; i <= 16; i++)
	{
		VinasuChojo_AimBurst(velocity, i << 12);
		Effect_Spawn(second->x.fixed, second->y.fixed, second->z.fixed, velocity[0], velocity[1],
			velocity[2], EFFECT_USE_UPDATE | EFFECT_USE_START_SCALE | EFFECT_USE_PALETTE,
			&second_burst);
	}
	Event_PlayCue(212);
	Task_Wait(6);
	VinasuChojo_FlashScreen();
	first = Actor_Get(ACTOR_FIRST_OF_PAIR);
	first_burst.palette = 7;
	first_burst.update = Effect_MoveWithDrag;
	first_burst.start_scale_x = 0x10000;
	first_burst.start_scale_y = 0x10000;
	for (i = 0; i <= 16; i++)
	{
		VinasuChojo_AimBurst(velocity, i << 12);
		Effect_Spawn(first->x.fixed, first->y.fixed, first->z.fixed, velocity[0], velocity[1],
			velocity[2], EFFECT_USE_UPDATE | EFFECT_USE_START_SCALE | EFFECT_USE_PALETTE,
			&first_burst);
	}
	Event_PlayCue(212);
	Actor_Jump(IWAN, 6, 20);
	Event_PlayCue(54);
	VinasuChojo_ShowMessage(IWAN);
	Actor_SetAnimation(ACTOR_FIRST_OF_PAIR, 4);
	VinasuChojo_ShowMessage(ACTOR_FIRST_OF_PAIR);
	VinasuChojo_FlashScreen();
	Actor_SetSpeed(ACTOR_FIRST_OF_PAIR, 0x3333, 0x1999);
	Actor_SetSpeed(ACTOR_SECOND_OF_PAIR, 0x3333, 0x1999);
	Actor_Get(ACTOR_FIRST_OF_PAIR)->steering_flags &= ~ACTOR_FACES_MOTION;
	Actor_Get(ACTOR_SECOND_OF_PAIR)->steering_flags &= ~ACTOR_FACES_MOTION;
	Actor_SetDestination(ACTOR_FIRST_OF_PAIR, 0x126, 196);
	Actor_SetDestination(ACTOR_SECOND_OF_PAIR, 0x126, 196);
	Task_AddCallback(VinasuChojo_UpdateBeamActors, TASK_PRIORITY_SCENE);
	Actor_StartRepeatedMotion(GERALD, 2);
	Event_ShowMessage(GERALD, 0);
	GameFlag_Set(FLAG_PAIR_SLIDING);
	Event_ShowMessage(IWAN, 0);
	GameFlag_Set(FLAG_PAIR_GROWING);
	VinasuChojo_FlashScreen();
	Event_Wait(20);
	VinasuChojo_FlashScreen();
	Event_Wait(20);
	Battle_SetStart(3);
	Party_SetFields1ceAnd1d0((s32)&SceneId_VinasuChojo, 3);
	Event_SetDefeatDestination((s32)&SceneId_VinasuChojo, 9);
	BattleFx_SetWeightedResult(98, 0);
	Owner_RefreshRatiosOnFlag();
	GameFlag_Set(0x351);
}

void VinasuChojo_FlashScreen(void)
{
	Engine_AudioPlayCue(187);
	Engine_ColorBufferApplyTarget(0x7fff, 1);
	Engine_ColorBufferInterpolate(1);
	Engine_TaskWait(4);
	Engine_ColorBufferApplyTarget(0x40250d, 1);
	Engine_ColorBufferInterpolate(1);
	Engine_TaskWait(1);
}

union ChojoOrbitEffect
{
	s32 words[26];
	struct
	{
		u8	unknown_00[8];
		s32 x;
		s32 y;
		s32 z;
		u8	unknown_14[0x1c];
		s32 radius;
		u8	unknown_34[4];
		s32 saved_x;
		s32 unknown_3c;
		s32 saved_z;
		u8	unknown_44[0x20];
		u16 angle;
		u16 unknown_66;
	} orbit;
};

enum
{
	ORBIT_CENTER_ACTOR = 24
};

void SceneEffect_UpdateOrbitAroundActor(union ChojoOrbitEffect *effect)
{
	struct FieldActor *center = Object_GetById(ORBIT_CENTER_ACTOR);
	u16				   angle = effect->orbit.angle;

	effect->orbit.x = center->x.fixed + Engine_MathCos(angle) * (effect->orbit.radius + 3);
	effect->orbit.z = center->z.fixed + (Engine_MathSin(angle) << 1);
	effect->orbit.saved_x = effect->orbit.x;
	effect->orbit.saved_z = effect->orbit.z;
	effect->orbit.angle -= 0x800;
}

void SceneEffect_UpdateCounterDrivenOrbit(u8 *actor)
{
	struct FieldActor *anchor = Object_GetById(23);
	u16				  *pangle = (u16 *)(actor + 100);
	s32				   angle = *pangle;
	s32				   cosine;
	s32				   sine;
	s32				   along;
	s32				   across;

	cosine = Engine_MathCos(angle);
	along = anchor->x.fixed + cosine * (*(s32 *)(actor + 48) + *(u8 *)(actor + 98) + 6);
	*(s32 *)(actor + 8) = along;

	sine = Engine_MathSin(angle);
	across = anchor->z.fixed + sine * (*(u8 *)(actor + 98) + 4);
	*(s32 *)(actor + 16) = across;

	*(s32 *)(actor + 56) = *(s32 *)(actor + 8);
	*(s32 *)(actor + 64) = across;

	{
		s32 next = *pangle;
		next += 0xfffff800;
		*pangle = next;
	}
}

void SceneEffect_SpawnParticlesAboveActor(void)
{
	struct FieldActor	   *center;
	union ChojoOrbitEffect *effect;
	struct FieldSprite	   *sprite;
	s32						angle;

	if (GameFlag_IsSet(0x236) == 0 && Math_RemainderUnsigned(gFrameCount, 3) != 0)
	{
		return;
	}
	center = Actor_Get(ORBIT_CENTER_ACTOR);
	if (GameFlag_IsSet(0x236) != 0)
	{
		effect = (union ChojoOrbitEffect *)Object_Create(284, center->x.fixed,
			(s32)((u32)(Random_Next() << 8) >> 16 << 16) + center->y.fixed - 0x1c0000,
			center->z.fixed);
	}
	else
	{
		effect = (union ChojoOrbitEffect *)Object_Create(284, center->x.fixed,
			(s32)((u32)(Random_Next() << 6) >> 16 << 16) + center->y.fixed - 0x1c0000,
			center->z.fixed);
	}
	if (effect != 0)
	{
		sprite = ((struct FieldEffect *)effect)->sprite;
		Object_SetScript((struct FieldActor *)effect, VinasuChojo_OrbitParticleScript);
		ObjectGroup_SetChildValue((struct FieldActor *)effect, 1);
		((struct FieldEffect *)effect)->motion_flags = 0;
		angle = Random_Next() & 0xffff000;
		effect->orbit.angle = angle;
		effect->orbit.unknown_66 = 0;
		((struct FieldEffect *)effect)->update =
			(void (*)(union FieldObject *))SceneEffect_UpdateOrbitAroundActor;
		effect->orbit.radius = Engine_MathSin((u32)(Random_Next() * 0xffff) >> 20) * 24 >> 16;
		sprite->flags = 0;
		sprite->priority = 1;
	}
}

enum
{
	PARTICLE_SOURCE_ACTOR = 23
};

// 目標なし (停止中) か
static __inline__ s32 Actor_IsParked(struct FieldActor *actor)
{
	if (actor->target_x == ACTOR_NO_TARGET && actor->target_y == actor->target_x
		&& actor->target_z == actor->target_y)
	{
		return 1;
	}
	return 0;
}

void VinasuChojo_UpdateBeamActors(void)
{
	struct FieldActor *first;
	struct FieldActor *second;
	s32				   stopped;

	first = Object_GetById(ACTOR_FIRST_OF_PAIR);
	second = Object_GetById(ACTOR_SECOND_OF_PAIR);
	stopped = Actor_IsParked(first); // 上書きされる: ROM に 1 体目の判定の読み込みだけが残っている
	stopped = Actor_IsParked(second);
	if (stopped != 0)
	{
		first->facing = 0;
		second->facing = 0;
		if (Engine_GameFlagIsSet(FLAG_PAIR_GROWING) != 0)
		{
			Engine_ActorSetChildValue(ACTOR_FIRST_OF_PAIR, 7);
			Engine_ActorSetChildValue(ACTOR_SECOND_OF_PAIR, 7);
			if (first->scale_x >= 0x14000)
			{
				goto slide;
			}
			first->scale_x += 0x200;
			first->scale_y += 0x200;
			second->scale_x += 0x200;
			second->scale_y += 0x200;
		}
		else
		{
			if ((*(s32 *)&gFrameCount & 2) != 0)
			{
				Engine_ActorSetChildValue(ACTOR_FIRST_OF_PAIR, 15);
				Engine_ActorSetChildValue(ACTOR_SECOND_OF_PAIR, 0);
			}
			else
			{
				Engine_ActorSetChildValue(ACTOR_FIRST_OF_PAIR, 0);
				Engine_ActorSetChildValue(ACTOR_SECOND_OF_PAIR, 15);
			}
		}
	slide:;
		if (Engine_GameFlagIsSet(FLAG_PAIR_SLIDING) != 0)
		{
			if (first->x.fixed < PIXELS(312))
			{
				first->x.fixed += 0x1000;
				second->x.fixed += 0x1000;
			}
			if (first->z.fixed > PIXELS(182))
			{
				first->z.fixed += -0x1000;
				second->z.fixed += -0x1000;
			}
		}
	}
}

void FieldScene_RunThreeStepsInBracket(void)
{
	Engine_EventBegin();
	FieldScene_RunPairDefeat();
	FieldScene_RunMultiActorPresentation();
	VinasuChojo_RunActorTransition();
	Engine_EventEnd();
}

void FieldScene_RunBracketedSceneWithFlag282(void)
{
	Engine_EventBegin();
	FieldScene_RestageParty();
	VinasuChojo_TryToPreventFall();
	Engine_GameFlagSet(141 << 1);

	gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 0);
	gEventWork->transition_frames = 24;

	Engine_EventCloseScreen();
	Engine_EventWaitForScreen();
	Engine_EventRequestExit(1);
	Engine_EventEnd();
}

void VinasuChojo_RunLeapEntrance(void)
{
	struct FieldActor *leader;
	struct FieldActor *actor;

	leader = Object_GetById(ACTOR_PARTY_LEADER);
	Engine_EventBegin();
	Engine_EventGetViewCenter()->motion_flags = 0;
	Map_CopyCellsTo(102, 4, 74, 4, 18, 23);
	Map_CopyCellsTo(39, 72, 11, 72, 16, 20);
	Map_CopyCellAttributes(19, 6, 3, 7, 22, 6);
	Map_CopyCellAttributes(19, 6, 3, 7, 13, 6);
	Map_CopyCellAttributes(19, 6, 3, 7, 22, 13);
	Map_CopyCellAttributes(19, 6, 3, 7, 13, 13);
	Engine_TaskWait(1);
	Camera_MoveTo(PIXELS(192), -PIXELS(64), PIXELS(238), 0);
	Engine_TaskWait(1);
	Engine_MapRedraw();
	Engine_TaskWait(1);
	Engine_ActorDestroy(ACTOR_FIRST_OF_PAIR);
	Engine_ActorDestroy(ACTOR_SECOND_OF_PAIR);
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 19);
	Engine_ActorSetSpriteFlags(Actor_Get(ACTOR_PARTY_LEADER), 0);
	leader->x.fixed = PIXELS(346);
	leader->z.fixed = PIXELS(205);
	leader->y.fixed = PIXELS(32);
	leader->facing = FACING_SOUTHWEST;
	SceneActor_ParkRecord(leader);
	Engine_ActorSetAnimation(GERALD, 18);
	Engine_ActorSetSpriteFlags(Actor_Get(GERALD), 0);
	actor = Actor_Get(GERALD);
	actor->x.fixed = PIXELS(356);
	actor->z.fixed = PIXELS(192);
	actor->facing = FACING_NORTHWEST;
	actor->y.fixed = PIXELS(32);
	SceneActor_ParkRecord(actor);
	Engine_ActorSetAnimation(IWAN, 18);
	Engine_ActorSetSpriteFlags(Actor_Get(IWAN), 0);
	actor = Actor_Get(IWAN);
	actor->x.fixed = PIXELS(360);
	actor->facing = FACING_SOUTHEAST;
	actor->y.fixed = PIXELS(32);
	actor->z.fixed = PIXELS(222);
	SceneActor_ParkRecord(actor);
	Engine_ActorSetAnimation(MEARI, 18);
	Engine_ActorSetSpriteFlags(Actor_Get(MEARI), 0);
	actor = Actor_Get(MEARI);
	actor->x.fixed = PIXELS(334);
	actor->facing = FACING_WEST;
	actor->y.fixed = PIXELS(32);
	actor->z.fixed = PIXELS(222);
	SceneActor_ParkRecord(actor);
	Actor_SetPosition(21, PIXELS(196), PIXELS(220));
	Engine_ActorSetAnimation(21, 5);
	Actor_SetPosition(6, PIXELS(188), PIXELS(316));
	Engine_ActorSetAnimation(6, 5);
	Engine_ActorSetSpriteFlags(Actor_Get(6), 0);
	actor = Actor_Get(8);
	actor->x.fixed -= PIXELS(16);
	SceneActor_ParkRecord(actor);
	actor = Object_GetById(9);
	actor->x.fixed -= PIXELS(16);
	SceneActor_ParkRecord(actor);
	actor = Actor_Get(10);
	actor->x.fixed += PIXELS(16);
	SceneActor_ParkRecord(actor);
	actor = Object_GetById(11);
	actor->x.fixed += PIXELS(16);
	SceneActor_ParkRecord(actor);
	Work_SetValuesIfNonNegative(0x10000, 0x10000, 0x10000);
	Engine_ActorSetSpriteFlags(Actor_Get(PARTICLE_SOURCE_ACTOR), 0);
	Object_GetById(PARTICLE_SOURCE_ACTOR)->motion_flags = 4;
	Actor_SetChildValue(PARTICLE_SOURCE_ACTOR, 4);
	Actor_Get(PARTICLE_SOURCE_ACTOR)->y.fixed = PIXELS(40);
	Engine_TaskAddCallback(SceneEffect_SpawnParticlesBesideActor, TASK_PRIORITY_SCENE);
	gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 0);
	gEventWork->transition_frames = 24;
	Engine_EventOpenScreen();
	Engine_EventWaitForScreen();
	Engine_EventWait(40);
	Scene_RunExtendedActorTransition();
	GameFlag_Set(0x9a7);
	Engine_EventRequestExit(2);
}

void FieldScene_RunPairDefeat(void)
{
	struct FieldActor *actor;

	Event_PlayCue(141);
	Map_CopyCellAttributes(17, 10, 4, 2, 17, 8);
	Actor_Get(ACTOR_PARTY_LEADER)->facing = FACING_NORTHWEST;
	Actor_Get(GERALD)->facing = FACING_NORTHWEST;
	Actor_SetPosition(GERALD, PIXELS(328), PIXELS(168));
	Actor_Get(IWAN)->facing = FACING_NORTHWEST;
	Actor_SetPosition(IWAN, PIXELS(340), PIXELS(196));
	Actor_Get(MEARI)->facing = FACING_NORTHWEST;
	Actor_SetPosition(MEARI, PIXELS(326), PIXELS(204));
	Actor_Get(21)->facing = FACING_NORTH + FACING_STEP;
	Actor_SetPosition(21, PIXELS(200), PIXELS(216));
	Actor_Get(6)->facing = FACING_NORTH + FACING_STEP;
	Actor_SetPosition(6, PIXELS(200), PIXELS(216));
	Actor_Get(ACTOR_FIRST_OF_PAIR)->facing = FACING_SOUTHEAST + FACING_STEP;
	Actor_SetPosition(ACTOR_FIRST_OF_PAIR, PIXELS(310), PIXELS(158));
	Actor_Get(ACTOR_SECOND_OF_PAIR)->facing = FACING_SOUTHEAST + FACING_STEP;
	Actor_SetPosition(ACTOR_SECOND_OF_PAIR, PIXELS(292), PIXELS(158));

	Engine_ActorSetSpriteFlags(Actor_Get(24), 0);
	Actor_SetChildValue(24, 7);
	Engine_ActorSetSpritePriority(24, 1);
	actor = Actor_Get(24);
	actor->scale_y = -0x10000;
	actor->scale_x = 0x3333;
	actor->motion_flags = 0;
	actor->x.fixed = PIXELS(304);
	actor->y.fixed = PIXELS(2);
	actor->z.fixed = PIXELS(96);

	Engine_ActorSetSpriteFlags(Actor_Get(25), 0);
	Actor_SetChildValue(25, 7);
	Engine_ActorSetSpritePriority(25, 1);
	actor = Actor_Get(25);
	actor->scale_y = -0x10000;
	actor->scale_x = 0x3333;
	actor->motion_flags = 0;
	actor->x.fixed = PIXELS(304);
	actor->y.fixed = PIXELS(34);
	actor->z.fixed = PIXELS(96);

	Engine_TaskAddCallback(SceneEffect_SpawnParticlesAboveActor, TASK_PRIORITY_SCENE);
	Engine_EventGetViewCenter()->motion_flags = 0;
	Camera_MoveTo(PIXELS(304), PIXELS(32), PIXELS(180), 0);
	Engine_TaskWait(1);
	Engine_MapRedraw();
	Engine_TaskWait(1);
	Work_SetValuesIfNonNegative(0x10000, 0x10000, 0x10000);
	Engine_EventOpenScreen();
	Engine_EventWaitForScreen();
	Engine_EventWait(40);
	Engine_ActorRunRepeatedMotion(ACTOR_FIRST_OF_PAIR, 2);
	Engine_EventWait(10);
	Engine_EventSetMessage((s32)MsgVinasuPairDefeated);
	VinasuChojo_ShowMessage(ACTOR_FIRST_OF_PAIR);
	Engine_ActorRunRepeatedMotion(ACTOR_SECOND_OF_PAIR, 3);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(ACTOR_SECOND_OF_PAIR, 0, 40);
	Event_PlayCue(17);

	actor = Actor_Get(ACTOR_FIRST_OF_PAIR);
	actor->x.fixed = PIXELS(308);
	actor->y.fixed = PIXELS(28);
	actor->z.fixed = PIXELS(152);
	Engine_ActorSetAnimation(ACTOR_FIRST_OF_PAIR, 10);
	Engine_EventWait(20);
	actor->x.fixed = PIXELS(306);
	actor->y.fixed = PIXELS(28);
	actor->z.fixed = PIXELS(152);
	Engine_ActorSetAnimation(ACTOR_FIRST_OF_PAIR, 11);
	Engine_EventWait(12);
	actor->x.fixed = PIXELS(304);
	actor->y.fixed = PIXELS(21);
	actor->z.fixed = PIXELS(152);
	Engine_ActorSetAnimation(ACTOR_FIRST_OF_PAIR, 12);
	Engine_EventWait(8);
	Engine_ActorDestroy(ACTOR_FIRST_OF_PAIR);

	actor = Actor_Get(ACTOR_SECOND_OF_PAIR);
	actor->x.fixed = PIXELS(294);
	actor->y.fixed = PIXELS(28);
	actor->z.fixed = PIXELS(152);
	Engine_ActorSetAnimation(ACTOR_SECOND_OF_PAIR, 8);
	Engine_EventWait(20);
	actor->x.fixed = PIXELS(300);
	actor->y.fixed = PIXELS(27);
	actor->z.fixed = PIXELS(152);
	Engine_ActorSetAnimation(ACTOR_SECOND_OF_PAIR, 9);
	Engine_EventWait(12);
	actor->x.fixed = PIXELS(304);
	actor->y.fixed = PIXELS(17);
	actor->z.fixed = PIXELS(152);
	Engine_ActorSetAnimation(ACTOR_SECOND_OF_PAIR, 10);
	Engine_EventWait(8);
	Engine_ActorDestroy(ACTOR_SECOND_OF_PAIR);
	Engine_EventWait(160);
}

void FieldScene_RunMultiActorPresentation(void)
{
	s32 count_flag;
	s32 request_a;
	s32 request_b;

	Engine_ActorRunRepeatedMotion(GERALD, 1);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(GERALD, 0, 20);
	Engine_ActorRunRepeatedMotion(IWAN, 1);
	Engine_EventWait(20);
	Event_OpenMessage(IWAN, 0);
	Actor_FaceDirection(IWAN, 0xc000, 0);
	VinasuChojo_FaceActor(0, 0x4000);
	count_flag = 0;
	if (Engine_EventChooseYesNo(0, 0) == 0)
	{
		Engine_EventWait(20);
		Engine_ActorSetAnimationAndWait(IWAN, 4);
		count_flag = 1;
	}
	else
	{
		Engine_EventWait(20);
		Engine_ActorRunRepeatedMotion(IWAN, 1);
		gEventWork->message += 1;
	}
	VinasuChojo_ShowMessage(2);
	if (count_flag != 0)
	{
		gEventWork->message += 1;
	}
	Actor_SetSpeed(GERALD, 0x19999, 0xcccc);
	request_a = 0x1001;
	Actor_WalkToAndWait(GERALD, 0x141, 174);
	VinasuChojo_FaceActor(1, 0x2000);
	VinasuChojo_ShowMessage(request_a);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xa000, 0);
	Actor_FaceDirection(IWAN, 0xa000, 0);
	VinasuChojo_FaceActor(3, 0xc000);
	VinasuChojo_FaceActor(1, 0x4000);
	VinasuChojo_ShowMessage(request_a);
	Actor_ShowEmote(GERALD, 0x105, 40);
	Actor_FaceDirection(GERALD, 0x2000, 20);
	VinasuChojo_ShowMessage(request_a);
	Actor_FaceDirection(GERALD, 0xa000, 40);
	Event_ShowMessageAndWait(0x8001, 0, 40);
	Actor_FaceDirection(GERALD, 0x2000, 20);
	VinasuChojo_ShowMessage(request_a);
	Event_PlayCue(17);
	Engine_ActorSetAnimation(MEARI, 4);
	VinasuChojo_ShowMessage(3);
	Actor_FaceDirection(GERALD, 0x4000, 80);
	Actor_FaceDirection(MEARI, 0x6000, 80);
	Actor_ShowEmote(MEARI, 0x100, 40);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x6000, 60);
	Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
	VinasuChojo_FaceActor(2, 0xc000);
	Actor_ShowEmote(IWAN, 0x101, 60);
	Actor_FaceDirection(IWAN, 0x6000, 40);
	Engine_ActorJump(IWAN, 4, 60);
	VinasuChojo_FaceActor(1, 0x2000);
	Actor_ShowEmote(GERALD, 0x101, 40);
	VinasuChojo_ShowMessage(request_a);
	Actor_FaceDirection(GERALD, 0x6000, 40);
	Actor_ShowEmote(GERALD, 0x100, 0);
	Engine_ActorJump(GERALD, 4, 40);
	VinasuChojo_ShowMessage(1);
	Actor_SetSpeed(21, 0xcccc, 0x6666);
	Actor_SetDestination(21, 200, 188);
	Actor_SetDestination(6, 200, 204);
	Camera_SetSpeed(0x33333, 0x6666);
	Camera_MoveTo(0xfc0000, 0, 0xbe0000, 1);
	Engine_CameraWaitForMove();
	Engine_EventWait(40);
	Event_PlayCue(23);
	Actor_SetAttachedEffect(21, 0x102);
	VinasuChojo_ShowMessage(21);
	Engine_ActorRunRepeatedMotion(21, 1);
	Engine_EventWait(20);
	VinasuChojo_ShowMessage(21);
	Engine_ActorSetAnimationAndWait(21, 4);
	VinasuChojo_ShowMessage(21);
	Actor_FaceDirection(21, 0x3000, 20);
	VinasuChojo_ShowMessage(21);
	Actor_SetAttachedEffect(6, 0x102);
	request_b = 0x2003;
	Engine_EventWait(20);
	Engine_ActorStartRepeatedMotion(MEARI, 2);
	VinasuChojo_ShowMessage(request_b);
	Engine_ActorRunRepeatedMotion(21, 2);
	Engine_EventWait(20);
	VinasuChojo_ShowMessage(0x2002);
	VinasuChojo_FaceActor(21, 0xe000);
	Engine_ActorRunRepeatedMotion(GERALD, 1);
	VinasuChojo_ShowMessage(1);
	Engine_ActorRunRepeatedMotion(21, 1);
	Event_ShowMessageAndWait(21, 0, 20);
	VinasuChojo_ShowMessage(request_b);
	Engine_ActorSetAnimationAndWait(21, 4);
	VinasuChojo_ShowMessage(21);
	Actor_SetAttachedEffect(IWAN, 0x102);
	VinasuChojo_ShowMessage(0x2002);
	Engine_ActorSetAnimation(21, 3);
	VinasuChojo_ShowMessage(21);
	Actor_ShowEmote(GERALD, 0x103, 40);
	Engine_ActorStartRepeatedMotion(GERALD, 2);
	VinasuChojo_ShowMessage(1);
	Engine_ActorSetAnimation(21, 4);
	VinasuChojo_ShowMessage(21);
	Actor_FaceDirection(21, 0x5000, 20);
	Engine_ActorJump(MEARI, 4, 20);
	Event_ShowMessageAndWait(request_b, 0, 20);
	Actor_ShowEmote(21, 0x103, 40);
}

static s32 VinasuChojo_TransitionTimer;
static s32 VinasuChojo_TransitionStep;

void VinasuChojo_RunActorTransition(void)
{
	struct FieldActor *lower;
	struct FieldActor *middle;
	struct FieldActor *upper;
	struct FieldActor *actor;

	Engine_AudioPlayCue(19);
	Engine_AudioPlayCue(0x120);
	Work_SetValuesIfNonNegative(0x30000, 0x30000, 0x10000);
	Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x100, 0);
	Actor_ShowEmote(GERALD, 0x100, 0);
	Actor_ShowEmote(IWAN, 0x100, 0);
	Actor_ShowEmote(MEARI, 0x100, 0);
	Actor_ShowEmote(21, 0x100, 0);
	Actor_ShowEmote(6, 0x100, 10);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
	Actor_FaceDirection(GERALD, FACING_NORTHWEST, 0);
	Actor_FaceDirection(IWAN, FACING_NORTHWEST, 0);
	Actor_FaceDirection(MEARI, FACING_NORTHWEST, 0);
	Actor_FaceDirection(21, FACING_NORTH + FACING_STEP, 0);
	Actor_FaceDirection(6, FACING_NORTH + FACING_STEP, 0);
	Work_SetValuesIfNonNegative(0x20000, 0x20000, 0x10000);
	Engine_EventWait(10);
	Work_SetValuesIfNonNegative(0x10000, 0x10000, 0x10000);
	Camera_MoveTo(PIXELS(304), PIXELS(32), PIXELS(180), 1);
	Engine_CameraWaitForMove();
	Object_GetById(24)->scale_x = 0x1999;
	Object_GetById(25)->scale_x = 0x1999;
	Engine_ActorEnableActionCallback(24, VinasuChojo_BeamOpenAction);
	Engine_ActorEnableActionCallback(25, VinasuChojo_BeamOpenAction);
	Engine_AudioPlayCue(145);
	Work_SetValuesIfNonNegative(0x60000, 0x60000, 0x10000);
	ColorBuffer_ApplyTarget(0x4063ff, 0);
	Engine_ColorBufferInterpolate(16);
	Engine_TaskWait(20);
	Engine_ColorBufferApplyTarget(0x7fff, 0);
	Engine_ColorBufferInterpolate(24);
	Engine_TaskWait(60);
	Engine_AudioPlayCue(141);
	Engine_GameFlagSet(0x236);
	Object_GetById(24)->y.fixed = -PIXELS(96);
	Object_GetById(25)->y.fixed = -PIXELS(64);
	Engine_ActorSetChildValue(26, 7);
	Engine_ActorSetSpriteFlags(Object_GetById(26), 0);
	lower = Object_GetById(26);
	lower->scale_y = -0x10000;
	lower->scale_x = Object_GetById(24)->scale_x;
	lower->motion_flags = 0;
	lower->x.fixed = PIXELS(304);
	lower->y.fixed = -PIXELS(32);
	lower->z.fixed = PIXELS(96);
	Engine_ActorSetChildValue(27, 7);
	Engine_ActorSetSpriteFlags(Object_GetById(27), 0);
	middle = Object_GetById(27);
	middle->scale_y = -0x10000;
	middle->scale_x = Object_GetById(24)->scale_x;
	middle->motion_flags = 0;
	middle->x.fixed = PIXELS(304);
	middle->y.fixed = 0;
	middle->z.fixed = PIXELS(96);
	Engine_ActorSetChildValue(28, 7);
	Engine_ActorSetSpriteFlags(Object_GetById(28), 0);
	upper = Object_GetById(28);
	upper->scale_y = -0x10000;
	upper->scale_x = Object_GetById(24)->scale_x;
	upper->motion_flags = 0;
	upper->x.fixed = PIXELS(304);
	upper->y.fixed = PIXELS(32);
	upper->z.fixed = PIXELS(96);
	Map_CopyCellsTo(102, 4, 74, 4, 18, 23);
	Map_CopyCellsTo(39, 72, 11, 72, 16, 21);
	Engine_MapCopyCellAttributes(19, 6, 3, 7, 22, 6);
	Engine_MapCopyCellAttributes(19, 6, 3, 7, 13, 6);
	Engine_MapCopyCellAttributes(19, 6, 3, 7, 22, 13);
	Engine_MapCopyCellAttributes(19, 6, 3, 7, 13, 13);
	Engine_TaskWait(1);
	actor = Object_GetById(8);
	actor->x.fixed -= PIXELS(16);
	SceneActor_ParkRecord(actor);
	actor = Object_GetById(9);
	actor->x.fixed -= PIXELS(16);
	SceneActor_ParkRecord(actor);
	actor = Object_GetById(10);
	actor->x.fixed += PIXELS(16);
	SceneActor_ParkRecord(actor);
	actor = Object_GetById(11);
	actor->x.fixed += PIXELS(16);
	SceneActor_ParkRecord(actor);
	actor = Object_GetById(ACTOR_PARTY_LEADER);
	actor->x.fixed += PIXELS(16);
	actor->z.fixed += PIXELS(16);
	SceneActor_ParkRecord(actor);
	actor = Object_GetById(GERALD);
	actor->x.fixed += PIXELS(16);
	actor->z.fixed += PIXELS(16);
	SceneActor_ParkRecord(actor);
	actor = Object_GetById(IWAN);
	actor->x.fixed += PIXELS(16);
	actor->z.fixed += PIXELS(16);
	SceneActor_ParkRecord(actor);
	actor = Object_GetById(MEARI);
	actor->x.fixed += PIXELS(16);
	actor->z.fixed += PIXELS(16);
	SceneActor_ParkRecord(actor);
	Actor_SetPosition(21, PIXELS(196), PIXELS(220));
	Engine_ActorSetAnimation(21, 5);
	Actor_SetPosition(6, PIXELS(188), PIXELS(316));
	Engine_ActorSetAnimation(6, 5);
	Engine_ActorSetSpriteFlags(Object_GetById(6), 0);
	Work_SetValuesIfNonNegative(0x40000, 0x40000, 0x10000);
	Engine_ColorBufferApplyTarget(0x4063ff, 0);
	Engine_ColorBufferInterpolate(120);
	Engine_ActorEnableActionCallback(24, VinasuChojo_BeamRiseAction);
	Engine_ActorEnableActionCallback(25, VinasuChojo_BeamRiseAction);
	Engine_ActorEnableActionCallback(26, VinasuChojo_BeamRiseAction);
	Engine_ActorEnableActionCallback(27, VinasuChojo_BeamRiseAction);
	Engine_ActorEnableActionCallback(28, VinasuChojo_BeamRiseAction);
	Engine_TaskWait(120);
	Work_SetValuesIfNonNegative(0x30000, 0x30000, 0x10000);
	Engine_ColorBufferApplyTarget(0x203210, 0);
	Engine_ColorBufferInterpolate(120);
	Engine_TaskWait(120);
	Work_SetValuesIfNonNegative(0x20000, 0x20000, 0x10000);
	ColorBuffer_ApplyTarget(0x10000, 0);
	Engine_ColorBufferInterpolate(120);
	Engine_TaskWait(120);
	Work_SetValuesIfNonNegative(0x10000, 0x10000, 0x10000);
	Object_GetById(24)->scale_y = 0x51e;
	Engine_ActorEnableActionCallback(25, VinasuChojo_BeamCloseAction);
	Engine_ActorEnableActionCallback(26, VinasuChojo_BeamCloseAction);
	Engine_ActorEnableActionCallback(27, VinasuChojo_BeamCloseAction);
	Object_SetActionCallbackAndRefreshById(28, VinasuChojo_BeamCloseAction);
	Engine_AudioPlayCue(0x121);
	Engine_ActorSetChildValue(24, 15);
	Engine_EventWait(20);
	Object_SetActionCallbackAndRefreshById(24, VinasuChojo_BeamEndAction);
	Scheduler_RemoveCallback((u32)SceneEffect_SpawnParticlesAboveActor);
	Engine_ActorJump(IWAN, 2, 20);
	VinasuChojo_ShowMessage(IWAN);
	Actor_FaceDirection(GERALD, FACING_SOUTHWEST, 20);
	Actor_SetAttachedEffect(GERALD, 0x102);
	Engine_EventWait(20);
	Engine_ActorStartRepeatedMotion(GERALD, 2);
	VinasuChojo_ShowMessage(GERALD);
	Actor_SetSpeed(MEARI, 0xcccc, 0x6666);
	Engine_ActorWalkToAndWait(MEARI, 326, 220);
	Engine_EventWait(40);
	Engine_ActorSetAttachedEffect(MEARI, 0x102);
	VinasuChojo_ShowMessage(MEARI);
	actor = Actor_Get(ACTOR_PARTY_LEADER);
	actor->rise_counter = 0;
	actor->rise_enabled = 1;
	actor->rise_base_y = actor->y.fixed;
	actor->facing = FACING_NORTHWEST;
	actor = Actor_Get(GERALD);
	actor->rise_counter = 0;
	actor->rise_enabled = 1;
	actor->facing = FACING_NORTHWEST;
	actor->rise_base_y = actor->y.fixed;
	actor = Actor_Get(IWAN);
	actor->rise_counter = 0;
	actor->rise_enabled = 1;
	actor->facing = FACING_NORTHWEST;
	actor->rise_base_y = actor->y.fixed;
	actor = Actor_Get(MEARI);
	actor->rise_counter = 0;
	actor->rise_enabled = 1;
	actor->facing = FACING_NORTHWEST;
	actor->rise_base_y = actor->y.fixed;
	actor = Actor_Get(21);
	actor->rise_counter = 0;
	actor->rise_enabled = 1;
	actor->rise_base_y = actor->y.fixed;
	actor = Actor_Get(6);
	actor->rise_counter = 0;
	actor->rise_enabled = 1;
	actor->rise_base_y = actor->y.fixed;
	Object_GetById(PARTICLE_SOURCE_ACTOR)->motion_flags = 0;
	Engine_ActorSetSpriteFlags(Object_GetById(PARTICLE_SOURCE_ACTOR), 0);
	Engine_ActorSetChildValue(PARTICLE_SOURCE_ACTOR, 7);
	Engine_ActorSetSpritePriority(PARTICLE_SOURCE_ACTOR, 2);
	VinasuChojo_TransitionStep = 0;
	VinasuChojo_TransitionTimer = 240;
	Engine_TaskAddCallback(VinasuChojo_RunTransitionStep, TASK_PRIORITY_SCENE);
	do
	{
		Engine_TaskWait(1);
	} while (Engine_GameFlagIsSet(0x237) == 0);
	Engine_GameFlagSet(0x101);
	Engine_EventWait(30);
	Engine_GameFlagSet(0x11a);
	ObjectTable_Snapshot();
	Event_SetDestination((s32)&SceneId_WorldMap, 91);
	{
		u16 white = 0x7fff;

		PLTT_BACKDROP = white;
	}
	gEventWork->transition_frames = 1;
	Engine_EventCloseScreen();
	Engine_EventWaitForScreen();
}

void FieldScene_RestageParty(void)
{
	struct FieldActor *actor;

	Engine_ActorDestroy(ACTOR_FIRST_OF_PAIR);
	Engine_ActorDestroy(ACTOR_SECOND_OF_PAIR);
	Event_PlayCue(141);
	Map_CopyCellAttributes(17, 10, 4, 2, 17, 8);
	Map_CopyCellsTo(102, 4, 74, 4, 18, 23);
	Map_CopyCellsTo(39, 72, 11, 72, 16, 21);
	Map_CopyCellAttributes(19, 6, 3, 7, 22, 6);
	Map_CopyCellAttributes(19, 6, 3, 7, 13, 6);
	Map_CopyCellAttributes(19, 6, 3, 7, 22, 13);
	Map_CopyCellAttributes(19, 6, 3, 7, 13, 13);
	Work_SetValuesIfNonNegative(0x20000, 0x20000, 0x10000);
	Camera_MoveTo(-1, -1, -1, 0);
	Engine_ActorsRefresh();
	Engine_MapRedraw();
	Engine_TaskWait(1);
	Event_PlayCue(SOUND_ITEM_BREAK);
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 19);
	Engine_ActorSetAnimation(GERALD, 18);
	Engine_ActorSetAnimation(IWAN, 18);
	Engine_ActorSetAnimation(MEARI, 18);
	Engine_ActorSetSpriteFlags(Actor_Get(ACTOR_PARTY_LEADER), 0);
	Engine_ActorSetSpriteFlags(Actor_Get(GERALD), 0);
	Engine_ActorSetSpriteFlags(Actor_Get(IWAN), 0);
	Engine_ActorSetSpriteFlags(Actor_Get(MEARI), 0);

	actor = Actor_Get(ACTOR_PARTY_LEADER);
	actor->x.fixed = PIXELS(346);
	actor->y.fixed = PIXELS(32);
	actor->z.fixed = PIXELS(205);
	SceneActor_ParkRecord(actor);
	actor->rise_enabled = 0;
	actor->velocity_y = 0x20000;

	actor = Actor_Get(GERALD);
	actor->x.fixed = PIXELS(356);
	actor->y.fixed = PIXELS(32);
	actor->z.fixed = PIXELS(192);
	SceneActor_ParkRecord(actor);
	actor->rise_enabled = 0;
	actor->velocity_y = 0x20000;

	actor = Actor_Get(IWAN);
	actor->x.fixed = PIXELS(360);
	actor->y.fixed = PIXELS(32);
	actor->z.fixed = PIXELS(222);
	SceneActor_ParkRecord(actor);
	actor->rise_enabled = 0;
	actor->velocity_y = 0x20000;

	actor = Actor_Get(MEARI);
	actor->x.fixed = PIXELS(334);
	actor->y.fixed = PIXELS(32);
	actor->z.fixed = PIXELS(222);
	SceneActor_ParkRecord(actor);
	actor->rise_enabled = 0;
	actor->velocity_y = 0x20000;

	Actor_Get(21)->rise_counter = 0;
	Actor_Get(6)->rise_counter = 0;
	Actor_Get(PARTICLE_SOURCE_ACTOR)->motion_flags |= 4;
	Actor_SetChildValue(PARTICLE_SOURCE_ACTOR, 4);
	Engine_TaskAddCallback(SceneEffect_SpawnParticlesBesideActor, TASK_PRIORITY_SCENE);
	gEventWork->transition_frames = 1;
	Engine_EventOpenScreen();
	ColorBuffer_ApplySource(0x7fff, 0);
	ColorBuffer_ApplyTarget(0x10000, 0);
	Engine_ColorBufferInterpolate(40);
	Engine_TaskWait(60);
}

void VinasuChojo_TryToPreventFall(void)
{
	Engine_EventSetMessage((s32)MsgVinasuNoooo);
	VinasuChojo_ShowMessage(21);
	Event_PlayCue(62);
	Work_SetValuesIfNonNegative(0x10000, 0x10000, 0x10000);
	Camera_SetSpeed(0x4cccc, 0x9999);
	Camera_SetSpeed(0x40000, 0x8000);
	Camera_MoveTo(0xc00000, -0x400000, 0xee0000, 1);
	Engine_CameraWaitForMove();
	Engine_EventWait(40);
	Engine_ActorRunRepeatedMotion(21, 1);
	Event_ShowMessageAndWait(0x2015, 0, 40);
	Engine_ActorRunRepeatedMotion(6, 3);
	VinasuChojo_ShowMessage(6);
	Actor_SetAttachedEffect(21, 0x102);
	Engine_EventWait(60);
	Event_ShowMessageAndWait(0x2015, 0, 80);
	Actor_SetAttachedEffect(6, 0x102);
	Engine_EventWait(40);
	Engine_ActorStartRepeatedMotion(6, 2);
	VinasuChojo_ShowMessage(6);
}

void Scene_RunExtendedActorTransition(void)
{
	struct FieldActor *actor;
	s32				   step_pending;

	step_pending = 0;
	Engine_EventSetMessage((s32)MsgVinasuWhyHappeningProtectVenusLighthouse);
	Actor_SetAttachedEffect(21, 0x102);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(0x2015, 0, 20);
	Engine_ActorRunRepeatedMotion(6, 2);
	Event_ShowMessageAndWait(6, 0, 20);
	Engine_ActorSetAnimation(6, 6);
	Engine_EventWait(10);
	Engine_ActorStartRepeatedMotion(21, 2);
	Event_ShowMessageAndWait(0x2015, 0, 40);
	Engine_ActorSetAnimation(6, 7);
	Event_ShowMessageAndWait(6, 0, 20);
	Event_PlayCue(17);
	Engine_ActorRunRepeatedMotion(6, 2);
	Engine_ActorEnableActionCallback(6, VinasuChojo_StepBackAction);
	Engine_EventWait(20);
	Actor_SetAttachedEffect(21, 0x102);
	Engine_ActorStartRepeatedMotion(21, 3);
	Object_RefreshSelectorById(6);
	Engine_EventWait(160);
	Engine_ActorRunRepeatedMotion(21, 1);
	Engine_EventWait(20);
	Object_GetById(21)->facing = FACING_SOUTH + FACING_STEP;
	Engine_ActorSetAnimation(21, 0);
	Engine_EventWait(80);
	Engine_ActorSetAnimationAndWait(21, 4);
	Engine_EventWait(40);
	Event_ShowMessageAndWait(21, 0, 40);
	Engine_ActorSetAnimation(21, 5);
	Engine_EventWait(10);
	Engine_ActorSetSpriteFlags(Object_GetById(21), 0);
	Engine_ActorSetAnimation(21, 0);
	Engine_ActorJump(21, 6, 0);
	Actor_SetSpeed(21, 0x30000, 0x18000);
	Object_GetById(21)->steering_flags &= ~ACTOR_FACES_MOTION;
	actor = Object_GetById(21);
	actor->update = (void (*)(union FieldObject *))VinasuChojo_StepSpriteRotation;
	actor->facing = FACING_WEST;
	Actor_MoveToAndWait(21, 184, 237);
	Object_SetActionCallbackAndRefreshById(21, VinasuChojo_LeapAction);
	Engine_EventWait(120);
	VinasuChojo_ShowMessage(ACTOR_PARTY_LEADER);
	Event_PlayCue(72);
	Camera_SetSpeed(0x40000, 0x8000);
	Camera_MoveTo(PIXELS(342), PIXELS(32), PIXELS(212), 1);
	Engine_CameraWaitForMove();
	Engine_EventWait(20);
	Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
	Engine_EventWait(40);
	Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_PARTY_LEADER), 1);
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 1);
	Engine_ActorJump(ACTOR_PARTY_LEADER, 6, 60);
	Object_GetById(ACTOR_PARTY_LEADER)->timer = 10;
	Engine_ActorEnableActionCallback(ACTOR_PARTY_LEADER, VinasuChojo_ScaleStepAction);
	Actor_ShowEmote(GERALD, 0x100, 80);
	Event_ShowMessageAndWait(GERALD, 0, 40);
	Engine_ActorRunRepeatedMotion(IWAN, 1);
	Engine_EventWait(20);
	VinasuChojo_ShowMessage(IWAN);
	Actor_ShowEmote(MEARI, 0x102, 40);
	VinasuChojo_ShowMessage(MEARI);
	Engine_ActorSetSpriteFlags(Object_GetById(GERALD), 1);
	Engine_ActorSetAnimation(GERALD, 1);
	Engine_ActorJump(GERALD, 6, 60);
	Engine_ActorRunRepeatedMotion(IWAN, 1);
	Engine_EventWait(20);
	Engine_ActorSetSpriteFlags(Object_GetById(IWAN), 1);
	Engine_ActorSetAnimation(IWAN, 1);
	Engine_ActorJump(IWAN, 6, 40);
	Engine_ActorSetSpriteFlags(Object_GetById(MEARI), 1);
	Engine_ActorSetAnimation(MEARI, 1);
	Engine_ActorJump(MEARI, 6, 60);
	Actor_FaceDirection(GERALD, FACING_SOUTHWEST, 60);
	Engine_ActorSetAnimation(GERALD, 4);
	VinasuChojo_ShowMessage(GERALD);
	Actor_FaceDirection(IWAN, FACING_SOUTHWEST, 20);
	Engine_ActorSetAnimation(IWAN, 4);
	VinasuChojo_ShowMessage(IWAN);
	VinasuChojo_FaceActor(MEARI, FACING_SOUTHWEST);
	Engine_ActorSetAnimation(MEARI, 4);
	Event_ShowMessageAndWait(MEARI, 0, 80);
	VinasuChojo_FaceActor(GERALD, FACING_SOUTH);
	Event_ShowMessageAndWait(GERALD, 0, 40);
	Actor_FaceDirection(IWAN, FACING_NORTH, 20);
	Event_ShowMessageAndWait(IWAN, 0, 40);
	Engine_ActorStartRepeatedMotion(GERALD, 1);
	Engine_ActorRunRepeatedMotion(IWAN, 1);
	Engine_EventWait(20);
	Actor_FaceDirection(GERALD, FACING_SOUTHWEST, 0);
	Actor_FaceDirection(IWAN, FACING_SOUTHWEST, 20);
	Engine_ActorRunRepeatedMotion(MEARI, 1);
	Actor_FaceDirection(MEARI, FACING_NORTHEAST, 20);
	VinasuChojo_ShowMessage(MEARI);
	Actor_ShowEmote(GERALD, 0x100, 20);
	VinasuChojo_FaceActor(GERALD, FACING_SOUTH);
	VinasuChojo_ShowMessage(GERALD);
	Actor_FaceDirection(IWAN, FACING_NORTHWEST, 0);
	Actor_SetSpeed(GERALD, 0x19999, 0xcccc);
	Actor_WalkToAndWait(GERALD, 333, 194);
	Actor_WalkToAndWait(GERALD, 333, 206);
	Actor_FaceDirection(GERALD, FACING_EAST, 20);
	Engine_ActorRunRepeatedMotion(GERALD, 1);
	Event_ShowMessageAndWait(GERALD, 0, 80);
	Engine_ActorStop(ACTOR_PARTY_LEADER);
	Engine_TaskWait(1);
	actor->scale_x = 0x10000;
	actor->scale_y = 0x10000;
	Engine_EventWait(40);
	Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x100, 80);
	VinasuChojo_FaceActor(GERALD, FACING_SOUTHEAST);
	Engine_ActorJump(GERALD, 4, 0);
	VinasuChojo_ShowMessage(GERALD);
	VinasuChojo_FaceActor(GERALD, FACING_NORTHEAST);
	Event_OpenMessage(GERALD, 0);
	if (Engine_EventChooseYesNo(0, 0) == 0)
	{
		Engine_EventWait(20);
		Engine_ActorSetAnimationAndWait(GERALD, 3);
		VinasuChojo_ShowMessage(GERALD);
		Engine_ActorRunRepeatedMotion(IWAN, 1);
		VinasuChojo_ShowMessage(IWAN);
		Engine_ActorSetAnimation(MEARI, 4);
		VinasuChojo_ShowMessage(MEARI);
		step_pending = 1;
	}
	else
	{
		gEventWork->message += 3;
		Engine_EventWait(60);
		VinasuChojo_ShowMessage(GERALD);
		Engine_ActorRunRepeatedMotion(IWAN, 1);
		VinasuChojo_ShowMessage(IWAN);
		Engine_ActorSetAnimation(MEARI, 4);
		VinasuChojo_ShowMessage(MEARI);
	}
	if (step_pending != 0)
	{
		gEventWork->message += 3;
	}
	VinasuChojo_FaceActor(ACTOR_PARTY_LEADER, FACING_SOUTH);
	Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
	Engine_EventWait(80);
	Event_PlayCue(17);
	Actor_ShowEmote(GERALD, 0x100, 20);
	Actor_FaceDirection(GERALD, FACING_NORTHWEST, 20);
	Actor_FaceDirection(GERALD, FACING_NORTHEAST, 40);
	Actor_FaceDirection(GERALD, FACING_WEST, 20);
	Actor_ShowEmote(IWAN, 0x101, 40);
	VinasuChojo_ShowMessage(IWAN);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
	Actor_FaceDirection(GERALD, FACING_SOUTHEAST, 20);
	Engine_ActorSetAnimation(GERALD, 4);
	VinasuChojo_ShowMessage(GERALD);
	Actor_FaceDirection(IWAN, FACING_NORTH, 0);
	Actor_FaceDirection(MEARI, FACING_WEST, 40);
	Actor_FaceDirection(IWAN, FACING_NORTHWEST, 0);
	Actor_FaceDirection(MEARI, FACING_NORTH, 40);
	Actor_SetAttachedEffect(MEARI, 0x102);
	Engine_EventWait(60);
	VinasuChojo_ShowMessage(MEARI);
	Event_PlayCue(141);
	Work_SetValuesIfNonNegative(0x60000, 0x60000, 0x10000);
	Engine_EventWait(40);
	Event_PlayCue(145);
	Map_CopyCellsTo(110, 105, 74, 4, 18, 23);
	Map_CopyCellsTo(92, 86, 83, 4, 8, 23);
	Map_CopyCellsTo(75, 28, 75, 4, 8, 23);
	Map_CopyCellsTo(92, 86, 11, 72, 16, 20);
	Map_CopyCellsTo(19, 92, 19, 68, 8, 21);
	actor = Object_GetById(ACTOR_PARTY_LEADER);
	actor->z.fixed -= PIXELS(32);
	actor->work = 0;
	SceneActor_ParkRecord(actor);
	actor = Object_GetById(GERALD);
	actor->x.fixed -= PIXELS(4);
	actor->z.fixed -= PIXELS(32);
	actor->work = 0;
	SceneActor_ParkRecord(actor);
	actor = Object_GetById(IWAN);
	actor->x.fixed -= PIXELS(4);
	actor->z.fixed -= PIXELS(32);
	actor->work = 0;
	SceneActor_ParkRecord(actor);
	actor = Object_GetById(MEARI);
	actor->x.fixed -= PIXELS(4);
	actor->z.fixed -= PIXELS(18);
	actor->work = 0;
	SceneActor_ParkRecord(actor);
	Object_GetById(23)->y.fixed = PIXELS(56);
	Camera_MoveTo(PIXELS(338), PIXELS(32), PIXELS(180), 0);
	Engine_MapRedraw();
	Engine_TaskWait(1);
	Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x100, 0);
	Actor_ShowEmote(GERALD, 0x100, 0);
	Actor_ShowEmote(IWAN, 0x100, 0);
	Actor_ShowEmote(MEARI, 0x100, 0);
	Engine_ActorJump(ACTOR_PARTY_LEADER, 6, 0);
	Engine_ActorJump(GERALD, 6, 0);
	Engine_ActorJump(IWAN, 6, 0);
	Engine_ActorJump(MEARI, 6, 0);
	Engine_ActorEnableActionCallback(ACTOR_PARTY_LEADER, VinasuChojo_PartyHeadingAction);
	Engine_ActorEnableActionCallback(GERALD, VinasuChojo_PartyHeadingAction);
	Engine_ActorEnableActionCallback(IWAN, VinasuChojo_PartyHeadingAction);
	Engine_ActorEnableActionCallback(MEARI, VinasuChojo_PartyHeadingAction);
	Work_SetValuesIfNonNegative(0x30000, 0x30000, 0x10000);
	Engine_EventWait(80);
	Work_SetValuesIfNonNegative(0x60000, 0x60000, 0x10000);
	Engine_EventWait(40);
	Event_PlayCue(145);
	Map_CopyCellAttributes(110, 106, 18, 14, 10, 5);
	Map_CopyCellsTo(110, 105, 74, 4, 18, 23);
	Map_CopyCellsTo(92, 86, 11, 68, 16, 20);
	actor = Object_GetById(ACTOR_PARTY_LEADER);
	actor->x.fixed -= PIXELS(16);
	SceneActor_ParkRecord(actor);
	actor = Object_GetById(GERALD);
	actor->x.fixed -= PIXELS(16);
	SceneActor_ParkRecord(actor);
	actor = Object_GetById(IWAN);
	actor->x.fixed -= PIXELS(16);
	SceneActor_ParkRecord(actor);
	actor = Object_GetById(MEARI);
	actor->x.fixed -= PIXELS(16);
	SceneActor_ParkRecord(actor);
	actor = Object_GetById(8);
	actor->x.fixed += PIXELS(16);
	SceneActor_ParkRecord(actor);
	actor = Object_GetById(9);
	actor->x.fixed += PIXELS(16);
	SceneActor_ParkRecord(actor);
	actor = Object_GetById(10);
	actor->x.fixed -= PIXELS(16);
	SceneActor_ParkRecord(actor);
	actor = Object_GetById(11);
	actor->x.fixed -= PIXELS(16);
	SceneActor_ParkRecord(actor);
	Camera_MoveTo(PIXELS(322), PIXELS(32), PIXELS(180), 0);
	Engine_MapRedraw();
	Engine_TaskWait(1);
	Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
	Actor_SetAttachedEffect(GERALD, 0x102);
	Actor_SetAttachedEffect(IWAN, 0x102);
	Actor_SetAttachedEffect(MEARI, 0x102);
	Engine_ActorJump(ACTOR_PARTY_LEADER, 6, 0);
	Engine_ActorJump(GERALD, 6, 0);
	Engine_ActorJump(IWAN, 6, 0);
	Engine_ActorJump(MEARI, 6, 0);
	Work_SetValuesIfNonNegative(-1, -1, 0xe666);
	Engine_EventWait(80);
	Event_PlayCue(0x121);
	Engine_EventWait(40);
	Engine_ActorStop(ACTOR_PARTY_LEADER);
	Engine_ActorStop(GERALD);
	Engine_ActorStop(IWAN);
	Engine_ActorStop(MEARI);
	Engine_EventWait(120);
	Actor_ShowEmote(IWAN, 0x105, 120);
	Engine_ActorRunRepeatedMotion(IWAN, 1);
	Actor_FaceDirection(IWAN, FACING_NORTHWEST, 20);
	VinasuChojo_ShowMessage(IWAN);
	Engine_ActorSetAnimationAndWait(GERALD, 3);
	Actor_FaceDirection(GERALD, FACING_SOUTHEAST, 0);
	VinasuChojo_ShowMessage(GERALD);
	Actor_FaceDirection(MEARI, FACING_NORTHEAST, 20);
	Engine_ActorSetAnimation(MEARI, 4);
	VinasuChojo_ShowMessage(MEARI);
	Engine_ActorJump(IWAN, 2, 20);
	VinasuChojo_ShowMessage(IWAN);
	Engine_ActorSetAnimation(GERALD, 3);
	Engine_ActorSetAnimation(IWAN, 3);
	Engine_ActorSetAnimationAndWait(MEARI, 3);
	Engine_EventWait(20);
	VinasuChojo_FaceActor(GERALD, FACING_EAST);
	VinasuChojo_ShowMessage(GERALD);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 20);
	Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
	Engine_EventWait(20);
	Actor_SetSpeed(GERALD, 0x10000, 0x8000);
	Actor_SetSpeed(IWAN, 0x10000, 0x8000);
	Actor_SetSpeed(MEARI, 0x10000, 0x8000);
	Engine_ActorEnableActionCallback(GERALD, VinasuChojo_PartyExitAction);
	Engine_ActorEnableActionCallback(IWAN, VinasuChojo_PartyExitAction);
	Engine_ActorEnableActionCallback(MEARI, VinasuChojo_PartyExitAction);
	Engine_EventWait(60);
	Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
	Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 272, 216);
	Actor_WalkTo(ACTOR_PARTY_LEADER, 272, 254);
	Engine_EventWait(80);
	((struct EventWork *)WORK_SLOT(HEAP_SLOT_EVENT))->start_transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, 1);
	((struct EventWork *)WORK_SLOT(HEAP_SLOT_EVENT))->transition_frames = 16;
	Engine_EventCloseScreen();
	Engine_EventWaitForScreen();
	Engine_EventWait(80);
	Graphics_EnableObjLayerAndCallbacks();
	((struct UiRenderWork *)WORK_SLOT(HEAP_SLOT_WINDOW))->result[0] = 0;
	((struct UiRenderWork *)WORK_SLOT(HEAP_SLOT_WINDOW))->result[1] = 0;
	UiText_ShowCenteredMessage((s32)MsgVinasuDespiteLongTiring, 0, 0);
	ObjectDispatch_StopCallbacksAndHideLayers();
	Engine_EventWait(80);
}

void SceneActor_ParkRecord(struct FieldActor *actor)
{
	actor->target_x = ACTOR_NO_TARGET;
	actor->target_y = ACTOR_NO_TARGET;
	actor->target_z = ACTOR_NO_TARGET;

	actor->velocity_x = 0;
	actor->velocity_y = 0;
	actor->velocity_z = 0;

	actor->timer = 0;
}

void VinasuChojo_RunTransitionStep(void)
{
	struct FieldActor  *center;
	struct FieldActor  *effect;
	struct FieldSprite *sprite;
	s32					spawn;
	u32					rise;
	s32					angle;

	center = Actor_Get(23);
	spawn = 0;
	switch (VinasuChojo_TransitionStep)
	{
	case 0:
		Event_PlayCue(220);
		Work_SetValuesIfNonNegative(0x60000, 0x60000, 0x10000);
		ColorBuffer_ApplyTarget(0x2063ff, 1);
		Engine_ColorBufferInterpolate(8);
		break;
	case 8:
		ColorBuffer_ApplyTarget(0x10000, 1);
		Engine_ColorBufferInterpolate(8);
		break;
	case 16:
		Work_SetValuesIfNonNegative(0x10000, 0x10000, 0x10000);
		break;
	case 24:
		center->x.fixed = 152 << 17;
		center->y.fixed = -0x1680000;
		center->z.fixed = 164 << 16;
		center->scale_x = 0x10000;
		center->scale_y = 0x10000;
		SceneActor_ParkRecord(center);
		Engine_ActorEnableActionCallback(23, VinasuChojo_RiseActionScript);
		break;
	case 25:
		VinasuChojo_TransitionStep--;
		if (center->y.fixed > 0)
		{
			ColorBuffer_ApplyTarget(0x203210, 0);
			Engine_ColorBufferInterpolate(16);
			VinasuChojo_TransitionStep++;
			Actor_Get(0)->rise_counter = 1;
			Actor_Get(1)->rise_counter = 1;
			Actor_Get(2)->rise_counter = 1;
			Actor_Get(3)->rise_counter = 1;
			Actor_Get(21)->rise_counter = 1;
			Actor_Get(6)->rise_counter = 1;
		}
		else
		{
			if ((gFrameCount & 7) == 0)
				Event_PlayCue(246);
			spawn = 1;
			center->y.fixed += 0x24000;
		}
		break;
	case 26:
		VinasuChojo_TransitionStep--;
		if (center->y.fixed > (160 << 14))
		{
			ColorBuffer_ApplyTarget(0x10000, 0);
			Engine_ColorBufferInterpolate(40);
			VinasuChojo_TransitionStep++;
		}
		else
		{
			if ((gFrameCount & 7) == 0)
				Event_PlayCue(246);
			spawn = 1;
			center->y.fixed += 0x24000;
		}
		break;
	case 27:
	case 28:
	case 29:
	case 30:
	case 31:
	case 32:
	case 33:
	case 34:
		spawn = 1;
		break;
	case 36:
		Event_PlayCue(187);
		ColorBuffer_ApplyTarget(0x7fff, 0);
		Engine_ColorBufferInterpolate(12);
		break;
	case 48:
		Engine_ActorStop(23);
		GameFlag_Set(0x237);
		break;
	}
	if (spawn)
	{
		rise = ((u32)(Random_Next() * 80) >> 16) << 16;
		effect = Object_Create(
			284, center->x.fixed, center->y.fixed - rise + 0xfff80000, center->z.fixed);
		if (effect != 0)
		{
			sprite = effect->sprite;
			Object_SetScript(effect, VinasuChojo_RiseParticleScript);
			ObjectGroup_SetChildValue(effect, 1);
			effect->motion_flags = 0;
			angle = Random_Next() & 0xffff000;
			effect->timer = angle;
			effect->work = 0;
			effect->rise_counter = (u32)Random_Next() >> 13;
			effect->update = (void (*)(union FieldObject *))SceneEffect_UpdateCounterDrivenOrbit;
			effect->speed = Engine_MathSin((u32)(Random_Next() * 0xffff) >> 20) * 24;
			effect->speed = center->speed >> 16;
			sprite->flags = 0;
			sprite->priority = 1;
		}
	}
	VinasuChojo_TransitionStep++;
	SceneEffect_AdvanceGatedRiseCounter(Actor_Get(0));
	SceneEffect_AdvanceGatedRiseCounter(Actor_Get(1));
	SceneEffect_AdvanceGatedRiseCounter(Actor_Get(2));
	SceneEffect_AdvanceGatedRiseCounter(Actor_Get(3));
	SceneEffect_AdvanceGatedRiseCounter(Actor_Get(21));
	SceneEffect_AdvanceGatedRiseCounter(Actor_Get(6));
}

void SceneEffect_AdvanceGatedRiseCounter(struct FieldActor *actor)
{
	if (actor->rise_enabled != 0)
	{
		u8 counter = actor->rise_counter;

		actor->y.fixed = actor->rise_base_y + ((counter >> 2) << 16);

		SceneActor_ParkRecord(actor);

		if (actor->rise_counter != 0)
		{
			if (actor->rise_counter <= 31)
			{
				++actor->rise_counter;
			}
		}
	}
}

void SceneEffect_SpawnParticlesBesideActor(void)
{
	struct FieldActor	 *center;
	struct FieldActor	 *effect;
	struct FieldSprite	 *sprite;
	struct MapScrollWork *map;
	s32					  drift;
	s32					  phase;
	s32					  angle;

	center = Actor_Get(23);
	map = gMapWork[0];
	drift = (u32)(Random_Next() * 48) >> 16 << 16;
	if ((s16)(map->view_y >> 16) <= 129)
	{
		if (gFrameCount & 1)
		{
			Actor_SetPosition(23, 152 << 17, 164 << 16);
			Actor_Get(23)->scale_x = 0x10000;
			Actor_Get(23)->scale_y = 0x10000;
		}
		else
		{
			Actor_SetPosition(23, 152 << 17, 171 << 16);
			Actor_Get(23)->scale_x = 0x14ccc;
			Actor_Get(23)->scale_y = 0x14ccc;
		}
	}
	else
	{
		Actor_SetPosition(23, 0, 0);
	}
	if (center == 0)
		return;
	phase = gFrameCount & 15;
	if (phase != 0)
		return;
	effect = Object_Create(
		284, center->x.fixed + 0x80000, center->y.fixed + drift + 0x80000, center->z.fixed);
	drift /= 0x60000;
	drift <<= 16;
	if (effect == 0)
		return;
	sprite = effect->sprite;
	Object_SetScript(effect, VinasuChojo_BesideParticleScript);
	ObjectGroup_SetChildValue(effect, 5);
	effect->motion_flags = phase;
	angle = Random_Next() & 0xffff000;
	effect->timer = angle;
	effect->work = phase;
	effect->linked_object = center;
	effect->update = VinasuChojo_UpdateOrbitingEffect;
	effect->speed = Engine_MathSin((drift & 0xfffff) >> 4) * 24 >> 16;
	/* FAKEMATCH: the flag byte is stored through a byte pointer, which may alias the priority read after it; as a plain field store the read's address is set up one instruction earlier. Field store: differ 2; it matches when center->sprite is read through union FieldObject. */
	(&sprite->flags)[0] = 0;
	sprite->priority = center->sprite->priority;
}

void VinasuChojo_UpdateOrbitingEffect(union FieldObject *object)
{
	struct FieldEffect *anchor;
	s32					spin;

	anchor = (struct FieldEffect *)object->actor.linked_object;
	spin = object->effect.spin;
	object->effect.x = anchor->x + Engine_MathCos(spin) * (object->actor.speed + 28);
	object->effect.z = (Engine_MathSin(spin) << 4) + 0xa40000;
	object->actor.target_x = object->effect.x;
	object->actor.target_z = object->effect.z;
	object->effect.spin -= 0x200;
}

s32 gEffectScript0[] = {
	SC_WAIT(1),
	SC_LOOP(22, SC_TOP),
	SC_WAIT(1),
	SC_POS(0, 0, 0),
	SC_WAIT(1),
	SC_FREE,
};

s32 gEffectScript1[] = {
	SC_WAIT(1),
	SC_LOOP(44, SC_TOP),
	SC_WAIT(1),
	SC_POS(0, 0, 0),
	SC_WAIT(1),
	SC_FREE,
};

s32 gEffectScript2[] = {
	SC_WAIT(1),
	SC_LOOP(126, SC_TOP),
	SC_WAIT(1),
	SC_POS(0, 0, 0),
	SC_WAIT(1),
	SC_FREE,
};

const s32 *gEffectScripts[] = {
	gEffectScript0,
	gEffectScript1,
	gEffectScript2,
};

s32 VinasuChojo_PairActionScript[] = {
	SC_ARG(2),
	SC_MOVE(PIXELS(492), 0, PIXELS(164)),
	SC_SYNC,
	SC_ARG(1),
	SC_POS(0, 0, 0),
	SC_WAIT(1),
	SC_END,
};

s32 SceneAction_EntryGroup[] = {
	SC_SET(SW_SPEED, 0x3333),
	SC_SET(SW_ACCEL, 0x1999),
	SC_ARG(2),
	SC_MOVE_BY(PIXELS(-8), 0, 0),
	SC_SYNC,
	SC_END,
};

s32 SceneAction_EntryPair[] = {
	SC_SET(SW_SPEED, 0x3333),
	SC_SET(SW_ACCEL, 0x1999),
	SC_ARG(2),
	SC_MOVE_BY(PIXELS(8), 0, 0),
	SC_SYNC,
	SC_END,
};

s32 VinasuChojo_ScaleStepAction[] = {
	SC_CALL(OverlayObject_StepScaleByCounter),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

s32 VinasuChojo_BeamOpenAction[] = {
	SC_ADD(SW_SCALE_X, 0x1000),
	SC_WAIT(1),
	SC_LOOP(32, SC_TOP),
	SC_END,
};

s32 Data_020060ac[] = {
	SC_ADD(SW_SCALE_X, -896),
	SC_WAIT(1),
	SC_LOOP(140, SC_TOP),
	SC_END,
};

s32 VinasuChojo_BeamRiseAction[] = {
	SC_ADD(SW_SCALE_X, -896),
	SC_WAIT(1),
	SC_LOOP(160, SC_TOP),
	SC_END,
};

s32 VinasuChojo_BeamCloseAction[] = {
	SC_ADD(SW_Y, PIXELS(4)),
	SC_WAIT(1),
	SC_LOOP(30, SC_TOP),
	SC_POS(0, 0, 0),
	SC_WAIT(1),
	SC_END,
};

s32 VinasuChojo_BeamEndAction[] = {
	SC_ADD(SW_Y, PIXELS(2)),
	SC_WAIT(1),
	SC_LOOP(160, SC_TOP),
	SC_POS(0, 0, 0),
	SC_WAIT(1),
	SC_END,
};

s32 VinasuChojo_OrbitParticleScript[] = {
	SC_SET(SW_SCALE_X, 0xcccc),
	SC_SET(SW_SCALE_Y, 0xcccc),
	SC_ARG(1),
	SC_LABEL(1),
	SC_ADD(SW_Y, PIXELS(1)),
	SC_WAIT(1),
	SC_LOOP(16, SC_TO(1)),
	SC_POS(0, 0, 0),
	SC_WAIT(1),
	SC_FREE,
};

s32 VinasuChojo_RiseParticleScript[] = {
	SC_SET(SW_SCALE_X, 0xcccc),
	SC_SET(SW_SCALE_Y, 0xcccc),
	SC_ARG(1),
	SC_LABEL(1),
	SC_ADD(SW_Y, PIXELS(-1)),
	SC_WAIT(1),
	SC_LOOP(16, SC_TO(1)),
	SC_POS(0, 0, 0),
	SC_WAIT(1),
	SC_FREE,
};

s32 Data_0200622c[] = {
	SC_ARG(2),
	SC_MOVE(PIXELS(268), 0, PIXELS(186)),
	SC_SYNC,
	SC_MOVE(PIXELS(268), 0, PIXELS(252)),
	SC_SYNC,
	SC_MOVE(PIXELS(232), 0, PIXELS(222)),
	SC_SYNC,
	SC_MOVE(PIXELS(232), 0, PIXELS(136)),
	SC_SYNC,
	SC_MOVE(PIXELS(200), 0, PIXELS(136)),
	SC_SYNC,
	SC_MOVE(PIXELS(200), 0, PIXELS(200)),
	SC_SYNC,
	SC_ARG(1),
	SC_POS(0, 0, 0),
	SC_WAIT(1),
	SC_END,
};

s32 VinasuChojo_RiseActionScript[] = {
	SC_LABEL(1),
	SC_ADD(SW_SCALE_X, -1536),
	SC_WAIT(1),
	SC_LOOP(4, SC_TO(1)),
	SC_LABEL(2),
	SC_ADD(SW_SCALE_X, 1536),
	SC_WAIT(1),
	SC_LOOP(4, SC_TO(2)),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

s32 VinasuChojo_StepBackAction[] = {
	SC_ADD(SW_Y, -0xc000),
	SC_WAIT(1),
	SC_LOOP(120, SC_TOP),
	SC_POS(0, 0, 0),
	SC_WAIT(1),
	SC_END,
};

s32 VinasuChojo_LeapAction[] = {
	SC_ADD(SW_Y, -0x48000),
	SC_WAIT(1),
	SC_LOOP(80, SC_TOP),
	SC_POS(0, 0, 0),
	SC_WAIT(1),
	SC_END,
};

s32 VinasuChojo_PartyExitAction[] = {
	SC_ARG(2),
	SC_MOVE(PIXELS(330), 0, PIXELS(173)),
	SC_SYNC,
	SC_FREE,
	SC_END,
};

s32 VinasuChojo_PartyHeadingAction[] = {
	SC_CALL(OverlayObject_UpdateHeadingTimer),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

struct SceneEntrance gVinasuChojoEntrances[] = {
	{ 0, CONDITION_ALWAYS, 304, 0, 216, FACING_SOUTHEAST + FACING_STEP, 0, 0, 0, 600, 420, 0 },
	{ 1, CONDITION_ALWAYS, 504, 0, 168, FACING_SOUTHEAST + FACING_STEP, 0, 0, 0, 600, 420, 0 },
	{ 2, CONDITION_ALWAYS, 340, 0, 184, FACING_WEST, 0, 0, 0, 600, 420, 0 },
	{ 3, CONDITION_ALWAYS, 340, 0, 184, FACING_WEST, 0, 0, 0, 600, 420, 0 },
	{ 9, CONDITION_ALWAYS, 0, 0, 0, FACING_WEST, 0, 0, 0, 600, 420, 0 },
	{ SCENE_TABLE_END },
};

u32 gVinasuChojoExits[] = {
	SCENE_EXITS(187),
	SCENE_EXIT(1, 2, 73),
	SCENE_EXIT(2, 180, 99),
	SCENE_EXIT(9, 179, 12),
	SCENE_EXITS_END,
};

struct ScenePlacement gVinasuChojoPlacements[] = {
	{ 0x102, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(248), 0, PIXELS(136), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x102, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(280), 0, PIXELS(104), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0x102, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(328), 0, PIXELS(104), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0x102, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(360), 0, PIXELS(136), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0xf2, CONDITION_ALWAYS, 7,
		PIXELS(440), 0, PIXELS(252), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xf2, CONDITION_ALWAYS, 7,
		PIXELS(472), 0, PIXELS(252), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0xf2, CONDITION_ALWAYS, 7,
		PIXELS(504), 0, PIXELS(252), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0xf2, CONDITION_ALWAYS, 7,
		PIXELS(104), 0, PIXELS(252), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0xf2, CONDITION_ALWAYS, 7,
		PIXELS(136), 0, PIXELS(252), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0xf2, CONDITION_ALWAYS, 7,
		PIXELS(168), 0, PIXELS(252), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0xf2, CONDITION_ALWAYS, 7,
		0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ GERALD, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(504), 0, PIXELS(148), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ IWAN, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(504), 0, PIXELS(148), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ MEARI, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(504), 0, PIXELS(148), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x21, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(280), 0, PIXELS(164), FACING_NORTH + FACING_STEP, TALK_KEEP_FACING, 0 },
	{ 0x20, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(296), 0, PIXELS(180), FACING_NORTHWEST + FACING_STEP, TALK_KEEP_FACING, 0 },
	{ 0x6, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(248), 0, PIXELS(172), FACING_EAST, TALK_KEEP_FACING, 0 },
	{ 0x23, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(264), 0, PIXELS(252), FACING_NORTH + FACING_STEP, TALK_KEEP_FACING, 0 },
	{ 0x16, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xf4, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xe5, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xe5, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0xe5, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0xe5, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0xe5, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ SCENE_TABLE_END },
};

s32 VinasuChojo_NearestActor = 0;

struct SceneEvent gVinasuChojoEvents[] = {
	{ 0x202, 11, CONDITION_ALWAYS, EVENT_SCRIPT(InitializeActorZeroMotion) },
	{ EVENT_TOUCH, 11, CONDITION_ALWAYS, EVENT_SCRIPT(VinasuChojo_ConfigureGroupMotion) },
	{ 0x202, 10, CONDITION_ALWAYS, EVENT_SCRIPT(InitializeActorZeroMotion) },
	{ EVENT_TOUCH, 10, 0x250, EVENT_SCRIPT(SceneEffect_SpawnAndBobWithActorZero) },
	{ EVENT_TOUCH, 5, CONDITION_ALWAYS, EVENT_SCRIPT(Scene_RunActorEntrySequence) },
	{ SCENE_EVENTS_END },
};

s32 VinasuChojo_BesideParticleScript[] = {
	SC_SET(SW_SCALE_Y, 0x16666),
	SC_SET(SW_SCALE_X, 0x16666),
	SC_ARG(4),
	SC_WAIT(30),
	SC_FREE,
};
