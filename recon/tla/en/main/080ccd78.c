// Matches: differ 0 of 127 (tla-en, 2026-11-06); tla-ja and tla-it also 0.
// Twin of ☀️'s Scene_FindTrigger (games/tbs/src/field/field_sequence/triggers.c):
// returns the first of the scene's events (12-byte entries, flags -1 ends) of
// kind for target whose condition holds, with the push (0x200, 12 or with 0x800
// 2 frames) and facing (0x400) requirements. New in ⚓️: kinds 8 and 9 match any
// target, there is no search-ignores-condition test, and the facing test alone
// is skipped while gMainRoutineWork.renderMode is 2.
// Closed 2026-11-06 (permuter): mode declared at the top of the loop body,
// not at function scope, swaps event into r5 and matches into r6; before
// that, differ 26, one register swap.
// Taken from the twin: flags declared before facing, touchRequirements. The
// mode local is what closes the skeleton: gMainRoutineWork's pool load moves
// ahead of the heap slot's loads and the mode address takes r1. Data_02000452
// is renderMode (0x02000240 + 0x212): &Data_02000452 in the same local gives
// differ 8 with the skeleton gap 3 and 3, the first pool load still after the
// heap slot's loads.
// Tried, skeleton 0 and differ 26 unchanged: a routine local for
// gMainRoutineWork; const u8 *mode; s32 facing; if (...) matches = 1 in the
// tails; do/while for the loop; register on event. Worse: mode declared before
// flags (130 instructions); mode in the inner block (131); matches at function
// scope (gap 1 and 1); matches after minimumPushFrames (gap 1 and 1); flags
// reread before the -1 test (gap 11 and 8); a for loop (differ 31 to 41, gap 1);
// the 0xf000 mask inline without assigning flags (differ 61, the facing tails
// no longer shared).
// Under -mthumb-slow-loads (2026-10-26) no empty asm before any one statement
// lowered the older form's differ.
#include "types.h"
#include "ram_buffer.h"
#include "main_routine_work.h"

s32 GameFlag_IsConditionActive(s32 condition);
void *ObjectTable_Get(s32 id);

struct SceneEventEntry {
  s32 flags;
  u8 value;
  u8 unk05;
  s16 condition;
  u32 result;
};

struct EventWorkView {
  u8 unk000[0x10];
  struct SceneEventEntry *events;
  u8 unk014[0x170];
  s16 blockedSteps;
};

struct FacingView {
  u8 unk00[6];
  u16 facing;
};

struct SceneEventEntry *Func_080ccd78(s32 kind, s32 target) {
  struct EventWorkView *work = Ram_HeapSlots->eventWork;
  struct SceneEventEntry *event = work->events;
  s32 flags;
  u32 facing =
      ((struct FacingView *)ObjectTable_Get(gMainRoutineWork.currentOwner))->facing;
  flags = event->flags;
  while (flags != -1) {
    // Holds one address for its whole life: no device (S2).
    u8 *mode = &gMainRoutineWork.renderMode;

    if ((flags & 0xf) == kind &&
        (event->value == target || kind == 8 || kind == 9) &&
        GameFlag_IsConditionActive(event->condition)) {
      s32 matches = 0;
      s32 minimumPushFrames = 12;
      s32 touchRequirements;

      flags = event->flags;
      if (flags & 0x800)
        minimumPushFrames = 2;
      touchRequirements = flags & 0x600;

      switch (touchRequirements) {
      case 0:
        matches = 1;
        break;
      case 0x200:
        if (work->blockedSteps > minimumPushFrames)
          matches = 1;
        break;
      case 0x400:
        if (*mode != 2) {
          flags &= 0xf000;
          matches = (u16)((flags - facing) + 0x1fff) <= 0x3ffe;
        }
        break;
      case 0x600:
        if (work->blockedSteps > minimumPushFrames) {
          flags &= 0xf000;
          matches = (u16)((flags - facing) + 0x1fff) <= 0x3ffe;
        }
        break;
      }
      if (matches)
        return event;
    }
    event++;
    if (event->flags == -1)
      break;
    flags = event->flags;
  }
  return NULL;
}
