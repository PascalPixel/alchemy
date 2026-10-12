#include "unit/battle/effect/fxscales.h"
#include "unit/battle/effect/koukabit.h"
#include "unit/battle/effect/koukatbl.h"
#include "unit/battle/effect/effect.h"
#include "unit/battle/effect/effect8.h"
#include "unit/battle/effect/effect9.h"
#include "unit/battle/effect/effect10.h"
#include "unit/battle/effect/effect11.h"
#include "unit/battle/effect/object.h"
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

void BattleFx_DispatchByIdRange(s32 *arg0)
{
	s32 kind;
	s32 value;

	value = 0x60e;
	Runtime_AllocateBlock(41, value);
	Runtime_AllocateHeapBlock(HEAP_SLOT_BATTLE_EFFECT, sizeof(struct BattleEffectWork));
	Runtime_AllocateHeapBlock(HEAP_SLOT_BATTLE_CANVAS, 0x4000);
	value = *arg0;
	kind = value;
	value = kind - 100;
	if ((u32)value <= 35)
	{
		BattleFx_RunCastingImpact((struct BattleEffectArgument *)arg0, value);
	}
	else if (kind > 199)
	{
		BattlePres_RunRingAndSparkScene((struct BattleEffectArgument *)arg0);
	}
	else
	{
		BattlePres_RunBeamSequence((struct BattleEffectArgument *)arg0);
	}
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BATTLE_CANVAS);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BATTLE_EFFECT);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BATTLE_SHEET);
}
void BattlePres_RunBeamSequence(struct BattleEffectArgument *effect)
{
	struct EffectPosition	 pos;
	struct EffectPosition	 anchor;
	struct EffectPosition	 point;
	void				   **heap_cache;
	void				   **cursor;
	struct BattleEffectWork *work;
	void					*canvas;
	s32						 shake;
	u8						*sheet;
	struct BattleCamera		*camera;
	s32						 kind;
	struct BtlUnit			*unit;
	DrawRectangle			 draw[2];
	DrawRectangle			*slots;
	struct MotionObject		*target;
	void					*colors = (void *)0x05000000;
	s32						 half;
	s32						 frame;
	s32						 i;

	heap_cache = (void **)gBattleFxWork;
	cursor = heap_cache;
	work = *cursor++;
	canvas = *cursor;
	sheet = heap_cache[2];
	camera = *(struct BattleCamera **)((u8 *)heap_cache - 108);
	kind = effect->kind;
	work->effect = effect;
	unit = UnitGetFar(effect->actor);
	WaitFrames(1);
	BattlePres_ConfigureEffectDisplay();
	BattleFx_SetupCanvasTileMap();
	REG_BG1CNT = 0x1f80;
	WaitFrames(1);
	if (kind == 5)
	{
		if (work->effect->side == 0)
		{
			BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 11, 3);
			BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, 11, 2);
		}
		else
		{
			BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 15, 3);
			BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, 15, 2);
		}
	}
	else
	{
		if (work->effect->side == 0)
		{
			BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 3, 3);
			BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, 3, 2);
		}
		else
		{
			BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 7, 3);
			BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, 7, 2);
		}
	}
	slots = (DrawRectangle *)gWorkSlot;
	draw[0] = slots[HEAP_SLOT_BLITTER];
	draw[1] = slots[HEAP_SLOT_BLITTER_ALTERNATE];
	WaitFrames(1);
	if (kind == 4)
	{
		Resource_LoadAndDecompress((s32)&ResourceId_WaveSheet, work, 1, 1);
	}
	else if (kind == 3)
	{
		Resource_LoadAndDecompress((s32)&ResourceId_BeamSequenceImage, work, 0, 0);
	}
	else
	{
		switch (kind)
		{
		case 0:
		case 1:
		case 5:
			Resource_LoadAndDecompress((s32)&ResourceId_BlueArcSheetA, work, 1, 1);
			break;
		case 2:
			Resource_LoadAndDecompress((s32)&ResourceId_BlueArcSheetB, work, 1, 1);
			break;
		}
	}
	if (work->effect->actor > 7)
		Iwram_CopyWords(colors, Resource_GetTableEntry((s32)&ResourceId_MarsDjinnSmallSheet), 128);
	else
		Iwram_CopyWords(colors, Resource_GetTableEntry((s32)&ResourceId_CrescentSheet), 128);
	WaitFrames(1);
	Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesD, sheet, 0, 0);
	Resource_LoadAndDecompress((s32)&ResourceId_YellowOrbSheet, Ram_MapCellBuffer, 1, 0);
	work->transfer_mode = 2;
	work->transfer_value = 50;
	Scheduler_AddOrUpdateCallback((s32)BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
	REG_BG1CNT = 0x1f81;
	EffectPosition_ApplyAnimationAndYOffset(work->effect->actors[0], &anchor);
	if (work->effect->side == 0)
		shake = 96 - anchor.x;
	else
		shake = 32 - anchor.x;
	if (shake > 0)
		shake = 0;
	if (shake < -128)
		shake = -128;
	anchor.x += shake;
	gBgScroll[1].x = shake;
	gBgScroll[1].y = 80;
	WaitFrames(1);
	target = GetBattleObjectSlotFar(work->effect->actors[0])->object;
	half = Battle_GetObjectTableValueFar(work->effect->actors[0]) / 2;
	for (i = 0; i != 64; i++)
	{
		struct EffectStep *spark = &work->particles[i];

		spark->x = target->x;
		spark->y = target->y + half;
		spark->z = target->z;
		spark->velocity_x = (Random16() & 255) << 10;
		spark->velocity_y = ((Random16() & 255) - 32) << 10;
		spark->velocity_z = ((Random16() & 255) - 127) << 10;
		if (spark->x > 0)
			spark->velocity_x = -spark->velocity_x;
		spark->velocity_x = -spark->velocity_x;
		spark->variant = i + 16;
	}

	for (frame = 0; frame != 32; frame++)
	{
		if (frame == 5)
		{
			if (Summon_IsEntrySecondaryFlaggedFar(unit->class_id))
				BattleEventRuntime_BeginPhaseFar(134);
			else
				BattleEventRuntime_BeginPhaseFar(133);
		}
		if (frame == 4)
			BattleMotion_ApplyVariantMotionFar(work->effect->actors[0], 0);
		EffectPosition_ApplyStepAndYOffset(work->effect->actor, &pos);
		pos.y += 16;
		if (kind == 4)
		{
			if (frame <= 11)
			{
				if (work->effect->side == 0)
					draw[1](canvas, work->sheet + (5 - frame / 2) * 0x300, pos.x + shake - 48,
						pos.y - 8, 48, 16);
				else
					draw[1](canvas, work->sheet + (5 - frame / 2) * 0x300, pos.x + shake, pos.y - 8,
						48, 16);
			}
		}
		else if ((u32)kind <= 2 || kind == 5)
		{
			if (frame <= 11)
			{
				if (work->effect->side == 0)
					draw[1](canvas, work->sheet + frame / 2 * 0xd80, pos.x + shake - 48, pos.y - 40,
						48, 72);
				else
					draw[1](
						canvas, work->sheet + frame / 2 * 0xd80, pos.x + shake, pos.y - 40, 48, 72);
			}
		}
		else
		{
			if (frame <= 17)
			{
				s32 cell = frame / 3;

				if (work->effect->side == 0)
					draw[1](canvas, work->sheet + BeamSequence_CellSheetOffsets[cell],
						pos.x + BeamSequence_CellReaches[cell] + shake - 58,
						pos.y - BeamSequence_CellHeights[cell] / 2, BeamSequence_CellWidths[cell],
						BeamSequence_CellHeights[cell]);
				else
					draw[1](canvas, work->sheet + BeamSequence_CellSheetOffsets[cell],
						pos.x - BeamSequence_CellReaches[cell] + shake
							- BeamSequence_CellWidths[cell] + 58,
						pos.y - BeamSequence_CellHeights[cell] / 2, BeamSequence_CellWidths[cell],
						BeamSequence_CellHeights[cell]);
			}
		}
		if (frame >= 4 && frame <= 15)
			draw[0](canvas, Ram_MapCellBuffer + (frame - 4) / 2 * 0x780, anchor.x - 16,
				anchor.y - 24, 40, 48);
		Render_ResetTransformState();
		Graphics_PrepareTransferInIwramWork(camera, (u8 *)camera->pos);
		if (frame >= 4 && frame <= 31)
		{
			for (i = 0; i != 64; i++)
			{
				s32				   pair = i / 2;
				struct EffectStep *spark = &work->particles[pair];
				s32				   size = spark->variant;

				if (size > 0)
				{
					Render_ProjectPoint(&spark->x, &point.x);
					point.x += shake;
					size >>= 3;
					size += 2;
					draw[pair & 1](canvas, sheet + BattleFx6_FlareCells[size - 1], point.x - size,
						point.y - size, size * 2, size * 2);
					EffectStep_AdvanceWithGravity3D(spark, 60, -0x400);
					spark->variant--;
				}
			}
		}
		work->transfer_pending = 1;
		WaitFrames(1);
	}

	Scheduler_RemoveCallback((u32)BattlePresentation_ProcessPendingGraphicsTransfer);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
	gBgScroll[1].y = 32;
	BattleFx_SetTransitionFlagAndDisplay();
}

void BattlePres_RunRingAndSparkScene(struct BattleEffectArgument *effect)
{
	struct EffectPosition	 pos;
	struct EffectPosition	 anchor;
	struct EffectPosition	 point;
	struct EffectPosition	 spot;
	void				   **heap_cache;
	void				   **cursor;
	struct BattleEffectWork *work;
	void					*canvas;
	s32						 shake;
	u8						*sheet;
	struct BattleCamera		*camera;
	s32						 velocity_x;
	s32						 velocity_y;
	s32						 velocity_z;
	s32						 acceleration;
	s32						 strength;
	DrawRectangle			 draw[2];
	DrawRectangle			*slots;
	struct MotionObject		*object;
	struct MotionObject		*target;
	u16						*background;
	void					*colors = (void *)0x05000000;
	s32						 words = 0x4000;
	s32						 big;
	s32						 half;
	s32						 frame;
	s32						 i;

	heap_cache = (void **)gBattleFxWork;
	cursor = heap_cache;
	work = *cursor++;
	canvas = *cursor;
	sheet = heap_cache[2];
	camera = *(struct BattleCamera **)((u8 *)heap_cache - 108);
	big = 1;
	if (effect->kind <= 199)
		big = 0;
	work->effect = effect;
	BattlePres_SetupTransitionAtPairMidpointFar(effect->actor, effect->unknown_000c, 130);
	WaitFrames(1);
	BattleFx_SetupCanvasTileMap();
	REG_BG1CNT = 0x1f80;
	if (work->effect->side == 0)
	{
		BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 3, 3);
		BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, 3, 2);
	}
	else
	{
		BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 7, 3);
		BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, 7, 2);
	}
	slots = (DrawRectangle *)gWorkSlot;
	draw[0] = slots[HEAP_SLOT_BLITTER];
	draw[1] = slots[HEAP_SLOT_BLITTER_ALTERNATE];
	BattlePres_SetupTransitionAtPairMidpointFar(
		work->effect->actor, work->effect->unknown_000c, 130);
	WaitFrames(1);
	Resource_LoadAndDecompress((s32)&ResourceId_StarBurstSheet, work, 1, 0);
	BattlePres_SetupTransitionAtPairMidpointFar(
		work->effect->actor, work->effect->unknown_000c, 130);
	WaitFrames(1);
	Resource_LoadAndDecompress((s32)&ResourceId_CrescentSheet, Ram_MapCellBuffer, 1, 1);
	if (work->effect->actor > 7)
		Iwram_CopyWords(colors, Resource_GetTableEntry((s32)&ResourceId_MarsDjinnSmallSheet), 128);
	BattlePres_SetupTransitionAtPairMidpointFar(
		work->effect->actor, work->effect->unknown_000c, 130);
	WaitFrames(1);
	Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesD, sheet, 0, 0);
	BattlePres_SetupTransitionAtPairMidpointFar(
		work->effect->actor, work->effect->unknown_000c, 130);
	WaitFrames(1);
	work->transfer_mode = 1;
	work->transfer_value = 0;
	Scheduler_AddOrUpdateCallback((s32)BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);

	if (big == 1)
	{
		object = GetBattleObjectSlotFar(work->effect->actor)->object;
		for (i = 0; i != 64; i++)
		{
			struct EffectStep *mote = &work->particles[i];

			mote->x = (Random16() & 63) + 16;
			mote->y = 0;
			mote->z = 0;
			mote->velocity_x = Random16() & 0xffff;
			mote->velocity_y = Random16() & 0xffff;
			mote->velocity_z = Random16() & 0xffff;
		}
		ObjectDispatch_ApplyValueToChildrenFar(object, 0);
		velocity_x = object->velocity_x;
		velocity_y = object->velocity_y;
		velocity_z = object->velocity_z;
		strength = object->vertical_motion_strength;
		acceleration = object->acceleration;
		object->velocity_x = 0;
		object->velocity_y = 0;
		object->velocity_z = 0;
		object->acceleration = 0;
		object->vertical_motion_strength = 0;
		EffectPosition_ApplyStepAndYOffset(work->effect->actor, &pos);
		shake = 64 - pos.x;
		gBgScroll[1].x = shake;
		gBgScroll[1].y = 80;
		work->fade_frames = 24;
		work->fade_step = 0;
		Scheduler_AddOrUpdateCallback((s32)Palette_StepFadeTransfer, 0xc80);
		AudioCommand_PlayFar(212);
		for (frame = 0; frame != 32; frame++)
		{
			BattlePres_SetupTransitionAtPairMidpointFar(
				work->effect->actor, work->effect->unknown_000c, 130);
			for (i = 0; i != 64; i++)
			{
				struct EffectStep *mote = &work->particles[i];

				if (mote->x >= 0 && frame >= i / 4)
				{
					s32 size = (i & 1) + 5;

					Render_ResetTransformState();
					SceneTransform_ApplyRoll(mote->velocity_z);
					SceneTransform_ApplyPitch(mote->velocity_x);
					SceneTransform_ApplyYaw(mote->velocity_y);
					EffectPosition_ApplyBaseAndYOffset(&mote->x, &point);
					point.x += 64;
					point.y = point.y + pos.y + 24;
					if (point.depth < -60)
						point.depth = -60;
					if (point.depth > 60)
						point.depth = 60;
					point.depth += 60;
					draw[0](canvas, sheet + BattleFx6_FlareCells[size - 1], point.x - size,
						point.y - size, size * 2, size * 2);
					mote->x -= 4;
				}
			}
			work->transfer_pending = 1;
			WaitFrames(1);
		}
		Scheduler_RemoveCallback((u32)Palette_StepFadeTransfer);
		ObjectDispatch_ApplyValueToChildrenFar(object, 16);
		object->velocity_x = velocity_x;
		object->velocity_y = velocity_y;
		object->velocity_z = velocity_z;
		object->acceleration = acceleration;
		object->vertical_motion_strength = strength;
	}

	Iwram_ClearWords(canvas, words);
	Iwram_ClearWords((void *)0x06004000, words);
	work->transfer_mode = 2;
	work->transfer_value = 75;
	REG_BG1CNT = 0x1f81;
	EffectPosition_ApplyStepAndYOffset(work->effect->actors[0], &anchor);
	if (work->effect->side == 0)
		shake = 32 - anchor.x;
	else
		shake = 96 - anchor.x;
	if (shake > 0)
		shake = 0;
	if (shake < -128)
		shake = -128;
	anchor.x += shake;
	gBgScroll[1].y = 80;
	gBgScroll[1].x = shake;
	target = GetBattleObjectSlotFar(work->effect->actors[0])->object;
	half = Battle_GetObjectTableValueFar(work->effect->actors[0]) / 2;
	for (i = 0; i != 64; i++)
	{
		struct EffectStep *spark = &work->particles[i];

		spark->x = target->x;
		spark->y = target->y + half;
		spark->z = target->z;
		spark->velocity_x = (Random16() & 255) << 10;
		spark->velocity_y = (Random16() & 255) << 10;
		spark->velocity_z = ((Random16() & 255) - 127) << 10;
		if (spark->x > 0)
			spark->velocity_x = -spark->velocity_x;
		spark->variant = i + 16;
	}

	for (frame = 0; frame != 32; frame++)
	{
		if (frame == 5)
			BattleEventRuntime_BeginPhaseFar(134);
		if (frame == 4)
			BattleMotion_ApplyVariantMotionFar(work->effect->actors[0], 0);
		EffectPosition_ApplyStepAndYOffset(work->effect->actor, &pos);
		pos.y += 16;
		if (frame <= 1)
			draw[0](canvas, work, 0, 0, 120, 120);
		else if (frame <= 3)
			draw[0](canvas, work->sheet + 0x3840, 0, 0, 120, 120);
		else if (frame <= 5)
			draw[0](canvas, Ram_MapCellBuffer, 0, 0, 120, 120);
		else if (frame <= 7)
			draw[0](canvas, Ram_MapCellBuffer + 0x3840, 0, 0, 120, 120);
		Render_ResetTransformState();
		Graphics_PrepareTransferInIwramWork(camera, (u8 *)camera->pos);
		if (frame >= 4 && frame <= 31)
		{
			for (i = 0; i != 64; i++)
			{
				s32				   pair = i / 2;
				struct EffectStep *spark = &work->particles[pair];
				s32				   size = spark->variant;

				if (size > 0)
				{
					EffectPosition_ApplyBaseAndYOffset(&spark->x, &spot);
					spot.x += shake;
					size >>= 3;
					size += 2;
					spot.y += 16;
					draw[pair & 1](canvas, sheet + BattleFx6_FlareCells[size - 1], spot.x - size,
						spot.y - size, size * 2, size * 2);
					EffectStep_AdvanceWithGravity3D(spark, 60, -0x400);
					spark->variant--;
				}
			}
		}
		work->transfer_pending = 1;
		WaitFrames(1);
	}

	Scheduler_RemoveCallback((u32)BattlePresentation_ProcessPendingGraphicsTransfer);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
	gBgScroll[1].y = 32;
	for (frame = 0, background = &BtlWk->background; frame != 7; frame++)
	{
		BattlePresentation_SetPaletteLevelFar(*background, 6 - frame);
		WaitFrames(1);
	}
	BattleFx_SetTransitionFlagAndDisplay();
}

void BattleFx_StepPaletteToResource(s32 resource_id)
{
	u16	 target[64];
	u16 *color = (u16 *)0x05000000;
	s32	 i;

	Iwram_CopyWords(target, Resource_GetTableEntry(resource_id), 128);
	target[0] = 0;
	for (i = 0; i != 64; i++, color++)
	{
		s32 red = *color & 0x1f;
		s32 green = (*color >> 5) & 0x1f;
		s32 blue = (*color >> 10) & 0x1f;
		s32 target_red = target[i] & 0x1f;
		s32 target_green = (target[i] >> 5) & 0x1f;
		s32 target_blue = (target[i] >> 10) & 0x1f;

		if (red < target_red)
			red++;
		else if (red > target_red)
			red--;
		if (green < target_green)
			green++;
		else if (green > target_green)
			green--;
		if (blue < target_blue)
			blue++;
		else if (blue > target_blue)
			blue--;
		target[i] = (blue << 10) | (green << 5) | red;
	}
	Iwram_CopyWords((u16 *)0x05000000, target, 128);
}

//--------------------------------------------------------------------------
// casting.c
// バトル演出：ジンの技（演出 100〜135）
//--------------------------------------------------------------------------

static __inline__ void LoadResource(const void *id, void *dest, s32 skip, s32 copy)
{
	/* FAKEMATCH: needed to match: inline scope keeps each resource call's arguments local */
	Resource_LoadAndDecompress((s32)id, dest, skip, copy);
}

static __inline__ void PackRows(void *src, void *dest, s32 width, s32 height)
{
	/* FAKEMATCH: needed to match: keep packing arguments within their own inline scope */
	Graphics_PackTileRows(src, dest, width, height);
}

static __inline__ void DrawImage(
	void *canvas, const void *pixels, s32 x, s32 y, s32 width, s32 height, DrawRectangle *draw)
{
	/* FAKEMATCH: needed to match: inline scope reloads each image address at its drawing call */
	(*draw)(canvas, pixels, x, y, width, height);
}

static __inline__ void DrawTallImage(
	void *canvas, const void *pixels, s32 x, s32 y, s32 width, DrawRectangle *draw)
{
	/* FAKEMATCH: needed to match: derive height inside the inline scope so the dimension stores reuse one register instead of keeping two argument temporaries alive */
	(*draw)(canvas, pixels, x, y, width, width * 2);
}

static __inline__ void DrawScrolledImage(void *canvas, const void *pixels, s32 x, s32 y, s32 width,
	s32 height, s32 scroll, DrawRectangle *draw)
{
	/* FAKEMATCH: needed to match: subtract the scrolled part inside the drawing scope */
	(*draw)(canvas, pixels, x, y, width, height - scroll);
}

static __inline__ void DrawCroppedImage(
	void *canvas, const void *pixels, s32 x, s32 y, s32 width, DrawRectangle *draw)
{
	/* FAKEMATCH: needed to match: keep the cropped height inside the drawing scope */
	(*draw)(canvas, pixels, x, y, width, 91);
}

//--------------------------------------------------------------------------
// ジンの技
// 行動側のまわりに火花を集め、ジンを対象へ飛ばして、技ごとの絵で当てる
// 15・17・24・26・35 は集めたあと別の演出へ引き継ぐ
// command : 演出の引数
// kind    : 技（演出番号 - 100）
//--------------------------------------------------------------------------
void BattleFx_RunCastingImpact(struct BattleEffectArgument *command, s32 kind)
{
	void					 **slots;
	void					 **slot;
	s32						   alive;
	s32						   cell_ofs;
	struct EffectPosition	   caster_pos;
	struct EffectPosition	   target_pos;
	struct FixedPointPosition  djinn_pos;
	s32						   phase;
	s32						   i;
	s32						  *scanline;
	s32						   n;
	s32						   djinn_x;
	DrawRectangle			  *slot_draw;
	struct BattleEffectWork	  *work;
	void					  *canvas;
	s32						   frame;
	struct BattleCamera		  *camera;
	u8						  *sprites;
	s32						   scroll_pos;
	s32						   scroll_speed;
	s32						   duration;
	struct EffectPosition	  *target_screen;
	struct EffectPosition	  *caster_screen;
	DrawRectangle			  *draw_pair;
	s32						   saved_velocity_x;
	s32						   saved_velocity_y;
	s32						   saved_velocity_z;
	s32						   saved_acceleration;
	s32						   saved_vertical_strength;
	struct MotionObject		  *target_actor;
	struct FixedPointPosition *motion;
	s32						   kind_from_two;
	s32						   kind_from_four;
	struct FixedPointPosition *position;
	struct MotionObject		  *caster_actor;
	struct EffectStep		  *particle;
	s32						   pair_x;
	s32						   strip_x;
	struct FixedPointPosition  velocity;
	struct EffectPosition	   spark_screen;
	DrawRectangle			   blitters[2];
	struct EffectPosition	   projected;

	slots = gWorkSlot + HEAP_SLOT_BATTLE_EFFECT;
	slot = slots;
	work = (struct BattleEffectWork *)*slot++;
	canvas = *slot;
	camera = *(gWorkSlot + HEAP_SLOT_CAMERA);
	sprites = (u8 *)slots[2];
	work->effect = command;
	if (kind == 11 || kind == 8 || kind == 32)
	{
		BattleFx_BeginTiledCanvas(0);
	}
	else
	{
		BattleFx_BeginCanvasLayer(0);
	}
	REG_BLDALPHA = 0x1010;

	//------------------------------------------------------------------
	// 絵
	//   work->sheet           : 行動側に重ねる絵（48x72 を 6 コマ）
	//   work->sheet + 0x5100  : 当たりの玉（20x48、横半分に詰める）
	//   gMapCellBuffer        : 技ごとの当たりの絵
	//   gMapCellBuffer+0x3c56 : ジン（属性ごと、20x40）
	//------------------------------------------------------------------
	LoadResource(&ResourceId_ParticleSpritesA, sprites, 0, 0);
	LoadResource(&ResourceId_FireStreakSheet, work, 1, 0);
	LoadResource(&ResourceId_YellowOrbSheet, Ram_MapCellBuffer, 1, 0);
	/* FAKEMATCH: needed to match: local dimensions preserve the packing argument order 2026-10-11: literal 40 and 288 differ 4 (movs r2, #40 and the buffer load move ahead of lsls r3). */
	{
		s32 width = 40;
		s32 height = 288;
		PackRows(Ram_MapCellBuffer, work->sheet + 0x5100, width, height);
	}
	if (kind == 5 || kind == 23)
	{
		LoadResource(&ResourceId_FlashBurstSheet, Ram_MapCellBuffer, 1, 0);
	}
	else if (kind == 12)
	{
		LoadResource(&ResourceId_SkullSheet, Ram_MapCellBuffer, 1, 0);
	}
	else if (kind == 6 || kind == 27)
	{
		LoadResource(&ResourceId_TornadoSheet, Ram_MapCellBuffer, 1, 0);
		LoadResource(&ResourceId_LightningBoltSheet, Ram_MapCellBuffer + 0xc56, 1, 0);
	}
	else if (kind == 31)
	{
		LoadResource(&ResourceId_RuneSheet, Ram_MapCellBuffer, 1, 1);
	}
	else if (kind == 8)
	{
		LoadResource(&ResourceId_RockSpireSheet, Ram_MapCellBuffer, 1, 1);
	}
	else if (kind == 14)
	{
		LoadResource(&ResourceId_BlueFlameSheet, Ram_MapCellBuffer, 1, 0);
	}
	else if (kind == 30)
	{
		LoadResource(&ResourceId_TornadoSheet, Ram_MapCellBuffer, 1, 0);
	}
	else if (kind == 16)
	{
		LoadResource(&ResourceId_IceChipSheet, Ram_MapCellBuffer, 1, 0);
	}
	else if (kind == 20)
	{
		LoadResource(&ResourceId_EmberStreakSheet, Ram_MapCellBuffer, 1, 0);
	}
	else if (kind == 33 || kind == 34)
	{
		LoadResource(&ResourceId_FlameSheetA, Ram_MapCellBuffer, 1, 0);
	}
	else if (kind != 11 && kind != 32)
	{
		LoadResource(&ResourceId_SmokeSheet, Ram_MapCellBuffer, 1, 0);
	}
	switch (kind) // ジンの属性
	{
	case 0: // 地
	case 4:
	case 7:
	case 8:
	case 9:
	case 10:
	case 11:
	case 12:
	case 13:
	case 33:
		LoadResource(&ResourceId_VenusDjinnSmallSheet, Ram_MapCellBuffer + 0x3c56, 1, 1);
		break;
	case 2: // 水
	case 14:
	case 15:
	case 16:
	case 17:
	case 18:
	case 19:
		LoadResource(&ResourceId_MercuryDjinnSmallSheet, Ram_MapCellBuffer + 0x3c56, 1, 1);
		break;
	case 3: // 火
	case 5:
	case 20:
	case 21:
	case 22:
	case 23:
	case 24:
	case 25:
	case 34:
	case 35:
		LoadResource(&ResourceId_MarsDjinnSmallSheet, Ram_MapCellBuffer + 0x3c56, 1, 1);
		break;
	case 1: // 風
	case 6:
	case 26:
	case 27:
	case 28:
	case 29:
	case 30:
	case 31:
	case 32:
		LoadResource(&ResourceId_JupiterDjinnSmallSheet, Ram_MapCellBuffer + 0x3c56, 1, 1);
		break;
	case 100:
		LoadResource(&ResourceId_MercuryDjinnSmallSheet, Ram_MapCellBuffer + 0x3c56, 1, 1);
		break;
	}
	work->transfer_mode = 2; // 残像（12 だけ 3/4、ほかは 1/2）
	if (kind == 12)
	{
		work->transfer_value = 75;
	}
	else
	{
		work->transfer_value = 50;
	}
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
	/* FAKEMATCH: needed to match: retain each output pointer while preparing its call 2026-10-11: the pointers set in statements before the calls differ 38, after the calls 142. */
	EffectPosition_ApplyStepAndYOffset(work->effect->actors[0], target_screen = &target_pos);
	EffectPosition_ApplyStepAndYOffset(work->effect->actor, caster_screen = &caster_pos);
	BattleFx_FetchRectangleBlitters(work->effect->side, draw_pair = blitters);
	work->fade_frames = 24;
	work->fade_step = 0;
	caster_actor = GetBattleObjectSlotFar(work->effect->actor)->object;

	// 集まる火花：x は半径（32〜95）、velocity は回す角度
	{
		for (i = 0; i != 64; i++)
		{
			particle = &work->particles[i];
			particle->x = (Random16() & 63) + 32;
			particle->y = 0;
			particle->z = 0;
			particle->velocity_x = Random16() & 0xffff;
			particle->velocity_y = Random16() & 0xffff;
			particle->velocity_z = Random16() & 0xffff;
		}
	}
	// 行動側の動きを止め、ジンを頭上 80 ドットに置く
	ObjectDispatch_ApplyValueToChildrenFar(caster_actor, 0);
	position = &djinn_pos;
	position->x = caster_actor->x;
	position->y = caster_actor->y + 0x500000;
	position->z = caster_actor->z;
	saved_velocity_x = caster_actor->velocity_x;
	saved_velocity_y = caster_actor->velocity_y;
	saved_velocity_z = caster_actor->velocity_z;
	saved_acceleration = caster_actor->acceleration;
	saved_vertical_strength = caster_actor->vertical_motion_strength;
	caster_actor->velocity_x = 0;
	caster_actor->velocity_y = 0;
	caster_actor->velocity_z = 0;
	caster_actor->acceleration = 0;
	caster_actor->vertical_motion_strength = 0;
	EffectPosition_ApplyStepAndYOffset(work->effect->actor, caster_screen);
	caster_screen->x = caster_screen->x / 2;
	AudioCommand_PlayFar(212);

	//------------------------------------------------------------------
	// 集める（32 フレーム）
	// 火花は毎フレーム 4 個ずつ動き出し、半径を 4 ずつ縮めて行動側へ集まる
	// 技 0〜7 は、火花が消え始めたらジンを出す
	//------------------------------------------------------------------
	frame = 0;
	do
	{
		n = 0;
		for (i = 0; i != 64; i++)
		{
			struct EffectStep *step = &work->particles[i];
			if (step->x >= 0)
			{
				if (frame >= i / 4)
				{
					{
						s32 size = 5;
						Render_ResetTransformState();
						SceneTransform_ApplyRoll(step->velocity_z);
						SceneTransform_ApplyPitch(step->velocity_x);
						SceneTransform_ApplyYaw(step->velocity_y);
						EffectPosition_ApplyBaseAndYOffset((s32 *)step, &spark_screen);
						spark_screen.x = spark_screen.x / 2 + caster_screen->x;
						if (kind <= 7)
							spark_screen.y = spark_screen.y + caster_screen->y - 8;
						else if (kind == 35)
							spark_screen.y = spark_screen.y + caster_screen->y + 44;
						else
							spark_screen.y = spark_screen.y + caster_screen->y + 12;
						if (spark_screen.depth < -60)
							spark_screen.depth = -60;
						if (spark_screen.depth > 60)
							spark_screen.depth = 60;
						spark_screen.depth += 60; // 0〜120
						blitters[1](canvas, sprites + ParticleStreams_CellOffsets[size - 1],
							spark_screen.x - size / 2, spark_screen.y - size, size, size * 2);
					}
					step->x -= 4;
				}
				n++;
			}
		}
		if (kind <= 7)
		{
			alive = n;
			if (alive <= 63)
			{
				Render_ResetTransformState();
				Graphics_PrepareTransferInIwramWork(camera->eye, camera->pos);
				EffectPosition_ApplyBaseAndYOffset((s32 *)&djinn_pos, &spark_screen);
				spark_screen.x /= 2;
				djinn_x = spark_screen.x;
				blitters[0](
					canvas, Ram_MapCellBuffer + 0x3c56, djinn_x - 10, spark_screen.y - 4, 20, 40);
			}
		}
		work->transfer_pending = 1;
		WaitFrames(1);
		frame++;
	} while (frame != 32);
	/* FAKEMATCH: needed to match: restart the pointer's lifetime after gathering so the projection above uses the stack position directly 2026-10-11: direct djinn_pos stores above, with this as the only assignment, differ 114, as does removing it. */
	position = &djinn_pos;

	//------------------------------------------------------------------
	// 技ごとの BG2 の準備：横を等倍にし、当たりの絵を対象に合わせる
	//------------------------------------------------------------------
	if (kind == 11)
	{
		REG_BG2PA = 0x100;
		if (work->effect->side == 0)
		{
			REG_BG2X = (frame - target_screen->x) << 8; // frame はここで 32
		}
		else
		{
			REG_BG2X = (96 - target_screen->x) << 8;
		}
	}
	else
	{
		if (kind == 32)
		{
			// 絵を横に流す（scroll_pos は 16.16 のドット）
			REG_BG2PA = 0x100;
			if (work->effect->side == 0)
			{
				scroll_pos = -0x800000;
				scroll_speed = 0xc0000;
			}
			else
			{
				scroll_pos = 0x80000;
				scroll_speed = -0xc0000;
			}
			REG_BG2X = (scroll_pos >> 16) << 8;
		}
	}
	if (kind == 8)
	{
		REG_BG2PA = 0x100;
		REG_BG2X = (64 - target_screen->x) << 8;
		work->transfer_mode = 1; // 毎フレーム 0 で消す
		work->transfer_value = 0;
		Iwram_ClearWords(BG_CHAR_BLOCK(1), 0x4000);
		Iwram_ClearWords(canvas, 0x4000);
		REG_BLDCNT = 0;
	}
	if (kind == 31)
	{
		REG_BG2PA = 0x100;
		if (work->effect->side == 0)
		{
			REG_BG2X = (32 - target_screen->x) << 8;
		}
		else
		{
			REG_BG2X = (96 - target_screen->x) << 8;
		}
	}
	// 15・17・24・26 は、キャンバスを片付けて別の演出へ引き継ぐ
	if (kind == 15 || kind == 17 || kind == 24 || kind == 26)
	{
		Iwram_ClearWords(BG_CHAR_BLOCK(1), 0x4000);
		Iwram_ClearWords(canvas, 0x4000);
		work->effect->prelude = 0;
		Scheduler_RemoveCallback(Palette_StepFadeTransfer);
		Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
		Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
		Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
		ObjectDispatch_ApplyArgumentToChildrenFar(caster_actor, 3);
		if (kind == 15)
		{
			BattleFx_RunProjectileVolley(command, 9);
		}
		if (kind == 24)
		{
			BattleFx_RenderMode5(command);
		}
		if (kind != 26)
		{
			return;
		}
		BattleFx_RunProjectileVolley(command, 8);
		return;
	}
	ObjectDispatch_ApplyValueToChildrenFar(caster_actor, 16);
	caster_actor->velocity_x = saved_velocity_x;
	caster_actor->velocity_y = saved_velocity_y;
	caster_actor->velocity_z = saved_velocity_z;
	caster_actor->acceleration = saved_acceleration;
	caster_actor->vertical_motion_strength = saved_vertical_strength;
	// 35 は、キャンバスを片付けて BattleFx_RunSparkGroups へ引き継ぐ
	if (kind == 35)
	{
		Iwram_ClearWords(BG_CHAR_BLOCK(1), 0x4000);
		Iwram_ClearWords(canvas, 0x4000);
		work->effect->prelude = 0;
		Scheduler_RemoveCallback(Palette_StepFadeTransfer);
		Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
		Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
		Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
		command->variant = 3;
		BattleFx_RunSparkGroups(command, 2);
		return;
	}

	// ジンの 1 フレームの進み：6 フレームで対象の 30 ドット上へ
	target_actor = GetBattleObjectSlotFar(work->effect->actors[0])->object;
	motion = &velocity;
	motion->x = (target_actor->x - position->x) / 6;
	motion->y = (target_actor->y - position->y + 0x1e0000) / 6;
	motion->z = (target_actor->z - position->z) / 6;
	for (i = 0; i != 64; i++)
	{
		particle = &work->particles[i];
		particle->variant = 0;
	}
	// 当たりの粒：対象の高さの半分から飛び散る（variant は寿命）
	if (kind != 14)
	{
		s32 height = Battle_GetObjectTableValueFar(work->effect->actors[0]) / 2;
		for (i = 0; i != 32; i++)
		{
			particle = &work->particles[i];
			particle->x = target_actor->x;
			particle->y = height;
			particle->z = target_actor->z;
			if (kind == 31)
			{
				particle->velocity_x = ((Random16() & 255) - 127) << 12;
				particle->velocity_y = ((Random16() & 255) - 64) << 10;
				particle->velocity_z = ((Random16() & 255) - 127) << 12;
			}
			else
			{
				particle->velocity_x = ((Random16() & 255) - 127) << 12;
				particle->velocity_y = ((Random16() & 255) - 64) << 12;
				particle->velocity_z = ((Random16() & 255) - 127) << 12;
			}
			particle->variant = i / 2 + 32;
		}
	}
	// 11・32 は行動側に重ねる絵と当たりの絵を差し替える
	if (kind == 11)
	{
		LoadResource(&ResourceId_CounterRevealSheet, work, 1, 1);
		LoadResource(&ResourceId_RedCrescentSheetA, Ram_MapCellBuffer, 1, 0);
		REG_BLDALPHA = 0xe10;
	}
	if (kind == 32)
	{
		LoadResource(&ResourceId_BlueBeastSheet, work, 1, 1);
		LoadResource(&ResourceId_RedCrescentSheetB, Ram_MapCellBuffer, 1, 0);
		REG_BLDALPHA = 0xe10;
	}
	// 当たりの絵の粒：gMapCellBuffer + 0x4000 に 64 個
	if (kind != 7 && kind != 13 && kind != 18 && kind != 11 && kind != 32 && kind != 19)
	{
		s32 height = 0;
		if (kind != 12)
			height = 0x140000;
		for (i = 0; i != 64; i++)
		{
			particle = &((struct EffectStep *)(Ram_MapCellBuffer + 0x4000))[i];
			particle->x = target_actor->x;
			particle->y = height;
			particle->z = target_actor->z;
			if (kind == 5 || kind == 23)
			{
				particle->velocity_x = ((Random16() & 255) - 127) << 11;
				particle->velocity_y = (Random16() & 255) << 11;
				particle->velocity_z = ((Random16() & 255) - 127) << 11;
			}
			else if (kind == 25)
			{
				particle->velocity_x = ((Random16() & 255) - 127) << 11;
				particle->velocity_y = (Random16() & 127) << 10;
				particle->velocity_z = ((Random16() & 255) - 127) << 11;
			}
			else
			{
				particle->velocity_x = ((Random16() & 255) - 127) << 10;
				particle->velocity_y = (Random16() & 127) << 10;
				particle->velocity_z = ((Random16() & 255) - 127) << 10;
			}
			particle->variant = 0;
		}
	}
	// 2・3・12・22・28・29 は、BG2 を走査線ごとに揺らす
	kind_from_two = kind - 2;
	if ((kind_from_two >= 0 && kind_from_two <= 1) || kind == 12 || kind == 22 || kind == 29
		|| kind == 28)
	{
		Scheduler_AddOrUpdateCallback(BattleFx_ArmBg2AffineHBlankDma, 0x480);
	}

	// 長さ（フレーム）
	kind_from_four = kind - 4;
	if ((kind_from_four >= 0 && kind_from_four <= 2) || kind == 23 || kind == 30 || kind == 27
		|| kind == 33 || kind == 34 || kind == 100)
	{
		duration = 32;
	}
	else if (kind >= 0 && kind <= 3 || kind == 8 || kind == 9 || kind == 10 || kind == 22
			 || kind == 25 || kind == 29 || kind == 31 || kind == 14)
	{
		duration = 48;
	}
	else if (kind == 21)
	{
		duration = 20;
	}
	else if (kind == 11 || kind == 32 || kind == 20)
	{
		duration = 40;
	}
	else if (kind == 28 || kind == 12)
	{
		duration = 64;
	}
	else
	{
		duration = 80;
	}
	//------------------------------------------------------------------
	// 当てる
	//------------------------------------------------------------------
	frame = 0;
	while (frame != duration)
	{
		// BG2 を揺らす表（走査線ごとの BG2X）
		if (kind != 11)
		{
			if (kind != 32)
			{
				s32 base;
				scanline = work->bg2_x;
				i = 0;
				base = 0x40000;
				phase = frame << 12;
				for (; i != 160; i++)
				{
					s32 offset = (base - (Trig_Sin(phase) << 2)) >> 10; // 1 - sin ドット（8.8）
					*scanline++ = offset;
					phase += 0x800;
				}
			}
		}
		// 最初の 3 フレームは行動側の位置を取り直す（16 ドット下）
		if (frame <= 2)
		{
			EffectPosition_ApplyStepAndYOffset(work->effect->actor, caster_screen);
			caster_screen->x = caster_screen->x / 2;
			caster_screen->y += 16;
		}
		// 行動側に重ねる絵（2 フレームずつ 6 コマ）
		if (kind != 11 && kind != 8 && kind != 32 && kind != 33 && kind != 34)
		{
			if (frame <= 11)
			{
				if (work->effect->side == 0)
				{
					blitters[0](canvas, work->sheet + frame / 2 * 3456, caster_screen->x - 32,
						caster_screen->y - 40, 48, 72);
				}
				else
				{
					blitters[0](canvas, work->sheet + frame / 2 * 3456, caster_screen->x,
						caster_screen->y - 40, 48, 72);
				}
			}
		}
		// 技ごとに、BG のパレットを決まったリソースのパレットへ 1 段ずつ近づける
		switch (kind)
		{
		case 33:
			BattleFx_StepPaletteToResource((s32)&ResourceId_FlameSheetA);
			break;
		case 14:
			BattleFx_StepPaletteToResource((s32)&ResourceId_BlueFlameSheet);
			break;
		case 31:
			BattleFx_StepPaletteToResource((s32)&ResourceId_RuneSheet);
			break;
		case 8:
			BattleFx_StepPaletteToResource((s32)&ResourceId_RockSpireSheet);
			break;
		case 0:
		case 10:
			BattleFx_StepPaletteToResource((s32)&ResourceId_MarsDjinnSheet);
			break;
		case 12:
		case 13:
		case 25:
			BattleFx_StepPaletteToResource((s32)&ResourceId_PinkBurstSheet);
			break;
		case 18:
			BattleFx_StepPaletteToResource((s32)&ResourceId_IceBlockSheet);
			break;
		case 19:
			BattleFx_StepPaletteToResource((s32)&ResourceId_BlastSheet);
			break;
		case 2:
		case 29:
			BattleFx_StepPaletteToResource((s32)&ResourceId_TanPalette);
			break;
		case 1:
		case 28:
			BattleFx_StepPaletteToResource((s32)&ResourceId_CyanPalette);
			break;
		case 3:
		case 20:
		case 22:
			BattleFx_StepPaletteToResource((s32)&ResourceId_EmberStreakSheet);
			break;
		case 9:
			BattleFx_StepPaletteToResource((s32)&ResourceId_LimePalette);
			break;
		case 5:
		case 23:
			BattleFx_StepPaletteToResource((s32)&ResourceId_FlashBurstSheet);
			break;
		}
		// 当たりの玉（4〜15 フレーム）と当たりの粒（4 フレーム目から）
		if (kind != 11 && kind != 8 && kind != 32)
		{
			if (frame >= 4 && frame <= 15)
			{
				draw_pair[1](canvas, work->sheet + (frame - 4) / 2 * 960 + 0x5100,
					target_screen->x / 2 - 8, caster_screen->y - 24, 20, 48);
			}
			Render_ResetTransformState();
			Graphics_PrepareTransferInIwramWork(camera->eye, camera->pos);
			if (frame > 3)
			{
				for (i = 0; i != 128; i++)
				{
					s32				   index = i / 2;
					struct EffectStep *step = &work->particles[index];
					s32				   size = step->variant;
					if (size > 0)
					{
						EffectPosition_ApplyBaseAndYOffset((s32 *)step, &projected);
						size >>= 4;
						size++;
						projected.x /= 2;
						blitters[index & 1](canvas, sprites + ParticleStreams_CellOffsets[size - 1],
							projected.x - size / 2, projected.y - size, size, size * 2);
						EffectStep_AdvanceWithGravity3D(step, 60, -0x1000);
						step->variant--;
					}
				}
			}
		}
		//------------------------------------------------------------------
		// 技ごとの当たり
		//------------------------------------------------------------------
		if (kind == 7 || kind == 13 || kind == 18 || kind == 19)
		{
			// 12 フレーム目から、対象の粒を行動側へ引き寄せる
			if (frame == 50)
			{
				ObjectGroup_UpdateMembers(work->effect->actor, 7, -1, -1, 0);
			}
			if (frame == 79)
			{
				ObjectGroup_UpdateMembers(work->effect->actor, 0, -1, -1, 0);
			}
			if (frame == 12)
			{
				particle = (struct EffectStep *)(Ram_MapCellBuffer + 0x4000);
				for (i = 0; i != 64; i++, particle++)
				{
					particle->x = target_actor->x;
					particle->y = 0x140000;
					particle->z = target_actor->z;
					particle->velocity_x = ((Random16() & 255) - 128) << 10;
					particle->velocity_y = ((Random16() & 255) - 128) << 10;
					particle->velocity_z = ((Random16() & 255) - 128) << 10;
					particle->variant = 0;
				}
			}
			if (frame > 11)
			{
				struct EffectStep	*step = (struct EffectStep *)(Ram_MapCellBuffer + 0x4000);
				struct MotionObject *caster;
				s32					 height;
				caster = GetBattleObjectSlotFar(work->effect->actor)->object;
				height = Battle_GetObjectTableValueFar(work->effect->actor) / 2;
				for (i = 0; i != 32; i++, step++)
				{
					if (step->variant >= 0)
					{
						s32 size = (i & 1) + 6;
						EffectPosition_ApplyBaseAndYOffset((s32 *)step, &projected);
						projected.x >>= 1;
						blitters[0](canvas, sprites + ParticleStreams_CellOffsets[size - 1],
							projected.x - (u32)size / 2, projected.y - size, size, size * 2);
						EffectStep_AdvanceWithGravity3D(step, 62, 0);
						if (frame > i + 22)
						{
							s32 dx = (caster->x - step->x) >> 8;
							s32 dy = (caster->y + height - step->y) >> 8;
							s32 dz = (caster->z - step->z) >> 8;
							step->velocity_x += dx;
							step->velocity_y += dy;
							step->velocity_z += dz;
							if ((dx >= -0xfff && dx <= 0xfff) && (dz >= -0xfff && dz <= 0xfff))
								step->variant = -1;
						}
					}
				}
			}
		}
		else if (kind == 21)
		{
			// 描かない（終わってから BattleFx_RunPaletteRampMode1 へ）
		}
		else if (kind == 6 || kind == 27)
		{
			// 縦長の絵（24x104 を 4 コマ）を 2 本と、対象のまわりにかけら
			if (frame >= 6 && frame <= 19)
			{
				phase = frame;
				i = 0;
				do
				{
					blitters[0](canvas, ((phase / 2) & 3) * 2880 + (Ram_MapCellBuffer + 0xc56),
						target_screen->x / 2 - 8, 0, 24, 104);
					i++;
					phase += 3;
				} while (i != 2);
			}
			if (frame >= 8 && frame <= 23)
			{
				for (i = 0; i != 3; i++)
				{
					s32 image = i & 3;
					s32 angle = Random16() & 0xffff;
					s32 image_x;
					s32 image_y;
					s32 draw_flags;
					image_x = Trig_Sin(angle);
					image_x <<= 3;
					image_x >>= 16;
					image_x += target_screen->x / 2;
					image_x -= BattleFx_GlintCellWidths[image] / 2;
					image_y = Trig_Cos(angle);
					image_y <<= 5;
					image_y >>= 16;
					image_y -= BattleFx_GlintCellHeights[image] / 2;
					Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
					Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
					draw_flags = Random16(); // 反転はランダム
					BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7,
						CastingImpact_GlintDrawFlags[draw_flags & 3] | GOUSEI_CLIP_X
							| GOUSEI_CLIP_Y,
						GOUSEI_HIKAKU);
					((DrawRectangle *)gWorkSlot)[HEAP_SLOT_BLITTER_ALTERNATE](canvas,
						Ram_MapCellBuffer + BattleFx_GlintCellOffsets[image], image_x, image_y + 56,
						BattleFx_GlintCellWidths[image], BattleFx_GlintCellHeights[image]);
					Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
					BattleFx_FetchRectangleBlitters(work->effect->side, blitters);
				}
			}
		}
		else if (kind == 14)
		{
			// 上から降りてくる柱（17 幅の絵を下へ流す）と、対象の位置から出る粒
			s32 fall;
			s32 scroll;
			Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
			Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
			if (frame >= 0 && frame <= 23)
			{
				DrawRectangle *draw;
				s32			   width;
				s32			   tile_height;
				s32			   column_x;
				s32			   column_y;
				s32			   top;
				s32			   origin_x;

				origin_x = target_screen->x / 2;
				fall = frame * 32 - 232;
				scroll = frame * 16 - 48;
				if (fall > 0)
					fall = 0;
				while (scroll > 104)
					scroll -= 104;
				BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7,
					GOUSEI_CLIP_X | GOUSEI_CLIP_Y, GOUSEI_HIKAKU);
				draw = (DrawRectangle *)gWorkSlot;
				draw += HEAP_SLOT_BLITTER_ALTERNATE;
				column_x = origin_x - 8;
				column_y = fall + scroll;
				top = column_y - 104;
				width = 17;
				tile_height = 104;
				DrawImage(canvas, Ram_MapCellBuffer, column_x, top, width, tile_height, draw);
				DrawScrolledImage(canvas, Ram_MapCellBuffer, column_x, column_y, width, tile_height,
					scroll, draw);
				(*draw)(
					canvas, Ram_MapCellBuffer + 0x6e8, origin_x - width, fall + 47, width * 2, 65);
				Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
				if (frame == 8)
				{
					work->shake_frames = frame;
				}
				if (frame > 1)
				{
					s32				   emitted = 0;
					struct EffectStep *step;
					for (i = 0; i != 64; i++)
					{
						if ((step = &work->particles[i])->variant == 0)
						{
							step->x = target_actor->x;
							step->y = 0x140000;
							step->z = target_actor->z;
							step->velocity_x = ((Random16() & 255) - 127) << 12;
							step->velocity_y = ((Random16() & 255) - 64) << 10;
							step->velocity_z = ((Random16() & 255) - 127) << 12;
							step->variant = i / 2 + 32;
							emitted++;
							if (emitted == 4)
								break;
						}
					}
				}
			}
			BattleFx_FetchRectangleBlitters(work->effect->side, blitters);
		}
		else if (kind == 31)
		{
			// 左右対称の 24x48 の絵
			s32 height;
			Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
			Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
			if (frame >= 4 && frame <= 23)
			{
				pair_x = target_screen->x / 2;
				height = 48;
				BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7,
					GOUSEI_CLIP_X | GOUSEI_CLIP_Y, GOUSEI_HIKAKU);
				slot_draw = (DrawRectangle *)gWorkSlot;
				DrawImage(canvas, Ram_MapCellBuffer, pair_x - 24, 48, 24, height,
					slot_draw + HEAP_SLOT_BLITTER_ALTERNATE);
				Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
				BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7,
					GOUSEI_CLIP_X | GOUSEI_CLIP_Y | GOUSEI_FLIP_X, GOUSEI_HIKAKU);
				DrawImage(canvas, Ram_MapCellBuffer, pair_x, 48, 24, height,
					slot_draw + HEAP_SLOT_BLITTER_ALTERNATE);
				Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
			}
			BattleFx_FetchRectangleBlitters(work->effect->side, blitters);
		}
		else if (kind == 30)
		{
			// 3 段の絵（40x32）を 2 フレームずつ 3 コマ、16 フレーム目から薄くする
			s32 image;
			if (frame > 15)
			{
				REG_BLDALPHA = (32 - frame) | 0x1000;
			}
			if (frame > 5)
			{
				strip_x = target_screen->x / 2 - 20;
				image = frame / 2 % 3;
				cell_ofs = image * 2560;
				blitters[0](canvas, Ram_MapCellBuffer + 0xc56 + cell_ofs, strip_x, 16, 40, 32);
				blitters[0](
					canvas, image * 1280 + (Ram_MapCellBuffer + 0x2a56), strip_x, 48, 40, 32);
				blitters[0](canvas, cell_ofs + (Ram_MapCellBuffer + 0x1156), strip_x, 80, 40, 32);
			}
		}
		else if (kind == 5 || kind == 23)
		{
			// 粒ごとに 32x64 の絵を 2 フレームずつ 6 コマ
			struct EffectStep *step = (struct EffectStep *)(Ram_MapCellBuffer + 0x4000);
			for (i = 0; i != 16; i++, step++)
			{
				if (frame >= i / 2 + 4)
				{
					s32 age = step->variant;
					if (age <= 11)
					{
						s32 image = age / 2;
						EffectPosition_ApplyBaseAndYOffset((s32 *)step, &projected);
						projected.x /= 2;
						blitters[0](canvas, Ram_MapCellBuffer + (image << 11), projected.x - 16,
							projected.y - 32, 32, 64);
						EffectStep_AdvanceWithGravity3D(step, 60, 0x1000);
						step->variant++;
					}
				}
			}
		}
		else if (kind == 4)
		{
			// 描かない
		}
		else if (kind == 11)
		{
			// 対象に重ねる大きな絵を 2 フレームずつ 7 枚（上下に揺らす）
			// 4・5 フレーム目はキャンバスを塗りつぶす
			s32 image_y;
			s32 sway;
			Trig_Sin(frame << 9); // 結果は使わない
			sway = Trig_Cos(frame << 9);
			sway <<= 2;
			sway >>= 16;
			image_y = (target_screen->y >> 16) + sway + 16;
			if (frame <= 3)
			{
				blitters[0](canvas, work, CastingImpact_ImageX[work->effect->side * 7],
					image_y + CastingImpact_ImageY[0], 57, 98);
			}
			else
			{
				if (frame <= 7)
				{
					blitters[0](canvas, work, CastingImpact_ImageX[work->effect->side * 7],
						image_y + CastingImpact_ImageY[0], 57, 98);
				}
				blitters[0](canvas, work->sheet + 0x15d2,
					CastingImpact_ImageX[work->effect->side * 7 + 1],
					image_y + CastingImpact_ImageY[1], 99, 69);
				if (frame == 4 || frame == 5)
				{
					Iwram_FillWords(canvas, 0x4000, 0x3f3f3f3f);
				}
				if (frame == 6 || frame == 7)
				{
					blitters[0](canvas, work->sheet + 0x3081,
						CastingImpact_ImageX[work->effect->side * 7 + 2],
						image_y + CastingImpact_ImageY[2], 128, 91);
				}
				if (frame == 8 || frame == 9)
				{
					DrawCroppedImage(canvas, Ram_MapCellBuffer,
						CastingImpact_ImageX[work->effect->side * 7 + 3],
						image_y + CastingImpact_ImageY[3], 128, &blitters[0]);
				}
				if (frame == 10 || frame == 11)
				{
					blitters[0](canvas, Ram_MapCellBuffer + 0x2d80,
						CastingImpact_ImageX[work->effect->side * 7 + 4],
						image_y + CastingImpact_ImageY[4], 128, 59);
				}
				if (frame == 12 || frame == 13)
				{
					blitters[0](canvas, Ram_MapCellBuffer + 0x4b00,
						CastingImpact_ImageX[work->effect->side * 7 + 5],
						image_y + CastingImpact_ImageY[5], 122, 29);
				}
				if (frame == 14 || frame == 15)
				{
					blitters[0](canvas, Ram_MapCellBuffer + 0x58d2,
						CastingImpact_ImageX[work->effect->side * 7 + 6],
						image_y + CastingImpact_ImageY[6], 76, 25);
				}
			}
		}
		else if (kind == 32)
		{
			// 横に流れて止まる絵と、2 フレームずつ続く大きな絵
			// 16 フレーム目から薄くする
			scroll_pos += scroll_speed;
			if (frame > 6)
			{
				scroll_speed = (scroll_speed * 48) / 64;
			}
			REG_BG2X = (scroll_pos >> 16) << 8;
			{
				s32 fade;
				fade = frame - 16;
				if (fade >= 0 && fade <= 15)
				{
					REG_BLDALPHA = (16 - fade) | 0x1000;
				}
			}
			if (frame == 4 || frame == 5)
			{
				Iwram_FillWords(canvas, 0x4000, 0x3f3f3f3f);
			}
			if (frame <= 3)
			{
				if (work->effect->side == 1)
				{
					blitters[0](canvas, work, 0, 24, 80, 104);
				}
				else
				{
					blitters[0](canvas, work, 48, 24, 80, 104);
				}
			}
			else
			{
				if (frame <= 7)
				{
					if (work->effect->side == 1)
					{
						blitters[0](canvas, work, 0, 24, 80, 104);
					}
					else
					{
						blitters[0](canvas, work, 48, 24, 80, 104);
					}
				}
				if (work->effect->side == 1)
				{
					blitters[0](canvas, work->sheet + 0x1e00, 16, 16, 80, 104);
				}
				else
				{
					blitters[0](canvas, work->sheet + 0x1e00, 32, 16, 80, 104);
				}
				if (frame == 6 || frame == 7)
				{
					blitters[0](canvas, work->sheet + 0x3e80, 0, 16, 128, 91);
				}
				if (frame == 8 || frame == 9)
				{
					DrawCroppedImage(canvas, Ram_MapCellBuffer, 0, 16, 128, &blitters[0]);
				}
				if (frame == 10 || frame == 11)
				{
					blitters[0](canvas, Ram_MapCellBuffer + 0x2d80, 0, 16, 128, 59);
				}
				if (frame == 12 || frame == 13)
				{
					blitters[0](canvas, Ram_MapCellBuffer + 0x4b00, 0, 16, 128, 29);
				}
				if (frame == 14 || frame == 15)
				{
					blitters[0](canvas, Ram_MapCellBuffer + 0x5980, 0, 16, 128, 26);
				}
			}
		}
		else if (kind == 20)
		{
			// 1 フレームおきに 12 個、左右へ広がりながら 2 フレームずつ 6 コマ
			for (i = 0; i != 12; i++)
			{
				if (frame >= i + 6 && frame < i + 18)
				{
					s32 image = (frame - i - 6) / 2;
					s32 image_x = target_screen->x / 2 - PuffArc_CellWidths[image] / 2;
					s32 mirrored;
					if (i & 1)
						image_x += (1 + i) / 2 * 3;
					else
						image_x -= (i + 1) / 2 * 3;
					mirrored = 1;
					if (i != 0)
					{
						mirrored = 0;
						if (((i - 1) & 3) > 1)
							mirrored = 1;
					}
					blitters[mirrored](canvas, Ram_MapCellBuffer + PuffArc_CellSourceOffsets[image],
						image_x, PuffArc_CellBiasY[image] + 48, PuffArc_CellWidths[image],
						PuffArc_CellHeights[image]);
				}
			}
		}
		else if (kind == 16)
		{
			// 64 個のかけらが対象のまわりからまっすぐ集まり、
			// 8 個おきに着くたびに対象が反応する
			if (frame == 0)
			{
				for (i = 0; i != 64; i++)
				{
					particle = &((struct EffectStep *)(Ram_MapCellBuffer + 0x4000))[i];
					particle->x = (Random16() & 127) + 32;
					particle->y = 0;
					particle->z = 0;
					particle->velocity_x = Random16() & 0xffff;
					particle->velocity_y = Random16() & 0xffff;
					particle->velocity_z = Random16() & 0xffff;
				}
				((struct EffectStep *)(Ram_MapCellBuffer + 0x4000))[63].y = 159;
			}
			{
				struct EffectStep	  *step = (struct EffectStep *)(Ram_MapCellBuffer + 0x4000);
				struct EffectPosition *origin;
				/* FAKEMATCH: needed to match: a zero offset known only after gcse keeps the origin copy, hoisted after the projection address */
				s32 bias = 0;
				for (i = 0; i != 64; i++, step++)
				{
					if (step->x >= 0 && frame >= i / 2)
					{
						s32 image = i & 3;
						Render_ResetTransformState();
						SceneTransform_ApplyPitch(step->velocity_x);
						SceneTransform_ApplyYaw(step->velocity_y);
						EffectPosition_ApplyBaseAndYOffset((s32 *)step, &projected);
						/* FAKEMATCH: needed to match: see bias above */
						origin = (struct EffectPosition *)((u8 *)target_screen + bias);
						projected.x = projected.x / 2 + origin->x / 2;
						projected.y += origin->y + 32;
						blitters[1](canvas, Ram_MapCellBuffer + CastingImpact_OrbitCells[image],
							projected.x - 4, projected.y - 4, 8, 8);
						step->x -= 6;
						if (step->x < 0 && ((i & 7) == 0 || i == 63))
						{
							AudioCommand_PlayFar(133);
							ObjectGroup_UpdateMembers(work->effect->actors[0], 7, 5, 0, 4);
						}
					}
				}
			}
		}
		else if (kind == 8)
		{
			// 地面から伸びて縮む 32 幅の絵（96 まで）
			s32 height;
			if (frame >= 5 && frame <= 49)
			{
				if (frame > 25)
				{
					height = 196 - (frame << 2);
				}
				else
				{
					height = (frame << 4) - 64;
				}
				if (height > 96)
				{
					height = 96;
				}
				DrawImage(canvas, Ram_MapCellBuffer, 48, 104 - height, 32, height, &blitters[0]);
			}
		}
		else if (kind == 33 || kind == 34)
		{
			// 斜め上から対象へ落ちる 32x64 の絵（6 フレーム）
			s32 offset;
			s32 image_x;
			s32 image_y;
			if (frame <= 5)
			{
				/* FAKEMATCH: needed to match: calculate each center before the shared centered draw 2026-10-11: target_screen->x / 2 in each arm differs 147; with offset set first 129; hoisted out of the arms 146. */
				if (work->effect->side == 0)
				{
					image_x = target_screen->x;
					image_x += (u32)image_x >> 31;
					image_x >>= 1;
					offset = (6 - frame) * 3;
					image_x += offset * 2;
					image_y = target_screen->y - offset * 4 + 24;
				}
				else
				{
					image_x = target_screen->x;
					image_x += (u32)image_x >> 31;
					image_x >>= 1;
					offset = (6 - frame) * 3;
					image_x -= offset * 2;
					image_y = target_screen->y - offset * 4 + 24;
				}
				DrawTallImage(
					canvas, Ram_MapCellBuffer, image_x - 16, image_y - 32, 32, &blitters[1]);
			}
		}
		else if (kind == 12)
		{
			// 16 個の粒に 24x24 の絵を 3 種、48 フレーム目から薄くする
			if (frame > 47)
			{
				REG_BLDALPHA = (64 - frame) | 0x1000;
			}
			{
				struct EffectStep *step = (struct EffectStep *)(Ram_MapCellBuffer + 0x4000);
				for (i = 0; i != 16; i++, step++)
				{
					s32 image = i % 3;
					EffectPosition_ApplyBaseAndYOffset((s32 *)step, &projected);
					projected.x /= 2;
					blitters[i & 1](canvas, Ram_MapCellBuffer + image * 576, projected.x - 12,
						projected.y - 12, 24, 24);
					EffectStep_AdvanceWithGravity3D(step, 60, 1 << ((i & 3) + 11));
					step->variant++;
				}
			}
		}
		else if (kind != 100)
		{
			// そのほか：粒ごとに 24x48 の絵を 4 フレームずつ 6 コマ
			struct EffectStep *step = (struct EffectStep *)(Ram_MapCellBuffer + 0x4000);
			for (i = 0; i != 16; i++, step++)
			{
				if (frame >= i + 4)
				{
					s32 age = step->variant;
					if (age <= 23)
					{
						s32 image = age / 4;
						EffectPosition_ApplyBaseAndYOffset((s32 *)step, &projected);
						projected.x /= 2;
						blitters[i & 1](canvas, Ram_MapCellBuffer + image * 1152, projected.x - 12,
							projected.y - 24, 24, 48);
						if (kind == 25)
							EffectStep_AdvanceWithGravity3D(step, 60, 0x400);
						else
							EffectStep_AdvanceWithGravity3D(step, 60, 0x1000);
						step->variant++;
					}
				}
			}
		}
		// ジン（0〜7）：6 フレームで対象へ飛ぶ
		if (kind <= 7)
		{
			if (frame <= 5)
			{
				EffectPosition_ApplyBaseAndYOffset((s32 *)position, &projected);
				projected.x /= 2;
				djinn_x = projected.x;
				draw_pair[1](
					canvas, Ram_MapCellBuffer + 0x3c56, djinn_x - 10, projected.y - 4, 20, 40);
				position->x += motion->x;
				position->y += motion->y;
				position->z += motion->z;
			}
		}
		// 当たり：6 フレーム目に技ごとの揺れ、6・14 フレーム目に対象が反応する
		if (frame == 3)
		{
			BattleEventRuntime_BeginPhaseFar(-1);
		}
		if (frame == 4)
		{
			AudioCommand_PlayFar(134);
		}
		if (frame == 6)
		{
			if (kind == 4 || kind == 5 || kind == 7 || kind == 13 || kind == 18 || kind == 19
				|| kind == 23 || kind == 34 || kind == 100)
			{
				BattleMotion_ApplyVariantMotionFar(work->effect->actors[0], 4);
				work->shake_frames = 8;
			}
			else if (kind == 20 || kind == 14 || kind == 33)
			{
				BattleMotion_ApplyVariantMotionFar(work->effect->actors[0], 1);
				work->shake_frames = 2;
			}
			else if (kind == 30 || kind == 8)
			{
				BattleMotion_ApplyVariantMotionFar(work->effect->actors[0], 3);
				work->shake_frames = 8;
			}
		}
		if (frame == 6)
		{
			ObjectGroup_UpdateMembers(work->effect->actors[0], 7, 5, 0, 4);
		}
		if (frame == 14)
		{
			ObjectGroup_UpdateMembers(work->effect->actors[0], 7, 5, 0, 4);
		}
		Camera_ApplyShake(8, 8);
		ObjectGroup_TickMemberTimers();
		work->transfer_pending = 1;
		WaitFrames(1);
		frame++;
	}
	if (kind == 21)
	{
		Iwram_ClearWords(BG_CHAR_BLOCK(1), 0x4000);
		Iwram_ClearWords(canvas, 0x4000);
		work->effect->prelude = 0;
		Scheduler_RemoveCallback(Palette_StepFadeTransfer);
		Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
		Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
		Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
		BattleFx_RunPaletteRampMode1(command);
	}
	else
	{
		if (kind == 2 || kind == 3 || kind == 12 || kind == 22 || kind == 28 || kind == 29)
		{
			Scheduler_RemoveCallback(BattleFx_ArmBg2AffineHBlankDma);
		}
		Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
		Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
		Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
		BattleFx_EndCanvasLayer();
	}
}
