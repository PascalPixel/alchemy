// Draft: differ 31 (listing 32 instructions, draft 31). The native reads the
// held id twice before the test, sign-extended into r6 and zero-extended into
// r4, ORs the halfword constant 0xffff (from the pool) into the second and
// stores it, and keeps a zero in r5 across the calls. The draft stores -1
// and reads the id once. Tried: a u16 pointer with |= and with a raw copy.
#include "../../../../games/tla/src/scene/mars_aerie/mars_aerie.h"

void Func_02004d50(s32 id) {
  u8 *work = Ram_HeapSlots->windowTiles;
  struct MarsAerieParty *party = (struct MarsAerieParty *)&gPartyState;
  s32 held = party->held;

  if (held == id) {
    work[0x138a] = 0;
    party->held |= 0xffff;
    Object_GetById(held)->update = NULL;
    Object_SetPartAttribute(Object_GetById(held), 0);
  }
}
