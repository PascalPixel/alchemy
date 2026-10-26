// Under -mthumb-slow-loads (26 October 2026): differ 2 of 305 in tla-en.
// Tried, one edit at a time: an empty asm barrier after each statement, each
// adjacent pair of statements swapped, a register tie on each local after
// each statement, each local pinned to each register, each call argument
// copied into its argument register (pinned) at the call or up to three
// statements before it, five minutes of permute; none reaches 0.
// Near miss, differ 2 of 305: drains one of a unit's Djinn at random, a Set one
// when it has any (Djinn_IsActiveFar), else one Djinn_IsReadyFar accepts: up
// to ten candidates are gathered and one picked. A Set Djinni goes on standby;
// with announce the event says so (MsgDjinnOnStandby) and stops. Otherwise
// the pick's standby entries are given their recovery turns, as ☀️'s
// BattleUnit_ProcessTurnEnd does, and the first drain is announced
// (MsgDjinnDrained, the next message for an enemy; MsgAllDjinnDrained with
// all, which repeats until none is left). The outer loop is a goto: as a
// do-while, loop.c hoists both arrays' addresses and the frame grows by 8.
// Left: the prologue's mov r9, r0 (unitId) one slot ahead of the stores of
// all and shown; sched2 takes the parameter copy first by RTL order, where
// the native has it after the shown store. Tried: a local copy of the
// parameter after shown = 0 (plain, pinned to r9, behind an empty volatile
// asm or an asm copy), u8, u16, s8 and s16 parameters, four minutes of permute.
// The three message names are not in the catalogs yet: English 3220, 3222 and
// 3223, the other editions to be checked.
#include "types.h"
#include "battle.h"
#include "battle_party.h"
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

void Battle_ApplyActionExtras(s32 unitId, s32 all, s32 announce) {
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
again: {
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
