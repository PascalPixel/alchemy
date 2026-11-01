// Draft, differ 596 of 479, skeleton gap 150 (tla-en, 2026-11-01)
// Packs the four units' base stats, levels, Djinn, items and their counts into
// clear data, sized by the password kind; FileScreen_PackPasswordData in
// games/tbs/src/scene/file_screen.c is its twin. Goto loops keep constants
// inside where the native does, offset is the native's i * 8 + 4 in the party
// loop, one counter per loop lets each take a low register, and the used u16
// quantity brings the count's halfword shifts close.
// Left: mode is spilled (a 64-byte frame) because found outranks it in
// global.c (18 refs, the update sits three loops deep); the native keeps mode
// in r11 and out in r8, which moves every hi register. PRE turns the hp and pp
// reloads after their stores into copies, hoists stats out of the zero loop
// and leaves two extra s16 loads; loop.c hoists the count loop's table entry
// and 0x1ff, and steps i * 8 and the row loop's stats. The quantity's lsls is
// not hoisted above the sign test. These look compiler-level, not C.
// Tried: the hp and pp reloads through a cast to a union, whose alias set
// escapes PRE, with pos, bit and shift at function scope, reach gap 127, but
// that steers the compiler untagged, so it stays out; value = unit->level
// before the zero test gains 5 and is a forced temporary. Each of these
// loses: the count loop's j as a goto with a local table pointer (5 to 13),
// all loops as gotos (11), the row loop with offset (2), the party loop as a
// goto (10), pos, bit and shift at function scope without the union (8).
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
  s32 offset;
  u32 stats[8];
  s32 i, k, j;
  s32 cleared, zeroed, flagIndex, packMember, countMember, row;
  s32 heldIndex, slot, packSlot, stackItem, wantedIndex, jc;

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
  cleared = 0;
  if (size != 0) {
    u8 *p = out;

  clear:
    *p = 0;
    cleared++;
    p++;
    if (cleared != size)
      goto clear;
  }
  levels = 0;
  djinn = 0;
  found = 0;
  flags = 0;
  zeroed = 0;
zero:
  stat = stats;
  stat[zeroed] = 0;
  zeroed++;
  if (zeroed != 8)
    goto zero;
  flagIndex = 0;
test:
  if (GameFlag_Test(gData_02002700[flagIndex]))
    flags = (u8)(flags | 1 << flagIndex);
  flagIndex++;
  if (flagIndex != 6)
    goto test;
  offset = 4;
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
    *(u32 *)((u8 *)stat + offset) =
        base->defense << 22 | base->agility << 12 | base->luck << 4;
    if (unit->level > 99)
      unit->level = 99;
    if (unit->level == 0)
      unit->level = 1;
    levels |= unit->level << (i * 7);
    djinni = unit->djinnAvailable;
    for (heldIndex = 0, shift = 0; heldIndex != 4; heldIndex++, shift += 7)
      djinn += *djinni++ << shift;
    items = unit->inventory;
    slot = 0;
  item:
    {
      s32 item = *items & 0x1ff;
      u16 *wanted = gData_0200270c;

      wantedIndex = 0;
    want:
      if (item == *wanted++)
        found = (u8)(found | 1 << wantedIndex);
      wantedIndex++;
      if (wantedIndex != 8)
        goto want;
    }
    slot++;
    items++;
    if (slot != 15)
      goto item;
    offset += 8;
  }
  if (mode == 0) {
    s32 pos = 39;
    s32 bit = 0;
    s32 shift;

    for (packMember = 0; packMember != 4; packMember++) {
      struct BattleUnit *unit = Unit_Get(gData_020026f0[packMember]);
      u16 *items = unit->inventory;
      u8 *p = out + pos;

      packSlot = 0;
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
      packSlot++;
      if (packSlot != 15)
        goto pack;
    }
    pos = 107;
    shift = -1;
    for (countMember = 0; countMember != 4; countMember++) {
      struct BattleUnit *unit = Unit_Get(gData_020026f0[countMember]);
      u8 *p = out + pos;

      stackItem = 0;
    count:
      {
        u16 *items = unit->inventory;
        s32 count = 0;
        u16 quantity;

        for (j = 0; j != 15; j++) {
          u32 value = *items++;

          if ((value & 0x1ff) == gData_0200271c[stackItem])
            count = value >> 11;
        }
        quantity = count;
        if (shift < 0) {
          *p += quantity >> -shift;
          shift += 8;
          pos++;
          p++;
        }
        *p += quantity << shift;
        shift -= 5;
        if (shift == -5) {
          pos++;
          p++;
          shift = 3;
        }
      }
      stackItem++;
      if (stackItem != 23)
        goto count;
    }
    out[165] = (u32)gMainRoutineWork.coins >> 16;
    out[166] = (u32)gMainRoutineWork.coins >> 8;
    out[167] = gMainRoutineWork.coins;
  }
  if (mode != 2) {
    u8 *p = out + (((u32)(-mode | mode) >> 31) + 8);

    for (row = 0; row != 2; row++) {
      u32 w0 = stat[row * 4];
      u32 w1, w2, w3;

      p[0] = w0 >> 24;
      p[1] = w0 >> 16;
      p[2] = w0 >> 8;
      p[3] = w0;
      w1 = stat[row * 4 + 1];
      p[4] = w1 >> 24;
      p[5] = w1 >> 16;
      p[6] = w1 >> 8;
      p[7] = w1;
      w2 = stat[row * 4 + 2];
      p[7] = w1 | w2 >> 28;
      p[8] = w2 >> 20;
      p[9] = w2 >> 12;
      p[10] = w2 >> 4;
      p[11] = w2 << 4;
      w3 = stat[row * 4 + 3];
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
