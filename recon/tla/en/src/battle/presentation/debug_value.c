#include "types.h"
#include "input.h"
#include "ram_buffer.h"
#include "edition.h"

// What Select picks: another value in the Japanese release.
#if LANGUAGE == LANGUAGE_JAPANESE
#define DEBUG_VALUE_SELECT 411
#else
#define DEBUG_VALUE_SELECT 544
#endif

struct DebugBattleWork {
  u8 unk000[0x854];
  s32 debugValue;
};

void wait(s32 frames);

/// A debug choice of a battle value: with Start held, the d-pad and L/R step
/// the battle work's word at 0x854 (by 1, 10 and 100) until A takes it; Select
/// held picks DEBUG_VALUE_SELECT and R held 399; otherwise value comes back
/// unchanged.
s32 Func_081269f8(s32 value) {
  if (gInput.held & 8) {
    while (1) {
      struct DebugBattleWork *work =
          (struct DebugBattleWork *)Ram_HeapSlots->battleWork;

      if (gInput.repeat & 0x20)
        work->debugValue--;
      if (gInput.repeat & 0x10)
        work->debugValue++;
      if (gInput.repeat & 0x200)
        work->debugValue -= 100;
      if (gInput.repeat & 0x100)
        work->debugValue += 100;
      if (gInput.repeat & 0x40)
        work->debugValue -= 10;
      if (gInput.repeat & 0x80)
        work->debugValue += 10;
      if (gInput.pressed & 1) {
        value = work->debugValue;
        break;
      }
      wait(1);
    }
  }
  if (gInput.held & 4)
    value = DEBUG_VALUE_SELECT;
  /* FAKEMATCH(volatile): the held buttons read again for the R test; as
   * gInput.held, 11 of 114 instructions differ in tla-en. */
  if (*(volatile u32 *)&gInput.held & 0x100)
    value = 399;
  return value;
}
