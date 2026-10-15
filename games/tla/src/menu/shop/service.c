#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "owner_state.h"
#include "types.h"

s32 Shop_ServicePrice(s32 entry_id, s32 kind) {
  u8 value = ((struct BattleUnit *)UnitGet(entry_id))->level;
  s32 result = 0;

  if (kind == 0) {
    result = value * 20;
  } else if (kind == 1) {
    result = 10;
  } else if (kind == 2) {
    result = 50;
  } else if (kind == 3) {
    result = value * 10;
  }
  return result;
}

s32 Shop_CanServe(s32 entry_id, s32 kind) {
  struct BattleUnit *entry = UnitGet(entry_id);
  s32 result = 0;

  if ((kind == 0 && entry->hp <= 0) || (kind == 1 && entry->poison != 0) ||
      (kind == 2 && entry->haunt != 0) ||
      (kind == 3 && entry->restraint != 0)) {
    result = 1;
  }
  return result;
}
