#include "game_state.h"
#include "vram_tab.h"
#include "unit/menu/title.h"
#include "unit/object/object.h"
#include "unit/object/objtable.h"
#include "unit/object/evbasic.h"
#include "unit/object/evactor.h"
#include "unit/game/chara.h"
#include "unit/graphics/palette/palette.h"
#include "unit/window/window.h"
#include "unit/window/output.h"
#include "unit/window/save.h"
#include "unit/runtime/runtime.h"
#include "unit/runtime/fade.h"
#include "unit/runtime/oamlist.h"
#include "unit/runtime/objvram.h"
#include "unit/runtime/heap.h"
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
#include "callback_scheduler.h"
#include "resource.h"
#include "global_cells.h"
#include "types.h"
#include "save_state.h"
#include "mem.h"
#include "field_event.h"
#include "field_service.h"
#include "scene_ids.h"
#include "resource_ids.h"
#include "title.h"
#include "dma.h"
#include "io_write_queue.h"
#include "io_reg.h"
#include "vram_block.h"
#include "ram_buffer.h"
#include "field_scene.h"
#include "map_scroll.h"

// 前方宣言
static void Title_RevealScreen(s32 unused);
static void Title_LoadBackground(void);

struct TitleWork
{
	u8	unknown_00[20];
	u16 mode;
};

u8 *Title_GetEntrances(void)
{
	return (u8 *)gTitleEntrances;
}

s32 SceneData_ReturnZero(void)
{
	return 0;
}

u8 *Title_GetExits(void)
{
	return (u8 *)gTitleExits;
}

u8 *Title_GetPlacements(void)
{
	return (u8 *)gTitlePlacements;
}

u8 *Title_GetEvents(void)
{
	return (u8 *)gTitleEvents;
}

s32 Title_Run(void)
{
	s32 wait;

	if (gGameState.entrance == 10)
	{
		Object_GetById(gGameState.selected_actor)->motion_flags = 0;
		Engine_AudioPlayCue(75);
		Title_RevealScreen(0);
		Engine_TaskWait(120);
		wait = 0;
		if (gKeyState == 0)
		{
			do
			{
				Engine_TaskWait(1);
				if (++wait > 3599)
					break;
			} while (gKeyState == 0);
		}
		Event_SetDestination((s32)&SceneId_Title, 2);
		return 0;
	}
	if (gGameState.entrance == 9)
	{
		Engine_AudioPlayCue(67);
		ScrollFar_Entry00(0);
		Engine_AudioPlayCue(17);
		Blend_SetDarkenTarget16(60);
		Blend_WaitForTransition();
		Engine_EventWait(240);
		Engine_AudioPlayCue(19);
		Event_SetDestination((s32)&SceneId_Clear, 2);
		return 0;
	}
	RuntimeDispatch_NoOpHook((s32)&ResourceId_PaletteFarCalls);
	if (gGameState.entrance == 2)
	{
		for (;;)
		{
			Engine_AudioPlayCue(19);
			Title_ShowSplashScreen(0);
			PaletteFar_Entry20(0);
			if (SaveState_ScanRecordFlags() > 0)
			{
				Engine_AudioPlayCue(70);
				if (PaletteFar_Entry00(1) == 0)
				{
					Engine_AudioPlayCue(17);
					Blend_SetDarkenTarget16(30);
					Blend_WaitForTransition();
					wait = 0;
					if (gKeysHeld == 0)
					{
						do
						{
							Engine_TaskWait(1);
							if (++wait > 119)
								break;
						} while (gKeysHeld == 0);
					}
					continue;
				}
			}
			break;
		}
		Event_SetDestination((s32)&SceneId_Clear, 1);
	}
	else
	{
		Engine_AudioPlayCue(64);
		PaletteFar_Entry00(0);
		Party_ApplyStatePreset();
		Event_SetDestination((s32)&SceneId_HaidiaMura, 16);
		Engine_AudioPlayCue(17);
	}
	Engine_AudioPlayCue(17);
	Blend_SetDarkenTarget16(30);
	Blend_WaitForTransition();
	Engine_EventWait(60);
	Engine_AudioPlayCue(19);
	return 0;
}

static void Title_LoadSprites(s32 unused)
{
	u8			 *buffer;
	volatile u32 *dma;
#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) \
	|| defined(TBS_EDITION_IT)
	u8 *res;

	buffer = Runtime_BumpAllocateAlternatePool(0x9600);
	res = Resource_GetTableEntry((s32)&ResourceId_IntroTilesB);
	if (gTitleVramBlock == -1)
		gTitleVramBlock = Resource_FindFreeEntry();
	Dma_Set(res, (void *)0x05000200, 0x84000070, REG_DMA3);
	res += 0x1c0;
	FieldResource_DecodeType01(res, buffer);
	Vram_Load(gTitleVramBlock, 0x1e00, buffer + 0x7800);
#else
	buffer = Runtime_BumpAllocateAlternatePool(0x520);
	if (gTitleVramBlock == -1)
		gTitleVramBlock = Resource_FindFreeEntry();
	FieldResource_DecodeType01(Resource_GetTableEntry((s32)&ResourceId_IntroTilesB), buffer);
	Dma_Set(buffer, (void *)0x050003e0, 0x84000008, REG_DMA3);
	Vram_Load(gTitleVramBlock, 0x500, buffer + 32);
#endif
	dma = REG_DMA3;
	while (dma[2] & 0x80000000)
		;
	MemFree(buffer);
}

static void Title_RevealSpriteRow(void)
{
	u32				   *w;
	struct TitleSprite *p;
	s32					tile;
	s32					i;
	s32					n;
	s32					y;
	s32					x;
#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) \
	|| defined(TBS_EDITION_IT)
	s32 row;

	p = gTitleSprites;
	w = gTitleSprites[0].attr;
	tile = gVramBlockCache[gTitleVramBlock].offset >> 5;
	row = 0;
	y = 128;
	for (; row <= 3; row++)
	{
		for (i = 0; i <= 29; i++)
		{
			*w++ = 0;
			*w++ = (i * 8 << 16) | y | 0x2400;
			*w++ = tile;
			n = gTitleRevealFrame / 2 - i;
			if (n < 0)
				n = 0;
			if (n <= 2 && (gFrameCount & 1))
				n = 0;
			if (n != 0)
				Runtime_PushSlotEntry(p, 255);
			p++;
			tile += 2;
		}
		y += 8;
	}
#else

	p = gTitleSprites;
	w = gTitleSprites[0].attr;
	tile = gVramBlockCache[gTitleVramBlock].offset >> 5;
	i = 0;
	y = 0x88;
	do
	{
		x = 232 - (18 - i) * 8;
		*w++ = 0;
		*w++ = (x << 16) | y | 0x8400;
		*w++ = 0xf000 | tile;
		n = gTitleRevealFrame / 2 - i;
		if (n < 0)
			n = 0;
		if (n <= 2 && (gFrameCount & 1))
			n = 0;
		if (n != 0)
			Runtime_PushSlotEntry(p++, 255);
		tile += 2;
	} while (++i <= 17);
#endif
	gTitleRevealFrame++;
}

static void Title_RevealScreen(s32 unused)
{
	s32 i;

	Title_LoadBackground();
	Engine_EventWait(30);
	gTitleRevealFrame = 0;
	Title_LoadSprites(0);
	Engine_TaskAddCallback(Title_RevealSpriteRow, 0xc80);
	IO_WRITE_QUEUE_PUSH(0x4000000, 0x1540, 0x20000);
	IO_WRITE_QUEUE_PUSH(0x4000050, 0x2fce, 0x20000);
	IO_WRITE_QUEUE_PUSH(0x4000054, 16, 0x20000);
	IO_WRITE_QUEUE_PUSH(0x4000052, 0x1010, 0x20000);
	Engine_EventWait(120);
	for (i = 0; i <= 16; i++)
	{
		IO_WRITE_QUEUE_PUSH(0x4000054, 16 - i, 0x20000);
		Engine_TaskWait(3);
	}
	gEventWork->start_transition = 0;
	gEventWork->transition_frames = 1;
	Engine_EventOpenScreen();
	Engine_EventWaitForScreen();
	gEventWork->transition_frames = 60;
}

static void Title_LoadBackground(void)
{
	u8				  *res;
	s32				   id;
	u16				  *map;
	u32				   x;
	u32				   y;
	u16				   tile;
	struct ScrollPair *scroll;
	s32				   blank;

	id = (s32)&ResourceId_GoldenSunLogo;
	Blend_SetDarkenTarget16(0);
	REG_BG2CNT = 0x681;
	gBgScroll[2].y = 0;
	blank = 0x1ff;
	res = Resource_GetTableEntry(id);
	Dma_Set(res, (void *)0x05000000, 0x84000070, REG_DMA3);
	res += 0x1c0;
	FieldResource_DecodeType01(res, Ram_MapCellBuffer);
	Dma_Set((void *)Ram_MapCellBuffer, (void *)0x06006800, 0x84002580, REG_DMA3);
	map = (u16 *)0x06003000;
	tile = 0x1a0;
	for (y = 0; y <= 19; y++)
	{
		for (x = 0; x <= 29; x++)
			*map++ = tile++;
		*map++ = blank;
		*map++ = blank;
	}
	scroll = (struct ScrollPair *)gBgScroll;
	for (y = 0; y <= 3; y++)
	{
		scroll->y = 0;
		scroll->x = 0;
		scroll++;
	}
	Dma_Set(gBgScroll, (void *)0x04000010, 0x84000004, REG_DMA3);
	((struct TitleWork *)gMapWork[0])->mode = 0x1400;
}

struct SceneEntrance gTitleEntrances[] = {
	{ 0, CONDITION_ALWAYS, 0, 0, 0, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ SCENE_TABLE_END },
};

u32 gTitleExits[] = {
	SCENE_EXITS(511),
};

struct ScenePlacement gTitlePlacements[] = {
	{ SCENE_TABLE_END },
};

struct SceneEvent gTitleEvents[] = {
	{ SCENE_EVENTS_END },
};

s16 gTitleVramBlock = -1;
