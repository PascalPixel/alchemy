#include "types.h"
#include "ram_buffer.h"
struct PendingAction { u8 unknown_00[6]; u8 event_kind; u8 unknown_07[9]; };
struct PendingEventRecord { u32 key; u16 metadata; s16 condition; s32 action; };
struct PendingEventList { u8 unknown_00[16]; struct PendingEventRecord *events; };
struct PendingFacingObject { u8 unknown_00[6]; u16 facing; };
const struct PendingAction *BtlActGet(s32 action);
struct PendingFacingObject *ObjectTable_Get(u32 owner);
s32 EventRuntime_GetControlledOwner(void);
s32 Func_080cb09c(void);
s32 Func_080cb144(void);
s32 GameFlag_IsConditionActive(s32 condition);
s32 Func_080cdac0(s32 kind, s32 owner, s32 target);

struct PendingEventRecord *Func_080ce458(u32 filter, s32 kind, s32 selector)
{
 struct PendingEventRecord *event = ((struct PendingEventList *)Ram_HeapSlots->event_work)->events;
 s32 facing = ObjectTable_Get(EventRuntime_GetControlledOwner())->facing;
 s32 low = selector & 255;
 s32 high = selector & 0xff00;

 if (selector == -1) return NULL;
 while (event->key != -1) {
  s32 angle = (s16)event->metadata & 0xf000;
  s16 limited = event->metadata & 0x0800;
  s32 target = event->metadata & 255;
  s32 event_kind = BtlActGet((event->key >> 8) & 255)->event_kind;

  if ((event->key & 15) == 5 && event_kind == kind &&
      GameFlag_IsConditionActive(event->condition)) {
   s32 distance = (s16)(angle - facing);
   if (distance < 0) distance = -distance;
   if ((!limited || distance <= 0x17ff) &&
       (filter == 0xf0000005 || (event->key & 0xf000000f) == filter)) {
    if (event->key & 0x80) {
     if (high == 0x200) return event;
    } else if (event->key & 0x10) {
     if (high == 0x100 && low == target) return event;
    } else if (high == 0 && low == target) {
     return event;
    }
   }
  }
  event++;
 }
 return NULL;
}
