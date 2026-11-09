#include "worka0.h"

/// Returns the first of WORK_A4's eight slots with no chain, or NULL.
struct WorkA4Slot *Func_080dae3c(void) {
  struct WorkA4 *work = WORK_A4;
  struct WorkA4Slot *found = NULL;
  s32 index;
  /* FAKEMATCH(dead): index set to 0 before the loop sets it again, so its
   * zero is copied from found's register (adds r1, r0, #0); without it, 2 of
   * 26 instructions differ in tla-en. */
  index = 0;

  for (index = 0; index <= 7; index++) {
    if (work->slots[index].root == NULL) {
      found = &work->slots[index];
      break;
    }
  }
  return found;
}
