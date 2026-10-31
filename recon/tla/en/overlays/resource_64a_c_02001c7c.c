// Near miss, differ 586 of 479 (tla-en, 2026-10-31): packs the four units'
// base stats, levels, Djinn, items and their counts into clear data, sized by
// the password kind. The shape is the native's (base stats through one
// pointer at baseHp, the u16 and u8 floors through an int, (u32)(-mode | mode)
// >> 31 for the offset, unsigned coins shifts, goto loops where the native
// keeps its constants inside); a register-blind comparison leaves 221 lines.
// Left, all compiler-level: mode is spilled ([sp, #28], a 64-byte frame)
// where the native keeps it in r11 and out in r8, which moves every hi
// register; gcse load motion turns each clamp's reload after its store into
// a copy (as -fno-gcse the clamps come out as the native's); gcse's PRE
// hoists stat = stats out of the zero loop and unit->inventory and k * 2 out
// of goto loops; loop.c steps stat as a pointer where the native indexes it
// from its stack slot with an i * 8 + 4 (and i * 16 + 4) offset giv, and
// hoists the j loops' 1 and the count loop's table entry and 0x1ff.
// Tried, each worse: one variable for bit and shift (595), counters scoped
// per loop (661), the j loops as goto loops (612, 635), the scalars and stats
// in one local struct (757); stat as a one-element array changes nothing.
#include "types.h"
#include "main_routine_work.h"
#include "battle_unit.h"

struct UnitBaseStats {
  s16 hp;
  s16 pp;
  s16 hpGauge;
  s16 ppGauge;
  u16 attack;
  u16 defense;
  u16 agility;
  u8 luck;
};

extern u16 gData_02002700[];
extern s32 gData_020026f0[];
extern u16 gData_0200270c[];
extern u16 gData_0200271c[];

s32 GameFlag_Test(s32 flag);
void *Unit_Get(s32 owner);

s32 Func_02001c7c(s32 unused, s32 mode, u8 *out) {
  s32 size = 11;
  u32 levels, djinn, found, flags;
  u32 *stat;
  u32 stats[8];
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
  i = 0;
  if (size != 0) {
    u8 *p = out;

  clear:
    *p = 0;
    i++;
    p++;
    if (i != size)
      goto clear;
  }
  levels = 0;
  djinn = 0;
  found = 0;
  flags = 0;
  i = 0;
zero:
  stat = stats;
  stat[i] = 0;
  i++;
  if (i != 8)
    goto zero;
  i = 0;
test:
  if (GameFlag_Test(gData_02002700[i]))
    flags = (u8)(flags | 1 << i);
  i++;
  if (i != 6)
    goto test;
  for (i = 0; i != 4; i++) {
    struct BattleUnit *unit = Unit_Get(gData_020026f0[i]);
    struct UnitBaseStats *base = (struct UnitBaseStats *)&unit->baseHp;
    u32 *djinni;
    u16 *items;
    s32 shift;
    s32 value;

    if (base->hp > 1999)
      base->hp = 1999;
    if (base->hp < 0)
      base->hp = 0;
    if (base->pp > 1999)
      base->pp = 1999;
    if (base->pp < 0)
      base->pp = 0;
    if (base->attack > 999)
      base->attack = 999;
    value = base->attack;
    if (value < 0)
      value = 0;
    base->attack = value;
    if (base->defense > 999)
      base->defense = 999;
    value = base->defense;
    if (value < 0)
      value = 0;
    base->defense = value;
    if (base->agility > 999)
      base->agility = 999;
    value = base->agility;
    if (value < 0)
      value = 0;
    base->agility = value;
    if (base->luck > 99)
      base->luck = 99;
    value = base->luck;
    if (value < 0)
      value = 0;
    base->luck = value;
    stat[i * 2] = base->hp << 21 | base->pp << 10 | base->attack;
    stat[i * 2 + 1] =
        base->defense << 22 | base->agility << 12 | base->luck << 4;
    if (unit->level > 99)
      unit->level = 99;
    if (unit->level == 0)
      unit->level = 1;
    levels |= unit->level << (i * 7);
    djinni = unit->djinnAvailable;
    for (k = 0, shift = 0; k != 4; k++, shift += 7)
      djinn += *djinni++ << shift;
    items = unit->inventory;
    k = 0;
  item:
    {
      s32 item = *items & 0x1ff;
      u16 *wanted = gData_0200270c;

      for (j = 0; j != 8; j++) {
        if (item == *wanted++)
          found = (u8)(found | 1 << j);
      }
    }
    k++;
    items++;
    if (k != 15)
      goto item;
  }
  if (mode == 0) {
    s32 pos = 39;
    s32 bit = 0;
    s32 shift;

    for (i = 0; i != 4; i++) {
      struct BattleUnit *unit = Unit_Get(gData_020026f0[i]);
      u16 *items = unit->inventory;
      u8 *p = out + pos;

      k = 0;
    pack:
      {
        s32 item = *items++ & 0x1ff;

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
      k++;
      if (k != 15)
        goto pack;
    }
    pos = 107;
    shift = -1;
    for (i = 0; i != 4; i++) {
      struct BattleUnit *unit = Unit_Get(gData_020026f0[i]);
      u8 *p = out + pos;

      k = 0;
    count:
      {
        u16 *items = unit->inventory;
        u16 count = 0;

        for (j = 0; j != 15; j++) {
          u32 value = *items++;

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
      k++;
      if (k != 23)
        goto count;
    }
    out[165] = (u32)gMainRoutineWork.coins >> 16;
    out[166] = (u32)gMainRoutineWork.coins >> 8;
    out[167] = gMainRoutineWork.coins;
  }
  if (mode != 2) {
    u8 *p = out + (((u32)(-mode | mode) >> 31) + 8);

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
