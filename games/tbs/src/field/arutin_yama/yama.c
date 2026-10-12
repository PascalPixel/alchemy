#include "game_state.h"
#include "scene_ids.h"
#include "unit/import.h"
#include "unit/field/arutin_yama/yama.h"
#include "unit/battle/effect/object.h"
#include "unit/field/common/script/field.h"
#include "unit/system/scheduler/runtime.h"
#include "battle.h"
#include "keystate.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "global_cells.h"
#include "types.h"
#include "field_event.h"
#include "io_reg.h"
#include "text/msg_ids.h"
#include "field_service.h"
#include "arutin.h"
#include "call.h"
#include "field_effect.h"
#include "iwram_call.h"
#include "dma.h"

#include "types.h"

#include "facing_object.h"
#include "field_event.h"
#include "field_scene.h"

#include "battle_effect_runtime.h"
#include "ability_ids.h"
#include "item_ids.h"
#include "objcmd.h"
#include "irq.h"

// 前方宣言
static void FieldScene_RunSharedSetPiece(s32 a0);
static void ArutinYama_ApplyEntryState(void);
static void ArutinYama_RestoreAreaThree(void);
static void ArutinYama_RestoreAreaFour(void);
static void ArutinYama_RestoreAreaFive(void);
static void ArutinYama_RestoreAreaSix(void);
static void ArutinYama_RestoreAreaSeven(void);
static void ArutinYama_PlaceFlaggedActors(void);
static void ArutinYama_RestoreAreaTen(void);
static void ArutinYama_RestoreAreaEleven(void);
static void ArutinYama_StartLeafGust(s32 x, s32 y, s32 z, s32 facing);
static void FieldScene_RunScene3a4SequenceG(void);
static void SceneState_ForwardByRuntimeSelector(s32 arg);
static void FieldScene_RunActorTenFourStepSequence(void);
static void FieldScene_RunOpeningAuxiliarySequence(void);
static void FieldScene_RunScene3a4SequenceF(void);
static void FieldScene_RunScene3a4SequenceE(void);
static void RunEventScript01(void);
static void SceneActor_SetActor12ModeByActorZeroHeight(void);
static void SceneActor_ClearCollisionFlagAndPlaceMarker(s32 no);
static void ArutinYama_RunRollingObject(s32 id, s32 heading);
static void ArutinYama_StartPaletteAnim(void);

#define ABS(x) ((x) < 0 ? -(x) : (x))

enum
{
	ITEM_FROST_JEWEL = 202
};

extern s32 ArutinYama_LeafMode;
extern s32 ArutinYama_LeafOrigin[3];

static void EV14_TALK(void)
{

	BattleFx_SetPhaseRequest(0xE, 0x17);
}

static s32 OverlayObject_UpdateFacingTowardTarget(struct FacingObject *obj)
{
	s32					 delta;
	u16					 old;
	s32					 angle;
	struct FacingObject *target;

	target = obj->facing_target;
	if (target != 0)
	{
		obj->facing_flags = (0xFE & obj->facing_flags);
		angle = (u16)ArcTan2(
			target->position_z - obj->position_z, target->position_x - obj->position_x);
		old = obj->facing;
		delta = (s16)(angle - old);
		if (delta != 0)
		{
			if (delta > 0x1000)
			{
				delta = 0x1000;
			}

			if (delta < -0x1000)
			{
				delta = -0x1000;
			}
			obj->facing = (old + delta);
		}
	}
	return 1;
}

static s32 SceneActor_CopyActor8PositionWithFixedY(struct FieldActor *dst)
{
	struct FieldActor *src = Object_GetById(8);

	dst->x.fixed = src->x.fixed;
	dst->y.fixed = 0xFFF40000;
	dst->z.fixed = src->z.fixed;
	return 0;
}

static s32 OverlayObject_IntegrateAndDamp(struct FieldActor *actor)
{
	s32 rise;
	s32 growth;

	actor->x.fixed += actor->velocity_x;
	rise = actor->velocity_y;
	actor->y.fixed += rise;
	growth = actor->velocity_z;
	actor->scale_x += growth;
	actor->scale_y += growth;
	actor->velocity_y = rise - actor->gravity;
	return 0;
}

const struct SceneEntrance *Scene_GetEntrances(void)
{
	s16 scene = gGameState.scene;

	if (scene == (s32)&SceneId_ArutinYama1)
	{
		return gArutinYamaEntrances1;
	}
	if (scene == (s32)&SceneId_ArutinYama2)
	{
		return gArutinYamaEntrances2;
	}
	if (scene == (s32)&SceneId_ArutinYama3)
	{
		return gArutinYamaEntrances3;
	}
	if (scene == (s32)&SceneId_ArutinYama4)
	{
		return gArutinYamaEntrances4;
	}
	if (scene == (s32)&SceneId_ArutinYama5)
	{
		return gArutinYamaEntrances5;
	}
	if (scene == (s32)&SceneId_ArutinYama6)
	{
		return gArutinYamaEntrances6;
	}
	if (scene == (s32)&SceneId_ArutinYama7)
	{
		return gArutinYamaEntrances7;
	}
	if (scene == (s32)&SceneId_ArutinYama8)
	{
		return gArutinYamaEntrances8;
	}
	if (scene == (s32)&SceneId_ArutinYama9)
	{
		return gArutinYamaEntrances9;
	}
	if (scene == (s32)&SceneId_ArutinYama10)
	{
		return gArutinYamaEntrances10;
	}
	if (scene == (s32)&SceneId_ArutinYama11)
	{
		return gArutinYamaEntrances11;
	}
	return gArutinYamaEntrancesOther;
}

const struct SceneRegion *Scene_GetRegions(void)
{
	s16 scene = gGameState.scene;

	if (scene == (s32)&SceneId_ArutinYama9)
	{
		return (const struct SceneRegion *)gArutinYamaRegions9;
	}
	if (scene == (s32)&SceneId_ArutinYama10)
	{
		return (const struct SceneRegion *)gArutinYamaRegions10;
	}
	return 0;
}

u8 *ArutinYama_GetExits(void)
{
	return (u8 *)ArutinYama_StatueTable;
}

const struct ScenePlacement *Scene_GetPlacements(void)
{
	s16 scene = gGameState.scene;

	if (scene == (s32)&SceneId_ArutinYama1)
	{
		return gArutinYamaPlacements1;
	}
	if (scene == (s32)&SceneId_ArutinYama3)
	{
		return gArutinYamaPlacements3;
	}
	if (scene == (s32)&SceneId_ArutinYama5)
	{
		return gArutinYamaPlacements5;
	}
	if (scene == (s32)&SceneId_ArutinYama6)
	{
		return gArutinYamaPlacements6;
	}
	if (scene == (s32)&SceneId_ArutinYama7)
	{
		return gArutinYamaPlacements7;
	}
	if (scene == (s32)&SceneId_ArutinYama8)
	{
		return gArutinYamaPlacements8;
	}
	if (scene == (s32)&SceneId_ArutinYama9)
	{
		return gArutinYamaPlacements9;
	}
	if (scene == (s32)&SceneId_ArutinYama10)
	{
		return gArutinYamaPlacements10;
	}
	if (scene == (s32)&SceneId_ArutinYama11)
	{
		return gArutinYamaPlacements11;
	}
	return gArutinYamaPlacementsOther;
}

static void EV08_TALK(s32 a0)
{
	struct FieldActor *rec7;
	struct FieldActor *rec8;

	rec8 = Object_GetById(0);
	rec7 = Object_GetById(8);
	Battle_ResetEffectCounter();
	Engine_EventBegin();
	Engine_ActorSetAnimation(0, 22);
	Engine_EventWait(10);
	Engine_AudioPlayCue(152);
	Engine_ActorSetSpeed(0, 0x33333, 0x19999);
	rec8->velocity_y = ((ABS(rec7->y.fixed - rec8->y.fixed) >> 14) << 14) + 0x40000;
	Engine_ActorSetAnimation(0, 7);
	Engine_ObjectSetPosition(rec8, rec7->x.fixed, rec7->y.fixed, rec7->z.fixed);
	Engine_TaskWait(10);
	rec8->sprite->priority = 3;
	Engine_ActorWaitForMove(0);
	for (;;)
	{
		if (!((rec7->y.fixed >> 14) < (rec8->y.fixed >> 14)))
			break;
		Engine_TaskWait(1);
	}
	Engine_EventEnd();
	Engine_AudioPlayCue(159);
	ArutinYama_RunRollingObject(a0, 0);
	Engine_TaskWait(20);
	BattleFx_PlayQueuedSound();
}

static void ArutinYama_StartAreaOneEncounter(void)
{
	Battle_SetStart(3);
	Party_SetFields1ceAnd1d0((s32)&SceneId_ArutinYama1, 99);
	BattleFx_SetWeightedResult(53, 2);
}

static void ArutinYama_StartAreaThreeEncounter(void)
{
	Battle_SetStart(3);
	Party_SetFields1ceAnd1d0((s32)&SceneId_ArutinYama3, 99);
	BattleFx_SetWeightedResult(53, 2);
}

static void ArutinYama_StartAreaFiveEncounter(void)
{
	Battle_SetStart(3);
	Party_SetFields1ceAnd1d0((s32)&SceneId_ArutinYama5, 99);
	BattleFx_SetWeightedResult(53, 2);
}

static void EV10_PSYNERGY(void)
{
	struct FieldActor *a = Object_GetById(10);

	if (a != 0)
	{
		s32 x = 24;
		s32 y = 26;
		s32 t;

		Engine_MapCopyCellAttributes(x, 27, 2, 1, x, y);
		t = a->x.fixed >> 20;
		if (t == 25)
		{
			Engine_MapCopyCellAttributes(0, 0, 1, 1, t, y);
		}
		else
		{
			Engine_MapCopyCellAttributes(0, 0, 1, 1, x, y);
		}
		Engine_ActorSetSpriteFlags(a, 0);
		a->motion_flags = 0;
		Engine_MapRedraw();
		Engine_TaskWait(1);
	}
}

static void EV08_PSYNERGY(void)
{

	struct FieldActor *p;
	s32				   s0;
	s32				   s1;

	p = Object_GetById(8);
	s0 = 9;
	s1 = 13;
	Engine_MapCopyCellAttributes(7, 13, 1, 1, s0, s1);
	if (p != 0)
	{
		Engine_ActorSetSpriteFlags(p, 0);
		p->y.fixed += 0xffe00000;
		p->priority_flags = 2;
	}
	Engine_GameFlagSet(0x200);
}

static void EV09_PSYNERGY(void)
{

	struct FieldActor *p;
	s32				   s0;
	s32				   s1;

	p = Object_GetById(9);
	s0 = 17;
	s1 = 13;
	Engine_MapCopyCellAttributes(29, 1, 3, 1, s0, s1);
	if (p != 0)
	{
		p->motion_flags = 2;
	}
	Engine_GameFlagSet(0x201);
}

static void EV09_PSYNERGY_7(void)
{
	struct FieldActor *p;
	s32				   s0;

	p = Object_GetById(9);
	s0 = 26;
	Engine_MapCopyCellAttributes(0, 0, 1, 1, s0, s0);
	if (p != 0)
	{
		Engine_ActorSetSpriteFlags(p, 0);
		p->y.fixed += 0xffe00000;
		p->priority_flags = 2;
	}
	Engine_GameFlagSet(0x200);
}

static void EV09_PSYNERGY_5(void)
{
	struct FieldActor *p;
	s32				   s0;
	s32				   s1;

	p = Object_GetById(9);
	s0 = 25;
	s1 = 13;
	Engine_MapCopyCellAttributes(23, 13, 1, 1, s0, s1);
	if (p != 0)
	{
		Engine_ActorSetSpriteFlags(p, 0);
		p->y.fixed += 0xffe00000;
		p->priority_flags = 2;
	}
	Engine_GameFlagSet(0x200);
}

static void EV09_PSYNERGY_6(void)
{

	struct FieldActor *rec;
	s32				   s0;
	s32				   s1;

	rec = Object_GetById(9);
	s0 = 43;
	s1 = 41;
	Engine_MapCopyCellAttributes(45, 41, 1, 1, s0, s1);
	if (rec != 0)
	{
		Engine_ActorSetSpriteFlags(rec, 0);
		rec->y.fixed += 0xffe00000;
		rec->priority_flags = 2;
	}
	Engine_GameFlagSet(0x200);
}

static void EV11_PSYNERGY_7(void)
{
	struct FieldActor *p;
	s32				   s0;
	s32				   s1;

	p = Object_GetById(11);
	s0 = 17;
	s1 = 10;
	Engine_MapCopyCellAttributes(1, 0, 1, 1, s0, s1);
	if (p != 0)
	{
		Engine_ActorSetSpriteFlags(p, 0);
		p->y.fixed += 0xffe00000;
		p->priority_flags = 2;
	}
	Engine_GameFlagSet(0x201);
}

static void EV12_PSYNERGY(void)
{

	struct FieldActor *p;
	s32				   s0;
	s32				   s1;

	p = Object_GetById(12);
	s0 = 26;
	s1 = 15;
	Engine_MapCopyCellAttributes(1, 0, 1, 1, s0, s1);
	if (p != 0)
	{
		Engine_ActorSetSpriteFlags(p, 0);
		p->y.fixed += 0xffe00000;
		p->priority_flags = 2;
	}
	Engine_GameFlagSet(0x204);
}

static void SceneState_SetDispcntBit9ByThreshold(void)
{
#ifdef MODERN
	// modern GCC holds one type for a name in a file
	extern u16 ArutinYama_RiseTimer_modern __asm__("ArutinYama_RiseTimer");
#define ArutinYama_RiseTimer ArutinYama_RiseTimer_modern
#else
	extern u16 ArutinYama_RiseTimer;
#endif

	volatile u16 *reg = &REG_DISPCNT;
	s16			  v = *reg & 0xfdff;

	if ((u32)(Engine_RandomNext() * 100) >> 16 >= ArutinYama_RiseTimer)
	{
		s32 k = 0x200;

		v |= k;
	}
	{
		u32 t = (u16)v;

		*reg = t;
	}
}
#ifdef MODERN
#undef ArutinYama_RiseTimer
#endif

static void FieldScene_RunEarlySequence(void)
{
	extern u8 ArutinYama_RiseTimer[];

	s32				   i;
	struct FieldActor *record;
	u8				  *p5;
	u8				  *rec;
	s32				   v;

	p5 = gMapWork[0];
	Event_PlayCue(230);
	Work_SetValuesIfNonNegative(0x20000, 0x20000, 0x10000);
	Engine_EventWait(10);
	rec = p5 + 356;
	for (i = 0; i <= 23; i++)
	{
		*(s32 *)(rec + 12) -= 0x10000;
		Engine_TaskWait(4);
		if (i == 8)
		{
			record = Object_GetById(8);
			record->scale_x = 0x1999;
			record = Actor_Get(8);
			record->scale_y = 0x1999;
			Actor_SetPosition(8, 0x980000, 0xd80000);
			Engine_ActorEnableActionCallback(8, (const u8 *)ArutinYama_EarlyActorScript);
		}
	}
	Runtime_SetIrqHandler(1, 0, SceneState_SetDispcntBit9ByThreshold);
	*(u16 *)ArutinYama_RiseTimer = 0;
	do
	{
		Engine_TaskWait(1);
		v = *(u16 *)ArutinYama_RiseTimer + 1;
		*(u16 *)ArutinYama_RiseTimer = v;
	} while ((u32)(v << 16) <= 0x640000);
	Engine_TaskWait(1);
	Runtime_SetIrqHandler(1, 0, 0);
	Event_PlayCue(0x121);
	Work_SetValuesIfNonNegative(-1, -1, 0xe666);
	Engine_EventWait(30);
	Map_CopyCellAttributes(0, 0, 1, 2, 3, 14);
	GameFlag_Set(0x8fd);
}

static void FieldScene_RunScene3a4SequenceH(void)
{
	extern u8 ArutinYama_RiseTimer[];

	s32 i;
	u8 *p5;
	u8 *rec;
	s32 v;

	p5 = gMapWork[0];
	Map_CopyCells(93, 41, 16, 4, 77, 28);
	Event_PlayCue(230);
	Work_SetValuesIfNonNegative(0x20000, 0x20000, 0x10000);
	Engine_EventWait(10);
	rec = p5 + 356;
	for (i = 23; i >= 0; i--)
	{
		*(s32 *)(rec + 12) -= 0x10000;
		Engine_TaskWait(4);
	}
	Runtime_SetIrqHandler(1, 0, SceneState_SetDispcntBit9ByThreshold);
	*(u16 *)ArutinYama_RiseTimer = 0;
	do
	{
		Engine_TaskWait(1);
		v = *(u16 *)ArutinYama_RiseTimer + 1;
		*(u16 *)ArutinYama_RiseTimer = v;
	} while ((u32)(v << 16) <= 0x640000);
	Engine_TaskWait(1);
	Runtime_SetIrqHandler(1, 0, 0);
	Event_PlayCue(0x121);
	Work_SetValuesIfNonNegative(-1, -1, 0xe666);
	Engine_EventWait(30);
	Map_CopyCells(77, 41, 16, 4, 77, 28);
	GameFlag_Set(0x8fe);
}

static void FieldScene_RunScene3a4SequenceI(void)
{
	extern u8 ArutinYama_RiseTimer[];

	s32 i;
	u8 *p8;
	u8 *rec;
	s32 v;

	p8 = gMapWork[0];
	Map_CopyCellsTo(113, 31, 103, 17, 1, 1);
	Map_CopyCellsTo(111, 32, 104, 18, 3, 2);
	Map_CopyCellsTo(64, 32, 103, 18, 1, 2);
	Event_PlayCue(230);
	Work_SetValuesIfNonNegative(0x20000, 0x20000, 0x10000);
	Engine_EventWait(10);
	rec = p8 + 356;
	for (i = 23; i >= 0; i--)
	{
		*(s32 *)(rec + 12) -= 0x10000;
		Engine_TaskWait(4);
	}
	Runtime_SetIrqHandler(1, 0, SceneState_SetDispcntBit9ByThreshold);
	*(u16 *)ArutinYama_RiseTimer = 0;
	do
	{
		Engine_TaskWait(1);
		v = *(u16 *)ArutinYama_RiseTimer + 1;
		*(u16 *)ArutinYama_RiseTimer = v;
	} while ((u32)(v << 16) <= 0x640000);
	Engine_TaskWait(1);
	Runtime_SetIrqHandler(1, 0, 0);
	Event_PlayCue(0x121);
	Work_SetValuesIfNonNegative(-1, -1, 0xe666);
	Engine_EventWait(30);
	Map_CopyCellsTo(103, 14, 103, 17, 4, 3);
	GameFlag_Set(0x907);
}

static void FieldScene_RunScene3a4SequenceB(void)
{
	extern u8 ArutinYama_RiseTimer[];

	if (GameFlag_IsSet(0x323) != 0)
	{
		Map_CopyCellAttributes(2, 0, 1, 1, 24, 80);
		Map_CopyCellsTo(2, 1, 24, 11, 1, 2);
		GameFlag_Clear(0x323);
	}
	else
	{
		Map_CopyCellAttributes(0, 0, 1, 1, 24, 80);
		Map_CopyCellsTo(0, 1, 24, 11, 1, 2);
		GameFlag_Set(0x323);
	}
}

static void EV83_SEARCH(void)
{
	Engine_EventBegin();
	Engine_MessageShowCentered((s32)MsgFieldFlippedSwitch, 1);
	Engine_AudioPlayCue(125);
	FieldScene_RunScene3a4SequenceB();
	Engine_TaskWait(20);
	BattleFx_PlayQueuedSound();
	Engine_EventEnd();
}

static void FieldScene_RunScene3a4SequenceA(void)
{
	extern u8 ArutinYama_RiseTimer[];

	if (GameFlag_IsSet(0x325) != 0)
	{
		Map_CopyCellAttributes(12, 72, 1, 1, 11, 73);
		Map_CopyCellsTo(48, 32, 11, 4, 1, 2);
		GameFlag_Clear(0x325);
	}
	else
	{
		Map_CopyCellAttributes(10, 72, 1, 1, 11, 73);
		Map_CopyCellsTo(49, 32, 11, 4, 1, 2);
		GameFlag_Set(0x325);
	}
}

static void EV50_SEARCH_5(void)
{
	extern u8 ArutinYama_RiseTimer[];

	Engine_EventBegin();
	Engine_MessageShowCentered((s32)MsgFieldFlippedSwitch, 1);
	Engine_AudioPlayCue(125);
	FieldScene_RunScene3a4SequenceA();
	Engine_TaskWait(20);
	BattleFx_PlayQueuedSound();
	Engine_EventEnd();
}

static void EV50_SEARCH_6(void)
{
	extern u8 ArutinYama_RiseTimer[];

	Engine_EventBegin();
	Engine_MessageShowCentered((s32)MsgFieldFlippedSwitch, 1);
	Event_PlayCue(125);
	if (GameFlag_IsSet(0x326) != 0)
	{
		Map_CopyCellAttributes(15, 93, 1, 1, 16, 92);
		Map_CopyCellsTo(47, 29, 16, 28, 1, 2);
		GameFlag_Clear(0x326);
	}
	else
	{
		Map_CopyCellAttributes(17, 93, 1, 1, 16, 92);
		Map_CopyCellsTo(46, 29, 16, 28, 1, 2);
		GameFlag_Set(0x326);
	}
	Engine_TaskWait(20);
	BattleFx_PlayQueuedSound();
	Engine_EventEnd();
}

static void EV50_SEARCH_7(void)
{
	extern u8 ArutinYama_RiseTimer[];

	Engine_EventBegin();
	Engine_MessageShowCentered((s32)MsgFieldFlippedSwitch, 1);
	Event_PlayCue(125);
	if (GameFlag_IsSet(0x327) != 0)
	{
		Map_CopyCellAttributes(28, 82, 1, 1, 29, 81);
		Map_CopyCellsTo(47, 28, 29, 17, 1, 2);
		GameFlag_Clear(0x327);
	}
	else
	{
		Map_CopyCellAttributes(30, 82, 1, 1, 29, 81);
		Map_CopyCellsTo(46, 28, 29, 17, 1, 2);
		GameFlag_Set(0x327);
	}
	Engine_TaskWait(20);
	BattleFx_PlayQueuedSound();
	Engine_EventEnd();
}

static void EV10_TOUCH_7(void)
{

	Object_GetById(10)->priority_flags = 3;
}

static void EV11_TOUCH_7(void)
{

	Object_GetById(10)->priority_flags = 1;
}

static void EV12_TOUCH(void)
{
	extern u8 ArutinYama_RiseTimer[];

	Engine_EventBegin();
	Map_CopyCellAttributes(24, 27, 2, 1, 24, 26);
	Event_PlayCue(185);
	Actor_SetSpeed(10, 0x3333, 0x1999);
	Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x3333, 0x1999);
	Object_GetById(10)->steering_flags &= ~ACTOR_FACES_MOTION;
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 8);
	Actor_SetDestination(ACTOR_PARTY_LEADER, 0x190, 0x1a8);
	Actor_SetDestination(10, 0x198, 0x1a8);
	Engine_ActorWaitForMove(10);
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 1);
	EV10_PSYNERGY();
	Engine_EventEnd();
}

static void EV11_PSYNERGY_9(void)
{
	struct FieldActor  *actor;
	struct FieldSprite *sprite;
	s32					x;
	s32					y;
	s32					c;
	s32					s;
	u16					step;

	actor = Actor_Get(10);
	sprite = actor->sprite;
	x = actor->x.fixed;
	y = actor->y.fixed;
	Event_Begin();
	Event_PlayCue(141);
	Work_SetValuesIfNonNegative(0x20000, 0x10000, 0x10000);
	Event_Wait(10);
	Event_PlayCue(0x121);
	Work_SetValuesIfNonNegative(-1, -1, 0xe666);
	Event_Wait(20);

	step = 0;
	while (1)
	{
		step += 8;
		sprite->rotation += step;
		c = Math_Cos(sprite->rotation + 0x4000);
		actor->x.fixed = (c << 4) + x;
		if (sprite->rotation > 0x8fff)
		{
			break;
		}
		Task_Wait(1);
	}
	step = 0;
	while (1)
	{
		step += 8;
		sprite->rotation -= step;
		c = Math_Cos(sprite->rotation + 0x4000);
		actor->x.fixed = (c << 4) + x;
		if (sprite->rotation <= 0x7000)
		{
			break;
		}
		Task_Wait(1);
	}
	step = 8;
	while (1)
	{
		step += step >> 3;
		sprite->rotation += step;
		c = Math_Cos(sprite->rotation + 0x4000);
		s = Math_Sin(sprite->rotation + 0x8000);
		actor->x.fixed = (c << 4) + x;
		if (sprite->rotation > 0x8000)
		{
			actor->y.fixed = y - (s << 3);
		}
		if (sprite->rotation + step > 0xbfff)
		{
			break;
		}
		Task_Wait(1);
	}
	Task_Wait(1);
	sprite->rotation = 0xc000;
	Event_PlayCue(183);
	Work_SetValuesIfNonNegative(0x30000, 0x30000, 0x10000);
	Event_Wait(20);
	Event_PlayCue(0x121);
	Work_SetValuesIfNonNegative(-1, -1, 0xe666);
	FieldScene_RunSharedSetPiece(5);
	Event_End();
}

union TimedActor
{
	struct FieldActor  actor;
	struct SceneMotion motion;
};

struct Half
{
	u16 v;
};

static void FieldScene_RunSharedSetPiece(s32 a0)
{
	struct FieldActor *actor0;
	struct FieldActor *actor8;
	struct FieldActor *actor9;
	struct FieldActor *actor10;
	struct FieldActor *actor;

	actor0 = Actor_Get(ACTOR_PARTY_LEADER);
	actor8 = Actor_Get(8);
	actor9 = Actor_Get(9);
	actor10 = Actor_Get(10);
	Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
	Engine_EventWait(40);
	Camera_SetSpeed(0x10000, 0x2000);
	Camera_MoveTo(0x3100000, -1, 0x740000, 1);
	Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x20000, 0x10000);
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 6);
	Actor_MoveToAndWait(ACTOR_PARTY_LEADER, 0x318, 140);
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 1);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 100);
	Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 60);
	Event_PlayCue(183);
	Work_SetValuesIfNonNegative(0x30000, 0x30000, 0x10000);
	Engine_EventWait(20);
	actor9->scale_x = 0x13333;
	actor9->scale_y = 0x13333;
	actor9->priority_flags |= ACTOR_PRIORITY_UNDERFOOT;
	actor9->update = (void (*)(union FieldObject *))SceneActor_CopyActor8PositionWithFixedY;
	Engine_ActorSetAnimation(8, 4);
	actor8->bounce = 0x8000;
	actor8->x.fixed = 0x3120000;
	actor8->y.fixed = 0x200000;
	actor8->z.fixed = 0x5a0000;
	actor8->scale_x = 0x20000;
	actor8->scale_y = 0x20000;
	Engine_EventWait(10);
	Event_PlayCue(183);
	Work_SetValuesIfNonNegative(0x40000, 0x20000, 0x10000);
	Engine_EventWait(20);
	actor10->x.fixed += 0xe0000;
	actor10->y.fixed += -0x80000;
	actor10->sprite->rotation = 0xc000;
	Event_PlayCue(107);
	Work_SetValuesIfNonNegative(0x10000, 0x10000, 0x10000);
	Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x102, 80);
	Event_PlayCue(55);
	Work_SetValuesIfNonNegative(0x10000, 0x30000, 0x10000);
	Engine_ActorSetSpritePriority(8, 0);
	Engine_ActorSetSpritePriority(ACTOR_PARTY_LEADER, 0);
	Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x101);
	Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x28000, 0x14000);
	actor0->timer = 0;
	Engine_ActorEnableActionCallback(ACTOR_PARTY_LEADER, (const u8 *)ArutinYama_LeaderRideScript);
	if (GameFlag_IsSet(0x205) != 0)
	{
		Actor_SetPosition(GERALD, 0x36e0000, 0x2100000);
		actor = Actor_Get(GERALD);
		actor->facing = FACING_SOUTH + FACING_STEP;
	}
	Camera_SetSpeed(0x14000, 0x2800);
	Camera_MoveTo(0x3120000, -1, 0x22c0000, 1);
	Engine_EventWait(a0);
	Engine_ActorSetSpritePriority(8, 1);
	Actor_SetSpeed(8, 0x195c2, 0xcae1);
	actor8->timer = 0;
	Engine_ActorEnableActionCallback(8, (const u8 *)ArutinYama_LogRideScript);
	do
	{
		Engine_TaskWait(1);
	} while (actor0->timer == 0);
	Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0);
	do
	{
		Engine_TaskWait(1);
	} while (actor8->timer == 0);
	Engine_ActorSetSpritePriority(ACTOR_PARTY_LEADER, 2);
	Actor_Get(ACTOR_PARTY_LEADER)->priority_flags |= ACTOR_PRIORITY_AUTOMATIC;
	Event_PlayCue(0x121);
	Work_SetValuesIfNonNegative(-1, -1, 0xe666);
	actor9->x.fixed = 0x3120000;
	actor9->update = NULL;
	actor9->z.fixed = 0x26a0000;
	actor9->y.fixed = -0x400000;
	Actor_SetSpeed(8, 0x19999, 0xcccc);
	actor8->bounce = 0x1999;
	actor8->gravity = 0x3333;
	actor8->velocity_y = 0x40000;
	Actor_MoveToAndWait(8, 0x312, 0x25c);
	Actor_SetSpeed(8, 0x33333, 0x19999);
	Actor_SetDestination(8, 0x312, 0x284);
	Engine_EventWait(15);
	Work_SetValuesIfNonNegative(0x50000, 0x70000, 0x10000);
	Map_CopyCellsTo(25, 36, 43, 36, 11, 9);
	Map_CopyCellAttributes(25, 35, 10, 5, 43, 35);
	Actor_SetPosition(8, 0, 0);
	Actor_SetPosition(9, 0, 0);
	Engine_TaskAddCallback(FieldScene_RunScene3a4SequenceG, 0xc80);
	Engine_EventWait(80);
	Engine_TaskRemoveCallback(FieldScene_RunScene3a4SequenceG);
	Engine_EventWait(60);
	Event_PlayCue(17);
	Work_SetValuesIfNonNegative(-1, -1, 0xe666);
	Engine_EventWait(120);
	if (GameFlag_IsSet(0x205) != 0)
	{
		Actor_SetSpeed(GERALD, 0x10000, 0x8000);
		Actor_WalkTo(GERALD, 0x338, 0x22e);
	}
	Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x9999, 0x4ccc);
	Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x356, 0x248);
	if (GameFlag_IsSet(0x205) != 0)
	{
		Engine_ActorSetAnimation(GERALD, 1);
		Actor_FaceDirection(GERALD, FACING_SOUTH, 0);
	}
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 40);
	Actor_ShowEmote(GERALD, 0x102, 0);
	Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x102, 60);
	Camera_MoveTo(0x3140000, -0x400000, 0x2620000, 1);
	Engine_CameraWaitForMove();
	Event_PlayCue(148);
	Engine_EventWait(240);
	if (GameFlag_IsSet(0x205) != 0)
	{
		Camera_SetSpeed(0x40000, 0x8000);
		Camera_MoveTo(0x3560000, 0, 0x2480000, 1);
		Engine_CameraWaitForMove();
		Actor_WalkToAndWait(GERALD, 0x348, 0x228);
		Actor_WalkToAndWait(GERALD, 0x356, 0x232);
		Engine_ActorSetAnimation(GERALD, 2);
		actor = Actor_Get(ACTOR_PARTY_LEADER);
		if (actor != NULL)
		{
			Actor_SetDestination(GERALD, actor->x.part.pixel, actor->z.part.pixel);
		}
		Engine_ActorWaitForMove(GERALD);
		Actor_SetPosition(GERALD, 0, 0);
	}
	Audio_PlayCueFromEventWork();
	GameFlag_Set(0x908);
}

static void EV12_SEARCH(void)
{
	struct FieldActor *record;
	s32				   msg;

	Engine_EventBegin();
	msg = (s32)MsgArutinWatchFallingRocks;
	Engine_MessageShowCentered(msg, 1);
	if (GameFlag_IsSet(0x908) == 0 && GameFlag_IsSet(0xf14) == 0)
	{
		GameFlag_Set(0x205);
		Actor_SetSpeed(ACTOR_PARTY_LEADER, 0xcccc, 0x6666);
		Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x316, 140);
		Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x30c, 140);
		Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
		record = Object_GetById(ACTOR_PARTY_LEADER);
		if (record != 0)
		{
			Actor_SetPosition(GERALD, record->x.fixed, record->z.fixed);
		}
		Actor_SetSpeed(GERALD, 0xcccc, 0x6666);
		Actor_WalkToAndWait(GERALD, 0x320, 140);
		Actor_FaceDirection(GERALD, 0xc000, 20);
		Engine_EventSetMessage(msg + 1);
		Engine_ActorSetAnimation(GERALD, 4);
		Engine_EventWait(20);
		Event_ShowMessageAndWait(GERALD, 0, 10);
		Engine_ActorJump(GERALD, 6, 0);
		Actor_SetSpeed(GERALD, 0x19999, 0xcccc);
		Object_GetById(GERALD)->steering_flags &= ~ACTOR_FACES_MOTION;
		Actor_WalkToAndWait(GERALD, 0x318, 110);
		Engine_EventWait(1);
		Object_GetById(GERALD)->steering_flags |= ACTOR_FACES_MOTION;
		Event_PlayCue(161);
		Work_SetValuesIfNonNegative(0x20000, 0x10000, 0x10000);
		Object_GetById(GERALD)->steering_flags &= ~ACTOR_FACES_MOTION;
		Actor_WalkToAndWait(GERALD, 0x318, 120);
		Engine_EventWait(1);
		{
			struct FieldActor *record = Actor_Get(GERALD);

			record->steering_flags = ACTOR_FACES_MOTION | record->steering_flags;
		}
		Work_SetValuesIfNonNegative(-1, -1, 0xe666);
		Engine_EventWait(80);
		Event_PlayCue(141);
		Work_SetValuesIfNonNegative(0x10000, 0x10000, 0x10000);
		Engine_EventWait(40);
		Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 0);
		Actor_ShowEmote(GERALD, 0x101, 60);
		Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x8000, 0);
		Actor_FaceDirection(GERALD, 0, 20);
		Actor_FaceDirection(ACTOR_PARTY_LEADER, 0, 0);
		Actor_FaceDirection(GERALD, 0x8000, 40);
		Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x8000, 0);
		Actor_FaceDirection(GERALD, 0, 40);
		Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
		Actor_FaceDirection(GERALD, 0xc000, 40);
		Actor_ShowEmote(GERALD, 0x102, 60);
		Actor_FaceDirection(GERALD, 0x4000, 20);
		Engine_ActorStartRepeatedMotion(GERALD, 2);
		Event_ShowMessageAndWait(GERALD, 0, 10);
		Actor_SetSpeed(GERALD, 0x28000, 0x14000);
		Engine_ActorSetAnimation(GERALD, 5);
		Actor_MoveToAndWait(GERALD, 0x31c, 138);
		Actor_FaceDirection(ACTOR_PARTY_LEADER, 0, 0);
		Actor_MoveToAndWait(GERALD, 0x324, 140);
		Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 0);
		Actor_MoveToAndWait(GERALD, 0x324, 166);
		Actor_MoveToAndWait(GERALD, 0x2fc, 166);
		Actor_MoveToAndWait(GERALD, 0x2fc, 198);
		Actor_MoveToAndWait(GERALD, 0x312, 198);
		Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x102, 0);
		Actor_MoveToAndWait(GERALD, 0x312, 246);
		Engine_ActorSetAnimation(GERALD, 1);
		Actor_SetPosition(GERALD, 0, 0);
		Engine_EventWait(40);
		FieldScene_RunSharedSetPiece(10);
	}
	Engine_EventEnd();
}

const struct SceneEvent *Scene_GetEvents(void)
{
	const struct SceneEvent *table;

	if (gGameState.scene == (s32)&SceneId_ArutinYama1)
	{
		if (Engine_GameFlagIsSet(0x8fd) != 0)
		{
			table = ArutinYama_OpenedAreaScript;
		}
		else
		{
			table = gArutinYamaEvents1;
		}
	}
	else if (gGameState.scene == (s32)&SceneId_ArutinYama2)
	{
		table = gArutinYamaEvents2;
	}
	else if (gGameState.scene == (s32)&SceneId_ArutinYama3)
	{
		table = gArutinYamaEvents3;
	}
	else if (gGameState.scene == (s32)&SceneId_ArutinYama4)
	{
		table = gArutinYamaEvents4;
	}
	else if (gGameState.scene == (s32)&SceneId_ArutinYama5)
	{
		table = gArutinYamaEvents5;
	}
	else if (gGameState.scene == (s32)&SceneId_ArutinYama6)
	{
		table = gArutinYamaEvents6;
	}
	else if (gGameState.scene == (s32)&SceneId_ArutinYama7)
	{
		table = gArutinYamaEvents7;
	}
	else if (gGameState.scene == (s32)&SceneId_ArutinYama8)
	{
		table = gArutinYamaEvents8;
	}
	else if (gGameState.scene == (s32)&SceneId_ArutinYama9)
	{
		table = gArutinYamaEvents9;
	}
	else if (gGameState.scene == (s32)&SceneId_ArutinYama10)
	{
		table = gArutinYamaEvents10;
	}
	else if (gGameState.scene == (s32)&SceneId_ArutinYama11)
	{
		table = gArutinYamaEvents11;
	}
	else
	{
		table = gArutinYamaEventsOther;
	}
	return table;
}

static void SceneMotion_UpdateTimedActor(struct SceneMotion *work)
{
	if (work->delay != 0)
	{
		if (--work->delay == 1)
			Work_SetValuesIfNonNegative(-1, -1, 0xe666);
	}
	if (work->velocity == 0)
	{
		Object_SetMode(work, 1);
		work->y += -0x18000;
		if (work->y < work->ground)
		{
			if (work->active != 0)
			{
				Engine_AudioPlayCue(229);
				work->active = 0;
				work->delay = 4;
				Work_SetValuesIfNonNegative(0x10000, 0, 0x10000);
			}
			work->y = work->ground;
		}
		work->state = 1;
	}
	else
	{
		work->state = 0;
	}
	if (work->timer == 0)
	{
		Engine_AudioPlayCue(152);
		work->active = 1;
		Object_SetMode(work, 2);
		work->velocity = 0x30000;
	}
	if (++work->timer == 60)
		work->timer = 0;
}

static void EV10_TOUCH_1(void)
{
	struct Half		   p10;
	s32				   p8;
	union TimedActor  *rec3;
	struct FieldActor *rec7;
	struct FieldActor *record;
	s32				   v6;

	rec3 = (union TimedActor *)Object_GetById(10);
	Engine_EventBegin();
	Engine_CameraSetSpeed(0x26666, 0x4ccc);
	Camera_MoveTo(0x12a0000, -1, 0x1510000, 1);
	Engine_CameraWaitForMove();
	Engine_AudioPlayCue(147);
	Engine_ActorRunRepeatedMotion(10, 2);
	Engine_EventWait(40);
	Engine_CameraSetSpeed(0xcccc, 0x1999);
	Engine_CameraMoveTo(0x1270000, 0x200000, 0xd40000, 1);
	rec3->motion.active = 0;
	rec3->motion.timer = 0;
	p8 = (s32)rec3 + 100;
	p10.v = 0;
	rec3->motion.delay = 0;
	rec3->actor.gravity = 0x6666;
	rec3->actor.update = (void (*)(union FieldObject *))SceneMotion_UpdateTimedActor;
	Actor_SetSpeed(10, 0x13333, 0x9999);
	Actor_MoveToAndWait(10, 0x134, 0x123);
	Engine_ActorMoveToAndWait(10, 0x137, 215);
	rec3->actor.update = 0;
	rec3->motion.state = p10.v;
	Engine_EventWait(16);
	Engine_ActorSetAnimation(10, 1);
	Engine_AudioPlayCue(229);
	Work_SetValuesIfNonNegative(0x10000, 0, 0x10000);
	Engine_EventWait(4);
	Work_SetValuesIfNonNegative(-1, -1, 0xe666);
	Engine_EventWait(40);
	Actor_FaceDirection(10, 0xb000, 20);
	Actor_FaceDirection(10, 0x8000, 40);
	FieldScene_RunActorTenFourStepSequence();
	Engine_EventWait(40);
	Actor_FaceDirection(10, 0x5000, 20);
	Actor_FaceDirection(10, 0x3000, 40);
	Camera_MoveTo(0x14e0000, -1, 0xf40000, 1);
	rec3->motion.active = 0;
	rec3->motion.timer = 0;
	rec3->motion.delay = 0;
	rec3->actor.update = (void (*)(union FieldObject *))SceneMotion_UpdateTimedActor;
	Actor_MoveToAndWait(10, 0x140, 232);
	Actor_MoveToAndWait(10, 0x154, 0x106);
	Engine_ActorMoveToAndWait(10, 0x176, 0x106);
	rec3->actor.update = 0;
	Engine_EventWait(16);
	Engine_ActorSetAnimation(10, 1);
	Engine_AudioPlayCue(229);
	Work_SetValuesIfNonNegative(0x10000, 0, 0x10000);
	Engine_EventWait(4);
	Work_SetValuesIfNonNegative(-1, -1, 0xe666);
	Engine_EventWait(20);
	Actor_FaceDirection(10, 0xf000, 20);
	Engine_ActorFaceDirection(10, 0xd000, 40);
	Engine_AudioPlayCue(153);
	record = Object_GetById(10);
	record->velocity_y = 0x40000;
	Engine_ActorSetAnimation(10, 2);
	Engine_ActorMoveToAndWait(10, 0x17c, 248);
	Engine_EventWait(10);
	Engine_AudioPlayCue(229);
	Work_SetValuesIfNonNegative(0x10000, 0, 0x10000);
	Engine_EventWait(4);
	Work_SetValuesIfNonNegative(-1, -1, 0xe666);
	Engine_EventWait(6);
	Engine_ActorSetAnimation(10, 1);
	Engine_EventWait(40);
	Actor_FaceDirection(10, 0xb000, 20);
	Actor_FaceDirection(10, 0x8000, 40);
	Camera_MoveTo(0x1300000, -1, 0xd70000, 1);
	rec3->motion.active = 0;
	rec3->motion.timer = 0;
	rec3->motion.delay = 0;
	rec3->actor.update = (void (*)(union FieldObject *))SceneMotion_UpdateTimedActor;
	Actor_MoveToAndWait(10, 0x149, 219);
	rec3->actor.update = 0;
	Engine_ActorSetAnimation(10, 1);
	Engine_EventWait(16);
	Engine_AudioPlayCue(229);
	Work_SetValuesIfNonNegative(0x10000, 0, 0x10000);
	Engine_EventWait(4);
	Work_SetValuesIfNonNegative(-1, -1, 0xe666);
	Engine_EventWait(40);
	Engine_ActorFaceDirection(10, 0x8000, 40);
	Object_GetById(9)->motion_flags = p10.v;
	Engine_MapCopyCellAttributes(3, 0, 1, 1, 17, 13);
	Engine_MapCopyCellAttributes(3, 0, 1, 1, 18, 13);
	Engine_MapCopyCellAttributes(3, 0, 1, 1, 19, 13);
	Engine_ActorSetSpeed(10, 0x16666, 0xb333);
	record = Object_GetById(10);
	Engine_ActorSetSpriteFlags(record, 0);
	Engine_AudioPlayCue(153);
	v6 = 160;
	record = Object_GetById(10);
	record->velocity_y = (v6 << 11);
	Engine_ActorSetAnimation(10, 3);
	Actor_MoveToAndWait(10, 0x127, 215);
	Engine_ActorSetAnimation(10, 1);
	record = Object_GetById(10);
	Engine_ActorSetSpriteFlags(record, 1);
	Engine_AudioPlayCue(229);
	Work_SetValuesIfNonNegative(0x10000, 0, 0x10000);
	Engine_EventWait(4);
	Work_SetValuesIfNonNegative(-1, -1, 0xe666);
	Engine_EventWait(40);
	Engine_AudioPlayCue(153);
	record = Object_GetById(10);
	record->velocity_y = (v6 << 11);
	Engine_ActorSetAnimation(10, 3);
	Actor_MoveToAndWait(10, 0x104, 215);
	Engine_ActorSetAnimation(10, 1);
	Engine_AudioPlayCue(229);
	Work_SetValuesIfNonNegative(0x10000, 0, 0x10000);
	Engine_EventWait(4);
	Work_SetValuesIfNonNegative(-1, -1, 0xe666);
	Engine_EventWait(40);
	Actor_FaceDirection(10, 0x5000, 20);
	Engine_ActorFaceDirection(10, 0x3000, 20);
	Engine_AudioPlayCue(147);
	Engine_ActorRunRepeatedMotion(10, 2);
	Engine_EventWait(40);
	Engine_MapCopyCellAttributes(4, 0, 1, 1, 17, 13);
	Engine_MapCopyCellAttributes(2, 0, 1, 1, 18, 13);
	Engine_MapCopyCellAttributes(4, 0, 1, 1, 19, 13);
	rec7 = Object_GetById(0);
	Engine_CameraSetSpeed(0x4cccc, 0x9999);
	Engine_CameraMoveTo(rec7->x.fixed, rec7->y.fixed, rec7->z.fixed, 1);
	Engine_CameraWaitForMove();
	FieldObject_SetTargetAndCallback(10, 0x10000, (const u8 *)ArutinYama_ActorScript);
	Engine_GameFlagSet(0x904);
	Engine_EventEnd();
}

static void EV11_TOUCH_1(void)
{
	union TimedActor  *actor;
	struct FieldActor *record;
	u32				   n;

	actor = (union TimedActor *)Object_GetById(10);
	Engine_EventBegin();
	ObjectMotion_EnableActionAndResetMotion(10);
	Camera_SetSpeed(0x26666, 0x4ccc);
	Camera_MoveTo(0x1170000, 0x400000, 0xd80000, 1);
	Engine_CameraWaitForMove();
	Engine_AudioPlayCue(147);
	Engine_ActorRunRepeatedMotion(10, 2);
	Engine_EventWait(40);
	Actor_FaceDirection(10, 0x3000, 20);
	Actor_FaceDirection(10, 0x5000, 20);
	Actor_FaceDirection(10, 0x8000, 40);
	Camera_SetSpeed(0xcccc, 0x1999);
	Camera_MoveTo(0x800000, 0x400000, 0xca0000, 1);
	actor->motion.active = 0;
	actor->motion.timer = 0;
	actor->motion.delay = 0;
	actor->actor.gravity = 0x6666;
	actor->actor.update = (void (*)(union FieldObject *))SceneMotion_UpdateTimedActor;
	Actor_SetSpeed(10, 0x13333, 0x9999);
	Actor_MoveToAndWait(10, 212, 200);
	Actor_MoveToAndWait(10, 103, 200);
	actor->actor.update = NULL;
	actor->motion.state = 0;
	Engine_EventWait(10);
	Engine_ActorSetAnimation(10, 1);
	Engine_AudioPlayCue(229);
	Work_SetValuesIfNonNegative(0x10000, 0, 0x10000);
	Engine_EventWait(4);
	Work_SetValuesIfNonNegative(-1, -1, 0xe666);
	Engine_EventWait(20);
	Actor_FaceDirection(10, 0x5000, 40);
	Object_GetById(10)->steering_flags &= ~ACTOR_FACES_MOTION;
	Actor_SetSpeed(10, 0x13333, 0x9999);
	record = Object_GetById(10);
	Engine_ActorSetSpriteFlags(record, 0);
	Engine_AudioPlayCue(153);
	record = Object_GetById(10);
	record->velocity_y = 0x40000;
	Actor_SetAnimation(10, 3);
	Engine_ActorMoveToAndWait(10, 86, 214);
	Engine_ActorSetAnimation(10, 1);
	record = Object_GetById(10);
	Engine_ActorSetSpriteFlags(record, 1);
	Engine_EventWait(10);
	Engine_AudioPlayCue(229);
	Work_SetValuesIfNonNegative(0x20000, 0, 0x10000);
	Engine_EventWait(8);
	Work_SetValuesIfNonNegative(-1, -1, 0xe666);
	Engine_EventWait(40);
	Object_GetById(10)->steering_flags |= ACTOR_FACES_MOTION;
	Actor_FaceDirection(10, 0x3000, 20);
	Engine_ActorFaceDirection(10, 0, 40);
	actor->motion.active = 0;
	actor->motion.timer = 0;
	actor->motion.delay = 0;
	actor->actor.update = (void (*)(union FieldObject *))SceneMotion_UpdateTimedActor;
	Actor_SetSpeed(10, 0x13333, 0x9999);
	Actor_MoveToAndWait(10, 120, 215);
	actor->actor.update = NULL;
	actor->motion.state = 0;
	Engine_ActorSetAnimation(10, 1);
	Engine_EventWait(16);
	Engine_AudioPlayCue(229);
	Work_SetValuesIfNonNegative(0x10000, 0, 0x10000);
	Engine_EventWait(4);
	Work_SetValuesIfNonNegative(-1, -1, 0xe666);
	Engine_EventWait(40);
	Engine_ActorFaceDirection(10, 0, 10);
	Engine_AudioPlayCue(147);
	Engine_ActorRunRepeatedMotion(10, 2);
	Engine_EventWait(80);
	Engine_ActorSetAnimation(10, 3);
	ArutinYama_StartLeafGust(0x820000, 0, 0xa80000, 0);
	Engine_EventWait(60);
	for (n = 0; n < 60 && gKeyState == 0; n++)
	{
		Engine_EventWait(1);
	}
	actor = (union TimedActor *)Object_GetById(0);
	Camera_SetSpeed(0x4cccc, 0x9999);
	Engine_CameraMoveTo(actor->actor.x.fixed, actor->actor.y.fixed, actor->actor.z.fixed, 1);
	Engine_CameraWaitForMove();
	GameFlag_Set(0x905);
	Engine_EventEnd();
}

struct Byte
{
	u8 v;
};

s32 ArutinYama_RunAreaScene(void)
{
	gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
	if (gGameState.scene == (s32)&SceneId_ArutinYama1)
	{
		ArutinYama_ApplyEntryState();
	}
	else if (gGameState.scene == (s32)&SceneId_ArutinYama3)
	{
		ArutinYama_RestoreAreaThree();
	}
	else if (gGameState.scene == (s32)&SceneId_ArutinYama4)
	{
		ArutinYama_RestoreAreaFour();
	}
	else if (gGameState.scene == (s32)&SceneId_ArutinYama5)
	{
		ArutinYama_RestoreAreaFive();
	}
	else if (gGameState.scene == (s32)&SceneId_ArutinYama6)
	{
		ArutinYama_RestoreAreaSix();
	}
	else if (gGameState.scene == (s32)&SceneId_ArutinYama7)
	{
		ArutinYama_RestoreAreaSeven();
	}
	else if (gGameState.scene == (s32)&SceneId_ArutinYama9)
	{
		ArutinYama_PlaceFlaggedActors();
	}
	else if (gGameState.scene == (s32)&SceneId_ArutinYama10)
	{
		ArutinYama_RestoreAreaTen();
	}
	else if (gGameState.scene == (s32)&SceneId_ArutinYama11)
	{
		ArutinYama_RestoreAreaEleven();
	}
	return 0;
}

static void ArutinYama_ApplyEntryState(void)
{
	s32				   opened;
	s32				   flag;
	struct FieldActor *actor;
	u8				  *map;

	if (Engine_GameFlagIsSet(0x109) == 0)
	{
		if (gGameState.entrance == 99)
		{
			FieldScene_RunOpeningAuxiliarySequence();
		}
	}
	ArutinYama_StartPaletteAnim();
	if (Engine_GameFlagIsSet(0x8fd) == 0)
	{
		Engine_ActorSetPosition(8, 0, 0);
	}
	else
	{
		actor = Object_GetById(8);
		if (actor != 0)
		{
			actor->priority_flags = 2;
			Engine_ActorSetSpriteFlags(actor, 0);
		}
	}
	actor = Object_GetById(9);
	if (actor != 0)
	{
		actor->priority_flags = 2;
		Engine_ActorSetSpriteFlags(actor, 0);
	}
	opened = Engine_GameFlagIsSet(0x8fd);
	if (opened == 0)
	{
		Engine_ActorSetChildValue(10, 2);
		flag = Engine_GameFlagIsSet(0x905);
		if (flag != 0)
		{
			Engine_ActorSetAnimation(9, 0);
			actor = Object_GetById(9);
			actor->update = SceneState_ForwardByRuntimeSelector;
			Object_GetById(9)->motion_flags = opened;
			actor = Object_GetById(9);
			actor->y.fixed = 0x200000;
			Map_CopyCellAttributes(2, 0, 1, 1, 18, 13);
			Engine_ActorSetPosition(10, 0x780000, 0xd70000);
			actor = Object_GetById(10);
			actor->facing = opened;
			Engine_ActorSetAnimation(10, 3);
			ArutinYama_StartLeafGust(0x820000, 0, 0xa80000, 0);
			return;
		}
		if (Engine_GameFlagIsSet(0x904) == 0)
		{
			return;
		}
		Engine_ActorSetAnimation(9, 0);
		actor = Object_GetById(9);
		actor->update = SceneState_ForwardByRuntimeSelector;
		Object_GetById(9)->motion_flags = flag;
		actor = Object_GetById(9);
		actor->y.fixed = 0x200000;
		Map_CopyCellAttributes(2, 0, 1, 1, 18, 13);
		Actor_SetPosition(10, 0x1040000, 0xd70000);
		FieldObject_SetTargetAndCallback(10, 0x10000, (const u8 *)ArutinYama_ActorScript);
	}
	else
	{
		map = gMapWork[0];
		Engine_ActorSetPosition(10, 0, 0);
		Map_CopyCellAttributes(0, 0, 1, 2, 3, 14);
		*(u16 *)(map + 20) &= 0xfdff;
		SceneActor_ClearCollisionFlagAndPlaceMarker(8);
		if (Engine_GameFlagIsSet(0x200) != 0)
		{
			Engine_ActorSetAnimation(8, 5);
			Map_CopyCellAttributes(7, 13, 1, 1, 9, 13);
			actor = Object_GetById(8);
			actor->y.fixed = 0;
			actor->priority_flags |= 2;
		}
		SceneActor_ClearCollisionFlagAndPlaceMarker(9);
		if (Engine_GameFlagIsSet(0x201) != 0)
		{
			Engine_ActorSetAnimation(9, 5);
			Map_CopyCellAttributes(29, 1, 3, 1, 17, 13);
			actor = Object_GetById(9);
			actor->y.fixed = 0x200000;
			actor->priority_flags |= 2;
		}
	}
}

static void ArutinYama_RestoreAreaThree(void)
{

	if (GameFlag_IsSet(0x8fe) != 0)
	{
		*(u16 *)(((u8 *)gMapWork[0]) + 20) &= ~0x200;
		Actor_SetPosition(9, 0, 0);
	}
	else
	{
		ArutinYama_StartPaletteAnim();
		if (GameFlag_IsSet(0x109) == 0 && gGameState.entrance == 99)
		{
			FieldScene_RunScene3a4SequenceF();
		}
		else
		{
			Map_CopyCellAttributes(38, 24, 1, 2, 37, 24);
			Map_CopyCellAttributes(44, 23, 1, 2, 45, 23);
			if (GameFlag_IsSet(0x8fe) == 0)
			{
				Actor_SetChildValue(9, 2);
				Engine_ActorSetAnimation(9, 3);
				ArutinYama_StartLeafGust(0xee0000, 0, 0x1a20000, 0x8000);
			}
		}
	}
	if (GameFlag_IsSet(0x323) != 0)
	{
		Map_CopyCellAttributes(0, 0, 1, 1, 24, 80);
		Map_CopyCellsTo(0, 1, 24, 11, 1, 2);
	}
	else
	{
		Map_CopyCellAttributes(2, 0, 1, 1, 24, 80);
		Map_CopyCellsTo(2, 1, 24, 11, 1, 2);
	}
}

static void ArutinYama_RestoreAreaFour(void)
{
	extern u8 ArutinYama_RiseTimer[];

	if (GameFlag_IsSet(0x8fe) != 0)
	{
		*(u16 *)(((u8 *)gMapWork[0]) + 20) &= ~0x200;
	}
	else
	{
		Map_CopyCellAttributes(52, 42, 1, 1, 53, 42);
	}
	{
		s32 index = 225;
		if ((u32)((((u16 *)&gGameState)[index] - 6) << 16) <= 0x10000)
		{
			GameFlag_Clear(FLAG_ARRIVAL_EVENT_PENDING);
		}
	}
}

static void ArutinYama_RestoreAreaFive(void)
{
	extern u8 ArutinYama_RiseTimer[];

	struct FieldActor *record;

	if (GameFlag_IsSet(0x907) != 0)
	{
		*(u16 *)(((u8 *)gMapWork[0]) + 20) &= ~0x200;
		Actor_SetPosition(10, 0, 0);
	}
	else
	{
		if (GameFlag_IsSet(0x109) == 0)
		{
			if (gGameState.entrance == 99)
			{
				FieldScene_RunScene3a4SequenceE();
			}
		}
		ArutinYama_StartPaletteAnim();
		if (GameFlag_IsSet(0x907) == 0)
		{
			Actor_SetChildValue(10, 2);
			Engine_ActorSetAnimation(10, 3);
			ArutinYama_StartLeafGust(0x2ec0000, 0x80000, 0x1180000, 0x8000);
		}
	}
	SceneActor_ClearCollisionFlagAndPlaceMarker(9);
	if (GameFlag_IsSet(0x200) != 0)
	{
		Engine_ActorSetAnimation(9, 5);
		Map_CopyCellAttributes(23, 13, 1, 1, 25, 13);
		{
			struct FieldActor *record = Actor_Get(9);
			record->priority_flags = ACTOR_PRIORITY_UNDERFOOT | record->priority_flags;
		}
	}
	if (GameFlag_IsSet(0x325) != 0)
	{
		Map_CopyCellAttributes(10, 72, 1, 1, 11, 73);
		Map_CopyCellsTo(49, 32, 11, 4, 1, 2);
	}
	else
	{
		Map_CopyCellAttributes(12, 72, 1, 1, 11, 73);
		Map_CopyCellsTo(48, 32, 11, 4, 1, 2);
	}
}

static void ArutinYama_RestoreAreaSix(void)
{
	extern u8 ArutinYama_RiseTimer[];

	struct FieldActor *rec7;

	if (gGameState.entrance == 2)
	{
		if (GameFlag_IsSet(0x109) == 0)
		{
			Actor_SetPosition(8, 0x1660000, 0x680000);
		}
	}
	SceneActor_ClearCollisionFlagAndPlaceMarker(9);
	if (GameFlag_IsSet(0x200) != 0)
	{
		rec7 = Actor_Get(9);
		Engine_ActorSetAnimation(9, 5);
		Map_CopyCellAttributes(45, 41, 1, 1, 43, 41);
		{
			rec7->priority_flags = ACTOR_PRIORITY_UNDERFOOT | rec7->priority_flags;
		}
	}
	if (GameFlag_IsSet(0x907) != 0)
	{
		*(u16 *)(((u8 *)gMapWork[0]) + 20) &= ~0x200;
	}
	if (GameFlag_IsSet(0x326) != 0)
	{
		Map_CopyCellAttributes(17, 93, 1, 1, 16, 92);
		Map_CopyCellsTo(46, 29, 16, 28, 1, 2);
	}
	else
	{
		Map_CopyCellAttributes(15, 93, 1, 1, 16, 92);
		Map_CopyCellsTo(47, 29, 16, 28, 1, 2);
	}
}

static void ArutinYama_RestoreAreaSeven(void)
{
	extern u8 ArutinYama_RiseTimer[];

	struct FieldActor *record;

	record = Actor_Get(9);
	Engine_ActorSetSpriteFlags((s32)record, 0);
	EV10_PSYNERGY();
	SceneActor_ClearCollisionFlagAndPlaceMarker(9);
	if (GameFlag_IsSet(0x200) != 0)
	{
		Engine_ActorSetAnimation(9, 5);
		Map_CopyCellAttributes(0, 0, 1, 1, 26, 26);
		{
			struct FieldActor *record = Actor_Get(9);
			record->priority_flags = ACTOR_PRIORITY_UNDERFOOT | record->priority_flags;
		}
	}
	SceneActor_ClearCollisionFlagAndPlaceMarker(11);
	if (GameFlag_IsSet(0x201) != 0)
	{
		Engine_ActorSetAnimation(11, 5);
		Map_CopyCellAttributes(1, 0, 1, 1, 17, 10);
		{
			struct FieldActor *record = Actor_Get(11);
			record->priority_flags = ACTOR_PRIORITY_UNDERFOOT | record->priority_flags;
		}
	}
	SceneActor_ClearCollisionFlagAndPlaceMarker(12);
	if (GameFlag_IsSet(0x204) != 0)
	{
		Engine_ActorSetAnimation(12, 5);
		Map_CopyCellAttributes(1, 0, 1, 1, 26, 15);
		{
			struct FieldActor *record = Actor_Get(12);
			record->priority_flags = ACTOR_PRIORITY_UNDERFOOT | record->priority_flags;
		}
	}
	Engine_TaskAddCallback(SceneActor_SetActor12ModeByActorZeroHeight, 0xc80);
	if (GameFlag_IsSet(0x327) != 0)
	{
		Map_CopyCellAttributes(30, 82, 1, 1, 29, 81);
		Map_CopyCellsTo(46, 28, 29, 17, 1, 2);
	}
	else
	{
		Map_CopyCellAttributes(28, 82, 1, 1, 29, 81);
		Map_CopyCellsTo(47, 28, 29, 17, 1, 2);
	}
}

static void ArutinYama_PlaceFlaggedActors(void)
{
	struct FieldActor *actor;

	actor = Object_GetById(10);
	Engine_ActorSetPosition(8, 0, 0);
	Engine_ActorSetPosition(9, 0, 0);
	Engine_ActorSetSpriteFlags(Object_GetById(9), 0);
	actor->motion_flags = 0;
	actor->scale_x = 0xe666;
	actor->scale_y = 0x9999;
	actor->sprite->rotation = 0x8000;
	Object_GetById(12)->motion_flags = 0;
	Object_GetById(12)->y.fixed = -0x1c0000;
	if (GameFlag_IsSet(0x908))
	{
		actor->x.fixed += 0xe0000;
		actor->y.fixed += -0x80000;
		actor->sprite->rotation = 0xc000;
	}
	if (Engine_GameFlagIsSet(0x908))
	{
		Map_CopyCellsTo(25, 36, 43, 36, 11, 9);
		Map_CopyCellAttributes(25, 35, 10, 5, 43, 35);
		Engine_MapRedraw();
		Engine_TaskWait(1);
	}
	if (gGameState.entrance == 6 && !Engine_GameFlagIsSet(0x109))
	{
		Engine_EventBegin();
		Object_GetById(0)->y.fixed = -0x580000;
		Engine_CameraMoveTo(0x3180000, -0x580000, 0x2410000, 0);
		Engine_MapRedraw();
		Engine_TaskWait(1);
		Engine_EventEnd();
	}
}

static void ArutinYama_RestoreAreaTen(void)
{
	extern u8 ArutinYama_RiseTimer[];

	s32				   rec7;
	struct FieldActor *record;
	s16				   flag;

	rec7 = Engine_GameFlagIsSet(0x909);
	if (rec7 != 0)
	{
		Engine_ActorSetPosition(8, 0, 0);
		Engine_ActorSetPosition(9, 0, 0);
	}
	else
	{
		record = Object_GetById(8);
		Engine_ActorSetSpriteFlags(record, 0);
		Engine_ActorSetSpritePriority(9, 3);
		record = Object_GetById(9);
		Engine_ActorSetSpriteFlags(record, 0);
		Object_GetById(9)->collision_flags = rec7;
	}
	flag = gGameState.entrance;
	if (flag == 1 || flag == 98)
	{
		if (Engine_GameFlagIsSet(0x109) == 0)
		{
			rec7 = Object_GetById(ACTOR_PARTY_LEADER);
			Engine_EventBegin();
			((struct FieldActor *)rec7)->y.fixed = 0x100000;
			Engine_EventEnd();
		}
	}
	else if (flag == 99)
	{
		if (Engine_GameFlagIsSet(0x109) == 0)
		{
			RunEventScript01();
		}
	}
}

static void ArutinYama_RestoreAreaEleven(void)
{
	extern u8 ArutinYama_RiseTimer[];

	struct FieldActor *record;

	record = Actor_Get(9);
	Engine_ActorSetSpriteFlags(record, 0);
	if (gGameState.entrance == 2)
	{
		Actor_SetPosition(9, 0xb80000, 0x1480000);
	}
}

static void ArutinYama_InitLeaf(struct FieldActor *leaf)
{
	leaf->motion_flags = 0;
	leaf->sprite->priority = 1;
	ObjectGroup_SetChildValue(leaf, 3);
	Engine_ActorSetSpriteFlags(leaf, 0);
	leaf->scale_x = 0x4ccc;
	leaf->scale_y = 0x4ccc;
}

static void ArutinYama_UpdateDriftingLeaf(union FieldObject *object)
{
	struct FieldActor *leaf = &object->actor;

	switch (ArutinYama_LeafMode)
	{
	case 0:
		leaf->x.fixed +=
			(leaf->timer << 12) + ((s32)((((u32)(Engine_RandomNext() << 1) >> 16) - 1) << 16) >> 1);
		break;
	case 0x8000:
		leaf->x.fixed -=
			(leaf->timer << 12) + ((s32)((((u32)(Engine_RandomNext() << 1) >> 16) - 1) << 16) >> 1);
		break;
	}
	if (leaf->timer <= 3)
	{
		switch (ArutinYama_LeafMode)
		{
		case 0:
			leaf->x.fixed += 0x8000;
			break;
		case 0x8000:
			leaf->x.fixed -= 0x8000;
			break;
		}
		leaf->scale_x += 0x1999;
		leaf->scale_y -= 0xccc;
	}
	else
	{
		leaf->z.fixed += 0x13333;
		leaf->scale_x += 0x7ae;
		leaf->scale_y += 0x7ae;
	}
	if ((u32)(leaf->timer * Engine_RandomNext()) >> 16 == 0)
		ObjectGroup_SetChildValue(leaf, 7);
	if (leaf->timer != 0)
		leaf->timer -= 2;
	else
		leaf->timer = ((u32)(Engine_RandomNext() * 5) >> 16) * 2 + 2;
	if (--object->effect.countdown == 0)
		Engine_ObjectDispatchRelease(leaf);
}

static void ArutinYama_SpawnLeaf(void)
{
	union FieldObject *leaf;

	if ((gFrameCount & 3) != 0)
		return;
	leaf = (union FieldObject *)Engine_ObjectCreate(
		222, ArutinYama_LeafOrigin[0], ArutinYama_LeafOrigin[1], ArutinYama_LeafOrigin[2]);
	if (leaf == NULL)
		return;
	leaf->actor.timer = 30;
	leaf->actor.work = 1;
	leaf->effect.countdown = 20;
	ArutinYama_InitLeaf(&leaf->actor);
	leaf->effect.update = ArutinYama_UpdateDriftingLeaf;
	Object_SetMode(&leaf->actor, 1);
}

static void ArutinYama_StartLeafGust(s32 x, s32 y, s32 z, s32 facing)
{
	ArutinYama_LeafOrigin[0] = x;
	ArutinYama_LeafOrigin[1] = y;
	ArutinYama_LeafOrigin[2] = z;
	ArutinYama_LeafMode = facing;

	BattleFx_SetQueuedSoundAndPlay(170);
	Engine_TaskAddCallback(ArutinYama_SpawnLeaf, 0xc80);
}

static void FieldScene_RunScene3a4SequenceG(void)
{
	struct FieldActor *spark;
	s32				   scale;

	if (gFrameCount % 3 == 0)
	{
		spark = Object_Create(
			200, (((u32)(Engine_RandomNext() * 48) >> 16) << 16) + 0x2fd0000, -0x400000, 0x2600000);
		if (spark != NULL)
		{
			if (gFrameCount % 9 == 0)
			{
				if ((u32)(Engine_RandomNext() << 1) >> 16 != 0)
					Engine_AudioPlayCue(145);
				else
					Engine_AudioPlayCue(144);
			}
			spark->motion_flags = 0;
			scale = ((u32)(Engine_RandomNext() << 15) >> 16) + 0x4ccc;
			spark->gravity = 0x6666;
			spark->scale_y = scale;
			spark->scale_x = scale;
			spark->unknown_5d[4] = 1;
			Engine_ActorSetSpriteFlags(spark, 0);
			spark->priority_flags &= ~ACTOR_PRIORITY_AUTOMATIC;
			spark->sprite->priority = 1;
			Object_SetMode(spark, 1);
			Engine_ObjectSetScript(spark, ArutinYama_SparkScript);
			spark->velocity_x = (((u32)(Engine_RandomNext() * 6) >> 16) - 3) << 16;
			spark->velocity_y = 0x80000;
			spark->velocity_z = ((u32)(Engine_RandomNext() * 0x600) >> 16) - 0x300;
		}
	}
}

static void SceneState_ForwardByRuntimeSelector(s32 arg)
{

	s32 sel = gFrameCount & 7;

	if (sel == 0)
	{
		ObjectGroup_SetChildValue(arg, 2);
	}
	else if (sel == 2)
	{
		ObjectGroup_SetChildValue(arg, 0);
	}
}

static void FieldScene_RunActorTenFourStepSequence(void)
{
	Engine_PsynergyBegin(24, 1);
	Engine_PsynergySetTarget(10, 9);
	Engine_PsynergyRaiseHands();
	Engine_ActorSetChildValue(10, 2);
	Engine_PsynergyPlayEffect(1);
	Engine_ActorSetChildValue(10, 2);
	Engine_PsynergyLowerHands();
	Engine_ActorSetChildValue(10, 2);
	BattleEffect_CleanupSceneObjects();
	Engine_AudioPlayCue(288);
	Engine_ActorSetChildValue(10, 2);
}

static void FieldScene_RunOpeningAuxiliarySequence(void)
{
	extern u8 ArutinYama_RiseTimer[];

	struct FieldActor *rec7;

	rec7 = Object_GetById(ACTOR_PARTY_LEADER);
	Engine_EventBegin();
	Actor_SetPosition(10, 0, 0);
	Actor_SetPosition(8, 0, 0);
	gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 1);
	Engine_EventOpenScreen();
	Engine_EventWaitForScreen();
	Engine_EventWait(20);
	Engine_ItemShowFound(ITEM_FROST_JEWEL, 3);
	Engine_PartyGiveItem(ITEM_FROST_JEWEL, 0);
	Battle_GetWorkObject1e0()->motion_flags = 0;
	Camera_SetSpeed(0x19999, 0x3333);
	Camera_MoveTo(0x640000, 0, 0xf90000, 1);
	Engine_CameraWaitForMove();
	Engine_EventWait(20);
	FieldScene_RunEarlySequence();
	Camera_MoveTo(rec7->x.fixed, rec7->y.fixed, rec7->z.fixed, 1);
	Engine_CameraWaitForMove();
	Event_LoadAreaScript(ArutinYama_OpenedAreaScript);
	gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
	Engine_EventEnd();
}

static void FieldScene_RunScene3a4SequenceF(void)
{
	extern u8 ArutinYama_RiseTimer[];

	Engine_EventBegin();
	Engine_ActorSetPosition(9, 0, 0);
	Engine_ActorSetPosition(8, 0x1480000, 0x1a80000);
	Engine_GameFlagSet(0x323);
	FieldScene_RunScene3a4SequenceB();
	Engine_MapRedraw();
	Engine_TaskWait(1);
	gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 1);
	Engine_EventOpenScreen();
	Engine_EventWaitForScreen();
	FieldScene_RunScene3a4SequenceH();
	gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
	Engine_EventEnd();
}

static void FieldScene_RunScene3a4SequenceE(void)
{
	extern u8 ArutinYama_RiseTimer[];

	Engine_EventBegin();
	Engine_ActorSetPosition(10, 0, 0);
	Engine_ActorSetPosition(8, 0x1e80000, 0x8a0000);
	Engine_GameFlagSet(0x325);
	FieldScene_RunScene3a4SequenceA();
	Engine_MapRedraw();
	Engine_TaskWait(1);
	gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 1);
	Engine_EventOpenScreen();
	Engine_EventWaitForScreen();
	FieldScene_RunScene3a4SequenceI();
	gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
	Engine_EventEnd();
}

static void SceneAudio_PlayCue183EverySixtyTicks(void)
{
	ArutinYama_CueTicks++;
	if (ArutinYama_CueTicks == 60)
	{
		Engine_AudioPlayCue(183);
		ArutinYama_CueTicks = 0;
	}
}

static void EV10_TOUCH_10(void)
{
	Engine_EventBegin();
	Camera_SetSpeed(0x9999, 0x1333);
	Camera_MoveTo(0x1480000, -1, 0x570000, 1);
	Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x9999, 0x4ccc);
	Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 328, 116);
	Event_PlayCue(148);
	Engine_TaskAddCallback(SceneAudio_PlayCue183EverySixtyTicks, 3200);
	Work_SetValuesIfNonNegative(0x10000, 0x10000, 0x10000);
	Actor_SetSpeed(8, 0x1999, 0xccc);
	Actor_SetSpeed(9, 0x1999, 0xccc);
	Engine_ActorSetAnimation(8, 2);
	Actor_SetDestination(8, 328, 104);
	Actor_SetDestination(9, 328, 108);
	Engine_EventWait(60);
	Actor_ShowEmote(ACTOR_PARTY_LEADER, 256, 0);
	Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
	Engine_ActorWaitForMove(8);
	Battle_SetStart(3);
	Party_SetFields1ceAnd1d0((s32)&SceneId_ArutinYama10, 99);
	BattleFx_SetWeightedResult(53, 3);
}

static void SceneActor_SetActor8ModeByCounterBit(void)
{

	if (((gFrameCount >> 1) & 1) != 0)
	{
		Engine_ActorSetChildValue(8, 7);
	}
	else
	{
		Engine_ActorSetChildValue(8, 6);
	}
}

static void RunEventScript01(void)
{
	struct FieldActor *record;
	struct EventWork  *work;
	s32				   update;
	s32				   celebrate;
	s32				   party;

	Engine_EventBegin();
	Actor_SetPosition(8, 0x1480000, 0x580000);
	Actor_SetPosition(9, 0x1480000, 0x580000);
	Engine_ActorSetAnimation(8, 0);
	work = gEventWork;
	work->start_transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
	work->transition_frames = 40;
	Engine_EventOpenScreen();
	Engine_EventWaitForScreen();
	Engine_EventWait(20);
	record = Object_GetById(0);
	if (record != 0)
	{
		Actor_SetPosition(GERALD, record->x.fixed, record->z.fixed);
	}
	record = Object_GetById(0);
	if (record != 0)
	{
		Actor_SetPosition(IWAN, record->x.fixed, record->z.fixed);
	}
	record = Object_GetById(0);
	if (record != 0)
	{
		Actor_SetPosition(MEARI, record->x.fixed, record->z.fixed);
	}
	Actor_SetSpeed(GERALD, 0x9999, 0x4ccc);
	Actor_SetSpeed(IWAN, 0x9999, 0x4ccc);
	Actor_SetSpeed(MEARI, 0x9999, 0x4ccc);
	Engine_ActorEnableActionCallback(1, (const u8 *)ArutinYama_GeraldScript);
	Engine_ActorEnableActionCallback(2, (const u8 *)ArutinYama_IvanScript);
	Object_SetActionCallbackAndRefreshById(3, ArutinYama_MiaScript);
	Actor_FaceDirection(GERALD, 0xc000, 0);
	Actor_FaceDirection(IWAN, 0xc000, 0);
	Actor_FaceDirection(MEARI, 0xc000, 40);
	Engine_ActorRunRepeatedMotion(GERALD, 1);
	Actor_FaceDirection(GERALD, 0xe000, 10);
	Engine_EventSetMessage((s32)MsgArutinWeDidRobinWeBeat);
	Event_ShowMessageAndWait(GERALD, 0, 10);
	Engine_ActorRunRepeatedMotion(IWAN, 1);
	Actor_FaceDirection(IWAN, 0xa000, 10);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x2000, 0);
	Event_OpenMessage(IWAN, 0);
	if (Engine_EventChooseYesNo(0, 0) == 0)
	{
		Engine_ActorSetAnimationAndWait(IWAN, 3);
	}
	else
	{
		Engine_ActorSetAnimationAndWait(IWAN, 4);
		Event_SkipMessages(1);
	}
	Event_ShowMessageAndWait(IWAN, 0, 20);
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
	Engine_ActorSetAnimation(GERALD, 3);
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
	Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
	Engine_EventWait(20);
	Engine_ActorRunRepeatedMotion(MEARI, 2);
	Engine_EventSetMessage((s32)MsgArutinGuardianStatuesWereCreatedLong);
	Event_ShowMessage(MEARI, 0);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 10);
	Actor_ShowEmote(GERALD, 0x102, 60);
	Actor_FaceDirection(GERALD, 0x2000, 40);
	Event_ShowMessage(GERALD, 0);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x6000, 10);
	Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 0);
	Actor_ShowEmote(GERALD, 0x101, 0);
	Actor_ShowEmote(IWAN, 0x101, 0);
	Actor_ShowEmote(MEARI, 0x101, 40);
	Event_PlayCue(190);
	Actor_SetChildValue(8, 7);
	Engine_EventWait(10);
	Event_PlayCue(0x121);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
	Actor_FaceDirection(GERALD, 0xc000, 0);
	Actor_FaceDirection(IWAN, 0xc000, 0);
	Actor_FaceDirection(MEARI, 0xc000, 0);
	Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x100, 0);
	Actor_ShowEmote(GERALD, 0x100, 0);
	Actor_ShowEmote(IWAN, 0x100, 0);
	Actor_ShowEmote(MEARI, 0x100, 40);
	Event_PlayCue(103);
	update = (s32)SceneActor_SetActor8ModeByCounterBit;
	Task_AddCallback((void (*)(void))update, 0xc80);
	celebrate = (s32)ArutinYama_CelebrateScript;
	Engine_ActorEnableActionCallback(9, celebrate);
	Object_SetActionCallbackAndRefreshById(8, celebrate);
	Engine_TaskRemoveCallback(update);
	Engine_EventWait(60);
	Engine_ActorRunRepeatedMotion(IWAN, 2);
	Engine_EventWait(20);
	Event_ShowMessage(IWAN, 0);
	Actor_FaceDirection(GERALD, 0xe000, 0);
	Actor_FaceDirection(IWAN, 0xa000, 20);
	Engine_ActorSetAnimation(GERALD, 3);
	Engine_ActorSetAnimation(IWAN, 3);
	Engine_ActorSetAnimationAndWait(MEARI, 3);
	Engine_EventWait(20);
	Engine_ActorRunRepeatedMotion(MEARI, 1);
	Event_ShowMessageAndWait(MEARI, 0, 20);
	Actor_FaceDirection(GERALD, 0xe000, 0);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x6000, 40);
	Engine_ActorSetAnimationAndWait(GERALD, 3);
	Event_ShowMessageAndWait(GERALD, 0, 10);
	Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
	Engine_ActorSetAnimation(GERALD, 3);
	Engine_ActorSetAnimation(IWAN, 3);
	Engine_ActorSetAnimationAndWait(MEARI, 3);
	Engine_EventWait(20);
	party = (s32)ArutinYama_PartyScript;
	Engine_ActorEnableActionCallback(GERALD, party);
	Engine_ActorEnableActionCallback(2, party);
	Object_SetActionCallbackAndRefreshById(3, party);
	Engine_EventWait(20);
	gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
	GameFlag_Clear(FLAG_ARRIVAL_EVENT_PENDING);
	gEventWork->transition_frames = 16;
	GameFlag_Set(0x909);
	Engine_EventEnd();
}

static void ArutinYama_RequestPsynergy(void)
{
	gEventWork->psynergy_request = 0x1018;
}

static void SceneActor_SetActor12ModeByActorZeroHeight(void)
{

	if (Object_GetById(0)->y.fixed > 0x00300000)
	{
		{
			u8 *flag = &Object_GetById(11)->priority_flags;
			s32 bit = 2;

			bit |= *flag;
			*flag = bit;
		}
		Engine_ActorSetSpritePriority(12, 3);
	}
	else
	{
		Engine_ActorSetSpritePriority(12, 2);
	}
}

static void SceneActor_ClearCollisionFlagAndPlaceMarker(s32 no)
{
	struct FieldActor *record;

	record = Object_GetById(no);
	record->collision_flags &= 0xfe;

	SetMapCellCollision(0, record->x.fixed, record->z.fixed, 255);
}

static void ArutinYama_SettleAndMountLeader(struct FieldActor *object)
{
	s32					x;
	s32					z;
	s32					center_z;
	struct FieldSprite *sprite;
	struct FieldActor  *leader;

	Engine_AudioPlayCue(288);
	Engine_AudioPlayCue(232);
	x = object->x.fixed & 0xfff00000;
	center_z = 0x80000;
	z = object->z.fixed & 0xfff00000;
	object->acceleration = 0x20000;
	x += center_z;
	center_z += z;
	Engine_ObjectSetPosition(object, x, object->y.fixed, center_z);
	Engine_ObjectCommitPosition(object);
	object->terrain_id = 0;
	object->x.fixed = x;
	object->z.fixed = center_z;
	object->velocity_x = 0;
	object->velocity_z = 0;
	Object_SetMode(object, 2);
	Engine_TaskWait(15);
	Object_SetMode(object, 1);
	Engine_TaskWait(30);
	sprite = object->sprite;
	sprite->part_count = 1;
	ResourceMetadata_ClearRecord(*(s32 *)((u8 *)sprite + 44));
	*(s32 *)((u8 *)sprite + 44) = 0;
	((u8 *)sprite)[37] = 1;
	leader = Engine_ActorLookup(gGameState.selected_actor);
	Engine_AudioPlayCue(152);
	leader->x.fixed = x;
	leader->velocity_y = 0x60000;
	leader->gravity = 0x10000;
	leader->z.fixed = center_z;
	leader->sprite->priority = 0;
	Object_SetMode(leader, 7);
	Engine_ObjectSetPosition(leader, x, leader->y.fixed, z + 0x180000);
	leader->facing = 0x4000;
	Engine_TaskWait(20);
	leader->sprite->priority = 2;
	Engine_AudioPlayCue(159);
}

static void ArutinYama_ReservedNoOp(void)
{
}

static void ArutinYama_TurnRollingObjectA(struct FieldActor *object)
{
	union FieldCoordinate *p;
	union FieldCoordinate  pos[3];
	s32					   angle;
	s32					   x;
	s32					   z;
	s32					   n;

	angle = (object->facing + 0x4000) & 0xc000;
	p = pos;
	p[0].fixed = object->x.fixed;
	p[1].fixed = object->y.fixed;
	p[2].fixed = object->z.fixed;
	Vector_AddPolarOffset(0x180000, angle, p);
	x = (p[0].fixed + 0x80000) & 0xfff00000;
	z = (p[2].fixed + 0x80000) & 0xfff00000;
	angle += 0x8000;
	Object_SetMode(object, 5);
	n = 0;
	Engine_AudioPlayCue(184);
	while (n < 16)
	{
		angle += 0x400;
		p[0].fixed = x;
		p[2].fixed = z;
		Vector_AddPolarOffset(0x180000, angle, p);
		object->x.fixed = p[0].fixed;
		object->z.fixed = p[2].fixed;
		object->facing = angle + 0x4000;
		Engine_TaskWait(1);
		n++;
	}
	Engine_AudioPlayCue(233);
}

static void ArutinYama_TurnRollingObjectB(struct FieldActor *object)
{
	union FieldCoordinate *p;
	union FieldCoordinate  pos[3];
	s32					   angle;
	s32					   x;
	s32					   z;
	s32					   n;

	angle = (object->facing - 0x4000) & 0xc000;
	p = pos;
	p[0].fixed = object->x.fixed;
	p[1].fixed = object->y.fixed;
	p[2].fixed = object->z.fixed;
	Vector_AddPolarOffset(0x180000, angle, p);
	x = (p[0].fixed + 0x80000) & 0xfff00000;
	z = (p[2].fixed + 0x80000) & 0xfff00000;
	angle += 0x8000;
	Object_SetMode(object, 6);
	n = 0;
	Engine_AudioPlayCue(184);
	while (n < 16)
	{
		angle -= 0x400;
		p[0].fixed = x;
		p[2].fixed = z;
		Vector_AddPolarOffset(0x180000, angle, p);
		object->x.fixed = p[0].fixed;
		object->z.fixed = p[2].fixed;
		object->facing = angle - 0x4000;
		Engine_TaskWait(1);
		n++;
	}
	Engine_AudioPlayCue(233);
}

static void ArutinYama_SetRollingStateAndPlayCue(u16 *record)
{
	record = (u16 *)((char *)record + 102);
	{
		s32 value = 0x21;

		*record = value;
	}
	Engine_AudioPlayCue(288);
}

static void ArutinYama_AdvanceRollingObject(struct FieldActor *object)
{
	s32					   angle = 0xc000;
	s32					   radius;
	s16					  *timer;
	union FieldCoordinate  pos[3];
	union FieldCoordinate *p;
	s32					   ahead;
	s32					   behind;

	angle &= object->facing;
	radius = object->y.fixed / 0x10000;
	radius = ArutinYama_RollRadii[object->timer - radius + 16];
	timer = (s16 *)&object->work;
	if (*timer != 0)
	{
		if (--*timer == 20)
		{
			Engine_AudioPlayCue(184);
		}
		if (*timer == 0)
		{
			Engine_AudioPlayCue(233);
		}
	}
	p = pos;
	p[0].fixed = object->x.fixed;
	p[1].fixed = object->y.fixed;
	p[2].fixed = object->z.fixed;
	Vector_AddPolarOffset(Iwram_MulQ16(radius, 0xc000), angle, p);
	object->x.fixed = p[0].fixed;
	object->z.fixed = p[2].fixed;
	ahead = Map_GetTerrainHeight(2, p[0].fixed, p[2].fixed);
	Vector_AddPolarOffset(-Iwram_MulQ16(radius, 0x18000), angle, p);
	behind = Map_GetTerrainHeight(2, p[0].fixed, p[2].fixed);
	if (*timer <= 20)
	{
		if (ahead == behind)
		{
			Object_SetMode(object, 2);
		}
		else if (ahead > behind)
		{
			Object_SetMode(object, 3);
		}
		else
		{
			Object_SetMode(object, 4);
		}
	}
}

static void ArutinYama_RunRollingObject(s32 id, s32 heading)
{
	struct FieldActor	 *object = Object_GetById(id);
	struct FieldActor	 *leader = Engine_ActorLookup(gGameState.selected_actor);
	union FieldCoordinate pos[3];
	struct Byte			  zero;
	s32					  n;

	Engine_EventBegin();
	if (heading == -1)
	{
		heading = object->facing;
	}
	for (n = 0; n <= 3; n++)
	{
		pos[0].fixed = object->x.fixed;
		pos[1].fixed = object->y.fixed;
		pos[2].fixed = object->z.fixed;
		Vector_AddPolarOffset(0x100000, heading, pos);
		if (Map_GetTerrainHeight(2, pos[0].fixed, pos[2].fixed) == object->y.fixed)
		{
			break;
		}
		heading += 0x4000;
	}
	if (n == 4)
	{
		return;
	}
	object->terrain_id = 2;
	leader->x.fixed = 0;
	leader->z.fixed = 0;
	ResourceMetadata_Register(object->sprite, 16);
	Engine_CameraFollowActor(id, 1);
	Engine_CameraWaitForMove();
	Camera_SetSpeed(0x100000, 0x20000);
	object->facing = heading;
	object->speed = 0x20000;
	object->acceleration = 0xccc;
	zero.v = 0;
	object->paused = zero.v;
	object->timer = object->y.fixed / 0x10000;
	object->work = 0;
	pos[0].fixed = object->x.fixed;
	pos[1].fixed = object->y.fixed;
	pos[2].fixed = object->z.fixed;
	Vector_AddPolarOffset(0x180000, heading, pos);
	Engine_ObjectSetPosition(object, pos[0].fixed, object->y.fixed, pos[2].fixed);
	Engine_ObjectCommitPosition(object);
	Engine_AudioPlayCue(233);
	for (;;)
	{
		switch (GetMapCellCollision(2, object->x.fixed, object->z.fixed))
		{
		case 98:
			ArutinYama_TurnRollingObjectA(object);
			break;
		case 97:
			ArutinYama_TurnRollingObjectB(object);
			break;
		case 96:
			ArutinYama_SetRollingStateAndPlayCue(object);
			break;
		case 99:
			goto arrived;
		}
		ArutinYama_AdvanceRollingObject(object);
		Engine_TaskWait(1);
	}
arrived:
	ArutinYama_SettleAndMountLeader(object);
	Engine_EventEnd();
}

static void ArutinYama_StepPaletteAnim(void)
{
	u16 *palette = gPaletteBufferWork;

	if (ArutinYama_PaletteHold <= 0)
	{
		s32			frame;
		struct Half zero;

	next:
		frame = ArutinYama_PaletteScript[ArutinYama_PaletteStep++];
		if (frame == -1)
		{
			zero.v = 0;
			ArutinYama_PaletteStep = zero.v;
			goto next;
		}
		ArutinYama_PaletteHold = ArutinYama_PaletteScript[ArutinYama_PaletteStep++];
		palette += frame;
		Dma_Set(palette, (void *)0x05000006, DMA_ENABLE32 | 9, REG_DMA3);
	}
	ArutinYama_PaletteHold--;
}

static void ArutinYama_StartPaletteAnim(void)
{
	struct Half zero;

	zero.v = 0;
	ArutinYama_PaletteStep = zero.v;
	ArutinYama_PaletteHold = zero.v;
	Engine_TaskAddCallback((s32)&ArutinYama_StepPaletteAnim, 0xc80);
}

s8 ArutinYama_PaletteScript[] = {
	3, 16,
	2, 32,
	1, 64,
	2, 32,
	-1,
};

s32 ArutinYama_ActorScript[] = {
	SC_CALL(OverlayObject_UpdateFacingTowardTarget),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

s32 ArutinYama_EarlyActorScript[] = {
	SC_ADD(SW_SCALE_X, 546),
	SC_ADD(SW_SCALE_Y, 546),
	SC_WAIT(1),
	SC_LOOP(108, SC_TOP),
	SC_END,
};

s32 ArutinYama_LogRideScript[] = {
	SC_MOVE(PIXELS(786), 0, PIXELS(220)),
	SC_SYNC,
	SC_MOVE(PIXELS(786), 0, PIXELS(326)),
	SC_SYNC,
	SC_MOVE(PIXELS(786), 0, PIXELS(326)),
	SC_SYNC,
	SC_MOVE(PIXELS(786), 0, PIXELS(480)),
	SC_SYNC,
	SC_MOVE(PIXELS(786), 0, PIXELS(520)),
	SC_SYNC,
	SC_SET(0x23, 1),
	SC_END,
};

s32 ArutinYama_LeaderRideScript[] = {
	SC_ARG(5),
	SC_MOVE(PIXELS(804), 0, PIXELS(140)),
	SC_SYNC,
	SC_MOVE(PIXELS(804), 0, PIXELS(166)),
	SC_SYNC,
	SC_MOVE(PIXELS(764), 0, PIXELS(166)),
	SC_SYNC,
	SC_MOVE(PIXELS(764), 0, PIXELS(198)),
	SC_SYNC,
	SC_MOVE(PIXELS(786), 0, PIXELS(198)),
	SC_SYNC,
	SC_MOVE(PIXELS(786), 0, PIXELS(246)),
	SC_SYNC,
	SC_MOVE(PIXELS(764), 0, PIXELS(246)),
	SC_SYNC,
	SC_MOVE(PIXELS(764), 0, PIXELS(282)),
	SC_SYNC,
	SC_MOVE(PIXELS(780), 0, PIXELS(298)),
	SC_SYNC,
	SC_MOVE(PIXELS(804), 0, PIXELS(298)),
	SC_SYNC,
	SC_MOVE(PIXELS(804), 0, PIXELS(326)),
	SC_SYNC,
	SC_MOVE(PIXELS(776), 0, PIXELS(326)),
	SC_SYNC,
	SC_MOVE(PIXELS(776), 0, PIXELS(358)),
	SC_SYNC,
	SC_MOVE(PIXELS(804), 0, PIXELS(358)),
	SC_SYNC,
	SC_MOVE(PIXELS(804), 0, PIXELS(392)),
	SC_SYNC,
	SC_MOVE(PIXELS(786), 0, PIXELS(392)),
	SC_SYNC,
	SC_MOVE(PIXELS(762), 0, PIXELS(416)),
	SC_SYNC,
	SC_MOVE(PIXELS(762), 0, PIXELS(438)),
	SC_SYNC,
	SC_MOVE(PIXELS(804), 0, PIXELS(438)),
	SC_SYNC,
	SC_MOVE(PIXELS(804), 0, PIXELS(472)),
	SC_SYNC,
	SC_MOVE(PIXELS(794), 0, PIXELS(472)),
	SC_SYNC,
	SC_MOVE(PIXELS(792), 0, PIXELS(504)),
	SC_SYNC,
	SC_MOVE(PIXELS(808), 0, PIXELS(504)),
	SC_SYNC,
	SC_MOVE(PIXELS(808), 0, PIXELS(552)),
	SC_SYNC,
	SC_SET(0x23, 1),
	SC_MOVE(PIXELS(854), 0, PIXELS(552)),
	SC_SYNC,
	SC_ARG(1),
	SC_LABEL(1),
	SC_ADD(SW_FACING, 1024),
	SC_LOOP(24, SC_TO(1)),
	SC_END,
};

s32 ArutinYama_SparkScript[] = {
	SC_CALL(OverlayObject_IntegrateAndDamp),
	SC_WAIT(1),
	SC_LOOP(60, SC_TOP),
	SC_POS(0, 0, 0),
	SC_WAIT(1),
	SC_FREE,
};

s32 ArutinYama_GeraldScript[] = {
	SC_ARG(2),
	SC_MOVE(PIXELS(314), 0, PIXELS(126)),
	SC_SYNC,
	SC_ARG(1),
	SC_WAIT(10),
	SC_END,
};

s32 ArutinYama_IvanScript[] = {
	SC_ARG(2),
	SC_MOVE(PIXELS(346), 0, PIXELS(126)),
	SC_SYNC,
	SC_ARG(1),
	SC_WAIT(10),
	SC_END,
};

s32 ArutinYama_MiaScript[] = {
	SC_ARG(2),
	SC_MOVE(PIXELS(328), 0, PIXELS(138)),
	SC_SYNC,
	SC_ARG(1),
	SC_WAIT(10),
	SC_END,
};

s32 ArutinYama_CelebrateScript[] = {
	SC_ADD(SW_SCALE_X, 128),
	SC_ADD(SW_SCALE_Y, -1024),
	SC_WAIT(1),
	SC_LOOP(60, SC_TOP),
	SC_POS(0, 0, 0),
	SC_WAIT(1),
	SC_END,
};

s32 ArutinYama_PartyScript[] = {
	SC_ARG(2),
	SC_MOVE(PIXELS(328), 0, PIXELS(118)),
	SC_SYNC,
	SC_POS(0, 0, 0),
	SC_WAIT(1),
	SC_END,
};

struct SceneEntrance gArutinYamaEntrancesOther[] = {
	{0, CONDITION_ALWAYS, 160, 0, 160, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{SCENE_TABLE_END},
};

struct SceneEntrance gArutinYamaEntrances1[] = {
	{0, CONDITION_ALWAYS, 160, 0, 160, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{1, CONDITION_ALWAYS, 456, 0, 520, FACING_NORTH, 0, 0, 56, 506, 530, 0},
	{98, CONDITION_ALWAYS, 456, 0, 488, FACING_NORTH, 0, 0, 56, 506, 530, 0},
	{99, CONDITION_ALWAYS, 104, 0, 232, FACING_SOUTH, 0, 0, 56, 506, 530, 0},
	{SCENE_TABLE_END},
};

struct SceneEntrance gArutinYamaEntrances2[] = {
	{0, CONDITION_ALWAYS, 160, 0, 160, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{1, CONDITION_ALWAYS, 248, 0, 464, FACING_NORTH, 0, -1, -1, -1, -1, 0},
	{2, CONDITION_ALWAYS, 56, 0, 72, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{SCENE_TABLE_END},
};

struct SceneEntrance gArutinYamaEntrances3[] = {
	{0, CONDITION_ALWAYS, 160, 0, 160, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{1, CONDITION_ALWAYS, 488, 0, 560, FACING_NORTH, 0, -1, -1, -1, -1, 0},
	{2, CONDITION_ALWAYS, 664, 0, 380, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{3, CONDITION_ALWAYS, 904, 0, 214, FACING_NORTH, 0, 842, 40, 1016, 248, 0},
	{98, CONDITION_ALWAYS, 488, 0, 560, FACING_NORTH, 0, -1, -1, -1, -1, 0},
	{99, CONDITION_ALWAYS, 320, 0, 472, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{SCENE_TABLE_END},
};

struct SceneEntrance gArutinYamaEntrances4[] = {
	{0, CONDITION_ALWAYS, 160, 0, 160, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{1, CONDITION_ALWAYS, 248, 0, 498, FACING_NORTH, 0, 24, 0, 648, 560, 0},
	{2, CONDITION_ALWAYS, 119, 0, 108, FACING_SOUTH, 0, 24, 0, 648, 560, 0},
	{3, CONDITION_ALWAYS, 567, 0, 105, FACING_SOUTH, 0, 24, 0, 648, 560, 0},
	{4, CONDITION_ALWAYS, 887, 0, 798, FACING_NORTH, 0, 712, 568, 960, 860, 0},
	{5, CONDITION_ALWAYS, 791, 0, 679, FACING_SOUTH, 0, 712, 560, 960, 860, 0},
	{6, CONDITION_ALWAYS, 88, 0, 840, FACING_NORTH, 0, 32, 704, 272, 864, 0},
	{7, CONDITION_ALWAYS, 216, 0, 840, FACING_NORTH, 0, 32, 704, 272, 864, 0},
	{SCENE_TABLE_END},
};

struct SceneEntrance gArutinYamaEntrances5[] = {
	{0, CONDITION_ALWAYS, 160, 0, 160, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{1, CONDITION_ALWAYS, 585, 0, 501, FACING_NORTH, 0, 0, 0, 798, 530, 0},
	{98, CONDITION_ALWAYS, 585, 0, 501, FACING_NORTH, 0, 0, 0, 798, 530, 0},
	{99, CONDITION_ALWAYS, 747, 0, 258, FACING_SOUTH, 0, 0, 0, 798, 530, 0},
	{SCENE_TABLE_END},
};

struct SceneEntrance gArutinYamaEntrances6[] = {
	{0, CONDITION_ALWAYS, 160, 0, 160, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{1, CONDITION_ALWAYS, 568, 0, 348, FACING_NORTH, 0, 0, 0, 798, 530, 0},
	{2, CONDITION_ALWAYS, 295, 0, 95, FACING_SOUTH, 0, 0, 0, 798, 530, 0},
	{3, CONDITION_ALWAYS, 442, 0, 774, FACING_NORTH, 0, 0, 530, 512, 948, 0},
	{4, CONDITION_ALWAYS, 72, 0, 870, FACING_NORTH, 0, 0, 530, 512, 948, 0},
	{5, CONDITION_ALWAYS, 918, 0, 712, FACING_SOUTH, 0, 520, 570, 992, 786, 0},
	{6, CONDITION_ALWAYS, 615, 0, 684, FACING_SOUTH, 0, 520, 570, 992, 786, 0},
	{SCENE_TABLE_END},
};

struct SceneEntrance gArutinYamaEntrances7[] = {
	{0, CONDITION_ALWAYS, 160, 0, 160, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{1, CONDITION_ALWAYS, 72, 0, 502, FACING_NORTH, 0, 0, 0, 820, 568, 0},
	{2, CONDITION_ALWAYS, 152, 0, 174, FACING_SOUTH, 0, 0, 0, 820, 568, 0},
	{3, CONDITION_ALWAYS, 264, 0, 890, FACING_NORTH, 0, 144, 720, 384, 908, 0},
	{SCENE_TABLE_END},
};

struct SceneEntrance gArutinYamaEntrances8[] = {
	{0, CONDITION_ALWAYS, 160, 0, 160, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{1, CONDITION_ALWAYS, 392, 0, 116, FACING_SOUTH, 0, 0, 0, 830, 564, 0},
	{2, CONDITION_ALWAYS, 40, 0, 116, FACING_NORTH, 0, 0, 0, 830, 564, 0},
	{3, CONDITION_ALWAYS, 520, 0, 616, FACING_SOUTH, 0, 244, 564, 1024, 1024, 0},
	{4, CONDITION_ALWAYS, 408, 0, 970, FACING_NORTH, 0, 244, 564, 1024, 1024, 0},
	{5, CONDITION_ALWAYS, 951, 0, 791, FACING_SOUTH, 0, 244, 564, 1024, 1024, 0},
	{SCENE_TABLE_END},
};

struct SceneEntrance gArutinYamaEntrances9[] = {
	{0, CONDITION_ALWAYS, 160, 0, 160, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{1, CONDITION_ALWAYS, 361, 0, 309, FACING_NORTH, 0, 0, 0, 524, 816, 0},
	{2, CONDITION_ALWAYS, 89, 0, 749, FACING_NORTH, 0, 0, 0, 524, 816, 0},
	{3, CONDITION_ALWAYS, 728, 0, 717, FACING_NORTH, 0, 618, 0, 990, 854, 0},
	{4, CONDITION_ALWAYS, 935, 0, 493, FACING_SOUTH, 0, 618, 0, 990, 854, 0},
	{5, CONDITION_ALWAYS, 791, 0, 566, FACING_SOUTH, 0, 618, 0, 990, 854, 0},
	{6, CONDITION_ALWAYS, 792, 16, 577, FACING_SOUTH, 0, 618, 0, 998, 854, 0},
	{SCENE_TABLE_END},
};

struct SceneEntrance gArutinYamaEntrances10[] = {
	{0, CONDITION_ALWAYS, 160, 0, 160, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{1, CONDITION_ALWAYS, 88, 4, 424, FACING_SOUTH, 0, 0, 0, 448, 524, 0},
	{98, CONDITION_ALWAYS, 88, 4, 424, FACING_SOUTH, 0, 0, 0, 448, 524, 0},
	{99, CONDITION_ALWAYS, 328, 0, 118, FACING_NORTH, 0, 0, 0, 448, 524, 0},
	{SCENE_TABLE_END},
};

struct SceneEntrance gArutinYamaEntrances11[] = {
	{0, CONDITION_ALWAYS, 160, 0, 160, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{1, CONDITION_ALWAYS, 408, 0, 517, FACING_NORTH, 0, 0, 0, 540, 590, 0},
	{2, CONDITION_ALWAYS, 198, 0, 336, FACING_SOUTH, 0, 0, 0, 540, 590, 0},
	{3, CONDITION_ALWAYS, 568, 0, 136, FACING_NORTH, 0, 480, 15, 684, 180, 0},
	{4, CONDITION_ALWAYS, 73, 0, 190, FACING_NORTH, 0, 0, 0, 540, 590, 0},
	{SCENE_TABLE_END},
};

s16 gArutinYamaRegions9[] = {
#if defined(TBS_EDITION_JA) || defined(TBS_EDITION_EN) || defined(TBS_EDITION_DE)
	790,
	-106,
	575,
	794,
	-102,
	579,
	-1,
	6,
	788,
	-108,
	579,
	796,
	-100,
	587,
	-1,
	6,
#else
	784,
	-116,
	572,
	800,
	-100,
	588,
	-1,
	6,
#endif
	-1,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
};

s16 gArutinYamaRegions10[] = {
#if defined(TBS_EDITION_JA) || defined(TBS_EDITION_EN) || defined(TBS_EDITION_DE)
	84,
	38,
	420,
	92,
	46,
	428,
	-1,
	1,
#else
	80,
	34,
	416,
	96,
	50,
	432,
	-1,
	1,
#endif
	-1,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
};

u32 ArutinYama_StatueTable[] = {
	SCENE_EXITS(77),
	SCENE_EXIT(1, 75, 10),
	SCENE_EXIT(0, 78, 0),
	SCENE_EXIT(1, 80, 3),
	SCENE_EXIT(2, 79, 1),
	SCENE_EXIT(0, 79, 0),
	SCENE_EXIT(1, 78, 2),
	SCENE_EXIT(2, 79, 3),
	SCENE_EXIT(3, 79, 2),
	SCENE_EXIT(0, 80, 0),
	SCENE_EXIT(1, 75, 11),
	SCENE_EXIT(2, 80, 4),
	SCENE_EXIT(3, 78, 1),
	SCENE_EXIT(4, 80, 2),
	SCENE_EXIT(5, 82, 1),
	SCENE_EXIT(6, 74, 4),
	SCENE_EXIT(7, 74, 5),
	SCENE_EXIT(0, 81, 0),
	SCENE_EXIT(1, 82, 6),
	SCENE_EXIT(0, 82, 0),
	SCENE_EXIT(1, 80, 5),
	SCENE_EXIT(2, 82, 3),
	SCENE_EXIT(3, 82, 2),
	SCENE_EXIT(4, 82, 5),
	SCENE_EXIT(5, 82, 4),
	SCENE_EXIT(6, 81, 1),
	SCENE_EXIT(0, 83, 0),
	SCENE_EXIT(1, 84, 1),
	SCENE_EXIT(2, 85, 1),
	SCENE_EXIT(3, 75, 13),
	SCENE_EXIT(0, 84, 0),
	SCENE_EXIT(1, 83, 1),
	SCENE_EXIT(2, 85, 5),
	SCENE_EXIT(3, 87, 1),
	SCENE_EXIT(4, 75, 12),
	SCENE_EXIT(5, 85, 3),
	SCENE_EXIT(0, 85, 0),
	SCENE_EXIT(1, 83, 2),
	SCENE_EXIT(2, 85, 4),
	SCENE_EXIT(3, 84, 5),
	SCENE_EXIT(4, 85, 2),
	SCENE_EXIT(5, 84, 2),
	SCENE_EXIT(6, 86, 1),
	SCENE_EXIT(0, 86, 0),
	SCENE_EXIT(1, 85, 6),
	SCENE_EXIT(0, 87, 0),
	SCENE_EXIT(1, 84, 3),
	SCENE_EXIT(2, 87, 3),
	SCENE_EXIT(3, 87, 2),
	SCENE_EXIT(4, 2, 52),
	SCENE_EXITS_END,
};

struct ScenePlacement gArutinYamaPlacementsOther[] = {
	{SCENE_TABLE_END},
};

struct ScenePlacement gArutinYamaPlacements1[] = {
	{0xe3, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(152), 0, PIXELS(216), FACING_SOUTH,
		TALK_KEEP_FACING, 0},
	{0xe3, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(296), 0, PIXELS(216), FACING_SOUTH,
		TALK_KEEP_FACING, 0},
	{0xbe, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(296), 0, PIXELS(336), FACING_SOUTH,
		TALK_KEEP_FACING, 0},
	{SCENE_TABLE_END},
};

struct ScenePlacement gArutinYamaPlacements3[] = {
	{0xe7, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(216), 0, PIXELS(312), FACING_EAST,
		TALK_KEEP_FACING, 0},
	{0xbe, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(248), 0, PIXELS(464), FACING_WEST,
		TALK_KEEP_FACING, 0},
	{0x127, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(888), 0, PIXELS(172), FACING_SOUTH,
		TALK_KEEP_FACING, 0},
	{SCENE_TABLE_END},
};

struct ScenePlacement gArutinYamaPlacements5[] = {
	{0xe7, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(536), 0, PIXELS(296), FACING_EAST,
		TALK_KEEP_FACING, 0},
	{0xe3, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(408), 0, PIXELS(216), FACING_SOUTH,
		TALK_KEEP_FACING, 0},
	{0xbe, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(759), 0, PIXELS(285), FACING_WEST,
		TALK_KEEP_FACING, 0},
	{SCENE_TABLE_END},
};

struct ScenePlacement gArutinYamaPlacements6[] = {
	{0xe7, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(712), 0, PIXELS(184), FACING_EAST,
		TALK_KEEP_FACING, 0},
	{0xe3, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(696), 0, PIXELS(664), FACING_SOUTH,
		TALK_KEEP_FACING, 0},
	{SCENE_TABLE_END},
};

struct ScenePlacement gArutinYamaPlacements7[] = {
	{0xe7, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(264), 0, PIXELS(408), FACING_EAST,
		TALK_KEEP_FACING, 0},
	{0xe3, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(424), 0, PIXELS(424), FACING_SOUTH,
		TALK_KEEP_FACING, 0},
	{0xd3, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(408), 0, PIXELS(424), FACING_SOUTH,
		TALK_KEEP_FACING, 0},
	{0xe3, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(280), 0, PIXELS(168), FACING_SOUTH,
		TALK_KEEP_FACING, 0},
	{0xe3, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(424), 0, PIXELS(248), FACING_SOUTH,
		TALK_KEEP_FACING, 0},
	{0x127, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(520), 0, PIXELS(632), FACING_SOUTH,
		TALK_KEEP_FACING, 0},
	{0x5b, 0x47, ACTOR_STAND, PIXELS(56), 0, PIXELS(392), FACING_SOUTH, TALK_FACE_PARTY, 0},
	{SCENE_TABLE_END},
};

struct ScenePlacement gArutinYamaPlacements8[] = {
	{0x127, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(536), 0, PIXELS(636), FACING_SOUTH,
		TALK_KEEP_FACING, 0},
	{SCENE_TABLE_END},
};

struct ScenePlacement gArutinYamaPlacements9[] = {
	{0xc8, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(784), 0, PIXELS(80), FACING_SOUTH,
		TALK_KEEP_FACING, 0},
	{0x120, CONDITION_ALWAYS, 7, PIXELS(784), 0, PIXELS(80), FACING_SOUTH, TALK_KEEP_FACING, 0},
	{0xcd, CONDITION_ALWAYS, 7, PIXELS(774), 0, PIXELS(76), FACING_SOUTH, TALK_KEEP_FACING, 0},
	{0x16, CONDITION_ALWAYS, 7, PIXELS(774), 0, PIXELS(88), FACING_SOUTH, TALK_KEEP_FACING, 0},
	{0x112, CONDITION_ALWAYS, 7, PIXELS(792), 0, PIXELS(88), FACING_SOUTH, TALK_KEEP_FACING, 0},
	{GERALD, CONDITION_ALWAYS, ACTOR_STAND, 0, 0, 0, FACING_EAST, TALK_KEEP_FACING, 0},
	{SCENE_TABLE_END},
};

struct ScenePlacement gArutinYamaPlacements10[] = {
	{0xc7, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(328), 0, PIXELS(84), FACING_SOUTH,
		TALK_KEEP_FACING, 0},
	{0x120, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(328), 0, PIXELS(88), FACING_SOUTH,
		TALK_KEEP_FACING, 0},
	{GERALD, CONDITION_ALWAYS, ACTOR_STAND, 0, 0, 0, FACING_EAST, TALK_KEEP_FACING, 0},
	{IWAN, CONDITION_ALWAYS, ACTOR_STAND, 0, 0, 0, FACING_EAST, TALK_KEEP_FACING, 0},
	{MEARI, CONDITION_ALWAYS, ACTOR_STAND, 0, 0, 0, FACING_EAST, TALK_KEEP_FACING, 0},
	{SCENE_TABLE_END},
};

struct ScenePlacement gArutinYamaPlacements11[] = {
	{0x127, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(312), 0, PIXELS(280), FACING_SOUTH,
		TALK_KEEP_FACING, 0},
	{0xd3, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(200), 0, PIXELS(328), FACING_SOUTH,
		TALK_KEEP_FACING, 0},
	{SCENE_TABLE_END},
};

struct SceneEvent gArutinYamaEventsOther[] = {
	{SCENE_EVENTS_END},
};

struct SceneEvent gArutinYamaEvents1[] = {
	{EVENT_EXIT, 1, CONDITION_ALWAYS, 1},
	{EVENT_TOUCH, 10, 0x904, EVENT_SCRIPT(EV10_TOUCH_1)},
	{EVENT_TOUCH, 11, 0x905, EVENT_SCRIPT(EV11_TOUCH_1)},
	{EVENT_TALK, 10, CONDITION_ALWAYS, EVENT_SCRIPT(ArutinYama_StartAreaOneEncounter)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 0x40a, CONDITION_ALWAYS,
		EVENT_SCRIPT(ArutinYama_StartAreaOneEncounter)},
	{SCENE_EVENTS_END},
};

struct SceneEvent ArutinYama_OpenedAreaScript[] = {
	{EVENT_EXIT, 1, CONDITION_ALWAYS, 1},
	{PSYNERGY_ON_ACTOR(ABILITY_FROST), 8, 0x200,
		EVENT_SCRIPT(EV08_PSYNERGY)},
	{PSYNERGY_ON_ACTOR(ABILITY_FROST), 9, 0x201, EVENT_SCRIPT(EV09_PSYNERGY)},
	{SCENE_EVENTS_END},
};

struct SceneEvent gArutinYamaEvents2[] = {
	{EVENT_EXIT, 1, CONDITION_ALWAYS, 1},
	{EVENT_EXIT, 2, CONDITION_ALWAYS, 2},
	{SCENE_EVENTS_END},
};

struct SceneEvent gArutinYamaEvents3[] = {
	{EVENT_EXIT, 1, CONDITION_ALWAYS, 1},
	{EVENT_EXIT, 2, CONDITION_ALWAYS, 2},
	{EVENT_EXIT, 3, CONDITION_ALWAYS, 3},
	{EVENT_TALK, 8, CONDITION_ALWAYS, EVENT_SCRIPT(EV08_TALK)},
	{EVENT_TALK, 9, CONDITION_ALWAYS, EVENT_SCRIPT(ArutinYama_StartAreaThreeEncounter)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 0x409, CONDITION_ALWAYS,
		EVENT_SCRIPT(ArutinYama_StartAreaThreeEncounter)},
	{PSYNERGY_ON_ACTOR(0x8f), 10, CONDITION_ALWAYS, 0},
	{SEARCH(SEARCH_UNNAMED), 83, CONDITION_ALWAYS, EVENT_SCRIPT(EV83_SEARCH)},
	{SEARCH(SEARCH_CHEST), 100, 0xf76, FIND_ITEM(0xc0)},
	{SCENE_EVENTS_END},
	{EVENT_EXIT, 3, CONDITION_ALWAYS, 3},
	{SEARCH(SEARCH_CHEST), 100, 0xf05, FIND_ITEM(0xb4)},
	{SCENE_EVENTS_END},
};

struct SceneEvent gArutinYamaEvents4[] = {
	{EVENT_EXIT, 1, CONDITION_ALWAYS, 1},
	{EVENT_EXIT, 2, CONDITION_ALWAYS, 2},
	{EVENT_EXIT, 3, CONDITION_ALWAYS, 3},
	{EVENT_EXIT, 4, CONDITION_ALWAYS, 4},
	{EVENT_EXIT, 5, CONDITION_ALWAYS, 5},
	{EVENT_EXIT, 6, CONDITION_ALWAYS, 6},
	{EVENT_EXIT, 7, CONDITION_ALWAYS, 7},
	{SCENE_EVENTS_END},
};

struct SceneEvent gArutinYamaEvents5[] = {
	{EVENT_EXIT, 1, CONDITION_ALWAYS, 1},
	{PSYNERGY_ON_ACTOR(ABILITY_FROST), 9, 0x200, EVENT_SCRIPT(EV09_PSYNERGY_5)},
	{EVENT_TALK, 10, CONDITION_ALWAYS, EVENT_SCRIPT(ArutinYama_StartAreaFiveEncounter)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 0x40a, CONDITION_ALWAYS,
		EVENT_SCRIPT(ArutinYama_StartAreaFiveEncounter)},
	{EVENT_TALK, 8, CONDITION_ALWAYS, EVENT_SCRIPT(EV08_TALK)},
	{SEARCH(SEARCH_UNNAMED), 50, CONDITION_ALWAYS, EVENT_SCRIPT(EV50_SEARCH_5)},
	{0x2413, 100, 0xf77, FIND_ITEM(0x7b)},
	{0x413, 100, 0xf77, FIND_ITEM(0x7b)},
	{0xe413, 100, 0xf77, FIND_ITEM(0x7b)},
	{SCENE_EVENTS_END},
};

struct SceneEvent gArutinYamaEvents6[] = {
	{EVENT_EXIT, 1, CONDITION_ALWAYS, 1},
	{EVENT_EXIT, 2, CONDITION_ALWAYS, 2},
	{EVENT_EXIT, 3, CONDITION_ALWAYS, 3},
	{EVENT_EXIT, 4, CONDITION_ALWAYS, 4},
	{EVENT_EXIT, 5, CONDITION_ALWAYS, 5},
	{EVENT_EXIT, 6, CONDITION_ALWAYS, 6},
	{PSYNERGY_ON_ACTOR(ABILITY_FROST), 9, 0x200, EVENT_SCRIPT(EV09_PSYNERGY_6)},
	{EVENT_TALK, 8, CONDITION_ALWAYS, EVENT_SCRIPT(EV08_TALK)},
	{SEARCH(SEARCH_UNNAMED), 50, CONDITION_ALWAYS, EVENT_SCRIPT(EV50_SEARCH_6)},
	{SEARCH(SEARCH_CHEST), 100, 0xef2, 0x500002},
	{SCENE_EVENTS_END},
};

struct SceneEvent gArutinYamaEvents7[] = {
	{EVENT_EXIT, 1, CONDITION_ALWAYS, 1},
	{EVENT_EXIT, 2, CONDITION_ALWAYS, 2},
	{EVENT_EXIT, 3, CONDITION_ALWAYS, 3},
	{SEARCH(SEARCH_CHEST), 101, 0xf07, FIND_ITEM(0xba)},
	{EVENT_TALK, 14, CONDITION_ALWAYS, EVENT_SCRIPT(EV14_TALK)},
	{EVENT_TALK, 8, CONDITION_ALWAYS, EVENT_SCRIPT(EV08_TALK)},
	{SEARCH(SEARCH_UNNAMED), 50, CONDITION_ALWAYS, EVENT_SCRIPT(EV50_SEARCH_7)},
	{EVENT_TOUCH, 10, CONDITION_ALWAYS, EVENT_SCRIPT(EV10_TOUCH_7)},
	{EVENT_TOUCH, 11, CONDITION_ALWAYS, EVENT_SCRIPT(EV11_TOUCH_7)},
	{PSYNERGY_ON_ACTOR(0x8c), 10, CONDITION_ALWAYS, EVENT_SCRIPT(EV10_PSYNERGY)},
	{TOUCH_WALKING_INTO(FACING_EAST), 12, CONDITION_ALWAYS,
		EVENT_SCRIPT(EV12_TOUCH)},
	{PSYNERGY_ON_ACTOR(ABILITY_FROST), 9, 0x200, EVENT_SCRIPT(EV09_PSYNERGY_7)},
	{PSYNERGY_ON_ACTOR(ABILITY_FROST), 11, 0x201,
		EVENT_SCRIPT(EV11_PSYNERGY_7)},
	{PSYNERGY_ON_ACTOR(ABILITY_FROST), 12, 0x204,
		EVENT_SCRIPT(EV12_PSYNERGY)},
	{SCENE_EVENTS_END},
};

struct SceneEvent gArutinYamaEvents8[] = {
	{EVENT_EXIT, 1, CONDITION_ALWAYS, 1},
	{EVENT_EXIT, 2, CONDITION_ALWAYS, 2},
	{EVENT_EXIT, 3, CONDITION_ALWAYS, 3},
	{EVENT_EXIT, 4, CONDITION_ALWAYS, 4},
	{EVENT_EXIT, 5, CONDITION_ALWAYS, 5},
	{SEARCH(SEARCH_CHEST), 100, 0xf78, FIND_ITEM(ITEM_LUCKY_MEDAL)},
	{PSYNERGY_ON_ACTOR(0x8f), 8, CONDITION_ALWAYS, 0},
	{SCENE_EVENTS_END},
};

struct SceneEvent gArutinYamaEvents9[] = {
	{EVENT_EXIT, 1, CONDITION_ALWAYS, 1},
	{EVENT_EXIT, 2, CONDITION_ALWAYS, 2},
	{EVENT_EXIT, 3, CONDITION_ALWAYS, 3},
	{EVENT_EXIT, 4, CONDITION_ALWAYS, 4},
	{EVENT_EXIT, 5, CONDITION_ALWAYS, 5},
	{SEARCH(SEARCH_UNNAMED), 12, CONDITION_ALWAYS, EVENT_SCRIPT(EV12_SEARCH)},
	{PSYNERGY_ON_ACTOR(0x8e), 11, 0x908, EVENT_SCRIPT(EV11_PSYNERGY_9)},
	{SCENE_EVENTS_END},
};

struct SceneEvent gArutinYamaEvents10[] = {
	{EVENT_TOUCH, 10, 0x909, EVENT_SCRIPT(EV10_TOUCH_10)},
	{SEARCH(SEARCH_CHEST), 100, 0xf08, FIND_ITEM(0xcb)},
	{SCENE_EVENTS_END},
};

struct SceneEvent gArutinYamaEvents11[] = {
	{EVENT_EXIT, 1, CONDITION_ALWAYS, 1},
	{EVENT_EXIT, 2, CONDITION_ALWAYS, 2},
	{EVENT_EXIT, 3, CONDITION_ALWAYS, 3},
	{EVENT_EXIT, 4, CONDITION_ALWAYS, 4},
	{PSYNERGY_ON_ACTOR(0x8f), 8, CONDITION_ALWAYS, 0},
	{PSYNERGY_ON_ACTOR(0x8c), 9, CONDITION_ALWAYS, 0},
	{SEARCH(SEARCH_CHEST), 100, 0xf79, FIND_ITEM(ITEM_VIAL)},
	{SCENE_EVENTS_END},
};

s32 ArutinYama_CueTicks = 0;

u32 ArutinYama_RollRadii[] = {
	0x10000,
	0x10b55,
	0x1172b,
	0x12387,
	0x1306f,
	0x13dea,
	0x14bfd,
	0x15ab0,
	0x16a09,
	0x17a11,
	0x18ace,
	0x19c49,
	0x1ae89,
	0x1c199,
	0x1d581,
	0x1ea4b,
	0x20000,
	0x216ab,
	0x22e56,
	0x2470f,
	0x260df,
	0x27bd4,
	0x297fb,
	0x2b560,
	0x2d413,
	0x2f422,
	0x3159c,
	0x33892,
	0x35d13,
	0x38333,
	0x3ab03,
	0x3d495,
	0x40000,
};
