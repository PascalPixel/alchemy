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
#include "battle_side.h"
#include "battle_system.h"
#include "types.h"
#include "battle_type.h"

s32 BattleParty_ResetActiveRuntimeFields(void) {
  u16 owners[10];
  s32 count;
  s32 i;

  count = BattleParty_PrepareActiveOwners(owners);
  count += BattleParty_PrepareReserveOwners(owners + count);

  i = 0;
  if (i < count) {
    struct BattleUnit *unit;
    u8 *cursor;
    s32 remaining;

    do {
      unit = Unit_Get(owners[i]);
      remaining = 3;
      cursor = (u8 *)&unit->elementLevels[3];

      do {
        remaining--;
        *cursor-- = 0;
      } while (remaining >= 0);

      unit->attackTurns = 0;
      unit->attackStage = 0;
      unit->defenseTurns = 0;
      unit->defenseStage = 0;
      unit->resistanceTurns = 0;
      unit->resistanceStage = 0;
      unit->delusion = 0;
      unit->confusion = 0;
      unit->charm = 0;
      unit->stun = 0;
      unit->sleep = 0;
      unit->psySeal = 0;
      unit->refrain = 0;
      unit->reflect = 0;
      unit->grimReaper = 0;
      unit->equipBonus[0] = 0;
      unit->equipBonus[1] = 0;
      unit->ready = 0;
      unit->immobile = 0;
      unit->agilityTurns = 0;
      unit->agilityStage = 0;
      unit->challenge = 0;

      BattleUnit_Recalculate(owners[i]);
      i++;
    } while (i < count);
  }
}
