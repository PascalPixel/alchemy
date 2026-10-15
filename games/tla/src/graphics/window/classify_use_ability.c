//--------------------------------------------------------------------------
// classify_use_ability.c
// アイテム：召喚えらびのメニューと、戦闘でアイテムを使えるかの判定
//--------------------------------------------------------------------------

#include "battle_system.h"
#include "item.h"
#include "types.h"
#include "scene.h"
#include "edition.h"
#include "graphics.h"
#include "uiwindow.h"

#define BATTLE_OK 0x80 // flags：戦闘で使える

//--------------------------------------------------------------------------
// 戦闘でアイテムを使えるか
// 戻り値 : 0 使える / 1 使えない / 2 戦闘では使えない
// unit   : キャラクター番号
// item   : 持ち物の値
//--------------------------------------------------------------------------
s32 Item_ClassifyUseAbility(s32 unit, s32 item) {
  s32 ret;
  struct ItemDefinition *def;

  if (item == 0) {
    return 1;
  }
#if GAME_TLA
  def = Item_Get(item);
#endif
  ret = 1;
  if (def->useType == 3) // エナジーを授ける装備
  {
    return ret;
  }
  if (def->actionId == 0) {
    return ret;
  }
  if ((def->type != 0) && (Item_CanOwnerEquip(unit, item) == 0)) {
    return ret;
  }
  if ((BATTLE_OK & BattleActGet(def->actionId)->flags) == 0) {
    return 2;
  }
  return 0;
}
