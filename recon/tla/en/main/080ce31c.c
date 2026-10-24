// Draft, differ 20 of 146 (40 with fallback declared before selected): picks
// the Psynergy event (kind 5) for kind in front of the controlled owner: the
// first facing match, else the nearest of the area events (target | 0x100),
// else 0x200 for a catch-all, else -1. Rewritten in
// Event_FindFacingTrigger's shape (☀️'s loop foot, the facing test leaving
// through a goto). Left: the native builds the shared -1 in r0 after storing
// special, and loads each control word into r1; this builds -1 in r3 before
// that store and loads the word into r0. Tried: flags declared elsewhere or
// as u32 (no change).
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

const struct PendingAction *BattleAction_Get(s32 action);
s32 EventRuntime_GetControlledOwner(void);
s32 Func_080cb09c(void);
s32 Func_080cb144(void);
s32 GameFlag_IsConditionActive(s32 condition);
s32 Func_080cdac0(s32 kind, s32 owner, s32 target);

s32 Func_080ce31c(s32 kind) {
  struct TriggerWork *work = Ram_HeapSlots->eventWork;
  struct TriggerEvent *event = work->events;
  u32 facing = ObjectTable_Get(EventRuntime_GetControlledOwner())->angle;
  s32 focused = Func_080cb09c();
  s32 special = Func_080cb144();
  s32 best = 9999;
  s32 nearby = -1;
  s32 selected = -1;
  s32 fallback = -1;
  s32 flags;

  flags = event->control;
  while (flags != -1) {
    s32 high = event->trigger & 0xf000;
    s16 hasFacing = (u16)event->trigger & 0x800;
    s32 target = (u16)event->trigger & 0xff;
    s32 eventKind = BattleAction_Get((u16)flags >> 8)->eventKind;

    if ((event->control & 15) == 5 && eventKind == kind &&
        GameFlag_IsConditionActive(event->condition)) {
      if (hasFacing) {
        s32 distance = (s16)(high - facing);

        if (distance < 0)
          distance = -distance;
        if (distance > 0x17ff)
          goto next;
      }
      if (event->control & 0x80) {
        fallback = 0x200;
      } else if (event->control & 0x10) {
        s32 distance =
            Func_080cdac0(kind, EventRuntime_GetControlledOwner(), target);

        if (distance != -1 && best > distance) {
          nearby = 0x100;
          nearby |= target;
          best = distance;
        }
      } else if (kind == 30) {
        if (target == special) {
          selected = target;
          break;
        }
      } else if (target == focused) {
        selected = target;
        break;
      }
    }
  next:
    event++;
    if (event->control == -1)
      break;
    flags = event->control;
  }
  if (selected != -1)
    return selected;
  if (nearby != -1)
    return nearby;
  return fallback;
}
