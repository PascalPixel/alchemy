#include "game_state.h"
#include "global_cells.h"
#include "unit/import.h"
#include "unit/field/mogoru_mori/mori.h"
#include "unit/field/common/object/orbit.h"
#include "unit/field/common/effect/create.h"
#include "unit/field/common/object/staged_actor.h"
#include "unit/field/common/object/distance.h"
#include "unit/field/common/effect/move.h"
#include "unit/battle/effect/slots.h"

#define FIELD_STAGED_ACTOR_IMPORTS
#include "fieldobj.h"
#include "mapcopy.h"
#include "types.h"
#include "field_event.h"
#include "field_scene.h"
#include "field_effect.h"
#include "staged_actor.h"
#include "select_overlay_data_by_runtime_selector.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "edition.h"
#include "call.h"
#include "scene_ids.h"
#include "text/msg_ids.h"
#include "ability_ids.h"
#include "objcmd.h"

enum
{
	ITEM_NUT = 181
};

// ここに共通ソースが入る（ROM の並びのとおり）
#include "../common/object/staged_actor.c"
#include "../common/effect/create.c"
#include "../common/effect/move.c"
#include "../common/effect/spawnov.c"

struct Vec
{
	s32 x;
	s32 y;
	s32 z;
};

extern struct EventWork *gEventWork;

void SceneState_SetValue18Mode2(void)
{
	BattleFx_SetPhaseRequest(18, 2);
}

s32 SceneActor_TryRunSlotZeroMoveStep(s16 *arg)
{
	struct FieldActor *p = Object_GetById(ACTOR_PARTY_LEADER);
	u8				  *f = &p->motion_flags;
	s32				   saved = *f;

	s32 r = Object_CheckMovementCollision(p, arg);

	if (r == 0)
	{
		s32 m;

		Engine_EventBegin();
		Object_SetMode(p, 6);
		Engine_TaskWait(6);
		Engine_AudioPlayCue(152);
		Object_SetMode(p, 7);
		p->speed = 0x30000;
		p->acceleration = 0x20000;
		p->velocity_y = 0x40000;
		m = 0x7e;
		m &= *f;
		*f = m;
		Engine_ActorSetSpriteFlags(p, 0);
		Engine_ActorMoveToAndWait(ACTOR_PARTY_LEADER, arg[1], arg[5]);
		Object_SetMode(p, 6);
		Engine_ActorSetSpriteFlags(p, 1);
		*f = saved;
		Engine_EventEnd();
		return 1;
	}
	return 0;
}

s32 MogoruMori_CopyTimerToChild(s32 a)
{
	ObjectGroup_SetChildValue(a, ((struct FieldActor *)a)->timer);
	return 0;
}

s32 OverlayObject_ApplyZero(s32 a)
{
	Engine_ActorSetSpriteFlags(a, 0);
	return 0;
}

void MogoruMori_HopActorTo(s32 a0, s32 a1, s32 a2, s32 a3)
{
	struct FieldActor *rec7;

	rec7 = Object_GetById(a0);
	Engine_ActorSetSpritePriority(a0, 1);
	Actor_SetSpeed(a0, 0x30000, 0x18000);
	Engine_AudioPlayCue(152);
	rec7->velocity_y = a3;
	rec7->gravity = 0x8000;
	rec7->bounce = 0;
	Engine_ActorSetSpriteFlags(rec7, 0);
	Actor_MoveToAndWait(a0, a1, a2);
	Actor_SetPosition(a0, a1 << 16, a2 << 16);
	Engine_ActorSetSpriteFlags(rec7, 1);
	rec7->gravity = 0x10000;
}

void MogoruMori_SpawnPuffRing(s32 id)
{
	struct Vec			  dir;
	struct EffectOptions  params;
	struct EffectOptions *p;
	struct Vec			 *v;
	struct FieldActor	 *actor;
	u32					  i;
	s32					  x;
	s32					  z;

	actor = Object_GetById(id);
	Engine_AudioPlayCue(188);
	p = &params;
	p->priority = 1;
	for (i = 0; i <= 16; i++)
	{
		v = &dir;
		v->x = Engine_MathCos(i << 12);
		v->y = 0;
		z = Engine_MathSin(i << 12);
		x = v->x;
		v->z = z;
		x += x / 3;
		v->x = x;
		Effect_Spawn(actor->x.fixed, 0x100000, actor->z.fixed, x, v->y + 0x1999, z, 0x20000, p);
	}
}

void FieldScene_RunSixCallSetupSequence(s32 no, s32 val)
{
	s32 v0 = 0x20000;
	s32 v1 = 0x4000;

	Engine_CameraSetSpeed(v0, v1);
	Engine_CameraMoveToActor(no, 1);
	Engine_CameraWaitForMove();
	Engine_EventWait(30);
	MogoruMori_SpawnPuffRing(no);
	Engine_ActorSetChildValue(no, val);
}

const struct SceneEntrance *Scene_GetEntrances(void)
{
	s16 selector = gGameState.scene;

	if (selector == (s32)&SceneId_MogoruMori1)
	{
		return gMogoruMoriEntrances1;
	}
	if (selector == (s32)&SceneId_MogoruMori2)
	{
		return gMogoruMoriEntrances2;
	}
	if (selector == (s32)&SceneId_MogoruMori3)
	{
		return gMogoruMoriEntrances3;
	}
	return gMogoruMoriEntrancesOther;
}

s32 SceneData_ReturnZero(void)
{
	return 0;
}

u8 *MogoruMori_GetExits(void)
{
	return MogoruMori_SceneTable;
}

const struct ScenePlacement *Scene_GetPlacements(void)
{
	s16 selector = gGameState.scene;

	if (selector == (s32)&SceneId_MogoruMori1)
	{
		return gMogoruMoriPlacements1;
	}
	if (selector == (s32)&SceneId_MogoruMori2)
	{
		return gMogoruMoriPlacements2;
	}
	if (selector == (s32)&SceneId_MogoruMori3)
	{
		return gMogoruMoriPlacements3;
	}
	return gMogoruMoriPlacementsOther;
}

void MogoruMori_RunProbedActorNineScene(void)
{
	struct StagedActorProbe probe;
	s32						fifth;
	s32						sixth;

	Engine_EventBegin();
	if (StagedActor_FindClearPosition(&probe) != 0)
	{
		SceneActor_MoveAndRedraw(probe);
		if (probe.actor_slot == 9 && (probe.position_z >> 20) == 26)
		{
			Engine_GameFlagSet(0x310);
			Engine_ActorSetAnimation(9, 3);
			Actor_SetSpeed(9, 0x4000, 0x8000);
			Engine_ActorSetDestinationOffset(9, 0, -16);
			Engine_EventWait(45);
			Engine_ActorSetAnimation(9, 8);
			Engine_AudioPlayCue(240);
			Engine_ActorSetSpritePriority(9, 1);
			Object_GetById(9)->priority_flags = 2;
			fifth = 31;
			sixth = 25;
			Engine_MapCopyCellAttributes(38, 27, 4, 2, fifth, sixth);
		}
	}
	Engine_EventEnd();
}

void SceneActor_PassOffsetPointOfActorZero(void)
{
	s32 v[3];
#if EDITION_INTERNATIONAL
	s32 *p = Object_GetById(ACTOR_PARTY_LEADER);

	v[0] = (p[2] & 0xfff00000) + 0x80000;
	v[1] = p[3];
	v[2] = (p[4] & 0xfff00000) + 0xffe80000;
#else
	v[0] = Object_GetById(ACTOR_PARTY_LEADER)->x.fixed;
	v[1] = Object_GetById(ACTOR_PARTY_LEADER)->y.fixed;
	v[2] = Object_GetById(ACTOR_PARTY_LEADER)->z.fixed - 0x200000;
#endif
	SceneActor_TryRunSlotZeroMoveStep(v);
}

void SceneActor_BobActorZeroWhenAheadClear(void)
{
	s32				   pos[3];
	struct FieldActor *actor = Object_GetById(ACTOR_PARTY_LEADER);
	u8				  *fp = &actor->motion_flags;
	s32				   saved = *fp;

#if EDITION_INTERNATIONAL
	pos[0] = (actor->x.fixed & 0xfff00000) + 0x80000;
	pos[1] = actor->y.fixed;
	pos[2] = (actor->z.fixed & 0xfff00000) + 0x280000;
#else
	pos[0] = Object_GetById(ACTOR_PARTY_LEADER)->x.fixed;
	pos[1] = Object_GetById(ACTOR_PARTY_LEADER)->y.fixed;
	pos[2] = Object_GetById(ACTOR_PARTY_LEADER)->z.fixed + 0x200000;
#endif
	if (SceneActor_TryRunSlotZeroMoveStep(pos) != 0)
	{
		Engine_EventBegin();
		*fp = 0;
		Engine_ActorSetAnimation(9, 7);
		actor->y.fixed += -0x10000;
		actor->terrain_height += -0x10000;
		Engine_TaskWait(2);
		actor->y.fixed += -0x10000;
		actor->terrain_height += -0x10000;
		Engine_TaskWait(10);
		actor->y.fixed += 0x10000;
		actor->terrain_height += 0x10000;
		Engine_TaskWait(4);
		actor->y.fixed += 0x10000;
		actor->terrain_height += 0x10000;
		*fp = saved;
		Engine_EventEnd();
	}
}

void FieldScene_RunScriptedSteps0And17E6(void)
{
	Engine_EventBegin();
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 1);
	Engine_MessageShowCentered(MSGNO(MsgMogoruMoriBrokenSign), 1);
	Engine_EventEnd();
}

void FieldScene_RunActor10WaypointSequence(void)
{
	struct FieldActor *slot;

	slot = Object_GetById(10);

	Engine_EventBegin();

	FieldScene_RunSixCallSetupSequence(10, 1);
	MogoruMori_HopActorTo(10, 88, 120, 0x60000);

	Effect_Spawn(slot->x.fixed, slot->y.fixed, slot->z.fixed + 0x180000, 0, 0, 0, 1, 0);

	Engine_CameraFollowActor(10, 1);
	Engine_ActorFaceEachOther(10, ACTOR_PARTY_LEADER, 0);
	Engine_EventWait(20);
	Engine_ActorStartRepeatedMotion(10, 2);
	Engine_ActorSetAttachedEffect(10, 258);
	Engine_EventWait(60);

	MogoruMori_HopActorTo(10, 88, 152, 0x30000);
	Engine_ActorFaceActor(ACTOR_PARTY_LEADER, 10, 0);
	Engine_EventWait(10);

	MogoruMori_HopActorTo(10, 120, 192, 0x30000);
	Engine_ActorFaceActor(ACTOR_PARTY_LEADER, 10, 0);
	Engine_EventWait(10);

	MogoruMori_HopActorTo(10, 120, 240, 0x30000);
	Engine_ActorFaceActor(ACTOR_PARTY_LEADER, 10, 0);
	Engine_EventWait(10);

	Engine_GameFlagSet(768);
	Engine_ActorSetPosition(13, 0, 0);
	Engine_ActorSetPosition(10, 0, 0);

	Engine_EventEnd();
}

void FieldScene_RunActorElevenPresentationBeat(void)
{
	struct FieldActor *slot;

	slot = Object_GetById(11);

	Engine_EventBegin();

	FieldScene_RunSixCallSetupSequence(11, 0);
	MogoruMori_HopActorTo(11, 408, 456, 0x60000);

	Effect_Spawn(slot->x.fixed, slot->y.fixed, slot->z.fixed + 0x180000, 0, 0, 0, 1, 0);

	Engine_CameraFollowActor(11, 1);
	Engine_ActorFaceEachOther(11, ACTOR_PARTY_LEADER, 0);
	Engine_EventWait(30);
	Engine_ActorStartRepeatedMotion(11, 2);
	Engine_ActorShowEmote(11, 0x103, 0);
	Engine_AudioPlayCue(147);
	Engine_EventWait(60);

	MogoruMori_HopActorTo(
		11, Object_GetById(0)->x.part.pixel, Object_GetById(0)->z.part.pixel, 0x40000);

	Engine_EventWait(10);
	Engine_GameFlagSet(0x301);
	Engine_ActorSetPosition(14, 0, 0);

	Battle_SetStart(3);

	BattleFx_SetWeightedResult(53, 0);

	Engine_EventEnd();
}

void SceneActor_RunActorTwelveThreeWaypointMotion(void)
{
	struct FieldActor *slot;

	slot = Object_GetById(12);

	Engine_EventBegin();

	FieldScene_RunSixCallSetupSequence(12, 1);
	MogoruMori_HopActorTo(12, 536, 344, 0x70000);

	Effect_Spawn(slot->x.fixed, slot->y.fixed, slot->z.fixed + 0x100000, 0, 0, 0, 1, 0);

	Engine_CameraFollowActor(12, 1);
	Engine_ActorFaceEachOther(12, ACTOR_PARTY_LEADER, 0);
	Engine_EventWait(20);
	Engine_ActorStartRepeatedMotion(12, 2);
	Engine_ActorSetAttachedEffect(12, 258);
	Engine_EventWait(60);

	MogoruMori_HopActorTo(12, 584, 344, 0x30000);
	Engine_ActorFaceActor(ACTOR_PARTY_LEADER, 12, 0);
	Engine_EventWait(6);

	MogoruMori_HopActorTo(12, 632, 344, 0x30000);
	Engine_ActorFaceActor(ACTOR_PARTY_LEADER, 12, 0);
	Engine_EventWait(6);

	MogoruMori_HopActorTo(12, 680, 344, 0x30000);
	Engine_ActorFaceActor(ACTOR_PARTY_LEADER, 12, 0);
	Engine_EventWait(6);

	Engine_GameFlagSet(0x302);
	Engine_ActorSetPosition(15, 0, 0);

	Engine_EventEnd();
}

void MogoruMori_AwardHiddenNut(s32 a)
{
	Engine_EventBegin();
	Engine_ActorSetPosition(16, 0, 0);
	Engine_GameFlagSet(4052);
	Engine_ItemShowFound(ITEM_NUT, 3);
	Engine_PartyGiveItem(ITEM_NUT, 0);
	Engine_EventEnd();
}

void FieldScene_RunProbedActorEightOrTenScene(void)
{
	struct StagedActorProbe probe;
	s32						fifth;
	s32						sixth;
	s32						height;
	s32						value;

	Engine_EventBegin();

	if (StagedActor_FindClearPosition(&probe) != 0)
	{
		SceneActor_MoveAndRedraw(probe);

		if (probe.actor_slot == 8 && (probe.position_z >> 20) == 23)
		{
			fifth = 35;
			sixth = 68;
			Engine_MapCopyCellAttributes(35, 67, 4, 1, fifth, sixth);
		}
		else if (probe.actor_slot == 10 && (probe.position_x >> 20) == 35)
		{

			value = 0;
			Engine_GameFlagSet(0x311);
			Engine_ActorSetAnimation(10, 3);
			Engine_ActorSetDestinationOffset(10, -16, 6);
			Engine_EventWait(30);
			Engine_ActorSetAnimation(10, 8);
			Engine_AudioPlayCue(240);

			Object_GetById(10)->priority_flags = 2;

			fifth = 34;
			sixth = 30;
			Engine_MapCopyCellAttributes(44, 30, 2, 4, fifth, sixth);
			height = 4;
			StagedActor_FillGridAttributeRectangle(2, 35, 30, 1, height, value);
		}
	}

	Engine_EventEnd();
}

void SceneState_ApplyCrossRectsAroundActor11(void)
{
	s32 x;
	s32 z;

	Engine_EventBegin();

	x = Object_GetById(11)->x.fixed >> 20;
	z = Object_GetById(11)->z.fixed >> 20;

	StagedActor_FillGridAttributeRectangle(2, x, z, 1, 1, 0xff);
	StagedActor_FillGridAttributeRectangle(2, x + 1, z, 1, 1, 0);
	StagedActor_FillGridAttributeRectangle(2, x - 1, z, 1, 1, 0);
	StagedActor_FillGridAttributeRectangle(2, x, z + 1, 1, 1, 0);
	StagedActor_FillGridAttributeRectangle(2, x, z - 1, 1, 1, 0);

	if (x == 36 && z == 24)
	{
		u8 *p = (u8 *)Object_GetById(11);

		((struct FieldActor *)p)->motion_flags = 0;
		((struct FieldActor *)p)->terrain_height = 0xfffe0000;
		((struct FieldActor *)p)->y.fixed = 0xfffe0000;
	}

	Engine_EventEnd();
}

void MogoruMori_RunBranchHopScene(void)
{
	struct FieldActor *rec7;

	rec7 = Object_GetById(12);
	Engine_EventBegin();
	FieldScene_RunSixCallSetupSequence(12, 1);
	MogoruMori_HopActorTo(12, 0x188, 104, 0x70000);
	Engine_EventWait(10);
	Effect_Spawn(rec7->x.fixed, rec7->y.fixed, (rec7->z.fixed + 0x40000), 0, 0, 0, 1, 0);
	Engine_CameraFollowActor(12, 1);
	Engine_ActorFaceEachOther(12, 0, 0);
	Engine_EventWait(20);
	Engine_ActorStartRepeatedMotion(12, 2);
	Engine_ActorSetAttachedEffect(12, 0x102);
	Engine_EventWait(60);
	MogoruMori_HopActorTo(12, 0x1a8, 120, 0x30000);
	Engine_ActorFaceActor(0, 12, 0);
	Engine_EventWait(6);
	MogoruMori_HopActorTo(12, 0x1a8, 168, 0x30000);
	Engine_ActorFaceActor(0, 12, 0);
	Engine_EventWait(6);
	MogoruMori_HopActorTo(12, 0x1a8, 208, 0x30000);
	Engine_ActorFaceActor(0, 12, 0);
	Engine_EventWait(6);
	MogoruMori_HopActorTo(12, 0x1a8, 232, 0x30000);
	Engine_ActorFaceActor(0, 12, 0);
	Engine_EventWait(6);
	Engine_ActorSetPosition(12, 0, 0);
	Engine_GameFlagSet(0x303);
	Engine_ActorSetPosition(15, 0, 0);
	Engine_EventEnd();
}

void FieldScene_RunActorThirteenPresentationBeat(void)
{
	struct FieldActor *slot;

	slot = Object_GetById(13);

	Engine_EventBegin();

	FieldScene_RunSixCallSetupSequence(13, 1);
	MogoruMori_HopActorTo(13, 456, 104, 0x70000);
	Engine_EventWait(10);

	Effect_Spawn(slot->x.fixed, slot->y.fixed, slot->z.fixed + 0x40000, 0, 0, 0, 1, 0);

	Engine_CameraFollowActor(13, 1);
	Engine_ActorFaceEachOther(13, ACTOR_PARTY_LEADER, 0);
	Engine_EventWait(20);
	Engine_ActorStartRepeatedMotion(13, 2);
	Engine_ActorSetAttachedEffect(13, 258);
	Engine_EventWait(60);

	MogoruMori_HopActorTo(13, 472, 136, 0x30000);
	Engine_ActorFaceActor(ACTOR_PARTY_LEADER, 13, 0);
	Engine_EventWait(6);

	MogoruMori_HopActorTo(13, 504, 136, 0x33333);
	Engine_ActorFaceActor(ACTOR_PARTY_LEADER, 13, 0);
	Engine_EventWait(6);

	MogoruMori_HopActorTo(13, 552, 136, 0x38000);
	Engine_ActorFaceActor(ACTOR_PARTY_LEADER, 13, 0);
	Engine_EventWait(6);

	MogoruMori_HopActorTo(13, 584, 136, 0x38000);
	Engine_ActorFaceActor(ACTOR_PARTY_LEADER, 13, 0);
	Engine_EventWait(6);

	Engine_ActorSetPosition(13, 0, 0);
	Engine_GameFlagSet(772);
	Engine_ActorSetPosition(16, 0, 0);

	Engine_EventEnd();
}

void MogoruMori_TransferActorFourteen(void)
{
	u32 i;
	s32 record;

	Engine_EventBegin();
	FieldScene_RunSixCallSetupSequence(14, 1);
	MogoruMori_HopActorTo(14, 0x1a8, 0x1e0, 0x79999);
	Engine_EventWait(2);
	MogoruMori_SpawnPuffRing(14);
	Actor_SetChildValue(14, 15);
	record = Actor_Get(14);
	Engine_ActorSetSpriteFlags(record, 0);
	Engine_EventWait(30);
	GameFlag_Set(0x305);
	Actor_SetPosition(17, 0x1a80000, 0x1e00000);
	Engine_EventEnd();
}

void SceneActor_RunActorFourteenFourWaypointMotion(void)
{
	struct FieldActor *slot;

	slot = Object_GetById(14);

	Engine_EventBegin();

	FieldScene_RunSixCallSetupSequence(14, 1);
	MogoruMori_HopActorTo(14, 392, 504, 0x60000);
	Engine_EventWait(10);

	Effect_Spawn(slot->x.fixed, slot->y.fixed, slot->z.fixed + 0x40000, 0, 0, 0, 1, 0);

	Engine_CameraFollowActor(14, 1);
	Engine_ActorFaceEachOther(14, ACTOR_PARTY_LEADER, 0);
	Engine_EventWait(20);
	Engine_ActorStartRepeatedMotion(14, 2);
	Engine_ActorSetAttachedEffect(14, 258);
	Engine_EventWait(60);

	MogoruMori_HopActorTo(14, 360, 528, 0x30000);
	Engine_ActorFaceActor(ACTOR_PARTY_LEADER, 14, 0);
	Engine_EventWait(6);

	MogoruMori_HopActorTo(14, 328, 528, 0x30000);
	Engine_ActorFaceActor(ACTOR_PARTY_LEADER, 14, 0);
	Engine_EventWait(6);

	MogoruMori_HopActorTo(14, 288, 528, 0x30000);
	Engine_ActorFaceActor(ACTOR_PARTY_LEADER, 14, 0);
	Engine_EventWait(6);

	MogoruMori_HopActorTo(14, 256, 528, 0x30000);
	Engine_ActorFaceActor(ACTOR_PARTY_LEADER, 14, 0);
	Engine_EventWait(6);

	Engine_CameraFollowActor(ACTOR_PARTY_LEADER, 1);
	Engine_ActorSetPosition(14, 0, 0);
	Engine_EventWait(30);
	Engine_GameFlagSet(0x306);
	Engine_ActorSetPosition(17, 0, 0);

	Engine_EventEnd();
}

void MogoruMori_RunProbedLandingScene(void)
{
	struct StagedActorProbe probe;
	s32						landed;

	Engine_EventBegin();
	landed = 0;
	if (StagedActor_FindClearPosition(&probe) != 0)
	{
		SceneActor_MoveAndRedraw(probe);
		if (probe.actor_slot == 9)
			goto nine;
		if (probe.actor_slot == 11)
			goto eleven;
		goto other;
	nine:
		Map_CopyCellAttributes(38, 68, 1, 4, probe.position_x >> 20, 68);
		if (probe.position_x >> 20 == 42)
		{
			Map_CopyCellAttributes(26, 20, 2, 4, probe.position_x >> 20, 23);
			Engine_ActorSetSpritePriority(9, 1);
			landed = 1;
			GameFlag_Set(0x312);
		}
		goto join;
	eleven:
		if (probe.position_x >> 20 == 40)
		{
			Map_CopyCellAttributes(26, 20, 2, 4, probe.position_x >> 20, 32);
			Engine_ActorSetSpritePriority(11, 1);
			landed = 1;
			GameFlag_Set(0x313);
		}
	join:
		if (landed == 0)
		{
			Engine_EventEnd();
			return;
		}
		Engine_ActorSetAnimation(probe.actor_slot, 3);
		Engine_ActorSetDestinationOffset(probe.actor_slot, 18, 6);
		Engine_EventWait(30);
		Engine_ActorSetAnimation(probe.actor_slot, 8);
		Engine_AudioPlayCue(240);
		Actor_Get(probe.actor_slot)->priority_flags = 2;
		goto end;
	other:
		if (probe.actor_slot == 8)
			Map_CopyCellAttributes(42, 49, 1, 4, probe.position_x >> 20, 49);
	}
end:
	Engine_EventEnd();
}

void SceneActor_BobActorZeroWhenTargetClear(void)
{
	struct FieldActor *record;
	u8				  *mode;
	u8				   saved;
	s32				   target[3];

	record = Object_GetById(ACTOR_PARTY_LEADER);
	mode = &record->motion_flags;
	saved = *mode;

	target[0] = Object_GetById(0)->x.fixed + 0xffe00000;
	target[1] = Object_GetById(0)->y.fixed;
	target[2] = Object_GetById(0)->z.fixed;

	if (SceneActor_TryRunSlotZeroMoveStep(target) != 0)
	{

		Engine_EventBegin();

		*mode = 0;
		Engine_ActorSetAnimation(11, 7);

		record->y.fixed += 0xffff0000;
		record->terrain_height += 0xffff0000;
		Engine_TaskWait(2);

		record->y.fixed += 0xffff0000;
		record->terrain_height += 0xffff0000;
		Engine_TaskWait(10);

		record->y.fixed += 0x10000;
		record->terrain_height += 0x10000;
		Engine_TaskWait(4);

		record->y.fixed += 0x10000;
		record->terrain_height += 0x10000;

		*mode = saved;
		Engine_EventEnd();
	}
}

void SceneActor_MarkActorThirteenTileAndPark(void)
{
	s32 x;
	s32 z;

	Engine_EventBegin();

	x = Object_GetById(13)->x.fixed >> 20;
	z = Object_GetById(13)->z.fixed >> 20;

	StagedActor_FillGridAttributeRectangle(2, x, z, 1, 1, 0xff);
	StagedActor_FillGridAttributeRectangle(2, x + 1, z, 1, 1, 0);
	StagedActor_FillGridAttributeRectangle(2, x - 1, z, 1, 1, 0);
	StagedActor_FillGridAttributeRectangle(2, x, z + 1, 1, 1, 0);
	StagedActor_FillGridAttributeRectangle(2, x, z - 1, 1, 1, 0);

	if (x == 45 && z == 6)
	{
		u8 *record = (u8 *)Object_GetById(13);

		((struct FieldActor *)record)->motion_flags = 0;
		((struct FieldActor *)record)->terrain_height = 0xfffe0000;
		((struct FieldActor *)record)->y.fixed = 0xfffe0000;
	}

	Engine_EventEnd();
}

void FieldScene_RunSupplementalSequenceOne(void)
{

	s32				   x;
	s32				   y;
	struct FieldActor *record;

	Engine_EventBegin();
	record = Object_GetById(14);
	x = record->x.fixed;
	record = Object_GetById(14);
	y = record->z.fixed;
	x >>= 20;
	y >>= 20;
	StagedActor_FillGridAttributeRectangle(2, x, y, 1, 1, 255);
	StagedActor_FillGridAttributeRectangle(2, x + 1, y, 1, 1, 0);
	StagedActor_FillGridAttributeRectangle(2, x - 1, y, 1, 1, 0);
	StagedActor_FillGridAttributeRectangle(2, x, y + 1, 1, 1, 0);
	StagedActor_FillGridAttributeRectangle(2, x, y - 1, 1, 1, 0);
	record = Object_GetById(14);
	if ((record->z.fixed >> 20) == 27)
	{
		record = Object_GetById(14);
		record->motion_flags = 0;
		record->terrain_height = -0x20000;
		record->y.fixed = -0x20000;
		Engine_GameFlagSet(0x214);
		StagedActor_FillGridAttributeRectangle(2, 43, 23, 1, 1, 255);
	}
	Engine_EventEnd();
}

void FieldScene_RunScene39fSequenceA(void)
{

	struct FieldActor *rec7;
	s32				   big;
	struct FieldActor *first;
	s32				   shown;
	struct FieldActor *second;

	rec7 = Object_GetById(15);
	big = 0x80000;
	Engine_EventBegin();
	FieldScene_RunSixCallSetupSequence(15, 0);
	MogoruMori_HopActorTo(15, 0x1d8, 104, big);
	Engine_EventWait(10);
	Effect_Spawn(rec7->x.fixed, rec7->y.fixed, (rec7->z.fixed + big), 0, 0, 0, 1, 0);
	Engine_CameraFollowActor(15, 1);
	Engine_ActorFaceEachOther(15, ACTOR_PARTY_LEADER, 0);
	Engine_EventWait(30);
	Engine_ActorStartRepeatedMotion(15, 2);
	Engine_ActorShowEmote(15, 0x103, 0);
	Engine_AudioPlayCue(147);
	Engine_EventWait(60);
	first = Object_GetById(ACTOR_PARTY_LEADER);
	shown = first->x.part.pixel;
	second = Object_GetById(ACTOR_PARTY_LEADER);
	MogoruMori_HopActorTo(15, shown, second->z.part.pixel, 0x60000);
	Engine_EventWait(10);
	Engine_GameFlagSet(0x307);
	Battle_SetStart(3);
	BattleFx_SetWeightedResult(53, 0);
	Engine_EventEnd();
}

void FieldScene_RunSlot16WaypointSequence(void)
{
	struct FieldActor *slot;

	slot = Object_GetById(16);

	Engine_EventBegin();

	FieldScene_RunSixCallSetupSequence(16, 1);
	MogoruMori_HopActorTo(16, 456, 152, 0x60000);
	Engine_EventWait(10);

	Effect_Spawn(slot->x.fixed, slot->y.fixed, slot->z.fixed + 0x40000, 0, 0, 0, 1, 0);

	Engine_CameraFollowActor(16, 1);
	Engine_ActorFaceEachOther(16, ACTOR_PARTY_LEADER, 0);
	Engine_EventWait(20);
	Engine_ActorStartRepeatedMotion(16, 2);
	Engine_ActorSetAttachedEffect(16, 258);
	Engine_EventWait(60);

	MogoruMori_HopActorTo(16, 448, 192, 0x30000);
	Engine_ActorFaceActor(ACTOR_PARTY_LEADER, 16, 0);
	Engine_EventWait(6);

	MogoruMori_HopActorTo(16, 424, 208, 0x30000);
	Engine_ActorFaceActor(ACTOR_PARTY_LEADER, 16, 0);
	Engine_EventWait(6);

	MogoruMori_HopActorTo(16, 424, 224, 0x30000);
	Engine_ActorFaceActor(ACTOR_PARTY_LEADER, 16, 0);
	Engine_EventWait(6);

	Engine_CameraFollowActor(ACTOR_PARTY_LEADER, 1);
	Engine_ActorSetPosition(16, 0, 0);
	Engine_EventWait(30);
	Engine_GameFlagSet(776);
	Engine_ActorSetPosition(20, 0, 0);

	Engine_EventEnd();
}

void FieldScene_RunActor17CameraSequence(void)
{
	struct FieldActor *slot;

	slot = Object_GetById(17);

	Engine_EventBegin();

	FieldScene_RunSixCallSetupSequence(17, 1);
	MogoruMori_HopActorTo(17, 392, 104, 0x60000);
	Engine_EventWait(10);

	Effect_Spawn(slot->x.fixed, slot->y.fixed, slot->z.fixed + 0x40000, 0, 0, 0, 1, 0);

	Engine_CameraFollowActor(17, 1);
	Engine_ActorFaceEachOther(17, ACTOR_PARTY_LEADER, 0);
	Engine_EventWait(20);
	Engine_ActorStartRepeatedMotion(17, 2);
	Engine_ActorSetAttachedEffect(17, 258);
	Engine_EventWait(60);

	MogoruMori_HopActorTo(17, 376, 152, 0x60000);
	Engine_ActorFaceActor(ACTOR_PARTY_LEADER, 17, 0);
	Engine_EventWait(10);

	MogoruMori_HopActorTo(17, 328, 160, 0x30000);
	Engine_ActorFaceActor(ACTOR_PARTY_LEADER, 17, 0);
	Engine_EventWait(6);

	MogoruMori_HopActorTo(17, 296, 160, 0x30000);
	Engine_ActorFaceActor(ACTOR_PARTY_LEADER, 17, 0);
	Engine_EventWait(6);

	Engine_CameraFollowActor(ACTOR_PARTY_LEADER, 1);
	Engine_ActorSetPosition(17, 0, 0);
	Engine_EventWait(30);
	Engine_GameFlagSet(0x309);
	Engine_ActorSetPosition(21, 0, 0);

	Engine_EventEnd();
}

void MogoruMori_TransferActorEighteen(void)
{
	u32 i;
	s32 record;

	Engine_EventBegin();
	FieldScene_RunSixCallSetupSequence(18, 1);
	Camera_MoveTo(0x2e80000, -1, 0x1f80000, 1);
	MogoruMori_HopActorTo(18, 0x2e8, 0x1f8, 0x90000);
	MogoruMori_SpawnPuffRing(18);
	Actor_SetChildValue(18, 15);
	record = Actor_Get(18);
	Engine_ActorSetSpriteFlags(record, 0);
	Engine_EventWait(30);
	GameFlag_Set(0x30a);
	Actor_SetPosition(22, 0x2e80000, 0x1f80000);
	Engine_EventEnd();
}

void FieldScene_RunActorEighteenEffectSequence(void)
{
	struct FieldActor *slot;

	slot = Object_GetById(18);

	Engine_EventBegin();

	FieldScene_RunSixCallSetupSequence(18, 1);
	MogoruMori_HopActorTo(18, 712, 536, 0x60000);
	Engine_EventWait(10);

	Effect_Spawn(slot->x.fixed, slot->y.fixed, slot->z.fixed + 0x40000, 0, 0, 0, 1, 0);

	Engine_CameraFollowActor(18, 1);
	Engine_ActorFaceEachOther(18, ACTOR_PARTY_LEADER, 0);
	Engine_EventWait(20);
	Engine_ActorStartRepeatedMotion(18, 2);
	Engine_ActorSetAttachedEffect(18, 258);
	Engine_EventWait(60);

	MogoruMori_HopActorTo(18, 712, 568, 0x60000);
	Engine_ActorFaceActor(ACTOR_PARTY_LEADER, 18, 0);
	Engine_EventWait(10);

	MogoruMori_HopActorTo(18, 712, 600, 0x30000);
	Engine_ActorFaceActor(ACTOR_PARTY_LEADER, 18, 0);
	Engine_EventWait(6);

	MogoruMori_HopActorTo(18, 736, 640, 0x30000);
	Engine_ActorFaceActor(ACTOR_PARTY_LEADER, 18, 0);
	Engine_EventWait(6);

	MogoruMori_HopActorTo(18, 736, 704, 0x30000);
	Engine_ActorFaceActor(ACTOR_PARTY_LEADER, 18, 0);
	Engine_EventWait(6);

	Engine_CameraFollowActor(ACTOR_PARTY_LEADER, 1);
	Engine_ActorSetPosition(18, 0, 0);
	Engine_EventWait(30);
	Engine_GameFlagSet(0x30b);

	Engine_EventEnd();
}

void MogoruMori_RunActorEighteenEscape(void)
{
	struct FieldActor *rec7;

	rec7 = Object_GetById(18);
	Engine_EventBegin();
	Actor_SetPosition(18, 0x880000, 0x1680000);
	FieldScene_RunSixCallSetupSequence(18, 1);
	MogoruMori_HopActorTo(18, 136, 0x198, 0x80000);
	Engine_EventWait(10);
	Effect_Spawn(rec7->x.fixed, rec7->y.fixed, (rec7->z.fixed + 0x40000), 0, 0, 0, 1, 0);
	Actor_FaceDirection(18, 0xc000, 40);
	Actor_SetAttachedEffect(18, 0x102);
	Engine_ActorRunRepeatedMotion(18, 2);
	Engine_CameraFollowActor(18, 1);
	MogoruMori_HopActorTo(18, 136, 0x1b8, 0x60000);
	Actor_FaceActor(ACTOR_PARTY_LEADER, 18, 0);
	Engine_EventWait(10);
	MogoruMori_HopActorTo(18, 136, 0x1d8, 0x30000);
	Actor_FaceActor(ACTOR_PARTY_LEADER, 18, 0);
	Engine_EventWait(6);
	MogoruMori_HopActorTo(18, 136, 0x1f8, 0x30000);
	Actor_FaceActor(ACTOR_PARTY_LEADER, 18, 0);
	Engine_EventWait(6);
	Engine_CameraFollowActor(ACTOR_PARTY_LEADER, 1);
	Actor_SetPosition(18, 0, 0);
	Engine_EventWait(60);
	GameFlag_Set(0x89d);
	Engine_EventEnd();
}

void MogoruMori_RunClosingChoreography(void)
{
	struct Vec		   dir;
	struct Vec		  *v;
	struct FieldActor *actor;
	u32				   i;
	s32				   x;
	s32				   z;

	actor = Object_GetById(18);
	Engine_EventBegin();
	Engine_ActorSetChildValue(18, 15);
	Engine_ActorSetSpriteFlags(Object_GetById(18), 0);
	Actor_SetPosition(18, 0x880000, 0x1680000);
	Engine_CameraSetSpeed(0x8000, 0x1000);
	Camera_MoveTo(0x880000, -1, 0x1880000, 1);
	Engine_CameraWaitForMove();
	Engine_EventWait(60);
	Work_SetValuesIfNonNegative(0x50000, 0x50000, 0x10000);
	MogoruMori_SpawnPuffRing(18);
	Engine_ActorStartRepeatedMotion(0, 2);
	Work_SetValuesIfNonNegative(-1, -1, 0xe666);
	Engine_MapWaitWorkValuesBelow256();
	Engine_ActorFaceDirection(0, 0xc000, 20);
	Engine_EventWait(40);
	Work_SetValuesIfNonNegative(0x50000, 0x50000, 0x10000);
	MogoruMori_SpawnPuffRing(18);
	Work_SetValuesIfNonNegative(-1, -1, 0xe666);
	Engine_MapWaitWorkValuesBelow256();
	Engine_EventWait(40);
	MogoruMori_SpawnPuffRing(18);
	actor->scale_x = 0x13333;
	actor->scale_y = 0x13333;
	Engine_ActorSetChildValue(18, 5);
	MogoruMori_HopActorTo(18, 136, 0x188, 0xf0000);
	Engine_EventWait(15);
	Work_SetValuesIfNonNegative(0x50000, 0x50000, 0x10000);
	for (i = 0; i <= 16; i++)
	{
		v = &dir;
		v->x = Engine_MathCos(i << 12);
		v->y = 0;
		z = Engine_MathSin(i << 12);
		x = v->x;
		v->z = z;
		x += x / 2;
		v->x = x;
		Effect_Spawn(actor->x.fixed, 0, actor->z.fixed, x, v->y, z, 1, NULL);
	}
	Engine_EventWait(30);
	Work_SetValuesIfNonNegative(-1, -1, 0xe666);
	Engine_MapWaitWorkValuesBelow256();
	Engine_AudioPlayCue(148);
	Engine_ActorRunRepeatedMotion(18, 2);
	Engine_EventWait(20);
	{
		s32 px = Object_GetById(0)->x.part.pixel;

		MogoruMori_HopActorTo(18, px, Object_GetById(0)->z.part.pixel - 16, 0x80000);
	}
	Engine_EventWait(10);
	Battle_SetStart(3);
	Party_SetFields1ceAnd1d0((s32)&SceneId_MogoruMori3, 15);
	BattleFx_SetWeightedResult(53, 1);
	Engine_EventEnd();
}

const struct SceneEvent *Scene_GetEvents(void)
{
	s16 selector = gGameState.scene;

	if (selector == (s32)&SceneId_MogoruMori1)
	{
		return gMogoruMoriEvents1;
	}
	if (selector == (s32)&SceneId_MogoruMori2)
	{
		return gMogoruMoriEvents2;
	}
	if (selector == (s32)&SceneId_MogoruMori3)
	{
		return gMogoruMoriEvents3;
	}
	return gMogoruMoriEventsOther;
}

s32 FieldScene_RunSceneEntryHook(void)
{
	struct FieldActor *actor;
	s32				   area;
	s32				   entry;
	s32				   pos;
	s32				   tmp;

	gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
	area = gGameState.scene;
	if (area == (s32)&SceneId_MogoruMori1)
	{
		entry = gGameState.entrance;
		switch (entry)
		{
		case 1:
		case 2:
		case 3:
		case 4:
			if (GameFlag_IsSet(0x89c) == 0)
			{
				Engine_EventBegin();
				Engine_TaskWait(1);
				Engine_ActorSetChildValue(10, 1);
				Actor_SetPosition(10, 0x5c0000, 0x780000);
				Actor_FaceDirection(10, 0xd000, 0);
				Actor_SetSpeed(0, 0x6666, 0x3333);
				Engine_ActorWalkTo(0, 136, 64);
				Engine_EventOpenScreen();
				Engine_EventWaitForScreen();
				Engine_ActorWaitForMove(0);
				Engine_EventWait(30);
				Actor_ShowEmote(10, 256, 0);
				Engine_ActorRunRepeatedMotion(10, 2);
				Engine_EventWait(30);
				MogoruMori_HopActorTo(10, 136, 116, 0x70000);
				MogoruMori_SpawnPuffRing(10);
				Engine_ActorSetChildValue(10, 15);
				Engine_ActorSetSpriteFlags(Object_GetById(10), 0);
				Engine_GameFlagSet(0x89c);
				Engine_EventWait(60);
				Engine_EventEnd();
			}
			if (Engine_GameFlagIsSet(0x109) == 0)
			{
				break;
			}
			if (Engine_GameFlagIsSet(768) != 0)
			{
				break;
			}
			Engine_ActorSetChildValue(10, 15);
			Actor_SetPosition(10, 0x880000, 0x740000);
			break;

		case 7:
		case 8:
		case 9:
			actor = Object_GetById(0);
			if (actor != 0)
			{
				Engine_ActorSetPosition(16, actor->x.fixed, actor->z.fixed);
			}
			actor = Object_GetById(16);
			actor->update = 0;
			if (Engine_GameFlagIsSet(0x109) != 0)
			{
				actor = Object_GetById(16);
				actor->y.fixed = 0x200000;
			}
			Engine_TaskWait(1);
			Engine_ActorSetPosition(16, 0x2780000, 0x1b80000);
			if (Engine_GameFlagIsSet(0xfd4) == 0)
			{
				InitializeStagedActorSceneOrbitingEffect(16);
			}
			Engine_ActorSetChildValue(11, 15);
			Engine_ActorSetChildValue(12, 15);
			Engine_ActorSetSpriteFlags(Object_GetById(11), 0);
			Engine_ActorSetSpriteFlags(Object_GetById(12), 0);
			FieldScene_RedrawActorFootprint(8);
			if (Engine_GameFlagIsSet(784) == 0)
			{
				FieldScene_RedrawActorFootprint(9);
				break;
			}
			Engine_TaskWait(1);
			Actor_SetPosition(9, 0x2100000, 0x1980000);
			Engine_ActorSetAnimation(9, 4);
			Map_CopyCellAttributes(38, 27, 4, 2, 31, 25);
			Object_GetById(9)->priority_flags = 2;
			break;
		}
	}
	else if (area == (s32)&SceneId_MogoruMori2)
	{
		entry = gGameState.entrance;
		switch (entry)
		{
		case 3:
		case 4:
		case 5:
		case 6:
			if (Engine_GameFlagIsSet(0x303) == 0)
			{
				Engine_ActorSetChildValue(12, 15);
				Engine_ActorSetSpriteFlags(Object_GetById(12), 0);
			}
			if (Engine_GameFlagIsSet(772) != 0)
			{
				break;
			}
			Engine_ActorSetChildValue(13, 15);
			Engine_ActorSetSpriteFlags(Object_GetById(13), 0);
			break;

		case 10:
		case 11:
		case 12:
			if (Engine_GameFlagIsSet(0x311) == 0)
			{
				FieldScene_RedrawActorFootprint(10);
			}
			else
			{
				s32 attr = 0;

				Engine_TaskWait(1);
				Actor_SetPosition(10, 0x2280000, 0x1fe0000);
				Engine_ActorSetAnimation(10, 4);
				Object_GetById(10)->priority_flags = 2;
				Map_CopyCellAttributes(44, 30, 2, 4, 34, 30);
#if EDITION_INTERNATIONAL
				StagedActor_FillGridAttributeRectangle(0, 35, 29, 1, 4, attr);
#endif
			}
			FieldScene_RedrawActorFootprint(8);
			FieldScene_RedrawActorFootprint(9);
			pos = Object_GetById(11)->x.fixed;
			tmp = Object_GetById(11)->z.fixed;
			pos >>= 20;
			StagedActor_FillGridAttributeRectangle(2, pos, tmp >> 20, 1, 1, 255);
			Engine_TaskWait(1);
			Engine_ActorSetChildValue(11, 6);
			Object_GetById(8)->collision_flags |= 8;
			if (Engine_GameFlagIsSet(0x306) != 0)
			{
				break;
			}
			Engine_ActorSetChildValue(14, 15);
			Engine_ActorSetSpriteFlags(Object_GetById(14), 0);
			if (Engine_GameFlagIsSet(0x305) == 0)
			{
				break;
			}
			Actor_SetPosition(14, 0x1a80000, 0x1e00000);
			Actor_SetPosition(17, 0x1a80000, 0x1e00000);
			break;
		}
	}
	else if (area == (s32)&SceneId_MogoruMori3)
	{
		entry = gGameState.entrance;
		switch (entry)
		{
		case 3:
		case 4:
		case 5:
		case 6:
			Engine_TaskWait(1);
			if (Engine_GameFlagIsSet(0x307) == 0)
			{
				Engine_ActorSetChildValue(15, 15);
				Engine_ActorSetSpriteFlags(Object_GetById(15), 0);
				Engine_ActorSetSpriteFlags(Object_GetById(19), 0);
			}
			if (Engine_GameFlagIsSet(776) == 0)
			{
				Engine_ActorSetChildValue(16, 15);
				Engine_ActorSetSpriteFlags(Object_GetById(16), 0);
				Engine_ActorSetSpriteFlags(Object_GetById(20), 0);
			}
			if (Engine_GameFlagIsSet(0x309) != 0)
			{
				break;
			}
			Engine_ActorSetChildValue(17, 15);
			Engine_ActorSetSpriteFlags(Object_GetById(17), 0);
			Engine_ActorSetSpriteFlags(Object_GetById(21), 0);
			break;

		case 7:
			pos = Object_GetById(13)->x.fixed;
			tmp = Object_GetById(13)->z.fixed;
			pos >>= 20;
			StagedActor_FillGridAttributeRectangle(2, pos, tmp >> 20, 1, 1, 255);
			Engine_ActorSetChildValue(13, 6);
			Engine_TaskWait(1);
			Object_GetById(8)->collision_flags |= 8;
			FieldScene_RedrawActorFootprint(8);
			break;

		case 8:
		case 9:
		case 10:
		case 11:
			OverlayObject_CreateConfiguredObject(0x2de0000, 0, 0x1720000, 223);
			OverlayObject_CreateConfiguredObject(0x2f20000, 0, 0x1720000, 223);
			FieldScene_RedrawActorFootprint(10);
			FieldScene_RedrawActorFootprint(12);
			if (Engine_GameFlagIsSet(0x312) == 0)
			{
				FieldScene_RedrawActorFootprint(9);
			}
			else
			{
				Engine_TaskWait(1);
				Engine_ActorSetAnimation(9, 4);
				Engine_ActorSetPosition(9, 0x2ba0000, 0x18e0000);
				Object_GetById(9)->priority_flags |= 2;
				Map_CopyCellAttributes(26, 20, 2, 4, 42, 23);
				Engine_GameFlagSet(532);
				Actor_SetPosition(14, 0x2780000, 0x1b80000);
				Engine_ActorSetSpriteFlags(Object_GetById(14), 0);
			}
			if (Engine_GameFlagIsSet(0x313) == 0)
			{
				FieldScene_RedrawActorFootprint(11);
			}
			else
			{
				Engine_TaskWait(1);
				Engine_ActorSetAnimation(11, 4);
				Engine_ActorSetPosition(11, 0x29a0000, 0x2260000);
				Object_GetById(11)->priority_flags = 2;
				Map_CopyCellAttributes(26, 20, 2, 4, 40, 32);
			}
			pos = Object_GetById(14)->x.fixed;
			tmp = Object_GetById(14)->z.fixed;
			pos >>= 20;
			StagedActor_FillGridAttributeRectangle(2, pos, tmp >> 20, 1, 1, 255);
			Engine_ActorSetChildValue(14, 6);
			Engine_TaskWait(1);
			Object_GetById(9)->collision_flags |= 8;
			if (Engine_GameFlagIsSet(0x30b) == 0)
			{
				Engine_ActorSetChildValue(18, 15);
				Engine_ActorSetSpriteFlags(Object_GetById(18), 0);
				if (Engine_GameFlagIsSet(0x30a) != 0)
				{
					Actor_SetPosition(22, 0x2e80000, 0x1f80000);
					Actor_SetPosition(18, 0x2e80000, 0x1f80000);
				}
#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) \
	|| defined(TBS_EDITION_IT)
			}
			else
			{
				Engine_ActorSetPosition(18, 0, 0);
#endif
			}
			FieldScene_RunSupplementalSequenceOne();
			break;

		case 12:
		case 13:
			Engine_ActorEnableActionCallback(18, MogoruMori_ActorScript);
			if (Engine_GameFlagIsSet(0x893) == 0)
			{
				break;
			}
			if (Engine_GameFlagIsSet(0x89e) == 0)
			{
				break;
			}
			Engine_GameFlagSet(0x88f);
			break;

		case 15:
			Engine_GameFlagSet(0x89e);
			break;
		}
	}
	return 0;
}

// ここに共通ソースが入る（ROM の並びのとおり）
#include "../common/object/orbit.c"

s16 StagedActor_DirectionSteps[] = {
	0, 16,
	0, 16,
	0, 16,
	16, 0,
	16, 0,
	16, 0,
	16, 0,
	0, -16,
	0, -16,
	0, -16,
	0, -16,
	-16, 0,
	-16, 0,
	-16, 0,
	-16, 0,
	0, 16,
};

u32 StagedActor_FootprintKinds[] = {
	207, 205, 228, 229, 298, 297,
};

s32 StagedActor_FootprintBounds[] = {
	-32, -8, 32, 8,
	-8, -32, 8, 32,
	-32, -16, 32, 0,
	-8, -32, 8, 32,
	-32, -8, 32, 8,
	-8, -32, 8, 32,
};

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

s32 gMogoruMoriPlacements2_0[] = {
	SC_ARG(2),
	SC_SET(0x23, 6),
	SC_CALL(MogoruMori_CopyTimerToChild),
	SC_END,
};

s32 MogoruMori_ActorScript[] = {
	SC_SET(SW_SCALE_X, 0xcccc),
	SC_SET(SW_SCALE_Y, 0xcccc),
	SC_SET(0x23, 15),
	SC_CALL(MogoruMori_CopyTimerToChild),
	SC_CALL(OverlayObject_ApplyZero),
	SC_END,
};

s32 gMogoruMoriPlacements1_1[] = {
	SC_SET(SW_MOTION, 0),
	SC_SET(SW_Y, 0),
	SC_SET(SW_TARGET_Y, PIXELS(-32768)),
	SC_CALL(OverlayObject_ApplyZero),
	SC_WAIT(2),
	SC_END,
};

struct SceneEntrance gMogoruMoriEntrances1[] = {
	{ 1, CONDITION_ALWAYS, 136, 0, 24, FACING_SOUTH, 0, 0, 0, 272, 224, 0 },
	{ 2, CONDITION_ALWAYS, 24, 0, 128, FACING_EAST, 0, 0, 0, 272, 224, 0 },
	{ 3, CONDITION_ALWAYS, 248, 0, 128, FACING_WEST, 0, 0, 0, 272, 224, 0 },
	{ 4, CONDITION_ALWAYS, 136, 0, 200, FACING_NORTH, 0, 0, 0, 272, 224, 0 },
	{ 5, CONDITION_ALWAYS, 72, 0, 264, FACING_SOUTH, 0, 0, 240, 272, 464, 0 },
	{ 6, CONDITION_ALWAYS, 200, 0, 440, FACING_NORTH, 0, 0, 240, 272, 464, 0 },
	{ 7, CONDITION_ALWAYS, 416, 0, 264, FACING_SOUTH, 0, 288, 240, 672, 624, 0 },
	{ 8, CONDITION_ALWAYS, 560, 0, 264, FACING_SOUTH, 0, 288, 240, 672, 624, 0 },
	{ 9, CONDITION_ALWAYS, 648, 0, 344, FACING_WEST, 0, 288, 240, 672, 624, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEntrance gMogoruMoriEntrances2[] = {
	{ 0, CONDITION_ALWAYS, 120, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 1, CONDITION_ALWAYS, 24, 0, 128, FACING_EAST, 0, 0, 0, 272, 224, 0 },
	{ 2, CONDITION_ALWAYS, 248, 0, 128, FACING_WEST, 0, 0, 0, 272, 224, 0 },
	{ 3, CONDITION_ALWAYS, 424, 0, 24, FACING_SOUTH, 0, 288, 0, 560, 224, 0 },
	{ 4, CONDITION_ALWAYS, 312, 0, 136, FACING_EAST, 0, 288, 0, 560, 224, 0 },
	{ 5, CONDITION_ALWAYS, 536, 0, 120, FACING_WEST, 0, 288, 0, 560, 224, 0 },
	{ 6, CONDITION_ALWAYS, 424, 0, 200, FACING_NORTH, 0, 288, 0, 560, 224, 0 },
	{ 7, CONDITION_ALWAYS, 744, 0, 24, FACING_SOUTH, 0, 576, 0, 848, 224, 0 },
	{ 8, CONDITION_ALWAYS, 744, 0, 200, FACING_NORTH, 0, 576, 0, 848, 224, 0 },
	{ 9, CONDITION_ALWAYS, 24, 0, 368, FACING_EAST, 0, 0, 240, 272, 464, 0 },
	{ 10, CONDITION_ALWAYS, 488, 0, 264, FACING_SOUTH, 0, 288, 240, 736, 624, 0 },
	{ 11, CONDITION_ALWAYS, 312, 0, 528, FACING_EAST, 0, 288, 240, 736, 624, 0 },
	{ 12, CONDITION_ALWAYS, 360, 0, 600, FACING_NORTH, 0, 288, 240, 736, 624, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEntrance gMogoruMoriEntrances3[] = {
	{ 0, CONDITION_ALWAYS, 172, 0, 160, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 1, CONDITION_ALWAYS, 24, 0, 128, FACING_EAST, 0, 0, 0, 272, 224, 0 },
	{ 2, CONDITION_ALWAYS, 248, 0, 128, FACING_WEST, 0, 0, 0, 272, 224, 0 },
	{ 3, CONDITION_ALWAYS, 424, 0, 24, FACING_SOUTH, 0, 288, 0, 560, 224, 0 },
	{ 4, CONDITION_ALWAYS, 312, 0, 160, FACING_EAST, 0, 288, 0, 560, 224, 0 },
	{ 5, CONDITION_ALWAYS, 536, 0, 160, FACING_WEST, 0, 288, 0, 560, 224, 0 },
	{ 6, CONDITION_ALWAYS, 424, 0, 200, FACING_NORTH, 0, 288, 0, 560, 224, 0 },
	{ 7, CONDITION_ALWAYS, 824, 0, 136, FACING_WEST, 0, 576, 0, 848, 224, 0 },
	{ 8, CONDITION_ALWAYS, 728, 0, 264, FACING_SOUTH, 0, 400, 240, 848, 640, 0 },
	{ 9, CONDITION_ALWAYS, 824, 0, 512, FACING_WEST, 0, 400, 240, 848, 640, 0 },
	{ 10, CONDITION_ALWAYS, 824, 0, 608, FACING_WEST, 0, 400, 240, 848, 640, 0 },
	{ 11, CONDITION_ALWAYS, 736, 0, 616, FACING_NORTH, 0, 400, 240, 848, 640, 0 },
	{ 12, CONDITION_ALWAYS, 264, 0, 264, FACING_SOUTH, 0, 0, 240, 384, 496, 0 },
	{ 13, CONDITION_ALWAYS, 136, 0, 472, FACING_NORTH, 0, 0, 240, 384, 496, 0 },
	{ 15, CONDITION_ALWAYS, 136, 0, 432, FACING_NORTH, 0, 0, 240, 384, 496, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEntrance gMogoruMoriEntrancesOther[] = {
	{ 1, CONDITION_ALWAYS, 312, 0, 128, FACING_EAST, 0, 288, 0, 560, 224, 0 },
	{ 2, CONDITION_ALWAYS, 248, 0, 368, FACING_WEST, 0, 0, 240, 272, 464, 0 },
	{ 3, CONDITION_ALWAYS, 424, 0, 440, FACING_NORTH, 0, 288, 240, 560, 464, 0 },
	{ 4, CONDITION_ALWAYS, 712, 0, 264, FACING_SOUTH, 0, 576, 240, 848, 464, 0 },
	{ 5, CONDITION_ALWAYS, 136, 0, 680, FACING_NORTH, 0, 0, 480, 272, 704, 0 },
	{ 6, CONDITION_ALWAYS, 312, 0, 552, FACING_EAST, 0, 288, 480, 560, 704, 0 },
	{ 7, CONDITION_ALWAYS, 312, 0, 648, FACING_EAST, 0, 288, 480, 560, 704, 0 },
	{ 8, CONDITION_ALWAYS, 648, 0, 680, FACING_NORTH, 0, 576, 480, 848, 704, 0 },
	{ 9, CONDITION_ALWAYS, 536, 0, 128, FACING_WEST, 0, 288, 0, 560, 224, 0 },
	{ 10, CONDITION_ALWAYS, 24, 0, 368, FACING_EAST, 0, 0, 240, 272, 464, 0 },
	{ SCENE_TABLE_END },
};

u32 MogoruMori_SceneTable[] = {
	SCENE_EXITS(68),
	SCENE_EXIT(1, 2, 13),
	SCENE_EXIT(2, 71, 2),
	SCENE_EXIT(3, 71, 1),
	SCENE_EXIT(4, 68, 5),
	SCENE_EXIT(5, 68, 4),
	SCENE_EXIT(6, 68, 7),
	SCENE_EXIT(7, 68, 6),
	SCENE_EXIT(8, 71, 8),
	SCENE_EXIT(9, 69, 1),
	SCENE_EXIT(0, 69, 0),
	SCENE_EXIT(1, 68, 9),
	SCENE_EXIT(2, 69, 4),
	SCENE_EXIT(3, 71, 3),
	SCENE_EXIT(4, 69, 2),
	SCENE_EXIT(5, 69, 9),
	SCENE_EXIT(6, 69, 7),
	SCENE_EXIT(7, 69, 6),
	SCENE_EXIT(8, 69, 10),
	SCENE_EXIT(9, 69, 5),
	SCENE_EXIT(10, 69, 8),
	SCENE_EXIT(11, 70, 2),
	SCENE_EXIT(12, 71, 4),
	SCENE_EXIT(0, 70, 0),
	SCENE_EXIT(1, 70, 5),
	SCENE_EXIT(2, 69, 11),
	SCENE_EXIT(3, 71, 5),
	SCENE_EXIT(4, 70, 7),
	SCENE_EXIT(5, 70, 1),
	SCENE_EXIT(6, 70, 8),
	SCENE_EXIT(7, 70, 4),
	SCENE_EXIT(8, 70, 6),
	SCENE_EXIT(9, 71, 6),
	SCENE_EXIT(10, 71, 7),
	SCENE_EXIT(11, 70, 12),
	SCENE_EXIT(12, 70, 11),
	SCENE_EXIT(13, 2, 43),
	SCENE_EXIT(0, 71, 0),
	SCENE_EXIT(1, 68, 3),
	SCENE_EXIT(2, 68, 2),
	SCENE_EXIT(3, 69, 3),
	SCENE_EXIT(4, 69, 12),
	SCENE_EXIT(5, 70, 3),
	SCENE_EXIT(6, 70, 9),
	SCENE_EXIT(7, 70, 10),
	SCENE_EXIT(8, 68, 8),
	SCENE_EXIT(11, 71, 1),
	SCENE_EXIT(12, 71, 2),
	SCENE_EXIT(13, 71, 3),
	SCENE_EXIT(14, 71, 4),
	SCENE_EXIT(15, 71, 5),
	SCENE_EXIT(16, 71, 6),
	SCENE_EXIT(17, 71, 7),
	SCENE_EXIT(18, 71, 8),
	SCENE_EXIT(19, 71, 9),
	SCENE_EXIT(20, 71, 10),
	SCENE_EXITS_END,
};

struct ScenePlacement gMogoruMoriPlacements1[] = {
	{ 0xcd, CONDITION_ALWAYS, 7,
		PIXELS(488), 0, PIXELS(464), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xcf, CONDITION_ALWAYS, 7,
		PIXELS(528), 0, PIXELS(504), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xbf, 0x300, (s32)MogoruMori_ActorScript,
		PIXELS(136), 0, PIXELS(120), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xbf, 0x301, (s32)MogoruMori_ActorScript,
		PIXELS(376), 0, PIXELS(432), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xbf, 0x302, (s32)MogoruMori_ActorScript,
		PIXELS(504), 0, PIXELS(312), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x16, 0x300, (s32)gMogoruMoriPlacements1_1,
		PIXELS(136), 0, PIXELS(120), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x16, 0x301, (s32)gMogoruMoriPlacements1_1,
		PIXELS(376), 0, PIXELS(432), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0x16, 0x302, (s32)gMogoruMoriPlacements1_1,
		PIXELS(504), 0, PIXELS(312), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0x16, 0xfd4, 7,
		PIXELS(632), 0, PIXELS(440), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ SCENE_TABLE_END },
};

struct ScenePlacement gMogoruMoriPlacements2[] = {
	{ 0xcf, CONDITION_ALWAYS, 7,
		PIXELS(592), 0, PIXELS(360), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xcf, CONDITION_ALWAYS, 7,
		PIXELS(592), 0, PIXELS(456), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xcd, CONDITION_ALWAYS, 7,
		PIXELS(648), 0, PIXELS(512), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x125, CONDITION_ALWAYS, (s32)gMogoruMoriPlacements2_0,
		PIXELS(584), 0, PIXELS(376), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xbf, 0x303, (s32)MogoruMori_ActorScript,
		PIXELS(360), 0, PIXELS(80), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xbf, 0x304, (s32)MogoruMori_ActorScript,
		PIXELS(488), 0, PIXELS(80), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xbf, 0x306, (s32)MogoruMori_ActorScript,
		PIXELS(504), 0, PIXELS(480), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x16, 0x303, (s32)gMogoruMoriPlacements1_1,
		PIXELS(360), 0, PIXELS(80), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x16, 0x304, (s32)gMogoruMoriPlacements1_1,
		PIXELS(488), 0, PIXELS(80), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0x16, 0x306, (s32)gMogoruMoriPlacements1_1,
		PIXELS(504), 0, PIXELS(480), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0x5a, 0x32, ACTOR_STAND,
		PIXELS(488), 0, PIXELS(568), FACING_EAST, TALK_FACE_PARTY, 0 },
	{ SCENE_TABLE_END },
};

struct ScenePlacement gMogoruMoriPlacements3[] = {
	{ 0xcd, CONDITION_ALWAYS, 7,
		PIXELS(680), 0, PIXELS(96), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xcd, CONDITION_ALWAYS, 7,
		PIXELS(616), 0, PIXELS(400), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xcd, CONDITION_ALWAYS, 7,
		PIXELS(552), 0, PIXELS(480), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xcd, CONDITION_ALWAYS, 7,
		PIXELS(600), 0, PIXELS(544), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xcf, CONDITION_ALWAYS, 7,
		PIXELS(544), 0, PIXELS(536), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x125, CONDITION_ALWAYS, (s32)gMogoruMoriPlacements2_0,
		PIXELS(696), 0, PIXELS(120), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x125, CONDITION_ALWAYS, (s32)gMogoruMoriPlacements2_0,
		PIXELS(632), 0, PIXELS(408), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0xbf, 0x307, (s32)MogoruMori_ActorScript,
		PIXELS(488), 0, PIXELS(88), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xbf, 0x308, (s32)MogoruMori_ActorScript,
		PIXELS(424), 0, PIXELS(120), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xbf, 0x309, (s32)MogoruMori_ActorScript,
		PIXELS(360), 0, PIXELS(80), FACING_SOUTH, TALK_KEEP_FACING, 0 },
#if defined(TBS_EDITION_EN)
	{ 0xbf, 0x30b, (s32)MogoruMori_ActorScript,
		PIXELS(744), 0, PIXELS(400), FACING_SOUTH, TALK_KEEP_FACING, 0 },
#else
	{ 0xbf, CONDITION_ALWAYS, (s32)MogoruMori_ActorScript,
		PIXELS(744), 0, PIXELS(400), FACING_SOUTH, TALK_KEEP_FACING, 0 },
#endif
	{ 0x16, 0x307, (s32)gMogoruMoriPlacements1_1,
		PIXELS(488), 0, PIXELS(88), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x16, 0x308, (s32)gMogoruMoriPlacements1_1,
		PIXELS(424), 0, PIXELS(120), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0x16, 0x309, (s32)gMogoruMoriPlacements1_1,
		PIXELS(360), 0, PIXELS(80), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
#if !EDITION_INTERNATIONAL
	{ 0x16, CONDITION_ALWAYS, (s32)gMogoruMoriPlacements1_1,
		PIXELS(744), 0, PIXELS(400), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
#else
	{ 0x16, 0x30b, (s32)gMogoruMoriPlacements1_1,
		PIXELS(744), 0, PIXELS(400), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
#endif
	{ SCENE_TABLE_END },
};

struct ScenePlacement gMogoruMoriPlacementsOther[] = {
	{ SCENE_TABLE_END },
};

struct SceneEvent gMogoruMoriEvents1[] = {
	{ EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
	{ EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
	{ EVENT_EXIT, 3, CONDITION_ALWAYS, 3 },
	{ EVENT_EXIT, 4, CONDITION_ALWAYS, 4 },
	{ EVENT_EXIT, 5, CONDITION_ALWAYS, 5 },
	{ EVENT_EXIT, 6, CONDITION_ALWAYS, 6 },
	{ EVENT_EXIT, 7, CONDITION_ALWAYS, 7 },
	{ EVENT_EXIT, 8, CONDITION_ALWAYS, 8 },
	{ EVENT_EXIT, 9, CONDITION_ALWAYS, 9 },
	{ 0x202, 20, CONDITION_ALWAYS, EVENT_SCRIPT(MogoruMori_RunProbedActorNineScene) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 25, CONDITION_ALWAYS, EVENT_SCRIPT(SceneActor_PassOffsetPointOfActorZero) },
	{ TOUCH_WALKING_INTO(FACING_SOUTH), 26, CONDITION_ALWAYS, EVENT_SCRIPT(SceneActor_BobActorZeroWhenAheadClear) },
	{ SEARCH(SEARCH_UNNAMED), 27, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunScriptedSteps0And17E6) },
	{ PSYNERGY_ON_ACTOR(0x8e), 13, 0x300, EVENT_SCRIPT(FieldScene_RunActor10WaypointSequence) },
	{ PSYNERGY_ON_ACTOR(0x8e), 14, 0x301, EVENT_SCRIPT(FieldScene_RunActorElevenPresentationBeat) },
	{ PSYNERGY_ON_ACTOR(0x8e), 15, 0x302, EVENT_SCRIPT(SceneActor_RunActorTwelveThreeWaypointMotion) },
	{ PSYNERGY_ON_ACTOR(ABILITY_CATCH), 16, 0xfd4, EVENT_SCRIPT(MogoruMori_AwardHiddenNut) },
	{ SCENE_EVENTS_END },
};

struct SceneEvent gMogoruMoriEvents2[] = {
	{ EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
	{ EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
	{ EVENT_EXIT, 3, CONDITION_ALWAYS, 3 },
	{ EVENT_EXIT, 4, CONDITION_ALWAYS, 4 },
	{ EVENT_EXIT, 5, CONDITION_ALWAYS, 5 },
	{ EVENT_EXIT, 6, CONDITION_ALWAYS, 6 },
	{ EVENT_EXIT, 7, CONDITION_ALWAYS, 7 },
	{ EVENT_EXIT, 8, CONDITION_ALWAYS, 8 },
	{ EVENT_EXIT, 9, CONDITION_ALWAYS, 9 },
	{ EVENT_EXIT, 10, CONDITION_ALWAYS, 10 },
	{ EVENT_EXIT, 11, CONDITION_ALWAYS, 11 },
	{ EVENT_EXIT, 12, CONDITION_ALWAYS, 12 },
	{ 0x202, 20, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunProbedActorEightOrTenScene) },
	{ PSYNERGY_ON_ACTOR(0x8c), 11, CONDITION_ALWAYS, EVENT_SCRIPT(SceneState_ApplyCrossRectsAroundActor11) },
	{ PSYNERGY_ON_ACTOR(0x8e), 15, 0x303, EVENT_SCRIPT(MogoruMori_RunBranchHopScene) },
	{ PSYNERGY_ON_ACTOR(0x8e), 16, 0x304, EVENT_SCRIPT(FieldScene_RunActorThirteenPresentationBeat) },
	{ PSYNERGY_ON_ACTOR(0x8e), 17, 0x305, EVENT_SCRIPT(MogoruMori_TransferActorFourteen) },
	{ PSYNERGY_ON_ACTOR(0x8e), 17, 0x306, EVENT_SCRIPT(SceneActor_RunActorFourteenFourWaypointMotion) },
	{ SEARCH(SEARCH_CHEST), 100, 0xf1b, FIND_ITEM(0xc1) },
	{ EVENT_TALK, 18, CONDITION_ALWAYS, EVENT_SCRIPT(SceneState_SetValue18Mode2) },
	{ SCENE_EVENTS_END },
};

struct SceneEvent gMogoruMoriEvents3[] = {
	{ EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
	{ EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
	{ EVENT_EXIT, 3, CONDITION_ALWAYS, 3 },
	{ EVENT_EXIT, 4, CONDITION_ALWAYS, 4 },
	{ EVENT_EXIT, 5, CONDITION_ALWAYS, 5 },
	{ EVENT_EXIT, 6, CONDITION_ALWAYS, 6 },
	{ EVENT_EXIT, 7, CONDITION_ALWAYS, 7 },
	{ EVENT_EXIT, 8, CONDITION_ALWAYS, 8 },
	{ EVENT_EXIT, 9, CONDITION_ALWAYS, 9 },
	{ EVENT_EXIT, 10, CONDITION_ALWAYS, 10 },
	{ EVENT_EXIT, 11, CONDITION_ALWAYS, 11 },
	{ EVENT_EXIT, 12, CONDITION_ALWAYS, 12 },
	{ EVENT_EXIT, 13, CONDITION_ALWAYS, 13 },
	{ 0x202, 20, CONDITION_ALWAYS, EVENT_SCRIPT(MogoruMori_RunProbedLandingScene) },
	{ TOUCH_WALKING_INTO(FACING_WEST), 25, CONDITION_ALWAYS, EVENT_SCRIPT(SceneActor_BobActorZeroWhenTargetClear) },
	{ EVENT_TOUCH, 22, 0x89d, EVENT_SCRIPT(MogoruMori_RunActorEighteenEscape) },
	{ EVENT_TOUCH, 23, 0x89e, EVENT_SCRIPT(MogoruMori_RunClosingChoreography) },
	{ PSYNERGY_ON_ACTOR(0x8c), 13, CONDITION_ALWAYS, EVENT_SCRIPT(SceneActor_MarkActorThirteenTileAndPark) },
	{ PSYNERGY_ON_ACTOR(0x8c), 14, 0x214, EVENT_SCRIPT(FieldScene_RunSupplementalSequenceOne) },
	{ PSYNERGY_ON_ACTOR(0x8e), 19, 0x307, EVENT_SCRIPT(FieldScene_RunScene39fSequenceA) },
	{ PSYNERGY_ON_ACTOR(0x8e), 20, 0x308, EVENT_SCRIPT(FieldScene_RunSlot16WaypointSequence) },
	{ PSYNERGY_ON_ACTOR(0x8e), 21, 0x309, EVENT_SCRIPT(FieldScene_RunActor17CameraSequence) },
	{ PSYNERGY_ON_ACTOR(0x8e), 22, 0x30a, EVENT_SCRIPT(MogoruMori_TransferActorEighteen) },
	{ PSYNERGY_ON_ACTOR(0x8e), 22, 0x30b, EVENT_SCRIPT(FieldScene_RunActorEighteenEffectSequence) },
	{ SEARCH(SEARCH_CHEST), 100, 0xf1a, FIND_ITEM(0x5d) },
	{ SCENE_EVENTS_END },
};

struct SceneEvent gMogoruMoriEventsOther[] = {
	{ EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
	{ EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
	{ EVENT_EXIT, 3, CONDITION_ALWAYS, 3 },
	{ EVENT_EXIT, 4, CONDITION_ALWAYS, 4 },
	{ EVENT_EXIT, 5, CONDITION_ALWAYS, 5 },
	{ EVENT_EXIT, 6, CONDITION_ALWAYS, 6 },
	{ EVENT_EXIT, 7, CONDITION_ALWAYS, 7 },
	{ EVENT_EXIT, 8, CONDITION_ALWAYS, 8 },
	{ EVENT_EXIT, 11, CONDITION_ALWAYS, 20 },
	{ EVENT_EXIT, 12, CONDITION_ALWAYS, 19 },
	{ EVENT_EXIT, 13, CONDITION_ALWAYS, 13 },
	{ EVENT_EXIT, 14, CONDITION_ALWAYS, 14 },
	{ EVENT_EXIT, 15, CONDITION_ALWAYS, 15 },
	{ EVENT_EXIT, 16, CONDITION_ALWAYS, 16 },
	{ EVENT_EXIT, 17, CONDITION_ALWAYS, 17 },
	{ EVENT_EXIT, 18, CONDITION_ALWAYS, 18 },
	{ SEARCH(SEARCH_UNNAMED), 100, 0x350, FIND_PSYNERGY_STONE },
	{ SCENE_EVENTS_END },
};
