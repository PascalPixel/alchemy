//--------------------------------------------------------------------------
// ROCKWALL.C
// バトル演出：岩の柱を 3 本せり上げ、対象を打ち上げる
//--------------------------------------------------------------------------

#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "CANVAS.H"
#include "MEM.H"
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
#include "HEAP_STATE.H"

#define Camera_ApplyShake ((void (*)(s32, s32))Camera_ApplyShake)

#define CLIP (GOUSEI_CLIP_X | GOUSEI_CLIP_Y) // 上下左右ではみ出しを切る

void Audio_PlayCue(s32 cue);

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
	/* FAKEMATCH: forwarded through Call3, the copy loads the buffer address straight into r0, so the loop below loads its own after the call. */
	Call3((void (*)())Iwram_CopyWords, (s32)Ram_MapCellBuffer, 0x06008000, 0x8000);
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
	Audio_PlayCue(141);

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
