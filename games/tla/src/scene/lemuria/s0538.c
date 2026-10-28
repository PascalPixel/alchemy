#include "types.h"
#include "adventure.h"
#include "scene_ids.h"

void Func_02001d74(void);
void Func_02001ccc(s32 scene, s32 entrance);
void GameFlag_SetBit(s32 flag);
s32 Func_02001d5c(s32 x, s32 z, s32 layer);
void Func_02001d64(s32 x, s32 z, s32 layer, s32 value);

void Func_02000538(void) {
  struct Adventure *party;

  Func_02001d74();
  Func_02001ccc((s32)&SceneId_Ov68b, 2);
  party = &gAdventure;
  /* FAKEMATCH: the loop note ends the scheduling block: ⚓️ loads the party
   * state's address before it builds the field's offset (2 differ without it).
   */
  do {
  } while (0);
  party->unk2c2 = 2;
}

void Func_02000560(void) {
  s32 x;
  s32 z;
  s32 flag;
  s32 value;
  s32 old;

  flag = 0x900;
  /* FAKEMATCH: ⚓️ shifts the flag's high part before it starts the two
   * coordinates; as a plain constant the shift comes after them (2 differ) */
  asm volatile("" : "+l"(flag));
  x = 0x3580000;
  z = 0x3f80000;
  GameFlag_SetBit(flag + 0x63);
  value = Func_02001d5c(x, z, 0);
  /* FAKEMATCH: the first z moves to a temporary so the second starts before the
   * first store call's arguments, as ⚓️ has it; with z reassigned after that
   * call its shift comes late (2 differ). */
  old = z;
  z = 0x4080000;
  Func_02001d64(x, old, 0, value | 64);
  Func_02001d64(x, z, 0, Func_02001d5c(x, z, 0) | 64);
}
