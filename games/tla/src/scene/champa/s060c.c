#include "types.h"
#include "scene_tables.h"
#include "event_inline.h"

void Func_0200060c(void) {
  s32 angle;

  Actor_ShowItemIconById(31, 415);
  /* FAKEMATCH: the local starts the facing before the other arguments, as the
   * native has (one reordered row as a plain argument). */
  angle = 0xc000;
  Actor_SetPositionAndFacing(31, PIXELS(312), PIXELS(593), angle);
  GameFlag_SetBit(0xa74);
}
