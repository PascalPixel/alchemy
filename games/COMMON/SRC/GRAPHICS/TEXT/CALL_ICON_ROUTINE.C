//--------------------------------------------------------------------------
// CALL_ICON_ROUTINE.C
// メニュー：えらんだキャラクターのグラフィックを読みなおす
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
// キャラクターのグラフィックをスロット 0 に読む
// menu : 呼び出し元のメニュー（使わない）
// unit : キャラクター番号
//--------------------------------------------------------------------------
void PsynergyMenu_CallIconRoutineWithValue(s32 menu, s32 unit)
{
	Ui_LoadCharacterEntryForSlotFar(0, unit, 0);
}
