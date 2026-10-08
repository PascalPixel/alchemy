//--------------------------------------------------------------------------
// GET_TABLE_WORD_ZERO.C
// ウィンドウ：固定のタイルの表の先頭を返す
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

//--------------------------------------------------------------------------
// Ui_FixedTileBlocks[0] を返す（0 以外の no も 0 として読む）
//--------------------------------------------------------------------------
s32 Ui_GetTableWordZero(s32 no)
{
	if (no != 0)
		no = 0;
	return Ui_FixedTileBlocks[no];
}
