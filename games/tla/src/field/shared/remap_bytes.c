#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"

void SpriteColor_RemapPixels(u8 *buf, s32 cnt) {
  s32 n;
  u8 *p;
  u8 *table;

  p = buf;
  table = gRuntime_ByteRemapTable;
  n = cnt - 1;
  if (n != -1) {
    do {
      u32 index = *p;

      n -= 1;
      *p = table[index];
      p += 1;
    } while (n != -1);
  }
}
