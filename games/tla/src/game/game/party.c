/// @file
/// Enemy units, the active party, coins and counters, and summons.
// Part of game.c.

#include "../game_internal.h"
#include "gamepak/far_call/window.h"
#include "module/game/game.h"
#include "text/msg_ids.h"

// A unit whose suffix is below SUFFIX_COUNT gets one more character after its
// name, SUFFIX_BASE plus the suffix: the digits 1 to 9 outside Japan, and the
// codes 65 to 90 in the Japanese release (String_FindFirstDigit reads it back).
#if LANGUAGE == LANGUAGE_JAPANESE
#define SUFFIX_BASE 65
#define SUFFIX_COUNT 26
#else
#define SUFFIX_BASE '1'
#define SUFFIX_COUNT 9
#endif

s32 BattleUnit_Set(s32 unitId, s32 enemyId, s32 suffix) {
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
    s32 (*clear)(void *destination, s32 size) = Iwram_ClearWordsEntry;

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

/// Returns the number of party members, counted from its FLAG_IN_PARTY flags.
s32 Party_CountMembers(void) {
  s32 count;
  s32 i;

  count = 0;
  for (i = 0; i <= 7; i++) {
    if (GameFlag_Get(i))
      count++;
  }
  return count;
}

/// Adds character `unitId` to the end of the party: sets its game flag and
/// appends it to gMainRoutineWork.party unless it is already listed.
/// Returns the party size. Called by the events where a character joins, by
/// the party member chooser and the File Screen, and by the debug parameter
/// sets.
s32 Party_AddMember(s32 value) {
  s32 count = Party_CountMembers();
  s32 index;

  GameFlag_Set(value);
  index = 0;
  while (index < count) {
    if (gMainRoutineWork.activeOwners[index] == value)
      return count;
    index++;
  }
  gMainRoutineWork.activeOwners[index] = value;
  return count + 1;
}

/// Takes character `unitId` out of the party: clears its game flag and closes
/// the gap it leaves in gMainRoutineWork.party. Returns the party size.
/// Called by the events where a character leaves, by the party member chooser
/// and the File Screen, and by the debug parameter sets.
s32 Party_RemoveMember(s32 value) {
  s32 count = Party_CountMembers();
  s32 i;
  s32 j;

  GameFlag_Clear(value);
  for (i = 0; i < count; i++) {
    if (gMainRoutineWork.activeOwners[i] == value)
      break;
  }
  for (j = i; j < count - 1; j++)
    gMainRoutineWork.activeOwners[j] = gMainRoutineWork.activeOwners[j + 1];
  return Party_CountMembers();
}

/// Writes the party members to `unitIds`, ending with BATTLE_UNIT_LIST_END, and
/// returns how many there are. Every screen that lists the party starts here:
/// the menus, the shops, the inn and the party inventory searches.
s32 Party_ListMembers(s16 *owners) {
  s32 count = 0;

  if (owners != NULL) {
    s32 index;

    count = Party_CountMembers();
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
/* VIEW(game): variable gSummon_OrderList as u8 gSummon_OrderList[29];
 * game/summon_table has const u8 gSummon_OrderList[29] = {; tried as the
 * definition's extern const u8 gSummon_OrderList[29], it does not compile in
 * tla-ja: games/tla/src/game/game/party.c:210: `gSummon_OrderList' undeclared
 * (first use in this function). */
extern u8 gSummon_OrderList__game[29] __asm__("gSummon_OrderList");
#define gSummon_OrderList gSummon_OrderList__game

/// Lists the summons, in the game's order, whose bit is set in the state's
/// first word, ends the list with 32 and returns how many it listed.
s32 Summon_ListLearned(u8 *output) {
  u8 *source = gSummon_OrderList;
  u8 *end = gSummon_OrderList + 28;
  s32 count = 0;

  do {
    u8 value = *source++;
    if (((*(u32 *)Djinn_GetRecoveryTable(0)) & (1 << value)) != 0) {
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

/// Returns the class series of a character whose strongest element is
/// `primaryElement` and second `secondaryElement`, from the 4 by 4 table.
/// Called by Class_Determine.
void Class_GetSeries(s32 bit) {
  u32 *state = (u32 *)Djinn_GetRecoveryTable(0);

  *state |= 1 << bit;
}

#undef SUFFIX_BASE
#undef SUFFIX_COUNT
