/* Pin-free BattleFx_RunOrbitingSparks: EN 080dc454.
 * All six editions: 265 native, 267 built, 212 common, differ108.
 * Whole-TU current pinned baseline gives differ0. Plain removal gives
 * 269 built, common208, differ118; loop.c moves record-address pseudo133
 * out of the member loop, and the native 88-byte frame grows to92.
 * Separate window-conditioned null/restore updates improve118 to108
 * and reduce built269 to267. The frame remains92, with spill/allocation
 * and loop-layout differences still present. Window is unchanged between
 * the two tests: either rec stays record, or is restored before access.
 * Seven bounded ordinary-C forms: direct array stores, comma argument,
 * complementary pointer assignment, first-store assignment and scoped
 * point array retain118; inline position-to-record helper gives115;
 * separate save/null/restore is the preserved108 form. Prior generic
 * operand-barrier and post-call-lifetime failures remain recorded below.
 * This is a measured near miss, not a live-source adoption. */
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "canvas.h"
#include "mem.h"
#include "resource.h"
#include "btldisp.h"
#include "types.h"
#include "iwram_call.h"
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
#include "heap_state.h"
#include "call.h"
#include "menu_list.h"
#include "animspr.h"
#include "scene.h"
#include "dma.h"
#include "project.h"
#include "b5_context.h"
#include "map_scroll.h"

void BattleFx_PrepareCanvasEffect(
	struct BattleEffectArgument *effect, s32 kind, s32 side, s32 anchor, s32 *out_x, s32 *out_y);

#define SceneTransform_ApplyPosition ((void (*)(s32 *))SceneTransform_ApplyPosition)

void AudioCommand_PlayFar(s32 cue);

void BattleFx_RunOrbitingSparks(void)
{
	/* FAKEMATCH: needed to match: relative heap-cell loads preserve load and literal order */
	void				   **heap_cache;
	void				   **cursor;
	struct BattleEffectWork *work;
	void					*canvas;
	s32						 record[3];
	struct EffectPosition	 center;
	struct EffectPosition	 pos;
	s32						 frame;
	/* FAKEMATCH: needed to match: only draw[0] is used: the second word fills four bytes of the frame */
	DrawRectangleFn		 draw[2];
	s32					 member;
	struct BattleCamera *camera;
	u8					*palette;
	s32					 x_offset;
	s32					 i;

	heap_cache = &((union HeapState *)gWorkSlot)->slots[HEAP_SLOT_BATTLE_EFFECT];
	cursor = heap_cache;
	work = *cursor++;
	canvas = *cursor;
	camera = ((struct BattleCamera **)gWorkSlot)[HEAP_SLOT_CAMERA];
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
	draw[0] = (DrawRectangleFn)heap_cache[7];
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
	if (work->effect->side == 0)
		x_offset = 0;
	else
		x_offset = -112;
	/* FAKEMATCH: needed to match: a one-pass loop is a sched2 barrier, so the seed counter is set before its pointer */
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
				/* FAKEMATCH: the split pointer restoration retains a CSE boundary;
				 * pin-free residual improves 118 to 108, still two extra instructions. */
				s32 *rec;

				Render_ResetTransformState();
				Graphics_PrepareTransferInIwramWork(camera->eye, camera->pos);
				rec = record;
				if (window)
					rec = 0;
				if (window)
					rec = record;
				rec[0] = member_object->x;
				rec[1] = member_object->y;
				rec[2] = member_object->z;
				EffectPosition_ApplyBaseAndYOffset(rec, &center);
				/* FAKEMATCH: needed to match: sched2 barrier, so this store precedes the spark counter as in the reference */
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


