//--------------------------------------------------------------------------
// CONFIRM_SELECTION.C
// メニュー：コマンドのアイコンを並べてえらばせる（はい・いいえ など）
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
#include "EDITION.H"
#include "TYPES.H"
#include "text/MSG_IDS.H"

s32 Menu_CenterResourceEntries(s32, s32, s32);

// アイコンは ResourceId_CommandIcons の番号。
// えらんでいるコマンドの名前をウィンドウに出す

//--------------------------------------------------------------------------
// アイコン 0x11〜0x14 を画面の中央に並べてえらぶ（行 17）
// sel : はじめにえらんでいる位置。戻り値はえらんだ位置
//--------------------------------------------------------------------------
s32 Menu_SelectEntry11To14(s32 sel)
{
	s32 ret;

	AffineEffect_InitializeWork();
	Menu_AppendResourceEntry(0x11);
	Menu_AppendResourceEntry(0x12);
	Menu_AppendResourceEntry(0x13);
	Menu_AppendResourceEntry(0x14);
#if defined(TBS_EDITION_DE) || defined(TLA_EDITION_DE)
	Menu_CenterResourceEntries(17, 8, 0); // 名前のウィンドウの幅
#elif EDITION_INTERNATIONAL
	Menu_CenterResourceEntries(17, 7, 0);
#else
	Menu_CenterResourceEntries(17, 6, 0);
#endif
	ret = Menu_RunResourceSelectionLoop(sel);
	Menu_EndResourceSelection();
	return ret;
}

//--------------------------------------------------------------------------
// アイコン 0x19〜0x1c を画面の中央に並べてえらぶ（行 17）
//--------------------------------------------------------------------------
s32 Menu_SelectEntry19To1c(s32 sel)
{
	s32 ret;

	AffineEffect_InitializeWork();
	Menu_AppendResourceEntry(0x19);
	Menu_AppendResourceEntry(0x1A);
	Menu_AppendResourceEntry(0x1B);
	Menu_AppendResourceEntry(0x1C);
#if EDITION_INTERNATIONAL
	Menu_CenterResourceEntries(17, 10, 0);
#else
	Menu_CenterResourceEntries(17, 8, 0);
#endif
	ret = Menu_RunResourceSelectionLoop(sel);
	Menu_EndResourceSelection();
	return ret;
}

//--------------------------------------------------------------------------
// はい・いいえ（アイコン 5・6）をえらぶ。キャンセルは 1（いいえ）
// low  : 0 なら行 0、0 でなければ行 17 に出す
// mes  : 名前のメッセージの先頭（0 ならコマンド名）
// w    : 名前のウィンドウの幅（0 なら 3）
// sel  : はじめにえらんでいる位置
//--------------------------------------------------------------------------
s32 Menu_RunConfirmSelection(s32 low, s32 mes, s32 w, s32 sel)
{
	s32 row;

	row = 0;

	AffineEffect_InitializeWork();

	if (w == 0)
	{
		w = 3;
	}
	if (low != 0)
	{
		row = 17;
	}

	Menu_AppendResourceEntry(5);
	Menu_AppendResourceEntry(6);
	Menu_CenterResourceEntries(row, w, mes);

	sel = Menu_RunResourceSelectionLoop(sel);
	Menu_EndResourceSelection();
	if (sel == -1)
	{
		sel = 1;
	}
	return sel;
}

//--------------------------------------------------------------------------
// はい・いいえを (x, y) に出してえらぶ。キャンセルは 1（いいえ）
//--------------------------------------------------------------------------
s32 Menu_RunConfirmSelectionAt(s32 x, s32 y, s32 sel)
{
	s32 mes = (s32)MsgCommandYes;

	AffineEffect_InitializeWork();
	Menu_AppendResourceEntry(5);
	Menu_AppendResourceEntry(6);
	Menu_LayoutResourceEntries(x, y, 3, mes);
	sel = Menu_RunResourceSelectionLoop(sel);
	Menu_EndResourceSelection();
	if (sel == -1)
	{
		sel = 1;
	}
	return sel;
}
