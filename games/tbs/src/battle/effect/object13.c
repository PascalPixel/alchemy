#include "unit/battle/effect/btlcue.h"
#include "unit/battle/effect/zonedata.h"
#include "vram_tab.h"
#include "unit/battle/effect/object.h"
#include "unit/battle/effect/object9.h"
#include "unit/battle/effect/object10.h"
#include "unit/battle/effect/object11.h"
#include "unit/battle/effect/object12.h"
#include "unit/battle/effect/object13.h"
#include "unit/battle/effect/object14.h"
#include "unit/battle/effect/object15.h"
#include "unit/battle/effect/object16.h"
#include "unit/battle/effect/object17.h"
#include "unit/battle/effect/object18.h"
#include "unit/field/common/effect/move.h"
#include "unit/graphics/display/scroll_initialize_hblank_dma.h"
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
#include "types.h"
#include "scene.h"
#include "btltype.h"
#include "game_state.h"
#include "fieldrun.h"
#include "btlsys.h"
#include "battle.h"
#include "battle_unit.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "mem.h"
#include "dma.h"
#include "scene_ids.h"
#include "io_reg.h"
#include "iowrite.h"
#include "irq.h"
#include "callback_scheduler.h"
#include "flag_ids.h"
#include "iwram_call.h"
#include "eventwrk.h"
#include "resource_ids.h"
#include "battle_effect_runtime.h"
#include "resource.h"
#include "field_scene.h"
#include "map.h"
#include "map_scroll.h"
#include "ram_buffer.h"
#include "global_cells.h"
#include "object_runtime.h"
#include "script_object_runtime.h"
#include "animspr.h"
#include "fieldobj.h"
#include "object_dispatch.h"
#include "field_event.h"
#include "event_runtime.h"
#include "field_sprite.h"
#include "battle_effect_counters.h"
#include "heap_state.h"
#include "keystate.h"
#include "ownerval.h"
#include "party_state.h"
#include "sound_ids.h"
#include "fixed_math.h"
#include "itemgift.h"
#include "btlcalc.h"
#include "disptran.h"
#include "text/msg_ids.h"
#include "object_lookup.h"
#include "objdisp.h"
#include "effect_runtime.h"
#include "object_effect.h"
#include "item.h"
#include "motobj.h"
#include "layout_guard.h"
#include "frame.h"
#include "io_write_queue.h"
#include "palbuf.h"
#include "vram_block.h"
#include "item_ids.h"
#include "inventory.h"
#include "ui.h"
#include "window.h"
#include "btlpty.h"
#include "b5_context.h"
#include "edition.h"
#include "facing_object.h"
#include "script_motion.h"
#include "tbs_edition.h"
#include "metadata_lookup.h"
#include "fieldcoo.h"
#include "scroll.h"
#include "fxblend.h"
#include "fx_scene.h"
#include "effect_slot.h"
#include "object_efx.h"
#include "menu_list.h"
#include "field_effect.h"
#include "fixed_point_position.h"

void DisplayScroll_ArmHBlankDma(void)
{
	volatile u16			 *dma;
	struct DisplayScrollWork *state;
	u32						 *source;
	u16						  status;
	u32						  control;
	volatile u32			 *destination;

	state = gHBlankScrollWork;
	source = (u32 *)state->rows[state->page];

	dma = REG_DMA0;
	destination = (volatile u32 *)0x04000014;
	status = dma[5];
	control = status & (u16)~DMA_START_HBLANK_REPEAT;
	dma[5] = control;
	status = dma[5];
	control = status & (u16)~DMA_ENABLE;
	dma[5] = control;
	(void)dma[5];

	*destination = *source++;
	*destination = *source++;
	*destination = *source++;

	control = 0xa6600003;
	Dma_Set(source, destination, control, (volatile u32 *)dma);
}

void DisplayScroll_BuildAndSwapHBlankPage(void)
{
	struct DisplayScrollWork *work = gHBlankScrollWork;
	s32						  x3 = (s16)gBgScroll[3].x;
	s32						  y3 = (s16)gBgScroll[3].y;
	s32						  x2 = (s16)gBgScroll[2].x;
	s32						  y2 = (s16)gBgScroll[2].y;
	s32						  x1 = (s16)gBgScroll[1].x;
	s32						  y1 = (s16)gBgScroll[1].y;
	u16						 *row;
	s32						  step;
	s32						  phase;
	s32						  amp;
	s32						  i;
	u16						  d;

	row = &work->rows[work->page ^ 1][0][0].x;
	step = work->step_x;
	phase = (work->frame + (u16)y3) * work->freq_x;
	if (step == 0)
	{
		for (i = 0; i != 160; i++)
		{
			row[i * 6] = x3;
			row[i * 6 + 2] = x2;
			row[i * 6 + 4] = x1;
		}
	}
	else
	{
		u16 a = x3;
		u16 b = x2;
		u16 c = x1;

		amp = work->amp_x;
		for (i = 0; i != 160; i++)
		{
			d = Iwram_MulQ16(DisplayScroll_WaveSine[(phase >> 16) & 255], amp) / 256;
			*row = a + d;
			row += 2;
			*row = b + d;
			row += 2;
			*row = c + d;
			row += 2;
			phase += step;
		}
	}
	row = &work->rows[work->page ^ 1][0][0].y;
	step = work->step_y;
	phase = (work->frame + (u16)y3) * work->freq_y;
	if (step == 0)
	{
		for (i = 0; i != 160; i++)
		{
			row[i * 6] = y3;
			row[i * 6 + 2] = y2;
			row[i * 6 + 4] = y1;
		}
	}
	else
	{
		u16 a = y3;
		u16 b = y2;
		u16 c = y1;

		amp = work->amp_y;
		for (i = 0; i != 160; i++)
		{
			d = Iwram_MulQ16(DisplayScroll_WaveSine[(phase >> 16) & 255], amp) / 256;
			*row = a + d;
			row += 2;
			*row = b + d;
			row += 2;
			*row = c + d;
			row += 2;
			phase += step;
		}
	}
	work->frame++;
	work->page ^= 1;
}

// ここに共通ソースが入る（ROM の並びのとおり）
#include "../../graphics/display/scroll_initialize_hblank_dma.c"

void DisplayScroll_DisarmHBlankDma(void)
{
	volatile u16 *dma0;

	Scheduler_RemoveCallback(DisplayScroll_ArmHBlankDma);
	Scheduler_RemoveCallback(DisplayScroll_BuildAndSwapHBlankPage);
	dma0 = REG_DMA0;
	dma0[5] = 0xC5FF & dma0[5];
	dma0[5] = 0x7FFF & dma0[5];
	(void)dma0[5];
}

struct GroundParticle
{
	u8	unknown_00[4];
	u16 y : 8;
	u16 affine : 2;
	u16 blend_mode : 2;
	u16 mosaic : 1;
	u16 full_color : 1;
	u16 shape : 2;
	u16 x : 9;
	u16 affine_index : 5;
	u16 size : 2;
	u16 tile : 10;
	u16 priority : 2;
	u16 palette : 4;
	u8	unknown_0a[2];
	s32 pos_x;
	s32 pos_y;
	s32 pos_z;
	s32 delay;
	u16 timer;
	u8	unknown_1e[2];
};

struct GroundParticleWork
{
	s32					  entry;
	s32					  tile_base;
	struct GroundParticle particles[32];
	s32					  unknown_408;
	s32					  stopped;
};

void FieldFx_UpdateGroundParticles(void)
{
	struct MapScrollWork	  *view = gMapWork[0];
	struct GroundParticleWork *work = gMapWork[21];
	s32						  *camera = &view->view_x;
	s32						   camera_x = camera[0];
	s32						   camera_z = camera[1];
	struct GroundParticle	  *particle = work->particles;
	u32						   i;

	for (i = 0; i < 32; i++, particle++)
	{
		s16 *frame;
		s32	 x;
		s32	 y;

		if (--particle->timer == 0xffff)
			continue;
		if (GameFlag_TestFar(0x166))
			particle->timer++;
		frame = &FieldFx_GroundParticleFrames[5 * particle->timer];
		x = (particle->pos_x - camera_x) / 0x10000 + *frame++;
		y = (particle->pos_z - particle->pos_y - camera_z) / 0x10000 + *frame++;
		if ((x >= -16 && x <= 239) && y >= -32 && y <= 159)
		{
			particle->priority = 1;
			particle->x = x;
			particle->y = y;
			particle->tile = work->tile_base + *(u16 *)frame;
			frame++;
			particle->shape = *(u8 *)frame;
			particle->size = *(u8 *)(frame + 1);
			Runtime_PushSlotEntry(particle, 240);
		}
		if (particle->timer == 0)
		{
			s32 *leader = view->origin;

			x = leader[0] + (Random16() << 8) - 0x800000;
			y = leader[2] + (Random16() << 8) - 0x800000;
			particle->pos_x = x;
			particle->pos_z = y;
			particle->pos_y = Map_GetTerrainHeightFar(0, x >> 16, y >> 16) << 16;
			particle->timer = 16;
		}
	}
}

void BattleFx_UpdateStormFlash(void)
{
	struct FieldBlendWork	   *work = gFieldBlendWork[0];
	struct BattleEffectBuffers *target = gFieldBlendWork[2];
	s16						   *timer = &work->timer;

	if (*timer < 0)
		return;
	if (GameFlag_TestFar(358))
		work->timer = 128;
	switch ((*timer)--)
	{
	case 0:
		if (work->enabled != 0)
		{
			u32 a = Random16();
			u32 b = Random16();
			work->timer = ((a * 400) >> 16) - ((b * 100) >> 16) + 150;
			if (work->loud != 0)
				AudioCommand_PlayFar(172);
			else
				AudioCommand_PlayFar(171);
		}
	case 5:
	case 10:
		BattleFx_ApplyColorToTargetBuffer(work, 1);
		Dma_Set(work->delta, target->delta, 0x840002a0, REG_DMA3);
		target->duration = 12;
		target->step = 0;
		break;
	case 1:
	case 6:
	case 11:
		BattleFx_ApplyColorToTargetBuffer(work->to, 1);
		target->duration = 1;
		target->step = 0;
		break;
	}
}

void FieldFx_StartGroundParticles(void)
{
	struct GroundParticleWork *work;
	s32						  *leader;
	struct GroundParticle	  *particle;
	u32						  *words;
	volatile u32			   fill;
	u32						   i;

	/* FAKEMATCH: needed to match: reusing the cursor preserves its register */
	work = Runtime_AllocateBlock(29, sizeof(struct GroundParticleWork));
	leader = ((struct MapScrollWork *)gMapWork[0])->origin;
	BattleFx_SetQueuedSoundAndPlay(170);
	words = 0;
	particle = work->particles;
	fill = (u32)words;
	Dma_Set(
		(const void *)&fill, work, 0x85000000 | (sizeof(struct GroundParticleWork) / 4), REG_DMA3);
	words = Runtime_AllocateBlock(14, 0x400);
	Resource_DecodeByteLz(FieldFx_GroundParticleTiles, words);
	work->entry = Resource_FindFreeEntry();
	work->tile_base = VramBlock_LoadCached(work->entry, 0x300, words);
	Runtime_ReleaseHeapBlock(14);
	for (i = 0; i < 32; i++)
	{
		s32 x;
		s32 z;

		words = (u32 *)particle;
		*words++ = 0;
		*words++ = 0x40000400;
		*words = 0xd400;
		x = leader[0];
		z = leader[2];
		particle->pos_x = x;
		particle->pos_z = z;
		particle->pos_y = Map_GetTerrainHeightFar(0, x >> 16, z >> 16) << 16;
		particle->timer = (i & 15) + 1;
		particle++;
	}
	Io_Write16(0x3f00, &REG_BLDCNT);
	Io_Write16(0x1008, &REG_BLDALPHA);
	Io_Write16(0, &REG_BLDY);
	Scheduler_AddOrUpdateCallback(FieldFx_UpdateGroundParticles, 0xc80);
}

struct SparkleFrame
{
	s16 dy;
	u16 tile;
};

extern const struct SparkleFrame FieldFx_SparkleFrames[];

union DustWord
{
	u32	  value;
	void *link;
};

void FieldMotes_Update(void)
{
	struct GroundParticleWork *work = WORK_SLOT(29);
	struct MapScrollWork	  *map = WORK_SLOT(8);
	u32						   spawned = 0;
	u32						   i;
	struct GroundParticle	  *mote = work->particles;

	for (i = 0; i < 32; i++, mote++)
	{
		s32 x;
		s32 y;

		if (--mote->timer != 0xffff)
		{
			s32 *camera = &map->view_x;
			s32	 camera_x = camera[0];
			s32	 camera_z = camera[1];
			u16	 age = mote->timer;
			u32	 a;
			u32	 b;

			if (GameFlag_TestFar(0x166))
			{
				mote->timer++;
				mote->delay--;
			}
			a = Random16();
			b = Random16();
			x = ((mote->pos_x - camera_x) >> 16) + (((a & 1) + (b & 1)) >> 1) - 1;
			y = ((mote->pos_z - mote->pos_y - camera_z) >> 16) - age;
			if ((u32)(x + 16) <= 255 && y >= -32 && y <= 159)
			{
				if (mote->timer < 60)
				{
					mote->tile = work->tile_base + 16;
					mote->delay += 3;
				}
				else if (mote->timer < 90)
				{
					mote->tile = work->tile_base + 8;
					mote->delay++;
				}
				else
				{
					mote->tile = work->tile_base;
				}
				if ((gFrameCount >> 3) & 1)
					mote->tile += 4;
				mote->x = x;
				mote->y = y - (mote->delay >> 2);
				mote->shape = 0;
				mote->size = 1;
				Runtime_PushSlotEntry(mote, 240);
			}
			else
			{
				mote->timer = 0;
			}
		}
		if (spawned < 8 && mote->timer == 0)
		{
			s32 *leader = map->origin;

			x = leader[0] + (Random16() << 8) - 0x800000;
			y = leader[2] + (Random16() << 8) - 0x800000;
			mote->pos_z = y;
			mote->pos_x = x;
			mote->pos_y = Map_GetTerrainHeightFar(0, x >> 16, y >> 16) << 16;
			mote->timer = 120;
			mote->delay = 0;
			spawned++;
		}
	}
}

void FieldMotes_Start(void)
{
	struct GroundParticleWork *work;
	struct GroundParticle	  *mote;
	u8						  *tiles;
	u32						   i;
	u32						   size;
	volatile u32			   zero;

	size = 0x410;
	work = Runtime_AllocateBlock(29, size);
	mote = work->particles;
	/* FAKEMATCH: needed to match: reusing the allocation size for the cleared word keeps it in r1. */
	size = 0;
	zero = size;
	Dma_Set((void *)&zero, work, DMA_ENABLE32 | DMA_32BIT | DMA_SRC_FIXED32 | 0x104, REG_DMA3);
	tiles = Runtime_AllocateBlock(14, 0x400);
	Resource_DecodeByteLz(FieldFx_MoteTiles, tiles);
	work->entry = Resource_FindFreeEntry();
	work->tile_base = VramBlock_LoadCached(work->entry, 0x300, tiles);
	Runtime_ReleaseHeapBlock(14);
	for (i = 0; i < 32; i++)
	{
		s32 *pos = ((struct MapScrollWork *)gMapWork[0])->origin;
		/* FAKEMATCH: needed to match: reusing the integer field cursor for the height x argument keeps the stores in r1. */
		u32 p = (u32)mote;
		s32 z;
		*(s32 *)p = 0;
		p += sizeof(s32);
		*(s32 *)p = 0x40000400;
		p += sizeof(s32);
		*(s32 *)p = 0xd400;
		p = pos[0];
		z = pos[2];
		mote->pos_x = (s32)p;
		mote->pos_z = z;
		mote->pos_y = Map_GetTerrainHeightFar(0, (s32)p >> 16, z >> 16) << 16;
		mote->timer = (i & 15) + 1;
		mote++;
	}
	Io_Write16(0x3f00, &REG_BLDCNT);
	Io_Write16(0x1008, &REG_BLDALPHA);
	Io_Write16(0, &REG_BLDY);
	Scheduler_AddOrUpdateCallback((s32)(FieldMotes_Update), 0xc80);
}

void FieldEffect_UpdateSparkles(void)
{
	/* FAKEMATCH: needed to match: an unsigned-long frame read keeps size and flip stores together. The plain volatile read differs by 3: it splits the two bit-field stores. */
	struct GroundParticleWork *work;
	u32						   spawned;
	u32						   i;
	struct MapScrollWork	  *map;
	s32						  *camera;
	s32						   spawn_x;
	s32						   spawn_z;
	struct GroundParticle	  *sparkle;
	const u16				  *frame;
	s32						  *leader;
	s32						   camera_x;
	s32						   camera_z;
	s32						   sx;
	s32						   sy;
	s32						   delay;
	s32						   x;
	u32						   a;
	u32						   b;
	u32						   frames;

	work = WORK_SLOT(29);
	spawned = 0;
	map = WORK_SLOT(8);
	camera = &map->view_x;
	spawn_x = 0;
	spawn_z = 0;
	i = 0;
	sparkle = work->particles;
	delay = 0;
	sx = 0;
	sy = 0;
	do
	{
		if (sparkle->timer != 0)
		{
			camera_x = camera[0];
			camera_z = camera[1];
			if (GameFlag_TestFar(0x166) != 0)
				sparkle->timer++;
			frame = (const u16 *)&FieldFx_SparkleFrames[sparkle->timer >> 1];
			a = Random16();
			b = Random16();
			x = ((sparkle->pos_x - camera_x) >> 16) + (((a & 1) + (b & 1)) >> 1);
			sx = x - 1;
			sy = (sparkle->pos_z - sparkle->pos_y - camera_z) / 0x10000 + *(const s16 *)frame;
			frame++;
			if ((u32)(x + 15) <= 255 && sy >= -32 && sy <= 159)
			{
				sparkle->tile = work->tile_base + *frame;
				sparkle->x = sx;
				sparkle->shape = 0;
				sparkle->size = 1;
				frames = *(unsigned long *)&gFrameCount;
				sparkle->y = sy;
				sparkle->affine_index = ((frames >> 1) & 1) << 3;
				Runtime_PushSlotEntry(sparkle, 240);
			}
			sparkle->timer--;
		}
		if (spawned <= 3 && sparkle->timer == 0 && work->stopped == 0)
		{
			if (delay != 0)
			{
				sparkle->pos_x = spawn_x;
				sparkle->pos_z = spawn_z;
				sparkle->pos_y = Map_GetTerrainHeightFar(0, sx >> 16, sy >> 16) << 16;
				sparkle->timer = 62 - delay;
				sparkle->delay = 0;
				spawned++;
				delay += 4;
			}
			else if ((Random16() & 255) == 0)
			{
				leader = map->origin;
				spawn_x = leader[0] + (Random16() << 8) - 0x800000;
				spawn_z = leader[2] + (Random16() << 8) - 0x800000;
				sparkle->pos_x = spawn_x;
				sparkle->pos_z = spawn_z;
				sparkle->pos_y = Map_GetTerrainHeightFar(0, sx >> 16, sy >> 16) << 16;
				sparkle->timer = 30;
				sparkle->delay = delay;
				spawned++;
				delay = 4;
			}
		}
		i++;
		sparkle++;
	} while (i <= 31);
}

void FieldEffect_InitSparkles(void)
{
	u32						   size = 0x410;
	struct GroundParticleWork *work = Runtime_AllocateBlock(29, size);
	struct GroundParticle	  *p = work->particles;
	u8						  *buf;
	u32						   i;
	s32						   clear;
	volatile u32			   zero;

	/* FAKEMATCH: needed to match: reusing the allocation size for the fill word keeps zero in r1; a separate helper value differs by four instructions. */
	size = 0;
	zero = size;
	Dma_Set(&zero, work, DMA_ENABLE32 | DMA_32BIT | DMA_SRC_FIXED32 | 0x104, REG_DMA3);
	buf = Runtime_AllocateBlock(14, 0x400);
	Resource_DecodeByteLz(FieldFx_SparkleTiles, buf);
	work->entry = Resource_FindFreeEntry();
	work->tile_base = VramBlock_LoadCached(work->entry, 0x200, buf);
	Runtime_ReleaseHeapBlock(14);
	i = 0;
	clear = 0;
loop:
	{
		struct MapScrollWork *view = gMapWork[0];
		s32					 *leader;
		/* FAKEMATCH: needed to match: an integer field cursor keeps the view load order; reusing it for the height x argument keeps the leader in r2. Separate temporaries differ by 14 instructions. */
		u32 attr;
		s32 z;

		attr = (u32)p;
		*(u32 *)attr = clear;
		attr += sizeof(union DustWord);
		leader = view->origin;
		*(u32 *)attr = 0x40000400;
		attr += sizeof(union DustWord);
		*(u32 *)attr = 0xd400;
		attr = leader[0];
		z = leader[2];
		p->pos_x = clear;
		p->pos_z = clear;
		p->pos_y = Map_GetTerrainHeightFar(0, (s32)attr >> 16, z >> 16) << 16;
		p->timer = (i & 15) + 1;
	}
	i++;
	p++;
	if (i < 32)
		goto loop;
	Scheduler_AddOrUpdateCallback((s32)(FieldEffect_UpdateSparkles), 0xc80);
}

void BattleFx_StartTwelveFrameBlend(void)
{
	struct FieldBlendWork	   *work;
	struct BattleEffectBuffers *buffers;
	volatile u32				zero;

	work = Runtime_AllocateBlock(30, sizeof(struct FieldBlendWork));
	buffers = gPaletteBufferWork;
	zero = 0;
	Dma_Set(
		(const void *)&zero, work, DMA_ENABLE32 | DMA_32BIT | DMA_SRC_FIXED32 | 0x7e2, REG_DMA3);
	((s32 (*)())BattleFx_BuildBuffer)(0x10003, buffers, work, 1); /* FAKEMATCH: called as an undeclared function (int result, no prototype) */
	((s32 (*)())BattleFx_BuildBuffer)(0x10005, buffers, work->to, 1); /* FAKEMATCH: called as an undeclared function (int result, no prototype) */
	BattleFx_InterpolateBuffers(work->to, work->from, work->delta, 12);
	((s32 (*)())BattleFx_BuildBuffer)((s32)work, 0, buffers->target, 1); /* FAKEMATCH: called as an undeclared function (int result, no prototype) */
	work->timer = 600;
	work->enabled = 1;
	Scheduler_AddOrUpdateCallback((s32)(BattleFx_UpdateStormFlash), 0xc80);
}

void BattleFx_SetBlock30ValuesMaxZero(void)
{
	struct FieldBlendWork *state = Runtime_AllocateBlock(30, sizeof(struct FieldBlendWork));
	state->timer = 0x7FFF;
	state->enabled = 0;
}

void BattleFx_SetBlock30Values12Zero(void)
{
	struct FieldBlendWork *state = Runtime_AllocateBlock(30, sizeof(struct FieldBlendWork));
	state->timer = 12;
	state->enabled = 0;
}

void BattleFx_SetBlock30Values128One(void)
{
	struct FieldBlendWork *state = Runtime_AllocateBlock(30, sizeof(struct FieldBlendWork));
	state->timer = 128;
	state->enabled = 1;
}

void BattleFx_StartBufferBlend(s32 from, s32 to)
{
	struct FieldBlendWork	   *work;
	struct BattleEffectBuffers *buffers;
	volatile u32				zero;
	s16						   *target;

	work = Runtime_AllocateBlock(30, sizeof(struct FieldBlendWork));
	buffers = gPaletteBufferWork;
	zero = 0;
	Dma_Set(
		(const void *)&zero, work, DMA_ENABLE32 | DMA_32BIT | DMA_SRC_FIXED32 | 0x7e2, REG_DMA3);
	((s32 (*)())BattleFx_BuildBuffer)(from, buffers, work, 1); /* FAKEMATCH: called as an undeclared function (int result, no prototype) */
	target = work->to;
	((s32 (*)())BattleFx_BuildBuffer)(to, buffers, target, 1); /* FAKEMATCH: called as an undeclared function (int result, no prototype) */
	BattleFx_InterpolateBuffers(target, work->from, work->delta, 12);
	((s32 (*)())BattleFx_BuildBuffer)((s32)work, 0, buffers->target, 1); /* FAKEMATCH: called as an undeclared function (int result, no prototype) */
	work->timer = 120;
	work->enabled = 0;
	Scheduler_AddOrUpdateCallback((s32)(BattleFx_UpdateStormFlash), 0xc80);
}

//--------------------------------------------------------------------------

// motion_move_halfway_toward_target.c

// オブジェクト：つながった相手への追従

//--------------------------------------------------------------------------

// ほかのモジュールの関数 (共通ヘッダーにまだ宣言が無いもの)

// このファイルの関数 (定義より前で使うもの)
