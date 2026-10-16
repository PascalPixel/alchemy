// Near miss, differ 2 of 49 (ported from The Broken Seal's
// Party_ResolveTablePair; rows here are eight halfwords): the native reads
// the scene, starts the entrance's offset, loads the table and then finishes
// the offset (movs, ldr, lsls); with the block below the offset is finished
// before the table's load. Plain, the offset starts before the scene read
// (differ 4); the table loaded first or the block around the read differ 2
// or 45; permute (3 min) found nothing.
#include "types.h"
#include "party_state.h"

extern const s16 gParty_PairResolveRules[];

void Func_080ca2fc(void) {
  struct PartyState *party = &gPartyState;
  s16 first;
  s16 second;
  const s16 *entry;

  first = party->scene;
  do {
  } while (0);
  second = party->entrance;
  entry = gParty_PairResolveRules;

  while (entry[0] != -1) {
    if (entry[0] == first && (entry[1] == -1 || entry[1] == second)) {
      party->savedScene = entry[2];
      party->savedEntrance = entry[3];
      return;
    }
    entry += 8;
  }
}
