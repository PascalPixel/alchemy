//--------------------------------------------------------------------------
// PLACE_FORMATION_OBJECTS.C
// バトル効果：オブジェクトの並べかたと、着弾の閃光・火花
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
#include "CANVAS.H"
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

//--------------------------------------------------------------------------
// 並べかた
//--------------------------------------------------------------------------
struct Scale
{
	s32 x; // 0x10000 で等倍
	s32 y;
};
struct Placement
{
	s32 x; // 16.16 ドット
	s32 y;
	s32 z;
	s32 w;
};

extern const struct Scale Formation_NormalScale; // 等倍
extern const struct Scale Formation_SmallScale;	 // 1.25 倍

void Object_ApplyProjectedPlacementFar(
	void *object, struct Placement *pos, struct Scale *scale, s32 flags);

//--------------------------------------------------------------------------
// work->objects を formation の形に並べる
// formation : 0 3×3 を 32 ドットおき、1 12 個（1・3・4・4 列）、
//             2 8 個（3・2・3 列）、3 8 個を 1.25 倍で
// x, z      : 並びの基準（16.16 ドット）
//--------------------------------------------------------------------------
void BattleFx_PlaceFormationObjects(s32 formation, s32 x, s32 z)
{
	struct BattleEffectWork *work = *(void **)gBattleFxWork;
	struct Scale			 normal = Formation_NormalScale;
	struct Scale			 big = Formation_SmallScale;
	struct Placement		 pos;
	s32						 i;

	pos.w = 0;
	pos.y = 255 << 16;
	switch (formation)
	{
	case 0:
		for (i = 0; i != 9; i++)
		{
			pos.x = (i % 3 << 21) + x; // 32 ドットおき
			pos.z = (i / 3 << 21) + z;
			Object_ApplyProjectedPlacementFar(work->objects[i], &pos, &normal, 0);
		}
		break;
	case 1:
		for (i = 0; i != 12; i++)
		{
			pos.x = (Formation_TwelveX[i] << 16) + x - (16 << 16);
			pos.z = (Formation_TwelveZ[i] << 16) + z - (32 << 16);
			Object_ApplyProjectedPlacementFar(work->objects[i], &pos, &normal, 0);
		}
		break;
	case 2:
		for (i = 0; i != 8; i++)
		{
			pos.x = (Formation_RightEightX[i] << 16) + x + (16 << 16);
			pos.z = (Formation_RightEightZ[i] << 16) + z;
			Object_ApplyProjectedPlacementFar(work->objects[i], &pos, &normal, 0);
		}
		break;
	case 3:
		for (i = 0; i != 8; i++)
		{
			pos.x = (Formation_SmallEightX[i] << 16) + x;
			pos.z = (Formation_SmallEightZ[i] << 16) + z;
			Object_ApplyProjectedPlacementFar(work->objects[i], &pos, &big, 0);
		}
		break;
	}
}

//--------------------------------------------------------------------------
// 着弾
//--------------------------------------------------------------------------
void AudioCommand_PlayFar(s32 cue);

extern struct EffectStep gMapCellBuffer[];

#define HI(v) (((s16 *)&(v))[1]) // 16.16 の整数部

#define CLIP (GOUSEI_CLIP_X | GOUSEI_CLIP_Y)

#define HAHEN_E 0x59d8 // 破片の絵の置き場（12×24 が 4 枚）

//--------------------------------------------------------------------------
// 着弾点で閃光をひらき、火花と破片を散らす（72 フレーム）
// はじめの 36 フレームは、オブジェクトも formation に並べなおす
// formation : BattleFx_PlaceFormationObjects の並べかた
// x, y      : 着弾点（16.16 ドット）
//--------------------------------------------------------------------------
void BattleEffect_RunImpactBurst(s32 formation, s32 x, s32 y)
{
	void				   **heap;
	void				   **cursor;
	struct BattleEffectWork *work;
	void					*canvas;
	DrawRectangle			 kasan;
	DrawRectangle			 hikaku;
	u8						*sheet;
	s32						 org_x;
	struct EffectStep		*p;
	s32						 frame;
	s32						 i;

	// ワーク、キャンバス、BG2 は横 2 倍で BG1・BG2 を半透明に
	heap = (void **)gBattleFxWork;
	cursor = heap;
	work = *cursor++;
	canvas = *cursor;
	sheet = heap[2];
	org_x = x;
	x += 40 << 16;
	x /= 2; // キャンバスは横半分
	REG_BG2PA = 0x80;
	REG_BG2X = 0;
	REG_BLDCNT = 0x3f46;
	BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, CLIP, GOUSEI_HIKAKU);
	hikaku = heap[7];
	BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, CLIP, GOUSEI_KASAN);
	kasan = heap[8];
	Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);
	Resource_LoadAndDecompress((s32)&ResourceId_FlareSheet, work->sheet, 1, 0);
	Resource_LoadAndDecompress((s32)&ResourceId_FlareImage, work->sheet + HAHEN_E, 0, 0);
	work->transfer_mode = 2;
	work->transfer_value = 50;
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);

	// 大きい火花：速さ 256〜511、寿命 16〜31 フレーム（使うのは前の 30 個）
	for (i = 0; i != 64; i++)
	{
		s32 speed = (Random16() & 255) + 256;
		s32 angle = Random16() & 0xffff;

		work->particles[i].x = x;
		work->particles[i].y = y;
		work->particles[i].velocity_x = (Trig_Sin(angle) * speed) >> 7;
		work->particles[i].velocity_y = -((Trig_Cos(angle) * speed) >> 6);
		work->particles[i].variant = (Random16() & 15) + 16;
	}

	// 破片 3 つを 120 度おきに
	for (i = 0; i != 3; i++)
	{
		work->particles[61 + i].x = x;
		work->particles[61 + i].y = y;
		work->particles[61 + i].velocity_x = (Trig_Sin(i * 0x5555) * 32) >> 6;
		work->particles[61 + i].velocity_y = -((Trig_Cos(i * 0x5555) * 32) >> 5);
	}

	// 小さい火花：速さ 32〜287、寿命 20〜35 フレーム
	for (i = 0; i != 64; i++)
	{
		s32 speed = (Random16() & 255) + 32;
		s32 angle = Random16() & 0xffff;

		gMapCellBuffer[i].x = x;
		gMapCellBuffer[i].y = y;
		gMapCellBuffer[i].velocity_x = (Trig_Sin(angle) * speed) >> 6;
		gMapCellBuffer[i].velocity_y = -((Trig_Cos(angle) * speed) >> 5);
		gMapCellBuffer[i].variant = (Random16() & 15) + 20;
	}

	for (frame = 0; frame != 72; frame++)
	{
		if (frame == 4)
			AudioCommand_PlayFar(154);
		if (frame == 32)
			AudioCommand_PlayFar(212);

		// 閃光：8 フレーム目から 5 フレームごとに次の絵
		if (frame <= 47)
		{
			s32 cell = (frame - 8) / 5;
			u32 width;
			u32 height;

			if (cell < 0)
				cell = 0;
			hikaku(canvas, work->sheet + ImpactBurst_CellSourceOffsets[cell],
				(x >> 16) - ((width = ImpactBurst_CellWidths[cell]) >> 1),
				(y >> 16) - ((height = ImpactBurst_CellHeights[cell]) >> 1), width, height);
		}

		// 大きい火花は 2 個ずつ 1 フレームおきに出る
		for (i = 0, p = work->particles; i != 30; i++, p++)
		{
			if (frame > i / 2 && p->variant > 0)
			{
				s32 size;
				s32 px;
				s32 py;

				p->variant--;
				EffectStep_AdvanceWithGravity2D(p, 60, 0);
				size = p->variant / 16 + 3;
				px = HI(p->x);
				py = HI(p->y);
				kasan(canvas, sheet + ParticleStreams_CellOffsets[size - 1], px - size / 2,
					py - size, size, size * 2);
			}
		}

		// 小さい火花は 36 フレーム目から
		for (i = 0, p = gMapCellBuffer; i != 60; i++, p++)
		{
			if (frame > 35 && p->variant > 0)
			{
				s32 size;
				s32 px;
				s32 py;

				p->variant--;
				EffectStep_AdvanceWithGravity2D(p, 60, 0);
				size = p->variant / 16 + 1;
				px = HI(p->x);
				py = HI(p->y);
				kasan(canvas, sheet + ParticleStreams_CellOffsets[size - 1], px - size / 2,
					py - size, size, size * 2);
			}
		}

		// 破片は 36〜63 フレーム、7 フレームごとに次の絵
		for (i = 0; i != 3; i++)
		{
			struct EffectStep *hahen = &work->particles[61 + i];

			if (frame >= 36 && frame <= 63)
			{
				s32 cell;
				s32 px;
				s32 py;

				EffectStep_AdvanceWithGravity2D(hahen, 64, 0);
				cell = (frame - 36) / 7;
				px = HI(hahen->x);
				py = HI(hahen->y);
				hikaku(canvas, work->sheet + cell * 0x120 + HAHEN_E, px - 6, py - 12, 12, 24);
			}
		}

		if (frame <= 35)
			BattleFx_PlaceFormationObjects(formation, org_x, y);
		work->transfer_pending = 1;
		WaitFrames(1);
	}

	// BG の文字ブロック 1 を消して終わる
	Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
	{
		/* FAKEMATCH: needed to match: the routine's address is loaded before the size, as a call through a local makes it */
		s32 (*clear)(void *, s32) = Iwram_ClearWords;

		clear(BG_CHAR_BLOCK(1), 0x4000);
	}
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
}
