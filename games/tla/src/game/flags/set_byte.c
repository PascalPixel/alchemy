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

// フラグ 8 個ぶんのバイトを 1 ふやす (255 で止まる)
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

// フラグ 8 個ぶんのバイトを 1 へらす (0 で止まる)
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
