#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "mem.h"
#include "btlpty.h"
#include "types.h"
#include "callback_scheduler.h"
#include "scene.h"
#include "global_cells.h"
#include "iwram_call.h"
#include "dma.h"
#include "resource.h"
#include "resource_ids.h"
#include "io_reg.h"
#include "btldisp.h"
#include "fixed_math.h"
#include "btlwork.h"
#include "heap_state.h"
#include "battle_efx.h"
#include "io_write_queue.h"
#include "effect_step.h"
#include "battle_formation.h"
#include "character.h"
#include "edition.h"
#include "tbs_edition.h"
#include "btlsmn.h"
#include "battle_unit.h"
#include "btlsys.h"
#include "b5_context.h"
#include "btlcalc.h"
#include "game_state.h"
#include "item.h"
#include "inventory.h"
#include "btltype.h"
#include "btlmes.h"

void QueueIoWriteDelay2(u32, u32);

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

#define ColorBuffer_BackupAndScaleThreeQuarters \
	((u32 (*)(void *, void *, u32))ColorBuffer_BackupAndScaleThreeQuarters)

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
	volatile u16   *blend_y;

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
		u32 zero = 0;
		u32 sixteen;

		battle->highlight_mode = mode;
		battle->highlight_step = zero;
		blend_y = &REG_BLDY;
		*blend_y = zero;
		sixteen = 16;

		/* FAKEMATCH: needed to match: removing this one-pass block changes instruction scheduling */
		do
		{
			blend_y[-1] = sixteen;
		} while (0);

		count = BattleParty_ListActorIds(3, (u16 *)active_actors);
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

s32 BattleFx_UpdateStarField(void)
{
	// 戻り値の型は s32 だが return 文は無い（pop {r1}; bx r1）
	s32				  j;
	s32				  frame;
	u32				  size;
	s32				  ring_x;
	s32				  ring_y;
	s32				  phase;
	s32				  age;
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
	void *source = gWorkSlot[HEAP_SLOT_BATTLE_CANVAS];
	u8	 *context = gWorkSlot[HEAP_SLOT_BATTLE_EFFECT];

	if (source != 0)
	{
		u32 *active = (u32 *)(context + 0x13C0);

		if (*active != 0)
		{
			*active = 0;
			return ColorBuffer_BackupAndScaleThreeQuarters(source, (void *)0x06004000, 0x4000);
		}
	}
	return (u32)source;
}

s32 BattleEffect_LoadWorkFar(s32 slot, s32 width_shift, s32 height_shift, s32 flags, u32 mode);

#define BattleEffect_LoadWorkFar ((s32 (*)(s32, s32, s32, s32, s32))BattleEffect_LoadWorkFar)
#define BattleFx_RunIceFleckBurstFar ((void (*)(struct EffectLaunch *))BattleFx_RunIceFleckBurstFar)

#define gTransitionWork (*(s32 **)&gTransitionWork)

s32 BattleFx_InitializeStarField(s32 mode)
{
	struct SparkWork *work;
	struct Spark	 *spark;
	struct Ring		 *ring;
	s32				  i;
	u8				 *data;
	s32				  scale;
	s32				  zero;
	/* FAKEMATCH: needed to match: an unused Spark, the 28 bytes the native frame reserves. */
	struct Spark unused;

	gTransitionWork[2] = 1;
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
	/* FAKEMATCH: needed to match: the two one-pass blocks keep the BG2 origin and matrix writes in source order, the zero loaded first. */
	do
	{
		zero = 0;
		REG_BG2X = zero;
		REG_BG2Y = zero;
	} while (0);
	do
	{
		scale = 0x100;
		REG_BG2PA = scale;
		REG_BG2PB = zero;
		REG_BG2PC = zero;
		REG_BG2PD = scale;
	} while (0);
	BattleEffect_LoadWorkFar(HEAP_SLOT_BLITTER, 7, 7, 3, 3);
	BattleEffect_LoadWorkFar(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, 3, 2);
	Scheduler_AddOrUpdateCallback((SchedulerCallback)BattleFx_UpdateStarField, 0xc80);
	return Scheduler_AddOrUpdateCallback((SchedulerCallback)Graphics_UploadVramBlock, 0xc80);
}

s32 Graphics_ResetVramBlockAndReleaseHeapBlocks(s32 unused)
{
	Iwram_ClearWords((void *)0x06004000, 0x4000);
	Runtime_ReleaseHeapBlock(47);
	Runtime_ReleaseHeapBlock(46);
	Runtime_ReleaseHeapBlock(40);
	Runtime_ReleaseHeapBlock(39);
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

struct Bg2OriginWork
{
	u8	unknown_0000[0x13c4];
	s32 origin_x;
	s32 origin_y;
	s32 origin_pending;
};

struct EffectLaunch
{
	s32 kind;
	s32 side;
	s32 actor;
	s32 target;
	s32 unknown_0010;
	s32 count;
	s32 variant;
	s32 prelude;
	s32 unknown_0020;
	s16 actors[24];
};

/* FAKEMATCH: needed to match: the write goes through its own int, which loads each value as a word, and the do/while (0) ends a scheduling region after it, as in the slideshow's register writes. */
#define Io_Write16(reg, value)                                                                                            \
	do                                                                                                                    \
	{                                                                                                                     \
		/* FAKEMATCH: needed to match: the one-pass register-write boundary preserves measured instruction scheduling. */ \
		/* FAKEMATCH: needed to match: the register-write word temporary preserves measured value allocation. */          \
		s32 value_ = (value);                                                                                             \
		(reg) = value_;                                                                                                   \
	} while (0)

void BattleFx_PlayUnitElementEffect(s32 unit, s32 kind, s32 mode, s32 variant)
{
	/* FAKEMATCH: needed to match: an unread ten-word launch tail, the native frame's 40 extra bytes. */
	s32					  spare[10];
	struct EffectPosition position;
	struct EffectPosition position2;
	struct EffectLaunch	  launch;
	struct EffectLaunch	  launch2;
	/* FAKEMATCH: needed to match: one variable, assigned twice, holds slot 12's address and later the IME address, as the bytes do; the session is slot 9, three words below. */
	volatile u16		  *ime = (volatile u16 *)&gWorkSlot[12];
	struct BtlWork		  *session = *(struct BtlWork **)((u8 *)ime - 12);
	struct Bg2OriginWork  *work;
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
	Io_Write16(REG_WIN0H, 240);
	Io_Write16(REG_WIN0V, 0x1088);
	Io_Write16(REG_WININ, 63);
	Io_Write16(REG_WINOUT, 17);
	if (mode == 0)
	{
		QueueIoWriteDelay2(0x04000052, 0x100e);
		BattleFx_InitializeStarField(kind);
		for (frame = 0, pos = &position; frame <= 44; frame++)
		{
			brightness = &session->brightness;
			ime = &REG_IME;
			work = gWorkSlot[39];
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
		BattlePresentation_PrepareSceneFar(kind);
		for (frame = 0; frame < 40; frame++)
		{
			work = gWorkSlot[39];
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
		launch.prelude = 0;
		launch.kind = kind;
		launch.variant = variant;
		launch.actor = unit;
		launch.actors[0] = unit;
		launch.target = unit;
		launch.count = 1;
		launch.unknown_0010 = 1;
		BattleFx_RunIceFleckBurstFar(&launch);
	}
	else
	{
		launch2.kind = kind;
		launch2.prelude = 0;
		launch2.variant = 0;
		launch2.actor = unit;
		launch2.actors[0] = unit;
		launch2.target = unit;
		launch2.count = 1;
		launch2.unknown_0010 = 1;
		BattleFx_RunGatheringBurstFar(&launch2);
	}
}

s32	 GameFlag_TestFar(s32 flag);
void BattlePresentation_SetPaletteLevel(s32 unused, s32 level);

void BattlePres_RunWithZeroArguments(void)
{
	BattlePresentation_SetPaletteLevel(0, 0);
}

typedef void (*InterruptHandler)(void);
void Runtime_SetIrqHandler(u32, s32, InterruptHandler);

void Runtime_RemoveIrqHandlerSlot2(void)
{
	Runtime_SetIrqHandler(2, 0, 0);
}

s32 Party_ComputeEligibleMemberAverage(s32 record_id)
{
	struct BattleFormationRecord *record;
	struct EnemyDefinition		 *enemy;
	u32							  i;
	s32							  id;
	s32							  sum;
	s32							  count;
	/* FAKEMATCH: needed to match: an unread seven-word level table, the native frame's 28 bytes */
	s32 levels[7];

	count = 0;
	sum = 0;
	record = &BattleFormation_Records[record_id];
	for (i = 0; i < 5; i++)
	{
		if (record->minimum_counts[i] != 0)
			break;
	}
	if (i == 5)
		return -1;
	for (i = 0; i < 5; i++)
	{
		if (record->maximum_counts[i] == 0)
			continue;
		id = record->member_ids[i];
		enemy = Owner_GetRecordFar(id + 8);
		if (enemy == 0)
			continue;
		if (enemy->level > 3 && !GameFlag_TestFar(372) && !GameFlag_TestFar(id + 1544))
			return -2;
		sum += enemy->level;
		count++;
	}
	if (count == 0)
		return -3;
	return sum / count;
}

struct FormationCandidate
{
	s16 record_id;
	s16 score;
};

struct BattleBaseStats
{
	s16						  base_hp;
	s16						  base_ep;
	s16						  hp_gauge;
	s16						  ep_gauge;
	u16						  base_attack;
	u16						  base_defense;
	u16						  base_agility;
	u8						  base_luck;
	u8						  action_count;
	u8						  hp_regen;
	u8						  ep_regen;
	u8						  unknown_12[2];
	struct BattleElementStats elm[4];
};

s32 BattleFormation_SelectLevelMatchedCandidate(s32 *out_margin)
{
	s32						   match_count = 0;
	struct FormationCandidate *pool =
		(struct FormationCandidate *)Runtime_BumpAllocateAlternatePool(128);
	u16 party[8];
	s32 level_total = 0;
	s32 unit_count = BattleParty_PrepareActiveOwners(party);
	s32 i;
	s32 j;
	s32 chance;
	s32 result;

	if (unit_count > 0)
	{
		u16 *p = party;

		i = unit_count;
		do
		{
			u8 level = UnitGetFar(*p)->level;

			i--;
			p++;
			level_total += level;
		} while (i != 0);
	}

	chance = level_total / unit_count;
	chance += (s8)GameFlag_GetByteFar(1016);
	if (chance <= 0)
		chance = 1;
	if (chance > 99)
		chance = 99;

	for (i = 0; i <= 31; i++)
		pool[i].score = -1;

	for (i = 0; (u32)i <= 19; i++)
	{
		Owner_GetRecordFar(BattleFormation_LevelMatchedEnemies[i]);
		GameFlag_ClearBitFar(BattleFormation_LevelMatchedEnemies[i] + 1536);
	}

	for (i = 0; (u32)i <= 379; i++)
	{
		s32 score = Party_ComputeEligibleMemberAverage(i);

		if (score >= 0 && score <= chance + 3)
		{
			s32 min_index = -1;
			s32 min_value;

			for (j = 0, min_value = 999; j <= 31; j++)
			{
				if (pool[j].score < min_value)
				{
					min_value = pool[j].score;
					min_index = j;
				}
			}

			if (min_index >= 0)
			{
				pool[min_index].score = score;
				pool[min_index].record_id = i;
				match_count++;
			}
		}
	}

	if (match_count > 32)
		match_count = 32;

	if (match_count != 0)
	{
		struct FormationCandidate *chosen = &pool[(match_count * Random16()) >> 16];

		result = chosen->record_id;
		*out_margin = chance - chosen->score;
	}
	else
	{
		*out_margin = match_count;
		result = 1;
	}

	MemFree(pool);
	return result;
}

s32 Owner_ApplyLevelGains(s32 owner, s32 levels)
{
	struct BattleBaseStats *base;
	struct BtlUnit		   *state;
	s32						value;
	s32						floor;
	s32						i;

	base =
		(struct BattleBaseStats *)Runtime_BumpAllocateAlternatePool(sizeof(struct BattleBaseStats));
	state = UnitGetFar(owner);
	Iwram_CopyWords(base, &state->base_hp, sizeof(struct BattleBaseStats));

	value = state->base_hp;
	value += levels * 97 / 10;
	floor = base->base_hp * 7 / 10;
	if (value < floor)
		value = floor;
	if (value > 9999)
		value = 9999;
	state->base_hp = value;

	value = state->base_ep;
	value += levels * 15 / 10;
	floor = base->base_ep * 7 / 10;
	if (value < floor)
		value = floor;
	if (value > 9999)
		value = 9999;
	state->base_ep = value;

	floor = levels * 123 / 10;
	value = state->base_attack;
	value += floor;
	floor = base->base_attack * 7 / 10;
	if (value < floor)
		value = floor;
	if (value > 999)
		value = 999;
	state->base_attack = value;

	floor = levels * 33 / 10;
	value = state->base_defense;
	value += floor;
	floor = base->base_defense * 7 / 10;
	if (value < floor)
		value = floor;
	if (value > 999)
		value = 999;
	state->base_defense = value;

	floor = levels * 51 / 10;
	value = state->base_agility;
	value += floor;
	floor = base->base_agility * 7 / 10;
	if (value < floor)
		value = floor;
	if (value > 999)
		value = 999;
	state->base_agility = value;

	for (i = 0; i < 4; i++)
	{
		value = state->base_elm[i].pow + levels * 15;
		floor = base->elm[i].pow * 7 / 10;
		if (value < floor)
			value = floor;
		if (value > 200)
			value = 200;
		state->base_elm[i].pow = value;
	}

	state->level += levels;
	UnitCalcFar(owner);
	MemFree(base);
}

#if EDITION_INTERNATIONAL
#define CH_CNT 9
#else
#define CH_CNT 26
#endif

s32 EnmCntDec(s32 no, s32 n)
{
	struct BtlWork *w;
	s32				num;
	s32				i;
	s32				retry;
	s32				ch;

	w = BtlWk;
	num = w->summon_count;
	for (i = 0; i < num; i++)
	{
		if (w->enemy_classes[i] == no)
			break;
	}
	if (i != num)
	{
		retry = 0;
		if (w->summon_channels[i] < 0)
		{
			w->summon_channels[i] = 1;
			w->summon_masks[i] = 3;
			return 0x8001;
		}
		for (; retry <= 31; retry++)
		{
			ch = (w->summon_channels[i] + 1) % CH_CNT;
			w->summon_channels[i] = ch;
			if ((w->summon_masks[i] & (1 << (s8)ch)) == 0)
				break;
		}
		w->summon_masks[i] |= 1 << w->summon_channels[i];
		return w->summon_channels[i];
	}
	if (num <= 4)
	{
		w->summon_channels[num] = -1;
		w->enemy_classes[num] = no;
		w->summon_masks[num] = 0;
		w->summon_count = num + 1;
		return CH_CNT;
	}
	return -1;
}

s32 EnmCntRel(s32 actor_id)
{
	struct BtlWork *state = BtlWk;
	struct BtlUnit *actor;
	s32				count = state->summon_count;
	s32				index;
	s32				name_length;
	s32				bit;
	s32				class_id;

	actor = UnitGetFar(actor_id);
	if (actor->class_index != 0)
		return;

	class_id = actor->class_id;
	for (index = 0; index < count; index++)
	{
		if (state->enemy_classes[index] == class_id)
			break;
	}
	if (index == count || state->summon_masks[index] == 0)
		return;

	for (name_length = 0; name_length <= 13; name_length++)
	{
		if (actor->name[name_length] == 0)
			break;
	}

	bit = 32;
	if (name_length > 0)
		bit = actor->name[name_length - 1] - LIST_MARKER_CHAR;
	state->summon_masks[index] &= ~(1 << bit);
}

s32 EnmCntClr(s32 class_id)
{
	struct BtlUnit *summon;
	s32				object_index;
	s32				slot;
	s32				next_slot;
	u8				occupied;
	u8				marker;

	object_index = 0;
	marker = LIST_MARKER_CHAR;
scan_objects:
	summon = UnitGetFar(object_index + 0x80);
	occupied = summon->side;
	if (occupied != 1)
		goto next_object;
	if (summon->class_id != class_id)
		goto next_object;
	slot = 0;
	if (summon->name[0] != 0)
		goto scan_slots;
	summon->name[0] = marker;
	summon->name[occupied] = slot;
	return;
scan_slots:
	slot++;
	if (slot > 13)
		return;
	occupied = summon->name[slot];
	if (occupied != 0)
		goto scan_slots;
	next_slot = slot + 1;
	summon->name[slot] = marker;
	summon->name[next_slot] = occupied;
	return;
next_object:
	object_index++;
	if (object_index <= 5)
		goto scan_objects;
}

s32 BtlGrpRnd(s32 record_id)
{
	s32							  choices[5];
	struct BattleFormationRecord *record;
	s32							 *choices_base;
	s32							 *choice_cursor;
	u8							 *maximum_count;
	u8							 *member_id;
	u32							  count;
	s32							  remaining;

	count = 0;
	if (record_id < 0 || record_id >= 380)
		record_id = 1;

	choices_base = choices;
	record = &BattleFormation_Records[record_id];
	member_id = record->member_ids;
	maximum_count = record->maximum_counts;
	choice_cursor = choices_base;
	remaining = 4;
	do
	{
		if (*maximum_count++ != 0)
		{
			*choice_cursor++ = *member_id + 8;
			count++;
		}
		member_id++;
		remaining--;
	} while (remaining >= 0);

	return choices[(Random16() * count) >> 16];
}

s32 BattleFormation_BuildEnemyList(s32 record_id)
{
	u16							 *list;
	struct BtlWork				 *work;
	struct BattleFormationRecord *record;
	s32							  count;
	s32							  margin;
	u16							  list_buffer[14];
	s32							  extra[5];
	s32							  counts[5];
	s32							  order[5];
	s32							  budget;
	s32							  changed;
	s32							  room;
	s32							  size;
	s32							  fit;
	s32							  id;
	s32							  k;
	s32							  n;
	s32							  total;
	u32							  i;
	s32							  j;

	work = BtlWk;
	margin = 0;
	work->summon_count = 0;
	list = list_buffer;
	if (GameFlag_TestFar(0x173))
		record_id = BattleFormation_SelectLevelMatchedCandidate(&margin);
	if (record_id < 0 || record_id >= 380)
		record_id = 1;
	record = &BattleFormation_Records[record_id];
	for (j = 0; j <= 4 && record->minimum_counts[j] == 0; j++)
		;
	if (j == 5)
		record = &BattleFormation_Records[1];

	budget = 6;
	count = 0;
	for (i = 0; i <= 4; i++)
	{
		if (record->minimum_counts[i] != 0)
		{
			s32 size1 = 2 - (Summon_IsEntryFlagged(record->member_ids[i] + 8) != 0);

			budget -= size1 * record->minimum_counts[i];
		}
	}

	for (i = 0; i <= 4; i++)
	{
		s32 minimum;
		s32 maximum;

		counts[i] = minimum = record->minimum_counts[i];
		maximum = record->maximum_counts[i];
		room = maximum - minimum;
		if (room > 0)
		{
			s32 size2 = 2 - (Summon_IsEntryFlagged(record->member_ids[i] + 8) != 0);

			fit = budget / size2;
			if (fit < room)
				room = fit;
			extra[i] = ((room + 1) * Random16()) >> 16;
		}
		else
		{
			extra[i] = 0;
		}
	}

	do
	{
		changed = 0;
		for (i = 0; i <= 4; i++)
		{
			id = record->member_ids[i] + 8;
			if (extra[i] != 0)
			{
				size = 2 - (Summon_IsEntryFlagged(id) != 0);
				if (size > budget)
				{
					extra[i] = 0;
				}
				else
				{
					counts[i]++;
					extra[i]--;
					budget -= size;
					changed = 1;
				}
			}
		}
	} while (changed);

	work->battle_type = record->battle_type;
	switch (work->battle_type)
	{
	case 0:
		for (i = 0; i <= 4; i++)
			order[i] = i;
		for (i = 0; i <= 9; i++)
		{
			s32 first = (Random16() * 5) >> 16;
			s32 second = (Random16() * 5) >> 16;
			s32 swap = order[first];

			order[first] = order[second];
			order[second] = swap;
		}
		for (i = 0; i <= 4; i++)
		{
			s32 m;
			s32 slot = order[i];

			for (m = 0; m < counts[slot]; m++)
				list[count++] = record->member_ids[slot] + 8;
		}
		break;
	case 1:
		for (;;)
		{
			n = 0;
			for (i = 0; i <= 4; i++)
			{
				if (counts[i] != 0)
					order[n++] = i;
			}
			if (n == 0)
				break;
			k = order[(Random16() * n) >> 16];
			list[count++] = record->member_ids[k] + 8;
			counts[k]--;
		}
		break;
	default:
		for (i = 0; i <= 4; i++)
		{
			total = counts[i];
			for (k = 0; k < total; k++)
				list[count++] = record->member_ids[i] + 8;
		}
		break;
	}
	list[count] = 0;
	work->first_defeated = 6;
	work->defeat_state = 0;
	for (i = 128; i <= 133; i++)
		Iwram_ClearWords(UnitGetFar(i), BATTLE_UNIT_SIZE);

	i = 0;
	if (list[0] == 0)
		goto assigned;
assign_enemy:
	{
		s32 charge = EnmCntDec(list[i], 1);

		if (charge & 0x8000)
			EnmCntClr(list[i]);
		BtlUnitSetFar(i + 128, list[i], charge & 0x7fff);
		UnitGetFar(i + 128);
		if (margin != 0)
			Owner_ApplyLevelGains(i + 128, margin);
	}
	i++;
	if (i > 5)
		goto assigned;
	if (list[i] != 0)
		goto assign_enemy;
assigned:
	return i;
}

struct ItemDefinition *Item_GetDirectFar(s32 item);

u32 Summon_GetLayoutKind(s32 index)
{
	u32 kind = Summon_EntryTable[index].layout_kind;

	if ((s32)kind > 4)
		kind = -1U;
	return kind;
}

s32 Summon_GetEntryValue(s32 index)
{
	if (index < 0 || index > 171)
		return Summon_EntryTable[0].animation;
	return Summon_EntryTable[index].animation;
}

s32 Summon_GetEntryFlag1Field(s32 index)
{
	if (index < 0 || index > 171)
		return Summon_EntryTable[0].animation;
	return Summon_EntryTable[index].layout_bits1;
}

s32 Summon_IsEntryFlagged(s32 index)
{
	s32 result;

	if (index < 0 || index > 171)
		return 0;
	result = 0;
	if (Summon_EntryTable[index].sprite_bit0)
		result = 1;
	return result;
}

u32 Battle_GetSpriteFlagNibble(u32 index)
{
	u16 value;

	if (index > 171)
		return 1;
	value = Summon_EntryTable[index].sprite_bits1;
	return value ? value : 1;
}

u32 Battle_GetSpriteFlagHighBits(u32 index)
{
	u16 value;

	if (index > 171)
		return 0;
	value = Summon_EntryTable[index].sprite_bits5;
	return value ? value : 0;
}

s32 Summon_IsEntrySecondaryFlagged(s32 index)
{
	if (index < 0 || index > 171)
		return 0;
	return Summon_EntryTable[index].layout_bit0;
}

s32 Summon_GetEntryByte4(s32 index)
{
	if (index < 0 || index > 171)
		return 0;
	return Summon_EntryTable[index].unknown_04[0];
}

u32 Item_EncodeBankedId(u32 value)
{
	u32 bank = 0;
	u32 base = value & 0x1ff;
	if (base == 0)
		return 0;
	{
		u8 flags = Item_GetDirectFar(base)->flags;

		if (flags & 8)
			bank = 1;
		bank <<= 1;
		if (flags & 4)
			bank++;
		bank <<= 9;
		bank += base;
	}
	return bank;
}

void Summon_ClearWorkFields(void)
{
	struct BtlWork		*work = BtlWk;
	struct BattleSpoils *spoils = &work->spoils;
	u16					*items;
	s32					 index;

	gGameState.pending_item = 0;
	spoils->coins = 0;
	spoils->experience = 0;
	spoils->defeated = 0;
	items = work->spoils.items;
	for (index = 3; index >= 0; index--)
		items[index] = 0;
}

#define Item_EncodeBankedId ((s32 (*)(s32))Item_EncodeBankedId)

u32 Random16(void);

static __inline__ s32 BattleEnemy_CalculateCoinReward(struct BtlUnit *unit, u16 *reward)
{
	s32 i;
	s32 bonus = 0;
	s32 base;
	s32 floor;

	for (i = 0; i < (u8)(unit->level / 10U) + 1; i++)
		bonus += ((Random16() * 6) >> 16) + 1;
	base = *reward;
	floor = base * 3 / 10;
	if (bonus < floor)
		bonus = floor;
	return bonus + base;
}

static __inline__ s32 BattleEnemy_CalculateExperienceReward(struct BtlUnit *unit, u16 *reward)
{
	s32 i;
	s32 bonus = 0;
	s32 base;
	s32 floor;

	for (i = 0; i < (u8)(unit->level / 10U) + 1; i++)
		bonus += ((Random16() * 4) >> 16) + 1;
	base = *reward;
	floor = base * 3 / 10;
	if (bonus < floor)
		bonus = floor;
	return bonus + base;
}

void UiWork_PushValueSlotFar(s32 value, s32 slot);
void AudioCommand_PlayFar(s32 cue);

s32 BattleEnemy_RecordDefeat(s32 unit_id, s32 earned)
{
	struct BtlUnit		   *unit;
	struct EnemyDefinition *rec;
	struct BtlWork		   *formation;
	struct BattleSpoils	   *spoils;
	s32						i;
	s32						chance;
	s32						slot;
	s32						lowest;
	s32						lowest_slot;
	s32						value;

	unit = UnitGetFar(unit_id);
	formation = BtlWk;
	spoils = &formation->spoils;
	if (unit_id >= 0 && unit_id < 8)
		return -1;
	if (unit->class_index != 0)
		return -2;
	slot = 0;
	i = 0;
	if (formation->enemy_classes[0] != unit->class_id)
	{
		do
		{
			if (++i > 5)
				break;
		} while (formation->enemy_classes[i] != unit->class_id);
	}
	if (i != 6)
		slot = i;
	if (formation->defeat_state != 2)
	{
		if (slot < formation->first_defeated)
			formation->first_defeated = slot;
		if (spoils->defeated)
			formation->defeat_state = 1;
	}
	spoils->defeated++;
	if (GameFlag_TestFar(0x173))
		return 0;
	GameFlag_SetBitFar(unit->class_id + 0x600);
	rec = Owner_GetRecordFar(unit->class_id);
	if (earned != 0)
	{
		if (rec->coins != 0)
			spoils->coins += BattleEnemy_CalculateCoinReward(unit, &rec->coins);
		if (rec->experience != 0)
			spoils->experience += BattleEnemy_CalculateExperienceReward(unit, &rec->experience);
	}
	else
	{
		spoils->coins += rec->coins;
		spoils->experience += rec->experience;
	}
	if (rec->item == 0 || rec->item_chance == 0)
		return 0;
	i = 0;
	if (spoils->items[0] != rec->item)
	{
		do
		{
			if (++i > 3)
				break;
		} while (spoils->items[i] != rec->item);
	}
	if (i != 4)
		return 0;
	chance = rec->item_chance;
	if (earned != 0)
		chance -= 2;
	if (chance < 0)
		chance = 0;
	if ((0x20000 >> chance) > (BtlRndFar() & 0xffff))
	{
		lowest_slot = -1;
		lowest = 0x40000000;
		for (i = 0; i <= 3; i++)
		{
			value = Item_EncodeBankedId(spoils->items[i]);
			if (value < lowest)
			{
				lowest = value;
				lowest_slot = i;
			}
		}
		if (Item_EncodeBankedId(rec->item) > lowest)
			spoils->items[lowest_slot] = rec->item;
	}
	return 0;
}

void Battle_AwardSpoils(void)
{
	struct BattleSpoils *spoils;
	s32					 count;
	u16					*list;
	s32					 i;
	s32					 unit_id;
	struct BtlUnit		*unit;
	struct BtlUnit		*backup;
	s32					 cnt;
	u32					 learned;
	s32					 j;
	s32					 best;
	s32					 best_slot;
	s32					 slot;
	u16					*found;
	s32					 value;
	s16					 gains[8];
	u16					 units[8];

	spoils = &BtlWk->spoils;
	if (spoils->experience != 0)
	{
		UiWork_PushValueSlotFar(spoils->experience, 5);
		UiText_ShowMessageAndWaitCoreFar((s32)MsgExpGained);
		BattlePresentation_WaitForAdvance();
	}
	list = units;
	count = BattleParty_ListLivingUnits(1, list);
	backup = (struct BtlUnit *)Runtime_BumpAllocateAlternatePool(sizeof(struct BtlUnit));
	for (i = 0; i < count; i++)
	{
		unit_id = list[i];
		unit = (struct BtlUnit *)UnitGetFar(unit_id);
		unit->experience += spoils->experience;
		while (Iwram_CopyWords(backup, unit, sizeof(struct BtlUnit)),
			Owner_GetValueIfLevelThresholdReachedFar(unit_id, gains) != 0)
		{
			AudioCommand_PlayFar(0x59);
			UiWork_ClearValueNameTablesFar();
			UiWork_PushValueSlotFar(unit->class_index, 3);
			UiWork_PushValueSlotFar(list[i], 1);
			UiWork_PushValueSlotFar(unit->level, 5);
			UiText_ShowMessageAndWaitCoreFar((s32)MsgLevelUp);
			BattlePresentation_WaitForAdvance();
			for (cnt = 0; cnt < 32; cnt++)
			{
				learned = unit->action_slots[cnt].encoded_action;
				if ((learned & OWNER_ACTION_ID_MASK) && (learned >> 15))
				{
					for (j = 0; j < 32; j++)
					{
						if (learned == backup->action_slots[j].encoded_action)
							break;
					}
					if (j == 32)
					{
						UiWork_ClearValueNameTablesFar();
						UiWork_PushValueSlotFar(unit->class_index, 3);
						UiWork_PushValueSlotFar(unit_id, 1);
						UiWork_PushValueSlotFar(learned & OWNER_ACTION_ID_MASK, 4);
						AudioCommand_PlayFar(0x9a);
						UiText_ShowMessageAndWaitCoreFar((s32)MsgAbilityMastered);
						BattlePresentation_WaitForAdvance();
					}
				}
			}
			if (gains[2] != 0)
			{
				UiWork_PushValueSlotFar(gains[2], 5);
				UiText_ShowMessageAndWaitCoreFar((s32)MsgMaxHpRises);
				BattlePresentation_WaitForAdvance();
			}
			if (gains[3] != 0)
			{
				UiWork_PushValueSlotFar(gains[3], 5);
				UiText_ShowMessageAndWaitCoreFar((s32)MsgMaxPpRises);
				BattlePresentation_WaitForAdvance();
			}
			if (gains[4] != 0)
			{
				UiWork_PushValueSlotFar(gains[4], 5);
				UiText_ShowMessageAndWaitCoreFar((s32)MsgAttackRises);
				BattlePresentation_WaitForAdvance();
			}
			if (gains[5] != 0)
			{
				UiWork_PushValueSlotFar(gains[5], 5);
				UiText_ShowMessageAndWaitCoreFar((s32)MsgDefenseRises);
				BattlePresentation_WaitForAdvance();
			}
			if (gains[6] != 0)
			{
				UiWork_PushValueSlotFar(gains[6], 5);
				UiText_ShowMessageAndWaitCoreFar((s32)MsgAgilityRises);
				BattlePresentation_WaitForAdvance();
			}
			if (gains[7] != 0)
			{
				UiWork_PushValueSlotFar(gains[7], 5);
				UiText_ShowMessageAndWaitCoreFar((s32)MsgLuckRises);
				BattlePresentation_WaitForAdvance();
			}
		}
	}
	MemFree(backup);
	if (spoils->coins != 0)
	{
		UiWork_PushValueSlotFar(spoils->coins, 5);
		UiText_ShowMessageAndWaitCoreFar((s32)MsgCoinsGained);
		Party_AdjustCoinsFar(spoils->coins);
		BattlePresentation_WaitForAdvance();
	}
	found = &gOverflowItem;
	for (;;)
	{
		best_slot = -1;
		best = -1;
		for (slot = 0; slot <= 3; slot++)
		{
			if (spoils->items[slot] != 0)
			{
				value = Item_EncodeBankedId(spoils->items[slot]);
				if (value >= best)
				{
					best = value;
					best_slot = slot;
				}
			}
		}
		if (best_slot == -1)
			break;
		UiWork_PushValueSlotFar(spoils->items[best_slot], 2);
		UiText_ShowMessageAndWaitCoreFar((s32)MsgItemGained);
		BattlePresentation_WaitForAdvance();
		if (PartyInventory_AddFar(spoils->items[best_slot]) == -1)
		{
			*found = spoils->items[best_slot];
			break;
		}
		spoils->items[best_slot] = 0;
	}
}

void Battle_ReservedNoOp2A08(void)
{
}
