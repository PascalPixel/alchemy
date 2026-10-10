/* Near miss: BattlePres_RunEntryTransition, sole EN 080c02a4 draft.
 * Current-source no-barrier attempt, 2026-10-10.
 *
 * All six native functions contain 414 instructions. This candidate contains
 * 415, common 402, differ 25, measured in the complete entry.c translation
 * unit and independently in this minimal includes/structs/function draft.
 * Native extents, pools included:
 * EN 080c02a4..080c0700; JA 080b72a4..080b7700;
 * DE 080c28bc..080c2d18; ES/FR 080c42bc..080c4718;
 * IT 080c02bc..080c0718. Complete ROM gates still required for adoption.
 *
 * Bare deletion from the complete current TU gives 415/common392/differ45.
 * The live tag describes a different experiment and is accurate: retaining
 * the asm while deleting only its volatile qualifier gives 414/common413/
 * differ2, moving movs r2,#1 before the DMA-to-DISPCNT subtraction. Complete
 * barrier deletion has a wider pointer-lifetime effect: first CSE reuses the
 * known DMA address for later transfers, requiring an extra preserved r5 copy
 * and changing downstream zero/one choices.
 * Restoring the s32 control local under the unchanged kind predicate keeps
 * the native DMA-to-DISPCNT subtraction and native r2 control store. Remaining
 * difference: an extra movs r3,#1 and r3 mode stores replace reuse of r2;
 * the extra instruction shifts ensuing literal loads and two loop branches.
 * No barrier or register pin was added; the proven Dma_Set helper is unchanged.
 *
 * Bounded trials, complete current TU (no compiler or routing changes):
 * - Four pointer/store forms: volatile hardware store 60, direct REG_DISPCNT
 *   63, const DMA pointer 49, u8 DMA pointer 45 differing rows.
 * - Eight separate zero/restore pairs under kind/enemy_count/work/timer:
 *   saved-pointer forms 155/149/76/139; constant restores all 31.
 * - Eight pointer increment/decrement forms, split or single scope under
 *   those four predicates: all 31. Two XOR pairs and byte subtract/restore
 *   also 31; zero/restore after the control store 46.
 * - Eight control/offset restores, s32/u16 under kind/work: s32 control 25,
 *   s32 offset 31; u16 control 51/47 and u16 offset 147/76.
 * - Two explicit reuses of control in the background mode stores, inner or
 *   function scope: both 31, folding the address into mov/lsl instead.
 *
 * Keep the whole-TU measurement when changing this draft: prior function
 * declarations and compilation context can affect scheduling in this compiler.
 */
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

extern void *gWorkSlot[64];

void BattlePres_AdvanceTransitionTimer(void);
void BattlePres_DrawTransitionRows(void);

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

			volatile u32 *dma;
			s32 control;

			Dma_Set(BattlePres_CurtainTiles, (void *)0x06005020, 0x84000008, REG_DMA3);
			Dma_Set(BattlePres_CurtainTiles + 32, (void *)0x06005040, 0x84000008, REG_DMA3);
			Dma_Set(BattlePres_CurtainTiles + 64, (void *)0x06005060, 0x84000008, REG_DMA3);
			Dma_Set(BattlePres_CurtainTiles + 96, (void *)0x06005080, 0x84000008, REG_DMA3);
			Dma_Set(BattlePres_CurtainTiles + 128, (void *)0x060050a0, 0x84000008, REG_DMA3);
			Dma_Set(BattlePres_CurtainTiles + 160, (void *)0x060050c0, 0x84000008, REG_DMA3);
			Dma_Set(BattlePres_CurtainTiles + 192, (void *)0x060050e0, 0x84000008, REG_DMA3);
			dma = REG_DMA3;
			/* FAKEMATCH: the restored control value ends first CSE's known
			 * DMA-address lifetime. Without this scope, deleting the barrier
			 * gives 45 differing rows; this no-barrier form gives 25 in all
			 * six editions, retaining the native subtraction and control store.
			 * kind is unchanged here and both temporary assignments vanish. */
			control = 1;
			if (kind)
				control = 0;
			if (kind)
				control = 1;
			((struct DisplayRegisters *)((u8 *)dma - 212))->control = control;
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
