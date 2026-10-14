#include "game_state.h"
#include "unit/scene/soru_funka.h"
#include "unit/scene/common/effect_psynergy.h"
#include "unit/scene/common/soru_star_ring_setup.h"
#include "unit/object/object.h"
#include "unit/object/objtable.h"
#include "unit/object/sparkle.h"
#include "unit/object/evbasic.h"
#include "unit/object/evactor.h"
#include "unit/object/evcamera.h"
#include "unit/object/scenefx.h"
#include "unit/object/djinnev.h"
#include "unit/object/psydisp.h"
#include "unit/object/psyfx.h"
#include "unit/field/field.h"
#include "unit/runtime/objvram.h"
#include "unit/runtime/heap.h"
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
#include "b5_context.h"
#include "soru_funka.h"
#include "types.h"
#include "field_event.h"
#include "field_service.h"
#include "iwram_call.h"
#include "field_effect.h"
#include "text/msg_ids.h"
#include "field_scene.h"
#include "objcmd.h"
#include "fieldobj.h"
#include "global_cells.h"
#include "vram_tab.h"

// 前方宣言
union PairObject;
static void SoruFunka_RaiseAndShrinkActor(s32 a0);
static void SoruFunka_ThrowEruptionRing(void);
static void SceneState_InitStateWordsAndSlots(void);
static void SoruFunka_StepEmbers(void);
static void SceneState_UpdateRandomTimerLevel(void);
static void Scene_RunExtendedEffectPresentation(void);
static void SoruFunka_PlaceBeforeStar(s32 actor);
static void SceneState_ApplyRectsByCondition(s32 a);
static void SceneState_ApplyRectPairByFlag(s32 a);
static void FieldScene_RunRandomHalfBranch(void);
static void FieldScene_RunLateRandomHalfBranch(void);
static void FieldScene_RunFourWayEffectSequence(u32 mode);
static void SceneEffect_AdvanceTenEntryTimers(void);
static void SceneActor_PlaceAtTileAndRunSteps(s32 a, s32 b);
static void SceneState_ConfigureEightCornerRegions(void);
static void FieldScene_RunVariantStep(s32 a, s32 b, s32 c);
static void FieldScene_RunStepByRuntimeBits(s32 a);
static void SoruFunka_SpawnEffectPair(union PairObject *parent);
static void FieldScene_RunActor15TwoStep(void);

s32 Math_RemainderUnsigned(s32 value, s32 modulus);

struct EmberSprite
{
	u8 unknown_00[9];
	u8 lo : 2;
	u8 layer : 2;
	u8 hi : 4;
};

struct Ember
{
	u8					unknown_00[8];
	s32					x;
	s32					y;
	s32					z;
	u8					unknown_14[16];
	s32					velocity_x;
	s32					velocity_y;
	s32					velocity_z;
	s32					speed;
	s32					acceleration;
	s32					target_x;
	s32					target_y;
	s32					target_z;
	u8					unknown_44[12];
	struct EmberSprite *sprite;
	u8					unknown_54;
	u8					motion_flags;
};

enum
{
	ACTOR_WISE_ONE = 15,
	ACTOR_FIRST_ROCK = 16,
	ROCK_COUNT = 16
};

enum
{
	OBJECT_ELEMENTAL_STAR = 22,
	ITEM_MARS_STAR = 222
};

enum
{
	FLAG_SOL_SANCTUM_ERUPTED = 0x814,
	FLAG_STAR_ROOM_COLLAPSED = 0x83f
};

struct QuakeWork
{
	u8	unknown_000[0x40c];
	s32 fading;
};

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

struct WorldMapOam
{
	u8	unknown_00[4];
	u16 attr0;
	u16 x : 9;
	u16 affine_index : 5;
	u16 size : 2;
};

u8 *SceneData_GetScriptTable(void)
{
	return (u8 *)Placement_Scripts;
}

s32 SceneData_ReturnZero(void)
{
	return 0;
}

u8 *SceneData_GetMessageTable(void)
{
	return (u8 *)Placement_Messages;
}

u8 *SceneData_GetActorTable(void)
{
	return (u8 *)Placement_Actors;
}

u8 *SceneData_GetEffectTable(void)
{
	return (u8 *)Placement_Effects;
}

static void Scene_SaturosTakesHostages(void)
{
	void Actor_ShowEmote();
	void ColorBuffer_ApplyTarget();
	void Event_PlayCue();

	u8 *record;
	s32 particle_work_ptr;
	s32 event_work_ptr;
	u8 *p7;

	particle_work_ptr = (s32)gParticleWork;
	p7 = *(u8 **)particle_work_ptr;
	Engine_EventBegin();
	*(s32 *)(((s32)p7 + 0x40c)) = 0;
	Event_PlayCue(141);
	Work_SetValuesIfNonNegative(0x10000, 0x10000, 0x10000);
	Actor_SetPosition(ACTOR_PARTY_LEADER, 0xe80000, 0x9c0000);
	Actor_SetPosition(GERALD, 0xda0000, 0xac0000);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xb000, 0);
	Actor_FaceDirection(GERALD, 0xd000, 0);
	Actor_SetPosition(JASMINE, 0x1db0000, 0x14c0000);
	Actor_SetPosition(ACTOR_SUKURETA, 0x1eb0000, 0x14c0000);
	Actor_SetPosition(ACTOR_MENARDI, 0x1cb0000, 0x15c0000);
	Actor_SetPosition(ACTOR_SATUROS, 0x1fb0000, 0x15c0000);
	Actor_SetPosition(ACTOR_GARCIA, 0x1d70000, 0x1320000);
	Actor_SetPosition(ACTOR_ALEX, 0x1df0000, 0x16a0000);
	Engine_ActorSetSpritePriority(15, 1);
	event_work_ptr = particle_work_ptr - 8;
	Camera_MoveTo(0xe80000, -1, 0x9c0000, 0);
	Engine_MapRedraw();
	*(s32 *)((*(s32 *)event_work_ptr + 0x1c8)) = 8;
	Engine_EventOpenScreen();
	Engine_EventWaitForScreen();
	ColorBuffer_ApplyTarget(0x7fff, 0);
	Engine_ColorBufferInterpolate(4);
	Engine_EventWait(4);
	ColorBuffer_ApplyTarget(0x10000, 0);
	Engine_ColorBufferInterpolate(4);
	Engine_EventWait(4);
	SoruFunka_ThrowEruptionRing();
	ColorBuffer_ApplyTarget(0x7fff, 0);
	Engine_ColorBufferInterpolate(4);
	Engine_EventWait(16);
	Event_PlayCue(144);
	ColorBuffer_ApplyTarget(0x10000, 0);
	Engine_ColorBufferInterpolate(4);
	Engine_EventWait(4);
	ColorBuffer_ApplyTarget(0x7fff, 0);
	Engine_ColorBufferInterpolate(4);
	Engine_EventWait(4);
	Event_PlayCue(144);
	ColorBuffer_ApplyTarget(0x10000, 0);
	Engine_ColorBufferInterpolate(48);
	Engine_EventWait(48);
	Engine_ActorJump(ACTOR_PARTY_LEADER, 6, 0);
	Engine_ActorJump(GERALD, 6, 20);
	FieldScene_RunVariantStep(1, 20, 20);
	FieldScene_RunVariantStep(0, 20, 40);
	Engine_EventSetMessage((s32)MsgSoruJasmineWhatHappened);
	Engine_EventShowMessageAndWait(11, 0, 20);
	Engine_EventShowMessage(10, 0);
	FieldScene_RunVariantStep(1, 20, 0);
	Event_ShowMessageAndWait(JASMINE, 0, 20);
	FieldScene_RunVariantStep(0, 20, 0);
	Event_ShowMessageAndWait(ACTOR_ALEX, 0, 20);
	Event_ShowMessageAndWait(ACTOR_SUKURETA, 0, 20);
	Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, GERALD, 0);
	Engine_EventWait(40);
	Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
	Engine_ActorRunRepeatedMotion(GERALD, 2);
	Engine_EventWait(20);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xb000, 0);
	Actor_FaceDirection(GERALD, 0xd000, 30);
	FieldScene_RunVariantStep(1, 20, 0);
	Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
	Engine_ActorRunRepeatedMotion(GERALD, 2);
	Engine_EventWait(20);
	FieldScene_RunVariantStep(0, 20, 20);
	record = Object_GetById(15);
	Engine_ActorSetSpriteFlags(record, 0);
	Actor_SetPosition(15, 0x1450000, 0x12e0000);
	record = Object_GetById(15);
	record[85] = 5;
	*(s32 *)(((s32)p7 + 0x40c)) = 1;
	Event_PlayCue(0x121);
	Work_SetValuesIfNonNegative(-1, -1, 0xe666);
	Engine_MapRenderWaitForValues();
	Engine_EventWait(150);
	Event_ShowMessageAndWait(ACTOR_MENARDI, 0, 20);
	Event_ShowMessage(JASMINE, 0);
	Event_ShowMessageAndWait(ACTOR_SATUROS, 0, 10);
	Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, GERALD, 20);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x3000, 0);
	Actor_FaceDirection(GERALD, 0x3000, 10);
	Actor_SetPosition(JASMINE, 0x1db0000, 0x14c0000);
	Actor_SetPosition(ACTOR_SUKURETA, 0x1eb0000, 0x14c0000);
	Actor_SetPosition(ACTOR_MENARDI, 0x1cb0000, 0x15c0000);
	Actor_SetPosition(ACTOR_SATUROS, 0x1fb0000, 0x15c0000);
	Actor_SetPosition(ACTOR_GARCIA, 0x1d70000, 0x1320000);
	Actor_SetPosition(ACTOR_ALEX, 0x1df0000, 0x16a0000);
	Camera_SetSpeed(0x66666, 0xcccc);
	Camera_MoveTo(0x1480000, -1, 0x12b0000, 1);
	Engine_CameraWaitForMove();
	Event_PlayCue(167);
	ColorBuffer_ApplyTarget(0x205294, 2);
	Engine_ColorBufferInterpolate(20);
	Engine_TaskWait(20);
	ColorBuffer_ApplyTarget(0x10000, 2);
	Engine_ColorBufferInterpolate(20);
	Engine_EventWait(200);
	Event_OpenMessage(0x1001, 0);
	if (Engine_EventChooseYesNo(0, 0) == 0)
	{
		Engine_EventSetMessage((s32)MsgSoruIKnowItsARock);
	}
	else
	{
		Engine_EventSetMessage((s32)MsgSoruSomeoneIsLiftingIt);
	}
	Event_ShowMessageAndWait(0x1001, 0, 80);
	Engine_EventSetMessage((s32)MsgSoruSukuretaCouldThatBeThe);
	Event_ShowMessageAndWait(ACTOR_SUKURETA, 0, 20);
	FieldScene_RunVariantStep(1, 20, 0);
	*(s32 *)(((s32)p7 + 0x40c)) = 0;
	Event_PlayCue(141);
	Work_SetValuesIfNonNegative(0x10000, 0x20000, 0x10000);
	Engine_EventWait(80);
	*(s32 *)(((s32)p7 + 0x40c)) = 1;
	Event_PlayCue(0x121);
	Work_SetValuesIfNonNegative(-1, -1, 0xe666);
	Engine_MapRenderWaitForValues();
	FieldScene_RunVariantStep(0, 20, 60);
	Event_ShowMessageAndWait(ACTOR_ALEX, 0, 30);
	Actor_FaceDirection(15, 0xa000, 40);
	FieldScene_RunVariantStep(1, 20, 20);
	FieldScene_RunVariantStep(0, 20, 20);
	Event_ShowMessageAndWait(0x1001, 0, 30);
	Actor_FaceDirection(15, 0x1000, 40);
	FieldScene_RunVariantStep(1, 20, 20);
	FieldScene_RunVariantStep(0, 20, 20);
	Event_ShowMessageAndWait(JASMINE, 0, 30);
	SceneState_InitStateWordsAndSlots();
	Task_AddCallback(SoruFunka_StepEmbers, 0xc80);
	Task_AddCallback(SceneState_UpdateRandomTimerLevel, 0xc80);
	Engine_EventWait(240);
	Event_ShowMessageAndWait(ACTOR_SATUROS, 0, 30);
	Actor_SetPosition(JASMINE, 0x1db0000, 0x14c0000);
	Actor_SetPosition(ACTOR_SUKURETA, 0x1eb0000, 0x14c0000);
	Actor_SetPosition(ACTOR_MENARDI, 0x1cb0000, 0x15c0000);
	Actor_SetPosition(ACTOR_SATUROS, 0x1fb0000, 0x15c0000);
	Actor_SetPosition(ACTOR_GARCIA, 0x1d70000, 0x1320000);
	Actor_SetPosition(ACTOR_ALEX, 0x1df0000, 0x16a0000);
	Actor_FaceDirection(JASMINE, 0x8000, 0);
	Actor_FaceDirection(ACTOR_SUKURETA, 0x8000, 0);
	Actor_FaceDirection(ACTOR_MENARDI, 0x8000, 0);
	Actor_FaceDirection(ACTOR_SATUROS, 0x8000, 0);
	Actor_FaceDirection(ACTOR_GARCIA, 0x8000, 0);
	Actor_FaceDirection(ACTOR_ALEX, 0x8000, 0);
	record = Object_GetById(5);
	Camera_MoveTo((*(s16 *)((s32)record + 10) << 16), -1, (*(s16 *)((s32)record + 18) << 16), 1);
	Engine_CameraWaitForMove();
	Engine_EventWait(40);
	Engine_ActorRunRepeatedMotion(ACTOR_GARCIA, 2);
	Engine_EventWait(20);
	Event_ShowMessage(ACTOR_GARCIA, 0);
	Engine_ActorRunRepeatedMotion(ACTOR_MENARDI, 2);
	Event_ShowMessageAndWait(ACTOR_MENARDI, 0, 10);
	Engine_ActorSetAnimation(ACTOR_ALEX, 4);
	Event_ShowMessageAndWait(ACTOR_ALEX, 0, 20);
	Engine_ActorFaceEachOther(ACTOR_SATUROS, ACTOR_MENARDI, 40);
	Engine_ActorSetAnimation(ACTOR_SATUROS, 4);
	Event_ShowMessageAndWait(ACTOR_SATUROS, 0, 10);
	Actor_ShowEmote(JASMINE, 0x100, 0);
	Actor_FaceDirection(JASMINE, 0x3000, 40);
	Event_ShowMessageAndWait(0x4005, 0, 10);
	Actor_FaceDirection(ACTOR_SATUROS, 0xb000, 0);
	Actor_FaceDirection(ACTOR_MENARDI, 0xd000, 40);
	Engine_ActorFaceEachOther(ACTOR_SATUROS, ACTOR_MENARDI, 40);
	Actor_ShowEmote(ACTOR_SATUROS, 0x105, 40);
	Actor_FaceDirection(ACTOR_SATUROS, 0xb000, 10);
	Event_ShowMessageAndWait(ACTOR_SATUROS, 0, 10);
	Actor_FaceDirection(ACTOR_SATUROS, 0x8000, 10);
	Engine_ActorRunRepeatedMotion(ACTOR_MENARDI, 2);
	Event_ShowMessageAndWait(ACTOR_MENARDI, 0, 10);
	Engine_ActorSetAnimation(ACTOR_SATUROS, 3);
	Event_ShowMessageAndWait(ACTOR_SATUROS, 0, 40);
	Actor_FaceDirection(ACTOR_ALEX, 0xb000, 60);
	Engine_ActorSetAnimationAndWait(ACTOR_ALEX, 3);
	Event_ShowMessageAndWait(ACTOR_ALEX, 0, 10);
	Actor_FaceDirection(ACTOR_GARCIA, 0x3000, 0);
	Actor_FaceDirection(JASMINE, 0x3000, 0);
	Actor_FaceDirection(ACTOR_SUKURETA, 0x5000, 0);
	Actor_FaceDirection(ACTOR_MENARDI, 0x3000, 0);
	Actor_FaceDirection(ACTOR_SATUROS, 0x5000, 20);
	Actor_FaceDirection(ACTOR_ALEX, 0xd000, 20);
	Engine_ActorSetAnimationAndWait(ACTOR_ALEX, 4);
	Event_ShowMessageAndWait(ACTOR_ALEX, 0, 10);
	Actor_ShowEmote(ACTOR_GARCIA, 0x103, 0);
	Actor_SetSpeed(ACTOR_GARCIA, 0x10000, 0x8000);
	Actor_WalkToAndWait(ACTOR_GARCIA, 0x1d7, 0x13a);
	Engine_ActorStartRepeatedMotion(ACTOR_GARCIA, 3);
	Event_ShowMessageAndWait(ACTOR_GARCIA, 0, 10);
	Actor_FaceDirection(ACTOR_ALEX, 0xb000, 10);
	Engine_ActorSetAnimation(ACTOR_ALEX, 4);
	Event_ShowMessageAndWait(ACTOR_ALEX, 0, 10);
	Engine_ActorStartRepeatedMotion(ACTOR_GARCIA, 3);
	Event_ShowMessageAndWait(ACTOR_GARCIA, 0, 10);
	Engine_ActorRunRepeatedMotion(ACTOR_SUKURETA, 2);
	Event_ShowMessageAndWait(0x4009, 0, 10);
	Actor_FaceDirection(ACTOR_SATUROS, 0xb000, 20);
	Event_ShowMessageAndWait(ACTOR_SATUROS, 0, 30);
	Actor_FaceDirection(ACTOR_MENARDI, 0, 20);
	Engine_ActorRunRepeatedMotion(ACTOR_MENARDI, 2);
	Engine_EventWait(30);
	Engine_ActorSetAnimationAndWait(ACTOR_MENARDI, 3);
	Engine_EventWait(30);
	Actor_FaceDirection(ACTOR_MENARDI, 0xd000, 30);
	Engine_ActorSetAnimationAndWait(ACTOR_MENARDI, 4);
	Engine_EventWait(10);
	Event_ShowMessageAndWait(ACTOR_MENARDI, 0, 10);
	Actor_FaceDirection(ACTOR_GARCIA, 0x5000, 20);
	Engine_ActorRunRepeatedMotion(ACTOR_GARCIA, 2);
	Engine_EventWait(20);
	Engine_ActorFaceEachOther(ACTOR_SATUROS, ACTOR_MENARDI, 0);
	Engine_EventWait(30);
	Engine_ActorSetAnimation(ACTOR_SATUROS, 3);
	Engine_ActorSetAnimationAndWait(ACTOR_MENARDI, 3);
	Actor_FaceDirection(ACTOR_MENARDI, 0xd000, 0);
	Actor_FaceDirection(ACTOR_SATUROS, 0xb000, 0);
	Engine_EventWait(40);
	Engine_ActorRunRepeatedMotion(ACTOR_GARCIA, 1);
	Engine_EventWait(30);
	Actor_FaceDirection(ACTOR_GARCIA, 0x8000, 0);
	Engine_EventWait(40);
	Actor_ShowEmote(ACTOR_GARCIA, 0x102, 0);
	Engine_EventWait(60);
	Actor_FaceDirection(ACTOR_ALEX, 0xb000, 0);
	Engine_EventWait(30);
	Engine_ActorSetAnimationAndWait(ACTOR_ALEX, 4);
	Event_ShowMessageAndWait(0x200e, 0, 30);
	Actor_FaceDirection(ACTOR_SUKURETA, 0xb000, 0);
	Actor_FaceDirection(JASMINE, 0xb000, 0);
	Actor_FaceDirection(ACTOR_GARCIA, 0x3000, 20);
	Actor_ShowEmote(JASMINE, 0x102, 40);
	Event_ShowMessageAndWait(0x2005, 0, 40);
	Actor_ShowEmote(ACTOR_GARCIA, 0x105, 60);
	Engine_ActorSetAnimationAndWait(ACTOR_GARCIA, 3);
	Actor_ShowEmote(ACTOR_SATUROS, 0x101, 0);
	Actor_ShowEmote(ACTOR_MENARDI, 0x101, 60);
	Engine_ActorRunRepeatedMotion(ACTOR_ALEX, 2);
	Event_ShowMessageAndWait(ACTOR_ALEX, 0, 60);
	Engine_ActorSetAnimationAndWait(ACTOR_GARCIA, 3);
	Engine_EventWait(40);
	Actor_ShowEmote(JASMINE, 0x102, 0);
	Actor_ShowEmote(ACTOR_SUKURETA, 0x102, 40);
	Engine_ActorFaceEachOther(ACTOR_SATUROS, ACTOR_MENARDI, 20);
	Engine_ActorSetAnimation(ACTOR_SATUROS, 3);
	Engine_ActorSetAnimationAndWait(ACTOR_MENARDI, 3);
	Engine_EventWait(20);
	Actor_SetSpeed(ACTOR_SATUROS, 0x9999, 0x4ccc);
	Actor_SetSpeed(ACTOR_MENARDI, 0x9999, 0x4ccc);
	Actor_WalkTo(ACTOR_MENARDI, 0x1db, 0x15c);
	Actor_WalkTo(ACTOR_SATUROS, 0x1eb, 0x15c);
	Engine_ActorWaitForMove(ACTOR_MENARDI);
	Engine_ActorWaitForMove(ACTOR_SATUROS);
	Engine_ActorSetAnimation(ACTOR_MENARDI, 1);
	Engine_ActorSetAnimation(ACTOR_SATUROS, 1);
	Actor_FaceDirection(ACTOR_MENARDI, 0xd000, 0);
	Actor_FaceDirection(ACTOR_SATUROS, 0xb000, 0);
	Actor_FaceDirection(ACTOR_GARCIA, 0x3000, 10);
	Actor_ShowEmote(ACTOR_MENARDI, 0x103, 0);
	Engine_ActorRunRepeatedMotion(ACTOR_MENARDI, 2);
	Event_ShowMessage(0x200b, 0);
	Actor_SetSpeed(ACTOR_MENARDI, 0x19999, 0xcccc);
	Actor_SetSpeed(JASMINE, 0x13333, 0x9999);
	Actor_WalkToAndWait(ACTOR_MENARDI, 0x1db, 0x152);
	record = Object_GetById(11);
	((struct FacingObject *)record)->facing_flags =
		(254 & ((struct FacingObject *)record)->facing_flags);
	Actor_WalkToAndWait(ACTOR_MENARDI, 0x1db, 0x15c);
	Engine_ActorRunRepeatedMotion(JASMINE, 1);
	Engine_ActorJump(JASMINE, 4, 0);
	Actor_WalkToAndWait(JASMINE, 0x1cb, 0x13c);
	Actor_SetSpeed(JASMINE, 0x8000, 0x4000);
	Actor_ShowEmote(ACTOR_GARCIA, 0x103, 0);
	Engine_ActorStartRepeatedMotion(ACTOR_GARCIA, 3);
	Event_ShowMessageAndWait(ACTOR_GARCIA, 0, 30);
	Engine_ActorRunRepeatedMotion(ACTOR_MENARDI, 1);
	Engine_EventWait(20);
	((struct FacingObject *)record)->facing_flags |= 1;
	Actor_SetSpeed(ACTOR_MENARDI, 0x10000, 0x8000);
	Actor_WalkToAndWait(ACTOR_MENARDI, 0x1db, 0x14c);
	Actor_FaceDirection(ACTOR_MENARDI, 0xb000, 20);
	Event_ShowMessage(0x200b, 0);
	Actor_FaceDirection(ACTOR_SATUROS, 0xb000, 0);
	Engine_ActorStartRepeatedMotion(ACTOR_SATUROS, 2);
	Event_ShowMessageAndWait(ACTOR_SATUROS, 0, 20);
	Actor_ShowEmote(ACTOR_MENARDI, 0x102, 20);
	Actor_FaceDirection(JASMINE, 0x3000, 0);
	Actor_FaceDirection(ACTOR_MENARDI, 0x3000, 20);
	Engine_ActorSetAnimationAndWait(ACTOR_SATUROS, 4);
	Engine_ActorSetAnimationAndWait(ACTOR_MENARDI, 3);
	Actor_FaceDirection(ACTOR_MENARDI, 0xb000, 20);
	Engine_ActorSetAnimationAndWait(ACTOR_MENARDI, 3);
	Engine_ActorSetAnimationAndWait(ACTOR_GARCIA, 3);
	Engine_EventWait(20);
	Actor_WalkTo(JASMINE, 0x1b0, 0x13c);
	Actor_WalkToAndWait(ACTOR_GARCIA, 0x1a6, 0x137);
	Engine_ActorSetAnimation(JASMINE, 1);
	Actor_FaceDirection(JASMINE, 0x3000, 0);
	Actor_FaceDirection(ACTOR_GARCIA, 0x3000, 20);
	Engine_ActorRunRepeatedMotion(ACTOR_SATUROS, 2);
	Actor_FaceDirection(ACTOR_SATUROS, 0xd000, 20);
	Event_ShowMessageAndWait(0x100a, 0, 40);
	Engine_ActorRunRepeatedMotion(ACTOR_SUKURETA, 2);
	Engine_EventWait(20);
	Actor_FaceDirection(ACTOR_SUKURETA, 0xb000, 30);
	Actor_SetSpeed(ACTOR_SUKURETA, 0x8000, 0x4000);
	Actor_WalkToAndWait(ACTOR_SUKURETA, 0x1eb, 0x128);
	Actor_FaceDirection(ACTOR_SUKURETA, 0x5000, 40);
	Actor_SetSpeed(ACTOR_SATUROS, 0x10000, 0x8000);
	Actor_SetSpeed(ACTOR_MENARDI, 0x10000, 0x8000);
	Actor_SetSpeed(ACTOR_GARCIA, 0x10000, 0x8000);
	Actor_SetSpeed(ACTOR_ALEX, 0x10000, 0x8000);
	Actor_WalkToAndWait(ACTOR_SATUROS, 0x1d7, 0x134);
	Actor_FaceDirection(ACTOR_SATUROS, 0x5000, 0);
	Actor_WalkToAndWait(ACTOR_MENARDI, 0x1c7, 0x134);
	Actor_FaceDirection(ACTOR_MENARDI, 0x3000, 0);
	Actor_WalkToAndWait(ACTOR_ALEX, 0x1e7, 0x134);
	Actor_FaceDirection(ACTOR_SATUROS, 0xc000, 0);
	Actor_FaceDirection(ACTOR_MENARDI, 0xd000, 0);
	Actor_FaceDirection(ACTOR_ALEX, 0xb000, 0);
	Actor_FaceDirection(JASMINE, 0xd000, 0);
	Actor_FaceDirection(ACTOR_GARCIA, 0xd000, 0);
	Camera_SetSpeed(0x8000, 0x1000);
	Camera_MoveTo(0x1d80000, -1, 0x12c0000, 1);
	Engine_CameraWaitForMove();
	Engine_EventWait(40);
	GameFlag_Set(0x246);
	Actor_SetSpeed(ACTOR_SATUROS, 0x8000, 0x4000);
	Actor_SetSpeed(ACTOR_MENARDI, 0x8000, 0x4000);
	Actor_SetSpeed(ACTOR_ALEX, 0x8000, 0x4000);
	Engine_ActorSetAnimationAndWait(ACTOR_SATUROS, 3);
	SoruFunka_RaiseAndShrinkActor(10);
	Engine_ActorRunRepeatedMotion(ACTOR_SUKURETA, 2);
	SoruFunka_RaiseAndShrinkActor(9);
	Engine_ActorSetAnimationAndWait(ACTOR_MENARDI, 3);
	SoruFunka_RaiseAndShrinkActor(11);
	Actor_FaceDirection(JASMINE, 0x9000, 40);
	Engine_ActorStartRepeatedMotion(JASMINE, 2);
	Event_ShowMessageAndWait(0x2005, 0, 40);
	Actor_FaceDirection(ACTOR_GARCIA, 0x3000, 0);
	Actor_FaceDirection(JASMINE, 0xb000, 30);
	Actor_FaceDirection(ACTOR_GARCIA, 0xe000, 0);
	SoruFunka_RaiseAndShrinkActor(5);
	SoruFunka_RaiseAndShrinkActor(13);
	Actor_FaceDirection(ACTOR_ALEX, 0x7000, 40);
	Event_ShowMessageAndWait(ACTOR_ALEX, 0, 30);
	Engine_ActorSetAnimationAndWait(ACTOR_ALEX, 4);
	Engine_EventWait(20);
	Event_ShowMessageAndWait(ACTOR_ALEX, 0, 30);
	SoruFunka_RaiseAndShrinkActor(14);
	Scene_RunExtendedEffectPresentation();
	Party_RemoveOwnerRestored(5);
}

static void SoruFunka_RaiseAndShrinkActor(s32 a0)
{
	struct FieldActor *rec7;
	struct FieldActor *recA;
	s32				   rec2;
	u8				   i;

	recA = Object_GetById(8);
	recA->scale_x = 0x10000;
	recA->scale_y = 0x10000;
	Engine_ActorWalkToAndWait(a0, 0x1d7, 0x122);
	Actor_FaceDirection(a0, 0xc000, 0);
	Engine_EventWait(10);
	Actor_SetPosition(8, 0x1d70000, 0x1220000);
	rec7 = Object_GetById(a0);
	rec2 = Object_GetById(a0);
	Engine_ActorSetSpriteFlags(rec2, 0);
	Actor_SetChildValue(a0, 0x100);
	rec7->motion_flags = 0;
	Event_PlayCue(201);
	i = 0;
	do
	{
		rec7->y.fixed += 0x8000;
		Engine_EventWait(1);
		i++;
	} while (i != 60);
	Event_PlayCue(190);
	i = 0;
	do
	{
		rec7->y.fixed += 0x1999;
		rec7->scale_x -= 0x28f;
		rec7->scale_y -= 0x28f;
		recA->scale_x -= 0x28f;
		recA->scale_y -= 0x28f;
		Engine_EventWait(1);
		i++;
	} while (i != 90);
	Actor_SetPosition(a0, 0, 0);
	Actor_SetPosition(8, 0, 0);
}

static void Resource381_NoOpCallbackA(void)
{
}

static void Resource381_NoOpCallbackB(void)
{
}

s32 SoruFunka_RunHostageSceneOnEntranceTen(void)
{
	if (gGameState.entrance == 10)
	{
		FieldEffect_InitSparkles();
		Scene_SaturosTakesHostages();
	}
	return 0;
}

static s32 OverlayObject_SetRecordAngleFromHeading(Ent *p)
{
	*(u16 *)(p->unk50 + 30) = p->unk6 + 0x4000;
	return 1;
}

static void SoruFunka_ThrowEruptionRing(void)
{
	struct FieldActor  *actor;
	struct FieldActor  *spun;
	u32					i;
	s32					angle;
	struct FieldSprite *sprite;
	s32					dx;
	s32					dz;

	for (i = 16; i <= 31; i++)
	{
		actor = Object_GetById(i);
		Engine_ActorStop(i);
		Engine_ActorSetSpriteFlags(actor, 0);
		Object_SetMode(actor, 2);
		actor->sprite->priority = 0;
		actor->motion_flags = 0;
		actor->speed = 0x80000;
		actor->acceleration = 0xc000;
		actor->scale_x = 0x1cccc;
		actor->scale_y = 0x1cccc;
		actor->x.fixed = 0xe80000;
		actor->y.fixed = 0x140000;
		actor->z.fixed = 0x840000;
	}
	Engine_AudioPlayCue(145);
	for (i = 0; i <= 15; i++)
	{
		spun = Object_GetById(i + 16);
		sprite = spun->sprite;
		angle = i << 12;
		sprite->rotation = angle - 0x4000;
		dx = Iwram_MulQ16(Engine_MathCos(angle), 0x1000000);
		dz = Iwram_MulQ16(Engine_MathSin(angle), 0x1000000);
		Engine_ObjectSetPosition(spun, spun->x.fixed + dx, spun->y.fixed, spun->z.fixed + dz);
	}
	Engine_EventWait(20);
	Engine_ActorWaitForMove(16);
	for (i = 16; i <= 31; i++)
	{
		actor = Object_GetById(i);
		Engine_ActorStop(i);
		actor->scale_x = 0x10000;
		actor->scale_y = 0x10000;
		actor->x.fixed = 0;
		actor->y.fixed = 0;
		actor->z.fixed = 0;
		actor->velocity_x = 0;
		actor->velocity_y = 0;
		actor->velocity_z = 0;
		actor->target_x = ACTOR_NO_TARGET;
		actor->target_y = ACTOR_NO_TARGET;
		actor->target_z = ACTOR_NO_TARGET;
	}
}

static void SceneState_InitStateWordsAndSlots(void)
{
	s32 *p;
	u32	 i;

	gEmberMask = 63;
	gEmberTimer = 0;
	gEmberLevel = 0;
	gEmberLevelTimer = 120;
	p = gEmberState;
	for (i = 0; i < 16; i++)
	{
		*p++ = 0;
	}
}

static void SoruFunka_StepEmbers(void)
{
	u8			 *work;
	s32			  level;
	s32			  t;
	u8			  i;
	struct Ember *spark;
	struct Ember *ember;
	s16			  angle;
	s32			  x;
	s32			  z;

	work = (*(u8 **)gParticleWork);
	level = (u32)gEmberTimer / 10;
	if (level != 0)
	{
		*(s32 *)(work + 0x40c) = 0;
		Work_SetValuesIfNonNegative(level << 16, level << 16, 0x10000);
	}
	else
	{
		*(s32 *)(work + 0x40c) = 1;
		Work_SetValuesIfNonNegative(-1, -1, 0xe666);
	}
	t = gEmberTimer;
	if (t != 0)
		gEmberTimer = t - 3;

	for (i = 0; i < 16; i++)
	{
		if (gEmberState[i] != 0)
		{
			spark = (void *)Object_GetById(i + 16);
			if (spark->target_x == 0x80000000 && spark->target_z == 0x80000000)
			{
				gEmberState[i]++;
				if (gEmberState[i] == 2)
					Object_SetMode(spark, 3);
				if (gEmberState[i] == 19)
				{
					spark->x = 0;
					spark->y = 0;
					spark->z = 0;
					spark->velocity_x = 0;
					spark->velocity_y = 0;
					spark->velocity_z = 0;
					spark->target_x = 0x80000000;
					spark->target_y = 0x80000000;
					spark->target_z = 0x80000000;
					ObjectGroup_SetChildValue(spark, 15);
				}
				else if (gEmberState[i] == 20)
				{
					gEmberState[i] = 0;
				}
			}
		}
	}

	if (gEmberMask == 999)
		return;
	if (gEmberMask & gFrameCount)
		return;
	for (i = 0; i < 16; i++)
	{
		angle = Math_RemainderUnsigned(Engine_RandomNext(), 0xffff);
		ember = (void *)Object_GetById(i + 16);
		if (gEmberState[i] == 0)
		{
			if (Engine_GameFlagIsSet(0x246) == 0)
				Engine_AudioPlayCue(246);
			gEmberState[i] = 1;
			ember->motion_flags = 0;
			ember->speed = 0x80000;
			ember->acceleration = 0x10000;
			Engine_ActorSetSpriteFlags(ember, 0);
			ember->sprite->layer = 1;
			Object_SetMode(ember, 2);
			Engine_ObjectSetScript(ember, Funka_EmberScript);
			ember->x = Iwram_MulQ16(Engine_MathCos((u16)angle),
						   (((u32)(Engine_RandomNext() << 8) >> 16) << 16) + 0x1000000)
					   + 0x1450000;
			ember->y = 0;
			ember->z = Iwram_MulQ16(Engine_MathSin((u16)angle),
						   (((u32)(Engine_RandomNext() << 8) >> 16) << 16) + 0x1000000)
					   + 0x12e0000;
			x = Iwram_MulQ16(
				Engine_MathCos((u16)angle), ((Engine_RandomNext() & 63) << 16) + 0x80000);
			z = Iwram_MulQ16(
				Engine_MathSin((u16)angle), ((Engine_RandomNext() & 63) << 16) + 0x80000);
			Engine_ObjectSetPosition(ember, x + 0x1450000, 0, z + 0x11e0000);
			ObjectGroup_SetChildValue(ember, 0);
			gEmberTimer = 30;
			return;
		}
	}
}

static void SceneState_UpdateRandomTimerLevel(void)
{
	u32 v;

	if (gEmberLevelTimer != 0)
	{
		gEmberLevelTimer--;
		return;
	}
	if (gEmberLevel != 0)
	{
		gEmberLevel--;
	}
	else
	{
		gEmberLevel = (u32)(Engine_RandomNext() << 2) >> 16;
	}
	v = gEmberLevel;
	switch (v)
	{
	case 3:
		gEmberMask = v;
		gEmberLevelTimer = ((u32)(Engine_RandomNext() * 20) >> 16) + 40;
		break;
	case 2:
		gEmberMask = 15;
		gEmberLevelTimer = ((u32)(Engine_RandomNext() * 40) >> 16) + 80;
		break;
	case 1:
		gEmberMask = 63;
		gEmberLevelTimer = ((u32)(Engine_RandomNext() * 80) >> 16) + 160;
		break;
	default:
		gEmberMask = 127;
		gEmberLevelTimer = ((u32)(Engine_RandomNext() * 160) >> 16) + 320;
		break;
	}
}

static void Scene_RunExtendedEffectPresentation(void)
{
	struct QuakeWork   *quake;
	struct FieldActor  *wise_one;
	struct FieldActor  *center;
	struct FieldActor  *leader;
	struct FieldActor  *actor;
	struct FieldActor  *gerald;
	struct FieldSprite *leader_sprite;
	struct FieldSprite *gerald_sprite;
	struct FieldActor  *star;
	struct FieldSprite *sprite;
	u8				   *buf;
	u8					i;
	u32					cnt;
	s32					wait;

	quake = (struct QuakeWork *)gParticleWork[0];
	wise_one = Actor_Get(ACTOR_WISE_ONE);
	Engine_TaskRemoveCallback(SceneState_UpdateRandomTimerLevel);
	gEmberMask = 3;
	Engine_EventWait(80);
	Event_PlayCue(17);
	ColorBuffer_ApplyTarget(0x7fff, 0);
	Engine_ColorBufferInterpolate(40);
	Engine_EventWait(40);
	Engine_TaskRemoveCallback(SoruFunka_StepEmbers);
	for (i = 0; i < ROCK_COUNT; i++)
	{
		Actor_SetPosition(ACTOR_FIRST_ROCK + i, 0, 0);
	}
	Engine_TaskWait(1);
	Actor_SetPosition(ACTOR_WISE_ONE, 0, 0);
	SoruFunka_PlaceBeforeStar(ACTOR_PARTY_LEADER);
	SoruFunka_PlaceBeforeStar(GERALD);
	quake->fading = 0;
	Work_SetValuesIfNonNegative(0x10000, 0x10000, 0x10000);
	Engine_EventWait(80);
	SceneState_ConfigureEightCornerRegions();
	center = Engine_EventGetViewCenter();
	center->motion_flags = 0;
	center->x.fixed = PIXELS(231);
	center->z.fixed = PIXELS(144);
	center->target_x = ACTOR_NO_TARGET;
	center->target_y = ACTOR_NO_TARGET;
	center->target_z = ACTOR_NO_TARGET;
	center->y.fixed = 0;
	center->velocity_x = 0;
	center->velocity_z = 0;
	Engine_TaskWait(4);
	Engine_MapRedraw();
	Engine_TaskWait(4);
	Engine_ActorSetSpritePriority(ACTOR_PARTY_LEADER, 3);
	Engine_ActorSetSpritePriority(GERALD, 3);
	ColorBuffer_ApplyTarget(0x10000, 0);
	Engine_ColorBufferInterpolate(40);
	Engine_EventWait(40);

	leader = Actor_Get(ACTOR_PARTY_LEADER);
	gerald = Actor_Get(GERALD);
	leader_sprite = leader->sprite;
	gerald_sprite = gerald->sprite;
	for (cnt = 0; cnt < 20; cnt++)
	{
		leader_sprite->rotation += 0x100;
		gerald_sprite->rotation -= 0x100;
		leader->x.fixed += 0x6000;
		gerald->x.fixed -= 0x6000;
		Engine_TaskWait(1);
	}
	Engine_EventWait(40);
	Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x20000, 0x10000);
	Actor_SetSpeed(GERALD, 0x20000, 0x10000);
	Actor_Get(ACTOR_PARTY_LEADER)->sprite->rotation = 0;
	Actor_Get(GERALD)->sprite->rotation = 0;
	Engine_ActorJump(ACTOR_PARTY_LEADER, 6, 0);
	Engine_ActorJump(GERALD, 6, 0);
	Actor_SetDestination(ACTOR_PARTY_LEADER, 246, 150);
	Engine_ObjectMotionSetPositionAndCommit(GERALD, 220, 150);
	Engine_ActorSetSpritePriority(ACTOR_PARTY_LEADER, 2);
	Engine_ActorSetSpritePriority(GERALD, 2);
	Actor_Get(ACTOR_PARTY_LEADER)->priority_flags |= ACTOR_PRIORITY_AUTOMATIC;
	Actor_Get(GERALD)->priority_flags |= ACTOR_PRIORITY_AUTOMATIC;
	Engine_ActorStop(ACTOR_PARTY_LEADER);
	Engine_ActorStop(GERALD);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 0);
	Engine_EventWait(20);
	Actor_FaceDirection(GERALD, FACING_NORTHEAST, 0);
	Engine_EventWait(40);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST + FACING_STEP, 0);
	Engine_EventWait(40);
	Actor_FaceDirection(GERALD, FACING_SOUTH + FACING_STEP, 0);
	Engine_EventWait(80);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
	Engine_EventWait(10);
	Actor_FaceDirection(GERALD, FACING_EAST + FACING_STEP, 0);
	Engine_EventWait(60);

	Engine_ActorRunRepeatedMotion(GERALD, 2);
	Engine_EventSetMessage((s32)MsgSoruWellTurnedBadly);
	Event_OpenMessage(GERALD, 0);
	if (Engine_EventChooseYesNo(0, 0) == 0)
	{
		Engine_ActorSetAnimationAndWait(GERALD, ANIM_NOD);
		Engine_EventSetMessage((s32)MsgSoruWellTurnedBadly + 1);
	}
	else
	{
		Engine_ActorRunRepeatedMotion(GERALD, 1);
		Engine_EventSetMessage((s32)MsgSoruWellTurnedBadly + 2);
	}
	Event_ShowMessageAndWait(GERALD, 0, 60);
	Actor_FaceDirection(GERALD, FACING_SOUTHEAST + FACING_STEP, 0);
	Engine_EventWait(40);
	Engine_ActorJump(GERALD, 2, 0);
	Actor_ShowEmote(GERALD, EMOTE_IN_FRONT | 2, 0);
	Engine_EventWait(40);
	Engine_EventSetMessage((s32)MsgSoruFriendsGone);
	Event_ShowMessageAndWait(GERALD, 0, 10);
	Engine_ActorJump(ACTOR_PARTY_LEADER, 2, 0);
	Engine_EventWait(10);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 0);
	Engine_EventWait(60);
	Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 2, 0);
	Engine_EventWait(80);
	Actor_FaceDirection(GERALD, FACING_EAST + FACING_STEP, 0);
	Engine_EventWait(10);
	Engine_ActorRunRepeatedMotion(GERALD, 2);
	Event_OpenMessage(GERALD, 0);
	if (Engine_EventChooseYesNo(0, 0) == 0)
	{
		Engine_EventWait(10);
		Engine_ActorRunRepeatedMotion(GERALD, 2);
		Engine_EventWait(10);
		Actor_FaceDirection(GERALD, FACING_SOUTHEAST + FACING_STEP, 0);
		Engine_EventWait(60);
		Actor_FaceDirection(GERALD, FACING_EAST + FACING_STEP, 0);
		Engine_EventWait(10);
		Engine_ActorSetAnimation(GERALD, ANIM_SHAKE_HEAD);
		Engine_EventSetMessage((s32)MsgSoruThanksALot);
	}
	else
	{
		Engine_EventWait(10);
		Engine_ActorRunRepeatedMotion(GERALD, 2);
		Engine_EventWait(10);
		Actor_FaceDirection(GERALD, FACING_SOUTHEAST + FACING_STEP, 0);
		Engine_EventWait(60);
		Actor_FaceDirection(GERALD, FACING_EAST + FACING_STEP, 0);
		Engine_EventWait(10);
		Engine_ActorSetAnimation(GERALD, ANIM_SHAKE_HEAD);
		Engine_EventSetMessage((s32)MsgSoruTheyllBeSafe);
	}
	Event_ShowMessageAndWait(GERALD, 0, 40);
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
	Engine_ActorSetAnimationAndWait(GERALD, ANIM_NOD);
	Engine_EventWait(40);

	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
	Actor_FaceDirection(GERALD, FACING_SOUTHWEST + FACING_STEP, 0);
	Camera_SetSpeed(0x20000, 0x4000);
	Camera_MoveTo(PIXELS(284), -1, PIXELS(92), 1);
	Engine_CameraWaitForMove();
	Engine_EventWait(20);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
	Actor_FaceDirection(GERALD, FACING_SOUTH + FACING_STEP, 0);
	Camera_SetSpeed(0x18000, 0x3000);
	Camera_MoveTo(PIXELS(127), -1, PIXELS(162), 1);
	Engine_CameraWaitForMove();
	Engine_EventWait(40);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 0);
	Actor_FaceDirection(GERALD, FACING_SOUTHEAST + FACING_STEP, 0);
	Camera_SetSpeed(0x40000, 0x8000);
	Camera_MoveTo(PIXELS(304), -1, PIXELS(294), 1);
	Engine_CameraWaitForMove();
	Engine_EventWait(20);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
	Actor_FaceDirection(GERALD, FACING_EAST + FACING_STEP, 0);
	Camera_SetSpeed(0x10000, 0x2000);
	Camera_MoveTo(PIXELS(400), -1, PIXELS(215), 1);
	Engine_CameraWaitForMove();
	Engine_EventWait(60);
	Camera_SetSpeed(0x40000, 0x8000);
	Camera_MoveTo(PIXELS(273), -1, PIXELS(145), 1);
	Engine_CameraWaitForMove();
	Engine_EventWait(20);
	Engine_ActorRunRepeatedMotion(GERALD, 2);
	Engine_EventSetMessage((s32)MsgSoruThisIsTerrible);
	Event_ShowMessageAndWait(GERALD, 0, 20);
	Actor_FaceDirection(GERALD, FACING_SOUTHEAST + FACING_STEP, 0);
	Engine_EventWait(40);
	Actor_FaceDirection(GERALD, FACING_NORTH + FACING_STEP, 0);
	Engine_EventWait(20);
	Actor_FaceDirection(GERALD, FACING_NORTHEAST + FACING_STEP, 0);
	Engine_EventWait(60);
	Actor_FaceDirection(GERALD, FACING_SOUTHEAST + FACING_STEP, 0);
	Engine_EventWait(40);
	Actor_FaceDirection(GERALD, FACING_NORTHEAST + FACING_STEP, 0);
	Engine_EventWait(10);
	Event_ShowMessageAndWait(GERALD, 0, 10);
	Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
	Engine_EventWait(10);

	Event_PlayCue(23);
	FieldScene_RunVariantStep(1, 4, 0);
	quake->fading = 0;
	Work_SetValuesIfNonNegative(0x50000, 0x50000, 0x10000);
	Engine_EventWait(10);
	FieldScene_RunVariantStep(0, 40, 0);
	Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x20000, 0x10000);
	Actor_SetSpeed(GERALD, 0x20000, 0x10000);
	Engine_ActorJump(ACTOR_PARTY_LEADER, 6, 0);
	Engine_ActorJump(GERALD, 6, 0);
	Actor_SetDestination(ACTOR_PARTY_LEADER, 243, 144);
	Actor_SetDestination(GERALD, 202, 144);
	Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
	Engine_EventWait(20);
	quake->fading = 1;
	Work_SetValuesIfNonNegative(0x10000, 0x10000, 0x10000);
	Engine_EventWait(60);
	Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 2, 0);
	Actor_ShowEmote(GERALD, EMOTE_IN_FRONT | 2, 0);
	Engine_EventWait(80);
	Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x6666, 0x3333);
	Actor_SetSpeed(GERALD, 0x6666, 0x3333);
	Actor_WalkTo(GERALD, 220, 150);
	Engine_ActorWalkToAndWait(ACTOR_PARTY_LEADER, 246, 150);
	Engine_ActorSetAnimation(GERALD, ANIM_STAND);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
	Actor_FaceDirection(GERALD, FACING_SOUTHWEST + FACING_STEP, 0);
	Engine_EventWait(60);
	Engine_ActorRunRepeatedMotion(GERALD, 2);
	Engine_EventWait(10);
	Actor_FaceDirection(GERALD, FACING_SOUTHEAST + FACING_STEP, 0);
	Engine_EventWait(40);
	Event_OpenMessage(GERALD, 0);
	if (Engine_EventChooseYesNo(0, 0) == 0)
	{
		Actor_FaceDirection(GERALD, FACING_EAST + FACING_STEP, 0);
		Engine_EventWait(10);
		Engine_ActorSetAnimationAndWait(GERALD, ANIM_NOD);
	}
	else
	{
		Actor_FaceDirection(GERALD, FACING_EAST + FACING_STEP, 0);
		Engine_EventWait(10);
		Engine_ActorSetAnimation(GERALD, ANIM_SHAKE_HEAD);
		Engine_EventSetMessage((s32)MsgSoruQuitActingTough);
	}
	Engine_EventWait(20);
	Event_ShowMessage(GERALD, 0);

	Actor_SetPosition(ACTOR_WISE_ONE, PIXELS(110), PIXELS(152));
	Engine_TaskWait(1);
	Actor_SetSpeed(ACTOR_WISE_ONE, 0x13333, 0x9999);
	Actor_SetDestination(ACTOR_WISE_ONE, 171, 152);
	Actor_FaceDirection(ACTOR_WISE_ONE, FACING_EAST, 0);
	Engine_ActorSetSpriteFlags(Actor_Get(ACTOR_WISE_ONE), 1);
	Actor_SetSpeed(GERALD, 0x10000, 0x8000);
	Actor_WalkTo(GERALD, 217, 182);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST + FACING_STEP, 0);
	Engine_EventWait(10);
	Camera_SetSpeed(0x20000, 0x4000);
	Camera_MoveTo(PIXELS(217), -1, PIXELS(176), 1);
	Engine_CameraWaitForMove();
	Engine_EventWait(20);
	Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 2);
	Engine_EventWait(20);
	Engine_ActorJump(ACTOR_PARTY_LEADER, 2, 0);
	Engine_EventWait(10);
	Engine_ActorJump(ACTOR_PARTY_LEADER, 4, 0);
	Engine_EventWait(30);
	Actor_ShowEmote(GERALD, EMOTE_IN_FRONT, 0);
	Engine_ActorSetAnimation(GERALD, ANIM_STAND);
	Engine_EventWait(40);
	Actor_FaceDirection(GERALD, FACING_SOUTHEAST, 0);
	Engine_EventWait(4);
	Engine_ActorWalkToAndWait(GERALD, 231, 175);
	Actor_FaceDirection(GERALD, FACING_NORTHEAST, 0);
	Engine_EventSetMessage((s32)MsgSoruOverHere);
	Event_ShowMessage(GERALD, 0);
	Engine_EventWait(40);
	Actor_ShowEmote(GERALD, EMOTE_IN_FRONT | 1, 0);
	Engine_EventWait(40);
	Actor_FaceDirection(GERALD, FACING_NORTHWEST, 0);
	Engine_EventWait(60);
	Engine_ActorStartRepeatedMotion(GERALD, 1);
	Actor_SetAttachedEffect(GERALD, EMOTE_IN_FRONT | 2);
	Engine_EventWait(40);

	FieldScene_RunVariantStep(1, 20, 0);
	quake->fading = 0;
	Work_SetValuesIfNonNegative(0x10000, 0x10000, 0x10000);
	Engine_EventWait(40);
	Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
	Engine_ActorStartRepeatedMotion(GERALD, 2);
	Event_PlayCue(0x121);
	ColorBuffer_ApplyTarget(0x10000, 1);
	Engine_ColorBufferInterpolate(40);
	quake->fading = 1;
	Work_SetValuesIfNonNegative(-1, -1, 0xe666);
	Engine_EventWait(120);
	Event_ShowMessage(ACTOR_WISE_ONE, 0);
	Engine_EventWait(20);
	Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 2, 0);
	Actor_ShowEmote(GERALD, EMOTE_IN_FRONT | 2, 0);
	Engine_EventWait(100);
	Event_ShowMessage(ACTOR_WISE_ONE, 0);
	Engine_EventWait(20);
	FieldScene_RunVariantStep(1, 10, 0);
	quake->fading = 0;
	Work_SetValuesIfNonNegative(0x20000, 0x20000, 0x10000);
	Engine_EventWait(20);
	Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x20000, 0x10000);
	Actor_SetSpeed(GERALD, 0x20000, 0x10000);
	Actor_Get(ACTOR_PARTY_LEADER)->steering_flags &= ~ACTOR_FACES_MOTION;
	Actor_Get(GERALD)->steering_flags &= ~ACTOR_FACES_MOTION;
	Engine_ActorJump(ACTOR_PARTY_LEADER, 4, 0);
	Engine_ActorJump(GERALD, 4, 0);
	Actor_SetDestination(ACTOR_PARTY_LEADER, 256, 150);
	Actor_SetDestination(GERALD, 231, 180);
	Engine_ActorWaitForMove(GERALD);
	FieldScene_RunVariantStep(0, 40, 0);
	Engine_EventWait(20);
	Actor_Get(ACTOR_PARTY_LEADER)->steering_flags |= ACTOR_FACES_MOTION;
	Actor_Get(GERALD)->steering_flags |= ACTOR_FACES_MOTION;
	Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
	Engine_ActorRunRepeatedMotion(GERALD, 2);
	Engine_EventWait(40);
	Actor_ShowEmote(GERALD, EMOTE_IN_FRONT | 3, 0);
	Engine_EventWait(40);
	Event_ShowMessage(GERALD, 0);
	Engine_EventWait(20);
	Actor_ShowEmote(ACTOR_WISE_ONE, EMOTE_IN_FRONT | 1, 0);
	Engine_EventWait(60);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH + FACING_STEP, 0);
	Actor_FaceDirection(GERALD, FACING_NORTH + FACING_STEP, 0);
	Engine_EventWait(20);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
	Actor_FaceDirection(GERALD, FACING_NORTHWEST + FACING_STEP, 0);
	Engine_EventWait(10);

	Event_PlayCue(107);
	FieldScheduler_AddCallback(FieldScene_RunRandomHalfBranch, TASK_PRIORITY_SCENE);
	Engine_EventWait(10);
	Actor_ShowEmote(ACTOR_WISE_ONE, EMOTE_IN_FRONT, 0);
	Engine_EventWait(40);
	Camera_SetSpeed(0x10000, 0x2000);
	Camera_MoveTo(PIXELS(186), -1, PIXELS(166), 1);
	Actor_SetDestination(ACTOR_WISE_ONE, 130, 113);
	Engine_ActorWaitForMove(ACTOR_WISE_ONE);
	Actor_FaceDirection(ACTOR_WISE_ONE, FACING_SOUTHEAST + FACING_STEP, 0);
	Engine_EventWait(20);
	quake->fading = 0;
	Work_SetValuesIfNonNegative(0x20000, 0x20000, 0x10000);
	ColorBuffer_ApplyTarget(0x20119e, 1);
	Engine_ColorBufferInterpolate(20);
	Engine_EventWait(20);
	SceneState_SetValue140Mode0();
	Engine_ActorSetAnimationAndWait(ACTOR_WISE_ONE, 2);
	for (cnt = 0; cnt < 40; cnt++)
	{
		FieldScene_RunStepByRuntimeBits((s32)wise_one);
		Engine_TaskWait(1);
	}
	FieldScheduler_AddCallback(FieldScene_RunActor15TwoStep, TASK_PRIORITY_SCENE);
	ColorBuffer_ApplyTarget(0x10000, 1);
	Engine_ColorBufferInterpolate(60);
	Engine_EventWait(30);
	Event_PlayCue(0x121);
	Engine_TaskRemoveCallback(FieldScene_RunRandomHalfBranch);
	Work_SetValuesIfNonNegative(0x10000, 0x10000, 0x10000);
	wait = 16;
	while (wait--)
	{
		SceneState_ApplyRectsByCondition(0);
		Engine_TaskWait(wait);
		SceneState_ApplyRectsByCondition(1);
		Engine_TaskWait(wait);
	}
	SceneState_ApplyRectsByCondition(0);
	quake->fading = 1;
	Work_SetValuesIfNonNegative(-1, -1, 0xe666);
	Engine_MapRenderWaitForValues();
	Engine_ActorSetAnimationAndWait(ACTOR_WISE_ONE, 3);
	Engine_TaskRemoveCallback(FieldScene_RunActor15TwoStep);
	Engine_TaskWait(1);
	Actor_SetChildValue(ACTOR_WISE_ONE, 0);
	Engine_EventWait(60);
	Event_ShowMessage(ACTOR_WISE_ONE, 0);
	Engine_EventWait(40);
	Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 2, 0);
	Actor_ShowEmote(GERALD, EMOTE_IN_FRONT | 2, 0);
	Engine_EventWait(60);
	Camera_SetSpeed(0x10000, 0x2000);
	Camera_MoveTo(PIXELS(218), -1, PIXELS(181), 1);
	Actor_SetSpeed(ACTOR_WISE_ONE, 0x10000, 0x8000);
	Actor_SetDestination(ACTOR_WISE_ONE, 169, 151);
	Engine_ActorWaitForMove(ACTOR_WISE_ONE);
	Engine_EventWait(10);
	Actor_FaceDirection(ACTOR_WISE_ONE, FACING_EAST + FACING_STEP, 0);
	Engine_EventWait(40);
	Event_ShowMessage(ACTOR_WISE_ONE, 0);
	Engine_EventWait(20);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH + FACING_STEP, 0);
	Actor_FaceDirection(GERALD, FACING_NORTH + FACING_STEP, 0);
	Engine_EventWait(40);
	Actor_FaceDirection(ACTOR_WISE_ONE, FACING_NORTHEAST + FACING_STEP, 0);
	Engine_EventWait(10);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST + FACING_STEP, 0);
	Actor_FaceDirection(GERALD, FACING_NORTHWEST + FACING_STEP, 0);
	Camera_MoveTo(PIXELS(224), -1, PIXELS(158), 1);
	Engine_CameraWaitForMove();
	FieldScene_RunFourWayEffectSequence(0);
	Engine_ActorRunRepeatedMotion(ACTOR_WISE_ONE, 2);
	Engine_EventWait(20);
	Event_ShowMessage(ACTOR_WISE_ONE, 0);
	Engine_EventWait(40);
	Event_OpenMessage(ACTOR_WISE_ONE, 0);
	if (Engine_EventChooseYesNo(0, 0) == 0)
	{
		Engine_EventWait(40);
	}
	else
	{
		Engine_MessageShowCentered((s32)MsgSoruCannotResist, 1);
		Engine_EventWait(40);
	}

	actor = Actor_Get(ACTOR_PARTY_LEADER);
	star = Object_Create(
		OBJECT_ELEMENTAL_STAR, actor->x.fixed, actor->y.fixed + PIXELS(36), actor->z.fixed);
	if (star != NULL)
	{
		buf = Engine_HeapAllocate(17, 0x608);
		sprite = star->sprite;
		sprite->flags = 0;
		sprite->part_count = 0;
		sprite->full_color = 0;
		sprite->palette = 0;
		sprite->priority = 1;
		Engine_ItemLoadIcon(ITEM_MARS_STAR);
		Engine_VramLoad(sprite->vram_block, 128, buf + 0x400);
		Engine_HeapRelease(17);
		Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 28);
		Engine_RunRisingObjectSequence(star, 3);
		Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 28);
	}
	FieldScene_RunVariantStep(1, 20, 0);
	for (cnt = 0; cnt < 24; cnt++)
	{
		FieldScene_RunStepByRuntimeBits((s32)wise_one);
		Engine_TaskWait(1);
		FieldScene_RunStepByRuntimeBits((s32)wise_one);
		Engine_TaskWait(1);
		star->scale_x = 0x6666;
		star->scale_y = 0x6666;
		FieldScene_RunStepByRuntimeBits((s32)wise_one);
		Engine_TaskWait(1);
		FieldScene_RunStepByRuntimeBits((s32)wise_one);
		Engine_TaskWait(1);
		star->scale_x = 0x10000;
		star->scale_y = 0x10000;
	}
	Actor_SetChildValue(ACTOR_WISE_ONE, 0);
	FieldScene_RunVariantStep(0, 20, 0);
	Engine_EventSetMessage((s32)MsgSoruReturnStarToBag);
	Event_ShowMessageAndWait(ACTOR_WISE_ONE, 0, 20);
	if (star != NULL)
	{
		Engine_ObjectDispatchRelease(star);
	}
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
	Engine_EventWait(20);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
	Actor_FaceDirection(GERALD, FACING_NORTHWEST, 60);
	Event_ShowMessage(ACTOR_WISE_ONE, 0);
	Engine_EventWait(40);

	Actor_FaceDirection(ACTOR_WISE_ONE, FACING_SOUTH, 0);
	Camera_SetSpeed(0x40000, 0x8000);
	SceneActor_PlaceAtTileAndRunSteps(232, 464);
	FieldScene_RunFourWayEffectSequence(1);
	Event_ShowMessage(ACTOR_WISE_ONE, 0);
	SceneActor_PlaceAtTileAndRunSteps(711, 144);
	FieldScene_RunFourWayEffectSequence(2);
	Event_ShowMessage(ACTOR_WISE_ONE, 0);
	SceneActor_PlaceAtTileAndRunSteps(711, 464);
	FieldScene_RunFourWayEffectSequence(3);
	Event_ShowMessage(ACTOR_WISE_ONE, 0);
	Actor_FaceDirection(ACTOR_WISE_ONE, FACING_EAST + FACING_STEP, 0);
	Actor_SetPosition(GERALD, PIXELS(582), PIXELS(345));
	Engine_EventWait(20);
	Event_ShowMessage(GERALD, 0);
	Actor_SetPosition(GERALD, PIXELS(231), PIXELS(180));
	Actor_FaceDirection(GERALD, FACING_NORTHWEST + FACING_STEP, 0);
	Engine_TaskWait(20);
	SceneActor_PlaceAtTileAndRunSteps(219, 171);
	Event_ShowMessage(ACTOR_WISE_ONE, 0);
	Engine_EventWait(10);
	Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
	Engine_ActorRunRepeatedMotion(GERALD, 2);
	Event_ShowMessage(ACTOR_WISE_ONE, 0);
	Engine_EventWait(20);
	Event_ShowMessage(ACTOR_WISE_ONE, 0);
	Engine_EventWait(40);

	quake->fading = 0;
	Work_SetValuesIfNonNegative(0x40000, 0x40000, 0x10000);
	ColorBuffer_ApplyTarget(0x20119e, 1);
	Engine_ColorBufferInterpolate(20);
	Engine_EventWait(20);
	Event_PlayCue(107);
	FieldScheduler_AddCallback(FieldScene_RunLateRandomHalfBranch, TASK_PRIORITY_SCENE);
	Engine_EventWait(20);
	Camera_SetSpeed(0x10000, 0x2000);
	Camera_MoveTo(PIXELS(184), -1, PIXELS(132), 1);
	Engine_ActorJump(ACTOR_PARTY_LEADER, 6, 0);
	Engine_ActorJump(GERALD, 6, 0);
	Actor_Get(ACTOR_PARTY_LEADER)->steering_flags &= ~ACTOR_FACES_MOTION;
	Actor_Get(GERALD)->steering_flags &= ~ACTOR_FACES_MOTION;
	Actor_SetDestination(ACTOR_PARTY_LEADER, 245, 145);
	Actor_SetDestination(GERALD, 215, 168);
	Engine_ActorWaitForMove(GERALD);
	Engine_EventWait(80);
	Actor_Get(ACTOR_PARTY_LEADER)->steering_flags |= ACTOR_FACES_MOTION;
	Actor_Get(GERALD)->steering_flags |= ACTOR_FACES_MOTION;
	Actor_SetDestination(ACTOR_WISE_ONE, 184, 87);
	Engine_ActorWaitForMove(ACTOR_WISE_ONE);
	Actor_FaceDirection(ACTOR_WISE_ONE, FACING_SOUTH, 0);
	Engine_EventWait(20);
	Engine_ActorSetAnimationAndWait(ACTOR_WISE_ONE, 2);
	for (cnt = 0; cnt < 40; cnt++)
	{
		FieldScene_RunStepByRuntimeBits((s32)wise_one);
		Engine_TaskWait(1);
	}
	FieldScheduler_AddCallback(FieldScene_RunActor15TwoStep, TASK_PRIORITY_SCENE);
	ColorBuffer_ApplyTarget(0x10000, 1);
	Engine_ColorBufferInterpolate(60);
	Engine_EventWait(30);
	Event_PlayCue(0x121);
	Engine_TaskRemoveCallback(FieldScene_RunLateRandomHalfBranch);
	Work_SetValuesIfNonNegative(0x20000, 0x20000, 0x10000);
	wait = 8;
	while (wait--)
	{
		SceneState_ApplyRectPairByFlag(0);
		Engine_TaskWait(wait);
		SceneState_ApplyRectPairByFlag(1);
		Engine_TaskWait(wait);
	}
	SceneState_ApplyRectPairByFlag(0);
	Work_SetValuesIfNonNegative(0x10000, 0x10000, 0x10000);
	Engine_ActorSetAnimationAndWait(ACTOR_WISE_ONE, 3);
	Engine_TaskRemoveCallback(FieldScene_RunActor15TwoStep);
	Engine_TaskWait(1);
	Actor_SetChildValue(ACTOR_WISE_ONE, 0);
	Engine_EventWait(60);

	Work_SetValuesIfNonNegative(0x40000, 0x40000, 0x10000);
	ColorBuffer_ApplyTarget(0x20119e, 1);
	Engine_ColorBufferInterpolate(20);
	Engine_EventWait(20);
	Event_PlayCue(107);
	FieldScheduler_AddCallback(FieldScene_RunRandomHalfBranch, TASK_PRIORITY_SCENE);
	Engine_EventWait(40);
	Engine_ObjectMotionSetPositionAndCommit(ACTOR_WISE_ONE, 127, 110);
	Actor_FaceDirection(ACTOR_WISE_ONE, FACING_SOUTH, 0);
	Engine_EventWait(20);
	Engine_ActorSetAnimationAndWait(ACTOR_WISE_ONE, 2);
	for (cnt = 0; cnt < 40; cnt++)
	{
		FieldScene_RunStepByRuntimeBits((s32)wise_one);
		Engine_TaskWait(1);
	}
	FieldScheduler_AddCallback(FieldScene_RunActor15TwoStep, TASK_PRIORITY_SCENE);
	ColorBuffer_ApplyTarget(0x10000, 1);
	Engine_ColorBufferInterpolate(60);
	Event_PlayCue(0x121);
	Engine_EventWait(30);
	Engine_TaskRemoveCallback(FieldScene_RunRandomHalfBranch);
	Work_SetValuesIfNonNegative(0x20000, 0x20000, 0x10000);
	wait = 8;
	while (wait--)
	{
		SceneState_ApplyRectsByCondition(0);
		Engine_TaskWait(wait);
		SceneState_ApplyRectsByCondition(1);
		Engine_TaskWait(wait);
	}
	SceneState_ApplyRectsByCondition(0);
	Work_SetValuesIfNonNegative(0x10000, 0x10000, 0x10000);
	Event_PlayCue(107);
	Event_PlayCue(63);
	Work_SetValuesIfNonNegative(0x40000, 0x40000, 0x10000);
	ColorBuffer_ApplyTarget(0x20119e, 1);
	Engine_ColorBufferInterpolate(20);
	Engine_EventWait(20);
	Event_PlayCue(107);
	FieldScheduler_AddCallback(FieldScene_RunLateRandomHalfBranch, TASK_PRIORITY_SCENE);
	Engine_ActorSetAnimationAndWait(ACTOR_WISE_ONE, 3);
	Engine_TaskRemoveCallback(FieldScene_RunActor15TwoStep);
	Engine_TaskWait(1);
	Actor_SetChildValue(ACTOR_WISE_ONE, 0);
	Engine_EventWait(60);
	Engine_ObjectMotionSetPositionAndCommit(ACTOR_WISE_ONE, 184, 87);
	Actor_FaceDirection(ACTOR_WISE_ONE, FACING_SOUTH, 0);
	Engine_EventWait(10);
	Engine_ActorSetAnimationAndWait(ACTOR_WISE_ONE, 3);
	Engine_TaskRemoveCallback(FieldScene_RunActor15TwoStep);
	Engine_TaskWait(1);
	Actor_SetChildValue(ACTOR_WISE_ONE, 0);
	Event_PlayCue(141);
	Engine_EventWait(100);
	Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 2, 0);
	Actor_ShowEmote(GERALD, EMOTE_IN_FRONT | 2, 0);
	Engine_EventWait(60);
	Event_ShowMessage(ACTOR_WISE_ONE, 0);
	Engine_ActorRunRepeatedMotion(GERALD, 3);
	Event_ShowMessage(GERALD, 0);
	Engine_EventWait(20);
	Event_PlayCue(0x121);
	Actor_FaceDirection(ACTOR_WISE_ONE, FACING_SOUTHEAST + FACING_STEP, 0);
	Engine_EventWait(20);
	Event_ShowMessage(ACTOR_WISE_ONE, 0);
	Engine_EventWait(20);
	for (cnt = 0; cnt < 40; cnt++)
	{
		FieldScene_RunStepByRuntimeBits((s32)wise_one);
		Engine_TaskWait(1);
	}
	FieldScheduler_AddCallback(FieldScene_RunActor15TwoStep, TASK_PRIORITY_SCENE);
	Engine_EventWait(20);
	ColorBuffer_ApplyTarget(0x7fff, 2);
	Engine_ColorBufferInterpolate(60);
	Engine_TaskWait(100);
	ColorBuffer_ApplyTarget(0x7fff, 1);
	Engine_ColorBufferInterpolate(60);
	Engine_TaskWait(60);
	Engine_TaskRemoveCallback(FieldScene_RunActor15TwoStep);
	quake->fading = 1;
	Work_SetValuesIfNonNegative(-1, -1, 0xe666);
	Engine_MapRenderWaitForValues();
	SceneEffect_CleanupSceneObjects();
	GameFlag_Set(FLAG_SOL_SANCTUM_ERUPTED);
	GameFlag_Set(FLAG_STAR_ROOM_COLLAPSED);
	Engine_EventRequestExit(5);
	GameFlag_Set(0x100);
}

static void SoruFunka_PlaceBeforeStar(s32 actor)
{
	struct FieldActor *object;

	object = Object_GetById(actor);
	Engine_ActorSetPosition(actor, PIXELS(232), PIXELS(125));
	object->facing = FACING_SOUTH;
	Engine_ActorSetSpritePriority(actor, 3);
}

static void SceneState_ApplyRectsByCondition(s32 a)
{
	if (a != 0)
	{
		s32 x;
		x = 1;
		Map_CopyCellsTo(8, 47, 64, 7, x, x);
		Map_CopyCellsTo(7, 48, 63, 8, 2, x);
		Map_CopyCellsTo(7, 49, 63, 9, 2, x);
	}
	else
	{
		s32 x;
		x = 1;
		Map_CopyCellsTo(56, 0, 64, 7, x, x);
		Map_CopyCellsTo(56, 0, 63, 8, x, x);
		Map_CopyCellsTo(56, 0, 63, 9, 2, x);
		Map_CopyCellsTo(58, 25, 64, 8, x, x);
	}
	Engine_MapRedraw();
}

static void SceneState_ApplyRectPairByFlag(s32 a)
{
	if (a != 0)
	{
		s32 n;
		n = 2;
		Map_CopyCellsTo(9, 45, 65, 5, n, n);
		Map_CopyCellsTo(11, 46, 67, 6, 1, n);
	}
	else
	{
		s32 n;
		n = 2;
		Map_CopyCellsTo(89, 2, 65, 5, n, n);
		Map_CopyCellsTo(102, 32, 67, 6, 1, n);
	}
	Engine_MapRedraw();
}

static void FieldScene_RunRandomHalfBranch(void)
{
	if ((gFrameCount & 1) == 0)
	{
		if ((u32)Math_RemainderUnsigned(Engine_RandomNext(), 100) > 50)
		{
			SceneState_ApplyRectsByCondition(1);
		}
		else
		{
			SceneState_ApplyRectsByCondition(0);
		}
	}
}

static void FieldScene_RunLateRandomHalfBranch(void)
{
	if ((gFrameCount & 1) == 0)
	{
		if ((u32)Math_RemainderUnsigned(Engine_RandomNext(), 100) > 50)
		{
			SceneState_ApplyRectPairByFlag(1);
		}
		else
		{
			SceneState_ApplyRectPairByFlag(0);
		}
	}
}

static void FieldScene_RunFourWayEffectSequence(u32 mode)
{
	extern u8 *gArcEffects[];
	void	   Actor_ShowEmote(s32, s32, s32);
	void	   ColorBuffer_ApplyTarget(s32, s32);
	void	   Event_PlayCue(s32);

	u32	 i, zero;
	s32	 x, y, z;
	s32 *pos;
	u8	*obj, *sprite;

	for (i = 0; i <= 15; i++)
		Engine_ActorDestroy(i + 16);
	switch (mode)
	{
	case 0:
		ColorBuffer_ApplyTarget(0x4039d2, 1);
		break;
	case 1:
		ColorBuffer_ApplyTarget(0x4049d2, 1);
		break;
	case 2:
		ColorBuffer_ApplyTarget(0x404a4e, 1);
		break;
	case 3:
		ColorBuffer_ApplyTarget(0x403a52, 1);
		break;
	}
	Engine_ColorBufferInterpolate(60);
	Event_PlayCue(214);
	i = 0;
	zero = i;
	for (pos = &Funka_ArcOrigins[0]; i <= 9; i++, pos += 2)
	{
		x = pos[0];
		y = pos[1];
		z = 0;
		switch (mode)
		{
		case 0:
			x += 0xe80000;
			z = 0x900000;
			break;
		case 1:
			x += 0xe80000;
			z = 0x1d00000;
			break;
		case 2:
			x += 0x2c70000;
			z = 0x900000;
			break;
		case 3:
			x += 0x2c70000;
			z = 0x1d00000;
			break;
		}
		gArcEffectTimers[i] = zero;
		obj = (u8 *)Engine_ObjectCreate(284, x, y, z);
		gArcEffects[i] = obj;
		obj[85] = zero;
		sprite = *(u8 **)(obj + 80);
		sprite[38] = zero;
		((SpriteMode *)sprite)->mode = 1;
		Object_SetMode(obj, 6);
		Engine_TaskWait(6);
	}
	if (mode == 0)
	{
		Actor_ShowEmote(ACTOR_PARTY_LEADER, 256, 0);
		Actor_ShowEmote(GERALD, 256, 0);
	}
	Engine_TaskWait(20);
	FieldScheduler_AddCallback(SceneEffect_AdvanceTenEntryTimers, 3200);
	Event_PlayCue(246);
	gArcEffectTimers[0] = 1;
	Engine_TaskWait(6);
	gArcEffectTimers[1] = 1;
	Engine_TaskWait(6);
	gArcEffectTimers[2] = 1;
	Engine_TaskWait(6);
	gArcEffectTimers[3] = 1;
	Engine_TaskWait(6);
	gArcEffectTimers[4] = 1;
	Engine_TaskWait(6);
	gArcEffectTimers[5] = 1;
	Engine_TaskWait(6);
	gArcEffectTimers[6] = 1;
	Engine_TaskWait(6);
	gArcEffectTimers[7] = 1;
	Engine_TaskWait(6);
	gArcEffectTimers[8] = 1;
	Engine_TaskWait(6);
	gArcEffectTimers[9] = 1;
	Engine_TaskWait(6);
	for (;;)
	{
		for (i = 0; i <= 9; i++)
		{
			if (gArcEffectTimers[i] != 0)
			{
				i = 888;
				break;
			}
		}
		if (i != 888)
			break;
		Engine_TaskWait(1);
	}
	Engine_TaskWait(40);
	Engine_TaskRemoveCallback(SceneEffect_AdvanceTenEntryTimers);
	ColorBuffer_ApplyTarget(65536, 1);
	Engine_ColorBufferInterpolate(40);
}

static void SceneEffect_AdvanceTenEntryTimers(void)
{
#ifdef MODERN
	// modern GCC holds one type for a name in a file
	extern Ent *gArcEffects_modern[] __asm__("gArcEffects");
#define gArcEffects gArcEffects_modern
#else
	extern Ent *gArcEffects[];
#endif

	u32			  i;
	s32			  v;
	ArcEffect *p;

	for (i = 0; i <= 9; i++)
	{
		v = gArcEffectTimers[i];
		if (v != 0)
		{
			p = gArcEffects[i];
			if ((u32)v <= 8)
			{
				p->unk18 += -0x1ccc;
				p->unk1C += 0x8000;
				p->unkC += 0x4ccc;
				p->unk3C += 0x4ccc;
			}
			else
			{
				p->unkC += 0x140000;
				p->unk3C += 0x140000;
			}
			v = gArcEffectTimers[i] + 1;
			gArcEffectTimers[i] = v;
			if ((u32)v > 14)
			{
				gArcEffectTimers[i] = 0;
			}
		}
	}
}
#ifdef MODERN
#undef gArcEffects
#endif

static void SceneActor_PlaceAtTileAndRunSteps(s32 a, s32 b)
{
	ViewCenter *p;

	p = (ViewCenter *)Engine_EventGetViewCenter();
	b <<= 16;
	a <<= 16;
	Engine_CameraMoveTo(a, -1, b, 1);
	Engine_ColorBufferApplyTarget(0, 0);
	Engine_ColorBufferInterpolate(20);
	Engine_TaskWait(40);
	p->unk8 = a;
	p->unk10 = b;
	p->unk38 = 0x80000000;
	p->unk40 = 0x80000000;
	p->unk24 = 0;
	p->unk2C = 0;
	Engine_TaskWait(5);
	Engine_MapRedraw();
	Engine_TaskWait(5);
	Engine_ColorBufferApplyTarget(0x10000, 0);
	Engine_ColorBufferInterpolate(20);
	Engine_TaskWait(30);
}

static void SceneState_ConfigureEightCornerRegions(void)
{

	Map_CopyCellAttributes(14, 8, 1, 1, 10, 0);
	Map_CopyCellAttributes(14, 28, 1, 1, 11, 0);
	Map_CopyCellAttributes(44, 8, 1, 1, 12, 0);
	Map_CopyCellAttributes(44, 28, 1, 1, 13, 0);
	Map_CopyCellAttributes(13, 8, 1, 1, 14, 8);
	Map_CopyCellAttributes(13, 28, 1, 1, 14, 28);
	Map_CopyCellAttributes(43, 8, 1, 1, 44, 8);
	Map_CopyCellAttributes(43, 28, 1, 1, 44, 28);
}

static void FieldScene_RunVariantStep(s32 a, s32 b, s32 c)
{
	if (a == 1)
	{
		Engine_AudioPlayCue(0x134);
		Engine_ColorBufferApplyTarget(0x203a52, 1);
	}
	else
	{
		Engine_AudioPlayCue(0x121);
		Engine_ColorBufferApplyTarget(0x10000, 1);
	}
	Engine_ColorBufferInterpolate(b);
	if (c != 0)
	{
		Engine_EventWait(c);
	}
}

static void FieldScene_RunStepByRuntimeBits(s32 a)
{
	if ((gFrameCount & 2) != 0)
	{
		Engine_ObjectSetPartPalettes(a, 7);
	}
	else
	{
		Engine_ObjectSetPartPalettes(a, 0);
	}
	if ((gFrameCount & 15) == 0)
	{
		SoruFunka_SpawnEffectPair(a);
	}
}

static void SceneState_ForwardByRuntimeWordBits(s32 a)
{
	if (gFrameCount & 1)
	{
		Engine_ObjectSetPartPalettes(a, Math_RemainderUnsigned(gFrameCount >> 1, 6));
	}
	if ((gFrameCount & 15) == 0)
	{
		SoruFunka_SpawnEffectPair(a);
	}
}

// ここに共通ソースが入る（ROM の並びのとおり）
#include "common/effect_vertical.c"

static void SoruFunka_SpawnEffectPair(union PairObject *parent)
{
	union PairObject   *pair[2];
	union PairObject   *child;
	struct PairSprite  *part;
	struct FieldSprite *sprite;
	struct PairWork	   *work = gEffectWork;
	s32					i;

	Engine_AudioPlayCue(292);
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
	pair[0]->object.actor.update = (void (*)(union FieldObject *))Effect_AnimateVerticalNegative;
	pair[0]->object.actor.sprite->priority = Object_GetById(15)->sprite->priority;
	{
		struct FieldActor *q = Object_GetById(15);
		struct FieldActor *p = &pair[1]->object.actor;

		p->sprite->priority = q->sprite->priority;
		p->update = (void (*)(union FieldObject *))Effect_AnimateVerticalPositive;
		p->priority_flags = 2;
	}
}

// ここに共通ソースが入る（ROM の並びのとおり）
#include "common/effect_psynergy.c"

static void FieldScene_RunActor15TwoStep(void)
{
	SceneState_ForwardByRuntimeWordBits((s32)Object_GetById(15));
}

// ここに共通ソースが入る（ROM の並びのとおり）
#include "common/soru_star_ring_setup.c"

s32 Funka_ArcOriginsHead[] = {
	SC_SET(SW_VELOCITY_Y, 0x60000),
	SC_LABEL(1),
	SC_ADD(SW_SCALE_X, 2621),
	SC_ADD(SW_SCALE_Y, 2621),
	SC_WAIT(1),
	SC_LOOP(18, SC_TO(1)),
	SC_SET(SW_SCALE_X, 0x10000),
	SC_SET(SW_SCALE_Y, 0x10000),
	SC_END,
};

s32 Funka_ArcOrigins[] = {
	PIXELS(-6), 0,
	PIXELS(10), PIXELS(10),
	PIXELS(8), PIXELS(25),
	PIXELS(4), PIXELS(30),
	PIXELS(-5), PIXELS(20),
	PIXELS(2), PIXELS(5),
	PIXELS(-6), PIXELS(35),
	PIXELS(-8), PIXELS(15),
	PIXELS(2), PIXELS(40),
	PIXELS(-2), PIXELS(15),
};

struct SceneEntrance Placement_Scripts[] = {
	{ 0, CONDITION_ALWAYS, 472, 0, 322, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ SCENE_TABLE_END },
};

u32 Placement_Messages[] = {
	SCENE_EXITS(18),
	SCENE_EXIT(5, 11, 8),
	SCENE_EXITS_END,
};

struct ScenePlacement Placement_Actors[] = {
	{ GERALD, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ JASMINE, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x16, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
	{ 0x1e, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x20, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x21, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x22, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x23, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x2b, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0xfc, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0xd8, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
	{ 0xd8, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
	{ 0xd8, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
	{ 0xd8, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
	{ 0xd8, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
	{ 0xd8, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
	{ 0xd8, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
	{ 0xd8, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
	{ 0xd8, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
	{ 0xd8, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
	{ 0xd8, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
	{ 0xd8, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
	{ 0xd8, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
	{ 0xd8, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
	{ 0xd8, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
	{ 0xd8, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEvent Placement_Effects[] = {
	{ SCENE_EVENTS_END },
};

u16 SoruFunka_StarCells[] = {
	MAP_CELL_STEP(59, 40, 3, 4, 6), MAP_CELL_STEP(62, 40, 3, 4, 6), MAP_CELL_STEP(65, 40, 3, 4, 6), MAP_CELL_STEP(68, 40, 3, 4, 6), MAP_CELL_STEP(71, 40, 3, 4, 6), MAP_CELL_STEP(74, 40, 3, 4, 6), MAP_CELL_STEP(77, 40, 3, 4, 6), MAP_CELL_STEP(80, 40, 3, 4, 6), MAP_CELL_STEP(83, 40, 3, 4, 0), MAP_CELL_STEPS_END,
};

s32 Funka_EmberScript[] = {
	SC_CALL(OverlayObject_SetRecordAngleFromHeading),
	SC_END,
};

u8 Soru_RingDrift[][3] = {
	{ 4, 4, 4 },
	{ 4, 0, 3 },
	{ 4, 0, 4 },
	{ 4, 0, 4 },
	{ 3, 0, 4 },
	{ 4, 6, 4 },
	{ 6, 0, 4 },
	{ 4, 0, 3 },
	{ 2, 0, 1 },
	{ 2, 0, 2 },
};

u8 Soru_RingSwing[][3] = {
	{ 1, 1, 2 },
	{ 1, 0, 2 },
	{ 1, 0, 2 },
	{ 1, 0, 1 },
	{ 1, 0, 2 },
	{ 1, 1, 2 },
	{ 1, 0, 2 },
	{ 1, 0, 2 },
	{ 1, 0, 2 },
	{ 1, 0, 2 },
};

s8 Soru_RingDirection[][3] = {
	{ 1, 1, 1 },
	{ 1, 0, 1 },
	{ 1, 0, 1 },
	{ -1, 0, 1 },
	{ -1, 0, -1 },
	{ 1, 1, 1 },
	{ -1, 0, -1 },
	{ -1, 0, -1 },
	{ -1, 0, -1 },
	{ -1, 0, -1 },
};

s32 Soru_RingOffsetX[] = {
	0x9999, 0xcccc, 0xb333, 0x9999, 0xcccc, 0x9999, 0xb333, 0x9999,
	0x9999, 0xb333,
};

s32 Soru_RingOffsetZ[] = {
	655, 458, 655, 458, 655, 458, 655, 655,
	524, 655,
};
