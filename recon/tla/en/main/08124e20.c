// Under -mthumb-slow-loads (26 October 2026): differ 31 of 544 in tla-en.
// Tried, one edit at a time: an empty asm barrier after each statement, each
// adjacent pair of statements swapped; none reaches 0.
// Draft, differ 35 of 544: ☀️'s Battle_ProcessTurnEnd
// (games/tbs/src/battle/battle/ turn_end.c), ported: advances Djinn recovery on
// each battle side (each Djinni that comes back plays its event and is Set),
// then for each unit in the round's side order applies HP and PP regeneration
// and announces the stat changes and conditions that wore off. ⚓️ adds the
// reserve members to the party's list, clears each unit's byte 0x143,
// regenerates ten times as fast for enemies 91 to 97 and 218, and announces the
// end of the effect sub_08124af8 counts down. The ten-fold enemies are a switch (as a
// range test the compiler folds it into one unsigned compare: 169). Left: the
// unit list's address is built as movs r3, #20; add r3, sp natively (two bytes
// longer, which moves the later branch offsets), and five loads or constants
// one slot from the native. The names of the messages that have none in the
// catalogs are ☀️'s.
#include "types.h"
#include "battle.h"
#include "battle_unit.h"
#include "battle_work.h"
#include "battle_party.h"
#include "battle_calc.h"
#include "battle_event.h"
#include "battle_status_icon.h"
#include "motobj.h"
#include "owner_value.h"
#include "ram_buffer.h"

struct TurnEndList {
  struct DjinnRecoveryEntry entries[72];
  s32 count;
};

struct TurnEndTable {
  u8 padding[8];
  struct TurnEndList list;
};

struct BattleGroupOrder {
  s32 group[2];
};

extern const struct BattleGroupOrder Data_081297c4;

extern char MsgDjinnIsSet;
extern char MsgHpFull;
extern char MsgHpRecover;
extern char MsgPpFull;
extern char MsgPpRecover;
extern char MsgAgiNormal;
extern char MsgAtkNormal;
extern char MsgDefNormal;
extern char MsgResNormal;
extern char MsgCureDelusion;
extern char MsgConscious;
extern char MsgConscious2;
extern char MsgCureStun;
extern char MsgWakes;
extern char MsgCureSeal;
extern char MsgRefrainEnd;
extern char MsgReflectEnd;

struct BattleUnit *Unit_Get(s32 unitId);
u32 BattleParty_IsUnitListed(u32 id);
s32 BattleParty_ListActorIds(s32 side, u16 *ids);
u32 BattleEventRuntime_Reset(void);
s32 BattleEventRuntime_SchedulePhase(s32 frames);
void BattleEventRuntime_WaitForReady(void);
void Object_SetMode(struct MotionObject *object, s32 mode);
void Func_08127308(s32 unitId, s32 element, s32 mode, s32 value);
void Func_0812628c(s32 mode, s32 background, s32 value);
void Sound_PlayFar(s32 cue);
void BattlePresentation_WaitForAdvance(void);
void BattlePres_SetActorModeAndAction(s32 unitId);
s32 sub_08124af8(s32 id);

#define ANNOUNCE_END(test, message)                                            \
  if (test(unitId) != 0) {                                                     \
    BattleUnit_BuildStatusFlags(unitId, BattleObject_Get(unitId));                 \
    MessageArgument_PushFar(unitId, 1);                                            \
    Message_ShowAndWaitCoreFar((s32)&message);                           \
    BattlePresentation_WaitForAdvance();                                       \
  }

#define ANNOUNCE_END_POSE(test, message)                                       \
  if (test(unitId) != 0) {                                                     \
    BattleUnit_BuildStatusFlags(unitId, BattleObject_Get(unitId));                 \
    MessageArgument_PushFar(unitId, 1);                                            \
    BattlePres_SetActorModeAndAction(unitId);                                  \
    Message_ShowAndWaitCoreFar((s32)&message);                           \
    BattlePresentation_WaitForAdvance();                                       \
  }

void Func_08124e20(void) {
  struct BattleWork *battle;
  s32 side;
  s32 sideCount;
  s32 count;
  s32 recoveryIndex;
  struct TurnEndList *list;
  struct DjinnRecoveryEntry *timedEntry;
  struct DjinnRecoveryEntry *expiredEntry;

  battle = Ram_HeapSlots->battleWork;
  sideCount = (battle->twoSided != 0) + 1;
  for (side = 0; side < sideCount; side++) {
    list = &((struct TurnEndTable *)Djinn_GetRecoveryTableFar(side))->list;
    recoveryIndex = 0;
    timedEntry = list->entries;
    if (recoveryIndex < list->count) {
      do {
        if (timedEntry->turns > 0 && BattleObject_Get(timedEntry->unitId) != 0 &&
            Unit_Get(timedEntry->unitId)->hp != 0) {
          timedEntry->turns--;
        }
        recoveryIndex++;
        timedEntry++;
      } while (recoveryIndex < list->count);
    }
    recoveryIndex = 0;
    if (recoveryIndex < list->count) {
      expiredEntry = list->entries;
      do {
        if (expiredEntry->turns == 0) {
          u8 unitId = expiredEntry->unitId;
          u8 element;

          if (BattleObject_Get(unitId) != 0) {
            BattleEventRuntime_Reset();
            BattleEventRuntime_SchedulePhase(30);
            BattleEvent_Push(0, unitId);
            BattleEvent_Push(3, expiredEntry->element * 20 + expiredEntry->index +
                                300);
            BattleEvent_Push(14, 175);
            BattleEvent_Push(10, 0);
            BattleEvent_Push(4, (u32)&MsgDjinnIsSet);
            BattleEvent_Push(11, unitId);
            Sound_PlayFar(212);
            if (BattleParty_IsUnitListed(unitId)) {
              Object_SetMode(BattleObject_Get(unitId)->object, 3);
              ObjectDispatch_SetAnimationSpeedFar(
                  (struct DispatchObject *)BattleObject_Get(unitId)->object, 32);
            }
            element = expiredEntry->element;
            Djinn_ActivateFar(unitId, element, expiredEntry->index);
            DjinnRecovery_RemoveFar(unitId, expiredEntry->element,
                                    expiredEntry->index);
            Unit_CalculateFar(unitId);
            if (BattleParty_IsUnitListed(unitId))
              Func_08127308(unitId, element, 3, 0);
            BattleEventRuntime_WaitForReady();
          }
        } else {
          expiredEntry++;
          recoveryIndex++;
        }
      } while (recoveryIndex < list->count);
    }
  }
  Func_0812628c(2, *(u16 *)&Ram_HeapSlots->battleWork->unk670[0], 0);
  {
    s32 *groupList;
    struct BattleGroupOrder order = Data_081297c4;
    u16 unitIds[16];
    s32 group;
    s32 unitIndex;

    if (battle->twoSided != 0 && battle->linkSide != 0) {
      order.group[0] = 2;
      order.group[1] = 1;
    }
    groupList = order.group;
    for (group = 0; group <= 1; group++) {
      count = BattleParty_ListActorIds(groupList[group], unitIds);
      if (groupList[group] == 1)
        count += BattleParty_PrepareReserveOwners(unitIds + count);
      for (unitIndex = 0; unitIndex < count; unitIndex++) {
        s32 hpAmount;
        s32 ppAmount;
        s32 unitId = unitIds[unitIndex];
        struct BattleUnit *unit = Unit_Get(unitId);

        unit->equipBonus[1] = 0;
        if (unit->ready != 0) {
          unit->ready--;
        }
        if (unit->hp != 0) {
          if (unit->hpRegen != 0 && unit->hp != unit->maxHp) {
            hpAmount = unit->hpRegen;
            switch (unit->enemyId) {
            case 91:
            case 92:
            case 93:
            case 94:
            case 95:
            case 96:
            case 97:
            case 218:
              hpAmount *= 10;
              break;
            }
            if (unit->hp + hpAmount > unit->maxHp) {
              hpAmount = unit->maxHp - unit->hp;
            }
            Unit_AddHpFar(unitId, hpAmount);
            MessageArgument_PushFar(unitId, 1);
            MessageArgument_PushFar(hpAmount, 5);
            if (unit->hp == unit->maxHp) {
              Message_ShowAndWaitCoreFar((s32)&MsgHpFull);
            } else {
              Message_ShowAndWaitCoreFar((s32)&MsgHpRecover);
            }
            Sound_PlayFar(175);
            BattlePresentation_WaitForAdvance();
          }
          if (unit->ppRegen != 0 && unit->pp != unit->maxPp) {
            ppAmount = unit->ppRegen;
            if (unit->pp + ppAmount > unit->maxPp) {
              ppAmount = unit->maxPp - unit->pp;
            }
            Unit_AddPpFar(unitId, ppAmount);
            MessageArgument_PushFar(unitId, 1);
            MessageArgument_PushFar(ppAmount, 5);
            if (unit->pp == unit->maxPp) {
              Message_ShowAndWaitCoreFar((s32)&MsgPpFull);
            } else {
              Message_ShowAndWaitCoreFar((s32)&MsgPpRecover);
            }
            Sound_PlayFar(175);
            BattlePresentation_WaitForAdvance();
          }
        }
        ANNOUNCE_END(BattleUnit_CountDownAgilityModifier, MsgAgiNormal);
        ANNOUNCE_END(BattleUnit_CountDownAttackModifier, MsgAtkNormal);
        ANNOUNCE_END(BattleUnit_CountDownDefenseModifier, MsgDefNormal);
        ANNOUNCE_END(BattleUnit_CountDownResistanceModifier, MsgResNormal);
        ANNOUNCE_END(BattleUnit_CountDownDelusion, MsgCureDelusion);
        ANNOUNCE_END(BattleUnit_CountDownConfusion, MsgConscious);
        ANNOUNCE_END(BattleUnit_CountDownCharm, MsgConscious2);
        ANNOUNCE_END_POSE(BattleUnit_CountDownStun, MsgCureStun);
        ANNOUNCE_END_POSE(BattleUnit_CountDownSleep, MsgWakes);
        ANNOUNCE_END(Battle_AdvanceCounterAndCheckChance, MsgCureSeal);
        ANNOUNCE_END(sub_08124af8, MsgRefrainEnd);
        ANNOUNCE_END(BattleUnit_CountDownReflect, MsgReflectEnd);
      }
    }
  }
}
