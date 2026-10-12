#include "game_state.h"
#include "vram_tab.h"
#include "unit/field/babi_fune/fune.h"
#include "unit/graphics/display/scroll_initialize_hblank_dma.h"
#include "unit/battle/effect/object.h"
#include "unit/field/common/script/field.h"
#include "unit/game/character/chara.h"
#include "unit/game/character/chara2.h"
#include "unit/game/character/chara3.h"
#include "unit/game/character/chara4.h"
#include "unit/game/character/chara5.h"
#include "unit/game/character/chara6.h"
#include "unit/game/character/chara7.h"
#include "unit/game/character/chara8.h"
#include "unit/menu/shop/shop.h"
#include "unit/menu/shop/shop2.h"
#include "unit/menu/shop/shop3.h"
#include "unit/menu/shop/shop4.h"
#include "unit/menu/shop/shop5.h"
#include "unit/menu/shop/shop6.h"
#include "unit/sound/sound.h"
#include "unit/system/scheduler/runtime.h"
#include "unit/system/scheduler/runtime8.h"
#include "unit/system/scheduler/runtime9.h"
#include "unit/system/scheduler/runtim10.h"
#include "unit/system/scheduler/runtim11.h"
#include "unit/system/scheduler/runtim13.h"
#include "battle.h"
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
#include "dma.h"
#include "io_reg.h"
#include "iwram_call.h"
#include "fixed_point_position.h"
#include "tbs_edition.h"
#include "scene_ids.h"
#include "call.h"
#include "fieldobj.h"
#include "iowrite.h"
#include "map_scroll.h"
#include "field_service.h"
#include "text/msg_ids.h"
#include "callback_scheduler.h"
#include "mem.h"
#include "scroll.h"
#include "resource.h"
#include "vram_block.h"
#include "ability_ids.h"
#include "field_scene.h"
#include "objcmd.h"

// 前方宣言
static void SceneEffect_LoadTablesAndStopDma0(void);
static void BabiFune_ScheduleFade(void);

s32 Math_DivideUnsigned();

u8 *BabiFune_GetEntrances(void)
{
	return BabiFune_SceneTableA;
}

s32 BabiFune_GetDefaultResult(void)
{
	return 0;
}

u8 *BabiFune_GetExits(void)
{
	return BabiFune_SceneTableB;
}

u8 *BabiFune_GetPlacements(void)
{
	return BabiFune_SceneTableC;
}

static void EV01_TOUCH(void)
{
	Engine_EventRequestExit(gEventWork->touched_trigger);
	Io_Write16(0, &PLTT_BACKDROP);
}

static void BabiFune_LoadTablesAndStopDma(void)
{
	SceneEffect_LoadTablesAndStopDma0();
}

static void FieldScene_ConfigureFixedPointValues(void)
{
	DisplayScroll_InitializeHBlankDma(0, 0x40000, 0x10000, 0x2000, 0x10000, 0x8000, 0x4000);
}

u8 *BabiFune_GetEvents(void)
{
	return BabiFune_SceneTableD;
}

static void BabiFune_CyclePalette(void)
{
	u16 *frame;
	s32	 v;

	frame = &BabiFune_PaletteStep;
	v = Math_DivideUnsigned(*frame, 6);
	Dma_Set((const void *)(((u32)(v << 16) >> 15) + (u32)BabiFune_PaletteFrames),
		(void *)0x050000e8, DMA_ENABLE32 | 6, REG_DMA3);
	(*frame)++;
	if (*frame > 35)
		*frame = 0;
}

static void BabiFune_UpdateDriftingObject(u8 *obj)
{
	u16 *wait = &((struct FieldActor *)obj)->timer;
	s32	 left = *wait;

	if (*(s16 *)wait != 0)
	{
		s32 drift;

		*wait = left - 1;
		drift = Engine_RandomNext();
		drift -= Engine_RandomNext();
		((struct FieldActor *)obj)->x.fixed += drift;
		((struct FieldActor *)obj)->y.fixed += 0xcccc;
	}
	else if (*(s16 *)&((struct FieldActor *)obj)->work != 0)
	{
		*(s16 *)&((struct FieldActor *)obj)->work = 0;
		Object_SetMode(obj, 1);
		{
			u16 *p = (u16 *)(obj + 94);
			s32	 delay = 20;

			*p = delay;
		}
		Engine_ObjectSetScript(obj, BabiFune_DriftScript);
	}
}

static void SceneState_CountDownEveryFortyTicks(void)
{
	s32 n = BabiFune_CountTicks + 1;

	BabiFune_CountTicks = n;
	if (n == 40)
	{
		if (BabiFune_Count > 4)
		{
			BabiFune_Count -= 1;
			BabiFune_CountTicks = 0;
		}
	}
}

struct WaveMapLayer
{
	s32 unknown_00[3];
	s32 y;
	s32 unknown_10[8];
};

struct WaveMap
{
	u8					unknown_00[20];
	struct WaveMapLayer layers[8];
};

static void BabiFune_UpdateWaves(void)
{
	volatile u16			   blend;
	struct FixedPointPosition  place;
	struct FixedPointPosition *pos;
	struct WaveMap			  *map;
	struct FieldActor		  *actor;
	s32						   swell;
	s32						   x, z;

	map = gMapWork[0];
	if (BabiFune_ShimmerActive != 0)
	{
		swell = Iwram_MulQ16(Engine_MathSin(BabiFune_ShimmerPhase << 9), 3);
		blend = BabiFune_Count + ((swell + 8) << 8);
		Io_Write16(blend, &REG_BLDALPHA);
		BabiFune_ShimmerPhase++;
	}
	if (BabiFune_SwellActive != 0)
	{
		swell = Iwram_MulQ16(Engine_MathSin(BabiFune_SwellPhase << 9), 2) << 16;
		map->layers[6].y = BabiFune_SwellLayerSixY + swell;
		map->layers[7].y = BabiFune_SwellLayerSevenY + swell;
		if (BabiFune_StoredSlot0 != -0x10000)
		{
			actor = Object_GetById(0);
			actor->y.fixed = BabiFune_StoredSlot0 + swell;
			actor->terrain_height = BabiFune_StoredSlot0 + swell;
			actor->motion_flags = 0;
		}
		if (BabiFune_StoredRecord1 != -0x10000)
		{
			actor = Object_GetById(1);
			actor->y.fixed = BabiFune_StoredRecord1 + swell;
			actor->terrain_height = BabiFune_StoredSlot0 + swell;
			actor->motion_flags = 0;
		}
		if (BabiFune_StoredSlot3 != -0x10000)
		{
			actor = Object_GetById(3);
			actor->y.fixed = BabiFune_StoredSlot3 + swell;
			actor->terrain_height = BabiFune_StoredSlot0 + swell;
			actor->motion_flags = 0;
		}
		if (BabiFune_StoredRecord2 != -0x10000)
		{
			actor = Object_GetById(2);
			actor->y.fixed = BabiFune_StoredRecord2 + swell;
			actor->terrain_height = BabiFune_StoredSlot0 + swell;
			actor->motion_flags = 0;
		}
		BabiFune_SwellPhase++;
	}
	if (BabiFune_DriftActive != 0 && (gFrameCount & 1) != 0)
	{
		x = map->layers[4].unknown_10[0] & -0x10000;
		z = map->layers[4].unknown_10[1] & -0x10000;
		x += Random_Next() * 240;
		pos = &place;
		pos->x = x;
		pos->y = 0;
		z += Random_Next() * 160;
		z += 0x1e0000;
		pos->z = z;
		actor = Object_Create(0x1f7, pos->x, pos->y, pos->z);
		if (actor != 0)
		{
			actor->update = (void (*)(union FieldObject *))BabiFune_UpdateDriftingObject;
			actor->timer = 60;
			actor->work = 1;
			actor->motion_flags = 0;
			actor->priority_flags = 2;
			actor->sprite->priority = 2;
			Engine_ActorSetSpriteFlags(actor, OBJECT_BLEND_NORMAL);
			Object_SetMode(actor, 0);
		}
	}
}

static s32 BabiFune_StoreLeaderHeight(void)
{
	s32				  *d;
	struct FieldActor *p;

	d = &BabiFune_StoredSlot0;
	p = Object_GetById(0);
	*d = p->y.fixed;
	return 0;
}

static s32 BabiFune_StoreActorOneHeight(void)
{
	s32				  *p;
	struct FieldActor *rec;

	p = &BabiFune_StoredRecord1;
	rec = Object_GetById(1);
	*p = rec->y.fixed;
	return 0;
}

static s32 BabiFune_StoreActorThreeHeight(void)
{
	s32				  *d;
	struct FieldActor *p;

	d = &BabiFune_StoredSlot3;
	p = Object_GetById(3);
	*d = p->y.fixed;
	return 0;
}

static s32 BabiFune_StoreActorTwoHeight(void)
{
	s32				  *d;
	struct FieldActor *p;

	d = &BabiFune_StoredRecord2;
	p = Object_GetById(2);
	*d = p->y.fixed;
	return 0;
}

static void Scene_RunExtendedPresentationSequence(void)
{
	struct MapScrollWork *scroll;
	struct FieldActor	 *ship;
	struct EventWork	 *event;

	Engine_EventBegin();
	Camera_MoveTo(-1, -1, -1, 0);
	Map_CopyCellAttributes(18, 0, 3, 1, 18, 12);
	Engine_EventSetMessage((s32)MsgFieldLooksLikeFinally);
	FieldMotion_LaunchFromFocusedObject(1, -10, 16, 49152);
	FieldMotion_LaunchFromFocusedObject(3, 0, 24, 49152);
	Motion_LaunchFromFocusedObject(2, 10, 16, 49152);
	Engine_ActorWaitForMove(1);
	Engine_EventWait(50);
	Engine_ActorRunRepeatedMotion(1, 2);
	Engine_EventWait(20);
	Engine_EventShowMessage(1, 0);
	Engine_EventWait(10);
	Engine_ActorSetAnimationAndWait(2, 3);
	Engine_EventWait(20);
	Engine_EventShowMessage(2, 0);
	Engine_EventWait(10);
	Actor_FaceDirection(3, 16384, 0);
	Engine_EventWait(40);
	Engine_EventShowMessage(3, 0);
	Engine_EventWait(10);
	Actor_FaceDirection(0, 16384, 0);
	Actor_FaceDirection(1, 16384, 0);
	Engine_ActorFaceDirection(2, 16384, 0);
	Engine_EventWait(60);
	Engine_ActorSetAnimation(0, 3);
	Engine_ActorSetAnimation(1, 3);
	Engine_ActorSetAnimationAndWait(2, 3);
	Engine_EventWait(60);
	Engine_ActorRunRepeatedMotion(1, 2);
	Engine_EventWait(30);
	Engine_ActorFaceDirection(1, 57344, 0);
	Engine_EventWait(20);
	UiText_OpenMessageAtObject(1, 0);
	if (Engine_EventChooseYesNo(0, 0) == 0)
	{
		Engine_EventWait(30);
		Actor_ShowEmote(1, 258, 50);
		Engine_EventShowMessage(1, 0);
		gEventWork->message++;
	}
	else
	{
		Engine_EventWait(30);
		Actor_ShowEmote(1, 258, 50);
		gEventWork->message++;
		Engine_EventShowMessage(1, 0);
	}
	Engine_EventWait(10);
	Engine_ActorFaceDirection(2, 32768, 0);
	Engine_EventWait(30);
	Actor_ShowEmote(2, 257, 50);
	Engine_EventShowMessage(2, 0);
	Engine_EventWait(10);
	Engine_ActorFaceDirection(1, 0, 0);
	Engine_EventWait(30);
	Actor_ShowEmote(1, 257, 60);
	Engine_ActorRunRepeatedMotion(3, 2);
	Engine_EventWait(30);
	Engine_ActorFaceDirection(3, 40960, 0);
	Engine_EventWait(30);
	Engine_EventShowMessage(3, 0);
	Engine_EventWait(10);
	Actor_ShowEmote(1, 256, 50);
	Engine_EventShowMessage(1, 0);
	Engine_EventWait(10);
	Engine_ActorRunRepeatedMotion(2, 2);
	Engine_EventWait(20);
	Engine_EventShowMessage(2, 0);
	Engine_EventWait(10);
	Engine_ActorFaceDirection(3, 49152, 0);
	Engine_EventWait(20);
	Engine_ActorSetAnimationAndWait(3, 4);
	Engine_EventWait(20);
	UiText_OpenMessageAtObject(3, 0);
	if (Engine_EventChooseYesNo(0, 0) == 0)
	{
		Engine_EventWait(20);
		Engine_ActorRunRepeatedMotion(1, 2);
		Engine_EventWait(20);
		Engine_ActorFaceDirection(1, 57344, 0);
		Engine_EventWait(20);
		Engine_EventShowMessage(1, 0);
		gEventWork->message++;
	}
	else
	{
		Engine_EventWait(20);
		Engine_ActorRunRepeatedMotion(1, 2);
		Engine_EventWait(20);
		Engine_ActorFaceDirection(1, 57344, 0);
		Engine_EventWait(20);
		gEventWork->message++;
		Engine_EventShowMessage(1, 0);
	}
	Engine_EventWait(10);
	Engine_ActorShowEmote(0, 258, 50);
	Engine_EventWait(10);
	Actor_FaceDirection(2, 40960, 0);
	Engine_EventWait(20);
	Engine_ActorSetAnimationAndWait(2, 3);
	Engine_EventWait(20);
	Engine_EventShowMessage(2, 0);
	Engine_EventWait(20);
	Engine_EventShowMessage(1, 0);
	Engine_EventWait(20);
	Engine_ActorSetAnimationAndWait(0, 3);
	Engine_EventWait(30);
	Engine_EventWait(10);
	Engine_ActorSetAnimationAndWait(3, 3);
	Engine_EventWait(20);
	UiText_OpenMessageAtObject(3, 0);
	Actor_FaceDirection(1, 57344, 0);
	Actor_FaceDirection(3, 49152, 0);
	Engine_ActorFaceDirection(2, 40960, 0);
	Engine_EventWait(20);
	if (Engine_EventChooseYesNo(0, 0) == 0)
	{
		Engine_EventWait(30);
		Engine_ActorSetAnimationAndWait(3, 3);
		Engine_EventWait(20);
		Engine_EventShowMessage(3, 0);
		gEventWork->message++;
	}
	else
	{
		Engine_EventWait(30);
		Engine_ActorSetAnimationAndWait(3, 3);
		Engine_EventWait(20);
		gEventWork->message++;
		Engine_EventShowMessage(3, 0);
	}
	Engine_EventWait(10);
	Actor_FaceDirection(1, 49152, 0);
	Engine_EventWait(30);
	Engine_EventShowMessage(1, 0);
	Engine_EventWait(10);
	Actor_FaceDirection(0, 49152, 0);
	Engine_EventWait(30);
	Engine_EventWait(10);
	Actor_FaceDirection(2, 49152, 0);
	Engine_EventWait(30);
	Engine_ActorSetAnimationAndWait(2, 3);
	Engine_EventWait(20);
	Engine_EventShowMessage(2, 0);
	Engine_EventWait(10);
	Engine_ActorSetAnimationAndWait(3, 3);
	Engine_EventWait(20);
	Engine_EventShowMessage(3, 0);
	Engine_AudioPlayCue(17);
	Engine_EventWait(10);
	Engine_ActorSetAnimation(0, 3);
	Engine_ActorSetAnimation(1, 3);
	Engine_ActorSetAnimationAndWait(2, 3);
	Engine_EventWait(40);
	Actor_SetSpeed(0, 78643, 39321);
	Actor_SetSpeed(1, 78643, 39321);
	Actor_SetSpeed(2, 78643, 39321);
	Actor_SetSpeed(3, 78643, 39321);
	Engine_ActorEnableActionCallback(0, BabiFune_ActionScriptA);
	Engine_EventWait(50);
	Camera_MoveTo(17301504, -1, 13107200, 1);
	Engine_ActorEnableActionCallback(1, BabiFune_ActionScriptB);
	Engine_EventWait(50);
	Engine_ActorEnableActionCallback(2, BabiFune_ActionScriptC);
	Object_RefreshSelectorById(2);
	Actor_FaceDirection(0, 16384, 0);
	Actor_FaceDirection(1, 16384, 0);
	Actor_FaceDirection(2, 16384, 0);
	FieldObjectMotion_CommitPositionAndActivate(3, 0, -32);
	Engine_EventWait(30);
	Engine_ActorFaceDirection(3, 16384, 0);
	Engine_EventWait(60);
	Actor_FaceDirection(3, 32768, 0);
	Engine_EventWait(20);
	Engine_ActorEnableActionCallback(3, BabiFune_ActionScriptD);
	Object_RefreshSelectorById(3);
	Engine_ActorFaceDirection(3, 49152, 0);
	Engine_EventWait(20);
	Camera_MoveTo(14155776, -1, 11010048, 1);
	Engine_CameraWaitForMove();
	Engine_EventWait(20);
	Actor_FaceDirection(0, 32768, 0);
	Actor_FaceDirection(1, 32768, 0);
	Actor_FaceDirection(2, 32768, 0);
	Engine_ActorFaceDirection(3, 32768, 0);
	Engine_EventWait(40);
	Engine_ActorSetAnimation(0, 3);
	Engine_ActorSetAnimation(1, 3);
	Engine_ActorSetAnimation(3, 3);
	Engine_ActorSetAnimationAndWait(2, 3);
	Engine_EventWait(30);
	Engine_AudioPlayCue(67);
	MusicCommand_SetPitchAndUpdateFrequency(240);
	BabiFune_ScheduleFade();
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
	Engine_EventWait(30);
#else
	Engine_EventWait(80);
#endif
	scroll = gMapWork[0];
	ship = Object_GetById(8);
	ship->acceleration = 131;
	ship->speed = 0x20000;
	scroll->layers[0].speed_x = -0x3000;
	ObjectMotion_CommitPositionAndActivate(8, 60, 0);
	scroll->layers[0].speed_x = -0x6000;
	ObjectMotion_CommitPositionAndActivate(8, 60, 0);
	Engine_EventWait(80);
	Engine_EventWait(100);
	Engine_CameraSetSpeed(144179, 655);
	Engine_CameraMoveTo(52953088, -1, 11010048, 1);
	Engine_EventWait(300);
	event = gMapWork[19];
	event->start_transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
	Io_Write16(0, &PLTT_BACKDROP);
	event->transition_frames = 96;
	Engine_EventCloseScreen();
	Engine_EventWaitForScreen();
	Engine_EventWait(30);
	Engine_GameFlagSet(282);
	Event_SetDestination((s32)&SceneId_Title, 9);
}

union GameStateRows
{
	u8	bytes[512][2];
	s16 halves[512][1];
	s32 words[256];
};

#define SPRITE_OF(actor) ((struct FieldSprite *)*(s32 *)((u8 *)(actor) + 0x50))
#define SET_PRIORITY(actor, n) \
	((n) == 9 ? (SPRITE_OF(actor)->priority = 1) : (SPRITE_OF(actor)->second_priority = 1))

struct ScrollWork
{
	u16 pages[2][0x3c0];
	u8	page;
	u8	mode;
	u16 phase;
	u8	unknown_f04[4];
	s32 frequency[2];
	s32 step[2];
	s32 amplitude[2];
};

static __inline__ s32 Wave_Offset(s32 angle, s32 amplitude, u16 base)
{
	return (u16)(Iwram_MulQ16(BabiFune_WaveSine[(angle >> 16) & 0xff], amplitude) / 256) + base;
}

static void EV10_SCRIPT(void)
{
	struct WaveMap *map;
	s32				i;
	s32				flash;
	s32				white;
	s32				dark;
	s32				sunk;
	volatile u16	cnt;

	map = gMapWork[0];

	Event_Begin();
	Battle_ResetEffectCounter();
	Actor_WalkToAndWait(0, 312, 232);
	Actor_FaceDirection(0, 0xc000, 0);
	Event_Wait(40);
	Event_PlayCue(140);
	for (i = 0; i <= 15; i++)
	{
		Io_Write16((i << 11) | (i << 5), &PLTT_BACKDROP);
		Event_Wait(10);
	}
	{
		u16 color = 0x7e00;

		PLTT_BACKDROP = color;
	}
	{
		u16 bright = 0x1010;

		for (flash = 0; flash < 3; flash++)
		{
			Event_PlayCue(212);
			REG_BLDALPHA = bright;
			Event_Wait(3);
			Io_Write16(0x810, &REG_BLDALPHA);
			Event_Wait(65);
		}
	}
	BabiFune_ShimmerActive = 1;
	BabiFune_ShimmerPhase = 0;
	Value2(Scheduler_AddOrUpdateCallback, (s32)BabiFune_UpdateWaves, TASK_PRIORITY_SCENE);
	BabiFune_DriftActive = 1;
	Event_Wait(20);
	Event_PlayCue(163);
	Work_SetValuesIfNonNegative(0x10000, 0x10000, 0x10000);
	Event_Wait(60);
	BabiFune_DriftActive = 1;
	Work_SetValuesIfNonNegative(0x20000, 0x20000, 0x10000);
	Event_Wait(60);
	Work_SetValuesIfNonNegative(0x30000, 0x30000, 0x10000);
	BabiFune_CountTicks = 0;
	Value2(Scheduler_AddOrUpdateCallback, (s32)SceneState_CountDownEveryFortyTicks,
		TASK_PRIORITY_SCENE);
	sunk = 0;
	do
	{
		map->layers[6].y += 0x3333;
		map->layers[7].y += 0x3333;
		sunk += 0x3333;
		Task_Wait(1);
	} while (sunk <= 0x59ffff);
	Engine_TaskRemoveCallback(SceneState_CountDownEveryFortyTicks);
	BabiFune_DriftActive = 0;
	REG_BG3CNT = cnt = (REG_BG3CNT & 0xfffc) | 3;
	REG_BG2CNT = cnt = (REG_BG2CNT & 0xfffc) | 3;
	REG_BG1CNT = cnt = (REG_BG1CNT & 0xfffc) | 2;
	BabiFune_ShimmerActive = 0;
	Event_PlayCue(288);
	Task_Wait(1);
	Event_PlayCue(145);
	{
		u16 blend = 191;

		do
		{
			REG_BLDCNT = blend;
		} while (0);
	}
	for (white = 0; white <= 16; white++)
	{
		REG_BLDY = white;
		Event_Wait(1);
	}
	Event_Wait(40);
	Work_SetValuesIfNonNegative(-1, -1, 0xe666);
	BabiFune_SwellLayerSixY = map->layers[6].y;
	BabiFune_SwellLayerSevenY = map->layers[7].y;
	BabiFune_SwellActive = 1;
	for (dark = 16; dark >= 0; dark--)
	{
		REG_BLDY = dark;
		Event_Wait(8);
	}
	Value2(Scheduler_AddOrUpdateCallback, (s32)BabiFune_CyclePalette, TASK_PRIORITY_SCENE);
	Event_PlayCue(80);
	AudioCommand_WaitForStateByteClear();
	Event_Wait(20);
	Event_End();
	Scene_RunExtendedPresentationSequence();
}

s32 BabiFune_SetupScene(void)
{
	struct WaveMap *map;
	volatile u16	cnt;

	map = gMapWork[0];
	if (((union GameStateRows *)&gGameState)->halves[225][0] == 99)
	{
		Inventory_AddItem(0, 242);
	}
	((struct EventWork *)gMapWork[19])->start_transition =
		SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
	Object_GetById(8)->collision_flags = 0;
	Object_GetById(8)->priority_flags = 2;
	Object_GetById(9)->collision_flags = 0;
	Object_GetById(9)->priority_flags = 2;
	{
		struct FieldActor *actor = Object_GetById(8);

		SET_PRIORITY(actor, 9);
	}
	{
		struct FieldActor *actor = Object_GetById(9);

		SET_PRIORITY(actor, 9);
	}
	{
		struct FieldActor *actor = Object_GetById(0);

		SET_PRIORITY(actor, 9);
		SET_PRIORITY(actor, 21);
	}
	{
		struct FieldActor *actor = Object_GetById(1);

		SET_PRIORITY(actor, 9);
		SET_PRIORITY(actor, 21);
	}
	{
		struct FieldActor *actor = Object_GetById(2);

		SET_PRIORITY(actor, 9);
		SET_PRIORITY(actor, 21);
	}
	{
		struct FieldActor *actor = Object_GetById(3);

		SET_PRIORITY(actor, 9);
		SET_PRIORITY(actor, 21);
	}
	{
		struct Half
		{
			u16 v;
		} two, three;

		cnt = REG_BG3CNT & 0xfffc;
		two.v = 2;
		cnt |= two.v;
		REG_BG3CNT = cnt;
		cnt = REG_BG2CNT & 0xfffc;
		three.v = 3;
		cnt |= three.v;
		REG_BG2CNT = cnt;
		cnt = (REG_BG1CNT & 0xfffc) | three.v;
		REG_BG1CNT = cnt;
	}
	Io_Write16(0x2648, &REG_BLDCNT);
	Io_Write16(0x810, &REG_BLDALPHA);
	{
		struct WaveMapLayer *layer = map->layers + 6;

		layer->y += -0x5a0000;
		layer = map->layers + 7;
		layer->y += -0x5a0000;
	}
	Engine_MapRedraw();
	FieldScene_ConfigureFixedPointValues();
	return 0;
}

void DisplayScroll_ArmHBlankDma(void)
{
	struct ScrollWork *work;
	u32				  *source;
	volatile u16	  *dma0;
	volatile u32	  *destination;

	work = gHBlankScrollWork;
	source = (u32 *)work->pages[work->page];

	dma0 = REG_DMA0;
	destination = (volatile u32 *)&REG_BG3HOFS;
	dma0[5] &= ~DMA_START_HBLANK_REPEAT;
	dma0[5] &= ~DMA_ENABLE;
	dma0[5];

	*destination = *source++;
	Dma_Set(source, destination, 0xa6600001, (volatile u32 *)dma0);
}

void DisplayScroll_BuildAndSwapHBlankPage(void)
{
	struct ScrollWork *work;
	u16				  *line;
	s32				   angle;
	u16				   base;
	s32				   amplitude;
	s32				   step;
	u16				   y;
	s32				   i;

	work = gHBlankScrollWork;
	y = gBgScroll[3].y;
	line = work->pages[work->page ^ 1];
	step = work->step[0];
	angle = (work->phase + y) * work->frequency[0];
	amplitude = work->amplitude[0];
	base = gBgScroll[3].x;
	for (i = 0; i != 160; i++)
	{
		*line = Wave_Offset(angle, amplitude, base);
		angle += step;
		line += 2;
	}
	line = work->pages[work->page ^ 1] + 1;
	step = work->step[1];
	angle = (work->phase + y) * work->frequency[1];
	amplitude = work->amplitude[1];
	for (i = 0; i != 160; i++)
	{
		*line = Wave_Offset(angle, amplitude, y);
		angle += step;
		line += 2;
	}
	work->phase++;
	work->page ^= 1;
}

// ここに共通ソースが入る（ROM の並びのとおり）
#include "../../graphics/display/scroll_initialize_hblank_dma.c"

static void SceneEffect_LoadTablesAndStopDma0(void)
{
	volatile u16 *reg;

	Engine_TaskRemoveCallback(DisplayScroll_ArmHBlankDma);
	Engine_TaskRemoveCallback(DisplayScroll_BuildAndSwapHBlankPage);
	reg = REG_DMA0;
	reg[5] = 0xC5FF & reg[5];
	reg[5] = 0x7FFF & reg[5];
	reg[5];
}

struct FadeSprite
{
	u32 unused;
	u32 attributes;
	u32 tile;
};

static struct FadeSprite BabiFune_FadeSprites[24];
static s16				 BabiFune_FadeStep;
static s16				 BabiFune_FadeSlot;

static void BabiFune_StepFade(void)
{
	u32 *entry;
	u32	 tile;
	s32	 step;
	s32	 x;
	u32	 i;

	tile = gVramBlockCache[BabiFune_FadeSlot].offset >> 5;
	entry = (u32 *)BabiFune_FadeSprites;
	if (BabiFune_FadeStep != 0)
		BabiFune_FadeStep--;
	for (i = 0; i < 8; i++)
	{
		s32 left;

		step = BabiFune_FadeStep;
		left = -step / 2;
		*entry++ = 0;
		*entry++ = (left & 0xff) | (i << 21) | 0x80004000;
		*entry++ = tile;
	}
	x = (step / 2 + 136) & 0xff;
	for (i = 0; i < 8; i++)
	{
		*entry++ = 0;
		*entry++ = (i << 21) | x | 0x80004000;
		*entry++ = tile;
	}
	i = 0;
	x = (BabiFune_FadeStep / 2 + 152) & 0xff;
	for (; i < 8; i++)
	{
		*entry++ = 0;
		*entry++ = (i << 21) | x | 0x80004000;
		*entry++ = tile;
	}
	for (i = 0; i < 24; i++)
		Runtime_PushSlotEntry(&BabiFune_FadeSprites[i], 255);
}

static void BabiFune_ScheduleFade(void)
{
	u8			*buf;
	volatile u32 fill;

	buf = Runtime_CallAlternatePool(Runtime_BumpAllocateAlternatePool, 0x100);
	BabiFune_FadeSlot = Resource_FindFreeEntry();
	fill = 0x11111111;
	Dma_Set((const void *)&fill, buf, DMA_ENABLE32 | DMA_32BIT | DMA_SRC_FIXED32 | 0x40, REG_DMA3);
	Engine_VramLoad(BabiFune_FadeSlot, 0x100, (s32)buf);

	BabiFune_FadeStep = 0x30;
	Scheduler_AddOrUpdateCallback((s32)BabiFune_StepFade, 0xc80);
}

struct SceneEvent BabiFune_PaletteFrames[] = {
	{0x69c05860, 0x7f20, 0x69c0, 0x48005860},
	{0x69c05860, 0x7f20, 0x69c0, 0x5860},
};

s32 BabiFune_DriftScript[] = {
	SC_FREE,
};

s16 BabiFune_WaveSine[256] = {
	0, 100, 200, 301, 401, 501, 601, 700,
	799, 897, 995, 1092, 1189, 1284, 1379, 1474,
	1567, 1659, 1751, 1841, 1930, 2018, 2105, 2191,
	2275, 2358, 2439, 2519, 2598, 2675, 2750, 2824,
	2896, 2966, 3034, 3101, 3166, 3229, 3289, 3348,
	3405, 3460, 3513, 3563, 3612, 3658, 3702, 3744,
	3784, 3821, 3856, 3889, 3919, 3947, 3973, 3996,
	4017, 4035, 4051, 4065, 4076, 4084, 4091, 4094,
	4096, 4094, 4091, 4084, 4076, 4065, 4051, 4035,
	4017, 3996, 3973, 3947, 3919, 3889, 3856, 3821,
	3784, 3744, 3702, 3658, 3612, 3563, 3513, 3460,
	3405, 3348, 3289, 3229, 3166, 3101, 3034, 2966,
	2896, 2824, 2750, 2675, 2598, 2519, 2439, 2358,
	2275, 2191, 2105, 2018, 1930, 1841, 1751, 1659,
	1567, 1474, 1379, 1284, 1189, 1092, 995, 897,
	799, 700, 601, 501, 401, 301, 200, 100,
	0, -100, -200, -301, -401, -501, -601, -700,
	-799, -897, -995, -1092, -1189, -1284, -1379, -1474,
	-1567, -1659, -1751, -1841, -1930, -2018, -2105, -2191,
	-2275, -2358, -2439, -2519, -2598, -2675, -2750, -2824,
	-2896, -2966, -3034, -3101, -3166, -3229, -3289, -3348,
	-3405, -3460, -3513, -3563, -3612, -3658, -3702, -3744,
	-3784, -3821, -3856, -3889, -3919, -3947, -3973, -3996,
	-4017, -4035, -4051, -4065, -4076, -4084, -4091, -4094,
	-4096, -4094, -4091, -4084, -4076, -4065, -4051, -4035,
	-4017, -3996, -3973, -3947, -3919, -3889, -3856, -3821,
	-3784, -3744, -3702, -3658, -3612, -3563, -3513, -3460,
	-3405, -3348, -3289, -3229, -3166, -3101, -3034, -2966,
	-2896, -2824, -2750, -2675, -2598, -2519, -2439, -2358,
	-2275, -2191, -2105, -2018, -1930, -1841, -1751, -1659,
	-1567, -1474, -1379, -1284, -1189, -1092, -995, -897,
	-799, -700, -601, -501, -401, -301, -200, -100,
};

struct SceneEntrance BabiFune_SceneTableA[] = {
	{0, CONDITION_ALWAYS, 408, 0, 200, FACING_NORTH, 0, -1, -1, -1, -1, 0},
	{1, CONDITION_ALWAYS, 312, 0, 296, FACING_NORTH, 0, -1, -1, -1, 328, 0},
	{SCENE_TABLE_END},
};

u32 BabiFune_SceneTableB[] = {
	SCENE_EXITS(188),
	SCENE_EXIT(1, 180, 31),
	SCENE_EXITS_END,
};

struct ScenePlacement BabiFune_SceneTableC[] = {
	{GERALD, CONDITION_ALWAYS, ACTOR_STAND, 0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0},
	{IWAN, CONDITION_ALWAYS, ACTOR_STAND, 0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0},
	{MEARI, CONDITION_ALWAYS, ACTOR_STAND, 0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0},
	{0x1f6, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(312), 0, PIXELS(248), FACING_SOUTH,
		TALK_KEEP_FACING, 0},
	{0x1f6, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(312), 0, PIXELS(312), FACING_SOUTH,
		TALK_KEEP_FACING, 0},
	{SCENE_TABLE_END},
};

struct SceneEvent BabiFune_SceneTableD[] = {
	{EVENT_TOUCH, 1, CONDITION_ALWAYS, EVENT_SCRIPT(EV01_TOUCH)},
	{PSYNERGY_IN_AREA(ABILITY_REVEAL, 5), 0, CONDITION_ALWAYS,
		EVENT_SCRIPT(BabiFune_LoadTablesAndStopDma)},
	{PSYNERGY_IN_AREA(ABILITY_REVEAL, 4), 0, CONDITION_ALWAYS,
		EVENT_SCRIPT(FieldScene_ConfigureFixedPointValues)},
	{0xf204, 10, CONDITION_ALWAYS, EVENT_SCRIPT(EV10_SCRIPT)},
	{SCENE_EVENTS_END},
};

s32 BabiFune_ShimmerActive = 0;

s32 BabiFune_ShimmerPhase = 0;

s32 BabiFune_Count = 16;

s32 BabiFune_CountTicks = 0;

s32 BabiFune_DriftActive = 0;

s32 BabiFune_SwellActive = 0;

s32 BabiFune_SwellPhase = 0;

s32 BabiFune_SwellLayerSixY = 0;

s32 BabiFune_SwellLayerSevenY = 0;

s32 BabiFune_StoredSlot0 = -0x10000;

s32 BabiFune_StoredRecord1 = -0x10000;

s32 BabiFune_StoredSlot3 = -0x10000;

s32 BabiFune_StoredRecord2 = -0x10000;

u32 BabiFune_PaletteStep[] = {
	0,
};

s32 BabiFune_ActionScriptA[] = {
	SC_ARG(2),
	SC_MOVE_BY(PIXELS(-8), 0, 0),
	SC_SYNC,
	SC_MOVE_BY(0, 0, PIXELS(-16)),
	SC_SYNC,
	SC_SET(SW_VELOCITY_Y, 0x80000),
	SC_SE(153),
	SC_MOVE_BY(0, 0, PIXELS(-24)),
	SC_SYNC,
	SC_ARG(1),
	SC_WAIT(20),
	SC_ARG(2),
	SC_MOVE_BY(PIXELS(-8), 0, 0),
	SC_SYNC,
	SC_MOVE_BY(0, 0, PIXELS(-56)),
	SC_SYNC,
	SC_ARG(1),
	SC_SET(SW_FACING, FACING_SOUTH),
	SC_WAIT(1),
	SC_CALL(BabiFune_StoreLeaderHeight),
	SC_END,
};

s32 BabiFune_ActionScriptB[] = {
	SC_ARG(2),
	SC_MOVE_BY(0, 0, PIXELS(-32)),
	SC_SYNC,
	SC_SET(SW_VELOCITY_Y, 0x80000),
	SC_SE(153),
	SC_MOVE_BY(0, 0, PIXELS(-24)),
	SC_SYNC,
	SC_ARG(1),
	SC_WAIT(20),
	SC_ARG(2),
	SC_MOVE_BY(PIXELS(-8), 0, 0),
	SC_SYNC,
	SC_MOVE_BY(0, 0, PIXELS(-40)),
	SC_SYNC,
	SC_ARG(1),
	SC_SET(SW_FACING, FACING_SOUTH),
	SC_WAIT(1),
	SC_CALL(BabiFune_StoreActorOneHeight),
	SC_END,
};

s32 BabiFune_ActionScriptC[] = {
	SC_ARG(2),
	SC_MOVE_BY(PIXELS(-20), 0, 0),
	SC_SYNC,
	SC_MOVE_BY(0, 0, PIXELS(-32)),
	SC_SYNC,
	SC_SET(SW_VELOCITY_Y, 0x80000),
	SC_SE(153),
	SC_MOVE_BY(0, 0, PIXELS(-24)),
	SC_SYNC,
	SC_ARG(1),
	SC_WAIT(20),
	SC_ARG(2),
	SC_MOVE_BY(PIXELS(-8), 0, 0),
	SC_SYNC,
	SC_MOVE_BY(0, 0, PIXELS(-24)),
	SC_SYNC,
	SC_ARG(1),
	SC_SET(SW_FACING, FACING_SOUTH),
	SC_WAIT(1),
	SC_CALL(BabiFune_StoreActorTwoHeight),
	SC_END,
};

s32 BabiFune_ActionScriptD[] = {
	SC_ARG(2),
	SC_MOVE_BY(PIXELS(-8), 0, 0),
	SC_SYNC,
	SC_MOVE_BY(0, 0, PIXELS(-8)),
	SC_SYNC,
	SC_SET(SW_VELOCITY_Y, 0x80000),
	SC_SE(153),
	SC_MOVE_BY(0, 0, PIXELS(-24)),
	SC_SYNC,
	SC_ARG(1),
	SC_WAIT(20),
	SC_ARG(2),
	SC_MOVE_BY(PIXELS(-8), 0, 0),
	SC_SYNC,
	SC_MOVE_BY(0, 0, PIXELS(-8)),
	SC_SYNC,
	SC_ARG(1),
	SC_CALL(BabiFune_StoreActorThreeHeight),
	SC_END,
};
