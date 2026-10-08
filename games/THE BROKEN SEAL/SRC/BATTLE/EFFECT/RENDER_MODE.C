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
#include "BTLDISP.H"
#include "TYPES.H"
#include "IWRAM_CALL.H"
#include "CALL.H"
#include "BATTLE_EFX.H"
#include "BATTLE_EFFECT_WORK.H"
#include "CALLBACK_SCHEDULER.H"
#include "SYSTEM.H"
#include "FIXED_MATH.H"
#include "RESOURCE_IDS.H"
#include "EFFECT_STEP.H"
#include "MOTOBJ.H"
#include "RAM_BUFFER.H"
#include "IO_REG.H"
#include "TRANSFORM.H"

#define SceneTransform_ApplyPosition ((void (*)(s32 *))SceneTransform_ApplyPosition)
#define Camera_ApplyShake ((void (*)(s32, s32))Camera_ApplyShake)

void Audio_PlayCue(s32 cue);

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
	Call3((void (*)())Iwram_CopyWords, (s32)BG_PLTT, (s32)palette, 128);
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
	Audio_PlayCue(142);

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
