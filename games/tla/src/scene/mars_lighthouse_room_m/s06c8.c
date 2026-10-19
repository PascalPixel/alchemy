#include "mars_lighthouse_room_m.h"

extern u16 gData_02003a7e;
extern s16 gData_02003a80;
extern s16 gData_02003a82;

void Func_02003178(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);

/// Moves the Func_0200080c cells for a new state: the cells of the old index
/// (gData_02003a80) are put back with (0, 122, 3, 3), gData_02003a7e becomes 0
/// for an odd state and 16 for an even one, the index becomes (state - 20) / 2
/// and its cells are set with (0, 125, 3, 3). Sound 560 plays when the low bit
/// of the state changes.
void Func_020006c8(s32 state) {
  s32 next;
  s32 odd;
  u16 *flash;

  if (state == gData_02003a82)
    return;
  if ((state ^ gData_02003a82) & 1)
    Func_020032b8(560);
  gData_02003a82 = state;
  /* FAKEMATCH: a forced temporary: the low bit is taken before the state is
   * copied for the next index, as in the native (in the test itself: differ
   * 12). */
  odd = state & 1;
  /* FAKEMATCH: through a pointer taken here, the flash's address loads before
   * 20 is subtracted and the odd case is the branch's fall-through, as in the
   * native (a direct store in each arm: differ 17; one store of a conditional
   * value: differ 4). */
  flash = (u16 *)&gData_02003a7e;
  next = state - 20;
  if (odd)
    *flash = 0;
  else
    *flash = 16;
  if (gData_02003a80 == 0)
    Call6((void (*)())Func_02003178, 0, 122, 3, 3, 17, 67);
  /* FAKEMATCH: each empty asm with a memory clobber keeps GCSE from carrying
   * one load of the index across the calls; the native reloads it for every
   * test (without them: differ 133). */
  asm("" : : : "memory");
  if (gData_02003a80 == 1)
    Call6((void (*)())Func_02003178, 0, 122, 3, 3, 8, 72);
  /* FAKEMATCH: as above. */
  asm("" : : : "memory");
  if (gData_02003a80 == 2)
    Call6((void (*)())Func_02003178, 0, 122, 3, 3, 17, 72);
  /* FAKEMATCH: as above. */
  asm("" : : : "memory");
  if (gData_02003a80 == 3)
    Call6((void (*)())Func_02003178, 0, 122, 3, 3, 8, 67);
  gData_02003a80 = next / 2;
  if (gData_02003a80 == 0)
    Call6((void (*)())Func_02003178, 0, 125, 3, 3, 17, 67);
  if (gData_02003a80 == 1)
    Call6((void (*)())Func_02003178, 0, 125, 3, 3, 8, 72);
  /* FAKEMATCH: as above. */
  asm("" : : : "memory");
  if (gData_02003a80 == 2)
    Call6((void (*)())Func_02003178, 0, 125, 3, 3, 17, 72);
  /* FAKEMATCH: as above. */
  asm("" : : : "memory");
  if (gData_02003a80 == 3)
    Call6((void (*)())Func_02003178, 0, 125, 3, 3, 8, 67);
}

/// Calls Func_02003178 with (0, 125, 3, 3) for four places, steps
/// gData_02003a7e through 12 and 8 to 0, then calls it with (0, 122, 3, 3) for
/// the same places.
void Func_0200080c(void) {
  Call6((void (*)())Func_02003178, 0, 125, 3, 3, 17, 67);
  Call6((void (*)())Func_02003178, 0, 125, 3, 3, 8, 72);
  Call6((void (*)())Func_02003178, 0, 125, 3, 3, 17, 72);
  Call6((void (*)())Func_02003178, 0, 125, 3, 3, 8, 67);
  WaitFrames(1);
  gData_02003a7e = 12;
  WaitFrames(12);
  gData_02003a7e = 8;
  WaitFrames(4);
  gData_02003a7e = 12;
  WaitFrames(4);
  gData_02003a7e = 8;
  WaitFrames(4);
  gData_02003a7e = 12;
  WaitFrames(4);
  gData_02003a7e = 0;
  WaitFrames(12);
  Call6((void (*)())Func_02003178, 0, 122, 3, 3, 17, 67);
  Call6((void (*)())Func_02003178, 0, 122, 3, 3, 8, 72);
  Call6((void (*)())Func_02003178, 0, 122, 3, 3, 17, 72);
  Call6((void (*)())Func_02003178, 0, 122, 3, 3, 8, 67);
  WaitFrames(1);
}
