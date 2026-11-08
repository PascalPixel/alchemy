#include "types.h"
#include "battle.h"
#include "battle_unit.h"
#include "battle_work.h"
#include "battle_side.h"
#include "range_falloff.h"
#include "battle_event.h"
#include "battle_status_icon.h"
#include "motobj.h"
#include "owner_value.h"
#include "ram_buffer.h"
#include "edition.h"

// The Japanese text holds these messages in the same order two places later,
// but its labels for several of them belong to their neighbours, so the
// Japanese release reaches each of those by its place beside a message whose
// label fits its text.
#if LANGUAGE == LANGUAGE_JAPANESE
// ねむりからさめた! ("wakes from slumber"), three before MsgResNormal.
#define MSG_WAKES ((s32)&MsgResNormal - 3)
// すばやさが もどった! ("Agility returns to normal"), after MsgDefNormal.
#define MSG_AGI_NORMAL ((s32)&MsgDefNormal + 1)
// われにかえった! ("regains consciousness"), two after MsgDefNormal.
#define MSG_CONSCIOUS ((s32)&MsgDefNormal + 2)
// まぼろしを うちやぶった! ("sees clearly once again"), three before
// MsgConscious2.
#define MSG_CURE_DELUSION ((s32)&MsgConscious2 - 3)
// エナジーふうじがとけた! ("Psynergy seal is gone"), two before MsgConscious2.
#define MSG_CURE_SEAL ((s32)&MsgConscious2 - 2)
// しびれがきえた! ("no longer stunned"), the message before MsgConscious2.
#define MSG_CURE_STUN ((s32)&MsgConscious2 - 1)
// リフレクトのこうかがきえた! ("Reflect no longer affects"), after
// MsgRefrainEnd.
#define MSG_REFLECT_END ((s32)&MsgRefrainEnd + 1)
#else
#define MSG_WAKES ((s32)&MsgWakes)
#define MSG_AGI_NORMAL ((s32)&MsgAgiNormal)
#define MSG_CONSCIOUS ((s32)&MsgConscious)
#define MSG_CURE_DELUSION ((s32)&MsgCureDelusion)
#define MSG_CURE_SEAL ((s32)&MsgCureSeal)
#define MSG_CURE_STUN ((s32)&MsgCureStun)
#define MSG_REFLECT_END ((s32)&MsgReflectEnd)
#endif

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
u32 BattlePlayback_Reset(void);
s32 BattlePlayback_SchedulePhase(s32 frames);
void BattlePlayback_WaitForReady(void);
void Actor_SetMode(struct MotionObject *object, s32 mode);
void Func_08127308(s32 unitId, s32 element, s32 mode, s32 value);
void Func_0812628c(s32 mode, s32 background, s32 value);
void Sound_PlayFar(s32 cue);
void BattleMessage_WaitForAdvance(void);
void BattleObject_UpdatePose(s32 unitId);
s32 sub_08124af8(s32 id);

#define ANNOUNCE_END(test, message)                                            \
  if (test(unitId) != 0) {                                                     \
    BattleUnit_BuildStatusFlags(unitId, BattleObject_Get(unitId));             \
    MessageArgument_PushFar(unitId, 1);                                        \
    Message_ShowAndWaitCoreFar(message);                                       \
    BattleMessage_WaitForAdvance();                                            \
  }

#define ANNOUNCE_END_POSE(test, message)                                       \
  if (test(unitId) != 0) {                                                     \
    BattleUnit_BuildStatusFlags(unitId, BattleObject_Get(unitId));             \
    MessageArgument_PushFar(unitId, 1);                                        \
    BattleObject_UpdatePose(unitId);                                           \
    Message_ShowAndWaitCoreFar(message);                                       \
    BattleMessage_WaitForAdvance();                                            \
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
        if (timedEntry->turns > 0 &&
            BattleObject_Get(timedEntry->unitId) != 0 &&
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
            BattlePlayback_Reset();
            BattlePlayback_SchedulePhase(30);
            BattleEvent_Push(0, unitId);
            BattleEvent_Push(3, expiredEntry->element * 20 +
                                    expiredEntry->index + 300);
            BattleEvent_Push(14, 175);
            BattleEvent_Push(10, 0);
            BattleEvent_Push(4, (u32)&MsgDjinnIsSet);
            BattleEvent_Push(11, unitId);
            Sound_PlayFar(212);
            if (BattleParty_IsUnitListed(unitId)) {
              Actor_SetMode(BattleObject_Get(unitId)->object, 3);
              Actor_SetAnimationSpeedFar(
                  (struct ActorView *)BattleObject_Get(unitId)->object, 32);
            }
            element = expiredEntry->element;
            Djinn_ActivateFar(unitId, element, expiredEntry->index);
            DjinnRecovery_RemoveFar(unitId, expiredEntry->element,
                                    expiredEntry->index);
            Unit_CalculateFar(unitId);
            if (BattleParty_IsUnitListed(unitId))
              Func_08127308(unitId, element, 3, 0);
            BattlePlayback_WaitForReady();
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
            BattleMessage_WaitForAdvance();
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
            BattleMessage_WaitForAdvance();
          }
        }
        ANNOUNCE_END(BattleUnit_CountDownAgilityModifier, MSG_AGI_NORMAL);
        ANNOUNCE_END(BattleUnit_CountDownAttackModifier, (s32)&MsgAtkNormal);
        ANNOUNCE_END(BattleUnit_CountDownDefenseModifier, (s32)&MsgDefNormal);
        ANNOUNCE_END(BattleUnit_CountDownResistanceModifier,
                     (s32)&MsgResNormal);
        ANNOUNCE_END(BattleUnit_CountDownDelusion, MSG_CURE_DELUSION);
        ANNOUNCE_END(BattleUnit_CountDownConfusion, MSG_CONSCIOUS);
        ANNOUNCE_END(BattleUnit_CountDownCharm, (s32)&MsgConscious2);
        ANNOUNCE_END_POSE(BattleUnit_CountDownStun, MSG_CURE_STUN);
        ANNOUNCE_END_POSE(BattleUnit_CountDownSleep, MSG_WAKES);
        ANNOUNCE_END(Battle_AdvanceCounterAndCheckChance, MSG_CURE_SEAL);
        ANNOUNCE_END(sub_08124af8, (s32)&MsgRefrainEnd);
        ANNOUNCE_END(BattleUnit_CountDownReflect, MSG_REFLECT_END);
      }
    }
  }
}
