//--------------------------------------------------------------------------
// copy_list_order.c
// メニュー：並べかえの順番の表を写す
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
// mode 0〜2 の順番の表（それ以外は標準の表）を order に写す
// 表は 0xff で終わる。写すのは 0xff を含めて 32 バイトまで
//--------------------------------------------------------------------------
void Menu_CopyListOrder(s32 mode, u8 *order) {
  u8 *src;
  s32 n;

  src = gMenu_ListOrderDefault;
  switch (mode) {
  case 0:
    src = gMenu_ListOrderMode0;
    break;
  case 1:
    src = gMenu_ListOrderMode1;
    break;
  case 2:
    src = gMenu_ListOrderMode2;
    break;
  }
  *order = *src;
  n = 0;
  if (*order != 0xff) {
    do {
      if (++n > 31)
        break;
      src++;
      order++;
      *order = *src;
    } while (*order != 0xff);
  }
}
