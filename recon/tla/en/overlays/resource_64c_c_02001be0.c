// Draft, differ 78 of 181, skeleton gap 4 (tla-en, 2026-11-01)
// Redraws the world map gauge (a 0x100-byte palette and tile strip) when its
// level or blink changes. The same function as overlay 64b's Func_02004744.
// Control flow, loads and calls all match. The gotos are the form: a for or
// do loop, at either depth, hoists the masks and the row out of the loop, and
// `if (j > 3) goto next; goto even;` gives the native branch polarity. The
// local copy pointer loads the entry ahead of the kind test.
// Left is allocation, and the skeleton gap follows from it: global.c ranks
// the source pointer (refs weighted by the inner loops) above count, so source
// takes r5, count r6, level r5 and the buffer r6; the native has count and the
// buffer in r5, level and source in r6, and keeps row in r1 and half in r0
// with the row pointers in r2 and r3. Its schedule of the row block rests on
// that reuse of r3. Separate sourceRow and destRow locals give those pseudos
// but put source in r4 and j in r5 (gap 5). A 240 mask local does not help.
// The clear call's `adds r0` and `lsls r1` also stay swapped.
#include "types.h"
#include "call.h"
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
  Call2((void (*)())Iwram_ClearWords, (s32)(gData_02003fd8), (s32)(0x100));
  {
    s32 (*copy)(void *, const void *, s32) = Iwram_CopyWords;

    if (kind == 0) {
      copy(gData_02003fd8, gData_020040d8 + 0x100, 0x80);
      goto done;
    }
    if (kind == 100)
      copy(gData_02003fd8, gData_020040d8 + 0x80, 0x80);
    else
      copy(gData_02003fd8, gData_020040d8 + 0x100, 0x80);
  }
  base = 0;
  count = level;
  if (count > 7) {
    Iwram_CopyWords(gData_02003fd8, gData_020040d8, 32);
    base = 8;
    count -= 8;
  }
  if (level > 15) {
    Iwram_CopyWords(gData_02003fd8 + 0x20, gData_020040d8 + 0x20, 32);
    count = level;
    base = 16;
    count -= 16;
  }
  if (level > 23) {
    Iwram_CopyWords(gData_02003fd8 + 0x40, gData_020040d8 + 0x40, 32);
    count = level;
    base = 24;
    count -= 24;
  }
  if (level > 30) {
    Iwram_CopyWords(gData_02003fd8 + 0x60, gData_020040d8 + 0x60, 32);
    base = 0;
    count = 0;
  }
  if (count != 0) {
    i = 0;
    if (i >= count)
      goto done;
  loop:
    {
      s32 odd, j;
      s32 half = i / 2;
      u8 *source = gData_020040d8 + 4;
      s32 row = (base / 8) * 32;
      u8 *dest = gData_02003fd8 + 4;

      source += row;
      dest += row;
      source += half;
      dest += half;

      odd = i & 1;
      j = 2;
      if (odd == 0) {
        j = 1;
      even:
        *dest = (*dest & 240) | (*source & 15);
        j++;
        source += 4;
        dest += 4;
        if (j > 3)
          goto next;
        goto even;
      } else {
      oddLoop:
        *dest = (*dest & 15) | (*source & 240);
        j--;
        source += 4;
        dest += 4;
        if (j >= 0)
          goto oddLoop;
      }
    }
  next:
    i++;
    if (i < count)
      goto loop;
  }
done:
  VramCache_Load(gData_02003fc6, 0x100, gData_02003fd8);
}
