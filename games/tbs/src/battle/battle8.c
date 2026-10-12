#include "vram_tab.h"
#include "window.h"
#include "unit/battle/battle.h"
#include "unit/battle/battle2.h"
#include "unit/battle/battle3.h"
#include "unit/battle/battle4.h"
#include "unit/battle/battle5.h"
#include "unit/battle/battle6.h"
#include "unit/battle/battle7.h"
#include "unit/battle/battle8.h"
#include "unit/battle/battle9.h"
#include "unit/battle/battle10.h"
#include "unit/battle/battle11.h"
#include "unit/battle/battle12.h"
#include "unit/battle/effect/object.h"
#include "unit/battle/effect/object10.h"
#include "unit/battle/effect/object11.h"
#include "unit/battle/effect/object12.h"
#include "unit/battle/effect/object13.h"
#include "unit/battle/effect/object14.h"
#include "unit/battle/effect/object15.h"
#include "unit/battle/effect/object16.h"
#include "unit/battle/effect/object17.h"
#include "unit/battle/effect/object18.h"
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
#include "keystate.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "owner_state.h"
#include "edition.h"
#include "io_reg.h"
#include "battle_unit.h"
#include "game_state.h"
#include "btlsys.h"
#include "text/msg_ids.h"
#include "mem.h"
#include "resource_ids.h"
#include "callback_scheduler.h"
#include "fixed_point_position.h"
#include "iwram_call.h"
#include "global_cells.h"
#include "btldisp.h"
#include "btlmes.h"
#include "btlpty.h"
#include "battle_intro.h"
#include "inventory.h"
#include "dma.h"
#include "scene.h"
#include "btlwork.h"
#include "btltype.h"
#include "btlsmn.h"
#include "tbs_edition.h"
#include "serial_runtime.h"
#include "ram_buffer.h"
#include "party_state.h"
#include "battle_command.h"
#include "resource.h"
#include "heap_state.h"
#include "flag_ids.h"
#include "menu_list.h"
#include "audio_engine_symbols.h"
#include "ui.h"
#include "btlcalc.h"
#include "objdisp.h"
#include "motobj.h"
#include "battle_status_icon.h"
#include "animspr.h"
#include "field_event.h"
#include "metadata_lookup.h"
#include "fixed_math.h"
#include "battle_motion.h"
#include "effect_step.h"
#include "battle_escape.h"
#include "battle_target.h"
#include "btlobj.h"
#include "btlev.h"
#include "project.h"
#include "battle_random.h"
#include "irqlock.h"
#include "transform.h"
#include "battle_efx.h"
#include "character.h"
#include "io_write_queue.h"
#include "item.h"
#include "irq.h"
#include "vram_block.h"
#include "ownerval.h"
#include "sound_ids.h"
#include "ramcode.h"
#include "map_scroll.h"
#include "iowrite.h"
#include "battle_formation.h"
#include "b5_context.h"
#include "frame.h"

s32 BattlePres_RunMosaicFadeOut(void)
{
	volatile u16  bg0cnt;
	volatile u16  bg1cnt;
	volatile u16  bg2cnt;
	volatile u16  bg3cnt;
	s32			  i;
	volatile u16 *reg;

	bg0cnt = REG_BG0CNT;
	REG_BG0CNT = bg0cnt | 0x40;
	reg = &REG_BG0CNT;
	bg1cnt = *++reg;
	*reg = bg1cnt | 0x40;
	bg2cnt = *++reg;
	*reg = bg2cnt | 0x40;
	bg3cnt = *++reg;
	*reg = bg3cnt | 0x40;
	reg += 0x21;
	*reg = 0x3eee;

	Blend_SetDarkenTarget16(16);
	for (i = 0; i <= 15; i++)
	{
		Random16();
		Random16();
		Random16();
		Random16();
		REG_MOSAIC = (i << 8) | i;
		WaitFrames(1);
	}

	REG_DISPCNT = 1;
	WaitFrames(4);
	reg = &REG_BG0CNT;
	*reg++ = bg0cnt;
	*reg++ = bg1cnt;
	*reg++ = bg2cnt;
	*reg = bg3cnt;
	return 0;
}

//--------------------------------------------------------------------------
// build_sequential_table.c
// タイル：1 バイトずつ連番のマップを作る
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// 0〜255 と 0〜223 の連番のバイトを続けて書き、
// 残りの 544 バイトを 0xff で埋める（あわせて 1024 バイト）
//--------------------------------------------------------------------------
void Graphics_BuildSequentialTileTable(s32 *dst)
{
	s32 v;
	s32 n;

	// 4 バイトずつ書くので 1 回に 4 つ進める
	v = 0x03020100;
	n = 0;
	do
	{
		n += 1;
		*dst++ = v;
		v += 0x04040404;
	} while (n >= 0 && n <= 63);

	v = 0x03020100;
	n = 0;
	do
	{
		n += 1;
		*dst++ = v;
		v += 0x04040404;
	} while (n >= 0 && n <= 55);

	Iwram_FillWords(dst, 544, -1);
}

void BattlePresentation_BuildTilemap(s32 *destination)
{
	s32 entry;
	u32 index;

	Iwram_FillWords(destination, 0x100, -1);
	destination += 0x40;
	Iwram_FillWords(destination, 0x80, 0x03ff03ff);
	entry = (0x0201 << 16) | 0x0200;
	destination += 0x20;
	index = 0;
	do
	{
		index++;
		*destination++ = entry;
		entry += 0x00020002;
	} while (index <= 239);
	Iwram_FillWords(destination, 0x280, 0x03ff03ff);
}

void BattlePres_UpdateHBlankScroll(void)
{
	struct BattleAffineHdma *records;
	u16						*record;

	if (((struct BattleBackgroundView *)WORK_SLOT(HEAP_SLOT_BATTLE_BACKGROUND))->mode == 2)
	{
		records = WORK_SLOT(10);
		record = records->lines[records->page];
		Io_Write16(record[0], &REG_BG2CNT);
		Dma_Set(record + 1, (void *)0x0400000c, 0xa2600001, &REG_DMA0SAD);
		Dma_Set(&records->affine, (void *)0x04000020, 0x84000004, REG_DMA3);
	}
}

void BattlePresentation_UploadTileVariant(void)
{
	u32 variant = *gDisp - 1;
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

	frame = *gDisp;
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

		tile |= 0x800;
		aligned = frame;
		if (frame < 0)
		{
			aligned += 7;
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

struct DisplayRegisters
{
	u16 control;
	u16 green_swap;
};

void BattlePres_RunEntryTransition(s32 enemy_count, s32 kind)
{
	volatile u32				  fill;
	struct BattlePresentationWork list;
	u16							  party[14];
	u16							  actors[14];
	struct BattleBackgroundView	 *work;
	s32							 *timer;
	u16							 *map;
	s32							  tile;
	u32							  i;
	u32							  j;
	s32							  count;
	s32							  id;
	struct BtlObj				 *object;
	struct BtlWork				 *session;
	u16							 *ids;
	s32							  n;

	work = WORK_SLOT(HEAP_SLOT_BATTLE_BACKGROUND);
	timer = Runtime_AllocateHeapBlock(42, 4);
	if (kind != 0x15b)
	{
		{

			volatile u32 *dma;

			Dma_Set(BattlePres_CurtainTiles, (void *)0x06005020, 0x84000008, REG_DMA3);
			Dma_Set(BattlePres_CurtainTiles + 32, (void *)0x06005040, 0x84000008, REG_DMA3);
			Dma_Set(BattlePres_CurtainTiles + 64, (void *)0x06005060, 0x84000008, REG_DMA3);
			Dma_Set(BattlePres_CurtainTiles + 96, (void *)0x06005080, 0x84000008, REG_DMA3);
			Dma_Set(BattlePres_CurtainTiles + 128, (void *)0x060050a0, 0x84000008, REG_DMA3);
			Dma_Set(BattlePres_CurtainTiles + 160, (void *)0x060050c0, 0x84000008, REG_DMA3);
			Dma_Set(BattlePres_CurtainTiles + 192, (void *)0x060050e0, 0x84000008, REG_DMA3);
			dma = REG_DMA3;
			/* FAKEMATCH: introducing the pointer after the transfers avoids a preserved-register copy; the volatile output orders its subtraction before the control value, differing by two instructions without volatile. 2026-10-11: the native subs r3, #212 is reload_cse_move2add on a constant address that happened to land in r3; plain pointer arithmetic is CSE-reused by the later Dma_Sets (differ 45), REG_DISPCNT = 1 takes a pool constant (63), and a constant struct-field store gets r2 from local-alloc, so no move2add (31). */
			asm volatile("" : "+l"(dma));
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
		session = WORK_SLOT(HEAP_SLOT_BATTLE);
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
			list.count = BattleParty_ListActorIds(2, list.actors);
			list.actors[list.count] = 0xff;
			BattleActor_SpawnObjectsForList(list.actors, 0);
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

void BattlePresentation_SetPaletteLevel(s32 unused, s32 level)
{
	struct BtlWork *screen = BtlWk;
	u16			   *palette = screen->palette;
	u32				ime;
	u32			   *slot = &ime;

	/* FAKEMATCH: needed to match: the one-pass block and the loop on a zero local keep the IME save through slot's register */
	do
	{
		u16 *ime_reg = (u16 *)0x04000208;
		s32	 once = 0;

		do
		{
			*slot = *ime_reg;
			*ime_reg = (u32)ime_reg;
		} while (once);
	} while (0);

	if (level == 0)
	{
		Dma_Set(palette, (void *)0x050000c0, 0x80000080, REG_DMA3);
	}
	else
	{
		Graphics_ScaleRgb555Clamped(
			palette, (u16 *)0x050000c0, screen->brightness = 0x10000 - level * 1092, 128);
	}
	{
		u32 saved = *slot;

		REG_IME = saved;
	}
}

void BattlePresentation_ConfigurePaletteFade(s32 mode, s32 value, s32 fade)
{
	struct BattleBackgroundView *transition = (struct BattleBackgroundView *)gTransitionWork;

	if (transition->mode == 0)
	{
		Scheduler_AddOrUpdateCallback(BattlePres_UpdateHBlankScroll, 0x4ff);
	}
	transition->mode = mode;

	if (mode == 1)
	{
		IO_WRITE_QUEUE_PUSH(0x0400000a, 0x1f83, 0x20000);
	}

	Dma_Set((void *)0x05000200, (void *)0x050000a0, 0x80000010, REG_DMA3);
	*(u16 *)0x050000bc = *(u16 *)0x050001e8;

	if (fade == 0x80)
	{
		Dma_Set(BtlWk->palette, (void *)0x050000c0, 0x80000080, REG_DMA3);
	}
	else if (fade != 0)
	{
		u16 *source = BtlWk->palette;
		u16 *destination = (u16 *)0x050000c0;
		s32	 i;

		for (i = 0; i != 128; i++)
		{
			s32 red = source[i] & 31;
			s32 green = (source[i] >> 5) & 31;
			s32 blue = (source[i] >> 10) & 31;

			if (red > fade)
			{
				red -= fade;
			}
			else
			{
				red = 0;
			}
			if (green > fade)
			{
				green -= fade;
			}
			else
			{
				green = 0;
			}
			if (blue > fade)
			{
				blue -= fade;
			}
			else
			{
				blue = 0;
			}
			destination[i] = (blue << 10) | (green << 5) | red;
		}
	}

	Graphics_BuildSequentialTileTable((void *)0x06003800);
	BattlePresentation_BuildTilemap((void *)0x0600f800);
}

void BattlePresentation_InitializeWorkAndResetState(void)
{
	struct BattleAffineHdma		*work;
	struct BattleBackgroundView *state;
	volatile u32				 zero;
	work = Runtime_AllocateBlock(10, sizeof *work);
	state = (struct BattleBackgroundView *)gTransitionWork;
	zero = 0;
	Dma_Set(&zero, work, 0x850000a8, REG_DMA3);
	state->mode = 0;
}

void Runtime_ReleaseHeapBlock10(void)
{
	Runtime_ReleaseHeapBlock(10);
}

typedef void (*BitDecoder)(const u8 *source, void *destination);

void BattleBackground_Load(s32 mode, s32 resource, s32 level)
{
	struct BattleBackgroundView *view = WORK_SLOT(HEAP_SLOT_BATTLE_BACKGROUND);
	u8							*data = Resource_GetTableEntry(resource);
	struct BtlWork				*session = WORK_SLOT(HEAP_SLOT_BATTLE);
	u16							*palette;

	RamCode_LoadSlot(49, BitDecoder_DecodeImage, BitDecoder_Size);
	{
		u32	  offset = 0x100;
		void *vram;

		/* FAKEMATCH: conditional cancelling updates end CSE before the
		 * decoder arguments, keeping the VRAM load before the data add.
		 * They disappear from all 103 instructions in every edition;
		 * a reassignment or XOR pair instead differs by four rows. */
		if (session)
		{
			offset++;
			offset--;
		}
		vram = (void *)0x06008000;
		((BitDecoder)WORK_SLOT(49))(data + offset, vram);
	}
	Runtime_ReleaseHeapBlock(49);
	palette = session->palette;
	Dma_Set(data, palette, 0x84000040, REG_DMA3);
	if (level >= 0)
		Graphics_ScaleRgb555Clamped(
			palette, (u16 *)0x050000c0, session->brightness = 0x10000 - level * 1092, 128);
	Dma_Set((void *)0x05000200, (void *)0x050000a0, 0x80000010, REG_DMA3);
	*(u16 *)0x050000bc = *(u16 *)0x050001e8;
	Graphics_BuildSequentialTileTable((void *)0x06003800);
	BattlePresentation_BuildTilemap((void *)0x0600f800);
	Iwram_ClearWords((void *)0x0600ffc0, 64);
	if (view->mode == 0)
		Scheduler_AddOrUpdateCallback(BattlePres_UpdateHBlankScroll, 0x4ff);
	view->mode = mode;
	if (mode == 1)
		REG_BG1CNT = 0x1f83;
}

void BattleCamera_SetRange(s32 cx, s32 cy, s32 ox, s32 oy, s32 scale)
{
	struct BattleBackgroundView *view = WORK_SLOT(HEAP_SLOT_BATTLE_BACKGROUND);
	struct BattleAffineHdma		*work = WORK_SLOT(10);
	struct BattleCamera			*camera = WORK_SLOT(HEAP_SLOT_CAMERA);
	u16							 wrap;
	s32							 horizon;
	u16							*line;
	struct BattleAffine			*affine;
	s32							 ratio;
	s32							 step;
	s32							 x;
	s32							 y;
	u32							 i;
	u32							 count;

	horizon = 0x800;
	wrap = 0;
	if (scale >= 0x10000)
	{
		wrap = 0x2000;
		horizon = 0x6800 + -(s16)camera->yaw * 3;
	}
	if (work == NULL)
		return;
	if ((view->mode == 1 || view->second_mode == 1) && view->busy == 0)
		gBgScroll[1].x = horizon >> 8;
	if (view->mode != 2)
		return;
	line = work->lines[work->page ^ 1];
	ratio = Iwram_RatioMulQ14(scale, 0x10000);
	step = ratio >> 8;
	affine = &work->affine;
	affine->pa = step;
	affine->pb = 0;
	affine->pc = 0;
	affine->pd = step;
	x = Iwram_MulQ16(ratio, Iwram_MulQ16(cx, scale - 0x10000));
	y = Iwram_MulQ16(ratio, Iwram_MulQ16(cy, scale - 0x10000));
	affine->x = ((x + 0x7fff) >> 8) + ox + horizon;
	y = ((y + 0x7fff) >> 8) + oy - 0x1000;
	affine->y = y;
	count = (Iwram_RatioMulQ14((s16)step, 0x4000 - y) >> 16) + 1;
	i = 0;
	if (!GameFlag_TestFar(0x16b))
	{
		for (; i < 16; i++)
			*line++ = 0x3f8e;
	}
	if (count > 136)
		count = 136;
	for (; i < count; i++)
		*line++ = wrap | 0x478a;
	for (; i < 136; i++)
		*line++ = wrap | 0x478e;
	for (; i < 160; i++)
		*line++ = 0x3f8e;
	work->page ^= 1;
}

void BattlePres_SetupTransitionSceneAtDepth(s32 x, s32 depth, s32 y)
{
	s32 span = 0x01fe0000;
	// 元のバグ：mode は初期化されず、入口の r4 をそのまま読む（lsls r0, r4, #16）
	s32					 mode;
	struct BattleCamera *scene = gCameraWork;
	s32					*pos = scene->pos;
	s32					 scale = (mode << 16) / 100;
	s32					 render_bounds[3];
	s32					 measured_bounds[3];
	s32					 source_bounds[3];
	s32					 alpha;
	s32					 half;
	s32					 width;
	u32					 result;

	pos[0] = x;
	pos[1] = depth;
	pos[2] = y;
	source_bounds[0] = 0;
	alpha = 0xc000;
	source_bounds[1] = 0;
	source_bounds[2] = 0;

	result = Iwram_RatioMulQ14(span, alpha);
	Camera_StoreSceneParameters(span, result, span * 2);
	Render_ResetTransformState();
	SceneTransform_ApplyPosition(pos);
	SceneTransform_ApplyYaw((s16)scene->yaw);
	SceneTransform_ApplyPitch((s16)scene->pitch);
	render_bounds[0] = 0;
	render_bounds[1] = 0;
	render_bounds[2] = scene->distance;
	Iwram_TransformVector(render_bounds, (s32 *)scene);
	gProjection.center_x = 120;
	gProjection.center_y = 120;
	Render_ResetTransformState();
	Graphics_PrepareTransferInIwramWork(scene->eye, pos);
	Render_ProjectPoint(source_bounds, measured_bounds);

	BattleCamera_SetRange(0x780000, 0x780000, (120 - measured_bounds[0]) << 8,
		(120 - measured_bounds[1]) << 8, scale);
	half = scale * 0xff;
	width = half * 2;
	result = Iwram_RatioMulQ14(width, alpha);
	Camera_StoreSceneParameters(width, result, half * 4);
}

void BattlePres_SetupTransitionScene(s32 x, s32 depth, s32 y, s32 zoom)
{
	struct BattleCamera *scene = gCameraWork;
	s32					*pos = scene->pos;
	s32					 scale = (zoom << 16) / 100;
	s32					 render_bounds[3];
	s32					 measured_bounds[3];
	s32					 source_bounds[3];
	s32					 span = 0x01fe0000;
	s32					 alpha;
	s32					 half;
	s32					 width;
	u32					 result;

	pos[0] = x;
	pos[1] = depth;
	pos[2] = y;
	source_bounds[0] = 0;
	alpha = 0xc000;
	source_bounds[1] = 0;
	source_bounds[2] = 0;

	result = Iwram_RatioMulQ14(span, alpha);
	Camera_StoreSceneParameters(span, result, span * 2);
	Render_ResetTransformState();
	SceneTransform_ApplyPosition(pos);
	SceneTransform_ApplyYaw((s16)scene->yaw);
	SceneTransform_ApplyPitch((s16)scene->pitch);
	render_bounds[0] = 0;
	render_bounds[1] = 0;
	render_bounds[2] = span;
	Iwram_TransformVector(render_bounds, (s32 *)scene);
	gProjection.center_x = 120;
	gProjection.center_y = 120;
	Render_ResetTransformState();
	Graphics_PrepareTransferInIwramWork(scene->eye, pos);
	Render_ProjectPoint(source_bounds, measured_bounds);

	BattleCamera_SetRange(0x780000, 0x780000, (120 - measured_bounds[0]) << 8,
		(120 - measured_bounds[1]) << 8, scale);
	half = scale * 0xff;
	width = half * 2;
	result = Iwram_RatioMulQ14(width, alpha);
	Camera_StoreSceneParameters(width, result, half * 4);
}

//--------------------------------------------------------------------------
// setup_transition_at_pair_midpoint.c
// バトル表示：ふたりの中間へのカメラ合わせ
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// ふたつのオブジェクトの中間（x, z）にカメラを合わせる
// a, b : オブジェクト番号
// zoom : 拡大率（%）
//--------------------------------------------------------------------------
void BattlePres_SetupTransitionAtPairMidpoint(s32 a, s32 b, s32 zoom)
{
	struct MotionObject *obj_a = BtlObjGet(a)->object;
	struct MotionObject *obj_b = BtlObjGet(b)->object;
	s32					 ax = obj_a->x;
	s32					 bx = obj_b->x;
	s32					 az = obj_a->z;
	s32					 bz = obj_b->z;
	s32					 x = (bx + ax) / 2;
	s32					 z = (bz + az) / 2;

	BattlePres_SetupTransitionScene(x, 0, z, zoom);
}

void Graphics_FadeAlphaOut(void)
{
	s32 alpha_step;

	REG_BLDCNT = 0x2044;
	for (alpha_step = 1; alpha_step <= 16; alpha_step += 2)
	{
		REG_BLDALPHA = 0x1010 - alpha_step;
		WaitFrames(1);
	}
}

void Graphics_FadeAlphaIn(void)
{
	s32 step;

	REG_BLDCNT = 0x2044;
	for (step = 1; step <= 16; step += 2)
	{
		REG_BLDALPHA = step + 0x1000;
		WaitFrames(1);
	}
}

struct BattleTransitionEntry
{
	u32 value;
	u32 sum;
	u32 field8;
	u32 fieldc;
};

void Graphics_SetBlendControl(void)
{
	REG_BLDCNT = 0xbf;
}

void BattlePresentation_InitializeTransitionEntries(struct BattleTransitionEntry *entries)
{
	u32 previous = entries[0].value;

	CAMELOT_ASM("the fixed-register identity store of transform.h")
	Transform_SetIdentity(entries);
	entries[0].sum = previous + entries[0].value;
}

s32 BattlePres_DivideBy16(s32 value)
{
	return value / 16;
}

#if EDITION_INTERNATIONAL
#define DEBUG_SELECT_VALUE 0x18f
#else
#define DEBUG_SELECT_VALUE 480
#endif

s32 Runtime_AdjustDebugValueWithButtons(s32 ret)
{
	struct BtlWork *battle;
	volatile u32   *keys;

	if (gKeysHeld & KEY_START)
	{
		keys = &gKeysRepeat;
	loop:
		battle = BtlWk;
		if (*keys & 0x20)
			battle->debug_value -= 1;
		if (*keys & 0x10)
			battle->debug_value += 1;
		if (*keys & 0x40)
			battle->debug_value -= 100;
		if (*keys & 0x80)
			battle->debug_value += 100;
		if (gKeyState & KEY_A)
		{
			ret = battle->debug_value;
			goto done;
		}
		WaitFrames(1);
		goto loop;
	}
done:
	if (gKeysHeld & KEY_SELECT)
		ret = DEBUG_SELECT_VALUE;
	return ret;
}

void BattlePres_SetActorRecordMode(s32 actor_id, s32 mode)
{
	struct BtlObj			*slot = BtlObjGet(actor_id);
	struct MotionObject		*object;
	struct AnimationObject	*record;
	struct AnimationObject **records;
	s32						 i;

	if (slot == 0)
		return;
	object = slot->object;
	if (object == 0)
		return;
	switch (object->record_storage_kind & 15)
	{
	case 1:
	{
		record = object->records;
		record->part[0].object_mode = mode;
		record->part[1].object_mode = mode;
		break;
	}
	case 2:
	{
		records = object->records;
		i = 0;
		do
		{
			record = *records++;
			if (record == 0)
				break;
			record->part[0].object_mode = mode;
			record->part[1].object_mode = mode;
			i++;
		} while (i <= 3);
		break;
	}
	}
}

//--------------------------------------------------------------------------
// present.c
// バトル表示：ユニットの絵の半透明の切りかえ
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// except 以外の全ユニットの絵を半透明（OBJ モード 1）にする
// except : そのままにするユニット番号
//--------------------------------------------------------------------------
void BattlePres_SetOtherActorRecordModes(s32 except)
{
	s16 list[14];
	s32 cnt = BattleParty_ListActorIds(BATTLE_SIDE_BOTH, list);
	s32 i;

	for (i = 0; i < cnt; i++)
	{
		if (list[i] != except)
			BattlePres_SetActorRecordMode(list[i], 1);
	}
}

//--------------------------------------------------------------------------
// 全ユニットの絵をふつう（OBJ モード 0）にもどす
//--------------------------------------------------------------------------
void BattlePres_ClearAllActorRecordModes(void)
{
	s16 list[14];
	s32 cnt = BattleParty_ListActorIds(BATTLE_SIDE_BOTH, list);
	s32 i;

	for (i = 0; i < cnt; i++)
		BattlePres_SetActorRecordMode(list[i], 0);
}

void Graphics_AdvancePaletteCycle(void)
{
	struct BtlWork *battle = BtlWk;
	s8			   *table;
	u16				step;
	s32				next;

	if (battle != NULL && battle->highlight_mode != 0)
	{
		REG_BLDCNT = 0x3f90;
		REG_BLDALPHA = 0x10;
		table = BattleFx_HighlightBlendRamp;
		REG_BLDY = table[battle->highlight_step];
		step = battle->highlight_step;
		next = (step + 1) & 0xf;
		if (step > 14)
			next |= 0x10;
		battle->highlight_step = next;
	}
}

void BattlePres_SetActorModes(u16 *actors, s32 mode)
{
	s16				active_actors[14];
	struct BtlWork *battle = BtlWk;
	u32				count;
	u32				i;

	if (mode == 0)
	{
		Scheduler_RemoveCallback(Graphics_AdvancePaletteCycle);
		REG_BLDY = mode;
		BattlePres_ClearAllActorRecordModes();
		WaitFrames(1);
		QueueIoWriteDelay2(0x04000050, 0);
	}
	if (battle != 0 && mode != 0)
	{
		battle->highlight_mode = mode;
		battle->highlight_step = 0;
		REG_BLDY = 0;
		Io_Write16(16, &REG_BLDALPHA);

		count = BattleParty_ListActorIds(3, active_actors);
		for (i = 0; i < count; i++)
			BattlePres_SetActorRecordMode(active_actors[i], mode & 1);

		if (actors != 0)
		{
			u32 actor = *actors;
			i = 0;
			actors++;
			if (actor != 0xff)
			{
				do
				{
					BattlePres_SetActorRecordMode(actor, (mode & 1) ^ 1);
					i++;
					if (i > 13)
						break;
					actor = *actors++;
				} while (actor != 0xff);
			}
		}
		WaitFrames(1);
		QueueIoWriteDelay2(0x04000050, 0);
		Scheduler_AddOrUpdateCallback(Graphics_AdvancePaletteCycle, 0x480);
	}
}
