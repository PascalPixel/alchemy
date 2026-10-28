#include "types.h"
#include "adventure.h"

extern const s16 gParty_PairResolveRules[];

/// The Broken Seal's Party_ResolveTablePair: finds the party's scene and
/// entrance in gParty_PairResolveRules (rows of eight halfwords, -1 ending
/// the table; an entrance of -1 takes any) and saves that row's scene and
/// entrance.
void Func_080ca2fc(void) {
  struct Adventure *party = &gAdventure;
  s16 first;
  s16 second;
  const s16 *entry;

  first = party->scene;
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
