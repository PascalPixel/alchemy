// Draft, differ 565 of 479, skeleton gap 125 (tla-en, 2026-11-01)
// Packs the four units' base stats, levels, Djinn, items and their counts into
// clear data, sized by the password kind; FileScreen_PackPasswordData in
// games/tbs/src/scene/file_screen.c is its twin (confirmed: same clamps, bit
// layout and coin bytes). This one drops Item_GetDirect and adds the clamps
// below zero, and the twin's code differs in every loop's hoisting.
// Shape found: PRE reads the hp, pp and level rereads as copies of the store,
// which the twin also compiles to, and the native does not. A second read as
// (u16) into a value tested as (s16) is another expression and keeps the
// ldrsh. Item loops and the row loop are gotos: the row loop as a real loop
// merges the frame address into a stepped pointer and moves the strb #7 and
// #11 stores. The party loop stays real: as a goto it reaches 479 instructions
// and differ 562, but loses the pooled 0x1ff (gap 138). Native r6 is one
// register for every outer counter, r10 for pos and the party offset.
// Left: 1. mode must sit in r11, and found outranks it in global.c: 4 * 18 /
// 646 against 3 * 10 / 345. The four refs in the want loop stay, as regmove
// folds any temporary back into found (tried), and a mode with no other uses
// cannot gain refs. A second found = 0 before the first variable set would
// double its live length again (local-alloc.c), but no plain form was found.
// 2. PRE hoists m << 1 and unit + 216 out of the count loops (pseudos in lr
// and r12 that the native gives to m and n), which spills out and every low
// counter after it. A goto entered at the bottom test does not stop it. 3. The
// native sets the frame address inside the zero loop and keeps it in sp+4; here
// PRE puts it before the loop. Everything left in the skeleton is these three
// and the movs and reloads they rotate.
// Tried, no gain: one shared i for all outer loops (gap 155), s32 quantity
// (differ 558, gap 130), the twin's unread quantity (128), a local table in
// the count loop, the pack loop's or count loop's outer as goto, a local index
// copy of m, the orig w0 to w3 row form (140), a one-constant hp maximum.
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
  s32 size;
  u32 levels;
  u32 djinn;
  u8 found;
  u8 flags;
  u32 stats[8];
  u32 *stat;
  s32 i;
  s32 packMember;
  s32 countMember;
  s32 row;
  s32 pos;
  s32 bit;
  s32 j;
  s32 k;
  s32 n;
  s32 m;
  s32 count;

  size = 11;
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
  j = 0;
  if (size != 0) {
    u8 *p = out;

  clear:
    *p = 0;
    j++;
    p++;
    if (j != size)
      goto clear;
  }
  levels = 0;
  djinn = 0;
  found = 0;
  flags = 0;
  j = 0;
zero:
  stat = stats;
  stat[j] = 0;
  j++;
  if (j != 8)
    goto zero;
  j = 0;
test:
  if (GameFlag_Test(gData_02002700[j]))
    flags |= 1 << j;
  j++;
  if (j != 6)
    goto test;
  pos = 4;
  for (i = 0; i != 4; i++) {
    struct BattleUnit *unit = Unit_Get(gData_020026f0[i]);
    struct UnitBaseStats *base = (struct UnitBaseStats *)&unit->baseHp;
    u16 *items;
    s32 value;

    if (base->hp > 1999)
      base->hp = 1999;
    value = (u16)base->hp;
    if ((s16)value < 0)
      base->hp = 0;
    if (base->pp > 1999)
      base->pp = 1999;
    value = (u16)base->pp;
    if ((s16)value < 0)
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
    stat[i * 2] = (u32)base->hp << 21 | base->pp << 10 | base->attack;
    *(u32 *)((u8 *)stat + pos) =
        (u32)base->defense << 22 | base->agility << 12 | base->luck << 4;
    if (unit->level > 99)
      unit->level = 99;
    value = unit->level;
    if (value == 0)
      unit->level = 1;
    levels |= unit->level << (i * 7);
    for (j = 0; j != 4; j++)
      djinn += unit->djinnAvailable[j] << (j * 7);
    items = unit->inventory;
    j = 0;
  item:
    {
      s32 item = *items & 0x1ff;
      u16 *wanted = gData_0200270c;

      k = 0;
    want:
      if (item == *wanted++)
        found |= 1 << k;
      k++;
      if (k != 8)
        goto want;
    }
    j++;
    items++;
    if (j != 15)
      goto item;
    pos += 8;
  }
  if (mode == 0) {
    pos = 39;
    bit = 0;
    for (packMember = 0; packMember != 4; packMember++) {
      struct BattleUnit *unit = Unit_Get(gData_020026f0[packMember]);
      u16 *items = unit->inventory;
      u8 *p = out + pos;

      n = 0;
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
      n++;
      if (n != 15)
        goto pack;
    }
    pos = 107;
    bit = -1;
    for (countMember = 0; countMember != 4; countMember++) {
      struct BattleUnit *unit = Unit_Get(gData_020026f0[countMember]);
      u8 *p = out + pos;

      m = 0;
    stack:
      {
        u16 *items = unit->inventory;
        u16 quantity;

        count = 0;
        n = 0;
      count:
        {
          u32 value = *items++;

          if ((value & 0x1ff) == gData_0200271c[m])
            count = value >> 11;
        }
        n++;
        if (n != 15)
          goto count;
        quantity = count;
        if (bit < 0) {
          *p += quantity >> -bit;
          bit += 8;
          pos++;
          p++;
        }
        *p += quantity << bit;
        bit -= 5;
        if (bit == -5) {
          pos++;
          p++;
          bit = 3;
        }
      }
      m++;
      if (m != 23)
        goto stack;
    }
    out[165] = (u32)gMainRoutineWork.coins >> 16;
    out[166] = (u32)gMainRoutineWork.coins >> 8;
    out[167] = gMainRoutineWork.coins;
  }
  if (mode != 2) {
    s32 statsOffset = 8 + (mode != 0);

    row = 0;
  rows:
    {
      out[statsOffset] = stat[row * 4] >> 24;
      out[statsOffset + 1] = stat[row * 4] >> 16;
      out[statsOffset + 2] = stat[row * 4] >> 8;
      out[statsOffset + 3] = stat[row * 4];
      out[statsOffset + 4] = stat[row * 4 + 1] >> 24;
      out[statsOffset + 5] = stat[row * 4 + 1] >> 16;
      out[statsOffset + 6] = stat[row * 4 + 1] >> 8;
      out[statsOffset + 7] = stat[row * 4 + 1];
      out[statsOffset + 7] |= stat[row * 4 + 2] >> 28;
      out[statsOffset + 8] = stat[row * 4 + 2] >> 20;
      out[statsOffset + 9] = stat[row * 4 + 2] >> 12;
      out[statsOffset + 10] = stat[row * 4 + 2] >> 4;
      out[statsOffset + 11] = stat[row * 4 + 2] << 4;
      out[statsOffset + 11] |= stat[row * 4 + 3] >> 28;
      out[statsOffset + 12] = stat[row * 4 + 3] >> 20;
      out[statsOffset + 13] = stat[row * 4 + 3] >> 12;
      out[statsOffset + 14] = stat[row * 4 + 3] >> 4;
      statsOffset += 15;
      row++;
      if (row != 2)
        goto rows;
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
