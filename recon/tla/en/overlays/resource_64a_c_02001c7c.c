// Draft, differ 481 of 479, skeleton gap 125 (tla-en, 2026-11-01)
// Packs the four units' base stats, levels, Djinn, items and their counts into
// clear data, sized by the password kind; FileScreen_PackPasswordData in
// games/tbs/src/scene/file_screen.c is its twin (confirmed: same clamps, bit
// layout and coin bytes). This one drops Item_GetDirect and adds the clamps
// below zero, and the twin's code differs in every loop's hoisting.
// Shape found: PRE reads the hp, pp and level rereads as copies of the store,
// which the twin also compiles to, and the native does not. A second read as
// (u16) into a value tested as (s16) is another expression and keeps the
// ldrsh. Item loops are gotos. The u32 *stat local is gone: stats[] alone gives
// the same code, and the native's sp+4 is GCSE's reaching copy of the frame
// address. A table start plus a cursor (wanted, cursor) gives the native's
// adds r2, r7, #0 in the item loop.
// Left: 1. Native r6 is one pseudo, i, for all seven outer loops, as the twin,
// but shared with the pack and count loops here it falls behind n in global.c
// (67 refs over 570 against 26 over 80) and takes r8, so packMember and
// countMember stay separate. Native n sits in ip, m in lr and count in r9.
// 2. mode must sit in r11, and found outranks it: 4 * 18 / 636 against
// 3 * 10 / 340. Regmove adds 8 refs when it ties found's ior result to found,
// since found dies there (fixup_match_1); at 10 refs found would rank below
// mode. Writing the operands the other way round compiles the same. 3. PRE
// hoists m << 1 and unit + 216 out of the count loops; the native spills unit
// to sp+0 and rebuilds both in the loop, and those pseudos take ip and lr.
// PRE also puts the frame address before the zero loop. 4. The party loop:
// as a for it strength-reduces i * 8 (add r8, r1) where the native keeps
// lsls r4, r6, #3 and subs r2, r4, r6, and as a goto (differ 472, gap 164) that
// part matches but item's 0x1ff turns from the pool into the split form: the
// native pools it in the item and pack loops, a HImode move, and splits it in
// the count loop, an SImode one. A u16 item gets the extra shifts (530).
// Lowest differ seen is 469 at gap 163 (a for over rows with its own counter,
// no quantity local); lowest gap is 120 at differ 516 (no cursor).
// Tried, no gain, over about 20,000 measured combinations: loop forms for the
// party, pack, count and row loops; which outer loops share i; one n or two;
// items++ position; pos, bit and p step orders; a local table in the count
// loop; item as s32, u32 or u16 and six spellings of its mask, one-constant
// masks included.
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
  s32 i;
  s32 packMember;
  s32 countMember;
  s32 pos;
  s32 bit;
  s32 j;
  s32 k;
  s32 n;
  s32 m;
  s32 count;
  s32 slot;

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
  stats[i] = 0;
  i++;
  if (i != 8)
    goto zero;
  i = 0;
test:
  if (GameFlag_Test(gData_02002700[i]))
    flags |= 1 << i;
  i++;
  if (i != 6)
    goto test;
  pos = 4;
  for (i = 0; i != 4; i++) {
    struct BattleUnit *unit = Unit_Get(gData_020026f0[i]);
    struct UnitBaseStats *base = (struct UnitBaseStats *)&unit->baseHp;
    u16 *items;
    u16 *wanted;
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
    stats[i * 2] = (u32)base->hp << 21 | base->pp << 10 | base->attack;
    *(u32 *)((u8 *)stats + pos) =
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
    wanted = gData_0200270c;
    j = 0;
  item:
    {
      s32 item = *items & 0x1ff;
      u16 *cursor = wanted;

      k = 0;
    want:
      if (item == *cursor++)
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

      slot = 0;
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
      slot++;
      if (slot != 15)
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

    i = 0;
  rows:
    {
      out[statsOffset] = stats[i * 4] >> 24;
      out[statsOffset + 1] = stats[i * 4] >> 16;
      out[statsOffset + 2] = stats[i * 4] >> 8;
      out[statsOffset + 3] = stats[i * 4];
      out[statsOffset + 4] = stats[i * 4 + 1] >> 24;
      out[statsOffset + 5] = stats[i * 4 + 1] >> 16;
      out[statsOffset + 6] = stats[i * 4 + 1] >> 8;
      out[statsOffset + 7] = stats[i * 4 + 1];
      out[statsOffset + 7] |= stats[i * 4 + 2] >> 28;
      out[statsOffset + 8] = stats[i * 4 + 2] >> 20;
      out[statsOffset + 9] = stats[i * 4 + 2] >> 12;
      out[statsOffset + 10] = stats[i * 4 + 2] >> 4;
      out[statsOffset + 11] = stats[i * 4 + 2] << 4;
      out[statsOffset + 11] |= stats[i * 4 + 3] >> 28;
      out[statsOffset + 12] = stats[i * 4 + 3] >> 20;
      out[statsOffset + 13] = stats[i * 4 + 3] >> 12;
      out[statsOffset + 14] = stats[i * 4 + 3] >> 4;
      statsOffset += 15;
      i++;
      if (i != 2)
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
