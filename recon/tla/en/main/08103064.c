// Draft, differ 62 of 126, register choice mostly: lists a member's djinn as
// packed entries (element << 5 | index, owner << 8 when listing all four
// elements, bit 15 when set), all elements when element is -1. The native
// keeps a row's set word in ip across the index loop and owner << 8 on the
// stack; here the set word is reread from a pointer to the set banks
// (differ 72 when read as banks.active[row] each time, 105 held in a local).
#include "types.h"
#include "owner_state.h"

#define DJINN_ENTRY_SET (-0x8000)

s32 Djinn_ListOwnerEntries(u16 *out, s32 owner, s32 element) {
  struct OwnerDjinnState *state = Unit_Get(owner);
  s32 count = 0;
  s32 row;
  s32 bit;
  u32 active;
  u32 *setBanks;

  if (element == -1) {
    for (row = 0; row < 4; row++) {
      setBanks = state->flags.banks.active;
      for (bit = 0; bit < 20; bit++) {
        active = setBanks[row];
        if (active & (1 << bit))
          out[count++] = (row << 5) | bit | DJINN_ENTRY_SET | (owner << 8);
        else if (state->flags.banks.owned[row] & (1 << bit))
          out[count++] = (row << 5) | bit | (owner << 8);
      }
    }
  } else {
    active = state->flags.banks.active[element];
    for (bit = 0; bit < 20; bit++) {
      if (active & (1 << bit))
        out[count++] = (element << 5) | bit | DJINN_ENTRY_SET;
      else if (state->flags.banks.owned[element] & (1 << bit))
        out[count++] = (element << 5) | bit;
    }
  }
  return count;
}
