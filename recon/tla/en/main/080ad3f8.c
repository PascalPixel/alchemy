// Draft, differ 28 of 900 (192 before, 2026-10-26): Unit_Calculate, The
// Broken Seal's Unit_Calculate (games/tbs/src/game/game/units.c) on this
// game's unit. This game's changes, read from the listing: no second
// equipment bonus (no reset of 0x143 and no effect 24), the lighthouse
// switch on the unit's id at 0x14a over the eight characters, an enemy's
// (class 0) turns clamped to 4 rather than 2, and an enemy's HP and PP
// limits 16383 and 9999 where a character's are 1999; the Attack bonus needs
// no once-only block here. Left, instruction order only: the native issues
// the element loop's address add before the Attack load, the stage
// multiplies' next offset after the store, and the final copy loop's count
// at the head of its stores. A once-only block around the Defense stage
// (28 to 20) and an empty asm after the second equipment loop's
// Item_GetDirect (20 to 18) are the best devices found.
#include "mem.h"
#include "types.h"
#include "item.h"
#include "game_flags.h"
#include "owner_state.h"

struct OwnerStats {
  u8 unk000[0x10];
  s16 baseHp;
  s16 basePp;
  s16 hpRatio;
  s16 ppRatio;
  u16 baseAttack;
  u16 baseDefense;
  u16 baseAgility;
  u8 baseLuck;
  u8 baseTurns;
  u8 base20;
  u8 base21;
  u8 unk022[2];
  s16 baseElement[4][2];
  s16 maxHp;
  s16 maxPp;
  s16 hp;
  s16 pp;
  s16 atk;
  s16 def;
  s16 agi;
  u8 luck;
  u8 turns;
  u8 stat44;
  u8 stat45;
  u8 unk046[2];
  s16 element[4][2];
  u8 unk058[0x80];
  u16 equipment[15];
  u8 unk0f6[0x12];
  u32 djinn[4];
  u8 unk118[0x10];
  u8 character;
  u8 classId;
  u8 unk12a[2];
  s8 elementLevel[4];
  s8 curse;
  u8 unk131[2];
  s8 attackLevel;
  u8 unk134;
  s8 defenseLevel;
  u8 unk136;
  s8 resistLevel;
  u8 unk138[3];
  u8 unk13b[7];
  u8 bonus142;
  u8 bonus143;
  u8 extraTurn;
  u8 unk145[2];
  s8 agilityLevel;
  u8 unk148[2];
  u16 id;
};

struct DjinnDefinition {
  u8 unk00[4];
  s8 hp;
  s8 pp;
  s8 atk;
  s8 def;
  s8 agi;
  s8 luck;
};

struct ClassRecord {
  u8 unk00[8];
  u8 hp;
  u8 pp;
  u8 atk;
  u8 def;
  u8 agi;
  u8 luck;
};

struct StatWork {
  s32 hp;
  s32 pp;
  s32 atk;
  s32 def;
  s32 agi;
  s32 unused14;
  s32 luck;
  s32 turns;
  s32 stat20;
  s32 stat24;
  s32 element[4][2];
  s32 kind;
  s32 unused4c[2];
  s32 amount;
  struct ItemDefinition *item;
  s32 unused5c;
};

s32 __divsi3(s32 numerator, s32 denominator);

#define STAT_DIFF(a, b) ((a) - (b) < 0 ? (b) - (a) : (a) - (b))

void Unit_Calculate(s32 owner) {
  struct StatWork *work;
  struct OwnerStats *st;
  s32 i, j, el;
  s32 value;
  s32 flag;
  s32 kind;
  s32 amount;
  s32 hpCap;
  s32 ppCap;
  s16 old;

  work = Runtime_BumpAllocateAlternatePool(sizeof(struct StatWork));
  st = Unit_Get(owner);
  work->hp = st->baseHp;
  work->pp = st->basePp;
  work->atk = st->baseAttack;
  work->def = st->baseDefense;
  work->agi = st->baseAgility;
  work->luck = st->baseLuck;
  work->turns = st->baseTurns & 15;
  work->stat20 = st->base20;
  work->stat24 = st->base21;
  {
    s16 *src = st->baseElement[0];
    s32 *dst = work->element[0];
    for (i = 0; i < 4; i++) {
      dst[0] = src[0];
      dst[1] = src[1];
      src += 2;
      dst += 2;
    }
  }

  if (STAT_DIFF(st->maxHp * st->hpRatio / 0x4000, st->hp) > 1 ||
      STAT_DIFF(st->maxPp * st->ppRatio / 0x4000, st->pp) > 1) {
    st->hpRatio = 0x4000;
    st->ppRatio = 0x4000;
    st->hp = st->maxHp;
    st->pp = st->maxPp;
  }

  st->curse &= ~3;
  if (st->curse & 4)
    st->curse |= 1;
  if (st->extraTurn)
    work->turns++;
  st->bonus142 = 0;

  if (st->classId) {
    for (i = 0; i < 15; i++) {
      if (!(st->equipment[i] & 0x200))
        continue;
      work->item = Item_GetDirect(st->equipment[i]);
      if (work->item->flags & 1)
        st->curse |= 3;
      work->atk += work->item->primaryBonus;
      work->def += work->item->secondaryBonus;
      for (j = 0; j < 4; j++) {
        kind = work->item->effects[j].kind;
        amount = work->item->effects[j].amount;
        work->kind = kind;
        work->amount = amount;
        switch (work->kind) {
        case 0:
          break;
        case 1:
          work->hp += work->amount;
          break;
        case 2:
          work->stat20 += work->amount;
          break;
        case 3:
          work->pp += work->amount;
          break;
        case 4:
          work->stat24 += work->amount;
          break;
        case 5:
          work->agi += work->amount;
          break;
        case 6:
          work->luck += work->amount;
          break;
        case 15:
          work->element[0][0] += work->amount;
          break;
        case 16:
          work->element[1][0] += work->amount;
          break;
        case 17:
          work->element[2][0] += work->amount;
          break;
        case 18:
          work->element[3][0] += work->amount;
          break;
        case 19:
          work->element[0][1] += work->amount;
          break;
        case 20:
          work->element[1][1] += work->amount;
          break;
        case 21:
          work->element[2][1] += work->amount;
          break;
        case 22:
          work->element[3][1] += work->amount;
          break;
        case 23:
          st->bonus142 += work->amount;
          break;
        case 25:
          st->curse |= 8;
          break;
        case 26:
          work->turns += work->amount;
          break;
        }
      }
    }
    if (st->curse & 8)
      st->curse &= ~9;

    for (el = 0; el < 4; el++) {
      u32 bits = st->djinn[el];

      for (i = 0; i < 20; i++) {
        if (bits & (1 << i)) {
          const struct DjinnDefinition *djinn =
              (const struct DjinnDefinition *)Djinn_GetDefinition(el, i);

          work->hp += djinn->hp;
          work->pp += djinn->pp;
          work->atk += djinn->atk;
          work->def += djinn->def;
          work->agi += djinn->agi;
          work->luck += djinn->luck;
        }
      }
    }

    {
      struct ClassRecord *class =
          (struct ClassRecord *)Class_GetDefinition(st->classId);

      work->hp = work->hp * class->hp / 10;
      work->pp = work->pp * class->pp / 10;
      work->atk = work->atk * class->atk / 10;
      work->def = work->def * class->def / 10;
      work->agi = work->agi * class->agi / 10;
      work->luck = work->luck * class->luck / 10;
    }

    for (i = 0; i < 15; i++) {
      if (!(st->equipment[i] & 0x200))
        continue;
      work->item = Item_GetDirect(st->equipment[i]);
      for (j = 0; j < 4; j++) {
        kind = work->item->effects[j].kind;
        amount = work->item->effects[j].amount;
        work->kind = kind;
        kind -= 7;
        work->amount = amount;
        switch (kind) {
        case 0:
          work->hp = work->hp * work->amount / 10;
          break;
        case 1:
          work->stat20 = work->stat20 * work->amount / 10;
          break;
        case 2:
          work->pp = work->pp * work->amount / 10;
          break;
        case 3:
          work->stat24 = work->stat24 * work->amount / 10;
          break;
        case 4:
          work->atk = work->atk * work->amount / 10;
          break;
        case 5:
          work->def = work->def * work->amount / 10;
          break;
        case 6:
          work->agi = work->agi * work->amount / 10;
          break;
        case 7:
          work->luck = work->luck * work->amount / 10;
          break;
        }
      }
    }
  }

  work->atk = work->atk * (st->attackLevel + 8) / 8;
  work->def = work->def * (st->defenseLevel + 8) / 8;
  work->agi = work->agi * (st->agilityLevel + 8) / 8;
  for (i = 0; i < 4; i++)
    work->element[i][0] +=
        (st->elementLevel[i] * st->elementLevel[i] + st->elementLevel[i]) * 5;
  for (i = 0; i < 4; i++)
    work->element[i][1] += st->resistLevel * 20;

  if (st->classId) {
    flag = 0;
    switch (st->id) {
    case 1:
      flag = GameFlag_Test(0x112);
      break;
    case 3:
      flag = GameFlag_Test(0x111);
      break;
    case 5:
      flag = GameFlag_Test(0x112);
      break;
    case 0:
    case 4:
      flag = GameFlag_Test(0x110);
      break;
    case 2:
    case 6:
      flag = GameFlag_Test(0x113);
      break;
    case 7:
      flag = GameFlag_Test(0x111);
      break;
    }
    if (flag)
      work->stat24 += 4;
  }

  if (work->atk < 0)
    work->atk = 0;
  if (work->atk > 999)
    work->atk = 999;
  if (work->def < 0)
    work->def = 0;
  if (work->def > 999)
    work->def = 999;
  if (work->agi < 0)
    work->agi = 0;
  if (work->agi > 999)
    work->agi = 999;
  if (work->luck < 0)
    work->luck = 0;
  if (work->luck > 99)
    work->luck = 99;
  if (st->classId == 0) {
    if (work->turns < 0)
      work->turns = 0;
    if (work->turns > 4)
      work->turns = 4;
  } else {
    if (work->turns < 0)
      work->turns = 0;
    if (work->turns > 2)
      work->turns = 2;
  }
  if (work->stat20 < 0)
    work->stat20 = 0;
  if (work->stat20 > 10000)
    work->stat20 = 10000;
  if (work->stat24 < 0)
    work->stat24 = 0;
  if (work->stat24 > 200)
    work->stat24 = 200;
  for (i = 0; i < 4; i++) {
    if (work->element[i][0] < 0)
      work->element[i][0] = 0;
    if (work->element[i][0] > 200)
      work->element[i][0] = 200;
    if (work->element[i][1] < 0)
      work->element[i][1] = 0;
    if (work->element[i][1] > 200)
      work->element[i][1] = 200;
  }

  st->atk = work->atk;
  st->def = work->def;
  st->agi = work->agi;
  st->luck = work->luck;
  st->turns = work->turns;
  st->stat44 = work->stat20;
  st->stat45 = work->stat24;
  for (i = 0; i < 4; i++) {
    st->element[i][0] = work->element[i][0];
    st->element[i][1] = work->element[i][1];
  }

  if (st->classId == 0) {
    hpCap = 16383;
    ppCap = 9999;
  } else {
    hpCap = ppCap = 1999;
  }

  old = st->maxHp;
  if (work->hp < 0)
    work->hp = 0;
  if (work->hp > hpCap)
    work->hp = hpCap;
  st->maxHp = work->hp;
  if (old != st->maxHp) {
    value = work->hp * st->hpRatio / 0x4000;
    if (value < 0)
      value = 0;
    if (value > hpCap)
      value = hpCap;
    if (st->hp != 0 && value == 0)
      value = 1;
    st->hp = value;
  }

  old = st->maxPp;
  if (work->pp < 0)
    work->pp = 0;
  if (work->pp > ppCap)
    work->pp = ppCap;
  st->maxPp = work->pp;
  if (old != st->maxPp) {
    value = work->pp * st->ppRatio / 0x4000;
    if (value < 0)
      value = 0;
    if (value > ppCap)
      value = ppCap;
    if (st->pp != 0 && value == 0)
      value = 1;
    st->pp = value;
  }

  MemFree(work);
}
