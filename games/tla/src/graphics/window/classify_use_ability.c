//--------------------------------------------------------------------------
// classify_use_ability.c
// Items: summon-selection menu and battle item-use eligibility.
//--------------------------------------------------------------------------

#include "battle_system.h"
#include "item.h"
#include "types.h"
#include "scene.h"
#include "edition.h"
#include "graphics.h"
#include "uiwindow.h"

#define BATTLE_OK 0x80 // flags: usable in battle.

//--------------------------------------------------------------------------
// Determine whether an item can be used in battle.
// Returns: 0 usable / 1 unusable / 2 unavailable in battle.
// unit: character index.
// item: carried item value.
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
  if (def->useType == 3) // Equipment that grants Psynergy.
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
