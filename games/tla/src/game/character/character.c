#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "owner_state.h"
#include "party_state.h"

struct TradeOfferState *Djinn_GetRecoveryTable(s32 which) {
  if (which != 0) {
    return UnitGet(0x83);
  }
  return (struct TradeOfferState *)&gPartyState.unk000[0xc];
}

u32 Party_GetAverageLevel(void) {
  s32 count;
  s32 total;
  s32 i;

  total = 0;
  count = Party_CountActiveOwners();
  if (count == 0) {
    return 0;
  }
  for (i = 0; i < count; i++) {
    total += ((struct BattleUnit *)UnitGet(gPartyState.activeOwners[i]))->level;
  }
  total = total / count;
  return total;
}

const u8 *Enemy_GetDefinition(s32 selector) {
  u32 recordIndex;

  recordIndex = selector - 8;
  if (recordIndex > 999U)
    recordIndex = 0;
  return &gEnemy_DefinitionTable[recordIndex * 76];
}
