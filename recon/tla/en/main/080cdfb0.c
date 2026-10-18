// Draft of Event_FindFacingTrigger: differ 2 of 115 (tla-en, 2026-10-18).
// It finds the scene's first kind-4 event whose condition holds, whose
// facing range (when it has one) takes the controlled actor's angle, whose
// item filter takes source (when given), and whose target is the actor the
// leader can talk to (flag 0x10) or the object it is touching.
// Rewritten from The Broken Seal's form: The Lost Age asks
// EventRuntime_GetControlledOwner for the actor and Item_MatchesFilter for
// the event's item.
//
// Left: instruction order only (the -1 build of the list end test sits one
// place apart).

#include "types.h"
#include "ram_buffer.h"
#include "field_scene.h"
#include "object_runtime.h"
#include "event_work.h"

// The field's event work as this function reads it: the scene's event list.
struct FieldStepWork {
  u8 unk00[0x10];
  const struct SceneEvent *events;
};

s32 Field_FindNearbyTalkTarget(s32 owner);
s32 Func_080cb09c(void);
s32 GameFlag_IsConditionActive(s32 condition);
s32 Item_MatchesFilter(s32 item, s32 filter);

s32 Event_FindFacingTrigger(s32 source) {
  /* FAKEMATCH: retain the existing word-result collision call boundary;
     the true u8 declaration adds two narrowing shifts, four bytes here. */
  struct FieldStepWork *runtime = Ram_HeapSlots->eventWork;
  const struct SceneEvent *trigger = runtime->events;
  s32 facing = ObjectTable_Get(EventRuntime_GetControlledOwner())->angle;
  s32 selected = Field_FindNearbyTalkTarget(EventRuntime_GetControlledOwner());
  s32 collision;

  source &= 0x1ff;
  collision = Func_080cb09c();

  while (trigger->control != -1) {
    s32 highValue = (s16)(u16)trigger->trigger & 0xf000;
    s16 hasFacing = (u16)trigger->trigger & 0x0800;
    s32 lowValue = (u16)trigger->trigger & 0xff;

    if ((trigger->control & 0x0f) == 4 &&
        GameFlag_IsConditionActive(trigger->condition) != 0 &&
        (hasFacing == 0 || (u16)(highValue - facing + 0x17ff) <= 0x2ffe)) {
      s32 item = (u16)((s32)trigger->control / 256);

      if (source == 0 || Item_MatchesFilter(item, source)) {
        if ((trigger->control & 0x10) != 0) {
          if (lowValue == selected)
            return (s32)trigger;
        } else if (lowValue == collision) {
          return (s32)trigger;
        }
      }
    }
    trigger++;
  }
  return 0;
}
