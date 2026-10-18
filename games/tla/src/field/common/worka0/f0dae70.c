#include "worka0.h"

// Returns the slot among WORK_A4's eight that holds id, or NULL.
struct WorkA4Slot *Func_080dae70(s32 id) {
  struct WorkA4 *work = WORK_A4;
  struct WorkA4Slot *found = NULL;
  s32 index;
  /* FAKEMATCH: a dead first assignment of the index: ⚓️ copies its zero from
   * the result's register (adds r1, r5, #0); without it, differ 2 (movs r1,
   * #0). */
  index = 0;

  for (index = 0; index <= 7; index++) {
    if (work->slots[index].id == id) {
      found = &work->slots[index];
      break;
    }
  }
  return found;
}
