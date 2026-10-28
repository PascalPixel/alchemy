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
s32 Party_GetLeaderActor(void);
s32 GameFlag_IsConditionActive(s32 condition);

struct TriggerEvent *Func_080ce458(s32 filter, s32 kind, s32 selector) {
  struct TriggerWork *work = Ram_HeapSlots->eventWork;
  struct TriggerEvent *event = work->events;
  u32 facing = ObjectTable_Get(Party_GetLeaderActor())->angle;
  s32 flags;

  if (selector == -1)
    return NULL;
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
      if (filter != 0xf0000005 && (event->control & 0xf000000f) != filter)
        goto next;
      flags = event->control;
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
