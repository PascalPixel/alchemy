// First draft of Func_080fd6f0: lists a unit's Psynergy into actions (32
// entries, cleared first) and returns how many; the twin of ☀️'s
// PsynergyMenu_CollectActions (games/tbs/src/menu/menu.c). mode 1 lists those
// whose byte 6 (☀️'s type0c, the field effect) is set; any other mode those
// with it set or usable in the field (flags & 0x40) first, then, unless mode
// is 2, the rest. ⚓️ writes the passes as separate loops, the slots counted
// down. Differ 48 of 136 (draft 134). Left: the clear keeps its byte offset
// as the counter (cmp r3, #0), where the native walks a pointer down to
// actions (cmp r3, ip), the form ☀️'s twin reaches only with a doubled
// FAKEMATCH store, not used here (an index from 31 gives 73, a pointer 84
// with an unsigned compare); after it, register choice (actions and the 0x3fff
// mask swap r0, r2 and r3 at each loop head). Clearing count first is
// measured: the native builds &actions[count] from a live zero.
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

struct BattleAct *BattleActGet(s32 actionId);

s32 Func_080fd6f0(struct BattleUnit *unit, u16 *actions, s32 mode) {
  s32 i;
  s32 count;
  struct BattleAct *ability;
  s32 offset;

  count = 0;
  for (offset = 62; offset >= 0; offset -= 2)
    *(u16 *)((u8 *)actions + offset) = 0;

  if (mode == 1) {
    for (i = 0; i < 32; i++) {
      if (unit->actionSlots[i].encodedAction != 0) {
        if (BattleActGet(unit->actionSlots[i].encodedAction & 0x3fff)
                ->fieldEffect != 0) {
          actions[count] = unit->actionSlots[i].encodedAction;
          count++;
        }
      }
    }
  } else {
    for (i = 0; i < 32; i++) {
      if (unit->actionSlots[i].encodedAction != 0) {
        ability = BattleActGet(unit->actionSlots[i].encodedAction & 0x3fff);
        if (ability->fieldEffect != 0 || (ability->flags & 0x40) != 0) {
          actions[count] = unit->actionSlots[i].encodedAction;
          count++;
        }
      }
    }
    if (mode != 2) {
      for (i = 0; i < 32; i++) {
        if (unit->actionSlots[i].encodedAction != 0) {
          ability = BattleActGet(unit->actionSlots[i].encodedAction & 0x3fff);
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
