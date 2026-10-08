//--------------------------------------------------------------------------
// HUE.C
// 背景の走査線スクロール DMA、色相の巡回、BG パレット 15 の切り替え
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
#include "WINDOW.H"
#include "GAME_STATE.H"
#include "OBJECT_RUNTIME.H"
#include "DMA.H"
#include "OBJDISP.H"
#include "TYPES.H"
#include "CALLBACK_SCHEDULER.H"
#include "IWRAM_CALL.H"
#include "GLOBAL_CELLS.H"
#include "TBS_EDITION.H"
#include "IO_REG.H"

#define PaletteGlow_UpdateFar ((s32 (*)(s32, s32))PaletteGlow_UpdateFar)

struct WaveFxWork
{
	u16 lines[2][162]; // 走査線ごとの BG0 横スクロール (2 面)
	u16 phase;
	u8	page;	// HBlank DMA で送る面
	u8	rgb[3]; // 色相から求めた色 (赤・緑・青、各 4〜11)
	u16 hue;	// 色相 (度)
	u16 source_id;
	u16 target_id;
	u8	delay; // 0 以外のあいだは DMA を張らない
	u8	counter;
};

#define GREY28 0x739c // 灰色 (赤・緑・青とも 28)

//--------------------------------------------------------------------------
// BG0 の横スクロールを走査線ごとに書き換える HBlank DMA を張りなおす
//--------------------------------------------------------------------------
void BattleFx_ArmBg0HBlankDma(void)
{
	struct WaveFxWork *work = gBattleBgFxWork;
	if (!work->delay)
	{
		u16			 *line = work->lines[work->page];
		volatile u16 *dma = REG_DMA0;
		dma[5] &= 0xc5ff; // 開始タイミングとリピートを消す
		dma[5] &= 0x7fff; // 止める
		(void)dma[5];
		// HBlank ごとに 16 ビットを 1 回 (リピート、転送先リロード)
		Dma_Set(line, REG_BG0HOFS, 0xa2600001, (volatile u32 *)dma);
	}
}

//--------------------------------------------------------------------------
// 色相を 4 度すすめ、その色を求める
//--------------------------------------------------------------------------
void BattleFx_AdvanceHueCycle(void)
{
	struct WaveFxWork *work = gBattleBgFxWork;
	s32				   r = 0;
	s32				   g = 0;
	s32				   b = 0;
	s32				   ofs;
	u8				  *pr;
	u8				  *pg;
	u8				  *pb;

	BattleFx_ComputeHueChannels(work->hue << 16, &r, &g, &b);

	// 0〜31 を 4〜11 に縮める (ofs は rgb の位置から 1 ずつ進む)
	ofs = 0x28b;
	pr = (u8 *)work + ofs;
	*pr = (r >> 18) + 4;
	ofs += 1;
	pg = (u8 *)work + ofs;
	*pg = (g >> 18) + 4;
	ofs += 1;
	pb = (u8 *)work + ofs;
	*pb = (b >> 18) + 4;

	work->hue += 4;

	*pr &= 0x1f;
	*pg &= 0x1f;
	*pb &= 0x1f;

	if (work->hue >= 360)
	{
		work->hue = 0;
	}
}

//--------------------------------------------------------------------------
// 色相から赤・緑・青を求める (各 0〜31、16.16 固定小数)
// hue : 色相 (度、16.16 固定小数)
//--------------------------------------------------------------------------
void BattleFx_ComputeHueChannels(s32 hue, s32 *r, s32 *g, s32 *b)
{
	*r = BattleFx_HueChannelRamp(hue + (120 << 16), 0, 31 << 16);
	*g = BattleFx_HueChannelRamp(hue, 0, 31 << 16);
	*b = BattleFx_HueChannelRamp(hue - (120 << 16), 0, 31 << 16);
}

//--------------------------------------------------------------------------
// 色相に対する色の強さ
// 0〜60 度で上がり、60〜180 度は high、180〜240 度で下がり、ほかは low
//--------------------------------------------------------------------------
s32 BattleFx_HueChannelRamp(s32 angle, s32 low, s32 high)
{
	s32 product;

	angle = Fixed_Remainder(angle, 360 << 16);
	if (angle < 60 << 16)
	{
		product = Iwram_MulQ16(high, angle);
	}
	else
	{
		if (angle >= 60 << 16 && angle < 180 << 16)
			return high;
		/* FAKEMATCH: needed to match: the goto places the low return after the divide */
		if (!(angle >= 180 << 16 && angle < 240 << 16))
			goto out;
		product = Iwram_MulQ16(high, (240 << 16) - angle);
	}
	{
		s32 (*divide)(s32, s32) = Iwram_RatioMulQ14;

		return divide(60 << 16, product);
	}
out:
	return low;
}

//--------------------------------------------------------------------------
// 16.16 固定小数の剰余 (divisor が 0 なら 0)
//--------------------------------------------------------------------------
s32 Fixed_Remainder(s32 value, s32 divisor)
{
	s32 quot;

	if (divisor == 0)
		return 0;
	if (divisor & 0xf0000000)
		divisor = -divisor;
	quot = Iwram_RatioMulQ14(divisor, value);
	return value - Iwram_MulQ16(quot & 0xffff0000, divisor); // 商の整数部 × divisor を引く
}

//--------------------------------------------------------------------------
// 移動の目標がなければ、目標なしのスクリプトを始める
//--------------------------------------------------------------------------
void BattleFx_SetCallbackWhenTargetUnset(struct ObjectRuntime *obj)
{
	s32 ty;
	s32 tx;

	tx = obj->target_x;
	if (tx == 0x80000000) // 目標なし
	{
		ty = obj->target_y;
		if ((ty == tx) && (obj->target_z == ty))
		{
			ObjectDispatch_InitializeFar(
				(struct DispatchObject *)obj, BattleFx_UntargetedObjectScript);
		}
	}
}

//--------------------------------------------------------------------------
// BG パレット 15 の色 1, 3〜12 を灰色にし、走査線スクロールを始める
//--------------------------------------------------------------------------
void Ui_FillBank15PaletteGrey(void)
{
	volatile u16 *p;

	((struct UiRenderWork *)gWindowWork[0])->mode = 1;
	p = &BG_PLTT_COLOR(15, 1);
	*p = GREY28;
	p += 2;
	*p = GREY28;
	p += 1;
	*p = GREY28;
	p += 1;
	*p = GREY28;
	p += 1;
	*p = GREY28;
	p += 1;
	*p = GREY28;
	p += 1;
	*p = GREY28;
	p += 1;
	*p = GREY28;
	p += 1;
	*p = GREY28;
	p += 1;
	*p = GREY28;
	p += 1;
	*p = GREY28;
	Scheduler_AddOrUpdateCallback(BattleFx_ArmBg0HBlankDma, 0x480);
}

//--------------------------------------------------------------------------
// 走査線スクロールを止め、BG パレット 15 を戻す
//--------------------------------------------------------------------------
void Ui_SetBank15PaletteAndClearRenderMode(void)
{
	void *work;

	work = gWindowWork[0];
	Scheduler_RemoveCallback(BattleFx_ArmBg0HBlankDma);
	BG_PLTT_COLOR(15, 1) = 0x7fff;	// 白
	BG_PLTT_COLOR(15, 3) = 0;		// 黒
	BG_PLTT_COLOR(15, 11) = 0x294a; // 灰 (10)
	BG_PLTT_COLOR(15, 12) = 0x5294; // 灰 (20)
	PaletteGlow_UpdateFar(gGameState.palette_glow[0], gGameState.palette_glow[1]);
	((struct UiRenderWork *)work)->mode = 0;
}
