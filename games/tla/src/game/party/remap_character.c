#include "types.h"
#include "party_state.h"
#include "scene_ids.h"

s32 GameFlag_Test(s32 flag);

// Picks the sprite a scene placement's character id shows, as ☀️'s function
// of the same name: character 8 becomes 9 on the world map and 11 elsewhere,
// one more once flag 0x8df is set.
s32 Party_RemapCharacterIdByFlags(s32 character) {
  s32 sprite;

  sprite = character;
  if (sprite <= 8) {
    if (sprite == 8) {
      sprite = 9;
      if (gPartyState.map != (s32)&SceneId_Ov64a)
        sprite = 11;
      if (GameFlag_Test(0x8df))
        sprite++;
    }
  }
  return sprite;
}
