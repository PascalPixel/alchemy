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
#include "map_scroll.h"
#include "field_service.h"
#include "text/msg_ids.h"
#include "callback_scheduler.h"

#define PALETTE ((volatile u16 *)0x05000000)

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

void PlayWorkspaceCueAndClearPaletteZero(void)
{
	Engine_EventRequestExit(gEventWork->touched_trigger);
	/* FAKEMATCH: removing this one-pass block changes instruction scheduling. */
	do
	{
		u16					   color = PALETTE == 0;
		register volatile u16 *palette = PALETTE;

		palette[color] = color;
	} while (0);
}

void BabiFune_LoadTablesAndStopDma(void)
{
	SceneEffect_LoadTablesAndStopDma0();
}

void FieldScene_ConfigureFixedPointValues(void)
{
	DisplayScroll_InitializeHBlankDma(0, 0x40000, 0x10000, 0x2000, 0x10000, 0x8000, 0x4000);
}

u8 *BabiFune_GetEvents(void)
{
	return BabiFune_SceneTableD;
}

void BabiFune_CyclePalette(void)
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

void BabiFune_UpdateDriftingObject(u8 *obj)
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

void SceneState_CountDownEveryFortyTicks(void)
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

void BabiFune_UpdateWaves(void)
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
		{
			u32 value = blend;

			/* FAKEMATCH: outside a one-pass block the register's address is formed before the halfword is read back. */
			do
			{
				REG_BLDALPHA = value;
			} while (0);
		}
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

void Motion_LaunchFromFocusedObject();
s32	 UiText_OpenMessageAtObject();
void Object_RefreshSelectorById();
void MusicCommand_SetPitchAndUpdateFrequency();
void BabiFune_ScheduleFade();
void Event_SetDestination();

s32 BabiFune_StoreLeaderHeight(void)
{
	s32				  *d;
	struct FieldActor *p;

	d = &BabiFune_StoredSlot0;
	p = Object_GetById(0);
	*d = p->y.fixed;
	return 0;
}

s32 BabiFune_StoreActorOneHeight(void)
{
	s32				  *p;
	struct FieldActor *rec;

	p = &BabiFune_StoredRecord1;
	rec = Object_GetById(1);
	*p = rec->y.fixed;
	return 0;
}

s32 BabiFune_StoreActorThreeHeight(void)
{
	s32				  *d;
	struct FieldActor *p;

	d = &BabiFune_StoredSlot3;
	p = Object_GetById(3);
	*d = p->y.fixed;
	return 0;
}

s32 BabiFune_StoreActorTwoHeight(void)
{
	s32				  *d;
	struct FieldActor *p;

	d = &BabiFune_StoredRecord2;
	p = Object_GetById(2);
	*d = p->y.fixed;
	return 0;
}

void Scene_RunExtendedPresentationSequence(void)
{
	struct MapScrollWork *scroll;
	struct FieldActor	 *ship;
	struct EventWork	 *event;
	s32					  blank;

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
	/* FAKEMATCH: keep the backdrop zero before its address calculation. */
	do
	{
		blank = 0;
	} while (0);
	PLTT_BACKDROP = blank;
	event->transition_frames = 96;
	Engine_EventCloseScreen();
	Engine_EventWaitForScreen();
	Engine_EventWait(30);
	Engine_GameFlagSet(282);
	Event_SetDestination((s32)&SceneId_Title, 9);
}

#define BabiFune_ShimmerActive (*(u32 *)&BabiFune_ShimmerActive)
#define BabiFune_ShimmerPhase (*(u32 *)&BabiFune_ShimmerPhase)
#define BabiFune_CountTicks (*(u32 *)&BabiFune_CountTicks)
#define BabiFune_DriftActive (*(u32 *)&BabiFune_DriftActive)
#define BabiFune_SwellActive (*(u32 *)&BabiFune_SwellActive)
#define BabiFune_SwellLayerSixY (*(u32 *)&BabiFune_SwellLayerSixY)
#define BabiFune_SwellLayerSevenY (*(u32 *)&BabiFune_SwellLayerSevenY)

void Inventory_AddItem(s32 a, s32 b);

union GameStateRows
{
	u8	bytes[512][2];
	s16 halves[512][1];
	s32 words[256];
};

#define SPRITE_BYTES(actor) ((u8 *)(actor)->sprite)
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

extern struct ScrollWork *gHBlankScrollWork;

static __inline__ s32 Wave_Offset(s32 angle, s32 amplitude, u16 base)
{
	return (u16)(Iwram_MulQ16(BabiFune_WaveSine[(angle >> 16) & 0xff], amplitude) / 256) + base;
}

void Battle_ResetEffectCounter(void);
void Engine_ActorWalkToAndWait(s32 actor, s32 x, s32 z);

static inline void BabiFune_AddTask(void (*task)(void), s32 order)
{
	/* FAKEMATCH: scenes reach every engine service through its inline; a direct call lets GCC 2.96 share costly constant arguments between calls. */
	Scheduler_AddOrUpdateCallback((s32)task, order);
}

void BabiFune_RunDeparture(void)
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
		/* FAKEMATCH: the one-pass block keeps the colour's two shifts in source order. */
		do
		{
			PLTT_BACKDROP = (i << 11) | (i << 5);
		} while (0);
		Event_Wait(10);
	}
	{
		u16 color = 0x7e00;

		PLTT_BACKDROP = color;
	}
	{
		u16 bright = 0x1010;
		u16 dim = 0x810;

		for (flash = 0; flash < 3; flash++)
		{
			Event_PlayCue(212);
			REG_BLDALPHA = bright;
			Event_Wait(3);
			/* FAKEMATCH: the one-pass block places the counter step after the wait's argument. */
			do
			{
				REG_BLDALPHA = dim;
			} while (0);
			Event_Wait(65);
		}
	}
	BabiFune_ShimmerActive = 1;
	BabiFune_ShimmerPhase = 0;
	BabiFune_AddTask(BabiFune_UpdateWaves, TASK_PRIORITY_SCENE);
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
	BabiFune_AddTask(SceneState_CountDownEveryFortyTicks, TASK_PRIORITY_SCENE);
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
	BabiFune_AddTask(BabiFune_CyclePalette, TASK_PRIORITY_SCENE);
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
	{
		s32 v = 0x2648;

		/* FAKEMATCH: a do-while(0) keeps the BG1CNT store ahead of the BLDCNT address load, as in the reference */
		do
		{
			REG_BLDCNT = v;
			v = 0x810;
		} while (0);
		REG_BLDALPHA = v;
	}
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
