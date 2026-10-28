#include "mars_lighthouse_aerie.h"

const struct Trigger *Func_02000164(void) {
  if (gAdventure.entrance == 2)
    return (const struct Trigger *)gData_0200644c;
  if ((u16)(gAdventure.entrance - 3) <= 2)
    return (const struct Trigger *)gData_020066bc;
  return (const struct Trigger *)gData_0200626c;
}

void Func_020001a0(void) {
  Func_02004fac();
  Func_020057cc();
  {
    u8 *party = (u8 *)&gAdventure;

    /* FAKEMATCH: a one-pass block loads the adventure before its offset is
     * built (one reordered row). */
    do {
    } while (0);
    party[0x2c2] = 2;
  }
  Func_02005744(1);
  FieldEvent_WaitFrames(40);
  Func_02005010();
}
