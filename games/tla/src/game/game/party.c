/// @file
/// Enemy units, the active party, coins and counters, and summons.
// Part of game.c.

#include "../game_internal.h"

// A unit whose suffix is below SUFFIX_COUNT gets one more character after its
// name, SUFFIX_BASE plus the suffix: the digits 1 to 9 outside Japan, and the
// codes 65 to 90 in the Japanese release (Func_08128228 reads it back).
#if LANGUAGE == LANGUAGE_JAPANESE
#define SUFFIX_BASE 65
#define SUFFIX_COUNT 26
#else
#define SUFFIX_BASE '1'
#define SUFFIX_COUNT 9
#endif

struct EnemyRecord {
  u8 unk00[15];
  u8 level;
  u16 hp;
  u16 pp;
  u16 atk;
  u16 def;
  u16 agi;
  u8 luck;
  u8 turns;
  u8 hpRegen;
  u8 ppRegen;
  u16 items[4];
  u8 counts[4];
  u8 unk2a[34];
};

extern u8 MsgEnemyName[];

void Message_DecodeRawFar(s32 message, u16 *buffer, s32 length);
void Unit_CalculateBaseElements(s32 owner, struct BattleElementStats *elements);
void Unit_Calculate(s32 unitId);

s32 Func_080afbec(s32 unitId, s32 enemyId, s32 suffix) {
  struct BattleUnit *unit;
  const struct EnemyRecord *enemy;
  u16 name[15];
  s32 i;
  u32 index;
  s32 q;
  s32 count;

  index = enemyId - 8;
  if (unitId <= 127)
    return 0;
  if (unitId > 134)
    return 0;
  if (index > 992)
    return 0;
  unit = Unit_Get(unitId);
  {
    /* FAKEMATCH: the entry taken first puts its pool load ahead of the size,
     * where the native issues it (differ 4 without) */
    s32 (*clear)(void *destination, s32 size) = Iwram_ClearWords;

    clear(unit, BATTLE_UNIT_SIZE);
  }
  if (index >= 380)
    index = 0;
  enemy = (const struct EnemyRecord *)&gEnemy_DefinitionTable[index];
  unit->level = enemy->level;
  unit->baseHp = enemy->hp;
  unit->hp = enemy->hp;
  unit->maxHp = enemy->hp;
  unit->basePp = enemy->pp;
  unit->pp = enemy->pp;
  unit->maxPp = enemy->pp;
  unit->hpGauge = 0x4000;
  unit->ppGauge = 0x4000;
  unit->baseAttack = enemy->atk;
  unit->baseDefense = enemy->def;
  unit->baseAgility = enemy->agi;
  unit->baseLuck = enemy->luck;
  unit->baseActionCount = enemy->turns;
  unit->baseHpRegen = enemy->hpRegen;
  unit->basePpRegen = enemy->ppRegen;
  Message_DecodeRawFar(index + (s32)MsgEnemyName, name, 15);
  for (i = 0; i < 14 && name[i] != 0; i++)
    unit->name[i] = name[i];
  if (suffix <= SUFFIX_COUNT - 1) {
    unit->name[i] = SUFFIX_BASE + suffix;
    i++;
  }
  unit->name[i] = 0;
  unit->name[14] = 0;
  count = 0;
  for (i = 0; i < 4; i++) {
    if (enemy->items[i] != 0) {
      for (q = 0; q < enemy->counts[i]; q++) {
        if (count < 15)
          unit->inventory[count++] = enemy->items[i];
      }
    }
  }
  unit->rewards = 0;
  unit->classIndex = 0;
  unit->enemyId = enemyId;
  Unit_CalculateBaseElements(unitId, unit->baseElement);
  Unit_Calculate(unitId);
  unit->side = 1;
  if ((u16)(unit->enemyId - 9) <= 57)
    unit->side = 2;
  if ((u16)(unit->enemyId - 369) <= 8)
    unit->side = 2;
  return 1;
}

s32 GameFlag_Test(s32 flag);

s32 Party_CountActiveOwners(void) {
  s32 count;
  s32 i;

  count = 0;
  for (i = 0; i <= 7; i++) {
    if (GameFlag_Test(i))
      count++;
  }
  return count;
}

s32 GameFlag_SetBit(s32);
void GameFlag_ClearBit(s32);

s32 Party_AddActiveOwner(s32 value) {
  s32 count = Party_CountActiveOwners();
  s32 index;

  GameFlag_SetBit(value);
  index = 0;
  while (index < count) {
    if (gMainRoutineWork.activeOwners[index] == value)
      return count;
    index++;
  }
  gMainRoutineWork.activeOwners[index] = value;
  return count + 1;
}

void GameFlag_ClearBit(s32);

s32 Party_RemoveActiveOwner(s32 value) {
  s32 count = Party_CountActiveOwners();
  s32 i;
  s32 j;

  GameFlag_ClearBit(value);
  for (i = 0; i < count; i++) {
    if (gMainRoutineWork.activeOwners[i] == value)
      break;
  }
  for (j = i; j < count - 1; j++)
    gMainRoutineWork.activeOwners[j] = gMainRoutineWork.activeOwners[j + 1];
  return Party_CountActiveOwners();
}

s32 Party_ListActiveOwners(s16 *owners) {
  s32 count = 0;

  if (owners != NULL) {
    s32 index;

    count = Party_CountActiveOwners();
    index = 0;
    if (count != 0) {
      do {
        *owners++ = gMainRoutineWork.activeOwners[index];
        index++;
      } while (index != count);
    }
    *owners = 0xff;
  }
  return count;
}

s32 Party_AdjustCoins(s32 amount) {
  s32 value;

  value = gMainRoutineWork.coins;
  value += amount;
  if (value > 0xf423f)
    value = 0xf423f;
  if (value < 0)
    value = 0;
  gMainRoutineWork.coins = value;
  return value;
}

s32 Party_AdjustShopCredit(s32 amount) {
  s32 value;

  value = gMainRoutineWork.counterB;
  value += amount;
  if (value > 0xf423f)
    value = 0xf423f;
  if (value < 0)
    value = 0;
  gMainRoutineWork.counterB = value;
  return value;
}

s32 Party_AdjustShopGifts(s32 amount) {
  s32 value;

  value = gMainRoutineWork.counterC;
  value += amount;
  if (value > 28)
    value = 28;
  if (value < 0)
    value = 0;
  gMainRoutineWork.counterC = value;
  return value;
}

/// The summons in the order the menus list them.
extern u8 gSummon_OrderList[29];

/// Lists the summons, in the game's order, whose bit is set in the state's
/// first word, ends the list with 32 and returns how many it listed.
s32 Summon_ListLearned(u8 *output) {
  u8 *source = gSummon_OrderList;
  u8 *end = gSummon_OrderList + 28;
  s32 count = 0;
  /* FAKEMATCH: the one is kept in a register across the loop, as the
     native keeps it in r8; written as a constant in the shift, with the
     word pinned below, it is built in the loop (differ 18). */
  u32 one = 1;

  do {
    u8 value = *source;
    /* FAKEMATCH: the word is pinned to the r3 it takes in the native, so
       the and keeps its result there; in a plain local the word and the
       bit trade registers (differ 10). */
    register u32 flags asm("r3") = *(u32 *)Djinn_GetRecoveryTable(0);

    /* FAKEMATCH: the barrier loads the word before the bit is started; as
       plain C, the copy of the one comes first and 2 instructions differ
       in tla-en. */
    asm("");
    source++;
    flags &= one << value;
    if (flags != 0) {
      *output++ = value;
      count++;
    }
  } while (source <= end);
  *output = 32;
  return count;
}

const struct SummonDefinition *SummonDefinition_GetNear(u32 summonId) {
  if (summonId > 28)
    return NULL;
  return &gSummon_DefinitionTable[summonId];
}

void Func_080aff94(s32 bit) {
  u32 *state = (u32 *)Djinn_GetRecoveryTable(0);
  /* FAKEMATCH: the word pinned to r3 is loaded before the bit is built; in a
   * plain local, 10 instructions differ in tla-en. */
  register u32 flags __asm__("r3") = *state;

  flags |= 1 << bit;
  *state = flags;
}

#undef SUFFIX_BASE
#undef SUFFIX_COUNT
