#include "types.h"
#include "scene_tables.h"
#include "event_inline.h"

void Func_0200113c(s32 actor, s32 target);

/// Places actors 10 to 19 in two columns, then sends actors 20 to 29 to them
/// one each.
void Func_02000958(void) {
  u32 i;
  /* FAKEMATCH: the native keeps the counter in r5 and the actor in r6; unpinned
   * the two swap (14 differ), and a do-while or a counter-derived actor
   * measured 14 to 29. */
  register s32 actor __asm__("r6");
  s32 target;

  Actor_SetPosition(14, PIXELS(136), PIXELS(258));
  Actor_SetPosition(19, PIXELS(184), PIXELS(258));
  Actor_SetPosition(13, PIXELS(120), PIXELS(226));
  Actor_SetPosition(18, PIXELS(200), PIXELS(226));
  Actor_SetPosition(12, PIXELS(120), PIXELS(194));
  Actor_SetPosition(17, PIXELS(200), PIXELS(194));
  Actor_SetPosition(11, PIXELS(136), PIXELS(162));
  Actor_SetPosition(16, PIXELS(184), PIXELS(162));
  Actor_SetPosition(10, PIXELS(136), PIXELS(138));
  Actor_SetPosition(15, PIXELS(184), PIXELS(138));
  target = 10;
  actor = 20;
  for (i = 0; i < 10; i++) {
    Actor_SetModeById(actor, 0);
    Func_0200113c(actor, target);
    target++;
    actor++;
  }
  MapGrid_CopyCellAttributes(0, 0, 2, 2, 9, 22);
  wait(1);
}
