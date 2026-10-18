// Draft, differ 11 of 114: debug choice of a battle value: with Start held,
// the d-pad and L/R step the battle work's word at 0x854 (by 1, 10 and 100)
// until A, which takes it; Select held instead picks 544, R held 399;
// otherwise value comes back unchanged. No ☀️ twin found. Left: other: after
// the Select test the native reloads gInput.held for the R test, where here
// the held word stays in r2 and only the mask is rebuilt (gcse reuses it
// across the join); with it, the first test loads held into r2 rather than
// r3, and the pool sits one word nearer.
#include "types.h"
#include "input.h"
#include "ram_buffer.h"

struct DebugBattleWork {
  u8 unk000[0x854];
  s32 debugValue;
};

void WaitFrames(s32 frames);

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
      WaitFrames(1);
    }
  }
  if (gInput.held & 4)
    value = 544;
  if (gInput.held & 0x100)
    value = 399;
  return value;
}
