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

struct Spark
{
	s32 pos[6];
	s32 life;
};

struct SparkWork
{
	u8			 sheet[0x11c0];
	struct Spark sparks[16];
	struct Ring	 rings[3];
	s32			 frames;
	s32			 ready;
	s32			 origin_x;
	s32			 origin_y;
	s32			 origin_pending;
};

typedef void (*DrawFunc)(void *dest, u8 *src, s32 x, s32 y, s32 width, s32 height);

s32 BattleFx_UpdateStarField(void)
{
	// 戻り値の型は s32 だが return 文は無い（pop {r1}; bx r1）
	s32				  j;
	s32				  frame;
	u32				  size;
	s32				  ring_x;
	s32				  ring_y;
	s32				  phase;
	void			 *dest;
	s32				  i;
	struct SparkWork *work;
	DrawFunc		  draw;
	struct Spark	 *spark;
	struct Ring		 *ring;
	s32				 *pos;
	s32				  k;
	s32				  dist;
	s32				  scale;
	s32				  value;
	s32				  step;
	u32				  radius;
	s32				  angle;
	u32				  half;
	s32				  x;
	s32				  y;
	s32				  life;

	dest = gWorkSlot[HEAP_SLOT_BATTLE_CANVAS];
	work = gWorkSlot[HEAP_SLOT_BATTLE_EFFECT];
	work->ready = 0;
	draw = (DrawFunc)gWorkSlot[HEAP_SLOT_BLITTER];
	for (i = 0; i < 16; i++)
	{
		spark = &work->sparks[i];
		life = spark->life;
		if (life != 0)
		{
			dist = FixedSqrt((spark->pos[0] >> 8) * (spark->pos[0] >> 8)
							 + (spark->pos[1] >> 8) * (spark->pos[1] >> 8)
							 + (spark->pos[2] >> 8) * (spark->pos[2] >> 8));
			if (dist <= 0xfff)
			{
				spark->life = 0;
			}
			else
			{
				scale = Iwram_RatioMulQ14(dist, 0x10000);
				spark->life--;
				pos = spark->pos;
				for (k = 2; k >= 0; k--)
				{
					value = *pos;
					step = Iwram_MulQ16(Iwram_MulQ16(-value >> 8, scale), 0x13000);
					pos[3] = pos[3] - (pos[3] >> 7) + step;
					*pos = value + pos[3];
					pos++;
				}
			}
			life = spark->life;
			if (life != 0)
				goto draw_spark;
		}
		if (work->frames <= 24)
		{
			angle = Random16();
			radius = Random16() + 0x10000;
			half = radius >> 1;
			spark->pos[0] = Iwram_MulQ16(Trig_Cos(angle), half);
			spark->pos[1] = Iwram_MulQ16(Trig_Sin(angle), half);
			if (spark->pos[0] & 1)
				spark->pos[0] = -spark->pos[0];
			if (spark->pos[1] & 1)
				spark->pos[1] = -spark->pos[1];
			spark->pos[2] = (Random16() + 0x8000) >> 2;
			spark->pos[3] = (-spark->pos[0] >> 7) + (spark->pos[1] >> 8);
			spark->pos[4] = (-spark->pos[1] >> 7) + (-spark->pos[0] >> 8);
			spark->pos[5] = 0;
			life = spark->life = (radius >> 13) + 1;
		}
		if (life != 0)
		{
		draw_spark:
			x = (spark->pos[0] >> 10) + 64;
			y = (spark->pos[1] >> 10) + 64;
			frame = life;
			if (frame < 0)
				frame = 0;
			else if (frame > 6)
				frame = 6;
			size = BattleFx_StarSizes[frame];
			draw(dest, (u8 *)work + BattleFx_StarWorkOffsets[frame], x - (size >> 1),
				y - (size >> 1), size, size);
		}
	}
	draw = (DrawFunc)gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE];
	for (ring = work->rings, j = 2; j >= 0; j--, ring++)
	{
		ring->x += ring->vel_x;
		ring->y += ring->vel_y;
		ring_x = ring->x >> 10;
		ring_y = ring->y >> 10;
		phase = 3 - ring->age / 8;
		if (phase >= 0)
		{
			ring->age++;
			draw(dest, (u8 *)work + BattleFx_RingFrameOffsets[phase], ring_x + 48, ring_y + 48, 32,
				32);
		}
	}
	work->frames++;
	work->ready = 1;
}

u32 Graphics_UploadVramBlock(void)
{
	void			 *source = gWorkSlot[HEAP_SLOT_BATTLE_CANVAS];
	struct SparkWork *work = gWorkSlot[HEAP_SLOT_BATTLE_EFFECT];

	if (source != 0)
	{
		if (work->ready != 0)
		{
			work->ready = 0;
			ColorBuffer_BackupAndScaleThreeQuarters(source, (void *)0x06004000, 0x4000);
		}
	}
}

s32 BattleFx_InitializeStarField(s32 mode)
{
	struct SparkWork *work;
	struct Spark	 *spark;
	struct Ring		 *ring;
	s32				  i;
	u8				 *data;
	s32				  zero;
	/* FAKEMATCH: needed to match: an unused Spark, the 28 bytes the native frame reserves. */
	struct Spark unused;

	((s32 *)gTransitionWork)[2] = 1;
	work = Runtime_AllocateBlock(HEAP_SLOT_BATTLE_EFFECT, sizeof *work);
	Runtime_AllocateHeapBlock(HEAP_SLOT_BATTLE_CANVAS, 0x4000);
	/* FAKEMATCH: needed to match: the empty loop ends a scheduling region, keeping the multiply routine's address load ahead of the spark cursor. */
	do
	{
	} while (0);
	for (i = 0; i < 16; i++)
	{
		s32 angle = Random16();
		u32 radius = Random16() + 0x10000;
		u32 half = radius >> 1;

		spark = &work->sparks[i];
		spark->pos[0] = Iwram_MulQ16(Trig_Cos(angle), half);
		spark->pos[1] = Iwram_MulQ16(Trig_Sin(angle), half);
		if (spark->pos[0] & 1)
			spark->pos[0] = -spark->pos[0];
		if (spark->pos[1] & 1)
			spark->pos[1] = -spark->pos[1];
		spark->pos[2] = (Random16() + 0x8000) >> 2;
		spark->pos[3] = (-spark->pos[0] >> 7) + (spark->pos[1] >> 8);
		spark->pos[4] = (-spark->pos[1] >> 7) + (-spark->pos[0] >> 8);
		spark->pos[5] = 0;
		spark->life = (radius >> 13) + 1;
	}
	for (i = 0; i < 3; i++)
	{
		s32 radius = 0x1000;
		s32 angle;

		ring = &work->rings[i];
		angle = i * 0x5555;
		ring->x = Iwram_MulQ16(Trig_Cos(angle), radius);
		ring->y = Iwram_MulQ16(Trig_Sin(angle), radius);
		ring->vel_x = Iwram_MulQ16(Trig_Cos(angle), 0x200);
		ring->vel_y = Iwram_MulQ16(Trig_Sin(angle), 0x200);
		ring->age = 0;
	}
	work->frames = 0;
	work->ready = 0;
	work->origin_pending = 0;
	Iwram_ClearWords(gWorkSlot[HEAP_SLOT_BATTLE_CANVAS], 0x4000);
	data = Resource_GetTableEntry((s32)&ResourceId_BlueRingSheet);
	Iwram_CopyWords(BG_PLTT, data, 128);
	data += 128;
	Resource_DecodeType01(data, work);
	switch (mode)
	{
	case 0:
		data = Resource_GetTableEntry((s32)&ResourceId_YellowPaletteB);
		break;
	case 1:
		data = Resource_GetTableEntry((s32)&ResourceId_BlueRingSheet);
		break;
	case 2:
		data = Resource_GetTableEntry((s32)&ResourceId_RedPaletteC);
		break;
	default:
		data = Resource_GetTableEntry((s32)&ResourceId_VioletPaletteD);
		break;
	}
	Dma_Set(data, BG_PLTT, DMA_ENABLE32 | DMA_32BIT | 32, REG_DMA3);
	/* FAKEMATCH: needed to match: the one-pass block keeps the BG2 origin writes in source order, the zero loaded first. */
	do
	{
		zero = 0;
		REG_BG2X = zero;
		REG_BG2Y = zero;
	} while (0);
	Io_Write16(0x100, &REG_BG2PA);
	Io_Write16(0, &REG_BG2PB);
	Io_Write16(0, &REG_BG2PC);
	Io_Write16(0x100, &REG_BG2PD);
	BattleEffect_LoadWorkFar(HEAP_SLOT_BLITTER, 7, 7, 3, 3);
	BattleEffect_LoadWorkFar(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, 3, 2);
	Scheduler_AddOrUpdateCallback((SchedulerCallback)BattleFx_UpdateStarField, 0xc80);
	return Scheduler_AddOrUpdateCallback((SchedulerCallback)Graphics_UploadVramBlock, 0xc80);
}

s32 Graphics_ResetVramBlockAndReleaseHeapBlocks(s32 unused)
{
	Iwram_ClearWords((void *)0x06004000, 0x4000);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BATTLE_CANVAS);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BATTLE_EFFECT);
	REG_DISPCNT = 0x1341;
	Scheduler_RemoveCallback((SchedulerCallback)Graphics_UploadVramBlock);
	return Scheduler_RemoveCallback((SchedulerCallback)BattleFx_UpdateStarField);
}

s32 Graphics_ScaleRgb555Clamped(u16 *source, u16 *destination, s32 scale, s32 count)
{
	s32 remaining;
	u32 mask_red;
	u32 mask_green;
	u32 mask_blue;
	u32 pixel;
	u32 red;
	u32 green;
	u32 blue;

	if (scale > 0x10000)
		scale = 0x10000;
	if (count > 0)
	{
		mask_red = 0x1f;
		mask_green = 0x3e0;
		mask_blue = 0x7c00;
		remaining = count;
		do
		{
			pixel = *source;
			red = pixel & mask_red;
			green = pixel & mask_green;
			blue = mask_blue & pixel;
			red *= scale;
			green *= scale;
			blue *= scale;
			pixel = ((red >> 16) & mask_red) | ((green >> 16) & mask_green);
			pixel |= (blue >> 16) & mask_blue;
			*destination = pixel;
			source++;
			destination++;
			remaining--;
		} while (remaining != 0);
	}
	return 0;
}

void BattleFx_PlayUnitElementEffect(s32 unit, s32 kind, s32 mode, s32 variant)
{
	/* FAKEMATCH: needed to match: an unread ten-word launch tail, the native frame's 40 extra bytes. */
	s32							  spare[10];
	struct EffectPosition		  position;
	struct EffectPosition		  position2;
	struct BattlePresentationWork launch;
	struct BattlePresentationWork launch2;
	/* FAKEMATCH: needed to match: one variable, assigned twice, holds slot 12's address and later the IME address, as the bytes do; the session is slot 9, three words below. */
	volatile u16		  *ime = (volatile u16 *)&gWorkSlot[12];
	struct BtlWork		  *session = *(struct BtlWork **)((u8 *)ime - 12);
	struct SparkWork	  *work;
	s32					   frame;
	s32					  *brightness;
	struct EffectPosition *pos;

	WaitFrames(1);
	BattlePresentation_ConfigurePaletteFade(1, session->background, 0);
	Iwram_ClearWords((void *)0x06004000, 0x4000);
	QueueIoWriteDelay2(0x04000000, 0x3741);
	QueueIoWriteDelay2(0x0400000c, 0x784);
	QueueIoWriteDelay2(0x04000050, 0x3f44);
	WaitFrames(1);
	Io_Write16(240, &REG_WIN0H);
	Io_Write16(0x1088, &REG_WIN0V);
	Io_Write16(63, &REG_WININ);
	Io_Write16(17, &REG_WINOUT);
	if (mode == 0)
	{
		QueueIoWriteDelay2(0x04000052, 0x100e);
		BattleFx_InitializeStarField(kind);
		for (frame = 0, pos = &position; frame <= 44; frame++)
		{
			brightness = &session->brightness;
			ime = &REG_IME;
			work = gWorkSlot[HEAP_SLOT_BATTLE_EFFECT];
			if (frame <= 24)
				Graphics_ScaleRgb555Clamped(
					session->palette, (u16 *)0x050000c0, *brightness = 0x10000 - frame * 1092, 128);
			BattleMotion_ProjectScaledPosition(unit, (s32 *)pos);
			work->origin_x = (64 - pos->x) << 8;
			work->origin_y = (64 - pos->y) << 8;
			{
				/* FAKEMATCH: needed to match: IO_WRITE_QUEUE_PUSH written out with the IME address in the function's own pointer, set at the top of the loop; the one-pass blocks keep the saved copy ahead of the IME store and the restore ahead of the next statement. With the macro the IME address and pos trade r6 and r7 (differ 20). */
				struct IoWriteQueue *q;
				u32					 saved;
				s32					 count;

				q = &gIoWriteQueue;
				do
				{
					saved = *ime;
				} while (0);
				*ime = (u16)(u32)ime;
				count = q->count;
				if (count <= 31)
				{
					u32 *entry = (u32 *)((u8 *)q + count * 12 + 4);

					*(u16 *)&q->count = count + 1;
					*entry++ = (u32)&work->origin_x;
					*entry++ = 0x04000028;
					*entry = 0x84000002;
				}
				do
				{
					*ime = saved;
				} while (0);
			}
			work->origin_pending = 1;
			WaitFrames(1);
		}
		Graphics_ResetVramBlockAndReleaseHeapBlocks(kind);
	}
	else if (mode == 1)
	{
		// ここのワークは effect.c の struct BattleEffectWork だが、星のワークと同じオフセットに書く
		BattlePresentation_PrepareSceneFar(kind);
		for (frame = 0; frame < 40; frame++)
		{
			work = gWorkSlot[HEAP_SLOT_BATTLE_EFFECT];
			BattleMotion_ProjectScaledPosition(unit, (s32 *)&position2);
			work->origin_x = (64 - position2.x) << 8;
			work->origin_y = (64 - position2.y) << 8;
			IO_WRITE_QUEUE_PUSH(0x04000028, (u32)&work->origin_x, 0x84000002);
			work->origin_pending = 1;
			WaitFrames(1);
		}
		BattleFx_ScheduleCallbacksAndReleaseBlocksFar();
	}
	else if (mode == 2)
	{
		launch.flags = 0;
		launch.kind = kind;
		launch.variant = variant;
		launch.actor = unit;
		launch.actors[0] = unit;
		launch.first_target = unit;
		launch.count = 1;
		launch.initial_value = 1;
		BattleFx_RunIceFleckBurstFar((struct BattleEffectArgument *)&launch);
	}
	else
	{
		launch2.kind = kind;
		launch2.flags = 0;
		launch2.variant = 0;
		launch2.actor = unit;
		launch2.actors[0] = unit;
		launch2.first_target = unit;
		launch2.count = 1;
		launch2.initial_value = 1;
		BattleFx_RunGatheringBurstFar(&launch2);
	}
}

void BattlePres_RunWithZeroArguments(void)
{
	BattlePresentation_SetPaletteLevel(0, 0);
}

void Runtime_RemoveIrqHandlerSlot2(void)
{
	Runtime_SetIrqHandler(2, 0, 0);
}
