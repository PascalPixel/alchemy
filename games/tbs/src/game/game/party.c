/// @file
/// Enemy units, the active party, coins and counters, and summons.
// Part of game.c.

#include "../game_internal.h"

// The letter that tells apart enemies of one kind: digits from 1 abroad, the
// alphabet in Japan.
#if (LANGUAGE != LANGUAGE_JAPANESE)
#define ENEMY_SUFFIX_FIRST '1' ///< suffix 0's character
#define ENEMY_SUFFIX_LAST 8    ///< the last suffix shown, '9'
#else
#define ENEMY_SUFFIX_FIRST 'A' ///< suffix 0's character
#define ENEMY_SUFFIX_LAST 25   ///< the last suffix shown, 'Z'
#endif

/// Fills battle unit `unitId` (0x80 to 0x86) with enemy `enemyId`: stats,
/// name with its suffix letter, the items it carries, rewards and elements.
/// Enemies 158 to 171 fight on the party's side. Returns 0 for an id out of
/// range, 1 otherwise; enemy ids 173 to 250, past the table's 165 rows but
/// within the check, are filled from row 0. Called by the battle set-up
/// (Battle_RunEncounter, BattleFormation_BuildEnemyList) and by
/// BattleTargetExec when an enemy calls for help.
/// XXX: which enemies 158 to 171 are, and when they join the party's side, is
/// unknown; the encounter tables that place them would say.
s32 BattleUnitSet(s32 unitId, s32 enemyId, s32 suffix) {
  struct BattleUnit *unit;
  const struct EnemyDefinition *enemy;
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
  if (index > 242)
    return 0;
  unit = UnitGet(unitId);
  Iwram_ClearWords(unit, BATTLE_UNIT_SIZE);
  if (index > 164)
    index = 0;
  enemy = &gEnemy_Definitions[index];
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
  UiText_DecodeMessageFar(index + MsgEnemyName, name, 15);
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
  unit->rewards = enemy->rewards;
  unit->classIndex = 0;
  unit->classId = enemyId;
#ifdef UBFIX
  Unit_CalcBaseElements(unitId, unit->baseElement);
#else
  Unit_CalcBaseElements(unitId, (s16 (*)[2])unit->baseElement);
#endif
  UnitCalc(unitId);
  unit->side = BATTLE_UNIT_ENEMY;
  switch (unit->classId) {
  case 158:
  case 159:
  case 160:
  case 161:
  case 162:
  case 163:
  case 164:
  case 165:
  case 166:
  case 167:
  case 168:
  case 169:
  case 170:
  case 171:
    unit->side = BATTLE_UNIT_PARTY;
    break;
  }
  return 1;
}

// The party: flags 0 to 7 say who is in it, and gGameState.activeOwners
// lists them in order.

/// Returns the number of party members, counted from game flags 0 to 7.
s32 Party_CountActiveOwners(void) {
  s32 owner;
  s32 count;

  count = 0;
  owner = 0;
  do {
    if (GameFlag_Test(owner) != 0)
      count++;
    owner++;
  } while (owner <= 7);
  return count;
}

/// Adds character `owner` to the end of the party: sets its game flag and
/// appends it to gGameState.activeOwners unless it is already listed.
/// Returns the party size. Called by the scenes where a character joins, by
/// the member chooser and the file select screen, and by the debug presets.
s32 Party_AddActiveOwner(s32 owner) {
  s32 partySize = Party_CountActiveOwners();
  s32 partyIndex;

  GameFlag_SetBit(owner);
  partyIndex = 0;
  while (partyIndex < partySize) {
    if (gGameState.activeOwners[partyIndex] == owner)
      return partySize;
    partyIndex++;
  }
  gGameState.activeOwners[partyIndex] = owner;
  return partySize + 1;
}

/// Takes character `owner` out of the party: clears its game flag and closes
/// the gap it leaves in gGameState.activeOwners. Returns the party size.
/// Called by the scenes where a character leaves, by the member chooser and
/// the file select screen, and by the debug presets.
s32 Party_RemoveActiveOwner(s32 owner) {
  s32 partySize = Party_CountActiveOwners();
  s32 removedIndex;
  s32 partyIndex;

  GameFlag_ClearBit(owner);
  for (removedIndex = 0; removedIndex < partySize; removedIndex++) {
    if (gGameState.activeOwners[removedIndex] == owner)
      break;
  }
  for (partyIndex = removedIndex; partyIndex < partySize - 1; partyIndex++)
    gGameState.activeOwners[partyIndex] =
        gGameState.activeOwners[partyIndex + 1];
  return Party_CountActiveOwners();
}

/// Writes the party members to `owners`, ending with 0xff, and returns how
/// many there are. Every screen that lists the party starts here: the menus,
/// the shops, the inn and the party inventory searches.
s32 Party_ListActiveOwners(s16 *owners) {
  s32 count = 0;

  if (owners != NULL) {
    s32 index;

    count = Party_CountActiveOwners();
    index = 0;
    if (count != 0) {
      do {
        *owners++ = gGameState.activeOwners[index];
        index++;
      } while (index != count);
    }
    *owners = 0xff;
  }
  return count;
}

/// Adds `amount` to the party's coins, kept between 0 and 999999, and returns
/// the new total. Called by the shops, the inn and the sanctum to charge,
/// by the battle spoils to pay, and by the Lucky Dice game.
s32 Party_AdjustCoins(s32 amount) {
  s32 value;
  struct GameState *work;
  struct GameState *store;

  work = &gGameState;
  value = work->coins;
  value += amount;
  store = work;
  if (value > 999999) {
    value = 999999;
  }
  if (value < 0) {
    value = 0;
  }
  work = store;
  work->coins = value;
  return value;
}

/// The party's Djinn recovery table as the summon list reads it: a mask of
/// summons, bit n for summon n.
struct TradeFlags {
  /// Bit n set: the party's Djinn meet summon n's requirement, as
  /// BattleSummon_UpdateAvailability last worked it out.
  u32 flags;
};

/// Each adds `amount` to a game state counter, kept between 0 and `max`, and
/// returns the new value. The shops call the two: each purchase adds its
/// price to the credit, and each Game Ticket given spends credit and counts
/// one more gift.
#define PARTY_DEFINE_ADJUST(name, field, max)                                  \
  s32 name(s32 amount) {                                                       \
    s32 value;                                                                 \
    struct GameState *progress = &gGameState;                                  \
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

/// Writes the summons whose bit is set in the first word of the party's Djinn
/// table to `output`, in menu order, ending with 32. Returns how many.
/// Called by SummonMenu_SelectSummon to list the battle's Summon command.
/// The bits are those BattleSummon_UpdateAvailability sets at the battle's
/// start and each round. `output` needs room for 17 bytes: up to 16 summons
/// and the 32.
s32 Summon_ListLearned(u8 *output) {
  u8 *source = gSummon_OrderList;
  u8 *end = gSummon_OrderList + 15;
  s32 count = 0;

  do {
    u8 value = *source++;
    if ((((struct TradeFlags *)Djinn_GetRecoveryTable(0))->flags &
         (1 << value)) != 0) {
      *output++ = value;
      count++;
    }
  } while (source <= end);
  *output = 32;
  return count;
}

/// Returns summon `summonId`'s definition, or NULL past the sixteenth. Reached
/// only through GAME.S, as SummonDefinition_Get.
const struct SummonDefinition *SummonDefinition_GetNear(u32 summonId) {
  if (summonId > 15)
    return NULL;
  return &gSummon_DefinitionTable[summonId];
}

#undef ENEMY_SUFFIX_FIRST
#undef ENEMY_SUFFIX_LAST
#undef PARTY_DEFINE_ADJUST
