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
#include "heap_state.h"
#include "canvas.h"
#include "resource.h"
#include "btldisp.h"
#include "types.h"
#include "iwram_call.h"
#include "call.h"
#include "battle_efx.h"
#include "battle_effect_work.h"
#include "callback_scheduler.h"
#include "fixed_math.h"
#include "resource_ids.h"
#include "effect_step.h"
#include "motobj.h"
#include "ram_buffer.h"
#include "io_reg.h"
#include "transform.h"

#define SceneTransform_ApplyPosition ((void (*)(s32 *))SceneTransform_ApplyPosition)
#define Camera_ApplyShake ((void (*)(s32, s32))Camera_ApplyShake)

void AudioCommand_PlayFar(s32 cue);

#define CONVERGES(mode) ((mode) <= 1 || (mode) == 4 || (mode) == 5 || (mode) == 6 || (mode) == 7)
#define DECAYS(mode) ((mode) <= 1 || (mode) == 4 || (mode) == 5 || (mode) == 6)

enum
{
	RENDER_MODE_TORNADO = 0x2580
};

void BattleFx_RenderMode(struct BattleEffectArgument *effect, u32 mode)
{
	void				   **cursor;
	struct BattleEffectWork *work;
	void					*canvas;
	s32						 frame;
	s32						 total;
	s32						 member;
	s32						 count;
	struct EffectPosition	 screen;
	s32						 point[3];
	struct EffectPosition	 base;
	DrawRectangle			 draw[2];
	void					*palette;
	s32						*row;
	s32						 i;

	cursor = (void **)gBattleFxWork;
	work = *cursor++;
	canvas = *cursor;
	count = 16;
	work->effect = effect;
	BattleFx_BeginCanvasLayer(0);
	Resource_LoadAndDecompress((s32)&ResourceId_SmokeSheet, work->sheet, 1, 1);
	if (mode == 0)
	{
		palette = Resource_GetTableEntry((s32)&ResourceId_LimePalette);
	}
	else if (mode == 1)
	{
		palette = Resource_GetTableEntry((s32)&ResourceId_PinkBurstSheet);
	}
	else if (mode == 2)
	{
		palette = Resource_GetTableEntry((s32)&ResourceId_CyanPalette);
	}
	else if (mode == 3)
	{
		palette = Resource_GetTableEntry((s32)&ResourceId_BlastSheet);
	}
	else if (mode == 4)
	{
		palette = Resource_GetTableEntry((s32)&ResourceId_PinkBurstSheet);
	}
	else if (mode == 5)
	{
		Resource_LoadAndDecompress((s32)&ResourceId_GlowOrbSheet, work->sheet, 1, 0);
		palette = Resource_GetTableEntry((s32)&ResourceId_GlowOrbSheet);
	}
	else if (mode == 7)
	{
		count = 24;
		Resource_LoadAndDecompress((s32)&ResourceId_GlowOrbSheet, work->sheet, 1, 0);
		palette = Resource_GetTableEntry((s32)&ResourceId_MarsDjinnSheet);
	}
	else
	{
		count = 32;
		Resource_LoadAndDecompress((s32)&ResourceId_VortexSheet, work->sheet, 1, 0);
		palette = Resource_GetTableEntry((s32)&ResourceId_VortexSheet);
	}
	/* FAKEMATCH: needed to match: forwarded through Call3, the palette copy loads its routine before it shifts the destination, as the reference does */
	Iwram_CopyWords((s32)BG_PLTT, (s32)palette, 128);
	if (mode == 4)
		Resource_LoadAndDecompress((s32)&ResourceId_HeartSheet, work->sheet, 1, 1);
	if (mode == 3)
		Resource_LoadAndDecompress(
			(s32)&ResourceId_TornadoSheet, work->sheet + RENDER_MODE_TORNADO, 1, 0);

	for (i = 0; i != 512; i++)
	{
		struct EffectStep *mote = &((struct EffectStep *)Ram_MapCellBuffer)[i];

		if (CONVERGES(mode))
		{
			mote->x = ((Random16() & 0xff) - 127) << 15;
			mote->y = ((Random16() & 0xff) - 127) << 14;
			mote->z = ((Random16() & 0xff) - 127) << 15;
		}
		else
		{
			mote->x = ((Random16() & 0xff) - 127) << 13;
			mote->y = ((Random16() & 0xff) - 255) << 13;
			mote->z = ((Random16() & 0xff) - 127) << 13;
		}
		mote->variant = 0;
	}

	if (CONVERGES(mode))
		total = work->effect->count * 8 + 64;
	else
		total = work->effect->count * 8 + 32;
	if (!(mode <= 1 || mode == 3))
		Scheduler_AddOrUpdateCallback(BattleFx_ArmBg2AffineHBlankDma, 0x480);
	work->transfer_mode = 2;
	work->transfer_value = 75;
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
	AudioCommand_PlayFar(142);

	for (frame = 0; frame != total; frame++)
	{
		struct BattleCamera *camera = gCameraWork;

		if (mode == 7)
		{
			if (frame == total - 46)
				BattleMotion_ApproachTargetFar(work->effect->actor, work->effect->actors[0], 16, 0);
			if (frame == total - 32)
			{
				BattleEventRuntime_BeginPhaseFar(134);
				BattleMotion_ApplyVariantMotionFar(work->effect->actors[0], 4);
				work->shake_frames = 8;
			}
		}
		else if (frame == total - 32)
		{
			BattleEventRuntime_BeginPhaseFar(133);
		}
		row = work->bg2_x;
		for (i = 0; i != 160; i++)
			*row++ = (0x40000 - (Trig_Sin((frame << 12) + i * 0x800) << 2)) >> 10;

		for (member = 0; member != work->effect->count; member++)
		{
			struct MotionObject *object =
				GetBattleObjectSlotFar(work->effect->actors[member])->object;

			if (mode == 3 && frame > member * 8 && frame < member * 8 + 32)
			{
				EffectPosition_ApplyStepAndYOffset(work->effect->actors[member], &base);
				for (i = 0; i != 2; i++)
				{
					s32 angle;
					s32 radius;
					s32 x;
					s32 y;

					angle = Random16() & 0xffff;
					radius = (Random16() & 31) + 4;
					x = base.x / 2 + ((Trig_Sin(angle) * radius) >> 17)
						- (BattleFx_GlintCellWidths[frame & 3] >> 1);
					y = base.y - ((Trig_Cos(angle) * radius) >> 16)
						- (BattleFx_GlintCellHeights[frame & 3] >> 1);
					BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7,
						3 | RenderMode_GlintFlips[Random16() & 3], 2);
					((DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE])(canvas,
						work->sheet + BattleFx_GlintCellOffsets[frame & 3] + RENDER_MODE_TORNADO, x,
						y + 16, BattleFx_GlintCellWidths[frame & 3],
						BattleFx_GlintCellHeights[frame & 3]);
					Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
				}
			}
			BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 3, 3);
			draw[0] = gWorkSlot[HEAP_SLOT_BLITTER];
			BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, 3, 2);
			draw[1] = gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE];
			Render_ResetTransformState();
			Graphics_PrepareTransferInIwramWork(camera->eye, camera->pos);
			point[0] = object->x;
			point[1] = 160 << 13;
			point[2] = object->z;
			SceneTransform_ApplyPosition(point);
			if (frame > member * 8)
			{
				SceneTransform_ApplyYaw(frame << 9);
				if (mode <= 1 || mode == 4)
					SceneTransform_ApplyPitch(frame << 9);
				for (i = 0; i != count; i++)
				{
					struct EffectStep *mote =
						&((struct EffectStep *)Ram_MapCellBuffer)[member * 64 + i];
					s32 distance;

					if (frame > member * 8 + i)
					{
						s32 xx = (mote->x >> 8) * (mote->x >> 8);
						s32 yy = (mote->y >> 8) * (mote->y >> 8);
						s32 zz = (mote->z >> 8) * (mote->z >> 8);

						distance = Iwram_Sqrt(xx + yy + zz) >> 9;
						if (distance != 0 && mote->variant <= 23)
						{
							s32 cel = mote->variant / 4;

							EffectPosition_ApplyBaseAndYOffset((s32 *)mote, &screen);
							screen.x >>= 1;
							if (mode == 5 || mode == 7)
								draw[1](canvas, work->sheet + cel * 1600, screen.x - 20,
									screen.y - 20, 40, 40);
							else if (mode == 6)
								draw[1](canvas, work->sheet + 0xc00, screen.x - 6, screen.y - 12,
									12, 24);
							else if (mode == 4)
								draw[1](canvas, work->sheet, screen.x - 11, screen.y - 21, 22, 42);
							else
								draw[(i & 3) != 0](canvas, work->sheet + cel * 1152, screen.x - 12,
									screen.y - 24, 24, 48);
							if (DECAYS(mode))
							{
								mote->x -= mote->x / distance;
								mote->y -= mote->y / distance;
								mote->z -= mote->z / distance;
							}
							else
							{
								mote->y += 0x10000;
							}
							mote->variant++;
							if (mote->variant == 24)
							{
								if (DECAYS(mode))
								{
									mote->variant = 0;
								}
								else
								{
									mote->x = ((Random16() & 0xff) - 127) << 13;
									mote->y = ((Random16() & 0xff) - 255) << 12;
									mote->z = ((Random16() & 0xff) - 127) << 13;
								}
							}
						}
					}
				}
			}
			Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
			Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
			if (frame == member * 8 + 16)
			{
				s32 delay = total - frame;

				if (delay > 31)
					delay = 31;
				ObjectGroup_UpdateMembers(work->effect->actors[member], 7, 5, member, delay);
			}
		}
		Camera_ApplyShake(16, 16);
		ObjectGroup_TickMemberTimers();
		work->transfer_pending = 1;
		WaitFrames(1);
	}

	Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
	if (!(mode <= 1 || mode == 3))
		Scheduler_RemoveCallback(BattleFx_ArmBg2AffineHBlankDma);
	BattleFx_EndCanvasLayer();
}

#define gBattleFxWork ((void *(*)) & gBattleFxWork)

void BattleFx_RunBouncingSheep(void *object)
{
	void				   **heap_cache;
	void				   **cursor;
	struct BattleEffectWork *work;
	void					*canvas;
	s32						 record[3];

	struct EffectStep *one;
	struct EffectStep *sheep;
	s32				   i;
	s32				   frame;
	DrawRectangleFn	   draw_b;
	DrawRectangleFn	   draw_a;
	s32				   member;

	sheep = (struct EffectStep *)Ram_MapCellBuffer;
	heap_cache = gBattleFxWork;
	cursor = heap_cache;
	work = *cursor++;
	canvas = *cursor;
	work->effect = object;
	BattleFx_BeginCanvasLayer(0);
	REG_BG2PA = 0x100;
	REG_BLDCNT = 0;
	Resource_LoadAndDecompress((s32)&ResourceId_SheepSheet, work->sheet, 1, 1);
	BattleEffect_LoadWork(46, 7, 7, 3, 1);
	draw_a = (DrawRectangleFn)heap_cache[7];
	BattleEffect_LoadWork(47, 7, 7, 15, 1);
	draw_b = (DrawRectangleFn)heap_cache[8];
	for (i = 0; i != 32; i++)
	{
		one = &sheep[i];
		one->x = ((Random16() & 63) + 32) << 16;
		one->y = -0x200000;
		one->velocity_y = Random16() & 0;
		one->z = Random16() & 3;
		one->variant = Random16() & 255;
	}
	if (work->effect->side == 1)
		REG_BG2X = -0x7000;
	work->transfer_mode = 1;
	work->transfer_value = 0;
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
	AudioCommand_PlayFar(142);
	for (frame = 0; frame != 148; frame++)
	{
		struct BattleCamera *camera;

		camera = gCameraWork;
		if (frame == 80)
			BattleEventRuntime_BeginPhaseFar(0);
		for (member = 0; member != work->effect->count; member++)
		{
			void *member_object;

			member_object = GetBattleObjectSlotFar(work->effect->actors[member])->object;
			Render_ResetTransformState();
			Graphics_PrepareTransferInIwramWork(camera->eye, camera->pos);
			record[0] = *(s32 *)((u8 *)member_object + 8);
			record[1] = 160 << 14;
			record[2] = *(s32 *)((u8 *)member_object + 16);
			SceneTransform_ApplyPosition(record);
			if (frame == member * 16 + 64)
				ObjectGroup_UpdateMembers(work->effect->actors[member], 0, 5, -1, 0);
		}
		one = sheep;
		for (i = 0; i != 12; i++, one++)
		{
			if (frame > i * 4 && one->y <= 0x7fffff)
			{
				s32 cel;

				cel = (one->variant / 16) & 7;
				if (cel < 4)
					draw_a(canvas, work->sheet + (cel << 10), (one->x >> 16) - 16,
						(one->y >> 16) - 16, 32, 32);
				else
					draw_b(canvas, work->sheet + (cel << 10) - 0x1000, (one->x >> 16) - 16,
						(one->y >> 16) - 16, 32, 32);
				one->y += one->velocity_y;
				one->velocity_y += 0x2000;
				one->variant += one->z;
				if (one->y > 0x5c0000 && one->velocity_y == 0)
				{
					s32 speed = one->velocity_y + 1;
					one->y = 0x5c0000;
					one->z += 4;
					one->velocity_y = -speed / 2;
				}
			}
		}
		work->transfer_pending = 1;
		WaitFrames(1);
	}
	Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
	Runtime_ReleaseHeapBlock(47);
	Runtime_ReleaseHeapBlock(46);
	BattleFx_EndCanvasLayer();
}

void BattleFx_RunTargetMarkers(struct BattleEffectArgument *efx)
{
	void				   **slot;
	struct BattleEffectWork *work;
	void					*canvas;
	/* FAKEMATCH: needed to match: the family's blitter pair: this effect loads only one */
	DrawRectangle		  draw[2];
	struct EffectStep	 *marker;
	s32					  i;
	s32					  frame;
	s32					  start;
	s32					  cel;
	s32					  y;
	struct EffectPosition pos;

	slot = &gWorkSlot[40];
	canvas = slot[0];
	work = slot[-1];
	work->effect = efx;
	BattleFx_BeginCanvasLayer(1);
	REG_BG2PA = 0x100;
	REG_BLDCNT = 0;
	Resource_LoadAndDecompress((s32)&ResourceId_FlameColumnSheet, work->sheet, 1, 1);
	if (work->effect->side == 1)
	{
		REG_BG2X = -0x7000;
	}
	BattleEffect_LoadWork(46, 7, 7, 3, 1);
	work->transfer_mode = 1;
	work->transfer_value = 0;
	draw[0] = (DrawRectangle)slot[6];
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
	for (i = 0; i != work->effect->count; i++)
	{
		work->particles[i].variant = Random16() & 63;
	}
	frame = 0;
	while (frame != (work->effect->count << 5) + 32)
	{
		if (frame == 32)
		{
			BattleEventRuntime_BeginPhaseFar(0);
		}
		i = 0;
		if (work->effect->count != 0)
		{
			marker = work->particles;
			do
			{
				start = i << 4;
				if (frame == start)
				{
					AudioCommand_PlayFar(143);
				}
				if (frame < start)
				{
					goto next;
				}
				if (frame >= start + 72)
				{
					goto next;
				}
				EffectPosition_ApplyStepAndYOffset(work->effect->actors[i], &pos);
				if (work->effect->side == 1)
				{
					pos.x -= 112;
				}
				y = pos.y;
				pos.y = y - 16;
				draw[0](canvas, work->sheet + 1728, pos.x - 8, y - 20, 16, 20);
				if (frame < start)
				{
					goto next;
				}
				cel = ((frame - start) + marker->variant) / 6 % 9;
				draw[0](canvas, work->sheet + cel * 192, pos.x - 8, pos.y - 16, 16, 12);
			next:
				marker++;
				i++;
			} while (i != work->effect->count);
		}
		work->transfer_pending = 1;
		WaitFrames(1);
		frame += 1;
	}
	Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
	Runtime_ReleaseHeapBlock(46);
	BattleFx_EndCanvasLayer();
}

#undef gBattleFxWork

void BattleFx_RunSparkTravel(struct BattleEffectArgument *effect)
{
	void				   **heap_cache;
	void				   **cursor;
	struct BattleEffectWork *work;
	void					*canvas;
	s32						 member;
	u8						*sheet;
	DrawRectangle			*draw;
	struct MotionObject		*actor;
	struct MotionObject		*goal;
	s32						 frame;
	s32						 i;
	s32						 position[3];
	s32						 target[3];
	s32						 delta[3];
	s32						 rider[3];
	struct EffectPosition	 screen;
	s32						 point[3];
	struct EffectPosition	 view;
	DrawRectangle			 callbacks[2];

	heap_cache = (void **)gBattleFxWork;
	cursor = heap_cache;
	work = *cursor++;
	canvas = *cursor;
	sheet = heap_cache[2];
	work->effect = effect;
	BattleFx_BeginCanvasLayer(0);
	Resource_LoadAndDecompress((s32)&ResourceId_MercuryDjinnSmallSheet, work->sheet, 1, 1);
	Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);
	BattleFx_FetchRectangleBlitters(work->effect->side ^ 1, draw = callbacks);
	actor = GetBattleObjectSlotFar(work->effect->actor)->object;
	goal = GetBattleObjectSlotFar(work->effect->actors[0])->object;
	for (i = 0; i != 64; i++)
	{
		struct EffectStep *spark = &((struct EffectStep *)Ram_MapCellBuffer)[i];
		s32				   heading = Random16() & 0xffff;
		s32				   speed = (Random16() & 0xff) + 128;

		spark->x = 0;
		spark->y = ((Random16() & 31) + 20) << 16;
		spark->z = 0;
		spark->velocity_x = (Trig_Sin(heading) * speed) >> 5;
		spark->velocity_y = 0;
		spark->velocity_z = (Trig_Cos(heading) * speed) >> 5;
		spark->variant = 0;
	}
	work->transfer_mode = 2;
	work->transfer_value = 75;
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
	position[0] = actor->x;
	position[1] = 0;
	position[2] = actor->z;
	target[0] = goal->x;
	target[1] = 180 << 15;
	target[2] = 0;
	delta[0] = (target[0] - position[0]) / 40;
	delta[1] = (target[1] - position[1]) / 40;
	delta[2] = (target[2] - position[2]) / 40;
	for (frame = 0; frame != 98; frame++)
	{
		struct BattleCamera *camera = gCameraWork;
		if (frame == 8)
			AudioCommand_PlayFar(212);
		if (frame == 80)
			AudioCommand_PlayFar(142);
		Render_ResetTransformState();
		Graphics_PrepareTransferInIwramWork(camera->eye, camera->pos);
		if (frame >= 30 && frame < 70)
		{
			position[0] += delta[0];
			position[1] += delta[1];
			position[2] += delta[2];
		}
		SceneTransform_ApplyPosition(position);
		if (frame == 0)
			ObjectGroup_UpdateMembers(work->effect->actor, 7, -1, -1, 0);
		if (frame == 24)
			ObjectGroup_UpdateMembers(work->effect->actor, 0, -1, -1, 0);
		for (i = 0; i != 32; i++)
		{
			struct EffectStep *spark = &((struct EffectStep *)Ram_MapCellBuffer)[i];
			if (frame > i && spark->variant == 0)
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
				EffectPosition_ApplyBaseAndYOffset((s32 *)spark, &screen);
				screen.x >>= 1;
				Graphics_RestoreTransferWork();
				if (screen.depth < 250)
					screen.depth = 250;
				if (screen.depth > 634)
					screen.depth = 634;
				size = 9 - (screen.depth - 250) / 64;
				callbacks[0](canvas, sheet + ParticleStreams_CellOffsets[size - 1],
					screen.x - size / 2, screen.y - size, size, size * 2);
				EffectStep_AdvanceWithGravity3D(spark, 60, 0);
				if (frame > i + 30)
				{
					s32 dx = -spark->x >> 8;
					s32 dy = -spark->y >> 8;
					s32 dz = -spark->z >> 8;

					spark->velocity_x += dx;
					spark->velocity_y += dy;
					spark->velocity_z += dz;
				}
			}
		}
		if (frame > 82)
		{
			rider[0] = 0;
			rider[1] = Trig_Sin(frame << 10) << 2;
			rider[2] = 0;
			EffectPosition_ApplyBaseAndYOffset(rider, &screen);
			screen.x >>= 1;
			draw[1](canvas, work, screen.x - 10, screen.y - 17, 20, 34);
		}
		work->transfer_pending = 1;
		WaitFrames(1);
	}
	for (i = 0; i != 512; i++)
	{
		struct EffectStep *spark = &((struct EffectStep *)Ram_MapCellBuffer)[i];
		spark->x = ((Random16() & 0xff) - 127) << 15;
		spark->y = ((Random16() & 0x7f) + 64) << 15;
		spark->z = ((Random16() & 0xff) - 127) << 15;
		spark->variant = 0;
	}
	Resource_LoadAndDecompress((s32)&ResourceId_SparkleDots, sheet, 0, 0);
	for (frame = 0; frame != work->effect->count * 8 + 72; frame++)
	{
		struct BattleCamera *camera = gCameraWork;
		Render_ResetTransformState();
		Graphics_PrepareTransferInIwramWork(camera->eye, camera->pos);
		if (frame >= work->effect->count * 8 + 40)
			target[1] += 0x40000;
		point[0] = target[0];
		point[1] = target[1];
		point[2] = target[2] + Trig_Sin(frame << 11) * 40;
		EffectPosition_ApplyBaseAndYOffset(point, &view);
		view.x >>= 1;
		callbacks[0](canvas, work, view.x - 10, view.y - 17, 20, 34);
		for (member = 0; member != work->effect->count; member++)
		{
			struct MotionObject *object =
				GetBattleObjectSlotFar(work->effect->actors[member])->object;
			s32 start = member * 8;
			Render_ResetTransformState();
			Graphics_PrepareTransferInIwramWork(camera->eye, camera->pos);
			position[0] = object->x;
			position[1] = 160 << 14;
			position[2] = object->z;
			SceneTransform_ApplyPosition(position);
			if (frame == start + 30)
				AudioCommand_PlayFar(126);
			if (frame == start + 40)
				ObjectGroup_UpdateMembers(work->effect->actors[member], 7, -1, -1, 0);
			if (frame == start + 64)
				ObjectGroup_UpdateMembers(work->effect->actors[member], 0, -1, -1, 0);
			if (frame > start)
			{
				SceneTransform_ApplyYaw((frame - start) << 9);
				for (i = 0; i != 64; i++)
				{
					struct EffectStep *spark =
						&((struct EffectStep *)Ram_MapCellBuffer)[member * 64 + i];

					if (frame > start + i / 2)
					{
						s32 xx = (spark->x >> 8) * (spark->x >> 8);
						s32 yy = (spark->y >> 8) * (spark->y >> 8);
						s32 zz = (spark->z >> 8) * (spark->z >> 8);
						s32 distance = Iwram_Sqrt(xx + yy + zz) >> 9;
						if (distance != 0)
						{
							s32 cel;

							EffectPosition_ApplyBaseAndYOffset((s32 *)spark, &view);
							view.x >>= 1;
							if (view.depth < 314)
								view.depth = 314;
							if (view.depth > 634)
								view.depth = 634;
							/* FAKEMATCH: needed to match: the size series A draws its sparks at, kept from that effect and never used here */
							cel = 3 - (view.depth - 314) / 128;
							cel = (i * 4 + frame) % 9;
							{
								u8 *cell = sheet + BattleFx_PuffCells[cel];
								u32 edge = BattleFx_PuffSizes[cel];
								u32 half = edge >> 1;

								draw[1](canvas, cell, view.x - half, view.y - half, edge, edge);
							}
							spark->x -= spark->x / distance;
							spark->y -= spark->y / distance;
							spark->z -= spark->z / distance;
						}
					}
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
