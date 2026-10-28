// First draft of Func_0811ea0c: ☀️'s BattleStaging_RunAttack
// (games/tbs/src/battle/battle/staging.c), ported: the attacker's object is
// put on its anchor and the camera turns to its bearing (one frame either
// way, where ☀️ waits 5 or 10), the class's sprite of Func_0811e3ac may stand
// in, effect kind 10 sets the attacker's mode 2 instead of the run up, mode 32
// runs a second attacker (the plan's byte 2) up beside it, and a critical hit
// whose kind is not 210 zooms on the pair; ObjMotRst returns the units.
// Differ 156 of 398 (2026-10-24), register choice mostly: the native keeps
// actorId in r5 and spills plan (here r6 and r11), so frames, the loop's
// counter and its multiple of 30 take r11, r9 and r10 there. Seen while
// searching: the same body with actorId compared as actorId + 1 > 8 (one
// constant and an add differ) gives the native's allocation, differ 39, so
// everything else is in place; no equivalent spelling found yet (block or
// function-level actorId at any declaration position, its type).
#include "types.h"
#include "battle.h"
#include "battle_unit.h"
#include "battle_type.h"
#include "battle_work.h"
#include "battle_display.h"
#include "battle_event.h"
#include "battle_party.h"
#include "task.h"
#include "motobj.h"
#include "ram_buffer.h"
#include "io_reg.h"
#include "mem.h"

struct AttackAnimationEntry {
  s16 animationId;
};

struct AttackAnimation {
  u8 unk00[40];
  struct AttackAnimationEntry *entries[1];
};

extern char MsgNimblyDodgesBlow;

s32 ArcTan2(s32 x, s32 y);
struct BattleUnit *Unit_Get(s32 unitId);
void Actor_SetMode(struct MotionObject *object, s32 mode);
void PartyStatus_DrawFar(s32 flags);
s32 ResourceMetadata_SumCommandLengthsFar(s32 animation, s32 a, s32 b);
s32 BattleClass_GetAttackHop(s32 classId);
void BattleMotion_ApproachTarget(s32 actorId, s32 targetId, s32 frames,
                                 s32 hop);
void BattleMotion_ResetObjectAtScaledAnchor(s32 objectId);
void BattlePres_SetupTransitionAtPairMidpoint(s32 first, s32 second, s32 scale);
void BattleParty_ListAllUnitsAndSubmit(void);
void *Func_0811e3ac(struct BattlePlan *plan,
                    struct BattlePresentationWork *work);
void Func_0811e7dc(void *saved);
void Func_0811e830(void *saved);
void Func_08138008(struct BattlePresentationWork *work);
void Func_08138018(struct BattlePresentationWork *work);
void BattleEventRuntime_WaitForReady(void);
void BattleEvent_Playback(void);
void ObjMotRst(s32 slot);

s32 Func_0811ea0c(struct BattlePlan *plan, s32 mode) {
  struct BattlePresentationWork work;
  struct MotionObject *object;
  struct AttackAnimation *animation;
  struct BattleUnit *unit;
  s16 criticalPair[3];
  s32 critical;
  struct BattlePresentationTransition *view;
  void *saved;
  s32 angle;
  s32 adjusted;
  s32 facingAngle;
  s32 frames;
  s32 missed;
  s32 index;
  s32 zoomOnCritical;
  u32 actorId;
  struct BattleObj *battleObject;

  view = Ram_HeapSlots->transitionWork;
  actorId = plan->actorId;
  battleObject = BattleObject_Get(actorId);
  {
    s32 x = battleObject->anchorX;
    s32 z;

    battleObject->object->x = x;
    z = battleObject->anchorZ;
    battleObject->object->z = z;
    angle = (u16)ArcTan2(x, z);
    adjusted = angle - 0x2000;
    if (actorId > 7)
      adjusted = angle + 0x6000;
  }
  adjusted &= 0x7fff;
  facingAngle = (adjusted - 0x2000) / 2 + 0x2000;
  if (view->targetYaw == facingAngle) {
    view->targetYaw = facingAngle;
    WaitFrames(1);
  } else {
    view->targetYaw = facingAngle;
    WaitFrames(1);
  }

  BattlePres_SetActorModes(0, 0);
  BattlePres_BuildTargetList(plan, &work);
  if (work.kind == 135)
    PartyStatus_DrawFar(Ram_HeapSlots->battleWork->partyStatus & ~1);
  saved = Func_0811e3ac(plan, &work);
  unit = Unit_Get(work.actor);
  Unit_Get(work.actors[0]);
  critical = plan->targetCritical[0];
  missed = plan->targetMultiplier[0] == 0;

  object = BattleObject_Get(plan->actorId)->object;
  animation = GetMotionRecord(object, 0);
  frames = ResourceMetadata_SumCommandLengthsFar(
      animation->entries[0]->animationId, 2, 1);
  if (work.kind == 10)
    Actor_SetMode(object, 2);
  else
    BattleMotion_ApproachTarget(work.actor, work.actors[0], frames,
                                BattleClass_GetAttackHop(unit->enemyId) << 16);
  if ((mode & 32) && plan->actorId != plan->actorId2 &&
      plan->actorId2 != (work.actors[0] & 0x80)) {
    struct BattleObj *partner = BattleObject_Get(plan->actorId2);

    animation = GetMotionRecord(partner->object, 0);
    frames = ResourceMetadata_SumCommandLengthsFar(
        animation->entries[0]->animationId, 2, 1);
    BattleMotion_ApproachTarget(
        plan->actorId2, work.actors[0], frames,
        BattleClass_GetAttackHop(Unit_Get(plan->actorId2)->enemyId) << 16);
    Actor_SetAnimationSpeedFar(
        (struct DispatchObject *)partner->object, 16);
  }
  Actor_SetAnimationSpeedFar(
      (struct DispatchObject *)BattleObject_Get(work.actor)->object, 16);

  if ((u16)work.actors[0] <= 7)
    work.side = 1;
  else
    work.side = 0;

  REG_WIN0H = 0xf0;
  REG_WIN0V = 0x1088;
  REG_WIN1H = 0xf0;
  REG_WIN1V = 0x1088;
  REG_WININ = 0x3537;
  REG_WINOUT = 0x3f21;
  REG_DISPCNT |= 0x6000;

  if (missed != 0) {
    WaitFrames(10);
    BattleMotion_ResetObjectAtScaledAnchor(work.actors[0]);
    WaitFrames(2);
    WaitFrames(4);
    WaitFrames(10);
    BattleEvent_Push(0, plan->targetIds[0]);
    BattleEvent_Push(4, (u32)&MsgNimblyDodgesBlow);
    BattleEvent_DispatchQueued();
    ObjMotRst(work.actors[0]);
  } else {
    zoomOnCritical = 0;
    work.flags = 0;
    if (plan->presentationFlags != 0)
      work.flags = 1;
    if (critical != 0)
      work.kind += 200;
    if (work.kind != 210 && critical != 0) {
      zoomOnCritical = 1;
      view->active = 1;
      criticalPair[0] = work.actor;
      criticalPair[1] = work.firstTarget;
      criticalPair[2] = 255;
      BattleActor_SpawnObjectsForList(criticalPair, 0);
    }
    frames -= 8;
    if (frames <= 0)
      frames = 1;
    {
      s32 first;
      s32 second;

      for (index = 0; index != frames; index++) {
        if (zoomOnCritical != 0) {
          first = work.actor;
          second = work.firstTarget;
          BattlePres_SetupTransitionAtPairMidpoint(first, second,
                                                   index * 30 / frames + 100);
        }
        WaitFrames(1);
      }
    }
    Task_Add(BattleEvent_Playback, TASK_PASS_VBLANK);
    if (work.kind != 0) {
      if (saved)
        Func_0811e7dc(saved);
      if (plan->presentationFlags & 0x4000)
        Func_08138008(&work);
      else
        Func_08138018(&work);
    }
    if (saved) {
      Func_0811e830(saved);
      Heap_Release(saved);
    }
    BattleEventRuntime_WaitForReady();
    if (critical != 0) {
      view->active = 0;
      BattleParty_ListAllUnitsAndSubmit();
      BattlePres_SetupTransitionScene(0, 0, 0, 100);
    }
    ObjMotRst(work.actors[0]);
  }
  if (!saved)
    ObjMotRst(work.actor);
  return 0;
}
