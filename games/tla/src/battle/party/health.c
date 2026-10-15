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
#include "owner_value.h"
#include "battle_unit.h"
#include "owner_state.h"
#include "party_state.h"
#include "fixed_math.h"
#include "battle_calc.h"
#include "field_event.h"

void BattleParty_ApplyDrain(s32 amount) {
  s32 targetCount = Party_CountActiveOwnersFar();
  s32 i;

  for (i = 0; i < targetCount; i++)
    UnitPpAddFar(gPartyState.activeOwners[i], amount);
}

void BattleParty_ApplyHealthDelta(s32 amount, s32 scaled) {
  s32 count;
  s32 i;
  s32 value;
  struct BattleUnit *unit;
  if (amount < 0) {
    BattleFx_ApplyColorToSourceBuffer(0x1ff, 0);
    BattleFx_StartBufferInterpolation(4);
    if (amount < -10)
      Audio_PlayCue(134);
    else
      Audio_PlayCue(133);
  } else {
    Audio_PlayCue(126);
  }
  count = Party_CountActiveOwnersFar();
  for (i = 0; i < count; i++) {
    unit = UnitGet(gPartyState.activeOwners[i]);
    if (!scaled) {
      value = amount;
    } else {
      value = unit->maxHp * amount / 100;
      if (value == 0) {
        value = amount;
        if (value < 0)
          value = -value;
      }
    }
    Unit_AddHpFar(gPartyState.activeOwners[i], value);
  }
}

s32 BattleParty_ApplyStatusDamage(void) {
  s32 result = 0;
  s32 count = Party_CountActiveOwnersFar();

  if (result < count) {
    s32 offset = 134;
    u8 *entry;
    s32 remaining;

    offset <<= 2;
    entry = (u8 *)&gPartyState + offset;
    remaining = count;

    do {
      struct BattleUnit *object = UnitGet(*entry);
      s32 amount;

      switch (object->poison) {
      case 1:
        amount = -((object->maxHp + 10) / 20);
        if (amount == 0)
          amount = -1;
        if (result <= 0)
          result = 1;
        break;
      case 2:
        amount = -((object->maxHp + 5) / 10);
        if (amount == 0)
          amount = -1;
        if (result <= 1)
          result = 2;
        break;
      default:
        amount = 0;
        break;
      }

      remaining--;
      Unit_AddHpFar(*entry, amount);
      entry++;
    } while (remaining != 0);
  }

  if (result != 0) {
    BattleFx_ApplyColorToSourceBuffer(0x1ff, 0);
    BattleFx_StartBufferInterpolation(4);
    Audio_PlayCue(133);
  }

  return result;
}
