// Draft, differ 2 of 149: ticks one side's Djinn recovery at the round's
// end, ☀️'s BattlePlacement_UpdateTimedEntries (games/tbs/src/battle/
// battle.c) with ⚓️'s presentation: an owner must be in the battle; each
// Djinni that comes back plays its event (its icon, sound 175, the flash and
// "... is set to ...!") before it is Set. Returns 1 when a count fell, 2 when
// one was Set, else 0. Left: instruction order only, one pair at the first
// loop's end: the increment's movs r2, #1 one slot ahead of the count's
// reload here, after it natively. Tried: for and while loops with one entry
// pointer (50, 10); ☀️'s guarded do-while with a pointer per loop gives 2.
#include "types.h"
#include "battle.h"
#include "battle_unit.h"
#include "battle_session.h"
#include "motobj.h"

struct TlaDjinnRecoveryEntry {
  u8 element;
  u8 index;
  u8 unitId;
  s8 turns;
};

struct TlaDjinnRecoveryList {
  struct TlaDjinnRecoveryEntry entries[72];
  s32 count;
};

struct TlaDjinnRecoveryTable {
  u8 unk00[8];
  struct TlaDjinnRecoveryList list;
};

extern u8 Msg03319[];

struct BattleUnit *UnitGet(s32 unitId);
u32 BattleParty_IsUnitListed(u32 id);
void BattleEventRuntime_Reset(void);
s32 BattleEventRuntime_SchedulePhase(s32 frames);
void BattleEventRuntime_WaitForReady(void);
void BattleEvPush(u32 opcode, u32 operand);
void Object_SetMode(struct MotionObject *object, s32 mode);
void Func_08127308(s32 unitId, s32 element, s32 mode, s32 value);

s32 Func_08124cc4(s32 side) {
  struct TlaDjinnRecoveryList *list;
  struct TlaDjinnRecoveryEntry *timedEntry;
  struct TlaDjinnRecoveryEntry *entry;
  s32 result = 0;
  s32 i;

  list =
      &((struct TlaDjinnRecoveryTable *)Djinn_GetRecoveryTableFar(side))->list;
  i = 0;
  if (i < list->count) {
    timedEntry = list->entries;
    do {
      if (timedEntry->turns > 0 &&
          BattleParty_IsUnitListed(timedEntry->unitId) &&
          UnitGet(timedEntry->unitId)->hp != 0) {
        timedEntry->turns--;
        result = 1;
      }
      i++;
      timedEntry++;
    } while (i < list->count);
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
          BattleEvPush(4, (u32)Msg03319);
          BattleEvPush(11, id);
          if (BattleParty_IsUnitListed(id)) {
            Object_SetMode(BattleObjGet(id)->object, 3);
            ObjectDispatch_ApplyValueToChildrenFar(
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
