/* Pin-free BattlePres_RunEntryTransition trial: all six editions 414 native,
 * 415 built, 397 common, differ 35. Ordinary dma alone differs 49; keeping
 * the pointer at the display-store boundary reduces that to 35. Input-only,
 * low-register and general output constraints all retain 35; an input after
 * the display store differs 38 (416 built). The pointer stays in r4 and
 * gains a copy into the DMA helper's r3; the later control and work stores
 * change order. Linked source retains its pin pending whole-image identity. */
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "iwram_call.h"
#include "dma.h"
#include "scene.h"
#include "ram_buffer.h"
#include "btldisp.h"
#include "map_scroll.h"
#include "io_reg.h"
#include "heap_state.h"
#include "mem.h"
#include "btlpty.h"
#include "callback_scheduler.h"
#include "io_write_queue.h"
#include "btlwork.h"
#include "resource_ids.h"
#include "motobj.h"
#include "btlsys.h"
#include "irq.h"
#include "battle_intro.h"
#include "resource.h"
#include "project.h"
#include "transform.h"

static __inline__ void FillWords(void *dst, s32 size, s32 value)
{
	/* FAKEMATCH: needed to match: a direct call changes BattlePresentation_BuildTilemap from push {r5, r6, lr} to push {r5, r6, r7, lr} */
	Iwram_FillWords(dst, size, value);
}

extern void *gWorkSlot[64];

extern u8 gDisp[];

void BattlePresentation_BuildTilemap(s32 *destination)
{
	s32 entry;
	u32 index;

	FillWords(destination, 0x100, -1);
	destination += 0x40;
	FillWords(destination, 0x80, 0x03ff03ff);
	entry = (0x0201 << 16) | 0x0200;
	destination += 0x20;
	index = 0;
	do
	{
		index++;
		*destination++ = entry;
		entry += 0x00020002;
	} while (index <= 239);
	FillWords(destination, 0x280, 0x03ff03ff);
}

void BattlePres_UpdateHBlankScroll(void)
{
	struct BattleAffineHdma *records;
	u16						*record;
	s32						 control;

	if (((struct BattleBackgroundView *)*(
			 void **)((u8 *)gWorkSlot + HEAP_SLOT_BATTLE_BACKGROUND * sizeof(void *)))
			->mode
		== 2)
	{
		records = *(struct BattleAffineHdma **)&gWorkSlot[10];
		record = records->lines[records->page];
		control = record[0];
		/* FAKEMATCH: needed to match: the do-while keeps the BG2CNT address load after the record read */
		do
		{
			REG_BG2CNT = control;
		} while (0);
		Dma_Set(record + 1, (void *)0x0400000c, 0xa2600001, &REG_DMA0SAD);
		Dma_Set(&records->affine, (void *)0x04000020, 0x84000004, REG_DMA3);
	}
}

void BattlePresentation_UploadTileVariant(void)
{
	u32 variant = **(u32 **)gDisp - 1;
	if (variant <= 31)
		Dma_Set(BattlePres_TileVariants + (variant >> 2) * 32, (void *)0x06005000, 0x84000008,
			REG_DMA3);
}

void BattlePres_AdvanceTransitionTimer(void)
{
	s32					 scroll;
	struct BattleCamera *camera;
	u32					*timer;
	struct BgScroll		*offset;
	u32					 frame;
	u32					 next;

	timer = *(u32 **)Ram_Disp;
	frame = *timer;
	camera = *(struct BattleCamera **)Ram_CameraWork;
	scroll = 0x34 - frame;
	if (scroll > 0x20)
		scroll = 0x20;
	offset = (struct BgScroll *)Ram_BgScroll;
	if (scroll < 0)
		scroll = 0;
	offset->y = scroll;
	if (frame <= 0x50U)
		camera->yaw = 45 * frame * 8 + 0xaf80;
	next = (*timer = *timer + 1);
	if (next <= 0x50U)
	{
		BattlePres_SetupTransitionScene(0, 0, 0, 0xb4 - next);
		return;
	}
	BattlePres_SetupTransitionScene(0, 0, 0, 0x64);
}

void BattlePres_DrawTransitionRows(void)
{
	u32	 i;
	s32	 frame;
	s32	 aligned;
	s32	 tile;
	u32	 row;
	u16 *tiles;

	frame = **(s32 **)gDisp;
	if (frame >= 0 && frame <= 79)
	{
		tile = (7 & frame) + 0xf081;
		if (frame >= 0)
		{
			aligned = frame;
		}
		else
		{
			aligned = frame + 7;
		}
		row = 13 - (aligned >> 3);
		i = 0;
		tiles = (u16 *)((row << 6) + 0x06006000);
		do
		{
			i++;
			*tiles = tile;
			tiles++;
		} while (i != 32);

		tile = tile | 0x800;
		aligned = frame;
		if (frame < 0)
		{
			aligned = aligned + 7;
		}
		row = (aligned >> 3) + 13;
		if (row <= 20)
		{
			i = 0;
			tiles = (u16 *)((row << 6) + 0x06006000);
			do
			{
				i++;
				*tiles = tile;
				tiles++;
			} while (i != 32);
		}
	}
}

void Graphics_ClearBg0Vofs(void)
{
	u32 zero = 0;

	REG_BG0VOFS = zero;
}

#define BattleActor_SpawnObjectsForList ((void (*)(u16 *, s32))BattleActor_SpawnObjectsForList)
#define Graphics_BuildSequentialTileTable ((void (*)(void *))Graphics_BuildSequentialTileTable)
#define BattlePresentation_BuildTilemap ((void (*)(void *))BattlePresentation_BuildTilemap)

#define gTransitionWork ((u8(*)) & gTransitionWork)

struct DisplayRegisters
{
	u16 control;
	u16 green_swap;
};

struct TransitionActorList
{
	u8	unknown_00[20];
	s32 count;
	u8	unknown_18[12];
	u16 ids[24];
};

void BattlePres_RunEntryTransition(s32 enemy_count, s32 kind)
{
	volatile u32				 fill;
	struct TransitionActorList	 list;
	u16							 party[14];
	u16							 actors[14];
	struct BattleBackgroundView *work;
	s32							*timer;
	u16							*map;
	s32							 tile;
	u32							 i;
	u32							 j;
	s32							 count;
	s32							 id;
	struct BtlObj				*object;
	struct BtlWork				*session;
	u16							*ids;
	s32							 n;

	work = *(gWorkSlot + HEAP_SLOT_BATTLE_BACKGROUND);
	timer = Runtime_AllocateHeapBlock(42, 4);
	if (kind != 0x15b)
	{
		{

			volatile u32 *dma = REG_DMA3;

			Dma_Set(BattlePres_CurtainTiles, (void *)0x06005020, 0x84000008, dma);
			Dma_Set(BattlePres_CurtainTiles + 32, (void *)0x06005040, 0x84000008, dma);
			Dma_Set(BattlePres_CurtainTiles + 64, (void *)0x06005060, 0x84000008, dma);
			Dma_Set(BattlePres_CurtainTiles + 96, (void *)0x06005080, 0x84000008, dma);
			Dma_Set(BattlePres_CurtainTiles + 128, (void *)0x060050a0, 0x84000008, dma);
			Dma_Set(BattlePres_CurtainTiles + 160, (void *)0x060050c0, 0x84000008, dma);
			Dma_Set(BattlePres_CurtainTiles + 192, (void *)0x060050e0, 0x84000008, dma);
			/* FAKEMATCH: keep the DMA pointer at the display-store boundary; removing this boundary increases 35 differing instructions to 49. */
			asm("" : "+l"(dma));
			((struct DisplayRegisters *)((u8 *)dma - 212))->control = 1;
		}
		work->second_mode = 1;
		work->mode = 1;
		work->busy = 0;
		fill = 0x33333333;
		Dma_Set((void *)&fill, (void *)0x06005000, 0x85000008, REG_DMA3);
		fill = 0;
		Dma_Set((void *)&fill, (void *)0x06005100, 0x85000008, REG_DMA3);
		REG_BG1CNT = 0xc04;
		REG_BG0CNT |= 2;
		work->mode = 2;
		map = (u16 *)0x06006000;
		for (i = 0; i <= 31; i++)
		{
			if (i <= 20)
				tile = 0xf080;
			else
				tile = 0xf088;
			for (j = 0; j <= 31; j++)
				*map++ = tile;
		}
		{
			s32 start = 0;

			gBgScroll[0].y = 32;
			gBgScroll[1].y = 32;
			gBgScroll[1].x = 8;
			WaitFrames(1);
			REG_WIN0H = 0xf0;
			REG_WIN0V = 0x88;
			REG_WIN1H = 0xf0;
			REG_WIN1V = 0x88;
			REG_WININ = 0x3537;
			REG_WINOUT = 0x3f21;
			QueueIoWriteDelay2(0x04000000, 0x7741);
			BattlePres_SetupTransitionScene(0, 0, 0, 180);
			*timer = start;
			Scheduler_AddOrUpdateCallback(BattlePres_AdvanceTransitionTimer, 3200);
			Scheduler_AddOrUpdateCallback(BattlePres_DrawTransitionRows, 0x480);
			Runtime_SetIrqHandler(2, 32, Graphics_ClearBg0Vofs);
			gBgScroll[0].y = 32;
			WaitFrames(1);
			UiWindow_CreateWithLayoutBoundsFar(BtlWk->pty_st);
			WaitFrames(20);
			QueueIoWriteDelay10(0x04000008, 2);
			QueueIoWriteDelay6(0x04000008, 0);
			BattleIntro_AnnounceEncounter(enemy_count);
			Scheduler_RemoveCallback(BattlePres_AdvanceTransitionTimer);
			Scheduler_RemoveCallback(BattlePres_DrawTransitionRows);
			gBgScroll[0].y = start;
			Runtime_SetIrqHandler(2, 0, 0);
		}
	}
	else
	{
		session = *(gWorkSlot + HEAP_SLOT_BATTLE);
		work->second_mode = 1;
		work->busy = 0;
		count = BattleParty_ListActorIds(3, (u16 *)party);
		for (i = 0; i != count; i++)
		{
			id = i + 120;
			if ((s32)i <= 7)
				id = i;
			object = BtlObjGet(id);
			if (UnitGetFar(id)->class_id != 148)
				object->scale = 0xb333;
		}
		IO_WRITE_QUEUE_PUSH(0x04000000, 0x6041, 0x20000);
		WaitFrames(1);
		{
			s32 start = 0;

			session->background = (u32)&ResourceId_VinasuChojoStormBackdrop;
			list.count = BattleParty_ListActorIds(2, list.ids);
			list.ids[list.count] = 0xff;
			BattleActor_SpawnObjectsForList(list.ids, 0);
			BattleEffect_RunTileAndPaletteAnimationFar(&list);
			BattlePres_SetupTransitionScene(0, 0, 0, 100);
			*timer = start;
		}
		Runtime_SetIrqHandler(2, 32, Graphics_ClearBg0Vofs);
		WaitFrames(1);
		WaitFrames(20);
		UiWindow_CreateWithLayoutBoundsFar(BtlWk->pty_st);
		QueueIoWriteDelay10(0x04000008, 2);
		QueueIoWriteDelay6(0x04000008, 0);
		REG_BLDCNT = 0x3f40;
		ids = actors;
		n = BattleParty_ListActorIds(3, (u16 *)ids);
		actors[n] = 0xff;
		BattleActor_SpawnObjectsForList(ids, 0);
		n = BattleParty_ListActorIds(1, (u16 *)ids);
		for (i = 0; i != n; i++)
			BattlePres_SetActorRecordMode(ids[i], 1);
		for (i = 0; i != 16; i++)
		{
			REG_BLDALPHA = i | 0x1000;
			WaitFrames(1);
		}
		for (i = 0; i != n; i++)
			BattlePres_SetActorRecordMode(ids[i], 0);
		BattleIntro_AnnounceEncounter(enemy_count);
		gBgScroll[0].y = 0;
		WaitFrames(1);
		Runtime_SetIrqHandler(2, 0, 0);
	}
	Runtime_SetIrqHandler(2, 0, 0);
	REG_BG1CNT = 0x1f83;
	WaitFrames(1);
	REG_BG1CNT = 0x1f83;
	REG_BG0CNT &= 0xfffd;
	gBgScroll[1].x = 8;
	REG_DISPCNT = 0x1541;
	Runtime_ReleaseHeapBlock(42);
}
