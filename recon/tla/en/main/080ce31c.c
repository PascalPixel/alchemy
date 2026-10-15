#include "types.h"
#include "ram_buffer.h"
struct PendingAction { u8 unk00[6]; u8 eventKind; u8 unk07[9]; };
struct PendingEventRecord { u32 key; u16 metadata; s16 condition; s32 action; };
struct PendingEventList { u8 unk00[16]; struct PendingEventRecord *events; };
struct PendingFacingObject { u8 unk00[6]; u16 facing; };
const struct PendingAction *BattleActGet(s32 action);
struct PendingFacingObject *ObjectTable_Get(u32 owner);
s32 EventRuntime_GetControlledOwner(void);
s32 Func_080cb09c(void);
s32 Func_080cb144(void);
s32 GameFlag_IsConditionActive(s32 condition);
s32 Func_080cdac0(s32 kind, s32 owner, s32 target);

s32 Func_080ce31c(s32 kind)
{
 struct PendingEventRecord *event = ((struct PendingEventList *)Ram_HeapSlots->eventWork)->events;
 s32 facing = ObjectTable_Get(EventRuntime_GetControlledOwner())->facing;
 s32 focused = Func_080cb09c();
 s32 special = Func_080cb144();
 s32 best = 9999, fallback = -1, selected = -1, nearby = -1;

 while (event->key != -1) {
  s32 angle = (s16)event->metadata & 0xf000;
  s16 limited = event->metadata & 0x0800;
  s32 target = event->metadata & 255;
  s32 eventKind = BattleActGet((u8)(event->key >> 8))->eventKind;

  if ((event->key & 15) == 5 && eventKind == kind &&
      GameFlag_IsConditionActive(event->condition)) {
   if (limited) {
    s32 distance = (s16)(angle - facing);
    if (distance < 0) distance = -distance;
    if (distance > 0x17ff) {
     event++;
     continue;
    }
   }
   if (event->key & 0x80) {
    fallback = 0x200;
   } else if (event->key & 0x10) {
    s32 distance = Func_080cdac0(kind, EventRuntime_GetControlledOwner(), target);
    if (distance != -1 && best > distance) {
     nearby = target | 0x100;
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
  event++;
 }
 return selected != -1 ? selected : nearby != -1 ? nearby : fallback;
}
