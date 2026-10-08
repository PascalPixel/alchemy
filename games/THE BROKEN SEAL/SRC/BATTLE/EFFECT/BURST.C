//--------------------------------------------------------------------------
// BURST.C
// バトル効果：相手へ寄って当て、輪と粒を飛び散らせる (色の違う四種)
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
#include "BATTLE_EFFECT_WORK.H"
#include "CANVAS.H"
#include "RESOURCE.H"
#include "TYPES.H"
#include "CALLBACK_SCHEDULER.H"
#include "SYSTEM.H"
#include "IWRAM_CALL.H"
#include "BATTLE_EFX.H"
#include "EFFECT_STEP.H"
#include "B5_CONTEXT.H"
#include "MOTOBJ.H"
#include "BTLDISP.H"
#include "RAM_BUFFER.H"
#include "RESOURCE_IDS.H"
#include "TRANSFORM.H"
#include "IO_REG.H"

#define Camera_ApplyShake ((void (*)(s32, s32))Camera_ApplyShake)

#define CLIP (GOUSEI_CLIP_X | GOUSEI_CLIP_Y)

#define TSUBU_MAX 64	// 粒の数
#define RING_SIZE 0x3c0 // 輪の一枚 (20x48)

void Graphics_PackTileRows(void *source, void *destination, s32 width, s32 rows);

//--------------------------------------------------------------------------
// 四種の入口
//--------------------------------------------------------------------------
void BattleFx_RunParticleFieldVariant0(s32 effect)
{
	BattleFx_RunParticleFieldVariant((struct BattleEffectArgument *)effect, 0);
}

void BattleFx_RunParticleFieldVariant1(s32 effect)
{
	BattleFx_RunParticleFieldVariant((struct BattleEffectArgument *)effect, 1);
}

void BattleFx_RunParticleFieldVariant2(s32 effect)
{
	BattleFx_RunParticleFieldVariant((struct BattleEffectArgument *)effect, 2);
}

void BattleFx_RunParticleFieldVariant3(s32 effect)
{
	BattleFx_RunParticleFieldVariant((struct BattleEffectArgument *)effect, 3);
}

//--------------------------------------------------------------------------
// 使う者が最初の対象へ寄り、10 フレーム目に当てる
// 対象の位置に輪を出し、粒を 64 個飛び散らせる。全部で 60 フレーム
// variant : 色 (パレットの元の絵 0-3)
//--------------------------------------------------------------------------
void BattleFx_RunParticleFieldVariant(struct BattleEffectArgument *effect, s32 variant)
{
	/* FAKEMATCH: the blitter/camera declarations and independent store order preserve spill slots and loop preheader order. */
	void					**heap;
	void					**cursor;
	struct BattleEffectWork	 *work;
	void					 *canvas;
	BattleEffectDrawRectangle draw[2];
	struct BattleCamera		 *camera;
	u8						 *sheet;
	struct EffectStep		 *p;
	struct EffectStep		 *step;
	struct EffectPosition	  origin;
	struct EffectPosition	  screen;
	struct EffectPosition	  pos;
	struct MotionObject		 *tgt;
	s32						  frame;
	s32						  n;
	s32						  size;
	s32						  t;
	s32						  pal;
	u8						 *cells;

	// ワーク、キャンバス、カメラ
	cells = Ram_MapCellBuffer;
	heap = (void **)&gBattleFxWork;
	cursor = heap;
	work = *cursor++;
	canvas = *cursor;
	camera =
		*(struct BattleCamera **)((u8 *)heap
								  - (HEAP_SLOT_BATTLE_EFFECT - HEAP_SLOT_CAMERA) * sizeof(void *));
	sheet = heap[2];
	work->effect = effect;
	BattleFx_BeginCanvasLayer(0);

	// 上半分と、上下を反転した下半分。向こう側なら左右も反転する
	if (work->effect->side == 0)
	{
		BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, CLIP, GOUSEI_HIKAKU);
		BattleEffect_LoadWork(
			HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, CLIP | GOUSEI_FLIP_Y, GOUSEI_HIKAKU);
	}
	else
	{
		BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, CLIP | GOUSEI_FLIP_X, GOUSEI_HIKAKU);
		BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, CLIP | GOUSEI_FLIP, GOUSEI_HIKAKU);
	}
	draw[0] = (BattleEffectDrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER];
	draw[1] = (BattleEffectDrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE];

	// 絵と色
	Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);
	Resource_LoadAndDecompress((s32)&ResourceId_YellowOrbSheet, work->sheet, 1, 0);
	Graphics_PackTileRows(work, cells, 40, 288);
	Resource_LoadAndDecompress((s32)&ResourceId_BlueBurstSheet, work->sheet, 1, 1);
	switch (variant)
	{
	case 0:
		pal = (s32)&ResourceId_FirePillarSheetB;
		break;
	case 1:
		pal = (s32)&ResourceId_IceBlockSheet;
		break;
	case 2:
		pal = (s32)&ResourceId_PinkBurstSheet;
		break;
	default:
		pal = (s32)&ResourceId_BlastSheet;
		break;
	}
	Iwram_CopyWords((void *)BG_PLTT, Resource_GetTableEntry(pal), 128);
	work->transfer_mode = 2;
	work->transfer_value = 75;
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);

	// 使う者を寄せ、対象の位置に粒を用意する (x は画面の中央の側へ飛ぶ)
	BattleFx_SetApproachMotion(work->effect->actor, work->effect->actors[0], 10);
	tgt = GetBattleObjectSlotFar(work->effect->actors[0])->object;
	for (n = 0; n != TSUBU_MAX; n++)
	{
		step = &work->particles[n];
		step->x = tgt->x;
		step->y = tgt->y + 0xa0000; // 10 ドット上
		step->z = tgt->z;
		step->velocity_x = (Random16() & 0x1ff) << 11;
		step->velocity_y = ((Random16() & 255) - 64) << 11;
		step->velocity_z = ((Random16() & 255) - 128) << 11;
		if (step->x > 0)
			step->velocity_x = -step->velocity_x;
		step->variant = n / 2 + 16; // 残りのフレーム
	}
	EffectPosition_ApplyAlternateStepAndYOffset(work->effect->actors[0], &origin);

	for (frame = 0; frame != 60; frame++)
	{
		// 15 フレームのあいだ、使う者の位置に絵を上下二つに分けて描く
		if (frame <= 14)
		{
			EffectPosition_ApplyAlternateStepAndYOffset(work->effect->actor, &screen);
			draw[0](canvas, work, screen.x / 2 - 16, screen.y - 48, 40, 32);
			draw[1](canvas, work, screen.x / 2 - 16, screen.y - 16, 40, 32);
		}
		// 当たり
		if (frame == 10)
		{
			ObjectGroup_UpdateMembers(work->effect->actors[0], 7, 5, 0, 8);
			BattleMotion_ApplyVariantMotionFar(work->effect->actors[0], 4);
			BattleEventRuntime_BeginPhaseFar(134);
			work->shake_frames = 8;
		}
		// 輪：8 フレーム目から 2 フレームずつ 6 枚
		t = frame - 8;
		if (t >= 0 && t <= 11)
		{
			s32 ring = t / 2;
			draw[0](canvas, cells + ring * RING_SIZE, origin.x / 2 - 16, screen.y - 40, 20, 48);
		}
		// 粒
		if (t >= 0 && t <= 55)
		{
			Render_ResetTransformState();
			Graphics_PrepareTransferInIwramWork(camera->eye, camera->pos);
			for (n = 0; n != TSUBU_MAX; n++)
			{
				p = &work->particles[n];
				size = p->variant;
				if (size > 0)
				{
					EffectPosition_ApplyBaseAndYOffset(&p->x, &pos);
					size >>= 4;
					size += 2;
					pos.x >>= 1;
					draw[0](canvas, sheet + ParticleStreams_CellOffsets[size - 1], pos.x - size / 2,
						pos.y - size, size, size * 2);
					EffectStep_AdvanceWithGravity3D(p, 60, -0x200);
					p->variant--;
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
