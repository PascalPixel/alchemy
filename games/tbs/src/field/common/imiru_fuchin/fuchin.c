#include "game_state.h"
#include "keystate.h"
#include "scene_ids.h"
#include "unit/import.h"
#include "unit/field/common/imiru_fuchin/fuchin.h"
#include "unit/battle/effect/object.h"
#include "unit/field/common/script/field.h"
#include "unit/field/common/script/field6.h"
#include "unit/field/common/script/field7.h"
#include "unit/field/common/script/field8.h"
#include "unit/field/common/script/field9.h"
#include "unit/field/common/script/field10.h"
#include "unit/field/common/script/field11.h"
#include "unit/game/character/chara.h"
#include "unit/game/character/chara2.h"
#include "unit/game/character/chara3.h"
#include "unit/game/character/chara4.h"
#include "unit/game/character/chara5.h"
#include "unit/game/character/chara6.h"
#include "unit/game/character/chara7.h"
#include "unit/game/character/chara8.h"
#include "unit/system/scheduler/runtime.h"
#include "unit/system/scheduler/runtime3.h"
#include "unit/system/scheduler/runtime4.h"
#include "unit/system/scheduler/runtime5.h"
#include "unit/system/scheduler/runtime6.h"
#include "unit/system/scheduler/runtime7.h"
#include "unit/system/scheduler/runtime8.h"
#include "unit/system/scheduler/runtime9.h"
#include "unit/system/scheduler/runtim10.h"
#include "unit/system/scheduler/runtim11.h"
#include "unit/system/scheduler/runtim13.h"
#include "battle.h"
#include "field_service.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "imiru_fuchin.h"
#include "types.h"
#include "glyph.h"
#include "field_event.h"
#include "field_scene.h"
#include "call.h"
#include "btlpty.h"
#include "field_effect.h"
#include "scene.h"
#include "fieldrun.h"
#include "disptran.h"
#include "fx_scene.h"
#include "text/msg_ids.h"
#include "ability_ids.h"
#include "objcmd.h"

// 前方宣言
static void DialogueLayout_ConfigureGroupOne(void);
static void DialogueLayout_ConfigureGroupTwo(void);
static void DialogueLayout_ConfigureGroupThree(void);
static void FieldScene_RunFlagBranchedLayoutSteps(void);
static void FieldScene_RunScene39aSequenceA(void);
static void ImiruFuchin_ApplyEntrySetup(void);
static void ImiruFuchin_ApplyRoomLayout(void);
static void ImiruFuchin_HopBy(s32 dx, s32 dz);
static void ImiruFuchin_PlaceDragonsEye(void);
static void SceneActor_StepSubjectAlongHeading(void);

s32 ImiruFuchin_TrackLeader;

struct CaveEffectScripts
{
	const s32 *script[3];
};

// EffectOptions cut after its scales: the caller's frame holds these 24 bytes only
struct CaveDustOptions
{
	s32 priority;
	s32 palette;
	s32 start_scale_x;
	s32 start_scale_y;
	s32 target_scale_x;
	s32 target_scale_y;
};

LAYOUT_OFFSET_GUARD(CaveDustOptions_Priority, struct CaveDustOptions, priority,
	(u32) & ((struct EffectOptions *)0)->priority);
LAYOUT_OFFSET_GUARD(CaveDustOptions_Palette, struct CaveDustOptions, palette,
	(u32) & ((struct EffectOptions *)0)->palette);
LAYOUT_OFFSET_GUARD(CaveDustOptions_StartX, struct CaveDustOptions, start_scale_x,
	(u32) & ((struct EffectOptions *)0)->start_scale_x);
LAYOUT_OFFSET_GUARD(CaveDustOptions_StartY, struct CaveDustOptions, start_scale_y,
	(u32) & ((struct EffectOptions *)0)->start_scale_y);
LAYOUT_OFFSET_GUARD(CaveDustOptions_TargetX, struct CaveDustOptions, target_scale_x,
	(u32) & ((struct EffectOptions *)0)->target_scale_x);
LAYOUT_OFFSET_GUARD(CaveDustOptions_TargetY, struct CaveDustOptions, target_scale_y,
	(u32) & ((struct EffectOptions *)0)->target_scale_y);

static void EV08_TALK(void)
{
	BattleFx_RunPageEffectForSlot(8, 2, 1);
}

static void EV11_TALK(void)
{
	BattleFx_SetPhaseRequest(0xB, 0x3E);
}

const struct SceneEntrance *Scene_GetEntrances(void)
{
	s16 v = gGameState.scene;

	if (v == (s32)&SceneId_ImiruFuchin1)
	{
		return gImiruFuchinEntrances1;
	}
	if (v == (s32)&SceneId_ImiruFuchin2)
	{
		return gImiruFuchinEntrances2;
	}
	if (v == (s32)&SceneId_ImiruFuchin3)
	{
		return gImiruFuchinEntrances3;
	}
	if (v == (s32)&SceneId_ImiruFuchin4)
	{
		return gImiruFuchinEntrances4;
	}
	if (v == (s32)&SceneId_ImiruFuchin5)
	{
		return gImiruFuchinEntrances5;
	}
	if (v == (s32)&SceneId_ImiruFuchin6)
	{
		return gImiruFuchinEntrances6;
	}
	if (v == (s32)&SceneId_ImiruFuchin7)
	{
		return gImiruFuchinEntrances7;
	}
	return gImiruFuchinEntrancesOther;
}

s32 SceneData_ReturnZero(void)
{
	return 0;
}

s32 *ImiruFuchin_GetExits(void)
{
	return gImiruFuchinExits;
}

const struct ScenePlacement *Scene_GetPlacements(void)
{
	s16 v = gGameState.scene;

	if (v == (s32)&SceneId_ImiruFuchin1)
	{
		return gImiruFuchinPlacements1;
	}
	if (v == (s32)&SceneId_ImiruFuchin2)
	{
		return gImiruFuchinPlacements2;
	}
	if (v == (s32)&SceneId_ImiruFuchin3)
	{
		return gImiruFuchinPlacements3;
	}
	if (v == (s32)&SceneId_ImiruFuchin4)
	{
		return gImiruFuchinPlacements4;
	}
	if (v == (s32)&SceneId_ImiruFuchin5)
	{
		return gImiruFuchinPlacements5;
	}
	if (v == (s32)&SceneId_ImiruFuchin7)
	{
		return gImiruFuchinPlacements7;
	}
	return gImiruFuchinPlacementsOther;
}

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

static void EV11_TOUCH_2(void)
{

	Engine_AudioPlayCue(0xF1);
	SceneActor_PlacePairAtOffset(8, 0x70, 0);
	SceneActor_PlacePairAtOffset(8, 0x70, 0);
	Engine_AudioPlayCue(0x121);
	Engine_GameFlagSet(0x301);
	Engine_TaskWait(2);
	DialogueLayout_ConfigureGroupOne();
}

static void EV12_TOUCH_2(void)
{
	s32 offset = 112;

	offset = -offset;
	Engine_AudioPlayCue(0xF1);
	SceneActor_PlacePairAtOffset(8, offset, 0);
	SceneActor_PlacePairAtOffset(8, offset, 0);
	Engine_AudioPlayCue(0x121);
	Engine_GameFlagClear(0x301);
	Engine_TaskWait(2);
	DialogueLayout_ConfigureGroupOne();
}

static void EV13_TOUCH_2(void)
{
	Engine_AudioPlayCue(0xF1);
	SceneActor_PlacePairAtOffset(9, 0, 0x40);
	Engine_AudioPlayCue(0x121);
	Engine_GameFlagClear(0x302);
	Engine_TaskWait(2);
	DialogueLayout_ConfigureGroupOne();
}

static void EV14_TOUCH_2(void)
{

	Engine_AudioPlayCue(0xF1);
	SceneActor_PlacePairAtOffset(9, 0, -64);
	Engine_AudioPlayCue(0x121);
	Engine_GameFlagSet(0x302);
	Engine_TaskWait(2);
	DialogueLayout_ConfigureGroupOne();
}

static void EV15_TOUCH_2(void)
{

	Engine_AudioPlayCue(0xF1);
	SceneActor_PlacePairAtOffset(0xA, 0, 0x40);
	Engine_AudioPlayCue(0x121);
	Engine_GameFlagClear(0x303);
	Engine_TaskWait(2);
	DialogueLayout_ConfigureGroupOne();
}

static void EV16_TOUCH_2(void)
{
	Engine_AudioPlayCue(0xF1);
	SceneActor_PlacePairAtOffset(0xA, 0, -64);
	Engine_AudioPlayCue(0x121);
	Engine_GameFlagSet(0x303);
	Engine_TaskWait(2);
	DialogueLayout_ConfigureGroupOne();
}

static void FieldScene_RunActor8Transition304(void)
{
	Engine_AudioPlayCue(0xF1);
	SceneActor_PlacePairAtOffset(8, 0x90, 0);
	Engine_AudioPlayCue(0x121);
	Engine_GameFlagSet(0x304);
	Engine_TaskWait(2);
	DialogueLayout_ConfigureGroupTwo();
}

static void EV12_TOUCH_3(void)
{
	Engine_AudioPlayCue(0xF1);
	SceneActor_PlacePairAtOffset(8, -144, 0);
	Engine_AudioPlayCue(0x121);
	Engine_GameFlagClear(0x304);
	Engine_GameFlagSet(0x305);
	Engine_TaskWait(2);
	DialogueLayout_ConfigureGroupTwo();
}

static void FieldScene_RunActor8Transition305(void)
{
	Engine_AudioPlayCue(0xF1);
	SceneActor_PlacePairAtOffset(8, -14, 0);
	Engine_AudioPlayCue(0x121);
	Engine_GameFlagSet(0x305);
	Engine_TaskWait(2);
	DialogueLayout_ConfigureGroupTwo();
}

static void EV17_TOUCH_3(void)
{
	Engine_AudioPlayCue(0xF1);

	if (Engine_GameFlagIsSet(0x306) != 0)
	{
		SceneActor_PlacePairAtOffset(8, 16, 0);
		Engine_GameFlagClear(0x305);
	}
	else
	{
		SceneActor_PlacePairAtOffset(8, 144, 0);

		Engine_GameFlagSet(0x304);
	}

	Engine_AudioPlayCue(0x121);
	Engine_TaskWait(2);
	DialogueLayout_ConfigureGroupTwo();
}

static void EV13_TOUCH_3(void)
{

	Engine_AudioPlayCue(0xF1);
	SceneActor_PlacePairAtOffset(9, 0, 0x40);
	Engine_AudioPlayCue(0x121);
	Engine_GameFlagSet(0x306);
	Engine_TaskWait(2);
	DialogueLayout_ConfigureGroupTwo();
}

static void EV14_TOUCH_3(void)
{
	Engine_AudioPlayCue(0xF1);
	SceneActor_PlacePairAtOffset(9, 0, -64);
	Engine_AudioPlayCue(0x121);
	Engine_GameFlagClear(0x306);
	Engine_TaskWait(2);
	DialogueLayout_ConfigureGroupTwo();
}

static void EV15_TOUCH_3(void)
{

	Engine_AudioPlayCue(0xF1);
	SceneActor_PlacePairAtOffset(10, 0, 144);
	SceneActor_PlacePairAtOffset(10, 0, 128);
	Engine_AudioPlayCue(0x121);
	Engine_GameFlagClear(0x307);
	Engine_TaskWait(2);
	DialogueLayout_ConfigureGroupTwo();
}

static void EV16_TOUCH_3(void)
{

	s32 depth = -96;

	Engine_AudioPlayCue(0xF1);
	SceneActor_PlacePairAtOffset(10, 0, depth);
	SceneActor_PlacePairAtOffset(10, 0, depth);
	SceneActor_PlacePairAtOffset(10, 0, -80);
	Engine_AudioPlayCue(0x121);
	Engine_GameFlagSet(0x307);
	Engine_TaskWait(2);
	DialogueLayout_ConfigureGroupTwo();
}

static void EV10_TOUCH_4(void)
{
	Engine_AudioPlayCue(0xF1);

	if (Engine_GameFlagIsSet(0x310) != 0 || Engine_GameFlagIsSet(0x30D) != 0)
	{
		SceneActor_PlacePairAtOffset(8, -48, 0);
		Engine_GameFlagClear(0x308);
		Engine_GameFlagSet(0x309);
	}
	else
	{
		SceneActor_PlacePairAtOffset(8, -96, 0);
		Engine_GameFlagSet(0x308);
		Engine_GameFlagClear(0x309);
	}

	Engine_AudioPlayCue(0x121);
	Engine_TaskWait(2);
	DialogueLayout_ConfigureGroupThree();
}

static void FieldScene_RunActor8Transition308And309(void)
{
	Engine_AudioPlayCue(0xF1);
	Engine_GameFlagClear(0x308);
	Engine_GameFlagClear(0x309);
	SceneActor_PlacePairAtOffset(8, 0x30, 0);
	Engine_AudioPlayCue(0x121);
	Engine_TaskWait(2);
	DialogueLayout_ConfigureGroupThree();
}

static void ActorPresentation_SetupActorEightForFlags308And309Guarded(void)
{

	if (Engine_GameFlagIsSet(0x310) != 0)
	{
		return;
	}
	if (Engine_GameFlagIsSet(0x30D) != 0)
	{
		return;
	}

	Engine_AudioPlayCue(0xF1);

	Engine_GameFlagSet(0x308);
	Engine_GameFlagClear(0x309);

	SceneActor_PlacePairAtOffset(8, -48, 0);
	Engine_AudioPlayCue(0x121);
	Engine_TaskWait(2);
	DialogueLayout_ConfigureGroupThree();
}

static void EV12_TOUCH_4(void)
{

	Engine_AudioPlayCue(0xF1);
	Engine_GameFlagClear(0x308);
	Engine_GameFlagClear(0x309);
	SceneActor_PlacePairAtOffset(8, 0x60, 0);
	Engine_AudioPlayCue(0x121);
	Engine_TaskWait(2);
	DialogueLayout_ConfigureGroupThree();
}

static void EV14_TOUCH_4(void)
{

	Engine_AudioPlayCue(0xF1);
	SceneActor_PlacePairAtOffset(9, -32, 0);
	Engine_AudioPlayCue(0x121);
	Engine_GameFlagSet(0x30A);
	Engine_TaskWait(2);
	DialogueLayout_ConfigureGroupThree();
}

static void EV13_TOUCH_4(void)
{
	Engine_AudioPlayCue(0xF1);
	SceneActor_PlacePairAtOffset(9, 0x20, 0);
	Engine_AudioPlayCue(0x121);
	Engine_GameFlagClear(0x30A);
	Engine_TaskWait(2);
	DialogueLayout_ConfigureGroupThree();
}

static void EV15_TOUCH_4(void)
{
	Engine_AudioPlayCue(241);
	if (Engine_GameFlagIsSet(0x308) != 0)
	{
		SceneActor_PlacePairAtOffset(10, 0, -64);
		Engine_GameFlagClear(0x30b);
		Engine_GameFlagSet(0x30c);
		Engine_GameFlagClear(0x30d);
		Engine_GameFlagClear(0x30e);
	}
	else
	{
		SceneActor_PlacePairAtOffset(10, 0, -128);
		Engine_GameFlagSet(0x30b);
		Engine_GameFlagClear(0x30c);
		Engine_GameFlagClear(0x30d);
		Engine_GameFlagClear(0x30e);
	}
	Engine_AudioPlayCue(0x121);
	Engine_TaskWait(2);
	DialogueLayout_ConfigureGroupThree();
}

static void EV16_TOUCH_4(void)
{
	Engine_AudioPlayCue(0xf1);

	if (Engine_GameFlagIsSet(0x308) != 0)
	{
		SceneActor_PlacePairAtOffset(10, 0, 16);
		Engine_GameFlagClear(0x30b);
		Engine_GameFlagSet(0x30c);
		Engine_GameFlagClear(0x30d);
		Engine_GameFlagClear(0x30e);
	}
	else if (Engine_GameFlagIsSet(0x310) != 0)
	{
		SceneActor_PlacePairAtOffset(10, 0, 16);
		Engine_GameFlagClear(0x30b);
		Engine_GameFlagSet(0x30c);
		Engine_GameFlagClear(0x30d);
		Engine_GameFlagClear(0x30e);
	}
	else if (Engine_GameFlagIsSet(0x311) != 0)
	{
		SceneActor_PlacePairAtOffset(10, 0, 64);
		Engine_GameFlagClear(0x30b);
		Engine_GameFlagClear(0x30c);
		Engine_GameFlagSet(0x30d);
		Engine_GameFlagClear(0x30e);
	}
	else
	{
		SceneActor_PlacePairAtOffset(10, 0, 128);
		Engine_GameFlagClear(0x30b);
		Engine_GameFlagClear(0x30c);
		Engine_GameFlagClear(0x30d);
		Engine_GameFlagClear(0x30e);
	}
	Engine_AudioPlayCue(0x121);
	Engine_TaskWait(2);
	DialogueLayout_ConfigureGroupThree();
}

static void FieldScene_RunActor10Transition30BTo30E(void)
{
	Engine_AudioPlayCue(241);
	SceneActor_PlacePairAtOffset(10, 0, -16);
	Engine_AudioPlayCue(0x121);
	Engine_GameFlagSet(0x30b);
	Engine_GameFlagClear(0x30c);
	Engine_GameFlagClear(0x30d);
	Engine_GameFlagClear(0x30e);
	Engine_TaskWait(2);
	DialogueLayout_ConfigureGroupThree();
}

static void FieldScene_RunFlag311DialogueBranch(void)
{
	Engine_AudioPlayCue(0xf1);
	if (Engine_GameFlagIsSet(0x311) != 0)
	{
		SceneActor_PlacePairAtOffset(10, 0, 48);
		Engine_GameFlagClear(0x30b);

		Engine_GameFlagClear(0x30c);
		Engine_GameFlagSet(0x30d);
		Engine_GameFlagClear(0x30e);
	}
	else if (Engine_GameFlagIsSet(0x310) != 0)
	{
		SceneActor_PlacePairAtOffset(10, 0, 32);
		Engine_GameFlagClear(0x30b);
		Engine_GameFlagSet(0x30c);
		Engine_GameFlagClear(0x30d);
		Engine_GameFlagClear(0x30e);
	}
	else
	{
		SceneActor_PlacePairAtOffset(10, 0, 112);
		Engine_GameFlagClear(0x30b);
		Engine_GameFlagClear(0x30c);
		Engine_GameFlagClear(0x30d);
		Engine_GameFlagClear(0x30e);
	}
	Engine_AudioPlayCue(0x121);
	Engine_TaskWait(2);
	DialogueLayout_ConfigureGroupThree();
}

static void ActorPresentation_SetupActorTenForFlags30bAnd30d(void)
{

	Engine_AudioPlayCue(0xF1);
	SceneActor_PlacePairAtOffset(0xA, 0, -64);
	Engine_AudioPlayCue(0x121);
	Engine_GameFlagSet(0x30B);
	Engine_GameFlagClear(0x30D);
	Engine_TaskWait(2);
	DialogueLayout_ConfigureGroupThree();
}

static void FieldScene_RunActor10Flags30bTo30eSequenceA(void)
{
	Engine_AudioPlayCue(241);
	SceneActor_PlacePairAtOffset(10, 0, 64);
	Engine_AudioPlayCue(0x121);
	Engine_GameFlagClear(0x30b);
	Engine_GameFlagClear(0x30c);
	Engine_GameFlagClear(0x30d);
	Engine_GameFlagClear(0x30e);
	Engine_TaskWait(2);
	DialogueLayout_ConfigureGroupThree();
}

static void FieldScene_RunActor10Flags30bTo30eSequenceB(void)
{
	Engine_AudioPlayCue(241);
	SceneActor_PlacePairAtOffset(10, 0, -80);
	Engine_AudioPlayCue(0x121);
	Engine_GameFlagSet(0x30b);
	Engine_GameFlagClear(0x30c);
	Engine_GameFlagClear(0x30d);
	Engine_GameFlagClear(0x30e);
	Engine_TaskWait(2);
	DialogueLayout_ConfigureGroupThree();
}

static void FieldScene_RunActor10Flags30bTo30eSequenceC(void)
{
	Engine_AudioPlayCue(0xF1);
	SceneActor_PlacePairAtOffset(0xA, 0, 0x40);
	Engine_AudioPlayCue(0x121);
	Engine_GameFlagClear(0x30B);
	Engine_GameFlagClear(0x30C);
	Engine_GameFlagClear(0x30D);
	Engine_GameFlagClear(0x30E);
	Engine_TaskWait(2);
	DialogueLayout_ConfigureGroupThree();
}

static void EV20_TOUCH(void)
{
	Engine_AudioPlayCue(0xf1);

	if (Engine_GameFlagIsSet(0x308) != 0 || Engine_GameFlagIsSet(0x30d) != 0)
	{
		SceneActor_PlacePairAtOffset(11, 0, -64);
		Engine_GameFlagClear(0x30f);
		Engine_GameFlagClear(0x310);
		Engine_GameFlagSet(0x311);
		Engine_GameFlagClear(0x312);
	}
	else if (Engine_GameFlagIsSet(0x30c) != 0)
	{
		SceneActor_PlacePairAtOffset(11, 0, -112);
		Engine_GameFlagClear(0x30f);
		Engine_GameFlagSet(0x310);
		Engine_GameFlagClear(0x311);
		Engine_GameFlagClear(0x312);
	}
	else
	{
		SceneActor_PlacePairAtOffset(11, 0, -128);
		Engine_GameFlagSet(0x30f);
		Engine_GameFlagClear(0x310);
		Engine_GameFlagClear(0x311);
		Engine_GameFlagClear(0x312);
	}
	Engine_AudioPlayCue(0x121);
	Engine_TaskWait(2);
	DialogueLayout_ConfigureGroupThree();
}

static void FieldScene_RunSteps30FTo312(void)
{
	Engine_GameFlagClear(0x30F);
	Engine_GameFlagClear(0x310);
	Engine_GameFlagClear(0x311);
	Engine_GameFlagClear(0x312);
}

static void EV21_TOUCH(void)
{
	Engine_AudioPlayCue(241);
	SceneActor_PlacePairAtOffset(11, 0, 128);
	FieldScene_RunSteps30FTo312();
	Engine_AudioPlayCue(0x121);
	Engine_TaskWait(2);
	DialogueLayout_ConfigureGroupThree();
}

static void FieldScene_RunActor11Flags30fTo312Sequence(void)
{
	Engine_AudioPlayCue(0xF1);
	SceneActor_PlacePairAtOffset(0xB, 0, -16);
	Engine_AudioPlayCue(0x121);
	Engine_GameFlagSet(0x30F);
	Engine_GameFlagClear(0x310);
	Engine_GameFlagClear(0x311);
	Engine_GameFlagClear(0x312);
	Engine_TaskWait(2);
	DialogueLayout_ConfigureGroupThree();
}

static void ActorPresentation_SetupActorElevenAt0_112(void)
{

	Engine_AudioPlayCue(241);
	SceneActor_PlacePairAtOffset(11, 0, 112);
	Engine_AudioPlayCue(0x121);
	FieldScene_RunSteps30FTo312();
	Engine_TaskWait(2);
	DialogueLayout_ConfigureGroupThree();
}

static void ActorPresentation_SetupActorElevenAt0_64(void)
{

	Engine_AudioPlayCue(0xF1);
	SceneActor_PlacePairAtOffset(0xB, 0, 0x40);
	Engine_AudioPlayCue(0x121);
	FieldScene_RunSteps30FTo312();
	Engine_TaskWait(2);
	DialogueLayout_ConfigureGroupThree();
}

static void ActorPresentation_SetupActorElevenAt0_80(void)
{

	Engine_AudioPlayCue(0xF1);
	SceneActor_PlacePairAtOffset(0xB, 0, 0x50);
	Engine_AudioPlayCue(0x121);
	FieldScene_RunSteps30FTo312();
	Engine_TaskWait(2);
	DialogueLayout_ConfigureGroupThree();
}

static void FieldScene_RunActor11Transition(void)
{
	Engine_AudioPlayCue(241);
	SceneActor_PlacePairAtOffset(11, 0, 48);
	Engine_AudioPlayCue(0x121);
	FieldScene_RunSteps30FTo312();
	Engine_TaskWait(2);
	DialogueLayout_ConfigureGroupThree();
}

static void EV11_TOUCH_5(void)
{

	Engine_AudioPlayCue(0xF1);
	SceneActor_PlacePairAtOffset(8, 0, 0x70);
	Engine_AudioPlayCue(0x121);
	Engine_GameFlagSet(0x313);
	Engine_TaskWait(2);
	FieldScene_RunFlagBranchedLayoutSteps();
}

static void EV12_TOUCH_5(void)
{
	Engine_AudioPlayCue(0xF1);
	SceneActor_PlacePairAtOffset(8, 0, -112);
	Engine_AudioPlayCue(0x121);
	Engine_GameFlagClear(0x313);
	Engine_TaskWait(2);
	FieldScene_RunFlagBranchedLayoutSteps();
}

static void EV13_TOUCH_5(void)
{
	Engine_AudioPlayCue(0xF1);
	SceneActor_PlacePairAtOffset(9, -128, 0);
	Engine_AudioPlayCue(0x121);
	Engine_GameFlagSet(0x314);
	Engine_TaskWait(2);
	FieldScene_RunFlagBranchedLayoutSteps();
}

static void EV14_TOUCH_5(void)
{

	Engine_AudioPlayCue(0xF1);
	SceneActor_PlacePairAtOffset(9, 0x80, 0);
	Engine_AudioPlayCue(0x121);
	Engine_GameFlagClear(0x314);
	Engine_TaskWait(2);
	FieldScene_RunFlagBranchedLayoutSteps();
}

static void EV15_TOUCH_5(void)
{

	Engine_AudioPlayCue(241);
	SceneActor_PlacePairAtOffset(10, 160, 0);
	Engine_AudioPlayCue(0x121);
	Engine_GameFlagSet(0x315);
	Engine_TaskWait(2);
	FieldScene_RunFlagBranchedLayoutSteps();
}

static void EV16_TOUCH_5(void)
{
	Engine_AudioPlayCue(241);
	SceneActor_PlacePairAtOffset(10, -160, 0);
	Engine_AudioPlayCue(0x121);
	Engine_GameFlagClear(0x315);
	Engine_TaskWait(2);
	FieldScene_RunFlagBranchedLayoutSteps();
}

static void ActorPresentation_AdvanceActorEightStates(void)
{

	Engine_ActorSetAnimation(8, 1);
	Engine_ActorSetAnimation(8, 2);
}

static void FieldScene_SetActor9Values1And2(void)
{
	Engine_ActorSetAnimation(9, 1);
	Engine_ActorSetAnimation(9, 2);
}

static void ActorPresentation_AdvanceActorTenStates(void)
{

	Engine_ActorSetAnimation(10, 1);
	Engine_ActorSetAnimation(10, 2);
}

static void SceneActor_SetActor11Values1And2(void)
{

	Engine_ActorSetAnimation(11, 1);
	Engine_ActorSetAnimation(11, 2);
}

static struct FieldActor *OverlayObject_CreateAndInitialize(s32 a, s32 b, s32 c, s32 d)
{
	struct FieldActor *actor = Engine_ObjectCreate(d, a, b, c);

	if (actor != 0)
	{
		actor->sprite->priority = 1;
		actor->motion_flags = 0;
		Engine_ActorSetSpriteFlags(actor, 0);
		ObjectGroup_SetChildValue(actor, 15);
		actor->priority_flags |= 2;
		return actor;
	}
	return 0;
}

const struct SceneEvent *Scene_GetEvents(void)
{
	s16 v = gGameState.scene;

	if (v == (s32)&SceneId_ImiruFuchin1)
	{
		return gImiruFuchinEvents1;
	}
	if (v == (s32)&SceneId_ImiruFuchin2)
	{
		return gImiruFuchinEvents2;
	}
	if (v == (s32)&SceneId_ImiruFuchin3)
	{
		return gImiruFuchinEvents3;
	}
	if (v == (s32)&SceneId_ImiruFuchin4)
	{
		return gImiruFuchinEvents4;
	}
	if (v == (s32)&SceneId_ImiruFuchin5)
	{
		return gImiruFuchinEvents5;
	}
	if (v == (s32)&SceneId_ImiruFuchin6)
	{
		return gImiruFuchinEvents6;
	}
	if (v == (s32)&SceneId_ImiruFuchin7)
	{
		return gImiruFuchinEvents7;
	}
	return gImiruFuchinEventsOther;
}

static void EV01_TOUCH(void)
{
	struct FieldActor *work;

	Engine_EventBegin();
	work = Object_GetById(0);
	work->facing = 0x4000;
	Engine_AudioPlayCue(123);
	Engine_EventCloseScreen();
	Engine_EventWaitForScreen();
	Engine_EventRequestExit(1);
}

static void EV10_TOUCH_1(void)
{
	SceneActor_StepSubjectAlongHeading();
}

static void SceneActor_PlaceAtTileAndMark(s32 id, s32 tile_x, s32 tile_z)
{
	struct FieldActor *actor = Object_GetById(id);

	if (actor != NULL)
	{
		Engine_ActorSetSpritePriority(id, 3);
		actor->terrain_id = 2;
		actor->priority_flags |= 2;
		actor->x.fixed = (tile_x << 20) + 0x80000;
		actor->z.fixed = (tile_z << 20) + 0x80000;
	}
}

static void DialogueLayout_ConfigureGroupOne(void)
{
	{
		s32 x = 8;
		s32 y = 29;
		Engine_MapCopyCellAttributes(8, 42, 15, 5, x, y);
	}

	if (Engine_GameFlagIsSet(0x301) != 0)
	{
		SceneActor_PlaceAtTileAndMark(8, 22, 31);
		{
			s32 x = 8;
			s32 y = 30;
			Engine_MapCopyCellAttributes(9, 30, 1, 3, x, y);
		}
	}
	else
	{
		SceneActor_PlaceAtTileAndMark(8, 8, 31);
		{
			s32 x = 22;
			s32 y = 30;
			Engine_MapCopyCellAttributes(9, 30, 1, 3, x, y);
		}
	}

	if (Engine_GameFlagIsSet(0x302) != 0)
	{
		SceneActor_PlaceAtTileAndMark(9, 12, 29);
		{
			s32 x = 11;
			s32 y = 33;
			Engine_MapCopyCellAttributes(14, 33, 3, 1, x, y);
		}
	}
	else
	{
		SceneActor_PlaceAtTileAndMark(9, 12, 33);
		{
			s32 x = 11;
			s32 y = 29;
			Engine_MapCopyCellAttributes(14, 29, 3, 1, x, y);
		}
	}

	if (Engine_GameFlagIsSet(0x303) != 0)
	{
		SceneActor_PlaceAtTileAndMark(10, 18, 29);
		{
			s32 x = 17;
			s32 y = 33;
			Engine_MapCopyCellAttributes(14, 33, 3, 1, x, y);
		}
	}
	else
	{
		SceneActor_PlaceAtTileAndMark(10, 18, 33);
		{
			s32 x = 17;
			s32 y = 29;
			Engine_MapCopyCellAttributes(14, 29, 3, 1, x, y);
		}
	}
}

static void DialogueLayout_ConfigureGroupTwo(void)
{
	{
		s32 x = 12;
		s32 y = 8;
		Engine_MapCopyCellAttributes(0, 28, 10, 18, x, y);
	}

	if (Engine_GameFlagIsSet(0x304) != 0)
	{
		SceneActor_PlaceAtTileAndMark(8, 21, 20);
		{
			s32 x = 13;
			s32 y = 19;
			Engine_MapCopyCellAttributes(20, 19, 1, 3, x, y);
		}
	}
	else
	{
		SceneActor_PlaceAtTileAndMark(8, 13, 20);
		{
			s32 x = 21;
			s32 y = 19;
			Engine_MapCopyCellAttributes(20, 19, 1, 3, x, y);
		}
	}

	if (Engine_GameFlagIsSet(0x305) != 0)
	{
		SceneActor_PlaceAtTileAndMark(8, 12, 20);
		{
			s32 x = 12;
			s32 y = 19;
			Engine_MapCopyCellAttributes(5, 19, 1, 3, x, y);
		}
		{
			s32 x = 13;
			s32 y = 19;
			Engine_MapCopyCellAttributes(20, 19, 1, 3, x, y);
		}
		if (Engine_GameFlagIsSet(0x304) != 0)
		{
			SceneActor_PlaceAtTileAndMark(8, 21, 20);
			{
				s32 x = 13;
				s32 y = 19;
				Engine_MapCopyCellAttributes(20, 19, 1, 3, x, y);
			}
			{
				s32 x = 12;
				s32 y = 19;
				Engine_MapCopyCellAttributes(20, 19, 1, 3, x, y);
			}
		}
	}

	if (Engine_GameFlagIsSet(0x306) != 0)
	{
		SceneActor_PlaceAtTileAndMark(9, 15, 21);
		{
			s32 x = 14;
			s32 y = 17;
			Engine_MapCopyCellAttributes(14, 18, 3, 1, x, y);
		}
	}
	else
	{
		SceneActor_PlaceAtTileAndMark(9, 15, 17);
		{
			s32 x = 14;
			s32 y = 21;
			Engine_MapCopyCellAttributes(14, 18, 3, 1, x, y);
		}
	}

	if (Engine_GameFlagIsSet(0x307) != 0)
	{
		SceneActor_PlaceAtTileAndMark(10, 19, 8);
		{
			s32 x = 18;
			s32 y = 25;
			Engine_MapCopyCellAttributes(14, 18, 3, 1, x, y);
		}
	}
	else
	{
		SceneActor_PlaceAtTileAndMark(10, 19, 25);
		{
			s32 x = 18;
			s32 y = 8;
			Engine_MapCopyCellAttributes(14, 18, 3, 1, x, y);
		}
	}
}

static void DialogueLayout_ConfigureGroupThree(void)
{
	{
		s32 x = 12, y = 21;
		Engine_MapCopyCellAttributes(12, 3, 9, 16, x, y);
	}

	if (Engine_GameFlagIsSet(0x308) != 0)
	{
		SceneActor_PlaceAtTileAndMark(8, 14, 25);
		{
			s32 x = 20, y = 24;
			Engine_MapCopyCellAttributes(16, 24, 1, 3, x, y);
		}
	}
	else if (Engine_GameFlagIsSet(0x309) != 0)
	{
		SceneActor_PlaceAtTileAndMark(8, 17, 25);
		{
			s32 y = 24;
			Engine_MapCopyCellAttributes(18, 24, 1, 3, 20, y);
			Engine_MapCopyCellAttributes(18, 24, 1, 3, 14, y);
			Engine_MapCopyCellAttributes(8, 41, 1, 3, 17, y);
		}
	}
	else
	{
		SceneActor_PlaceAtTileAndMark(8, 20, 25);
		{
			s32 x = 14, y = 24;
			Engine_MapCopyCellAttributes(16, 24, 1, 3, x, y);
		}
	}

	if (Engine_GameFlagIsSet(0x30a) != 0)
	{
		SceneActor_PlaceAtTileAndMark(9, 13, 35);
		{
			s32 x = 15, y = 34;
			Engine_MapCopyCellAttributes(14, 34, 1, 3, x, y);
		}
	}
	else
	{
		SceneActor_PlaceAtTileAndMark(9, 15, 35);
		{
			s32 x = 13, y = 34;
			Engine_MapCopyCellAttributes(14, 34, 1, 3, x, y);
		}
	}

	if (Engine_GameFlagIsSet(0x30b) != 0)
	{
		SceneActor_PlaceAtTileAndMark(10, 15, 22);
		{
			s32 x = 14;
			Engine_MapCopyCellAttributes(14, 29, 3, 1, x, 30);
			Engine_MapCopyCellAttributes(5, 41, 3, 1, x, 22);
		}
	}
	else if (Engine_GameFlagIsSet(0x30c) != 0)
	{
		SceneActor_PlaceAtTileAndMark(10, 15, 23);
		{
			s32 x = 14;
			Engine_MapCopyCellAttributes(5, 42, 3, 1, x, 23);
			Engine_MapCopyCellAttributes(14, 29, 3, 1, x, 30);
			Engine_MapCopyCellAttributes(10, 44, 3, 1, x, 21);
		}
	}
	else if (Engine_GameFlagIsSet(0x30d) != 0)
	{
		SceneActor_PlaceAtTileAndMark(10, 15, 26);
		{
			s32 x = 14;
			Engine_MapCopyCellAttributes(14, 29, 3, 1, x, 22);
			Engine_MapCopyCellAttributes(5, 43, 3, 1, x, 26);
			Engine_MapCopyCellAttributes(14, 29, 3, 1, x, 30);
		}
	}
	else if (Engine_GameFlagIsSet(0x30e) != 0)
	{
		SceneActor_PlaceAtTileAndMark(10, 15, 27);
		{
			s32 x = 14;
			Engine_MapCopyCellAttributes(14, 29, 3, 1, x, 22);
			Engine_MapCopyCellAttributes(14, 29, 3, 1, x, 30);
			Engine_MapCopyCellAttributes(5, 44, 3, 1, x, 27);
		}
	}
	else
	{
		SceneActor_PlaceAtTileAndMark(10, 15, 30);
	}

	if (Engine_GameFlagIsSet(0x30f) != 0)
	{
		SceneActor_PlaceAtTileAndMark(11, 15, 23);
		{
			s32 x = 14;
			Engine_MapCopyCellAttributes(14, 29, 3, 1, x, 31);
			Engine_MapCopyCellAttributes(10, 40, 3, 1, x, 23);
		}
	}
	else if (Engine_GameFlagIsSet(0x310) != 0)
	{
		SceneActor_PlaceAtTileAndMark(11, 15, 24);
		{
			s32 x = 14;
			Engine_MapCopyCellAttributes(14, 29, 3, 1, x, 31);
			Engine_MapCopyCellAttributes(10, 41, 3, 1, x, 24);
		}
	}
	else if (Engine_GameFlagIsSet(0x311) != 0)
	{
		SceneActor_PlaceAtTileAndMark(11, 15, 27);
		{
			s32 x = 14;
			Engine_MapCopyCellAttributes(14, 29, 3, 1, x, 31);
			Engine_MapCopyCellAttributes(10, 42, 3, 1, x, 27);
		}
	}
	else if (Engine_GameFlagIsSet(0x312) != 0)
	{
		SceneActor_PlaceAtTileAndMark(11, 15, 28);
		{
			s32 x = 14;
			Engine_MapCopyCellAttributes(14, 29, 3, 1, x, 31);
			Engine_MapCopyCellAttributes(10, 43, 3, 1, x, 28);
		}
	}
	else
	{
		SceneActor_PlaceAtTileAndMark(11, 15, 31);
	}
}

static void FieldScene_RunFlagBranchedLayoutSteps(void)
{

	if ((((u16)gGameState.entrance - 1) << 16) <= (u32)0x10000)
	{
		{
			s32 x = 14;
			s32 y = 10;
			Engine_MapCopyCellAttributes(22, 20, 9, 8, x, y);
		}
	}
	else
	{
		{
			s32 x = 7;
			s32 y = 45;
			Engine_MapCopyCellAttributes(20, 45, 11, 4, x, y);
		}
	}

	if (Engine_GameFlagIsSet(0x313) != 0)
	{
		SceneActor_PlaceAtTileAndMark(8, 20, 17);
		{
			s32 x = 19;
			s32 y = 10;
			Engine_MapCopyCellAttributes(19, 11, 3, 1, x, y);
		}
	}
	else
	{
		SceneActor_PlaceAtTileAndMark(8, 20, 10);
		{
			s32 x = 19;
			s32 y = 17;
			Engine_MapCopyCellAttributes(19, 11, 3, 1, x, y);
		}
	}

	if (Engine_GameFlagIsSet(0x314) != 0)
	{
		SceneActor_PlaceAtTileAndMark(9, 14, 16);
		{
			s32 x = 22;
			s32 y = 15;
			Engine_MapCopyCellAttributes(16, 15, 1, 3, x, y);
		}
	}
	else
	{
		SceneActor_PlaceAtTileAndMark(9, 22, 16);
		{
			s32 x = 14;
			s32 y = 15;
			Engine_MapCopyCellAttributes(16, 15, 1, 3, x, y);
		}
	}

	if (Engine_GameFlagIsSet(0x315) != 0)
	{
		SceneActor_PlaceAtTileAndMark(10, 17, 46);
		{
			s32 x = 7;
			s32 y = 45;
			Engine_MapCopyCellAttributes(15, 15, 1, 3, x, y);
		}
	}
	else
	{
		SceneActor_PlaceAtTileAndMark(10, 7, 46);
		{
			s32 x = 17;
			s32 y = 45;
			Engine_MapCopyCellAttributes(15, 15, 1, 3, x, y);
		}
	}
}

static void EV15_TOUCH_7(void)
{
	if (ImiruFuchin_TrackLeader != 0)
	{
		struct TrackingWork *work = *(gWorkSlot + 36);

		work->actor = Object_GetById(ACTOR_PARTY_LEADER);
	}
}

static void EV16_TOUCH_7(void)
{
	if (ImiruFuchin_TrackLeader != 0)
	{
		struct TrackingWork *work = *(gWorkSlot + 36);

		work->actor = NULL;
	}
}

s32 ImiruFuchin_ApplyEntryHook(void)
{
	gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
	if (Engine_GameFlagIsSet(0x109) == 0 && gGameState.scene == (s32)&SceneId_ImiruFuchin1)
	{
		Engine_GameFlagSet(0x144);
		FieldScene_RunScene39aSequenceA();
	}
	else
	{
		ImiruFuchin_ApplyEntrySetup();
	}
	return 0;
}

static void FieldScene_RunScene39aSequenceA(void)
{
	struct FieldActor *actor;

	Engine_EventBegin();
	actor = Object_GetById(8);
	Engine_ActorSetSpriteFlags(actor, 0);
	Engine_EventOpenScreen();
	Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x20000, 0x1999);
	Actor_MoveToAndWait(ACTOR_PARTY_LEADER, 0x108, 196);
	Engine_EventEnd();
}

static void ImiruFuchin_ApplyEntrySetup(void)
{
	struct FieldActor *actor;
	/* FAKEMATCH: needed to match: the shared zero/address-word lifetime avoids swapping registers and adding a heap-cell add. Literal zeros and the slot read in place: differ 51. */
	s32 value;

	ImiruFuchin_ApplyRoomLayout();
	if (gGameState.scene == (s32)&SceneId_ImiruFuchin4)
	{
		if (!GameFlag_IsSet(0xf13) && gGameState.entrance == 1)
		{
			ImiruFuchin_PlaceDragonsEye();
		}
		if ((u16)(gGameState.entrance - 2) <= 3)
		{
			OverlayObject_CreateAndInitialize(0x9c0000, 0, 0x1c40000, 223);
			OverlayObject_CreateAndInitialize(0xbc0000, 0, 0x1c40000, 223);
		}
	}
	else if (gGameState.scene == (s32)&SceneId_ImiruFuchin7)
	{
		value = 0;
		actor = Actor_Get(8);
		ImiruFuchin_TrackLeader = value;
		actor->motion_flags = value;
		actor->y.fixed = value;
		Engine_ActorSetSpritePriority(8, 1);
		Actor_SetChildValue(8, 15);
		switch (gGameState.entrance)
		{
		case 1:
		case 2:
			BattleFx_StartFadeOverlay(0);
			ImiruFuchin_TrackLeader = 1;
			break;
		case 5:
			BattleFx_StartFadeOverlay(0);
			ImiruFuchin_TrackLeader = 1;
			value = *(s32 *)(gWorkSlot + 36);
			((struct TrackingWork *)value)->actor = NULL;
			break;
		}
		if (gGameState.entrance <= 6)
		{
			if (GameFlag_IsSet(0x820))
			{
				Map_CopyCellsTo(30, 57, 19, 57, 1, 1);
				Map_CopyCellsTo(30, 8, 12, 8, 8, 7);
			}
			else
			{
				gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
				ColorBuffer_ApplySource(0x203108, 1);
				ColorBuffer_ApplyTarget(0x203108, 1);
				Engine_ColorBufferInterpolate(1);
				Engine_TaskWait(1);
			}
		}
	}
}

static void ImiruFuchin_ApplyRoomLayout(void)
{
	if (gGameState.scene == (s32)&SceneId_ImiruFuchin2)
	{
		Map_CopyCellAttributes(8, 29, 15, 5, 8, 42);
		DialogueLayout_ConfigureGroupOne();
	}
	else if (gGameState.scene == (s32)&SceneId_ImiruFuchin3)
	{
		Map_CopyCellAttributes(12, 8, 10, 18, 0, 28);
		DialogueLayout_ConfigureGroupTwo();
	}
	else if (gGameState.scene == (s32)&SceneId_ImiruFuchin4 && gGameState.entrance != 1)
	{
		Map_CopyCellAttributes(12, 21, 9, 16, 12, 3);
		DialogueLayout_ConfigureGroupThree();
	}
	else if (gGameState.scene == (s32)&SceneId_ImiruFuchin5)
	{
		if (gGameState.entrance == 1 || gGameState.entrance == 2)
		{
			Map_CopyCellAttributes(14, 10, 9, 8, 22, 20);
		}
		else
		{
			Map_CopyCellAttributes(7, 45, 11, 4, 20, 45);
		}
		FieldScene_RunFlagBranchedLayoutSteps();
	}
}

static void ImiruFuchin_HopOnTrigger(void)
{
	s32 trigger = gEventWork->touched_trigger;

	if (gGameState.scene == (s32)&SceneId_ImiruFuchin3)
	{
		if (trigger == 17)
		{
			ImiruFuchin_HopBy(0, -32);
		}
		else
		{
			ImiruFuchin_HopBy(-32, 0);
		}
	}
	if (gGameState.scene == (s32)&SceneId_ImiruFuchin4 && trigger == 25
		&& Engine_GameFlagIsSet(0x309))
	{
		ImiruFuchin_HopBy(0, 32);
	}
}

static void FieldScene_ApplyOffset0Neg32(void)
{
	ImiruFuchin_HopBy(0, -32);
}

static void SceneState_ApplyOffsetMinus32(void)
{
	ImiruFuchin_HopBy(-32, 0);
}

static void ImiruFuchin_HopBy(s32 dx, s32 dz)
{
	Engine_EventBegin();
	Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x28000, 0x14000);
	Engine_ActorSetDestinationOffset(0, dx, dz);
	Engine_ActorJump(ACTOR_PARTY_LEADER, 4, 0);
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 7);
	Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 6);
	Engine_EventEnd();
}

static void ImiruFuchin_PlaceDragonsEye(void)
{
	struct FieldActor  *actor;
	struct FieldSprite *sprite;
	GlyphTransfer	   *glyph;
	u8					clear = 0;

	actor = Object_Create(22, 0xf80000, 0x80000, 0x980000);
	if (actor != NULL)
	{
		sprite = actor->sprite;
		sprite->flags = clear;
		sprite->part_count = clear;
		sprite->full_color = 0;
		sprite->palette = 0;
		actor->motion_flags = clear;
		actor->unknown_5c = 1;
		glyph = Engine_HeapAllocate(17, sizeof(*glyph));
		Engine_ItemLoadIcon(ITEM_DRAGONS_EYE);
		Engine_VramLoad(sprite->vram_block, 128, glyph->tiles);
		Engine_HeapRelease(17);
		gImiruFuchinDragonsEye = actor;
	}
}

static void EV30_SEARCH(void)
{

	Engine_EventBegin();

	if (gImiruFuchinDragonsEye != NULL)
	{
		Engine_RunRisingObjectSequence(gImiruFuchinDragonsEye, 3);
	}

	Engine_PartyGiveItem(0xE6, 0);
	Engine_GameFlagSet(0xF13);

	if (gImiruFuchinDragonsEye != NULL)
	{
		Engine_ObjectDispatchRelease(gImiruFuchinDragonsEye);
	}

	Engine_EventEnd();
}

static void OverlayObject_AdvancePositionByDelta(union FieldObject *raw)
{
	struct FieldEffect *object = &raw->effect;

	object->x += object->velocity_x;
	object->y += object->velocity_y;
	object->z += object->velocity_z;
	object->scale_x += object->scale_rate_x;
	object->scale_y += object->scale_rate_y;
}

static s32 OverlayObject_ApplyValue15(s32 obj)
{
	ObjectGroup_SetChildValue((struct FieldActor *)obj, 15);
	return 0;
}

void Effect_Spawn(s32 x, s32 y, s32 z, s32 velocity_x, s32 velocity_y, s32 velocity_z, u32 flags,
	const struct EffectOptions *extra)
{
	struct FieldEffect *obj;
	struct FieldSprite *spr;
	const s32		   *script;
	/* FAKEMATCH: needed to match: copying three scripts keeps the 12-byte frame and prevents an early scale-script load. Reading gEffectScripts in place: differ 107. */
	struct CaveEffectScripts scripts = *(const struct CaveEffectScripts *)gEffectScripts;

	obj = (struct FieldEffect *)Engine_ObjectCreate(222, x, y, z);
	if (obj == 0)
		return;
	spr = obj->sprite;
	Object_SetMode((struct FieldActor *)obj, (flags + 1) & EFFECT_SCRIPT_MASK);
	Engine_ObjectSetScript((struct FieldActor *)obj, scripts.script[flags & EFFECT_SCRIPT_MASK]);
	obj->motion_flags = 0;
	spr->flags = 0;
	obj->update = OverlayObject_AdvancePositionByDelta;
	obj->velocity_x = velocity_x;
	obj->velocity_y = velocity_y;
	obj->velocity_z = velocity_z;
	obj->scale_rate_x = 0;
	obj->scale_rate_y = 0;
	spr->priority = 1;
	if ((flags & 0xffff0000) == 0 || extra == 0)
		return;
	if (flags & EFFECT_USE_PALETTE)
		ObjectGroup_SetChildValue((struct FieldActor *)obj, extra->palette);
	if (flags & EFFECT_USE_PRIORITY)
	{
		obj->priority_flags &= ~ACTOR_PRIORITY_AUTOMATIC;
		spr->priority = extra->priority;
	}
	if (flags & EFFECT_USE_START_SCALE)
	{
		obj->scale_x = extra->start_scale_x;
		obj->scale_y = extra->start_scale_y;
	}
	if (flags & EFFECT_SCALE_TO_TARGET)
	{
		script = scripts.script[flags & EFFECT_SCRIPT_MASK];
		if (flags & EFFECT_USE_START_SCALE)
		{
			obj->scale_rate_x = (extra->target_scale_x - obj->scale_x) / script[3];
			obj->scale_rate_y = (extra->target_scale_y - obj->scale_y) / script[3];
		}
		else
		{
			obj->scale_rate_x = (extra->target_scale_x - 0x10000) / script[3];
			obj->scale_rate_y = (extra->target_scale_y - 0x10000) / script[3];
		}
	}
}

static void ImiruFuchin_BlowCaveMouthDust(void)
{
	struct CaveDustOptions params;
	s32					   phase;
	s32					   dx;
	s32					   dy;

	phase = gFrameCount & 3;
	if (phase != 0)
		return;
	params.palette = 10;
	params.start_scale_x = 0x8000;
	params.start_scale_y = 0x8000;
	params.target_scale_x = 0x1cccc;
	params.target_scale_y = 0x1cccc;
	if ((gFrameCount & 7) == 0)
		Engine_AudioPlayCue(136);
	dx = -0x10000 - ((((u32)Engine_RandomNext() << 1) >> 16) << 16);
	dy = -((((u32)Engine_RandomNext() * 3) >> 16) * 0x3333);
	Effect_Spawn(0x1340000, 0x400000, 0xde0000, dx, dy, phase, 0xd0001,
		(const struct EffectOptions *)&params);
}

static void EV20_SCRIPT(void)
{
	u32 pass;

	Engine_AudioPlayCue(19);
	Engine_AudioPlayCue(182);
	Engine_EventBegin();
	Battle_ResetEffectCounter();

	for (pass = 0; pass < 4; pass++)
	{
		Engine_ColorBufferApplyTarget(0x204318, 1);
		Engine_ColorBufferInterpolate(1);
		Engine_TaskWait(2);
		if (pass == 0)
		{
			Map_CopyCellsTo(30, 8, 12, 8, 8, 7);
			Map_CopyCellsTo(30, 57, 19, 57, 1, 1);
		}
		Engine_ColorBufferApplyTarget(0x203108, 1);
		Engine_ColorBufferInterpolate(1);
		Engine_TaskWait(2);
	}

	Engine_TaskWait(30);

	Engine_TaskAddCallback(ImiruFuchin_BlowCaveMouthDust, 0xc80);
	Engine_TaskWait(40);
	Engine_ColorBufferApplyTarget(0x201090, 1);
	Engine_ColorBufferInterpolate(40);
	Engine_TaskWait(80);
	Engine_TaskRemoveCallback(ImiruFuchin_BlowCaveMouthDust);
	Engine_TaskWait(20);

	Engine_ColorBufferApplyTarget(0x10000, 1);
	Engine_ColorBufferInterpolate(80);

	Engine_TaskWait(80);

	Engine_GameFlagSet(0x820);
	PartyInventory_Discard(230);
	Audio_PlayCueFromEventWork();

	Engine_EventEnd();
}

static void EV01_SEARCH(void)
{
	Engine_EventBegin();
	Engine_MessageShowCentered((s32)MsgFuchinDragonFlameShowsPath, 1);
	Engine_EventEnd();
}

static void EV07_SEARCH(void)
{
	Engine_EventBegin();
	Engine_MessageShowCentered((s32)MsgFuchinSecretKiRevealed, 1);
	Engine_EventEnd();
}

static void EV19_SEARCH(void)
{
	Engine_EventBegin();
	Engine_MessageShowCentered((s32)MsgFuchinLightRevealsShadows, 1);
	Engine_EventEnd();
}

static void EV20_SEARCH(void)
{

	Engine_EventBegin();

	if (Engine_GameFlagIsSet(0x820) != 0)
	{
		Engine_MessageShowCentered((s32)MsgFuchinDragonRedEyes, 1);
	}
	else
	{
		Engine_MessageShowCentered((s32)MsgFuchinEyelessDragon, 1);
		if (PartyInventory_FindOwner(0xe6) != -1)
		{
			gEventWork->unknown_172 = 1;
		}
	}
	Engine_EventEnd();
}

static void EV00_PSYNERGY(void)
{
	struct BattleFxScene		  *scene;
	struct DisplayTransitionState *transition;

	if (gGameState.entrance <= 6)
	{
		scene = *(gWorkSlot + 56);
		transition = *(gWorkSlot + 31);
		scene->child_option = 1;
		transition->next_mode = 0;
		transition->duration = 1;
		transition->step = 1;
		Engine_ColorBufferApplySource(0, 1);
		Engine_ColorBufferApplyTarget(0x203108, 1);
		Engine_ColorBufferInterpolate(16);
		Engine_TaskWait(16);
	}
}

static void SceneActor_TurnTowardTableAngle(union FieldObject *object)
{
	struct FieldActor *actor = &object->actor;
	s32				   timer;
	s32				   direction;
	u16				   facing;

	timer = actor->timer;
	if (timer != 0)
	{
		actor->timer--;
		return;
	}
	actor->steering_flags = 0;
	direction = gImiruFuchinKeyHeadings[(gKeysHeld >> 4) & 15];
	if (direction == -1)
	{
		Object_SetMode(actor, 9);
		return;
	}
	facing = actor->facing;
	direction = (s16)(direction - facing);
	if (direction > 0x1000)
		direction = 0x1000;
	if (direction < -0x1000)
		direction = -0x1000;
	actor->facing = facing + direction;
	Object_SetMode(actor, 2);
	ObjectDispatch_ApplyValueToChildren(actor, 0x30);
}

static void SceneActor_StepSubjectAlongHeading(void)
{

	struct FieldActor	*subject;
	struct FieldPosition probe;
	s32					 heading;
	s32					 goal;
	s32					 marker;
	s32					 z;
	s32					 x;

	subject = Engine_ActorLookup(gGameState.selected_actor);

	for (;;)
	{
		heading = gImiruFuchinHeadings[(gKeysHeld >> 4) & 15];
		if ((heading << 16) == -0x10000)
		{
			return;
		}

		Engine_EventBegin();

		probe.x = (subject->x.fixed & 0xfff00000) + 0x80000;
		probe.y = subject->y.fixed;
		probe.z = (subject->z.fixed & 0xfff00000) + 0x80000;
		z = probe.z;
		x = probe.x;
		goal = GetMapCellCollision(subject->terrain_id, x, z);
		FieldVector_AddPolarOffset(0x100000, heading, &probe);

		marker = GetMapCellCollision(subject->terrain_id, probe.x, probe.z);
		if (marker == 255
			|| Map_GetTerrainHeight(subject->terrain_id, probe.x, probe.z) - subject->y.fixed
				   > 0x80000)
		{
			subject->facing = heading;
			goto tail;
		}

		probe.x = x;
		probe.z = z;
		subject->speed = 0x20000;
		subject->acceleration = 0x1999;
		subject->timer = 0;
		Engine_ObjectSetPosition(subject, x, subject->y.fixed, z);

		Object_SetMode(subject, 2);
		ObjectDispatch_ApplyValueToChildren(subject, 48);
		Object_CommitPosition(subject);
		subject->update = SceneActor_TurnTowardTableAngle;

		goto advance_probe;
	continue_probe:
		if (Map_GetTerrainHeight(subject->terrain_id, probe.x, probe.z) - subject->y.fixed
			> 0x80000)
		{
			goto finish_probe;
		}
		x = probe.x;
		z = probe.z;
		subject->speed = 0x20000;
		subject->acceleration = 0x1999;
		Engine_ObjectSetPosition(subject, probe.x, probe.y, probe.z);
		Object_CommitPosition(subject);
		if (marker != goal)
		{
			goto blocked;
		}

	advance_probe:
		FieldVector_AddPolarOffset(0x100000, heading, &probe);
		marker = GetMapCellCollision(subject->terrain_id, probe.x, probe.z);
		if (marker != 255)
		{
			goto continue_probe;
		}

	finish_probe:
		subject->speed = 0x20000;
		subject->acceleration = 0x10000;
		Engine_ObjectSetPosition(subject, x, subject->y.fixed, z);
		Object_CommitPosition(subject);
		Engine_TaskWait(2);
	}

blocked:
	subject->update = NULL;
	subject->steering_flags |= ACTOR_FACES_MOTION;

	subject->acceleration = 0x4000;

tail:
	Engine_TaskWait(10);
	Engine_EventEnd();
}

s32 *gEffectScripts[] = {
	gEffectScript0,
	gEffectScript1,
	gEffectScript2,
};

s16 gImiruFuchinKeyHeadings[] = {
	0xffff, FACING_EAST, FACING_WEST, 0xffff, FACING_NORTH, FACING_NORTHEAST, FACING_NORTHWEST, FACING_NORTH,
	FACING_SOUTH, FACING_SOUTHEAST, FACING_SOUTHWEST, FACING_SOUTH, 0xffff, FACING_EAST, FACING_WEST, 0xffff,
	0xffff, FACING_EAST, FACING_WEST, 0xffff, FACING_NORTH, 0xffff, 0xffff, FACING_NORTH,
	FACING_SOUTH, 0xffff, 0xffff, FACING_SOUTH, 0xffff, FACING_EAST, FACING_WEST, 0xffff,
};

s16 gImiruFuchinHeadings[] = {
	0xffff, FACING_EAST, FACING_WEST, 0xffff, FACING_NORTH, FACING_EAST, FACING_WEST, FACING_NORTH,
	FACING_SOUTH, FACING_EAST, FACING_WEST, FACING_SOUTH, 0xffff, FACING_EAST, FACING_WEST, 0xffff,
	FACING_EAST, FACING_EAST,
};

s32 gImiruFuchinDragonsEye = 0;

struct SceneEntrance gImiruFuchinEntrancesOther[] = {
	{ 0, CONDITION_ALWAYS, 256, 0, 600, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEntrance gImiruFuchinEntrances1[] = {
	{ 0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 1, CONDITION_ALWAYS, 264, 0, 289, FACING_NORTH, 0, 135, 62, 376, 328, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEntrance gImiruFuchinEntrances2[] = {
	{ 0, CONDITION_ALWAYS, 248, 0, 600, FACING_SOUTH, 0, 64, 64, 416, 656, 0 },
	{ 1, CONDITION_ALWAYS, 248, 0, 632, FACING_NORTH, 0, 64, 368, 416, 656, 0 },
	{ 2, CONDITION_ALWAYS, 376, 0, 488, FACING_SOUTH, 0, 64, 368, 416, 656, 0 },
	{ 3, CONDITION_ALWAYS, 120, 0, 488, FACING_SOUTH, 0, 64, 368, 416, 656, 0 },
	{ 4, CONDITION_ALWAYS, 232, 0, 408, FACING_SOUTH, 0, 64, 368, 416, 656, 0 },
	{ 5, CONDITION_ALWAYS, 232, 0, 344, FACING_NORTH, 0, 64, 48, 416, 376, 0 },
	{ 6, CONDITION_ALWAYS, 376, 0, 152, FACING_NORTH, 0, 64, 48, 416, 376, 0 },
	{ 7, CONDITION_ALWAYS, 120, 0, 200, FACING_SOUTH, 0, 64, 48, 416, 376, 0 },
	{ 8, CONDITION_ALWAYS, 248, 0, 104, FACING_SOUTH, 0, 64, 48, 416, 376, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEntrance gImiruFuchinEntrances3[] = {
	{ 1, CONDITION_ALWAYS, 248, 0, 120, FACING_SOUTH, 0, 96, 64, 416, 480, 0 },
	{ 2, CONDITION_ALWAYS, 152, 0, 200, FACING_NORTH, 0, 96, 64, 416, 480, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEntrance gImiruFuchinEntrances4[] = {
	{ 1, CONDITION_ALWAYS, 248, 0, 104, FACING_SOUTH, 0, 128, 64, 368, 256, 0 },
	{ 2, CONDITION_ALWAYS, 184, 0, 328, FACING_SOUTH, 0, 96, 256, 416, 656, 0 },
	{ 3, CONDITION_ALWAYS, 152, 0, 432, FACING_SOUTH, 0, 96, 256, 416, 656, 0 },
	{ 4, CONDITION_ALWAYS, 184, 0, 552, FACING_SOUTH, 0, 96, 256, 416, 656, 0 },
	{ 5, CONDITION_ALWAYS, 280, 0, 616, FACING_NORTH, 0, 96, 256, 416, 656, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEntrance gImiruFuchinEntrances5[] = {
	{ 1, CONDITION_ALWAYS, 328, 0, 72, FACING_SOUTH, 0, 64, 16, 416, 416, 0 },
	{ 2, CONDITION_ALWAYS, 248, 0, 392, FACING_NORTH, 0, 64, 16, 416, 416, 0 },
	{ 3, CONDITION_ALWAYS, 248, 0, 472, FACING_SOUTH, 0, 64, 416, 416, 800, 0 },
	{ 4, CONDITION_ALWAYS, 344, 0, 632, FACING_NORTH, 0, 64, 416, 416, 800, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEntrance gImiruFuchinEntrances6[] = {
	{ 1, CONDITION_ALWAYS, 264, 0, 232, FACING_NORTH, 0, 128, 16, 384, 256, 0 },
	{ 2, CONDITION_ALWAYS, 248, 0, 472, FACING_SOUTH, 0, 64, 256, 416, 720, 0 },
	{ 3, CONDITION_ALWAYS, 216, 0, 664, FACING_NORTH, 0, 64, 256, 416, 720, 0 },
	{ 4, CONDITION_ALWAYS, 280, 0, 664, FACING_NORTH, 0, 64, 256, 416, 720, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEntrance gImiruFuchinEntrances7[] = {
	{ 1, CONDITION_ALWAYS, 280, 0, 120, FACING_SOUTH, 0, 64, 0, 416, 344, 0 },
	{ 2, CONDITION_ALWAYS, 216, 0, 120, FACING_SOUTH, 0, 64, 0, 416, 344, 0 },
	{ 3, CONDITION_ALWAYS, 104, 0, 280, FACING_SOUTH, 0, 64, 0, 416, 344, 0 },
	{ 4, CONDITION_ALWAYS, 200, 0, 344, FACING_NORTH, 0, 64, 16, 416, 344, 0 },
	{ 5, CONDITION_ALWAYS, 248, 0, 312, FACING_NORTH, 0, 64, 16, 416, 344, 0 },
	{ 6, CONDITION_ALWAYS, 296, 0, 344, FACING_NORTH, 0, 64, 16, 416, 344, 0 },
	{ 7, CONDITION_ALWAYS, 200, 0, 440, FACING_SOUTH, 0, 32, 344, 432, 736, 0 },
	{ 8, CONDITION_ALWAYS, 248, 0, 440, FACING_SOUTH, 0, 32, 344, 432, 736, 0 },
	{ 9, CONDITION_ALWAYS, 296, 0, 424, FACING_SOUTH, 0, 32, 344, 432, 736, 0 },
	{ 10, CONDITION_ALWAYS, 376, 0, 472, FACING_SOUTH, 0, 32, 344, 432, 736, 0 },
	{ 11, CONDITION_ALWAYS, 248, 0, 536, FACING_NORTH, 0, 32, 344, 432, 736, 0 },
	{ 12, CONDITION_ALWAYS, 120, 0, 696, FACING_SOUTH, 0, 32, 344, 432, 752, 0 },
	{ SCENE_TABLE_END },
};

u32 gImiruFuchinExits[] = {
	SCENE_EXITS(52),
	SCENE_EXIT(1, 50, 9),
	SCENE_EXIT(0, 62, 0),
	SCENE_EXIT(1, 60, 3),
	SCENE_EXIT(2, 63, 2),
	SCENE_EXIT(3, 65, 4),
	SCENE_EXIT(4, 62, 5),
	SCENE_EXIT(5, 62, 4),
	SCENE_EXIT(6, 64, 4),
	SCENE_EXIT(7, 67, 12),
	SCENE_EXIT(8, 67, 11),
	SCENE_EXIT(0, 63, 0),
	SCENE_EXIT(1, 64, 5),
	SCENE_EXIT(2, 62, 2),
	SCENE_EXIT(0, 64, 0),
	SCENE_EXIT(1, 64, 3),
	SCENE_EXIT(2, 67, 10),
	SCENE_EXIT(3, 64, 1),
	SCENE_EXIT(4, 62, 6),
	SCENE_EXIT(5, 63, 1),
	SCENE_EXIT(0, 65, 0),
	SCENE_EXIT(1, 67, 3),
	SCENE_EXIT(2, 65, 3),
	SCENE_EXIT(3, 65, 2),
	SCENE_EXIT(4, 62, 3),
	SCENE_EXIT(0, 66, 0),
	SCENE_EXIT(1, 66, 2),
	SCENE_EXIT(2, 66, 1),
	SCENE_EXIT(3, 67, 2),
	SCENE_EXIT(4, 67, 1),
	SCENE_EXIT(0, 67, 0),
	SCENE_EXIT(1, 66, 3),
	SCENE_EXIT(2, 66, 4),
	SCENE_EXIT(3, 65, 1),
	SCENE_EXIT(4, 67, 7),
	SCENE_EXIT(5, 67, 8),
	SCENE_EXIT(6, 67, 9),
	SCENE_EXIT(7, 67, 4),
	SCENE_EXIT(8, 67, 5),
	SCENE_EXIT(9, 67, 6),
	SCENE_EXIT(10, 64, 2),
	SCENE_EXIT(11, 62, 8),
	SCENE_EXIT(12, 62, 7),
	SCENE_EXITS_END,
};

struct ScenePlacement gImiruFuchinPlacementsOther[] = {
	{ SCENE_TABLE_END },
};

struct ScenePlacement gImiruFuchinPlacements1[] = {
	{ 0x5c, 0x59, ACTOR_STAND,
		PIXELS(263), 0, PIXELS(204), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ SCENE_TABLE_END },
};

struct ScenePlacement gImiruFuchinPlacements2[] = {
	{ 0xed, CONDITION_ALWAYS, 7,
		PIXELS(136), 0, PIXELS(504), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xec, CONDITION_ALWAYS, 7,
		PIXELS(200), 0, PIXELS(536), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xec, CONDITION_ALWAYS, 7,
		PIXELS(296), 0, PIXELS(536), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ SCENE_TABLE_END },
};

struct ScenePlacement gImiruFuchinPlacements3[] = {
	{ 0xed, CONDITION_ALWAYS, 7,
		PIXELS(216), 0, PIXELS(328), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xec, CONDITION_ALWAYS, 7,
		PIXELS(248), 0, PIXELS(280), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xec, CONDITION_ALWAYS, 7,
		PIXELS(312), 0, PIXELS(408), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x5d, 0x6e, ACTOR_STAND,
		PIXELS(344), 0, PIXELS(280), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ SCENE_TABLE_END },
};

struct ScenePlacement gImiruFuchinPlacements4[] = {
	{ 0xed, CONDITION_ALWAYS, 7,
		PIXELS(328), 0, PIXELS(408), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xed, CONDITION_ALWAYS, 7,
		PIXELS(248), 0, PIXELS(568), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xec, CONDITION_ALWAYS, 7,
		PIXELS(248), 0, PIXELS(488), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xec, CONDITION_ALWAYS, 7,
		PIXELS(248), 0, PIXELS(504), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ SCENE_TABLE_END },
};

struct ScenePlacement gImiruFuchinPlacements5[] = {
	{ 0xec, CONDITION_ALWAYS, 7,
		PIXELS(328), 0, PIXELS(168), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xed, CONDITION_ALWAYS, 7,
		PIXELS(360), 0, PIXELS(264), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xed, CONDITION_ALWAYS, 7,
		PIXELS(120), 0, PIXELS(744), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ SCENE_TABLE_END },
};

struct ScenePlacement gImiruFuchinPlacements7[] = {
	{ 0xdf, CONDITION_ALWAYS, 7,
		PIXELS(312), 0, PIXELS(128), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEvent gImiruFuchinEventsOther[] = {
	{ SCENE_EVENTS_END },
};

struct SceneEvent gImiruFuchinEvents1[] = {
	{ EVENT_TOUCH, 1, CONDITION_ALWAYS, EVENT_SCRIPT(EV01_TOUCH) },
	{ EVENT_TOUCH, 10, CONDITION_ALWAYS, EVENT_SCRIPT(EV10_TOUCH_1) },
	{ EVENT_TALK, 8, CONDITION_ALWAYS, EVENT_SCRIPT(EV08_TALK) },
	{ SCENE_EVENTS_END },
};

struct SceneEvent gImiruFuchinEvents2[] = {
	{ TOUCH_WALKING_INTO(FACING_EAST), 11, CONDITION_ALWAYS, EVENT_SCRIPT(EV11_TOUCH_2) },
	{ TOUCH_WALKING_INTO(FACING_WEST), 12, CONDITION_ALWAYS, EVENT_SCRIPT(EV12_TOUCH_2) },
	{ EVENT_TOUCH, 11, CONDITION_ALWAYS, EVENT_SCRIPT(ActorPresentation_AdvanceActorEightStates) },
	{ EVENT_TOUCH, 12, CONDITION_ALWAYS, EVENT_SCRIPT(ActorPresentation_AdvanceActorEightStates) },
	{ TOUCH_WALKING_INTO(FACING_SOUTH), 13, CONDITION_ALWAYS, EVENT_SCRIPT(EV13_TOUCH_2) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 14, CONDITION_ALWAYS, EVENT_SCRIPT(EV14_TOUCH_2) },
	{ EVENT_TOUCH, 13, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_SetActor9Values1And2) },
	{ EVENT_TOUCH, 14, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_SetActor9Values1And2) },
	{ TOUCH_WALKING_INTO(FACING_SOUTH), 15, CONDITION_ALWAYS, EVENT_SCRIPT(EV15_TOUCH_2) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 16, CONDITION_ALWAYS, EVENT_SCRIPT(EV16_TOUCH_2) },
	{ EVENT_TOUCH, 15, CONDITION_ALWAYS, EVENT_SCRIPT(ActorPresentation_AdvanceActorTenStates) },
	{ EVENT_TOUCH, 16, CONDITION_ALWAYS, EVENT_SCRIPT(ActorPresentation_AdvanceActorTenStates) },
	{ EVENT_EXIT, 21, CONDITION_ALWAYS, 1 },
	{ EVENT_EXIT, 22, CONDITION_ALWAYS, 2 },
	{ EVENT_EXIT, 23, CONDITION_ALWAYS, 3 },
	{ EVENT_EXIT, 24, CONDITION_ALWAYS, 4 },
	{ EVENT_EXIT, 25, CONDITION_ALWAYS, 5 },
	{ EVENT_EXIT, 26, CONDITION_ALWAYS, 6 },
	{ 0x21, 27, CONDITION_ALWAYS, 7 },
	{ EVENT_EXIT, 28, CONDITION_ALWAYS, 8 },
	{ SEARCH(SEARCH_UNNAMED), 1, CONDITION_ALWAYS, EVENT_SCRIPT(EV01_SEARCH) },
	{ SEARCH(SEARCH_CHEST), 100, 0xef1, 0x500001 },
	{ SCENE_EVENTS_END },
};

struct SceneEvent gImiruFuchinEvents3[] = {
	{ TOUCH_WALKING_INTO(FACING_EAST), 11, 0x306, EVENT_SCRIPT(FieldScene_RunActor8Transition304) },
	{ TOUCH_WALKING_INTO(FACING_WEST), 11, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunActor8Transition305) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 17, CONDITION_ALWAYS, EVENT_SCRIPT(ImiruFuchin_HopOnTrigger) },
	{ TOUCH_WALKING_INTO(FACING_EAST), 17, CONDITION_ALWAYS, EVENT_SCRIPT(EV17_TOUCH_3) },
	{ TOUCH_WALKING_INTO(FACING_WEST), 12, CONDITION_ALWAYS, EVENT_SCRIPT(EV12_TOUCH_3) },
	{ EVENT_TOUCH, 11, CONDITION_ALWAYS, EVENT_SCRIPT(ActorPresentation_AdvanceActorEightStates) },
	{ EVENT_TOUCH, 12, CONDITION_ALWAYS, EVENT_SCRIPT(ActorPresentation_AdvanceActorEightStates) },
	{ EVENT_TOUCH, 17, CONDITION_ALWAYS, EVENT_SCRIPT(ActorPresentation_AdvanceActorEightStates) },
	{ TOUCH_WALKING_INTO(FACING_WEST), 13, CONDITION_ALWAYS, EVENT_SCRIPT(ImiruFuchin_HopOnTrigger) },
	{ TOUCH_WALKING_INTO(FACING_WEST), 14, 0x304, EVENT_SCRIPT(ImiruFuchin_HopOnTrigger) },
	{ TOUCH_WALKING_INTO(FACING_SOUTH), 13, CONDITION_ALWAYS, EVENT_SCRIPT(EV13_TOUCH_3) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 14, CONDITION_ALWAYS, EVENT_SCRIPT(EV14_TOUCH_3) },
	{ EVENT_TOUCH, 13, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_SetActor9Values1And2) },
	{ EVENT_TOUCH, 14, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_SetActor9Values1And2) },
	{ TOUCH_WALKING_INTO(FACING_SOUTH), 15, CONDITION_ALWAYS, EVENT_SCRIPT(EV15_TOUCH_3) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 16, CONDITION_ALWAYS, EVENT_SCRIPT(EV16_TOUCH_3) },
	{ EVENT_TOUCH, 15, CONDITION_ALWAYS, EVENT_SCRIPT(ActorPresentation_AdvanceActorTenStates) },
	{ EVENT_TOUCH, 16, CONDITION_ALWAYS, EVENT_SCRIPT(ActorPresentation_AdvanceActorTenStates) },
	{ EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
	{ EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
	{ EVENT_TALK, 11, CONDITION_ALWAYS, EVENT_SCRIPT(EV11_TALK) },
	{ SCENE_EVENTS_END },
};

struct SceneEvent gImiruFuchinEvents4[] = {
	{ TOUCH_WALKING_INTO(FACING_WEST), 10, CONDITION_ALWAYS, EVENT_SCRIPT(EV10_TOUCH_4) },
	{ TOUCH_WALKING_INTO(FACING_EAST), 11, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunActor8Transition308And309) },
	{ TOUCH_WALKING_INTO(FACING_WEST), 11, CONDITION_ALWAYS, EVENT_SCRIPT(ActorPresentation_SetupActorEightForFlags308And309Guarded) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 11, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_ApplyOffset0Neg32) },
	{ TOUCH_WALKING_INTO(FACING_EAST), 12, CONDITION_ALWAYS, EVENT_SCRIPT(EV12_TOUCH_4) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 12, IF_FLAG_SET(0x30b), EVENT_SCRIPT(FieldScene_ApplyOffset0Neg32) },
	{ EVENT_TOUCH, 10, CONDITION_ALWAYS, EVENT_SCRIPT(ActorPresentation_AdvanceActorEightStates) },
	{ EVENT_TOUCH, 11, CONDITION_ALWAYS, EVENT_SCRIPT(ActorPresentation_AdvanceActorEightStates) },
	{ EVENT_TOUCH, 12, CONDITION_ALWAYS, EVENT_SCRIPT(ActorPresentation_AdvanceActorEightStates) },
	{ TOUCH_WALKING_INTO(FACING_WEST), 14, CONDITION_ALWAYS, EVENT_SCRIPT(EV14_TOUCH_4) },
	{ TOUCH_WALKING_INTO(FACING_EAST), 13, CONDITION_ALWAYS, EVENT_SCRIPT(EV13_TOUCH_4) },
	{ EVENT_TOUCH, 13, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_SetActor9Values1And2) },
	{ EVENT_TOUCH, 14, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_SetActor9Values1And2) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 15, CONDITION_ALWAYS, EVENT_SCRIPT(EV15_TOUCH_4) },
	{ TOUCH_WALKING_INTO(FACING_SOUTH), 16, CONDITION_ALWAYS, EVENT_SCRIPT(EV16_TOUCH_4) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 17, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunActor10Transition30BTo30E) },
	{ TOUCH_WALKING_INTO(FACING_SOUTH), 17, 0x308, EVENT_SCRIPT(FieldScene_RunFlag311DialogueBranch) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 18, CONDITION_ALWAYS, EVENT_SCRIPT(ActorPresentation_SetupActorTenForFlags30bAnd30d) },
	{ TOUCH_WALKING_INTO(FACING_SOUTH), 18, 0x310, EVENT_SCRIPT(FieldScene_RunActor10Flags30bTo30eSequenceA) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 19, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunActor10Flags30bTo30eSequenceB) },
	{ TOUCH_WALKING_INTO(FACING_SOUTH), 19, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunActor10Flags30bTo30eSequenceC) },
	{ EVENT_TOUCH, 15, CONDITION_ALWAYS, EVENT_SCRIPT(ActorPresentation_AdvanceActorTenStates) },
	{ EVENT_TOUCH, 16, CONDITION_ALWAYS, EVENT_SCRIPT(ActorPresentation_AdvanceActorTenStates) },
	{ EVENT_TOUCH, 17, CONDITION_ALWAYS, EVENT_SCRIPT(ActorPresentation_AdvanceActorTenStates) },
	{ EVENT_TOUCH, 18, CONDITION_ALWAYS, EVENT_SCRIPT(ActorPresentation_AdvanceActorTenStates) },
	{ EVENT_TOUCH, 19, CONDITION_ALWAYS, EVENT_SCRIPT(ActorPresentation_AdvanceActorTenStates) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 20, CONDITION_ALWAYS, EVENT_SCRIPT(EV20_TOUCH) },
	{ TOUCH_WALKING_INTO(FACING_SOUTH), 21, 0x308, EVENT_SCRIPT(EV21_TOUCH) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 22, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunActor11Flags30fTo312Sequence) },
	{ TOUCH_WALKING_INTO(FACING_SOUTH), 22, CONDITION_ALWAYS, EVENT_SCRIPT(ActorPresentation_SetupActorElevenAt0_112) },
	{ TOUCH_WALKING_INTO(FACING_WEST), 23, CONDITION_ALWAYS, EVENT_SCRIPT(SceneState_ApplyOffsetMinus32) },
	{ TOUCH_WALKING_INTO(FACING_SOUTH), 23, CONDITION_ALWAYS, EVENT_SCRIPT(ActorPresentation_SetupActorElevenAt0_64) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 24, CONDITION_ALWAYS, EVENT_SCRIPT(ActorPresentation_SetupActorElevenAt0_80) },
	{ TOUCH_WALKING_INTO(FACING_SOUTH), 24, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunActor11Transition) },
	{ EVENT_TOUCH, 20, CONDITION_ALWAYS, EVENT_SCRIPT(SceneActor_SetActor11Values1And2) },
	{ EVENT_TOUCH, 21, CONDITION_ALWAYS, EVENT_SCRIPT(SceneActor_SetActor11Values1And2) },
	{ EVENT_TOUCH, 22, CONDITION_ALWAYS, EVENT_SCRIPT(SceneActor_SetActor11Values1And2) },
	{ EVENT_TOUCH, 23, CONDITION_ALWAYS, EVENT_SCRIPT(SceneActor_SetActor11Values1And2) },
	{ EVENT_TOUCH, 24, CONDITION_ALWAYS, EVENT_SCRIPT(SceneActor_SetActor11Values1And2) },
	{ TOUCH_WALKING_INTO(FACING_SOUTH), 25, CONDITION_ALWAYS, EVENT_SCRIPT(ImiruFuchin_HopOnTrigger) },
	{ 0x21, 1, CONDITION_ALWAYS, 1 },
	{ 0x21, 2, CONDITION_ALWAYS, 2 },
	{ 0x31, 3, CONDITION_ALWAYS, 3 },
	{ EVENT_EXIT, 4, CONDITION_ALWAYS, 4 },
	{ EVENT_EXIT, 5, CONDITION_ALWAYS, 5 },
	{ SEARCH(SEARCH_UNNAMED), 30, 0xf13, EVENT_SCRIPT(EV30_SEARCH) },
	{ SCENE_EVENTS_END },
};

struct SceneEvent gImiruFuchinEvents5[] = {
	{ TOUCH_WALKING_INTO(FACING_SOUTH), 11, CONDITION_ALWAYS, EVENT_SCRIPT(EV11_TOUCH_5) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 12, CONDITION_ALWAYS, EVENT_SCRIPT(EV12_TOUCH_5) },
	{ EVENT_TOUCH, 11, CONDITION_ALWAYS, EVENT_SCRIPT(ActorPresentation_AdvanceActorEightStates) },
	{ EVENT_TOUCH, 12, CONDITION_ALWAYS, EVENT_SCRIPT(ActorPresentation_AdvanceActorEightStates) },
	{ TOUCH_WALKING_INTO(FACING_WEST), 13, 0x313, EVENT_SCRIPT(EV13_TOUCH_5) },
	{ TOUCH_WALKING_INTO(FACING_EAST), 14, CONDITION_ALWAYS, EVENT_SCRIPT(EV14_TOUCH_5) },
	{ EVENT_TOUCH, 13, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_SetActor9Values1And2) },
	{ EVENT_TOUCH, 14, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_SetActor9Values1And2) },
	{ TOUCH_WALKING_INTO(FACING_EAST), 15, CONDITION_ALWAYS, EVENT_SCRIPT(EV15_TOUCH_5) },
	{ TOUCH_WALKING_INTO(FACING_WEST), 16, CONDITION_ALWAYS, EVENT_SCRIPT(EV16_TOUCH_5) },
	{ EVENT_TOUCH, 15, CONDITION_ALWAYS, EVENT_SCRIPT(ActorPresentation_AdvanceActorTenStates) },
	{ EVENT_TOUCH, 16, CONDITION_ALWAYS, EVENT_SCRIPT(ActorPresentation_AdvanceActorTenStates) },
	{ 0x21, 1, CONDITION_ALWAYS, 1 },
	{ EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
	{ EVENT_EXIT, 3, CONDITION_ALWAYS, 3 },
	{ EVENT_EXIT, 4, CONDITION_ALWAYS, 4 },
	{ SEARCH(SEARCH_CHEST), 100, 0xf6b, FIND_ITEM(0x8) },
	{ SCENE_EVENTS_END },
};

struct SceneEvent gImiruFuchinEvents6[] = {
	{ EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
	{ EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
	{ EVENT_EXIT, 3, CONDITION_ALWAYS, 3 },
	{ EVENT_EXIT, 4, CONDITION_ALWAYS, 4 },
	{ SEARCH(SEARCH_UNNAMED), 7, CONDITION_ALWAYS, EVENT_SCRIPT(EV07_SEARCH) },
	{ SEARCH(SEARCH_CHEST), 100, 0xf14, FIND_ITEM(0xc8) },
	{ SCENE_EVENTS_END },
};

struct SceneEvent gImiruFuchinEvents7[] = {
	{ EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
	{ EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
	{ 0x31, 3, CONDITION_ALWAYS, 3 },
	{ EVENT_EXIT, 4, CONDITION_ALWAYS, 4 },
	{ EVENT_EXIT, 5, CONDITION_ALWAYS, 5 },
	{ EVENT_EXIT, 6, CONDITION_ALWAYS, 6 },
	{ EVENT_EXIT, 7, CONDITION_ALWAYS, 7 },
	{ EVENT_EXIT, 8, CONDITION_ALWAYS, 8 },
	{ EVENT_EXIT, 9, CONDITION_ALWAYS, 9 },
	{ 0x31, 10, CONDITION_ALWAYS, 10 },
	{ EVENT_EXIT, 11, CONDITION_ALWAYS, 11 },
	{ 0x31, 12, CONDITION_ALWAYS, 12 },
	{ EVENT_TOUCH, 15, CONDITION_ALWAYS, EVENT_SCRIPT(EV15_TOUCH_7) },
	{ EVENT_TOUCH, 16, CONDITION_ALWAYS, EVENT_SCRIPT(EV16_TOUCH_7) },
	{ SEARCH(SEARCH_UNNAMED), 19, CONDITION_ALWAYS, EVENT_SCRIPT(EV19_SEARCH) },
	{ 0xe604, 20, 0x820, EVENT_SCRIPT(EV20_SCRIPT) },
	{ SEARCH(SEARCH_UNNAMED), 20, CONDITION_ALWAYS, EVENT_SCRIPT(EV20_SEARCH) },
	{ PSYNERGY_IN_AREA(ABILITY_REVEAL, 4), 0, 0x820, EVENT_SCRIPT(EV00_PSYNERGY) },
	{ SCENE_EVENTS_END },
};

s32 gEffectScript0[] = {
	SC_WAIT(1),
	SC_LOOP(22, SC_TOP),
	SC_CALL(OverlayObject_ApplyValue15),
	SC_WAIT(1),
	SC_POS(0, 0, 0),
	SC_WAIT(1),
	SC_FREE,
};

s32 gEffectScript1[] = {
	SC_WAIT(1),
	SC_LOOP(44, SC_TOP),
	SC_CALL(OverlayObject_ApplyValue15),
	SC_WAIT(1),
	SC_POS(0, 0, 0),
	SC_WAIT(1),
	SC_FREE,
};

s32 gEffectScript2[] = {
	SC_WAIT(1),
	SC_LOOP(126, SC_TOP),
	SC_CALL(OverlayObject_ApplyValue15),
	SC_WAIT(1),
	SC_POS(0, 0, 0),
	SC_WAIT(1),
	SC_FREE,
};
