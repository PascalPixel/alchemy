//--------------------------------------------------------------------------
// draw_four_numbers_in_row.c
// セーブ：記録のジンの数を属性ごとに横に並べて書く
//--------------------------------------------------------------------------

#include "text_render_runtime.h"
#include "types.h"
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
#include "text/msg_ids.h"

// セーブの記録（SaveSummary）のうち、ここで読むところ
struct NumberRow
{
	u8 unk00[0x28];
	s8 values[4]; // 属性ごとのジンの数
};

//--------------------------------------------------------------------------
// 4 つの数を 2 けたで、24 ドットおきに y 16 に書く
// win : 書き込み先（0 なら何もしない）
//--------------------------------------------------------------------------
void UiText_DrawFourNumbersInRow(struct RenderInput *win, struct NumberRow *row)
{
	s32 i;

	if (win != 0)
	{
		for (i = 0; i < 4; i++)
			UiText_DrawNumberAtOffset(row->values[i], 2, win, i * 24, 16);
	}
}

//--------------------------------------------------------------------------

// draw_character_summary.c

// セーブ：記録の名前・レベル・クラス・コイン・プレイ時間を書く

//--------------------------------------------------------------------------

#define UiText_DrawStringAtOffset ((void (*)(void *, s32, s32, s32))UiText_DrawStringAtOffset)
#define UiText_DrawNumberAtOffset ((void (*)(s32, s32, s32, s32, s32))UiText_DrawNumberAtOffset)
#define UiText_DrawCharacterAtOffset ((void (*)(s32, s32, s32, s32))UiText_DrawCharacterAtOffset)

void RenderOutput_PrepareForRedraw(void);
void UiWindow_DrawDividerLine(s32, s32, s32, s32, s32);
void UiText_DrawStringInWindow(s32, s32, s32, s32);
void UiText_DrawNumberInWindow(s32, s32, s32, s32, s32);

// 版ごとの「Lv」と「コイン」の x（ドット）
#if defined(TBS_EDITION_ES) || defined(TLA_EDITION_ES)
#define SUMMARY_LEVEL_X 64
#define SUMMARY_COINS_X 56
#elif defined(TBS_EDITION_FR) || defined(TLA_EDITION_FR)
#define SUMMARY_LEVEL_X 59
#define SUMMARY_COINS_X 56
#elif defined(TLA_EDITION_DE) || defined(TLA_EDITION_IT)
#define SUMMARY_COINS_X 56
#else
#define SUMMARY_COINS_X 48
#endif

//--------------------------------------------------------------------------
// win : 書き込み先（0 なら何もしない）
// rec : セーブの記録（SaveSummary）
//       +16 名前、+28 レベル、+29 クラス、+32 プレイ時間、+36 コイン
//--------------------------------------------------------------------------
void StatusMenu_DrawCharacterSummary(s32 win, u8 *rec)
{
	u32 buf[4]; // プレイ時間の文字列
	s32 y;

	if (win != 0)
	{
		RenderOutput_PrepareForRedraw();
		UiWindow_DrawDividerLine(win, 0, 4, 13, 4);
		UiText_DrawStringAtOffset(rec + 16, win, 0, 0);
		y = 0;
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TLA_EDITION_ES) \
	|| defined(TLA_EDITION_FR)
		UiText_DrawStringAtOffset(gStatusMenu_LevelLetterString, win, SUMMARY_LEVEL_X, 0);
#else
		UiText_DrawStringInWindow((s32)gStatusMenu_LevelLetterString, win, 72, 0);
#endif
		UiText_DrawNumberAtOffset(rec[28], 2, win, 80, y);
		UiText_DrawCharacterAtOffset(rec[29] + (s32)MsgClassName, win, 0, 16);
#if !EDITION_INTERNATIONAL
		y = 32;
		UiText_DrawNumberInWindow(*(s32 *)(rec + 36), 6, win, 24, y);
		UiText_DrawCharacterAtOffset((s32)MsgCoins, win, 72, 32);
		UiText_DrawCharacterAtOffset((s32)MsgStatusLabel, win, 0, 48);
		UiText_DrawStringInWindow(
			(s32)Text_FormatPlayTime(*(s32 *)(rec + 32), (u8 *)buf), win, 48, 48);
#else
		UiText_DrawCharacterAtOffset((s32)MsgStatusLabel, win, 0, 32);
		UiText_DrawStringInWindow(
			(s32)Text_FormatPlayTime(*(s32 *)(rec + 32), (u8 *)buf), win, 48, 40);
		y = 48;
		UiText_DrawNumberInWindow(*(s32 *)(rec + 36), 6, win, 0, y);
#if defined(TBS_EDITION_FR) || defined(TLA_EDITION_FR)
		// フランス語版は 1 枚以下なら単数形
		if (*(u32 *)(rec + 36) > 1)
			UiText_DrawCharacterAtOffset((s32)MsgCoins, win, SUMMARY_COINS_X, 48);
		else
			UiText_DrawCharacterAtOffset((s32)MsgPiece, win, SUMMARY_COINS_X, 48);
#else
		UiText_DrawCharacterAtOffset((s32)MsgCoins, win, SUMMARY_COINS_X, 48);
#endif
#endif
	}
}
