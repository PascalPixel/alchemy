// Draft, differ 187 of 181: redraws the credits gauge (a 0x100-byte
// palette and tile strip) when its level or blink changes. Left: register
// allocation (the native reuses the level register for the source pointer
// and keeps the base in ip, and saves no r8).
#include "types.h"
#include "iwram_call.h"

extern u32 gFrameCount;
extern s16 gData_02003fc4;
extern s16 gData_02003fc6;
extern u8 gData_02003fd8[];
extern u8 gData_020040d8[];

void VramCache_Load(s32 entry, s32 size, void *source);

void Func_02001be0(s32 progress) {
  s32 level = ((progress * 30 + 0x8000) >> 16) + 1;
  s32 kind = 0;
  s32 count, base, i;

  if (progress != 0)
    kind = 100;
  if (level <= 15 && (gFrameCount & 15) <= 1)
    kind = 200;
  if (level <= 7 && (gFrameCount & 7) <= 1)
    kind = 200;
  if (kind + level == gData_02003fc4)
    return;
  gData_02003fc4 = kind + level;
  Iwram_ClearWords(gData_02003fd8, 0x100);
  if (kind == 0) {
    Iwram_CopyWords(gData_02003fd8, gData_020040d8 + 0x100, 0x80);
  } else {
    if (kind == 100)
      Iwram_CopyWords(gData_02003fd8, gData_020040d8 + 0x80, 0x80);
    else
      Iwram_CopyWords(gData_02003fd8, gData_020040d8 + 0x100, 0x80);
    base = 0;
    count = level;
    if (count > 7) {
      Iwram_CopyWords(gData_02003fd8, gData_020040d8, 32);
      base = 8;
      count -= 8;
    }
    if (level > 15) {
      Iwram_CopyWords(gData_02003fd8 + 0x20, gData_020040d8 + 0x20, 32);
      count = level - 16;
      base = 16;
    }
    if (level > 23) {
      Iwram_CopyWords(gData_02003fd8 + 0x40, gData_020040d8 + 0x40, 32);
      count = level - 24;
      base = 24;
    }
    if (level > 30) {
      Iwram_CopyWords(gData_02003fd8 + 0x60, gData_020040d8 + 0x60, 32);
      base = 0;
      count = 0;
    }
    if (count != 0) {
      for (i = 0; i < count; i++) {
        s32 row = (base / 8) * 32;
        u8 *source = gData_020040d8 + 4 + row + i / 2;
        u8 *dest = gData_02003fd8 + 4 + row + i / 2;
        s32 j;

        if (i & 1) {
          for (j = 2; j >= 0; j--) {
            *dest = (*dest & 15) | (*source & 240);
            source += 4;
            dest += 4;
          }
        } else {
          for (j = 1; j <= 3; j++) {
            *dest = (*dest & 240) | (*source & 15);
            source += 4;
            dest += 4;
          }
        }
      }
    }
  }
  VramCache_Load(gData_02003fc6, 0x100, gData_02003fd8);
}
