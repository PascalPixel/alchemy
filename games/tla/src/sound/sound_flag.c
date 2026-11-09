#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"

u16 Sound_GetCurrentBgm(void) { return gData_02005830; }

s32 SoundFlag_Test(s32 sound) {
  s32 mask;
  u8 *bytes;
  s32 value;

  mask = 1 << (7 & sound);
  sound = (sound & 0x3ff) >> 3;
  bytes = gSoundFlags;
  value = bytes[sound] & mask;
  return (u32)(-value | value) >> 31;
}

void SoundFlag_Set(s32 sound) {
  s32 mask;
  s32 bits;
  u8 *bytes;

  mask = 1 << (7 & sound);
  bits = 0x300;
  bytes = gSoundFlags;
  sound = (sound & (bits + 0xff)) >> 3;
  bytes[sound] = (u8)(bytes[sound] | mask);
}

void SoundFlag_Clear(s32 sound) {
  s32 mask;
  s32 bits;
  u8 *bytes;

  mask = 1 << (7 & sound);
  bits = 0x300;
  bytes = gSoundFlags;
  sound = (sound & (bits + 0xff)) >> 3;
  bytes[sound] = (u8)(bytes[sound] & ~mask);
}
