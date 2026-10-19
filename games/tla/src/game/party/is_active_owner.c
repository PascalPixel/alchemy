#include "types.h"
#include "party_state.h"

extern char ResourceId_GrasslandBBackdrop;
extern char ResourceId_RockyBeachBackdrop;
extern char ResourceId_DenseForestBackdrop;
extern char ResourceId_ShipDeckBackdrop;
extern char ResourceId_SnowfieldBBackdrop;
extern char ResourceId_BlizzardBackdrop;
extern char ResourceId_DesertBackdrop;

s32 Party_CountActiveOwnersFar(void);
s32 GameFlag_Test(s32 flag);

// The battle music table: each song as 0x8000 | song, followed by the
// encounters that use it as (zone << 4) + slot, ended by 0.
extern const s16 Data_080efd1c[];

// Chooses the battle backdrop for a terrain; the ship deck's is the default.
void Func_080ca514(s32 terrain) {
  u16 value;

  switch (terrain - 1) {
  case 0:
    value = (u32)&ResourceId_GrasslandBBackdrop;
    break;
  case 1:
    value = (u32)&ResourceId_DenseForestBackdrop;
    break;
  case 2:
    value = (u32)&ResourceId_DesertBackdrop;
    break;
  case 3:
  case 6:
    value = (u32)&ResourceId_RockyBeachBackdrop;
    break;
  case 4:
  case 5:
    value = (u32)&ResourceId_SnowfieldBBackdrop;
    break;
  case 10:
  case 11:
    value = (u32)&ResourceId_BlizzardBackdrop;
    break;
  default:
    value = (u32)&ResourceId_ShipDeckBackdrop;
    break;
  }
  gPartyState.special = value;
}

s32 Func_080ca5a0(s32 owner) {
  s32 count = Party_CountActiveOwnersFar();
  s32 i;

  if (count > 4)
    count = 4;
  for (i = 0; i < count; i++) {
    if (gPartyState.activeOwners[i] == owner)
      return 1;
  }
  return 0;
}

// Chooses the battle music for slot of zone (gPartyState.sceneCue), as ☀️'s
// function of the same name: a key takes the song listed last before it in
// Data_080efd1c, and flag 0x16c's battles always use music 79. With no song
// found, it is 56 while ownerOverride is set and flag 0x8df clear, else 50
// when owner 4 is active, 750 when owner 0 is, 49 when owner 5 is, or 50.
void BattleFx_SelectBattleCue(s32 zone, s32 slot) {
  s16 key = (zone << 4) + slot;
  const s16 *table = Data_080efd1c;
  u16 music;
  u16 value;
  struct PartyState *party;

  if (GameFlag_Test(0x16c)) {
    music = 79;
  } else {
    for (;;) {
      value = *table++;
      if (value == 0)
        break;
      if (value == (u16)key)
        break;
      if (value & 0x8000)
        music = value & 0xfff;
    }
  }
  if (music == 0) {
    party = &gPartyState;
    /* FAKEMATCH(one-pass block): the empty block after taking gPartyState's
     * address loads it before ownerOverride's offset is built, as the native
     * does; as plain statements the offset comes first, 2 of 92 instructions
     * differ in tla-en. */
    do {
    } while (0);
    if (party->ownerOverride != 0 && !GameFlag_Test(0x8df))
      music = 56;
    else if (Func_080ca5a0(4))
      music = 50;
    else if (Func_080ca5a0(0))
      music = 750;
    else if (Func_080ca5a0(5))
      music = 49;
    else
      music = 50;
  }
  party = &gPartyState;
  /* FAKEMATCH(one-pass block): the empty block after taking gPartyState's
   * address loads it before sceneCue's offset is built, as the native does;
   * as plain statements the offset comes first, 2 of 92 instructions differ
   * in tla-en. */
  do {
  } while (0);
  party->sceneCue = music;
}
