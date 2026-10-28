#include "types.h"
#include "io_write_queue.h"
#include "call.h"
#include "io_reg.h"

extern s16 Data_02003a7c;
extern s16 gData_02003a7e;
extern s16 gData_02003a80;

void Func_02003170(s32 a, s32 b, s32 c, s32 d, s32 x, s32 z);

/// Scheduled by Func_02000934: steps room 1's blend (Data_02003a7c) one toward
/// its target (gData_02003a7e) each frame and queues the BLDALPHA write; as it
/// reaches 0 or 16, sets the cells the step (gData_02003a80) selects. The cell
/// calls go through Call6, as in Func_020006c8 (s06c8.c).
void Func_0200055c(void) {
  s32 step;

  if (Data_02003a7c == gData_02003a7e)
    return;
  step = gData_02003a7e - Data_02003a7c;
  if (step < 0)
    step = -1;
  else
    step = 1;
  Data_02003a7c += step;
  IO_WRITE_QUEUE_PUSH(0x04000052, BLEND_ALPHA(Data_02003a7c, 16 - Data_02003a7c),
                      0x20000);
  if (Data_02003a7c == 0) {
    if (gData_02003a80 == 0)
      Call6((void (*)())Func_02003170, 34, 8, 3, 3, 17, 8);
    /* FAKEMATCH(barrier): each empty asm with a memory clobber keeps GCSE
     * from carrying one load of the step across the calls; the native reloads
     * it for every test (without the six of them: differ 98 of 164 in
     * tla-en). */
    asm("" : : : "memory");
    if (gData_02003a80 == 1)
      Call6((void (*)())Func_02003170, 30, 8, 3, 3, 8, 8);
    /* FAKEMATCH(barrier): as above. */
    asm("" : : : "memory");
    if (gData_02003a80 == 2)
      Call6((void (*)())Func_02003170, 34, 8, 3, 3, 17, 8);
    /* FAKEMATCH(barrier): as above. */
    asm("" : : : "memory");
    if (gData_02003a80 == 3)
      Call6((void (*)())Func_02003170, 30, 8, 3, 3, 8, 8);
  }
  /* FAKEMATCH(barrier): as above, so the blend is loaded again with its
   * address; without it, 13 of 164 instructions differ in tla-en. */
  asm("" : : : "memory");
  if (Data_02003a7c == 16) {
    if (gData_02003a80 == 0)
      Call6((void (*)())Func_02003170, 34, 0, 3, 3, 17, 8);
    /* FAKEMATCH(barrier): as above. */
    asm("" : : : "memory");
    if (gData_02003a80 == 1)
      Call6((void (*)())Func_02003170, 30, 4, 3, 3, 8, 8);
    /* FAKEMATCH(barrier): as above. */
    asm("" : : : "memory");
    if (gData_02003a80 == 2)
      Call6((void (*)())Func_02003170, 34, 4, 3, 3, 17, 8);
    /* FAKEMATCH(barrier): as above. */
    asm("" : : : "memory");
    if (gData_02003a80 == 3)
      Call6((void (*)())Func_02003170, 30, 0, 3, 3, 8, 8);
  }
}
