#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "battle_party.h"
#include "battle_unit.h"
#include "owner_state.h"

#define Unit_CalculateFar ((void (*)(u16))Unit_CalculateFar)

void BattleUnit_ResetGuardLevels(void) {
  u16 ids[14];
  s32 count;
  s32 index;

  count = BattleParty_ListActorIds(BATTLE_SIDE_BOTH, (u16 *)ids);
  for (index = 0; index < count; index++) {
    struct BattleUnit *actor;

    actor = Unit_Get(ids[index]);
    actor->guardLevel = 0;
    Unit_CalculateFar(ids[index]);
  }
}
