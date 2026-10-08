//--------------------------------------------------------------------------
// draw_value_pair_rows.c
// 表示：HP と EP を「今 / 最大」の 2 行で書く
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
#include "edition.h"
#include "types.h"
#include "battle_unit.h"

#define UiText_DrawStringInWindowFar ((void (*)(s32, s32, s32, s32))UiText_DrawStringInWindowFar)

void UiText_DrawStringAtOffsetFar(s32 image, s32 layer, s32 x, s32 y);
void UiText_DrawNumberAtOffsetFar(s32 value, s32 digits, s32 layer, s32 x, s32 y);
void UiText_DrawNumberInWindowFar(s32 value, s32 digits, s32 layer, s32 x, s32 y);
s32	 UiWork_SetParamNibbleFar(s32 color);

//--------------------------------------------------------------------------
// ウィンドウの y 40 に HP、y 48 に EP を書く
// HP は最大の 1/4 未満なら色 4、0 なら色 2 で書く
// unit : キャラクター
// win  : 書き込み先のウィンドウ
//--------------------------------------------------------------------------
void Ui_DrawValuePairRows(struct BtlUnit *unit, s32 win)
{
	s16 val;

#if EDITION_INTERNATIONAL
	UiText_DrawStringAtOffsetFar((s32)Ui_HpString, win, 0, 40);
	UiText_DrawStringInWindowFar((s32)Ui_SlashString, win, 48, 40);
	val = unit->max_hp;
	UiText_DrawNumberRightAlignedFar(val, win, 88, 40);
	val = unit->hp;
	if (val < ((u16)unit->max_hp << 16) >> 18)
	{
		UiWork_SetParamNibbleFar(4);
	}
	if (val == 0)
	{
		UiWork_SetParamNibbleFar(2);
	}
	UiText_DrawNumberRightAlignedFar(val, win, 48, 40);
	UiWork_SetParamNibbleFar(15);
	UiText_DrawStringAtOffsetFar((s32)Ui_PpString, win, 0, 48);
	UiText_DrawStringInWindowFar((s32)Ui_SlashString, win, 48, 48);
	val = unit->ep;
	UiText_DrawNumberRightAlignedFar(val, win, 48, 48);
	val = unit->max_ep;
	UiText_DrawNumberRightAlignedFar(val, win, 88, 48);
#else
	UiText_DrawStringInWindowFar((s32)Ui_HpRowString, win, 0, 40);
	val = unit->max_hp;
	UiText_DrawNumberInWindowFar(val, 4, win, 56, 40);
	val = unit->hp;
	if (val < ((u16)unit->max_hp << 16) >> 18)
	{
		UiWork_SetParamNibbleFar(4);
	}
	if (val == 0)
	{
		UiWork_SetParamNibbleFar(2);
	}
	UiText_DrawNumberAtOffsetFar(val, 4, win, 16, 40);
	UiWork_SetParamNibbleFar(15);
	UiText_DrawStringInWindowFar((s32)Ui_EpRowString, win, 0, 48);
	val = unit->ep;
	UiText_DrawNumberAtOffsetFar(val, 4, win, 16, 48);
	val = unit->max_ep;
	UiText_DrawNumberInWindowFar(val, 4, win, 56, 48);
#endif
}
