#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"

u32 GameFlag_SetByte(u32 flag, u8 value) {
  u32 shifted = flag << 20;
  flag = shifted >> 23;
  gGameFlagBytes[flag] = value;
  return flag;
}

/// Increments the byte holding eight flags, saturating at 255, and returns
/// it.
s32 Func_08016d7c(s32 flag) {
  u8 *bytes = gGameFlagBytes;

  /* FAKEMATCH: ⚓️ loads the flag bytes before it shifts the flag id; without
   * the barrier the shift is scheduled first (differ 2) */
  asm volatile("");
  flag = ((u32)flag << 20) >> 23;
  if (bytes[flag] < 255)
    bytes[flag]++;
  return bytes[flag];
}

/// Decrements the byte holding eight flags, saturating at 0, and returns it.
s32 Func_08016d98(s32 flag) {
  u8 *bytes = gGameFlagBytes;

  /* FAKEMATCH: ⚓️ loads the flag bytes before it shifts the flag id; without
   * the barrier the shift is scheduled first (differ 2) */
  asm volatile("");
  flag = ((u32)flag << 20) >> 23;
  if (bytes[flag] != 0)
    bytes[flag]--;
  return bytes[flag];
}
