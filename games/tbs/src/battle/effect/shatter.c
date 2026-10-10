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
#include "types.h"
#include "battle_efx.h"
#include "callback_scheduler.h"
#include "fixed_math.h"
#include "resource_ids.h"
#include "battle_effect_work.h"
#include "effect_step.h"
#include "iwram_call.h"
#include "ram_buffer.h"
#include "io_reg.h"
#include "btldisp.h"
#include "b5_context.h"
#include "motobj.h"
#include "transform.h"
#include "map_scroll.h"
#include "project.h"
#include "scene.h"
#include "global_cells.h"

#define EffectPosition_ApplyAlternateStepAndYOffset \
	((void (*)(s16, s32 *))EffectPosition_ApplyAlternateStepAndYOffset)

#define Camera_ApplyShake ((void (*)(s32, s32))Camera_ApplyShake)

void AudioCommand_PlayFar(s32 cue);

#define HI(v) (((s16 *)&(v))[1])

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
		((struct EffectStep *)Ram_MapCellBuffer)[i].variant = -1;
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

						mote = &((struct EffectStep *)Ram_MapCellBuffer)[i * 32 + j];
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
			mote = &((struct EffectStep *)Ram_MapCellBuffer)[i];
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
			mote = &((struct EffectStep *)Ram_MapCellBuffer)[member * 16 + i];
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
		step = (struct EffectStep *)Ram_MapCellBuffer;
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

extern struct EffectStep gMapCellBuffer[];
#define PARTICLE_COUNT 64

void BattleFx_PrepareCanvasEffect(
	struct BattleEffectArgument *effect, s32 kind, s32 side, s32 anchor, s32 *out_x, s32 *out_y);

void BattleFx_RunParticleReveal(struct BattleEffectArgument *effect)
{
	void				   **heap_cache;
	void				   **cursor;
	struct BattleEffectWork *work;
	void					*canvas;
	s32						 spawn[3];
	s32						 screen_y;
	s32						 screen_x;
	DrawRectangleFn			 routine[2];
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
	draw[0] = (BattleEffectDrawRectangle)((union HeapState *)gWorkSlot)->slots[HEAP_SLOT_BLITTER];
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
				((BattleEffectDrawRectangle)((union HeapState *)gWorkSlot)
						->slots[HEAP_SLOT_BLITTER_ALTERNATE])(
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
						draw[1] = (BattleEffectDrawRectangle)((union HeapState *)gWorkSlot)
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

#undef Camera_ApplyShake

#define gBattleFxWork (*(struct BattleEffectWork **)&gBattleFxWork)

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

	work = gBattleFxWork;
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

#undef gBattleFxWork

#define Camera_ApplyShake ((void (*)(s32, s32))Camera_ApplyShake)
#define gWorkSlot (*(u8(*)[]) & gWorkSlot)

extern s16 Mode12_Points[][2];
extern u8  Mode12_Animations[];
extern u8  Mode12_SmokeFlips[];

struct Point
{
	s32 x;
	s32 y;
	s32 z;
};

struct BlitterPair
{
	DrawRectangle upper;
	DrawRectangle lower;
};

#define PARTICLES ((struct EffectStep *)(Ram_MapCellBuffer + 0xc58))
#define SLOT(n) (((DrawRectangle *)gWorkSlot)[n])

static __inline__ s32 DivQ16(s32 divisor, s32 value)
{
	/* FAKEMATCH: needed to match: a direct call keeps 0x1fe0000 in r5 for the camera call */
	return Iwram_RatioMulQ14(divisor, value);
}

void BattleFx_InitializeMode12(struct BattleEffectArgument *efx)
{
	u8						 ramp[32];
	s32						 vec[3];
	struct Point			 pts[11];
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
	struct EffectStep *p;
	DrawRectangle	  *cache;
	s32				   rings = 0x2710;

	slots = (void **)(gWorkSlot + 44 * 4);
	ctrl = slots[0];
	work = slots[-5];
	dst = slots[-4];
	aux = slots[-3];
	work->effect = efx;
	BattleFx_BeginCanvasLayer(0x2000);
	*(u16 *)0x04000020 = 0x100;
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
	*(u16 *)0x05000000 = 0;
	*(u16 *)0x05000002 = 0;
	*(u16 *)0x0400000c = 0x2784;
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
	*(u16 *)0x04000020 = 0x80;
	BattleEffect_LoadWork(46, 7, 7, 3, 3);
	cache = (DrawRectangle *)gWorkSlot;
	blit[0] = cache[46];
	*(u16 *)0x04000000 = 0x7741;
	*(u16 *)0x04000020 = 0x80;
	*(u16 *)0x04000052 = 0x100f;
	*(u16 *)0x04000050 = 0x3f44;
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
	*(u16 *)0x0400000c = 0x784;

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
		Camera_StoreSceneParameters(0x1fe0000, DivQ16(0x1fe0000, 0xc000), 0x7fff0000);
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
				EffectPosition_ApplyBaseAndYOffset(vec, (struct EffectPosition *)&pts[i]);
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
				BattleEffect_LoadWork(47, 7, 7, 3, 3);
				blit47 = SLOT(47);
			}
			else
			{
				BattleEffect_LoadWork(47, 7, 7, 3, 2);
				blit47 = SLOT(47);
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
				EffectPosition_ApplyBaseAndYOffset(vec, (struct EffectPosition *)&pts[0]);
				pts[0].x = (pts[0].x >> 17) + 64;
				pts[0].y = (pts[0].y >> 16) + 60;
				blit47(dst,
					work->sheet + (lvl * 0x302 + ParticleStreams_CellOffsets[s - 1]) + rings,
					pts[0].x - s / 2, pts[0].y - s, s, s * 2);
			}
			Runtime_ReleaseHeapBlock(47);
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
				/* FAKEMATCH: loop.c matches this 32 with the smoke calls' 32 (life 19, savings 2: 12 * 2 * 19 = 456 >= 450 insns) and hoists it out of the frame loop, so it loses its register and later reloads rotate; the native builds it in this loop's preheader. */
				asm("" : "+r"(k));
				blit[0](dst, ramp + w, w + 48, i + 16, k - w * 2, 1);
			}
			BattleEffect_LoadWork(47, 7, 7, 3, 2);
			SLOT(47)(dst, work, 32, (frame - 151) * 20 - 12, 32, 96);
			Runtime_ReleaseHeapBlock(47);
			BattleEffect_LoadWork(47, 7, 7, 7, 2);
			SLOT(47)(dst, work, 64, (frame - 151) * 20 - 12, 32, 96);
			Runtime_ReleaseHeapBlock(47);
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
	*(u16 *)0x04000052 = 0x1010;
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
			PARTICLES[i].variant = 0;
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
						for (i = 0, p = PARTICLES; i != 910; i++, p++)
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
							BattleEffect_LoadWork(47, 7, 7, 3, 2);
							SLOT(47)(dst, work->sheet + i % 3 * 0x1440, rx, ry, 72, 72);
							Runtime_ReleaseHeapBlock(47);
						}
						if (frame == i * 2 + 80)
						{
							s32 n = 0;
							for (j = 0, p = PARTICLES; j != 910; j++, p++)
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
				BattleEffect_LoadWork(47, 7, 7, 3, 2);
				SLOT(47)(dst, work->sheet + 0x6000, ox + 60, oy - 24, 12, 24);
				Runtime_ReleaseHeapBlock(47);
				BattleEffect_LoadWork(47, 7, 7, 7, 2);
				SLOT(47)(dst, work->sheet + 0x6000, sx, oy - 24, 12, 24);
				Runtime_ReleaseHeapBlock(47);
				BattleEffect_LoadWork(47, 7, 7, 11, 2);
				SLOT(47)(dst, work->sheet + 0x6000, ox + 60, oy, 12, 24);
				Runtime_ReleaseHeapBlock(47);
				BattleEffect_LoadWork(47, 7, 7, 15, 2);
				SLOT(47)(dst, work->sheet + 0x6000, sx, oy, 12, 24);
				Runtime_ReleaseHeapBlock(47);
				j = 0;
				for (i = 0, p = PARTICLES; i != 910; i++, p++)
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

			BattleEffect_LoadWork(47, 7, 7, 15, 2);
			{
				DrawRectangle	   draw = SLOT(47);
				struct EffectStep *q;
				for (i = 0, q = PARTICLES; i != 910; i++, q++)
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
			Runtime_ReleaseHeapBlock(47);

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
					BattleEffect_LoadWork(47, 7, 7, 3 | Mode12_SmokeFlips[Random16() & 3], 2);
					SLOT(47)(dst, Ram_MapCellBuffer + BattleFx_GlintCellOffsets[k], x, y + 56,
						BattleFx_GlintCellWidths[k], BattleFx_GlintCellHeights[k]);
					Runtime_ReleaseHeapBlock(47);
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

		Runtime_ReleaseHeapBlock(46);
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
	DrawRectangleFn			 blit;
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
	BattleEffect_LoadWork(46, 7, 7, 3, 1);
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
			struct EffectStep *seed = &((struct EffectStep *)Ram_MapCellBuffer)[i * 21 + j];

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
					shard = &((struct EffectStep *)Ram_MapCellBuffer)[i * 21 + j];
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
	Runtime_ReleaseHeapBlock(46);
	BattleFx_EndCanvasLayer();
}

#undef gWorkSlot
