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
#include "field_service.h"
#include "callback_scheduler.h"
#include "dma.h"
#include "iwram_call.h"
#include "io_reg.h"

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

struct MapLayer
{
	u8	unknown_00[12];
	s32 y;
	u8	unknown_10[32];
};

struct MapWork
{
	u8				unknown_00[20];
	struct MapLayer layers[8];
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

extern s16 gBgScroll[];

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
	struct MapWork *map;
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
	struct MapWork *map;
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
		struct MapLayer *layer = map->layers + 6;

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
	y = gBgScroll[7];
	line = work->pages[work->page ^ 1];
	step = work->step[0];
	angle = (work->phase + y) * work->frequency[0];
	amplitude = work->amplitude[0];
	base = gBgScroll[6];
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
