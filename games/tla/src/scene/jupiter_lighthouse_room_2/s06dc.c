#include "types.h"
#include "adventure.h"

void Func_02002304(s32 x, s32 z, s32 a2, s32 a3);

void Func_020006dc(void) {
  struct Adventure *party = &gAdventure;

  /* FAKEMATCH: a one-pass block loads the adventure before its offset is
   * built (one reordered row). */
  do {
  } while (0);
  if (party->renderMode == 10)
    Func_02002304(0x1500000, 0x2500000, 0, 255);
}
