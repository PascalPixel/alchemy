#include "types.h"
#include "owner_state.h"

#define DJINN_ENTRY_SET (-0x8000)

s32 Djinn_ListOwnerEntries(u16 *out, s32 owner, s32 element) {
  struct OwnerDjinnState *state = UnitGet(owner);
  s32 count = 0;
  s32 row;
  s32 bit;
  s32 entry;

  if (element == -1) {
    for (row = 0; row < 4; row++) {
      for (bit = 0; bit < 20; bit++) {
        if (state->flags.banks.active[row] & (1 << bit)) {
          entry = (row << 5) | bit | DJINN_ENTRY_SET;
          entry |= owner << 8;
          out[count++] = entry;
        } else if (state->flags.banks.owned[row] & (1 << bit)) {
          out[count++] = (row << 5) | bit | (owner << 8);
        }
      }
    }
  } else {
    for (bit = 0; bit < 20; bit++) {
      if (state->flags.banks.active[element] & (1 << bit))
        out[count++] = (element << 5) | bit | DJINN_ENTRY_SET;
      else if (state->flags.banks.owned[element] & (1 << bit))
        out[count++] = (element << 5) | bit;
    }
  }
  return count;
}
