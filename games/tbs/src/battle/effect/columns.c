#include "battle.h"
#include "keystate.h"
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

void BattleFx_RunPaletteRampMode0(struct BattleEffectArgument *effect)
{
	BattleFx_RunPaletteRamp(effect, 0);
}

void BattleFx_RunPaletteRampMode1(struct BattleEffectArgument *effect)
{
	BattleFx_RunPaletteRamp(effect, 1);
}

void BattleFx_RunPaletteRampMode2(struct BattleEffectArgument *effect)
{
	BattleFx_RunPaletteRamp(effect, 2);
}

void BattleFx_RunPaletteRampMode3(struct BattleEffectArgument *effect)
{
	BattleFx_RunPaletteRamp(effect, 3);
}

#define RAMP_IMAGE 0x2580
#define RAMP_SIZE 696

void BattleFx_RunPaletteRamp(struct BattleEffectArgument *effect, s32 mode)
{
	void				   **cursor;
	struct BattleEffectWork *work;
	void					*canvas;
	s32						 frame;
	s32						 j;
	s32						 x_offset;
	DrawRectangle			*draw;
	struct BattleCamera		*camera;
	s32						 origin[3];
	struct EffectPosition	 screen;
	s32						 point[3];
	DrawRectangle			 callbacks[2];
	s32						 i;
	s32						 palette;

	cursor = (void **)gBattleFxWork;
	work = *cursor++;
	canvas = *cursor;
	work->effect = effect;
	BattleFx_BeginCanvasLayer(0);
	REG_BG2PA = 0x100;
	if (mode == 0)
		Resource_LoadAndDecompress((s32)&ResourceId_DaggerSheet, work->sheet, 1, 1);
	else
		Resource_LoadAndDecompress((s32)&ResourceId_ShieldSheet, work->sheet, 1, 1);
	if (mode == 0)
		palette = (s32)&ResourceId_PinkBurstSheet;
	else if (mode == 1)
		palette = (s32)&ResourceId_GlowOrbSheet;
	else
		palette = (s32)&ResourceId_PinkBurstSheet;
	Iwram_CopyWords((void *)BG_PLTT, Resource_GetTableEntry(palette), 128);
	Resource_LoadAndDecompress((s32)&ResourceId_PaletteRampImage, work->sheet + RAMP_IMAGE, 0, 0);

	for (i = 1; i != 8; i++)
	{
		for (j = 0; j != RAMP_SIZE; j++)
		{
			s32 level = work->sheet[RAMP_IMAGE + j];

			if (level > 64 - i * 7)
				level = 64 - i * 7;
			if (level < 0)
				level = 0;
			work->sheet[RAMP_IMAGE + i * RAMP_SIZE + j] = level;
		}
	}

	if (work->effect->side == 1)
	{
		REG_BG2X = -0x7000;
		x_offset = -112;
	}
	else
	{
		REG_BG2X = 0;
		x_offset = 0;
	}

	for (i = 0; i != 512; i++)
	{
		struct EffectStep *shard = &((struct EffectStep *)Ram_MapCellBuffer)[i];
		s32				   radius = 192;
		s32				   angle;

		angle = Random16() & 0xffff;
		shard->x = 0;
		if (mode == 0)
		{
			shard->y = ((i & 31) / 4 * 6 - 10) << 16;
			shard->z = (i % 4 * 2 - 2) << 16;
		}
		else
		{
			shard->y = ((i & 31) / 4 * 6 - 10) << 16;
			shard->z = (i % 4 * 8 - 16) << 16;
		}
		if (work->effect->side == 1)
			shard->velocity_x = 0x20000;
		else
			shard->velocity_x = -0x20000;
		shard->velocity_y = ((Trig_Cos(angle) * radius) >> 6) + 0x10000;
		shard->velocity_z = (Trig_Sin(angle) * radius) >> 6;
		shard->variant = Random16() & 0xff;
	}

	BattleFx_FetchRectangleBlitters(work->effect->side, draw = callbacks);
	work->transfer_mode = 2;
	work->transfer_value = 50;
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);

	for (frame = 0; frame != work->effect->count * 4 + 64; frame++)
	{
		camera = gCameraWork;
		if (frame == 72)
			BattleEventRuntime_BeginPhaseFar(0);
		for (j = 0; j != work->effect->count; j++)
		{
			struct MotionObject *object;
			s32					 time;
			s32					 x;
			s32					 y;

			time = frame - j * 4;
			object = GetBattleObjectSlotFar(work->effect->actors[j])->object;
			if (time > 0)
			{
				Render_ResetTransformState();
				Graphics_PrepareTransferInIwramWork(camera->eye, camera->pos);
				point[0] = object->x;
				point[1] = 160 << 13;
				point[2] = object->z;
				Render_ResetTransformState();
				Graphics_PrepareTransferInIwramWork(camera->eye, camera->pos);
				SceneTransform_ApplyPosition(point);
				origin[0] = 0;
				origin[1] = 0;
				origin[2] = 0;
				EffectPosition_ApplyBaseAndYOffset(origin, &screen);
				x = screen.x + x_offset;
				y = screen.y;
				if (mode == 0)
				{
					if (time <= 26)
						callbacks[0](
							canvas, work->sheet + time / 4 % 7 * 960, x - 12, y - 20, 24, 40);
				}
				else
				{
					if (time <= 23)
						draw[1](canvas, work->sheet + time / 4 % 6 * 1600, x - 20, y - 20, 40, 40);
				}
				if (time == 24)
					AudioCommand_PlayFar(143);
				if (time >= 24 && time <= 60)
				{
					s32 level;

					level = 0;
					if (time > 28)
					{
						level = (time - 24) / 4;
						if (level > 7)
							level = 7;
					}
					for (i = 0; i != 24; i++)
					{
						struct EffectStep *shard =
							&((struct EffectStep *)Ram_MapCellBuffer)[j * 32 + i];
						s32 cell;

						cell = i % 4 * 3 + (shard->variant + time) / 8 % 3;
						EffectPosition_ApplyBaseAndYOffset((s32 *)shard, &screen);
						x = screen.x + x_offset;
						y = screen.y;
						callbacks[0](canvas,
							work->sheet + (level * RAMP_SIZE + PaletteRamp_ShardCells[cell])
								+ RAMP_IMAGE,
							x, y, PaletteRamp_ShardWidths[cell], PaletteRamp_ShardHeights[cell]);
						EffectStep_AdvanceWithGravity3D(shard, 60, 0);
					}
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

//--------------------------------------------------------------------------

// puffarc.c

// バトル効果：弧にならんで順に立ちのぼる炎

//--------------------------------------------------------------------------

void AudioCommand_PlayFar(s32);

typedef struct Puff // work->particles と同じ置き場
{
	s32 x; // キャンバスのドット
	s32 y;
	s32 unk08;
	s32 unk0C;
	s32 unk10;
	s32 unk14;
	s32 tick; // 立ちのぼってからのフレーム（負なら待ち）
} Puff;

typedef struct
{
	s32 reserved_00[3];
	s32 current_value;
	s32 index_radius;
	s32 entry_limit;
	s32 reserved_18[3];
	s16 values[1];
} BattleEffectIndexWindow;

#define HONOO_KAZU 9 // 炎の数

//--------------------------------------------------------------------------
// 9 つの炎が半円の弧の上に 4 フレームおきに立ちのぼる（80 フレーム）
// variant 1 なら 16 ドット上にもうひとつ、variant 2 なら 32 ドット上にも
// 対象は 16 フレーム目から 8 フレームおきに当たる
//--------------------------------------------------------------------------
void BattleFx_RunPuffArc(struct BattleEffectArgument *efx)
{
	u32						*heap;
	u32						*cursor;
	struct BattleEffectWork *work;
	void					*canvas;
	s32						 frame;
	DrawRectangle			 draw;
	s32						 i;
	s32						 cell;
	u32						 w;
	s32						 sign;
	s32						 org;
	s32						 angle;
	s32						 tick;
	Puff					*puff;
	Puff					*p;

	// ワーク、キャンバス、BG1・BG2 を半透明に（14:16）
	heap = (u32 *)&gWorkSlot[HEAP_SLOT_BATTLE_EFFECT];
	cursor = heap;
	work = (struct BattleEffectWork *)*cursor++;
	canvas = (void *)*cursor;
	work->effect = efx;
	BattleFx_BeginCanvasLayer(0);
	REG_BLDCNT = 0x3f46;
	REG_BLDALPHA = 0x100e;
	Resource_LoadAndDecompress((s32)&ResourceId_EmberStreakSheet, work->sheet, 1, 1);
	BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, GOUSEI_CLIP_X | GOUSEI_CLIP_Y, GOUSEI_KASAN);
	draw = (DrawRectangle)heap[HEAP_SLOT_BLITTER - HEAP_SLOT_BATTLE_EFFECT];

	// 弧：-90 度から 22.5 度ずつ、横の半径 32、縦の半径 16
	// 対象が 128 番以上なら左から、ほかは右から並べる
	if (work->effect->actors[0] > 127)
	{
		org = 0;
		sign = 1;
	}
	else
	{
		org = 64;
		sign = -1;
	}
	angle = -0x4000;
	i = 0;
	tick = 0;
	puff = (Puff *)work->particles;
	do
	{
		puff->x = ((sign * ((Trig_Sin(angle) << 5) >> 16)) + org) + 20;
		puff->y = ((Trig_Cos(angle) << 4) >> 16) + 40;
		puff->tick = tick;
		angle += 0x1000;
		tick -= 4;
		i += 1;
		puff++;
	} while (i != HONOO_KAZU);

	work->transfer_mode = 2;
	if (work->effect->variant == 2)
	{
		work->transfer_value = 75;
	}
	else
	{
		work->transfer_value = 50;
	}
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
	AudioCommand_PlayFar(136);
	frame = 0;
	do
	{
		if (frame == 24)
		{
			BattleEventRuntime_BeginPhaseFar(133);
		}

		// 炎の絵は 4 フレームごとに次へ、6 枚で消える
		i = 0;
		p = (Puff *)work->particles;
		do
		{
			if ((p->tick >= 0) && (p->tick <= 23))
			{
				cell = p->tick / 4;
				draw(canvas, work->sheet + PuffArc_CellSourceOffsets[cell],
					p->x - ((w = PuffArc_CellWidths[cell]) >> 1), p->y + PuffArc_CellBiasY[cell], w,
					PuffArc_CellHeights[cell]);
				if (work->effect->variant != 0)
				{
					draw(canvas, work->sheet + PuffArc_CellSourceOffsets[cell],
						p->x - ((w = PuffArc_CellWidths[cell]) >> 1),
						(p->y + PuffArc_CellBiasY[cell]) - 16, w, PuffArc_CellHeights[cell]);
				}
				if (work->effect->variant == 2)
				{
					draw(canvas, work->sheet + PuffArc_CellSourceOffsets[cell],
						p->x - ((w = PuffArc_CellWidths[cell]) >> 1),
						(p->y + PuffArc_CellBiasY[cell]) - 32, w, PuffArc_CellHeights[cell]);
				}
			}
			i += 1;
			p->tick += 1;
			p++;
		} while (i != HONOO_KAZU);

		i = 0;
		while (i != work->effect->count)
		{
			if (frame == (i * 8) + 16)
			{
				ObjectGroup_UpdateMembers(work->effect->actors[i], 10, 5, i, 12);
			}
			i += 1;
		}
		ObjectGroup_TickMemberTimers();
		work->transfer_pending = 1;
		WaitFrames(1);
		frame += 1;
	} while (frame != 80);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
	Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
	BattleFx_EndCanvasLayer();
}

//--------------------------------------------------------------------------
// values のうち current_value のある位置を中心に、前後 index_radius の
// 番号を out に並べ、その数を返す。0〜entry_limit - 1 の番号だけを入れる
//--------------------------------------------------------------------------
s32 BattleFx_BuildCenteredIndexList(BattleEffectIndexWindow *win, s16 *out)
{
	s32 len = win->index_radius * 2 + 1;
	s32 n = 0;
	s32 center = 0;
	s32 i;

	for (i = 0; i != len; i++)
	{
		if (win->current_value == win->values[i])
		{
			center = i;
			break;
		}
	}
	for (i = 0; i != len; i++)
	{
		s32 no = center + i - win->index_radius;

		if (no >= 0 && no < win->entry_limit)
		{
			out[n++] = no;
		}
	}
	return n;
}

//--------------------------------------------------------------------------

// rockwall.c

// バトル演出：岩の柱を 3 本せり上げ、対象を打ち上げる

//--------------------------------------------------------------------------

#define Camera_ApplyShake ((void (*)(s32, s32))Camera_ApplyShake)

#define CLIP (GOUSEI_CLIP_X | GOUSEI_CLIP_Y) // 上下左右ではみ出しを切る

// RockWall_Timings[variant] の並び
enum
{
	TIMING_FRAMES,		// 全体のフレーム数
	TIMING_RISE_FRAMES, // 柱が出ているフレーム数
	TIMING_SHAKE		// 画面の揺れ幅
};

struct RockWallHeights
{
	s32 column[4]; // 柱ごとの高さ (ドット)
};

#define HASHIRA_W 40   // 柱の幅 (ドット)
#define IWA_OFS 0x1e00 // sheet の中の岩の絵 (先頭 40x16 は背景の帯)

// 横 32 キャラの 8bpp キャラ並びの中で、(x, y) のドットの位置
#define BG_DOT(x, y) (((x) & 7) + (((x) / 8) << 6) + (((y) & 7) << 3) + (((y) / 8) << 11))

//--------------------------------------------------------------------------
// 岩の柱を左から 4 フレームおきに 3 本せり上げて沈め、上に乗った対象を
// 打ち上げる。柱の頭には背景から切り出した帯をのせる
// effect->variant で RockWall_Timings の行を選ぶ
//--------------------------------------------------------------------------
void BattleFx_RunRockWall(struct BattleEffectArgument *effect)
{
	void				   **slots;
	void				   **cursor;
	struct BattleEffectWork *work;
	void					*canvas;
	s32						 frame;
	s32						 column;
	DrawRectangle			 draw;
	struct BattleCamera		*camera;
	s32						 x_offset;
	struct RockWallHeights	 heights;
	s32						 point[3];
	struct EffectPosition	 screen;
	s32						 member;
	s32						 row;
	s32						 col;
	s32						 y;

	slots = &gWorkSlot[HEAP_SLOT_BATTLE_EFFECT];
	cursor = slots;
	work = *cursor++;
	canvas = *cursor;
	camera = *(struct BattleCamera **)((u8 *)gWorkSlot + HEAP_SLOT_CAMERA * 4);
	work->effect = effect;
	BattleFx_BeginCanvasLayer(1);
	REG_BG2PA = 0x100; // 横も等倍
	REG_BLDCNT = 0;	   // 半透明にしない
	Resource_LoadAndDecompress((s32)&ResourceId_RockWallSheet, work->sheet + IWA_OFS, 1, 1);

	// 背景のキャラ (VRAM 0x06008000 から 0x8000 バイト) を写し、
	// x 32〜71・y 96〜111 の帯を sheet の先頭へ切り出す
	/* FAKEMATCH: needed to match: forwarded through Call3, the copy loads the buffer address straight into r0, so the loop below loads its own after the call */
	Iwram_CopyWords((s32)Ram_MapCellBuffer, 0x06008000, 0x8000);
	for (row = 0; row != 16; row++)
	{
		y = row + 96;
		for (col = 0; col != HASHIRA_W; col++)
		{
			s32 pixel = BG_DOT(col + 32, y);

			work->sheet[row * HASHIRA_W + col] = Ram_MapCellBuffer[pixel];
		}
	}

	// 向こう側ならキャンバスを右へ 112 ドットずらし、対象の座標もそれに合わせる
	if (work->effect->side == 1)
	{
		REG_BG2X = -(112 << 8);
		x_offset = -112;
	}
	else
	{
		x_offset = 0;
	}
	BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, CLIP, GOUSEI_NURI);
	draw = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER];
	for (member = 0; member != work->effect->count; member++)
		work->particles[member].variant = 0; // 着地したら 1
	work->transfer_mode = 1;				 // 転送のたびにキャンバスを 0 で消す
	work->transfer_value = 0;
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
	heights = RockWall_Heights;
	work->shake_frames = 128;
	AudioCommand_PlayFar(141);

	for (frame = 0; frame != RockWall_Timings[work->effect->variant][TIMING_FRAMES]; frame++)
	{
		Render_ResetTransformState();
		Graphics_PrepareTransferInIwramWork(camera->eye, camera->pos);
		if (frame == RockWall_Timings[work->effect->variant][TIMING_FRAMES] - 16)
			BattleEventRuntime_BeginPhaseFar(133); // 終わりの 16 フレーム前に結果へ進む

		// 柱は 16・20・24 フレーム目から出る
		for (column = 0; column != 3; column++)
		{
			s32 start = column * 4 + 16;

			if ((frame & 31) + 32 == start) // 左辺は 32 以上なので成り立たない
				heights.column[column] += 32;
			if (frame >= start
				&& frame < start + RockWall_Timings[work->effect->variant][TIMING_RISE_FRAMES])
			{
				s32 height;
				s32 top;

				// 高さは sin で上がって下がる (1 フレームに 1/64 周)
				height = (heights.column[column] * Trig_Sin((frame - start) << 10)) >> 16;
				if (height < 0)
					height = -height;
				top = 112 - height;
				draw(canvas, work->sheet + IWA_OFS, column * HASHIRA_W + 8, top, HASHIRA_W, height);
				draw(canvas, work, column * HASHIRA_W + 8, 96 - height, HASHIRA_W, 16);

				// 柱の上にいる対象を打ち上げる
				for (member = 0; member != work->effect->count; member++)
				{
					struct MotionObject *object;

					object = GetBattleObjectSlotFar(work->effect->actors[member])->object;
					point[0] = object->x;
					point[1] = object->y;
					point[2] = object->z;
					EffectPosition_ApplyBaseAndYOffset(point, &screen);
					screen.x += x_offset;
					if (screen.x >= column * HASHIRA_W + 8
						&& screen.x <= column * HASHIRA_W + 8 + HASHIRA_W && screen.y >= top)
					{
						object->velocity_y = 12 << 16; // 上へ 12 ドット/フレーム
						object->vertical_motion_strength = 0xab85;
					}
					if (object->y < 0)
						ObjectGroup_UpdateMembers(work->effect->actors[member], 0, 5, -1, 0);
				}
			}
		}

		// 落ちてきて着地した対象を、パレット 7 にして 5 フレーム後に戻す
		for (member = 0; member != work->effect->count; member++)
		{
			struct MotionObject *object;
			struct EffectStep	*state;

			object = GetBattleObjectSlotFar(work->effect->actors[member])->object;
			state = &work->particles[member];
			if (state->variant == 0 && object->y <= 0 && object->velocity_y < 0)
			{
				state->variant = 1;
				ObjectGroup_UpdateMembers(work->effect->actors[member], 7, 5, member, 5);
			}
		}
		Camera_ApplyShake(RockWall_Timings[work->effect->variant][TIMING_SHAKE],
			RockWall_Timings[work->effect->variant][TIMING_SHAKE]);
		ObjectGroup_TickMemberTimers();
		work->transfer_pending = 1;
		WaitFrames(1);
	}

	Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
	BattleFx_EndCanvasLayer();
}

//--------------------------------------------------------------------------

// homing.c

// バトル演出：使い手から飛び出した火の粉が対象を追って落ちる

//--------------------------------------------------------------------------

#define EMBER(n) (&((struct EffectStep *)Ram_MapCellBuffer)[n])

//--------------------------------------------------------------------------
// 火の粉を 1 フレームに 2 個ずつ飛ばし、出てから 48 フレーム後に対象へ向かわせる
// 地面に落ちた火の粉は着弾の絵を 12 フレーム出す
// effect : variant で火の粉の数 (HomingEmbers_Counts) を選ぶ
//--------------------------------------------------------------------------
void BattleFx_RunHomingEmbers(struct BattleEffectArgument *effect)
{
	void				   **heap_cache;
	void				   **cursor;
	struct BattleEffectWork *work;
	void					*canvas;
	s32						 frame;
	struct BattleCamera		*camera;
	u8						*sheet;
	s32						 top;
	struct EffectPosition	 screen;
	DrawRectangle			 draw[2];
	struct MotionObject		*user;
	s32						 i;

	heap_cache = &((union HeapState *)gWorkSlot)->slots[HEAP_SLOT_BATTLE_EFFECT];
	cursor = heap_cache;
	work = *cursor++;
	canvas = *cursor;
	camera = *(struct BattleCamera **)((u8 *)gWorkSlot + HEAP_SLOT_CAMERA * 4);
	sheet = heap_cache[2];
	work->effect = effect;
	if (effect->side == 1)
		BattleFx_BeginCanvasLayer(1);
	else
		BattleFx_BeginCanvasLayer(0);
	Resource_LoadAndDecompress((s32)&ResourceId_EmberStreakSheet, work->sheet, 1, 1);
	Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);
	BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, GOUSEI_CLIP_X | GOUSEI_CLIP_Y, GOUSEI_KASAN);
	draw[0] = (DrawRectangle)((union HeapState *)gWorkSlot)->slots[HEAP_SLOT_BLITTER];
	BattleEffect_LoadWork(
		HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, GOUSEI_CLIP_X | GOUSEI_CLIP_Y, GOUSEI_HIKAKU);
	draw[1] = (DrawRectangle)((union HeapState *)gWorkSlot)->slots[HEAP_SLOT_BLITTER_ALTERNATE];
	REG_BLDALPHA = 0x1010;

	// 使い手の体の上端から、水平は乱数の向きと速さ、縦は -16〜111 (1/64 ドット単位) で飛び出す
	user = GetBattleObjectSlotFar(work->effect->actor)->object;
	top = user->y + Battle_GetObjectTableValueFar(work->effect->actor);
	for (i = 0; i != 64; i++)
	{
		struct EffectStep *ember = EMBER(i);
		s32				   angle;
		s32				   speed;

		angle = Random16();
		speed = (Random16() & 127) + 127;
		ember->velocity_x = (Trig_Sin(angle) * speed) >> 6;
		ember->velocity_y = (s32)(((Random16() & 127) - 16) << 16) >> 6;
		ember->velocity_z = (Trig_Cos(angle) * speed) >> 6;
		ember->x = user->x;
		ember->y = top;
		ember->z = user->z;
		ember->variant = -1; // -1 は飛行中、0 以上は着弾の絵の経過フレーム
	}

	work->camera_delta = 0;
	work->camera_phase = 0;
	Scheduler_AddOrUpdateCallback(Camera_ApplyPhasedDelta, 0x480);
	work->transfer_mode = 2;
	work->transfer_value = 75; // 転送のたびに 3/4 へ減らして残像にする
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);

	for (frame = 0; frame != (HomingEmbers_Counts[work->effect->variant] >> 1) + 132; frame++)
	{
		if (frame > 16 && frame < 80)
			work->camera_delta = 0x100;
		else
			work->camera_delta = 0;
		if (frame == (HomingEmbers_Counts[work->effect->variant] >> 1) + 108)
			BattleEventRuntime_BeginPhaseFar(133);
		BattlePres_SetupTransitionSceneFar(0, 0, 0, 100);
		Render_ResetTransformState();
		Graphics_PrepareTransferInIwramWork(camera->eye, camera->pos);

		//------------------------------------------------------------------
		// 飛んでいる火の粉
		//------------------------------------------------------------------
		for (i = 0; i != HomingEmbers_Counts[work->effect->variant]; i++)
		{
			struct EffectStep *ember = EMBER(i);
			s32				   start;

			start = i / 2;
			if (frame > start && ember->variant == -1)
			{
				s32 size;

				// 奥行き 160〜799 を大きさ 10〜1 に。最初の 48 フレームは比較、後は加算合成
				EffectPosition_ApplyBaseAndYOffset((s32 *)ember, &screen);
				screen.x >>= 1;
				if (screen.depth < 160)
					screen.depth = 160;
				if (screen.depth > 799)
					screen.depth = 799;
				size = 10 - (screen.depth - 160) / 64;
				draw[frame < start + 48 ? 1 : 0](canvas,
					sheet + ParticleStreams_CellOffsets[size - 1], screen.x - size / 2,
					screen.y - size, size, size * 2);
				ember->x += ember->velocity_x;
				ember->y += ember->velocity_y;
				ember->z += ember->velocity_z;
			}

			// 対象へ距離の 1/512 ずつ加速し、出てから 85 フレームまでは速度を 60/64 に抑える
			if (frame > start + 48 && ember->variant == -1)
			{
				struct MotionObject *target;
				s32					 vx;
				s32					 vy;
				s32					 vz;

				target =
					GetBattleObjectSlotFar(work->effect->actors[i % work->effect->count])->object;
				vx = ember->velocity_x += (target->x - ember->x) >> 9;
				vy = ember->velocity_y += (target->y - ember->y) >> 9;
				vz = ember->velocity_z += (target->z - ember->z) >> 9;
				if (frame < start + 85)
				{
					ember->velocity_x = vx * 60 / 64;
					ember->velocity_y = vy * 60 / 64;
					ember->velocity_z = vz * 60 / 64;
				}

				// 地面より下で着弾：画面位置を残して着弾の絵に切り替え、対象のパレットを
				// 4 フレーム 10 番に替えてアニメ 5 にし、画面を 2 フレーム揺らす
				if (ember->y < 0)
				{
					ember->variant = 0;
					ember->x = screen.x;
					ember->y = screen.y;
					AudioCommand_PlayFar(136);
					ObjectGroup_UpdateMembers(work->effect->actors[i % work->effect->count], 10, 5,
						i % work->effect->count, 4);
					work->shake_frames = 2;
				}
			}
		}

		//------------------------------------------------------------------
		// 着弾の絵 (2 フレームごとに進め、6 枚で終わる)
		//------------------------------------------------------------------
		for (i = 0; i != HomingEmbers_Counts[work->effect->variant]; i++)
		{
			struct EffectStep *ember = EMBER(i);

			if (ember->variant >= 0 && ember->variant < 12)
			{
				s32 cel;
				u32 width;

				cel = ember->variant / 2;
				draw[1](canvas, work->sheet + HomingEmbers_FlareCells[cel],
					ember->x - ((width = HomingEmbers_FlareWidths[cel]) >> 1),
					ember->y + HomingEmbers_FlareBiasY[cel] - 56, width,
					HomingEmbers_FlareHeights[cel]);
				ember->variant++;
			}
		}

		if (work->camera_phase == 0)
			work->camera_phase = 1;
		Camera_ApplyShake(8, 8);
		ObjectGroup_TickMemberTimers();
		work->transfer_pending = 1;
		WaitFrames(1);
	}

	Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
	Scheduler_RemoveCallback(Camera_ApplyPhasedDelta);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
	BattleFx_EndCanvasLayer();
}

//--------------------------------------------------------------------------

// motes.c

// 戦闘エフェクト：渦を巻いて昇る粒、立ち並ぶ柱、はじけて散る破片と煙

//--------------------------------------------------------------------------

#define HI(v) (((s16 *)&(v))[1]) // 16.16 固定小数の整数部

#define CHIPS ((struct EffectStep *)Ram_MapCellBuffer)

// RisingMotes_Timings の 1 組 (variant ごと)
enum
{
	RISE_MOTES,	  // 昇る粒の数
	RISE_COLUMNS, // 柱の数
	RISE_FRAME	  // はじけるフレーム
};

//--------------------------------------------------------------------------
// 粒が渦を巻いて昇り、柱が 3 フレームおきに立ち並ぶ。
// RISE_FRAME で粒は広がって落ち、破片が跳ね、煙が上がる
// effect : variant が粒と柱の数、はじけるフレームを決める
//--------------------------------------------------------------------------
void BattleFx_RunRisingMotes(struct BattleEffectArgument *effect)
{
	void				   **heap;
	void				   **cursor;
	struct BattleEffectWork *work;
	void					*canvas;
	s32						 frame;
	u8						*dots;
	DrawRectangle			*draw;
	s32						 count;
	s32						 ymin;
	s32						 ymax;
	s32						 xyz[3];
	struct EffectPosition	 screen;
	s32						 point[3];
	DrawRectangle			 pair[2];
	struct EffectStep		*mote;
	s32						 i;

	heap = &gWorkSlot[HEAP_SLOT_BATTLE_EFFECT];
	cursor = heap;
	work = *cursor++;
	canvas = *cursor;
	dots = heap[HEAP_SLOT_BATTLE_SHEET - HEAP_SLOT_BATTLE_EFFECT];
	work->effect = effect;
	BattleFx_BeginCanvasLayer(1);
	REG_BG2PA = 0x100;
	Resource_LoadAndDecompress((s32)&ResourceId_IceChipSheet, work->sheet, 1, 1);
	Resource_LoadAndDecompress((s32)&ResourceId_SparkleDots, dots, 0, 0);
	BattleFx_FetchRectangleBlitters(0, draw = pair);

	// 粒 : x が角度、z が半径 (56〜119)、y が高さ (-64〜-33 ドット)
	for (i = 0; i != 64; i++)
	{
		struct EffectStep *seed = &work->particles[i];

		seed->x = Random16() & 0xffff;
		seed->z = (Random16() & 63) + 56;
		seed->y = ((Random16() & 31) - 64) << 16;
	}

	work->transfer_mode = 2;
	work->transfer_value = 50;
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
	if (work->effect->side == 1)
		REG_BG2X = -0x7000;

	for (frame = 0; frame != RisingMotes_Timings[work->effect->variant * 3 + RISE_FRAME] + 75;
		frame++)
	{
		ymin = 120 << 16;
		ymax = 0;
		if (frame == RisingMotes_Timings[work->effect->variant * 3 + RISE_FRAME] + 11)
			BattleEventRuntime_BeginPhaseFar(132);
		point[0] = 0;
		point[1] = 0;
		point[2] = 512 << 16;
		Render_ResetTransformState();
		SceneTransform_ApplyPosition(point);
		if (frame >= 36 && frame < 64 && (frame & 3) == 0)
			AudioCommand_PlayFar(115);
		if (frame == 85)
			AudioCommand_PlayFar(136);
		for (i = 0; i != work->effect->count; i++)
		{
			if (frame == i * 4 + 40)
				ObjectGroup_UpdateMembers(work->effect->actors[i], 9, 5, -1, 0);
		}

		//------------------------------------------------------------------
		// 粒 : はじけるまでは半径を縮め、5 ドットずつ 48 ドットまで昇りながら回る。
		// はじけたあとは半径を広げて落ち、高さの上下限を求める
		//------------------------------------------------------------------
		if (frame < RisingMotes_Timings[work->effect->variant * 3 + RISE_FRAME])
			count = RisingMotes_Timings[work->effect->variant * 3 + RISE_MOTES];
		else
			count = 16;
		if (frame < RisingMotes_Timings[work->effect->variant * 3 + RISE_FRAME] + 35)
		{
			for (i = 0; i != count; i++)
			{
				if (frame > i)
				{
					s32 cel = i % 8;

					mote = &work->particles[i];
					if (mote->y < (48 - i / 2) << 16 && mote->y > -(48 << 16))
					{
						struct EffectPosition *pos;
						u32					   width;
						u32					   height;

						xyz[0] = mote->z * Trig_Sin(mote->x);
						xyz[1] = mote->y;
						xyz[2] = mote->z * Trig_Cos(mote->x);
						EffectPosition_ApplyBaseAndYOffset(xyz, pos = &screen);
						pos->x = (pos->x >> 17) + 64;
						pos->y = HI(pos->y) + 60;
						draw[1](canvas, work->sheet + RisingMotes_MoteCells[cel],
							pos->x - ((width = RisingMotes_MoteWidths[cel]) >> 1),
							pos->y - ((height = RisingMotes_MoteHeights[cel]) >> 1), width, height);
					}
					if (frame < RisingMotes_Timings[work->effect->variant * 3 + RISE_FRAME])
					{
						if (frame > i + 16)
						{
							if (mote->z > 4)
								mote->z -= 2;
							if (mote->y < 48 << 16)
								mote->y += 5 << 16;
							mote->x += 0x200;
						}
					}
					else
					{
						mote->z += 8;
						mote->y -= (i % 5 + 2) << 16;
						if (ymin > mote->y)
							ymin = mote->y;
						if (ymax < mote->y)
							ymax = mote->y;
					}
				}
			}
		}
		ymin += 64 << 16;
		ymax += 64 << 16;

		// 柱 (幅 32) : 36 フレーム目から 3 フレームおきに 1 本ずつ立てる
		if (frame < RisingMotes_Timings[work->effect->variant * 3 + RISE_FRAME])
		{
			for (i = 0; i != RisingMotes_Timings[work->effect->variant * 3 + RISE_COLUMNS]; i++)
			{
				if (i < (frame - 36) / 3)
				{
					s32 cel = i % 3;
					u32 height;

					if (frame >= RisingMotes_Timings[work->effect->variant * 3 + RISE_FRAME] - 7)
					{
						draw[1](canvas, work->sheet + RisingMotes_ColumnCells[cel],
							RisingMotes_ColumnSpots[i * 2],
							RisingMotes_ColumnSpots[i * 2 + 1]
								- (height = RisingMotes_ColumnHeights[cel]),
							32, height);
					}
					else
					{
						pair[0](canvas, work->sheet + RisingMotes_ColumnCells[cel],
							RisingMotes_ColumnSpots[i * 2],
							RisingMotes_ColumnSpots[i * 2 + 1]
								- (height = RisingMotes_ColumnHeights[cel]),
							32, height);
					}
				}
			}
		}

		// 破片 : はじけたとき、下の方から上へ 32 個を飛ばす (描くのは 24 個)
		if (frame == RisingMotes_Timings[work->effect->variant * 3 + RISE_FRAME])
		{
			for (i = 0; i != 32; i++)
			{
				struct EffectStep *chip = &CHIPS[i];

				chip->x = (Random16() & 127) << 16;
				chip->y = ((Random16() & 15) + 80) << 16;
				chip->z = ((Random16() & 63) - 32) << 12;
				chip->velocity_y = ((-Random16() & 15) - 16) << 13;
				chip->variant = (Random16() & 15) + 16;
			}
		}
		if (frame >= RisingMotes_Timings[work->effect->variant * 3 + RISE_FRAME])
		{
			for (i = 0; i != 24; i++)
			{
				struct EffectStep *chip = &CHIPS[i];

				if (chip->variant >= 0)
				{
					s32 cel = i % 8;

					draw[1](canvas, work->sheet + RisingMotes_EmberCells[cel], HI(chip->x),
						HI(chip->y), RisingMotes_EmberWidths[cel], RisingMotes_EmberHeights[cel]);
					chip->x += chip->velocity_x;
					chip->y += chip->velocity_y;
					chip->variant--;
				}
				if (ymin > chip->y)
					ymin = chip->y;
				if (ymax < chip->y)
					ymax = chip->y;
			}
		}
		ymin >>= 16;
		ymax >>= 16;
		if (ymax <= ymin)
			ymax = ymin + 1;

		// 煙 : 粒と破片の高さの範囲に出し、1 フレームに 1/2 ドット上がる
		if (frame == RisingMotes_Timings[work->effect->variant * 3 + RISE_FRAME])
		{
			for (i = 0; i != 32; i++)
			{
				struct EffectStep *puff = &work->particles[i];

				puff->velocity_x = (Random16() & 127) << 16;
				if (ymax == ymin)
					puff->velocity_y = ymin << 16;
				else
					puff->velocity_y = (Random16() % (ymax - ymin) + ymin) << 16;
				puff->variant = (Random16() & 15) + 20;
			}
		}
		if (frame >= RisingMotes_Timings[work->effect->variant * 3 + RISE_FRAME])
		{
			s32 rise = (frame - RisingMotes_Timings[work->effect->variant * 3 + RISE_FRAME]) / 2;

			for (i = 0; i != 32; i++)
			{
				struct EffectStep *puff = &work->particles[i];

				if (puff->variant >= 0 && puff->variant <= 17)
				{
					s32 cel = (17 - puff->variant) / 2;
					u32 size;

					draw[1](canvas, dots + BattleFx_PuffCells[cel],
						HI(puff->velocity_x) - ((size = BattleFx_PuffSizes[cel]) >> 1),
						HI(puff->velocity_y) - (size >> 1) - rise, size, size);
				}
				puff->variant--;
				if ((puff->variant == -1 || puff->variant == 17)
					&& frame < RisingMotes_Timings[work->effect->variant * 3 + RISE_FRAME] + 35)
				{
					puff->variant = 17;
					puff->velocity_x = (Random16() & 127) << 16;
					puff->velocity_y = (Random16() % (ymax - ymin) + ymin) << 16;
				}
			}
		}
		work->transfer_pending = 1;
		WaitFrames(1);
	}

	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
	Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
	BattleFx_EndCanvasLayer();
}

//--------------------------------------------------------------------------

// bolts.c

// バトル効果：まわりから寄ってくる四本の稲妻と、飛び散る火花

//--------------------------------------------------------------------------

#define SPARK_COUNT 1024
#define SPARKS ((struct EffectStep *)Ram_MapCellBuffer)

#define BOLT_SHEET 0x60e // 稲妻の絵の置き場
#define BOLT_SIZE 2880	 // 24x120 の一枚
#define BOLT_COUNT 4

//--------------------------------------------------------------------------
// 四本の稲妻が回りながら中心へ寄り、根元から火花を散らす
// 16 フレーム描き、火花は 64 フレームまで動かす
//--------------------------------------------------------------------------
void BattleFx_RunClosingBolts(struct BattleEffectArgument *effect)
{
	void				   **heap;
	void				   **cursor;
	struct BattleEffectWork *work;
	void					*canvas;
	s32						 frame;
	u8						*sheet;
	s32						 bolt;
	s32						 cnt;
	DrawRectangle			 draw[2];
	s32						 i;

	// ワーク、キャンバス、BG2 の拡大と位置
	heap = &gWorkSlot[HEAP_SLOT_BATTLE_EFFECT];
	cursor = heap;
	work = *cursor++;
	canvas = *cursor;
	sheet = heap[2];
	work->effect = effect;
	BattleFx_BeginTiledCanvas(1);
	if (work->effect->variant == 2)
		REG_BG2PA = 0x80; // 横 2 倍
	else
		REG_BG2PA = 0x100;
	BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, CLIP, GOUSEI_HIKAKU);
	draw[0] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER];
	BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, CLIP | GOUSEI_FLIP_X, GOUSEI_KASAN);
	draw[1] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE];
	Resource_LoadAndDecompress((s32)&ResourceId_LightningBoltSheet, work->sheet + BOLT_SHEET, 1, 1);
	Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);
	if (work->effect->variant == 2)
	{
		if (work->effect->side == 1)
			REG_BG2X = -0x1000;
		else
			REG_BG2X = 0x1000;
	}
	else
	{
		if (work->effect->side == 1)
			REG_BG2X = -0x8000;
	}

	// 火花の初期化
	for (i = 0; i != SPARK_COUNT; i++)
	{
		struct EffectStep *spark = &SPARKS[i];
		s32				   speed;
		s32				   angle;

		speed = (Random16() & 0x3ff) + 256;
		angle = (Random16() & 0x7fff) - 0x4000;
		spark->x = 0x4000;
		spark->y = 0x7000;
		spark->z = (Trig_Sin(angle) * speed) >> 16;
		spark->velocity_z = -(Trig_Cos(angle) * speed * 2) >> 16;
		spark->variant = 0;
	}

	work->transfer_mode = 2;
	work->transfer_value = 75;
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
	AudioCommand_PlayFar(138);

	for (frame = 0; frame != 64; frame++)
	{
		if (frame == 20)
			BattleEventRuntime_BeginPhaseFar(133); // 結果の再生を始める
		if (frame < 16)
		{
			// 5 フレームごとにキャンバスを消す
			if (frame % 5 == 2)
				Iwram_FillWords(canvas, 0x4000, 0x10101010);
			for (bolt = 0; bolt != BOLT_COUNT; bolt++)
			{
				s32 angle;
				s32 x;
				s32 y;

				// 横の半径が 32 から 1 フレームに 1 ドットずつ縮む
				cnt = 0;
				angle = ((frame << 11) + 0x4000) * bolt;
				x = (((32 - frame) * Trig_Sin(angle)) >> 16) + 64;
				y = -((Trig_Cos(angle) << 3) >> 16) - 8;
				if (work->effect->variant == 0)
					draw[0](canvas, work->sheet + (Random16() & 3) * BOLT_SIZE + BOLT_SHEET,
						x + (Random16() & 7) - 16, y, 24, 120);
				else
					draw[bolt & 1](canvas, work->sheet + (Random16() & 3) * BOLT_SIZE + BOLT_SHEET,
						x + (Random16() & 7) - 16, y, 24, 120);

				// 根元から火花を出す
				for (i = 0; i != SPARK_COUNT; i++)
				{
					struct EffectStep *spark = &SPARKS[i];

					if (spark->variant == 0)
					{
						s32 speed;
						s32 spread;

						speed = (Random16() & 0x1ff) + 128;
						spread = (Random16() & 0x7fff) - 0x4000;
						spark->x = x << 16;
						spark->y = (y + 112) << 16;
						spark->velocity_x = (Trig_Sin(spread) * speed) >> 9;
						spark->velocity_y = -(Trig_Cos(spread) * speed * 2) >> 7;
						spark->variant = (Random16() & 7) + 32;
						cnt++;
						if (cnt == LightningBolts_Sparks[work->effect->variant][1])
							break;
					}
				}
			}
			work->shake_frames = 1;
		}

		// 火花を動かし、下 (y 120) ではね返す
		for (i = 0; i != SPARK_COUNT; i++)
		{
			struct EffectStep *spark = &SPARKS[i];

			if (spark->variant > 0)
			{
				spark->variant--;
				EffectStep_AdvanceWithGravity2D(spark, 60, -0x800);
				if (spark->y > 0x780000)
				{
					spark->velocity_y = -spark->velocity_y / 2;
				}
				else if (spark->x >= 0 && spark->x < 0x7f0000 && spark->y >= 0)
				{
					s32 x = spark->x >> 16;
					s32 y = spark->y >> 16;
					s32 size = spark->variant / 8 + 1;

					draw[0](canvas, sheet + ParticleStreams_CellOffsets[size - 1], x - size / 2,
						y - size, size, size * 2);
				}
			}
		}

		// 対象は 4 フレームずつ遅れて当たる
		if (frame >= 4 && frame < 96)
		{
			for (i = 0; i != work->effect->count; i++)
			{
				if (frame == i * 4 + 4)
					ObjectGroup_UpdateMembers(work->effect->actors[i], 7, 5, i, 10);
			}
		}
		Camera_ApplyShake(2, 4);
		ObjectGroup_TickMemberTimers();
		work->transfer_pending = 1;
		WaitFrames(1);
	}

	Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
	BattleFx_EndCanvasLayer();
}

void BattleFx_RunParticleFieldMode1(struct BattleEffectArgument *effect)
{
	BattleFx_RunParticleField(effect, 1);
}

void BattleFx_RunParticleFieldMode0(struct BattleEffectArgument *effect)
{
	BattleFx_RunParticleField(effect, 0);
}

enum
{
	FIELD_MOTES,
	FIELD_PUFFS,
	FIELD_FRAMES
};

void BattleFx_RunParticleField(struct BattleEffectArgument *effect, s32 mode)
{
	void				   **cursor;
	struct BattleEffectWork *work;
	void					*canvas;
	DrawRectangle			 draw[2];
	s32						 variant;
	struct EffectPosition	 screen;
	s32						 frame;
	s32						 i;

	cursor = (void **)gBattleFxWork;
	work = *cursor++;
	canvas = *cursor;
	work->effect = effect;
	BattleFx_BeginCanvasLayer(1);
	Resource_LoadAndDecompress((s32)&ResourceId_BlastSheet, work->sheet, 1, 0);
	if (mode == 1)
	{
		for (i = 0; i != 64; i++)
		{
			s32 level = i / 2;

			BG_PLTT[i] = (level << 10) | (level << 5) | level;
		}
		variant = 1;
	}
	else
	{
		Iwram_CopyWords(
			(void *)BG_PLTT, Resource_GetTableEntry((s32)&ResourceId_FireStreakSheet), 128);
		variant = work->effect->variant;
	}

	for (i = 0; i != 32; i++)
	{
		struct EffectStep *puff = &work->particles[i];

		if (work->effect->side == 1)
			puff->x = 200 << 14;
		else
			puff->x = -(200 << 14);
		puff->y = 0;
		puff->z = 0;
		puff->velocity_x = ((Random16() & 63) - 32) << 13;
		puff->velocity_y = ((Random16() & 63) + 16) << 12;
		puff->velocity_z = ((Random16() & 63) - 32) << 13;
		puff->variant = 0;
	}
	for (i = 0; i != 1024; i++)
	{
		struct EffectStep *mote = &((struct EffectStep *)Ram_MapCellBuffer)[i];

		if (work->effect->side == 1)
			mote->x = 200 << 14;
		else
			mote->x = -(200 << 14);
		mote->y = 0;
		mote->z = 0;
		mote->velocity_x = ((Random16() & 63) - 32) << 13;
		mote->velocity_y = ((Random16() & 31) + 8) << 13;
		mote->velocity_z = ((Random16() & 63) - 32) << 13;
		mote->variant = 0;
	}

	BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 3, 2);
	draw[0] = (DrawRectangle)((union HeapState *)gWorkSlot)->slots[HEAP_SLOT_BLITTER];
	work->transfer_mode = 2;
	work->transfer_value = 75;
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);

	for (frame = 0; frame != ParticleField_Counts[variant][FIELD_FRAMES]; frame++)
	{
		struct BattleCamera *camera;

		camera = gCameraWork;
		Render_ResetTransformState();
		Graphics_PrepareTransferInIwramWork(camera->eye, camera->pos);
		if (frame == 2)
			AudioCommand_PlayFar(144);
		if (frame == ParticleField_Counts[variant][FIELD_FRAMES] - 48)
			BattleEventRuntime_BeginPhaseFar(133);

		for (i = 0; i != ParticleField_Counts[variant][FIELD_MOTES]; i++)
		{
			struct EffectStep *mote = &((struct EffectStep *)Ram_MapCellBuffer)[i];

			if (mote->y >= 0)
			{
				s32 size;

				EffectPosition_ApplyBaseAndYOffset((s32 *)mote, &screen);
				screen.x >>= 1;
				screen.x += work->effect->side * 32 - 16;
				if (screen.depth < 160)
					screen.depth = 160;
				if (screen.depth > 799)
					screen.depth = 799;
				size = 9 - (screen.depth - 160) / 64;
				draw[0](canvas,
					work->sheet + (ParticleStreams_CellOffsets[size - 1] + (i & 1) * 770) + 0x3200,
					screen.x - size / 2, screen.y - size, size, size * 2);
				EffectStep_AdvanceWithGravity3D(mote, 64, -0x2000);
			}
		}

		if (frame > 2)
		{
			for (i = 0; i != ParticleField_Counts[variant][FIELD_PUFFS]; i++)
			{
				struct EffectStep *puff = &work->particles[i];

				if (i < frame && puff->y >= 0)
				{
					EffectPosition_ApplyBaseAndYOffset((s32 *)puff, &screen);
					screen.x >>= 1;
					screen.x += work->effect->side * 32 - 16;
					if (puff->variant >= 0 && puff->variant <= 20)
					{
						s32 cel = puff->variant / 3;
						u32 width;

						draw[0](canvas, work->sheet + ParticleField_PuffCells[cel],
							screen.x - ((width = ParticleField_PuffSizes[cel]) >> 1),
							screen.y - (width >> 1), width, width);
					}
					if (puff->variant <= 20)
						puff->variant++;
					EffectStep_AdvanceWithGravity3D(puff, 64, -0x2000);
				}
			}
		}

		if (mode == 0)
		{
			for (i = 0; i != work->effect->count; i++)
			{
				if (frame == i + 6)
				{
					ObjectGroup_UpdateMembers(work->effect->actors[i], 7, 5, i, 10);
					BattleMotion_ApplyVariantMotionFar(work->effect->actors[i], 2);
				}
			}
		}
		else
		{
			for (i = 0; i != work->effect->count; i++)
			{
				if (frame == i + 6)
					ObjectGroup_UpdateMembers(work->effect->actors[i], 7, 5, i, 10);
			}
		}
		if (frame == 2)
			work->shake_frames = 6;
		Camera_ApplyShake(16, 16);
		ObjectGroup_TickMemberTimers();
		work->transfer_pending = 1;
		WaitFrames(1);
	}

	Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
	BattleFx_EndCanvasLayer();
}

#define gBattleFxWork (*(struct BattleEffectWork **)&gBattleFxWork)

void BattleFx_SpawnObjects(s32 entry_count, s32 kind, u32 variant)
{
	struct BattleEffectWork *work = gBattleFxWork;
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

#undef Camera_ApplyShake
#undef SPARKS
#undef SceneTransform_ApplyPosition
#undef gBattleFxWork

#define EffectPosition_ApplyAnimationAndYOffset \
	((void (*)(s32, s32 *))EffectPosition_ApplyAnimationAndYOffset)
#define Camera_ApplyShake ((void (*)(s32, s32))Camera_ApplyShake)
#define SceneTransform_ApplyPosition ((void (*)(s32 *))SceneTransform_ApplyPosition)

typedef s32 (*CopyWords)(void *, const void *, s32);
typedef struct BattleEffectArgument Efx;

typedef struct Spark
{
	s32 x;
	s32 y;
	s32 unk08;
	s32 dx;
	s32 dy;
	s32 unk14;
	s32 tick;
} Spark;

#define WORK_EFX ((Efx *)work->effect)
#define SHEET (work->sheet + 0xC56)
#define SPARKS ((Spark *)work->particles)

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

void BattleEffect_RunBurstShower(Efx *efx, s32 mode)
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
		BattleFx_FetchRectangleBlitters(WORK_EFX->side, blit);
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
		SPARKS[i].tick = -1;
	}

	EffectPosition_ApplyAnimationAndYOffset(WORK_EFX->actor, pos);
	if (mode == 3)
	{
		pos[1] -= 16;
	}
	if (mode == 4)
	{
		if (WORK_EFX->side == 1)
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
		if (WORK_EFX->side == 1)
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
	while (i != WORK_EFX->count)
	{
		EffectPosition_ApplyStepAndYOffset(WORK_EFX->actors[i], (struct EffectPosition *)seat[i]);
		i += 1;
	}

	frame = 0;
	do
	{
		pick = frame % WORK_EFX->count;
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
			if (WORK_EFX->side == 1)
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
				SPARKS[frame].x = pos[0] << 15;
				SPARKS[frame].y = pos[1] << 16;
				SPARKS[frame].dx = (aim[0] - pos[0]) << 11;
				SPARKS[frame].dy = (aim[1] - pos[1]) << 11;
				SPARKS[frame].tick = 0;
			}
		}

		i = 0;
		width = 32;
		height = 64;
		do
		{
			Spark *spark = &SPARKS[i];
			if (spark->tick >= 0)
			{
				if (mode == 7)
				{
					if (spark->tick > 5)
					{
						((DrawRectangle)blit[WORK_EFX->side])(dst, SHEET,
							((s16 *)&spark->x)[1] - 16, ((s16 *)&spark->y)[1] - 32, width, height);
					}
				}
				else if (mode == 4)
				{
					if (spark->tick > 5)
					{
						((DrawRectangle)blit[0])(dst, SHEET + ((spark->tick / 4) << 11),
							((s16 *)&spark->x)[1] - 16, ((s16 *)&spark->y)[1] - 32, width, height);
					}
				}
				else if (mode != 5)
				{
					if (spark->tick > 1)
					{
						((DrawRectangle)blit[0])(dst, SHEET + ((spark->tick / 4) << 11),
							((s16 *)&spark->x)[1] - 16, ((s16 *)&spark->y)[1] - 32, width, height);
					}
				}
				spark->x += spark->dx;
				spark->y += spark->dy;
				spark->tick += 1;
				if (spark->tick == 24)
				{
					spark->tick = -1;
				}
			}
			i += 1;
		} while (i != 64);

		if (mode == 5)
		{
			i = 0;
			while (i != WORK_EFX->count)
			{
				if ((frame >= (i * 4) + 2) && ((frame & 7) == i))
				{
					work->shake_frames = 8;
					ObjectGroup_UpdateMembers(WORK_EFX->actors[i], 7, 5, i, 4);
				}
				i += 1;
			}
		}
		else
		{
			i = 0;
			while (i != WORK_EFX->count)
			{
				if ((frame >= (i * 4) + 16) && ((frame & 7) == i))
				{
					work->shake_frames = 8;
					if (mode == 6)
					{
						ObjectGroup_UpdateMembers(WORK_EFX->actors[i], 14, 5, i, 4);
					}
					else
					{
						ObjectGroup_UpdateMembers(WORK_EFX->actors[i], 7, 5, i, 4);
					}
					BattleMotion_ApplyVariantMotionFar(WORK_EFX->actors[i], 4);
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
	DrawRectangleFn			 draw[2];
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
		star = &((struct EffectStep *)Ram_MapCellBuffer)[i];
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
		star = (struct EffectStep *)Ram_MapCellBuffer;
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
	DrawRectangleFn		 draw[2];
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
	draw[0] = (DrawRectangleFn)WORK_SLOT(HEAP_SLOT_BLITTER);
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
				/* FAKEMATCH: record is filled through r0 so loop.c cannot hoist its address into fp 2026-10-11: loop.c's second pass moves the invariant &record when 15 x savings x life 8 reaches the loop's insn count (about 100 after giv reduction); plain forms differ 118 (hoisted into fp, frame 92), loads before stores differ 12, and two one-pass blocks around the fill and the apply differ 2 (add r5, sp, #64 after the last store). */
				register s32 *rec asm("r0");

				Render_ResetTransformState();
				Graphics_PrepareTransferInIwramWork(camera->eye, camera->pos);
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
						((DrawRectangleFn)rectangle_a)(canvas, work->sheet + cel * 0x480 + 0x1b00,
							screen.x - 12, screen.y - 36, 24, 48);
					}
					else
					{
						EffectStep_AdvanceWithGravity3D(p, 60, 0x200);
						((DrawRectangleFn)rectangle_b)(canvas, work->sheet + cel * 0x480,
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


#define gTransitionWork (*(struct BattleBackgroundView **)&gTransitionWork)

extern u8 gMapCellBuffer[];

typedef struct Scale
{
	s32 x;
	s32 y;
} Scale;

void Object_ApplyProjectedPlacementFar(void *object, s32 *position, const Scale *scale, s32 mode);

extern const Scale StagedParticles_UnitScale;

#define OBJECTS (work->objects)
#define TRAILS ((struct EffectStep *)Ram_MapCellBuffer)
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
	/* FAKEMATCH: needed to match: the one-pass block keeps the table address after the second read */
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
			TRAILS[i * 24 + j].x = (Random16() & 15) + 48;
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
						OBJECTS[row * 3 + column], position, &StagedParticles_UnitScale, 0);
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
						struct EffectStep *trail = &TRAILS[i * 24 + j];

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
		ResourceObject_ReleaseFar((struct ResourceObjectWork *)OBJECTS[i]);
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

#undef Camera_ApplyShake

#define Camera_ApplyShake ((void (*)(s32, s32))Camera_ApplyShake)

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

// 土けむり 1 つ（gMapCellBuffer に並べる）
struct DualPuff
{
	s32 x;
	s32 y;
	u8	unknown_08[16];
	s32 age; // 経過フレーム（-1 は空き）
};

#define PUFFS ((struct EffectStep *)Ram_MapCellBuffer) // とげの土けむり

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

	s32				 i;
	struct DualPuff *puff;
	s32				 puff_age;
	s32				 image;

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
	/* FAKEMATCH: needed to match: the columns draw through a second pointer to the pair, whose address the frame keeps in a stack slot */
	draw = rectangle;
	draw[1] = second_slot;

	duration = DualTable_ColumnCounts[work->effect->variant] * 4 + 56;

	// 土けむりをすべて空きにする
	age = gMapCellBuffer + 0x18;
	i = 0;
	do
	{
		i++;
		*(s32 *)age = -1;
		age += sizeof(struct DualPuff);
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
									((DrawRectangleFn)draw[side])(canvas, work->sheet + cell,
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
									((DrawRectangleFn)draw[side])(canvas, work->sheet + cell,
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
							/* FAKEMATCH: needed to match: the scan reuses length as its puff pointer */
							length = (u32)gMapCellBuffer;
							n = 0;
							while (n != 512)
							{
								if (((struct DualPuff *)length)->age == -1)
								{
									((struct DualPuff *)length)->x =
										((Random16() & 0xf) + column->x) - 8;
									((struct DualPuff *)length)->y = (Random16() & 0xf) + 80;
									((struct DualPuff *)length)->age = 0;
									break;
								}
								length += sizeof(struct DualPuff);
								n++;
							}
						}
						i++;
					} while (i != DualTable_ColumnCounts[work->effect->variant]);
				}
			}

			// 土けむり：7 コマを 2 フレームずつ、左右対称に描く
			i = 0;
			puff = (struct DualPuff *)gMapCellBuffer;
			do
			{
				puff_age = puff->age;
				if (puff_age >= 0)
				{
					image = puff_age / 2;
					puff_cells = 0x1e59;
					if (mode != 0)
					{
						puff_cells = 0xaff;
					}
					((DrawRectangleFn)rectangle[0])(canvas,
						work->sheet + (DualTable_PuffCells[image] + puff_cells),
						puff->x - DualTable_PuffWidths[image],
						puff->y - (s8)DualTable_PuffHeights[image] / 2, DualTable_PuffWidths[image],
						(s8)DualTable_PuffHeights[image]);
					((DrawRectangleFn)rectangle[1])(canvas,
						work->sheet + (DualTable_PuffCells[image] + puff_cells), puff->x,
						puff->y - (s8)DualTable_PuffHeights[image] / 2, DualTable_PuffWidths[image],
						(s8)DualTable_PuffHeights[image]);
					puff->age++;
					if (puff->age == 14)
					{
						puff->age = -1;
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
		PUFFS[i].variant = -1; // 空き
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
						p = &PUFFS[j];
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
			if (PUFFS[i].variant >= 0)
			{
				s32 n = PUFFS[i].variant / 2;
				draw(canvas, work->sheet + BattleFxPillar_PuffCells[n],
					PUFFS[i].x - BattleFxPillar_PuffWidths[n],
					PUFFS[i].y - (s8)BattleFxPillar_PuffHeights[n] / 2,
					BattleFxPillar_PuffWidths[n], (s8)BattleFxPillar_PuffHeights[n]);
				draw_flip(canvas, work->sheet + BattleFxPillar_PuffCells[n], PUFFS[i].x,
					PUFFS[i].y - (s8)BattleFxPillar_PuffHeights[n] / 2,
					BattleFxPillar_PuffWidths[n], (s8)BattleFxPillar_PuffHeights[n]);
				if (++PUFFS[i].variant == 14)
				{
					PUFFS[i].variant = -1;
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

#undef SPARKS

#define SPARKS ((struct EffectStep *)Ram_MapCellBuffer) // 対象ひとりに 128 個
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
		SPARKS[j].variant = 0;
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
						struct EffectStep *spark = &SPARKS[i * 128 + j];
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
			struct EffectStep *spark = &SPARKS[j];

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

#undef SceneTransform_ApplyPosition

#define SceneTransform_ApplyPosition ((void (*)(s32 *))SceneTransform_ApplyPosition)

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
	s32						 size;

	heap_cache = (void **)gBattleFxWork;
	cursor = heap_cache;
	work = *cursor++;
	canvas = *cursor;
	sheet = heap_cache[2];
	work->effect = effect;
	BattleFx_BeginCanvasLayer(0);
	/* FAKEMATCH: needed to match: the canvas size sits in a local set here, well ahead of the two clears that use it */
	size = 0x4000;
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
	draw[0] = (BattleEffectDrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER];
	draw[1] = (BattleEffectDrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE];
	actor = GetBattleObjectSlotFar(work->effect->actor)->object;
	target = GetBattleObjectSlotFar(work->effect->actors[0])->object;
	for (i = 0; i != 64; i++)
	{
		struct EffectStep *spark = &((struct EffectStep *)Ram_MapCellBuffer)[i];
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
			struct EffectStep *spark = &((struct EffectStep *)Ram_MapCellBuffer)[i];

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
	Iwram_ClearWords((void *)0x06004000, size);
	Iwram_ClearWords(canvas, size);
	REG_BLDALPHA = 0x1010;
}
