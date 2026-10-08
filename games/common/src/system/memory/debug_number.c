//--------------------------------------------------------------------------
// debug_number.c
// デバッグ：数を右そろえでデバッグ文字にする
//--------------------------------------------------------------------------

#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "types.h"

// どちらも数を作業域に右づめで作り、その終わりから keta 文字を書く

//--------------------------------------------------------------------------
// 16 進で keta けた（1〜8、範囲外なら 8）
//--------------------------------------------------------------------------
void Text_DrawHexRightAligned(u32 value, s32 keta)
{
	s32 n;

	n = keta;
	if ((n - 1) > 7U)
	{
		n = 8;
	}
	Text_FormatHexToWork(value);
	Runtime_WriteDebugTextTiles((const u8 *)(((u32)&gNumberTextHexEnd) - n));
}

//--------------------------------------------------------------------------
// 符号つきの 10 進で keta けた（1〜10、範囲外なら 10）
//--------------------------------------------------------------------------
void Text_DrawSignedDecimalRightAligned(s32 value, s32 keta)
{
	s32 n;

	n = keta;
	if ((n - 1) > 9U)
	{
		n = 10;
	}
	Text_FormatSignedDecimalToWork(value);
	Runtime_WriteDebugTextTiles((const u8 *)(((u32)&gNumberTextDecimalEnd) - n));
}
