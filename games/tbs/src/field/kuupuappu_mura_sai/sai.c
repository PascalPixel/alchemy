#include "game_state.h"
#include "window.h"
#include "unit/field/kuupuappu_mura_sai/sai.h"
#include "unit/field/common/effect/create.h"
#include "unit/field/common/object/distance.h"
#include "unit/field/common/effect/move.h"
#include "unit/field/kuupuappu_mura/scene_cells.h"
#include "unit/battle/effect/slots.h"
#include "unit/game/character/chara.h"
#include "unit/system/scheduler/callback.h"
#include "fieldobj.h"
#include "types.h"
#include "field_event.h"
#include "field_scene.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "iwram_call.h"
#include "tbs_edition.h"
#include "inventory.h"
#include "text/msg_ids.h"
#include "edition.h"
#include "call.h"

#define NULL ((void *)0)

// ここに共通ソースが入る（ROM の並びのとおり）
#include "../common/effect/create.c"

#define FIELD_AT_OFFSET(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

struct EffectRecord
{
	u8 pad[9];
	u8 flags_lo : 2;
	u8 mode : 2;
	u8 flags_hi : 4;
};

union MotionWork
{
	struct
	{
		u32	 unk_00[2];
		s32	 x, y, z;
		u32	 unk_14;
		s32	 accum_x, accum_y;
		u32	 unk_20[4];
		s32	 rate_x, rate_y;
		u32	 unk_38[3];
		s32	 velocity_x, velocity_y, velocity_z;
		u16 *record;
		u8	 unk_54[16];
		u16	 angle_step;
	} fields;
	u8 bytes[102];
};

struct SceneActor
{
	u8	unk_00[6];
	u16 facing;
	u8	unk_08[92];
	u16 state_flags;
};

struct SceneActor_02000350
{
	u8	unk_00[6];
	u16 facing;
	s32 x, y, z;
	u8	unk_14[71];
	u8	active;
};

// ここに共通ソースが入る（ROM の並びのとおり）
#include "../common/effect/move.c"
#include "../common/effect/spawnov.c"
#include "../common/object/distance.c"

s32 SceneActor_UpdateProximity(
	struct SceneActor_02000350 *actor, struct SceneActor_02000350 *target, s32 range, s32 force)
{
	s32	 result = 0;
	s32 *targetPos = &target->x;
	s32 *actorPos = &actor->x;

	if (FixedPoint_Distance(targetPos, actorPos) < range || force != 0)
	{
		u32 angle = (u16)ArcTan2(target->z - actor->z, *targetPos - *actorPos);
		u32 left = (angle - 0x1000) & 0xf000;
		u32 right = (angle + 0x1000) & 0xf000;
		u32 forward = angle & 0xf000;
		u32 facing = actor->facing & 0xf000;

		if (forward == facing || right == facing || left == facing || force != 0)
		{
			actor->active = 1;
			Object_SetMode(actor, 1);
			result = 1;
		}
	}
	else
	{
		actor->active = 0;
		Object_SetMode(actor, 2);
	}
	return result;
}

s32 UpdateActorProximity(u8 *actor)
{
	u8				 **globals = gWindowWork;
	u8				  *scene = globals[0];
	u8				  *work = globals[12];
	u16				  *flags = (u16 *)(actor + 100);
	s32				   force = 0;
	s32				   range = 18;
	struct FieldActor *partner;
	struct FieldActor *player;

	if ((*flags & 1) != 0)
	{
		partner = Object_GetById(15);
	}
	else
	{
		partner = Object_GetById(14);
	}
	if (SceneActor_UpdateProximity(actor, partner, 32, 0) != 0)
	{
		return 0;
	}

	player = Object_GetById(ACTOR_PARTY_LEADER);

	if (*(s16 *)(work + 376) != 0 || scene[RENDER_MODE_OFS] != 0)
	{
		range = 26;
		if ((*flags & 2) != 0)
		{
			force = 1;
		}
	}

	SceneActor_UpdateProximity(actor, player, range, force);
	return 0;
}

const void *SceneData_GetScriptTable(void)
{
	return KuupuappuMuraSai_Scripts;
}

int SceneData_ReturnZero(void)
{
	return 0;
}

const void *SceneData_GetMessageTable(void)
{
	return KuupuappuMuraSai_Messages;
}

const void *SceneData_GetActorTable(void)
{
	return KuupuappuMuraSai_Actors;
}

void ActorPresentation_RunActorModeOneThenZero(s32 actor)
{
	Engine_EventBegin();
	Engine_ActorSetAnimation(actor, 1);
	Engine_EventShowMessage(actor, 0);
	Engine_EventEnd();
}

void SceneDialogue_RunActor8FlagScene(void)
{
	Engine_EventBegin();
	Engine_ActorFaceActor(8, ACTOR_PARTY_LEADER, 2);
	Engine_GameFlagSet(0x305);
	Engine_EventSetMessage((s32)MsgKuupuappuWarriorsWhoCaptured);
	Engine_EventShowMessage(8, 0);
	Engine_EventEnd();
}

void SceneDialogue_RunActor11Line(void)
{

	Engine_EventSetMessage((s32)MsgKuupuappuRumorsThievesLunpa);
	Engine_ActorFaceEachOther(11, ACTOR_PARTY_LEADER, 2);
	ActorPresentation_RunActorModeOneThenZero(11);
}

void SceneDialogue_RunActor12TwoFlagScene(void)
{

	Engine_EventBegin();
	Engine_ActorFaceActor(12, ACTOR_PARTY_LEADER, 2);
	Engine_GameFlagSet(0x306);
	Engine_GameFlagSet(0x868);
	Engine_EventSetMessage((s32)MsgKuupuappuTalkingMayorStrong);
	Engine_EventShowMessage(12, 0);
	Engine_EventEnd();
}

void SceneDialogue_ShowLine1CB0ForActor13(void)
{

	Engine_EventSetMessage((s32)MsgKuupuappuVolcanoStaysCalm);
	Engine_ActorFaceEachOther(13, ACTOR_PARTY_LEADER, 2);
	ActorPresentation_RunActorModeOneThenZero(13);
}

void SceneDialogue_RunActorFourteenFlagDialogue(void)
{
	struct SceneActor *actor = Actor_Get(14);
	u16				   facing = actor->facing;
	s32				   text;

	actor->state_flags |= 2;
	Engine_EventBegin();
	text = (s32)MsgKuupuappuWarriorGuy;
	Engine_EventSetMessage(text);
	Engine_ActorSetAnimation(14, 0);
	Engine_ActorFaceEachOther(14, ACTOR_PARTY_LEADER, 2);
	if (GameFlag_IsSet(0x300) == 0)
	{
		Actor_ShowEmote(14, 256, 60);
		Event_ShowMessageAndWait(14, 0, 10);
		Event_ShowMessageAndWait(14, 0, 10);
		GameFlag_Set(0x300);
	}
	Engine_EventSetMessage(text + 2);
	Event_ShowMessageAndWait(14, 0, 10);
	actor->facing = facing;
	Engine_TaskWait(1);
	Engine_EventEnd();
	actor->state_flags = 1;
	GameFlag_Set(0x307);
}

void SceneDialogue_RunActorFifteenFacingPreservedDialogue(void)
{
	struct Actor_02000640 *actor;
	s16					   facing0;

	actor = Object_GetById(15);
	facing0 = actor->facing;
	actor->state_flags |= 2;
	Engine_EventBegin();
	Engine_EventSetMessage((s32)MsgKuupuappuWishVaultsElders);
	Engine_ActorSetAnimation(15, 0);
	Engine_ActorFaceEachOther(15, ACTOR_PARTY_LEADER, 2);
	Engine_EventShowMessageAndWait(15, 0, 10);
	actor->facing = facing0;
	Engine_TaskWait(1);
	Engine_EventEnd();
	actor->state_flags = 0;
}

void SceneDialogue_RunActor16CountedDialogue(void)
{
	Engine_EventBegin();
	Engine_EventSetMessage((s32)MsgKuupuappuGuysCheckJail);
	Engine_ActorFaceEachOther(16, ACTOR_PARTY_LEADER, 2);
	Engine_EventOpenMessage(16, 0);
	if (Engine_EventChooseYesNo(0, 0) != 0)
	{
		((struct SceneWork_020006b4 *)gEventWork)->branch_counter += 1;
	}
	Engine_EventShowMessage(16, 0);
	Engine_GameFlagSet(0x308);
	Engine_EventEnd();
}

void SceneDialogue_RunActorEightTimedDialogue(void)
{
	Engine_EventBegin();
	Engine_ActorRunRepeatedMotion(8, 1);
	Engine_EventWait(20);
	Engine_ActorFaceActor(8, ACTOR_PARTY_LEADER, 20);
	Engine_GameFlagSet(0x305);
	Engine_EventSetMessage((s32)MsgKuupuappuWarriorsWhoCaptured);
	Engine_EventShowMessageAndWait(8, 0, 20);
	Engine_EventEnd();
}

void SceneDialogue_RunActor11AcceptanceDialogue(void)
{
	Engine_EventSetMessage((s32)MsgKuupuappuOnceDodonpaTook);
	((struct Actor_02000754 *)Object_GetById(11))->accepted = 1;
	ActorPresentation_RunActorModeOneThenZero(11);
	((struct Actor_02000754 *)Object_GetById(11))->accepted = 0;
}

void SceneDialogue_RunActor12TimedTwoFlagScene(void)
{
	Engine_EventBegin();
	Engine_ActorRunRepeatedMotion(12, 1);
	Engine_EventWait(20);
	Engine_ActorFaceActor(12, ACTOR_PARTY_LEADER, 20);
	Engine_GameFlagSet(0x306);
	Engine_GameFlagSet(0x868);
	Engine_EventSetMessage((s32)MsgKuupuappuTalkingMayorStrong);
	Engine_EventShowMessageAndWait(12, 0, 20);
	Engine_EventEnd();
}

void ActorPresentation_RunActor13AcceptanceDialogue(void)
{
	Engine_EventSetMessage((s32)MsgKuupuappuGreatEverythingSolved);
	((struct Actor_020007d4 *)Object_GetById(13))->accepted = 1;
	ActorPresentation_RunActorModeOneThenZero(13);
	((struct Actor_020007d4 *)Object_GetById(13))->accepted = 0;
}

void KuupuappuMuraSai_RunActor14Talk(void)
{
	((struct SceneActor *)Actor_Get(14))->state_flags |= 2;
	Engine_EventBegin();
	if (GameFlag_IsSet(0x307) != 0)
	{
		Engine_EventSetMessage((s32)MsgKuupuappuWatchingGuysMakes);
		ActorPresentation_RunActorModeOneThenZero(14);
	}
	else
	{
		SceneDialogue_RunActorFourteenFlagDialogue();
		GameFlag_Set(0x307);
	}
	Engine_EventEnd();
	((struct SceneActor *)Actor_Get(14))->state_flags = 1;
}

void SceneDialogue_RunActorFifteenDialogue(void)
{
	{
		struct SceneActor *actor = Object_GetById(15);
		actor->state_flags |= 2;
	}
	Engine_EventBegin();
	Engine_EventSetMessage((s32)MsgKuupuappuGrownUpsAlways);
	ActorPresentation_RunActorModeOneThenZero(15);
	Engine_EventEnd();
	{
		s32				   clear = 0;
		struct SceneActor *actor = Object_GetById(15);
		actor->state_flags = clear;
	}
}

void FieldScene_RunScene385SequenceA(void)
{
	u32 i;
	s32 rec7;
	s32 record;
	s32 v5;

	rec7 = GameFlag_IsSet(0x308);
	if (rec7 == 0)
	{
		Engine_EventBegin();
		*((u8 *)Actor_Get(16) + 91) = 1;
		Engine_ActorSetAnimation(16, 1);
		Engine_ActorRunRepeatedMotion(16, 1);
		Engine_EventWait(20);
		Engine_EventSetMessage((s32)MsgKuupuappuGuysCheckJail);
		Engine_ActorFaceEachOther(16, 0, 2);
		Event_OpenMessage(16, 0);
		if (Engine_EventChooseYesNo(0, 0) != 0)
		{
			gEventWork->message++;
		}
		Event_ShowMessage(16, 0);
		*((u8 *)Actor_Get(16) + 91) = rec7;
		Engine_ActorEnableActionCallback(16, 2);
		Engine_EventEnd();
		GameFlag_Set(0x308);
	}
	else
	{
		Engine_EventSetMessage((s32)MsgKuupuappuNotLikeEasy);
		*((u8 *)Actor_Get(16) + 91) = 1;
		ActorPresentation_RunActorModeOneThenZero(16);
		v5 = 0;
		*((u8 *)Actor_Get(16) + 91) = v5;
	}
}

void ActorPresentation_MoveActorToPositionAndWait(int actor, int x, int z, int field40)
{
	u8 *record = Object_GetById(actor);
	int frames;
	Engine_ActorSetSpeed(actor, 0x30000, 0x18000);
	*(s32 *)(record + 72) = 0x8000;
	*(s32 *)(record + 68) = 0;
	*(s32 *)(record + 40) = field40;
	Engine_ActorSetSpriteFlags(record, 0);
	Engine_ActorMoveToAndWait(actor, x, z);
	Engine_ActorSetPosition(actor, x << 16, z << 16);
	for (frames = 60; frames != 0; --frames)
	{
		Engine_TaskWait(1);
		if (*(s16 *)(record + 42) == 0)
			break;
	}
	Engine_ActorSetSpriteFlags(record, 1);
	*(s32 *)(record + 72) = 0x10000;
}

void FieldScene_RunActor21SequenceOnce(void)
{
	u32 i;
	s32 record;

	Engine_EventBegin();
	Event_PlayCue(100);
	Engine_EventWait(40);
	if (GameFlag_IsSet(0x867) == 0)
	{
		Actor_SetAttachedEffect(21, 0x102);
		Engine_ActorJump(21, 4, 0);
		Engine_EventWait(12);
		Engine_ActorJump(21, 4, 0);
		Engine_EventWait(20);
		ActorPresentation_MoveActorToPositionAndWait(21, 0x188, 104, 0x70000);
		Engine_EventWait(20);
		Actor_WalkToAndWait(21, 0x198, 104);
		Actor_WalkToAndWait(21, 0x198, 120);
		GameFlag_Set(0x867);
	}
	Engine_EventEnd();
}

void SceneActor_PlaceAndSetSceneDelay(s32 x, s32 y, s32 delay)
{
	s32 zero = 0;

	Actor_SetSpeed(zero, 0x8000, 0x4000);
	Actor_WalkTo(zero, x, y);
	gEventWork->transition_frames = 16;
	Engine_EventRequestExit(delay);
}

void FieldScene_SetupScene5(void)
{
	Engine_AudioPlayCue(158);
	Engine_MapAnimateCells(KuupuappuMuraSai_Scene5Cells, 56, 19);
	SceneActor_PlaceAndSetSceneDelay(408, 320, 5);
}

void ActorPresentation_SetupActorEighteenAt312_304(void)
{
	Engine_AudioPlayCue(158);
	Engine_MapAnimateCells(KuupuappuMuraSai_Scene6Cells, 50, 18);
	SceneActor_PlaceAndSetSceneDelay(312, 304, 6);
}

void FieldScene_SetupScene7(void)
{
	Engine_AudioPlayCue(158);
	Engine_MapAnimateCells(KuupuappuMuraSai_Scene7Cells, 44, 17);
	SceneActor_PlaceAndSetSceneDelay(216, 288, 7);
}

void ActorPresentation_SetupActorZeroForSceneEight(void)
{
	struct SceneActor_020004b4 *actor = Object_GetById(ACTOR_PARTY_LEADER);
	struct Presentation		   *presentation = actor->presentation;
	u8							flags;

	Engine_AudioPlayCue(158);
	Engine_MapAnimateCells(KuupuappuMuraSai_Scene8Cells, 54, 13);
	{
		s32 cell = 23;
		s32 row = 12;

		Engine_MapCopyCellAttributes(33, 20, 1, 3, cell, row);
	}
	actor->state_23 &= ~1;
	flags = presentation->flags;
	flags |= 12;
	presentation->flags = flags;
	SceneActor_PlaceAndSetSceneDelay(376, 224, 8);
}

void ActorPresentation_SetupActorZeroForSceneNine(void)
{
	struct SceneActor_020004b4 *actor = Object_GetById(ACTOR_PARTY_LEADER);
	struct Presentation		   *presentation = actor->presentation;
	u8							flags;

	Engine_AudioPlayCue(158);
	Engine_MapAnimateCells(KuupuappuMuraSai_Scene9Cells, 49, 10);
	{
		s32 cell = 18;
		s32 row = 10;

		Engine_MapCopyCellAttributes(33, 20, 1, 3, cell, row);
	}
	actor->state_23 &= ~1;
	flags = presentation->flags;
	flags |= 12;
	presentation->flags = flags;
	SceneActor_PlaceAndSetSceneDelay(296, 176, 9);
}

void FieldScene_SetupScene10(void)
{
	Engine_AudioPlayCue(158);
	Engine_MapAnimateCells(KuupuappuMuraSai_Scene10Cells, 38, 6);
	SceneActor_PlaceAndSetSceneDelay(120, 144, 10);
}

// ここに共通ソースが入る（ROM の並びのとおり）
#include "../kuupuappu_mura/scene_cells.c"
#include "ability_ids.h"
#include "item_ids.h"
#include "objcmd.h"

void ActorPresentation_SetupActorZeroForSceneTwelve(void)
{
	struct SceneActor_02000c1c	 *actor = Object_GetById(ACTOR_PARTY_LEADER);
	struct Presentation_02000c1c *presentation = actor->presentation;
	u8							  flags;

	Engine_AudioPlayCue(158);
	Engine_MapAnimateCells(KuupuappuMuraSai_Scene12Cells, 35, 9);
	{
		s32 cell = 4;
		s32 row = 10;

		Engine_MapCopyCellAttributes(33, 20, 1, 3, cell, row);
	}
	actor->state_23 &= ~1;
	flags = presentation->flags;
	flags |= 12;
	presentation->flags = flags;
	SceneActor_PlaceAndSetSceneDelay(72, 160, 12);
}

void SceneState_SetFlag200AndConfigureRegion55_26(void)
{
	Engine_GameFlagSet(0x200);
	{
		s32 a = 23;
		s32 b = 26;
		Engine_MapCopyCellAttributes(55, 26, 4, 2, a, b);
	}
}

void ActorPresentation_SetFlag200AndSceneCell23(void)
{
	Engine_GameFlagClear(0x200);
	{
		s32 first_value = 23;
		s32 second_value = 26;
		Engine_MapCopyCellAttributes(23, 23, 4, 2, first_value, second_value);
	}
}

void FieldScene_SetActor21Values0And4(void)
{
	BattleFx_RunPageEffectForSlot(21, 0, 4);
}

void FieldScene_RunActor18MotionSequence(void)
{
	u32 i;
	s32 record;

	PartyInventory_Discard(231);
	Engine_EventBegin();
	Engine_EventWait(10);
	Engine_ActorRunRepeatedMotion(18, 2);
	Actor_SetSpeed(18, 0xcccc, 0x6666);
	Actor_WalkToAndWait(18, 216, 0x198);
	Engine_EventWait(10);
	Actor_FaceDirection(18, 0x4000, 20);
	Engine_ActorJump(18, 6, 0);
	Engine_EventWait(30);
	Engine_ActorJump(18, 6, 0);
	Engine_EventWait(30);
	Engine_ActorJump(18, 6, 0);
	Engine_EventWait(30);
	Actor_WalkToAndWait(18, 216, 0x188);
	Engine_EventWait(10);
	Actor_FaceDirection(18, 0x4000, 20);
	GameFlag_Set(0x858);
	Engine_EventEnd();
}

void ActorPresentation_SetPairedSceneCells(void)
{
	s32 v1 = 13;
	s32 v2 = 25;

	Engine_MapCopyCellAttributes(41, 43, 1, 1, v1, v2);
	Engine_MapCopyCells(40, 42, 12, 22, 3, 3);
}

void ActorPresentation_SetAlternatePairedSceneCells(void)
{
	s32 v1 = 13;
	s32 v2 = 25;

	Engine_MapCopyCellAttributes(37, 43, 1, 1, v1, v2);
	Engine_MapCopyCells(36, 42, 12, 22, 3, 3);
}

void FieldScene_RunActorEighteenDialogue(void)
{
	Engine_EventBegin();
	Engine_EventSetMessage((s32)MsgKuupuappuRuffRrruff2);
	Engine_ActorSetAnimation(18, 0);
	Engine_ActorFaceEachOther(18, ACTOR_PARTY_LEADER, 0);
	Engine_EventWait(2);
	Engine_EventShowMessage(18, 0);
	Engine_ActorSetAnimation(18, 1);

	if (PartyInventory_FindOwner(231) != -1 && Engine_GameFlagIsSet(0x858) == 0)
	{
		((struct SceneWork_02000e90 *)gEventWork)->actor18_marker = 1;
	}

	Engine_EventEnd();
}

void KuupuappuMuraSai_InspectWell(void)
{
	Engine_EventBegin();
	Engine_MessageShowCentered((s32)MsgFieldPeeredWell, 1);
	Engine_MessageShowCentered((s32)MsgKuupuappuCanHearWaterRumblingDown, 1);
	Engine_EventEnd();
}

const u8 *SceneData_GetExtraTable(void)
{
	return (const u8 *)KuupuappuMuraSai_Extras;
}

s32 SceneSetup_InitializeActorsAndFlags(void)
{

	struct FieldActor *actor;
	s16				  *scene;
	s32				   mode;

	if (Engine_GameFlagIsSet(0x200))
		Map_CopyCellAttributes(55, 26, 4, 2, 23, 26);
	OverlayObject_CreateConfiguredObject(0x800000, 0, 0x1a40000, 223);
	Engine_MapCopyCells(45, 41, 8, 45, 3, 3);
	Engine_TaskWait(1);
	actor = Object_GetById(14);
	actor->update = (void (*)(union FieldObject *))UpdateActorProximity;
	{
		struct FieldActor *actor = Object_GetById(14);
		s32				   mode = 1;
		actor->timer = mode;
	}
	mode = 0;
	actor = Object_GetById(15);
	actor->update = (void (*)(union FieldObject *))UpdateActorProximity;
	Object_GetById(15)->timer = mode;
	if (Engine_GameFlagIsSet(0x858))
		Actor_SetPosition(18, 0xd80000, 0x1880000);
	if (gGameState.entrance <= 2 && !Engine_GameFlagIsSet(52)
#if EDITION_INTERNATIONAL
		&& !Engine_GameFlagIsSet(0x109)
#endif
	)
		Engine_GameFlagClear(0x867);
	if (Engine_GameFlagIsSet(0x867) && !Engine_GameFlagIsSet(52))
		Actor_SetPosition(21, 0x1980000, 0x780000);
	scene = (s16 *)&gGameState;
	if (scene[225] == 11)
		Engine_GameFlagClear(FLAG_ARRIVAL_EVENT_PENDING);
	if (scene[225] == 13)
		Engine_GameFlagClear(0x120);
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

s32 KuupuappuMuraSai_Actor0[] = {
	SC_ARG(2),
	SC_SET(SW_SPEED, 0xcccc),
	SC_SET(SW_ACCEL, 0x3333),
	SC_LABEL(1),
	SC_MOVE(PIXELS(136), 0, PIXELS(376)),
	SC_SYNC,
	SC_MOVE(PIXELS(120), 0, PIXELS(376)),
	SC_SYNC,
	SC_MOVE(PIXELS(104), 0, PIXELS(392)),
	SC_SYNC,
	SC_MOVE(PIXELS(104), 0, PIXELS(408)),
	SC_SYNC,
	SC_MOVE(PIXELS(120), 0, PIXELS(424)),
	SC_SYNC,
	SC_MOVE(PIXELS(136), 0, PIXELS(424)),
	SC_SYNC,
	SC_MOVE(PIXELS(152), 0, PIXELS(408)),
	SC_SYNC,
	SC_MOVE(PIXELS(152), 0, PIXELS(392)),
	SC_SYNC,
	SC_LOOP(SC_FOREVER, SC_TO(1)),
	SC_END,
};

s32 KuupuappuMuraSai_Actor1[] = {
	SC_ARG(2),
	SC_SET(SW_SPEED, 0xcccc),
	SC_SET(SW_ACCEL, 0x3333),
	SC_LABEL(1),
	SC_MOVE(PIXELS(120), 0, PIXELS(424)),
	SC_SYNC,
	SC_MOVE(PIXELS(136), 0, PIXELS(424)),
	SC_SYNC,
	SC_MOVE(PIXELS(152), 0, PIXELS(408)),
	SC_SYNC,
	SC_MOVE(PIXELS(152), 0, PIXELS(392)),
	SC_SYNC,
	SC_MOVE(PIXELS(136), 0, PIXELS(376)),
	SC_SYNC,
	SC_MOVE(PIXELS(120), 0, PIXELS(376)),
	SC_SYNC,
	SC_MOVE(PIXELS(104), 0, PIXELS(392)),
	SC_SYNC,
	SC_MOVE(PIXELS(104), 0, PIXELS(408)),
	SC_SYNC,
	SC_LOOP(SC_FOREVER, SC_TO(1)),
	SC_END,
};

struct SceneEntrance KuupuappuMuraSai_Scripts[] = {
	{ 0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 1, CONDITION_ALWAYS, 400, 0, 472, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
	{ 2, CONDITION_ALWAYS, 224, 0, 138, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 5, CONDITION_ALWAYS, 408, 0, 344, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 6, CONDITION_ALWAYS, 312, 0, 328, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 7, CONDITION_ALWAYS, 216, 0, 312, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 8, CONDITION_ALWAYS, 376, 0, 248, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 9, CONDITION_ALWAYS, 296, 0, 200, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 10, CONDITION_ALWAYS, 120, 0, 168, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 11, CONDITION_ALWAYS, 64, 0, 294, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 12, CONDITION_ALWAYS, 70, 0, 183, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 13, CONDITION_ALWAYS, 217, 0, 409, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 14, CONDITION_ALWAYS, 420, 0, 136, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ SCENE_TABLE_END },
};

u32 KuupuappuMuraSai_Messages[] = {
	SCENE_EXITS(23),
	SCENE_EXIT(1, 2, 3),
	SCENE_EXIT(2, 2, 44),
	SCENE_EXIT(5, 24, 5),
	SCENE_EXIT(6, 24, 6),
	SCENE_EXIT(7, 24, 7),
	SCENE_EXIT(8, 24, 8),
	SCENE_EXIT(9, 24, 9),
	SCENE_EXIT(10, 24, 10),
	SCENE_EXIT(11, 9, 11),
	SCENE_EXIT(12, 22, 10),
	SCENE_EXIT(13, 96, 10),
	SCENE_EXIT(14, 98, 1),
	SCENE_EXITS_END,
};

struct ScenePlacement KuupuappuMuraSai_Actors[] = {
	{ 0x6c, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(381), 0, PIXELS(342), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
	{ 0x6a, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(280), 0, PIXELS(396), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x65, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(264), 0, PIXELS(150), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x73, CONDITION_ALWAYS, ACTOR_WANDER,
		PIXELS(244), 0, PIXELS(204), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
	{ 0x67, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(124), 0, PIXELS(196), FACING_SOUTHEAST, TALK_FACE_PARTY, 0 },
	{ 0x6f, CONDITION_ALWAYS, ACTOR_WANDER,
		PIXELS(244), 0, PIXELS(300), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
	{ 0x6b, CONDITION_ALWAYS, (s32)KuupuappuMuraSai_Actor0,
		PIXELS(132), 0, PIXELS(380), FACING_WEST, TALK_KEEP_FACING, 0 },
	{ 0x66, CONDITION_ALWAYS, (s32)KuupuappuMuraSai_Actor1,
		PIXELS(132), 0, PIXELS(420), FACING_EAST, TALK_KEEP_FACING, 0 },
	{ 0x74, CONDITION_ALWAYS, ACTOR_WANDER,
		PIXELS(332), 0, PIXELS(412), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
	{ 0x7a, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(92), 0, PIXELS(296), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
	{ 0x68, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(216), 0, PIXELS(440), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
	{ 0x6c, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(152), 0, PIXELS(296), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
	{ 0xf8, CONDITION_ALWAYS, 7,
		0, 0, 0, FACING_NORTH, TALK_FACE_PARTY, 0 },
	{ 0x5a, 0x34, ACTOR_STAND,
		PIXELS(344), 0, PIXELS(96), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ SCENE_TABLE_END },
};

u16 KuupuappuMuraSai_Scene5Cells[] = {
	MAP_CELL_STEP(35, 52, 2, 2, 4), MAP_CELL_STEP(33, 52, 2, 2, 4), MAP_CELL_STEPS_END,
};

u16 KuupuappuMuraSai_Scene6Cells[] = {
	MAP_CELL_STEP(35, 50, 2, 2, 4), MAP_CELL_STEP(33, 50, 2, 2, 4), MAP_CELL_STEPS_END,
};

u16 KuupuappuMuraSai_Scene7Cells[] = {
	MAP_CELL_STEP(35, 48, 2, 2, 4), MAP_CELL_STEP(33, 48, 2, 2, 4), MAP_CELL_STEPS_END,
};

u16 KuupuappuMuraSai_Scene8Cells[] = {
	MAP_CELL_STEP(42, 48, 2, 2, 4), MAP_CELL_STEP(40, 48, 2, 2, 4), MAP_CELL_STEPS_END,
};

u16 KuupuappuMuraSai_Scene9Cells[] = {
	MAP_CELL_STEP(42, 46, 2, 2, 4), MAP_CELL_STEP(40, 46, 2, 2, 4), MAP_CELL_STEPS_END,
};

u16 KuupuappuMuraSai_Scene10Cells[] = {
	MAP_CELL_STEP(35, 46, 2, 2, 4), MAP_CELL_STEP(33, 46, 2, 2, 4), MAP_CELL_STEPS_END,
};

u16 KuupuappuMuraSai_Scene12Cells[] = {
	MAP_CELL_STEP(35, 33, 2, 2, 4), MAP_CELL_STEP(33, 33, 2, 2, 4), MAP_CELL_STEPS_END,
};

struct SceneEvent KuupuappuMuraSai_Extras[] = {
	{ EVENT_EXIT, 20, CONDITION_ALWAYS, 1 },
	{ EVENT_EXIT, 21, CONDITION_ALWAYS, 2 },
	{ EVENT_EXIT, 40, CONDITION_ALWAYS, 13 },
	{ EVENT_EXIT, 41, CONDITION_ALWAYS, 14 },
	{ EVENT_TOUCH, 1, CONDITION_ALWAYS, EVENT_SCRIPT(SceneState_SetFlag200AndConfigureRegion55_26) },
	{ EVENT_TOUCH, 2, CONDITION_ALWAYS, EVENT_SCRIPT(ActorPresentation_SetFlag200AndSceneCell23) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 5, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_SetupScene5) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 6, CONDITION_ALWAYS, EVENT_SCRIPT(ActorPresentation_SetupActorEighteenAt312_304) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 7, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_SetupScene7) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 8, CONDITION_ALWAYS, EVENT_SCRIPT(ActorPresentation_SetupActorZeroForSceneEight) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 9, CONDITION_ALWAYS, EVENT_SCRIPT(ActorPresentation_SetupActorZeroForSceneNine) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 10, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_SetupScene10) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 11, CONDITION_ALWAYS, EVENT_SCRIPT(ActorDraw_SetupActorSceneCells) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 12, CONDITION_ALWAYS, EVENT_SCRIPT(ActorPresentation_SetupActorZeroForSceneTwelve) },
	{ EVENT_TALK, 8, CONDITION_ALWAYS, EVENT_SCRIPT(SceneDialogue_RunActor8FlagScene) },
	{ EVENT_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgKuupuappuYouveProbablyAlreadyHeardButThe) },
	{ EVENT_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgKuupuappuIWonderWhatHappenedToHammet) },
	{ EVENT_TALK, 11, CONDITION_ALWAYS, EVENT_SCRIPT(SceneDialogue_RunActor11Line) },
	{ EVENT_TALK, 12, CONDITION_ALWAYS, EVENT_SCRIPT(SceneDialogue_RunActor12TwoFlagScene) },
	{ EVENT_TALK, 13, CONDITION_ALWAYS, EVENT_SCRIPT(SceneDialogue_ShowLine1CB0ForActor13) },
	{ EVENT_TALK, 14, CONDITION_ALWAYS, EVENT_SCRIPT(SceneDialogue_RunActorFourteenFlagDialogue) },
	{ EVENT_TALK, 15, CONDITION_ALWAYS, EVENT_SCRIPT(SceneDialogue_RunActorFifteenFacingPreservedDialogue) },
	{ EVENT_TALK, 16, CONDITION_ALWAYS, EVENT_SCRIPT(SceneDialogue_RunActor16CountedDialogue) },
	{ EVENT_TALK, 17, CONDITION_ALWAYS, MSGNO(MsgKuupuappuTheThievesEscapedButIThink) },
	{ EVENT_TALK, 18, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunActorEighteenDialogue) },
	{ EVENT_TALK, 19, CONDITION_ALWAYS, MSGNO(MsgKuupuappuThereYouSeeTheRoofRepairs) },
	{ EVENT_TALK, 21, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_SetActor21Values0And4) },
	{ SEARCH(SEARCH_UNNAMED), 30, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunActor21SequenceOnce) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 0x408, 0x305, EVENT_SCRIPT(SceneDialogue_RunActorEightTimedDialogue) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgKuupuappuICantBelieveTheyreHereWhat) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgKuupuappuWhenThoseGuysFromKalayCame) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgKuupuappuYeahSomeWarriorsFromKalayCame) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, EVENT_SCRIPT(SceneDialogue_RunActor11AcceptanceDialogue) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 0x40c, 0x306, EVENT_SCRIPT(SceneDialogue_RunActor12TimedTwoFlagScene) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgKuupuappuThoseStupidThievesEscapedAndWe) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, EVENT_SCRIPT(ActorPresentation_RunActor13AcceptanceDialogue) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 0x40e, 0x307, EVENT_SCRIPT(KuupuappuMuraSai_RunActor14Talk) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, EVENT_SCRIPT(KuupuappuMuraSai_RunActor14Talk) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, EVENT_SCRIPT(SceneDialogue_RunActorFifteenDialogue) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 0x410, 0x308, EVENT_SCRIPT(FieldScene_RunScene385SequenceA) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunScene385SequenceA) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS, MSGNO(MsgKuupuappuIKeepWonderingWasOurStatue) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, IF_FLAG_SET(0x858), MSGNO(MsgKuupuappuCarefulSearchWillRevealPassage) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS, MSGNO(MsgKuupuappuIfYouBringMeSomeBones) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS, MSGNO(MsgKuupuappuThatsTheLastOfTheRoof) },
	{ SEARCH(SEARCH_UNNAMED), 90, CONDITION_ALWAYS, EVENT_SCRIPT(KuupuappuMuraSai_InspectWell) },
	{ SEARCH(SEARCH_WOODEN_BOX), 101, 0xf4a, FIND_ITEM(ITEM_NUT) },
	{ 0xe714, 18, 0x858, EVENT_SCRIPT(FieldScene_RunActor18MotionSequence) },
	{ PSYNERGY_IN_AREA(ABILITY_REVEAL, 5), 0, CONDITION_ALWAYS, EVENT_SCRIPT(ActorPresentation_SetAlternatePairedSceneCells) },
	{ PSYNERGY_IN_AREA(ABILITY_REVEAL, 4), 0, CONDITION_ALWAYS, EVENT_SCRIPT(ActorPresentation_SetPairedSceneCells) },
	{ SCENE_EVENTS_END },
};
