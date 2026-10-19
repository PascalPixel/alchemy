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
#include "ram_buffer.h"

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

/// The sanctum's view of the shop work (heap slot 220).
struct ShopRuntime {
  u8 unk000[0x4ca];
  s16 partyMemberIds[8];
  u8 unk4da[0x2a];
  s8 partyMemberCount;
  u8 unk505[2];
  s8 partyAction;
};

/// Counts the party members who need the selected sanctum service, as ☀️'s
/// Shop_CountUnits.
s32 Shop_CountUnits(void) {
  struct ShopRuntime *shop;
  s32 eligibleCount;
  s32 serviceKind;
  s32 memberIndex;

  shop = Ram_HeapSlots->menuRuntime;
  /* FAKEMATCH(barrier): the work loaded before the service kind's offset is
   * built in r1, as native; the clobber keeps that build below the load. As
   * plain C, 2 of 56 instructions differ in tla-en. */
  asm("" : "+l"(shop) : : "r1");
  eligibleCount = 0;
  serviceKind = shop->partyAction;
  for (memberIndex = 0; memberIndex < shop->partyMemberCount; memberIndex++) {
    if (Shop_CanServe(shop->partyMemberIds[memberIndex], serviceKind) != 0)
      eligibleCount++;
  }
  return eligibleCount;
}
