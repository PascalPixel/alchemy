// Draft: differ 31 (listing 32 instructions, draft 31). The native reads the
// held id twice before the test, sign-extended into r6 and zero-extended into
// r4, ORs the halfword constant 0xffff (from the pool) into the second and
// stores it, and keeps a zero in r5 across the calls. The draft stores -1
// and reads the id once. Tried: a u16 pointer with |= and with a raw copy.
#include "../../../../games/tla/src/scene/mars_lighthouse_aerie/mars_lighthouse_aerie.h"

void Func_02004d50(s32 id) {
  u8 *work = Ram_HeapSlots->windowTiles;
  struct MarsLighthouseAerieParty *party = (struct MarsLighthouseAerieParty *)&gMainRoutineWork;
  s32 held = party->held;

  if (held == id) {
    work[0x138a] = 0;
    party->held |= 0xffff;
    /* FAKEMATCH(one-pass): the statement in a once-only block, whose end holds the
     * order around it; as the plain statement, 31 of 32 instructions differ
     * in tla-en. */
    do {
      Actor_GetById(held)->update = NULL;
    } while (0);
    Actor_SetPartAttribute(Actor_GetById(held), 0);
  }
}
