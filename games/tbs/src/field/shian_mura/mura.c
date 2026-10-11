#include "game_state.h"
#include "unit/import.h"
#include "unit/field/shian_mura/mura.h"
#include "unit/field/common/effect/move.h"
#include "unit/battle/effect/slots.h"
#include "unit/field/common/script/script.h"
#include "unit/system/scheduler/callback.h"

#define NULL ((void *)0)
#include "fieldobj.h"
#include "types.h"
#include "field_event.h"
#include "field_scene.h"
#include "scene_effect_sequence.h"
#include "iwram_call.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "call.h"
#include "field_service.h"
#include "text/msg_ids.h"
#include "ability_ids.h"
#include "item_ids.h"
#include "objcmd.h"

struct PeriodicEffectConfig
{
	s32 kind;
	s32 variant;
	u8	pad_08[16];
	u16 id;
	u8	pad_1a[2];
	u8 *data;
	u8	pad_20[8];
};

void Effect_Spawn();

void SetEffectRecordMode(struct FieldActor *work, s32 mode)
{
	work->sprite->priority = mode;
}

void *SceneEffect_SpawnPrimary(s32 x, s32 y, s32 z, s32 kind)
{
	struct FieldActor *effect = Engine_ObjectCreate(kind, x, y, z);

	if (effect != NULL)
	{
		struct FieldSprite *sprite = effect->sprite;

		sprite->priority = 0;
		effect->motion_flags = 0;
		effect->collision_flags = 8;
		Engine_ActorSetSpriteFlags(effect, 0);
		ObjectGroup_SetChildValue(effect, 14);
		Engine_ObjectSetBlendMode(effect, 1);
		return effect;
	}
	return NULL;
}

void *SceneEffect_SpawnSecondary(s32 x, s32 y, s32 z, s32 kind)
{
	struct FieldActor *effect = Engine_ObjectCreate(kind, x, y, z);

	if (effect != NULL)
	{
		struct FieldSprite *sprite = effect->sprite;

		sprite->priority = 1;
		effect->motion_flags = 0;
		effect->collision_flags = 8;
		Engine_ActorSetSpriteFlags(effect, 0);
		ObjectGroup_SetChildValue(effect, 15);
		effect->priority_flags = (effect->priority_flags & 0xfe) | 2;
		return effect;
	}
	return NULL;
}

// ここに共通ソースが入る（ROM の並びのとおり）
#include "../common/effect/move.c"
#include "../common/effect/spawnov.c"

#define FLAG_SHIAN_WATER_SPILLED 0x200

struct SceneState
{
	u8	unk_00[14];
	s16 field_0e;
};

struct Actor
{
	u8 unk_00[35];
	u8 presentation_flags;
};

struct MapActor
{
	u8	reserved_00[6];
	u16 heading;
	s32 x;
	u8	reserved_0c[4];
	s32 z;
	u8	reserved_14[71];
	u8	tracking_state;
	u8	reserved_5c[8];
	s16 tracking_mode;
};

struct SpawnParams
{
	s32 count;
	u8	unknown_04[20];
	u16 sprite;
	u16 pad_1a;
	s32 callback;
	u16 facing;
	u8	unknown_22[6];
};

void SceneEffect_RequestFixedEffect(void)
{
	BattleFx_RunPageEffectForSlot(22, 1, 2);
}

void Actor_UpdatePresentationFlag(void)
{
	struct SceneState *scene = (void *)Object_GetById(0);
	if (scene->field_0e > 31)
	{
		((struct Actor *)Object_GetById(20))->presentation_flags |= 2;
	}
	else
	{
		((struct Actor *)Object_GetById(20))->presentation_flags &= ~2;
	}
}

void SceneEffect_AdvanceRotatingSprite(void)
{
	u8 *entity = Object_GetById(19);
	u8 *sprite = (u8 *)((struct FieldActor *)entity)->sprite;
	((struct FieldSprite *)sprite)->rotation += 0x1400;
}

void SceneEffect_SpawnPeriodicEffect(void)
{
	u8 *entity = Object_GetById(14);

	if ((gFrameCount & 3) == 0)
	{
		struct PeriodicEffectConfig config;
		config.kind = 1;
		config.variant = 9;
		config.id = 169;
		config.data = ShianMura_EffectConfig;
		Effect_Spawn(*(s32 *)(entity + 8), *(s32 *)(entity + 12), *(s32 *)(entity + 16) - 0x10000,
			0, -0x10000, -0x10000, 0x330000, &config);
	}
}

s32 SceneEffect_CalculatePositionDistance(s32 *first_position, s32 *second_position)
{
	s32 delta_x = (*first_position++ - *second_position++) >> 16;
	s32 delta_y = (*first_position++ - *second_position++) >> 16;
	s32 delta_z = (*first_position - *second_position) >> 16;
	s32 delta_x_squared = delta_x * delta_x;
	s32 delta_y_squared = delta_y * delta_y;
	s32 delta_z_squared = delta_z * delta_z;

	return Iwram_Sqrt(delta_x_squared + delta_y_squared + delta_z_squared);
}

s32 SceneActor_UpdateTracking(
	struct MapActor *actor, struct MapActor *target, s32 distance_limit, s32 force_tracking)
{
	s32 result = 0;

	if (actor->tracking_state == 1 && actor->tracking_mode == 0)
	{
		Object_SetMode(actor, 1);
		return 1;
	}

	if (SceneEffect_CalculatePositionDistance(&target->x, &actor->x) < distance_limit
		|| force_tracking != 0)
	{
		u16 angle = ArcTan2(target->z - actor->z, target->x - actor->x);
		s32 prev = (angle - 0x1000) & 0xf000;
		s32 next = (angle + 0x1000) & 0xf000;
		s32 facing = angle & 0xf000;
		s32 heading = actor->heading & 0xf000;

		if (facing == heading || next == heading || prev == heading || force_tracking != 0)
		{
			actor->tracking_state = 1;
			Object_SetMode(actor, 1);
			result = 1;
			actor->tracking_mode = result;
		}
		else
		{
			actor->tracking_state = 0;
			Object_SetMode(actor, 2);
			actor->tracking_mode = 0;
		}
	}
	else
	{
		actor->tracking_state = 0;
		Object_SetMode(actor, 2);
		actor->tracking_mode = 0;
	}

	return result;
}

s32 ShianMura_WatchGateTrigger(struct FieldActor *self)
{
	struct FieldActor *leader;
	s32				   near = 0;

	if (self->target_x == ACTOR_NO_TARGET && self->target_z == ACTOR_NO_TARGET)
		return 0;
	leader = Object_GetById(0);
	if ((u32)((leader->x.fixed >> 20) - 17) <= 1 && leader->z.fixed >> 20 == 14
		&& self->x.fixed >> 20 <= 19 && self->velocity_x <= 0)
	{
		if (leader->x.fixed <= self->x.fixed)
		{
			self->rise_counter++;
			near = 1;
		}
	}
	else
	{
		self->rise_counter = 0;
	}
	if (near && self->rise_counter > 119)
	{
		gEventWork->raised_trigger = 200;
		self->rise_counter = 0;
	}
	SceneActor_UpdateTracking(self, leader, 18, near);
	return 0;
}

u8 *SceneEffect_GetPrimaryData(void)
{
	return gShianMuraEntrances;
}

s32 SceneEffect_GetInitialValue(void)
{
	return 0;
}

u8 *SceneEffect_GetSecondaryData(void)
{
	return gShianMuraExits;
}

s32 SceneEffect_PrepareState(void)
{
	if (Engine_GameFlagIsSet(0x895) != 0)
		gShianMuraPlacements[7].talk_facing = 0;
	return (s32)gShianMuraPlacements;
}

void SceneEffect_ShowActorSetupMessage(void)
{
	Engine_EventBegin();
	Engine_EventSetMessage((s32)MsgShianYoungWarriorsVeryGallantCame);
	Engine_EventAskYesNo(9, 0);
	Engine_EventEnd();
}

void FieldScene_RunPrimarySequence(void)
{
	struct FieldActor *puddle;
	struct FieldActor *record;
	struct FieldActor *leader;
	s32				   moved;
	s32				   facing;
	s32				   x;
	s32				   z;
	s32				   column;
	s32				   scale_y;
	s32				   scale_x;

	puddle = Object_GetById(20);
	Engine_EventBegin();
	moved = 0;
	record = Actor_Get(18);
	record->update = NULL;
	if (GameFlag_IsSet(FLAG_SHIAN_WATER_SPILLED) != 0 || (Object_GetById(18)->x.fixed >> 20) <= 19)
	{
		record = Actor_Get(18);
		facing = record->facing;
		Actor_FaceActor(18, ACTOR_PARTY_LEADER, 0);
		Engine_EventWait(10);
		Engine_EventSetMessage((s32)MsgShianNowMustGetWaterAgain);
		if (GameFlag_IsSet(FLAG_SHIAN_WATER_SPILLED) == 0)
		{
			gEventWork->message++;
			Event_ShowMessage(18, 0);
			Object_GetById(18)->timer = 0;
			record = Object_GetById(18);
			record->facing = facing;
		}
		else
		{
			Event_ShowMessage(18, 0);
			Actor_FaceDirection(18, 0x8000, 20);
		}
		record = Actor_Get(18);
		record->update = (void *)ShianMura_WatchGateTrigger;
		Engine_EventEnd();
		return;
	}
	if ((Object_GetById(ACTOR_PARTY_LEADER)->z.fixed >> 19) > 27
		&& (Object_GetById(ACTOR_PARTY_LEADER)->z.fixed >> 19) <= 29
		&& (Object_GetById(ACTOR_PARTY_LEADER)->x.fixed >> 20) != 26)
	{
		Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x8000, 0x4000);
		Engine_ActorFaceActor(0, 18, 0);
		Engine_EventWait(5);
		leader = Object_GetById(ACTOR_PARTY_LEADER);
		record = Actor_Get(18);
		if (leader->x.fixed < record->x.fixed)
		{
			Object_GetById(0)->steering_flags &= ~ACTOR_FACES_MOTION;
			record = Object_GetById(18);
			Actor_WalkTo(ACTOR_PARTY_LEADER, ((record->x.fixed >> 20) << 4) - 8, 232);
			moved = 1;
		}
		else
		{
			Object_GetById(0)->steering_flags &= ~ACTOR_FACES_MOTION;
			record = Object_GetById(18);
			Actor_WalkTo(ACTOR_PARTY_LEADER, ((record->x.fixed >> 20) << 4) + 24, 232);
		}
		Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
	}
	record = Actor_Get(18);
	record->target_x = ACTOR_NO_TARGET;
	record = Object_GetById(18);
	record->target_y = ACTOR_NO_TARGET;
	record = Actor_Get(18);
	record->target_z = ACTOR_NO_TARGET;
	Engine_ActorEnableActionCallback(18, 1);
	Engine_ActorSetAnimation(18, 1);
	Engine_ActorRunRepeatedMotion(18, 2);
	Engine_EventWait(10);
	Event_PlayCue(228);
	puddle->scale_x = 0x4ccc;
	puddle->scale_y = 0x4ccc;
	record = Object_GetById(18);
	x = record->x.fixed;
	record = Object_GetById(18);
	z = record->z.fixed >> 20;
	Actor_SetPosition(20, ((x >> 20) << 20) + (8 << 16), (z << 20) + (8 << 16));
	record = Object_GetById(18);
	column = record->x.fixed;
	record = Object_GetById(18);
	Map_CopyCellAttributes(16, 16, 1, 1, column >> 20, record->z.fixed >> 20);
	Engine_ActorSetSpritePriority(20, 2);
	puddle->priority_flags |= ACTOR_PRIORITY_UNDERFOOT;
	do
	{
		Engine_TaskWait(3);
		scale_y = puddle->scale_y;
		scale_x = puddle->scale_x;
		puddle->scale_y = scale_y + 0x1999;
		scale_x += 0x1999;
		puddle->scale_x = scale_x;
	} while (scale_x <= 0xffff);
	Actor_ShowEmote(18, 0x105, 70);
	Actor_FaceActor(18, ACTOR_PARTY_LEADER, 0);
	Engine_EventWait(20);
	Actor_ShowEmote(18, 0x103, 0);
	Engine_ActorStartRepeatedMotion(18, 2);
	Engine_EventWait(70);
	Engine_EventSetMessage((s32)MsgShianDoingMadeMeSpillMy);
	Event_ShowMessageAndWait(18, 0, 20);
	BattleFx_PlayQueuedSound();
	if ((Object_GetById(ACTOR_PARTY_LEADER)->x.fixed >> 20) == 26
		&& (Object_GetById(ACTOR_PARTY_LEADER)->z.fixed >> 20) > 13)
	{
		moved = 1;
	}
	if (moved != 0)
	{
		Actor_SetSpeed(ACTOR_PARTY_LEADER, 0xcccc, 0x6666);
		Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 10);
		Object_GetById(0)->steering_flags &= ~ACTOR_FACES_MOTION;
		Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 2);
		Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, 16);
		Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
		Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 1);
	}
	Actor_SetSpeed(18, 0xcccc, 0x6666);
	record = Object_GetById(18);
	if ((record->z.fixed >> 20) != 14)
	{
		record = Actor_Get(18);
		Actor_WalkToAndWait(18, record->x.part.pixel, 232);
	}
	Actor_WalkToAndWait(18, 0x118, 232);
	GameFlag_Set(FLAG_SHIAN_WATER_SPILLED);
	Actor_Get(ACTOR_PARTY_LEADER)->steering_flags |= ACTOR_FACES_MOTION;
	Engine_EventEnd();
}

void SceneEffect_ActivateNearbyActor(void)
{
	u8 *leader = Object_GetById(ACTOR_PARTY_LEADER);
	if ((*(s32 *)(leader + 16) >> 20) <= 13)
		Engine_ActorSetSpritePriority(20, 1);
}

void ShianMura_ActivateActorTwentyPlatform(void)
{
	u32 i;
	s32 record;
	s32 v5;
	s32 x;

	Engine_EventBegin();
	Object_GetById(20)->priority_flags &= 253;
	v5 = 0;
	Object_GetById(20)->motion_flags = v5;
	record = Object_GetById(20);
	x = *(s32 *)(record + 8);
	record = Object_GetById(20);
	Map_CopyCellAttributes(3, 17, 1, 1, (x >> 20), (*(s32 *)(record + 16) >> 20));
	Engine_TaskAddCallback((s32)Actor_UpdatePresentationFlag, 0xc80);
	GameFlag_Set(0x201);
	Engine_ActorSetSpritePriority(20, 2);
	Engine_EventEnd();
}

void ShianMura_RunNpcMeetScene(void)
{
	s32				   flag;
	struct FieldActor *leader;
	s32				   cb;
	s32				   back;
	s32				   pos[3];
	struct SpawnParams params;

	Engine_EventBegin();
	flag = Engine_GameFlagIsSet(0x202);
	if (flag != 0)
	{
		Engine_ActorFaceActor(14, 0, 0);
		Engine_EventWait(10);
		Engine_EventSetMessage((s32)MsgShianKungFuStrong);
		Engine_EventShowMessage(14, 0);
		Engine_ActorFaceDirection(14, 0, 10);
		Engine_EventEnd();
	}
	else
	{
		Engine_EventSetMessage((s32)MsgShianHooWhaaaHachaaa);
		Engine_EventShowMessage(14, 0);
		Engine_ActorRunRepeatedMotion(0, 2);
		Object_GetById(0)->steering_flags &= ~ACTOR_FACES_MOTION;
		pos[0] = flag;
		pos[1] = flag;
		pos[2] = flag;
		leader = Object_GetById(0);
		FieldVector_AddPolarOffset(-0x80000, leader->facing, (struct FieldPosition *)pos);
		Engine_ActorSetAnimation(0, 2);
		Engine_ActorSetDestinationOffset(0, pos[0] / 0x10000, pos[2] / 0x10000);
		Engine_ActorWaitForMove(0);
		Object_GetById(0)->steering_flags |= ACTOR_FACES_MOTION;
		Engine_EventWait(30);
		Engine_ActorRunRepeatedMotion(14, 2);
		params.count = 1;
		Effect_Spawn(0xc00000, 0, 0x1380000, 0x1999, 0x3333, 0, 0x20001, 0);
		Effect_Spawn(0xc00000, 0, 0x1380000, 0x3333, 0x1999, 0, 0x20001, 0);
		Engine_AudioPlayCue(132);
		cb = (s32)SceneEffect_SpawnPeriodicEffect;
		Task_AddCallback((void (*)(void))cb, 0xc80);
		Object_GetById(14)->velocity_y = 0x60000;
		Object_GetById(14)->gravity = 0x10000;
		Object_GetById(14)->bounce = 0;
		Actor_SetSpeed(14, 0x30000, 0x18000);
		Actor_SetDestination(14, 168, 0x138);
		Engine_ActorWaitForMove(14);
		Engine_AudioPlayCue(134);
		Engine_ActorEnableActionCallback(19, (s32)ShianMura_Actor19Motion);
		Engine_TaskAddCallback((s32)SceneEffect_AdvanceRotatingSprite, 0xc80);
		params.sprite = 0x11b;
		params.callback = (s32)ShianMura_EffectScript;
		params.facing = 0x4000;
		Effect_Spawn(0xa80000, 0, 0x14c0000, 0, 0, 0, 0x720000, &params);
		Actor_SetDestination(14, 146, 0x138);
		Engine_ActorWaitForMove(14);
		Engine_TaskRemoveCallback((void (*)(void))cb);
		Effect_Spawn(0x900000, 0, 0x1380000, 0, 0, 0, 0x20001, 0);
		Effect_Spawn(0x900000, 0, 0x1380000, -0x3333, 0x1999, 0, 0x20001, 0);
		back = -0x8000;
		Effect_Spawn(0x900000, 0, 0x1380000, back, 0, 0, 0x20001, 0);
		Object_RefreshSelectorById(19);
		Engine_AudioPlayCue(124);
		Effect_Spawn(0xa80000, 0x80000, 0x1380000, 0, 0, 0, 0x20001, 0);
		Effect_Spawn(0xa80000, 0x80000, 0x1380000, 0x3333, 0, 0, 0x20001, 0);
		Effect_Spawn(0xa80000, 0x80000, 0x1380000, -0x3333, 0, 0, 0x20001, 0);
		Engine_TaskRemoveCallback(SceneEffect_AdvanceRotatingSprite);
		Object_GetById(19)->sprite->rotation = back;
		Object_GetById(14)->bounce = 0x4000;
		Object_GetById(14)->gravity = 0x10000;
		Engine_EventWait(30);
		Engine_ActorFaceDirection(14, 0, 20);
		Engine_ActorRunRepeatedMotion(14, 2);
		Engine_EventWait(20);
		Engine_EventShowMessageAndWait(14, 0, 20);
		Engine_GameFlagSet(0x202);
		BattleFx_PlayQueuedSound();
		Engine_EventEnd();
	}
}

void SceneEffect_RunActorSceneMessage(void)
{
	Engine_EventBegin();
	Engine_EventSetMessage((s32)MsgShianWarriorsFromSchoolStrongWarriors);
	Engine_EventAskYesNo(17, 0);
	Engine_EventEnd();
}

void ShianMura_HopLeaderAhead(void)
{
	s32					   flags;
	struct FieldActor	  *leader;
	union FieldCoordinate  pos[3];
	union FieldCoordinate *p;

	leader = Object_GetById(0);
	flags = leader->motion_flags;
	if (Engine_GameFlagIsSet(0x200) != 0)
	{
		p = pos;
		p[0].fixed = (leader->x.fixed & -0x100000) + 0x80000;
		p[1].fixed = leader->y.fixed;
		p[2].fixed = (leader->z.fixed & -0x100000) + 0x80000;
		FieldVector_AddPolarOffset(
			0x200000, (leader->facing + 0x2000) & 0xc000, (struct FieldPosition *)p);
		if (((s32 (*)())Object_CheckMovementCollision)((s32)leader, (s32)p) == 0)
		{
			Engine_EventBegin();
			Object_SetMode(leader, 6);
			Engine_TaskWait(6);
			Engine_AudioPlayCue(152);
			Object_SetMode(leader, 7);
			leader->speed = 0x30000;
			leader->acceleration = 0x20000;
			leader->velocity_y = 0x40000;
			leader->motion_flags &= 126;
			Engine_ActorSetSpriteFlags(leader, 0);
			Engine_ObjectMotionSetPositionAndCommit(0, p[0].part.pixel, p[2].part.pixel);
			Object_SetMode(leader, 6);
			Engine_ActorSetSpriteFlags(leader, 1);
			leader->motion_flags = flags;
			Engine_EventEnd();
		}
	}
}

void ShianMura_WalkThroughExit(s32 a0)
{
	u32 i;
	s32 record;

	Object_GetById(0)->motion_flags = 0;
	Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x8000, 0x4000);
	if (a0 == 6)
	{
		Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 2);
		Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, -16);
	}
	else
	{
		Actor_CenterAndWalk(ACTOR_PARTY_LEADER, 2, -16);
	}
	gEventWork->transition_frames = 16;
	Engine_EventRequestExit(a0);
}

void SceneEffect_DispatchStep(void)
{
	s16 *scene = (s16 *)gEventWork;
	u8	*shared0;
	s32	 shared1;

	Engine_EventBegin();

	switch (scene[182])
	{
	case 1:
		Engine_AudioPlayCue(158);
		Engine_MapAnimateCells(ShianMura_GateSteps1, 81, 18);
		break;
	case 2:
		Engine_AudioPlayCue(158);
		shared0 = ShianMura_GateSteps2;
		shared1 = 83;
		goto shared;
	case 3:
		Engine_AudioPlayCue(158);
		shared0 = ShianMura_GateSteps2;
		shared1 = 86;
		goto shared;
	case 4:
		Engine_AudioPlayCue(158);
		Engine_MapAnimateCells(ShianMura_GateSteps3, 84, 24);
		break;
	case 5:
		Engine_AudioPlayCue(158);
		Engine_MapAnimateCells(ShianMura_GateSteps3, 72, 7);
		break;
	case 6:
		Engine_AudioPlayCue(188);
		shared0 = ShianMura_GateSteps4;
		shared1 = 69;
	shared:
		Engine_MapAnimateCells(shared0, shared1, 11);
		break;
	case 7:
		Engine_AudioPlayCue(158);
		Engine_MapAnimateCells(ShianMura_GateSteps5, 83, 7);
		break;
	default:
		break;
	}

	ShianMura_WalkThroughExit(scene[182]);
	Engine_EventEnd();
}

void Scene_RunActorNineteenScript(void)
{
	s32 cb;

	Engine_EventBegin();
	((s32 (*)())Engine_ActorEnableActionCallback)(19, (s32)ShianMura_Actor19Motion);
	cb = (s32)SceneEffect_AdvanceRotatingSprite;
	((s32 (*)())Engine_TaskAddCallback)(cb, 0xc80);
	Object_RefreshSelectorById(19);
	Engine_AudioPlayCue(124);
	Effect_Spawn(0xa80000, 0x80000, 0x1380000, 0, 0, 0, 0x20001, 0);
	Effect_Spawn(0xa80000, 0x80000, 0x1380000, 0x3333, 0, 0, 0x20001, 0);
	Effect_Spawn(0xa80000, 0x80000, 0x1380000, -0x3333, 0, 0, 0x20001, 0);
	Engine_TaskRemoveCallback(cb);
	Object_GetById(19)->sprite->rotation = 0x8000;
	Engine_ActorSetPosition(21, 0xa80000, 0x1380000);
	Engine_EventWait(20);
	Engine_ActorFaceActor(14, 19, 0);
	Engine_ActorRunRepeatedMotion(14, 2);
	Engine_EventWait(10);
	Engine_EventSetMessage((s32)MsgShianWhaWhaHappened);
	if (GameFlag_IsSet(0x203))
	{
		gEventWork->message++;
	}
	Engine_EventShowMessage(14, 0);
	Engine_GameFlagSet(0x203);
	BattleFx_PlayQueuedSound();
	Engine_EventEnd();
}

void ShianMura_EscortActorEighteen(void)
{
	u32 i;
	s32 record;

	Engine_EventBegin();
	((void (*)())Engine_ActorEnableActionCallback)(18, 1);
	record = Actor_Get(18);
	*(s32 *)(record + 108) = 0;
	record = Actor_Get(18);
	*(s32 *)(record + 56) = -0x80000000;
	record = Object_GetById(18);
	*(s32 *)(record + 64) = -0x80000000;
	record = Object_GetById(18);
	*(s32 *)(record + 36) = 0;
	record = Object_GetById(18);
	*(s32 *)(record + 44) = 0;
	record = Object_GetById(18);
	*(s32 *)(record + 48) = 0;
	record = Actor_Get(18);
	*(s32 *)(record + 52) = 0;
	Actor_ShowEmote(18, 0x103, 0);
	Engine_ActorStartRepeatedMotion(18, 2);
	Engine_EventWait(60);
	Actor_SetSpeed(18, 0x18000, 0xc000);
	Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x18000, 0xc000);
	Actor_WalkTo(18, 0x118, 232);
	Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x128, 232);
	Engine_ActorWaitForMove(18);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x8000, 20);
	Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x102, 60);
	Engine_ActorEnableActionCallback(18, ShianMura_ActionTable);
	record = Actor_Get(18);
	*(s32 *)(record + 108) = (s32)ShianMura_WatchGateTrigger;
	Engine_EventEnd();
}

u8 *SceneEffect_GetTertiaryData(void)
{
	return gShianMuraEvents;
}

s32 ShianMura_SetupScene(void)
{
	struct FieldActor *actor;
	u32				   n;
	s32				   x;

	gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
	BattleFx_SetQueuedSoundAndPlay(169);
	if (gGameState.entrance > 9)
	{
		Engine_GameFlagClear(0x12f);
	}
	if (Engine_GameFlagIsSet(0x895))
	{
		Actor_FaceDirection(13, 0x8000, 0);
		Actor_SetPosition(14, 0x920000, 0x1380000);
		Engine_ActorFaceDirection(14, 0, 0);
		if (Engine_GameFlagIsSet(0x89a))
		{
			Engine_ActorSetPosition(17, 0, 0);
		}
	}
	if (Engine_GameFlagIsSet(0x8b0))
	{
		Engine_ActorSetPosition(17, 0, 0);
	}
	for (n = 0; n <= 2; n++)
	{
		struct FieldActor *actor;
		actor = Object_GetById(n + 23);
		actor->sprite->priority = 1;
		actor->motion_flags = 0;
		actor->collision_flags = 8;
		Engine_ActorSetSpriteFlags(actor, 0);
		ObjectGroup_SetChildValue(actor, 15);
		actor->priority_flags = (actor->priority_flags & 254) | 2;
	}
	if (Engine_GameFlagIsSet(0x202))
	{
		Actor_SetPosition(14, 0x920000, 0x1380000);
		Engine_ActorFaceDirection(14, 0, 0);
	}
	if (Engine_GameFlagIsSet(0x201))
	{
		Engine_ActorSetAnimation(20, 5);
		{
			s32 px = Object_GetById(20)->x.fixed;

			Engine_MapCopyCellAttributes(3, 17, 1, 1, px >> 20, Object_GetById(20)->z.fixed >> 20);
		}
		((void (*)())Engine_TaskAddCallback)((s32)Actor_UpdatePresentationFlag, 0xc80);
	}
	Engine_ActorSetChildValue(18, 2);
	Object_GetById(18)->update = ShianMura_WatchGateTrigger;
	actor = Object_GetById(19);
	actor->motion_flags = 0;
	actor->y.fixed = 0x100000;
	actor->target_y = 0x100000;
	actor->scale_x = 0x8ccc;
	actor->scale_y = 0x6666;
	actor->sprite->rotation = 0x8000;
	Engine_ActorSetSpriteFlags(Object_GetById(21), 0);
	Object_GetById(21)->motion_flags = 0;
	Object_GetById(21)->y.fixed = 0;
	Object_GetById(21)->target_y = -0x80000000;
	return 0;
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

s32 ShianMura_ActionTable[] = {
	SC_SET(SW_SPEED, 0x6666),
	SC_SET(SW_ACCEL, 0x1999),
	SC_WAIT(60),
	SC_LABEL(1),
	SC_ARG(3),
	SC_WAIT(60),
	SC_ARG(3),
	SC_WAIT(180),
	SC_ARG(2),
	SC_MOVE(PIXELS(424), 0, PIXELS(232)),
	SC_SYNC,
	SC_MOVE(PIXELS(424), 0, PIXELS(216)),
	SC_SYNC,
	SC_WAIT(10),
	SC_SET(SW_FACING, 0x1000),
	SC_WAIT(10),
	SC_ARG(3),
	SC_WAIT(60),
	SC_ARG(3),
	SC_WAIT(120),
	SC_ARG(2),
	SC_MOVE(PIXELS(424), 0, PIXELS(232)),
	SC_SYNC,
	SC_MOVE(PIXELS(280), 0, PIXELS(232)),
	SC_SYNC,
	SC_LOOP(SC_FOREVER, SC_TO(1)),
	SC_END,
};

s32 ShianMura_Actor19Motion[] = {
	SC_SET(SW_SNAP, 0),
	SC_SET(SW_SPEED, 0x30000),
	SC_SET(SW_ACCEL, 0x18000),
	SC_MOVE(PIXELS(168), PIXELS(48), PIXELS(312)),
	SC_SYNC,
	SC_SET(SW_SPEED, 0x30000),
	SC_SET(SW_ACCEL, 0x1999),
	SC_SET(SW_SNAP, 1),
	SC_MOVE(PIXELS(168), PIXELS(16), PIXELS(312)),
	SC_SYNC,
	SC_END,
};

s32 ShianMura_EffectScript[] = {
	SC_WAIT(18),
	SC_WAIT(1),
	SC_POS(0, 0, 0),
	SC_WAIT(1),
	SC_FREE,
};

s32 ShianMura_EffectConfig[] = {
	SC_SET(SW_FACING, FACING_WEST),
	SC_WAIT(16),
	SC_WAIT(1),
	SC_POS(0, 0, 0),
	SC_WAIT(1),
	SC_FREE,
};

u16 ShianMura_GateSteps1[] = {
	MAP_CELL_STEP(64, 48, 1, 2, 4), MAP_CELL_STEP(63, 48, 1, 2, 4), MAP_CELL_STEPS_END,
};

u16 ShianMura_GateSteps2[] = {
	MAP_CELL_STEP(69, 50, 2, 2, 4), MAP_CELL_STEP(65, 50, 2, 2, 4), MAP_CELL_STEPS_END,
};

u16 ShianMura_GateSteps3[] = {
	MAP_CELL_STEP(66, 46, 3, 2, 4), MAP_CELL_STEP(63, 46, 3, 2, 4), MAP_CELL_STEPS_END,
};

u16 ShianMura_GateSteps4[] = {
	MAP_CELL_STEP(67, 50, 4, 2, 4), MAP_CELL_STEP(63, 50, 4, 2, 4), MAP_CELL_STEPS_END,
};

u16 ShianMura_GateSteps5[] = {
	MAP_CELL_STEP(66, 44, 3, 2, 4), MAP_CELL_STEP(63, 44, 3, 2, 4), MAP_CELL_STEPS_END,
};

struct SceneEntrance gShianMuraEntrances[] = {
	{ 0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 1, CONDITION_ALWAYS, 376, 0, 504, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
	{ 5, CONDITION_ALWAYS, 344, 0, 328, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 6, CONDITION_ALWAYS, 376, 0, 216, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 7, CONDITION_ALWAYS, 424, 0, 216, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 8, CONDITION_ALWAYS, 408, 0, 424, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 9, CONDITION_ALWAYS, 216, 0, 184, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 10, CONDITION_ALWAYS, 392, 0, 184, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 11, CONDITION_ALWAYS, 176, 0, 216, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ SCENE_TABLE_END },
};

u32 gShianMuraExits[] = {
	SCENE_EXITS(72),
	SCENE_EXIT(1, 73, 1),
	SCENE_EXIT(2, 73, 2),
	SCENE_EXIT(3, 73, 3),
	SCENE_EXIT(4, 73, 4),
	SCENE_EXIT(5, 73, 6),
	SCENE_EXIT(6, 9, 32),
	SCENE_EXIT(7, 61, 2),
	SCENE_EXIT(8, 2, 14),
	SCENE_EXITS_END,
};

struct ScenePlacement gShianMuraPlacements[] = {
	{ 0x9c, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(328), 0, PIXELS(448), FACING_EAST, TALK_FACE_PARTY, 0 },
	{ 0x9f, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(152), 0, PIXELS(184), FACING_SOUTHEAST, TALK_FACE_PARTY, 0 },
	{ 0xa0, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(420), 0, PIXELS(304), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0xa4, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(144), 0, PIXELS(232), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
	{ 0xa6, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(312), 0, PIXELS(200), FACING_EAST, TALK_FACE_PARTY, 0 },
	{ 0xa8, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(264), 0, PIXELS(184), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0xa9, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(192), 0, PIXELS(312), FACING_WEST, TALK_KEEP_FACING, 0 },
	{ 0xaa, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(428), 0, PIXELS(432), FACING_WEST, TALK_FACE_PARTY, 0 },
	{ 0x9f, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(200), 0, PIXELS(440), FACING_NORTH, TALK_FACE_PARTY, 0 },
	{ 0x27, IF_FLAG_SET(0x89a), ACTOR_STAND,
		PIXELS(368), 0, PIXELS(496), FACING_SOUTHEAST + FACING_STEP, 3, 0 },
	{ 0x9f, CONDITION_ALWAYS, (s32)ShianMura_ActionTable,
		PIXELS(280), 0, PIXELS(232), FACING_WEST, TALK_KEEP_FACING, 0 },
	{ 0xcd, CONDITION_ALWAYS, 7,
		PIXELS(168), 0, PIXELS(312), FACING_WEST, TALK_FACE_PARTY, 0 },
	{ 0xe3, CONDITION_ALWAYS, 7,
		0, 0, 0, FACING_WEST, TALK_FACE_PARTY, 0 },
	{ 0x16, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(168), 0, PIXELS(312), FACING_WEST, TALK_FACE_PARTY, 0 },
	{ 0x5b, 0x46, ACTOR_STAND,
		PIXELS(448), 0, PIXELS(328), FACING_WEST, TALK_FACE_PARTY, 0 },
	{ 0xdf, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(426), 0, PIXELS(428), FACING_WEST, TALK_FACE_PARTY, 0 },
	{ 0xdf, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(299), 0, PIXELS(222), FACING_WEST, TALK_FACE_PARTY, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0xdf, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(168), 0, PIXELS(280), FACING_WEST, TALK_FACE_PARTY, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ SCENE_TABLE_END },
};

struct SceneEvent gShianMuraEvents[] = {
	{ TOUCH_WALKING_INTO(FACING_NORTH), 1, CONDITION_ALWAYS, EVENT_SCRIPT(SceneEffect_DispatchStep) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 2, CONDITION_ALWAYS, EVENT_SCRIPT(SceneEffect_DispatchStep) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 3, CONDITION_ALWAYS, EVENT_SCRIPT(SceneEffect_DispatchStep) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 4, CONDITION_ALWAYS, EVENT_SCRIPT(SceneEffect_DispatchStep) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 5, CONDITION_ALWAYS, EVENT_SCRIPT(SceneEffect_DispatchStep) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 6, CONDITION_ALWAYS, EVENT_SCRIPT(SceneEffect_DispatchStep) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 7, CONDITION_ALWAYS, EVENT_SCRIPT(SceneEffect_DispatchStep) },
	{ EVENT_EXIT, 8, CONDITION_ALWAYS, 8 },
	{ 0x202, 20, 0x201, EVENT_SCRIPT(ShianMura_HopLeaderAhead) },
	{ TOUCH_WALKING_INTO(FACING_SOUTH), 21, 0x201, EVENT_SCRIPT(ShianMura_HopLeaderAhead) },
	{ TOUCH_WALKING_INTO(FACING_SOUTH), 2, 0x201, EVENT_SCRIPT(ShianMura_HopLeaderAhead) },
	{ TOUCH_WALKING_INTO(FACING_SOUTH), 3, 0x201, EVENT_SCRIPT(ShianMura_HopLeaderAhead) },
	{ EVENT_TALK, 8, 0x895, MSGNO(MsgShianMuraThisIsXianTravelersAreWelcome) },
	{ EVENT_TALK, 9, 0x895, EVENT_SCRIPT(SceneEffect_ShowActorSetupMessage) },
	{ EVENT_TALK, 10, 0x895, MSGNO(MsgShianMuraXianIsTheVillageOfKinu) },
	{ EVENT_TALK, 11, 0x895, MSGNO(MsgShianMuraMartialArtsAreVeryPopularIn) },
	{ EVENT_TALK, 12, 0x895, MSGNO(MsgShianMuraYouWarriorsShouldVisitMasterFehs) },
	{ EVENT_TALK, 13, 0x895, MSGNO(MsgShianMuraMyClothesAreNotSilkSilk) },
	{ EVENT_TALK, 14, 0x895, EVENT_SCRIPT(ShianMura_RunNpcMeetScene) },
	{ EVENT_TALK, 15, 0x895, MSGNO(MsgShianMuraDisastersFromTheSkyManyMany) },
	{ EVENT_TALK, 16, 0x895, MSGNO(MsgShianMuraThisIsTheMulberryOrchardWhen) },
	{ EVENT_TALK, 17, 0x895, EVENT_SCRIPT(SceneEffect_RunActorSceneMessage) },
	{ EVENT_TALK, 18, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunPrimarySequence) },
	{ EVENT_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgShianMuraXianIsAPeacefulVillageBut) },
	{ EVENT_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgShianMuraThereWasARockslideOnSilk) },
	{ EVENT_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgShianMuraIWentToHelpMoveBoulders) },
	{ EVENT_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgShianMuraThePeopleWhoClearedTheRockslide) },
	{ EVENT_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgShianMuraHsuIsVeryGoodAtKung) },
	{ EVENT_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgShianMuraThePeopleOfXianLoveTheir) },
	{ EVENT_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgShianMuraAltinWasFloodedWithWaterBut) },
	{ EVENT_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgShianMuraWeClearedTheRocksFromSilk) },
	{ EVENT_TALK, 16, CONDITION_ALWAYS, MSGNO(MsgShianMuraManyStrangeThingsHappenedAllAt) },
	{ 0x10001815, 20, 0x201, EVENT_SCRIPT(SceneEffect_ActivateNearbyActor) },
	{ PSYNERGY_ON_ACTOR(ABILITY_FROST), 20, 0x201, EVENT_SCRIPT(ShianMura_ActivateActorTwentyPlatform) },
	{ PSYNERGY_ON_ACTOR(0x8e), 21, CONDITION_ALWAYS, EVENT_SCRIPT(Scene_RunActorNineteenScript) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, 0x895, MSGNO(MsgShianMuraTheseChildrenMustHaveComeTo) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, 0x895, MSGNO(MsgShianMuraYouYoungWarriorsLookStrongBut) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, 0x895, MSGNO(MsgShianMuraIHopeManyPeopleComeTo) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, 0x895, MSGNO(MsgShianMuraFightingWithKicksAndPunchesIs) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, 0x895, MSGNO(MsgShianMuraMasterFehsSchoolIsTrainingIt) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, 0x895, MSGNO(MsgShianMuraSilkIsShinyAndSmoothIt) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, 0x895, MSGNO(MsgShianMuraBreakingTreesWithMyHandHurts) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, 0x895, MSGNO(MsgShianMuraMasterFehSaidYahAndThen) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, 0x895, MSGNO(MsgShianMuraTheOrchardGivesTheVillageLife) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, 0x895, MSGNO(MsgShianMuraGoingWestAloneIsVeryDangerous) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS, MSGNO(MsgShianMuraIMustCarryWaterEveryDay) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgShianMuraHsuWasCaughtUnderABoulder) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgShianMuraTheRocksThereWereVeryHard) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgShianMuraSomePeopleDidNotCarryRocks) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgShianMuraSandIsTheColorOfThe) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgShianMuraHsuIsStrongHeadedHeWould) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgShianMuraEveryoneStaysInXianTheyDo) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgShianMuraThisWarriorSavedAltinVillageNo) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgShianMuraItIsBadForXianIf) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, MSGNO(MsgShianMuraIWasHappyToSeeGems) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS, MSGNO(MsgShianMuraWhereAreTheMerchantsNoOne) },
	{ SEARCH(SEARCH_JAR), 101, 0xf6e, FIND_ITEM(ITEM_LUCKY_MEDAL) },
	{ EVENT_TALK, 22, CONDITION_ALWAYS, EVENT_SCRIPT(SceneEffect_RequestFixedEffect) },
	{ EVENT_RAISED, 200, CONDITION_ALWAYS, EVENT_SCRIPT(ShianMura_EscortActorEighteen) },
	{ SCENE_EVENTS_END },
};
