#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "MEM.H"
#include "HEAP_STATE.H"
#include "CANVAS.H"
#include "RESOURCE.H"
#include "TYPES.H"
#include "BATTLE_EFX.H"
#include "CALLBACK_SCHEDULER.H"
#include "SYSTEM.H"
#include "FIXED_MATH.H"
#include "RESOURCE_IDS.H"
#include "BATTLE_EFFECT_WORK.H"
#include "EFFECT_STEP.H"
#include "IWRAM_CALL.H"
#include "RAM_BUFFER.H"
#include "IO_REG.H"
#include "BTLDISP.H"
#include "B5_CONTEXT.H"
#include "MOTOBJ.H"
#include "TRANSFORM.H"

#define EffectPosition_ApplyAlternateStepAndYOffset \
	((void (*)(s16, s32 *))EffectPosition_ApplyAlternateStepAndYOffset)

#define Camera_ApplyShake ((void (*)(s32, s32))Camera_ApplyShake)
#define Camera_ApplyShake ((void (*)(s32, s32))Camera_ApplyShake)

void Audio_PlayCue(s32 cue);

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
	Audio_PlayCue(141);
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
						Audio_PlayCue(133);
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
	s32						 burst_offset;
	s32						 member;
	s32						 frame;
	s32						 i;
	s32						 curtain_y;
	s32						 angle_mask;
	s32						 speed_mask;

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

	burst_offset = 0;
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
		i = 0;
		/* FAKEMATCH: the two random masks sit in locals set in this order, ahead of the mote pointer; that order gives 0xffff r9 and 127 fp and keeps the reload registers of the reference. */
		speed_mask = 127;	 /* FAKEMATCH: mask local, see above */
		angle_mask = 0xffff; /* FAKEMATCH: mask local, see above */
		mote = (struct EffectStep *)(Ram_MapCellBuffer + burst_offset);
		do
		{
			mote->x = (((Random16() & 15) + step->x) - 8) << 16;
			mote->y = ((Random16() & 7) + 96) << 16;
			mote->velocity_x = ((Random16() & speed_mask) - 64) << 11;
			mote->velocity_y = ((Random16() & speed_mask) - 64) << 10;
			mote->z = Random16() & angle_mask;
			mote->velocity_z = Random16() & angle_mask;
			i++;
			mote++;
		} while (i != 16);
		step++;
		member++;
		burst_offset += 16 * sizeof(struct EffectStep);
	} while (member != 9);

	Audio_PlayCue(0x88);
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
			Audio_PlayCue(0x91);
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
						Audio_PlayCue(134);
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
