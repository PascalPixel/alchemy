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
#include "owner_state.h"
#include "battle_party.h"
#include "b5_context.h"
#include "motobj.h"

struct PlacementList {
  struct DjinnRecoveryEntry entries[72];
  s32 count;
};

struct PlacementTable {
  u8 padding[8];
  struct PlacementList list;
};

extern char MsgHpRecover;
extern char MsgHpFull;

u32 BattleEventRuntime_Reset(void);

s32 sub_08124af8(s32 id) {
  struct BattleUnit *unit = UnitGet(id);
  u8 *count = &unit->refrain;

  if (*count != 0) {
    s32 max;
    s32 before;
    s32 hp;
    s32 gain;

    BattleEventRuntime_Reset();
    hp = unit->hp;
    max = unit->maxHp;
    before = hp;
    hp += max * (*count + 1) / 10;
    if (hp > max)
      hp = max;
    gain = hp - before;
    if (gain != 0) {
      BattleEvPush(0, id);
      if (hp == unit->maxHp) {
        BattleEvPush(4, (u32)&MsgHpFull);
      } else {
        {
          u32 opcode = 1;

          /* FAKEMATCH: the barrier builds the opcode before the gain is copied
           * to its argument register, as the native does; plain, the copy comes
           * first (one reordered pair, differ 2). */
          asm volatile("" : "+l"(opcode));
          BattleEvPush(opcode, gain);
        }
        BattleEvPush(4, (u32)&MsgHpRecover);
      }
      unit->hp = hp;
      UnitRatioFar(id);
    }
    BattleEv_DispatchQueued();
    if (--unit->refrain == 0)
      return 1;
  }
  return 0;
}

s32 BattleUnit_TickReflect(s32 id) {
  u8 *value = (u8 *)&((struct BattleUnit *)UnitGet(id))->reflect;
  if (*value != 0) {
    (*value)--;
    if (*value == 0) {
      return 1;
    }
  }
  return 0;
}

s32 BattleUnit_TickAgilityModifier(s32 id) {
  struct BattleUnit *unit = UnitGet(id);
  u8 *value = &unit->agilityTurns;
  if (*value != 0) {
    (*value)--;
    if (*value == 0) {
      unit->agilityStage = 0;
      return 1;
    }
  }
  return 0;
}

s32 DjinnRecovery_Tick(void) {
  struct PlacementList *list;
  struct DjinnRecoveryEntry *timedEntry;
  struct DjinnRecoveryEntry *expiredEntry;
  s32 index;
  s32 removed;
  s32 initialCount;

  list = &Djinn_GetRecoveryTableFar(0)->list;
  initialCount = list->count;
  index = 0;
  removed = 0;
  if (index < initialCount) {
    timedEntry = list->entries;
    do {
      if (timedEntry->turns > 0 &&
          ((struct BattleUnit *)UnitGet(timedEntry->unitId))->hp != 0) {
        timedEntry->turns--;
      }
      index++;
      timedEntry++;
    } while (index < list->count);
  }
  index = 0;
  if (index < list->count) {
    expiredEntry = list->entries;
    do {
      if (expiredEntry->turns == 0) {
        u8 id = expiredEntry->unitId;

        Djinn_ActivateFar(id, expiredEntry->element, expiredEntry->index);
        DjinnRecovery_RemoveFar(id, expiredEntry->element, expiredEntry->index);
        UnitCalcFar(id);
        removed = 1;
      } else {
        expiredEntry++;
        index++;
      }
    } while (index < list->count);
  }
  return removed;
}

s32 DjinnRecovery_TickTwenty(void) {
  s32 turns;

  turns = 19;
  do {
    turns--;
    DjinnRecovery_Tick();
  } while (turns >= 0);
  return 0;
}

void Func_08124cc0(void) {}

extern char MsgDjinnIsSet;

void BattleEvPush(u32 opcode, u32 operand);
u32 BattleParty_IsUnitListed(u32 id);
s32 BattleEventRuntime_SchedulePhase(s32 frames);
void BattleEventRuntime_WaitForReady(void);
void Func_08127308(s32 unitId, s32 element, s32 mode, s32 value);

/// Ticks one side's Djinn recovery at the round's end: an owner must be in the
/// battle; each Djinni that comes back plays its event (its icon, sound 175,
/// the flash and its message) before it is Set. The message is
/// "... is set to ...!" Returns 1 when a count fell, 2 when one was Set, else
/// 0.
// ☀️'s DjinnRecovery_Tick with ⚓️'s presentation.
s32 Func_08124cc4(s32 side) {
  struct PlacementList *list;
  struct DjinnRecoveryEntry *timedEntry;
  struct DjinnRecoveryEntry *entry;
  s32 result = 0;
  s32 i;
  s32 count;

  list = &Djinn_GetRecoveryTableFar(side)->list;
  i = 0;
  if (i < list->count) {
    timedEntry = list->entries;
    do {
      if (timedEntry->turns > 0 &&
          BattleParty_IsUnitListed(timedEntry->unitId) &&
          ((struct BattleUnit *)UnitGet(timedEntry->unitId))->hp != 0) {
        timedEntry->turns--;
        result = 1;
      }
      count = list->count;
      /* FAKEMATCH(one-pass): the count is read before i steps; as
       * i++; timedEntry++; } while (i < list->count);, 2 of 149 instructions
       * differ in tla-en. */
      do {
      } while (0);
      i++;
      timedEntry++;
    } while (i < count);
  }
  i = 0;
  if (i < list->count) {
    entry = list->entries;
    do {
      if (entry->turns == 0) {
        u8 id = entry->unitId;

        if (BattleParty_IsUnitListed(id)) {
          u8 element;

          result = 2;
          BattleEventRuntime_Reset();
          BattleEventRuntime_SchedulePhase(30);
          BattleEvPush(0, id);
          BattleEvPush(3, entry->element * 20 + entry->index + 300);
          BattleEvPush(14, 175);
          BattleEvPush(10, 0);
          BattleEvPush(4, (u32)&MsgDjinnIsSet);
          BattleEvPush(11, id);
          if (BattleParty_IsUnitListed(id)) {
            Object_SetMode((struct ObjectRuntime *)BattleObjGet(id)->object, 3);
            ObjectDispatch_SetAnimationSpeedFar(
                (struct DispatchObject *)BattleObjGet(id)->object, 32);
          }
          element = entry->element;
          Djinn_ActivateFar(id, element, entry->index);
          DjinnRecovery_RemoveFar(id, entry->element, entry->index);
          UnitCalcFar(id);
          if (BattleParty_IsUnitListed(id))
            Func_08127308(id, element, 3, 0);
          BattleEventRuntime_WaitForReady();
        }
      } else {
        entry++;
        i++;
      }
    } while (i < list->count);
  }
  return result;
}
