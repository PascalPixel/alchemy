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
#include "unit/battle/effect/object13.h"
#include "unit/battle/effect/object14.h"
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
	Iwram_CopyWords((void *)BG_PLTT, palette, 128);

	// 粒：x は角度、z は半径 (1/16 ドット単位)、i フレーム待ってから昇る
	for (i = 0; i != 128; i++)
	{
		PARTICLES[i].y = 0;
		PARTICLES[i].x = Random16() & 0xffff;
		PARTICLES[i].z = (Random16() & 0x1ff) + i * 2;
		PARTICLES[i].variant = -i;
	}
	/* FAKEMATCH: the cancelling pointer updates retain work's allocation
	 * priority without operand asm. Without them, work and the cell index
	 * exchange r9/r11 and two spill slots swap (420 instructions, 50 rows).
	 * The adjacent scheduling asm is unnecessary with these updates. */
	work++;
	work--;
	work->transfer_mode = 2;
	work->transfer_value = 75; // 転送のたびに 3/4 へ減らして残像にする
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
	BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, GOUSEI_CLIP_X | GOUSEI_CLIP_Y, GOUSEI_KASAN);
	draw.upper = gWorkSlot[HEAP_SLOT_BLITTER];
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
			draw.lower = ((DrawRectangle *)Ram_WorkSlot)[HEAP_SLOT_BLITTER_ALTERNATE];
			draw.lower(canvas, work->sheet + n * 1088, 47, ground.y - 64, 17, 64);
			n = frame / 4 % 3;
			glow = work->sheet + n * 1032 + 0x1100;
			draw.lower(canvas, glow, 40, ground.y - 36, 24, 43);
			Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
			BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7,
				GOUSEI_CLIP_X | GOUSEI_CLIP_Y | GOUSEI_FLIP_X, GOUSEI_HIKAKU);
			draw.lower = ((DrawRectangle *)Ram_WorkSlot)[HEAP_SLOT_BLITTER_ALTERNATE];
			n = i % 4;
			draw.lower(canvas, work->sheet + n * 1088, 64, ground.y - 64, 17, 64);
			draw.lower(canvas, glow, 64, ground.y - 36, 24, 43);
			Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
		}
		GetBattleObjectSlotFar(work->effect->actor);
		Render_ResetTransformState();
		Graphics_PrepareTransferInIwramWork(camera->eye, camera->pos);

		// 粒 (最初の 32 個)：1 フレームに 0x400 回り、5 ドット昇り、半径を 4 ドット広げる
		for (i = 0; i != 32; i++)
		{
			struct EffectStep *fleck = &PARTICLES[i];

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
				draw.upper(canvas, sheet + BattleFx6_FlareCells[size - 1], pos.x - size,
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
	/* FAKEMATCH: cancelling frame updates preserve the allocation lifetimes
	 * without the two trailing asm statements. Deleting both swaps the
	 * ground and camera-position pointers at sp+8/sp+12 (420 instructions,
	 * 18 differing rows); one dead increment leaves the same swap. */
	frame ^= 1;
	frame ^= 1;
}

// 割れて飛ぶ 33 片：始まりの位置、大きさ、絵の位置

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
	/* FAKEMATCH: needed to match: never read: the native frame keeps 12 unused bytes at sp+44..55, exactly this struct 2026-10-11: without it only the frame shrinks (differ 4). */
	struct EffectPosition		 position;
	struct BattleEffectWork		*work;
	void						*canvas;
	s32							 frame;
	struct BlitterPair			 draw;
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
	draw.upper = cache[HEAP_SLOT_BLITTER];
	BattleEffect_LoadWork(
		HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, GOUSEI_CLIP_X | GOUSEI_CLIP_Y, GOUSEI_HIKAKU);
	draw.lower = cache[HEAP_SLOT_BLITTER_ALTERNATE];

	// 片：中心 (32, 60) から外へ、距離の 1/4 の速さで飛ぶ
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
	Iwram_CopyWords(Ram_MapCellBuffer, (void *)0x06008000, 0x7800);
	Iwram_FillWords(Ram_MapCellBuffer, 0x7800, 0x01010101);
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
				draw.upper(canvas, (u8 *)work + BattlePieceOffset[i], HI(piece->x), HI(piece->y),
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
					draw.lower(canvas, (u8 *)work + (cell << 11), HI(piece->x) - 16,
						HI(piece->y) - 32, 32, 64);
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

// 光の筋 1 本。PARTICLES と同じ 28 バイトの置き場を別の意味で読む
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

void BattleFx_RunGatheringBurst(struct BattleEffectArgument *effect)
{
	struct EffectPosition	 ground;
	struct BattleEffectWork *work;
	void					*canvas;
	struct BlitterPair		 draw;
	u8						*sheet;
	void					*palette;
	s32						 frame;
	s32						 i;

	work = Runtime_AllocateHeapBlock(HEAP_SLOT_BATTLE_EFFECT, sizeof *work);
	canvas = Runtime_AllocateHeapBlock(HEAP_SLOT_BATTLE_CANVAS, 0x4000);
	sheet = Runtime_AllocateHeapBlock(HEAP_SLOT_BATTLE_SHEET, 0x60e);
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
		Iwram_CopyWords((void *)0x05000000, palette, 128);
	}

	for (i = 0; i != 128; i++)
	{
		PARTICLES[i].y = 0x800000;
		PARTICLES[i].x = Random16() & 0xffff;
		PARTICLES[i].z = (Random16() & 0x1ff) + 0x400;
		PARTICLES[i].variant = -i;
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
	draw.upper = (DrawRectangle)((union HeapState *)gWorkSlot)->slots[HEAP_SLOT_BLITTER];
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
	struct Vector3			 point;
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
			point.x = streak->head;
			EffectPosition_ApplyBaseAndYOffset((s32 *)&point, &head);
			head.x += 64;
			head.y += 80;
			point.x = streak->tail;
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
		Iwram_ClearWords((void *)0x06004000, 0x4000);
	}
	Scheduler_RemoveCallback(Palette_StepFadeTransfer);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BATTLE_CANVAS);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BATTLE_EFFECT);
}
