/// @file
/// The party's units and their stats, a new game, the prologue, HP and PP.
// Part of game.c.

#include "../game_internal.h"

void Func_080addf0(void);
void MainRoutine_RunFar(s32 mode);

/// Starts a new game: sets up a new game's state (Func_080addf0) and runs the
/// main routine afresh from mode 0.
void Game_RestartQuest(void) {
  Func_080addf0();
  MainRoutine_RunFar(0);
}

struct TradeOfferState *Djinn_GetRecoveryTable(s32 which) {
  if (which != 0) {
    return Unit_Get(0x83);
  }
  return (struct TradeOfferState *)&gMainRoutineWork.unk000[0xc];
}

u32 Party_GetAverageLevel(void) {
  s32 count;
  s32 total;
  s32 i;

  total = 0;
  count = Party_CountActiveOwners();
  if (count == 0) {
    return 0;
  }
  for (i = 0; i < count; i++) {
    total += ((struct BattleUnit *)Unit_Get(gMainRoutineWork.activeOwners[i]))->level;
  }
  total = total / count;
  return total;
}

const u8 *Enemy_GetDefinition(s32 selector) {
  u32 recordIndex;

  recordIndex = selector - 8;
  if (recordIndex > 999U)
    recordIndex = 0;
  return (const u8 *)&gEnemy_DefinitionTable[recordIndex];
}

void Memory_CopyBytesDirectional(u8 *first, u8 *second, s32 count,
                                  s32 direction) {
  s32 index;

  if (direction != 0) {
    index = 0;
    if (index < count) {
      do {
        *second++ = *first++;
        index++;
      } while (index < count);
    }
  } else if (count > 0) {
    index = count;
    do {
      *first++ = *second++;
      index--;
    } while (index != 0);
  }
}

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

  work = Heap_AllocEwram(sizeof(struct StatWork));
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
/* FAKEMATCH(barrier): an empty asm after `work->atk += work->item->primaryBonus;`; as plain statements, 2 of 900 instructions differ in tla-en. */
asm("");
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

  Heap_Release(work);
}

void GameFlag_ClearBit(s32 flag);

void GameFlag_RefreshLureCap(void) {
  s32 partySize;
  s32 partyIndex;

  GameFlag_ClearBit(0x167);
  partySize = Party_CountActiveOwners();
  for (partyIndex = 0; partyIndex < partySize; partyIndex++) {
    struct OwnerInventoryState *unit;
    s32 inventorySlot;

    unit = Unit_Get(gMainRoutineWork.activeOwners[partyIndex]);
    for (inventorySlot = 0; inventorySlot < INVENTORY_SLOTS; inventorySlot++) {
      if (unit->inventory[inventorySlot] & INVENTORY_EQUIPPED) {
        struct ItemEffect *effect;
        s32 effectIndex;

        effect = Item_GetDirect(unit->inventory[inventorySlot])->effects;
        for (effectIndex = 0; effectIndex < 4; effectIndex++) {
          u8 effectKind;

          effectKind = effect->kind;
          effect++;
          if (effectKind == 27) {
            GameFlag_SetBit(0x167);
          }
        }
      }
    }
  }
}

static __inline__ s32 Owner_GetPpRatio(s32 owner) {
  struct BattleUnit *unit = Unit_Get(owner);

  if (unit->maxPp <= 0)
    return 0;
  return (unit->pp << 16) / unit->maxPp;
}

s32 Func_080adc90(void) {
  s32 pp = 0;
  s32 max = 0;
  s32 count = Party_CountActiveOwners();
  s32 i;

  if (count == 0)
    return 0;
  for (i = 0; i < count; i++) {
    struct BattleUnit *unit = Unit_Get(gMainRoutineWork.activeOwners[i]);

    pp += unit->pp;
    max += unit->maxPp;
  }
  if (max == 0)
    return 0;
  return (pp << 16) / max;
}

s32 Func_080adcec(void) {
  s32 best = 0;
  s32 bestOwner = -1;
  s32 count = Party_CountActiveOwners();
  s32 i;

  if (count == 0)
    return 0;
  for (i = 0; i < count; i++) {
    s32 ratio = Owner_GetPpRatio(gMainRoutineWork.activeOwners[i]);

    if (ratio > best) {
      best = ratio;
      bestOwner = gMainRoutineWork.activeOwners[i];
    }
  }
  if (bestOwner == -1)
    return 0;
  // Its result taken as a whole word, where the definition returns s16.
  return ((s32 (*)(s32, s32))Unit_AddPp)(bestOwner, -1);
}

void Func_080add5c(s32 count) {
  s32 i;

  for (i = 0; i < count; i++)
    Func_080adcec();
  Func_080adc90();
}

extern char ResourceId_BuildStamp;

u16 BuildStamp_GetTime(void) {
  u8 *digits;
  s32 hourTens;
  s32 hourUnits;
  s32 minuteTens;
  s32 minuteUnits;
  s32 secondTens;
  s32 secondUnits;
  s32 hours;
  s32 minutes;
  s32 seconds;
  s32 packed;
  s32 shifted;
  s32 result;

  digits = Resource_GetTableEntry((s32)&ResourceId_BuildStamp);
  hourTens = *digits;
  hours = (hourTens - '0') * 10;
  digits++;
  hourUnits = *digits;
  digits++;
  hours += hourUnits - '0';
  minuteTens = *digits;
  minutes = (minuteTens - '0') * 10;
  digits++;
  minuteUnits = *digits;
  digits++;
  minutes += minuteUnits - '0';
  secondTens = digits[0];
  seconds = (secondTens - '0') * 10;
  secondUnits = digits[1];
  seconds += secondUnits - '0';
  packed = (((hours << 4) + minutes) << 6) + seconds;
  shifted = 0x80 << 21;
  shifted |= packed << 16;
  result = shifted >> 16;
  if (gDebugMode != 0) {
    result |= ~0x7fff;
  }
  return result;
}

#define DMA3 ((volatile struct DmaRegisters *)&REG_DMA3SAD)
#define DMA3_BUSY (DMA_ENABLE << 16)
#define START_DMA(source_, destination_, control_)                             \
  Dma_Set((const void *)(source_), (void *)(destination_), (control_), REG_DMA3)

static __inline__ void WaitDma(struct DmaRegisters volatile *channel) {
  while ((channel->control & DMA3_BUSY) != 0) {
  }
}

extern u8 gData_02001000[];
extern u8 gGameFlagBytes[];
extern u8 gOwnerStates[];
extern u32 gFrameWaitCount;
#ifndef MODERN
// A current GCC refuses the volatile redeclaration and reads frame.h's.
extern volatile u8 gAutoSleepEnabled;
#endif
extern volatile u16 gIdleFrameCount;
extern s16 gData_020036d0;
extern u16 gData_02003860;
extern const u8 Data_080b1f2c[4][5];

void Func_080af7ac(void);
s32 Party_AddActiveOwner(s32 owner);
s32 Unit_LearnPsynergy(s32 owner, s32 ability);

u32 Random16(void);

/// Starts a new game's state: clears gMainRoutineWork, the work block, the
/// flags and the owners by DMA, builds the characters, sets the defaults (the
/// button layout, Felix and Jenna in the party, their first Psynergy, 200
/// coins, the message settings and the play time clock) and fills the work
/// block's random tables.
void Func_080addf0(void) {
  volatile s32 zero;
  s32 i;
  s32 j;

  zero = 0;
  START_DMA(&zero, &gMainRoutineWork,
            (DMA_ENABLE | DMA_32BIT | DMA_SRC_FIXED) << 16 | 184);
  zero = 0;
#if TLA_PATCH_1
  START_DMA(&zero, gData_02001000,
            (DMA_ENABLE | DMA_32BIT | DMA_SRC_FIXED) << 16 | 1992);
  WaitDma(DMA3);
#else
  {
    /* FAKEMATCH(pin): the channel kept in r3 through the transfer and copied
     * from there for the wait, r3 then holding the busy bit; as START_DMA and
     * WaitDma(DMA3), 10 of 320 instructions differ in tla-ja. */
    register u32 held asm("r3") = (u32)DMA3;
    struct DmaRegisters volatile *channel;

    Dma_Set((const void *)&zero, gData_02001000,
            (DMA_ENABLE | DMA_32BIT | DMA_SRC_FIXED) << 16 | 1992,
            (volatile u32 *)held);
    channel = (struct DmaRegisters volatile *)held;
    held = DMA3_BUSY;
    while ((channel->control & held) != 0) {
    }
  }
#endif
  zero = 0;
  START_DMA(&zero, gGameFlagBytes,
            (DMA_ENABLE | DMA_32BIT | DMA_SRC_FIXED) << 16 | 128);
  gData_02001000[4] = 0xff;
  zero = 0;
  START_DMA(&zero, gOwnerStates,
            (DMA_ENABLE | DMA_32BIT | DMA_SRC_FIXED) << 16 | 664);
  Func_080af7ac();
  gMainRoutineWork.keys230 = 1;
  gMainRoutineWork.unk232 = 2;
  gMainRoutineWork.keys234 = 4;
  gMainRoutineWork.keys236 = 8;
  gMainRoutineWork.keys238 = 0x200;
  gMainRoutineWork.keys23a = 0x100;
  *(u16 *)gMainRoutineWork.unk23c = 2;
  gMainRoutineWork.packedEvents[0] = 0;
  gMainRoutineWork.packedEvents[1] = 0;
  gMainRoutineWork.currentOwner = 4;
  Party_AddActiveOwner(4);
  Party_AddActiveOwner(5);
  Unit_LearnPsynergy(4, 149);
  Unit_LearnPsynergy(4, 140);
  Unit_LearnPsynergy(6, 141);
  Unit_LearnPsynergy(7, 33);
  Unit_LearnPsynergy(0, 149);
  Unit_LearnPsynergy(0, 140);
  Unit_LearnPsynergy(1, 140);
  Unit_LearnPsynergy(2, 141);
  Unit_LearnPsynergy(2, 144);
  gMainRoutineWork.coins = 200;
  *(u32 *)&gMainRoutineWork.unk284[8] = 0;
  gMainRoutineWork.forgeItem = 0;
  gMainRoutineWork.counter274 = 0;
  gMainRoutineWork.eventMode = 1;
  gMainRoutineWork.battleFxEnabled = 1;
  gMainRoutineWork.unk22b = 1;
  gMainRoutineWork.paletteGlow[0] = 0;
  gMainRoutineWork.paletteGlow[1] = 8;
  *(u32 *)gMainRoutineWork.unk000 = 0;
  // Its result taken as a whole word, where the definition returns u16.
  *(u32 *)((u8 *)&gMainRoutineWork + 0x2d8) =
      ((u32 (*)(void))BuildStamp_GetTime)();
  gFrameWaitCount = 0;
  gAutoSleepEnabled = 0;
  gMainRoutineWork.playTime = 0;
  /* FAKEMATCH(volatile): the auto-sleep flag, declared volatile above, read
   * again after the play time store, as The Broken Seal declares its
   * gIdleSleepEnabled; as plain u8, as frame.h declares it, 108 of 322
   * instructions differ in tla-en. */
  gMainRoutineWork.unk244[6] = gAutoSleepEnabled;
  gIdleFrameCount = 0;
  gData_020036d0 = -1;
  gMainRoutineWork.unk284[2] = gData_02003860;
  gMainRoutineWork.unk13d[0] = 4;
  gMainRoutineWork.unk13d[1] = 4;
  gMainRoutineWork.unk13d[2] = 4;
  gMainRoutineWork.unk13d[3] = 8;
  gMainRoutineWork.unk13d[4] = 8;
  gMainRoutineWork.unk13d[5] = 8;
  gMainRoutineWork.unk13d[6] = 16;
  gMainRoutineWork.unk13d[7] = 16;
  gMainRoutineWork.unk13d[8] = 16;
  gMainRoutineWork.unk13d[9] = 32;
  gMainRoutineWork.unk13d[10] = 32;
  gMainRoutineWork.unk13d[11] = 32;
  gMainRoutineWork.unk13d[12] = 64;
  gMainRoutineWork.unk13d[13] = 64;
  gMainRoutineWork.unk13d[14] = 64;

  for (i = 0; i < 32; i++) {
    for (j = 0; j < 8; j++)
      gData_02001000[0x11b8 + i * 12 + j] = (Random16() * 5) >> 16;
  }
  for (i = 0; i < 4; i++) {
    for (j = 0; j < 5; j++)
      gData_02001000[0x1338 + i * 5 + j] = Data_080b1f2c[i][j];
  }
}

s32 Party_AddActiveOwner(s32 owner);
s32 Party_RemoveActiveOwner(s32 owner);

void Party_SetLeaderJenna(void) { gMainRoutineWork.currentOwner = 5; }

void Party_SetLeaderFelix(void) {
  gMainRoutineWork.currentOwner = 4;
  Party_AddActiveOwner(4);
  Party_RemoveActiveOwner(5);
  Party_RemoveActiveOwner(6);
}

void Unit_Calculate(s32 unit);

void Func_080ae118(void) {
  s32 i;
  s32 count;

  for (i = 10; i < 12; i++) {
    GameFlag_SetBit(i + 67);
    Djinn_AddToOwner(7, 1, i - 1);
    Djinn_Activate(7, 1, i - 1);
  }
  // Called with an argument it does not take.
  ((void (*)(s32))Func_080ae358)(0);
  count = Party_CountActiveOwners();
  for (i = 0; i < count; i++)
    Unit_Calculate(gMainRoutineWork.activeOwners[i]);
}

void GameFlag_ClearBit(s32 flag);

/// Restores every party member's HP and PP; with `cureConditions` 1 it also
/// cures poison and haunting.
void Party_RestoreHpPp(s32 cureConditions) {
  s32 count;
  s32 n;
  u8 *obj;
  s32 t;
  s32 v14;
  s32 v16;
  s32 one;
  s16 v34;

  count = Party_CountActiveOwners();
  for (n = 0; n < count; n++) {
    {
      struct MainRoutineWork *party = &gMainRoutineWork;

      obj = Unit_Get(party->activeOwners[n]);
    }

    do {
      *(u16 *)(obj + 0x38) = *(u16 *)(obj + 0x34);
      *(u16 *)(obj + 0x3A) = *(u16 *)(obj + 0x36);
    } while (0);

    v34 = *(s16 *)(obj + 0x34);
    t = __divsi3(v34 << 14, v34);
    v14 = 0x4000;
    if (t <= 0x4000) {
      v14 = 0;
      if (t >= 0) {
        v14 = t;
      }
    }
    *(s16 *)(obj + 0x14) = (s16)v14;
    if (((v14 << 16) == 0) && (*(s16 *)(obj + 0x38) != 0)) {
      one = 1;
      *(s16 *)(obj + 0x14) = (s16)one;
    }

    t = __divsi3(*(s16 *)(obj + 0x3A) << 14, *(s16 *)(obj + 0x36));
    v16 = 0x4000;
    if (t <= 0x4000) {
      v16 = 0;
      if (t >= 0) {
        v16 = t;
      }
    }
    *(s16 *)(obj + 0x16) = (s16)v16;
    if (((v16 << 16) == 0) && (*(s16 *)(obj + 0x3A) != 0)) {
      one = 1;
      *(s16 *)(obj + 0x16) = (s16)one;
    }

    if (cureConditions == 1) {
      *(s8 *)(obj + 0x131) = 0;
      *(s8 *)(obj + 0x140) = 0;
    }
  }
}

extern const u8 gCharacter_ElementGroupTable[];

// Party_RestorePpOfLitElements from The Broken Seal.
void Func_080ae220(void) {
  s32 count;
  s32 n;
  u8 ownerId;
  s32 group;
  s32 doRefresh;
  u8 *obj;
  s32 t;
  s32 v14;
  s32 v16;
  s32 v34;
  s32 v36;
  s32 v38;
  s32 v3A;
  s32 one;

  count = Party_CountActiveOwners();
  for (n = 0; n < count; n++) {
    {
      struct MainRoutineWork *party = &gMainRoutineWork;

      ownerId = party->activeOwners[n];
    }
    group = gCharacter_ElementGroupTable[ownerId];
    doRefresh = 0;
    if (group == 0) {
      if (GameFlag_Test(0x110) || GameFlag_Test(0x112)) {
        doRefresh = 1;
      }
    } else {
      if (GameFlag_Test(0x111) || GameFlag_Test(0x113)) {
        doRefresh = 1;
      }
    }

    if (doRefresh == 0) {
      continue;
    }

    obj = Unit_Get(ownerId);
    do {
      *(u16 *)(obj + 0x3A) = *(u16 *)(obj + 0x36);
    } while (0);

    v38 = *(s16 *)(obj + 0x38);
    v34 = *(s16 *)(obj + 0x34);
    t = __divsi3(v38 << 14, v34);
    v14 = 0x4000;
    if (t <= 0x4000) {
      v14 = 0;
      if (t >= 0) {
        v14 = t;
      }
    }
    *(s16 *)(obj + 0x14) = (s16)v14;
    if (((v14 << 16) == 0) && (*(s16 *)(obj + 0x38) != 0)) {
      one = 1;
      *(s16 *)(obj + 0x14) = (s16)one;
    }

    v3A = *(s16 *)(obj + 0x3A);
    v36 = *(s16 *)(obj + 0x36);
    t = __divsi3(v3A << 14, v36);
    v16 = 0x4000;
    if (t <= 0x4000) {
      v16 = 0;
      if (t >= 0) {
        v16 = t;
      }
    }
    *(s16 *)(obj + 0x16) = (s16)v16;
    if (((v16 << 16) == 0) && (*(s16 *)(obj + 0x3A) != 0)) {
      one = 1;
      *(s16 *)(obj + 0x16) = (s16)one;
    }
  }
}

struct UnitDjinnSets {
  u8 unk00[0xf8];
  u32 djinn[4];
};

/// Element and index of the last owned Djinni.
s32 Func_080ae2fc(s32 unit, s32 *element_out, s32 *index_out) {
  u32 *sets = ((struct UnitDjinnSets *)Unit_Get(unit))->djinn;
  s32 element = -1;
  s32 index = -1;
  s32 i;
  s32 j;

  for (i = 0; i < 20; i++) {
    for (j = 0; j < 4; j++) {
      if (sets[j] & (1 << i)) {
        element = j;
        index = i;
      }
    }
  }
  if (element == -1)
    return element;
  *element_out = element;
  *index_out = index;
  return 0;
}

s32 Func_080ae2fc(s32 unit, s32 *element_out, s32 *index_out);

/// Until the Djinn counts of the active owners are within one, moves the last
/// Djinni of the owner holding most to the one holding least.
void Func_080ae358(void) {
  s32 count = Party_CountActiveOwners();

  for (;;) {
    s32 lowNo = 0;
    s32 high = 0;
    s32 low = 999;
    s32 highNo = 0;
    s32 i;
    s32 sum;
    s32 element;
    s32 index;

    for (i = 0; i < count; i++) {
      s32 owner = gMainRoutineWork.activeOwners[i];
      struct OwnerDjinnState *unit = Unit_Get(owner);
      u8 *counts;
      s32 j;

      sum = 0;
      counts = unit->counts.banks.owned;
      j = 3;
      do {
        u8 value = *counts;
        counts++;
        sum += value;
        j--;
      } while (j >= 0);
      if (sum > high) {
        high = sum;
        highNo = owner;
      }
      if (sum < low) {
        low = sum;
        lowNo = owner;
      }
    }
    if (high - low <= 1)
      break;
    sum = Func_080ae2fc(highNo, &element, &index);
    if (sum != 0)
      break;
    Djinn_Transfer(highNo, element, index, lowNo);
  }
}

s32 FlagBits_Test(u8 *bytes, s32 flag) {
  s32 bit;
  u32 index;

  bit = flag & 7;
  index = (u32)flag << 20;
  flag = index >> 23;
  return (bytes[flag] >> bit) & 1;
}

s32 FlagBits_Test(u8 *bytes, s32 flag);
s32 GameFlag_Test(s32 flag);
void GameFlag_ClearBit(s32 flag);
void Func_08038348(s32 a, s32 b);
void *Unit_Get(s32 owner);
s32 Inventory_Remove(s32 owner, s32 slot);
void Unit_Calculate(s32 owner);
void Djinn_ActivateOfferedForParty(void);
void Party_RestoreHpPp(s32 cureConditions);

extern const u16 Data_080b1284[6];

/// Copies a The Broken Seal save's djinn, flags, coins, options (unless
/// keepSettings) and units into this game's state.
void Func_080ae410(u8 *save, s32 keepSettings) {
  u8 *flags = save + 0x40;
  struct MainRoutineWork *party = (struct MainRoutineWork *)(save + 0x240);
  struct BattleUnit *units = (struct BattleUnit *)(save + 0x520);
  struct MainRoutineWork *state;
  u8 name[16];
  s32 i;
  s32 k;
  s32 slot;
  s32 count;
  s32 j;

  for (i = 0, k = 0; i < 4; i++, k += 7) {

    for (j = 0; j < 7; j++) {
      s32 flag = 0x800 + k + j;

      if (FlagBits_Test(flags, 0x30 + i * 20 + j))
        GameFlag_SetBit(flag);
      else
        GameFlag_ClearBit(flag);
      /* FAKEMATCH(barrier): an empty asm reading j, which keeps the counter
       * the native's one long-lived register; without it, 105 of 219
       * instructions differ in tla-en. */
      asm("" : : "r"(j));
    }
  }
  for (i = 0; i < 6; i++) {
    if (FlagBits_Test(flags, Data_080b1284[i]))
      GameFlag_SetBit(0x820 + i);
    else
      GameFlag_ClearBit(0x820 + i);
  }
  if (GameFlag_Test(0x822))
    GameFlag_ClearBit(0x822);
  else
    GameFlag_SetBit(0x822);

  state = &gMainRoutineWork;
  *(s32 *)&state->unk284[8] = party->coins;
  if (keepSettings == 0) {
    state->paletteGlow[0] = party->paletteGlow[0];
    state->paletteGlow[1] = party->paletteGlow[1];
    state->eventMode = party->eventMode;
    state->battleFxEnabled = party->battleFxEnabled;
    state->unk244[6] = party->unk244[6];
    Func_08038348(state->paletteGlow[0], state->paletteGlow[1]);
  }

  j = 4; // A local reused for a second value: the unit loop's bound.
  for (i = 0; i < (s32)j; i++) {
    struct BattleUnit *unit = Unit_Get(i);

    memcpy(name, &units[i], 15);
    Iwram_CopyWords(unit, &units[i], sizeof(struct BattleUnit));
    memcpy(unit, name, 15);
    if (keepSettings == 0)
      unit->enemyId = unit->classId;
    for (slot = 14; slot >= 0; slot--) {
      s32 item = unit->inventory[slot];

      if (item != 0 && (Item_GetDirect(item)->flags & 0x20)) {
        while (Inventory_Remove(i, slot) == 1)
          ;
      }
    }
    Unit_Calculate(i);
  }
  Djinn_ActivateOfferedForParty();
  Party_RestoreHpPp(1);
  GameFlag_SetBit((keepSettings & 1) + 44);
  GameFlag_SetBit(47);
  gFrameWaitCount = gMainRoutineWork.playTime;
}

void Unit_Calculate(s32 unitId);
void Func_08038348(s32 a, s32 b);

void Func_080ae5fc(u8 *save) {
  struct MainRoutineWork *party = (struct MainRoutineWork *)(save + 0x240);
  struct BattleUnit *units = (struct BattleUnit *)(save + 0x520);
  u8 unused[16]; // Unused: gives the stack frame its 16 bytes.
  s32 i;
  s32 count;

  gMainRoutineWork.paletteGlow[0] = party->paletteGlow[0];
  gMainRoutineWork.paletteGlow[1] = party->paletteGlow[1];
  gMainRoutineWork.eventMode = party->eventMode;
  gMainRoutineWork.battleFxEnabled = party->battleFxEnabled;
  gMainRoutineWork.unk244[6] = party->unk244[6];
  Func_08038348(gMainRoutineWork.paletteGlow[0],
                gMainRoutineWork.paletteGlow[1]);
  count = 8;
  for (i = 0; i < count; i++) {
    struct BattleUnit *unit = Unit_Get(i);
    struct BattleUnit *saved = &units[i];

    unit->baseHp = saved->baseHp;
    unit->basePp = saved->basePp;
    unit->hpGauge = unit->baseHp;
    unit->ppGauge = unit->basePp;
    unit->baseAttack = saved->baseAttack;
    unit->baseDefense = saved->baseDefense;
    unit->baseAgility = saved->baseAgility;
    unit->baseLuck = saved->baseLuck;
    unit->level = saved->level;
    unit->experience = saved->experience;
    Unit_Calculate(i);
  }
  gMainRoutineWork.coins = party->coins;
}

void Func_080ae6c8(s32 item, s32 owner) {
  if (PartyInventory_FindOwner(item) == -1)
    Inventory_AddItem(owner, item);
}

s32 GameFlag_Test(s32 flag);
void Djinn_ActivateOfferedForParty(void);
void Unit_Calculate(s32 unitId);
void Party_RestoreHpPp(s32 cureConditions);
void Func_080ae6c8(s32 a, s32 b);
u32 Random16(void);

extern u8 gData_02001000[];

void Func_080ae6e4(void) {
  s32 i;
  s32 djinni;
  s8 missing[8];
  s32 count;
  s32 k;

  for (i = 0, k = 0; i < 4; i++, k += 7) {
    s32 j;

    for (j = 0; j < 7; j++) {
      if (GameFlag_Test(0x800 + k + j))
        GameFlag_SetBit(0x30 + i * 20 + j);
      /* FAKEMATCH(barrier): an empty asm reading j keeps loop.c from
       * reversing the signed counter; as the same loop without it, 8 of
       * 142 instructions differ in tla-en. */
      asm("" : : "r"(j));
    }
    /* FAKEMATCH(barrier): an empty asm on i keeps i * 20 from being
     * strength-reduced; as the same loops without it, 20 of 142
     * instructions differ in tla-en. */
    asm("" : "+r"(i));
  }
  PartyInventory_Remove(222);
  Party_AddActiveOwner(0);
  Party_AddActiveOwner(1);
  Party_AddActiveOwner(2);
  Party_AddActiveOwner(3);
  // Called with an argument it does not take.
  ((void (*)(s32))Func_080ae358)(0);
  Djinn_ActivateOfferedForParty();
  for (i = 0; i < 8; i++)
    Unit_Calculate(i);
  Party_RestoreHpPp(1);
  GameFlag_SetBit(34);

  for (i = 0; i < 4; i++) {
    count = 0;
    for (djinni = 0; djinni < 7; djinni++) {
      if (!GameFlag_Test(0x30 + i * 20 + djinni))
        missing[count++] = djinni;
    }
    if (count != 0)
      djinni = missing[(Random16() * count) >> 16] + 1;
    else
      djinni = 0;
    gData_02001000[0x1950 + i] = djinni;
  }
  Func_080ae6c8(202, 3);
  Func_080ae6c8(201, 3);
  Func_080ae6c8(203, 1);
  Func_080ae6c8(206, 0);
  Func_080ae6c8(207, 2);
  gMainRoutineWork.coins += *(s32 *)&gMainRoutineWork.unk284[8];
#if TLA_PATCH_4
  if ((u32)gMainRoutineWork.coins > 999999)
    gMainRoutineWork.coins = 999999;
#endif
}

void Func_080ae830(void) {}

void Djinn_AddFoundToOwner(s32 owner, s32 element, s32 djinn) {
  s32 flag = element * 20 + djinn;

  Djinn_AddToOwner(owner, element, djinn);
  flag += 0x30;
  DjinnRecovery_AddStandby(owner, element, djinn);
  GameFlag_SetBit(flag);
}

s32 GameFlag_Test(s32 flag);
void GameFlag_ClearBit(s32 flag);
s32 Party_CountActiveOwners(void);
s32 Party_AddActiveOwner(s32 owner);
s32 Party_RemoveActiveOwner(s32 owner);
void Djinn_AddFoundToOwner(s32 owner, s32 element, s32 djinn);
s32 Djinn_AddToLeastLoadedOwner(s32 element, s32 djinn);
void Djinn_ActivateOfferedForParty(void);
u32 Random16(void);

extern const s16 Data_080b1290[4][7];

/// Unless flag 0x800 is set, sets the party aside, makes the first four owners
/// the party, gives them their first djinn, then hands out 16 random djinn
/// still unowned, keeping the elements' counts within one, each with the
/// table's percentage chance; then moves the djinn flags to 0x800 (seven per
/// element) and puts the party back.
void Func_080ae868(void) {
  s8 saved[8];
  s8 counts[4];
  s32 count;
  s32 remaining;
  s32 element;
  s32 djinn;
  s32 i;
  s32 k;

  if (GameFlag_Test(0x800))
    return;

  count = Party_CountActiveOwners();
  for (i = 0; i < count; i++)
    saved[i] = gMainRoutineWork.activeOwners[i];
  for (i = 0; i < count; i++)
    Party_RemoveActiveOwner(saved[i]);
  for (i = 0; i < 4; i++)
    Party_AddActiveOwner(i);
  Djinn_AddFoundToOwner(0, 0, 0);
  Djinn_AddFoundToOwner(3, 1, 0);

  counts[0] = 1;
  counts[1] = 1;
  counts[2] = 0;
  counts[3] = 0;
  remaining = 16;
  do {
    element = (Random16() * 4) >> 16;
    djinn = (Random16() * 7) >> 16;
    if (!GameFlag_Test(element * 20 + djinn + 0x30)) {
      s8 low;
      s8 high;

      counts[element]++;
      low = 9;
      high = 0;
      for (i = 0; i < 4; i++) {
        if (low > counts[i])
          low = counts[i];
        if (high < counts[i])
          high = counts[i];
      }
      counts[element]--;
      if (high - low <= 1 &&
          ((Random16() * 100) >> 16) < Data_080b1290[element][djinn]) {
        Djinn_AddToLeastLoadedOwner(element, djinn);
        counts[element]++;
        remaining--;
      }
    }
  } while (remaining != 0);
  Djinn_ActivateOfferedForParty();

  for (i = 0, k = 0; i < 4; i++, k += 7) {
    s32 j;
    s32 flag;

    for (j = 0; j < 7; j++) {
      /* FAKEMATCH(temporary): the form kept; as GameFlag_SetBit(0x800 + k + j),
         8 of 206 instructions differ in tla-en. */
      flag = 0x800 + k + j;
      /* FAKEMATCH(temporary): element reused for the flag; with its own local
         or inline, 13 of 206 instructions differ in tla-en. */
      element = 0x30 + i * 20 + j;
      if (GameFlag_Test(element))
        GameFlag_SetBit(flag);
      GameFlag_ClearBit(element);
    }
  }
  for (i = 0; i < 4; i++)
    Party_RemoveActiveOwner(i);
  for (i = 0; i < count; i++)
    Party_AddActiveOwner(saved[i]);
}

void Unit_UpdateRatios(s32 ownerId) {
  s32 first;
  s32 second;
  s32 firstValue;
  s32 secondValue;
  struct BattleUnit *owner;

  owner = Unit_Get(ownerId);
  first = (owner->hp << 14) / owner->maxHp;
  firstValue = 0x4000;
  if (first <= 0x4000) {
    firstValue = 0;
    if (first >= 0) {
      firstValue = first;
    }
  }
  owner->hpGauge = firstValue;
  if (owner->hpGauge == 0 && owner->hp != 0) {
    firstValue = 1;
    owner->hpGauge = firstValue;
  }
  second = (owner->pp << 14) / owner->maxPp;
  secondValue = 0x4000;
  if (second <= 0x4000) {
    secondValue = 0;
    if (second >= 0) {
      secondValue = second;
    }
  }
  owner->ppGauge = secondValue;
  if (owner->ppGauge == 0 && owner->pp != 0) {
    secondValue = 1;
    owner->ppGauge = secondValue;
  }
}

/// Sets the unit's HP, clamped to 0..maxHp, and recomputes its HP and PP
/// gauges inline.
void Unit_SetHp(struct BattleUnit *unit, s32 hp) {
  s32 value;

  if (hp > unit->maxHp) {
    value = unit->maxHp;
  } else {
    value = 0;
    if (hp >= 0) {
      value = hp;
    }
  }
  unit->hp = value;
  value = ((value << 16) >> 2) / unit->maxHp;

  {
    s32 output = 0x4000;

    if (value <= output) {
      output = 0;
      if (value >= 0) {
        output = value;
      }
    }
    unit->hpGauge = output;
    if ((output << 16) == 0 && unit->hp != 0) {
      unit->hpGauge = 1;
    }
  }

  value = (unit->pp << 14) / unit->maxPp;
  {
    s32 output = 0x4000;

    if (value <= output) {
      output = 0;
      if (value >= 0) {
        output = value;
      }
    }
    unit->ppGauge = output;
    if ((output << 16) == 0 && unit->pp != 0) {
      unit->ppGauge = 1;
    }
  }
}

/// Sets the unit's PP, clamped to 0..maxPp, and recomputes its HP and PP
/// gauges.
void Unit_SetPp(struct BattleUnit *unit, s32 pp) {
  s32 clamped;

  if (pp > unit->maxPp) {
    clamped = unit->maxPp;
  } else {
    clamped = 0;
    if (pp >= 0)
      clamped = pp;
  }
  unit->pp = clamped;
  Unit_UpdateGauges(unit);
}

void Unit_UpdateRatios(s32);

/// Adds delta to the HP of unit owner, clamped to 0..maxHp, recomputes its
/// gauges and returns the new HP.
s32 Unit_AddHp(s32 owner, s32 delta) {
  struct BattleUnit *state = Unit_Get(owner);
  s32 cur = state->hp;
  s32 max = state->maxHp;
  s32 value = cur + delta;
  s32 result;

  if (value > max)
    result = max;
  else {
    result = 0;
    if (value >= 0)
      result = value;
  }
  state->hp = result;
  Unit_UpdateRatios(owner);
  return state->hp;
}

void Unit_UpdateRatios(s32);

/// Adds delta to the PP of unit owner, clamped to 0..maxPp, recomputes its
/// gauges and returns the new PP.
s16 Unit_AddPp(s32 owner, s32 delta) {
  struct BattleUnit *state = Unit_Get(owner);
  s32 cur = state->pp;
  s32 max = state->maxPp;
  s32 value = cur + delta;
  s32 result;

  if (value > max)
    result = max;
  else {
    result = 0;
    if (value >= 0)
      result = value;
  }
  state->pp = result;
  Unit_UpdateRatios(owner);
  return state->pp;
}

#undef STAT_DIFF
#undef DMA3
#undef DMA3_BUSY
#undef START_DMA
