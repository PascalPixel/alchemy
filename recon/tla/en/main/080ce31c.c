/* Pending ability-event selection trial, 2026-10-02.
 * EN ordinary compiler: 312 bytes versus the listing's 316-byte extent;
 * score 910 (18 register, 5 reordered, 2 inserted, 3 deleted).
 * Ordinary width/branch/result reshapes improved the initial score 1955;
 * a finite 128-rewrite search did not improve the final result.
 * Traverses 12-byte event records and chooses a focused or nearby target.
 * No steering device. EN-only listing score; complete six-edition linked
 * body, pools, call identities and storage ownership remain unproved.
 */
#include "TYPES.H"
#include "RAM_BUFFER.H"
struct PendingAction { u8 unknown_00[6]; u8 event_kind; u8 unknown_07[9]; };
struct PendingEventRecord { u32 key; u16 metadata; s16 condition; s32 action; };
struct PendingEventList { u8 unknown_00[16]; struct PendingEventRecord *events; };
struct PendingFacingObject { u8 unknown_00[6]; u16 facing; };
const struct PendingAction *BattleAction_Get(s32 action);
struct PendingFacingObject *ObjectTable_Get(u32 owner);
s32 EventRuntime_GetControlledOwner(void);
s32 Func_080cb09c(void);
s32 Func_080cb144(void);
s32 GameFlag_IsConditionActive(s32 condition);
s32 Func_080cdac0(s32 kind, s32 owner, s32 target);

s32 Func_080ce31c(s32 kind)
{
 struct PendingEventRecord *event = ((struct PendingEventList *)Ram_HeapSlots->event_work)->events;
 s32 facing = ObjectTable_Get(EventRuntime_GetControlledOwner())->facing;
 s32 focused = Func_080cb09c();
 s32 special = Func_080cb144();
 s32 best = 9999, fallback = -1, selected = -1, nearby = -1;

 while (event->key != -1) {
  s32 angle = (s16)event->metadata & 0xf000;
  s16 limited = event->metadata & 0x0800;
  s32 target = event->metadata & 255;
  s32 event_kind = BattleAction_Get((u8)(event->key >> 8))->event_kind;

  if ((event->key & 15) == 5 && event_kind == kind &&
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
