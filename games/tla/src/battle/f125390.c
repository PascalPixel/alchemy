#include "types.h"
#include "battle.h"
#include "battle_side.h"
#include "battle_system.h"

struct PlacementList {
  struct DjinnRecoveryEntry entries[72];
  s32 count;
};

struct PlacementTable {
  u8 padding[8];
  struct PlacementList list;
};

s32 Djinn_IsActiveFar(s32 unitId, s32 element, s32 djinni);
s32 Djinn_IsReadyFar(s32 unitId, s32 element, s32 djinni);
u32 *DjinnRecovery_AddStandbyFar(u32 unitId, u32 element, u32 djinni);
void Djinn_DeactivateFar(s32 unitId, s32 element, s32 djinni);
extern char MsgDjinnDrained;
extern char MsgAllDjinnDrained;
extern char MsgDjinnOnStandby;

void BattleEvent_Push(u32 opcode, u32 operand);

void Battle_ApplyActionExtras(s32 unitIdArg, s32 all, s32 announce) {
  s32 unitId;
  u8 elements[10];
  u8 indices[10];
  s32 shown;
  s32 count;
  s32 active;
  s32 element;
  s32 djinni;
  s32 i;
  s32 owner;
  s32 recoveryTurn;
  struct PlacementList *list;
  struct PlacementList *pending;

  shown = 0;
  /* FAKEMATCH(dead): the parameter overwritten after its copy, a store that
     does nothing; as the copy alone, 2 of 305 instructions differ in tla-en. */
  unitId = unitIdArg;
  unitIdArg = 0;
again:
  {
    count = 0;
    active = 0;
    for (element = 0; element <= 3; element++) {
      for (djinni = 0; djinni <= 19; djinni++) {
        if (Djinn_IsActiveFar(unitId, element, djinni)) {
          elements[count] = element;
          indices[count] = djinni;
          count++;
          active = 1;
          if (count == 10)
            goto picked;
        }
      }
    }
    if (count == 0) {
      for (element = 0; element <= 3; element++) {
        for (djinni = 0; djinni <= 19; djinni++) {
          if (Djinn_IsReadyFar(unitId, element, djinni)) {
            elements[count] = element;
            indices[count] = djinni;
            count++;
            if (count == 10)
              goto picked;
          }
        }
      }
    }
  picked:
    if (count == 0)
      return;

    count = ((u32)Battle_RandomFar() * count) >> 16;
    element = elements[count];
    djinni = indices[count];
    if (active) {
      DjinnRecovery_AddStandbyFar(unitId, element, djinni);
      Djinn_DeactivateFar(unitId, element, djinni);
      if (announce) {
        BattleEvent_Push(0, unitId);
        BattleEvent_Push(3, element * 20 + djinni + 300);
        BattleEvent_Push(4, (u32)&MsgDjinnOnStandby);
        return;
      }
    } else if (announce) {
      return;
    }

    list = &Djinn_GetRecoveryTableFar(unitId < 0 || unitId > 7)->list;
    for (i = 0; i < list->count; i++) {
      if (list->entries[i].element == element &&
          list->entries[i].index == djinni && list->entries[i].turns == -1)
        list->entries[i].turns = -2;
    }

    pending = &Djinn_GetRecoveryTableFar(unitId < 0 || unitId > 7)->list;
    for (;;) {
      owner = -1;
      for (i = 0; i < pending->count; i++) {
        if (pending->entries[i].turns == -2) {
          owner = pending->entries[i].unitId;
          break;
        }
      }
      if (owner == -1)
        break;
      recoveryTurn = -1;
      for (i = 0; i < pending->count; i++) {
        if (pending->entries[i].unitId == owner &&
            pending->entries[i].turns > recoveryTurn)
          recoveryTurn = pending->entries[i].turns;
      }
      recoveryTurn++;
      if (recoveryTurn <= 1)
        recoveryTurn = 2;
      for (i = 0; i < pending->count; i++) {
        if (pending->entries[i].unitId == owner &&
            pending->entries[i].turns == -2) {
          pending->entries[i].turns = recoveryTurn;
          recoveryTurn++;
        }
      }
    }

    if (!shown) {
      shown = 1;
      BattleEvent_Push(0, unitId);
      if (!all) {
        s32 message;

        BattleEvent_Push(3, element * 20 + djinni + 300);
        message = (u32)&MsgDjinnDrained;
        if (unitId < 0 || unitId > 7)
          message++;
        BattleEvent_Push(4, message);
      } else {
        BattleEvent_Push(4, (u32)&MsgAllDjinnDrained);
      }
    }
    if (all)
      goto again;
  }
}
