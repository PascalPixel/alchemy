#include "types.h"
#include "battle_work.h"
#include "battle_unit.h"

s32 BattleParty_ListActorIds(s32 groups, u16 *dst) {
  struct BattleWork *order;
  s32 count;
  s32 i;

  order = Ram_HeapSlots->battleWork;
  count = 0;
  if (groups & 1) {
    for (i = 0; order->partyUnits[i] != 255; i++) {
      if (order->partyUnits[i] != 254) {
        if (dst != NULL)
          *dst++ = order->partyUnits[i];
        count++;
      }
    }
  }
  if (groups & 2) {
    for (i = 0; order->enemyIds[i] != 255; i++) {
      if (order->enemyIds[i] != 254) {
        if (dst != NULL)
          *dst++ = order->enemyIds[i];
        count++;
      }
    }
  }
  if (dst != NULL)
    *dst = 255;
  return count;
}

struct BattleUnit *UnitGet(s32 unitId);

/// As BattleParty_ListActorIds, listing only the actors whose HP is not 0.
s32 Func_0811a39c(s32 groups, u16 *dst) {
  struct BattleWork *order;
  s32 count;
  s32 i;

  order = Ram_HeapSlots->battleWork;
  count = 0;
  if (groups & 1) {
    for (i = 0; order->partyUnits[i] != 255; i++) {
      if (order->partyUnits[i] != 254 &&
          UnitGet(order->partyUnits[i])->hp != 0) {
        if (dst != NULL)
          *dst++ = order->partyUnits[i];
        count++;
      }
    }
  }
  if (groups & 2) {
    for (i = 0; order->enemyIds[i] != 255; i++) {
      if (order->enemyIds[i] != 254 && UnitGet(order->enemyIds[i])->hp != 0) {
        if (dst != NULL)
          *dst++ = order->enemyIds[i];
        count++;
      }
    }
  }
  if (dst != NULL)
    *dst = 255;
  return count;
}
