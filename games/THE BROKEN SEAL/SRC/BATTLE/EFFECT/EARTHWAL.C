//--------------------------------------------------------------------------
// EARTHWAL.C
// バトル演出：キャンバス描画ルーチンの準備と、48 フレームの壁の演出
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
#include "MEM.H"
#include "HEAP_STATE.H"
#include "CALLBACK_SCHEDULER.H"
#include "CANVAS.H"
#include "RESOURCE.H"
#include "TYPES.H"
#include "IWRAM_CALL.H"
#include "RESOURCE_IDS.H"
#include "EFFECT_STEP.H"
#include "BATTLE_EFX.H"
#include "BATTLE_EFFECT_WORK.H"
#include "IO_REG.H"
#include "MENU_LIST.H"

#define Camera_ApplyShake ((void (*)(s32, s32))Camera_ApplyShake)

typedef s32 (*WordCopy)(void *, const void *, s32);

//--------------------------------------------------------------------------
// 128×128 のキャンバスへ描く 2 つのルーチンを組み立てる
// flip : 0 以外なら左右反転して描く
// out  : [0] 比較合成 (大きい色番号を残す)、[1] 加算合成 (63 で飽和)
//--------------------------------------------------------------------------
void BattleFx_FetchRectangleBlitters(s32 flip, BattleEffectDrawRectangle *out)
{
	if (flip == 0)
	{
		BattleEffect_LoadWork(
			HEAP_SLOT_BLITTER, 7, 7, GOUSEI_CLIP_X | GOUSEI_CLIP_Y, GOUSEI_HIKAKU);
		out[0] = (BattleEffectDrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER];
		BattleEffect_LoadWork(
			HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, GOUSEI_CLIP_X | GOUSEI_CLIP_Y, GOUSEI_KASAN);
		out[1] = (BattleEffectDrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE];
	}
	else
	{
		BattleEffect_LoadWork(
			HEAP_SLOT_BLITTER, 7, 7, GOUSEI_CLIP_X | GOUSEI_CLIP_Y | GOUSEI_FLIP_X, GOUSEI_HIKAKU);
		out[0] = (BattleEffectDrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER];
		BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7,
			GOUSEI_CLIP_X | GOUSEI_CLIP_Y | GOUSEI_FLIP_X, GOUSEI_KASAN);
		out[1] = (BattleEffectDrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE];
	}
}

//--------------------------------------------------------------------------
// 3 つの入口：mode を決めて本体を呼ぶ
//--------------------------------------------------------------------------
void BattleFx_RunFortyEightFrameMode1(struct BattleEffectArgument *effect)
{
	BattleFx_RunFortyEightFrameEffect(effect, 1);
}

void BattleFx_RunFortyEightFrameMode0(struct BattleEffectArgument *effect)
{
	BattleFx_RunFortyEightFrameEffect(effect, 0);
}

void BattleFx_RunFortyEightFrameMode2(struct BattleEffectArgument *effect)
{
	BattleFx_RunFortyEightFrameEffect(effect, 2);
}

static __inline__ void CopyPalette(WordCopy copy, void *dst, const void *src, s32 size)
{
	/* FAKEMATCH: the three palette copies share the routine's address but build the palette address again at each call, which only an inlined constant argument compiles to; a direct call keeps it in a register. */
	copy(dst, src, size);
}

//--------------------------------------------------------------------------
// 最初の対象の足元に、左右対称の壁 (EarthWall の絵) を立てる
// mode : 0 等倍、32 フレーム小さく揺らす
//        1 横 1.25 倍、8 フレーム大きく揺らす
//        2 横 1.25 倍、揺らさず Fireball のパレットで描く
//--------------------------------------------------------------------------
void BattleFx_RunFortyEightFrameEffect(struct BattleEffectArgument *effect, s32 mode)
{
	void				   **cursor;
	struct BattleEffectWork *work;
	void					*canvas;
	u8						*palette;
	s32						 x;
	struct EffectPosition	 pos;
	DrawRectangle			 draw_r;
	DrawRectangle			 draw_l;
	u8						*state;
	s32						 frame;
	s32						 cell;

	cursor = (void **)gBattleFxWork;
	work = *cursor++;
	canvas = *cursor;
	work->effect = effect;
	BattleFx_BeginCanvasLayer(0);

	// 絵は EarthWall、パレットは MarsDjinn (mode 2 は Fireball) で上書きする
	palette = Resource_GetTableEntry((s32)&ResourceId_EarthWallSheet);
	CopyPalette(Iwram_CopyWords, (void *)0x05000000, palette, 128);
	palette += 128;
	Resource_DecodeType01(palette, work);
	palette = Resource_GetTableEntry((s32)&ResourceId_MarsDjinnSheet);
	CopyPalette(Iwram_CopyWords, (void *)0x05000000, palette, 128);
	if (mode == 2)
	{
		palette = Resource_GetTableEntry((s32)&ResourceId_FireballSheet);
		CopyPalette(Iwram_CopyWords, (void *)0x05000000, palette, 128);
	}

	// キャンバスの x = 64 が対象の位置にくるよう BG2 をずらす
	EffectPosition_ApplyAlternateStepAndYOffset(work->effect->actors[0], &pos);
	if (mode == 0)
	{
		REG_BG2PA = 0x100; // 等倍
		x = 64 - pos.x;
		REG_BG2X = x << 8;
	}
	else
	{
		REG_BG2PA = 0xcc; // 0.8 (横 1.25 倍に表示)
		x = (-pos.x * 4) / 5 + 64;
		REG_BG2X = x << 8;
	}

	// 左半分はそのまま、右半分は左右反転して比較合成で描く
	BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, GOUSEI_CLIP_X | GOUSEI_CLIP_Y, GOUSEI_HIKAKU);
	state = (u8 *)gWorkSlot;
	draw_l = *(DrawRectangle *)(state + HEAP_SLOT_BLITTER * 4);
	BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7,
		GOUSEI_CLIP_X | GOUSEI_CLIP_Y | GOUSEI_FLIP_X, GOUSEI_HIKAKU);
	work->transfer_mode = 2;
	/* FAKEMATCH: reading the second blitter through the slot pointer's own variable keeps it in that register between the two transfer stores; a direct assignment only orders three instructions differently. */
	state += 188; // gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE]
	state = *(u8 **)state;
	work->transfer_value = 50; // 転送のたびに半分へ減らして残像にする
	draw_r = (DrawRectangle)state;
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);

	if (mode == 2)
	{
		work->shake_frames = 0;
		AudioCommand_PlayFar(212);
	}
	else if (mode == 1)
	{
		work->shake_frames = 8;
		AudioCommand_PlayFar(212);
	}
	else
	{
		work->shake_frames = 32;
	}

	for (frame = 0; frame != 48; frame++)
	{
		if (frame == 0)
		{
			if (mode == 2)
				ObjectGroup_UpdateMembers(work->effect->actors[0], 7, -1, 0, 32);
			else
				ObjectGroup_UpdateMembers(work->effect->actors[0], 10, -1, 0, 32);
		}
		if (frame == 24)
			BattleEventRuntime_BeginPhaseFar(0);
		if (frame == 8 && mode == 0)
			AudioCommand_PlayFar(126);

		// 4 フレームごとに絵 0, 1, 2 と進め、その後は 2 と 1 を交互に (27 フレームまで)
		if (frame <= 31)
		{
			cell = frame / 4;
			if (cell > 2)
				cell = (cell & 1) + 1;
			if (frame <= 27)
			{
				draw_l(canvas, work->sheet + EarthWall_CellSourceOffsets[cell],
					64 - EarthWall_CellWidths[cell], (pos.y - EarthWall_CellHeights[cell]) + 8,
					EarthWall_CellWidths[cell], EarthWall_CellHeights[cell]);
				draw_r(canvas, work->sheet + EarthWall_CellSourceOffsets[cell], 64,
					(pos.y - EarthWall_CellHeights[cell]) + 8, EarthWall_CellWidths[cell],
					EarthWall_CellHeights[cell]);
			}
		}
		if (mode == 0)
			Camera_ApplyShake(2, 2);
		else
			Camera_ApplyShake(16, 16);
		ObjectGroup_TickMemberTimers();
		work->transfer_pending = 1;
		WaitFrames(1);
	}

	Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
	BattleFx_EndCanvasLayer();
}
