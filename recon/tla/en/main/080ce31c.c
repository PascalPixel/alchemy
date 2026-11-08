// Differ 14 of 146 (tla-en, 2026-11-08, -mtune=gba): flags, dead in the
// nearby arm, holds the leader for Func_080cdac0 (20 to 14; a leader local
// stays 20), which puts flags in r1 as the native has it. Left: the shared -1
// is built in r3 before special is stored, the native builds it in r0 after;
// the -1s chained, reordered or named once, special or best set later, do no
// better (14 to 34); 20 permuter minutes found nothing below 14.
// Picks
// the Psynergy event (kind 5) for kind in front of the controlled owner: the
// first facing match, else the nearest of the area events (target | 0x100),
// else 0x200 for a catch-all, else -1. Rewritten in
// Event_FindFacingTrigger's shape (☀️'s loop foot, the facing test leaving
// through a goto). Before 2026-11-08 also tried: flags declared elsewhere or
// as u32, selected before nearby (no change); the -1s assigned after
// the calls (46), special read late (37), the loop test as an assignment (35)
// or an if around for (;;) (61), control in a local (139).
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
        // flags is dead here and holds the leader for the call.
        s32 distance =
            Func_080cdac0(kind, flags = Party_GetLeaderActor(), target);

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
