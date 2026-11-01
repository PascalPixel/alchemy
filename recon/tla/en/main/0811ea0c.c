// Draft, differ 6 of 398, skeleton gap 3 (tla-en, 2026-11-01)
// Func_0811ea0c: The Broken Seal's BattleStaging_RunAttack
// (games/tbs/src/battle/battle/staging.c), ported: the attacker's object is
// put on its anchor and the camera turns to its bearing (one frame either way,
// where The Broken Seal waits 5 or 10), the class's sprite of Func_0811e3ac may
// stand in, effect kind 10 sets the attacker's mode 2 instead of the run up,
// mode 32 runs a second attacker (the plan's byte 2) up beside it, and a
// critical hit whose kind is not 210 zooms on the pair; ObjMotRst returns the
// units.
// Allocation is closed. The pseudo for the second attacker's BattleObj must be
// `object` itself, hence the void pointer: as its own local it joins the local
// allocations in r5, the i * 30 giv (7 refs over 40) outranks r5's locals (10
// over 64), find_reg evicts them, and plan takes a register.
// Left: three order swaps of one instruction each, all a store the native
// issues one step sooner. `str r0, [sp, #16]` before the view address's
// `movs r3, #192`: sched2 has that split chain at priority 87 and the store at
// 86. `mov sl, r4` before the load of targetMultiplier[0]: the load is ready
// one cycle after `str r3, [sp, #12]`, and outranks it. REG_WINOUT's `strh`
// before REG_DISPCNT's `movs r1, #128`. One more cycle of store cost in sched2
// would fit the first two (the third's priorities were not read); that is an
// inference, and a compiler question (K2, K3) for Pascal, not a source one.
// Tried: missed spelled as two statements (no change); actorId read before view
// (differ 12); a one-pass block around the window registers, or around
// REG_DISPCNT alone, closes the third swap (differ 4); a one-pass around
// missed = 0 fixes the second but pushes `ldr r1, [sp, #16]` below the store,
// and a temporary for plan is copy-propagated away even with a second set. A
// barrier for the first swap would also move the mode copy `adds r7, r1` above
// the view load, and that copy is set before any statement can sit. With no
// way to differ 0 the devices came back out.
#include "types.h"
#include "battle.h"
#include "battle_unit.h"
#include "battle_type.h"
#include "battle_work.h"
#include "battle_display.h"
#include "battle_event.h"
#include "battle_side.h"
#include "task.h"
#include "motobj.h"
#include "ram_buffer.h"
#include "io_reg.h"
#include "heap_release.h"

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
void BattleCamera_SetViewAtPairMidpoint(s32 first, s32 second, s32 scale);
void BattleParty_ListAllUnitsAndSubmit(void);
void *Func_0811e3ac(struct BattlePlan *plan,
                    struct BattlePresentationWork *work);
void Func_0811e7dc(void *saved);
void Func_0811e830(void *saved);
void Func_08138008(struct BattlePresentationWork *work);
void Func_08138018(struct BattlePresentationWork *work);
void BattlePlayback_WaitForReady(void);
void BattleEvent_Playback(void);
void ObjMotRst(s32 slot);

s32 Func_0811ea0c(struct BattlePlan *plan, s32 mode) {
  struct BattlePresentationWork work;
  void *object;
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
    wait(1);
  } else {
    view->targetYaw = facingAngle;
    wait(1);
  }

  BattleHighlight_SetModes(0, 0);
  BattleStaging_BuildTargetList(plan, &work);
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
    object = BattleObject_Get(plan->actorId2);

    animation = GetMotionRecord(((struct BattleObj *)object)->object, 0);
    frames = ResourceMetadata_SumCommandLengthsFar(
        animation->entries[0]->animationId, 2, 1);
    BattleMotion_ApproachTarget(
        plan->actorId2, work.actors[0], frames,
        BattleClass_GetAttackHop(Unit_Get(plan->actorId2)->enemyId) << 16);
    Actor_SetAnimationSpeedFar(
        (struct ActorView *)((struct BattleObj *)object)->object, 16);
  }
  Actor_SetAnimationSpeedFar(
      (struct ActorView *)BattleObject_Get(work.actor)->object, 16);

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
    wait(10);
    BattleMotion_ResetObjectAtScaledAnchor(work.actors[0]);
    wait(2);
    wait(4);
    wait(10);
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
      view->flag = 1;
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
          BattleCamera_SetViewAtPairMidpoint(first, second,
                                                   index * 30 / frames + 100);
        }
        wait(1);
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
    BattlePlayback_WaitForReady();
    if (critical != 0) {
      view->flag = 0;
      BattleParty_ListAllUnitsAndSubmit();
      BattleCamera_SetView(0, 0, 0, 100);
    }
    ObjMotRst(work.actors[0]);
  }
  if (!saved)
    ObjMotRst(work.actor);
  return 0;
}
