// Near miss, differ 8 of 136 (2026-10-18): finds the Psynergy event (kind
// 5) that Func_080ce31c's selector names, of kind, matching filter
// (0xf0000005 for any). Rewritten in Event_FindFacingTrigger's shape. Left:
// two scheduling swaps at the start (movs r3, #192 before the stored
// argument, movs r3, #1 before the facing load), and the filter's mismatch
// branches to the next event through beq over b where the native has bne to
// it and b to the join. Tried: the filter test as a ternary (35), the facing
// test inside the condition (35, 36).
#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"

struct TriggerEvent {
  s32 control;
  s16 trigger;
  s16 condition;
  u32 value;
};

struct TriggerWork {
  u8 unk000[0x10];
  struct TriggerEvent *events;
};

struct PendingAction {
  u8 unk00[6];
  u8 eventKind;
};

const struct PendingAction *BattleActGet(s32 action);
s32 EventRuntime_GetControlledOwner(void);
s32 GameFlag_IsConditionActive(s32 condition);

struct TriggerEvent *Func_080ce458(s32 filter, s32 kind, s32 selector) {
  struct TriggerWork *work = Ram_HeapSlots->eventWork;
  struct TriggerEvent *event = work->events;
  u32 facing = ObjectTable_Get(EventRuntime_GetControlledOwner())->angle;
  s32 flags;

  if (selector == -1)
    return NULL;
  flags = event->control;
  while (flags != -1) {
    s32 high = event->trigger & 0xf000;
    s16 hasFacing = (u16)event->trigger & 0x800;
    s32 target = (u16)event->trigger & 0xff;
    s32 eventKind = BattleActGet((u16)flags >> 8)->eventKind;

    if ((event->control & 15) == 5 && eventKind == kind &&
        GameFlag_IsConditionActive(event->condition)) {
      if (hasFacing) {
        s32 distance = (s16)(high - facing);

        if (distance < 0)
          distance = -distance;
        if (distance > 0x17ff)
          goto next;
      }
      if (filter != 0xf0000005) {
        if (((flags = event->control) & 0xf000000f) != filter)
          goto next;
      } else {
        flags = event->control;
      }
      {
        if (flags & 0x80) {
          if ((selector & 0xff00) == 0x200)
            return event;
        } else if (flags & 0x10) {
          if ((selector & 0xff00) == 0x100 && (selector & 0xff) == target)
            return event;
        } else if ((selector & 0xff00) == 0 && (selector & 0xff) == target) {
          return event;
        }
      }
    }
  next:
    event++;
    if (event->control == -1)
      break;
    flags = event->control;
  }
  return NULL;
}
