/// @file
/// The Lost Age's form of the field's trigger search (the Broken Seal's
/// Scene_FindTrigger).
#include "types.h"
#include "actor.h"
#include "event_work.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "scene_tables.h"

s32 GameFlag_IsConditionActive(s32 condition);

/// Returns the first of the scene's triggers of kind for target whose
/// condition holds and whose push (STEP_TRIGGER_FLAG_WALKING) and facing
/// (TRIGGER_FLAG_FACING) requirements are met, or NULL. Kinds 8 and 9 answer
/// any target, and the facing test alone is skipped while
/// gMainRoutineWork.renderMode is 2.
// XXX: what kinds 8 and 9 are; the triggers of the scenes that use them would
// settle it.
struct TriggerView *Func_080ccd78(s32 kind, s32 target) {
  struct EventWork *work = Ram_HeapSlots->eventWork;
  struct TriggerView *trigger = work->triggers;
  s32 flags;
  u32 facing = ObjectTable_Get(gMainRoutineWork.currentOwner)->angle;

  flags = trigger->flags;
  while (flags != -1) {
    // Holds one address for its whole life.
    u8 *mode = &gMainRoutineWork.renderMode;

    if ((flags & TRIGGER_KIND_MASK) == kind &&
        (trigger->value == target || kind == 8 || kind == 9) &&
        GameFlag_IsConditionActive(trigger->condition)) {
      s32 matches = 0;
      s32 minimumPushFrames = 12;
      s32 touchRequirements;

      flags = trigger->flags;
      if (flags & STEP_TRIGGER_FLAG_QUICK)
        minimumPushFrames = 2;
      touchRequirements =
          flags & (STEP_TRIGGER_FLAG_WALKING | TRIGGER_FLAG_FACING);

      switch (touchRequirements) {
      case 0:
        matches = 1;
        break;
      case STEP_TRIGGER_FLAG_WALKING:
        if (work->blockedSteps > minimumPushFrames)
          matches = 1;
        break;
      case TRIGGER_FLAG_FACING:
        if (*mode != 2) {
          flags &= TRIGGER_FACING_MASK;
          matches = (u16)((flags - facing) + 0x1fff) <= 0x3ffe;
        }
        break;
      case STEP_TRIGGER_FLAG_WALKING | TRIGGER_FLAG_FACING:
        if (work->blockedSteps > minimumPushFrames) {
          flags &= TRIGGER_FACING_MASK;
          matches = (u16)((flags - facing) + 0x1fff) <= 0x3ffe;
        }
        break;
      }
      if (matches)
        return trigger;
    }
    trigger++;
    if (trigger->flags == -1)
      break;
    flags = trigger->flags;
  }
  return NULL;
}
