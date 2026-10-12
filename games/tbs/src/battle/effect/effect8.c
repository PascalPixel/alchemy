#include "unit/battle/effect/fxscales.h"
#include "unit/battle/effect/koukabit.h"
#include "unit/battle/effect/koukatbl.h"
#include "unit/battle/effect/effect.h"
#include "unit/battle/effect/effect2.h"
#include "unit/battle/effect/effect3.h"
#include "unit/battle/effect/effect4.h"
#include "unit/battle/effect/effect5.h"
#include "unit/battle/effect/effect6.h"
#include "unit/battle/effect/effect7.h"
#include "unit/battle/effect/effect8.h"
#include "unit/battle/effect/effect9.h"
#include "unit/battle/effect/effect10.h"
#include "unit/battle/effect/effect11.h"
#include "unit/battle/effect/object.h"
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
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "mem.h"
#include "canvas.h"
#include "map_scroll.h"
#include "heap_state.h"
#include "menu_list.h"
#include "types.h"
#include "io_write_queue.h"
#include "io_reg.h"
#include "scene.h"
#include "global_cells.h"
#include "dma.h"
#include "resource_ids.h"
#include "battle_efx.h"
#include "fixed_math.h"
#include "callback_scheduler.h"
#include "effect_step.h"
#include "battle_effect_work.h"
#include "btldisp.h"
#include "ram_buffer.h"
#include "resource.h"
#include "b5_context.h"
#include "motobj.h"
#include "iwram_call.h"
#include "transform.h"
#include "btlwork.h"
#include "project.h"
#include "field_event.h"
#include "keystate.h"
#include "btltype.h"
#include "sound_ids.h"
#include "call.h"
#include "btlsys.h"
#include "battle_session.h"
#include "animspr.h"
#include "battle_unit.h"
#include "fixed_point_position.h"
#include "irq.h"

#define ABS(v) ((v) < 0 ? -(v) : (v))

// 倍率 {x, y} (Q16)
typedef struct Scale
{
	s32 x;
	s32 y;
} Scale;

void BattleFx_SpawnObjects(s32 entry_count, s32 kind, u32 variant)
{
	struct BattleEffectWork *work = *(struct BattleEffectWork **)gBattleFxWork;
	s32						 entry_index = 0;

	if (entry_count == 0)
	{
		return;
	}
	do
	{
		struct AnimationObject *object = ResourceObject_CreateFar(kind);

		work->objects[entry_index] = object;
		if (object != 0)
		{
			object->flags = 0;
			AnimationObjects_SelectAnimationFar(object, entry_index);
			((struct AnimationObject *)work->objects[entry_index])->part[0].priority = variant;
		}
		entry_index++;
	} while (entry_index != entry_count);
}

void BattleFx_RunNoEffect(void)
{
}

#define SHEET (work->sheet + 0xC56)

void BattleFx_ArmBg2AffineHBlankDma(void)
{
	struct BattleEffectWork *work = *(void **)gBattleFxWork;
	volatile u16			*channel = REG_DMA0;
	channel[5] &= 0xc5ff;
	channel[5] &= 0x7fff;
	(void)channel[5];
	Dma_Set(work->bg2_x, (void *)0x04000028, 0xa6600001, (volatile u32 *)channel);
}

void BattleFx_RunSeriesIMode0(struct BattleEffectArgument *effect)
{
	BattleEffect_RunBurstShower(effect, 0);
}

void BattleFx_RunSeriesIMode2(struct BattleEffectArgument *effect)
{
	BattleEffect_RunBurstShower(effect, 2);
}

void BattleFx_RunSeriesIMode6(struct BattleEffectArgument *effect)
{
	BattleEffect_RunBurstShower(effect, 6);
}

void BattleFx_RunSeriesIMode3(struct BattleEffectArgument *effect)
{
	BattleEffect_RunBurstShower(effect, 3);
}

void BattleFx_RunSeriesIMode5(struct BattleEffectArgument *effect)
{
	BattleEffect_RunBurstShower(effect, 5);
}

void BattleFx_RunSeriesIMode7(struct BattleEffectArgument *effect)
{
	BattleEffect_RunBurstShower(effect, 7);
}

void BattleFx_RunSeriesIMode4(struct BattleEffectArgument *effect)
{
	BattleEffect_RunBurstShower(effect, 4);
}

void BattleEffect_RunBurstShower(struct BattleEffectArgument *efx, s32 mode)
{
	u32						*cache;
	struct BattleEffectWork *work;
	void					*dst;
	u8						*aux;
	DrawRectangle			 blit[2];
	s32						 pos[3];
	s32						 seat[8][3];
	s32						 aim[3];
	u16						*pal;
	s32						 frame;
	s32						 i;
	s32						 pick;
	s32						 lum;
	s32						 id;
	s32						 width;
	s32						 height;

	cache = (u32 *)&gWorkSlot[HEAP_SLOT_BATTLE_CANVAS];
	dst = (void *)cache[0];
	work = (struct BattleEffectWork *)cache[-1];
	aux = (u8 *)cache[1];
	work->effect = efx;
	BattleFx_BeginCanvasLayer(0);
	REG_BLDALPHA = 0x1010;

	if (mode == 7)
	{
		BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 3, 2);
		blit[0] = (DrawRectangle)cache[46 - 40];
		BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, 7, 2);
		blit[1] = (DrawRectangle)cache[47 - 40];
	}
	else
	{
		BattleFx_FetchRectangleBlitters(work->effect->side, blit);
	}

	Resource_LoadAndDecompress((s32)&ResourceId_TornadoSheet, work->sheet, 1, 0);
	if (mode == 5)
	{
		Resource_LoadAndDecompress((s32)&ResourceId_VioletLightningSheetA, SHEET, 1, 1);
	}
	else if (mode == 7)
	{
		Resource_LoadAndDecompress((s32)&ResourceId_DemonFaceSheet, SHEET, 1, 1);
	}
	else
	{
		Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet, SHEET, 1, 1);
		Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, aux, 0, 0);
		if (mode == 6)
		{
			for (i = 0, pal = (u16 *)0x05000000; i != 64; i++)
			{
				lum = i / 4;
				*pal = ((lum << 10) | (lum << 5)) | lum;
				pal++;
			}
			REG_BLDCNT = 0;
		}
		else
		{
			switch (mode)
			{
			case 0:
				id = (s32)&ResourceId_FlashBurstSheet;
				break;
			case 1:
				id = (s32)&ResourceId_IceBlockSheet;
				break;
			case 2:
				id = (s32)&ResourceId_WaterSpraySheet;
				break;
			case 3:
				id = (s32)&ResourceId_PinkPalette;
				break;
			case 4:
			default:
				id = (s32)&ResourceId_MarsDjinnSheet;
				break;
			}
			Iwram_CopyWords((void *)0x05000000, Resource_GetTableEntry(id), 128);
		}
	}

	if (mode == 7)
	{
		work->transfer_mode = 2;
		work->transfer_value = 50;
	}
	else
	{
		work->transfer_mode = 2;
		work->transfer_value = 75;
	}
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);

	for (i = 0; i != 64; i++)
	{
		work->particles[i].variant = -1;
	}

	EffectPosition_ApplyAnimationAndYOffset(work->effect->actor, pos);
	if (mode == 3)
	{
		pos[1] -= 16;
	}
	if (mode == 4)
	{
		if (work->effect->side == 1)
		{
			pos[0] += 28;
		}
		else
		{
			pos[0] -= 28;
		}
	}
	if (mode == 7)
	{
		if (work->effect->side == 1)
		{
			pos[0] += 16;
		}
		else
		{
			pos[0] -= 16;
		}
	}
	if (mode == 5)
	{
		pos[0] = pos[0] / 3;
		REG_BG2PA = 85;
	}

	i = 0;
	while (i != work->effect->count)
	{
		EffectPosition_ApplyStepAndYOffset(
			work->effect->actors[i], (struct EffectPosition *)seat[i]);
		i += 1;
	}

	frame = 0;
	do
	{
		pick = frame % work->effect->count;
		if (frame == 4)
		{
			AudioCommand_PlayFar(0x88);
		}
		if (mode != 6)
		{
			if (frame == 24)
			{
				BattleEventRuntime_BeginPhaseFar(0x86);
			}
		}
		else
		{
			if (frame == 60)
			{
				BattleEventRuntime_BeginPhaseFar(0x86);
			}
		}

		if (mode == 5)
		{
			if (work->effect->side == 1)
			{
				((DrawRectangle)blit[0])(
					dst, SHEET + ((frame / 3 % 3 * 9) << 9), pos[0] - 2, pos[1] - 32, 72, 62);
			}
			else
			{
				((DrawRectangle)blit[0])(
					dst, SHEET + ((frame / 3 % 3 * 9) << 9), pos[0] - 70, pos[1] - 32, 72, 62);
			}
		}
		else
		{
			aim[0] = (seat[pick][0] + (Random16() & 31)) - 16;
			aim[1] = (seat[pick][1] + (Random16() & 63)) - 16;
			if (frame <= 47)
			{
				work->particles[frame].x = pos[0] << 15;
				work->particles[frame].y = pos[1] << 16;
				work->particles[frame].velocity_x = (aim[0] - pos[0]) << 11;
				work->particles[frame].velocity_y = (aim[1] - pos[1]) << 11;
				work->particles[frame].variant = 0;
			}
		}

		i = 0;
		width = 32;
		height = 64;
		do
		{
			struct EffectStep *spark = &work->particles[i];
			if (spark->variant >= 0)
			{
				if (mode == 7)
				{
					if (spark->variant > 5)
					{
						((DrawRectangle)blit[work->effect->side])(dst, SHEET,
							((s16 *)&spark->x)[1] - 16, ((s16 *)&spark->y)[1] - 32, width, height);
					}
				}
				else if (mode == 4)
				{
					if (spark->variant > 5)
					{
						((DrawRectangle)blit[0])(dst, SHEET + ((spark->variant / 4) << 11),
							((s16 *)&spark->x)[1] - 16, ((s16 *)&spark->y)[1] - 32, width, height);
					}
				}
				else if (mode != 5)
				{
					if (spark->variant > 1)
					{
						((DrawRectangle)blit[0])(dst, SHEET + ((spark->variant / 4) << 11),
							((s16 *)&spark->x)[1] - 16, ((s16 *)&spark->y)[1] - 32, width, height);
					}
				}
				spark->x += spark->velocity_x;
				spark->y += spark->velocity_y;
				spark->variant += 1;
				if (spark->variant == 24)
				{
					spark->variant = -1;
				}
			}
			i += 1;
		} while (i != 64);

		if (mode == 5)
		{
			i = 0;
			while (i != work->effect->count)
			{
				if ((frame >= (i * 4) + 2) && ((frame & 7) == i))
				{
					work->shake_frames = 8;
					ObjectGroup_UpdateMembers(work->effect->actors[i], 7, 5, i, 4);
				}
				i += 1;
			}
		}
		else
		{
			i = 0;
			while (i != work->effect->count)
			{
				if ((frame >= (i * 4) + 16) && ((frame & 7) == i))
				{
					work->shake_frames = 8;
					if (mode == 6)
					{
						ObjectGroup_UpdateMembers(work->effect->actors[i], 14, 5, i, 4);
					}
					else
					{
						ObjectGroup_UpdateMembers(work->effect->actors[i], 7, 5, i, 4);
					}
					BattleMotion_ApplyVariantMotionFar(work->effect->actors[i], 4);
				}
				i += 1;
			}
		}

		Camera_ApplyShake(4, 4);
		if (mode != 6)
		{
			ObjectGroup_TickMemberTimers();
		}
		work->transfer_pending = 1;
		WaitFrames(1);
		frame += 1;
	} while (frame != 64);

	Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
	BattleFx_EndCanvasLayer();
}

void BattleFx_RunSwirlingStars(void *object)
{
	void				   **heap_cache;
	void				   **cursor;
	struct BattleEffectWork *work;
	void					*canvas;
	s32						 record[3];
	struct EffectPosition	 pos;
	DrawRectangle			 draw[2];
	struct EffectStep		*star;
	s32						 i;
	s32						 frame;

	heap_cache = (void **)gBattleFxWork;
	cursor = heap_cache;
	work = *cursor++;
	canvas = *cursor;
	work->effect = object;
	BattleFx_BeginCanvasLayer(0);
	Resource_LoadAndDecompress((s32)&ResourceId_StarDotSheet, work->sheet, 1, 1);
	BattleFx_FetchRectangleBlitters(work->effect->side ^ 1, draw);
	for (i = 0; i != 256; i++)
	{
		star = &PARTICLES[i];
		star->x = ((Random16() & 0xff) - 127) << 16;
		star->y = ((Random16() & 0xff) - 127) << 16;
		star->z = ((Random16() & 0xff) - 127) << 16;
		star->velocity_x = 0;
		star->velocity_y = 0;
		star->velocity_z = 0;
		star->variant = 0;
	}
	work->transfer_mode = 1;
	work->transfer_value = 0;
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
	record[0] = 0;
	record[1] = 160 << 15;
	record[2] = 0;
	for (frame = 0; frame != 160; frame++)
	{
		struct BattleCamera *camera;

		camera = gCameraWork;
		Render_ResetTransformState();
		Graphics_PrepareTransferInIwramWork(camera->eye, camera->pos);
		SceneTransform_ApplyPosition(record);
		star = PARTICLES;
		for (i = 0; i != 64; i++, star++)
		{
			if (frame > i / 4 && star->variant == 0)
			{
				s32 size;

				Graphics_SaveTransferWorkOnce();
				switch (i & 3)
				{
				case 0:
					SceneTransform_ApplyYaw(frame * (i + 32) * 8);
					break;
				case 1:
					SceneTransform_ApplyPitch(-frame * (i + 32) * 8);
					break;
				case 2:
					SceneTransform_ApplyRoll(-frame * (i + 32) * 8);
					break;
				case 3:
					SceneTransform_ApplyPitch(-frame * (i + 32) * 8);
					SceneTransform_ApplyRoll(-frame * (i + 32) * 8);
					break;
				}
				EffectPosition_ApplyBaseAndYOffset((s32 *)star, &pos);
				pos.x >>= 1;
				Graphics_RestoreTransferWork();
				if (pos.depth < 250)
					pos.depth = 250;
				if (pos.depth > 0x27a)
					pos.depth = 0x27a;
				size = 9 - (pos.depth - 250) / 64;
				draw[0](canvas,
					work->sheet + ((i & 3) * 770 + ParticleStreams_CellOffsets[size - 1]),
					pos.x - size / 2, pos.y - size, size, size * 2);
				EffectStep_AdvanceWithGravity3D(star, 60, 0);
				if (frame > i / 4 + 30)
				{
					s32 dx = -star->x >> 8;
					s32 dy = -star->y >> 8;
					s32 dz = -star->z >> 8;

					star->velocity_x += dx;
					star->velocity_y += dy;
					star->velocity_z += dz;
				}
			}
		}
		work->transfer_pending = 1;
		WaitFrames(1);
	}
	Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
	BattleFx_EndCanvasLayer();
}

void BattleFx_RunOrbitingSparks(void)
{
	struct BattleEffectWork *work;
	void					*canvas;
	s32						 record[3];
	struct EffectPosition	 center;
	struct EffectPosition	 pos;
	s32						 frame;
	/* FAKEMATCH: needed to match: only draw[0] is used: the second word fills four bytes of the frame */
	DrawRectangle		 draw[2];
	s32					 member;
	struct BattleCamera *camera;
	u8					*palette;
	s32					 x_offset;
	s32					 i;

	work = WORK_SLOT(HEAP_SLOT_BATTLE_EFFECT);
	canvas = WORK_SLOT(HEAP_SLOT_BATTLE_CANVAS);
	camera = WORK_SLOT(HEAP_SLOT_CAMERA);
	BattleFx_BeginCanvasLayer(1);
	REG_BG2PA = 0x100;
	REG_BLDALPHA = 0x1010;
	palette = Resource_GetTableEntry((s32)&ResourceId_SkullSheet);
	Iwram_CopyWords(BG_PLTT, palette, 128);
	Resource_DecodeType01(palette + 128, work);
	BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 3, 2);
	work->transfer_mode = 1;
	work->transfer_value = 0;
	work->transfer_mode = 2;
	work->transfer_value = 75;
	draw[0] = (DrawRectangle)WORK_SLOT(HEAP_SLOT_BLITTER);
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
	if (work->effect->side == 0)
		x_offset = 0;
	else
		x_offset = -112;
	/* FAKEMATCH: needed to match: a one-pass loop is a sched2 barrier, so the seed counter is set before its pointer 2026-10-11: without it differ 2 (movs r4, #0 falls behind the particle offset); i = 0 ahead of the store differs 170. */
	do
	{
		REG_BG2X = x_offset << 8;
	} while (0);
	for (i = 0; i != 64; i++)
	{
		struct EffectStep *step = &work->particles[i];

		step->x = 0;
		step->y = 0;
		step->velocity_x = 0;
		step->z = 4;
	}
	for (frame = 0; frame != work->effect->count * 16 + 64; frame++)
	{
		for (member = 0; member != work->effect->count; member++)
		{
			struct MotionObject *member_object;
			u32					 window;

			member_object = GetBattleObjectSlotFar(work->effect->actors[member])->object;
			EffectPosition_ApplyStepAndYOffset(work->effect->actors[member], &pos);
			window = frame - member * 16;
			pos.x += x_offset;
			if (window < 64)
			{
				/* Ordinary rec differs 118 in EN (265 native, 269 built).
				 * A generic output barrier before filling differs 155 (272
				 * built); after filling it differs 118 (267 built). Keeping
				 * rec live after the call differs 179 (270 built). None keeps
				 * the native loop layout. */
				/* FAKEMATCH: record is filled through r0 so loop.c cannot hoist its address into fp 2026-10-11: loop.c's second pass moves the invariant &record when 15 x savings x life 8 reaches the loop's insn count (about 100 after giv reduction); plain forms differ 118 (hoisted into fp, frame 92), loads before stores differ 12, and two one-pass blocks around the fill and the apply differ 2 (add r5, sp, #64 after the last store). */
				register s32 *rec asm("r0");

				Render_ResetTransformState();
				Graphics_PrepareTransferInIwramWork(camera->eye, camera->pos);
				rec = record;
				rec[0] = member_object->x;
				rec[1] = member_object->y;
				rec[2] = member_object->z;
				EffectPosition_ApplyBaseAndYOffset(rec, &center);
				/* FAKEMATCH: needed to match: sched2 barrier, so this store precedes the spark counter as in the reference 2026-10-11: without it differ 2 (movs r4, #0 moves ahead of the store). */
				do
				{
					center.x += x_offset;
				} while (0);
				for (i = 0; i != 3; i++)
				{
					struct EffectStep *spark = &work->particles[member * 3 + i];
					s32				   x;
					s32				   y;

					x = pos.x + ((Trig_Sin(spark->velocity_x + i * 0x5555) << 3) >> 16);
					y = pos.y + ((Trig_Cos(spark->velocity_x + i * 0x5555) << 3) >> 16);
					spark->velocity_x += 0x200;
					draw[0](canvas, work->sheet + i * 0x240, x - 12, y - 28, 24, 24);
				}
			}
		}
		work->transfer_pending = 1;
		WaitFrames(1);
	}
	Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
	BattleFx_EndCanvasLayer();
}

void BattleFx_RunTargetBursts(void *object)
{
	void				   **heap_cache;
	void				   **cursor;
	struct BattleEffectWork *work;
	void					*canvas;
	void					*palette;
	s32						 frame;
	void					*rectangle_b;
	void					*rectangle_a;
	struct EffectStep		*step;
	s32						 i;
	struct EffectPosition	 screen;
	s32						 record[3];

	heap_cache = (void **)gBattleFxWork;
	cursor = heap_cache;
	work = *cursor++;
	canvas = *cursor;
	work->effect = object;
	BattleFx_BeginCanvasLayer(0);
	Resource_LoadAndDecompress((s32)&ResourceId_SmokeSheet, work->sheet, 1, 1);
	Resource_LoadAndDecompress((s32)&ResourceId_TargetBurstImage, work->sheet + 0x1b00, 0, 0);
	palette = Resource_GetTableEntry((s32)&ResourceId_PinkBurstSheet);
	Iwram_CopyWords((void *)0x05000000, palette, 128);
	BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 3, 3);
	rectangle_a = heap_cache[7];
	BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, 3, 2);
	rectangle_b = heap_cache[8];

	for (i = 0; i != 64; i++)
	{
		s32 angle;
		s32 magnitude;

		step = &work->particles[i];
		angle = Random16() & 0xffff;
		magnitude = Random16() & 0xff;
		step->x = 0;
		step->y = 0;
		step->z = 0;
		if (i % 6 == 5)
		{
			step->velocity_x = 0;
			step->velocity_y = 0;
		}
		else
		{
			step->velocity_x = (Trig_Sin(angle) * magnitude) >> 7;
			step->velocity_y = (Trig_Cos(angle) * magnitude) >> 9;
		}
		step->velocity_z = 0;
		step->variant = 0;
	}

	work->transfer_mode = 2;
	work->transfer_value = 75;
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);

	for (frame = 0; frame != 96; frame++)
	{
		s32					 member;
		struct BattleCamera *camera;

		camera = gCameraWork;
		for (member = 0; member != work->effect->count; member++)
		{
			struct MotionObject *member_object;

			member_object = GetBattleObjectSlotFar(work->effect->actors[member])->object;
			Render_ResetTransformState();
			Graphics_PrepareTransferInIwramWork(camera->eye, camera->pos);
			record[0] = member_object->x;
			record[1] = 160 << 13;
			record[2] = member_object->z;
			SceneTransform_ApplyPosition(record);
			if (frame >= member * 8 && frame < member * 8 + 40)
			{
				struct EffectStep *p;

				for (i = 0; i != 6; i++)
				{
					s32 cel;

					p = &work->particles[member * 6 + i];
					cel = p->variant / 6;
					if (cel > 5)
						cel = 5;
					EffectPosition_ApplyBaseAndYOffset((s32 *)p, &screen);
					screen.x >>= 1;
					if (i == 5)
					{
						EffectStep_AdvanceWithGravity3D(p, 62, 0x800);
						((DrawRectangle)rectangle_a)(canvas, work->sheet + cel * 0x480 + 0x1b00,
							screen.x - 12, screen.y - 36, 24, 48);
					}
					else
					{
						EffectStep_AdvanceWithGravity3D(p, 60, 0x200);
						((DrawRectangle)rectangle_b)(canvas, work->sheet + cel * 0x480,
							screen.x - 12, screen.y - 36, 24, 48);
					}
					p->variant += 1;
				}
			}
		}
		work->transfer_pending = 1;
		WaitFrames(1);
	}
	Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
	BattleFx_EndCanvasLayer();
}

extern const Scale StagedParticles_UnitScale;

#define MATRICES (Ram_MapCellBuffer + 0x3800)

enum SlashSheetCell
{
	SLASH_FLASH_1 = 0x0000,
	SLASH_FLASH_2 = 0x0188,
	SLASH_FLASH_3_TOP = 0x057c,
	SLASH_FLASH_3_BOTTOM = 0x07d4,
	SLASH_FLASH_4_TOP = 0x0aaa,
	SLASH_FLASH_4_MIDDLE = 0x0c90,
	SLASH_FLASH_4_BOTTOM = 0x0e74,
	SLASH_FLASH_5_TOP = 0x1088,
	SLASH_FLASH_5_MIDDLE = 0x11f8,
	SLASH_FLASH_5_BOTTOM = 0x147c,
	SLASH_TOP = 0x16ac,
	SLASH_MIDDLE = 0x17fc,
	SLASH_BOTTOM = 0x1bf3
};

void BattleEffect_RunStagedParticles(struct BattleEffectArgument *effect)
{
	s32							 point[3];
	struct EffectPosition		 screen;
	s32							 position[4];
	struct EffectPosition		 target;
	DrawRectangle				 draw[2];
	void					   **cache;
	struct BattleBackgroundView *presentation;
	void						*canvas;
	struct BattleEffectWork		*work;
	s8							*path;
	s32							 i;
	s32							 j;
	s32							 frame;
	u8							*sheet;
	s32							 scroll_x;
	s32							 path_x;
	s32							 path_y;
	s32							 row;
	s32							 column;

	cache = (void **)&gTransitionWork;
	presentation = cache[0];
	canvas = cache[-4];
	work = cache[-5];
	sheet = cache[-3];
	scroll_x = gBgScroll[1].x;
	work->effect = effect;
	BattleFx_BeginCanvasLayer(0x2000);
	REG_BG2PA = 0x100;
	BattlePres_ConfigureEffectDisplay();
	BG_PLTT[0] = 0;
	BG_PLTT[1] = 0;
	work->transfer_mode = 0;
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
	BattleEffect_WipeCanvas(0, 0);
	BattleFx_SelectLivingTargets(work->effect);
	BattleFx_SpawnObjects(9, 370, 1);
	Resource_LoadAndDecompress((s32)&ResourceId_SlashSheet, work->sheet, 1, 1);
	Iwram_CopyWords((void *)BG_PLTT, Resource_GetTableEntry((s32)&ResourceId_LimePalette), 128);
	Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);
	path = Resource_GetTableEntry((s32)&ResourceId_StagedParticlePath);
	BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 3, 2);
	BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, 3, 3);
	draw[0] = cache[2];
	/* FAKEMATCH: needed to match: the one-pass block keeps the table address after the second read 2026-10-11: without the block differ 2 (mov r2, sp moves ahead of the first store). */
	do
	{
		draw[1] = cache[3];
	} while (0);
	gProjection.center_y = 240;
	WaitFrames(1);
	BattleBackground_LoadFar(1, (s32)&ResourceId_VioletSkyBackdrop, 0);
	work->scroll_timer = 0;
	work->scroll_interval = 4;
	work->scroll_step_x = -1;
	work->scroll_step_y = 0;
	Scheduler_AddOrUpdateCallback(BattleFx_AdvanceScrollOnInterval, 0x480);
	presentation->busy = 1;
	BattleEffect_WipeCanvas(0, 1);
	REG_DISPCNT = 0x7741;
	REG_BG2PA = 0x80;
	REG_BLDALPHA = 0x1010;
	REG_BLDCNT = 0x3f44;
	path_x = 0;
	path_y = 0;
	for (i = 0; i != 16; i++)
	{
		struct EffectStep *slash = &work->particles[i];

		slash->x = (Random16() % 96 + 12) << 16;
		slash->y = ((Random16() & 63) + 32) << 16;
		slash->velocity_x = 0;
		slash->velocity_y = 0;
		slash->variant = 0;
		for (j = 0; j != 24; j++)
		{
			PARTICLES[i * 24 + j].x = (Random16() & 15) + 48;
			Render_ResetTransformState();
			SceneTransform_ApplyRoll(Random16() & 0xffff);
			SceneTransform_ApplyPitch(Random16() & 0xffff);
			SceneTransform_ApplyYaw(Random16() & 0xffff);
			Graphics_SaveTransferWork(MATRICES + (i * 24 + j) * 48);
		}
	}
	work->transfer_mode = 2;
	work->transfer_value = 50;
	REG_BG2CNT = 0x784;
	for (frame = 0; frame != 220 && !(gKeysRepeat & (KEY_A | KEY_B)); frame++)
	{
		if (frame <= 209)
		{
			if (frame == 0)
			{
				path_x = (path[0] << 8) + (u8)path[1];
				path_y = (path[2] << 8) + (u8)path[3];
				path += 4;
			}
			else
			{
				path_x += path[0];
				path_y += path[1];
				path += 2;
			}
			position[3] = 0;
			position[1] = 0xff0000;
			for (row = 0; row != 3; row++)
			{
				for (column = 0; column != 3; column++)
				{
					position[0] = (path_x + 80 + column * 32) << 16;
					position[2] = (64 - path_y + row * 32) << 16;
					Object_ApplyProjectedPlacementFar(
						work->objects[row * 3 + column], position, &StagedParticles_UnitScale, 0);
				}
			}
		}
		point[1] = 0;
		point[2] = 0;
		if (frame == 48)
		{
			work->fade_frames = 24;
			work->fade_step = 0;
		}
		for (i = 0; i != 16; i++)
		{
			s32 start = i * 8 + 64;

			if (frame >= start)
			{
				struct EffectStep *slash = &work->particles[i];
				s32				   x = HI(slash->x);
				s32				   y = HI(slash->y);

				if (frame == i * 8 + 84)
					AudioCommand_PlayFar(212);
				if (frame >= i * 8 + 85)
				{
					slash->x += slash->velocity_x;
					slash->y += slash->velocity_y;
					slash->velocity_x -= 0x10000;
					slash->velocity_y += 0x20000;
					draw[0](canvas, work->sheet + SLASH_TOP, x + 4, y - 40, 16, 21);
					draw[0](canvas, work->sheet + SLASH_MIDDLE, x - 16, y - 19, 29, 35);
					draw[0](canvas, work->sheet + SLASH_BOTTOM, x - 20, y + 16, 21, 24);
				}
				else if (frame >= i * 8 + 80)
				{
					switch (frame - start - 16)
					{
					case 0:
						draw[0](canvas, work->sheet + SLASH_FLASH_1, x - 7, y - 14, 14, 28);
						break;
					case 1:
						draw[0](canvas, work->sheet + SLASH_FLASH_2, x - 11, y - 22, 23, 44);
						break;
					case 2:
						draw[0](canvas, work->sheet + SLASH_FLASH_3_TOP, x - 4, y - 31, 20, 30);
						draw[0](canvas, work->sheet + SLASH_FLASH_3_BOTTOM, x - 16, y - 1, 22, 33);
						break;
					case 3:
						draw[0](canvas, work->sheet + SLASH_FLASH_4_TOP, x + 1, y - 38, 18, 27);
						draw[0](canvas, work->sheet + SLASH_FLASH_4_MIDDLE, x - 11, y - 11, 22, 22);
						draw[0](canvas, work->sheet + SLASH_FLASH_4_BOTTOM, x - 19, y + 11, 19, 28);
						break;
					case 4:
						draw[0](canvas, work->sheet + SLASH_FLASH_5_TOP, x + 4, y - 40, 16, 23);
						draw[0](canvas, work->sheet + SLASH_FLASH_5_MIDDLE, x - 10, y - 17, 23, 28);
						draw[0](canvas, work->sheet + SLASH_FLASH_5_BOTTOM, x - 20, y + 11, 20, 28);
						break;
					}
				}
				else
				{
					for (j = 0; j != 24; j++)
					{
						struct EffectStep *trail = &PARTICLES[i * 24 + j];

						if (trail->x > 0)
						{
							Graphics_LoadTransferWork(MATRICES + (i * 24 + j) * 48);
							point[0] = trail->x;
#if defined(TBS_EDITION_JA)
							{
								/* FAKEMATCH: needed to match: naming the result pointer keeps the point address in a register across this loop */
								struct EffectPosition *out = &screen;

								EffectPosition_ApplyBaseAndYOffset(point, out);
							}
							screen.x = (screen.x >> 1) + x;
							screen.y = screen.y + y;
#else
							EffectPosition_ApplyBaseAndYOffset(point, &screen);
							screen.x = (screen.x >> 1) + x;
							screen.y = screen.y + y + 16;
#endif
							trail->x -= 4;
							draw[1](canvas, sheet + ParticleStreams_CellOffsets[4], screen.x - 2,
								screen.y - 5, 5, 10);
						}
					}
				}
			}
		}
		work->transfer_pending = 1;
		WaitFrames(1);
	}
	Scheduler_RemoveCallback(BattleFx_AdvanceScrollOnInterval);
	presentation->busy = 0;
	gBgScroll[1].x = scroll_x;
	BattleEffect_SetupBlendedDisplay();
	for (i = 0; i != 9; i++)
		ResourceObject_ReleaseFar((struct ResourceObjectWork *)work->objects[i]);
	REG_BG2PA = 0x80;
	REG_DISPCNT = 0x7741;
	Resource_LoadAndDecompress((s32)&ResourceId_EmberStreakSheet, Ram_MapCellBuffer, 1, 0);
	for (i = 0; i != 32; i++)
	{
		struct EffectStep *slash = &work->particles[i];
		s32				   member = i % 6;

		if (member < work->effect->count)
		{
			EffectPosition_ApplyStepAndYOffset(work->effect->actors[member], &target);
			slash->y = -((Random16() & 31) + 40);
			slash->x = target.x / 2 + (80 - slash->y) / 2;
		}
		else
		{
			slash->x = (Random16() & 63) + 80;
			slash->y = -((Random16() & 31) + 40);
		}
		slash->variant = -1;
	}
	for (frame = 0; frame != 88; frame++)
	{
		for (i = 0; i != 24; i++)
		{
			struct EffectStep *slash = &work->particles[i];

			if (frame >= i * 2 || frame > 40)
			{
				if (slash->variant >= 0)
				{
					if (slash->variant <= 23)
					{
						s32 cell = slash->variant / 4;
						u32 width;

						draw[i & 1](canvas, Ram_MapCellBuffer + PuffArc_CellSourceOffsets[cell],
							slash->x - ((width = PuffArc_CellWidths[cell]) >> 1) - 8,
							slash->y + PuffArc_CellBiasY[cell] - 40, width,
							PuffArc_CellHeights[cell]);
						if (slash->variant <= 11)
						{
							draw[0](canvas, work->sheet + SLASH_TOP, slash->x + 4, slash->y - 40,
								16, 21);
							draw[0](canvas, work->sheet + SLASH_MIDDLE, slash->x - 16,
								slash->y - 19, 29, 35);
						}
					}
					slash->variant++;
				}
				else
				{
					s32 height = 24;

					if (slash->y > 56)
						height = height - slash->y + 56;
					draw[0](canvas, work->sheet + SLASH_TOP, slash->x + 4, slash->y - 40, 16, 21);
					draw[0](
						canvas, work->sheet + SLASH_MIDDLE, slash->x - 16, slash->y - 19, 29, 35);
					if (height > 0)
						draw[0](canvas, work->sheet + SLASH_BOTTOM, slash->x - 20, slash->y + 16,
							21, height);
					slash->x -= 6;
					slash->y += 12;
					if (slash->y > 79)
					{
						s32 member;

						slash->variant = 0;
						work->shake_frames = 2;
						AudioCommand_PlayFar(134);
						member = i % 6;
						if (member < work->effect->count)
						{
							ObjectGroup_UpdateMembers(
								work->effect->actors[member], 7, 5, member, 8);
							BattleMotion_ApplyVariantMotionFar(work->effect->actors[member], 1);
						}
					}
				}
			}
		}
		Camera_ApplyShake(4, 8);
		ObjectGroup_TickMemberTimers();
		work->transfer_pending = 1;
		WaitFrames(1);
	}
	Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
	BattleFx_EndCanvasLayer();
}

//--------------------------------------------------------------------------
// columns.c
// バトル演出：地面から次々に伸びるつる・柱・とげと土けむり
//--------------------------------------------------------------------------

// 伸びる 1 本（struct EffectStep に重ねて使う）
struct DualColumn
{
	s32 x; // 根もと（キャンバスのドット）
	s32 y;
	u8	unknown_08[8];
	s32 height; // 伸びきったときの長さ
	u8	unknown_14[4];
	s32 start; // 伸び始めるフレーム
};

// PARTICLES : とげの土けむり

void BattleFx_RunDualTableMode0(struct BattleEffectArgument *effect)
{
	BattleFx_RunDualTable(effect, 0);
}

void BattleFx_RunDualTableMode1(struct BattleEffectArgument *effect)
{
	BattleFx_RunDualTable(effect, 1);
}

//--------------------------------------------------------------------------
// つる（mode 0）か緑の柱（mode 1）が、左右交互に 4 フレームおきに伸びる
// 本数は DualTable_ColumnCounts[variant]、根もとから土けむりが上がる
//--------------------------------------------------------------------------
void BattleFx_RunDualTable(struct BattleEffectArgument *effect, s32 mode)
{
	void				   **heap;
	void				   **cursor;
	struct BattleEffectWork *work;
	void					*canvas;
	void					*rectangle[2];
	void					*second_slot;

	s32	   frame;
	s32	   duration;
	void **draw;
	s32	   cue_frame;
	s32	   fade_start;
	s32	   fade_end;
	s32	   start;

	u8 *age;

	struct DualColumn *seed;
	s32				   x;
	s32				   height;
	s32				   limit;

	s32				   puff_cells;
	struct DualColumn *column;
	s32				   length;
	s32				   reach;
	s8				   pillar_height;

	s32 n;
	s32 react_frame;

	s32				   i;
	struct EffectStep *puff;
	s32				   puff_age;
	s32				   image;

	heap = (void **)gBattleFxWork;
	cursor = heap;
	work = *cursor++;
	canvas = *cursor;
	work->effect = effect;
	BattleFx_BeginCanvasLayer(1);
	REG_BG2PA = 0x100; // 横も等倍
	REG_BLDCNT = 0;
	if (mode == 1)
	{
		Resource_LoadAndDecompress((s32)&ResourceId_GreenPillarSheet, work->sheet, 1, 1);
	}
	else
	{
		Resource_LoadAndDecompress((s32)&ResourceId_VineSheet, work->sheet, 1, 1);
	}
	if (work->effect->side == 1)
	{
		REG_BG2X = -0x7000; // 112 ドット右へ
	}
	BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, GOUSEI_CLIP_X | GOUSEI_CLIP_Y, GOUSEI_NURI);
	rectangle[0] = gWorkSlot[HEAP_SLOT_BLITTER];
	BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7,
		GOUSEI_CLIP_X | GOUSEI_CLIP_Y | GOUSEI_FLIP_X, GOUSEI_NURI);
	second_slot = gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE];
	/* FAKEMATCH: needed to match: the columns draw through a second pointer to the pair, whose address the frame keeps in a stack slot 2026-10-11: rectangle[side] at the columns differs 22 (the address slot of the pair moves from sp+28 to sp+16). */
	draw = rectangle;
	draw[1] = second_slot;

	duration = DualTable_ColumnCounts[work->effect->variant] * 4 + 56;

	// 土けむりをすべて空きにする
	age = (u8 *)gMapCellBuffer + 0x18;
	i = 0;
	do
	{
		i++;
		*(s32 *)age = -1;
		age += sizeof(struct EffectStep);
	} while (i != 1024);

	// 16 本の根もとと長さ
	i = 0;
	seed = (struct DualColumn *)work->particles;
	do
	{
		x = (DualTable_ColumnX[i] + (7 & Random16())) - 4;
		seed->y = i / 2 + 108;
		seed->x = x;
		height = (63 & Random16()) + 55;
		seed->height = height;
		limit = DualTable_HeightLimits[i % 3];
		if (limit < height)
		{
			seed->height = limit;
		}
		limit = i * 4 + 8;
		seed->start = limit;
		i++;
		seed++;
	} while (i != 16);

	work->transfer_mode = 1; // 毎フレーム 0 で消す
	work->transfer_value = 0;
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);

	frame = 0;
	if (duration != 0)
	{
		cue_frame = duration - 64;
		fade_start = duration - 20;
		fade_end = duration - 4;
		do
		{
			if (frame == cue_frame)
			{
				BattleEventRuntime_BeginPhaseFar(132);
			}
			if (frame >= fade_start && frame < fade_end)
			{
				// 終わりの 16 フレームで薄くする
				REG_BLDCNT = 0x3f44;
				REG_BLDALPHA = ((duration - frame) - 5) | 0x1000;
			}
			if (frame < fade_end)
			{
				i = 0;
				if (DualTable_ColumnCounts[work->effect->variant] != 0)
				{
					do
					{
						column = (struct DualColumn *)((u8 *)work->particles
													   + i * sizeof(struct DualColumn));
						start = i * 4 + 8;
						if (frame == i * 4 + 9)
						{
							work->shake_frames = 2;
						}

						// 1 フレームに 8 ドットずつ伸ばす（奇数番は左右反転）
						if (frame > start)
						{
							u32 shape = i % 3;

							length = (frame - start) * 8;
							reach = column->height;
							if (length > reach)
							{
								length = reach;
							}
							if (mode == 0)
							{
								{
									s32 side = 1 & i;
									s32 cell = DualTable_VineCells[shape];
									shape = DualTable_VineWidths[shape];
									((DrawRectangle)draw[side])(canvas, work->sheet + cell,
										column->x - (shape >> 1), column->y - length, shape,
										length);
								}
							}
							else
							{
								pillar_height = DualTable_PillarHeights[7 & i];
								if (length > pillar_height)
								{
									length = pillar_height;
								}
								{
									s32 side = 1 & i;
									s32 cell = DualTable_PillarCells[shape];
									shape = DualTable_PillarWidths[shape];
									((DrawRectangle)draw[side])(canvas, work->sheet + cell,
										column->x - (shape >> 1), column->y - length, shape,
										length);
								}
							}
						}

						// 伸び始めて 4 フレーム目に対象が反応する
						n = 0;
						if (work->effect->count != 0)
						{
							react_frame = start + 4;
							do
							{
								if (frame == react_frame)
								{
									if (!(i & 1))
									{
										AudioCommand_PlayFar(133);
									}
									ObjectGroup_UpdateMembers(work->effect->actors[n], 7, 5, n, 3);
								}
								n++;
							} while (n != work->effect->count);
						}
						else
						{
							react_frame = start + 4;
						}

						// 4・8 フレーム目に、空きの土けむりをひとつ根もとに出す
						if ((frame == react_frame) || (frame == (start + 8)))
						{
							/* FAKEMATCH: needed to match: the scan reuses length as its puff pointer 2026-10-11: a block pointer of its own differs 48 (r5 and r6 swap); the puff pointer of the function 271. */
							length = (u32)gMapCellBuffer;
							n = 0;
							while (n != 512)
							{
								if (((struct EffectStep *)length)->variant == -1)
								{
									((struct EffectStep *)length)->x =
										((Random16() & 0xf) + column->x) - 8;
									((struct EffectStep *)length)->y = (Random16() & 0xf) + 80;
									((struct EffectStep *)length)->variant = 0;
									break;
								}
								length += sizeof(struct EffectStep);
								n++;
							}
						}
						i++;
					} while (i != DualTable_ColumnCounts[work->effect->variant]);
				}
			}

			// 土けむり：7 コマを 2 フレームずつ、左右対称に描く
			i = 0;
			puff = gMapCellBuffer;
			do
			{
				puff_age = puff->variant;
				if (puff_age >= 0)
				{
					image = puff_age / 2;
					puff_cells = 0x1e59;
					if (mode != 0)
					{
						puff_cells = 0xaff;
					}
					((DrawRectangle)rectangle[0])(canvas,
						work->sheet + (DualTable_PuffCells[image] + puff_cells),
						puff->x - DualTable_PuffWidths[image],
						puff->y - (s8)DualTable_PuffHeights[image] / 2, DualTable_PuffWidths[image],
						(s8)DualTable_PuffHeights[image]);
					((DrawRectangle)rectangle[1])(canvas,
						work->sheet + (DualTable_PuffCells[image] + puff_cells), puff->x,
						puff->y - (s8)DualTable_PuffHeights[image] / 2, DualTable_PuffWidths[image],
						(s8)DualTable_PuffHeights[image]);
					puff->variant++;
					if (puff->variant == 14)
					{
						puff->variant = -1;
					}
				}
				puff++;
				i++;
			} while (i != 512);

			Camera_ApplyShake(4, 4);
			ObjectGroup_TickMemberTimers();
			work->transfer_pending = 1;
			WaitFrames(1);
			frame++;
		} while (frame != duration);
	}
	Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
	BattleFx_EndCanvasLayer();
}

//--------------------------------------------------------------------------
// 対象の列のまん中から、緑のつるが 16 本、2 フレームおきに伸びて縮む
//--------------------------------------------------------------------------
void BattleEffect_RunRisingColumns(struct BattleEffectArgument *effect)
{
	u32						*cache, *entry;
	struct BattleEffectWork *work;
	void					*canvas;
	DrawRectangle			 draw[2];
	s32						 origin_x, origin_y;
	struct EffectPosition	 first, last;
	s32						 i, frame, cell, height, ofs, middle;
	struct EffectStep		*column;

	cache = (u32 *)(gWorkSlot + HEAP_SLOT_BATTLE_EFFECT);
	entry = cache;
	work = (struct BattleEffectWork *)*entry++;
	canvas = (void *)*entry;
	work->effect = effect;
	BattleFx_PrepareCanvasEffect(effect, 4, effect->side, 4, &origin_x, &origin_y);
	BattleFx_BeginCanvasLayer(1);
	REG_BG2PA = 0x100; // 横も等倍
	REG_BLDCNT = 0;
	Resource_LoadAndDecompress((s32)&ResourceId_GreenVineSheet, work->sheet, 1, 1);

	// 最初と最後の対象のまん中を、キャンバスの x = 64 に合わせる
	EffectPosition_ApplyStepAndYOffset(work->effect->actors[0], &first);
	EffectPosition_ApplyStepAndYOffset(work->effect->actors[work->effect->count - 1], &last);
	middle = first.x;
	middle += (last.x - middle) / 2;
	first.x = middle;
	REG_BG2X = (64 - first.x) << 8;
	BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, GOUSEI_CLIP_X | GOUSEI_CLIP_Y, GOUSEI_NURI);
	draw[0] = (DrawRectangle)cache[HEAP_SLOT_BLITTER - HEAP_SLOT_BATTLE_EFFECT];
	BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7,
		GOUSEI_CLIP_X | GOUSEI_CLIP_Y | GOUSEI_FLIP_X, GOUSEI_NURI);
	draw[1] = (DrawRectangle)cache[HEAP_SLOT_BLITTER_ALTERNATE - HEAP_SLOT_BATTLE_EFFECT];
	column = work->particles;
	i = 0;
	do
	{
		column[i].x = RisingColumns_ColumnOffsets[i] + 64;
		i++;
	} while (i != 16);
	work->transfer_mode = 1; // 毎フレーム 0 で消す
	work->transfer_value = 0;
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
	frame = 0;
	do
	{
		if (frame == 32)
		{
			AudioCommand_PlayFar(143);
			for (i = 0; i != work->effect->count; i++)
				ObjectGroup_UpdateMembers(work->effect->actors[i], 7, 5, i, 16);
		}
		i = 0;
		column = work->particles;
		do
		{
			if (frame == i * 4 + 5)
				work->shake_frames = 2;

			// 1 フレームに 4 ドット伸びて 32 で止まり、i * 2 + 32 フレーム目から縮む
			ofs = i * 2;
			if (frame > ofs + 4)
			{
				cell = (frame / 4 + i) % 5; // 32x32 を 5 コマ
				if (frame < ofs + 32)
				{
					height = (frame - ofs) * 4 - 16;
					if (height > 32)
						height = 32;
				}
				else
				{
					height = 160 - (frame - ofs) * 4;
				}
				if (height > 0)
					draw[i & 1](canvas, work->sheet + (cell << 10), column->x - 16,
						(i & 7) - height + 104, 32, height);
			}
			i++;
			column++;
		} while (i != 16);
		Camera_ApplyShake(4, 4);
		ObjectGroup_TickMemberTimers();
		work->transfer_pending = 1;
		WaitFrames(1);
		frame++;
	} while (frame != 70);
	Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
	BattleFx_EndCanvasLayer();
}

//--------------------------------------------------------------------------
// とげが 8 フレームおきに地面から突き出し、根もとから土けむりが上がる
// 本数は BattleFxPillar_Counts[variant]、向きと大きさは BattleFxPillar_Kinds
//--------------------------------------------------------------------------
void BattleFx_RaisePillars(struct BattleEffectArgument *effect)
{
	void					*canvas;
	s32						 frame;
	DrawRectangle			 draw_flip;
	DrawRectangle			 draw;
	s32						 total;
	u32						*cache;
	u32						*cursor;
	struct BattleEffectWork *work;
	s32						 i;
	s32						 j;
	s32						 age;
	s32						 start;

	cache = (u32 *)(gWorkSlot + HEAP_SLOT_BATTLE_EFFECT);
	cursor = cache;
	work = (struct BattleEffectWork *)*cursor++;
	canvas = (void *)*cursor;
	work->effect = effect;
	BattleFx_BeginCanvasLayer(1);
	REG_BG2PA = 0x100; // 横も等倍
	REG_BLDCNT = 0;
	REG_BLDALPHA = 0x1010;
	Resource_LoadAndDecompress((s32)&ResourceId_ThornSheet, work->sheet, 1, 1);
	if (work->effect->side == 1)
	{
		REG_BG2X = -0x7000; // 112 ドット右へ
	}
	BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, GOUSEI_CLIP_X | GOUSEI_CLIP_Y, GOUSEI_NURI);
	draw = ((DrawRectangle *)cache)[HEAP_SLOT_BLITTER - HEAP_SLOT_BATTLE_EFFECT];
	BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7,
		GOUSEI_CLIP_X | GOUSEI_CLIP_Y | GOUSEI_FLIP_X, GOUSEI_NURI);
	work->transfer_mode = 1; // 毎フレーム 0 で消す
	work->transfer_value = 0;
	draw_flip = ((DrawRectangle *)cache)[HEAP_SLOT_BLITTER_ALTERNATE - HEAP_SLOT_BATTLE_EFFECT];
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
	total = BattleFxPillar_Counts[work->effect->variant] * 8 + 56;

	for (i = 0; i != 1024; i++)
	{
		PARTICLES[i].variant = -1; // 空き
	}

	for (frame = 0; frame != total; frame++)
	{
		if (frame == total - 64)
		{
			BattleEventRuntime_BeginPhaseFar(132);
		}
		if (frame >= total - 16)
		{
			// 終わりの 16 フレームで薄くする
			REG_BLDCNT = 0x3f44;
			REG_BLDALPHA = (total - frame - 1) | 0x1000;
		}

		for (i = 0; i != BattleFxPillar_Counts[work->effect->variant]; i++)
		{
			start = i * 8 + 8;
			age = frame - (i * 8 + 8);
			if (frame > start)
			{
				s32 h;
				s32 w;

				// 種類 0・1 は大きいとげ（48 幅）、2・3 は小さいとげ（32 幅）
				// 奇数は左右反転で、伸びるにつれて横へずれる
				if (BattleFxPillar_Kinds[i] < 2)
				{
					h = age * 16;
					w = age * 6;
					if (h > 80)
					{
						h = 80;
					}
					if (w > 30)
					{
						w = 30;
					}
					if (BattleFxPillar_Kinds[i] & 1)
					{
						draw_flip(canvas, work->sheet, BattleFxPillar_X[i] - w, 108 - h, 48, h);
					}
					else
					{
						draw(canvas, work->sheet, BattleFxPillar_X[i] + w, 108 - h, 48, h);
					}
				}
				else
				{
					h = age * 8;
					w = age;
					if (h > 64)
					{
						h = 64;
					}
					if (age > 8)
					{
						w = 8;
					}
					if (BattleFxPillar_Kinds[i] & 1)
					{
						draw_flip(
							canvas, work->sheet + 0xf00, BattleFxPillar_X[i] - w, 108 - h, 32, h);
					}
					else
					{
						draw(canvas, work->sheet + 0xf00, BattleFxPillar_X[i] + w, 108 - h, 32, h);
					}
				}
				if (frame == start + 1)
				{
					work->shake_frames = 3;
				}

				// 突き出して 2 フレームの間、毎フレーム土けむりをひとつ出す
				if (frame < start + 3)
				{
					s32				   y = (Random16() & 31) + 72;
					struct EffectStep *p;
					for (j = 0; j != 64; j++)
					{
						p = &PARTICLES[j];
						if (p->variant == -1)
						{
							p->x = BattleFxPillar_X[i] + (Random16() & 31) + 32;
							if (p->x > 96)
							{
								p->x = 96;
							}
							p->y = y;
							p->variant = 0;
							break;
						}
					}
				}
			}
			for (j = 0; j != work->effect->count; j++)
			{
				if (frame == start + 4)
				{
					AudioCommand_PlayFar(132);
					ObjectGroup_UpdateMembers(work->effect->actors[j], 7, 5, j, 3);
				}
			}
		}

		// 土けむり：7 コマを 2 フレームずつ、左右対称に描く
		for (i = 0; i != 64; i++)
		{
			if (PARTICLES[i].variant >= 0)
			{
				s32 n = PARTICLES[i].variant / 2;
				draw(canvas, work->sheet + BattleFxPillar_PuffCells[n],
					PARTICLES[i].x - BattleFxPillar_PuffWidths[n],
					PARTICLES[i].y - (s8)BattleFxPillar_PuffHeights[n] / 2,
					BattleFxPillar_PuffWidths[n], (s8)BattleFxPillar_PuffHeights[n]);
				draw_flip(canvas, work->sheet + BattleFxPillar_PuffCells[n], PARTICLES[i].x,
					PARTICLES[i].y - (s8)BattleFxPillar_PuffHeights[n] / 2,
					BattleFxPillar_PuffWidths[n], (s8)BattleFxPillar_PuffHeights[n]);
				if (++PARTICLES[i].variant == 14)
				{
					PARTICLES[i].variant = -1;
				}
			}
		}
		Camera_ApplyShake(8, 8);
		ObjectGroup_TickMemberTimers();
		work->transfer_pending = 1;
		WaitFrames(1);
	}

	Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
	BattleFx_EndCanvasLayer();
}

//--------------------------------------------------------------------------
// lightning_bolts.c
// 戦闘エフェクト：対象ごとに落ちる稲妻と、火花・光
//--------------------------------------------------------------------------

// PARTICLES : 対象ひとりに 128 個
// 光ごとに作りなおす 2 つめの描画関数
#define BLIT_ALT \
	((DrawRectangle *)gBattleFxWork)[HEAP_SLOT_BLITTER_ALTERNATE - HEAP_SLOT_BATTLE_EFFECT]

//--------------------------------------------------------------------------
// 対象ごとに 8 フレームずつずらして稲妻を落とす
// effect : variant が稲妻の本数・火花の数・光の数と広がりを決める
//--------------------------------------------------------------------------
void BattleFx_RunLightningBolts(struct BattleEffectArgument *effect)
{
	void				   **heap;
	void				   **cursor;
	struct BattleEffectWork *work;
	void					*canvas;
	s32						 i;
	s32						 frame;
	DrawRectangle			 draw[2];
	u8						*sprites;
	struct EffectPosition	 pos;
	s32						 j;

	heap = (void **)gBattleFxWork;
	cursor = heap;
	work = *cursor++;
	canvas = *cursor;
	sprites = heap[HEAP_SLOT_BATTLE_SHEET - HEAP_SLOT_BATTLE_EFFECT];
	work->effect = effect;
	BattleFx_BeginCanvasLayer(1);
	BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, GOUSEI_CLIP_X | GOUSEI_CLIP_Y, GOUSEI_HIKAKU);
	draw[0] = heap[HEAP_SLOT_BLITTER - HEAP_SLOT_BATTLE_EFFECT];
	Resource_LoadAndDecompress((s32)&ResourceId_TornadoSheet, work->sheet, 1, 0);
	Resource_LoadAndDecompress((s32)&ResourceId_LightningBoltSheet, work->sheet + 0xc56, 1, 1);
	Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sprites, 0, 0);
	for (j = 0; j != 1024; j++)
		PARTICLES[j].variant = 0;
	for (j = 0; j != 64; j++)
		work->particles[j].variant = -1;
	work->transfer_mode = 2;
	work->transfer_value = 75;
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
	AudioCommand_PlayFar(138);
	for (frame = 0; frame != work->effect->count * 8 + 40; frame++)
	{
		if (frame == 24)
			BattleEventRuntime_BeginPhaseFar(133);
		for (i = 0; i != work->effect->count; i++)
		{
			if (frame == i * 8)
				Iwram_FillWords(canvas, 0x4000, 0x10101010);
		}
		for (i = 0; i != work->effect->count; i++)
		{
			s32 start = i * 8;

			EffectPosition_ApplyAlternateStepAndYOffset(work->effect->actors[i], &pos);
			pos.x /= 2;
			if (frame == start + 1)
				work->shake_frames = 4;
			if (frame == start + 4)
			{
				ObjectGroup_UpdateMembers(work->effect->actors[i], 7, 5, i, 6);
				BattleMotion_ApplyVariantMotionFar(work->effect->actors[i], 6);
			}

			// 稲妻 (幅 24) : 1 フレームに 64 ドット伸び、104 ドットで止まる
			if (frame >= start && frame < start + 16)
			{
				s32 height = (frame - start) << 6;

				if (height > 104)
					height = 104;
				for (j = 0; j != LightningBolts_Counts[work->effect->variant * 4 + 3]; j++)
				{
					draw[0](canvas, work->sheet + (((i + frame + j) / 2) & 3) * 2880 + 0xc56,
						pos.x - 12, 0, 24, height);
				}

				// 足もと (y = 104) から上向き ±90 度へ火花を散らす
				if (frame == start + 2)
				{
					for (j = 0; j != LightningBolts_Counts[work->effect->variant * 4]; j++)
					{
						struct EffectStep *spark = &PARTICLES[i * 128 + j];
						s32				   speed = (Random16() & 0x1ff) + 64;
						s32				   angle = (Random16() & 0x7fff) - 0x4000;

						spark->x = pos.x << 16;
						spark->y = 104 << 16;
						spark->velocity_x = (Trig_Sin(angle) * speed) >> 5;
						spark->velocity_y = -(Trig_Cos(angle) * speed) >> 6;
						spark->variant = (Random16() & 7) + 32;
					}
				}
			}

			// 光 : 足もとから上のでたらめな位置に、でたらめな向き (左右・上下反転) で描く
			if (frame >= start + 2 && frame < start + 24)
			{
				for (j = 0; j != LightningBolts_Counts[work->effect->variant * 4 + 1]; j++)
				{
					s32 kind = j & 3;
					s32 drop = Random16() % LightningBolts_Counts[work->effect->variant * 4 + 2];
					s32 spread = LightningBolts_Counts[work->effect->variant * 4 + 2] - drop + 1;
					s32 y = pos.y - drop - (BattleFx_GlintCellHeights[kind] >> 1) + 8;
					u32 r = Random16();
					s32 x = pos.x;

					x += r % spread;
					x -= spread / 2;
					x -= (BattleFx_GlintCellWidths[kind] >> 1);

					BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7,
						LightningBolts_GlintPalettes[Random16() & 3] | GOUSEI_CLIP_X
							| GOUSEI_CLIP_Y,
						LightningBolts_GlintModes[work->effect->variant]);
					BLIT_ALT(canvas, work->sheet + BattleFx_GlintCellOffsets[kind], x, y,
						BattleFx_GlintCellWidths[kind], BattleFx_GlintCellHeights[kind]);
					Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
				}
			}
		}

		// 火花 : 地面で半分の速さで跳ね返り、残りが少ないほど小さく描く
		for (j = 0; j != 1024; j++)
		{
			struct EffectStep *spark = &PARTICLES[j];

			if (spark->variant > 0)
			{
				spark->variant--;
				EffectStep_AdvanceWithGravity2D(spark, 60, 0x1000);
				if (spark->y > 104 << 16)
				{
					spark->velocity_y = -spark->velocity_y / 2;
				}
				else if ((spark->x >= 0 && spark->x < 127 << 16) && spark->y >= 0)
				{
					s32 size = spark->variant / 16 + 1;
					s32 x = spark->x >> 16;
					s32 y = spark->y >> 16;

					draw[0](canvas, sprites + ParticleStreams_CellOffsets[size - 1], x - size / 2,
						y - size, size, size * 2);
				}
			}
		}
		Camera_ApplyShake(2, 8);
		ObjectGroup_TickMemberTimers();
		work->transfer_pending = 1;
		WaitFrames(1);
	}
	Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
	BattleFx_EndCanvasLayer();
}

void BattleFx_PrepareCanvasEffect(
	struct BattleEffectArgument *effect, s32 kind, s32 side, s32 anchor, s32 *out_x, s32 *out_y)
{
	s32						 base[3];
	s32						 goal[3];
	s32						 step[3];
	s32						 point[3];
	struct EffectPosition	 position;
	void				   **heap_cache;
	void				   **cursor;
	struct BattleEffectWork *work;
	void					*canvas;
	DrawRectangle			 draw[2];
	u8						*sheet;
	s32						 total;
	struct MotionObject		*target;
	struct MotionObject		*actor;
	u8						*palette;
	s32						 resource;
	s32						 frame;
	s32						 i;

	heap_cache = (void **)gBattleFxWork;
	cursor = heap_cache;
	work = *cursor++;
	canvas = *cursor;
	sheet = heap_cache[2];
	work->effect = effect;
	BattleFx_BeginCanvasLayer(0);
	work->fade_frames = 24;
	work->fade_step = 0;
	if (kind > 3)
	{
		kind -= 4;
		total = 84;
	}
	else
	{
		total = 64;
	}
	switch (kind)
	{
	case 0:
		resource = (s32)&ResourceId_VenusDjinnSmallSheet;
		break;
	case 1:
		resource = (s32)&ResourceId_MercuryDjinnSmallSheet;
		break;
	case 2:
		resource = (s32)&ResourceId_MarsDjinnSmallSheet;
		break;
	default:
		resource = (s32)&ResourceId_JupiterDjinnSmallSheet;
		break;
	}
	palette = Resource_GetTableEntry(resource);
	Iwram_CopyWords((void *)0x05000000, palette, 128);
	Resource_DecodeType01(palette + 128, work);
	Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);
	if (side == 1)
	{
		BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 7, 3);
		BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, 7, 2);
	}
	else
	{
		BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 3, 3);
		BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, 3, 2);
	}
	draw[0] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER];
	draw[1] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE];
	actor = GetBattleObjectSlotFar(work->effect->actor)->object;
	target = GetBattleObjectSlotFar(work->effect->actors[0])->object;
	for (i = 0; i != 64; i++)
	{
		struct EffectStep *spark = &PARTICLES[i];
		s32				   angle = Random16() & 0xffff;
		s32				   speed = (Random16() & 255) + 128;

		spark->x = 0;
		spark->y = ((Random16() & 31) + 20) << 16;
		spark->z = 0;
		spark->velocity_x = (Trig_Sin(angle) * speed) >> 5;
		spark->velocity_y = 0;
		spark->velocity_z = (Trig_Cos(angle) * speed) >> 5;
		spark->variant = 0;
	}
	work->transfer_mode = 2;
	work->transfer_value = 75;
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
	base[0] = actor->x;
	base[1] = 0;
	base[2] = actor->z;
	switch (anchor)
	{
	case 0:
		goal[0] = target->x;
		goal[1] = 0x3c0000;
		goal[2] = target->z;
		break;
	case 1:
		goal[0] = target->x;
		goal[1] = 0x3c0000;
		goal[2] = 0;
		break;
	case 2:
		goal[0] = actor->x;
		goal[1] = 0x3c0000;
		goal[2] = actor->z;
		break;
	case 3:
		goal[0] = actor->x;
		goal[1] = 0x3c0000;
		goal[2] = 0;
		break;
	case 4:
		goal[0] = 0;
		goal[1] = 0x3c0000;
		goal[2] = 0;
		break;
	}
	step[0] = (goal[0] - base[0]) / 40;
	step[1] = (goal[1] - base[1]) / 40;
	step[2] = (goal[2] - base[2]) / 40;
	for (frame = 0; frame != total; frame++)
	{
		struct BattleCamera *camera = gCameraWork;

		if (frame > 75)
			REG_BLDALPHA = (168 - frame * 2) | 0x1000;
		if (frame == 8)
			AudioCommand_PlayFar(212);
		Render_ResetTransformState();
		Graphics_PrepareTransferInIwramWork(camera->eye, camera->pos);
		if (frame >= 6 && frame <= 45)
		{
			base[0] += step[0];
			base[1] += step[1];
			base[2] += step[2];
		}
		SceneTransform_ApplyPosition(base);
		if (frame == 0)
			ObjectGroup_UpdateMembers(work->effect->actor, 7, -1, -1, 0);
		if (frame == 24)
			ObjectGroup_UpdateMembers(work->effect->actor, 0, -1, -1, 0);
		for (i = 0; i != 32; i++)
		{
			struct EffectStep *spark = &PARTICLES[i];

			if (frame >= i / 8 && spark->variant == 0)
			{
				s32 size;

				Graphics_SaveTransferWorkOnce();
				switch (i & 3)
				{
				case 0:
					SceneTransform_ApplyYaw(frame * (i * 32 + 256));
					break;
				case 1:
					SceneTransform_ApplyPitch(-frame * (i * 32 + 256));
					break;
				case 2:
					SceneTransform_ApplyRoll(-frame * (i * 32 + 256));
					break;
				case 3:
					SceneTransform_ApplyPitch(-frame * (i * 32 + 256));
					SceneTransform_ApplyRoll(-frame * (i * 32 + 256));
					break;
				}
				EffectPosition_ApplyBaseAndYOffset(&spark->x, &position);
				position.x >>= 1;
				Graphics_RestoreTransferWork();
				if (position.depth < 250)
					position.depth = 250;
				if (position.depth > 634)
					position.depth = 634;
				size = 8 - (position.depth - 250) / 64;
				draw[1](canvas, sheet + ParticleStreams_CellOffsets[size - 1],
					position.x - size / 2, position.y - size, size, size * 2);
				EffectStep_AdvanceWithGravity3D(spark, 60, 0);
				if (frame >= i / 8 + 24)
				{
					s32 pull_x = -spark->x >> 7;
					s32 pull_y = -spark->y >> 7;
					s32 pull_z = -spark->z >> 7;

					spark->velocity_x += pull_x;
					spark->velocity_y += pull_y;
					spark->velocity_z += pull_z;
					spark->velocity_x = spark->velocity_x * 62 / 64;
					spark->velocity_y = spark->velocity_y * 62 / 64;
					spark->velocity_z = spark->velocity_z * 62 / 64;
					if ((pull_x > -2048 && pull_x < 2048) && (pull_z > -2048 && pull_z < 2048))
						spark->variant = -1;
				}
			}
		}
		if (frame >= 54 && frame <= 69)
		{
			point[0] = Trig_Sin(frame << 10) << 2;
			point[1] = 0;
			point[2] = 0;
			EffectPosition_ApplyBaseAndYOffset(point, &position);
			*out_x = position.x;
			*out_y = position.y;
			position.x >>= 1;
			draw[0](canvas, work, position.x - 10, position.y - 20, 20, 40);
		}
		if (frame == 64)
		{
			for (i = 0; i != 64; i++)
			{
				struct EffectStep *mote = &work->particles[i];
				s32				   angle = Random16() & 0xffff;
				s32				   speed = (Random16() & 255) + 128;

				mote->x = *out_x << 15;
				mote->y = *out_y << 16;
				mote->velocity_x = (Trig_Sin(angle) * speed) >> 6;
				mote->velocity_y = (Trig_Cos(angle) * speed) >> 5;
				mote->variant = (Random16() & 15) + 8;
			}
		}
		if (frame > 63)
		{
			for (i = 0; i != 64; i++)
			{
				struct EffectStep *mote = &work->particles[i];

				if (mote->variant >= 0)
				{
					s32 size = (mote->variant >> 3) + 2;

					draw[0](canvas, sheet + ParticleStreams_CellOffsets[size - 1],
						HI(mote->x) - size / 2, HI(mote->y) - size, size, size * 2);
					EffectStep_AdvanceWithGravity2D(mote, 60, 0);
					mote->variant--;
				}
			}
		}
		work->transfer_pending = 1;
		WaitFrames(1);
	}
	Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
	Scheduler_RemoveCallback(Palette_StepFadeTransfer);
	Iwram_ClearWords((void *)0x06004000, 0x4000);
	Iwram_ClearWords(canvas, 0x4000);
	REG_BLDALPHA = 0x1010;
}

//--------------------------------------------------------------------------

// projectile_volley.c

// バトル効果：BattleFx_RunProjectileVolley の型ごとの入り口

//--------------------------------------------------------------------------

#define ARG(e) ((struct BattleEffectArgument *)(e))

#define STEP_HAYAI 0x30 // 絵のアニメの step（ふだんは 0x10）
#define STEP_FUTSU 0x10

void BattleFx_RunMode0(void *effect)
{
	BattleFx_RunProjectileVolley(effect, 0);
}

void BattleFx_RunMode1(void *effect)
{
	BattleFx_RunProjectileVolley(effect, 1);
}

void BattleFx_RunMode2(void *effect)
{
	BattleFx_RunProjectileVolley(effect, 2);
}

void BattleFx_RunMode3(void *effect)
{
	BattleFx_RunProjectileVolley(effect, 3);
}

void BattleFx_RunMode11(void *effect)
{
	BattleFx_RunProjectileVolley(effect, 11);
}

void BattleFx_RunMode4(void *effect)
{
	BattleFx_RunProjectileVolley(effect, 4);
}

//--------------------------------------------------------------------------
// 使う者を mode 2 にし、アニメを速めているあいだに型 mode の弾を放つ
//--------------------------------------------------------------------------
static __inline__ void RunVolleyWithAction(void *effect, s32 mode)
{
	s32 obj;

	obj = (s32)GetBattleObjectSlotFar(ARG(effect)->actor)->object;
	ObjectDispatch_ApplyArgumentToChildrenFar(obj, 2);
	ObjectDispatch_ApplyValueToChildrenFar(obj, STEP_HAYAI);
	BattleFx_RunProjectileVolley(effect, mode);
	ObjectDispatch_ApplyValueToChildrenFar(obj, STEP_FUTSU);
}

//--------------------------------------------------------------------------
// 型 5 : 使う者を mode 2 にし、アニメを速めているあいだに放つ
//--------------------------------------------------------------------------
void BattleEffect_RunMode5WithAction(void *effect)
{
	RunVolleyWithAction(effect, 5);
}

//--------------------------------------------------------------------------
// 型 6 : 型 5 と同じく、アニメを速めて放つ
//--------------------------------------------------------------------------
void BattleFx_RunMode6WithAction(void *effect)
{
	RunVolleyWithAction(effect, 6);
}

void BattleFx_RunMode7(void *effect)
{
	BattleFx_RunProjectileVolley(effect, 7);
}

void BattleFx_RunMode10(void *effect)
{
	BattleFx_RunProjectileVolley(effect, 10);
}

//--------------------------------------------------------------------------
// 型 9 : 型 5 と同じく、アニメを速めて放つ
//--------------------------------------------------------------------------
void BattleFx_RunMode9WithAction(void *effect)
{
	RunVolleyWithAction(effect, 9);
}

//--------------------------------------------------------------------------
// approach.c
// バトル効果：対象へ飛ぶ弾の一斉射撃と、キャラの移動
//--------------------------------------------------------------------------

// mode ごとの表
extern u8 Volley_CometHeights[];
extern u8
	Volley_Modes[]; // 5 バイトずつ：弾の数、届くまでのフレーム、弾の間隔、対象の間隔、火花の数
extern u8  Volley_FlashWidths[];
extern u8  Volley_FlashHeights[];
extern u8  Volley_FlashSteps[];
extern u8  Volley_FlashRises[];
extern u16 Volley_FlashOffsets[];
extern u16 Volley_PelletOffsets[];

// 粒の表：0- 当たりの光、100- 火花、512- 火の玉の尾
#define PTCL_MAX 1024

//--------------------------------------------------------------------------
// 使う者から対象へ弾を飛ばし、当たったら光と火花を出す
// mode : 弾の絵と飛び方 (0-11)
//--------------------------------------------------------------------------
void BattleFx_RunProjectileVolley(struct BattleEffectArgument *effect, s32 mode)
{
	struct EffectPosition	 position;
	s32						 screen_x;
	s32						 screen_y;
	void				   **heap;
	void				   **cursor;
	struct BattleEffectWork *work;
	void					*canvas;
	s32						 i;
	s32						 j;
	s32						 frame;
	struct BlitterPair		 draw;
	struct BattleCamera		*camera;
	u8						*sheet;
	s32						 se_wait;
	s32						 total;
	s32						 kind;
	s32						 shots;
	s32						 steps;
	struct MotionObject		*user;
	s32						 k;
	s32						 shift;
	s32						 hit;
	s32						 x;

	// ワーク、キャンバス、カメラ
	heap = (void **)gBattleFxWork;
	cursor = heap;
	work = *cursor++;
	canvas = *cursor;
	camera =
		*(struct BattleCamera **)((u8 *)heap
								  - (HEAP_SLOT_BATTLE_EFFECT - HEAP_SLOT_CAMERA) * sizeof(void *));
	sheet = heap[2];
	se_wait = 0;
	work->effect = effect;
	kind = effect->variant;
	if (mode == 10)
		BattleFx_BeginCanvasLayer(0);
	else
		BattleFx_BeginCanvasLayer(1);
	if (work->effect->prelude == 1)
		BattleFx_PrepareCanvasEffect(effect, 1, work->effect->side, 2, &screen_x, &screen_y);

	// 128x128 の合成。向こう側なら左右を反転する
	if (mode == 5)
	{
		if (work->effect->side == 1)
			BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, CLIP | GOUSEI_FLIP_X, GOUSEI_HIKAKU);
		else
			BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, CLIP, GOUSEI_HIKAKU);
	}
	else
	{
		if (work->effect->side == 1)
			BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, CLIP | GOUSEI_FLIP_X, GOUSEI_KASAN);
		else
			BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, CLIP, GOUSEI_KASAN);
	}
	draw.upper = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER];

	// 弾の絵
	Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);
	if (mode == 0 || mode == 5 || mode == 8)
	{
		if (mode == 5)
			kind = 2;
		if (mode == 8)
			kind = 0;
		if (kind == 0)
			Resource_LoadAndDecompress((s32)&ResourceId_CometSheetA, work->sheet + 0x1000, 1, 1);
		else if (kind == 1)
			Resource_LoadAndDecompress((s32)&ResourceId_CometSheetB, work->sheet + 0x1000, 1, 1);
		else
			Resource_LoadAndDecompress((s32)&ResourceId_CometSheetC, work->sheet + 0x1000, 1, 1);
		if (mode == 5)
		{
			const void *pal = Resource_GetTableEntry((s32)&ResourceId_IceBlockSheet);
			{
				Iwram_CopyWords((void *)BG_PLTT, pal, 128);
			}
		}
		Resource_LoadAndDecompress((s32)&ResourceId_WindStreakSheet, work->sheet + 0x2000, 1, 0);
		if (mode == 5)
		{
			const void *pal = Resource_GetTableEntry((s32)&ResourceId_IceBlockSheet);
			{
				Iwram_CopyWords((void *)BG_PLTT, pal, 128);
			}
		}
		work->transfer_mode = 2;
		work->transfer_value = 75;
	}
	else if (mode == 1)
	{
		Resource_LoadAndDecompress((s32)&ResourceId_ShurikenSheet, work, 1, 1);
		REG_BLDCNT = 0;
		work->transfer_mode = 1;
		work->transfer_value = 0;
	}
	else if (mode == 2)
	{
		Iwram_CopyWords((void *)BG_PLTT, Resource_GetTableEntry((s32)&ResourceId_CometSheetA), 128);
		Resource_LoadAndDecompress((s32)&ResourceId_ProjectileVolleyImage, work, 0, 0);
		work->transfer_mode = 2;
		work->transfer_value = 50;
	}
	else if (mode == 3 || mode == 4 || mode == 11)
	{
		Resource_LoadAndDecompress((s32)&ResourceId_YellowSparkSheet, work, 1, 1);
		work->transfer_mode = 2;
		work->transfer_value = 50;
	}
	else if (mode == 6)
	{
		Resource_LoadAndDecompress((s32)&ResourceId_FireballSheet, work, 1, 1);
		work->transfer_mode = 2;
		work->transfer_value = 50;
	}
	else
	{
		Resource_LoadAndDecompress((s32)&ResourceId_IceChipSheet, work, 1, 1);
		Resource_LoadAndDecompress(
			(s32)&ResourceId_MercuryDjinnSmallSheet, work->sheet + 0x65c0, 1, 0);
		work->transfer_mode = 2;
		work->transfer_value = 50;
	}
	Scheduler_AddOrUpdateCallback((s32)BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);

	// 弾の出る位置と速さ
	user = GetBattleObjectSlotFar(work->effect->actor)->object;
	shots = Volley_Modes[mode * 5];
	steps = Volley_Modes[mode * 5 + 1];
	{
		s32 stagger = Volley_Modes[mode * 5 + 2];
		s32 delay = Volley_Modes[mode * 5 + 3];
		s32 height = Battle_GetObjectTableValueFar(work->effect->actor);

		if (work->effect->count * shots > 63) // particles[64] に収める
			work->effect->count = 1;

		for (i = 0; i != work->effect->count; i++)
		{
			struct MotionObject *tgt = GetBattleObjectSlotFar(work->effect->actors[i])->object;
			s32 tgt_height = Battle_GetObjectTableValueFar(work->effect->actors[i]);

			for (j = 0; j != shots; j++)
			{
				struct EffectStep *shot = &work->particles[i * shots + j];

				shot->x = user->x;
				if (mode == 7)
					shot->y = user->y + ((Random16() & 15) << 16) + 0x3a0000;
				else if (mode == 10)
					shot->y = user->y + height / 2;
				else if (mode == 6)
					shot->y = user->y + height / 2;
				else if (mode == 9)
					shot->y = user->y + height / 2 + ((16 - (Random16() & 31)) << 16);
				else if ((u32)(mode - 3) <= 1)
					shot->y = user->y + height / 2 + ((16 - (Random16() & 31)) << 16);
				else if (mode == 11)
					shot->y = user->y + height / 2 + ((32 - (Random16() & 63)) << 16);
				else if (mode == 5)
					shot->y = user->y + height / 2;
				else
					shot->y = user->y + height;
				shot->z = user->z;
				shot->velocity_x = (tgt->x - shot->x) / steps;
				if (mode == 7)
					shot->velocity_y =
						(s32)(tgt->y + ((Random16() & 63) << 16) - shot->y - 0xc0000) / steps;
				else if (mode == 8)
					shot->velocity_y =
						(s32)(tgt->y + ((Random16() & 7) << 16) - shot->y + 0x160000) / steps;
				else if (mode == 9)
					shot->velocity_y =
						(s32)(tgt->y + ((64 - (Random16() & 63)) << 16) - shot->y) / steps;
				else if (mode == 10)
					shot->velocity_y =
						(s32)(tgt->y + ((Random16() & 31) << 16) - shot->y + 0x40000) / steps;
				else if ((u32)(mode - 3) <= 1 || mode == 11 || mode == 5)
					shot->velocity_y = 0;
				else if (mode == 6)
					shot->velocity_y =
						(s32)(tgt->y + tgt_height / 2 + ((Random16() & 15) << 16) - shot->y)
						/ steps;
				else
					shot->velocity_y =
						(s32)(tgt->y + tgt_height - ((Random16() & 15) << 16) - shot->y) / steps;
				shot->velocity_z = (tgt->z - shot->z) / steps;
				shot->variant = 0;
			}
		}
		for (i = 0; i != PTCL_MAX; i++)
			PARTICLES[i].variant = 0;
		if (mode == 6)
			total = work->effect->count * delay + shots * stagger + 32;
		else
			total = work->effect->count * delay + shots * stagger + 16;

		//------------------------------------------------------------------
		// 毎フレームの動きと描画
		//------------------------------------------------------------------
		for (frame = 0; frame != total; frame++)
		{
			s32 index;

			// 結果の再生を始める
			if (se_wait > 0)
				se_wait--;
			if (mode == 6)
			{
				if (frame == 4)
					AudioCommand_PlayFar(136);
				if (frame == 32)
					BattleEventRuntime_BeginPhaseFar(134);
			}
			else if (mode == 7)
			{
				if (frame == 48)
					BattleEventRuntime_BeginPhaseFar(133);
			}
			else if (mode != 5)
			{
				if (frame == 16)
					BattleEventRuntime_BeginPhaseFar(133);
			}

			// 前置き：ジンの小さな絵が円を描き、70 フレームから上へ去る
			if (work->effect->prelude == 1)
			{
				s32 x = (-Trig_Sin(frame << 11) << 2 >> 16) + screen_x / 2 - 10;
				s32 y = (Trig_Cos(frame << 11) << 1 >> 16) + screen_y - 24;

				if (frame > 69)
					y = y - frame * 2 + 138;
				if (work->effect->side == 1)
					BattleEffect_LoadWork(
						HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, CLIP | GOUSEI_FLIP_X, GOUSEI_KASAN);
				else
					BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, CLIP, GOUSEI_KASAN);
				draw.lower = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE];
				if (frame <= 3)
					draw.lower(canvas, work->sheet + 0x65c0, x, y, 20, 40);
				draw.lower(canvas, work->sheet + 0x65c0, x, y, 20, 40);
				Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
			}

			// 弾
			Render_ResetTransformState();
			Graphics_PrepareTransferInIwramWork(camera->eye, camera->pos);
			index = 0;
			for (i = 0; i != work->effect->count; i++)
			{
				struct MotionObject *tgt = GetBattleObjectSlotFar(work->effect->actors[i])->object;

				for (j = 0; j != shots; j++, index++)
				{
					if (j * stagger + i * delay < frame)
					{
						struct EffectStep	  *shot = &work->particles[i * shots + j];
						struct EffectPosition *pos = &position;

						// 画面の位置 (キャンバスの横は半分)
						EffectPosition_ApplyBaseAndYOffset((s32 *)shot, pos);
						pos->x >>= 1;
						shot->x += shot->velocity_x;
						shot->y += shot->velocity_y;
						shot->z += shot->velocity_z;
						if (mode == 6)
						{
							// 火の玉の尾を二つ出す
							shift = 0;
							for (k = 512; k != PTCL_MAX; k++)
							{
								struct EffectStep *p = &PARTICLES[k];

								if (p->variant == 0)
								{
									p->x = position.x << 16;
									p->y = position.y << 16;
									p->velocity_x = ((Random16() & 255) - 128) << 11;
									p->velocity_y = ((Random16() & 255) - 128) << 11;
									p->variant = (Random16() & 7) + 32;
									shift++;
									if (shift == 2)
										break;
								}
							}
						}
						if (shot->variant == 0)
						{
							// 対象の側に入り、対象の x を越えたら当たり
							x = shot->x;
							if (tgt->x < 0)
							{
								hit = x < 0;
								/* FAKEMATCH: conditional cancelling updates end CSE
								 * before the hit test, preserving the native sign
								 * result and duplicated side check. Both updates
								 * disappear; removing both guards loses three
								 * English instructions and changes 77 rows. 2026-10-11: one test after an if/else that sets hit differs 65. */
								if (tgt)
								{
									hit ^= x;
									hit ^= x;
								}
								if (!hit)
									goto draw_shot;
							}
							else
							{
								hit = x >= 0;
								/* FAKEMATCH: the same CSE block end keeps the
								 * opposite sign result before its side check;
								 * the cancelling updates emit no instructions. */
								if (tgt)
								{
									hit ^= x;
									hit ^= x;
								}
								if (!hit)
									goto draw_shot;
							}
							if (ABS(x) >= ABS(tgt->x))
							{
								s32				   spawned = 0;
								struct EffectStep *flash = &PARTICLES[index];

								shot->variant = 1;
								if (mode == 5)
								{
									BattleEventRuntime_BeginPhaseFar(134);
								}
								else if (mode != 6 && se_wait == 0)
								{
									se_wait = 8;
									AudioCommand_PlayFar(132);
								}
								if (mode == 2)
								{
									// はね返る
									s32 kick = ((Random16() & 31) + 32) << 12;

									if (Random16() & 1)
										shot->velocity_y += kick;
									else
										shot->velocity_y -= kick;
									shot->velocity_x = -shot->velocity_x;
								}
								flash->variant = 1;
								flash->x = pos->x;
								flash->y = pos->y;
								flash->z = 0;
								if (mode != 7)
									work->shake_frames = 2;
								ObjectGroup_UpdateMembers(work->effect->actors[i], 7, 5, i, 8);
								if (mode != 7 && mode != 9 && mode != 10)
								{
									if (mode == 5)
										BattleMotion_ApplyVariantMotionFar(
											work->effect->actors[i], 4);
									else
										BattleMotion_ApplyVariantMotionFar(
											work->effect->actors[i], 5);
								}
								// 火花
								shift = mode == 5 ? 12 : 11;
								for (k = 100; k != 512; k++)
								{
									struct EffectStep *p = &PARTICLES[k];

									if (p->variant == 0)
									{
										p->x = position.x << 16;
										p->y = position.y << 16;
										p->velocity_x = ((Random16() & 255) - 128) << shift;
										p->velocity_y = ((Random16() & 255) - 128) << shift;
										p->variant = (Random16() & 7) + 16;
										spawned++;
										if (spawned == Volley_Modes[mode * 5 + 4])
											break;
									}
								}
							}
						}
					draw_shot:
						if (mode == 0 || mode == 5 || mode == 8)
						{
							// 絵は上半分だけで、下半分は上下を反転して描く
							if (work->effect->side == 1)
								BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7,
									CLIP | GOUSEI_FLIP_X, GOUSEI_HIKAKU);
							else
								BattleEffect_LoadWork(
									HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, CLIP, GOUSEI_HIKAKU);
							((DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE])(canvas,
								work->sheet + 0x1000, pos->x - 16,
								pos->y - Volley_CometHeights[kind], 32, Volley_CometHeights[kind]);
							Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
							if (work->effect->side == 1)
								BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7,
									CLIP | GOUSEI_FLIP, GOUSEI_HIKAKU);
							else
								BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7,
									CLIP | GOUSEI_FLIP_Y, GOUSEI_HIKAKU);
							((DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE])(canvas,
								work->sheet + 0x1000, pos->x - 16, pos->y, 32,
								Volley_CometHeights[kind]);
							Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
						}
						else if (mode == 1)
						{
							if (ABS(shot->x) <= ABS(tgt->x))
								draw.upper(canvas, work->sheet + frame % 6 * 768, pos->x - 16,
									pos->y - 12, 32, 24);
						}
						else if (mode == 7 || mode == 9 || mode == 10)
						{
							s32 cell = j & 3;
							draw.upper(canvas, work->sheet + Volley_PelletOffsets[cell], pos->x - 4,
								pos->y - 4, 8, 8);
						}
						else if (mode == 2)
						{
							draw.upper(
								canvas, work->sheet + j % 6 * 128, pos->x - 4, pos->y - 8, 8, 16);
						}
						else if (mode == 3)
						{
							draw.upper(canvas, work->sheet + 96, pos->x - 9, pos->y - 7, 18, 13);
						}
						else if (mode == 4)
						{
							draw.upper(canvas, work, pos->x - 6, pos->y - 4, 12, 8);
						}
						else if (mode == 11)
						{
							draw.upper(canvas, work->sheet + 330, pos->x - 15, pos->y - 12, 29, 23);
						}
						else
						{
							draw.upper(canvas, work, pos->x - 20, pos->y - 32, 40, 64);
						}
					}
				}
			}

			// 当たりの光：12 フレーム。半分の絵を、反転してもう半分描く (奇数は縦横を入れ替えた絵)
			if (mode == 0 || mode == 5 || mode == 8)
			{
				for (i = 0; i != work->effect->count * shots; i++)
				{
					struct EffectStep *flash = &PARTICLES[i];

					if (flash->variant == 1)
					{
						s32 step = flash->z / 2;

						if (!(i & 1))
						{
							BattleEffect_LoadWork(
								HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, CLIP, GOUSEI_HIKAKU);
							((DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE])(canvas,
								work->sheet + Volley_FlashOffsets[step],
								flash->x - Volley_FlashWidths[step],
								flash->y - Volley_FlashRises[step], Volley_FlashWidths[step],
								Volley_FlashHeights[step]);
							Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
							BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7,
								CLIP | GOUSEI_FLIP, GOUSEI_HIKAKU);
							((DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE])(canvas,
								work->sheet + Volley_FlashOffsets[step], flash->x,
								flash->y + Volley_FlashSteps[step] - Volley_FlashRises[step],
								Volley_FlashWidths[step], Volley_FlashHeights[step]);
							Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
						}
						else
						{
							BattleEffect_LoadWork(
								HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, CLIP, GOUSEI_HIKAKU);
							((DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE])(canvas,
								work->sheet + Volley_FlashOffsets[step] + 0x128a,
								flash->x - Volley_FlashRises[step],
								flash->y - Volley_FlashWidths[step], Volley_FlashHeights[step],
								Volley_FlashWidths[step]);
							Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
							BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7,
								CLIP | GOUSEI_FLIP, GOUSEI_HIKAKU);
							((DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE])(canvas,
								work->sheet + Volley_FlashOffsets[step] + 0x128a,
								flash->x + Volley_FlashSteps[step] - Volley_FlashRises[step],
								flash->y, Volley_FlashHeights[step], Volley_FlashWidths[step]);
							Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
						}
						flash->z++;
						if (flash->z == 12)
							flash->variant = 0;
					}
				}
			}

			// 火花：落ちて、下 (y 112) ではね返る
			for (i = 100; i != 512; i++)
			{
				struct EffectStep *p = &PARTICLES[i];

				if (p->variant > 0)
				{
					s32 size = (p->variant >> 3) + 1;

					draw.upper(canvas, sheet + ParticleStreams_CellOffsets[size - 1],
						HI(p->x) - size / 2, HI(p->y) - size, size, size * 2);
					EffectStep_AdvanceWithGravity2D(p, 60, 0x1000);
					if (p->y > 0x700000)
						p->velocity_y = -p->velocity_y / 2;
					p->variant--;
				}
			}

			// 火の玉の尾：上へ昇る
			for (i = 512; i != PTCL_MAX; i++)
			{
				struct EffectStep *p = &PARTICLES[i];

				if (p->variant > 0)
				{
					s32 size = (p->variant >> 4) + 1;

					draw.upper(canvas, sheet + ParticleStreams_CellOffsets[size - 1],
						HI(p->x) - size / 2, HI(p->y) - size, size, size * 2);
					EffectStep_AdvanceWithGravity2D(p, 60, -0x4000);
					p->variant--;
				}
			}
			Camera_ApplyShake(4, 4);
			ObjectGroup_TickMemberTimers();
			work->transfer_pending = 1;
			WaitFrames(1);
		}
	}

	Scheduler_RemoveCallback((u32)BattlePresentation_ProcessPendingGraphicsTransfer);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
	BattleFx_EndCanvasLayer();
}

//--------------------------------------------------------------------------
// キャラを、上へはずみをつけて元の位置 (anchor) へ戻す
//--------------------------------------------------------------------------
void BattleFx_InitializeRisingObject(s32 id)
{
	struct BtlObj		*slot = GetBattleObjectSlotFar(id);
	struct MotionObject *obj = slot->object;

	obj->acceleration = 0x20000;
	obj->speed_limit = 0x80000; // 8 ドット/フレーム
	obj->velocity_y = 0x40000;
	obj->vertical_motion_strength = 0xab85;
	obj->vertical_motion_phase = 0;
	obj->auto_face_motion = 0;
	obj->snap_to_target = 1;
	ObjectDispatch_InitFromTable6Far(obj);
	Object_SetMoveTargetFar(obj, slot->anchor_x, 0, slot->anchor_z);
	ObjectDispatch_ApplyArgumentToChildrenFar(obj, 1);
}

//--------------------------------------------------------------------------
// キャラを、相手までの 8 割の位置へ frames フレームで動かす
//--------------------------------------------------------------------------
void BattleFx_SetApproachMotion(s32 id, s32 tgt_id, s32 frames)
{
	struct BtlObj		*slot = GetBattleObjectSlotFar(id);
	struct BtlObj		*tgt_slot = GetBattleObjectSlotFar(tgt_id);
	struct MotionObject *obj = slot->object;
	struct MotionObject *tgt = tgt_slot->object;
	s32					 rate = 80; // %
	s32					 diff_x = tgt->x - obj->x;
	s32					 start_x = obj->x;
	s32					 dx = rate * diff_x / 100;
	s32					 diff_z = tgt->z - obj->z;
	s32					 start_z = obj->z;
	s32					 dz = rate * diff_z / 100;
	s32					 x = start_x + dx;
	s32					 z = start_z + dz;
	s32					 sx = dx >> 8; // 1/256 ドットで距離を求める
	s32					 sz = dz >> 8;
	s32					 dist;

	dist = Iwram_Sqrt(sx * sx + sz * sz);
	dist = (dist << 8) / frames;
	obj->acceleration = dist;
	obj->speed_limit = dist;
	obj->snap_to_target = 1;
	obj->vertical_motion_strength = 0xab85;
	obj->velocity_y = 0;
	obj->vertical_motion_phase = 0;
	obj->auto_face_motion = 1;
	ObjectDispatch_InitFromTable6Far(obj);
	Object_SetMoveTargetFar(obj, x, 0, z);
	ObjectDispatch_ApplyArgumentToChildrenFar(obj, 2);
}

//--------------------------------------------------------------------------
// pack_rows.c
// タイル：横 2 バイトを 1 バイトに詰める
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// 1 行 40 バイトを 288 行、横に 2 バイトずつ 1 バイトに詰めて写す
// （あとの奇数列のバイトが残る）
// stride : src の 1 行のバイト数（dst はその半分）
//--------------------------------------------------------------------------
void Graphics_PackTileRows(u8 *src, u8 *dst, s32 stride, s32 unused)
{
	s32 y = 0;
	s32 h = 288;
	s32 ofs = 0;

	do
	{
		s32 x;

		for (x = 0; x != 40; x++)
		{
			dst[ofs / 2 + x / 2] = src[ofs + x];
		}
		ofs += stride;
		y++;
	} while (y != h);
}

#define TSUBU_MAX 64	// 粒の数
#define RING_SIZE 0x3c0 // 輪の一枚 (20x48)

//--------------------------------------------------------------------------
// 四種の入口
//--------------------------------------------------------------------------
void BattleFx_RunParticleFieldVariant0(s32 effect)
{
	BattleFx_RunParticleFieldVariant((struct BattleEffectArgument *)effect, 0);
}

void BattleFx_RunParticleFieldVariant1(s32 effect)
{
	BattleFx_RunParticleFieldVariant((struct BattleEffectArgument *)effect, 1);
}

void BattleFx_RunParticleFieldVariant2(s32 effect)
{
	BattleFx_RunParticleFieldVariant((struct BattleEffectArgument *)effect, 2);
}

void BattleFx_RunParticleFieldVariant3(s32 effect)
{
	BattleFx_RunParticleFieldVariant((struct BattleEffectArgument *)effect, 3);
}

//--------------------------------------------------------------------------
// 使う者が最初の対象へ寄り、10 フレーム目に当てる
// 対象の位置に輪を出し、粒を 64 個飛び散らせる。全部で 60 フレーム
// variant : 色 (パレットの元の絵 0-3)
//--------------------------------------------------------------------------
void BattleFx_RunParticleFieldVariant(struct BattleEffectArgument *effect, s32 variant)
{
	/* FAKEMATCH: needed to match: the blitter/camera declarations and independent store order preserve spill slots and loop preheader order 2026-10-11: the camera read is the plain slot now; Ram_MapCellBuffer at the two uses of cells differs 2 (its load moves ahead of lsls r3 at the packing call). */
	void				   **heap;
	void				   **cursor;
	struct BattleEffectWork *work;
	void					*canvas;
	DrawRectangle			 draw[2];
	struct BattleCamera		*camera;
	u8						*sheet;
	struct EffectStep		*p;
	struct EffectStep		*step;
	struct EffectPosition	 origin;
	struct EffectPosition	 screen;
	struct EffectPosition	 pos;
	struct MotionObject		*tgt;
	s32						 frame;
	s32						 n;
	s32						 size;
	s32						 t;
	s32						 pal;
	u8						*cells;

	// ワーク、キャンバス、カメラ
	cells = Ram_MapCellBuffer;
	heap = (void **)gBattleFxWork;
	cursor = heap;
	work = *cursor++;
	canvas = *cursor;
	camera = heap[HEAP_SLOT_CAMERA - HEAP_SLOT_BATTLE_EFFECT];
	sheet = heap[2];
	work->effect = effect;
	BattleFx_BeginCanvasLayer(0);

	// 上半分と、上下を反転した下半分。向こう側なら左右も反転する
	if (work->effect->side == 0)
	{
		BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, CLIP, GOUSEI_HIKAKU);
		BattleEffect_LoadWork(
			HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, CLIP | GOUSEI_FLIP_Y, GOUSEI_HIKAKU);
	}
	else
	{
		BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, CLIP | GOUSEI_FLIP_X, GOUSEI_HIKAKU);
		BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, CLIP | GOUSEI_FLIP, GOUSEI_HIKAKU);
	}
	draw[0] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER];
	draw[1] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE];

	// 絵と色
	Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);
	Resource_LoadAndDecompress((s32)&ResourceId_YellowOrbSheet, work->sheet, 1, 0);
	Graphics_PackTileRows((u8 *)work, (u8 *)cells, 40, 288);
	Resource_LoadAndDecompress((s32)&ResourceId_BlueBurstSheet, work->sheet, 1, 1);
	switch (variant)
	{
	case 0:
		pal = (s32)&ResourceId_FirePillarSheetB;
		break;
	case 1:
		pal = (s32)&ResourceId_IceBlockSheet;
		break;
	case 2:
		pal = (s32)&ResourceId_PinkBurstSheet;
		break;
	default:
		pal = (s32)&ResourceId_BlastSheet;
		break;
	}
	Iwram_CopyWords((void *)BG_PLTT, Resource_GetTableEntry(pal), 128);
	work->transfer_mode = 2;
	work->transfer_value = 75;
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);

	// 使う者を寄せ、対象の位置に粒を用意する (x は画面の中央の側へ飛ぶ)
	BattleFx_SetApproachMotion(work->effect->actor, work->effect->actors[0], 10);
	tgt = GetBattleObjectSlotFar(work->effect->actors[0])->object;
	for (n = 0; n != TSUBU_MAX; n++)
	{
		step = &work->particles[n];
		step->x = tgt->x;
		step->y = tgt->y + 0xa0000; // 10 ドット上
		step->z = tgt->z;
		step->velocity_x = (Random16() & 0x1ff) << 11;
		step->velocity_y = ((Random16() & 255) - 64) << 11;
		step->velocity_z = ((Random16() & 255) - 128) << 11;
		if (step->x > 0)
			step->velocity_x = -step->velocity_x;
		step->variant = n / 2 + 16; // 残りのフレーム
	}
	EffectPosition_ApplyAlternateStepAndYOffset(work->effect->actors[0], &origin);

	for (frame = 0; frame != 60; frame++)
	{
		// 15 フレームのあいだ、使う者の位置に絵を上下二つに分けて描く
		if (frame <= 14)
		{
			EffectPosition_ApplyAlternateStepAndYOffset(work->effect->actor, &screen);
			draw[0](canvas, work, screen.x / 2 - 16, screen.y - 48, 40, 32);
			draw[1](canvas, work, screen.x / 2 - 16, screen.y - 16, 40, 32);
		}
		// 当たり
		if (frame == 10)
		{
			ObjectGroup_UpdateMembers(work->effect->actors[0], 7, 5, 0, 8);
			BattleMotion_ApplyVariantMotionFar(work->effect->actors[0], 4);
			BattleEventRuntime_BeginPhaseFar(134);
			work->shake_frames = 8;
		}
		// 輪：8 フレーム目から 2 フレームずつ 6 枚
		t = frame - 8;
		if (t >= 0 && t <= 11)
		{
			s32 ring = t / 2;
			draw[0](canvas, cells + ring * RING_SIZE, origin.x / 2 - 16, screen.y - 40, 20, 48);
		}
		// 粒
		if (t >= 0 && t <= 55)
		{
			Render_ResetTransformState();
			Graphics_PrepareTransferInIwramWork(camera->eye, camera->pos);
			for (n = 0; n != TSUBU_MAX; n++)
			{
				p = &work->particles[n];
				size = p->variant;
				if (size > 0)
				{
					EffectPosition_ApplyBaseAndYOffset(&p->x, &pos);
					size >>= 4;
					size += 2;
					pos.x >>= 1;
					draw[0](canvas, sheet + ParticleStreams_CellOffsets[size - 1], pos.x - size / 2,
						pos.y - size, size, size * 2);
					EffectStep_AdvanceWithGravity3D(p, 60, -0x200);
					p->variant--;
				}
			}
		}
		Camera_ApplyShake(8, 8);
		ObjectGroup_TickMemberTimers();
		work->transfer_pending = 1;
		WaitFrames(1);
	}
	Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
	BattleFx_EndCanvasLayer();
}

//--------------------------------------------------------------------------
// transpose_copy.c
// タイル：バイトの絵を右に 90 度回して写す
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// 幅 w・高さ h の src を、幅 h・高さ w の dst に右へ 90 度回して写す
// （src のいちばん上の行が dst のいちばん右の列になる）
//--------------------------------------------------------------------------
void Graphics_TransposeCopy(const u8 *src, u8 *dst, s32 w, s32 h)
{
	s32		  y;
	s32		  x;
	s32		  col;
	s32		  ofs;
	u8		 *d;
	const u8 *s;

	y = 0;
	if (h != 0)
	{
		ofs = 0;
		col = h;
		do
		{
			x = 0;
			if (w != 0)
			{
				d = (u8 *)(col + (u32)dst - 1);
				s = (const u8 *)(ofs + (u32)src);
				do
				{
					x++;
					*d = *s;
					s++;
					d += h;
				} while (x != w);
			}
			y++;
			ofs += w;
			col--;
		} while (y != h);
	}
}

//--------------------------------------------------------------------------
// 水星のジン：(96, 60) のまわりを渦を巻いて回りながら、青い炎を 5 発左へ撃つ
// 炎は 5 フレーム進むと 32 個の粒に砕ける
//--------------------------------------------------------------------------
void BattleFx_RunMercuryDjinnFlames(struct BattleEffectArgument *effect)
{
	void				   **heap_cache;
	void				   **cursor;
	struct BattleEffectWork *work;
	void					*canvas;
	s32						 frame;
	u8						*sheet;
	DrawRectangle			 blit[2];
	DrawRectangle			*draw;
	struct EffectStep		*flame;
	struct EffectStep		*mote;
	u8						*cells;
	s32						 i;
	s32						 j;

	heap_cache = (void **)gBattleFxWork;
	cursor = heap_cache;
	work = *cursor++;
	canvas = *cursor;
	sheet = heap_cache[2];
	work->effect = effect;
	BattleFx_BeginCanvasLayer(0);
	draw = blit;
	BattleFx_FetchRectangleBlitters(0, draw);

	// 炎の絵は BlueFlame を転置して写す
	cells = Ram_MapCellBuffer;
	Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);
	Resource_LoadAndDecompress((s32)&ResourceId_MercuryDjinnSmallSheet, work->sheet, 1, 0);
	Resource_LoadAndDecompress((s32)&ResourceId_BlueFlameSheet, cells, 1, 1);
	Graphics_TransposeCopy(cells, work->sheet + 0x2a8, 17, 104);
	Graphics_TransposeCopy(cells + 0x6e8, work->sheet + 0x990, 34, 65);
	work->transfer_mode = 2;
	work->transfer_value = 75; // 転送のたびに 3/4 へ減らして残像にする
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
	for (i = 0; i != 8; i++)
		work->particles[i].variant = -1;
	for (i = 0; i != 512; i++)
		PARTICLES[i].variant = -1;
	AudioCommand_PlayFar(162);
	frame = 0;
	do
	{
		if (frame == 56)
			BattleEventRuntime_BeginPhaseFar(133);

		// 炎：1 フレームに 12 ドット左へ進み、5 フレーム目で砕ける
		for (i = 0; i != 5; i++)
		{
			flame = &work->particles[i];
			if (flame->variant != -1)
			{
				blit[0](canvas, work->sheet + 0x990, flame->x - 16, flame->y - 17, 65, 34);
				flame->x -= 12;
				flame->variant++;
				if (flame->variant == 5)
				{
					AudioCommand_PlayFar(133);
					work->shake_frames = 4;
					for (j = 0; j != 32; j++)
					{
						s32 dir;
						s32 speed;

						mote = &PARTICLES[i * 32 + j];
						dir = Random16() & 0xffff;
						speed = (Random16() & 0x1ff) + 256;
						mote->x = flame->x << 16;
						mote->y = flame->y << 16;
						mote->velocity_x = (Trig_Sin(dir) * speed) >> 8;
						mote->velocity_y = (Trig_Cos(dir) * speed) >> 7;
						mote->variant = (Random16() & 15) + 32; // 寿命 32〜47 フレーム
					}
				}
			}
		}

		// ジン：32 フレームで 1 周する。半径 64 - 2 × frame (横はその半分) は
		// 32 フレームで 0 になって中心を通り、その後は反対側へ広がる
		if (frame <= 95)
		{
			s32 x = ((Trig_Sin(frame << 11) * (64 - frame * 2)) >> 17) + 96;
			s32 y = ((Trig_Cos(frame << 11) * (64 - frame * 2)) >> 16) + 60;

			draw[1](canvas, work, x - 10, y - 17, 20, 34);
			for (i = 0; i != 5; i++)
			{
				// 決まったフレームに炎を撃ち、その 6 フレーム後に対象のパレットを
				// 6 フレーム 7 番に替えてアニメ 5 にし、動き 6 を与える
				if (frame == MercuryDjinnFlames_LaunchFrames[i])
				{
					struct EffectStep *shot = &work->particles[i];

					if (shot->variant == -1)
					{
						shot->x = x - 8;
						shot->velocity_x = x;
						shot->y = y;
						shot->variant = 0;
						break;
					}
				}
				if (frame == MercuryDjinnFlames_LaunchFrames[i] + 6)
				{
					for (j = 0; j != work->effect->count; j++)
					{
						ObjectGroup_UpdateMembers(work->effect->actors[j], 7, 5, j, 6);
						BattleMotion_ApplyVariantMotionFar(work->effect->actors[j], 6);
					}
				}
			}
		}

		// 粒：寿命に応じて小さくなり、落ちながら減速する
		for (i = 0; i != 256; i++)
		{
			mote = &PARTICLES[i];
			if (mote->variant != -1)
			{
				s32 size = mote->variant / 16 + 2;

				draw[1](canvas, sheet + ParticleStreams_CellOffsets[size - 1],
					HI(mote->x) - size / 2, HI(mote->y) - size, size, size * 2);
				EffectStep_AdvanceWithGravity2D(mote, 62, 0x2000);
				mote->variant--;
			}
		}
		Camera_ApplyShake(4, 4);
		ObjectGroup_TickMemberTimers();
		work->transfer_pending = 1;
		WaitFrames(1);
		frame++;
	} while (frame != 96);
	Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
	BattleFx_EndCanvasLayer();
}

//--------------------------------------------------------------------------
// 木星のジン：回りながら、右上から左下へ火の玉を 8 つ落とす
// 火の玉は y が 95 を越えると 32 個の粒に砕ける
//--------------------------------------------------------------------------
void BattleFx_RunJupiterDjinnFall(struct BattleEffectArgument *effect)
{
	void				   **heap_cache;
	void				   **cursor;
	struct BattleEffectWork *work;
	void					*canvas;
	u8						*sheet;
	DrawRectangle			 blit[2];
	DrawRectangle			*draw;
	struct EffectStep		*ball;
	struct EffectStep		*mote;
	s32						 frame;
	s32						 i;
	s32						 j;
	s32						 angle;

	heap_cache = (void **)gBattleFxWork;
	cursor = heap_cache;
	work = *cursor++;
	canvas = *cursor;
	sheet = heap_cache[2];
	work->effect = effect;
	BattleFx_BeginCanvasLayer(0);
	REG_BLDALPHA = 0x1010;
	draw = blit;
	BattleFx_FetchRectangleBlitters(0, draw);
	Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);
	Resource_LoadAndDecompress((s32)&ResourceId_JupiterDjinnSmallSheet, work->sheet, 1, 1);
	Resource_LoadAndDecompress((s32)&ResourceId_FlameballSheet, work->sheet + 0x320, 1, 0);
	work->transfer_mode = 2;
	work->transfer_value = 75; // 転送のたびに 3/4 へ減らして残像にする
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);

	// 火の玉の出だし：x 64〜127、y -80〜-17
	for (i = 0; i != 32; i++)
	{
		ball = &work->particles[i];
		ball->x = (Random16() & 63) + 64;
		ball->y = (Random16() & 63) - 80;
	}
	for (i = 0; i != 512; i++)
		PARTICLES[i].variant = -1;
	AudioCommand_PlayFar(171);
	frame = 0;
	angle = 0x8000; // 180°
	do
	{
		if (frame == 56)
			BattleEventRuntime_BeginPhaseFar(133);

		// ジン：1 フレームに 11.25° 回る。半径 64 - 2 × frame (横はその半分) は
		// 32 フレームで 0 になって中心を通り、その後は反対側へ広がる
		if (frame <= 95)
		{
			s32 x = (Trig_Sin(angle) * (64 - frame * 2)) >> 17;
			s32 y = (Trig_Cos(angle) * (64 - frame * 2)) >> 16;

			blit[0](canvas, work, x + 86, y + 28, 20, 40);
		}

		// 火の玉：4 フレームおきに 1 つずつ、1 フレームに左へ 6、下へ 12 ドット
		for (i = 0; i != 8; i++)
		{
			ball = &work->particles[i];
			if (frame >= i * 4 + 8 && ball->y <= 95)
			{
				blit[0](canvas, work->sheet + 0x320, ball->x - 20, ball->y - 32, 40, 64);
				ball->x -= 6;
				ball->y += 12;
				if (ball->y > 95)
				{
					for (j = 0; j != 32; j++)
					{
						s32 dir;
						s32 speed;

						mote = &PARTICLES[i * 32 + j];
						dir = Random16() & 0xffff;
						speed = (Random16() & 0x1ff) + 256;
						mote->x = ball->x << 16;
						mote->y = ball->y << 16;
						mote->velocity_x = (Trig_Sin(dir) * speed) >> 7;
						mote->velocity_y = (Trig_Cos(dir) * speed) >> 6;
						mote->variant = (Random16() & 15) + 32; // 寿命 32〜47 フレーム
					}
					AudioCommand_PlayFar(133);
					work->shake_frames = 4;
					for (j = 0; j != work->effect->count; j++)
					{
						ObjectGroup_UpdateMembers(work->effect->actors[j], 7, 5, j, 6);
						BattleMotion_ApplyVariantMotionFar(work->effect->actors[j], 6);
					}
				}
			}
		}

		// 粒：寿命に応じて小さくなり、落ちながら減速する
		for (i = 0; i != 512; i++)
		{
			mote = &PARTICLES[i];
			if (mote->variant != -1)
			{
				s32 size = mote->variant / 16 + 1;

				draw[1](canvas, sheet + ParticleStreams_CellOffsets[size - 1],
					HI(mote->x) - size / 2, HI(mote->y) - size, size, size * 2);
				EffectStep_AdvanceWithGravity2D(mote, 62, 0x2000);
				mote->variant--;
			}
		}
		Camera_ApplyShake(4, 4);
		ObjectGroup_TickMemberTimers();
		work->transfer_pending = 1;
		WaitFrames(1);
		angle -= 0x800; // 11.25°
		frame++;
	} while (frame != 96);
	Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
	BattleFx_EndCanvasLayer();
}

//--------------------------------------------------------------------------
// load_and_decompress.c
// リソース：パレットつきのリソースを展開する
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// リソース id を dst に展開する。先頭 128 バイトがパレットのとき、
// copy_pal なら BG パレットの先頭に写し、skip_pal ならとばして展開する
//--------------------------------------------------------------------------
void Resource_LoadAndDecompress(s32 id, void *dst, s32 skip_pal, s32 copy_pal)
{
	u8 *res = Resource_GetTableEntry(id);

	if (copy_pal != 0)
		Iwram_CopyWords((void *)0x05000000, res, 128);
	if (skip_pal != 0)
		res += 128;
	Resource_DecodeType01(res, dst);
}

void BattleFx_RunVenusDjinnColumns(struct BattleEffectArgument *effect)
{
	void				   **heap_cache;
	void				   **cursor;
	struct BattleEffectWork *work;
	void					*canvas;
	s32						 frame;
	DrawRectangle			 draw[2];
	u8						*sheet;
	struct EffectStep		*step;
	struct EffectStep		*mote;
	s32						 i;
	s32						 j;
	s32						 angle;

	heap_cache = (void **)gBattleFxWork;
	cursor = heap_cache;
	work = *cursor++;
	canvas = *cursor;
	sheet = heap_cache[2];
	work->effect = effect;
	BattleFx_BeginCanvasLayer(0);
	REG_BLDALPHA = 0x1010;
	BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 11, 2);
	BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, 3, 3);
	draw[0] = heap_cache[7];
	draw[1] = heap_cache[8];
	Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);
	Resource_LoadAndDecompress((s32)&ResourceId_VenusDjinnSmallSheet, work->sheet, 1, 1);
	Resource_LoadAndDecompress((s32)&ResourceId_BlueFlameSheet, work->sheet + 0x2f8, 1, 0);
	work->transfer_mode = 2;
	work->transfer_value = 75;
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
	for (i = 0; i != 32; i++)
	{
		step = &work->particles[i];
		step->x = Random16() & 63;
		step->y = 104;
	}
	for (i = 0; i != 512; i++)
		PARTICLES[i].variant = -1;
	AudioCommand_PlayFar(141);
	frame = 0;
	angle = 0x8000;
	do
	{
		if (frame <= 79)
		{
			s32 x = (Trig_Sin(angle) * 24) >> 16;
			s32 y = (Trig_Cos(angle) * (64 - frame * 2)) >> 16;

			draw[1](canvas, work, x + 22, y + 29, 20, 38);
		}
		if (frame == 56)
			BattleEventRuntime_BeginPhaseFar(133);
		for (i = 0; i != 10; i++)
		{
			step = &work->particles[i];
			if (frame >= i * 4 + 16)
			{
				draw[0](canvas, work->sheet + 0x9e0, step->x - 17, step->y - 32, 34, 65);
				if (frame == i * 4 + 16)
				{
					for (j = 0; j != 16; j++)
					{
						s32 direction;
						s32 speed;

						mote = &PARTICLES[i * 32 + j];
						direction = (Random16() & 0x7fff) + 0x4000;
						speed = (Random16() & 0x1ff) + 256;
						mote->x = step->x << 16;
						mote->y = (step->y + 16) << 16;
						mote->velocity_x = (Trig_Sin(direction) * speed) >> 7;
						mote->velocity_y = (Trig_Cos(direction) * speed) >> 6;
						mote->variant = (Random16() & 15) + 32;
					}
					if (i & 1)
						AudioCommand_PlayFar(133);
					work->shake_frames = 4;
					for (j = 0; j != work->effect->count; j++)
					{
						ObjectGroup_UpdateMembers(work->effect->actors[j], 7, 5, j, 6);
						BattleMotion_ApplyVariantMotionFar(work->effect->actors[j], 6);
					}
				}
				step->y -= 12;
			}
		}
		for (i = 0; i != 512; i++)
		{
			mote = &PARTICLES[i];
			if (mote->variant != -1)
			{
				s32 size = mote->variant / 16 + 2;

				draw[1](canvas, sheet + ParticleStreams_CellOffsets[size - 1],
					HI(mote->x) - size / 2, HI(mote->y) - size, size, size * 2);
				EffectStep_AdvanceWithGravity2D(mote, 62, 0x2000);
				mote->variant--;
			}
		}
		Camera_ApplyShake(4, 4);
		ObjectGroup_TickMemberTimers();
		work->transfer_pending = 1;
		WaitFrames(1);
		angle -= 0x800;
		frame++;
	} while (frame != 96);
	Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
	BattleFx_EndCanvasLayer();
}

void BattleFx_RunMarsDjinnPuffs(struct BattleEffectArgument *effect)
{
	void				   **heap_cache;
	void				   **cursor;
	struct BattleEffectWork *work;
	void					*canvas;
	u8						*sheet;
	DrawRectangle			 callbacks[2];
	DrawRectangle			*draw;
	struct EffectStep		*step;
	struct EffectStep		*mote;
	s32						 member;
	s32						 frame;
	s32						 i;
	s32						 curtain_y;

	heap_cache = (void **)gBattleFxWork;
	cursor = heap_cache;
	work = *cursor++;
	canvas = *cursor;
	sheet = heap_cache[2];
	work->effect = effect;
	BattleFx_BeginCanvasLayer(0);
	draw = callbacks;
	BattleFx_FetchRectangleBlitters(0, draw);
	Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);
	Resource_LoadAndDecompress((s32)&ResourceId_MarsDjinnSmallSheet, work->sheet, 1, 0);
	Resource_LoadAndDecompress((s32)&ResourceId_GlowOrbSheet, work->sheet + 0x320, 1, 1);
	work->transfer_mode = 2;
	work->transfer_value = 75;
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);

	member = 0;
	step = work->particles;
	do
	{
		s32 angle = member << 11;

		step->x = (Trig_Sin(angle) * 24) >> 16;
		step->y = ((Trig_Cos(angle) * 4) >> 16) + 52;
		if (member & 1)
			step->x = 32 - step->x;
		else
			step->x += 32;
		step->variant = -(member * 2);
		for (i = 0; i != 16; i++)
		{
			mote = &PARTICLES[member * 16 + i];
			mote->x = (((Random16() & 15) + step->x) - 8) << 16;
			mote->y = ((Random16() & 7) + 96) << 16;
			mote->velocity_x = ((Random16() & 127) - 64) << 11;
			mote->velocity_y = ((Random16() & 127) - 64) << 10;
			mote->z = Random16() & 0xffff;
			mote->velocity_z = Random16() & 0xffff;
		}
		step++;
		member++;
	} while (member != 9);

	AudioCommand_PlayFar(0x88);
	frame = 0;
	curtain_y = -172;
	do
	{
		if (frame == 56)
			BattleEventRuntime_BeginPhaseFar(0x85);
		if (frame <= 23)
			callbacks[0](canvas, work->sheet + 0x320 + (frame / 4) * 0x640, 40, 20, 40, 40);
		if (frame == 20)
			Iwram_CopyWords(
				(void *)BG_PLTT, Resource_GetTableEntry((s32)&ResourceId_MarsDjinnSmallSheet), 128);
		if (frame >= 20 && frame <= 31)
		{
			if (frame > 23)
				draw[1](canvas, work->sheet, 146 - frame * 4, curtain_y, 20, 40);
			else
				callbacks[0](canvas, work->sheet, 50, 20, 20, 40);
		}
		if (frame == 32)
		{
			AudioCommand_PlayFar(0x91);
			work->shake_frames = 8;
			Resource_LoadAndDecompress((s32)&ResourceId_EmberStreakSheet, work->sheet, 1, 1);
		}
		if (frame > 31)
		{
			member = 0;
			step = work->particles;
			do
			{
				if (step->variant >= 0 && step->variant <= 47)
				{
					s32 cell = step->variant / 8;
					u32 width;

					callbacks[0](canvas, work->sheet + PuffArc_CellSourceOffsets[cell],
						step->x - ((width = PuffArc_CellWidths[cell]) >> 1),
						step->y + PuffArc_CellBiasY[cell], width, PuffArc_CellHeights[cell]);
				}
				member++;
				step->variant++;
				step++;
			} while (member != 9);
		}
		step = PARTICLES;
		member = 0;
		do
		{
			if (frame >= (member / 16) * 2 + 40)
			{
				s32 size;
				s32 x;
				s32 angle;

				size = (member & 1) + 3;
				x = HI(step->x) + ((Trig_Sin(step->z) * 4) >> 16);
				draw[1](canvas, sheet + ParticleStreams_CellOffsets[size - 1], x - ((u32)size >> 1),
					HI(step->y) - size, size, size * 2);
				EffectStep_AdvanceWithGravity2D(step, 64, -0x2000);
				angle = step->z;
				step->z = angle + 0x800;
				if (step->z > 0xffff)
					step->z = angle + 0x800 - 0xffff;
			}
			member++;
			step++;
		} while (member != 144);

		if (frame == 38)
		{
			for (member = 0; member != work->effect->count; member++)
			{
				ObjectGroup_UpdateMembers(work->effect->actors[member], 7, 5, member, 16);
				BattleMotion_ApplyVariantMotionFar(work->effect->actors[member], 6);
			}
		}
		Camera_ApplyShake(8, 8);
		ObjectGroup_TickMemberTimers();
		work->transfer_pending = 1;
		WaitFrames(1);
		curtain_y += 8;
		frame++;
	} while (frame != 112);
	Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
	BattleFx_EndCanvasLayer();
}
#define PARTICLE_COUNT 64

void BattleFx_RunParticleReveal(struct BattleEffectArgument *effect)
{
	void				   **heap_cache;
	void				   **cursor;
	struct BattleEffectWork *work;
	void					*canvas;
	s32						 spawn[3];
	s32						 screen_y;
	s32						 screen_x;
	DrawRectangle			 routine[2];
	struct EffectStep		*p;
	s32						 i;
	s32						 frame;
	s32						 rise;

	heap_cache = (void **)gBattleFxWork;
	cursor = heap_cache;
	work = *cursor++;
	canvas = *cursor;
	work->effect = effect;
	BattleFx_BeginCanvasLayer(0);
	BattleFx_PrepareCanvasEffect(effect, 1, work->effect->side, 2, &screen_x, &screen_y);
	BattleFx_FetchRectangleBlitters(work->effect->side, routine);
	Resource_LoadAndDecompress((s32)&ResourceId_WaterSpraySheet, work->sheet, 1, 1);
	work->transfer_mode = 2;
	work->transfer_value = 75;
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
	EffectPosition_ApplyAlternateStepAndYOffset(work->effect->actors[0], spawn);

	for (i = 0; i != PARTICLE_COUNT; i++)
	{
		s32 angle;
		s32 speed;

		angle = (Random16() & 0x7fff) + 0x4000;
		speed = (Random16() & 0x1ff) + 0x80;
		gMapCellBuffer[i].x = ((spawn[0] / 2 + (Random16() & 0xf)) - 8) << 16;
		gMapCellBuffer[i].y = (spawn[1] + 8) << 16;
		gMapCellBuffer[i].velocity_x = (Trig_Sin(angle) * speed) >> 9;
		gMapCellBuffer[i].velocity_y = (Trig_Cos(angle) * speed) >> 6;
		gMapCellBuffer[i].z = Random16() & 0x7f;
		gMapCellBuffer[i].velocity_z = Random16() & 0x7f;
		gMapCellBuffer[i].variant = (Random16() & 0xf) + 32;
	}

	for (frame = 0; frame != 64; frame++)
	{
		if (frame > 47)
			REG_BLDALPHA = (64 - frame) | 0x1000;
		if (frame == 1)
		{
			Resource_LoadAndDecompress((s32)&ResourceId_IceChipSheet, work->sheet + 0x400, 1, 1);
			Resource_LoadAndDecompress(
				(s32)&ResourceId_MercuryDjinnSmallSheet, work->sheet + 0x65c0, 1, 0);
		}

		if (work->effect->prelude == 1)
		{
			s32 orbit;
			s32 x;
			s32 y;

			orbit = frame << 11;
			x = ((-Trig_Sin(orbit) << 2) >> 16) + screen_x / 2 - 10;
			y = ((Trig_Cos(orbit) << 1) >> 16) + screen_y - 22;
			if (frame > 69)
				y = (y - frame * 2) + 138;
			routine[1](canvas, work->sheet + 0x65c0, x, y, 20, 40);
			if (frame <= 3)
				routine[1](canvas, work->sheet + 0x65c0, x, y, 20, 40);
		}

		for (i = 0, p = gMapCellBuffer; i != PARTICLE_COUNT; i++, p++)
		{
			if (frame >= i / 4 + 4)
			{
				s32 cell;
				s32 w;
				s32 h;

				cell = (p->z / 128) & 3;
				routine[i & 1](canvas, work->sheet + 0x400 + ParticleReveal_CellSourceOffsets[cell],
					HI(p->x) - (w = ParticleReveal_CellWidths[cell]) / 2,
					HI(p->y) - (h = ParticleReveal_CellHeights[cell]) / 2, w, h);
				EffectStep_AdvanceWithGravity3D(p, 63, 0x1000);
			}
		}

		if (frame == 8)
		{
			work->shake_frames = frame;
			BattleEventRuntime_BeginPhaseFar(0x86);
			ObjectGroup_UpdateMembers(work->effect->actors[0], 7, 5, 0, 16);
			BattleMotion_ApplyVariantMotionFar(work->effect->actors[0], 3);
		}

		rise = frame * 4;
		if (rise > 32)
			rise = 32;
		if (work->effect->side == 0)
		{
			for (i = 0; i != 5; i++)
				routine[0](canvas, work, (i << 5) - (frame / 4 & 31), 120 - rise, 32, 32);
		}
		else
		{
			for (i = 0; i != 5; i++)
				routine[0](canvas, work, ((i << 5) + (frame / 4 & 31)) - 32, 120 - rise, 32, 32);
		}

		Camera_ApplyShake(4, 8);
		ObjectGroup_TickMemberTimers();
		work->transfer_pending = 1;
		WaitFrames(1);
	}

	Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
	BattleFx_EndCanvasLayer();
}

void BattleFx_RunVenusDjinnRockToss(struct BattleEffectArgument *effect)
{
	struct BattleEffectWork *work;
	void					*canvas;
	s32						 i;
	s32						 frame;
	DrawRectangle			 draw[2];
	struct BattleCamera		*camera;
	struct MotionObject		*target;
	struct MotionObject		*actor;
	struct EffectPosition	 position;
	s32						 screen_x;
	s32						 screen_y;
	s32						 j;

	work = *(gWorkSlot + HEAP_SLOT_BATTLE_EFFECT);
	canvas = *(gWorkSlot + HEAP_SLOT_BATTLE_CANVAS);
	camera = *(gWorkSlot + HEAP_SLOT_CAMERA);
	work->effect = effect;
	BattleFx_BeginCanvasLayer(0);
	BattleFx_PrepareCanvasEffect(effect, 0, work->effect->side, 2, &screen_x, &screen_y);
	REG_BLDALPHA = 0x1010;
	if (work->effect->side == 1)
		BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 7, 2);
	else
		BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 3, 2);
	draw[0] = (DrawRectangle)((union HeapState *)gWorkSlot)->slots[HEAP_SLOT_BLITTER];
	Resource_LoadAndDecompress((s32)&ResourceId_PortalSheet, work->sheet, 1, 0);
	Resource_LoadAndDecompress((s32)&ResourceId_VenusDjinnSmallSheet, work->sheet + 0x65c0, 1, 1);
	work->transfer_mode = 2;
	work->transfer_value = 75;
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
	actor = GetBattleObjectSlotFar(work->effect->actor)->object;
	target = GetBattleObjectSlotFar(work->effect->actors[0])->object;
	for (i = 0; i != 8; i++)
	{
		struct EffectStep *rock = &work->particles[i];

		rock->x = actor->x;
		rock->y = 0x420000;
		rock->z = actor->z;
		rock->velocity_x = (i * 0x500000) >> 5;
		rock->velocity_y = (s32)(((Random16() & 127) - 64) << 16) >> 6;
		rock->velocity_z = (s32)(((Random16() & 255) - 127) << 16) >> 5;
		if (rock->x > 0)
			rock->velocity_x = -rock->velocity_x;
		rock->variant = 1;
	}
	for (frame = 0; frame != 96; frame++)
	{
		if (frame > 16)
			BattleFx_StepPaletteToResource((s32)&ResourceId_PortalSheet);
		if (work->effect->prelude == 1)
		{
			s32 angle = frame << 11;
			s32 x = ((-Trig_Sin(angle) << 2) >> 16) + screen_x / 2 - 10;
			s32 y = ((Trig_Cos(angle) << 1) >> 16) + screen_y - 22;

			if (frame > 16)
				y = y - frame * 2 + 32;
			if (work->effect->side == 1)
				BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, 7, 3);
			else
				BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, 3, 3);
			if (frame <= 3)
				((DrawRectangle)((union HeapState *)gWorkSlot)->slots[HEAP_SLOT_BLITTER_ALTERNATE])(
					canvas, work->sheet + 0x65c0, x, y, 20, 40);
			Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
			draw[0](canvas, work->sheet + 0x65c0, x, y, 20, 40);
		}
		if ((frame & 1) == 0)
		{
			for (i = 0; i != 32; i++)
			{
				struct EffectStep *chip = &work->particles[32 + i];

				chip->velocity_x = Random16() % 6 + 3;
				chip->velocity_y = RockToss_ChipFlips[Random16() & 3];
			}
		}
		Render_ResetTransformState();
		Graphics_PrepareTransferInIwramWork(camera->eye, camera->pos);
		for (i = 0; i != 6; i++)
		{
			struct EffectStep *rock = &work->particles[i];

			if (rock->variant == 1)
			{
				if (frame > i * 2)
				{
					s32 x;
					s32 y;

					EffectPosition_ApplyBaseAndYOffset(&rock->x, &position);
					position.x >>= 1;
					x = position.x - 12;
					y = position.y - 24;
					draw[0](canvas, work, x, y, 24, 48);
					if ((frame & 3) <= 1)
						draw[0](canvas, work->sheet + RockToss_ChipCells[1], x + RockToss_ChipX[1],
							y + RockToss_ChipY[1], RockToss_ChipWidths[1], RockToss_ChipHeights[1]);
					else
						draw[0](canvas, work->sheet + RockToss_ChipCells[2], x + RockToss_ChipX[2],
							y + RockToss_ChipY[2], RockToss_ChipWidths[2], RockToss_ChipHeights[2]);
					for (j = 0; j != 4; j++)
					{
						struct EffectStep *chip = &work->particles[32 + i * 4 + j];
						s32				   chip_x;
						s32				   chip_y;

						BattleEffect_LoadWork(
							HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, chip->velocity_y, 2);
						draw[1] = (DrawRectangle)((union HeapState *)gWorkSlot)
									  ->slots[HEAP_SLOT_BLITTER_ALTERNATE];
						if (chip->velocity_y & 4)
							chip_x = x - RockToss_ChipWidths[chip->velocity_x]
									 - RockToss_ChipX[chip->velocity_x] + 24;
						else
							chip_x = x + RockToss_ChipX[chip->velocity_x];
						if (chip->velocity_y & 8)
							chip_y = y - RockToss_ChipHeights[chip->velocity_x]
									 - RockToss_ChipY[chip->velocity_x] + 48;
						else
							chip_y = y + RockToss_ChipY[chip->velocity_x];
						draw[1](canvas, work->sheet + RockToss_ChipCells[chip->velocity_x], chip_x,
							chip_y, RockToss_ChipWidths[chip->velocity_x],
							RockToss_ChipHeights[chip->velocity_x]);
						Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
					}
					rock->x += rock->velocity_x;
					rock->y += rock->velocity_y;
					rock->z += rock->velocity_z;
				}
				if (frame > i * 2 + 16)
				{
					rock->velocity_x += (target->x - rock->x) >> 8;
					rock->velocity_y += (0x140000 - rock->y) >> 8;
					rock->velocity_z += (target->z - rock->z) >> 8;
					if (frame < i * 2 + 85)
					{
						rock->velocity_x = rock->velocity_x * 60 / 64;
						rock->velocity_y = rock->velocity_y * 60 / 64;
						rock->velocity_z = rock->velocity_z * 60 / 64;
					}
					if (rock->y < 0x140000)
					{
						work->shake_frames = 8;
						rock->variant = 0;
						AudioCommand_PlayFar(134);
						ObjectGroup_UpdateMembers(work->effect->actors[0], 7, 5, 0, 4);
						BattleMotion_ApplyVariantMotionFar(work->effect->actors[0], 4);
					}
				}
			}
		}
		Camera_ApplyShake(16, 16);
		ObjectGroup_TickMemberTimers();
		work->transfer_pending = 1;
		WaitFrames(1);
	}
	Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
	BattleFx_EndCanvasLayer();
}

void Camera_ApplyShake(s32 random_mask, u32 shake_range)
{
	s32						 display_y;
	s32						 offset_x;
	s32						*remaining_frames;
	s32						 restored_position;
	struct BattleEffectWork *work;
	s32						 offset_y;
	s32						 half_range;
	s32						 random_x;
	struct BgScroll			*display_position;
	struct Projection		*display_center;
	struct BgScroll			*disp_p;
	struct Projection		*center_p;

	work = *(struct BattleEffectWork **)gBattleFxWork;
	remaining_frames = &work->shake_frames;
	if (*remaining_frames > 0)
	{
		random_x = (random_mask - 1) & Random16();
		half_range = (s32)(shake_range + (shake_range >> 0x1F)) >> 1;
		offset_y = ((shake_range - 1) & Random16()) - half_range;
		display_position = gBgScroll;
		offset_x = random_x - half_range;
		display_y = offset_y + 0x20;
		display_position[1].x = offset_x;
		display_position[1].y = display_y;
		display_center = &gProjection;
		display_center->center_x = 0x78 - offset_x;
		display_center->center_y = 0x78 - offset_y;
		*remaining_frames -= 1;
		return;
	}
	restored_position = work->saved_bg1_x;
	disp_p = gBgScroll;
	disp_p[1].x = restored_position;
	restored_position = work->saved_bg1_y;
	disp_p[1].y = restored_position;
	center_p = &gProjection;
	center_p->center_x = 0x78;
	center_p->center_y = 0x78;
}

extern s16 Mode12_Points[][2];
extern u8  Mode12_Animations[];
extern u8  Mode12_SmokeFlips[];

// Ram_MapCellBuffer + 0xc58 から 910 個（その前には絵を読み込む）
#define MODE12_PARTICLES ((struct EffectStep *)(Ram_MapCellBuffer + 0xc58))

void BattleFx_InitializeMode12(struct BattleEffectArgument *efx)
{
	u8						 ramp[32];
	s32						 vec[3];
	struct EffectPosition	 pts[11];
	s32						 pos[3];
	s32						 scale[3];
	void				   **slots;
	s32						*ctrl;
	struct BattleEffectWork *work;
	void					*dst;
	u8						*aux;
	s32						 frame;
	DrawRectangle			 blit[2];
	s32						 size;
	s32						 count;
	s32						 i;
	s32						 j;
	s32						 n;
	struct EffectStep		*p;
	DrawRectangle			*cache;
	s32						 rings = 0x2710;

	slots = &gWorkSlot[HEAP_SLOT_BATTLE_BACKGROUND];
	ctrl = slots[0];
	work = slots[-5];
	dst = slots[-4];
	aux = slots[-3];
	work->effect = efx;
	BattleFx_BeginCanvasLayer(0x2000);
	REG_BG2PA = 0x100;
	Resource_LoadAndDecompress((s32)&ResourceId_LightFanSheet, work, 1, 1);
	Resource_LoadAndDecompress((s32)&ResourceId_LightFanSheet, work, 1, 1);
	Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesC, work->sheet + 0x1800, 0, 0);
	Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, aux, 0, 0);

	{
		s32 offset;
		for (i = 0, offset = 0; i != 8; i++)
		{
			u8 *src;
			for (j = 0, src = aux; j != 0x302; j++)
			{
				s32 v = *src++;

				if (v > 64 - i * 7)
				{
					v = 64 - i * 7;
				}
				if (v < 0)
				{
					v = 0;
				}
				(work->sheet + 10000)[offset + j] = v;
			}
			offset += j;
		}
	}

	BattlePres_ConfigureEffectDisplay();
	PLTT_BACKDROP = 0;
	*(u16 *)0x05000002 = 0;
	REG_BG2CNT = 0x2784;
	work->scroll_timer = 0;
	work->scroll_interval = 2;
	work->scroll_step_x = 1;
	work->scroll_step_y = 0;
	ctrl[4] = 1;
	Scheduler_AddOrUpdateCallback((s32)BattleFx_AdvanceScrollOnInterval, 0x480);
	Scheduler_AddOrUpdateCallback((s32)BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
	work->transfer_mode = 0;
	BattleEffect_WipeCanvas(0, 0);
	BattleBackground_LoadFar(1, (s32)&ResourceId_DuskCloudsBackdrop, 0);
	gProjection.center_y = 240;
	BattleEffect_WipeCanvas(0, 1);
	REG_BG2PA = 0x80;
	BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 3, 3);
	cache = (DrawRectangle *)gWorkSlot;
	blit[0] = cache[46];
	REG_DISPCNT = 0x7741;
	REG_BG2PA = 0x80;
	REG_BLDALPHA = 0x100f;
	REG_BLDCNT = 0x3f44;
	work->transfer_mode = 2;
	work->transfer_value = 75;

	for (i = 0; i != 16; i++)
	{
		ramp[i] = i * 8 + 3;
		ramp[31 - i] = i * 8 + 3;
		if (ramp[i] > 63)
		{
			ramp[i] = 63;
		}
		if (ramp[31 - i] > 63)
		{
			ramp[31 - i] = 63;
		}
	}
	REG_BG2CNT = 0x784;

	for (frame = 0; frame != 170; frame++)
	{
		size = 2;
		count = frame * 2;
		if (frame == 16)
		{
			AudioCommand_PlayFar(140);
		}
		if (frame == 132)
		{
			AudioCommand_PlayFar(131);
		}
		if (frame == 151)
		{
			AudioCommand_PlayFar(145);
		}
		if (gKeysRepeat & (KEY_A | KEY_B))
		{
			break;
		}
		Graphics_UpdatePhasePalette(frame * 2, 0xaaab, 0x5555, 0);
		if (frame == 150)
		{
			work->transfer_mode = 1;
			work->transfer_value = 0x1a1a1a1a;
		}
		else
		{
			work->transfer_mode = 2;
			work->transfer_value = 75;
		}
		Camera_StoreSceneParameters(0x1fe0000, Iwram_RatioMulQ14(0x1fe0000, 0xc000), 0x7fff0000);
		Render_ResetTransformState();
		if (frame > 128)
		{
			s32 a = frame - 128;
			if (a > 22)
			{
				a = 20;
			}
			pos[0] = 0;
			pos[1] = -a << 17;
			pos[2] = 0x2000000;
			size = (a >> 2) + 2;
			if (size > 8)
			{
				size = 8;
			}
			SceneTransform_ApplyPosition(pos);
			SceneTransform_ApplyPitch(-0x1000);
			SceneTransform_ApplyRoll(0x8000);
			SceneTransform_ApplyPitch(0x8000);
			SceneTransform_ApplyYaw(a << 12);
			if (frame > 150)
			{
				scale[0] = frame * 0x1800 - 0xd9400;
				scale[1] = scale[0];
				scale[2] = scale[0];
				size = 5;
			}
			else
			{
				scale[0] = 0x10000 - a * 3 * 0x400;
				scale[1] = scale[0];
				scale[2] = scale[0];
			}
			SceneTransform_ApplyScale(scale);
		}
		else
		{
			pos[0] = 0;
			pos[1] = 0;
			pos[2] = 0x1000000;
			SceneTransform_ApplyPosition(pos);
			SceneTransform_ApplyPitch(-0x1000);
			SceneTransform_ApplyRoll(frame << 8);
			SceneTransform_ApplyPitch(frame << 8);
		}

		if (frame <= 149)
		{
			for (i = 0; i != 6; i++)
			{
				vec[0] = (Mode12_Points[i][0] - 96) << 16;
				vec[1] = 0;
				vec[2] = (Mode12_Points[i][1] - 96) << 16;
				EffectPosition_ApplyBaseAndYOffset(vec, &pts[i]);
				pts[i].x = (pts[i].x >> 17) + 64;
				pts[i].y = (pts[i].y >> 16) + 60;
			}
			for (i = 0; i != 3; i++)
			{
				n = count * 2 - i * 48 - 256;
				if (n > 48)
				{
					n = 48;
				}
				if (n >= 0)
				{
					for (j = 0; j != n; j++)
					{
						s32 x = pts[i * 2].x
								+ Iwram_MulQ16(j * (pts[i * 2 + 1].x - pts[i * 2].x), 0x555);
						s32 y = pts[i * 2].y
								+ Iwram_MulQ16(j * (pts[i * 2 + 1].y - pts[i * 2].y), 0x555);
						blit[0](dst, aux + ParticleStreams_CellOffsets[size - 1], x - size / 2,
							y - size, size, size * 2);
					}
				}
			}
		}

		if (frame <= 179)
		{
			DrawRectangle blit47;
			s32			  lvl = 0;

			if (frame > 155)
			{
				lvl = frame - 156;
			}
			if (lvl > 7)
			{
				lvl = 7;
			}
			if (frame <= 139)
			{
				BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, 3, 3);
				blit47 = gBlitters[HEAP_SLOT_BLITTER_ALTERNATE];
			}
			else
			{
				BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, 3, 2);
				blit47 = gBlitters[HEAP_SLOT_BLITTER_ALTERNATE];
			}
			n = count;
			if (n > 128)
			{
				n = 128;
			}
			for (i = 0; i != n; i++)
			{
				s32 s = size + 1;
				vec[0] = Trig_Sin(i << 9) * 96;
				vec[2] = -(Trig_Cos(i << 9) * 96);
				EffectPosition_ApplyBaseAndYOffset(vec, &pts[0]);
				pts[0].x = (pts[0].x >> 17) + 64;
				pts[0].y = (pts[0].y >> 16) + 60;
				blit47(dst,
					work->sheet + (lvl * 0x302 + ParticleStreams_CellOffsets[s - 1]) + rings,
					pts[0].x - s / 2, pts[0].y - s, s, s * 2);
			}
			Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
		}

		if ((u32)(frame - 151) <= 16)
		{
			s32 w = 0;
			s32 h = (frame - 151) * 20 + 44;
			if (frame > 151)
			{
				w = frame - 152;
			}
			if (w > 15)
			{
				w = 15;
			}
			for (i = 1; i != 10; i++)
			{
				if (w + i <= 15)
				{
					blit[0](dst, &ramp[w + i], w + 48 + i, 16 - i, 32 - w * 2 - i * 2, 1);
				}
			}
			for (i = 0; i != h; i++)
			{
				s32 k = 32;
				/* FAKEMATCH: loop.c matches this 32 with the smoke calls' 32 (life 19, savings 2: 12 * 2 * 19 = 456 >= 450 insns) and hoists it out of the frame loop, so it loses its register and later reloads rotate; the native builds it in this loop's preheader. 2026-10-11: loop.c's threshold is 12 after an earlier move (savings 2 x life 19 x 12 = 456 >= 448 insns); a 96 local declared before the ramp loops lowers that to 432 and defeats the hoist, but the asm pair's two codeless preheader insns also raise i's live length to 1292, which orders n (28 refs/106) before i (195 refs) and gives n r9; without them i takes r9 and a slot at [sp, #60] appears (differ 699). */
				asm("" : "+r"(k));
				blit[0](dst, ramp + w, w + 48, i + 16, k - w * 2, 1);
			}
			BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, 3, 2);
			gBlitters[HEAP_SLOT_BLITTER_ALTERNATE](dst, work, 32, (frame - 151) * 20 - 12, 32, 96);
			Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
			BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, 7, 2);
			gBlitters[HEAP_SLOT_BLITTER_ALTERNATE](dst, work, 64, (frame - 151) * 20 - 12, 32, 96);
			Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
		}
		work->transfer_pending = 1;
		WaitFrames(1);
	}

	ctrl[4] = 0;
	Scheduler_RemoveCallback((u32)BattleFx_AdvanceScrollOnInterval);
	BattleEffect_SetupBlendedDisplay();
	BattleFx_SelectLivingTargets(work->effect);
	BattleFx_SpawnObjects(9, 0x173, 1);
	Resource_LoadAndDecompress((s32)&ResourceId_TornadoSheet, Ram_MapCellBuffer, 1, 0);
	Resource_LoadAndDecompress((s32)&ResourceId_LightningPillarSheet, work, 1, 1);
	Resource_LoadAndDecompress((s32)&ResourceId_VioletMoteSheet, work->sheet + 0x6000, 1, 0);
	Iwram_CopyWords((void *)0x05000000, Resource_GetTableEntry((s32)&ResourceId_CrystalSheet), 128);
	Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesB, aux, 0, 0);
	REG_BLDALPHA = 0x1010;
	{
		s32 cam_x;
		s32 cam_y;
		s32 vel_x;
		s32 vel_y;
		s32 cx;
		s32 cy;

		cam_x = 0x500000;
		cam_y = 0x400000;
		vel_x = 0;
		vel_y = 0;
		for (i = 0; i != 910; i++)
		{
			MODE12_PARTICLES[i].variant = 0;
		}
		for (i = 0; i != 16; i++)
		{
			work->particles[i].x = (Random16() & 31) - 16;
			work->particles[i].y = Random16() & 63;
			work->particles[i].variant = -(Random16() & 15);
		}
		work->transfer_mode = 2;
		work->transfer_value = 50;
		AudioCommand_PlayFar(145);
		cx = 60;
		cy = 112;

		for (frame = 0; frame != 192; frame++)
		{
			s32 d;
			if (frame == 20)
			{
				Resource_LoadAndDecompress((s32)&ResourceId_VioletLightningSheetB, work, 1, 0);
			}
			d = frame - 80;
			if ((u32)d <= 59 && !(frame & 7))
			{
				AudioCommand_PlayFar(134);
			}
			if (frame == 140)
			{
				BattleEventRuntime_BeginPhaseFar(134);
			}
			for (j = 0; j != 1; j++)
			{
				s32 start = j * 128 + 8;
				if (frame >= start && frame < j * 128 + 17)
				{
					if (frame >= j * 128 + 9 && frame < j * 128 + 12)
					{
						blit[0](dst, work->sheet, 36, 0, 48, cy);
					}
					if (frame >= j * 128 + 12 && frame < start + 8)
					{
						blit[0](dst, work->sheet + 0x1500, 36, 0, 48, 112);
					}
					if (frame == start + 2)
					{
						s32 n = 0;
						for (i = 0, p = MODE12_PARTICLES; i != 910; i++, p++)
						{
							if (p->variant == 0)
							{
								s32 speed = 0x3ff;
								s32 angle;
								speed &= Random16();
								angle = (Random16() & 0x7fff) - 0x4000;
								p->x = cx << 16;
								p->y = cy << 16;
								speed += 32;
								p->velocity_x = (Trig_Sin(angle) * speed) >> 7;
								p->velocity_y = -((Trig_Cos(angle) * speed) << 1) >> 7;
								p->variant = (Random16() & 7) + 32;
								if (++n == 512)
								{
									break;
								}
							}
						}
						work->shake_frames = 8;
					}
				}
			}
			if (frame == 10)
			{
				for (i = 0; i != work->effect->count; i++)
				{
					ObjectGroup_UpdateMembers(work->effect->actors[i], 7, 5, i, 8);
					BattleMotion_ApplyVariantMotionFar(work->effect->actors[i], 4);
				}
			}
			if ((u32)d <= 63)
			{
				s32 ox;
				s32 oy;
				s32 sx;
				for (i = 0; i != 6; i++)
				{
					if (frame == i * 8 + 80)
					{
						Iwram_FillWords(dst, 0x4000, 0x10101010);
					}
				}
				for (i = 0; i != 30; i++)
				{
					if (frame >= i * 2 + 80)
					{
						if (frame < i * 2 + 82)
						{
							s32 rx = (Random16() & 3) - 3;
							s32 ry = (Random16() & 3) + 32;
							BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, 3, 2);
							gBlitters[HEAP_SLOT_BLITTER_ALTERNATE](
								dst, work->sheet + i % 3 * 0x1440, rx, ry, 72, 72);
							Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
						}
						if (frame == i * 2 + 80)
						{
							s32 n = 0;
							for (j = 0, p = MODE12_PARTICLES; j != 910; j++, p++)
							{
								if (p->variant == 0)
								{
									s32 speed = 0x3ff;
									s32 angle;
									speed &= Random16();
									angle = Random16() & 0xffff;
									p->x = 0x100000;
									p->y = 0x500000;
									speed += 32;
									p->velocity_x = (Trig_Sin(angle) * speed) >> 7;
									p->velocity_y = -((Trig_Cos(angle) * speed) << 1) >> 7;
									p->variant = (Random16() & 7) + 32;
									if (++n == 16)
									{
										break;
									}
								}
							}
							work->shake_frames = 8;
							for (j = 0; j != work->effect->count; j++)
							{
								ObjectGroup_UpdateMembers(work->effect->actors[j],
									Mode12_Animations[Random16() & 3], 5, j, 4);
								BattleMotion_ApplyVariantMotionFar(work->effect->actors[j], 4);
							}
						}
					}
				}
				{
					s32 r = 7;
					s32 angle;
					r &= Random16();
					angle = Random16() & 0xffff;
					ox = (Trig_Sin(angle) * (r + 8)) >> 16;
					sx = ox + 72;
					oy = 32 - ((Trig_Cos(angle) * (r + 8)) >> 16);
				}
				BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, 3, 2);
				gBlitters[HEAP_SLOT_BLITTER_ALTERNATE](
					dst, work->sheet + 0x6000, ox + 60, oy - 24, 12, 24);
				Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
				BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, 7, 2);
				gBlitters[HEAP_SLOT_BLITTER_ALTERNATE](
					dst, work->sheet + 0x6000, sx, oy - 24, 12, 24);
				Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
				BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, 11, 2);
				gBlitters[HEAP_SLOT_BLITTER_ALTERNATE](
					dst, work->sheet + 0x6000, ox + 60, oy, 12, 24);
				Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
				BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, 15, 2);
				gBlitters[HEAP_SLOT_BLITTER_ALTERNATE](dst, work->sheet + 0x6000, sx, oy, 12, 24);
				Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
				j = 0;
				for (i = 0, p = MODE12_PARTICLES; i != 910; i++, p++)
				{
					if (p->variant == 0)
					{
						s32 speed = 63;
						s32 angle;
						speed &= Random16();
						angle = Random16() & 0xffff;
						p->x = sx << 16;
						p->y = oy << 16;
						speed += 64;
						p->velocity_x = (Trig_Sin(angle) * speed) >> 7;
						p->velocity_y = -((Trig_Cos(angle) * speed) << 1) >> 7;
						p->variant = (Random16() & 7) + 16;
						if (++j == 4)
						{
							break;
						}
					}
				}
			}

			BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, 15, 2);
			{
				DrawRectangle	   draw = gBlitters[HEAP_SLOT_BLITTER_ALTERNATE];
				struct EffectStep *q;
				for (i = 0, q = MODE12_PARTICLES; i != 910; i++, q++)
				{
					if (q->variant > 0)
					{
						q->variant--;
						EffectStep_AdvanceWithGravity2D(q, 60, 0);
						if (q->y > 0x780000)
						{
							q->velocity_y = -q->velocity_y / 2;
						}
						else if ((u32)q->x <= 0x7effff && q->y >= 0)
						{
							s32 x = q->x >> 16;
							s32 y = q->y >> 16;
							s32 s = q->variant / 16 + 1;
							draw(dst, aux + ParticleStreams_CellOffsets[s - 1], x - s / 2, y - s, s,
								s * 2);
						}
					}
				}
			}
			Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);

			if (frame > 15)
			{
				if (frame == 32)
				{
					vel_x = 0x40000;
					vel_y = -0x4000;
				}
				if (frame > 31)
				{
					cam_x += vel_x;
					cam_y += vel_y;
					vel_x = vel_x * 60 / 64;
					vel_y = vel_y * 60 / 64;
				}
				BattleFx_PlaceFormationObjects(0, cam_x, cam_y);
			}
			if ((u32)(frame - 16) <= 127)
			{
				s32 base;
				for (i = 0, base = cam_x >> 17; i != 3; i++)
				{
					s32 k = i & 3;
					s32 angle = Random16() & 0xffff;
					s32 x =
						((Trig_Sin(angle) << 3) >> 16) + base - (BattleFx_GlintCellWidths[k] >> 1);
					s32 y = ((Trig_Cos(angle) * 40) >> 16) - (BattleFx_GlintCellHeights[k] >> 1);
					BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7,
						3 | Mode12_SmokeFlips[Random16() & 3], 2);
					gBlitters[HEAP_SLOT_BLITTER_ALTERNATE](dst,
						Ram_MapCellBuffer + BattleFx_GlintCellOffsets[k], x, y + 56,
						BattleFx_GlintCellWidths[k], BattleFx_GlintCellHeights[k]);
					Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
				}
			}
			if (frame <= 31)
			{
				Camera_ApplyShake(8, 16);
			}
			else
			{
				Camera_ApplyShake(4, 4);
			}
			ObjectGroup_TickMemberTimers();
			work->transfer_pending = 1;
			WaitFrames(1);
		}

		Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
		Scheduler_RemoveCallback((u32)BattlePresentation_ProcessPendingGraphicsTransfer);
		BattleEffect_RunImpactBurst(0, cam_x, cam_y);
	}
	for (i = 0; i != 9; i++)
	{
		ResourceObject_ReleaseFar((struct ResourceObjectWork *)work->objects[i]);
	}
	BattleFx_EndCanvasLayer();
}

void BattleFx_RunShatteringRocks(struct BattleEffectArgument *object)
{
	void				   **heap_cache;
	void				   **cursor;
	struct BattleEffectWork *work;
	void					*canvas;
	DrawRectangle			 blit;
	s32						 rows;
	struct EffectPosition	 first;
	struct EffectPosition	 final;
	struct EffectStep		*rock;
	struct EffectStep		*shard;
	s32						 frame;
	s32						 i;
	s32						 j;
	s32						 cell;
	u32						 width;
	u32						 height;

	heap_cache = (void **)gBattleFxWork;
	cursor = heap_cache;
	work = *cursor++;
	canvas = *cursor;
	work->effect = object;
	BattleFx_BeginCanvasLayer(1);
	REG_BG2PA = 0x100;
	REG_BLDCNT = 0;
	Resource_LoadAndDecompress((s32)&ResourceId_BoulderSheet, work->sheet, 1, 1);
	BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 3, 1);
	blit = heap_cache[7];
	EffectPosition_ApplyStepAndYOffset(work->effect->actors[0], &first);
	EffectPosition_ApplyStepAndYOffset(work->effect->actors[work->effect->count - 1], &final);
	first.x += (final.x - first.x) / 2;
	REG_BG2X = (64 - first.x) << 8;
	work->transfer_mode = 1;
	work->transfer_value = 0;
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
	rows = ShatterRocks_RockCounts[work->effect->variant];
	for (i = 0; i != rows; i++)
	{
		rock = &work->particles[i];
		rock->y = -0x400000;
		rock->velocity_y = 0;
	}
	for (i = 0; i != rows; i++)
	{
		for (j = 0; j != 21; j++)
		{
			struct EffectStep *seed = &PARTICLES[i * 21 + j];

			seed->x = (ShatterRocks_ShardOffsets[j * 2] + ShatterRocks_RockX[i]) << 16;
			seed->y = ShatterRocks_ShardOffsets[j * 2 + 1] << 16;
			seed->velocity_x = ((Random16() % 96) - 48) << 10;
			seed->velocity_y = -((Random16() & 127) + 32) << 11;
			seed->z = 32;
			seed->variant = 0;
		}
	}
	for (frame = 0; frame != ShatterRocks_DropFrames[rows - 1] + 80; frame++)
	{
		if (frame == ShatterRocks_DropFrames[rows - 1] + 48)
			BattleEventRuntime_BeginPhaseFar(132);
		for (i = 0; i != rows; i++)
		{
			rock = &work->particles[i];
			if (frame == ShatterRocks_DropFrames[i] + 18)
			{
				AudioCommand_PlayFar(134);
				work->shake_frames = 4;
			}
			if (frame >= ShatterRocks_DropFrames[i] + 18)
			{
				for (j = 0; j != 21; j++)
				{
					shard = &PARTICLES[i * 21 + j];
					cell = (j % 5) * 3 + shard->variant / 96 % 3;
					blit(canvas, work->sheet + ShatterRocks_ShardCells[cell] + 0x83c,
						HI(shard->x) - ((width = ShatterRocks_ShardWidths[cell]) >> 1),
						HI(shard->y) - ((height = ShatterRocks_ShardHeights[cell]) >> 1), width,
						height);
					EffectStep_AdvanceWithGravity2D(shard, 64, 0x4000);
					shard->variant += shard->z;
					if (shard->z > 1 && (frame & 1))
						shard->z--;
				}
			}
			if (frame < ShatterRocks_DropFrames[i] + 18)
			{
				if (frame >= ShatterRocks_DropFrames[i])
					blit(canvas, work, ShatterRocks_RockX[i] + 47, HI(rock->y), 34, 62);
				rock->y += rock->velocity_y;
				if (frame > ShatterRocks_DropFrames[i])
					rock->velocity_y += 0x10000;
				if (rock->y > 0x320000)
					rock->y = 0x320000;
			}
			if (frame == ShatterRocks_DropFrames[i] + 18)
			{
				for (j = 0; j != work->effect->count; j++)
					ObjectGroup_UpdateMembers(work->effect->actors[j], 7, 5, j, 8);
			}
		}
		Camera_ApplyShake(2, 4);
		ObjectGroup_TickMemberTimers();
		work->transfer_pending = 1;
		WaitFrames(1);
	}
	Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
	BattleFx_EndCanvasLayer();
}

//--------------------------------------------------------------------------

// animation_modes.c

// 演出：バースト演出の入口

//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
// 演出ごとの入口。バーストの型を BattlePres_RunBurstScene に渡す
// step : 演出の引数
//--------------------------------------------------------------------------
void EffectStep_RunAnimationMode6Or7Or8(struct EffectStep *step)
{
	if (step->variant == 0)
	{
		BattlePres_RunBurstScene(step, 6);
		return;
	}
	if (step->variant == 1)
	{
		BattlePres_RunBurstScene(step, 7);
		return;
	}
	BattlePres_RunBurstScene(step, 8);
}

void EffectStep_RunAnimationMode0(struct EffectStep *step)
{
	BattlePres_RunBurstScene(step, 0);
}

void EffectStep_RunAnimationMode0Alternate(struct EffectStep *step)
{
	BattlePres_RunBurstScene(step, 0);
}

void EffectStep_RunAnimationMode1(struct EffectStep *step)
{
	BattlePres_RunBurstScene(step, 1);
}

void EffectStep_RunAnimationMode2(struct EffectStep *step)
{
	BattlePres_RunBurstScene(step, 2);
}

void EffectStep_RunAnimationMode3Or4Or5(struct EffectStep *step)
{
	if (step->variant == 0)
	{
		BattlePres_RunBurstScene(step, 3);
		return;
	}
	if (step->variant == 1)
	{
		BattlePres_RunBurstScene(step, 4);
		return;
	}
	BattlePres_RunBurstScene(step, 5);
}

//--------------------------------------------------------------------------

// burst_scene.c

// バトル演出：対象に重ねて弾ける連撃と、せり上がる岩の壁

//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
// 対象に何度も重ねて弾ける連撃
// BurstScene_Records は variant ごとに 7 バイト
//   [0] 弧の絵（0 なら BlueArcSheetA、ほかは B。1 ならすべての回を弧で描く）
//   [1] パレットを取るリソース（0〜3）
//   [2] 回数
//   [3] 1 回で飛ばす粒の数
//   [4] 回と回の間（フレーム）
//   [5] 全体のフレーム数
//   [6] 1 なら当たるたびにキャンバスを塗りつぶす
//--------------------------------------------------------------------------
void BattlePres_RunBurstScene(struct BattleEffectArgument *effect, s32 variant)
{
	struct EffectPosition	 actor_pos;
	struct EffectPosition	 target_pos;
	struct EffectPosition	 pos;
	s32						 screen_x;
	s32						 screen_y;
	DrawRectangle			 draw[2];
	struct BattleEffectWork *work;
	void					*canvas;
	s32						 frame;
	u8						*sprites;
	struct BattleCamera		*camera;
	struct MotionObject		*target;
	u8						*pltt;
	s32						 res;
	s32						 row;
	s32						 col;
	s32						 i;
	s32						 j;

	work = *(gWorkSlot + HEAP_SLOT_BATTLE_EFFECT);
	canvas = *(gWorkSlot + HEAP_SLOT_BATTLE_CANVAS);
	sprites = *(gWorkSlot + HEAP_SLOT_BATTLE_SHEET);
	camera = *(gWorkSlot + HEAP_SLOT_CAMERA);
	work->effect = effect;
	BattleFx_BeginCanvasLayer(0);
	REG_BLDALPHA = 0x1010;
	if (work->effect->prelude == 1)
		BattleFx_PrepareCanvasEffect(effect, 7, work->effect->side, 2, &screen_x, &screen_y);

	//------------------------------------------------------------------
	// 絵とパレット
	//------------------------------------------------------------------
	Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sprites, 0, 0);

	// 玉（40x288）を横半分に詰めて 0x5100 から置く（キャンバスは横 2 倍で見せる）
	Resource_LoadAndDecompress((s32)&ResourceId_YellowOrbSheet, work->sheet, 1, 0);
	for (row = 0; row != 288; row++)
	{
		for (col = 0; col != 40; col++)
		{
			s32 src = row * 40 + col;
			s32 dst = row * 20 + col / 2 + 0x5100;

			work->sheet[dst] = work->sheet[src];
		}
	}
	if (BurstScene_Records[variant * 7] == 0)
		Resource_LoadAndDecompress((s32)&ResourceId_BlueArcSheetA, work->sheet, 1, 1);
	else
		Resource_LoadAndDecompress((s32)&ResourceId_BlueArcSheetB, work->sheet, 1, 1);
	Resource_LoadAndDecompress((s32)&ResourceId_WaveSheet, Ram_MapCellBuffer + 0x5e00, 1, 0);
	switch (BurstScene_Records[variant * 7 + 1])
	{
	case 0:
		res = (s32)&ResourceId_MarsDjinnSheet;
		break;
	case 1:
		res = (s32)&ResourceId_LimePalette;
		break;
	case 2:
		res = (s32)&ResourceId_BlueArcSheetB;
		break;
	default:
		res = (s32)&ResourceId_EmberStreakSheet;
		break;
	}
	pltt = Resource_GetTableEntry(res);
	Iwram_CopyWords((void *)BG_PLTT, pltt, 128);
	WaitFrames(1);

	EffectPosition_ApplyStepAndYOffset(work->effect->actors[0], &target_pos);
	work->transfer_mode = 2; // 残像 3/4
	work->transfer_value = 75;
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);

	// 粒：対象の 25 ドット上に用意しておく（x の速さは原点の側へ向ける）
	target = GetBattleObjectSlotFar(work->effect->actors[0])->object;
	for (i = 0; i != 768; i++)
	{
		struct EffectStep *mote = &PARTICLES[i];

		mote->x = target->x;
		mote->y = target->y + 0x190000;
		mote->z = target->z;
		mote->velocity_x = (Random16() & 255) << 12;
		mote->velocity_y = ((Random16() & 255) - 127) << 12;
		mote->velocity_z = ((Random16() & 255) - 127) << 12;
		if (mote->x > 0)
			mote->velocity_x = -mote->velocity_x;
		mote->variant = -1; // 寿命（負なら出ていない）
	}
	BattleMotion_ApproachTargetFar(work->effect->actor, work->effect->actors[0], 4, 0);

	for (frame = 0; frame != BurstScene_Records[variant * 7 + 5]; frame++)
	{
		s32 bursts = BurstScene_Records[variant * 7 + 2];

		EffectPosition_ApplyStepAndYOffset(work->effect->actor, &actor_pos);
		actor_pos.x /= 2;

		// 弧と波：奇数回は上下反転で描く
		if (work->effect->side == 0)
		{
			BattleEffect_LoadWork(
				HEAP_SLOT_BLITTER, 7, 7, GOUSEI_CLIP_X | GOUSEI_CLIP_Y, GOUSEI_HIKAKU);
			BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7,
				GOUSEI_CLIP_X | GOUSEI_CLIP_Y | GOUSEI_FLIP_Y, GOUSEI_HIKAKU);
		}
		else
		{
			BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7,
				GOUSEI_CLIP_X | GOUSEI_CLIP_Y | GOUSEI_FLIP_X, GOUSEI_HIKAKU);
			BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7,
				GOUSEI_CLIP_X | GOUSEI_CLIP_Y | GOUSEI_FLIP, GOUSEI_HIKAKU);
		}
		draw[0] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER];
		draw[1] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE];
		for (i = 0; i != bursts; i++)
		{
			s32 start = i * BurstScene_Records[variant * 7 + 4];

			if (frame >= start && frame < start + 6)
			{
				s32 step = frame - start;

				if ((i & 3) <= 1 || BurstScene_Records[variant * 7] == 1)
				{
					// 弧（48x72 を 6 コマ）
					if (work->effect->side == 0)
						draw[i & 1](canvas, work->sheet + step * 3456, target_pos.x / 2 - 16,
							target_pos.y - 40, 48, 72);
					else
						draw[i & 1](canvas, work->sheet + step * 3456, target_pos.x / 2 - 32,
							target_pos.y - 40, 48, 72);
				}
				else
				{
					// 波（48x16 を 6 コマ）
					if (work->effect->side == 0)
						draw[i & 1](canvas, Ram_MapCellBuffer + 0x5e00 + step * 768,
							target_pos.x / 2 - 16, actor_pos.y - 8, 48, 16);
					else
						draw[i & 1](canvas, Ram_MapCellBuffer + 0x5e00 + step * 768,
							target_pos.x / 2 - 32, actor_pos.y - 8, 48, 16);
				}
			}
			if (frame == start + 2)
			{
				// 当たり：最後の回で締める
				if (BurstScene_Records[variant * 7 + 6] == 1)
					Iwram_FillWords(canvas, 0x4000, 0x2f2f2f2f);
				ObjectGroup_UpdateMembers(work->effect->actors[0], 7, 5, 0, 4);
				if (i == bursts - 1)
				{
					BattleMotion_ApplyVariantMotionFar(work->effect->actors[0], 4);
					work->shake_frames = 8;
					BattleEventRuntime_BeginPhaseFar(134);
				}
				else
				{
					if (i & 1)
						BattleMotion_ApplyVariantMotionFar(work->effect->actors[0], 7);
					work->shake_frames = 4;
					AudioCommand_PlayFar(134);
				}
				for (j = 0; j != BurstScene_Records[variant * 7 + 3]; j++)
					PARTICLES[i * 32 + j].variant = (Random16() & 7) + 15;
			}
			// 玉（20x48 を 6 コマ、2 フレームずつ）
			if (frame >= start + 2 && frame < start + 14)
				draw[0](canvas, work->sheet + ((frame - start - 2) / 2) * 960 + 0x5100,
					target_pos.x / 2 - 10, target_pos.y - 24, 20, 48);
		}
		Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
		Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);

		// 粒：寿命が尽きるまで描き、地面で跳ね返す
		Render_ResetTransformState();
		Graphics_PrepareTransferInIwramWork(camera->eye, camera->pos);
		BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, GOUSEI_CLIP_X | GOUSEI_CLIP_Y, GOUSEI_KASAN);
		BattleEffect_LoadWork(
			HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, GOUSEI_CLIP_X | GOUSEI_CLIP_Y, GOUSEI_HIKAKU);
		draw[0] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER];
		draw[1] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE];
		for (i = 0; i != 512; i++)
		{
			struct EffectStep *mote = &PARTICLES[i];

			s32 size = mote->variant;

			if (size > 0)
			{
				EffectPosition_ApplyBaseAndYOffset(&mote->x, &pos);
				pos.x /= 2;
				size >>= 3;
				size++;
				draw[(i / 2) & 1](canvas, sprites + ParticleStreams_CellOffsets[size - 1],
					pos.x - size / 2, pos.y - size, size, size * 2);
				EffectStep_AdvanceWithGravity3D(mote, 60, -0x400);
				if (mote->y < 0x80000)
					mote->velocity_y = -mote->velocity_y / 2;
				mote->variant--;
			}
		}
		Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
		Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
		Camera_ApplyShake(8, 8);
		ObjectGroup_TickMemberTimers();
		work->transfer_pending = 1;
		WaitFrames(1);
	}
	Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
	BattleFx_EndCanvasLayer();
}

extern const struct Scale RisingWall_UnitScale;

//--------------------------------------------------------------------------
// 地面が盛り上がり、岩の壁がせり上がって、しばらくして沈む
// 壁は左右対称で、左半分を描いてから右半分を左右反転で描く
// 壁の中心はキャンバスの x = 64 + wall_x
// 岩は 11 個のアニメーションオブジェクト（動きは particles[10〜20]）
//--------------------------------------------------------------------------
void BattlePres_RunRisingWall(struct BattleEffectArgument *effect)
{
	void				   **heap;
	void				   **cursor;
	struct BattleEffectWork *work;
	void					*canvas;
	DrawRectangle			 draw[2];
	u8						*sprites;
	s32						 wall_x;
	s32						 rock_x;
	struct Scale			 scale;
	s32						 pos[4];
	u8						*src;
	u16						*col;
	void					*pltt = (void *)BG_PLTT;
	s32						 n;
	s32						 frame;
	s32						 i;
	s32						 j;
	s32						 k;

	heap = (void **)gBattleFxWork;
	cursor = heap;
	work = *cursor++;
	canvas = *cursor;
	sprites = heap[2];
	work->effect = effect;
	BattleFx_BeginCanvasLayer(0);

	// variant が大きいほど壁を横に広げる
	if (work->effect->variant == 0)
		REG_BG2PA = 204; // 1.25 倍
	else if (work->effect->variant == 1)
		REG_BG2PA = 170; // 1.5 倍
	if (work->effect->side == 1)
	{
		wall_x = 8;
		if (work->effect->variant == 0)
			rock_x = 40;
		else if (work->effect->variant == 1)
			rock_x = 36;
		else
			rock_x = 40;
	}
	else
	{
		wall_x = -16;
		rock_x = -12;
	}
	REG_BG2X = RisingWall_Bg2Shifts[work->effect->side][work->effect->variant] << 8;

	//------------------------------------------------------------------
	// パレット：BG の 1〜63 番を各成分 8 段暗くする
	//------------------------------------------------------------------
	Resource_LoadAndDecompress((s32)&ResourceId_EarthWallSheet, Ram_MapCellBuffer, 1, 0);
	Iwram_CopyWords(pltt, Resource_GetTableEntry((s32)&ResourceId_OrangePaletteA), 128);
	col = (u16 *)&BG_PLTT[1];
	for (i = 0; i != 63; i++)
	{
		s32 c = *col;
		s32 b = ((u16)c >> 10) & 31;
		s32 g = ((u16)c >> 5) & 31;
		s32 r = 31;

		r &= c;
		b -= 8;
		g -= 8;
		r -= 8;

		if (b < 0)
			b = 0;
		if (g < 0)
			g = 0;
		if (r < 0)
			r = 0;
		*col++ = (b << 10) | (g << 5) | r;
	}

	//------------------------------------------------------------------
	// 絵を work->sheet に並べ直す
	//   伸びていく壁   : 1 ライン × 9 段を、それぞれ 32 ラインに伸ばす
	//   壁の上 0x13c0  : 48x32 を縦 3 倍にして 48x96
	//   壁の下 0x25c0  : 48x21
	//   盛り上がり     : 3 コマ
	//------------------------------------------------------------------
	src = Ram_MapCellBuffer;
	n = 0;
	for (j = 0; j != 9; j++)
	{
		for (i = 0; i != 32; i++)
		{
			for (k = 0; k != RisingWall_RowWidths[j]; k++, n++)
				work->sheet[n] = src[k];
		}
		src += RisingWall_RowWidths[j];
	}
	for (j = 0; j != 32; j++)
	{
		for (i = 0; i != 3; i++)
		{
			for (k = 0; k != 48; k++, n++)
				work->sheet[n] = src[k];
		}
		src += 48;
	}
	for (i = 0; i != 1008; i++)
		work->sheet[n++] = *src++;
	for (i = 0; i != 3; i++)
	{
		for (k = 0; k != RisingWall_MoundWidths[i] * RisingWall_MoundHeights[i]; k++)
			work->sheet[n++] = *src++;
	}
	Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sprites, 0, 0);

	// 岩のオブジェクト
	for (i = 0; i != 11; i++)
	{
		struct AnimationObject *obj = ResourceObject_CreateFar(390);

		work->objects[i] = obj;
		if (obj != 0)
		{
			obj->flags = 0;
			AnimationObjects_SelectAnimationFar(obj, i / 4);
			((struct AnimationObject *)work->objects[i])->part[0].priority = 1;
		}
	}
	BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, GOUSEI_CLIP_X | GOUSEI_CLIP_Y, GOUSEI_HIKAKU);
	draw[0] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER];
	BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7,
		GOUSEI_CLIP_X | GOUSEI_CLIP_Y | GOUSEI_FLIP_X, GOUSEI_HIKAKU);
	draw[1] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE];
	REG_BLDCNT = 0x3f46;
	REG_BLDALPHA = 0x1010;
	work->transfer_mode = 2; // 残像 3/4
	work->transfer_value = 75;
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);

	// 岩：x は列、y は 128 から z ずつ減らす数え、
	// velocity_x は 1〜12 を回る数え、variant は出るまでの待ち
	for (i = 0; i != 11; i++)
	{
		struct EffectStep *rock = &work->particles[10 + i];

		rock->x = (Random16() & 15) + 88;
		rock->y = 128;
		rock->z = (Random16() & 15) + 2;
		rock->velocity_x = i;
		rock->velocity_y = 1;
		rock->velocity_z = 0x8000;
		rock->variant = 44 - i * 4;
	}
	for (i = 0; i != 6; i++)
		work->particles[16 + i].x = RisingWall_LateRockColumns[i];

	// 土ぼこり：壁の根もとから上へ昇る
	for (i = 0; i != 256; i++)
	{
		struct EffectStep *dust = &PARTICLES[i];

		dust->x = ((Random16() & 63) + wall_x + 32) << 16;
		dust->y = ((Random16() & 7) + 96) << 16;
		dust->velocity_y = ((Random16() & 63) + 32) << 13;
		dust->variant = Random16() & 31;
	}
	REG_BG2CNT = 0x785;
	work->shake_frames = 250;
	for (frame = 0; frame != 192; frame++)
	{
		if (frame == 0)
			AudioCommand_PlayFar(212);
		if (frame == 40)
			AudioCommand_PlayFar(141);
		if (frame == 96)
			AudioCommand_PlayFar(145);
		if (frame == 120)
			BattleEventRuntime_BeginPhaseFar(134);

		// 盛り上がる地面（0〜81）
		if (frame <= 81)
		{
			s32 cell = frame / 4;

			if (cell > 2)
				cell = (cell & 1) + 1;
			draw[0](canvas, work->sheet + RisingWall_MoundSheetOffsets[cell],
				wall_x - RisingWall_MoundWidths[cell] + 64, 116 - RisingWall_MoundHeights[cell],
				RisingWall_MoundWidths[cell], RisingWall_MoundHeights[cell]);
			draw[1](canvas, work->sheet + RisingWall_MoundSheetOffsets[cell], wall_x + 64,
				116 - RisingWall_MoundHeights[cell], RisingWall_MoundWidths[cell],
				RisingWall_MoundHeights[cell]);
		}

		// せり上がる壁（12〜87）
		if (frame >= 12 && frame <= 87)
		{
			s32 row = (frame - 64) / 3;

			if (row < 0)
				row = 0;
			if (row > 7)
				row = 7;
			for (i = 0; i != 4; i++)
			{
				draw[0](canvas, work->sheet + RisingWall_RowSheetOffsets[row],
					wall_x - RisingWall_RowWidths[row] + 64, i * 32 - 12, RisingWall_RowWidths[row],
					32);
				draw[1](canvas, work->sheet + RisingWall_RowSheetOffsets[row], wall_x + 64,
					i * 32 - 12, RisingWall_RowWidths[row], 32);
			}
		}

		// 沈む壁（160〜183）
		if (frame >= 160 && frame <= 183)
		{
			s32 row = 7 - (frame - 160) / 3;

			if (row < 0)
				row = 0;
			if (row > 7)
				row = 7;
			for (i = 0; i != 4; i++)
			{
				draw[0](canvas, work->sheet + RisingWall_RowSheetOffsets[row],
					wall_x - RisingWall_RowWidths[row] + 64, i * 32 - 12, RisingWall_RowWidths[row],
					32);
				draw[1](canvas, work->sheet + RisingWall_RowSheetOffsets[row], wall_x + 64,
					i * 32 - 12, RisingWall_RowWidths[row], 32);
			}
		}

		// 立ち切った壁（88〜159）
		if (frame >= 88 && frame <= 159)
		{
			s32 width = 48;

			draw[0](canvas, &work->sheet[0x13c0], wall_x + 16, 0, width, 96);
			draw[1](canvas, &work->sheet[0x13c0], wall_x + 64, 0, width, 96);
			draw[0](canvas, &work->sheet[0x25c0], wall_x + 16, 96, width, 21);
			draw[1](canvas, &work->sheet[0x25c0], wall_x + 64, 96, width, 21);
		}

		// 土ぼこり（88〜）：上に抜けたら根もとへ戻す（159 まで）
		if (frame > 87)
		{
			for (i = 0; i != 64; i++)
			{
				struct EffectStep *dust = &PARTICLES[i];

				if (dust->variant == 0)
				{
					s32 size = (i & 3) + 5;

					draw[0](canvas, sprites + ParticleStreams_CellOffsets[size - 1],
						HI(dust->x) - size / 2, HI(dust->y) - size, size, size * 2);
					dust->y -= dust->velocity_y;
					if (dust->y < 0 && frame <= 159)
						dust->y = 0x600000;
				}
				else
				{
					dust->variant--;
				}
			}
		}

		// 岩（5〜）：72 フレーム目からは 11 個に増やし、大きくする
		if (frame > 4)
		{
			s32 rocks = 6;

			if (frame > 71)
				rocks = 11;
			for (k = 0; k != rocks; k++)
			{
				struct EffectStep *rock = &work->particles[10 + k];
				s32				   slot;

				if (rock->variant == 0)
				{
					scale = RisingWall_UnitScale;
					if (frame > 71)
					{
						scale.x = (k << 12) + 0x8000;
						scale.x += work->effect->variant << 14;
					}
					else
					{
						scale.x = 0x8000;
					}
					scale.y = scale.x;
					pos[3] = 0;
					pos[0] = (rock->x + rock_x * 2) << 16;
					pos[1] = 0x2000000 - (rock->y << 16);
					pos[2] = 0x2000000;
					slot = (frame / 2 + k) % 11;
					if (slot != -1)
						Object_ApplyProjectedPlacementFar(work->objects[slot], pos, &scale, 0);
					rock->y -= rock->z;
					rock->velocity_x += rock->velocity_y;
					if (rock->velocity_x > 12)
						rock->velocity_x -= 12;
					if (rock->y < 0)
					{
						// y が 0 を切ったら 128 から出し直す（88 フレーム目からは列と速さも選び直す）
						if (frame > 159)
						{
							rock->variant = -1;
						}
						else
						{
							if (frame > 87)
							{
								rock->z = (Random16() & 7) + 8;
								if (work->effect->variant == 0)
									rock->x = Random16() % 96 + 42;
								else if (work->effect->variant == 1)
									rock->x = Random16() % 112 + 34;
								else
									rock->x = Random16() % 160 + 10;
							}
							rock->y = 128;
							rock->variant = 8;
						}
					}
				}
				else
				{
					rock->variant--;
				}
			}
		}

		// 対象（86〜158）
		if (frame <= 158)
		{
			for (i = 0; i != work->effect->count; i++)
			{
				if (frame > 85)
				{
					if (frame % 12 == 0)
						ObjectGroup_UpdateMembers(work->effect->actors[i], 7, 5, i, 6);
					if ((frame & 3) == 0)
						BattleMotion_ApplyVariantMotionFar(work->effect->actors[i], 5);
				}
			}
		}
		if (frame < 90 || frame > 160)
			Camera_ApplyShake(2, 2);
		else
			Camera_ApplyShake(8, 8);
		ObjectGroup_TickMemberTimers();
		work->transfer_pending = 1;
		WaitFrames(1);
	}
	Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
	for (i = 0; i != 11; i++)
		ResourceObject_ReleaseFar(work->objects[i]);
	BattleFx_EndCanvasLayer();
}
