// Draft, differ 8 of 92: chooses the battle music for slot of zone, as ☀️
// BattleFx_SelectBattleCue (games/tbs/src/object/object.c): link battles
// (flag 0x16c) take music 79, otherwise Data_080efd1c lists each song (bit
// 15 set) followed by the (zone << 4) + slot keys that use it, ended by 0.
// New in ⚓️: with no song found, an owner override without flag 0x8df takes
// 56, else Func_080ca5a0 picks 50, 750, or 49/50. Stored in sceneCue.
// Left: instruction order and register choice in the last case only (the
// native computes 50 - flag in r6 and builds the store offset after it).
// Tried: the last case as a conditional (same).
#include "types.h"
#include "party_state.h"

s32 GameFlag_Test(s32 flag);
s32 Func_080ca5a0(s32 kind);

extern const u16 Data_080efd1c[];

void BattleFx_SelectBattleCue(s32 zone, s32 slot) {
  s16 key = (zone << 4) + slot;
  const u16 *table = Data_080efd1c;
  u16 music;
  u16 value;

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
    if (gPartyState.ownerOverride != 0 && !GameFlag_Test(0x8df))
      music = 56;
    else if (Func_080ca5a0(4))
      music = 50;
    else if (Func_080ca5a0(0))
      music = 750;
    else
      music = Func_080ca5a0(5) ? 49 : 50;
  }
  gPartyState.sceneCue = music;
}
