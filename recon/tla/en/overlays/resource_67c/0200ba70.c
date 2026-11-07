// Matches (tla-en, 2026-11-07).
// Shaped as its matched twin Func_02003a70 in games/tla/src/scene/aqua_rock_inside/s39f0.c:
// the entry pointer set at its declaration and stepped in the loop header.
#include "../../../../../games/tla/src/scene/aqua_rock_inside/local.h"

void Func_02003a70(void) {
  struct VentWork *work = VENT_WORK;
  struct VentEntry *entry = work->entries;
  s32 i;

  work->hook();
  for (i = 15; i >= 0; i--, entry++) {
    if (entry->active != 0) {
      if (entry->timer <= 0) {
        Func_020039f0(&entry->x, entry->value);
        entry->timer = 9;
      } else {
        entry->timer--;
      }
    }
  }
}
