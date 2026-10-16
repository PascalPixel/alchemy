//--------------------------------------------------------------------------
// debugnum.c
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

// どちらも数を作業域に右づめで作り、その終わりから digits 文字を書く

//--------------------------------------------------------------------------
// 16 進で digits けた（1〜8、範囲外なら 8）
//--------------------------------------------------------------------------
void DebugText_PrintHex(u32 value, s32 digits) {
  s32 n;

  n = digits;
  if ((n - 1) > 7U) {
    n = 8;
  }
  NumberText_FormatHex(value);
  DebugText_Print((const u8 *)(((u32)&gNumberTextHexEnd) - n));
}

//--------------------------------------------------------------------------
// 符号つきの 10 進で digits けた（1〜10、範囲外なら 10）
//--------------------------------------------------------------------------
void DebugText_PrintDecimal(s32 value, s32 digits) {
  s32 n;

  n = digits;
  if ((n - 1) > 9U) {
    n = 10;
  }
  NumberText_FormatDecimal(value);
  DebugText_Print((const u8 *)(((u32)&gNumberTextDecimalEnd) - n));
}
