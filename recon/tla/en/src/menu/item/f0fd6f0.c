/// @file
/// Lists a unit's Psynergy for a menu.

#include "types.h"

struct OwnerActionSlot {
  u16 encodedAction;
  u16 unk02;
};

struct BattleUnit {
  u8 unk00[0x58];
  struct OwnerActionSlot actionSlots[32];
};

struct BattleAct {
  u8 targetMode;
  u8 flags;
  u8 unk02[4];
  u8 fieldEffect;
};

struct BattleAct *BattleAction_Get(s32 actionId);

/// Lists a unit's Psynergy into actions (32 entries, cleared first) and returns
/// how many: mode 1 those with a field effect, any other mode those first and
/// then, unless mode is 2, the rest.
s32 Func_080fd6f0(struct BattleUnit *unit, u16 *actions, s32 mode) {
  s32 i;
  s32 count;
  struct BattleAct *ability;
  s32 offset;

  count = 0;
  for (i = 0; i < 32; i++)
    actions[i] = 0;

  if (mode == 1) {
    for (i = 0; i < 32; i++) {
      if (unit->actionSlots[i].encodedAction != 0) {
        if (BattleAction_Get(unit->actionSlots[i].encodedAction & 0x3fff)
                ->fieldEffect != 0) {
          actions[count] = unit->actionSlots[i].encodedAction;
          count++;
        }
      }
    }
  } else {
    for (i = 0; i < 32; i++) {
      if (unit->actionSlots[i].encodedAction != 0) {
        ability = BattleAction_Get(unit->actionSlots[i].encodedAction & 0x3fff);
        if (ability->fieldEffect != 0 || (ability->flags & 0x40) != 0) {
          actions[count] = unit->actionSlots[i].encodedAction;
          count++;
        }
      }
    }
    if (mode != 2) {
      for (i = 0; i < 32; i++) {
        if (unit->actionSlots[i].encodedAction != 0) {
          ability =
              BattleAction_Get(unit->actionSlots[i].encodedAction & 0x3fff);
          if (ability->fieldEffect == 0 && (ability->flags & 0x40) == 0) {
            actions[count] = unit->actionSlots[i].encodedAction;
            count++;
          }
        }
      }
    }
  }
  return count;
}
