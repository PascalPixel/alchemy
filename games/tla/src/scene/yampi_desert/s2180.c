#include "scene.h"

void Func_02002180(void) {
  if (*(s16 *)((u8 *)Ram_HeapSlots->eventWork + 358) == 0 &&
      gFrameCount % 5 == 0) {
    struct SequenceParentWork *party = &gSequenceParentWork;

    /* FAKEMATCH: ⚓️ loads gSequenceParentWork's address before it builds the render
     * mode offset */
    asm volatile("");
    if (party->renderMode != 5 || gInput == 0)
      Func_020041f8(596);
  }
}
