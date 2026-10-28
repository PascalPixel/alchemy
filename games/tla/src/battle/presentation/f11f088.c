// Func_0811f088: The Broken Seal's BattleStaging_RunPsynergyCast
// (games/tbs/src/battle/battle/staging.c), ported: turns the camera to the
// caster (across the field or reversed when the plan's flags say so), raises
// the caster's hand with its element's glow, then either fails ("But the
// Psynergy was blocked!" or "...But doesn't have enough PP!") or plays the
// effect with the events. This game adds a second effect object
// (Func_0811e3ac, shown by Func_0811e7dc, put back and freed by
// Func_0811e830), sets the caster's mode from the battle work's byte 0x869,
// and skips the playback callback and its wait when the plan's word at 100 is
// set. mode: 1 a Djinni, 2 an enemy's cast. The two fixed yaws are stores in
// each arm of an if (as a ternary or one value the arms are folded into a
// default and an override).
#include "types.h"
#include "battle.h"
#include "battle_unit.h"
#include "battle_type.h"
#include "battle_work.h"
#include "battle_display.h"
#include "battle_event.h"
#include "task.h"
#include "motobj.h"
#include "ram_buffer.h"
#include "mem.h"

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

s32 ArcTan2(s32 x, s32 y);
void Actor_SetMode(struct MotionObject *object, s32 mode);
void PartyStatus_DrawFar(s32 flags);
void *Func_0811e3ac(struct BattlePlan *plan,
                    struct BattlePresentationWork *work);
void Func_0811ddd8(struct BattlePlan *plan,
                   struct BattlePresentationWork *work);
void Func_0811e7dc(void *saved);
void Func_0811e830(void *saved);
void Sound_PlayFar(s32 cue);
void Func_08127308(s32 unitId, s32 element, s32 mode, s32 value);
void Func_0812756c(void);
void Func_08138008(struct BattlePresentationWork *work);
void Func_08138018(struct BattlePresentationWork *work);
void BattlePlayback_WaitForReady(void);
void BattleEvent_Playback(void);
void ObjMotRst(s32 slot);

s32 Func_0811f088(struct BattlePlan *plan, s32 mode) {
  struct BattlePresentationWork work;
  struct BattlePresentationTransition *transition =
      Ram_HeapSlots->transitionWork;
  struct MotionObject *object;
  struct HeapSlots *heap;
  void *saved;
  s32 isDjinni;
  s32 targetIndex;

  if (plan->presentationFlags & 0x40000) {
    if (plan->actorId <= 7)
      transition->targetYaw = -0x2000;
    else
      transition->targetYaw = 0x5000;
    transition->frames = 60;
  } else {
    struct MotionObject *actor = BattleObject_Get(plan->actorId)->object;
    u32 target = plan->targetIds[0];
    u32 actorId = plan->actorId;
    s32 angle = (u16)ArcTan2(actor->x, actor->z);
    s32 targetYaw;

    if (actorId <= 7)
      targetYaw = angle - 0x1800;
    else
      targetYaw = angle + 0x1800;
    targetYaw = (s16)targetYaw;
    targetYaw += ((actorId <= 7 ? 0x2000 : -0x2000) - targetYaw) * 3 / 4;
    if (!((actorId <= 7) ^ (target <= 7)))
      targetYaw = actorId <= 7 ? 0x2400 : -0x2400;
    if (transition->targetYaw != targetYaw)
      transition->targetYaw = targetYaw;
  }
  if (plan->presentationFlags & 0x80000) {
    if (plan->actorId <= 7)
      transition->targetYaw = -0x2000;
    else
      transition->targetYaw = 0x2000;
    transition->frames = 60;
  }

  Func_0811ddd8(plan, &work);
  isDjinni = mode & 1;
  if (isDjinni)
    work.flags = 1;
  BattleHighlight_SetModes(0, 0);
  heap = Ram_HeapSlots;
  PartyStatus_DrawFar(heap->battleWork->partyStatus & ~1);
  saved = Func_0811e3ac(plan, &work);
  object = BattleObject_Get(work.actor)->object;
  Actor_SetMode(object, ((u8 *)heap->battleWork)[0x869]);
  Actor_SetAnimationSpeedFar((struct ActorView *)object, 16);
  Sound_PlayFar(154);
  if (mode & 2)
    Func_08127308(work.actor, plan->element, 1, 0);
  else if (!isDjinni)
    Func_08127308(work.actor, plan->element, 0, 0);
  if (plan->targetIds[0] <= 7)
    work.side = 1;
  else
    work.side = 0;

  for (targetIndex = 0; targetIndex != work.count; targetIndex++) {
    struct CastAnimation *animation =
        GetMotionRecord(BattleObject_Get(work.actors[targetIndex])->object, 0);
    s32 entryIndex;

    for (entryIndex = 0; entryIndex != animation->count - 1; entryIndex++)
      work.params[targetIndex][entryIndex] =
          animation->entries[entryIndex]->param;
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
    if (((s32 *)plan)[25] == 0)
      Task_Add(BattleEvent_Playback, TASK_PASS_VBLANK);
    if (work.kind) {
      if (saved)
        Func_0811e7dc(saved);
      if (plan->presentationFlags & 0x4000)
        Func_08138008(&work);
      else
        Func_08138018(&work);
    } else {
      Func_0812756c();
    }
    if (saved) {
      Func_0811e830(saved);
      Heap_Release(saved);
    }
    if (((s32 *)plan)[25] == 0)
      BattlePlayback_WaitForReady();
    Actor_SetMode(object, 1);
    for (targetIndex = 0; targetIndex != work.count; targetIndex++)
      ObjMotRst(work.actors[targetIndex]);
  }
  return 0;
}
