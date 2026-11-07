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

void Actor_SetMode(struct MotionObject *object, s32 mode);
void PartyStatus_DrawFar(s32 flags);
void Sound_PlayFar(s32 cue);
void Func_08127308(s32 unitId, s32 element, s32 mode, s32 value);
void Func_0812756c(void);
void Func_08138008(struct BattlePresentationWork *work);
void Func_08138018(struct BattlePresentationWork *work);
void BattlePlayback_WaitForReady(void);
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
  u16 *list;

  Func_0811ddd8(plan, &work);
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

  BattleHighlight_SetModes(0, 0);
  PartyStatus_DrawFar(Ram_HeapSlots->battleWork->partyStatus & ~1);
  object = BattleObject_Get(actor)->object;
  REG_BLDCNT = 0x3f40;
  list = units;
  i = 0;
  count = BattleParty_ListActorIds(3, list);
  for (; i != count; i++) {
    if (units[i] == 254)
      continue;
    if ((mode & 2) || units[i] != actor) {
      u32 unit = units[i];

      if ((target <= 7) != (unit <= 7))
        BattleHighlight_SetSpriteMode(unit, 1);
    } else {
      Actor_SetMode(object, ((u8 *)Ram_HeapSlots->battleWork)[0x869]);
    }
  }
  if (!(mode & 2)) {
    Sound_PlayFar(154);
    Func_08127308(work.actor, plan->element, 0, 0);
  }
  if (mode & 1)
    BattleHighlight_SetSpriteMode(actor, 1);
  for (i = 0; i != 16; i++) {
    REG_BLDALPHA = (16 - i) | 0x1000;
    wait(1);
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
    Task_Add(BattleEvent_Playback, TASK_PASS_VBLANK);
    if (plan->presentationFlags & 0x8000)
      BattleEffect_RunSceneEffectFar((s32 *)&work);
    else if (plan->presentationFlags & 0x4000)
      Func_08138008(&work);
    else
      Func_08138018(&work);
    BattlePlayback_WaitForReady();
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
      BattleHighlight_SetSpriteMode(unit, 1);
  }
  for (i = 0; i != 16; i++) {
    REG_BLDALPHA = i | 0x1000;
    wait(1);
  }
  for (i = 0; i != count; i++)
    BattleHighlight_SetSpriteMode(units[i], 0);
  BattleCamera_SetView(0, 0, 0, 100);
  wait(1);
  return 0;
}
