//--------------------------------------------------------------------------
// get_table_word_zero.c
// ウィンドウ：固定のタイルの表の先頭を返す
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

//--------------------------------------------------------------------------
// Ui_FixedTileBlocks[0] を返す（0 以外の no も 0 として読む）
//--------------------------------------------------------------------------
s32 Ui_GetTableWordZero(s32 no)
{
	if (no != 0)
		no = 0;
	return Ui_FixedTileBlocks[no];
}
