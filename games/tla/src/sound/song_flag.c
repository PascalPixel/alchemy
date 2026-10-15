#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"

u16 Audio_GetBackgroundCue(void) { return gData_02005830; }

s32 SongFlag_Test(s32 song) {
  s32 mask;
  u8 *bytes;
  s32 value;

  mask = 1 << (7 & song);
  song = (song & 0x3ff) >> 3;
  bytes = gSongFlags;
  value = bytes[song] & mask;
  return (u32)(-value | value) >> 31;
}

void SongFlag_Set(s32 song) {
  s32 mask;
  s32 bits;
  u8 *bytes;

  mask = 1 << (7 & song);
  bits = 0x300;
  /* FAKEMATCH: ⚓️ loads the flag bytes between the halves of the 0x3ff mask */
  asm("" : "+l"(bits));
  bytes = gSongFlags;
  /* FAKEMATCH: and adds the low half of the mask only after that load */
  asm volatile("");
  song = (song & (bits + 0xff)) >> 3;
  bytes[song] = (u8)(bytes[song] | mask);
}

void SongFlag_Clear(s32 song) {
  s32 mask;
  s32 bits;
  u8 *bytes;

  mask = 1 << (7 & song);
  bits = 0x300;
  /* FAKEMATCH: ⚓️ loads the flag bytes between the halves of the 0x3ff mask */
  asm("" : "+l"(bits));
  bytes = gSongFlags;
  /* FAKEMATCH: and adds the low half of the mask only after that load */
  asm volatile("");
  song = (song & (bits + 0xff)) >> 3;
  bytes[song] = (u8)(bytes[song] & ~mask);
}
