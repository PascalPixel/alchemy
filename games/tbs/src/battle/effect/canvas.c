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
#include "types.h"
#include "scene.h"
#include "resource_ids.h"
#include "resource.h"
#include "battle_efx.h"
#include "battle_effect_work.h"
#include "btldisp.h"
#include "effect_step.h"
#include "callback_scheduler.h"
#include "fixed_math.h"
#include "b5_context.h"
#include "motobj.h"
#include "iwram_call.h"
#include "ram_buffer.h"
#include "io_reg.h"
#include "menu_list.h"
#include "transform.h"
#include "btlwork.h"
#include "io_write_queue.h"
#include "map_scroll.h"
#include "global_cells.h"
#include "project.h"

#define Camera_ApplyShake ((void (*)(s32, s32))Camera_ApplyShake)

#define gBlitters ((DrawRectangle *)gWorkSlot)
#define gMotes ((struct EffectStep *)Ram_MapCellBuffer)
#define gSparks ((struct EffectStep *)(Ram_MapCellBuffer + 0x3800))
#define HI(v) (((s16 *)&(v))[1])

void BattleFx_RunTwelveModeMode6(struct BattleEffectArgument *effect)
{
	BattleFx_RunTwelveMode(effect, 6);
}

void BattleFx_RunTwelveModeMode3(struct BattleEffectArgument *effect)
{
	BattleFx_RunTwelveMode(effect, 3);
}

void BattleFx_RunTwelveModeMode1(struct BattleEffectArgument *effect)
{
	BattleFx_RunTwelveMode(effect, 1);
}

void BattleFx_RunTwelveModeMode10(struct BattleEffectArgument *effect)
{
	BattleFx_RunTwelveMode(effect, 0xA);
}

void BattleFx_RunTwelveModeMode5(struct BattleEffectArgument *effect)
{
	BattleFx_RunTwelveMode(effect, 5);
}

void BattleFx_RunTwelveModeMode9(struct BattleEffectArgument *effect)
{
	BattleFx_RunTwelveMode(effect, 9);
}

void BattleFx_RunTwelveModeMode4(struct BattleEffectArgument *effect)
{
	BattleFx_RunTwelveMode(effect, 4);
}

void BattleFx_RunTwelveModeMode0(struct BattleEffectArgument *effect)
{
	BattleFx_RunTwelveMode(effect, 0);
}

void BattleFx_RunTwelveModeMode8(struct BattleEffectArgument *effect)
{
	BattleFx_RunTwelveMode(effect, 8);
}

void BattleFx_RunTwelveModeMode7(struct BattleEffectArgument *effect)
{
	BattleFx_RunTwelveMode(effect, 7);
}

void BattleFx_RunTwelveModeMode2(struct BattleEffectArgument *effect)
{
	BattleFx_RunTwelveMode(effect, 2);
}

void BattleFx_RunTwelveModeMode11(struct BattleEffectArgument *effect)
{
	BattleFx_RunTwelveMode(effect, 0xB);
}

void BattleFx_RunTwelveMode(struct BattleEffectArgument *effect, s32 mode)
{
	struct EffectPosition	 ground;
	struct EffectPosition	 target_position;
	struct EffectPosition	 alternate;
	struct EffectPosition	 position;
	DrawRectangle			 draw[2];
	void				   **heap_cache;
	void				   **cursor;
	struct BattleEffectWork *work;
	void					*canvas;
	s32						 frame;
	s32						 k;
	u8						*sheet;
	struct BattleCamera		*camera;
	struct MotionObject		*target;
	struct MotionObject		*source;
	s32						 resource;
	s32						 i;
	s32						 wisp_sheet = 0x3200;
	DrawRectangle			*lower = &gBlitters[47];

	heap_cache = (void **)gBattleFxWork;
	cursor = heap_cache;
	work = *cursor++;
	canvas = *cursor;
	sheet = heap_cache[2];
	camera = heap_cache[HEAP_SLOT_CAMERA - HEAP_SLOT_BATTLE_EFFECT];
	work->effect = effect;
	if (mode == 8)
		BattleFx_BeginCanvasLayer(0);
	else
		BattleFx_BeginCanvasLayer(1);
	REG_BLDALPHA = 0x1010;
	Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);
	Resource_LoadAndDecompress((s32)&ResourceId_TornadoSheet, work->sheet, 1, 0);
	if (TwelveMode_Records[mode * 7] == 0)
		Resource_LoadAndDecompress((s32)&ResourceId_BeamSequenceImage, work->sheet + 0xc80, 0, 0);
	else
		Resource_LoadAndDecompress((s32)&ResourceId_TwelveModeImage, work->sheet + 0xc80, 0, 0);
	switch (TwelveMode_Records[mode * 7 + 3])
	{
	case 0:
		resource = (s32)&ResourceId_BlueBeamSheet;
		break;
	case 1:
		resource = (s32)&ResourceId_LimePalette;
		break;
	case 2:
		resource = (s32)&ResourceId_PinkPalette;
		break;
	case 3:
		resource = (s32)&ResourceId_EmberStreakSheet;
		break;
	case 4:
		resource = (s32)&ResourceId_MarsDjinnSheet;
		break;
	case 5:
	default:
		resource = (s32)&ResourceId_LightningBoltSheet;
		break;
	}
	Iwram_CopyWords((void *)0x05000000, Resource_GetTableEntry(resource), 128);
	Resource_LoadAndDecompress((s32)&ResourceId_SmokeSheet, work->sheet + 0x3200, 1, 0);

	for (k = 0; k != 1; k++)
	{
		target = GetBattleObjectSlotFar(work->effect->actors[k])->object;
		for (i = 0; i != 64; i++)
		{
			struct EffectStep *step = &work->particles[i];

			EffectPosition_ApplyAlternateStepAndYOffset(work->effect->actors[k], &ground);
			step->x = (ground.x / 2) << 16;
			step->y = 0x500000;
			step->z = 0;
			step->velocity_x = ((Random16() & 255) - 128) << 9;
			step->velocity_y = ((Random16() & 255) - 128) << 9;
			step->velocity_z = 0;
			step->variant = -1;
		}
		for (i = 0; i != 128; i++)
		{
			struct EffectStep *step = &gMotes[k * 128 + i];

			step->x = target->x;
			step->y = 0x140000;
			step->z = target->z;
			step->velocity_x = ((Random16() & 255) - 128) << 11;
			step->velocity_y = ((Random16() & 255) - 128) << 11;
			step->velocity_z = ((Random16() & 255) - 128) << 11;
			step->variant = -1;
		}
		for (i = 0; i != 512; i++)
		{
			gSparks[i].x = target->x;
			gSparks[i].y = 0x140000;
			gSparks[i].z = target->z;
			if (TwelveMode_Records[mode * 7] == 1)
			{
				gSparks[i].velocity_x = ((Random16() & 255) - 128) << 11;
				gSparks[i].velocity_y = ((Random16() & 255) - 128) << 11;
				gSparks[i].velocity_z = ((Random16() & 255) - 128) << 11;
			}
			else
			{
				if (TwelveMode_Records[mode * 7 + 2] & 16)
				{
					gSparks[i].velocity_x = (Random16() & 0x1ff) << 11;
					gSparks[i].velocity_y = ((Random16() & 0x1ff) - 256) << 11;
					gSparks[i].velocity_z = ((Random16() & 0x1ff) - 256) << 11;
				}
				else
				{
					gSparks[i].velocity_x = (Random16() & 255) << 11;
					gSparks[i].velocity_y = ((Random16() & 255) - 128) << 11;
					gSparks[i].velocity_z = ((Random16() & 255) - 128) << 11;
				}
				if (gSparks[i].x > 0)
					gSparks[i].velocity_x = -gSparks[i].velocity_x;
			}
			gSparks[i].variant = -1;
		}
	}

	work->transfer_mode = 2;
	work->transfer_value = 75;
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
	EffectPosition_ApplyStepAndYOffset(work->effect->actors[0], &target_position);
	EffectPosition_ApplyAlternateStepAndYOffset(work->effect->actors[0], &alternate);
	target_position.y += (alternate.y - target_position.y) / 2;

	for (frame = 0; frame != TwelveMode_Records[mode * 7 + 6]; frame++)
	{
		s32 span = TwelveMode_Records[mode * 7 + 1] * TwelveMode_Records[mode * 7 + 4];
		s32 length = span * 4;
		s32 glint = 0;

		if (TwelveMode_Records[mode * 7] == 0)
		{
			if (frame < span * 6)
			{
				s32 picture = frame / TwelveMode_Records[mode * 7 + 4] % 6;

				if (work->effect->side == 1)
				{
					BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 7, 2);
					gBlitters[46](canvas, work->sheet + TwelveMode_StrikeOffsets[picture],
						target_position.x / 2 - (TwelveMode_StrikeReach[picture] / 2)
							- TwelveMode_StrikeWidths[picture] + 8,
						target_position.y - (TwelveMode_StrikeHeights[picture] / 2),
						TwelveMode_StrikeWidths[picture], TwelveMode_StrikeHeights[picture]);
				}
				else
				{
					BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 3, 2);
					gBlitters[46](canvas, work->sheet + TwelveMode_StrikeOffsets[picture],
						target_position.x / 2 + (TwelveMode_StrikeReach[picture] / 2) - 8,
						target_position.y - (TwelveMode_StrikeHeights[picture] / 2),
						TwelveMode_StrikeWidths[picture], TwelveMode_StrikeHeights[picture]);
				}
				Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
				if (frame % (TwelveMode_Records[mode * 7 + 4] * 6)
					== TwelveMode_Records[mode * 7 + 4] * 4)
				{
					if (mode == 8)
					{
						BattleEventRuntime_BeginPhaseFar(134);
					}
					else
					{
						AudioCommand_PlayFar(133);
						BattleEventRuntime_BeginPhaseFar(133);
					}
					if (TwelveMode_Records[mode * 7 + 2] & 16)
					{
						work->shake_frames = 8;
						ObjectGroup_UpdateMembers(work->effect->actors[0], 7, 5, 0, 12);
						BattleMotion_ApplyVariantMotionFar(work->effect->actors[0], 4);
						for (i = 0; i != 512; i++)
							gSparks[i].variant = (Random16() & 15) + 15;
					}
					else
					{
						work->shake_frames = 4;
						ObjectGroup_UpdateMembers(work->effect->actors[0], 7, 5, 0, 8);
						for (i = 0; i != 32; i++)
						{
							struct EffectStep *step =
								&gSparks[frame / (TwelveMode_Records[mode * 7 + 4] * 6) * 32 + i];

							step->variant = (Random16() & 15) + 7;
						}
					}
					for (i = 0; i != 8; i++)
					{
						struct EffectStep *step =
							&work->particles[frame / (TwelveMode_Records[mode * 7 + 4] * 6) * 16
											 + i];

						step->variant = 0;
					}
				}
			}
			if (frame >= 12 && frame <= 31)
				glint = 1;
		}
		else if (frame < length + 4)
		{
			s32 stage;

			if (frame < length)
			{
				s32 picture = frame / TwelveMode_Records[mode * 7 + 4];

				while (picture > 4)
					picture -= 4;
				stage = TwelveMode_ColumnStages[picture];
			}
			else
			{
				stage = 3;
			}
			BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 3, TwelveMode_Records[mode * 7 + 5]);
			gBlitters[46](
				canvas, work->sheet + stage * 864 + 0xc80, target_position.x / 2 - 18, 56, 18, 48);
			Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
			BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 7, TwelveMode_Records[mode * 7 + 5]);
			gBlitters[46](
				canvas, work->sheet + stage * 864 + 0xc80, target_position.x / 2, 56, 18, 48);
			Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
			if (frame % (TwelveMode_Records[mode * 7 + 4] * 4)
				== TwelveMode_Records[mode * 7 + 4] * 3)
			{
				ObjectGroup_UpdateMembers(work->effect->actors[0], 7, 5, 0, 8);
				work->shake_frames = 4;
				if (frame
					> (TwelveMode_Records[mode * 7 + 1] * 4 - 4) * TwelveMode_Records[mode * 7 + 4])
					BattleEventRuntime_BeginPhaseFar(133);
				else
					AudioCommand_PlayFar(133);
				for (i = 0; i != 64; i++)
				{
					struct EffectStep *step =
						&gSparks[frame / (TwelveMode_Records[mode * 7 + 4] * 6) * 64 + i];

					step->variant = (Random16() & 15) + 7;
				}
				for (i = 0; i != 8; i++)
				{
					struct EffectStep *step =
						&work->particles[frame / (TwelveMode_Records[mode * 7 + 4] * 6) * 16 + i];

					step->variant = 0;
				}
				for (i = 0; i != 16; i++)
					gMotes[frame / (TwelveMode_Records[mode * 7 + 4] * 6) * 16 + i].variant = 0;
			}
			if (stage == 3)
				glint = 1;
		}

		if ((TwelveMode_Records[mode * 7 + 2] & 4) && glint)
		{
			for (i = 0; i != 3; i++)
			{
				s32 image = frame & 3;
				s32 angle = Random16() & 0xffff;
				s32 range = 31;
				s32 radius = Random16() & range;
				s32 x;
				s32 y;

				x = target_position.x / 2 + (Trig_Sin(angle) * (radius + 4) >> 17)
					- (BattleFx_GlintCellWidths[image] / 2);
				y = target_position.y - (Trig_Cos(angle) * (radius + 4) >> 17)
					- (BattleFx_GlintCellHeights[image] / 2);
				BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7,
					TwelveMode_GlintDrawFlags[Random16() & 3] | 3, 3);
				(*lower)(canvas, work->sheet + BattleFx_GlintCellOffsets[image], x, y,
					BattleFx_GlintCellWidths[image], BattleFx_GlintCellHeights[image]);
				Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
			}
		}

		Render_ResetTransformState();
		Graphics_PrepareTransferInIwramWork(camera->eye, camera->pos);
		BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 3, 2);
		draw[0] = gBlitters[46];
		BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, 3, 3);
		draw[1] = *lower;
		for (k = 0; k != 1; k++)
		{
			source = GetBattleObjectSlotFar(work->effect->actor)->object;
			if (TwelveMode_Records[mode * 7 + 2] & 1)
			{
				for (i = 0; i != 64; i++)
				{
					struct EffectStep *step = &work->particles[i];

					if (step->variant >= 0 && step->variant <= 23)
					{
						s32 picture = step->variant / 4;

						draw[i & 1](canvas, work->sheet + picture * 1152 + wisp_sheet,
							HI(step->x) - 12, HI(step->y) - 24, 24, 48);
						EffectStep_AdvanceWithGravity2D(step, 62, -0x400);
						step->variant++;
					}
				}
			}
			if (TwelveMode_Records[mode * 7 + 2] & 2)
			{
				s32 size = 3;

				if (mode == 11)
					size = 8;
				if (frame == 55)
					ObjectGroup_UpdateMembers(work->effect->actor, 7, -1, -1, 0);
				if (frame == 90)
					ObjectGroup_UpdateMembers(work->effect->actor, 0, -1, -1, 0);
				for (i = 0; i != 64; i++)
				{
					struct EffectStep *step = &gMotes[i];

					if (step->variant >= 0)
					{
						EffectPosition_ApplyBaseAndYOffset((s32 *)step, &position);
						position.x >>= 1;
						draw[1](canvas, sheet + ParticleStreams_CellOffsets[size - 1],
							position.x - ((u32)size >> 1), position.y - size, size, size * 2);
						EffectStep_AdvanceWithGravity3D(step, 60, 0);
						step->variant++;
						if (step->variant > 10)
						{
							s32 dx = (source->x - step->x) >> 8;
							s32 dy = (source->y - step->y + 0x140000) >> 8;
							s32 dz = (source->z - step->z) >> 8;

							step->velocity_x += dx;
							step->velocity_y += dy;
							step->velocity_z += dz;
							if ((dx >= -0xfff && dx <= 0xfff) && (dz >= -0xfff && dz <= 0xfff))
								step->variant = -1;
						}
					}
				}
			}
			if (TwelveMode_Records[mode * 7 + 2] & 8)
			{
				for (i = 0; i != 128; i++)
				{
					struct EffectStep *step = &gSparks[i];
					s32				   size = step->variant;

					if (size >= 0)
					{
						EffectPosition_ApplyBaseAndYOffset((s32 *)step, &position);
						position.x >>= 1;
						EffectStep_AdvanceWithGravity3D(step, 60, 0);
						size >>= 3;
						size++;
						draw[1](canvas, sheet + ParticleStreams_CellOffsets[size - 1],
							position.x - size / 2, position.y - size, size, size * 2);
						step->variant--;
					}
				}
			}
		}
		Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
		Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
		if (TwelveMode_Records[mode * 7 + 2] & 16)
			Camera_ApplyShake(8, 8);
		else
			Camera_ApplyShake(2, 4);
		ObjectGroup_TickMemberTimers();
		work->transfer_pending = 1;
		WaitFrames(1);
	}

	Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
	BattleFx_EndCanvasLayer();
}

//--------------------------------------------------------------------------

// flipping_burst.c

// バトル演出：逆さに跳ぶふたりと閃光、剣の雨、柱とらせんの粒、割れて飛ぶ絵

//--------------------------------------------------------------------------

#undef Camera_ApplyShake

void Audio_PlayCue(s32 cue);

#define Camera_ApplyShake ((void (*)(s32, s32))Camera_ApplyShake)

void ObjectDispatch_InitFromTable6Far(struct MotionObject *object);
void ObjectDispatch_ApplyArgumentToChildrenFar(struct MotionObject *object, s32 mode);

//--------------------------------------------------------------------------
// 使い手を最初の対象との間の 9/10 の位置に置き、ふたりで跳んで逆さになる
// 54 フレーム目に対象の色を変えてふたりを跳ね返らせ、閃光を輪に広げながら昇らせる
//--------------------------------------------------------------------------
void BattleFx_RunFlippingBurst(struct BattleEffectArgument *effect)
{
	void				   **heap_cache;
	void				   **cursor;
	struct BattleEffectWork *work;
	void					*canvas;
	/* FAKEMATCH: needed to match: an unused second blitter word fills the frame's 8-byte draw slot */
	DrawRectangleFn		  draw[2];
	struct BtlObj		 *first;
	struct BtlObj		 *second;
	struct MotionObject	 *user;
	struct MotionObject	 *target;
	s32					  x;
	s32					  z;
	s32					  user_height;
	s32					  target_height;
	s32					  point[3];
	struct EffectPosition screen;
	s32					  frame;
	s32					  percent;

	heap_cache = (void **)gBattleFxWork;
	cursor = heap_cache;
	work = *cursor++;
	canvas = *cursor;
	work->effect = effect;
	BattleFx_BeginCanvasLayer(0);
	Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet, work->sheet, 1, 1);
	BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, GOUSEI_CLIP_X | GOUSEI_CLIP_Y, GOUSEI_HIKAKU);
	work->transfer_mode = 2;
	work->transfer_value = 75; // 転送のたびに 3/4 へ減らして残像にする
	draw[0] = heap_cache[HEAP_SLOT_BLITTER - HEAP_SLOT_BATTLE_EFFECT];
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
	first = GetBattleObjectSlotFar(work->effect->actor);
	second = GetBattleObjectSlotFar(work->effect->actors[0]);
	user = first->object;
	target = second->object;
	percent = 90;
	x = user->x + percent * (target->x - user->x) / 100;
	z = user->z + percent * (target->z - user->z) / 100;
	user_height = Battle_GetObjectTableValueFar(work->effect->actor);
	target_height = Battle_GetObjectTableValueFar(work->effect->actors[0]);
	ObjectDispatch_InitFromTable6Far(user);
	Object_SetMoveTargetFar(user, x, 0, z);
	ObjectDispatch_ApplyArgumentToChildrenFar(user, 2);
	user->snap_to_target = 1;
	user->auto_face_motion = 1;
	user->acceleration = 2 << 16;
	user->speed_limit = 8 << 16;
	WaitFrames(20);
	for (frame = 0; frame != 96; frame++)
	{
		struct BattleCamera *camera;

		camera = gCameraWork;
		Render_ResetTransformState();
		Graphics_PrepareTransferInIwramWork(camera->eye, camera->pos);

		// 0：ふたりで跳ぶ、11：逆さにして体の高さだけ持ち上げる
		if (frame == 0)
		{
			target->velocity_y = 15 << 16;
			target->vertical_motion_strength = 0x91eb;
			user->velocity_y = 15 << 16;
			user->vertical_motion_strength = 0x91eb;
		}
		if (frame == 11)
		{
			target->scale_y = -target->scale_y;
			user->scale_y = -user->scale_y;
			user->y += user_height;
			target->y += target_height;
		}

		// 54：対象の色を変え、ふたりを跳ね返らせる
		if (frame == 54)
		{
			ObjectGroup_UpdateMembers(work->effect->actors[0], 7, 5, 0, 10);
			target->velocity_y = 8 << 16;
			target->vertical_motion_strength = 0xab85;
			user->velocity_y = 5 << 16;
			user->vertical_motion_strength = 0x7851;
			user->acceleration = 1 << 16;
			user->speed_limit = 2 << 16;
			user->auto_face_motion = 0;
			ObjectDispatch_InitFromTable6Far(user);
			Object_SetMoveTargetFar(user, 0, 0, user->z);
		}
		Render_ResetTransformState();
		Graphics_PrepareTransferInIwramWork(camera->eye, camera->pos);
		point[0] = user->x;
		point[1] = user->y;
		point[2] = user->z;
		EffectPosition_ApplyBaseAndYOffset(point, &screen);
		screen.x >>= 1;

		// 54〜55：大きな閃光、56〜67：16 個の閃光が半径を広げながら昇る
		if (frame == 54 || frame == 55)
			draw[0](canvas, work, screen.x - 16, screen.y - 16, 32, 64);
		if (frame >= 56 && frame <= 67)
		{
			s32 i;

			for (i = 0; i != 16; i++)
			{
				s32 angle;
				s32 rx;
				s32 ry;

				angle = i << 12; // 22.5° ずつ
				rx = screen.x + ((Trig_Sin(angle) * (frame - 46)) >> 16);
				ry = ((Trig_Cos(angle) * (frame - 46)) >> 16) - frame + 100;
				draw[0](canvas, work->sheet + (((frame - 56) / 2) << 11), rx - 16, ry, 32, 64);
			}
		}

		// 64：向きと高さを元に戻す
		if (frame == 64)
		{
			target->scale_y = -target->scale_y;
			user->scale_y = -user->scale_y;
			user->y -= user_height;
			target->y -= target_height;
			ObjectDispatch_ApplyArgumentToChildrenFar(user, 0);
		}
		if (frame == 54)
			BattleEventRuntime_BeginPhaseFar(134);
		if (frame == 0)
		{
			AudioCommand_PlayFar(136);
			work->shake_frames = 6;
		}
		if (frame == 53)
			work->shake_frames = 6;
		Camera_ApplyShake(16, 16);
		work->transfer_pending = 1;
		WaitFrames(1);
	}
	Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
	BattleFx_EndCanvasLayer();
}

//--------------------------------------------------------------------------
// 剣の雨：16 本の剣が 4 フレームおきに降り、中央 (x = 64) へ寄りながら落ちて刺さる
// 刺さった剣は下 4 ドットを描かない。画面は 16 フレームで現れ、104 から消える
//--------------------------------------------------------------------------
void BattleFx_RunBladeRain(struct BattleEffectArgument *effect)
{
	struct EffectPosition	 pos;
	DrawRectangle			 draw[2];
	void				   **heap_cache;
	void				   **cursor;
	struct BattleEffectWork *work;
	void					*canvas;
	DrawRectangle			*blit;
	struct EffectStep		*blade;
	s32						 frame;
	s32						 i;

	heap_cache = (void **)gBattleFxWork;
	cursor = heap_cache;
	work = *cursor++;
	canvas = *cursor;
	work->effect = effect;
	BattleFx_BeginCanvasLayer(1);
	REG_BG2PA = 0x100; // 等倍
	REG_BLDALPHA = 0x1000;

	// [0] そのまま、[1] 左右反転 (どちらも 0 以外の画素だけ描く)
	BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, GOUSEI_CLIP_X | GOUSEI_CLIP_Y, GOUSEI_NURI);
	draw[0] = (DrawRectangle)heap_cache[HEAP_SLOT_BLITTER - HEAP_SLOT_BATTLE_EFFECT];
	BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7,
		GOUSEI_CLIP_X | GOUSEI_CLIP_Y | GOUSEI_FLIP_X, GOUSEI_NURI);
	draw[1] = (DrawRectangle)heap_cache[HEAP_SLOT_BLITTER_ALTERNATE - HEAP_SLOT_BATTLE_EFFECT];
	blit = draw;
	Resource_LoadAndDecompress((s32)&ResourceId_SwordSlashSheet, work->sheet, 1, 1);
	work->transfer_mode = 1; // 転送のたびにキャンバスを 0 で消す
	work->transfer_value = 0;
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
	EffectPosition_ApplyStepAndYOffset(work->effect->actors[0], &pos);
	REG_BG2X = (64 - pos.x) << 8;

	// 剣：x 16〜111、中央から遠いほど横の速さ (2 ドット単位) を大きくする
	for (i = 0; i != 64; i++)
	{
		s32 column;

		blade = &work->particles[i];
		column = Random16() % 96 + 16;
		blade->x = column;
		blade->y = (24 - i / 4) << 16;
		if (column <= 43)
			blade->velocity_x = 3;
		else if (column <= 51)
			blade->velocity_x = 2;
		else if (column <= 59)
			blade->velocity_x = 1;
		else if (column <= 67)
			blade->velocity_x = 0;
		else if (column <= 75)
			blade->velocity_x = -1;
		else if (column <= 83)
			blade->velocity_x = -2;
		else
			blade->velocity_x = -3;
		blade->velocity_x <<= 17;
		blade->velocity_y = 8 << 16;
		blade->x <<= 16;
	}
	AudioCommand_PlayFar(212);
	for (frame = 0; frame != 120; frame++)
	{
		if (frame <= 16)
		{
			REG_BLDALPHA = frame | 0x1000;
			if (frame == 16)
				REG_BLDCNT = 0;
		}
		if (frame > 103)
		{
			REG_BLDALPHA = (120 - frame) | 0x1000;
			if (frame == 104)
				REG_BLDCNT = 0x3f44;
		}

		// 絵は横の速さで選び、左へ進む剣は左右反転して描く
		for (i = 15; i != -1; i--)
		{
			/* FAKEMATCH: the drift is pinned to r6, as the reference keeps it. */
			register s32 drift __asm__("r6");
			s32			 speed;
			s32			 cell;
			u32			 wide;
			u32			 high;

			blade = &work->particles[i];
			drift = blade->velocity_x;
			speed = drift;
			if (drift < 0)
				speed = -drift;
			cell = speed >> 17;
			if (frame < i * 4 + 25)
			{
				blit[(u32)drift >> 31](canvas, work->sheet + BladeRain_CellSourceOffsets[cell],
					(blade->x >> 16) - ((wide = BladeRain_CellWidths[cell]) >> 1),
					(blade->y >> 16) - ((high = BladeRain_CellHeights[cell]) >> 1), wide, high);
				if (frame >= i * 4 + 16)
				{
					blade->x += blade->velocity_x;
					blade->y += blade->velocity_y;
				}
			}
			else
			{
				blit[(u32)drift >> 31](canvas, work->sheet + BladeRain_CellSourceOffsets[cell],
					(blade->x >> 16) - ((wide = BladeRain_CellWidths[cell]) >> 1),
					(blade->y >> 16) - ((high = BladeRain_CellHeights[cell]) >> 1), wide,
					high -= 4);
			}
		}

		// 24〜84 フレームの 4 フレームごとに対象のパレットを 2 フレーム 7 番に替えて
		// アニメ 5 にし、画面を揺らし、8 フレームごとに音を鳴らす
		if (frame >= 23 && frame <= 87 && (frame & 3) == 0)
		{
			ObjectGroup_UpdateMembers(work->effect->actors[0], 7, 5, 0, 2);
			work->shake_frames = 1;
			if ((frame & 7) == 0)
				AudioCommand_PlayFar(133);
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

#define FLECKS ((struct EffectStep *)Ram_MapCellBuffer) // らせんの粒 (128 個)

struct BlitterPair
{
	DrawRectangle kasan;  // 加算合成
	DrawRectangle hikaku; // 比較合成
};

//--------------------------------------------------------------------------
// 使い手の足元に左右対称の柱 (IceTile の絵) を立て、周りにらせんを描いて昇る粒を出す
// effect->kind	   : パレット (0 YellowPaletteA、1 IceTile、2 RedPaletteA、他 VioletPaletteA)
// effect->variant : 1 足して 1〜4 に収め、粒の大きさにする
//--------------------------------------------------------------------------
void BattleFx_RunIceFleckBurst(struct BattleEffectArgument *effect)
{
	struct EffectPosition	 ground;
	struct EffectPosition	 pos;
	s32						 point[3];
	struct BattleEffectWork *work;
	void					*canvas;
	struct BlitterPair		 draw;
	u8						*sheet;
	struct BattleCamera		*camera;
	s32						 frame;
	void					*palette;
	s32						 i;

	work =
		(struct BattleEffectWork *)Runtime_AllocateHeapBlock(HEAP_SLOT_BATTLE_EFFECT, sizeof *work);
	canvas = (void *)Runtime_AllocateHeapBlock(HEAP_SLOT_BATTLE_CANVAS, 0x4000);
	sheet = (u8 *)Runtime_AllocateHeapBlock(HEAP_SLOT_BATTLE_SHEET, 0x60e);
	camera = gCameraWork;
	work->effect = effect;
	BattleFx_BeginCanvasLayer(0);
	work->fade_frames = 24;
	work->fade_step = 0;
	REG_BLDALPHA = 0x100c;
	REG_BG2PA = 0x100; // 等倍
	Resource_LoadAndDecompress((s32)&ResourceId_IceTileSheet, work->sheet, 1, 0);
	Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesD, sheet, 0, 0);
	switch (work->effect->kind)
	{
	case 0:
		palette = Resource_GetTableEntry((s32)&ResourceId_YellowPaletteA);
		break;
	case 1:
		palette = Resource_GetTableEntry((s32)&ResourceId_IceTileSheet);
		break;
	case 2:
		palette = Resource_GetTableEntry((s32)&ResourceId_RedPaletteA);
		break;
	default:
		palette = Resource_GetTableEntry((s32)&ResourceId_VioletPaletteA);
		break;
	}
	{
		/* FAKEMATCH: needed to match: called through a pointer, the routine's address loads before the palette's */
		s32 (*copy)(void *, const void *, s32) = Iwram_CopyWords;

		copy((void *)BG_PLTT, palette, 128);
	}

	// 粒：x は角度、z は半径 (1/16 ドット単位)、i フレーム待ってから昇る
	for (i = 0; i != 128; i++)
	{
		FLECKS[i].y = 0;
		FLECKS[i].x = Random16() & 0xffff;
		FLECKS[i].z = (Random16() & 0x1ff) + i * 2;
		FLECKS[i].variant = -i;
	}
	/* FAKEMATCH: read-only work use exchanges r11/r9, fixing 16 register differences. */
	__asm__("" : : "r"(work));
	/* FAKEMATCH: a scheduling barrier the native order needs. */
	__asm__("");
	work->transfer_mode = 2;
	work->transfer_value = 75; // 転送のたびに 3/4 へ減らして残像にする
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
	BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, GOUSEI_CLIP_X | GOUSEI_CLIP_Y, GOUSEI_KASAN);
	draw.kasan = gWorkSlot[HEAP_SLOT_BLITTER];
	work->effect->variant++;
	if (work->effect->variant <= 0)
		work->effect->variant = 1;
	if (work->effect->variant > 4)
		work->effect->variant = 4;
	AudioCommand_PlayFar(212);

	for (frame = 0; frame != 56; frame++)
	{
		EffectPosition_ApplyAlternateStepAndYOffset(work->effect->actor, &ground);
		REG_BG2X = (64 - ground.x) << 8;
		if (frame > 49)
			REG_BLDALPHA = (112 - frame * 2) | 0x1000;
		if (frame == 16)
			ObjectGroup_UpdateMembers(work->effect->actors[0], 7, -1, 0, 20);

		// 柱：左半分 (x = 47) と左右反転した右半分 (x = 64) を比較合成で描く
		// 柱の絵 (17×64) は 2 フレームごとに 4 枚、光 (24×43) は 4 フレームごとに 3 枚
		if (frame <= 55)
		{
			s32 n;
			u8 *glow;

			i = frame / 2;
			n = i % 4;
			BattleEffect_LoadWork(
				HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, GOUSEI_CLIP_X | GOUSEI_CLIP_Y, GOUSEI_HIKAKU);
			draw.hikaku = ((DrawRectangle *)Ram_WorkSlot)[HEAP_SLOT_BLITTER_ALTERNATE];
			draw.hikaku(canvas, work->sheet + n * 1088, 47, ground.y - 64, 17, 64);
			n = frame / 4 % 3;
			glow = work->sheet + n * 1032 + 0x1100;
			draw.hikaku(canvas, glow, 40, ground.y - 36, 24, 43);
			Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
			BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7,
				GOUSEI_CLIP_X | GOUSEI_CLIP_Y | GOUSEI_FLIP_X, GOUSEI_HIKAKU);
			draw.hikaku = ((DrawRectangle *)Ram_WorkSlot)[HEAP_SLOT_BLITTER_ALTERNATE];
			n = i % 4;
			draw.hikaku(canvas, work->sheet + n * 1088, 64, ground.y - 64, 17, 64);
			draw.hikaku(canvas, glow, 64, ground.y - 36, 24, 43);
			Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
		}
		GetBattleObjectSlotFar(work->effect->actor);
		Render_ResetTransformState();
		Graphics_PrepareTransferInIwramWork(camera->eye, camera->pos);

		// 粒 (最初の 32 個)：1 フレームに 0x400 回り、5 ドット昇り、半径を 4 ドット広げる
		for (i = 0; i != 32; i++)
		{
			struct EffectStep *fleck = &FLECKS[i];

			if (fleck->variant >= 0)
			{
				s32 size;

				point[0] = fleck->z * Trig_Sin(fleck->x) >> 4;
				point[2] = -(fleck->z * Trig_Cos(fleck->x) >> 4);
				point[1] = fleck->y;
				fleck->x += 0x400;
				fleck->y += 5 << 16;
				fleck->z += 64;
				EffectPosition_ApplyBaseAndYOffset(point, &pos);
				pos.x /= 2;
				size = (i & 1) + work->effect->variant;
				draw.kasan(canvas, sheet + BattleFx6_FlareCells[size - 1], pos.x - size,
					pos.y - size, size * 2, size * 2);
			}
			fleck->variant++;
		}
		ObjectGroup_TickMemberTimers();
		work->transfer_pending = 1;
		WaitFrames(1);
	}

	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
	Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
	BattleFx_EndCanvasLayer();
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BATTLE_SHEET);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BATTLE_CANVAS);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BATTLE_EFFECT);
	/* FAKEMATCH: a scheduling barrier the native order needs. */
	__asm__("");
	/* FAKEMATCH: a scheduling barrier the native order needs. */
	__asm__("");
}

// 割れて飛ぶ 33 片：始まりの位置、大きさ、絵の位置
extern u8  BattlePieceStartX[];
extern u8  BattlePieceStartY[];
extern u8  BattlePieceWidth[];
extern u8  BattlePieceHeight[];
extern u16 BattlePieceOffset[];

// 256×120 の 8bpp タイル並び (横 32 タイル) の点 (x, y) に色 2 を置く
#define PLOT(x, y) \
	(*(Ram_MapCellBuffer + ((x) & 7) + (x) / 8 * 64 + ((y) & 7) * 8 + (y) / 8 * 2048) = 2)

//--------------------------------------------------------------------------
// 中心 (96, 60) から灰色の円が 1 フレームに半径 32 ずつ広がり、
// TileAnimationMask の絵を 33 片に割って飛ばしながら、パレット 6〜13 を
// 白から戦闘背景の色へ戻す。51 フレームからは閃光 32 個が昇る
//--------------------------------------------------------------------------
void BattleEffect_RunTileAndPaletteAnimation(struct BattleEffectArgument *effect)
{
	/* FAKEMATCH: needed to match: never read: the native frame keeps 12 unused bytes at sp+44..55, exactly this struct */
	struct EffectPosition		 position;
	struct BattleEffectWork		*work;
	void						*canvas;
	s32							 frame;
	struct
	{
		DrawRectangle nuri;
		DrawRectangle hikaku;
	} draw;
	struct BtlWork				*battle;
	struct BattleBackgroundView *view;
	void					   **slots;
	void					   **cache;
	s32							 rmax;
	struct EffectStep			*piece;
	s32							 r;
	s32							 i;
	s32							 px;
	s32							 j;
	s32							 k;
	s32							 d;
	s32							 py;
	s32							 x1;
	s32							 size;
	s32							 left;
	s32							 left2;
	s32							 y1;
	s32							 x2;
	s32							 y2;
	u8							*start_y;
	u8							*start_x;
	s32							 cx;
	s32							 cy;
	s32							 cell_size;
	u8							*cells;
	u32							 cell_fill;

	work = Runtime_AllocateHeapBlock(HEAP_SLOT_BATTLE_EFFECT, sizeof(*work));
	size = 0x4000;
	canvas = Runtime_AllocateHeapBlock(HEAP_SLOT_BATTLE_CANVAS, size);
	Runtime_AllocateHeapBlock(HEAP_SLOT_BATTLE_SHEET, 0x302);
	slots = gWorkSlot + HEAP_SLOT_BATTLE;
	battle = slots[0];
	view = slots[HEAP_SLOT_BATTLE_BACKGROUND - HEAP_SLOT_BATTLE];
	work->effect = effect;

	// BG2 を 128×128 のキャンバスにする (BattleFx_BeginCanvasLayer と同じ設定)
	BattleFx_ClearActorTimers();
	view->second_mode = 1;
	gBgScroll[1].y = 32;
	BattlePresentation_ConfigurePaletteFadeFar(1, battle->background, 0);
	REG_BG2CNT = 0x784;
	BattlePres_SetupTransitionSceneFar(0, 0, 0, 100);
	view->second_mode = 0;
	REG_BG2X = 0;
	REG_BG2Y = 0xfffff000;
	REG_BG2PA = 0x80;
	REG_BG2PB = 0;
	REG_BG2PC = 0;
	REG_BG2PD = 0x100;
	REG_WIN0H = 0xf0;
	REG_WIN0V = 0x1088;
	REG_WIN1H = 0xf0;
	REG_WIN1V = 0x1088;
	REG_WININ = 0x3537;
	REG_WINOUT = 0x3f21;

	i = 0;
	for (k = 0; k != 16; k++)
	{
		for (j = 0; j != 8; j++, i += 2)
		{
			s16 entry;

			entry = ((k * 16 + j * 2 + 1) << 8) | ((k * 8 + j) * 2);
			*(volatile s16 *)(0x06003800 + i) = entry;
		}
	}

	Iwram_ClearWords(canvas, size);
	Iwram_ClearWords((void *)0x06004000, size);
	IO_WRITE_QUEUE_PUSH(0x04000000, 0x7741, 0x20000);
	REG_BLDALPHA = 0x1010;
	REG_BLDCNT = 0;
	Resource_LoadAndDecompress((s32)&ResourceId_TileAnimationMask, work, 1, 1);
	work->transfer_mode = 1; // 転送のたびにキャンバスを 0 で消す
	work->transfer_value = 0;
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
	BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, GOUSEI_CLIP_X | GOUSEI_CLIP_Y, GOUSEI_NURI);
	cache = gWorkSlot;
	draw.nuri = cache[HEAP_SLOT_BLITTER];
	BattleEffect_LoadWork(
		HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, GOUSEI_CLIP_X | GOUSEI_CLIP_Y, GOUSEI_HIKAKU);
	draw.hikaku = cache[HEAP_SLOT_BLITTER_ALTERNATE];

	// 片：中心 (32, 60) から外へ、距離の 1/4 の速さで飛ぶ
	/* FAKEMATCH: needed to match: the copy and fill operands set here, before the piece loop, so CSE does not share them and the pair reloads both as the native does */
	cell_size = 0x7800;
	cells = Ram_MapCellBuffer;
	cell_fill = 0x01010101;
	start_y = BattlePieceStartY;
	start_x = BattlePieceStartX;
	r = 0;
	piece = work->particles;
	for (i = 0; i != 33; i++)
	{
		s32 x, y, vx, vy;

		x = *start_x++ << 16;
		y = *start_y++ << 16;
		piece->x = x;
		piece->y = y;
		vx = (x - 0x200000) >> 2;
		vy = (y - 0x3c0000) >> 2;
		piece->velocity_x = vx;
		piece->velocity_y = vy;
		piece++;
	}

	// VRAM 0x06008000 の絵を写してから色 1 で埋め、パレット 6〜13 を白にする
	Iwram_CopyWords(cells, (void *)0x06008000, cell_size);
	Iwram_FillWords(cells, cell_size, cell_fill);
	view->busy = 1;
	work->saved_bg1_x = gBgScroll[1].x;
	work->saved_bg1_y = gBgScroll[1].y;
	gBgScroll[1].x = 0;
	IO_WRITE_QUEUE_PUSH(0x0400000a, 0x1f81, 0x20000);
	Iwram_FillWords((void *)0x050000c0, 0x100, 0x7fff7fff);
	AudioCommand_PlayFar(212);
	ObjectGroup_UpdateMembers(work->effect->actors[0], 7, 3, 0, 30);

	for (frame = 0; frame != 128; frame++)
	{
		if (frame == 2)
			AudioCommand_PlayFar(212);
		if (frame == 3)
			AudioCommand_PlayFar(212);
		if (frame == 28)
			ObjectGroup_UpdateMembers(work->effect->actors[0], -1, 3, -1, 0);
		if (frame == 32)
			AudioCommand_PlayFar(149);
		/* FAKEMATCH: keep the frame-5 cue and background restore in separate CSE blocks; a combined block loads the saved-bg offset before reloading work. */
		if (frame == 5)
			AudioCommand_PlayFar(145);
		if (frame == 5)
		{
			gBgScroll[1].x = work->saved_bg1_x;
			BattleBackground_LoadFar(1, battle->background, -1);
		}

		// 8 フレームから、パレット 6〜13 の各色を 1 段ずつ戦闘背景の色へ近づける
		if (frame > 7)
		{
			u16 *pal;

			pal = (u16 *)0x050000c0;
			for (i = 0; i != 128; i++)
			{
				s32 red = *pal & 0x1f;
				s32 green = (*pal >> 5) & 0x1f;
				s32 blue = (*pal >> 10) & 0x1f;
				s32 goal_red = battle->palette[i] & 0x1f;
				s32 goal_green = (battle->palette[i] >> 5) & 0x1f;
				s32 goal_blue = (battle->palette[i] >> 10) & 0x1f;

				if (red < goal_red)
					red++;
				else if (red > goal_red)
					red--;
				if (green < goal_green)
					green++;
				else if (green > goal_green)
					green--;
				if (blue < goal_blue)
					blue++;
				else if (blue > goal_blue)
					blue--;
				*pal++ = (blue << 10) | (green << 5) | red;
			}
		}

		if (frame == 4)
			Iwram_FillWords((void *)0x06008000, 0x7800, 0x02020202);

		// 0〜3 フレーム：円を半径 32 ずつ広げ、色 2 の灰色を明るくしていく
		if (frame <= 3)
		{
			s32 v;

			v = frame * 4 + 8;
			rmax = frame * 32;
			BG_PLTT[2] = (v << 10) | (v << 5) | v;
			cx = 96;
			cy = 60;
			for (; r != rmax; r++)
			{
				// 中点円の描き方で、8 分の 1 ずつ対称に点を置く
				px = r;
				py = 0;
				d = r;
				while (px >= py)
				{
					x2 = px + cx;
					x1 = cx - px;
					left = x1;
					y1 = cy - py;
					y2 = py + cy;
					if (y1 < 0)
						y1 = 0;
					if (y2 > 119)
						y2 = 119;
					if (x1 < 0)
						x1 = 0;
					if (x2 > 255)
						x2 = 255;
					PLOT(x2, y2);
					PLOT(x2, y1);
					PLOT(x1, y2);
					PLOT(x1, y1);
					x1 = left + 1;
					x2 = px + 97;
					if (x1 < 0)
						x1 = 0;
					if (x2 > 255)
						x2 = 255;
					PLOT(x2, y2);
					PLOT(x2, y1);
					PLOT(x1, y2);
					PLOT(x1, y1);

					x2 = py + cx;
					x1 = cx - py;
					left2 = x1;
					y2 = px + cy;
					y1 = cy - px;
					if (x1 < 0)
						x1 = 0;
					if (x2 > 255)
						x2 = 255;
					if (y1 < 0)
						y1 = 0;
					if (y2 > 119)
						y2 = 119;
					PLOT(x2, y2);
					PLOT(x2, y1);
					PLOT(x1, y2);
					PLOT(x1, y1);
					x1 = left2 + 1;
					x2 = py + 97;
					if (x1 < 0)
						x1 = 0;
					if (x2 > 255)
						x2 = 255;
					PLOT(x2, y2);
					PLOT(x2, y1);
					PLOT(x1, y2);
					PLOT(x1, y1);

					d -= py * 2 + 1;
					if (d < 0)
					{
						d += px * 2 - 2;
						px--;
					}
					py++;
				}
			}
			Iwram_CopyWords((void *)0x06008000, Ram_MapCellBuffer, 0x7800);
		}

		// 50 フレームまで片を描き、4 フレームから落とす
		if (frame <= 50)
		{
			for (i = 0; i != 33; i++)
			{
				piece = &work->particles[i];
				draw.nuri(canvas, (u8 *)work + BattlePieceOffset[i], HI(piece->x), HI(piece->y),
					BattlePieceWidth[i], BattlePieceHeight[i]);
				if (frame > 3)
					EffectStep_AdvanceWithGravity2D(piece, 64, 0x4000);
			}
		}

		// 8〜50 フレーム：色 1 を黒から白へ
		if (frame >= 8 && frame <= 50)
		{
			s32 v;

			v = frame - 8;
			if (v > 31)
				v = 31;
			BG_PLTT[1] = (v << 10) | (v << 5) | v;
		}

		// 51 フレーム：閃光の絵と色を用意し、x 32〜63、y 80〜111 に 32 個を置く
		if (frame == 51)
		{
			Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet, work, 1, 0);
			for (i = 1; i != 64; i++)
			{
				s32 v;

				v = i / 2;
				if (v < 0)
					v = 0;
				BG_PLTT[i] = (v << 10) | ((v / 2) << 5) | v;
			}
			REG_BLDCNT = 0x3f44;
			for (i = 0; i != 32; i++)
			{
				piece = &work->particles[i];
				piece->x = ((Random16() & 31) + 32) << 16;
				piece->y = ((Random16() & 31) + 80) << 16;
				piece->velocity_x = ((Random16() & 0x1ff) - 0x100) << 12;
				piece->velocity_y = 0;
				piece->variant = 0;
			}
			work->transfer_mode = 2;
			work->transfer_value = 50; // 転送のたびに半分へ減らして残像にする
		}

		// 閃光：i/4 フレームずつ遅れて出て、4 フレームごとに絵を進めながら昇る
		if (frame > 52)
		{
			for (i = 0; i != 32; i++)
			{
				piece = &work->particles[i];
				if (frame >= i / 4 + 52 && piece->variant <= 39)
				{
					s32 cell;

					cell = piece->variant / 4;
					if (cell > 5)
						cell = 5;
					draw.hikaku(canvas, (u8 *)work + (cell << 11), HI(piece->x) - 16, HI(piece->y) - 32,
						32, 64);
					EffectStep_AdvanceWithGravity2D(piece, 60, -0x1000);
					piece->variant++;
				}
			}
		}

		ObjectGroup_TickMemberTimers();
		work->transfer_pending = 1;
		WaitFrames(1);
	}

	// 背景と表示を元に戻す
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
	Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
	ObjectGroup_UpdateMembers(work->effect->actors[0], -1, 1, -1, 0);
	gBgScroll[1].x = work->saved_bg1_x;
	gBgScroll[1].y = 32;
	BattlePresentation_ConfigurePaletteFadeFar(2, battle->background, 0);
	WaitFrames(1);
	IO_WRITE_QUEUE_PUSH(0x04000000, 0x7541, 0x20000);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BATTLE_SHEET);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BATTLE_CANVAS);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BATTLE_EFFECT);
}

#undef Camera_ApplyShake

struct Streak
{
	s32 head;
	s32 tail;
	s32 unused_08;
	s32 pitch;
	s32 yaw;
	s32 roll;
	s32 unused_18;
};

struct StreakPoint
{
	s32 distance;
	s32 y;
	s32 z;
};

#define gFlecks ((struct EffectStep *)Ram_MapCellBuffer)

struct BlitterPair_2
{
	DrawRectangle upper;
	DrawRectangle lower;
};

void BattleFx_RunGatheringBurst(struct BattleEffectArgument *effect)
{
	struct EffectPosition	 ground;
	struct BattleEffectWork *work;
	void					*canvas;
	struct BlitterPair_2	 draw;
	u8						*sheet;
	void					*palette;
	s32						 frame;
	s32						 i;

	work =
		(struct BattleEffectWork *)Runtime_AllocateHeapBlock(HEAP_SLOT_BATTLE_EFFECT, sizeof *work);
	canvas = (void *)Runtime_AllocateHeapBlock(HEAP_SLOT_BATTLE_CANVAS, 0x4000);
	sheet = (u8 *)Runtime_AllocateHeapBlock(HEAP_SLOT_BATTLE_SHEET, 0x60e);
	work->effect = effect;
	BattleFx_BeginCanvasLayer(0);
	work->fade_frames = 24;
	work->fade_step = 0;
	REG_BLDALPHA = 0x100c;
	REG_BG2PA = 0x100;
	Resource_LoadAndDecompress((s32)&ResourceId_CyanSparkSheet, work->sheet, 1, 0);
	Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesD, sheet, 0, 0);
	switch (work->effect->kind)
	{
	case 0:
		palette = Resource_GetTableEntry((s32)&ResourceId_YellowPaletteA);
		break;
	case 1:
		palette = Resource_GetTableEntry((s32)&ResourceId_IceTileSheet);
		break;
	case 2:
		palette = Resource_GetTableEntry((s32)&ResourceId_RedPaletteA);
		break;
	default:
		palette = Resource_GetTableEntry((s32)&ResourceId_VioletPaletteA);
		break;
	}
	{
		s32 (*copy)(void *, const void *, s32) = Iwram_CopyWords;

		copy((void *)0x05000000, palette, 128);
	}

	for (i = 0; i != 128; i++)
	{
		gFlecks[i].y = 0x800000;
		gFlecks[i].x = Random16() & 0xffff;
		gFlecks[i].z = (Random16() & 0x1ff) + 0x400;
		gFlecks[i].variant = -i;
	}
	for (i = 0; i != 64; i++)
	{
		work->particles[i].x = Random16() & 0xffff;
		work->particles[i].y = (Random16() & 31) + 16;
		work->particles[i].variant = (i & 15) + 16;
	}
	work->transfer_mode = 2;
	work->transfer_value = 75;
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
	BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 7, 3);
	draw.upper =
		(BattleEffectDrawRectangle)((union HeapState *)gWorkSlot)->slots[HEAP_SLOT_BLITTER];
	AudioCommand_PlayFar(140);

	for (frame = 0; frame != 56; frame++)
	{
		EffectPosition_ApplyStepAndYOffset(effect->actor, &ground);
		REG_BG2X = (64 - ground.x) << 8;
		if (frame > 49)
			REG_BLDALPHA = (0x70 - frame * 2) | 0x1000;
		if (frame == 26)
		{
			AudioCommand_PlayFar(212);
			ObjectGroup_UpdateMembers(work->effect->actors[0], 7, -1, 0, 20);
		}
		if (frame >= 28 && frame <= 48)
			draw.upper(
				canvas, work->sheet + (frame - 28) / 3 * 2304 + 0x1400, 40, ground.y - 24, 48, 48);
		if (frame >= 0 && frame <= 14)
		{
			s32			   picture = frame / 3 % 5;
			DrawRectangle *slot = &((DrawRectangle *)Ram_WorkSlot)[47];

			for (i = 0; i != 4; i++)
			{
				BattleEffect_LoadWork(
					HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, CornerSparks_DrawFlags[i] | 3, 2);
				draw.lower = *slot;
				draw.lower(canvas, work->sheet + (picture << 10), CornerSparks_X[i] + 32,
					ground.y + CornerSparks_Y[i] - 32, 32, 32);
				Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
			}
		}
		if (frame >= 0)
		{
			for (i = 0; i != 64; i++)
			{
				struct EffectStep *mote = &work->particles[i];

				if (mote->variant >= 0 && mote->y > 0)
				{
					s32 size = (mote->variant >> 3) + 1;
					s32 x = (mote->y * Trig_Sin(mote->x) >> 16) + 64;
					s32 y = (mote->y * Trig_Cos(mote->x) >> 16) + ground.y;

					if (size <= 0)
						size = 1;
					draw.upper(canvas, sheet + BattleFx6_FlareCells[size - 1], x - size, y - size,
						size * 2, size * 2);
					mote->y -= 2;
					mote->variant--;
				}
			}
		}
		ObjectGroup_TickMemberTimers();
		work->transfer_pending = 1;
		WaitFrames(1);
	}

	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
	Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
	BattleFx_EndCanvasLayer();
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BATTLE_SHEET);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BATTLE_CANVAS);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BATTLE_EFFECT);
}

void BattlePresentation_DrawStreaks(void)
{
	struct BattleEffectWork *work = *(void **)gBattleFxWork;
	s32						 frame;
	s32						 i;
	struct Streak			*streak;
	struct StreakPoint		 point;
	struct EffectPosition	 head;
	struct EffectPosition	 tail;

	frame = (work->frame)++;
	if (frame == 0)
	{
		for (i = 0; i != 256; i++)
		{
			struct Streak *seed = &((struct Streak *)Ram_MapCellBuffer)[i];
			s32			   r = Random16() & 15;

			seed->head = r + 48;
			seed->tail = r + 40;
			seed->pitch = Random16() & 0xffff;
			seed->yaw = Random16() & 0xffff;
			seed->roll = Random16() & 0xffff;
		}
	}
	point.y = 0;
	point.z = 0;
	i = 0;
	for (; i != 64; i++)
	{
		streak = &((struct Streak *)Ram_MapCellBuffer)[i];
		if (frame > i / 4 && streak->head > 0)
		{
			s32 fade;

			Render_ResetTransformState();
			SceneTransform_ApplyRoll(streak->roll);
			SceneTransform_ApplyPitch(streak->pitch);
			SceneTransform_ApplyYaw(streak->yaw);
			point.distance = streak->head;
			EffectPosition_ApplyBaseAndYOffset((s32 *)&point, &head);
			head.x += 64;
			head.y += 80;
			point.distance = streak->tail;
			EffectPosition_ApplyBaseAndYOffset((s32 *)&point, &tail);
			tail.x += 64;
			tail.y += 80;
			streak->tail -= 4;
			streak->head -= 4;
			if (streak->tail < 0)
				streak->tail = 0;
			fade = -streak->tail / 2;
			BattleFx_DrawClippedCanvasLine(tail.x - 1, tail.y, head.x - 1, head.y, fade + 48);
			BattleFx_DrawClippedCanvasLine(tail.x, tail.y - 1, head.x, head.y - 1, fade + 48);
			BattleFx_DrawClippedCanvasLine(tail.x, tail.y, head.x, head.y, fade + 56);
		}
	}
	work->transfer_pending = 1;
}

void BattlePresentation_PrepareScene(s32 kind)
{
	struct BattleEffectWork *work;
	void					*palette;
	s32						 id;

	work = Runtime_AllocateHeapBlock(HEAP_SLOT_BATTLE_EFFECT, sizeof *work);
	Runtime_AllocateHeapBlock(HEAP_SLOT_BATTLE_CANVAS, 0x4000);
	BattleFx_BeginCanvasLayer(0);
	work->fade_frames = 24;
	REG_BG2PA = 0x100;
	REG_BLDALPHA = 0x1010;
	switch (kind)
	{
	case 0:
		id = (s32)&ResourceId_YellowPaletteB;
		break;
	case 1:
		id = (s32)&ResourceId_CrystalSheet;
		break;
	case 2:
		id = (s32)&ResourceId_EmberStreakSheet;
		break;
	case 3:
		id = (s32)&ResourceId_VioletPaletteD;
		break;
	case 4:
	default:
		id = (s32)&ResourceId_DustPuffSheet;
		break;
	}
	palette = Resource_GetTableEntry(id);
	Iwram_CopyWords((void *)0x05000000, palette, 128);
	work->frame = 0;
	work->transfer_mode = 3;
	work->transfer_value = 0x06060606;
	Scheduler_AddOrUpdateCallback(BattlePresentation_DrawStreaks, 0xC80);
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
}

void BattleFx_ScheduleCallbacksAndReleaseBlocks(void)
{
	Scheduler_RemoveCallback(BattlePresentation_DrawStreaks);
	Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
	{
		s32 (*transfer)(void *, s32) = Iwram_ClearWords;

		transfer((void *)0x06004000, 0x4000);
	}
	Scheduler_RemoveCallback(Palette_StepFadeTransfer);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BATTLE_CANVAS);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BATTLE_EFFECT);
}

void BattleFx_RunTwoResourceMode0(struct BattleEffectArgument *efx)
{
	BattleFx_RunTwoResource(efx, 0);
}

void BattleFx_RunTwoResourceMode1(struct BattleEffectArgument *efx)
{
	BattleFx_RunTwoResource(efx, 1);
}

void BattleFx_RunTwoResource(struct BattleEffectArgument *efx, s32 mode)
{
	struct BattleEffectWork *work;
	void					*canvas;
	struct MotionObject		*object;
	struct EffectPosition	 position;
	/* FAKEMATCH: needed to match: only draw[0] is used: the second word fills four bytes of the frame */
	DrawRectangle draw[2];
	s32			  frames;
	s32			  frame;
	s32			  cell;
	void		**heap;
	void		**p;

	heap = (void **)gBattleFxWork;
	p = heap;
	work = *p++;
	canvas = *p;
	work->effect = efx;
	BattleFx_BeginCanvasLayer(0);
	REG_BG2PA = 0x100;
	if (work->effect->side == 1)
	{
		BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 3, 1);
		draw[0] = (DrawRectangle)heap[7];
	}
	else
	{
		BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 7, 1);
		draw[0] = (DrawRectangle)heap[7];
	}
	Resource_LoadAndDecompress((s32)&ResourceId_MagentaSwirlSheet, work->sheet, 1, 1);
	Resource_LoadAndDecompress((s32)&ResourceId_MagentaTailSheet, Ram_MapCellBuffer, 1, 0);
	if (mode == 0)
	{
		void *palette = Resource_GetTableEntry((s32)&ResourceId_LimePalette);
		/* FAKEMATCH: needed to match: the copier address is set before the costly 0x05000000 operand, as the reference loads it */
		s32 (*copy)(void *, const void *, s32) = Iwram_CopyWords;

		copy((void *)0x05000000, palette, 128);
	}
	work->transfer_mode = 2;
	work->transfer_value = 75;
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
	object = GetBattleObjectSlotFar(work->effect->actors[0])->object;
	EffectPosition_ApplyStepAndYOffset(work->effect->actors[0], &position);
	if (work->effect->side == 0)
		REG_BG2X = (16 - position.x) << 8;
	else
		REG_BG2X = (112 - position.x) << 8;
	frames = 74;
	if (mode != 1)
		frames = 48;
	for (frame = 0; frame != frames; frame++)
	{
		cell = frame / 4;
		if (cell <= 5)
		{
			if (cell <= 3)
				draw[0](canvas, work->sheet + TwoResource_CellSourceOffsets[cell],
					TwoResource_CellX[cell + work->effect->side * 6],
					TwoResource_CellBiasY[cell] + 32, TwoResource_CellWidths[cell],
					TwoResource_CellHeights[cell]);
			else
				draw[0](canvas, Ram_MapCellBuffer + TwoResource_CellSourceOffsets[cell],
					TwoResource_CellX[cell + work->effect->side * 6],
					TwoResource_CellBiasY[cell] + 32, TwoResource_CellWidths[cell],
					TwoResource_CellHeights[cell]);
		}
		if (frame == 8)
		{
			if (mode == 0)
			{
				BattleEventRuntime_BeginPhaseFar(0x85);
				BattleMotion_ApplyVariantMotionFar(work->effect->actors[0], 1);
			}
			else
			{
				AudioCommand_PlayFar(0x86);
				ObjectGroup_UpdateMembers(work->effect->actors[0], 7, 5, 0, 4);
			}
			work->shake_frames = 8;
		}
		if (mode == 1)
		{
			if (frame == 13)
			{
				object->velocity_y = 0xc0000;
				object->vertical_motion_strength = 0x7851;
				object->vertical_motion_phase = 0x4000;
			}
			if (frame == 65)
			{
				work->shake_frames = 4;
				BattleEventRuntime_BeginPhaseFar(0x86);
			}
		}
		Camera_ApplyShake(8, 8);
		ObjectGroup_TickMemberTimers();
		work->transfer_pending = 1;
		WaitFrames(1);
	}
	Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
	BattleFx_EndCanvasLayer();
}

#define ColorBuffer_BackupAndHalve ((void (*)(void *, void *, s32))ColorBuffer_BackupAndHalve)
#define ColorBuffer_BackupAndScaleThreeQuarters \
	((void (*)(void *, void *, s32))ColorBuffer_BackupAndScaleThreeQuarters)
#define ColorBuffer_BackupAndDarken \
	((void (*)(void *, s32, void *, s32))ColorBuffer_BackupAndDarken)
#define ColorBuffer_BackupAndBrighten \
	((void (*)(void *, s32, void *, s32))ColorBuffer_BackupAndBrighten)

static __inline__ void CopyWords(void *destination, const void *source, s32 size)
{
	/* FAKEMATCH: needed to match: a direct call changes BattlePresentation_ProcessPendingGraphicsTransfer from push {r5, r6, lr} to push {r5, r6, r7, lr} */
	Iwram_CopyWords(destination, source, size);
}

static __inline__ void FillWords(void *destination, s32 size, s32 value)
{
	/* FAKEMATCH: needed to match: a direct call changes BattlePresentation_ProcessPendingGraphicsTransfer from mov r0, r5 to lsl r1, r1, #7 */
	Iwram_FillWords(destination, size, value);
}

void AudioCommand_PlayFar(s32 cue);

void BattleFx_RunSpiderWeb(struct BattleEffectArgument *efx)
{
	struct EffectPosition	 first;
	struct EffectPosition	 last;
	void					*canvas;
	struct BattleEffectWork *work;
	s32						 size;
	s32						 step;

	/* FAKEMATCH: the frame counter is pinned to r9, as the reference keeps it. */
	register s32 frame __asm__("r9");
	s32			 i;

	/* FAKEMATCH: needed to match: size and step first hold the slot table and blend register addresses, sharing their registers */
	size = (s32)&Ram_WorkSlot[40];
	canvas = *(void **)size;
	work = *(struct BattleEffectWork **)(size - 4);
	work->effect = efx;
	BattleFx_BeginCanvasLayer(2);
	step = 0x04000052;
	REG_BG2PA = 0x100;
	*(u16 *)step = 0x1000;
	EffectPosition_ApplyStepAndYOffset(work->effect->actors[0], &first);
	EffectPosition_ApplyStepAndYOffset(work->effect->actors[work->effect->count - 1], &last);
	first.x += (last.x - first.x) / 2;
	REG_BG2X = (64 - first.x) << 8;
	Resource_LoadAndDecompress((s32)&ResourceId_SpiderWebSheet, work->sheet, 1, 1);
	work->transfer_mode = 1;
	work->transfer_value = 0;
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
	AudioCommand_PlayFar(143);
	frame = 0;
	step = 1;
	size = 32;
	do
	{
		if (frame <= 8)
			REG_BLDALPHA = (frame << 1) | 0x1000;
		if (frame > 53)
			REG_BLDALPHA = (0x7c - (frame << 1)) | 0x1000;
		BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 3, step);
		((DrawRectangle)Ram_WorkSlot[46])(canvas, work, 33, 41, size, size);
		Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
		BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 7, step);
		((DrawRectangle)Ram_WorkSlot[46])(canvas, work, 64, 41, size, size);
		Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
		BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 11, step);
		((DrawRectangle)Ram_WorkSlot[46])(canvas, work, 33, 72, size, size);
		Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
		BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 15, step);
		((DrawRectangle)Ram_WorkSlot[46])(canvas, work, 64, 72, size, size);
		Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
		if (frame == 32)
			BattleEventRuntime_BeginPhaseFar(143);
		for (i = 0; i != work->effect->count; i++)
		{
			if (frame == 10)
				ObjectGroup_UpdateMembers(work->effect->actors[i], 7, -1, i, 8);
		}
		ObjectGroup_TickMemberTimers();
		work->transfer_pending = step;
		WaitFrames(1);
		frame++;
	} while (frame != 63);
	Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
	BattleFx_EndCanvasLayer();
}

void BattleEffect_WipeCanvas(s32 mode, s32 value)
{
	struct BattleEffectWork *work;
	u8						*canvas;
	u8						 delay[128];
	s32						 x;
	s32						 y;
	s32						 pos;

	canvas = *(gWorkSlot + HEAP_SLOT_BATTLE_CANVAS);
	work = *(gWorkSlot + HEAP_SLOT_BATTLE_EFFECT);
	for (x = 0; x != 128; x++)
		delay[x] = Random16() & 0x3f;

	if (mode == 1)
	{
		s32 front;
		s32 speed;

		front = 0;
		speed = 1;
		x = 0;
		do
		{
			front += speed;
			speed++;
			for (; x != front; x++)
			{
				for (y = 0; y != 128; y++)
				{
					pos = x - delay[y];
					if (pos >= 0)
						if (pos <= 127)
							canvas[(((y / 8) * 16 + pos / 8) * 8 + (y & 7)) * 8 + (pos & 7)] =
								1 - value;
				}
			}
			work->transfer_pending = 1;
			WaitFrames(1);
		} while (front <= 256);
	}
	else
	{
		s32 front;
		s32 speed;

		front = 0;
		speed = 1;
		y = 0;
		do
		{
			front += speed / 2;
			speed += 4;
			for (; y != front; y++)
			{
				for (x = 0; x != 128; x++)
				{
					pos = y - delay[x];
					if (pos >= 0)
						if (pos <= 127)
							canvas[(((pos / 8) * 16 + x / 8) * 8 + (pos & 7)) * 8 + (x & 7)] =
								1 - value;
				}
			}
			work->transfer_pending = 1;
			WaitFrames(1);
		} while (front <= 191);
	}
}

void BattlePresentation_ProcessPendingGraphicsTransfer(void)
{
	void				   **heap_cache = (void **)gBattleFxWork;
	struct BattleEffectWork *work = heap_cache[0];
	void					*source;
	s32						*counter;
	s32						 next_counter;

	if (work->transfer_pending == 1)
	{
		source = heap_cache[1];
		switch (work->transfer_mode)
		{
		case 0:
			CopyWords((void *)0x06004000, source, 0x4000);
			break;
		case 1:
			CopyWords((void *)0x06004000, source, 0x4000);
			FillWords(source, 0x4000, work->transfer_value);
			break;
		case 2:
			if (work->transfer_value == 50)
			{
				ColorBuffer_BackupAndHalve(source, (void *)0x06004000, 0x4000);
			}
			else
			{
				ColorBuffer_BackupAndScaleThreeQuarters(source, (void *)0x06004000, 0x4000);
			}
			break;
		case 3:
			ColorBuffer_BackupAndDarken(source, work->transfer_value, (void *)0x06004000, 0x4000);
			break;
		case 4:
			ColorBuffer_BackupAndBrighten(source, work->transfer_value, (void *)0x06004000, 0x4000);
			break;
		}
		work->transfer_pending = 0;
		counter = &work->frames_since_transfer;
		next_counter = 1;
	}
	else
	{
		counter = &work->frames_since_transfer;
		next_counter = *counter + 1;
	}
	*counter = next_counter;
}

void BattleFx_FlushPendingGraphicsTransfer(void)
{
	void				   **heap_cache;
	struct BattleEffectWork *work;
	void					*source;
	s32						 transfer_mode;

	heap_cache = (void **)gBattleFxWork;
	work = heap_cache[0];
	source = Ram_MapCellBuffer;
	if (work->transfer_pending != 1)
		return;

	transfer_mode = work->transfer_mode;
	switch (transfer_mode)
	{
	case 0:
		CopyWords((void *)0x06008000, source, 0x7800);
		break;
	case 1:
		CopyWords((void *)0x06008000, source, 0x7800);
		FillWords(source, 0x7800, work->transfer_value);
		break;
	case 2:
		if (work->transfer_value == 50)
		{
			ColorBuffer_BackupAndHalve(source, (void *)0x06008000, 0x7800);
		}
		else
		{
			ColorBuffer_BackupAndScaleThreeQuarters(source, (void *)0x06008000, 0x7800);
		}
		break;
	case 3:
		ColorBuffer_BackupAndDarken(source, work->transfer_value, (void *)0x06008000, 0x7800);
		break;
	}

	work->transfer_pending = 0;
}

#define gBattleFxWork (*(struct BattleEffectWork **)&gBattleFxWork)

#define FADE_STEP 1092

void Graphics_ApplyWindowBlendRegisters(void)
{
	struct BattleEffectWork *work = gBattleFxWork;

	REG_WIN0H = work->win0h;
	REG_WIN0V = work->win0v;
	REG_WIN1H = work->win1h;
	REG_WIN1V = work->win1v;
	REG_WININ = work->winin;
	REG_WINOUT = work->winout;
	REG_DISPCNT = work->dispcnt;
	REG_BLDCNT = work->bldcnt;
	REG_BLDALPHA = work->bldalpha;
}

void Display_ApplyBg2Reference(void)
{
	struct BattleEffectWork *work = gBattleFxWork;

	REG_BG2X = work->bg2x;
	REG_BG2Y = work->bg2y;
}

void Palette_StepFadeTransfer(void)
{
	/* FAKEMATCH: needed to match: relative heap cells 9 and 39 avoid address instructions added by named globals or full heap indexing. */
	void				   **slots = (void **)((u8 *)gWorkSlot + 9 * sizeof(void *));
	struct BattleEffectWork *work = slots[39 - 9];
	struct BtlWork			*battle = slots[0];

	if (work->fade_frames > 0)
	{
		s32 step = ++work->fade_step;

		Graphics_ScaleRgb555ClampedFar(
			battle->palette, (void *)&BG_PLTT_COLOR(6, 0), 0x10000 - step * FADE_STEP, 128);
		work->fade_frames--;
	}
}

void BattleFx_ClearActorTimers(void)
{
	Iwram_ClearWords(gBattleFxWork->actor_timers, sizeof(gBattleFxWork->actor_timers));
}

void ObjectGroup_TickMemberTimers(void)
{
	struct BattleEffectWork *work;
	s32						 i;

	work = gBattleFxWork;
	i = 0;
	do
	{
		if (work->actor_timers[i] != 0)
		{
			if ((work->actor_timers[i] = work->actor_timers[i] - 1) == 0)
			{
				ObjectGroup_UpdateMembers(work->effect->actors[i], 0, -1, -1, 0);
			}
		}
		i += 1;
	} while (i != 8);
}

//--------------------------------------------------------------------------

// canvas.c

// バトル演出：BG2 のキャンバス面を開く・閉じる

//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
// キャンバスは 128x128 の 8bpp（0x4000 バイト）で、BG2 のアフィン面に
// キャラクタブロック 1 から並べる。横は 2 倍に伸ばして 256 ドットで見せる
//
// REG_DISPCNT（モード 1・OBJ 1 次元）
//   0x7741 : BG0〜2・OBJ・WIN0/1
//   0x7341 : BG0・BG1・OBJ・WIN0/1（BG2 を消す）
//   0x7541 : BG0・BG2・OBJ・WIN0/1（BG1 を消す）
//   0x1741 : BG0〜2・OBJ
//   0x1341 : BG0・BG1・OBJ
//   0x1541 : BG0・BG2・OBJ
//--------------------------------------------------------------------------

#define BattlePresentation_SetPaletteLevelFar \
	((void (*)(u16, s32))BattlePresentation_SetPaletteLevelFar)

typedef s32 (*ClearWordsFn)(void *destination, s32 size);

static __inline__ void ClearWords(ClearWordsFn clear, void *destination, s32 size)
{
	/* FAKEMATCH: needed to match: direct calls share the size in r5 and move the routine to r6, changing the canvas tails */
	clear(destination, size);
}

void AudioCommand_PlayFar(s32);

// REG_DISPCNT への書き込みを I/O 書き込みキューに積む（IME を止めて）
#define QUEUE_DISPLAY_CONTROL(control)                                                                \
	do                                                                                                \
	{                                                                                                 \
		/* FAKEMATCH: removing this one-pass boundary changes measured instruction scheduling. */     \
		u32 saved;                                                                                    \
		s32 count;                                                                                    \
                                                                                                      \
		do                                                                                            \
		{                                                                                             \
			/* FAKEMATCH: removing this one-pass boundary changes measured instruction scheduling. */ \
			saved = *ime;                                                                             \
		} while (0);                                                                                  \
		*ime = (u16)ime;                                                                              \
		count = q->count;                                                                             \
		if (count <= 31)                                                                              \
		{                                                                                             \
			u32 *destination = (u32 *)((u8 *)q + count * 12 + 4);                                     \
			*(u16 *)&q->count = count + 1;                                                            \
			*destination++ = (control);                                                               \
			*destination++ = (u32) & REG_DISPCNT;                                                     \
			*destination = 0x20000;                                                                   \
		}                                                                                             \
		*ime = saved;                                                                                 \
	} while (0)

//--------------------------------------------------------------------------
// キャンバス面を開く（背景パレットをフェードさせ、BG2 をキャンバスにする）
// bg_control : REG_BG2CNT に足す値
//--------------------------------------------------------------------------
void BattleFx_BeginCanvasLayer(s32 bg_control)
{
	/* FAKEMATCH: needed to match: the one-pass IME read and u16 count store preserve queue scheduling and whole-function pointer lifetimes */

	struct BattleEffectWork		*work = *(gWorkSlot + HEAP_SLOT_BATTLE_EFFECT);
	struct BtlWork				*btl = *(gWorkSlot + HEAP_SLOT_BATTLE);
	void						*canvas = *(gWorkSlot + HEAP_SLOT_BATTLE_CANVAS);
	struct BattleBackgroundView *display = *(gWorkSlot + HEAP_SLOT_BATTLE_BACKGROUND);
	volatile u16				*ime;
	struct IoWriteQueue			*q;
	s32							 row;
	s32							 col;
	s32							 ofs;

	BattleFx_ClearActorTimers();
	display->second_mode = 1;
	WaitFrames(1);
	REG_BLDCNT = 0;
	q = &gIoWriteQueue;
	ime = &REG_IME;
	QUEUE_DISPLAY_CONTROL(0x7741);
	gBgScroll[1].y = 32;
	WaitFrames(1);

	// 背景パレットのフェードを始める
	BattlePresentation_ConfigurePaletteFadeFar(1, btl->background, 0);
	work->fade_frames = 0;
	work->fade_step = 0;
	Scheduler_AddOrUpdateCallback(Palette_StepFadeTransfer, 0xc80);

	// BG2 を消している間に設定を変える
	IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT, 0x7341, 0x20000);
	WaitFrames(1);
	REG_BG2CNT = bg_control | 0x784;
	IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT, 0x7341, 0x20000);
	BattlePres_SetupTransitionSceneFar(0, 0, 0, 100);
	display->second_mode = 0;
	WaitFrames(1);

	REG_BLDCNT = 0x3f44;   // BG2 を半透明にして下の面と重ねる
	REG_BLDALPHA = 0x100e; // 14:16
	REG_BG2X = 0;
	REG_BG2Y = -0x1000; // 16 ライン下げる
	REG_BG2PA = 0x80;	// 横 2 倍
	REG_BG2PB = 0;
	REG_BG2PC = 0;
	REG_BG2PD = 0x100;
	REG_WIN0H = 0xf0;	// 0〜240
	REG_WIN0V = 0x1088; // 16〜136
	REG_WIN1H = 0xf0;
	REG_WIN1V = 0x1088;
	REG_WININ = 0x3537;
	REG_WINOUT = 0x3f21;
	QUEUE_DISPLAY_CONTROL(0x7741);

	// スクリーン：16x16 キャラクタを順に並べる（1 キャラ 1 バイトなので 2 個ずつ書く）
	ofs = 0;
	for (row = 0; row != 16; row++)
	{
		for (col = 0; col != 8; col++, ofs += 2)
		{
			s32 chr;
			s16 pair;

			chr = row * 16 + col * 2;
			pair = ((chr + 1) << 8) | chr;
			*(volatile s16 *)((u8 *)BG_CANVAS_MAP + ofs) = pair;
		}
	}

	ClearWords(Iwram_ClearWords, canvas, 0x4000);
	ClearWords(Iwram_ClearWords, BG_CHAR_BLOCK(1), 0x4000);
	work->shake_frames = 0;
	work->saved_bg1_x = gBgScroll[1].x;
	work->saved_bg1_y = gBgScroll[1].y;
	WaitFrames(1);
}

//--------------------------------------------------------------------------
// キャンバス面を開く（ウインドウなしで始め、fade_frames を 24 にする版）
// bg_control : REG_BG2CNT に足す値
//--------------------------------------------------------------------------
void BattleFx_OpenCanvasLayer(s32 bg_control)
{
	/* FAKEMATCH: needed to match: the one-pass IME read and u16 count store preserve queue scheduling and whole-function pointer lifetimes */

	struct BattleEffectWork		*work = *(gWorkSlot + HEAP_SLOT_BATTLE_EFFECT);
	struct BtlWork				*btl = *(gWorkSlot + HEAP_SLOT_BATTLE);
	void						*canvas = *(gWorkSlot + HEAP_SLOT_BATTLE_CANVAS);
	struct BattleBackgroundView *display = *(gWorkSlot + HEAP_SLOT_BATTLE_BACKGROUND);
	volatile u16				*ime;
	struct IoWriteQueue			*q;
	s32							 row;
	s32							 col;
	s32							 ofs;

	BattleFx_ClearActorTimers();
	display->second_mode = 1;
	WaitFrames(1);
	REG_BLDCNT = 0;
	q = &gIoWriteQueue;
	ime = &REG_IME;
	QUEUE_DISPLAY_CONTROL(0x1741);
	gBgScroll[1].y = 32;
	WaitFrames(1);
	BattlePresentation_ConfigurePaletteFadeFar(1, btl->background, 128);
	work->fade_frames = 24;
	work->fade_step = 0;
	Scheduler_AddOrUpdateCallback(Palette_StepFadeTransfer, 0xc80);
	QUEUE_DISPLAY_CONTROL(0x1341);
	WaitFrames(1);
	REG_BG2CNT = bg_control | 0x784;
	QUEUE_DISPLAY_CONTROL(0x1341);
	BattlePres_SetupTransitionSceneFar(0, 0, 0, 100);
	display->second_mode = 0;
	WaitFrames(1);

	REG_BLDCNT = 0x3f44;
	REG_BLDALPHA = 0x100e;
	REG_BG2X = 0;
	REG_BG2Y = -0x1000;
	REG_BG2PA = 0x80;
	REG_BG2PB = 0;
	REG_BG2PC = 0;
	REG_BG2PD = 0x100;
	REG_WIN0H = 0xf0;
	REG_WIN0V = 0x1088;
	REG_WIN1H = 0xf0;
	REG_WIN1V = 0x1088;
	REG_WININ = 0x3537;
	REG_WINOUT = 0x3f21;
	QUEUE_DISPLAY_CONTROL(0x7741);
	WaitFrames(1);

	ofs = 0;
	for (row = 0; row != 16; row++)
	{
		for (col = 0; col != 8; col++, ofs += 2)
		{
			s32 chr;
			s16 pair;

			chr = row * 16 + col * 2;
			pair = ((chr + 1) << 8) | chr;
			*(volatile s16 *)((u8 *)BG_CANVAS_MAP + ofs) = pair;
		}
	}

	ClearWords(Iwram_ClearWords, canvas, 0x4000);
	ClearWords(Iwram_ClearWords, BG_CHAR_BLOCK(1), 0x4000);
	WaitFrames(1);
}

#undef QUEUE_DISPLAY_CONTROL

//--------------------------------------------------------------------------
// キャンバス面を開き、BG2 を 256x256 の折り返しありにする
// スクリーンの各行は 16 キャラのキャンバスと、16 キャラの 0 番
// bg_control : REG_BG2CNT に足す値
//--------------------------------------------------------------------------
void BattleFx_BeginTiledCanvas(s32 bg_control)
{
	s32 row;
	s32 col;
	s32 ofs;

	BattleFx_BeginCanvasLayer(bg_control);
	REG_BG2CNT = bg_control | 0x6784;
	ofs = 0;
	for (row = 0; row != 16; row++)
	{
		for (col = 0; col != 8; col++, ofs += 2)
		{
			s32 chr;
			s16 pair;

			chr = row * 16 + col * 2;
			pair = ((chr + 1) << 8) | chr;
			*(volatile s16 *)((u8 *)BG_CANVAS_MAP + ofs) = pair;
		}
		for (col = 0; col != 8; col++, ofs += 2)
		{
			*(volatile u16 *)((u8 *)BG_CANVAS_MAP + ofs) = 0;
		}
	}
}

//--------------------------------------------------------------------------
// キャンバス面を閉じ、背景パレットのレベルを 8 フレームで 21 から 0 へ戻す
//--------------------------------------------------------------------------
void BattleFx_EndCanvasLayer(void)
{
	/* FAKEMATCH: needed to match: the one-pass IME/queue scopes and u16 count store preserve scheduling, loading the queue before the preceding store */

	struct BattleEffectWork *work = *(gWorkSlot + HEAP_SLOT_BATTLE_EFFECT);
	struct BtlWork			*btl = *(gWorkSlot + HEAP_SLOT_BATTLE);
	s32						 i;

	AudioCommand_PlayFar(289);
	gBgScroll[1].x = work->saved_bg1_x;
	gBgScroll[1].y = work->saved_bg1_y;
	gProjection.center_x = 120;
	gProjection.center_y = 120;
	REG_BG2CNT = 0x787;
	Iwram_ClearWords(BG_CHAR_BLOCK(1), 0x4000);
	Scheduler_RemoveCallback(Palette_StepFadeTransfer);
	gBgScroll[1].y = 32;
	IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT, 0x7341, 0x20000);
	REG_BLDCNT = 0;
	WaitFrames(1);

	BattlePresentation_ConfigurePaletteFadeFar(2, btl->background, 7);
	WaitFrames(1);
	for (i = 0; i != 8; i++)
	{
		BattlePresentation_SetPaletteLevelFar(btl->background, 21 - i * 3);
		WaitFrames(1);
	}

	IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT, 0x7541, 0x20000);
	WaitFrames(1);
}

//--------------------------------------------------------------------------
// BG1 とウインドウを消して、背景パレットのフェードを設定する
//--------------------------------------------------------------------------
void BattleFx_SetTransitionFlagAndDisplay(void)
{
	struct BtlWork *btl;
	s32			   *flag;

	flag = &((struct BattleBackgroundView *)Ram_WorkSlot[HEAP_SLOT_BATTLE_BACKGROUND])->second_mode;
	btl = Ram_WorkSlot[HEAP_SLOT_BATTLE];
	*flag = 1;
	QueueIoWriteDelay2((u32)&REG_DISPCNT, 0x1541);
	WaitFrames(1);
	BattlePresentation_ConfigurePaletteFadeFar(2, btl->background, 0);
	WaitFrames(1);
}

#define BattlePresentation_ConfigurePaletteFadeFar \
	((void (*)(s32, s32, s32))BattlePresentation_ConfigurePaletteFadeFar)

typedef s32 (*ClearFn)(void *dst, s32 size);
typedef s32 (*FillFn)(void *dst, s32 size, u32 value);

static __inline__ void ClearWords_2(ClearFn clear, void *dst, s32 size)
{
	/* FAKEMATCH: needed to match: direct calls share the 0x4000 size in r5 and move the clear routine to r6 */
	clear(dst, size);
}

static __inline__ void FillWords_2(FillFn fill, void *dst, s32 size, s32 value)
{
	/* FAKEMATCH: needed to match: a direct call loads the destination before the routine and reverses their pool words */
	fill(dst, size, value);
}

struct BattleScreen
{
	u8	unknown_000[0x648];
	u16 palette_fade;
};

struct BattleLayer
{
	u8	unknown_00[12];
	s32 offset;
};

void BattleFx_SetupCanvasTileMap(void)
{
	u8				   **cells = (u8 **)&BtlWk;
	void				*canvas = cells[31];
	struct BattleScreen *screen = (struct BattleScreen *)cells[0];
	struct BattleLayer	*layer = (struct BattleLayer *)cells[35];
	s32					 row;
	s32					 col;
	s32					 off;

	BattleFx_ClearActorTimers();
	BattlePresentation_ConfigurePaletteFadeFar(2, screen->palette_fade, 0);
	gBgScroll[1].y = 32;
	layer->offset = 0;
	ClearWords_2(Iwram_ClearWords, (void *)0x06003fc0, 64);
	FillWords_2(Iwram_FillWords, (void *)0x0600f900, 0x200, -1);
	off = 0;
	for (row = 0; row != 16; row++)
	{
		for (col = 0; col != 32; col++, off += 2)
		{
			if (col > 15)
				*(volatile u16 *)(0x0600fb00 + off) = 0xff;
			else
				*(volatile u16 *)(0x0600fb00 + off) = row * 16 + 0x100 + col;
		}
	}
	REG_DISPCNT = 0x7741;
	REG_BG1CNT = 0x1f81;
	REG_BLDCNT = 0x3f42;
	REG_WIN0H = 0xf0;
	REG_WIN0V = 0x1088;
	REG_WIN1H = 0xf0;
	REG_WIN1V = 0x1088;
	REG_WININ = 0x3537;
	REG_WINOUT = 0x3f21;
	REG_BLDALPHA = 0x100e;
	ClearWords_2(Iwram_ClearWords, canvas, 0x4000);
	ClearWords_2(Iwram_ClearWords, BG_CHAR_BLOCK(1), 0x4000);
	WaitFrames(1);
}
