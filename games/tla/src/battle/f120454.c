/// @file
/// An ability applied to one target: damage, healing and its added effect.

#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "battle_side.h"
#include "iwram_call.h"
#include "battle_obj.h"
#include "range_falloff.h"
#include "battle_effect.h"
#include "battle_event.h"
#include "battle_message.h"
#include "battle_system.h"
#include "battle_summon.h"
#include "battle_type.h"
#include "battle_work.h"
#include "motobj.h"
#include "heap_release.h"
#include "battle_display.h"
#include "main_routine_work.h"
#include "ram_buffer.h"

#define ALLY 2 /* Ally. */

#define STEP_LIM(v)                                                            \
  {                                                                            \
    if ((v) < -4)                                                              \
      (v) = -4;                                                                \
    if ((v) > 4)                                                               \
      (v) = 4;                                                                 \
  }

#define ALLY_CHECK(id) ((id) >= 0 && (id) < 8)

#define ELEMENT_BONUS()                                                        \
  {                                                                            \
    if (element != 4)                                                          \
      bonus = power - target->element[element].resistance;                     \
  }

#define MESSAGE_PUSH(m) BattleEvent_Push(BATTLE_EVENT_MESSAGE, (u32)(m))

#define MESSAGE_SIDE(m, e)                                                     \
  {                                                                            \
    if (ALLY_CHECK(tid))                                                       \
      MESSAGE_PUSH(m);                                                         \
    else                                                                       \
      MESSAGE_PUSH(e);                                                         \
  }

#define GUARD_CUT()                                                            \
  {                                                                            \
    guard = target->guardLevel;                                                \
    if (guard != 0) {                                                          \
      if (guard == 1)                                                          \
        dmg /= 2;                                                              \
      else if (guard == 2)                                                     \
        dmg = dmg * 2 / 5;                                                     \
      else                                                                     \
        dmg /= 10;                                                             \
    }                                                                          \
  }

#define RES_STEP(d, v, message)                                                \
  {                                                                            \
    target->resistanceStage += (d);                                            \
    STEP_LIM(target->resistanceStage);                                         \
    BattleEvent_Push(BATTLE_EVENT_NUMBER, (v));                                \
    MESSAGE_PUSH(message);                                                     \
    target->resistanceTurns = 7;                                               \
  }

#define STAT_SET(f, message)                                                   \
  {                                                                            \
    MESSAGE_PUSH(message);                                                     \
    (f) = 7;                                                                   \
  }

/// Applies the acting unit's ability to one of its targets: damage or
/// healing by the ability kind, then its added effect, as The Broken Seal's
/// BattleAbility_ApplyToTarget does, with this game's differences.
s32 BattleTarget_Execute(struct BattlePlan *plan, s32 slot) {
  struct BattleAct *action;
  s32 abilityId;
  struct BattleUnit *actor;
  s32 aid;
  s32 element;
  s32 bonus;
  struct BattleWork *work;
  s32 dist;
  s32 half;
  s32 adj;
  s32 dealt;
  s32 crush;
  s32 hit;
  s32 crit;
  s32 skip;
  s32 type;
  s32 aff;
  struct BattleUnit *bak;
  s32 power;
  s32 hp;
  s32 kind;
  s32 scale;
  s32 pass;
  s16 *cmd;
  u8 tid;
  s8 *rm;
  struct BattleUnit *target;
  u32 n;
  s32 value;
  s32 dmg;
  s32 ap;
  s8 *am;
  s32 guard;
  s16 list[8];
  s32 cnt;
  s32 tmp;
  s32 size;
  bonus = 0;
  work = Ram_HeapSlots->battleWork;
  half = 0;
  dealt = 0;
  crush = 0;
  skip = 0;
  aff = 0;
  size = sizeof(struct BattleUnit);
  bak = Heap_Alloc(size);

  if (abilityId == 312 || abilityId == 316)
    aid = plan->actorId2;
  else
    aid = plan->actorId;
  tid = plan->targetIds[slot];
  abilityId = plan->abilityId;
  element = plan->element;
  adj = plan->targetMultiplier[slot];
  crit = plan->targetCritical[slot];

  action = BattleAction_Get(abilityId);
  actor = Unit_Get(aid);
  target = Unit_Get(tid);
  Iwram_CopyWords(bak, target, size);
  switch (target->enemyId) {
  case 221:
    if (plan->command == 1 && (u32)(aid ^ tid) >> 7 != 0) {
      BattleEvent_Push(BATTLE_EVENT_REFRESH_UNIT, tid);
      BattleEvent_Push(BATTLE_EVENT_UNIT_NAME, tid);
      MESSAGE_PUSH(MsgDissipated);
      goto done;
    }
    break;
  case 101:
  case 102:
  case 103:
    if (abilityId != 573 && work->unk868 != 0 && (u32)(aid ^ tid) >> 7 != 0) {
      BattleEvent_Push(BATTLE_EVENT_REFRESH_UNIT, tid);
      BattleEvent_Push(BATTLE_EVENT_UNIT_NAME, tid);
      MESSAGE_PUSH(MsgFieldProtects);
      goto done;
    }
    break;
  }
  if (target->guardLevel == 4 && (u32)(aid ^ tid) >> 7 != 0) {
    BattleEvent_Push(BATTLE_EVENT_REFRESH_UNIT, tid);
    BattleEvent_Push(BATTLE_EVENT_UNIT_NAME, tid);
    MESSAGE_PUSH(MsgNoEffect);
    goto done;
  }

  if (action->range != 255) {
    dist = plan->targetOffset[slot];
    if (dist < 0)
      dist = -dist;
  } else {
    dist = 0;
  }

  if (element != 4) {
    s32 i;

    value = target->baseElement[element].resistance;
    for (i = 0; i < 4 && value >= target->baseElement[i].resistance; i++)
      ;
    if (i == 4)
      aff = -1;

    for (i = 0; i < 4 && value <= target->baseElement[i].resistance; i++)
      ;
    if (i == 4)
      aff = 1;
  }

  if (plan->element >= 0 && plan->element <= 3) {
    cmd = &plan->command;
    if (*cmd != 2) {
      power = actor->element[plan->element].power;
      goto power_end;
    }
  } else
    cmd = &plan->command;
  power = 100;
power_end:

  if (plan->command == 5 && (plan->element >= 0 && plan->element <= 3) &&
      aff > 0) {
    s32 chance;

    chance = power - target->element[plan->element].resistance;
    chance += 30;
    chance *= 65536 / 100;
    if (chance > (Battle_RandomFar() & 0xffff))
      BattleEvent_Push(BATTLE_EVENT_DJINN_BONUS, 5);
  }

  if (abilityId == 686 || abilityId == 357)
    Battle_ApplyActionExtras(tid, 0, 0);
  if (abilityId == 720 || abilityId == 721)
    Battle_ApplyActionExtras(tid, 1, 0);
  if (abilityId == 728)
    Battle_ApplyActionExtras(tid, 1, 1);
  type = action->flags & 15;
  {
    s32 res;

    res = plan->targetHit[slot];
    if (res == -1)
      hit = BattleEffect_RollSuccessFar(aid, tid, element, action->effect,
                                        gHitTable[dist]);
    else
      hit = res;
  }
  if (action->effect == 50 || action->effect == 51 || action->effect == 86 ||
      action->effect == 87) {
    s32 cls;
    s32 ent;
    s32 chg;

    cls = actor->enemyId;
    chg = -1;
    ent = Enemy_GetFreeSlot();
    if (action->effect == 51) {
      cls = BattleFormation_GetRandomMember(work->enemyGroup);
    } else if (action->effect == 86) {
      if (actor->enemyId == 164)
        cls = (Battle_RandomFar() & 3) + 378;
      else
        cls = 81;
    } else if (action->effect == 87) {
      if (work->queueCount != 0) {
        cls = work->queuedSummons[work->queueHead];
        chg = work->queuedCharges[work->queueHead];
      } else
        hit = 0;
    }
    if (hit != 0 && Enemy_CheckRoom(cls) != 0 && ent >= 0) {
      if (chg == -1) {
        chg = BattleEnemy_ClaimNameLetter(cls, 1);
        if (chg & 0x8000)
          BattleEnemy_LetterFirstOfClass(cls);
      }
      BattleUnit_SetFar(ent, cls, chg & 0x7fff);
      if (action->effect == 87) {
        work->queueCount--;
        work->queueHead = (work->queueHead + 1) % 2;
      }
      if (actor->enemyId == 164)
        BattleParty_InsertUnitCentered(work->enemyIds, ent);
      else {
        s32 i;

        for (i = 0; i < 6; i++) {
          if (work->enemyIds[i] == 254) {
            work->enemyIds[i] = ent;
            break;
          }
          if (work->enemyIds[i] == 255) {
            work->enemyIds[i] = ent;
            work->enemyIds[i + 1] = 255;
            break;
          }
        }
      }
      Enemy_Update();
      {
        struct BattleObj *obj;

        obj = BattleObject_Get(ent);
        BattleObject_Make((struct BattleObj *)obj, ent,
                          obj->anchorX / 0x10000,
                          obj->anchorZ / 0x10000);
      }
      BattleObject_Put();
      {
        s32 num;

        num = BattleEnemy_List(list);
        for (cnt = 0; cnt < num; cnt++)
          ObjMotRst((u16)list[cnt]);
      }
      BattleEvent_Push(BATTLE_EVENT_UNIT_NAME, ent);
      if (action->effect == 87)
        MESSAGE_PUSH(MsgUpToFight);
      else if (abilityId != 503)
        MESSAGE_PUSH(MsgAppears);
      else
        MESSAGE_PUSH(MsgSplitOff);
    } else if (abilityId == 503) {
      MESSAGE_PUSH(MsgSplitFailed);
    } else {
      MESSAGE_PUSH(MsgNoOneCame);
    }
  }

  if (hit != 0) {
    if (action->effect == 53 || action->effect == 83) {
      hit = 0;
      for (n = 0; n < 20; n++) {
        if (work->actions[n].unitId == tid) {
          hit = 1;
          break;
        }
      }
    } else if (action->effect == 35) {
      half = 1;
    } else if (action->effect == 34) {
      crush = 1;
    } else if (action->effect == 27) {
      skip = 1;
    } else if (action->effect == 55) {
      if (actor->hp != 0)
        BattleEvent_Push(BATTLE_EVENT_REMOVE_UNIT, aid);
    } else if (action->effect == 32) {
      if (target->pp != 0)
        type = 10;
      else
        hit = 0;
    } else if (action->effect == 90) {
      half = 2;
    } else if (action->effect == 91) {
      half = 2;
    }
  }

  if ((action->effect != 75 || slot == 0) && skip == 0 &&
      (target->hp != 0 || Battle_IsReviveEffectFar(action->effect) != 0)) {
    s32 pp;
    s32 cur;

    switch (type) {
    case 3:
    case 4: {
      s32 def;

      if (target->guardLevel == 4)
        break;
      hp = target->hp;
      def = target->def;
      scale = def;
      if (half != 0) {
        if (half == 2)
          scale = 0;
        else
          scale = def / 2;
      }
      pass = 1;
      do {
        ELEMENT_BONUS();
        if (pass == 0)
          bonus = 0;
        ap = action->power;
        if (type == 4)
          dmg = Battle_CalculateAttackDamageFar(actor->atk, scale, 0, bonus) *
                ap / 10;
        else {
          s32 atk;

          atk = actor->atk;
          if (abilityId == 312 || abilityId == 316) {
            atk = ((struct BattleUnit *)Unit_Get(plan->actorId))->atk;
            if (abilityId == 312)
              atk += ((struct BattleUnit *)Unit_Get(plan->actorId2))->atk / 4;
          }
          dmg = Battle_CalculateAttackDamageFar(atk, scale, ap, bonus);
        }
        dmg *= adj;
        if (crit != 0) {
          if (crit == 1)
            dmg = dmg * 5 / 4;
          else
            dmg = dmg * 3 / 2;
          dmg += (target->level / 5) + 6;
          if (pass == 0) {
            BattleEvent_Push(BATTLE_EVENT_MARK, 0);
            {
              s32 message;

              message = (s32)MsgCritical;
              if (ALLY_CHECK(tid))
                message += 1;
              BattleEvent_Push(BATTLE_EVENT_MESSAGE_NO_WAIT, message);
            }
          }
        }
        dmg += Battle_RandomFar() & 3;
        GUARD_CUT();
        if (dmg <= 0)
          dmg = 1;
        if (crush != 0) {
          if (dmg < hp - 1) {
            dmg = hp - 1;
            if (dmg <= 0)
              dmg = 1;
          }
        }
        if (GameFlag_Test(366) != 0 && plan->command == 5 && hp <= dmg) {
          dmg = hp - 1;
        }
        pass++;
      } while (pass <= 1);
      hp -= dmg;
      BattleEvent_Push(BATTLE_EVENT_HIT, tid);
      BattleEvent_Push(BATTLE_EVENT_UNIT_NAME, tid);
      BattleEvent_Push(BATTLE_EVENT_NUMBER, dmg);
      {
        s32 message;

        if (ALLY_CHECK(tid))
          message = (s32)MsgDmgEmphP + aff;
        else
          message = (s32)MsgDmgEmphE + aff;
        BattleEvent_Push(BATTLE_EVENT_MESSAGE, message);
      }

      if (hp <= 0 && BattleUnit_KeepsOneHp(tid) != 0)
        hp = 1;
      if (hp <= 0) {
        hp = 0;
        BattleEvent_Push(BATTLE_EVENT_DEFEAT, tid);
        if (target->enemyId != 369 && target->enemyId != 373) {
          BattleEvent_Push(BATTLE_EVENT_UNIT_NAME, tid);
          MESSAGE_SIDE(MsgGoesDown, MsgFelled);
        }
      } else
        BattleEvent_Push(BATTLE_EVENT_REFRESH_UNIT, tid);
      dealt = target->hp - hp;
      target->hp = hp;
      Unit_UpdateRatiosFar(tid);
      break;
    }

    case 10: {
      if (target->guardLevel == 4)
        break;
      if (action->power == 0)
        break;
      pp = target->pp;
      ELEMENT_BONUS();
      ap = action->power;
      dmg = Battle_CalculatePsynergyPowerFar(ap, bonus, 256);
      dmg = dmg * gPpLossTable[dist] / 100;
      dmg *= adj;
      GUARD_CUT();
      if (action->effect == 32 && dmg > pp)
        dmg = pp;
      if (dmg > pp)
        dmg = pp;

      BattleEvent_Push(BATTLE_EVENT_HIT, tid);
      BattleEvent_Push(BATTLE_EVENT_NUMBER, dmg);
      BattleEvent_Push(BATTLE_EVENT_UNIT_NAME, tid);
      {
        s32 message;

        if (ALLY_CHECK(tid))
          message = (s32)MsgPpLossP;
        else
          message = (s32)MsgPpLossE;
        BattleEvent_Push(BATTLE_EVENT_MESSAGE, message);
        pp -= dmg;
      }
      if (pp <= 0)
        pp = 0;
      BattleEvent_Push(BATTLE_EVENT_REFRESH_UNIT, tid);
      dealt = target->pp - pp;
      target->pp = pp;
      Unit_UpdateRatiosFar(tid);
      break;
    }

    case 1: {
      if (action->power == 0)
        break;
      cur = target->hp;
      if (cur == 0)
        break;

      ap = action->power;
      dmg = Battle_CalculateRecoveryFar(ap, element == 4 ? 100 : power, 256);
      dmg = dmg * gHpRecoveryTable[dist] / 100;
      dmg *= adj;
      dmg += Battle_RandomFar() & 3;
      cur += dmg;
      if (cur > target->maxHp) {
        cur = target->maxHp;
        dmg = cur - target->hp;
      }
      BattleEvent_Push(BATTLE_EVENT_UNIT_NAME, tid);
      if (cur == target->maxHp)
        MESSAGE_PUSH(MsgHpFull);
      else {
        BattleEvent_Push(BATTLE_EVENT_NUMBER, dmg);
        MESSAGE_PUSH(MsgHpRecover);
      }
      dealt = target->hp - cur;
      target->hp = cur;
      Unit_UpdateRatiosFar(tid);
      break;
    }

    case -1: {
      if (target->guardLevel == 4)
        break;
      if (action->power == 0)
        break;
      pp = target->pp;
      ELEMENT_BONUS();
      ap = action->power;
      dmg = Battle_CalculatePsynergyPowerFar(ap, bonus, 256);
      dmg = dmg * gPpDmgTable[dist] / 100;
      dmg *= adj;
      GUARD_CUT();
      BattleEvent_Push(BATTLE_EVENT_HIT, tid);
      BattleEvent_Push(BATTLE_EVENT_NUMBER, dmg);
      BattleEvent_Push(BATTLE_EVENT_UNIT_NAME, tid);
      {
        s32 message;

        if (ALLY_CHECK(tid))
          message = (s32)MsgDmgP;
        else
          message = (s32)MsgDmgE;
        BattleEvent_Push(BATTLE_EVENT_MESSAGE, message);
        pp -= dmg;
      }
      if (pp <= 0)
        pp = 0;
      BattleEvent_Push(BATTLE_EVENT_REFRESH_UNIT, tid);
      target->pp = pp;
      Unit_UpdateRatiosFar(tid);
      break;
    }

    case 5:
    case 6:
    case 8: {
      s32 round;

      if (target->guardLevel == 4)
        break;
      if (action->power == 0)
        break;
      cur = target->hp;
      round = 1;
      do {
        ELEMENT_BONUS();
        if (round == 0)
          bonus = 0;
        ap = action->power;
        if (abilityId == 683 || abilityId == 673 || abilityId == 724 ||
            plan->command == 6 || plan->command == 10) {
          s32 rate;

          rate = 0;
          switch (abilityId) {
          default:
          case 393:
            break;
          case 406:
          case 388:
          case 396:
          case 380:
            rate = 3;
            break;
          case 409:
          case 399:
          case 391:
          case 383:
            rate = 12;
            break;
          case 401:
            rate = 12;
            break;
          case 683:
            rate = 35;
            break;
          case 407:
          case 400:
          case 397:
          case 389:
          case 384:
          case 381:
            rate = 6;
            break;
          case 398:
          case 390:
          case 408:
          case 382:
            rate = 9;
            break;
          case 392:
          case 410:
            rate = 9;
            break;
          case 402:
            rate = 7;
            break;
          case 411:
          case 403:
          case 385:
            rate = 15;
            break;
          case 394:
            rate = 21;
            break;
          case 412:
            rate = 24;
            break;
          case 673:
          case 386:
            rate = 30;
            break;
          case 724:
          case 404:
            rate = 40;
            break;
          }
          value = target->maxHp;
          if (value > 10000)
            value = 10000;
          ap += value * rate / 100;
        }
        dmg = Battle_CalculatePsynergyPowerFar(ap, bonus, 256);
        dmg *= adj;
        switch (type & 15) {
        case 5:
          dmg = gHpDmgTable5[dist] * dmg / 100;
          break;
        case 8:
          dmg = dmg * gHpDmgTable8[dist] / 100;
          break;
        case 6:
          dmg = dmg * gHpDmgTable6[dist] / 100;
          break;
        }
        dmg += Battle_RandomFar() & 3;
        GUARD_CUT();
        if ((gMainRoutineWork.battleRule24b == 6 ||
             (GameFlag_Test(366) != 0 && plan->command == 6)) &&
            cur > dmg) {
          dmg = cur;
        }
        round++;
      } while (round <= 1);
      BattleEvent_Push(BATTLE_EVENT_HIT, tid);
      BattleEvent_Push(BATTLE_EVENT_NUMBER, dmg);
      BattleEvent_Push(BATTLE_EVENT_UNIT_NAME, tid);
      {
        s32 message;

        if (ALLY_CHECK(tid))
          message = (s32)MsgDmgEmphP + aff;
        else
          message = (s32)MsgDmgEmphE + aff;
        BattleEvent_Push(BATTLE_EVENT_MESSAGE, message);
        cur -= dmg;
      }

      if (cur <= 0 && BattleUnit_KeepsOneHp(tid) != 0)
        cur = 1;
      if (gMainRoutineWork.battleRule24b == 6)
        cur = 0;
      if (cur <= 0) {
        BattleEvent_Push(BATTLE_EVENT_DEFEAT, tid);
        BattleEvent_Push(BATTLE_EVENT_UNIT_NAME, tid);
        {
          s32 message;

          cur = 0;
          if (ALLY_CHECK(tid))
            message = (s32)MsgGoesDown;
          else
            message = (s32)MsgFelled;
          BattleEvent_Push(BATTLE_EVENT_MESSAGE, message);
        }
      } else
        BattleEvent_Push(BATTLE_EVENT_REFRESH_UNIT, tid);
      dealt = target->hp - cur;
      target->hp = cur;
      Unit_UpdateRatiosFar(tid);
      break;
    }

    case 11: {
      if (action->power == 0)
        break;
      pp = target->pp;
      ap = action->power;
      dmg = Battle_CalculateRecoveryFar(ap, element == 4 ? 100 : power, 256);
      dmg = dmg * gPpRecoveryTable[dist] / 100;
      dmg *= adj;
      pp += dmg;
      if (pp > target->maxPp) {
        pp = target->maxPp;
        dmg = pp - target->pp;
      }
      BattleEvent_Push(BATTLE_EVENT_UNIT_NAME, tid);
      if (pp == target->maxPp)
        MESSAGE_PUSH(MsgPpFull);
      else {
        BattleEvent_Push(BATTLE_EVENT_NUMBER, dmg);
        MESSAGE_PUSH(MsgPpRecover);
      }
      target->pp = pp;
      Unit_UpdateRatiosFar(tid);
      break;
    }

    case 0:
    case 7:
    case 9:
      break;

    case 2:
      if (target->guardLevel == 4)
        break;
      if (hit != 0) {
        if (action->power == 0)
          break;
        cur = target->hp;
        ELEMENT_BONUS();
        ap = action->power;
        dmg = Battle_CalculatePsynergyPowerFar(ap, bonus, 256);
        dmg *= adj;
        dmg = dmg * gHpDmgTable[dist] / 100;
        GUARD_CUT();
        BattleEvent_Push(BATTLE_EVENT_HIT, tid);
        BattleEvent_Push(BATTLE_EVENT_NUMBER, dmg);
        BattleEvent_Push(BATTLE_EVENT_UNIT_NAME, tid);
        {
          s32 message;

          if (ALLY_CHECK(tid))
            message = (s32)MsgDmgP;
          else
            message = (s32)MsgDmgE;
          BattleEvent_Push(BATTLE_EVENT_MESSAGE, message);
          cur -= dmg;
        }

        if (cur <= 0 && BattleUnit_KeepsOneHp(tid) != 0)
          cur = 1;
        if (cur <= 0) {
          BattleEvent_Push(BATTLE_EVENT_DEFEAT, tid);
          cur = 0;
          if (target->enemyId != 369 && target->enemyId != 373) {
            BattleEvent_Push(BATTLE_EVENT_UNIT_NAME, tid);
            MESSAGE_SIDE(MsgGoesDown, MsgFelled);
          }
        } else
          BattleEvent_Push(BATTLE_EVENT_REFRESH_UNIT, tid);
        dealt = target->hp - cur;
        target->hp = cur;
        Unit_UpdateRatiosFar(tid);
        break;
      }
      BattleEvent_Push(BATTLE_EVENT_REFRESH_UNIT, tid);
      BattleEvent_Push(BATTLE_EVENT_UNIT_NAME, tid);
      MESSAGE_PUSH(MsgNoEffect);
      break;
    }
  }

  if (work->unk868 != 0 && abilityId == 573) {
    BattleEvent_Push(BATTLE_EVENT_UNIT_NAME, tid);
    MESSAGE_PUSH(MsgFieldBroken);
    work->unk868 = 0;
  }
  BattleEvent_Push(BATTLE_EVENT_UNIT_NAME, tid);
  if ((action->effect != 75 || slot != 0) &&
      (Battle_IsReviveEffectFar(action->effect) != 0 || target->hp != 0 ||
       BattleEffect_CheckDead(action->effect) != 0) &&
      hit != 0) {
    switch (action->effect) {

    case 64:
      if (target->delusion != 0) {
        target->delusion = 0;
        MESSAGE_PUSH(MsgCureDelusion);
        BattleEvent_Push(BATTLE_EVENT_UNIT_NAME, tid);
      }
      if (target->stun != 0) {
        target->stun = 0;
        BattleEvent_Push(BATTLE_EVENT_CLEAR_ARGUMENTS, 0);
        BattleEvent_Push(BATTLE_EVENT_UNIT_NAME, tid);
        MESSAGE_PUSH(MsgCureStun);
      }
      if (target->sleep != 0) {
        target->sleep = 0;
        MESSAGE_PUSH(MsgWakes);
        BattleEvent_Push(BATTLE_EVENT_UNIT_NAME, tid);
      }
      if (target->psySeal != 0) {
        target->psySeal = 0;
        MESSAGE_PUSH(MsgCureSeal);
        BattleEvent_Push(BATTLE_EVENT_UNIT_NAME, tid);
      }
      if (target->grimReaper != 0) {
        target->grimReaper = 0;
        MESSAGE_PUSH(MsgCureCurse);
        BattleEvent_Push(BATTLE_EVENT_UNIT_NAME, tid);
      }
      if (target->haunt != 0) {
        target->haunt = 0;
        MESSAGE_PUSH(MsgCureSpirit);
        BattleEvent_Push(BATTLE_EVENT_UNIT_NAME, tid);
      }
      if (target->poison != 0) {
        MESSAGE_PUSH(MsgCurePoison);
        target->poison = 0;
      }
      BattleEvent_Push(BATTLE_EVENT_CLEAR_ARGUMENTS, 0);
      break;

    case 4:
      if (target->delusion != 0) {
        target->delusion = 0;
        BattleEvent_Push(BATTLE_EVENT_CLEAR_ARGUMENTS, 0);
        BattleEvent_Push(BATTLE_EVENT_UNIT_NAME, tid);
        MESSAGE_PUSH(MsgCureDelusion);
      }
      if (target->stun != 0) {
        target->stun = 0;
        BattleEvent_Push(BATTLE_EVENT_CLEAR_ARGUMENTS, 0);
        BattleEvent_Push(BATTLE_EVENT_UNIT_NAME, tid);
        MESSAGE_PUSH(MsgCureStun);
      }
      if (target->sleep != 0) {
        target->sleep = 0;
        BattleEvent_Push(BATTLE_EVENT_CLEAR_ARGUMENTS, 0);
        BattleEvent_Push(BATTLE_EVENT_UNIT_NAME, tid);
        MESSAGE_PUSH(MsgWakes);
      }
      if (target->psySeal != 0) {
        target->psySeal = 0;
        BattleEvent_Push(BATTLE_EVENT_CLEAR_ARGUMENTS, 0);
        BattleEvent_Push(BATTLE_EVENT_UNIT_NAME, tid);
        MESSAGE_PUSH(MsgCureSeal);
      }
      if (target->grimReaper == 0)
        break;
      target->grimReaper = 0;
      BattleEvent_Push(BATTLE_EVENT_CLEAR_ARGUMENTS, 0);
      BattleEvent_Push(BATTLE_EVENT_UNIT_NAME, tid);
      MESSAGE_PUSH(MsgCureCurse);
      break;

    case 61:
    case 62:
    case 70:
    case 71:
    case 76: {
      s32 old;
      s32 maxu;
      s32 maxv;
      s32 heal;
      u16 *pt;

      /* FAKEMATCH(temporary): a pointer walked down from hp to maxHp reads each
       * stat twice, unsigned and signed; as reads of target->hp and
       * target->maxHp, 94 of 3451 instructions differ in tla-en. */
      pt = (u16 *)&target->hp;
      old = *pt--;
      heal = *(s16 *)(pt + 1);
      if (action->effect == 76) {
        s32 m;

        pt--;
        maxu = *pt--;
        m = *(s16 *)(pt + 1);
        heal += m * 2 / 5;
      } else if (action->effect == 71) {
        pt--;
        maxu = *pt--;
        maxv = *(s16 *)(pt + 1);
        heal += maxv * 7 / 10;
      } else if (action->effect == 70) {
        maxu = (u16)target->maxHp;
        heal += (s16)maxu / 2;
        /* FAKEMATCH(barrier): an empty asm on maxu, so the clamp shifts maxu
         * left by 16 again instead of sharing this halving's shift; without
         * it, 45 of 3451 instructions differ in tla-en. */
        asm("" : "+r"(maxu));
      } else {
        pt--;
        maxu = *pt--;
        maxv = *(s16 *)(pt + 1);
        if (action->effect == 61) {
          s32 p = maxv * 60;
          heal += p / 100;
        } else {
          s32 p = maxv * 30;
          heal += p / 100;
        }
      }
      if (heal > (s16)maxu)
        heal = (s16)maxu;
      tmp = heal - (s16)old;
      if (tmp == 0 && type != 1)
        break;
      if (heal == (s16)maxu)
        MESSAGE_PUSH(MsgHpFull);
      else {
        BattleEvent_Push(BATTLE_EVENT_NUMBER, tmp);
        MESSAGE_PUSH(MsgHpRecover);
      }
      target->hp = heal;
      Unit_UpdateRatiosFar(tid);
      break;
    }

    case 63:
    case 77:
    case 78: {
      s32 old;
      s32 maxv;
      s32 heal;
      s32 maxu;
      u16 *pt;

      /* FAKEMATCH(temporary): a pointer walked down from pp to maxPp reads each
       * stat twice, unsigned and signed; as reads of target->pp and
       * target->maxPp, 255 of 3451 instructions differ in tla-en. */
      pt = (u16 *)&target->pp;
      old = *pt--;
      heal = *(s16 *)(pt + 1);
      if (action->effect == 77) {
        maxv = target->maxPp;
        maxu = (u16)target->maxPp;
        heal += (s16)maxv / 10;
      } else if (action->effect == 78) {
        pt--;
        maxu = *pt--;
        maxv = *(s16 *)(pt + 1);
        heal += maxv * 3 / 10;
      } else {
        pt--;
        maxu = *pt--;
        maxv = *(s16 *)(pt + 1);
        heal += maxv * 7 / 100;
      }
      if (heal > (s16)maxu)
        heal = (s16)maxu;
      tmp = heal - (s16)old;
      if (tmp == 0 && type != 11)
        break;
      if (heal == (s16)maxu)
        MESSAGE_PUSH(MsgPpFull);
      else {
        BattleEvent_Push(BATTLE_EVENT_NUMBER, tmp);
        MESSAGE_PUSH(MsgPpRecover);
      }
      target->pp = heal;
      Unit_UpdateRatiosFar(tid);
      break;
    }

    case 59:
      target->agilityStage = 8;
      target->agilityTurns = 5;
      Unit_CalculateFar(tid);
      BattleEvent_Push(BATTLE_EVENT_NUMBER, target->agi - bak->agi);
      MESSAGE_PUSH(MsgAgiUp);
      break;

    case 58:
      am = &target->agilityStage;
      {
        u8 v;

        v = -4;
        *am = v;
      }
      target->agilityTurns = 5;
      Unit_CalculateFar(tid);
      BattleEvent_Push(BATTLE_EVENT_NUMBER, bak->agi - target->agi);
      MESSAGE_PUSH(MsgAgiDown);
      break;

    case 9:
      target->attackStage -= 1;
      STEP_LIM(target->attackStage);
      Unit_CalculateFar(tid);
      BattleEvent_Push(BATTLE_EVENT_NUMBER, bak->atk - target->atk);
      MESSAGE_PUSH(MsgAtkDown);
      target->attackTurns = 7;
      break;

    case 8:
      target->attackStage -= 2;
      STEP_LIM(target->attackStage);
      Unit_CalculateFar(tid);
      BattleEvent_Push(BATTLE_EVENT_NUMBER, bak->atk - target->atk);
      MESSAGE_PUSH(MsgAtkDown);
      target->attackTurns = 7;
      break;

    case 7:
      target->attackStage += 1;
      STEP_LIM(target->attackStage);
      Unit_CalculateFar(tid);
      BattleEvent_Push(BATTLE_EVENT_NUMBER, target->atk - bak->atk);
      MESSAGE_PUSH(MsgAtkUp);
      target->attackTurns = 7;
      break;

    case 6:
      target->attackStage += 2;
      STEP_LIM(target->attackStage);
      Unit_CalculateFar(tid);
      BattleEvent_Push(BATTLE_EVENT_NUMBER, target->atk - bak->atk);
      MESSAGE_PUSH(MsgAtkUp);
      target->attackTurns = 7;
      break;

    case 13:
      target->defenseStage -= 1;
      STEP_LIM(target->defenseStage);
      Unit_CalculateFar(tid);
      BattleEvent_Push(BATTLE_EVENT_NUMBER, bak->def - target->def);
      MESSAGE_PUSH(MsgDefDown);
      target->defenseTurns = 7;
      break;

    case 12:
      target->defenseStage -= 2;
      STEP_LIM(target->defenseStage);
      Unit_CalculateFar(tid);
      BattleEvent_Push(BATTLE_EVENT_NUMBER, bak->def - target->def);
      MESSAGE_PUSH(MsgDefDown);
      target->defenseTurns = 7;
      break;

    case 11:
      target->defenseStage += 1;
      STEP_LIM(target->defenseStage);
      Unit_CalculateFar(tid);
      BattleEvent_Push(BATTLE_EVENT_NUMBER, target->def - bak->def);
      MESSAGE_PUSH(MsgDefUp);
      target->defenseTurns = 7;
      break;

    case 10:
      target->defenseStage += 2;
      STEP_LIM(target->defenseStage);
      Unit_CalculateFar(tid);
      BattleEvent_Push(BATTLE_EVENT_NUMBER, target->def - bak->def);
      MESSAGE_PUSH(MsgDefUp);
      target->defenseTurns = 7;
      break;

    case 5:
      if (target->hp != 0)
        break;
      MESSAGE_PUSH(MsgRevived);
      target->hp = target->maxHp;
      Unit_UpdateRatiosFar(tid);
      break;

    case 56:
      if (target->hp != 0)
        break;
      MESSAGE_PUSH(MsgRevived);
      target->hp = target->maxHp / 2;
      Unit_UpdateRatiosFar(tid);
      break;

    case 57:
      if (target->hp != 0)
        break;
      MESSAGE_PUSH(MsgRevived);
      target->hp = target->maxHp * 8 / 10;
      Unit_UpdateRatiosFar(tid);
      break;

    case 73:
      if (target->hp != 0)
        break;
      MESSAGE_PUSH(MsgRevived);
      target->hp = (s16)(target->maxHp * 6 / 10);
      Unit_UpdateRatiosFar(tid);
      break;

    case 3:
      if (target->poison != 0)
        MESSAGE_PUSH(MsgCurePoison);
      target->poison = 0;
      break;

    case 17:
      RES_STEP(-1, (bak->resistanceStage - target->resistanceStage) * 20,
               MsgResDown);
      break;

    case 16: {
      target->resistanceStage -= 2;
      if (-4 > target->resistanceStage)
        target->resistanceStage = -4;
      if (target->resistanceStage > 4)
        target->resistanceStage = 4;
      BattleEvent_Push(BATTLE_EVENT_NUMBER,
                       (bak->resistanceStage - target->resistanceStage) * 20);
      MESSAGE_PUSH(MsgResDown);
      target->resistanceTurns = 7;
    } break;

    case 15:
      RES_STEP(1, (target->resistanceStage - bak->resistanceStage) * 20,
               MsgResUp);
      break;

    case 14: {
      target->resistanceStage += 2;
      if (*(rm = &target->resistanceStage) < -4)
        target->resistanceStage = -4;
      if (target->resistanceStage > 4)
        target->resistanceStage = 4;
      BattleEvent_Push(BATTLE_EVENT_NUMBER,
                       (target->resistanceStage - bak->resistanceStage) * 20);
      MESSAGE_PUSH(MsgResUp);
      target->resistanceTurns = 7;
    } break;

    case 18:
      if (target->poison != 0)
        break;
      MESSAGE_PUSH(MsgPoisoned);
      target->poison = 1;
      break;

    case 19:
      if (target->poison > 1)
        break;
      MESSAGE_PUSH(MsgVenom);
      target->poison = 2;
      break;

    case 20:
    case 75:
      STAT_SET(target->delusion, MsgDelusion);
      break;

    case 21:
      STAT_SET(target->confusion, MsgConfused);
      break;

    case 22:
      STAT_SET(target->charm, MsgCharmed);
      break;

    case 23:
    case 85:
      STAT_SET(target->stun, MsgStunned);
      break;

    case 24:
      STAT_SET(target->sleep, MsgAsleep);
      break;

    case 25:
      MESSAGE_SIDE(MsgPsyBlock, MsgPsySeal);
      target->psySeal |= 7;
      break;

    case 67:
      MESSAGE_SIDE(MsgPsyBlock, MsgPsySeal);
      target->psySeal |= 16;
      break;

    case 27:
      if (BattleUnit_KeepsOneHp(tid) != 0)
        break;
      BattleEvent_Push(BATTLE_EVENT_DEFEAT, tid);
      if (target->side == ALLY)
        MESSAGE_PUSH(MsgKoDown);
      else if (abilityId == 219)
        MESSAGE_PUSH(MsgSuffocate);
      else
        MESSAGE_PUSH(MsgSpiritDrain);
      target->hp = 0;
      Unit_UpdateRatiosFar(tid);
      break;

    case 29:
      MESSAGE_PUSH(MsgHpRegen);
      target->refrain = 5;
      break;

    case 30:
      STAT_SET(target->reflect, MsgReflect);
      break;

    case 31:
    case 60:
      ap = dealt;
      dmg = actor->hp;
      /* FAKEMATCH(barrier): an empty asm on dmg, so its copy comes before
       * dealt is reloaded from the stack; without it, 2 of 3451 instructions
       * differ in tla-en. */
      __asm__("" : "=r"(dmg) : "0"(dmg));
      if (action->effect == 60)
        ap /= 2;
      kind = ap;
      dmg += kind;
      if (dmg > actor->maxHp) {
        dmg = actor->maxHp;
        kind = dmg - actor->hp;
      }
      BattleEvent_Push(BATTLE_EVENT_CLEAR_ARGUMENTS, 0);
      BattleEvent_Push(BATTLE_EVENT_UNIT_NAME, aid);
      if (dmg == actor->maxHp)
        MESSAGE_PUSH(MsgHpFull);
      else {
        BattleEvent_Push(BATTLE_EVENT_NUMBER, kind);
        MESSAGE_PUSH(MsgHpRecover);
      }
      actor->hp = dmg;
      Unit_UpdateRatiosFar(aid);
      break;

    case 32: {
      s32 heal;
      s32 amt;

      heal = actor->pp;
      amt = dealt;
      heal += amt;
      if (heal > actor->maxPp) {
        heal = actor->maxPp;
        amt = heal - actor->pp;
      }
      BattleEvent_Push(BATTLE_EVENT_CLEAR_ARGUMENTS, 0);
      BattleEvent_Push(BATTLE_EVENT_UNIT_NAME, aid);
      if (heal == actor->maxPp)
        MESSAGE_PUSH(MsgPpFull);
      else {
        BattleEvent_Push(BATTLE_EVENT_NUMBER, amt);
        MESSAGE_PUSH(MsgPpRecover);
      }
      actor->pp = heal;
      Unit_UpdateRatiosFar(aid);
      break;
    }

    case 69:
      dmg = dealt / 10;
      if (target->pp < dmg)
        dmg = target->pp;
      if (actor->pp + dmg > actor->maxPp)
        dmg = actor->maxPp - actor->pp;
      if (dmg == 0)
        break;
      BattleEvent_Push(BATTLE_EVENT_NUMBER, dmg);
      MESSAGE_SIDE(MsgLeechTake, MsgLeechGain);
      Unit_AddPpFar(aid, dmg);
      break;

    case 84:
      dmg = (s16)(target->maxPp / 10);
      if (target->pp < dmg)
        dmg = target->pp;
      if (dmg == 0)
        break;
      BattleEvent_Push(BATTLE_EVENT_NUMBER, dmg);
      MESSAGE_SIDE(MsgMorePpLossP, MsgMorePpLossE);
      Unit_AddPpFar(tid, -dmg);
      break;

    case 33:
      if (target->attackStage > 0) {
        target->attackStage = 0;
        target->attackTurns = 0;
      }
      if (target->defenseStage > 0) {
        target->defenseStage = 0;
        target->defenseTurns = 0;
      }
      if (target->resistanceStage > 0) {
        target->resistanceStage = 0;
        target->resistanceTurns = 0;
      }
      if (target->agilityStage > 0)
        target->agilityStage = 0;
      target->elementLevels[0] = 0;
      target->elementLevels[1] = 0;
      target->elementLevels[2] = 0;
      target->elementLevels[3] = 0;
      MESSAGE_PUSH(MsgBuffsReset);
      break;

    case 26:
      MESSAGE_PUSH(MsgEvilSpirit);
      target->haunt = 1;
      break;

    case 28:
    case 80:
      if (target->grimReaper == 0) {
        MESSAGE_PUSH(MsgDeathCurse);
        target->grimReaper = 7;
        break;
      }
      if (target->grimReaper <= 1)
        break;
      target->grimReaper -= 1;
      BattleEvent_Push(BATTLE_EVENT_NUMBER, target->grimReaper);
      MESSAGE_PUSH(MsgDeathCount);
      break;

    case 66:
      MESSAGE_PUSH(MsgReadies);
      target->ready = 2;
      break;

    case 54:
      MESSAGE_PUSH(MsgChallenge);
      target->challenge = 1;
      if (ALLY_CHECK(tid))
        work->challengeFlag |= 2;
      break;

    case 53:
    case 83:

      MESSAGE_PUSH(MsgImmobile);
      target->immobile = 1;
      break;

    case 46:

      MESSAGE_PUSH(MsgAura);
      if (target->guardLevel > 0)
        break;
      target->guardLevel = 1;
      break;

    case 72:
      MESSAGE_PUSH(MsgAura);
      if ((u32)target->guardLevel > 1)
        break;
      target->guardLevel = 2;
      break;

    case 47:
    case 88:
      MESSAGE_PUSH(MsgAura2);
      if (target->guardLevel > 2)
        break;
      target->guardLevel = 3;
      break;

    case 79:
      MESSAGE_PUSH(MsgVanished);
      if ((u32)target->guardLevel > 3)
        break;
      target->guardLevel = 4;
      break;

    case 45:
      BattleEvent_Push(BATTLE_EVENT_MESSAGE, -1);
      break;

    case 81:
      MESSAGE_PUSH(MsgAllResting);
      work->unk046[1] = 1;
      break;

    case 82:
      MESSAGE_PUSH(MsgBlownAway);
      BattleEvent_Push(15, tid);
      break;

    case 74:
      MESSAGE_PUSH(MsgCounterReady);
      target->equipBonus[1] = 1;
      break;

    default:
      break;
    }
  }

done:
  BattleEvent_Push(BATTLE_EVENT_CLEAR_ARGUMENTS, 0);
  if (plan->command != 9 && (s8)target->equipBonus[1] != 0 &&
      action->targetMode == 1 && target->hp != 0) {
    work->unk858 = tid;
    work->unk85c = aid;
  }

  if (target->hp != 0) {
    if (target->sleep != 0)
      if (target->sleep <= 6 && dealt > 0 && (3 & Battle_RandomFar()) == 0) {
        target->sleep = 0;
        BattleEvent_Push(BATTLE_EVENT_UNIT_NAME, tid);
        MESSAGE_PUSH(MsgWakes);
      }
  }
  Heap_Release(bak);
  Unit_CalculateFar(tid);
  PartyStatus_DrawFar(Ram_HeapSlots->battleWork->partyStatus);
  if (target->hp != 0)
    BattleEvent_Push(BATTLE_EVENT_REFRESH_UNIT, tid);
  if (abilityId != 403 && actor->haunt != 0 && (Battle_RandomFar() & 3) == 0 &&
      dealt > 0) {
    s32 share;

    share = dealt >> 2;
    if (share == 0)
      share = 1;
    plan->hauntDamage += share;
  }
}
