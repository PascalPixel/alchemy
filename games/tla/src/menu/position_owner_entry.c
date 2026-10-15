//--------------------------------------------------------------------------
// position_owner_entry.c
// アイテムメニュー：持ち物のアイコンを格子の位置に置く
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
#include "m7_interfaces.h"

//--------------------------------------------------------------------------
// i 番目（15 より大きければ 0）を、横 cols 個の格子（1 つ 24x16 ドット）の
// 位置に置く
// x0, y0 : 格子の左上
//--------------------------------------------------------------------------
void ItemMenu_PosOwner(struct RenderOutput **slot, s32 i, s32 x0, s32 y0,
                       s32 cols) {
  struct RenderOutput *obj;

  if (i > 15)
    i = 0;
  obj = *slot;
  obj->y = i / cols * 16 + y0;
  obj->x = i % cols * 24 + x0;
  UiIcon_PrepareObject(obj);
}
