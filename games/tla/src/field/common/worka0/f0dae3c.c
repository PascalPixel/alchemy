#include "worka0.h"

// Returns the first of WORK_A4's eight slots with no chain, or NULL.
struct WorkA4Slot *Func_080dae3c(void) {
  struct WorkA4 *work = WORK_A4;
  struct WorkA4Slot *found = NULL;
  s32 index;
  /* FAKEMATCH: a dead first assignment of the index: ⚓️ copies its zero from
   * the result's register (adds r1, r0, #0); without it, differ 2 (movs r1,
   * #0). */
  index = 0;

  for (index = 0; index <= 7; index++) {
    if (work->slots[index].root == NULL) {
      found = &work->slots[index];
      break;
    }
  }
  return found;
}
