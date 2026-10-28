// Under -mthumb-slow-loads (26 October 2026): differ 6 of 453 in tla-en.
// Tried, one edit at a time: an empty asm barrier after each statement, each
// adjacent pair of statements swapped, each local pinned to each register,
// each call argument copied into its argument register (pinned) at the call
// or up to three statements before it, five minutes of permute; none reaches
// 0.
// Draft of Func_0811df70: ☀️'s BattleStaging_HideField
// (games/tbs/src/battle/battle/staging.c), ported: a summon or full-screen
// cast. The camera turns (the summon's yaw at once over 60 frames, else the
// acting side's view), the side not targeted fades out over 16 frames while
// the actor takes its mode from the battle work's byte 0x869, then a failed
// cast shows its message, or the effect plays (BattleEffect_RunSceneEffectFar
// for flag 0x8000, Func_08138008 for 0x4000, else Func_08138018) with the
// events; everyone fades back in. ⚓️ differs: mode 2 keeps the actor's mode
// and skips the cast sound and glow (Func_08127308), mode 1 hides the actor;
// an actor whose effect kind is 3 stays faded. Near miss, differ 22 of 453
// (2026-10-24), instruction order only, 11 pairs, each a load the native
// issues one slot earlier (ahead of an independent constant, copy or and).
// Forms measured: the unit read into a u32 inside the side test (139 without:
// the 254 test's narrowed halfword read then serves every later read, where
// the native reloads the unit for the side test), the summon's two yaws as
// stores in each arm, the target copy reusing i, and if (target <= 7) for the
// side.
#include "types.h"
#include "battle.h"
#include "battle_unit.h"
#include "battle_type.h"
#include "battle_work.h"
#include "battle_display.h"
#include "battle_event.h"
#include "battle_party.h"
#include "callback_scheduler.h"
#include "motobj.h"
#include "ram_buffer.h"
#include "io_reg.h"

struct CastAnimationEntry {
  u8 unk00[5];
  u8 param;
};

struct CastAnimation {
  u8 unk00[27];
  u8 count;
  u8 unk1c[12];
  struct CastAnimationEntry *entries[1];
};

extern char MsgCastLacksPp;
extern char MsgPsynergyBlocked;

void Object_SetMode(struct MotionObject *object, s32 mode);
void PartyStatus_DrawFar(s32 flags);
void Sound_PlayFar(s32 cue);
void Func_08127308(s32 unitId, s32 element, s32 mode, s32 value);
void Func_0812756c(void);
void Func_08138008(struct BattlePresentationWork *work);
void Func_08138018(struct BattlePresentationWork *work);
void BattleEventRuntime_WaitForReady(void);
void BattleEvent_Playback(void);

s32 Func_0811df70(struct BattlePlan *plan, s32 mode) {
  u16 units[14];
  struct BattlePresentationWork work;
  u32 actor;
  u32 target;
  u32 count;
  u32 i;
  u32 kept;
  struct MotionObject *object;

  BattlePres_BuildTargetList(plan, &work);
  actor = plan->actorId;
  target = plan->targetIds[0];
  if (plan->presentationFlags & 0x8000) {
    struct BattlePresentationTransition *view = Ram_HeapSlots->transitionWork;

    if (actor <= 7)
      view->targetYaw = 0x2000;
    else
      view->targetYaw = 0x5000;
    view->frames = 60;
  } else {
    struct BattlePresentationTransition *view = Ram_HeapSlots->transitionWork;
    s32 yaw;

    yaw = -0x2000;
    if (actor <= 7)
      yaw = 0x2000;
    if (view->targetYaw != yaw)
      view->targetYaw = yaw;
  }

  BattlePres_SetActorModes(0, 0);
  PartyStatus_DrawFar(Ram_HeapSlots->battleWork->partyStatus & ~1);
  object = BattleObject_Get(actor)->object;
  REG_BLDCNT = 0x3f40;
  count = BattleParty_ListActorIds(3, units);
  for (i = 0; i != count; i++) {
    if (units[i] == 254)
      continue;
    if ((mode & 2) || units[i] != actor) {
      u32 unit = units[i];

      if ((target <= 7) != (unit <= 7))
        BattlePres_SetActorRecordMode(unit, 1);
    } else {
      Object_SetMode(object, ((u8 *)Ram_HeapSlots->battleWork)[0x869]);
    }
  }
  if (!(mode & 2)) {
    Sound_PlayFar(154);
    Func_08127308(work.actor, plan->element, 0, 0);
  }
  if (mode & 1)
    BattlePres_SetActorRecordMode(actor, 1);
  for (i = 0; i != 16; i++) {
    REG_BLDALPHA = (16 - i) | 0x1000;
    WaitFrames(1);
  }

  if (plan->failure != 0) {
    if (plan->failure == 1) {
      BattleEvent_Push(0, plan->actorId);
      BattleEvent_Push(4, (u32)&MsgPsynergyBlocked);
    } else {
      BattleEvent_Push(4, (u32)&MsgCastLacksPp);
    }
    BattleEvent_DispatchQueued();
    Func_0812756c();
  } else {
    kept = 0;
    for (i = 0; i < count; i++) {
      u32 unit = units[i];

      if (unit == actor) {
        if (!(mode & 1))
          units[kept++] = actor;
      } else if ((target <= 7) == (unit <= 7)) {
        units[kept++] = unit;
      }
    }
    units[kept] = 255;
    BattleActor_SpawnObjectsForList((s16 *)units, 0);
    for (i = 0; i < plan->targetCount; i++)
      units[i] = plan->targetIds[i];
    units[i] = 255;
    for (i = 0; i != work.count; i++) {
      struct CastAnimation *animation =
          GetMotionRecord(BattleObject_Get(work.actors[i])->object, 0);
      u32 child;

      for (child = 0; child != animation->count - 1; child++)
        work.params[i][child] = animation->entries[child]->param;
    }
    if (plan->presentationFlags & 0x8000) {
      if (target <= 7)
        work.side = 1;
      else
        work.side = 0;
    } else {
      if (plan->targetIds[0] <= 7)
        work.side = 1;
      else
        work.side = 0;
    }
    if (plan->presentationFlags & 0x20000)
      work.side ^= 1;
    Scheduler_AddOrUpdateCallback(BattleEvent_Playback, 0x480);
    if (plan->presentationFlags & 0x8000)
      BattleEffect_RunSceneEffectFar((s32 *)&work);
    else if (plan->presentationFlags & 0x4000)
      Func_08138008(&work);
    else
      Func_08138018(&work);
    BattleEventRuntime_WaitForReady();
  }

  BattleObject_Put();
  count = BattleParty_ListActorIds(3, units);
  REG_BLDCNT = 0x3f40;
  for (i = 0; i != count; i++) {
    u32 unit;

    if (units[i] == 254)
      continue;
    if (!(mode & 2) && units[i] == actor && work.kind == 3)
      continue;
    unit = units[i];
    if ((target <= 7) != (unit <= 7))
      BattlePres_SetActorRecordMode(unit, 1);
  }
  for (i = 0; i != 16; i++) {
    REG_BLDALPHA = i | 0x1000;
    WaitFrames(1);
  }
  for (i = 0; i != count; i++)
    BattlePres_SetActorRecordMode(units[i], 0);
  BattlePres_SetupTransitionScene(0, 0, 0, 100);
  WaitFrames(1);
  return 0;
}
