#include "types.h"
#include "call.h"
#include "iwram_call.h"

extern u32 gFrameCount;
extern s16 gData_02006e0c;
extern s16 gData_02006e0e;
extern u8 Data_02006e20[][32];
extern u8 gData_02006f20[][32];

void VramCache_Load(s32 entry, s32 size, void *source);

void Func_02004744(s32 progress) {
  /* FAKEMATCH(wrapper): calls forwarded through inlines (call.h's Call and
   * Value, this file's own); as direct calls, 2 of 181 instructions differ in
   * tla-en. */
  s32 level = ((progress * 30 + 0x8000) >> 16) + 1;
  s32 kind = 0;
  s32 count, base, i;

  if (progress != 0)
    kind = 100;
  if (level <= 15 && (gFrameCount & 15) <= 1)
    kind = 200;
  if (level <= 7 && (gFrameCount & 7) <= 1)
    kind = 200;
  if (kind + level == gData_02006e0c)
    return;
  gData_02006e0c = kind + level;
  Call2((void (*)())Iwram_ClearWords, (s32)(Data_02006e20), (s32)(0x100));
  {
    s32 (*copy)(void *, const void *, s32) = Iwram_CopyWords;

    if (kind == 0) {
      copy(Data_02006e20, gData_02006f20[8], 0x80);
      goto done;
    }
    if (kind == 100)
      copy(Data_02006e20, gData_02006f20[4], 0x80);
    else
      copy(Data_02006e20, gData_02006f20[8], 0x80);
  }
  base = 0;
  count = level;
  if (count > 7) {
    Iwram_CopyWords(Data_02006e20, gData_02006f20, 32);
    base = 8;
    count -= 8;
  }
  if (level > 15) {
    Iwram_CopyWords(Data_02006e20[1], gData_02006f20[1], 32);
    count = level;
    base = 16;
    count -= 16;
  }
  if (level > 23) {
    Iwram_CopyWords(Data_02006e20[2], gData_02006f20[2], 32);
    count = level;
    base = 24;
    count -= 24;
  }
  if (level > 30) {
    Iwram_CopyWords(Data_02006e20[3], gData_02006f20[3], 32);
    base = 0;
    count = 0;
  }
  if (count != 0) {
    i = 0;
    if (i >= count)
      goto done;
  loop:
    {
      s32 j;
      u8 *source = &gData_02006f20[base / 8][4] + i / 2;
      u8 *dest = &Data_02006e20[base / 8][4] + i / 2;
      /* FAKEMATCH(barrier): the empty asm on dest changes its allocation rank;
       * as no asm, 22 of 181 instructions differ in tla-en. */
      asm("" : "=r"(dest) : "0"(dest));

      j = 2;
      if ((i & 1) == 0) {
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
  VramCache_Load(gData_02006e0e, 0x100, Data_02006e20);
}
