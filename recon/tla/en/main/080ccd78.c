// Under -mthumb-slow-loads (2026-10-26): differ 12 of 127 in tla-en,
// instruction order and register choice. No empty asm before any one statement
// lowers it.
// Draft, differ 16 of 127: ⚓️'s Scene_FindEvent, as ☀️'s
// (games/tbs/src/object/object.c): returns the first of the scene's events
// (12-byte entries, flags -1 ends) of kind for target whose condition holds,
// with the push (0x200, 12 or with 0x800 2 frames) and facing (0x400)
// requirements. New in ⚓️: kinds 8 and 9 match any target, there is no
// search-ignores-condition test, and the facing test alone is skipped while
// Data_02000452 is 2. Left: instruction order and register choice only:
// gPartyState's pool load goes to r6 in the native before the work slot's
// loads (r3 here), the -1 is built before the facing load, and
// Data_02000452's address takes r1 against r3. Tried: the loop as a for
// with (++event)->flags (differ 31); the 0xf000 mask inline without
// assigning flags (differ 61, the facing tails no longer shared).
#include "types.h"
#include "ram_buffer.h"
#include "party_state.h"

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

extern u8 Data_02000452;

struct SceneEventEntry *Func_080ccd78(s32 kind, s32 target) {
  struct EventWorkView *work = Ram_HeapSlots->eventWork;
  struct SceneEventEntry *event = work->events;
  u32 facing =
      ((struct FacingView *)ObjectTable_Get(gPartyState.currentOwner))->facing;
  s32 flags;

  flags = event->flags;
  while (flags != -1) {
    if ((flags & 0xf) == kind &&
        (event->value == target || kind == 8 || kind == 9) &&
        GameFlag_IsConditionActive(event->condition)) {
      s32 matches = 0;
      s32 minimumPushFrames = 12;

      flags = event->flags;
      if (flags & 0x800)
        minimumPushFrames = 2;
      switch (flags & 0x600) {
      case 0:
        matches = 1;
        break;
      case 0x200:
        if (work->blockedSteps > minimumPushFrames)
          matches = 1;
        break;
      case 0x400:
        if (Data_02000452 != 2) {
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
