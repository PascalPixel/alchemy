/// @file
/// Enemy formations: the enemies chosen, levelled and lettered.
// Part of battle.c.

#include "../battle_internal.h"

/// The rows of gFormations.
#define FORMATION_COUNT 380

// How many name letters tell the enemies of one class apart: 26 in Japanese,
// 9 in the localizations.
#if (LANGUAGE != LANGUAGE_JAPANESE)
#define ENEMY_NAME_SUFFIX_COUNT 9
#else
#define ENEMY_NAME_SUFFIX_COUNT 26
#endif

/// Returns a formation's average enemy level, for the monster battles to
/// choose by.
/// Returns -1 for a formation with no required member, -2 when it holds an
/// enemy above level 3 the party has not defeated (ENEMY_DEFEATED_FLAG,
/// unless FLAG_MONSTER_BATTLE_ANY_ENEMY is set), and -3 when it has no
/// member. Called by BattleFormation_SelectLevelMatchedCandidate for every
/// formation.
s32 BattleFormation_AverageLevel(s32 recordId) {
  const struct Formation *record;
  struct EnemyDefinition *enemy;
  u32 memberIndex;
  s32 enemyId;
  s32 levelTotal;
  s32 enemyCount;
  /* LEFTOVER(frame): an unused local the ROM's frame reserves and never
   * touches: no instruction addresses any of the 28 bytes it reserves (sub sp,
   * #28); as without it, 4 of 94 instructions differ in tbs-en. */
  UNUSED s32 levels[7];

  enemyCount = 0;
  levelTotal = 0;
  record = &gFormations[recordId];
  for (memberIndex = 0; memberIndex < FORMATION_MEMBERS; memberIndex++) {
    if (record->minimumCounts[memberIndex] != 0)
      break;
  }
  if (memberIndex == FORMATION_MEMBERS)
    return -1;
  // The average counts each member that may appear once, whatever its
  // numbers.
  for (memberIndex = 0; memberIndex < FORMATION_MEMBERS; memberIndex++) {
    if (record->maximumCounts[memberIndex] == 0)
      continue;
    enemyId = record->memberIds[memberIndex];
    enemy = (struct EnemyDefinition *)Enemy_GetDefinitionFar(enemyId +
                                                             ENEMY_ID_FIRST);
    if (enemy == 0)
      continue;
    if (enemy->level > 3 && !GameFlag_GetFar(FLAG_MONSTER_BATTLE_ANY_ENEMY) &&
        !GameFlag_GetFar(ENEMY_DEFEATED_FLAG(enemyId + ENEMY_ID_FIRST)))
      return -2;
    levelTotal += enemy->level;
    enemyCount++;
  }
  if (enemyCount == 0)
    return -3;
  return levelTotal / enemyCount;
}

/// A formation that may be chosen for the party's level: its row and its
/// enemies' average level. BattleFormation_SelectLevelMatchedCandidate keeps
/// a pool of FORMATION_POOL_SIZE in an EWRAM pool block, freed before it
/// returns.
struct FormationCandidate {
  s16 recordId; ///< the formation's row
  s16 score;    ///< its average level (BattleFormation_AverageLevel); -1 empty
};

/// The sprite slots one of member's enemies takes: two for a large enemy.
#define FORMATION_MEMBER_SLOTS(record, member)                                 \
  (2 -                                                                         \
   (BattleSprite_IsSmall((record)->memberIds[member] + ENEMY_ID_FIRST) != 0))

/// The most formations a monster battle chooses among.
#define FORMATION_POOL_SIZE 32
/// How far above the party's level a monster battle's formation may be.
#define FORMATION_LEVEL_MARGIN 3
/// The entries of gBattleFormation_LevelMatchedEnemies.
#define LEVEL_MATCHED_ENEMY_COUNT 20

/// A copy of a unit's base stats, from baseHp to its element stats, kept
/// while its level is adjusted: BattleEnemy_AdjustLevel takes it from the
/// EWRAM pool, reads only the stats it changes, for their 70 percent
/// floors, and frees it before returning.
struct BattleBaseStats {
  s16 baseHp;
  s16 basePp;
  s16 hpGauge; ///< copied, not read
  s16 ppGauge; ///< copied, not read
  u16 baseAttack;
  u16 baseDefense;
  u16 baseAgility;
  u8 baseLuck;                                      ///< copied, not read
  u8 actionCount;                                   ///< copied, not read
  u8 hpRegen;                                       ///< copied, not read
  u8 ppRegen;                                       ///< copied, not read
  u8 unused[2];                                     ///< Unit's baseUnused
  struct BattleElementStats element[ELEMENT_COUNT]; ///< the base power is read
};

/// Chooses a monster battle's formation for the party's level, and returns it
/// with the levels its enemies must gain in outMargin.
/// The target is the active party members' average level plus the monster
/// battles won in a row (FLAG_MONSTER_BATTLE_WINS), between 1 and 99; a
/// formation qualifies when its average level is at most 3 above it, and one of
/// up to 32 qualifying is chosen at random. With none, it returns formation 1
/// and a margin of 0. Called by BattleFormation_BuildEnemyList under
/// FLAG_MONSTER_BATTLE; clears the defeated flags of the classes in
/// gBattleFormation_LevelMatchedEnemies, which excludes those above level 3
/// unless FLAG_MONSTER_BATTLE_ANY_ENEMY bypasses the defeated-enemy check.
s32 BattleFormation_SelectLevelMatchedCandidate(s32 *outMargin) {
  struct FormationCandidate *pool =
      Heap_AllocEwram(FORMATION_POOL_SIZE * sizeof *pool);
  u16 party[8];
  s32 partySize = BattleParty_PrepareMembers(party);
  s32 levelTotal = 0;
  s32 matchCount = 0;
  s32 targetLevel;
  s32 result;
  s32 i;

  for (i = 0; i < partySize; i++)
    levelTotal += Unit_GetFar(party[i])->level;

  // The flag byte is -1 once a series has ended, which takes a level off.
  targetLevel = levelTotal / partySize;
  targetLevel += (s8)GameFlag_GetByteFar(FLAG_MONSTER_BATTLE_WINS);
  if (targetLevel <= 0)
    targetLevel = 1;
  if (targetLevel > UNIT_LEVEL_MAX)
    targetLevel = UNIT_LEVEL_MAX;

  for (i = 0; i < FORMATION_POOL_SIZE; i++)
    pool[i].score = -1;

  // Forget defeated classes that the monster battles normally exclude;
  // FLAG_MONSTER_BATTLE_ANY_ENEMY would still admit them. The definition
  // read is unused.
  for (i = 0; i < LEVEL_MATCHED_ENEMY_COUNT; i++) {
    Enemy_GetDefinitionFar(gBattleFormation_LevelMatchedEnemies[i]);
    GameFlag_ClearFar(
        ENEMY_DEFEATED_FLAG(gBattleFormation_LevelMatchedEnemies[i]));
  }

  // Each qualifying formation replaces the pool's lowest entry; empty
  // entries score -1, so the pool fills first.
  for (i = 0; i < FORMATION_COUNT; i++) {
    s32 score = BattleFormation_AverageLevel(i);

    if (score >= 0 && score <= targetLevel + FORMATION_LEVEL_MARGIN) {
      s32 lowestIndex = -1;
      s32 lowestLevel = 999;
      s32 entry;

      for (entry = 0; entry < FORMATION_POOL_SIZE; entry++) {
        if (pool[entry].score < lowestLevel) {
          lowestLevel = pool[entry].score;
          lowestIndex = entry;
        }
      }
      if (lowestIndex >= 0) {
        pool[lowestIndex].score = score;
        pool[lowestIndex].recordId = i;
        matchCount++;
      }
    }
  }

  // matchCount includes formations evicted from the pool.
  if (matchCount > FORMATION_POOL_SIZE)
    matchCount = FORMATION_POOL_SIZE;

  if (matchCount != 0) {
    struct FormationCandidate *chosen = &pool[(matchCount * Random16()) >> 16];

    result = chosen->recordId;
    *outMargin = targetLevel - chosen->score;
  } else {
    *outMargin = 0;
    result = 1;
  }

  Heap_Release(pool);
  return result;
}

/// Raises an enemy by levels, or lowers it for a negative count: each base
/// stat changes by a fixed amount a level, kept between 70 percent of what it
/// was and its cap.
/// A level is worth 9.7 HP, 1.5 PP, 12.3 Attack, 3.3 Defense, 5.1 Agility and
/// 15 power in each element; HP and PP are capped at UNIT_HP_MAX, the other
/// stats at UNIT_STAT_MAX and power at UNIT_ELEMENT_MAX. Changes the unit's
/// base stats and level and works out its stats again (Unit_Calculate). Called
/// by BattleFormation_BuildEnemyList for each enemy of a monster battle.
UNREAD_RESULT BattleEnemy_AdjustLevel(s32 unitId, s32 levels) {
  struct BattleBaseStats *base = Heap_AllocEwram(sizeof *base);
  struct Unit *unit = Unit_GetFar(unitId);
  s32 value;
  s32 element;

  Iwram_CopyWords(base, &unit->baseHp, sizeof *base);

  value = unit->baseHp + levels * 97 / 10;
  CLAMP(value, base->baseHp * 7 / 10, UNIT_HP_MAX);
  unit->baseHp = value;

  value = unit->basePp + levels * 15 / 10;
  CLAMP(value, base->basePp * 7 / 10, UNIT_HP_MAX);
  unit->basePp = value;

  value = unit->baseAttack + levels * 123 / 10;
  CLAMP(value, base->baseAttack * 7 / 10, UNIT_STAT_MAX);
  unit->baseAttack = value;

  value = unit->baseDefense + levels * 33 / 10;
  CLAMP(value, base->baseDefense * 7 / 10, UNIT_STAT_MAX);
  unit->baseDefense = value;

  value = unit->baseAgility + levels * 51 / 10;
  CLAMP(value, base->baseAgility * 7 / 10, UNIT_STAT_MAX);
  unit->baseAgility = value;

  for (element = 0; element < ELEMENT_COUNT; element++) {
    value = unit->baseElement[element].power + levels * 15;
    CLAMP(value, base->element[element].power * 7 / 10, UNIT_ELEMENT_MAX);
    unit->baseElement[element].power = value;
  }

  unit->level += levels;
  Unit_CalculateFar(unitId);
  Heap_Release(base);
}

/// Gives an enemy of the class its name letter, which tells it from others of
/// its kind, and returns the letter's number for BattleUnit_SetFar.
/// The letters are digits from 1 abroad and the alphabet in Japanese. The
/// class's first enemy gets ENEMY_NAME_SUFFIX_COUNT, which BattleUnit_SetFar
/// takes as no letter. Its second gets letter 1 with ENEMY_LETTER_FIRST_TOO,
/// telling the caller to give the first letter 0
/// (BattleEnemy_LetterFirstOfKind); later ones take the next free letter,
/// wrapping. Returns -1 when five classes are already listed. Called by
/// BattleFormation_BuildEnemyList for each enemy and by
/// BattleAbility_ApplyToTarget for an enemy called into the battle. Changes
/// BattleWork's enemyKinds, the up to five classes listed, and their letters:
/// lastNameLetter each class's last letter given (-1 for none yet),
/// nameLetterMasks its letters in use (bit n for letter n), and enemyKindCount
/// how many classes are listed.
s32 BattleEnemy_ClaimNameLetter(s32 classId, s32 unused) {
  struct BattleWork *battle;
  s32 classCount;
  s32 classIndex;
  s32 attempt;
  s32 suffix;

  battle = gBattleWork;
  classCount = battle->enemyKindCount;
  for (classIndex = 0; classIndex < classCount; classIndex++) {
    if (battle->enemyKinds[classIndex] == classId)
      break;
  }
  if (classIndex != classCount) {
    attempt = 0;
    // The second of its class: letters 0 and 1 both taken.
    if (battle->lastNameLetter[classIndex] < 0) {
      battle->lastNameLetter[classIndex] = 1;
      battle->nameLetterMasks[classIndex] = 3;
      return ENEMY_LETTER_FIRST_TOO | 1;
    }
    // The next letter after the last given that is free; after 32 tries the
    // last one tried is reused.
    for (; attempt <= 31; attempt++) {
      suffix =
          (battle->lastNameLetter[classIndex] + 1) % ENEMY_NAME_SUFFIX_COUNT;
      battle->lastNameLetter[classIndex] = suffix;
      if ((battle->nameLetterMasks[classIndex] & (1 << (s8)suffix)) == 0)
        break;
    }
    battle->nameLetterMasks[classIndex] |=
        1 << battle->lastNameLetter[classIndex];
    return battle->lastNameLetter[classIndex];
  }
  if (classCount <= 4) {
    battle->lastNameLetter[classCount] = -1;
    battle->enemyKinds[classCount] = classId;
    battle->nameLetterMasks[classCount] = 0;
    battle->enemyKindCount = classCount + 1;
    return ENEMY_NAME_SUFFIX_COUNT;
  }
  return -1;
}

/// Frees a departing enemy's name letter for the next of its class, reading
/// the letter back from the end of its name.
/// Does nothing for a unit that is not an enemy, a class not listed or one
/// with no letters given. Clears the letter's bit in
/// BattleWork.nameLetterMasks. Called by BattleObject_RemoveFromLists.
UNREAD_RESULT BattleEnemy_ReleaseNameLetter(s32 unitId) {
  struct BattleWork *battle = gBattleWork;
  struct Unit *unit;
  s32 count = battle->enemyKindCount;
  s32 index;
  s32 nameLength;
  s32 suffixIndex;
  s32 classId;

  unit = Unit_GetFar(unitId);
  if (unit->classId != CLASS_NONE)
    return;

  classId = unit->id;
  for (index = 0; index < count; index++) {
    if (battle->enemyKinds[index] == classId)
      break;
  }
  if (index == count || battle->nameLetterMasks[index] == 0)
    return;

  for (nameLength = 0; nameLength <= 13; nameLength++) {
    if (unit->name[nameLength] == 0)
      break;
  }

  // An empty name uses a shift of 32, which emits zero on ARM. A name with
  // no suffix instead uses its last character as a suffix.
  suffixIndex = 32;
  if (nameLength > 0)
    suffixIndex = unit->name[nameLength - 1] - LIST_MARKER_CHAR;
  battle->nameLetterMasks[index] &= ~(1 << suffixIndex);
}

/// Gives the first enemy of the class on the field the first letter (0),
/// appended to its name, once a second of its class arrives.
/// Called by BattleFormation_BuildEnemyList and BattleAbility_ApplyToTarget
/// when BattleEnemy_ClaimNameLetter's result has ENEMY_LETTER_FIRST_TOO set.
/// Writes Unit.name.
UNREAD_RESULT BattleEnemy_LetterFirstOfKind(s32 classId) {
  s32 enemy;

  for (enemy = 0; enemy < ENEMY_BATTLE_SIZE; enemy++) {
    struct Unit *unit = Unit_GetFar(FIRST_ENEMY_UNIT + enemy);
    s32 length;

    if (unit->side != BATTLE_SIDE_ENEMY || unit->id != classId)
      continue;
    // The letter goes after the name, with a new terminator after it; a name
    // that fills all 14 characters takes none.
    length = 0;
  findEnd:
    if (unit->name[length] != 0) {
      if (++length > 13)
        return;
      goto findEnd;
    }
    unit->name[length] = LIST_MARKER_CHAR;
    unit->name[length + 1] = 0;
    return;
  }
}

/// Returns the class id of a random member of the formation, among those
/// that may appear (maximum count above 0); formation 1 stands in for an id
/// out of range.
/// Called by BattleAbility_ApplyToTarget when an enemy calls for help.
s32 BattleFormation_GetRandomMember(s32 recordId) {
  s32 choices[FORMATION_MEMBERS];
  const struct Formation *record;
  u32 choiceCount = 0;
  s32 member;

  if (recordId < 0 || recordId >= FORMATION_COUNT)
    recordId = 1;
  record = &gFormations[recordId];
  for (member = 0; member < FORMATION_MEMBERS; member++) {
    if (record->maximumCounts[member] != 0)
      choices[choiceCount++] = record->memberIds[member] + ENEMY_ID_FIRST;
  }
  return choices[(Random16() * choiceCount) >> 16];
}

/// Builds the battle's enemies from formation recordId and returns how many
/// there are.
/// Each member comes its minimum number of times, plus a random number more
/// up to its maximum while the six sprite slots hold them (a large enemy
/// takes two). The formation's layout orders them: shuffled kinds, each
/// kind together (FORMATION_LAYOUT_SHUFFLED), interleaved at random
/// (FORMATION_LAYOUT_INTERLEAVED), or in the row's order. Under
/// FLAG_MONSTER_BATTLE the formation is chosen for the party's level instead,
/// and each enemy raised to it. An empty or out-of-range formation becomes
/// formation 1. Called once by Battle_Run. Fills the enemy units
/// from FIRST_ENEMY_UNIT, with their name letters, and sets BattleWork's
/// battleType, firstDefeated, defeatState and the name-letter lists.
s32 BattleFormation_BuildEnemyList(s32 recordId) {
  struct BattleWork *battle = gBattleWork;
  const struct Formation *record;
  u16 enemyKinds[UNIT_MAX];
  s32 extraCounts[FORMATION_MEMBERS];
  s32 memberCounts[FORMATION_MEMBERS];
  s32 memberOrder[FORMATION_MEMBERS];
  s32 levelAdjustment = 0;
  s32 freeSpriteSlots;
  s32 enemyCount;
  s32 changed;
  s32 member;
  s32 copy;
  s32 i;

  battle->enemyKindCount = 0;
  if (GameFlag_GetFar(FLAG_MONSTER_BATTLE))
    recordId = BattleFormation_SelectLevelMatchedCandidate(&levelAdjustment);
  if (recordId < 0 || recordId >= FORMATION_COUNT)
    recordId = 1;
  record = &gFormations[recordId];
  for (member = 0; member < FORMATION_MEMBERS; member++) {
    if (record->minimumCounts[member] != 0)
      break;
  }
  if (member == FORMATION_MEMBERS)
    record = &gFormations[1];

  // The six sprite slots left once every member's minimum is placed; a
  // large enemy takes two.
  freeSpriteSlots = ENEMY_BATTLE_SIZE;
  for (member = 0; member < FORMATION_MEMBERS; member++) {
    if (record->minimumCounts[member] != 0)
      freeSpriteSlots -= FORMATION_MEMBER_SLOTS(record, member) *
                         record->minimumCounts[member];
  }

  // Each member's extra enemies: a random number from 0 to as many more as
  // its maximum allows and the free slots would hold on their own.
  for (member = 0; member < FORMATION_MEMBERS; member++) {
    s32 room = record->maximumCounts[member] - record->minimumCounts[member];

    memberCounts[member] = record->minimumCounts[member];
    extraCounts[member] = 0;
    if (room > 0) {
      s32 fitting = freeSpriteSlots / FORMATION_MEMBER_SLOTS(record, member);

      if (fitting < room)
        room = fitting;
      extraCounts[member] = ((room + 1) * Random16()) >> 16;
    }
  }

  // The extras then take the shared slots one per member a pass, in member
  // order, until none fits.
  do {
    changed = 0;
    for (member = 0; member < FORMATION_MEMBERS; member++) {
      s32 slots = FORMATION_MEMBER_SLOTS(record, member);

      if (extraCounts[member] == 0)
        continue;
      if (slots > freeSpriteSlots) {
        extraCounts[member] = 0;
      } else {
        memberCounts[member]++;
        extraCounts[member]--;
        freeSpriteSlots -= slots;
        changed = 1;
      }
    }
  } while (changed);

  // The list of class ids, in the order the battle type gives, ended by 0.
  enemyCount = 0;
  battle->layout = record->layout;
  switch (battle->layout) {
  case FORMATION_LAYOUT_SHUFFLED:
    // The members' order shuffled by ten random swaps.
    for (member = 0; member < FORMATION_MEMBERS; member++)
      memberOrder[member] = member;
    for (i = 0; i < 10; i++) {
      s32 first = (Random16() * FORMATION_MEMBERS) >> 16;
      s32 second = (Random16() * FORMATION_MEMBERS) >> 16;
      s32 swap = memberOrder[first];

      memberOrder[first] = memberOrder[second];
      memberOrder[second] = swap;
    }
    for (i = 0; i < FORMATION_MEMBERS; i++) {
      member = memberOrder[i];
      for (copy = 0; copy < memberCounts[member]; copy++)
        enemyKinds[enemyCount++] = record->memberIds[member] + ENEMY_ID_FIRST;
    }
    break;
  case FORMATION_LAYOUT_INTERLEAVED:
    // Each enemy drawn from a random member with any left.
    for (;;) {
      s32 remainingKinds = 0;

      for (member = 0; member < FORMATION_MEMBERS; member++) {
        if (memberCounts[member] != 0)
          memberOrder[remainingKinds++] = member;
      }
      if (remainingKinds == 0)
        break;
      member = memberOrder[(Random16() * remainingKinds) >> 16];
      enemyKinds[enemyCount++] = record->memberIds[member] + ENEMY_ID_FIRST;
      memberCounts[member]--;
    }
    break;
  default:
    for (member = 0; member < FORMATION_MEMBERS; member++) {
      for (copy = 0; copy < memberCounts[member]; copy++)
        enemyKinds[enemyCount++] = record->memberIds[member] + ENEMY_ID_FIRST;
    }
    break;
  }
  // No class has fallen yet (a slot past the last), and the enemy units
  // start cleared.
  enemyKinds[enemyCount] = 0;
  battle->firstDefeated = ENEMY_BATTLE_SIZE;
  battle->defeatState = DEFEAT_ONE;
  for (i = 0; i < ENEMY_BATTLE_SIZE; i++)
    Iwram_ClearWords(Unit_GetFar(FIRST_ENEMY_UNIT + i), BATTLE_UNIT_SIZE);

  // Each enemy unit, with its name letter (BattleEnemy_ClaimNameLetter) and,
  // in a monster battle, the level margin.
  for (i = 0; i < ENEMY_BATTLE_SIZE && enemyKinds[i] != 0; i++) {
    s32 letter = BattleEnemy_ClaimNameLetter(enemyKinds[i], 1);

    if (letter & ENEMY_LETTER_FIRST_TOO)
      BattleEnemy_LetterFirstOfKind(enemyKinds[i]);
    BattleUnit_SetFar(FIRST_ENEMY_UNIT + i, enemyKinds[i],
                      letter & ENEMY_LETTER_MASK);
    // DEAD: the unit, fetched and not used; the ROM keeps the call.
    Unit_GetFar(FIRST_ENEMY_UNIT + i);
    if (levelAdjustment != 0)
      BattleEnemy_AdjustLevel(FIRST_ENEMY_UNIT + i, levelAdjustment);
  }
  return i;
}
