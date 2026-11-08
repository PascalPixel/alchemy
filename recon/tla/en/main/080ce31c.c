// Matches: differ 0 (tla-en, 2026-11-08)
// Every edition measures 0: tla-ja and tla-it at 080ce31c, tla-de at
// 080db31c, tla-es at 080d731c, tla-fr at 080d331c (pass the address there).
// Picks the Psynergy event (kind 5) for kind in front of the controlled owner:
// the first facing match, else the nearest of the area events
// (target | 0x100), else 0x200 for a catch-all, else -1. Written in
// Event_FindFacingTrigger's shape (☀️'s loop foot, the facing test leaving
// through a goto), as its neighbour Func_080ce458 is.
// The match came from the return: a result variable set by an if/else chain
// at the end, with a -1 default declared before the other -1s. CSE makes that
// default the -1 the others copy and the first loop test compares with; it is
// dead across the loop and copied into r0 at the return, so it takes r0, and
// flags takes r1 beside it, which also made the old trick of flags holding
// the leader for Func_080cdac0 unnecessary (the plain call matches) and let
// nearby be written as one or. The default is never read (every path sets
// result), so it is tagged dead; with no default, 20 of 146 differ. Review:
// S2 lets a 0 or NULL default go untagged; whether a -1 default counts as one
// is Pascal's call. result has to come first: after the other -1s (14 to 27),
// or before them in another order (4).
// Before: 14 of 146 with flags holding the leader and the -1s in nearby,
// selected and fallback (a reload into r3; the native's r0); the -1s chained,
// reordered, named once in a scoped local or assigned around the calls, best
// or special set later, facing, focused or special retyped, the loop test as
// an assignment or an if around for (;;), and 29 permuter minutes found nothing
// below 14; an asm barrier on a scoped -1 (46 to 57) was worse; a register
// pinned to r0 matched but is the worst device. A tail that leaves the default
// meaningful (else if (fallback != -1)) keeps result live across the loop (76).
#include "types.h"
#include "ram_buffer.h"
#include "actor.h"

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
s32 Func_080cb09c(void);
s32 Func_080cb144(void);
s32 GameFlag_IsConditionActive(s32 condition);
s32 Func_080cdac0(s32 kind, s32 owner, s32 target);

s32 Func_080ce31c(s32 kind) {
  struct TriggerWork *work = Ram_HeapSlots->eventWork;
  struct TriggerEvent *event = work->events;
  u32 facing = ObjectTable_Get(Party_GetLeaderActor())->angle;
  s32 focused = Func_080cb09c();
  s32 special = Func_080cb144();
  s32 best = 9999;
  /* FAKEMATCH(dead): result's -1 default, which every path below overwrites;
   * as s32 result; with no default, 20 of 146 instructions differ in tla-en. */
  s32 result = -1;
  s32 selected = -1;
  s32 nearby = -1;
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
        s32 distance = Func_080cdac0(kind, Party_GetLeaderActor(), target);

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
  next:
    event++;
    if (event->control == -1)
      break;
    flags = event->control;
  }
  if (selected != -1)
    result = selected;
  else if (nearby != -1)
    result = nearby;
  else
    result = fallback;
  return result;
}
