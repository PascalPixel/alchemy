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
  const struct EnemyDefinition *enemy;
  u16 name[15];
  s32 i;
  u32 index;
  s32 q;
  s32 count;

  index = enemyId - ENEMY_ID_FIRST;
  if (unitId <= FIRST_ENEMY_UNIT - 1)
    return 0;
  if (unitId > FIRST_ENEMY_UNIT + 6)
    return 0;
  if (index > 992)
    return 0;
  unit = Unit_Get(unitId);
  {
    s32 (*clear)(void *destination, s32 size) = Iwram_ClearWordsEntry;

    clear(unit, BATTLE_UNIT_SIZE);
  }
  if (index >= ENEMY_COUNT)
    index = 0;
  enemy = &gEnemy_DefinitionTable[index];
  unit->level = enemy->level;
  unit->baseHp = enemy->hp;
  unit->hp = enemy->hp;
  unit->maxHp = enemy->hp;
  unit->basePp = enemy->pp;
  unit->pp = enemy->pp;
  unit->maxPp = enemy->pp;
  unit->hpGauge = UNIT_GAUGE_FULL;
  unit->ppGauge = UNIT_GAUGE_FULL;
  unit->baseAttack = enemy->atk;
  unit->baseDefense = enemy->def;
  unit->baseAgility = enemy->agi;
  unit->baseLuck = enemy->luck;
  unit->baseActionCount = enemy->actionCount;
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
  s32 unitId;
  s32 count;

  count = 0;
  for (unitId = 0; unitId <= PIERS; unitId++) {
    if (GameFlag_Get(FLAG_IN_PARTY(unitId)) != 0)
      count++;
  }
  return count;
}

/// Adds a character to the end of the party unless it is there already, and
/// returns the party size.
s32 Party_AddMember(s32 unitId) {
  s32 partySize = Party_CountMembers();
  s32 partyIndex;

  GameFlag_Set(FLAG_IN_PARTY(unitId));
  for (partyIndex = 0; partyIndex < partySize; partyIndex++) {
    if (gMainRoutineWork.party[partyIndex] == unitId)
      return partySize;
  }
  gMainRoutineWork.party[partyIndex] = unitId;
  return partySize + 1;
}

/// Takes character `unitId` out of the party: clears its game flag and closes
/// the gap it leaves in gMainRoutineWork.party. Returns the party size.
s32 Party_RemoveMember(s32 unitId) {
  s32 partySize = Party_CountMembers();
  s32 removedIndex;
  s32 partyIndex;

  GameFlag_Clear(FLAG_IN_PARTY(unitId));
  for (removedIndex = 0; removedIndex < partySize; removedIndex++) {
    if (gMainRoutineWork.party[removedIndex] == unitId)
      break;
  }
  for (partyIndex = removedIndex; partyIndex < partySize - 1; partyIndex++)
    gMainRoutineWork.party[partyIndex] = gMainRoutineWork.party[partyIndex + 1];
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
        *owners++ = gMainRoutineWork.party[index];
        index++;
      } while (index != count);
    }
    *owners = 0xff;
  }
  return count;
}

/// Adds `amount` to the party's coins, kept between 0 and 999999, and returns
/// the new total.
s32 Party_AdjustCoins(s32 amount) {
  s32 value;
  struct MainRoutineWork *work;

  work = &gMainRoutineWork;
  value = work->coins;
  value += amount;
  if (value > 999999) {
    value = 999999;
  }
  if (value < 0) {
    value = 0;
  }
  work->coins = value;
  return value;
}

s32 Party_AdjustShopCredit(s32 amount) {
  s32 value;

  value = gMainRoutineWork.shopCredit;
  value += amount;
  if (value > 0xf423f)
    value = 0xf423f;
  if (value < 0)
    value = 0;
  gMainRoutineWork.shopCredit = value;
  return value;
}

s32 Party_AdjustShopGifts(s32 amount) {
  s32 value;

  value = gMainRoutineWork.shopGifts;
  value += amount;
  if (value > 28)
    value = 28;
  if (value < 0)
    value = 0;
  gMainRoutineWork.shopGifts = value;
  return value;
}

/// Lists the summons, in the game's order, whose bit is set in the state's
/// first word, ends the list with 32 and returns how many it listed.
s32 Summon_ListLearned(u8 *output) {
  const u8 *summon = gSummon_OrderList;
  const u8 *last = &gSummon_OrderList[SUMMON_COUNT - 1];
  s32 count = 0;

  do {
    u8 summonId = *summon++;

    if (Djinn_GetRecoveryTable(0)->summonMask & (1 << summonId)) {
      *output++ = summonId;
      count++;
    }
  } while (summon <= last);
  *output = SUMMON_END;
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
