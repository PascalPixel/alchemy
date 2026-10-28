// Draft, differ 706 of 479: packs the four units' stats, levels, Djinn,
// items and their counts into clear data, sized by the password kind. Left:
// loop forms (the native indexes its tables) and register choice.
#include "types.h"
#include "adventure.h"

extern u16 gData_02002700[];
extern s32 gData_020026f0[];
extern u16 gData_0200270c[];
extern u16 gData_0200271c[];

s32 GameFlag_Test(s32 flag);
u8 *Unit_Get(s32 id);

s32 Func_02001c7c(s32 unused, s32 mode, u8 *out) {
  s32 size = 11;
  u32 stats[8];
  u32 *stat;
  u32 flags, levels, djinn, found;
  s32 i, k, j;

  switch (mode) {
  case 0:
    size = 173;
    break;
  case 1:
    size = 39;
    break;
  case 2:
    size = 9;
    break;
  }
  for (i = 0; i != size; i++)
    out[i] = 0;
  levels = 0;
  djinn = 0;
  found = 0;
  flags = 0;
  for (i = 0; i != 8; i++) {
    stat = stats;
    stat[i] = 0;
  }
  for (i = 0; i != 6; i++) {
    if (GameFlag_Test(gData_02002700[i]))
      flags = (u8)(flags | 1 << i);
  }
  for (i = 0; i != 4; i++) {
    u8 *unit = Unit_Get(gData_020026f0[i]);
    s16 *hp = (s16 *)(unit + 16);
    u16 *pp = (u16 *)(unit + 24);
    u32 *djinni;
    u16 *items;
    s32 shift;

    if (hp[0] > 1999)
      hp[0] = 1999;
    if (hp[0] < 0)
      hp[0] = 0;
    if (hp[1] > 1999)
      hp[1] = 1999;
    if (hp[1] < 0)
      hp[1] = 0;
    if (pp[0] > 999)
      pp[0] = 999;
    if (pp[0] < 0)
      pp[0] = 0;
    if (pp[1] > 999)
      pp[1] = 999;
    if (pp[1] < 0)
      pp[1] = 0;
    if (pp[2] > 999)
      pp[2] = 999;
    if (pp[2] < 0)
      pp[2] = 0;
    if (unit[30] > 99)
      unit[30] = 99;
    if (unit[30] < 0)
      unit[30] = 0;
    stat[i * 2] = hp[0] << 21 | hp[1] << 10 | pp[0];
    stat[i * 2 + 1] = pp[1] << 22 | pp[2] << 12 | unit[30] << 4;
    if (unit[15] > 99)
      unit[15] = 99;
    if (unit[15] == 0)
      unit[15] = 1;
    levels |= unit[15] << (i * 8 - i);
    djinni = (u32 *)(unit + 248);
    for (k = 0, shift = 0; k != 4; k++, shift += 7)
      djinn += *djinni++ << shift;
    items = (u16 *)(unit + 216);
    for (k = 0; k != 15; k++, items++) {
      u32 item = *items & 0x1ff;
      u16 *wanted = gData_0200270c;

      for (j = 0; j != 8; j++) {
        if (item == *wanted++)
          found = (u8)(found | 1 << j);
      }
    }
  }
  if (mode == 0) {
    s32 pos = 39;
    s32 bit = 0;
    s32 shift;

    for (i = 0; i != 4; i++) {
      u16 *items = (u16 *)(Unit_Get(gData_020026f0[i]) + 216);
      u8 *p = out + pos;

      for (k = 0; k != 15; k++) {
        u32 item = *items++ & 0x1ff;

        p[0] += item >> (bit + 1);
        p[1] += item << (7 - bit);
        pos++;
        bit++;
        p++;
        if (bit == 7) {
          bit = 0;
          p++;
          pos++;
        }
      }
    }
    pos = 107;
    shift = -1;
    for (i = 0; i != 4; i++) {
      u8 *unit = Unit_Get(gData_020026f0[i]);
      u8 *p = out + pos;

      for (k = 0; k != 23; k++) {
        u16 *items = (u16 *)(unit + 216);
        u16 count = 0;

        for (j = 0; j != 15; j++) {
          u16 value = *items++;

          if ((value & 0x1ff) == gData_0200271c[k])
            count = value >> 11;
        }
        if (shift < 0) {
          *p += count >> -shift;
          shift += 8;
          pos++;
          p++;
        }
        *p += count << shift;
        shift -= 5;
        if (shift == -5) {
          pos++;
          p++;
          shift = 3;
        }
      }
    }
    out[165] = *(u16 *)((u8 *)&gAdventure + 18);
    out[166] = *(u32 *)((u8 *)&gAdventure + 16) >> 8;
    out[167] = *(u32 *)((u8 *)&gAdventure + 16);
  }
  if (mode != 2) {
    u8 *p = out + 8 + (mode != 0);

    for (i = 0; i != 2; i++) {
      u32 w0 = stat[i * 4];
      u32 w1, w2, w3;

      p[0] = w0 >> 24;
      p[1] = w0 >> 16;
      p[2] = w0 >> 8;
      p[3] = w0;
      w1 = stat[i * 4 + 1];
      p[4] = w1 >> 24;
      p[5] = w1 >> 16;
      p[6] = w1 >> 8;
      p[7] = w1;
      w2 = stat[i * 4 + 2];
      p[7] = w1 | w2 >> 28;
      p[8] = w2 >> 20;
      p[9] = w2 >> 12;
      p[10] = w2 >> 4;
      p[11] = w2 << 4;
      w3 = stat[i * 4 + 3];
      p[11] = w2 << 4 | w3 >> 28;
      p[12] = w3 >> 20;
      p[13] = w3 >> 12;
      p[14] = w3 >> 4;
      p += 15;
    }
  }
  out[0] = levels;
  out[1] = levels >> 8;
  out[2] = levels >> 16;
  out[3] = ((levels >> 20) & 0xf0) | (djinn & 15);
  out[4] = djinn >> 4;
  out[5] = djinn >> 12;
  out[6] = djinn >> 20;
  out[7] = flags;
  if (mode != 0)
    out[8] = found;
  return size;
}
