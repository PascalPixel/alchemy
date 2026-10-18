// Draft, differ 0 of 26 (English): the ⚓️ counterpart of ☀️
// Party_RemapCharacterIdByFlags (games/tbs/src/object/object.c) for
// character 8, who takes sprite 9 on map 1 and 11 elsewhere, one more under
// flag 0x8df. The map is compared with a pool word of 1, which a plain 1
// does not give (movs, differ 19), so it is written as an id symbol of value
// 1 like ☀️'s (s32)&SceneId_Clear; MapId_0001 is a placeholder until the map
// id tables name it (O2/O3), and the other editions are not checked here.
#include "types.h"
#include "party_state.h"

s32 GameFlag_Test(s32 flag);

// A linker symbol whose value is 1 (a map id, placeholder name).
extern u8 MapId_0001[];

s32 Func_080ca6a4(s32 character) {
  s32 sprite = character;

  if (sprite <= 8) {
    if (sprite == 8) {
      sprite = 9;
      if (gPartyState.map != (s32)MapId_0001)
        sprite = 11;
      if (GameFlag_Test(0x8df))
        sprite++;
    }
  }
  return sprite;
}
