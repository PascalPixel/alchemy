#include "mars_lighthouse_room_m.h"
#include "event_inline.h"

extern u16 gData_02003a7e;
extern s16 gData_02003a80;
extern s16 gData_02003a82;


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
    Event_PlaySound(560);
  gData_02003a82 = state;
  /* FAKEMATCH(temporary): the low bit held in a local, so it is taken before
   * the state is copied for the next index, as in the native; as the test
   * itself, differ 12 (of M not recorded). */
  odd = state & 1;
  /* FAKEMATCH(temporary): a local pointer to the flash, taken here, loads its
   * address before 20 is subtracted; as a direct store in each arm, differ 17
   * in tla-en (of M not recorded). */
  flash = (u16 *)&gData_02003a7e;
  next = state - 20;
  if (odd)
    *flash = 0;
  else
    *flash = 16;
  if (gData_02003a80 == 0)
    MapGrid_CopyCellsLayered(0, 122, 3, 3, 17, 67);
  if (gData_02003a80 == 1)
    MapGrid_CopyCellsLayered(0, 122, 3, 3, 8, 72);
  if (gData_02003a80 == 2)
    MapGrid_CopyCellsLayered(0, 122, 3, 3, 17, 72);
  if (gData_02003a80 == 3)
    MapGrid_CopyCellsLayered(0, 122, 3, 3, 8, 67);
  gData_02003a80 = next / 2;
  if (gData_02003a80 == 0)
    MapGrid_CopyCellsLayered(0, 125, 3, 3, 17, 67);
  if (gData_02003a80 == 1)
    MapGrid_CopyCellsLayered(0, 125, 3, 3, 8, 72);
  if (gData_02003a80 == 2)
    MapGrid_CopyCellsLayered(0, 125, 3, 3, 17, 72);
  if (gData_02003a80 == 3)
    MapGrid_CopyCellsLayered(0, 125, 3, 3, 8, 67);
}

/// Calls Func_0802b450 with (0, 125, 3, 3) for four places, steps
/// gData_02003a7e through 12 and 8 to 0, then calls it with (0, 122, 3, 3) for
/// the same places.
void Func_0200080c(void) {
  MapGrid_CopyCellsLayered(0, 125, 3, 3, 17, 67);
  MapGrid_CopyCellsLayered(0, 125, 3, 3, 8, 72);
  MapGrid_CopyCellsLayered(0, 125, 3, 3, 17, 72);
  MapGrid_CopyCellsLayered(0, 125, 3, 3, 8, 67);
  System_Wait(1);
  gData_02003a7e = 12;
  System_Wait(12);
  gData_02003a7e = 8;
  System_Wait(4);
  gData_02003a7e = 12;
  System_Wait(4);
  gData_02003a7e = 8;
  System_Wait(4);
  gData_02003a7e = 12;
  System_Wait(4);
  gData_02003a7e = 0;
  System_Wait(12);
  MapGrid_CopyCellsLayered(0, 122, 3, 3, 17, 67);
  MapGrid_CopyCellsLayered(0, 122, 3, 3, 8, 72);
  MapGrid_CopyCellsLayered(0, 122, 3, 3, 17, 72);
  MapGrid_CopyCellsLayered(0, 122, 3, 3, 8, 67);
  System_Wait(1);
}
