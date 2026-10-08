//--------------------------------------------------------------------------
// BLEND.C
// 表示：画面の明るさのフェード（白・黒）の設定と、終わるまでの待ち
//--------------------------------------------------------------------------

#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "LIB.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "TYPES.H"
#include "FIXED_MATH.H"

// フェードは VBlank ごとに BlendTransition_Update が進める。
// 明るさ（BLDY）は 0 で元のまま、16 で真っ白または真っ黒。
//   gBlendBrighten   : 0 黒へ / 1 白へ
//   gBlendLayers     : BLDCNT の対象レイヤー（下の 4 つは 0x3e で BG1〜3・OBJ・背景色）
//   gBlendFramesLeft : 残りフレーム数

//--------------------------------------------------------------------------
// 黒へフェードアウト
// frames : かけるフレーム数
//--------------------------------------------------------------------------
void Blend_SetDarkenTarget16(s32 frames)
{
	gBlendBrighten = 0;
	gBlendLayers = 0x3e;
	gBlendStartLevel = gBlendTargetLevel;
	gBlendTargetLevel = 16;
	gBlendDuration = frames;
	gBlendFramesLeft = gBlendDuration;
}

//--------------------------------------------------------------------------
// 黒からフェードイン
//--------------------------------------------------------------------------
void Blend_SetDarkenTarget0(s32 frames)
{
	gBlendBrighten = 0;
	gBlendLayers = 0x3e;
	gBlendStartLevel = gBlendTargetLevel;
	gBlendTargetLevel = 0;
	gBlendDuration = frames;
	gBlendFramesLeft = gBlendDuration;
}

//--------------------------------------------------------------------------
// 白へフェードアウト
//--------------------------------------------------------------------------
void Blend_SetBrightenTarget16(s32 frames)
{
	gBlendBrighten = 1;
	gBlendLayers = 0x3e;
	gBlendStartLevel = gBlendTargetLevel;
	gBlendTargetLevel = 16;
	gBlendDuration = frames;
	gBlendFramesLeft = gBlendDuration;
}

//--------------------------------------------------------------------------
// 白からフェードイン
//--------------------------------------------------------------------------
void Blend_SetBrightenTarget0(s32 frames)
{
	gBlendBrighten = 1;
	gBlendLayers = 0x3e;
	gBlendStartLevel = gBlendTargetLevel;
	gBlendTargetLevel = 0;
	gBlendDuration = frames;
	gBlendFramesLeft = gBlendDuration;
}

//--------------------------------------------------------------------------
// フェードを指定する
// bright : 0 黒へ / 1 白へ
// layer  : BLDCNT の対象レイヤー（下位 6 ビット）
// start  : はじめの明るさ（16 より大きければ今の目標の明るさから）
// target : 目標の明るさ
// frames : かけるフレーム数
//--------------------------------------------------------------------------
void Blend_ConfigureTransition(s8 bright, s32 layer, u32 start, s32 target, s32 frames)
{
	gBlendBrighten = bright;
	gBlendLayers = layer & 0x3f;
	if (start > 16U)
	{
		gBlendStartLevel = gBlendTargetLevel;
	}
	else
	{
		gBlendStartLevel = start;
	}
	gBlendTargetLevel = target;
	gBlendFramesLeft = (gBlendDuration = frames);
}

//--------------------------------------------------------------------------
// フェードが終わるまで待つ
//--------------------------------------------------------------------------
void Blend_WaitForTransition(void)
{
	if (gBlendDuration != 0)
	{
		do
		{
			WaitFrames(1);
		} while (gBlendDuration != 0);
	}
}
