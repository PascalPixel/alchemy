#include "game_state.h"
#include "unit/import.h"
#include "unit/field/soru_star/star.h"
#include "unit/field/common/effect/move.h"
#include "unit/field/soru_star/ring_setup.h"
#include "unit/battle/effect/object.h"
#include "unit/battle/effect/object2.h"
#include "unit/battle/effect/object3.h"
#include "unit/battle/effect/object4.h"
#include "unit/battle/effect/object5.h"
#include "unit/battle/effect/object6.h"
#include "unit/battle/effect/object7.h"
#include "unit/battle/effect/object8.h"
#include "unit/battle/effect/object9.h"
#include "unit/battle/effect/object10.h"
#include "unit/battle/effect/object11.h"
#include "unit/battle/effect/object12.h"
#include "unit/battle/effect/object13.h"
#include "unit/battle/effect/object14.h"
#include "unit/battle/effect/object15.h"
#include "unit/battle/effect/object16.h"
#include "unit/battle/effect/object17.h"
#include "unit/battle/effect/object18.h"
#include "unit/field/common/script/field.h"
#include "unit/field/common/script/field2.h"
#include "unit/field/common/script/field3.h"
#include "unit/field/common/script/field5.h"
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
#include "unit/graphics/window/window.h"
#include "unit/graphics/window/window9.h"
#include "unit/graphics/window/window10.h"
#include "unit/graphics/window/window11.h"
#include "unit/graphics/window/window12.h"
#include "unit/menu/shop/shop.h"
#include "unit/menu/shop/shop2.h"
#include "unit/menu/shop/shop3.h"
#include "unit/menu/shop/shop4.h"
#include "unit/menu/shop/shop5.h"
#include "unit/menu/shop/shop6.h"
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
#include "fieldobj.h"
#include "types.h"
#include "field_event.h"
#include "field_scene.h"
#include "create_configured_overlay_object.h"
#include "overlay_object.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "battle_unit.h"
#include "call.h"
#include "field_service.h"
#include "text/msg_ids.h"
#include "glyph.h"
#include "ability_ids.h"
#include "objcmd.h"
#include "facing_object.h"

// 前方宣言
static void Scene_BagJupiterStar(void);
static void FieldScene_StagePairedActors(void);
static void FieldScene_RunElementalStarDemand(void);
static void Scene_OfferGuarantee(void);
static void Scene_UnmaskGarcia(void);
static void Scene_AlexTakesStars(void);
static void SoruStar_LineUpFollowers(void);
static void Event_SayThenWait(s32 speaker, s32 frames);
static s32 Scene_PresentItem(s32 item, s32 x, s32 y, s32 z);
static void Scene_GiveMythrilBags(void);
static void SoruStar_RiseActorFourteenSparks(void);

enum StarRoomActor
{
	ACTOR_SUKURETA = ACTOR_FIRST_PLACED + 1,
	ACTOR_SATUROS,
	ACTOR_MENARDI,
	ACTOR_GARCIA_MASKED,
	ACTOR_GARCIA,
	ACTOR_ALEX
};

enum StarRoomFlag
{
	FLAG_STAR_ROOM_EXPLAINED = 0x83b,
	FLAG_FIRST_STAR_BAGGED = 0x83c,
	FLAG_MERCURY_STAR_BAGGED = 0x83d,
	FLAG_STARS_GIVEN_TO_ALEX = 0x83e
};

enum
{
	ITEM_MYTHRIL_BAG = 224
};

#define NULL ((void *)0)

struct ObjectRuntime;

extern struct SceneEvent Placement_Effects[];

static void SetEffectRecordMode(struct FieldActor *work, s32 mode)
{
	work->sprite->priority = mode;
}

static void *OverlayObject_PrepareObject(s32 x, s32 y, s32 z, s32 type)
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

static void *OverlayObject_CreateConfiguredObject(s32 x, s32 y, s32 z, s32 type)
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
#include "../common/effect/spawn.c"

static __inline__ void SkipMessage(s32 amount)
{
	gEventWork->message += amount;
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

s32 *SceneData_GetScriptTable(void)
{
	return (s32 *)Placement_Scripts;
}

s32 SceneData_ReturnZero(void)
{
	return 0;
}

s32 *SceneData_GetMessageTable(void)
{
	return Placement_Messages;
}

s32 *SceneData_GetActorTable(void)
{
	return (s32 *)Placement_Actors;
}

s32 *SceneData_GetEffectTable(void)
{
	return (s32 *)Placement_Effects;
}

static void EV02_SEARCH(void)
{
	u32 i;
	s32 obj;
	s32 mes;

	Engine_EventBegin();
	Engine_AudioPlayCue(141);
	for (i = 0; i != 6; i++)
	{
		Engine_ColorBufferApplyTarget(0x403a52, 1);
		Engine_ColorBufferInterpolate(8);
		Engine_EventWait(8);
		Engine_ColorBufferApplyTarget(0x10000, 1);
		Engine_ColorBufferInterpolate(8);
		Engine_EventWait(8);
		if (i == 1)
		{
			Engine_WorkSetValuesIfNonNegative(0x10000, 0x10000, 0x10000);
		}
	}
	Work_SetValuesIfNonNegative(0x10000, 0x20000, 0x10000);
	Engine_EventWait(30);
	Engine_CameraSetSpeed(0x26666, 0x4ccc);
	Camera_MoveTo(0x2980000, -1, 0x1f10000, 1);
	Engine_CameraWaitForMove();
	Engine_EventWait(20);
	Work_SetValuesIfNonNegative(0x10000, 0x20000, 0x10000);
	Engine_AudioPlayCue(144);
	Engine_MapAnimateCells(SoruStar_StarCells, 96, 29);
	Engine_MapCopyCellAttributes(0, 0, 1, 1, 41, 29);
	Map_CopyCellsTo(87, 42, 41, 31, 1, 2);
	Engine_EventWait(40);
	Engine_WorkSetValuesIfNonNegative(0, 0, 0);
	Engine_CameraSetSpeed(0x66666, 0xcccc);
	Camera_MoveTo(0x1370000, -1, 0x1f10000, 1);
	Engine_CameraWaitForMove();
	Work_SetValuesIfNonNegative(0x10000, 0x20000, 0x10000);
	Engine_EventWait(20);
	Engine_AudioPlayCue(144);
	Engine_MapAnimateCells(SoruStar_StarCells, 74, 29);
	Engine_MapCopyCellAttributes(0, 0, 1, 1, 19, 29);
	Engine_MapCopyCellsTo(87, 42, 19, 31, 1, 2);
	Engine_EventWait(40);
	Engine_WorkSetValuesIfNonNegative(0, 0, 0);
	Camera_MoveTo(0x2970000, -1, 0xc00000, 1);
	Engine_CameraWaitForMove();
	Work_SetValuesIfNonNegative(0x10000, 0x20000, 0x10000);
	Engine_EventWait(20);
	Engine_AudioPlayCue(144);
	Engine_MapAnimateCells(SoruStar_StarCells, 96, 10);
	Engine_MapCopyCellAttributes(0, 0, 1, 1, 41, 10);
	Engine_MapCopyCellsTo(87, 42, 41, 12, 1, 2);
	Engine_EventWait(40);
	gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 2);
	Engine_EventCloseScreen();
	Engine_EventWaitForScreen();
	Camera_MoveTo(0x2c60000, -1, 0x1da0000, 0);
	Engine_MapRedraw();
	Engine_TaskWait(1);
	Work_SetValuesIfNonNegative(0x20000, 0x10000, 0x10000);
	Engine_EventOpenScreen();
	Engine_EventWaitForScreen();
	Engine_EventWait(40);
	Engine_AudioPlayCue(0x121);
	Work_SetValuesIfNonNegative(-1, -1, 0xe666);
	Engine_MapRenderWaitForValues();
	Engine_EventWait(20);
	Engine_MapCopyCellsTo(0, 40, 43, 66, 3, 3);
	Engine_EventWait(20);
	obj = Scene_PresentItem(220, 0x2c80000, 0x100000, 0x1d00000);
	Engine_EventWait(40);
	UiWork_PushValueSlot(obj, 1);
	mes = (s32)MsgSoruVenusStarBagged;
	Engine_MessageShowCentered(mes, 1);
	Actor_FaceDirection(9, FACING_SOUTHEAST, 0);
	Actor_FaceDirection(5, FACING_SOUTHEAST, 20);
	Engine_EventCloseScreen();
	Engine_EventWaitForScreen();
	Camera_MoveTo(0x1ce0000, -1, 0x15e0000, 0);
	Engine_MapRedraw();
	Engine_TaskWait(1);
	Engine_EventOpenScreen();
	Engine_EventWaitForScreen();
	Engine_EventWait(40);
	Engine_ActorJump(9, 4, 30);
	Engine_EventSetMessage(mes - 1);
	Event_SayThenWait(9, 20);
	Engine_EventCloseScreen();
	Engine_EventWaitForScreen();
	Engine_CameraMoveTo(0x2c60000, -1, 0x1da0000, 0);
	Engine_MapRedraw();
	Engine_TaskWait(1);
	Engine_EventOpenScreen();
	Engine_EventWaitForScreen();
	Engine_GameFlagSet(0x83c);
	Engine_EventEnd();
}

static void EV03_SEARCH(void)
{
	u32 i;
	s32 obj;
	s32 mes;

	Engine_EventBegin();
	Event_PlayCue(141);
	for (i = 0; i != 6; i++)
	{
		ColorBuffer_ApplyTarget(0x404a4e, 1);
		Engine_ColorBufferInterpolate(8);
		Engine_EventWait(8);
		ColorBuffer_ApplyTarget(0x10000, 1);
		Engine_ColorBufferInterpolate(8);
		Engine_EventWait(8);
		if (i == 1)
		{
			Work_SetValuesIfNonNegative(0x10000, 0x10000, 0x10000);
		}
	}
	Work_SetValuesIfNonNegative(0x10000, 0x20000, 0x10000);
	Engine_EventWait(30);
	Work_SetValuesIfNonNegative(0, 0, 0);
	Camera_SetSpeed(0x59999, 0xb333);
	Camera_MoveTo(0x1d80000, -1, 0x620000, 1);
	Engine_CameraWaitForMove();
	Work_SetValuesIfNonNegative(0x10000, 0x20000, 0x10000);
	Engine_EventWait(20);
	Event_PlayCue(144);
	Engine_MapAnimateCells(SoruStar_StarCells, 84, 4);
	Map_CopyCellAttributes(0, 0, 1, 1, 29, 4);
	Map_CopyCellsTo(87, 42, 29, 6, 1, 2);
	Engine_EventWait(40);
	Work_SetValuesIfNonNegative(0, 0, 0);
	Camera_MoveTo(0x1570000, -1, 0x1710000, 1);
	Engine_CameraWaitForMove();
	Work_SetValuesIfNonNegative(0x10000, 0x20000, 0x10000);
	Engine_EventWait(20);
	Event_PlayCue(144);
	Engine_MapAnimateCells(SoruStar_StarCells, 76, 21);
	Map_CopyCellAttributes(0, 0, 1, 1, 21, 21);
	Map_CopyCellsTo(87, 42, 21, 23, 1, 2);
	Engine_EventWait(40);
	Work_SetValuesIfNonNegative(0, 0, 0);
	Camera_SetSpeed(0x33333, 0x6666);
	Camera_MoveTo(0x1570000, -1, 0x1f10000, 1);
	Engine_CameraWaitForMove();
	Work_SetValuesIfNonNegative(0x10000, 0x20000, 0x10000);
	Engine_EventWait(20);
	Event_PlayCue(144);
	Engine_MapAnimateCells(SoruStar_StarCells, 76, 29);
	Map_CopyCellAttributes(0, 0, 1, 1, 21, 29);
	Map_CopyCellsTo(87, 42, 21, 31, 1, 2);
	Engine_EventWait(40);
	gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 2);
	Engine_EventCloseScreen();
	Engine_EventWaitForScreen();
	Camera_MoveTo(0x2c80000, -1, 0x980000, 0);
	Engine_MapRedraw();
	Engine_TaskWait(1);
	Work_SetValuesIfNonNegative(0x20000, 0x10000, 0x10000);
	Engine_EventOpenScreen();
	Engine_EventWaitForScreen();
	Engine_EventWait(40);
	Event_PlayCue(0x121);
	Work_SetValuesIfNonNegative(-1, -1, 0xe666);
	Engine_MapRenderWaitForValues();
	Engine_EventWait(20);
	Map_CopyCellsTo(0, 40, 43, 46, 3, 3);
	Engine_EventWait(20);
	obj = Scene_PresentItem(221, 0x2c80000, 0x100000, 0x900000);
	Engine_EventWait(40);
	UiWork_PushValueSlot(obj, 1);
	mes = (s32)MsgSoruPutMercuryStar;
	Engine_MessageShowCentered(mes, 1);
	Actor_FaceDirection(ACTOR_SUKURETA, FACING_NORTHEAST, 0);
	Actor_FaceDirection(JASMINE, FACING_NORTHEAST, 20);
	Engine_EventCloseScreen();
	Engine_EventWaitForScreen();
	Camera_MoveTo(0x1ce0000, -1, 0x15e0000, 0);
	Engine_MapRedraw();
	Engine_TaskWait(1);
	Engine_EventOpenScreen();
	Engine_EventWaitForScreen();
	Engine_EventWait(40);
	Engine_ActorJump(ACTOR_SUKURETA, 4, 30);
	Engine_EventSetMessage(mes - 2);
	Event_SayThenWait(9, 20);
	Engine_EventCloseScreen();
	Engine_EventWaitForScreen();
	Camera_MoveTo(0x2c80000, -1, 0x980000, 0);
	Engine_MapRedraw();
	Engine_TaskWait(1);
	Engine_EventOpenScreen();
	Engine_EventWaitForScreen();
	GameFlag_Set(FLAG_MERCURY_STAR_BAGGED);
	Engine_EventEnd();
}

static void EV04_SEARCH(void)
{
	Engine_EventBegin();
	Scene_BagJupiterStar();
	FieldScene_StagePairedActors();
	FieldScene_RunElementalStarDemand();
	Scene_OfferGuarantee();
	Scene_UnmaskGarcia();
	Scene_AlexTakesStars();
	Engine_GameFlagSet(FLAG_STARS_GIVEN_TO_ALEX);
	Engine_EventEnd();
	SoruStar_LineUpFollowers();
}

static void Scene_BagJupiterStar(void)
{
	u32 i;
	s32 obj;

	Event_PlayCue(141);
	for (i = 0; i != 6; i++)
	{
		ColorBuffer_ApplyTarget(0x4049d2, 1);
		Engine_ColorBufferInterpolate(8);
		Engine_EventWait(8);
		ColorBuffer_ApplyTarget(0x10000, 1);
		Engine_ColorBufferInterpolate(8);
		Engine_EventWait(8);
		if (i == 1)
		{
			Work_SetValuesIfNonNegative(0x10000, 0x10000, 0x10000);
		}
	}
	Work_SetValuesIfNonNegative(0x20000, 0x10000, 0x10000);
	Engine_EventWait(30);
	Camera_SetSpeed(0x26666, 0x4ccc);
	Camera_MoveTo(0xa70000, -1, 0x2110000, 1);
	Engine_CameraWaitForMove();
	Work_SetValuesIfNonNegative(0x10000, 0x20000, 0x10000);
	Engine_EventWait(20);
	Event_PlayCue(144);
	Engine_MapAnimateCells(SoruStar_StarCells, 65, 31);
	Map_CopyCellAttributes(0, 0, 1, 1, 10, 31);
	Map_CopyCellsTo(87, 42, 10, 33, 1, 2);
	Engine_EventWait(40);
	Work_SetValuesIfNonNegative(0, 0, 0);
	Camera_SetSpeed(0x66666, 0xcccc);
	Camera_MoveTo(0x1870000, -1, 0xb10000, 1);
	Engine_CameraWaitForMove();
	Work_SetValuesIfNonNegative(0x10000, 0x20000, 0x10000);
	Engine_EventWait(20);
	Event_PlayCue(144);
	Engine_MapAnimateCells(SoruStar_StarCells, 79, 9);
	Map_CopyCellAttributes(0, 0, 1, 1, 24, 9);
	Map_CopyCellsTo(87, 42, 24, 11, 1, 2);
	Engine_EventWait(40);
	Work_SetValuesIfNonNegative(0, 0, 0);
	Camera_SetSpeed(0x26666, 0x4ccc);
	Camera_MoveTo(0x2470000, -1, 0xc10000, 1);
	Engine_CameraWaitForMove();
	Work_SetValuesIfNonNegative(0x10000, 0x20000, 0x10000);
	Engine_EventWait(20);
	Event_PlayCue(144);
	Engine_MapAnimateCells(SoruStar_StarCells, 91, 10);
	Map_CopyCellAttributes(0, 0, 1, 1, 36, 10);
	Map_CopyCellsTo(87, 42, 36, 12, 1, 2);
	Engine_EventWait(40);
	gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 2);
	Engine_EventCloseScreen();
	Engine_EventWaitForScreen();
	Camera_MoveTo(0xe80000, -1, 0x1dd0000, 0);
	Engine_MapRedraw();
	Engine_TaskWait(1);
	Work_SetValuesIfNonNegative(0x20000, 0x10000, 0x10000);
	Engine_EventOpenScreen();
	Engine_EventWaitForScreen();
	Engine_EventWait(40);
	Event_PlayCue(0x121);
	Work_SetValuesIfNonNegative(-1, -1, 0xe666);
	Engine_MapRenderWaitForValues();
	Engine_EventWait(20);
	Map_CopyCellsTo(0, 40, 13, 66, 3, 3);
	Engine_EventWait(20);
	obj = Scene_PresentItem(223, 0xe80000, 0x100000, 0x1d00000);
	Engine_EventWait(40);
	UiWork_PushValueSlot(obj, 1);
	Engine_MessageShowCentered((s32)MsgSoruJupiterStarBagged, 1);
}

static void FieldScene_StagePairedActors(void)
{
	struct FieldActor *record;

	Event_PlayCue(17);
	Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x8000, 0x4000);
	Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 231, 0x1ea);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 30);
	Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
	Engine_EventWait(180);
	Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
	Engine_EventWait(80);
	Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 60);
	Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x8000, 0x4000);
	Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 246, 0x1df);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHEAST, 10);
	record = Actor_Get(ACTOR_PARTY_LEADER);
	if (record != 0)
	{
		Actor_SetPosition(GERALD, record->x.fixed, record->z.fixed);
	}
	Actor_SetSpeed(GERALD, 0x10000, 0x8000);
	Actor_WalkToAndWait(GERALD, 0x101, 0x1eb);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 0);
	Actor_FaceDirection(GERALD, FACING_NORTHWEST, 40);
	Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
	Engine_ActorRunRepeatedMotion(GERALD, 2);
	Engine_EventWait(20);
	Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 0);
	Actor_ShowEmote(GERALD, 0x101, 80);
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
	Engine_ActorSetAnimationAndWait(GERALD, 4);
	Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x13333, 0x9999);
	Actor_SetSpeed(GERALD, 0x13333, 0x9999);
	Actor_WalkTo(ACTOR_PARTY_LEADER, 0x109, 0x1c5);
	Actor_WalkToAndWait(GERALD, 0x11a, 0x1d5);
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 1);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHEAST, 0);
	Actor_FaceDirection(GERALD, FACING_NORTHEAST, 40);
	Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x100, 0);
	Actor_ShowEmote(GERALD, 0x100, 0);
	Engine_ActorJump(ACTOR_PARTY_LEADER, 6, 0);
	Engine_ActorJump(GERALD, 6, 60);
	Actor_SetPosition(JASMINE, 0x1db0000, 0x14c0000);
	Actor_SetPosition(ACTOR_SUKURETA, 0x1eb0000, 0x14c0000);
	Actor_SetPosition(ACTOR_MENARDI, 0x1cb0000, 0x15c0000);
	Actor_SetPosition(ACTOR_SATUROS, 0x1fb0000, 0x15c0000);
	Camera_SetSpeed(0x73333, 0xe666);
	Camera_MoveTo(0x1e50000, -1, 0x1590000, 1);
	Actor_FaceDirection(JASMINE, FACING_SOUTHWEST, 0);
	Actor_FaceDirection(ACTOR_SUKURETA, 0x5000, 0);
	Actor_FaceDirection(ACTOR_MENARDI, 0x5000, 0);
	Actor_FaceDirection(ACTOR_SATUROS, 0x5000, 0);
	Engine_CameraWaitForMove();
	Engine_EventWait(40);
}

static void FieldScene_RunElementalStarDemand(void)
{
	u8				  *motion_flags;
	u8				  *sprite_flags;
	struct FieldActor *star;
	struct FieldActor *glow;
	s32				   i;
	u8				   zero;

	Engine_AudioPlayCue(61);
	Engine_ActorSetAnimation(10, 4);
	Engine_EventSetMessage((s32)MsgSoruLooksLikeTheyveSpottedUs);
	Event_SayThenWait(10, 10);
	Engine_ActorSetAnimation(11, 4);
	Event_SayThenWait(11, 30);
	Actor_ShowEmote(9, 0x102, 60);
	Engine_ActorJump(9, 4, 10);
	Engine_ActorJump(9, 6, 30);
	Event_SayThenWait(9, 10);
	Engine_ActorRunRepeatedMotion(10, 1);
	Actor_FaceDirection(10, 0xb000, 10);
	Event_SayThenWait(10, 20);
	Engine_ActorRunRepeatedMotion(11, 1);
	Actor_FaceDirection(11, 0xd000, 20);
	Event_SayThenWait(11, 30);
	Actor_ShowEmote(9, 0x102, 60);
	Engine_ActorFaceDirection(5, FACING_EAST, 0);
	Actor_FaceDirection(9, 0x7000, 80);
	Actor_ShowEmote(5, 0x102, 40);
	Event_SayThenWait(5, 20);
	Engine_ActorRunRepeatedMotion(9, 2);
	Engine_ActorSetAnimation(9, 4);
	Event_SayThenWait(9, 10);
	star = Object_GetById(12);
	glow = Object_GetById(8);
	sprite_flags = &star->sprite->flags;
	zero = 0;
	*sprite_flags = zero;
	star->scale_x = 0x1999;
	star->scale_y = 0x1999;
	glow->scale_x = 0x1999;
	glow->scale_y = 0x1999;
	Actor_SetChildValue(12, 0x100);
	Actor_SetPosition(12, 0x1d70000, 0x1220000);
	motion_flags = &star->motion_flags;
	*motion_flags = zero;
	star->y.fixed = 0x280000;
	Engine_EventWait(1);
	Event_SayThenWait(12, 10);
	Actor_ShowEmote(5, 0x100, 0);
	Actor_ShowEmote(9, 0x100, 30);
	Actor_FaceDirection(5, FACING_NORTH, 0);
	Actor_FaceDirection(9, 0xb000, 10);
	Actor_FaceDirection(11, 0xd000, 0);
	Actor_FaceDirection(10, 0xb000, 0);
	Camera_SetSpeed(0x20000, 0x4000);
	Camera_MoveTo(0x1d70000, -1, 0x1350000, 1);
	Engine_CameraWaitForMove();
	Engine_ActorSetPosition(8, 0x1d70000, 0x1220000);
	Event_PlayCue(190);
	Engine_ActorSetSpritePriority(12, 2);
	for (i = 0; i != 90; i++)
	{
		star->y.fixed -= 0x1999;
		star->scale_x += 0x28f;
		star->scale_y += 0x28f;
		glow->scale_x += 0x28f;
		glow->scale_y += 0x28f;
		Engine_EventWait(1);
	}
	*motion_flags = 5;
	Engine_EventWait(80);
	for (i = 0; i != 60; i++)
	{
		star->y.fixed -= 0x8000;
		Engine_EventWait(1);
	}
	*motion_flags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
	Engine_EventWait(30);
	*sprite_flags = 1;
	Engine_ActorSetPosition(8, 0, 0);
	Engine_ActorSetSpritePriority(12, 1);
	{
		u8 *flags = &Object_GetById(12)->priority_flags;

		/* FAKEMATCH: the bit held in the loop counter first keeps the 1 in r5 ahead of the actor 8 call's arguments; a plain |= loads it after. Plain |=: differ 31, one instruction shorter. */
		i = ACTOR_PRIORITY_AUTOMATIC;
		i |= *flags;
		*flags = i;
	}
	Engine_ActorSetChildValue(12, 0);
	Actor_SetSpeed(12, 0x8000, 0x4000);
	Actor_WalkToAndWait(12, 0x1d7, 0x132);
	Engine_EventWait(40);
	Engine_ActorRunRepeatedMotion(12, 2);
	Event_SayThenWait(0x400c, 20);
	Engine_ActorFaceEachOther(5, 9, 0);
	Engine_EventWait(20);
	Engine_ActorStartRepeatedMotion(JASMINE, 2);
	Engine_ActorRunRepeatedMotion(9, 2);
	Engine_EventWait(40);
	Engine_ActorStartRepeatedMotion(10, 1);
	Engine_ActorRunRepeatedMotion(11, 1);
	Engine_EventWait(20);
	Engine_ActorSetAnimationAndWait(10, 4);
	Actor_FaceDirection(10, 0x5000, 0);
	Actor_FaceDirection(11, 0x5000, 10);
	Event_SayThenWait(10, 30);
	Engine_ActorSetAttachedEffect(12, 0x102);
	Engine_EventWait(60);
	Engine_ActorRunRepeatedMotion(11, 1);
	Engine_EventWait(10);
	Event_SayThenWait(11, 30);
	Actor_FaceDirection(11, 0xd000, 30);
	Engine_ActorSetAnimationAndWait(11, 3);
	Engine_EventWait(20);
	Event_SayThenWait(11, 30);
	Engine_ActorSetAnimationAndWait(12, 3);
	Engine_EventWait(20);
	Actor_FaceDirection(11, 0x5000, 40);
	Actor_FaceDirection(9, 0x5000, 0);
	Actor_FaceDirection(5, FACING_SOUTHWEST, 20);
	Engine_ActorSetAnimationAndWait(10, 3);
	Engine_EventWait(20);
	Engine_EventCloseScreen();
	Engine_EventWaitForScreen();
	Camera_MoveTo(0x1080000, -1, 0x1cc0000, 0);
	Engine_MapRedraw();
	Engine_TaskWait(1);
	Engine_EventOpenScreen();
	Engine_EventWaitForScreen();
	Engine_EventWait(40);
	Event_SayThenWait(10, 40);
	Engine_ActorStartRepeatedMotion(0, 3);
	Engine_ActorRunRepeatedMotion(1, 3);
	Engine_EventWait(80);
	Engine_EventOpenMessage(11, 0);
	if (Engine_EventChooseYesNo(0, 0) != 0)
	{
		SkipMessage(1);
	}
	BattlePres_RunActionThenWaitIfModeZero(9, 0, 20);
	Engine_EventCloseScreen();
	Engine_EventWaitForScreen();
	Engine_CameraMoveTo(0x1dd0000, -1, 0x14e0000, 0);
	Engine_MapRedraw();
	Engine_TaskWait(1);
	Engine_EventOpenScreen();
	Engine_EventWaitForScreen();
	Engine_EventWait(20);
	Engine_ActorRunRepeatedMotion(10, 2);
	Actor_FaceDirection(10, 0xb000, 10);
	Engine_EventSetMessage((s32)MsgSoruWhyDenyDont);
	Event_SayThenWait(10, 20);
	Actor_FaceDirection(5, FACING_SOUTHEAST, 0);
	Actor_FaceDirection(9, 0x3000, 10);
	Engine_ActorSetAnimation(9, 4);
	Event_SayThenWait(0x5009, 40);
	Engine_ActorRunRepeatedMotion(11, 1);
	Event_SayThenWait(11, 10);
	Engine_ActorStartRepeatedMotion(5, 2);
	Engine_ActorRunRepeatedMotion(9, 2);
}

static void Scene_OfferGuarantee(void)
{
	Actor_FaceDirection(JASMINE, FACING_SOUTHWEST, 0);
	Actor_FaceDirection(ACTOR_SUKURETA, 0x5000, 0);
	Actor_FaceDirection(ACTOR_SATUROS, FACING_WEST, 0);
	Actor_FaceDirection(ACTOR_GARCIA_MASKED, 0x5000, 40);
	Actor_FaceDirection(ACTOR_MENARDI, 0xd000, 40);
	Engine_ActorRunRepeatedMotion(ACTOR_MENARDI, 2);
	Event_SayThenWait(11, 20);
	Engine_ActorRunRepeatedMotion(ACTOR_GARCIA_MASKED, 2);
	Actor_SetAttachedEffect(ACTOR_GARCIA_MASKED, 0x102);
	Engine_EventWait(60);
	Event_SayThenWait(12, 10);
	Actor_FaceActor(ACTOR_SATUROS, ACTOR_GARCIA_MASKED, 0);
	Actor_FaceActor(JASMINE, ACTOR_GARCIA_MASKED, 0);
	Actor_FaceActor(ACTOR_SUKURETA, ACTOR_GARCIA_MASKED, 0);
	Engine_EventWait(40);
	Engine_ActorRunRepeatedMotion(ACTOR_SATUROS, 1);
	Actor_FaceDirection(ACTOR_SATUROS, FACING_WEST, 10);
	Engine_ActorSetAnimation(ACTOR_SATUROS, 3);
	Event_SayThenWait(10, 10);
	Actor_FaceDirection(ACTOR_MENARDI, FACING_EAST, 10);
	Engine_ActorSetAnimationAndWait(ACTOR_MENARDI, 3);
	Engine_EventWait(10);
	Actor_FaceDirection(JASMINE, FACING_EAST, 0);
	Actor_FaceDirection(ACTOR_SUKURETA, FACING_WEST, 20);
	Engine_ActorStartRepeatedMotion(JASMINE, 2);
	Engine_ActorRunRepeatedMotion(ACTOR_SUKURETA, 2);
	Engine_EventWait(10);
	Actor_FaceDirection(JASMINE, FACING_NORTH, 0);
	Actor_FaceDirection(ACTOR_SUKURETA, 0xb000, 10);
	Actor_FaceDirection(ACTOR_SATUROS, 0xb000, 40);
	Engine_ActorSetAnimationAndWait(ACTOR_SATUROS, 3);
	Event_SayThenWait(10, 10);
	Actor_ShowEmote(JASMINE, 0x100, 20);
	Actor_SetAttachedEffect(ACTOR_GARCIA_MASKED, 0x102);
	Engine_ActorRunRepeatedMotion(ACTOR_GARCIA_MASKED, 3);
	Engine_EventWait(40);
	Actor_SetPosition(GERALD, 0x15a0000, 0x1b80000);
	Engine_TaskWait(1);
	Event_ShowMessage(GERALD, 0);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 0);
	Actor_FaceDirection(GERALD, FACING_NORTHWEST, 20);
	Engine_EventCloseScreen();
	Engine_EventWaitForScreen();
	Actor_SetPosition(GERALD, 0x1180000, 0x1d60000);
	Camera_MoveTo(0x1050000, -1, 0x1d20000, 0);
	Engine_MapRedraw();
	Engine_TaskWait(1);
	Engine_EventOpenScreen();
	Engine_EventWaitForScreen();
	Engine_EventWait(20);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x3000, 10);
	Actor_ShowEmote(GERALD, 0x102, 0);
	Engine_ActorRunRepeatedMotion(GERALD, 3);
	Event_AskYesNo(GERALD, 0);
	Engine_EventWait(10);
	Engine_ActorSetAnimationAndWait(GERALD, 4);
	Engine_EventWait(10);
	Event_AskYesNo(GERALD, 0);
	Engine_EventWait(10);
	Engine_EventSetMessage((s32)MsgSoruMenardiDontYouWantThem);
	Event_ShowMessage(ACTOR_MENARDI, 0);
	Actor_FaceDirection(ACTOR_MENARDI, 0xd000, 10);
	Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
	Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
	Engine_EventWait(10);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHEAST, 0);
	Actor_FaceDirection(GERALD, FACING_NORTHEAST, 20);
	Engine_EventCloseScreen();
	Engine_EventWaitForScreen();
	Camera_MoveTo(0x1dd0000, -1, 0x14e0000, 0);
	Engine_MapRedraw();
	Engine_TaskWait(1);
	Engine_EventOpenScreen();
	Engine_EventWaitForScreen();
	Engine_EventWait(20);
	Engine_ActorRunRepeatedMotion(ACTOR_SUKURETA, 2);
	Engine_EventWait(10);
	Actor_FaceDirection(ACTOR_SUKURETA, FACING_WEST, 30);
	Event_SayThenWait(9, 20);
	Actor_FaceDirection(JASMINE, FACING_EAST, 40);
	Engine_ActorSetAnimationAndWait(JASMINE, 4);
	Engine_EventWait(20);
	Event_SayThenWait(5, 10);
	Engine_ActorRunRepeatedMotion(ACTOR_GARCIA_MASKED, 2);
	Engine_EventWait(80);
	Event_SayThenWait(12, 20);
	Engine_ActorRunRepeatedMotion(JASMINE, 2);
	Actor_FaceDirection(JASMINE, FACING_NORTH, 0);
	Actor_FaceDirection(ACTOR_SUKURETA, 0xb000, 20);
	Engine_ActorSetAnimationAndWait(ACTOR_GARCIA_MASKED, 3);
	Event_SayThenWait(12, 20);
	Engine_ActorStartRepeatedMotion(JASMINE, 2);
	Engine_ActorRunRepeatedMotion(ACTOR_SUKURETA, 2);
	Actor_FaceDirection(ACTOR_GARCIA_MASKED, 0xb000, 40);
}

static void Scene_UnmaskGarcia(void)
{
	struct FieldActor *obj;
	struct FieldActor *other;
	u16				  *tbl;
	s32				   left;
	s32				   cnt;
	s32				   mes_a;
	s32				   mes_b;

	Event_PlayCue(161);
	Engine_ActorRunRepeatedMotion(ACTOR_GARCIA_MASKED, 3);
	Engine_EventWait(40);
	other = Object_GetById(ACTOR_GARCIA_MASKED);
	if (other != 0)
	{
		Actor_SetPosition(ACTOR_GARCIA, other->x.fixed, other->z.fixed);
	}
	Actor_SetPosition(ACTOR_GARCIA_MASKED, 0, 0);
	Engine_EventWait(20);
	Actor_FaceDirection(ACTOR_GARCIA, 0x3000, 40);
	Engine_ActorRunRepeatedMotion(JASMINE, 3);
	Engine_ActorRunRepeatedMotion(JASMINE, 3);
	Engine_EventWait(40);
	Engine_ActorRunRepeatedMotion(JASMINE, 2);
	Event_SayThenWait(5, 20);
	Engine_ActorSetAnimationAndWait(ACTOR_GARCIA, 3);
	Engine_EventWait(10);
	Engine_ActorStartRepeatedMotion(JASMINE, 3);
	Actor_FaceDirection(ACTOR_SUKURETA, FACING_WEST, 10);
	Engine_ActorRunRepeatedMotion(ACTOR_SUKURETA, 2);
	Event_SayThenWait(9, 40);
	Engine_ActorSetAnimationAndWait(JASMINE, 3);
	Engine_EventWait(40);
	Actor_FaceDirection(ACTOR_SUKURETA, 0xb000, 40);
	Engine_ActorRunRepeatedMotion(ACTOR_GARCIA, 2);
	Event_SayThenWait(13, 20);
	Engine_ActorRunRepeatedMotion(JASMINE, 1);
	Engine_EventWait(40);
	Engine_ActorSetAnimationAndWait(ACTOR_GARCIA, 3);
	Engine_EventWait(10);
	Event_SayThenWait(13, 40);
	Engine_ActorStartRepeatedMotion(ACTOR_SATUROS, 1);
	Engine_ActorSetAnimationAndWait(ACTOR_SATUROS, 3);
	Engine_EventWait(10);
	Event_SayThenWait(10, 10);
	Engine_ActorRunRepeatedMotion(ACTOR_MENARDI, 1);
	Engine_ActorSetAnimation(ACTOR_MENARDI, 3);
	Event_SayThenWait(11, 80);
	Engine_ActorRunRepeatedMotion(ACTOR_GARCIA, 2);
	Event_SayThenWait(13, 40);
	Engine_ActorRunRepeatedMotion(JASMINE, 2);
	Engine_EventWait(10);
	Event_SayThenWait(5, 10);
	Engine_ActorStartRepeatedMotion(ACTOR_GARCIA, 2);
	Engine_ActorRunRepeatedMotion(ACTOR_SUKURETA, 2);
	Engine_EventWait(80);
	Engine_ActorSetAnimationAndWait(JASMINE, 4);
	Engine_EventWait(20);
	Event_SayThenWait(5, 80);
	Engine_ActorSetAnimationAndWait(ACTOR_GARCIA, 4);
	Event_SayThenWait(13, 80);
	Engine_ActorRunRepeatedMotion(JASMINE, 2);
	Engine_EventWait(4);
	Event_SayThenWait(5, 20);
	Engine_ActorRunRepeatedMotion(ACTOR_SATUROS, 1);
	Engine_ActorSetAnimation(ACTOR_SATUROS, 3);
	Event_SayThenWait(10, 10);
	Engine_ActorRunRepeatedMotion(ACTOR_MENARDI, 1);
	Event_SayThenWait(11, 10);
	Engine_ActorRunRepeatedMotion(ACTOR_SATUROS, 1);
	Event_SayThenWait(10, 10);
	Actor_FaceDirection(ACTOR_SUKURETA, 0x3000, 80);
	Actor_ShowEmote(ACTOR_SUKURETA, 0x105, 80);
	Engine_ActorRunRepeatedMotion(ACTOR_MENARDI, 1);
	Actor_FaceDirection(ACTOR_MENARDI, 0x5000, 40);
	Engine_ActorStartRepeatedMotion(ACTOR_MENARDI, 2);
	Event_SayThenWait(11, 20);
	Engine_EventCloseScreen();
	Engine_EventWaitForScreen();
	Camera_MoveTo(0x1050000, -1, 0x1d20000, 0);
	Engine_MapRedraw();
	Engine_TaskWait(1);
	Engine_EventOpenScreen();
	Engine_EventWaitForScreen();
	Engine_EventWait(20);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x3000, 0);
	Actor_FaceDirection(GERALD, 0xb000, 20);
	Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
	Engine_ActorRunRepeatedMotion(GERALD, 2);
	Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x8000, 0x4000);
	Actor_SetSpeed(GERALD, 0x8000, 0x4000);
	Actor_WalkTo(ACTOR_PARTY_LEADER, 244, 0x1de);
	Actor_WalkToAndWait(GERALD, 0x104, 0x1ea);
	Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 1);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x3000, 0);
	Actor_FaceDirection(GERALD, 0xb000, 20);
	Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
	Engine_ActorSetAnimationAndWait(GERALD, 4);
	Engine_EventWait(20);
	cnt = 0;
	tbl = UnitGet(1)->inventory;
	left = 14;
	do
	{
		u32 id = *tbl & 0x1ff;
		tbl++;
		if (id == 220 || id == 221 || id == 223)
			cnt++;
		left--;
	} while (left >= 0);
	Event_OpenMessage(GERALD, 0);
	if (Engine_EventChooseYesNo(0, 0) == 0)
	{
		mes_a = (s32)MsgSoruGuessTakeElemental;
		Engine_EventSetMessage(mes_a);
		Engine_ActorSetAnimationAndWait(GERALD, 3);
		Engine_EventWait(10);
		if (cnt <= 2)
		{
			Event_SayThenWait(1, 30);
			Actor_WalkToAndWait(GERALD, 252, 0x1e6);
			Engine_ActorRunRepeatedMotion(GERALD, 2);
			Engine_EventWait(10);
			UiText_ShowCenteredMessage((mes_a + 1), 1, 0);
		}
		else
		{
			Engine_EventSetMessage((s32)MsgSoruRightTake);
			Event_SayThenWait(GERALD, 30);
		}
	}
	else
	{
		if (cnt <= 2)
		{
			mes_b = (s32)MsgSoruDontHandOver;
			Engine_EventSetMessage(mes_b);
			Engine_ActorRunRepeatedMotion(GERALD, 3);
			Engine_ActorSetAnimationAndWait(GERALD, 4);
			Event_SayThenWait(1, 10);
			Engine_ActorSetAnimationAndWait(GERALD, 4);
			Engine_ActorRunRepeatedMotion(GERALD, 1);
			Actor_SetSpeed(GERALD, 0x20000, 0x10000);
			obj = Object_GetById(ACTOR_PARTY_LEADER);
			obj->steering_flags &= ~ACTOR_FACES_MOTION;
			Actor_WalkToAndWait(GERALD, 244, 0x1de);
			Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x18000, 0xc000);
			Engine_ActorJump(ACTOR_PARTY_LEADER, 6, 0);
			Actor_SetDestination(ACTOR_PARTY_LEADER, 218, 0x1d7);
			Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
			UiText_ShowCenteredMessage((mes_b + 1), 1, 0);
			Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
			Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 30);
			obj->steering_flags |= ACTOR_FACES_MOTION;
		}
		else
		{
			Engine_EventSetMessage((s32)MsgSoruWontLetGo);
			Engine_ActorRunRepeatedMotion(GERALD, 3);
			Engine_ActorSetAnimationAndWait(GERALD, 4);
			Event_SayThenWait(1, 10);
			Engine_ActorSetAnimationAndWait(GERALD, 4);
			Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHEAST, 30);
		}
	}
	Camera_SetSpeed(0x8000, 0x1000);
	Engine_CameraFollowActor(GERALD, 1);
	Engine_CameraWaitForMove();
	Actor_FaceDirection(GERALD, FACING_WEST, 30);
	Engine_ActorRunRepeatedMotion(GERALD, 2);
	Actor_SetSpeed(GERALD, 0x8000, 0x4000);
	obj = Object_GetById(GERALD);
	obj->steering_flags &= ~ACTOR_FACES_MOTION;
	Actor_WalkToAndWait(GERALD, 0x108, 0x1e2);
	obj->steering_flags |= ACTOR_FACES_MOTION;
	Actor_WalkToAndWait(GERALD, 0x116, 0x1e0);
	obj->speed = 0x30000;
	obj->acceleration = 0x20000;
	Event_PlayCue(153);
	obj->velocity_y = 0x60000;
	Engine_ActorSetAnimation(GERALD, 7);
	Actor_MoveToAndWait(GERALD, 0x138, 0x1d6);
	Engine_ActorSetAnimation(GERALD, 1);
	Engine_EventWait(30);
	Event_PlayCue(153);
	obj->velocity_y = 0x60000;
	Engine_ActorSetAnimation(GERALD, 7);
	Actor_MoveToAndWait(GERALD, 0x156, 0x1d6);
	Engine_ActorSetAnimation(GERALD, 1);
	Engine_EventWait(30);
	Event_PlayCue(153);
	obj->velocity_y = 0x60000;
	Engine_ActorSetAnimation(GERALD, 7);
	Actor_MoveToAndWait(GERALD, 0x178, 0x1d6);
	Engine_ActorSetAnimation(GERALD, 1);
}

static void Scene_AlexTakesStars(void)
{
	u32				   i;
	struct FieldActor *rec;
	s32				   record;
	u8				   none;

	Engine_ActorRunRepeatedMotion(GERALD, 3);
	Engine_EventWait(10);
	Actor_FaceDirection(GERALD, 0x3000, 0);
	record = Actor_Get(ACTOR_ALEX);
	Engine_ActorSetSpriteFlags(record, 0);
	Actor_SetChildValue(ACTOR_ALEX, 15);
	Actor_SetPosition(ACTOR_ALEX, 0x1880000, 0x1c60000);
	SoruStar_RiseActorFourteenSparks();
	Actor_FaceDirection(GERALD, 0xd000, 10);
	Engine_ActorStartRepeatedMotion(GERALD, 2);
	Actor_ShowEmote(GERALD, 0x100, 40);
	Actor_FaceDirection(ACTOR_ALEX, 0x5000, 10);
	Engine_ActorRunRepeatedMotion(ACTOR_ALEX, 2);
	Engine_EventWait(20);
	Engine_EventSetMessage((s32)MsgSoruPermitRelieveElemental);
	Event_ShowMessage(ACTOR_ALEX, 0);
	Actor_SetPosition(ACTOR_SATUROS, 0x1d50000, 0x15c0000);
	Engine_EventWait(20);
	Event_SayThenWait(0x200a, 10);
	Event_SayThenWait(0x200a, 40);
	Actor_SetPosition(ACTOR_SATUROS, 0x1fb0000, 0x15c0000);
	Engine_ActorRunRepeatedMotion(GERALD, 2);
	Engine_EventWait(40);
	Engine_ActorSetAnimationAndWait(GERALD, 3);
	Engine_EventWait(40);
	Actor_SetSpeed(GERALD, 0x8000, 0x4000);
	Actor_WalkToAndWait(GERALD, 0x185, 0x1d4);
	Actor_FaceDirection(GERALD, 0xd000, 60);
	Event_SayThenWait(1, 20);
	UiText_ShowCenteredMessage((s32)MsgSoruPermitRelieveElemental + 4, 1, 10);
	Actor_SetSpeed(GERALD, 0x8000, 0x4000);
	rec = Object_GetById(GERALD);
	rec->steering_flags &= ~ACTOR_FACES_MOTION;
	none = 0;
	Actor_WalkToAndWait(GERALD, 0x178, 0x1d6);
	Engine_EventWait(30);
	rec->steering_flags |= ACTOR_FACES_MOTION;
	Engine_ActorSetAnimationAndWait(ACTOR_ALEX, 4);
	Engine_EventWait(10);
	Engine_EventSetMessage((s32)MsgSoruPermitRelieveElemental + 5);
	Event_SayThenWait(14, 20);
	Actor_ShowEmote(GERALD, 0x101, 60);
	Engine_ActorSetAnimationAndWait(ACTOR_ALEX, 3);
	Event_SayThenWait(14, 20);
	Actor_ShowEmote(GERALD, 0x102, 60);
	Engine_ActorSetAnimationAndWait(ACTOR_ALEX, 3);
	Engine_EventWait(20);
	Actor_FaceDirection(ACTOR_ALEX, FACING_NORTH, 20);
	Actor_SetChildValue(ACTOR_ALEX, 0x100);
	record = Actor_Get(ACTOR_ALEX);
	Engine_ActorSetSpriteFlags(record, 0);
	rec = Object_GetById(ACTOR_ALEX);
	rec->motion_flags = none;
	Event_PlayCue(220);
	for (i = 0; i != 30; i++)
	{
		rec->y.fixed += 0x10000;
		Engine_EventWait(1);
	}
	rec->motion_flags = 5;
	Engine_ActorStartRepeatedMotion(GERALD, 2);
	Event_SayThenWait(1, 10);
	Actor_ShowEmote(ACTOR_ALEX, 0x101, 60);
	Actor_FaceDirection(ACTOR_ALEX, 0x5000, 10);
	Event_SayThenWait(1, 20);
	Engine_ActorRunRepeatedMotion(ACTOR_ALEX, 1);
	Event_SayThenWait(14, 20);
	Actor_ShowEmote(GERALD, 0x103, 20);
	Event_SayThenWait(1, 30);
	Actor_ShowEmote(ACTOR_ALEX, 0x105, 80);
	Actor_FaceDirection(ACTOR_ALEX, 0xd000, 40);
	Actor_FaceDirection(ACTOR_SATUROS, 0x5000, 10);
	Engine_EventCloseScreen();
	Engine_EventWaitForScreen();
	Camera_MoveTo(0x1dd0000, -1, 0x14e0000, 0);
	Engine_MapRedraw();
	Engine_TaskWait(1);
	Engine_EventOpenScreen();
	Engine_EventWaitForScreen();
	Engine_EventWait(20);
	Engine_ActorSetAnimationAndWait(ACTOR_SATUROS, 4);
	Event_SayThenWait(10, 10);
	Event_OpenMessage(ACTOR_MENARDI, 0);
	Camera_SetSpeed(0x66666, 0xcccc);
	Camera_MoveTo(0x1760000, -1, 0x1d60000, 1);
	Engine_CameraWaitForMove();
	Actor_FaceDirection(ACTOR_ALEX, 0x5000, 0);
	Actor_FaceDirection(GERALD, FACING_NORTHEAST, 10);
	Engine_ActorRunRepeatedMotion(GERALD, 2);
	if (Engine_EventChooseYesNo(1, 0) != 0)
	{
		Engine_EventWait(10);
		Engine_ActorSetAnimationAndWait(14, 4);
		Engine_EventSetMessage((s32)MsgSoruDontWantAnything);
		Event_OpenMessage(ACTOR_ALEX, 0);
		if (Engine_EventChooseYesNo(1, 0) == 0)
		{
			do
			{
				Engine_EventWait(20);
				Engine_ActorSetAnimationAndWait(14, 4);
				Engine_EventWait(10);
				Engine_EventSetMessage((s32)MsgSoruDoubtHowFeel);
				Event_OpenMessage(ACTOR_ALEX, 0);
			} while (Engine_EventChooseYesNo(1, 0) == 0);
		}
	}
	Engine_EventWait(30);
	Engine_ActorSetAnimationAndWait(ACTOR_ALEX, 3);
	Engine_EventWait(20);
	Engine_EventSetMessage((s32)MsgSoruThankCooperation);
	Event_SayThenWait(14, 30);
	Engine_ActorSetAnimationAndWait(ACTOR_ALEX, 3);
	Engine_EventWait(10);
	Event_SayThenWait(14, 30);
	rec->motion_flags = 0;
	Actor_SetSpeed(ACTOR_ALEX, 0x26666, 0x13333);
	FieldObject_SetMoveTarget((s32)rec, 0x1cc0000, 0, 0x1680000);
	Engine_ActorWaitForMove(ACTOR_ALEX);
	Actor_SetChildValue(ACTOR_ALEX, 0);
	record = Actor_Get(ACTOR_ALEX);
	Engine_ActorSetSpriteFlags(record, 1);
	Engine_EventWait(30);
	Engine_CameraFollowActor(GERALD, 1);
	Engine_CameraWaitForMove();
	Engine_EventWait(40);
	Actor_ShowEmote(GERALD, 0x103, 40);
	Engine_ActorRunRepeatedMotion(GERALD, 3);
	Engine_EventWait(20);
	rec = Object_GetById(GERALD);
	rec->steering_flags |= ACTOR_FACES_MOTION;
	rec->speed = 0x30000;
	rec->acceleration = 0x20000;
	Event_PlayCue(153);
	rec->velocity_y = 0x60000;
	Engine_ActorSetAnimation(GERALD, 7);
	Actor_MoveToAndWait(GERALD, 0x156, 0x1d6);
	Engine_ActorSetAnimation(GERALD, 1);
	Engine_EventWait(30);
	Event_PlayCue(153);
	rec->velocity_y = 0x60000;
	Engine_ActorSetAnimation(GERALD, 7);
	Actor_MoveToAndWait(GERALD, 0x138, 0x1d6);
	Engine_ActorSetAnimation(GERALD, 1);
	Engine_EventWait(30);
	Event_PlayCue(153);
	rec->velocity_y = 0x60000;
	Engine_ActorSetAnimation(GERALD, 7);
	Actor_MoveToAndWait(GERALD, 0x116, 0x1e0);
	Engine_ActorSetAnimation(GERALD, 1);
	Engine_EventWait(30);
	Camera_SetSpeed(0x8000, 0x1000);
	Engine_CameraFollowActor(ACTOR_PARTY_LEADER, 1);
	Actor_SetSpeed(GERALD, 0x19999, 0xcccc);
	Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, GERALD, 0);
	Engine_EventWait(30);
	Engine_ActorSetAnimationAndWait(GERALD, 3);
	Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 4);
	Engine_ActorRunRepeatedMotion(GERALD, 2);
	Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
	Engine_EventWait(20);
	Engine_ActorSetAnimation(GERALD, 2);
	{
		s32 slot = Object_GetById(ACTOR_PARTY_LEADER);

		if (slot != 0)
		{
			Actor_SetDestination(GERALD, *(s16 *)(slot + 10), *(s16 *)(slot + 18));
		}
	}
	Engine_ActorWaitForMove(GERALD);
	Actor_SetPosition(GERALD, 0, 0);
	PartyInventory_Discard(220);
	PartyInventory_Discard(221);
	PartyInventory_Discard(223);
}

struct StarFollower
{
	u8				   unknown_00[12];
	s32				   y;
	u8				   unknown_10[8];
	s32				   scale_x;
	s32				   scale_y;
	u8				   unknown_20[0x35];
	u8				   motion_flags;
	u8				   unknown_56[4];
	u8				   flags;
	u8				   unknown_5b[13];
	struct FieldActor *leader;
};

static void SoruStar_LineUpFollowers(void)
{
	struct FieldActor	*leader;
	struct StarFollower *actor;
	struct StarFollower *menardi;

	leader = Object_GetById(0);
	Engine_EventBegin();
	Engine_ActorEnableActionCallback(5, (const s32 *)1);
	Engine_ActorEnableActionCallback(9, (const s32 *)1);
	Engine_ActorEnableActionCallback(11, (const s32 *)1);
	Engine_ActorEnableActionCallback(10, (const s32 *)1);
	Engine_ActorEnableActionCallback(14, (const s32 *)1);
	Engine_ActorEnableActionCallback(13, (const s32 *)1);
	Actor_SetPosition(5, 0x1db0000, 0x14c0000);
	Actor_SetPosition(9, 0x1eb0000, 0x14c0000);
	Actor_SetPosition(11, 0x1cb0000, 0x15c0000);
	Actor_SetPosition(10, 0x1fb0000, 0x15c0000);
	Actor_SetPosition(14, 0x1cc0000, 0x1680000);
	Actor_SetPosition(13, 0x1d70000, 0x1320000);
	actor = (struct StarFollower *)Object_GetById(5);
	actor->leader = leader;
	actor->flags |= 1;
	Engine_ObjectSetScript((struct FieldActor *)actor, SoruStar_AngleScript);
	actor = (struct StarFollower *)Object_GetById(9);
	actor->leader = leader;
	actor->flags |= 1;
	Engine_ObjectSetScript((struct FieldActor *)actor, SoruStar_AngleScript);
	actor = (struct StarFollower *)Object_GetById(11);
	actor->leader = leader;
	actor->flags |= 1;
	Engine_ObjectSetScript((struct FieldActor *)actor, SoruStar_AngleScript);
	actor = (struct StarFollower *)Object_GetById(10);
	actor->leader = leader;
	actor->flags |= 1;
	Engine_ObjectSetScript((struct FieldActor *)actor, SoruStar_AngleScript);
	menardi = (struct StarFollower *)Object_GetById(14);
	menardi->leader = leader;
	menardi->flags |= 1;
	menardi->scale_x = 0x10000;
	menardi->scale_y = 0x10000;
	menardi->motion_flags = Object_GetById(11)->motion_flags;
	menardi->y = 0;
	Engine_ObjectSetScript((struct FieldActor *)menardi, SoruStar_AngleScript);
	actor = (struct StarFollower *)Object_GetById(13);
	actor->leader = leader;
	actor->flags |= 1;
	Engine_ObjectSetScript((struct FieldActor *)actor, SoruStar_AngleScript);
	Engine_EventEnd();
}

struct Actor
{
	u8	pad[8];
	s32 x;
	s32 y;
	s32 z;
};

struct EffectParams
{
	s32 count;
	s32 kind;
	s32 spread;
	s32 rise;
	u8	pad10[8];
	u16 tile;
	u16 pad1a;
	u8	pad1c[12];
};

static void EV05_SEARCH(void)
{
	u8				   pass;
	s32				   star;
	struct FieldActor *leader;

	Engine_EventBegin();
	Event_PlayCue(141);
	for (pass = 0; pass != 6; pass++)
	{
		ColorBuffer_ApplyTarget(0x4039d2, 1);
		Engine_ColorBufferInterpolate(8);
		Engine_EventWait(8);
		ColorBuffer_ApplyTarget(0x10000, 1);
		Engine_ColorBufferInterpolate(8);
		Engine_EventWait(8);
		if (pass == 1)
		{
			Work_SetValuesIfNonNegative(0x10000, 0x10000, 0x10000);
		}
	}
	Event_PlayCue(0x121);
	Work_SetValuesIfNonNegative(-1, -1, 0xe666);
	Map_CopyCellsTo(0, 40, 13, 46, 3, 3);
	Engine_EventWait(20);
	star = Scene_PresentItem(222, 0xe80000, 0x100000, 0x900000);
	Engine_EventWait(40);
	UiWork_PushValueSlot(star, 1);
	Engine_MessageShowCentered((s32)MsgSoruMarsStarBagged, 1);
	Actor_SetPosition(JASMINE, 0x1330000, 0x1150000);
	Actor_SetPosition(ACTOR_SUKURETA, 0x1330000, 0x1150000);
	Actor_SetPosition(ACTOR_MENARDI, 0x1330000, 0x1150000);
	Actor_SetPosition(ACTOR_SATUROS, 0x1330000, 0x1150000);
	Actor_SetPosition(ACTOR_ALEX, 0x1330000, 0x1150000);
	Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x13333, 0x9999);
	Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 232, 156);
	Engine_EventWait(10);
	leader = Object_GetById(ACTOR_PARTY_LEADER);
	if (leader != NULL)
	{
		Actor_SetPosition(GERALD, leader->x.fixed, leader->z.fixed);
	}
	Actor_SetSpeed(GERALD, 0x13333, 0x9999);
	Actor_WalkToAndWait(GERALD, 218, 172);
	Engine_ActorFaceEachOther(GERALD, ACTOR_PARTY_LEADER, 0);
	Engine_EventWait(20);
	Event_PlayCue(145);
	Work_SetValuesIfNonNegative(0x40000, 0x40000, 0x10000);
	Engine_EventWait(20);
	Work_SetValuesIfNonNegative(0x10000, 0x10000, 0x10000);
	Engine_EventWait(40);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xd000, 0);
	Actor_FaceDirection(GERALD, 0x5000, 50);
	Event_PlayCue(144);
	Work_SetValuesIfNonNegative(0x30000, 0x30000, 0x10000);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
	Actor_FaceDirection(GERALD, FACING_EAST, 50);
	Work_SetValuesIfNonNegative(0x10000, 0x10000, 0x10000);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
	Actor_FaceDirection(GERALD, FACING_WEST, 50);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xb000, 0);
	Actor_FaceDirection(GERALD, 0xd000, 0);
	Event_PlayCue(144);
	Work_SetValuesIfNonNegative(0x30000, 0x30000, 0x10000);
	Engine_EventWait(30);
	Engine_ActorJump(ACTOR_PARTY_LEADER, 2, 0);
	Engine_ActorJump(GERALD, 2, 20);
	Engine_ActorJump(ACTOR_PARTY_LEADER, 6, 0);
	Engine_ActorJump(GERALD, 6, 40);
	gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
	Engine_EventCloseScreen();
	Engine_EventWaitForScreen();
	Engine_EventRequestExit(2);
}

static void EV09_TALK(void)
{
	Engine_EventBegin();
	if (Engine_GameFlagIsSet(FLAG_STARS_GIVEN_TO_ALEX))
	{
		Engine_EventSetMessage((s32)MsgSoruSukuretaIAmResponsible);
		Engine_EventShowMessage(ACTOR_SUKURETA, 0);
	}
	else
	{
		if (Engine_GameFlagIsSet(FLAG_FIRST_STAR_BAGGED) == 0)
			Engine_EventSetMessage((s32)MsgSoruSukuretaBringTheStarsHere);
		else
			Engine_EventSetMessage((s32)MsgSoruSukuretaFetchTheOthers);
		Engine_ActorFaceEachOther(ACTOR_SUKURETA, ACTOR_PARTY_LEADER, 0);
		Engine_EventWait(10);
		Engine_EventShowMessage(ACTOR_SUKURETA, 0);
	}
	Engine_EventEnd();
}

static void EV05_SCRIPT(void)
{
	Engine_EventBegin();
	if (Engine_GameFlagIsSet(FLAG_STARS_GIVEN_TO_ALEX))
	{
		Engine_EventSetMessage((s32)MsgSoruJasmineMyBrotherIsAlive);
		Engine_EventShowMessage(JASMINE, 0);
	}
	else
	{
		if (Engine_GameFlagIsSet(FLAG_FIRST_STAR_BAGGED) == 0)
			Engine_EventSetMessage((s32)MsgSoruJasmineBeCareful);
		else
			Engine_EventSetMessage((s32)MsgSoruJasmineSukuretaIsGiddy);
		Engine_ActorFaceEachOther(JASMINE, ACTOR_PARTY_LEADER, 0);
		Engine_EventWait(10);
		Engine_EventShowMessage(JASMINE, 0);
	}
	Engine_EventEnd();
}

static void EV10_TALK(void)
{
	Engine_EventBegin();
	Engine_EventSetMessage((s32)MsgSoruSaturosBringTheFinalStar);
	Engine_EventShowMessage(ACTOR_SATUROS, 0);
	Engine_EventEnd();
}

static void EV11_SCRIPT(void)
{
	Engine_EventBegin();
	Engine_EventSetMessage((s32)MsgSoruMenardiBringTheFinalGem);
	Engine_EventShowMessage(ACTOR_MENARDI, 0);
	Engine_EventEnd();
}

static void EV13_TALK(void)
{
	Engine_EventBegin();
	Engine_EventSetMessage((s32)MsgSoruGarciaSilence);
	Engine_EventShowMessage(ACTOR_GARCIA, 0);
	Engine_EventEnd();
}

static void EV14_TALK(void)
{
	Engine_EventBegin();
	Engine_EventSetMessage((s32)MsgSoruAlexOnlyOneLeft);
	Engine_EventShowMessage(ACTOR_ALEX, 0);
	Engine_EventEnd();
}

static void EV01_TOUCH(void)
{
	Engine_EventBegin();
	Engine_EventSetMessage((s32)MsgSoruNoStarsOutThere);
	Event_SayThenWait(9, 10);
	Engine_EventEnd();
}

static void SoruStar_CheckLeaderAhead(void)
{
	Engine_LeaderCheckAhead();
}

static void Scene_EnterStarRoom(void)
{
	u16				   i;
	u8				  *leader_motion;
	u8				  *sukureta_motion;
	u8				  *leader_sprite;
	u8				  *sukureta_sprite;
	struct FieldActor *rising;
	struct FieldActor *glow;
	struct FieldActor *leader;
	u8				  *sukureta_flags;
	s32				   say_4009;
	s32				   height;
	s32				   say_8009;
	s32				   zero;

	Engine_EventBegin();
	Map_CopyCellAttributes(27, 16, 5, 1, 27, 17);
	Camera_MoveTo(0x1d70000, -1, 0x1050000, 0);
	Engine_CameraWaitForMove();
	Engine_MapRedraw();
	glow = Actor_Get(8);
	glow->scale_x = 0x1999;
	glow->scale_y = 0x1999;
	rising = Actor_Get(ACTOR_PARTY_LEADER);
	leader_sprite = &rising->sprite->flags;
	*leader_sprite = 0;
	rising->scale_x = 0x1999;
	rising->scale_y = 0x1999;
	Actor_SetChildValue(ACTOR_PARTY_LEADER, 0x100);
	Actor_SetPosition(ACTOR_PARTY_LEADER, 0x1d70000, 0x1220000);
	leader_motion = &rising->motion_flags;
	*leader_motion = 0;
	rising->y.fixed = 0x280000;
	gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 3);
	gEventWork->transition_frames = 32;
	Engine_EventOpenScreen();
	Engine_EventWaitForScreen();
	Engine_EventWait(20);
	Actor_SetPosition(8, 0x1d70000, 0x1220000);
	Event_PlayCue(190);
	Engine_ActorSetSpritePriority(ACTOR_PARTY_LEADER, 2);
	for (i = 0; i != 90; i++)
	{
		rising->y.fixed += -0x1999;
		rising->scale_x += 0x28f;
		rising->scale_y += 0x28f;
		glow->scale_x += 0x28f;
		glow->scale_y += 0x28f;
		Engine_EventWait(1);
	}
	*leader_motion = 5;
	Engine_EventWait(80);
	Camera_SetSpeed(0x4ccc, 0x999);
	Camera_MoveTo(0x1d70000, -1, 0x1220000, 1);
	for (i = 0; i != 60; i++)
	{
		rising->y.fixed += -0x8000;
		Engine_EventWait(1);
	}
	*leader_motion = 3;
	Engine_EventWait(20);
	Engine_ActorSetSpritePriority(ACTOR_PARTY_LEADER, 1);
	{
		struct FieldActor *actor = Actor_Get(ACTOR_PARTY_LEADER);
		s32				   flags = 1 | actor->priority_flags;
		actor->priority_flags = flags;
	}
	Actor_SetChildValue(ACTOR_PARTY_LEADER, 0);
	{
		s32 shown = 1;

		*leader_sprite = shown;
	}
	Engine_ActorSetPosition(8, 0, 0);
	Engine_CameraWaitForMove();
	Engine_EventWait(20);
	Engine_CameraFollowActor(ACTOR_PARTY_LEADER, 1);
	Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x8000, 0x4000);
	Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x1d7, 0x136);
	Engine_EventWait(20);
	Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
	Engine_EventWait(20);
	zero = 0;
	leader = Actor_Get(ACTOR_PARTY_LEADER);
	if (leader != NULL)
	{
		Actor_SetPosition(JASMINE, leader->x.fixed, leader->z.fixed);
	}
	leader = Actor_Get(ACTOR_PARTY_LEADER);
	if (leader != NULL)
	{
		Actor_SetPosition(GERALD, leader->x.fixed, leader->z.fixed);
	}
	Actor_SetSpeed(GERALD, 0x8000, 0x4000);
	Actor_SetSpeed(JASMINE, 0x8000, 0x4000);
	Actor_WalkTo(JASMINE, 0x1c5, 0x12e);
	Actor_WalkToAndWait(GERALD, 0x1e9, 0x12e);
	Engine_ActorSetAnimation(JASMINE, 1);
	Actor_FaceDirection(GERALD, 0x3000, 0);
	Actor_FaceDirection(JASMINE, FACING_SOUTHWEST, 10);
	Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x100, 0);
	Actor_ShowEmote(JASMINE, 0x100, 0);
	Actor_ShowEmote(GERALD, 0x100, 0);
	Engine_ActorJump(ACTOR_PARTY_LEADER, 2, 0);
	Engine_ActorJump(JASMINE, 2, 0);
	Engine_ActorJump(GERALD, 2, 40);
	Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 3);
	Engine_ActorStartRepeatedMotion(JASMINE, 3);
	Engine_ActorRunRepeatedMotion(GERALD, 3);
	Engine_EventWait(40);
	Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
	Actor_SetAttachedEffect(JASMINE, 0x102);
	Actor_SetAttachedEffect(GERALD, 0x102);
	Engine_EventWait(60);
	Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x13333, 0x9999);
	Actor_SetSpeed(JASMINE, 0x13333, 0x9999);
	Actor_SetSpeed(GERALD, 0x13333, 0x9999);
	Actor_WalkTo(ACTOR_PARTY_LEADER, 0x1d7, 0x15a);
	Actor_WalkTo(JASMINE, 0x1af, 0x152);
	Actor_WalkTo(GERALD, 0x1ff, 0x152);
	Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 1);
	Engine_ActorWaitForMove(JASMINE);
	Engine_ActorSetAnimation(JASMINE, 1);
	Engine_ActorWaitForMove(GERALD);
	Engine_ActorSetAnimation(GERALD, 1);
	Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x4ccc, 0x2666);
	Actor_SetSpeed(JASMINE, 0x4ccc, 0x2666);
	Actor_SetSpeed(GERALD, 0x4ccc, 0x2666);
	Actor_FaceDirection(JASMINE, FACING_NORTH, 0);
	Actor_FaceDirection(GERALD, FACING_NORTH, 0);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 60);
	Actor_FaceDirection(JASMINE, FACING_WEST, 0);
	Actor_FaceDirection(GERALD, FACING_EAST, 0);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 60);
	Actor_FaceDirection(JASMINE, FACING_EAST, 0);
	Actor_FaceDirection(GERALD, FACING_WEST, 40);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 40);
	Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
	Engine_ActorRunRepeatedMotion(GERALD, 2);
	Engine_EventWait(20);
	Engine_ActorSetAnimationAndWait(GERALD, 4);
	Engine_EventWait(20);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 40);
	Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
	Engine_ActorRunRepeatedMotion(JASMINE, 2);
	Engine_EventWait(20);
	Actor_ShowEmote(JASMINE, 0x101, 60);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
	Actor_FaceDirection(GERALD, 0x3000, 0);
	Actor_FaceDirection(JASMINE, FACING_SOUTHWEST, 60);
	rising = Actor_Get(ACTOR_SUKURETA);
	sukureta_sprite = &rising->sprite->flags;
	*sukureta_sprite = zero;
	rising->scale_x = 0x1999;
	rising->scale_y = 0x1999;
	glow->scale_x = 0x1999;
	glow->scale_y = 0x1999;
	Actor_SetChildValue(ACTOR_SUKURETA, 0x100);
	Actor_SetPosition(ACTOR_SUKURETA, 0x1d70000, 0x1220000);
	sukureta_motion = &rising->motion_flags;
	*sukureta_motion = zero;
	rising->y.fixed = 0x280000;
	Engine_EventWait(1);
	Engine_EventSetMessage((s32)MsgSoruSukuretaWhereAreWe);
	Event_ShowMessage(ACTOR_SUKURETA, 0);
	Engine_ActorJump(ACTOR_PARTY_LEADER, 4, 0);
	Engine_ActorJump(JASMINE, 4, 0);
	Engine_ActorJump(GERALD, 4, 40);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	Actor_FaceDirection(5, 0xe000, 0);
	Actor_FaceDirection(GERALD, 0xb000, 0);
	Camera_SetSpeed(0x20000, 0x4000);
	Camera_MoveTo(0x1d70000, -1, 0x1350000, 1);
	Engine_CameraWaitForMove();
	Actor_SetPosition(8, 0x1d70000, 0x1220000);
	Event_PlayCue(190);
	Engine_ActorSetSpritePriority(ACTOR_SUKURETA, 2);
	for (i = 0; i != 90; i++)
	{
		rising->y.fixed += -0x1999;
		rising->scale_x += 0x28f;
		rising->scale_y += 0x28f;
		glow->scale_x += 0x28f;
		glow->scale_y += 0x28f;
		Engine_EventWait(1);
	}
	*sukureta_motion = 5;
	Engine_EventWait(80);
	for (i = 0; i != 60; i++)
	{
		rising->y.fixed += -0x8000;
		Engine_EventWait(1);
	}
	*sukureta_motion = 3;
	Engine_EventWait(30);
	Engine_ActorSetSpritePriority(ACTOR_SUKURETA, 1);
	{
		struct FieldActor *actor = Actor_Get(ACTOR_SUKURETA);
		s32				   flags = 1 | actor->priority_flags;
		actor->priority_flags = flags;
	}
	Actor_SetChildValue(ACTOR_SUKURETA, 0);
	{
		s32 shown = 1;

		*sukureta_sprite = shown;
	}
	Actor_SetPosition(8, 0, 0);
	Engine_EventWait(30);
	Actor_SetSpeed(ACTOR_SUKURETA, 0x13333, 0x9999);
	Actor_WalkToAndWait(ACTOR_SUKURETA, 0x1d7, 0x132);
	Engine_EventWait(20);
	Actor_ShowEmote(ACTOR_SUKURETA, 0x100, 0);
	Engine_ActorJump(ACTOR_SUKURETA, 2, 80);
	Engine_ActorRunRepeatedMotion(ACTOR_SUKURETA, 3);
	Engine_EventWait(30);
	Actor_FaceDirection(ACTOR_SUKURETA, FACING_WEST, 30);
	Engine_ActorRunRepeatedMotion(ACTOR_SUKURETA, 2);
	Engine_EventWait(20);
	Actor_FaceDirection(ACTOR_SUKURETA, FACING_EAST, 30);
	Engine_ActorRunRepeatedMotion(ACTOR_SUKURETA, 2);
	Engine_EventWait(20);
	Actor_FaceDirection(ACTOR_SUKURETA, FACING_SOUTH, 30);
	Engine_ActorRunRepeatedMotion(ACTOR_SUKURETA, 3);
	Engine_EventWait(30);
	Event_SayThenWait(9, 20);
	Actor_SetSpeed(ACTOR_SUKURETA, 0x26666, 0x13333);
	Actor_WalkToAndWait(ACTOR_SUKURETA, 0x1a7, 0x132);
	Actor_FaceDirection(ACTOR_SUKURETA, 0xb000, 0);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xb000, 0);
	Actor_FaceDirection(JASMINE, FACING_NORTHWEST, 0);
	Actor_FaceDirection(GERALD, 0xb000, 30);
	Engine_ActorRunRepeatedMotion(ACTOR_SUKURETA, 2);
	Engine_EventWait(30);
	Engine_ActorSetAnimationAndWait(ACTOR_SUKURETA, 3);
	Engine_EventWait(40);
	Actor_WalkToAndWait(ACTOR_SUKURETA, 0x207, 0x132);
	Actor_FaceDirection(ACTOR_SUKURETA, 0xd000, 0);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xd000, 0);
	Actor_FaceDirection(JASMINE, FACING_NORTHEAST, 0);
	Actor_FaceDirection(GERALD, 0xd000, 20);
	Engine_ActorRunRepeatedMotion(ACTOR_SUKURETA, 2);
	Engine_EventWait(30);
	say_4009 = 0x4009;
	Engine_ActorSetAnimationAndWait(ACTOR_SUKURETA, 4);
	Engine_EventWait(40);
	Event_SayThenWait(say_4009, 30);
	Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, JASMINE, 0);
	Engine_EventWait(40);
	Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
	Engine_ActorRunRepeatedMotion(JASMINE, 2);
	Actor_ShowEmote(GERALD, 0x101, 40);
	Event_SayThenWait(1, 40);
	Actor_FaceDirection(ACTOR_SUKURETA, 0x5000, 20);
	Event_SayThenWait(say_4009, 20);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xd000, 0);
	Actor_FaceDirection(JASMINE, FACING_NORTHEAST, 20);
	Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 0);
	Actor_ShowEmote(JASMINE, 0x101, 40);
	Engine_ActorSetAnimationAndWait(ACTOR_SUKURETA, 4);
	Event_SayThenWait(say_4009, 10);
	Engine_ActorSetAnimation(GERALD, 3);
	Engine_ActorSetAnimation(JASMINE, 3);
	Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
	Engine_EventWait(20);
	Engine_ActorSetAnimationAndWait(ACTOR_SUKURETA, 3);
	Engine_EventWait(40);
	Camera_SetSpeed(0x8000, 0x1000);
	Camera_MoveTo(0x2150000, -1, 0x1530000, 1);
	Actor_SetSpeed(ACTOR_SUKURETA, 0x8000, 0x4000);
	Actor_WalkToAndWait(ACTOR_SUKURETA, 0x215, 0x153);
	Engine_CameraWaitForMove();
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
	Actor_FaceDirection(JASMINE, FACING_EAST, 0);
	Actor_FaceDirection(GERALD, FACING_EAST, 20);
	Actor_FaceDirection(ACTOR_SUKURETA, 0xd000, 40);
	Actor_FaceDirection(ACTOR_SUKURETA, FACING_EAST, 40);
	Actor_FaceDirection(ACTOR_SUKURETA, 0x3000, 60);
	Engine_ActorRunRepeatedMotion(ACTOR_SUKURETA, 3);
	Engine_EventWait(30);
	Engine_CameraFollowActor(ACTOR_SUKURETA, 1);
	Actor_SetSpeed(ACTOR_SUKURETA, 0x19999, 0xcccc);
	Actor_WalkTo(ACTOR_SUKURETA, 0x1c7, 0x168);
	Engine_EventWait(20);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
	Actor_FaceDirection(JASMINE, FACING_SOUTHEAST, 0);
	Actor_FaceDirection(GERALD, 0x5000, 0);
	Actor_WalkToAndWait(ACTOR_SUKURETA, 0x1c7, 0x168);
	Engine_EventWait(20);
	Actor_FaceDirection(ACTOR_SUKURETA, 0x5000, 30);
	Engine_ActorRunRepeatedMotion(ACTOR_SUKURETA, 3);
	Engine_EventWait(30);
	Actor_WalkToAndWait(ACTOR_SUKURETA, 0x1d7, 0x168);
	Engine_EventWait(20);
	Actor_FaceDirection(ACTOR_SUKURETA, 0x3000, 30);
	Actor_ShowEmote(ACTOR_SUKURETA, 0x100, 0);
	Engine_ActorRunRepeatedMotion(ACTOR_SUKURETA, 2);
	Engine_EventWait(30);
	Event_SayThenWait(9, 30);
	Engine_ActorRunRepeatedMotion(ACTOR_SUKURETA, 3);
	Engine_EventWait(20);
	Event_SayThenWait(9, 30);
	Actor_SetSpeed(JASMINE, 0xcccc, 0x6666);
	Actor_WalkToAndWait(JASMINE, 0x1b8, 0x15a);
	Engine_EventWait(10);
	Actor_FaceDirection(JASMINE, FACING_SOUTHEAST, 10);
	Event_SayThenWait(5, 20);
	Actor_SetSpeed(GERALD, 0xcccc, 0x6666);
	Actor_WalkToAndWait(GERALD, 0x1ef, 0x15a);
	Engine_CameraWaitForMove();
	Engine_EventWait(10);
	Actor_FaceDirection(GERALD, FACING_SOUTHWEST, 10);
	Event_OpenMessage(GERALD, 0);
	Actor_FaceDirection(JASMINE, FACING_EAST, 0);
	Actor_FaceDirection(GERALD, FACING_WEST, 0);
	if (Engine_EventChooseYesNo(0, 0) == 1)
	{
		gEventWork->message++;
	}
	Actor_FaceDirection(ACTOR_SUKURETA, FACING_NORTH, 30);
	Event_SayThenWait(ACTOR_SUKURETA, 30);
	Engine_EventSetMessage((s32)MsgSoruSukuretaTheElementsAreThe);
	Actor_FaceDirection(JASMINE, FACING_SOUTHEAST, 0);
	Actor_FaceDirection(GERALD, FACING_SOUTHWEST, 30);
	Engine_ActorRunRepeatedMotion(ACTOR_SUKURETA, 2);
	Event_SayThenWait(9, 10);
	Engine_ActorSetAnimationAndWait(ACTOR_SUKURETA, 3);
	Event_SayThenWait(9, 20);
	Engine_ActorRunRepeatedMotion(ACTOR_SUKURETA, 2);
	Event_SayThenWait(9, 10);
	Actor_SetAttachedEffect(ACTOR_SUKURETA, 0x102);
	Engine_EventWait(40);
	Event_SayThenWait(9, 10);
	Actor_FaceDirection(ACTOR_SUKURETA, FACING_SOUTHEAST, 20);
	Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
	Engine_ActorStartRepeatedMotion(JASMINE, 2);
	Engine_ActorRunRepeatedMotion(GERALD, 2);
	Engine_EventWait(10);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 0);
	Actor_FaceDirection(JASMINE, FACING_SOUTHEAST, 0);
	Actor_FaceDirection(GERALD, FACING_SOUTHEAST, 20);
	Engine_ActorStop(ACTOR_SUKURETA);
	Engine_ActorStop(ACTOR_PARTY_LEADER);
	Engine_ActorStop(JASMINE);
	Engine_ActorStop(GERALD);
	Camera_SetSpeed(0x80000, 0x10000);
	Camera_MoveTo(0x2c70000, -1, 0x1d00000, 1);
	Engine_CameraWaitForMove();
	Actor_SetPosition(ACTOR_SUKURETA, 0x24d0000, 0x1610000);
	Engine_EventWait(40);
	Event_ShowMessage(0x1009, 0);
	Actor_SetPosition(ACTOR_SUKURETA, 0x1d70000, 0x1680000);
	Engine_EventWait(80);
	Camera_MoveTo(0x1d70000, -1, 0x1720000, 1);
	Engine_CameraWaitForMove();
	Engine_EventWait(10);
	Engine_ActorRunRepeatedMotion(ACTOR_SUKURETA, 3);
	Engine_EventWait(10);
	Actor_FaceDirection(ACTOR_SUKURETA, 0xd000, 20);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHEAST, 0);
	Actor_FaceDirection(JASMINE, FACING_NORTHEAST, 0);
	Actor_FaceDirection(GERALD, FACING_NORTHEAST, 20);
	Camera_MoveTo(0x2c70000, -1, 0x930000, 1);
	Engine_CameraWaitForMove();
	Actor_SetPosition(ACTOR_SUKURETA, 0x2540000, 0xee0000);
	Engine_EventWait(40);
	Event_ShowMessage(0x1009, 0);
	Actor_SetPosition(ACTOR_SUKURETA, 0x1d70000, 0x1680000);
	Engine_EventWait(80);
	Camera_MoveTo(0x1d70000, -1, 0x1720000, 1);
	Engine_CameraWaitForMove();
	Engine_EventWait(10);
	Engine_ActorRunRepeatedMotion(ACTOR_SUKURETA, 2);
	Engine_EventWait(10);
	Engine_ActorJump(ACTOR_SUKURETA, 4, 0);
	Actor_FaceDirection(ACTOR_SUKURETA, 0xb000, 20);
	Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
	Engine_ActorStartRepeatedMotion(JASMINE, 1);
	Engine_ActorRunRepeatedMotion(GERALD, 1);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
	Actor_FaceDirection(JASMINE, FACING_NORTHWEST, 0);
	Actor_FaceDirection(GERALD, FACING_NORTHWEST, 20);
	Camera_MoveTo(0xe70000, -1, 0x930000, 1);
	Engine_CameraWaitForMove();
	Actor_SetPosition(ACTOR_SUKURETA, 0x1340000, 0xfa0000);
	Engine_EventWait(40);
	Event_ShowMessage(0x2009, 0);
	Actor_SetPosition(ACTOR_SUKURETA, 0x1d70000, 0x1680000);
	Engine_EventWait(80);
	Camera_MoveTo(0x1d70000, -1, 0x1720000, 1);
	Engine_CameraWaitForMove();
	Engine_EventWait(10);
	Engine_ActorRunRepeatedMotion(ACTOR_SUKURETA, 3);
	Engine_EventWait(10);
	Engine_ActorJump(ACTOR_SUKURETA, 6, 0);
	Actor_FaceDirection(ACTOR_SUKURETA, 0x5000, 20);
	Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
	Engine_ActorStartRepeatedMotion(JASMINE, 2);
	Engine_ActorRunRepeatedMotion(GERALD, 2);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
	Actor_FaceDirection(JASMINE, FACING_SOUTHWEST, 0);
	Actor_FaceDirection(GERALD, FACING_SOUTHWEST, 20);
	Camera_MoveTo(0xe70000, -1, 0x1d00000, 1);
	Engine_CameraWaitForMove();
	Actor_SetPosition(ACTOR_SUKURETA, 0x1320000, 0x16a0000);
	Engine_EventWait(40);
	Event_ShowMessage(0x2009, 0);
	Actor_SetPosition(ACTOR_SUKURETA, 0x1d70000, 0x1680000);
	Engine_EventWait(80);
	Camera_MoveTo(0x1d70000, -1, 0x1720000, 1);
	Engine_CameraWaitForMove();
	Engine_EventWait(30);
	Actor_ShowEmote(ACTOR_SUKURETA, 0x104, 60);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
	Actor_FaceDirection(JASMINE, FACING_SOUTHEAST, 0);
	Actor_FaceDirection(GERALD, FACING_SOUTHWEST, 40);
	Actor_ShowEmote(JASMINE, 0x102, 40);
	Engine_ActorRunRepeatedMotion(JASMINE, 1);
	Event_SayThenWait(5, 20);
	Engine_ActorJump(ACTOR_SUKURETA, 4, 0);
	Actor_FaceDirection(ACTOR_SUKURETA, 0xb000, 20);
	Engine_ActorRunRepeatedMotion(ACTOR_SUKURETA, 2);
	Event_SayThenWait(0xa009, 10);
	Engine_ActorStartRepeatedMotion(ACTOR_SUKURETA, 3);
	Event_SayThenWait(0xa009, 20);
	Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 0);
	Actor_ShowEmote(JASMINE, 0x101, 0);
	Actor_ShowEmote(GERALD, 0x101, 60);
	Actor_FaceDirection(ACTOR_SUKURETA, FACING_NORTH, 20);
	Engine_ActorSetAnimation(ACTOR_SUKURETA, 4);
	Event_OpenMessage(0x8009, 0);
	if (Engine_EventChooseYesNo(0, 0) == 1)
	{
		gEventWork->message++;
	}
	Engine_EventWait(20);
	Actor_FaceDirection(GERALD, FACING_WEST, 10);
	Event_SayThenWait(GERALD, 10);
	Engine_EventSetMessage((s32)MsgSoruSukuretaTheWisdomStone);
	Actor_FaceDirection(GERALD, FACING_SOUTHWEST, 20);
	Engine_ActorJump(ACTOR_SUKURETA, 4, 40);
	Event_SayThenWait(0x8009, 10);
	Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 0);
	Actor_ShowEmote(JASMINE, 0x101, 0);
	Actor_ShowEmote(GERALD, 0x101, 80);
	Actor_SetAttachedEffect(ACTOR_SUKURETA, 0x102);
	Engine_EventWait(40);
	Event_SayThenWait(0x8009, 40);
	Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x106, 0);
	Actor_ShowEmote(JASMINE, 0x106, 0);
	Actor_ShowEmote(GERALD, 0x106, 60);
	Actor_FaceDirection(JASMINE, FACING_EAST, 0);
	Actor_FaceDirection(GERALD, FACING_WEST, 20);
	Event_OpenMessage(0x8009, 0);
	if (Engine_EventChooseYesNo(0, 0) == 1)
	{
		gEventWork->message++;
	}
	Engine_ActorRunRepeatedMotion(ACTOR_SUKURETA, 1);
	Engine_EventWait(20);
	Actor_FaceDirection(JASMINE, FACING_SOUTHEAST, 0);
	Actor_FaceDirection(GERALD, 0x5000, 20);
	Event_SayThenWait(0x8009, 40);
	Engine_EventSetMessage((s32)MsgSoruTheStarsHaveEnormousPower);
	Engine_ActorSetAnimation(JASMINE, 4);
	Event_SayThenWait(5, 10);
	Actor_FaceDirection(ACTOR_SUKURETA, 0xb000, 10);
	Engine_ActorRunRepeatedMotion(ACTOR_SUKURETA, 1);
	Engine_ActorJump(ACTOR_SUKURETA, 4, 40);
	Event_SayThenWait(0xa009, 10);
	Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
	Engine_ActorStartRepeatedMotion(JASMINE, 1);
	Engine_ActorRunRepeatedMotion(GERALD, 1);
	Engine_EventWait(40);
	Actor_FaceDirection(ACTOR_SUKURETA, FACING_NORTH, 10);
	Engine_ActorRunRepeatedMotion(ACTOR_SUKURETA, 1);
	Event_SayThenWait(0x8009, 40);
	Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x105, 0);
	Actor_ShowEmote(JASMINE, 0x105, 0);
	Actor_ShowEmote(GERALD, 0x105, 120);
	Actor_ShowEmote(GERALD, 0x107, 0);
	Engine_ActorJump(GERALD, 4, 40);
	Event_SayThenWait(1, 10);
	Engine_ActorRunRepeatedMotion(ACTOR_SUKURETA, 1);
	Engine_EventWait(40);
	Actor_FaceDirection(ACTOR_SUKURETA, FACING_SOUTH, 80);
	Event_SayThenWait(0x8009, 10);
	Actor_FaceDirection(GERALD, FACING_WEST, 0);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 30);
	Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
	Engine_ActorRunRepeatedMotion(GERALD, 2);
	Engine_EventWait(20);
	Actor_FaceDirection(JASMINE, FACING_EAST, 0);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 30);
	Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
	Engine_ActorRunRepeatedMotion(JASMINE, 2);
	Engine_EventWait(20);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
	Actor_FaceDirection(JASMINE, FACING_SOUTHEAST, 0);
	Actor_FaceDirection(GERALD, 0x5000, 40);
	Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
	Actor_SetAttachedEffect(JASMINE, 0x102);
	Actor_SetAttachedEffect(GERALD, 0x102);
	Engine_EventWait(80);
	Engine_ActorSetAnimationAndWait(ACTOR_SUKURETA, 3);
	Engine_EventWait(20);
	Event_SayThenWait(0x8009, 40);
	rising = Actor_Get(ACTOR_SUKURETA);
	height = 192;
	Engine_TaskWait(6);
	rising->speed = 0x30000;
	rising->acceleration = 0x20000;
	Event_PlayCue(153);
	rising->velocity_y = (height << 11);
	Actor_MoveToAndWait(ACTOR_SUKURETA, 0x1d7, 0x18b);
	Engine_TaskWait(6);
	Actor_SetSpeed(ACTOR_SUKURETA, 0x4ccc, 0x2666);
	sukureta_flags = &rising->steering_flags;
	*sukureta_flags &= 254;
	Actor_SetDestination(ACTOR_SUKURETA, 0x1d9, 0x18b);
	Engine_ActorWaitForMove(ACTOR_SUKURETA);
	Engine_ActorRunRepeatedMotion(ACTOR_SUKURETA, 2);
	Actor_SetDestination(ACTOR_SUKURETA, 0x1d5, 0x18b);
	Engine_ActorWaitForMove(ACTOR_SUKURETA);
	Engine_ActorRunRepeatedMotion(ACTOR_SUKURETA, 2);
	Actor_SetDestination(ACTOR_SUKURETA, 0x1d7, 0x18b);
	Engine_ActorWaitForMove(ACTOR_SUKURETA);
	Event_SayThenWait(9, 10);
	Actor_SetSpeed(ACTOR_SUKURETA, 0xcccc, 0x6666);
	Actor_WalkToAndWait(ACTOR_SUKURETA, 0x1d7, 0x19b);
	Actor_SetSpeed(ACTOR_SUKURETA, 0x4ccc, 0x2666);
	*sukureta_flags &= 254;
	Actor_SetDestination(ACTOR_SUKURETA, 0x1da, 0x19b);
	Engine_ActorWaitForMove(ACTOR_SUKURETA);
	Engine_ActorRunRepeatedMotion(ACTOR_SUKURETA, 3);
	Actor_SetDestination(ACTOR_SUKURETA, 0x1d4, 0x19b);
	Engine_ActorWaitForMove(ACTOR_SUKURETA);
	Engine_ActorRunRepeatedMotion(ACTOR_SUKURETA, 3);
	Actor_SetDestination(ACTOR_SUKURETA, 0x1d7, 0x19b);
	Engine_ActorWaitForMove(ACTOR_SUKURETA);
	Actor_SetAttachedEffect(ACTOR_SUKURETA, 0x102);
	Engine_ActorStartRepeatedMotion(ACTOR_SUKURETA, 3);
	Event_SayThenWait(9, 10);
	Actor_SetSpeed(ACTOR_SUKURETA, 0x3333, 0x1999);
	Actor_WalkToAndWait(ACTOR_SUKURETA, 0x1d7, 0x18b);
	Engine_ActorEnableActionCallback(ACTOR_SUKURETA, 1);
	Engine_EventWait(30);
	{
		s32 flags = 1 | *sukureta_flags;
		*sukureta_flags = flags;
	}
	Actor_FaceDirection(ACTOR_SUKURETA, FACING_NORTH, 60);
	Engine_TaskWait(6);
	rising->speed = 0x30000;
	rising->acceleration = 0x20000;
	Event_PlayCue(153);
	rising->velocity_y = (height << 11);
	Actor_MoveToAndWait(ACTOR_SUKURETA, 0x1d7, 0x168);
	Engine_TaskWait(6);
	Engine_EventWait(40);
	Actor_ShowEmote(ACTOR_SUKURETA, 0x102, 80);
	Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x102, 0);
	Actor_ShowEmote(JASMINE, 0x102, 0);
	Actor_ShowEmote(GERALD, 0x102, 80);
	Engine_ActorRunRepeatedMotion(JASMINE, 1);
	Actor_ShowEmote(JASMINE, 0x107, 40);
	Event_SayThenWait(5, 20);
	Actor_FaceDirection(ACTOR_SUKURETA, 0xb000, 40);
	Engine_ActorSetAnimationAndWait(ACTOR_SUKURETA, 4);
	Event_SayThenWait(0xa009, 30);
	Actor_FaceDirection(JASMINE, FACING_EAST, 30);
	Actor_ShowEmote(JASMINE, 0x106, 60);
	Actor_FaceDirection(JASMINE, FACING_SOUTHEAST, 30);
	Event_SayThenWait(5, 20);
	Engine_ActorJump(ACTOR_PARTY_LEADER, 4, 0);
	Engine_ActorJump(GERALD, 4, 0);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
	Actor_FaceDirection(GERALD, FACING_WEST, 40);
	Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
	Engine_ActorRunRepeatedMotion(GERALD, 1);
	Engine_EventWait(40);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 30);
	Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
	Engine_ActorRunRepeatedMotion(GERALD, 2);
	Engine_EventWait(30);
	Actor_FaceDirection(ACTOR_SUKURETA, 0xd000, 30);
	Engine_ActorRunRepeatedMotion(ACTOR_SUKURETA, 1);
	Engine_EventWait(20);
	Event_SayThenWait(0x8009, 20);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
	Actor_FaceDirection(GERALD, 0x5000, 30);
	Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
	Engine_ActorRunRepeatedMotion(GERALD, 2);
	Engine_EventWait(20);
	Engine_ActorSetAnimationAndWait(JASMINE, 3);
	Engine_EventWait(20);
	Actor_FaceDirection(JASMINE, FACING_EAST, 30);
	Event_OpenMessage(JASMINE, 0);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
	while (Engine_EventChooseYesNo(0, 0) != 0)
	{
		Engine_EventSetMessage((s32)MsgSoruJasmineWontYouPlease);
		Actor_ShowEmote(JASMINE, 0x107, 0);
		Engine_ActorJump(JASMINE, 4, 60);
		Event_OpenMessage(JASMINE, 0);
	}
	Engine_EventSetMessage((s32)MsgSoruJasmineThankYou);
	Engine_EventWait(20);
	Engine_ActorSetAnimationAndWait(JASMINE, 3);
	Engine_EventWait(20);
	Actor_FaceDirection(JASMINE, FACING_SOUTHEAST, 30);
	Event_SayThenWait(5, 10);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 10);
	Actor_FaceDirection(ACTOR_SUKURETA, 0xb000, 30);
	Engine_ActorSetAnimationAndWait(ACTOR_SUKURETA, 3);
	Engine_EventWait(20);
	Actor_FaceDirection(ACTOR_SUKURETA, FACING_NORTH, 30);
	Actor_SetSpeed(ACTOR_SUKURETA, 0x3333, 0x1999);
	Actor_WalkToAndWait(ACTOR_SUKURETA, 0x1d7, 0x160);
	Engine_EventWait(20);
	say_8009 = 0x8009;
	Engine_ActorRunRepeatedMotion(ACTOR_SUKURETA, 2);
	Engine_EventWait(20);
	Event_SayThenWait(say_8009, 60);
	*sukureta_flags &= 254;
	Actor_WalkToAndWait(ACTOR_SUKURETA, 0x1c8, 0x168);
	Engine_EventWait(20);
	Engine_ActorRunRepeatedMotion(JASMINE, 2);
	Engine_EventWait(20);
	Engine_EventSetMessage((s32)MsgSoruTheBagsAreFromSukuretas);
	Event_SayThenWait(5, 30);
	Actor_FaceDirection(ACTOR_SUKURETA, 0xb000, 30);
	Engine_ActorSetAnimationAndWait(ACTOR_SUKURETA, 3);
	Engine_EventWait(20);
	Actor_ShowEmote(GERALD, 0x101, 60);
	Event_SayThenWait(1, 10);
	Actor_FaceDirection(ACTOR_SUKURETA, 0xd000, 20);
	Engine_ActorSetAnimationAndWait(ACTOR_SUKURETA, 3);
	Event_SayThenWait(say_8009, 20);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
	Actor_FaceDirection(GERALD, FACING_WEST, 40);
	Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 0);
	Actor_ShowEmote(GERALD, 0x101, 40);
	Engine_ActorRunRepeatedMotion(ACTOR_SUKURETA, 2);
	Event_SayThenWait(say_8009, 10);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
	Actor_FaceDirection(GERALD, 0x5000, 30);
	Engine_ActorSetAnimationAndWait(ACTOR_SUKURETA, 3);
	Engine_EventWait(10);
	Event_SayThenWait(say_8009, 30);
	Engine_ActorSetAnimation(GERALD, 3);
	Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
	Engine_EventWait(20);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
	Actor_FaceDirection(GERALD, FACING_WEST, 30);
	Engine_ActorRunRepeatedMotion(GERALD, 2);
	Engine_EventWait(30);
	Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
	Engine_EventWait(10);
	Actor_SetSpeed(GERALD, 0x10000, 0x8000);
	Engine_ActorSetAnimation(GERALD, 2);
	leader = Object_GetById(ACTOR_PARTY_LEADER);
	if (leader != NULL)
	{
		Actor_SetDestination(GERALD, leader->x.part.pixel, leader->z.part.pixel);
	}
	Engine_ActorWaitForMove(GERALD);
	Actor_SetPosition(GERALD, 0, 0);
	GameFlag_Set(FLAG_STAR_ROOM_EXPLAINED);
	Party_RemoveOwnerRestored(5);
	Scene_GiveMythrilBags();
	Map_CopyCellAttributes(8, 0, 5, 1, 27, 17);
	gEventWork->transition_frames = 16;
	GameFlag_Clear(FLAG_ARRIVAL_EVENT_PENDING);
	Engine_EventEnd();
}

s32 SoruStar_ApplyEntryState(void)
{
	s32 base5_f;

	Engine_ColorBufferApplySource(0x10000, 0);
	Engine_GameFlagSet(0x144);
	base5_f = 15;
	do
	{
		Object_GetById(base5_f)->collision_flags = 0;
		Engine_ActorSetSpritePriority(base5_f++, 1);
	} while ((u32)base5_f <= 24);
	SoruStar_SetupElementalRings(15, 16);
	if (GameFlag_IsSet(0x83b) != 0)
	{
		Actor_SetPosition(9, 0x1c80000, 0x1680000);
		Actor_SetPosition(5, 0x1b80000, 0x15a0000);
	}
	if (Engine_GameFlagIsSet(0x83c) != 0)
	{
		Engine_MapCopyCellsTo(0, 40, 43, 66, 3, 3);
		Engine_MapCopyCellsTo(83, 40, 96, 29, 3, 4);
		Engine_MapCopyCellAttributes(0, 0, 1, 1, 41, 29);
		Map_CopyCellsTo(87, 42, 41, 31, 1, 2);
		Engine_MapCopyCellsTo(83, 40, 74, 29, 3, 4);
		Engine_MapCopyCellAttributes(0, 0, 1, 1, 19, 29);
		Engine_MapCopyCellsTo(87, 42, 19, 31, 1, 2);
		Engine_MapCopyCellsTo(83, 40, 96, 10, 3, 4);
		Map_CopyCellAttributes(0, 0, 1, 1, 41, 10);
		Engine_MapCopyCellsTo(87, 42, 41, 12, 1, 2);
	}
	if (Engine_GameFlagIsSet(0x83d) != 0)
	{
		Engine_MapCopyCellsTo(0, 40, 43, 46, 3, 3);
		Engine_MapCopyCellsTo(83, 40, 84, 4, 3, 4);
		Engine_MapCopyCellAttributes(0, 0, 1, 1, 29, 4);
		Map_CopyCellsTo(87, 42, 29, 6, 1, 2);
		Engine_MapCopyCellsTo(83, 40, 76, 21, 3, 4);
		Engine_MapCopyCellAttributes(0, 0, 1, 1, 21, 21);
		Engine_MapCopyCellsTo(87, 42, 21, 23, 1, 2);
		Engine_MapCopyCellsTo(83, 40, 76, 29, 3, 4);
		Engine_MapCopyCellAttributes(0, 0, 1, 1, 21, 29);
		Engine_MapCopyCellsTo(87, 42, 21, 31, 1, 2);
	}
	if (Engine_GameFlagIsSet(0x83e) != 0)
	{
		Engine_MapCopyCellsTo(0, 40, 13, 66, 3, 3);
		Engine_MapCopyCellsTo(83, 40, 65, 31, 3, 4);
		Map_CopyCellAttributes(0, 0, 1, 1, 10, 31);
		Map_CopyCellsTo(87, 42, 10, 33, 1, 2);
		Engine_MapCopyCellsTo(83, 40, 79, 9, 3, 4);
		Map_CopyCellAttributes(0, 0, 1, 1, 24, 9);
		Engine_MapCopyCellsTo(87, 42, 24, 11, 1, 2);
		Engine_MapCopyCellsTo(83, 40, 91, 10, 3, 4);
		Engine_MapCopyCellAttributes(0, 0, 1, 1, 36, 10);
		Engine_MapCopyCellsTo(87, 42, 36, 12, 1, 2);
		SoruStar_LineUpFollowers();
	}
	if (Engine_GameFlagIsSet(0x83b) == 0)
	{
		if (gGameState.entrance == 10)
		{
			Scene_EnterStarRoom();
		}
	}
	return 0;
}

static void Event_SayThenWait(s32 speaker, s32 frames)
{
	Engine_EventShowMessage(speaker, 0);
	Engine_EventWait(frames);
}

static s32 Scene_PresentItem(s32 item, s32 x, s32 y, s32 z)
{
	GlyphTransfer	   *glyph;
	struct FieldActor  *actor;
	struct FieldSprite *sprite;
	s32					owner;
	s32					slot;

	glyph = NULL;
	actor = Engine_ObjectCreate(22, x, y, z);
	owner = PartyInventory_FindOwner(224);
	slot = Inventory_Find(owner, 224);
	if (actor == NULL)
		return owner;
	Engine_ObjectSetScript(actor, SoruStar_PresentItemScript);
	sprite = actor->sprite;
	sprite->flags = 0;
	sprite->part_count = 0;
	sprite->full_color = 0;
	sprite->palette = 0;
	actor->velocity_y = 0x28000;
	actor->gravity = 0x4000;
	glyph = Engine_HeapAllocate(17, sizeof(*glyph));
	Engine_ItemLoadIcon(item);
	Engine_VramLoad(sprite->vram_block, 128, glyph->tiles);
	Engine_HeapRelease(17);
	Engine_AudioPlayCue(83);
	Engine_RunRisingObjectSequence(actor, 3);
	Inventory_Discard(owner, slot);
	Inventory_AddItem(owner, item);
	Engine_ObjectDispatchRelease(actor);
	Engine_ActorSetAnimation(0, 1);
	return owner;
}

static void Scene_GiveMythrilBags(void)
{
	u8 *work = (u8 *)gEventWork;
	s16 saved = *(s16 *)(work + 472);
	s32 first;
	s32 second;
	s32 cnt;

	Engine_AudioPlayCue(0x53);
	Engine_ItemShowFound(ITEM_MYTHRIL_BAG, 3);
	Engine_MessageShowCentered((s32)MsgSoruGotFourMythrilBags, 1);
	do
	{
		cnt = 30 - Inventory_Count(0);
		cnt -= Inventory_Count(1);

		if (cnt <= 3)
		{
			Engine_MessageShowCentered((s32)MsgSoruTooManyItems, 1);
			if (Shop_PickUnitItem(&second, &first) != -1)
				Inventory_Discard(second, first);
		}
	} while (cnt <= 3);
	PartyInventory_Add(ITEM_MYTHRIL_BAG);
	PartyInventory_Add(ITEM_MYTHRIL_BAG);
	PartyInventory_Add(ITEM_MYTHRIL_BAG);
	PartyInventory_Add(ITEM_MYTHRIL_BAG);
	*(s16 *)(work + 472) = saved;
}

static void SoruStar_RiseActorFourteenSparks(void)
{
	struct EffectParams	 params;
	struct EffectParams *p;
	struct Actor		*actor;
	u32					 i;
	s32					 x;
	s32					 r;

	actor = (struct Actor *)Object_GetById(14);
	Engine_AudioPlayCue(190);
	Engine_ActorSetSpriteFlags((struct Actor *)Object_GetById(14), 0);
	p = &params;
	p->count = 1;
	p->kind = 5;
	p->tile = 0x11c;
	p->spread = 0x6666;
	p->rise = 0x30000;
	for (i = 0; i <= 31; i++)
	{
		Engine_EventWait(1);
		if (!(i & 1))
		{
			r = (((u32)Engine_RandomNext() * 24) >> 16) << 16;
			x = actor->x;
			x += r;
			x += -0xc0000;
			Effect_Spawn(x, actor->y + ((((u32)Engine_RandomNext() << 5) >> 16) << 16) + 0x200000,
				actor->z, 0, -0x40000, 0, 0x1b0000, p);
		}
		if (i == 20)
			Engine_ActorSetChildValue(14, 0x100);
	}
	Engine_ActorSetChildValue(14, 0);
	Engine_ActorSetSpriteFlags((struct Actor *)Object_GetById(14), 1);
}

// ここに共通ソースが入る（ROM の並びのとおり）
#include "ring_setup.c"

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

s32 SoruStar_AngleScript[] = {
	SC_CALL(OverlayObject_UpdateFacingTowardTarget),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

s32 SoruStar_PresentItemScript[] = {
	SC_SET(SW_SCALE_X, 0x8000),
	SC_SET(SW_SCALE_Y, 0x8000),
	SC_WAIT(2),
	SC_SET(SW_SCALE_X, 0xc000),
	SC_SET(SW_SCALE_Y, 0xc000),
	SC_WAIT(2),
	SC_SET(SW_SCALE_X, 0xe000),
	SC_SET(SW_SCALE_Y, 0xe000),
	SC_WAIT(2),
	SC_SET(SW_SCALE_X, 0xf000),
	SC_SET(SW_SCALE_Y, 0xf000),
	SC_WAIT(2),
	SC_SET(SW_SCALE_X, 0x10000),
	SC_SET(SW_SCALE_Y, 0x10000),
	SC_WAIT(16),
	SC_SET(SW_SCALE_X, 0x16000),
	SC_SET(SW_SCALE_Y, 0x16000),
	SC_WAIT(2),
	SC_SET(SW_SCALE_X, 0x18000),
	SC_SET(SW_SCALE_Y, 0x18000),
	SC_WAIT(2),
	SC_SET(SW_SCALE_X, 0x12000),
	SC_SET(SW_SCALE_Y, 0x12000),
	SC_WAIT(2),
	SC_SET(SW_SCALE_X, 0x10000),
	SC_SET(SW_SCALE_Y, 0x10000),
	SC_WAIT(2),
	SC_SET(SW_SCALE_X, 0x16000),
	SC_SET(SW_SCALE_Y, 0x16000),
	SC_WAIT(2),
	SC_SET(SW_SCALE_X, 0x18000),
	SC_SET(SW_SCALE_Y, 0x18000),
	SC_WAIT(2),
	SC_SET(SW_SCALE_X, 0x12000),
	SC_SET(SW_SCALE_Y, 0x12000),
	SC_WAIT(2),
	SC_SET(SW_SCALE_X, 0x10000),
	SC_SET(SW_SCALE_Y, 0x10000),
	SC_WAIT(2),
	SC_END,
};

struct SceneEntrance Placement_Scripts[] = {
	{ 0, CONDITION_ALWAYS, 472, 0, 322, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ SCENE_TABLE_END },
};

u32 Placement_Messages[] = {
	SCENE_EXITS(17),
	SCENE_EXIT(2, 18, 10),
	SCENE_EXITS_END,
};

struct ScenePlacement Placement_Actors[] = {
	{ GERALD, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ JASMINE, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_SOUTH, 3, 0 },
	{ 0x16, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
	{ 0x1e, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_SOUTH, 3, 0 },
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
	{ 0x11f, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(617), 0, PIXELS(408), FACING_EAST, TALK_FACE_PARTY, 0 },
	{ 0x11f, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(429), 0, PIXELS(221), FACING_EAST, TALK_FACE_PARTY, 0 },
	{ 0x11f, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(696), 0, PIXELS(113), FACING_EAST, TALK_FACE_PARTY, 0 },
	{ 0x11f, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(170), 0, PIXELS(168), FACING_EAST, TALK_FACE_PARTY, 0 },
	{ 0x11f, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(308), 0, PIXELS(511), FACING_EAST, TALK_FACE_PARTY, 0 },
	{ 0x11f, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(397), 0, PIXELS(418), FACING_EAST, TALK_FACE_PARTY, 0 },
	{ 0x11f, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(542), 0, PIXELS(269), FACING_EAST, TALK_FACE_PARTY, 0 },
	{ 0x11f, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(421), 0, PIXELS(117), FACING_EAST, TALK_FACE_PARTY, 0 },
	{ 0x11f, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(295), 0, PIXELS(210), FACING_EAST, TALK_FACE_PARTY, 0 },
	{ 0x11f, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(742), 0, PIXELS(310), FACING_EAST, TALK_FACE_PARTY, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEvent Placement_Effects[] = {
	{ EVENT_TALK, 9, CONDITION_ALWAYS, EVENT_SCRIPT(EV09_TALK) },
	{ EVENT_TALK, 5, CONDITION_ALWAYS, EVENT_SCRIPT(EV05_SCRIPT) },
	{ EVENT_TALK, 10, CONDITION_ALWAYS, EVENT_SCRIPT(EV10_TALK) },
	{ EVENT_TALK, 11, CONDITION_ALWAYS, EVENT_SCRIPT(EV11_SCRIPT) },
	{ EVENT_TALK, 13, CONDITION_ALWAYS, EVENT_SCRIPT(EV13_TALK) },
	{ EVENT_TALK, 14, CONDITION_ALWAYS, EVENT_SCRIPT(EV14_TALK) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgSoruSukuretaIAmResponsible) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 5, CONDITION_ALWAYS, EVENT_SCRIPT(EV05_SCRIPT) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgSoruSaturosBringTheFinalStar) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, EVENT_SCRIPT(EV11_SCRIPT) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgSoruAlexOnlyOneLeft) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 1, CONDITION_ALWAYS, EVENT_SCRIPT(EV01_TOUCH) },
	{ TOUCH_WALKING_INTO(FACING_EAST), 10, CONDITION_ALWAYS, EVENT_SCRIPT(SoruStar_CheckLeaderAhead) },
	{ TOUCH_WALKING_INTO(FACING_SOUTH), 11, CONDITION_ALWAYS, EVENT_SCRIPT(SoruStar_CheckLeaderAhead) },
	{ SEARCH(SEARCH_UNNAMED), 2, 0x83c, EVENT_SCRIPT(EV02_SEARCH) },
	{ SEARCH(SEARCH_UNNAMED), 3, 0x83d, EVENT_SCRIPT(EV03_SEARCH) },
	{ SEARCH(SEARCH_UNNAMED), 4, 0x83e, EVENT_SCRIPT(EV04_SEARCH) },
	{ SEARCH(SEARCH_UNNAMED), 5, 0x83f, EVENT_SCRIPT(EV05_SEARCH) },
	{ SCENE_EVENTS_END },
};

u16 SoruStar_StarCells[] = {
	MAP_CELL_STEP(59, 40, 3, 4, 6), MAP_CELL_STEP(62, 40, 3, 4, 6), MAP_CELL_STEP(65, 40, 3, 4, 6), MAP_CELL_STEP(68, 40, 3, 4, 6), MAP_CELL_STEP(71, 40, 3, 4, 6), MAP_CELL_STEP(74, 40, 3, 4, 6), MAP_CELL_STEP(77, 40, 3, 4, 6), MAP_CELL_STEP(80, 40, 3, 4, 6), MAP_CELL_STEP(83, 40, 3, 4, 0), MAP_CELL_STEPS_END,
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
