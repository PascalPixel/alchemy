#include "game_state.h"
#include "global_cells.h"
#include "heap_state.h"
#include "keystate.h"
#include "unit/import.h"
#include "unit/field/makyuri_iriguchi/iriguchi.h"
#include "unit/field/common/effect/move.h"
#include "unit/field/common/makyuri/lights.h"
#include "unit/field/makyuri_iriguchi/pillar.h"
#include "unit/field/common/makyuri/makyuri.h"
#include "unit/battle/effect/object.h"
#include "unit/field/common/script/field.h"
#include "unit/graphics/window/window.h"
#include "unit/system/scheduler/runtime.h"
#include "unit/system/scheduler/runtime9.h"
#include "unit/system/scheduler/runtim10.h"
#include "unit/system/scheduler/runtim11.h"
#include "unit/system/scheduler/runtim13.h"
#include "iowrite.h"

#define NULL ((void *)0)
#include "fieldobj.h"
#include "mem.h"
#include "types.h"
#include "makyuri.h"
#include "field_scene.h"
#include "staged_actor.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "mapcopy.h"
#include "call.h"
#include "scene_ids.h"
#include "field_effect.h"
#include "io_reg.h"
#include "field_event.h"
#include "text/msg_ids.h"
#include "iwram_call.h"
#include "ability_ids.h"
#include "objcmd.h"

// 前方宣言
static void MakyuriIriguchi_CrossDoorway(void);
static void MakyuriIriguchi_RunDoorScene(void);

struct WorkPointers
{
	u8 *event;
	u8 *unknown_04[7];
	u8 *scene;
};

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
#include "../common/object/staged_actor.c"

static s32 MakyuriIriguchi_TrailSparks(struct FieldActor *a0)
{
	s32					 value;
	s32					 magic;
	struct EffectOptions options;

	options.palette = 7;
	if ((gFrameCount & 1) == 0)
	{
		options.palette = 5;
	}
	options.start_scale_x = 0xcccc;
	options.start_scale_y = 0xcccc;
	options.priority = 0;
	value = Engine_RandomNext();
	magic = -(((u32)(value << 3) >> 16) * 0x3333);
	Effect_Spawn((a0->x.fixed + ((8 - (gFrameCount & 15)) << 16)), (a0->y.fixed + 0x1a0000),
		a0->z.fixed, 0, magic, 0, 0xb0000, &options);
	return 0;
}

static s32 SceneAudio_PlayCue118AndReturnZero(void)
{
	Engine_AudioPlayCue(118);
	return 0;
}

static s32 SceneActor_FaceLeaderWhileGrounded(struct FieldActor *object)
{
	struct FieldActor *leader = Object_GetById(ACTOR_PARTY_LEADER);

	if ((leader->z.fixed >> 19) <= 22)
	{
		object->facing =
			ArcTan2(leader->z.fixed - object->z.fixed, leader->x.fixed - object->x.fixed);
	}
	else if (object->facing != 0xc000)
	{
		Engine_ActorFaceDirection(MEARI, 0xc000, 0);
	}
	return 0;
}

// ここに共通ソースが入る（ROM の並びのとおり）
#include "../common/makyuri/palette_cycle.c"

static void EV21_TOUCH(void)
{
	Makyuri_TickSpawnTimer();
}

static void EV31_TOUCH(void)
{
	SceneState_ClearCurrentRecordAndReleaseTarget();
}

static void FieldScene_RunSingleStep(void)
{
	Makyuri_RunActorMove();
}

static void EV11_TOUCH(void)
{
	MakyuriIriguchi_RaisePillar();
}

void *SceneData_GetSceneTableA(void)
{
	return MakyuriIriguchi_SceneTableA;
}

int SceneData_ReturnZero(void)
{
	return 0;
}

void *SceneData_GetSceneTableB(void)
{
	return MakyuriIriguchi_SceneTableB;
}

void *SceneData_GetSceneTableC(void)
{
	return MakyuriIriguchi_SceneTableC;
}

static void EV01_SEARCH(void)
{
	Engine_EventBegin();
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 1);
	Engine_MessageShowCentered((s32)MsgMakyuriWhoHonorsHeart, 1);
	Engine_EventEnd();
}

static void SceneActor_RunActorZeroHandledMotion(s32 a)
{
	struct FieldActor *v = Object_GetById(ACTOR_PARTY_LEADER);
	Engine_EventBegin();
	Engine_AudioPlayCue(0xe4);
	v->update = (void (*)(union FieldObject *))MakyuriIriguchi_TrailSparks;
	v->speed = 0x3333;
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 2);
	Engine_ActorSetDestinationOffset(ACTOR_PARTY_LEADER, 0, -6);
	Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
	Engine_ActorSetChildValue(ACTOR_PARTY_LEADER, 15);
	Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_PARTY_LEADER), 0);
	v->update = 0;
	Engine_EventWait(30);
	Engine_EventCloseScreen();
	Engine_EventWaitForScreen();
	Engine_EventRequestExit(a);
	Engine_EventEnd();
}

static void MakyuriIriguchi_DropLeaderToColumn(s32 a0)
{
	s32 record;

	Engine_EventBegin();
	Engine_AudioPlayCue(228);
	Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x6666, 0x3333);
	Engine_ActorSetSpritePriority(ACTOR_PARTY_LEADER, 2);
	Engine_ActorSetDestinationOffset(ACTOR_PARTY_LEADER, 0, -8);
	record = Actor_Get(ACTOR_PARTY_LEADER);
	Engine_ActorSetSpriteFlags(record, 0);
	Engine_EventWait(8);
	Actor_SetPosition(ACTOR_PARTY_LEADER, ((a0 << 19) + 0x80000), 0);
	Engine_EventWait(30);
}

static void EV67_TOUCH(void)
{
	SceneActor_RunActorZeroHandledMotion(17);
}

static void EV68_TOUCH(void)
{
	SceneActor_RunActorZeroHandledMotion(18);
}

static void EV69_TOUCH(void)
{
	SceneActor_RunActorZeroHandledMotion(19);
}

void *SceneData_GetSceneTableD(void)
{
	return MakyuriIriguchi_SceneTableD;
}

static void EV16_PSYNERGY(void)
{
	s32 a;
	s32 b;
	s32 zero;
	s32 counter;
	s32 x;
	s32 y;
	s32 t;
	s32 record;
	u8 *slot;
	u8	slot16[40];

	a = Object_GetById(ACTOR_PARTY_LEADER)->x.fixed / 0x100000;
	b = Object_GetById(ACTOR_PARTY_LEADER)->z.fixed / 0x100000;
	if (a == 12 && b == 32)
	{
		Engine_EventBegin();
		ColorBuffer_ApplyTarget(0x10000, 0);
		Engine_ColorBufferInterpolate(60);
		Engine_EventWait(120);
		ColorBuffer_ApplyTarget(0x10005, 1);
		Engine_ColorBufferInterpolate(60);
		Engine_EventWait(40);
		counter = 0;
		slot = slot16;
		zero = 0;
		do
		{
			*(s32 *)slot = 1;
			{
				u16 shown = 0x11e;

				*(u16 *)(slot + 24) = shown;
			}
			*(s32 *)(slot + 28) = (s32)MakyuriIriguchi_SparkBurstScript;
			Engine_AudioPlayCue(246);
			x = 208 - ((u32)(Random_Next() << 4) >> 16);
			y = 560 - ((u32)(Random_Next() << 4) >> 16);
			t = ((u32)(Engine_RandomNext() << 2) >> 16);
			record = Math_Divide((((t << 4) - t) << 16) + 0x3c0000, 100);
			Effect_Spawn(x << 16, 0, y << 16, 0, record, zero, 0x320001, slot);
			Engine_EventWait(4);
			counter++;
		} while ((u32)counter <= 14);
		Engine_AudioPlayCue(220);
		Engine_EventWait(60);
		GameFlag_Set(0x875);
		Engine_TaskAddCallback((s32)Makyuri_CyclePalette, 0xc80);
		Map_CopyCellsTo(37, 98, 10, 97, 5, 3);
		Map_CopyCellAttributes(70, 32, 13, 7, 6, 32);
		ColorBuffer_ApplyTarget(0x10000, 0);
		Engine_ColorBufferInterpolate(60);
		Engine_EventWait(120);
		Engine_EventEnd();
	}
}

static void EV10_TOUCH(void)
{
	Engine_EventRequestExit(63);
}

static void MakyuriIriguchi_InstallActorEightPath(void)
{
	Engine_GameFlagClear(0x205);
	Engine_ActorEnableActionCallback(8, MakyuriIriguchi_Actor8Path1);
}

static void EV41_TOUCH(void)
{
	struct FieldActor *record;
	s32				   field8;
	s32				   quotient;

	record = Object_GetById(ACTOR_PARTY_LEADER);
	field8 = record->x.fixed;
	quotient = field8 / 0x100000;
	Engine_GameFlagSet(0x205);
	if (quotient == 7)
	{
		Engine_ActorEnableActionCallback(8, MakyuriIriguchi_Actor8Path2);
	}
	else
	{
		Engine_ActorEnableActionCallback(8, MakyuriIriguchi_Actor8Path3);
	}
}

static void MakyuriIriguchi_OpenEntrance(void)
{
	Engine_EventBegin();
	Engine_ActorRunRepeatedMotion(8, 2);
	Engine_EventWait(20);
	gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 0);
	Party_SetFields1ceAnd1d0((s32)&SceneId_MakyuriIriguchi, 31);
	Battle_SetStart(3);
	BattleFx_SetWeightedResult(36, 1);
	Engine_EventEnd();
}

static void MakyuriIriguchi_ArriveWithSparks(void)
{
	struct FieldActor *actor;
	s32				   flag;
	s32				   record;

	actor = Object_GetById(ACTOR_PARTY_LEADER);
	flag = GameFlag_IsSet(0x109);
	if (flag == 0)
	{
		Engine_EventBegin();
		Camera_MoveTo(-1, -1, -1, 0);
		actor->motion_flags = 0;
		Engine_ActorSetPosition(
			0, actor->x.part.pixel << 16, (actor->z.part.pixel << 16) + -0x100000);
		Actor_SetChildValue(ACTOR_PARTY_LEADER, 15);
		record = Actor_Get(ACTOR_PARTY_LEADER);
		Engine_ActorSetSpriteFlags(record, 0);
		Engine_EventOpenScreen();
		Engine_EventWaitForScreen();
		Engine_AudioPlayCue(228);
		actor->update = (void (*)(union FieldObject *))MakyuriIriguchi_TrailSparks;
		Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x6666, 0x3333);
		Actor_WalkByAndWait(ACTOR_PARTY_LEADER, 0, 8);
		Actor_SetChildValue(ACTOR_PARTY_LEADER, 0);
		record = Actor_Get(ACTOR_PARTY_LEADER);
		Engine_ActorSetSpriteFlags(record, 1);
		actor->sprite->priority = 1;
		Actor_WalkByAndWait(ACTOR_PARTY_LEADER, 0, 10);
		actor->motion_flags = 3;
		actor->update = NULL;
		BattleFx_PlayQueuedSound();
		Engine_EventEnd();
	}
}

s32 MakyuriIriguchi_ApplyEntryState(void)
{
	struct EffectOptions options;
	s32					 velocity[3];
	struct FieldActor	*actor;
	u32					 i;
	s32					 angle;
	s32					 flag;

	Io_Write16(0x3f40, &REG_BLDCNT);
	{
		s32 alpha = 0x1010;
		REG_BLDALPHA = alpha;
	}
	Makyuri_SpawnLightObjects(21, (struct MakyuriLights *)gSceneState);
	GameFlag_Set(0x111);
	gGameState.retreat_entrance = 11;
	gGameState.retreat_scene = (s32)&SceneId_MakyuriHeya4;
	BattleFx_StartFadeOverlay(0);
	if (GameFlag_IsSet(0x875))
		Engine_TaskAddCallback(Makyuri_CyclePalette, 0xc80);
	else
		Makyuri_ClearPalette();
	gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
	switch (gGameState.entrance)
	{
	case 1:
		if (!GameFlag_IsSet(0x872))
			Engine_EventRequestExit(20);
	case 2:
		Actor_Get(12)->scale_x = -0x10000;
		Actor_Get(13)->scale_x = -0x10000;
		Actor_Get(14)->scale_x = -0x10000;
		Engine_TaskWait(1);
		break;
	case 7:
	case 8:
	case 9:
	case 10:
	case 11:
	case 12:
		if (GameFlag_IsSet(0x875))
		{
			Map_CopyCellAttributes(84, 5, 10, 7, 20, 5);
			Map_CopyCellAttributes(101, 5, 12, 7, 37, 5);
		}
		break;
	case 3:
	case 4:
	case 5:
	case 6:
		Engine_TaskAddCallback(Makyuri_CyclePalette, 0xc80);
		if (GameFlag_IsSet(0x875))
		{
			Map_CopyCellsTo(37, 98, 10, 97, 5, 3);
			Engine_MapRedraw();
			Engine_TaskWait(1);
			Map_CopyCellAttributes(70, 32, 13, 7, 6, 32);
		}
		if (gGameState.entrance != 6)
			break;
		flag = GameFlag_IsSet(0x251);
		if (flag != 0)
			break;
		GameFlag_Set(0x251);
		Engine_EventBegin();
		Camera_MoveTo(-1, -1, -1, 0);
		Engine_MapRedraw();
		Engine_TaskWait(1);
		Actor_Get(0)->y.fixed = 0x820000;
		Actor_Get(0)->gravity = 0x8000;
		Actor_Get(0)->bounce = flag;
		Actor_Get(0)->motion_flags = flag;
		Engine_EventOpenScreen();
		Engine_EventWaitForScreen();
		Engine_EventWait(30);
		Actor_Get(0)->motion_flags = 3;
		Engine_AudioPlayCue(204);
		Engine_EventWait(24);
		actor = Actor_Get(0);
		options.palette = 7;
		for (i = 0; i <= 16; i++)
		{
			angle = i << 12;
			velocity[0] = Engine_MathCos(angle);
			velocity[1] = 0;
			velocity[2] = Engine_MathSin(angle);
			velocity[0] -= velocity[0] / 4;
			velocity[2] -= velocity[2] / 2;
			Effect_Spawn(actor->x.fixed, actor->y.fixed, actor->z.fixed, velocity[0], velocity[1],
				velocity[2], EFFECT_USE_PALETTE | 1, &options);
		}
		Engine_AudioPlayCue(188);
		Actor_SetAttachedEffect(0, 0x101);
		Engine_ActorSetAnimation(0, 22);
		Work_SetValuesIfNonNegative(0x50000, 0x50000, 0x10000);
		Work_SetValuesIfNonNegative(-1, -1, 0xe666);
		Engine_MapRenderWaitForValues();
		Actor_SetAttachedEffect(0, 0x100);
		Actor_Get(0)->gravity = 0x10000;
		Actor_Get(0)->bounce = 0x4000;
		if (!GameFlag_IsSet(0x875))
		{
			ColorBuffer_ApplySource(0x10000, 0);
			ColorBuffer_ApplyTarget(0x10003, 1);
			Engine_ColorBufferInterpolate(30);
			Engine_EventWaitForScreen();
			Engine_ActorSetAnimation(0, 1);
			Engine_EventWait(30);
			UiText_ShowCenteredMessage((s32)MsgMakyuriWhoHonorsHeart, 0, 0);
			ColorBuffer_ApplyTarget(0x10000, 0);
			Engine_ColorBufferInterpolate(30);
		}
		Engine_EventEnd();
		break;
	case 18:
	case 19:
	case 20:
		MakyuriIriguchi_ArriveWithSparks();
	case 17:
		BattleFx_SetQueuedSoundAndPlay(170);
		break;
	case 25:
		Actor_SetChildValue(0, 15);
		Engine_ActorSetSpriteFlags(Actor_Get(0), 0);
		Engine_EventBegin();
		Engine_MapRedraw();
		Engine_TaskWait(1);
		gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
		Engine_EventOpenScreen();
		Engine_EventWaitForScreen();
		Engine_EventWait(120);
		Engine_EventRequestExit(50);
		Engine_EventEnd();
		break;
	case 30:
		if (!GameFlag_IsSet(0x109))
			MakyuriIriguchi_CrossDoorway();
		else
			Map_CopyCellAttributes(0, 0, 3, 3, 7, 9);
		break;
	case 31:
		if (!GameFlag_IsSet(0x109))
			MakyuriIriguchi_RunDoorScene();
		break;
	}
	return 0;
}

static void MakyuriIriguchi_CrossDoorway(void)
{
	s32				   i;
	struct FieldActor *ivan;
	struct FieldActor *actor;

	Engine_EventBegin();
	actor = Object_GetById(12);
	actor->scale_x = -0x10000;
	actor = Object_GetById(13);
	actor->scale_x = -0x10000;
	actor = Object_GetById(14);
	actor->scale_x = -0x10000;
	Actor_SetPosition(3, 0x880000, 0xb80000);
	Actor_SetPosition(0, 0x880000, 0x1280000);
	Actor_SetPosition(8, 0x880000, 0x980000);
	Actor_SetSpeed(3, 0x18000, 0xc000);
	Actor_SetSpeed(8, 0x18000, 0xc000);
	Actor_SetSpeed(0, 0xcccc, 0x6666);
	Engine_CameraSetSpeed(0xcccc, 0x1999);
	Engine_CameraMoveTo(0x880000, -1, 0xb80000, 0);
	Engine_EventOpenScreen();
	Engine_EventWaitForScreen();
	Engine_ActorRunRepeatedMotion(3, 1);
	Object_GetById(8)->steering_flags &= ~ACTOR_FACES_MOTION;
	Engine_EventWait(20);
	for (i = 0; i < 2; i++)
	{
		Engine_ActorWalkTo(3, 152, 168);
		Engine_EventWait(10);
		Engine_ActorEnableActionCallback(8, (s32)MakyuriIriguchi_Actor8Path3);
		Engine_ActorWaitForMove(3);
		Actor_FaceDirection(3, 0xc000, 30);
		Engine_ActorRunRepeatedMotion(3, 1);
		ivan = Object_GetById(3);
		ivan->steering_flags &= ~ACTOR_FACES_MOTION;
		Engine_ActorWalkTo(3, 136, 184);
		Engine_EventWait(10);
		Engine_ActorEnableActionCallback(8, (s32)MakyuriIriguchi_Actor8Path1);
		Engine_ActorWaitForMove(3);
		Object_GetById(3)->steering_flags |= ACTOR_FACES_MOTION;
		Engine_EventWait(30);
		Engine_ActorWalkTo(3, 120, 168);
		Engine_EventWait(5);
		Engine_ActorEnableActionCallback(8, (s32)MakyuriIriguchi_Actor8Path2);
		Engine_ActorWaitForMove(3);
		Engine_ActorFaceDirection(3, 0xc000, 30);
		Engine_ActorRunRepeatedMotion(3, 1);
		Engine_EventWait(15);
		Object_GetById(3)->steering_flags &= ~ACTOR_FACES_MOTION;
		Engine_ActorWalkTo(3, 136, 184);
		Engine_EventWait(15);
		Engine_ActorEnableActionCallback(8, (s32)MakyuriIriguchi_Actor8Path1);
		Engine_ActorWaitForMove(3);
		Engine_ActorRunRepeatedMotion(3, 1);
		Object_GetById(3)->steering_flags |= ACTOR_FACES_MOTION;
	}
	Engine_EventWait(20);
	Engine_ActorShowEmote(3, 0x102, 60);
	actor = Object_GetById(3);
	actor->update = (void (*)(union FieldObject *))SceneActor_FaceLeaderWhileGrounded;
	Engine_CameraMoveToActor(0, 1);
	Engine_EventWait(30);
	Actor_WalkToAndWait(0, 136, 0x108);
	Engine_CameraWaitForMove();
	Map_CopyCellAttributes(0, 0, 3, 3, 7, 9);
	Engine_EventEnd();
}

static void MakyuriIriguchi_RunDoorScene(void)
{
	struct FieldActor *record;

	if (GameFlag_IsSet(0x250) == 0)
	{
		GameFlag_Set(0x250);
		Engine_EventBegin();
		record = Actor_Get(12);
		record->scale_x = -0x10000;
		record = Object_GetById(13);
		record->scale_x = -0x10000;
		record = Actor_Get(14);
		record->scale_x = -0x10000;
		Actor_SetPosition(MEARI, 0x880000, 0x900000);
		Actor_FaceDirection(MEARI, 0x4000, 10);
		gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 1);
		Engine_EventOpenScreen();
		Engine_EventWaitForScreen();
		Engine_EventWait(60);
		Actor_FaceActor(MEARI, ACTOR_PARTY_LEADER, 0);
		Engine_ActorSetAnimationAndWait(MEARI, 3);
		Engine_EventWait(30);
		Actor_WalkTo(MEARI, 136, 72);
		Engine_EventWait(40);
		Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 1);
		Engine_ActorWaitForMove(MEARI);
		Actor_SetPosition(MEARI, 0, 0);
		GameFlag_Set(0x872);
		gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
		Engine_EventEnd();
	}
}

// ここに共通ソースが入る（ROM の並びのとおり）
#include "../common/makyuri/lights.c"

static void OverlayObject_SpawnKind24AtObject(struct FieldActor *src)
{
	struct FieldActor *obj = Engine_ObjectCreate(24, src->x.fixed, src->y.fixed, src->z.fixed);

	if (obj != 0)
	{
		struct FieldSprite *sprite = obj->sprite;

		Engine_ObjectSetScript(obj, MakyuriIriguchi_RiseScript);
		obj->motion_flags = 0;
		obj->terrain_id = 1;
		obj->priority_flags = 2;
		if (sprite != 0)
		{
			AnimationObjects_SelectAnimation(sprite, 2);
			sprite->flags = 0;
			((struct FieldSpritePriority *)sprite)->priority = 3;
		}
	}
}

// ここに共通ソースが入る（ROM の並びのとおり）
#include "pillar.c"
#include "../common/makyuri/makyuri.c"

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

// 向きごとの一歩: 上位16ビットがX、下位16ビットがZ (ピクセル)
#define DIRECTION_STEP(dx, dz) (((dx) << 16) | ((dz) & 0xffff))

s32 StagedActor_DirectionSteps[] = {
	DIRECTION_STEP(16, 0),
	DIRECTION_STEP(16, 0),
	DIRECTION_STEP(16, 0),
	DIRECTION_STEP(0, 16),
	DIRECTION_STEP(0, 16),
	DIRECTION_STEP(0, 16),
	DIRECTION_STEP(0, 16),
	DIRECTION_STEP(-16, 0),
	DIRECTION_STEP(-16, 0),
	DIRECTION_STEP(-16, 0),
	DIRECTION_STEP(-16, 0),
	DIRECTION_STEP(0, -16),
	DIRECTION_STEP(0, -16),
	DIRECTION_STEP(0, -16),
	DIRECTION_STEP(0, -16),
	DIRECTION_STEP(16, 0),
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

s32 MakyuriIriguchi_RiseScript[] = {
	SC_WAIT(30),
	SC_FREE,
};

s32 MakyuriHeya_SparkScript[] = {
	SC_CALL(SceneEffect_AdvanceAnchoredRiseFrame),
	SC_FREE,
};

s32 Makyuri_RampScript[] = {
	SC_CALL(OverlayObject_RampWords24And28Over16Frames),
	SC_END,
};

s32 Makyuri_ScaleCounterScript[] = {
	SC_CALL(OverlayObject_AdvanceScaleCounter),
	SC_FREE,
};

s32 Makyuri_PillarScript[] = {
	SC_CALL(Makyuri_FollowLeader),
	SC_END,
};

u16 Makyuri_MoveAngles[] = {
	0xffff, FACING_EAST, FACING_WEST, 0xffff, FACING_NORTH, 0xffff, 0xffff, FACING_NORTH,
	FACING_SOUTH, 0xffff, 0xffff, FACING_SOUTH, 0xffff, FACING_EAST, FACING_WEST, 0xffff,
};

const s32 *gEffectScripts[] = {
	gEffectScript0,
	gEffectScript1,
	gEffectScript2,
};

s32 MakyuriIriguchi_Actor8Path1[] = {
	SC_SET(SW_SPEED, 0x20000),
	SC_SET(SW_ACCEL, 0x20000),
	SC_SET(SW_STEERING, 0),
	SC_ARG(2),
	SC_MOVE(PIXELS(136), 0, PIXELS(152)),
	SC_CALL(SceneAudio_PlayCue118AndReturnZero),
	SC_SYNC,
	SC_ARG(1),
	SC_END,
};

s32 MakyuriIriguchi_Actor8Path2[] = {
	SC_SET(SW_SPEED, 0x20000),
	SC_SET(SW_ACCEL, 0x20000),
	SC_SET(SW_STEERING, 0),
	SC_ARG(2),
	SC_MOVE(PIXELS(120), 0, PIXELS(152)),
	SC_CALL(SceneAudio_PlayCue118AndReturnZero),
	SC_SYNC,
	SC_ARG(1),
	SC_END,
};

s32 MakyuriIriguchi_Actor8Path3[] = {
	SC_SET(SW_SPEED, 0x20000),
	SC_SET(SW_ACCEL, 0x20000),
	SC_SET(SW_STEERING, 0),
	SC_ARG(2),
	SC_MOVE(PIXELS(152), 0, PIXELS(152)),
	SC_CALL(SceneAudio_PlayCue118AndReturnZero),
	SC_SYNC,
	SC_ARG(1),
	SC_END,
};

s32 gPaletteCycleRed = 0;

s32 gPaletteCycleGreen = 0;

s32 gPaletteCycleBlue = 0;

struct SceneEntrance MakyuriIriguchi_SceneTableA[] = {
	{ 0, CONDITION_ALWAYS, 209, 0, 279, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 1, CONDITION_ALWAYS, 136, 0, 280, FACING_NORTH, 0, 16, 32, 256, 320, 0 },
	{ 2, CONDITION_ALWAYS, 136, 0, 120, FACING_SOUTH, 0, 16, 32, 256, 320, 0 },
	{ 3, CONDITION_ALWAYS, 200, 0, 648, FACING_NORTH, 0, 80, 432, 320, 688, 0 },
	{ 4, CONDITION_ALWAYS, 120, 0, 520, FACING_SOUTH, 0, 80, 432, 320, 688, 0 },
	{ 5, CONDITION_ALWAYS, 280, 0, 520, FACING_SOUTH, 0, 80, 432, 320, 688, 0 },
	{ 6, CONDITION_ALWAYS, 200, 0, 552, FACING_SOUTH, 0, 80, 432, 320, 688, 0 },
	{ 7, CONDITION_ALWAYS, 360, 0, 184, FACING_NORTH, 0, 304, 32, 800, 208, 0 },
	{ 8, CONDITION_ALWAYS, 456, 0, 184, FACING_NORTH, 0, 304, 32, 800, 208, 0 },
	{ 9, CONDITION_ALWAYS, 616, 0, 184, FACING_NORTH, 0, 304, 32, 800, 208, 0 },
	{ 10, CONDITION_ALWAYS, 760, 0, 184, FACING_NORTH, 0, 304, 32, 800, 208, 0 },
	{ 11, CONDITION_ALWAYS, 344, 0, 88, FACING_SOUTH, 0, 304, 32, 800, 208, 0 },
	{ 12, CONDITION_ALWAYS, 536, 0, 88, FACING_SOUTH, 0, 304, 32, 800, 208, 0 },
	{ 13, 0x876, 744, 0, 680, FACING_NORTH, 0, 544, 512, 784, 720, 0 },
	{ 14, 0x876, 584, 0, 568, FACING_SOUTH, 0, 544, 512, 784, 720, 0 },
	{ 15, 0x876, 744, 0, 568, FACING_SOUTH, 0, 544, 512, 784, 720, 0 },
	{ 13, CONDITION_ALWAYS, 584, 0, 424, FACING_NORTH, 0, 384, 256, 624, 448, 0 },
	{ 14, CONDITION_ALWAYS, 424, 0, 312, FACING_SOUTH, 0, 384, 256, 624, 448, 0 },
	{ 15, CONDITION_ALWAYS, 584, 0, 312, FACING_SOUTH, 0, 384, 256, 624, 448, 0 },
	{ 16, CONDITION_ALWAYS, 744, 0, 424, FACING_NORTH, 0, 704, 256, 944, 448, 0 },
	{ 17, CONDITION_ALWAYS, 344, 0, 872, FACING_SOUTH, 0, 48, 768, 400, 960, 0 },
	{ 18, CONDITION_ALWAYS, 232, 0, 872, FACING_SOUTH, 0, 48, 768, 400, 960, 0 },
	{ 19, CONDITION_ALWAYS, 568, 0, 872, FACING_SOUTH, 0, 464, 768, 816, 960, 0 },
	{ 20, CONDITION_ALWAYS, 696, 0, 872, FACING_SOUTH, 0, 464, 768, 816, 960, 0 },
	{ 25, CONDITION_ALWAYS, 744, 0, 600, FACING_SOUTH, 0, 544, 512, 784, 720, 0 },
	{ 30, CONDITION_ALWAYS, 136, 0, 296, FACING_NORTH, 0, 16, 32, 256, 320, 0 },
	{ 31, CONDITION_ALWAYS, 136, 0, 168, FACING_NORTH, 0, 16, 32, 256, 320, 0 },
	{ 60, CONDITION_ALWAYS, 136, 0, 152, FACING_SOUTH, 0, 16, 32, 256, 320, 0 },
	{ 99, CONDITION_ALWAYS, 136, 0, 232, FACING_EAST, 0, 16, 32, 256, 320, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEntrance MakyuriIriguchi_SceneTableB[] = {
	{ 53, 0x0, -16327, 16, 12341, 0x20, 8245, 48, -28619, 64, -32715, 80 },
	{ 4149, 0x61, 16437, 112, 20533, 0x80, 12341, 145, 4150, 160, 8246, 176 },
	{ 28726, 0xc0, 16438, 208, 8245, 0xe1, 16437, 241, 28725, 256, -12235, 272 },
	{ -24523, 0x120, 53, 305, -8139, 0x141, -4042, 800, -4043, 1009, 511, 0 },
};

struct SceneEntrance MakyuriIriguchi_SceneTableC[] = {
	{ 3, CONDITION_ALWAYS, 1, 0, 0, 0x8, 0, 0, 0, 8, -16384, 2 },
	{ 192, CONDITION_ALWAYS, 1, 0, 0, 0x8, 0, 0, 0, 8, 16384, 2 },
	{ 224, CONDITION_ALWAYS, 7, 0, 0, 0x58, 0, 0, 0, 136, 16384, 2 },
	{ 224, CONDITION_ALWAYS, 7, 0, 0, 0x58, 0, 0, 0, 200, 16384, 258 },
	{ 224, CONDITION_ALWAYS, 7, 0, 0, 0x58, 0, 0, 0, 264, 16384, 258 },
	{ 224, CONDITION_ALWAYS, 7, 0, 0, 0xb8, 0, 0, 0, 136, 16384, 258 },
	{ 224, CONDITION_ALWAYS, 7, 0, 0, 0xb8, 0, 0, 0, 200, 16384, 258 },
	{ 224, CONDITION_ALWAYS, 7, 0, 0, 0xb8, 0, 0, 0, 264, 16384, 258 },
	{ 22, CONDITION_ALWAYS, 1, 0, 0, 0xc8, 0, 0, 0, 616, 16384, 2 },
	{ 22, CONDITION_ALWAYS, 7, 0, 0, 0xc8, 0, 0, 0, 512, 16384, 2 },
	{ SCENE_TABLE_END },
};

struct SceneEvent MakyuriIriguchi_SceneTableD[] = {
	{ 0x202, 31, IF_FLAG_SET(0x875), EVENT_SCRIPT(FieldScene_RunSingleStep) },
	{ EVENT_TOUCH, 31, IF_FLAG_SET(0x875), EVENT_SCRIPT(EV31_TOUCH) },
	{ 0xa02, 21, IF_FLAG_SET(0x875), EVENT_SCRIPT(FieldScene_RunSingleStep) },
	{ EVENT_TOUCH, 21, IF_FLAG_SET(0x875), EVENT_SCRIPT(EV21_TOUCH) },
	{ 0x202, 11, IF_FLAG_SET(0x875), EVENT_SCRIPT(FieldScene_RunSingleStep) },
	{ EVENT_TOUCH, 11, IF_FLAG_SET(0x875), EVENT_SCRIPT(EV11_TOUCH) },
	{ EVENT_TOUCH, 10, CONDITION_ALWAYS, EVENT_SCRIPT(EV10_TOUCH) },
	{ EVENT_TOUCH, 40, IF_FLAG_SET(0x205), EVENT_SCRIPT(MakyuriIriguchi_InstallActorEightPath) },
	{ EVENT_TOUCH, 42, IF_FLAG_SET(0x205), EVENT_SCRIPT(MakyuriIriguchi_InstallActorEightPath) },
	{ EVENT_TOUCH, 41, 0x205, EVENT_SCRIPT(EV41_TOUCH) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 40, CONDITION_ALWAYS, EVENT_SCRIPT(MakyuriIriguchi_OpenEntrance) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 41, CONDITION_ALWAYS, EVENT_SCRIPT(MakyuriIriguchi_OpenEntrance) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 67, CONDITION_ALWAYS, EVENT_SCRIPT(EV67_TOUCH) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 68, CONDITION_ALWAYS, EVENT_SCRIPT(EV68_TOUCH) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 69, CONDITION_ALWAYS, EVENT_SCRIPT(EV69_TOUCH) },
	{ EVENT_EXIT, 51, CONDITION_ALWAYS, 1 },
	{ EVENT_EXIT, 52, CONDITION_ALWAYS, 2 },
	{ EVENT_EXIT, 53, CONDITION_ALWAYS, 3 },
	{ EVENT_EXIT, 54, CONDITION_ALWAYS, 4 },
	{ EVENT_EXIT, 55, CONDITION_ALWAYS, 5 },
	{ EVENT_EXIT, 56, CONDITION_ALWAYS, 6 },
	{ EVENT_EXIT, 57, CONDITION_ALWAYS, 7 },
	{ EVENT_EXIT, 58, CONDITION_ALWAYS, 8 },
	{ EVENT_EXIT, 59, CONDITION_ALWAYS, 9 },
	{ EVENT_EXIT, 60, CONDITION_ALWAYS, 10 },
	{ EVENT_EXIT, 61, CONDITION_ALWAYS, 11 },
	{ 0x21, 62, CONDITION_ALWAYS, 12 },
	{ EVENT_EXIT, 63, CONDITION_ALWAYS, 13 },
	{ EVENT_EXIT, 64, CONDITION_ALWAYS, 14 },
	{ EVENT_EXIT, 65, CONDITION_ALWAYS, 15 },
	{ EVENT_EXIT, 66, CONDITION_ALWAYS, 16 },
	{ EVENT_TALK, 3, CONDITION_ALWAYS, MSGNO(MsgMakyuriMonsterInsideTower) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 3, CONDITION_ALWAYS, MSGNO(MsgMakyuriLetMePass) },
	{ EVENT_TALK, 8, CONDITION_ALWAYS, EVENT_SCRIPT(MakyuriIriguchi_OpenEntrance) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, EVENT_SCRIPT(MakyuriIriguchi_OpenEntrance) },
	{ SEARCH(SEARCH_UNNAMED), 1, CONDITION_ALWAYS, EVENT_SCRIPT(EV01_SEARCH) },
	{ PSYNERGY_ON_ACTOR(0x5d), 16, 0x875, EVENT_SCRIPT(EV16_PSYNERGY) },
	{ PSYNERGY_IN_AREA(0x95, 1), 0, CONDITION_ALWAYS, EVENT_SCRIPT(OverlayObject_ReleasePublishedAttachment) },
	{ SEARCH(SEARCH_CHEST), 100, 0xf65, FIND_ITEM(0x4c) },
	{ SEARCH(SEARCH_CHEST), 101, 0xef0, 0x500000 },
	{ SCENE_EVENTS_END },
};

u32 MakyuriIriguchi_SparkBurstScript[] = {
	21, 10, 0x10000, 21, 9, 0x10000, 0, 6,
	21, 10, 0xa000, 21, 9, 0xa000, 0, 6,
	21, 10, 0x4000, 21, 9, 0x4000, 0, 6,
	21, 10, 0xa000, 21, 9, 0xa000, 0, 6,
	12, 4, 0, 0, 1, 2, 0, 0,
	0, 0, 1, 27,
};
