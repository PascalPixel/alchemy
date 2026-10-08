//--------------------------------------------------------------------------
// dispatch.c
// バトル演出：演出の振り分け・走査線ごとのウインドウ端・生きている対象
//--------------------------------------------------------------------------

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
#include "battle_effect_work.h"
#include "types.h"
#include "battle_efx.h"
#include "global_cells.h"
#include "system.h"
#include "dma.h"
#include "ram_buffer.h"
#include "btlsys.h"
#include "btltype.h"
#include "io_reg.h"

#define BattleEffect_RunCirclingFallingScene ((void (*)(s32 *))BattleEffect_RunCirclingFallingScene)
#define BattleEffect_RunEmberColumns ((void (*)(s32 *))BattleEffect_RunEmberColumns)
#define BattleFx_RunObjectRow ((void (*)(s32 *))BattleFx_RunObjectRow)
#define BattleEffect_RunStagedParticles ((void (*)(s32 *))BattleEffect_RunStagedParticles)
#define BattleFx_InitializeMode6 ((void (*)(s32 *))BattleFx_InitializeMode6)
#define BattleFx_InitializeMode10 ((void (*)(s32 *))BattleFx_InitializeMode10)
#define BattleFx_InitializeMode12 ((void (*)(s32 *))BattleFx_InitializeMode12)

typedef void (*Callback)(s32 *);
extern Callback BattleFx_ModeHandlers[];

extern u8 gMapCellBuffer[];

//--------------------------------------------------------------------------
// 演出の作業領域を取り、種類（arg[0]）ごとの演出を流して返す
// arg : struct BattleEffectArgument
//--------------------------------------------------------------------------
void BattleFx_InitializeMode(s32 *arg)
{
	Runtime_AllocateHeapBlock(HEAP_SLOT_BATTLE_SHEET, 0x302);
	Runtime_AllocateHeapBlock(HEAP_SLOT_BATTLE_EFFECT, sizeof(struct BattleEffectWork));
	Runtime_AllocateHeapBlock(HEAP_SLOT_BATTLE_CANVAS, 0x4000);

	if (*arg == 0)
	{
		BattleFx_InitializeDefaultMode((struct BattleEffectArgument *)arg);
	}
	else
	{
		switch (*arg)
		{
		case 1:
			BattleEffect_RunCirclingFallingScene(arg);
			break;
		case 2:
			BattleEffect_RunEmberColumns(arg);
			break;
		case 3:
			BattleFx_RunObjectRow(arg);
			break;
		case 4:
			BattleEffect_RunStagedParticles(arg);
			break;
		case 5:
			BattleEffect_RunDitherDissolveScene((struct BattleEffectArgument *)arg);
			break;
		case 6:
			BattleFx_InitializeMode6(arg);
			break;
		case 7:
			BattleFx_RenderAnimationMode0(arg);
			break;
		case 8:
			BattleFx_RunWhirlwindComets((struct BattleEffectArgument *)arg);
			break;
		case 9:
			BattleEffect_RunShockwaveScene((struct BattleEffectArgument *)arg);
			break;
		case 10:
			BattleFx_InitializeMode10(arg);
			break;
		case 11:
			BattleFx_InitializeDefaultMode((struct BattleEffectArgument *)arg);
			break;
		case 12:
			BattleFx_InitializeMode12(arg);
			break;
		}
	}

	Runtime_ReleaseHeapBlock(HEAP_SLOT_BATTLE_CANVAS);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BATTLE_EFFECT);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BATTLE_SHEET);
}

//--------------------------------------------------------------------------
// 演出の作業領域を取り、BattleFx_ModeHandlers の表から演出を流す
// 種類 0 は演出なしで、variant（arg[6]）を 0 にする
// arg : struct BattleEffectArgument
//--------------------------------------------------------------------------
void BattleFx_DispatchMode(s32 *arg)
{
	s32						 kind;
	struct BattleEffectWork *work;

	Runtime_AllocateHeapBlock(HEAP_SLOT_BATTLE_SHEET, 0x302);
	Runtime_AllocateHeapBlock(HEAP_SLOT_BATTLE_EFFECT, sizeof(struct BattleEffectWork));
	Runtime_AllocateHeapBlock(HEAP_SLOT_BATTLE_CANVAS, 0x4000);

	work = *(struct BattleEffectWork **)gBattleFxWork;
	work->effect = (struct BattleEffectArgument *)arg;
	kind = arg[0];
	if (kind == 0)
		arg[6] = 0;
	else
		BattleFx_ModeHandlers[kind - 1](arg);

	Runtime_ReleaseHeapBlock(HEAP_SLOT_BATTLE_CANVAS);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BATTLE_EFFECT);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BATTLE_SHEET);
}

//--------------------------------------------------------------------------
// ウインドウ 0 の右端を走査線ごとに決め、HBlank DMA で毎ライン書き込む
// （スケジューラのコールバック）
// gMapCellBuffer +0   : 基準の右端（u16）
// gMapCellBuffer +2   : 8〜135 ライン目のずれ（u8 × 128）
// gMapCellBuffer +0x82: 作った WIN0H の表（u16 × 160）
// 表の外のラインは 0xfff1（左端 > 右端）で、ウインドウに入らない
//--------------------------------------------------------------------------
void BattleFx_BuildWindowEdgeTable(void)
{
	u16 *base;
	u16 *tbl;
	u8	*ofs;
	s32	 i;
	s32	 x;

	base = (u16 *)gMapCellBuffer;
	tbl = (u16 *)(Ram_MapCellBuffer + 0x82);
	ofs = Ram_MapCellBuffer + 2;
	for (i = 0; i != 160; i++)
	{
		if (i >= 8 && i <= 135)
		{
			x = *base - ofs[i - 8];
			if (x < 0)
				x = 0;
			if (x > 240)
				x = 240;
			tbl[i] = x;
		}
		else
		{
			tbl[i] = 0xfff1;
		}
	}
	{
		volatile u16 *channel = REG_DMA0;

		// DMA0 のリピートと開始条件を消してから止める
		channel[5] &= 0xc5ff;
		channel[5] &= 0x7fff;
		(void)channel[5];
		// HBlank ごと・リピート・転送先リロードで 1 ハーフワード
		Dma_Set(gMapCellBuffer + 0x82, (void *)&REG_WIN0H, 0xa2600001, (volatile u32 *)channel);
	}
}

//--------------------------------------------------------------------------
// 対象の側（敵か味方か）で HP の残っている者を集め、オブジェクトを出す
// 敵の番号は 128 から
//--------------------------------------------------------------------------
void BattleFx_SelectLivingTargets(struct BattleEffectArgument *arg)
{
	s16 list[14];
	s32 cnt;
	s32 i;

	cnt = 0;
	if (arg->actors[0] > 127)
	{
		for (i = 0; i != 6; i++)
		{
			s32 id = i + 128;

			if (UnitGetFar(id)->hp > 0)
				list[cnt++] = id;
		}
	}
	else
	{
		for (i = 0; i != 8; i++)
		{
			if (UnitGetFar(i)->hp > 0)
				list[cnt++] = i;
		}
	}
	list[cnt] = 0xff; // 終わり
	BattleActor_SpawnObjectsForListFar(list, 0);
}
