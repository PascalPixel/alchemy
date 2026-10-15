//--------------------------------------------------------------------------
// call_icon_routine.c
// メニュー：えらんだキャラクターのグラフィックを読みなおす
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
// キャラクターのグラフィックをスロット 0 に読む
// menu : 呼び出し元のメニュー（使わない）
// unit : キャラクター番号
//--------------------------------------------------------------------------
void PsynergyMenu_CallIconRoutineWithValue(s32 menu, s32 unit) {
  Ui_LoadCharacterEntryForSlotFar(0, unit, 0);
}
