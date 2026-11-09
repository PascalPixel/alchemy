#include "types.h"
#include "main_routine_work.h"
#include "battle_unit.h"
#include "event_inline.h"

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

void *Unit_Get(s32 owner);

s32 Func_02001c7c(s32 unused, s32 mode, u8 *out) {
  s32 size;
  u32 levels;
  u32 djinn;
  u8 found;
  u8 flags;
  u32 stats[8];
  s32 i;
  s32 pos;
  s32 bit;
  s32 j;
  s32 k;
  s32 n;
  s32 m;

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
  for (i = 0; i != size; i++)
    out[i] = 0;
  levels = 0;
  djinn = 0;
  found = 0;
  flags = 0;
  for (i = 0; i != 8; i++)
    stats[i] = 0;
  for (i = 0; i != 6; i++) {
    if (GameFlag_Test(gData_02002700[i]))
      flags |= 1 << i;
  }
  i = 0;
  pos = 4;
  for (; i != 4; i++) {
    struct BattleUnit *unit = Unit_Get(gData_020026f0[i]);
    struct UnitBaseStats *base = (struct UnitBaseStats *)&unit->baseHp;
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
    for (j = 0; j != 15; j++) {
      s32 item = unit->inventory[j] & 0x1ff;

      for (k = 0; k != 8; k++) {
        if (item == gData_0200270c[k])
          found |= 1 << k;
      }
    }
    pos += 8;
  }
  if (mode == 0) {
    pos = 39;
    bit = 0;
    for (i = 0; i != 4; i++) {
      struct BattleUnit *unit = Unit_Get(gData_020026f0[i]);

      for (n = 0; n != 15; n++) {
        u16 item = unit->inventory[n] & 0x1ff;

        out[pos] += item >> (bit + 1);
        out[pos + 1] += item << (7 - bit);
        pos++;
        bit++;
        if (bit == 7) {
          bit = 0;
          pos++;
        }
      }
    }
    pos = 107;
    bit = -1;
    for (i = 0; i != 4; i++) {
      struct BattleUnit *unit = Unit_Get(gData_020026f0[i]);

      for (m = 0; m != 23; m++) {
        s32 count = 0;
        u16 quantity;
        /* FAKEMATCH(dead): a halfword copy of the count nothing reads, which
         * keeps the zero-extension hoisted above the sign test; without it, 139
         * of 479 instructions differ in tla-en. */

        for (n = 0; n != 15; n++) {
          if ((unit->inventory[n] & 0x1ff) == gData_0200271c[m])
            count = unit->inventory[n] >> 11;
        }
        quantity = count;
        if (bit < 0) {
          out[pos] += (u16)count >> -bit;
          bit += 8;
          pos++;
        }
        out[pos] += (u16)count << bit;
        bit -= 5;
        if (bit == -5) {
          pos++;
          bit = 3;
        }
      }
    }
    out[165] = (u32)gMainRoutineWork.coins >> 16;
    out[166] = (u32)gMainRoutineWork.coins >> 8;
    out[167] = gMainRoutineWork.coins;
  }
  if (mode != 2) {
    s32 statsOffset = 8 + (mode != 0);

    for (i = 0; i != 2; i++) {
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
