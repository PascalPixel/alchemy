/// @file
/// Enemy units, the active party, coins and counters, and summons.
// Part of game.c.

#include "game_internal.h"

// The letter that tells apart enemies of one kind: digits from 1 abroad, the
// alphabet in Japan.
#if (LANGUAGE != LANGUAGE_JAPANESE)
#define ENEMY_SUFFIX_FIRST '1' ///< suffix 0's character
#define ENEMY_SUFFIX_LAST 8    ///< the last suffix shown, '9'
#else
#define ENEMY_SUFFIX_FIRST 'A' ///< suffix 0's character
#define ENEMY_SUFFIX_LAST 25   ///< the last suffix shown, 'Z'
#endif

/// Fills a battle unit with an enemy: its stats, its name with its letter, its
/// items, rewards and elements. A Djinni fought is put on the party's side
/// (README.md, "The unit"). Returns 0 for an id out of range.
s32 BattleUnit_Set(s32 unitId, s32 enemyId, s32 suffix) {
  struct Unit *unit;
  const struct EnemyDefinition *enemy;
  u16 name[15];
  s32 i;
  u32 index;
  s32 q;
  s32 count;

  index = enemyId - ENEMY_ID_FIRST;
  if (unitId < FIRST_ENEMY_UNIT)
    return 0;
  // BUG: one too many: unit FIRST_ENEMY_UNIT + 6 passes, but Unit_Get has no
  // seventh enemy unit and gives NULL. No caller passes it.
  if (unitId > FIRST_ENEMY_UNIT + 6)
    return 0;
  if (index > 242)
    return 0;
  unit = Unit_Get(unitId);
  Iwram_ClearWords(unit, BATTLE_UNIT_SIZE);
  if (index > 164)
    index = 0;
  enemy = &gEnemy_Definitions[index];
  unit->level = enemy->level;
  unit->baseHp = enemy->hp;
  unit->hp = enemy->hp;
  unit->hpMax = enemy->hp;
  unit->basePp = enemy->pp;
  unit->pp = enemy->pp;
  unit->ppMax = enemy->pp;
  unit->hpGauge = UNIT_GAUGE_FULL;
  unit->ppGauge = UNIT_GAUGE_FULL;
  unit->baseAttack = enemy->atk;
  unit->baseDefense = enemy->def;
  unit->baseAgility = enemy->agi;
  unit->baseLuck = enemy->luck;
  unit->baseActionCount = enemy->actionCount;
  unit->baseHpRegen = enemy->hpRegen;
  unit->basePpRegen = enemy->ppRegen;
  Message_DecodeRawFar(index + MsgEnemyName, name, 15);
  for (i = 0; i < 14 && name[i] != 0; i++)
    unit->name[i] = name[i];
  // Enemies of one kind are told apart by a letter after the name; past the
  // last letter the name stays bare.
  if (suffix <= ENEMY_SUFFIX_LAST) {
    unit->name[i] = ENEMY_SUFFIX_FIRST + suffix;
    i++;
  }
  unit->name[i] = 0;
  unit->name[14] = 0;
  count = 0;
  for (i = 0; i < 4; i++) {
    if (enemy->items[i] != 0) {
      for (q = 0; q < enemy->counts[i]; q++) {
        if (count < INVENTORY_SLOTS)
          unit->inventory[count++] = enemy->items[i];
      }
    }
  }
  unit->aiActionIndex = enemy->aiActionIndex;
  unit->classId = 0;
  unit->id = enemyId;
  Unit_CalculateBaseElements(unitId, unit->baseElement);
  Unit_Calculate(unitId);
  unit->side = BATTLE_SIDE_ENEMY;
  switch (unit->id) {
  case ENEMY_VENUS_DJINNI_QUARTZ:
  case ENEMY_VENUS_DJINNI_VINE:
  case ENEMY_VENUS_DJINNI_UNUSED_BANE:
  case ENEMY_MERCURY_DJINNI_SLEET:
  case ENEMY_MERCURY_DJINNI_SPRITZ:
  case ENEMY_MERCURY_DJINNI_HAIL:
  case ENEMY_MERCURY_DJINNI_DEW:
  case ENEMY_MARS_DJINNI_FORGE:
  case ENEMY_MARS_DJINNI_CORONA:
  case ENEMY_MARS_DJINNI_FLASH:
  case ENEMY_JUPITER_DJINNI_BREEZE:
  case ENEMY_JUPITER_DJINNI_ZEPHYR:
  case ENEMY_JUPITER_DJINNI_SQUALL:
  case ENEMY_JUPITER_DJINNI_LUFF:
    unit->side = BATTLE_SIDE_PARTY;
    break;
  }
  return 1;
}

// The party: the FLAG_IN_PARTY flags say who is in it, and
// gMainRoutineWork.party lists them in order.

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

/// Writes the party members to unitIds and returns how many there are.
s32 Party_ListMembers(s16 *unitIds) {
  s32 count = 0;

  if (unitIds != NULL) {
    s32 index;

    count = Party_CountMembers();
    for (index = 0; index != count; index++) {
      *unitIds++ = gMainRoutineWork.party[index];
    }
    *unitIds = BATTLE_UNIT_LIST_END;
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

/// Each adds amount to a counter, kept between 0 and max, and returns the new
/// value: the shop credit and the Game Tickets given.
#define PARTY_DEFINE_ADJUST(name, field, max)                                  \
  s32 name(s32 amount) {                                                       \
    s32 value;                                                                 \
    struct MainRoutineWork *progress = &gMainRoutineWork;                      \
                                                                               \
    value = progress->field;                                                   \
    value += amount;                                                           \
    if (value > max)                                                           \
      value = max;                                                             \
    if (value < 0)                                                             \
      value = 0;                                                               \
    progress->field = value;                                                   \
    return value;                                                              \
  }

PARTY_DEFINE_ADJUST(Party_AdjustShopCredit, shopCredit, 999999)
PARTY_DEFINE_ADJUST(Party_AdjustShopGifts, shopGifts, 28)

/// Writes the summons the party can call to output, in menu order, ending with
/// SUMMON_END, and returns how many.
s32 Summon_ListLearned(u8 *output) {
  const u8 *summon = gSummon_OrderList;
  const u8 *last = &gSummon_OrderList[15];
  s32 count = 0;

  do {
    u8 summonId = *summon++;

    if (Djinn_GetRecoveryTable(DJINN_RECOVERY_PARTY)->summonMask &
        (1 << summonId)) {
      *output++ = summonId;
      count++;
    }
  } while (summon <= last);
  *output = SUMMON_END;
  return count;
}

/// Returns summon `summonId`'s definition, or NULL past the sixteenth.
const struct SummonDefinition *SummonDefinition_GetNear(u32 summonId) {
  if (summonId > 15)
    return NULL;
  return &gSummon_DefinitionTable[summonId];
}

#undef ENEMY_SUFFIX_FIRST
#undef ENEMY_SUFFIX_LAST
#undef PARTY_DEFINE_ADJUST
