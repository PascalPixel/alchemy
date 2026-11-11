#include "types.h"
#include "scene_tables.h"
#include "event_inline.h"

void Func_0200060c(void) {
  s32 angle;

  Actor_ShowItemIconById(31, 415);
  /* FAKEMATCH(temporary): the facing in a local, which starts it before the
   * other arguments as the native has; as a plain argument, one row is
   * reordered. */
  angle = 0xc000;
  Actor_SetPositionAndFacing(31, PIXELS(312), PIXELS(593), angle);
  Event_SetFlag(0xa74);
}
