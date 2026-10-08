//--------------------------------------------------------------------------
// select_entry_20_to_21.c
// メニュー：コマンドのアイコン 0x20・0x21 の 2 つからえらばせる
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

// 版ごとの名前のウィンドウの幅
#if !EDITION_INTERNATIONAL
#define PAIR_WIDTH 7
#elif defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT) || defined(TLA_EDITION_FR) \
	|| defined(TLA_EDITION_IT)
#define PAIR_WIDTH 10
#else
#define PAIR_WIDTH 9
#endif
s32 Menu_CenterResourceEntries(s32, s32, s32);

//--------------------------------------------------------------------------
// アイコン 0x20・0x21 を画面の中央に並べてえらぶ（行 17）
// sel : はじめにえらんでいる位置。戻り値はえらんだ位置
//--------------------------------------------------------------------------
s32 Menu_SelectEntry20To21(s32 sel)
{
	s32 ret;

	AffineEffect_InitializeWork();
	Menu_AppendResourceEntry(0x20);
	Menu_AppendResourceEntry(0x21);
	Menu_CenterResourceEntries(17, PAIR_WIDTH, 0);
	ret = Menu_RunResourceSelectionLoop(sel);
	Menu_EndResourceSelection();
	return ret;
}
