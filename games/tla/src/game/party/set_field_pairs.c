#include "types.h"
#include "adventure.h"

void Party_SetFields1eeAnd1f0(u16 first, u16 second) {
  gAdventure.pair1ee[0] = first;
  gAdventure.pair1ee[1] = second;
}

void Party_SetFields1f2And1f4(u16 first, u16 second) {
  gAdventure.pair1f2[0] = first;
  gAdventure.pair1f2[1] = second;
}
