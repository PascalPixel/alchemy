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
/// unless flag 0x174 is set), and -3 when it has no member. Called by
/// BattleFormation_SelectLevelMatchedCandidate for every formation.
s32 BattleFormation_AverageLevel(s32 recordId) {
  struct Formation *record;
  struct EnemyDefinition *enemy;
  u32 memberIndex;
  s32 enemyId;
  s32 levelTotal;
  s32 enemyCount;
  /* FAKEMATCH(dead): an unused array for the frame's 28 bytes; without it, 2
   * of 94 instructions differ in tbs-en. */
  UNUSED s32 levels[7];

  enemyCount = 0;
  levelTotal = 0;
  record = &gFormations[recordId];
  for (memberIndex = 0; memberIndex < 5; memberIndex++) {
    if (record->minimumCounts[memberIndex] != 0)
      break;
  }
  if (memberIndex == 5)
    return -1;
  // The average counts each member that may appear once, whatever its
  // numbers.
  for (memberIndex = 0; memberIndex < 5; memberIndex++) {
    if (record->maximumCounts[memberIndex] == 0)
      continue;
    enemyId = record->memberIds[memberIndex];
    enemy = (struct EnemyDefinition *)Enemy_GetDefinitionFar(enemyId +
                                                             ENEMY_ID_FIRST);
    if (enemy == 0)
      continue;
    // XXX: flag 0x174 makes every enemy eligible; what sets it would name it.
    if (enemy->level > 3 && !GameFlag_TestFar(0x174) &&
        !GameFlag_TestFar(ENEMY_DEFEATED_FLAG(enemyId + ENEMY_ID_FIRST)))
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
/// a pool of 32 in a 128-byte EWRAM pool block, freed before it returns.
struct FormationCandidate {
  s16 recordId; ///< the formation's row
  s16 score;    ///< its average level (BattleFormation_AverageLevel); -1 empty
};

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
/// The target is the active members' average level plus the monster battles
/// won in a row (FLAG_MONSTER_BATTLE_WINS), between 1 and 99; a formation
/// qualifies when its average level is at most 3 above it, and one of up to
/// 32 qualifying is chosen at random. With none, it returns formation 1 and a
/// margin of 0. Called by BattleFormation_BuildEnemyList under
/// FLAG_MONSTER_BATTLE; clears the defeated flags of the classes in
/// gBattleFormation_LevelMatchedEnemies, excluding them unless flag 0x174
/// bypasses the defeated-enemy check.
s32 BattleFormation_SelectLevelMatchedCandidate(s32 *outMargin) {
  s32 matchCount = 0;
  struct FormationCandidate *pool = AllocEwram(128);
  u16 party[8];
  s32 levelTotal = 0;
  s32 unitCount = BattleParty_PrepareMembers(party);
  s32 candidateOrUnitIndex;
  s32 poolIndex;
  s32 targetLevel;
  s32 result;

  if (unitCount > 0) {
    u16 *unitCursor = party;

    candidateOrUnitIndex = unitCount;
    do {
      u8 level = Unit_GetFar(*unitCursor)->level;

      candidateOrUnitIndex--;
      unitCursor++;
      levelTotal += level;
    } while (candidateOrUnitIndex != 0);
  }

  // The flag byte is -1 once a series has ended, which takes a level off.
  targetLevel = levelTotal / unitCount;
  targetLevel += (s8)GameFlag_GetByteFar(FLAG_MONSTER_BATTLE_WINS);
  if (targetLevel <= 0)
    targetLevel = 1;
  if (targetLevel > 99)
    targetLevel = 99;

  for (candidateOrUnitIndex = 0; candidateOrUnitIndex <= 31;
       candidateOrUnitIndex++)
    pool[candidateOrUnitIndex].score = -1;

  // Forget defeated classes that the monster battles normally exclude;
  // flag 0x174 can still admit them. The definition read is unused.
  for (candidateOrUnitIndex = 0; (u32)candidateOrUnitIndex <= 19;
       candidateOrUnitIndex++) {
    Enemy_GetDefinitionFar(
        gBattleFormation_LevelMatchedEnemies[candidateOrUnitIndex]);
    GameFlag_ClearBitFar(ENEMY_DEFEATED_FLAG(
        gBattleFormation_LevelMatchedEnemies[candidateOrUnitIndex]));
  }

  // Each qualifying formation replaces the pool's lowest entry; empty
  // entries score -1, so the pool fills first.
  for (candidateOrUnitIndex = 0; (u32)candidateOrUnitIndex < FORMATION_COUNT;
       candidateOrUnitIndex++) {
    s32 score = BattleFormation_AverageLevel(candidateOrUnitIndex);

    if (score >= 0 && score <= targetLevel + 3) {
      s32 lowestIndex = -1;
      s32 lowestLevel;

      for (poolIndex = 0, lowestLevel = 999; poolIndex <= 31; poolIndex++) {
        if (pool[poolIndex].score < lowestLevel) {
          lowestLevel = pool[poolIndex].score;
          lowestIndex = poolIndex;
        }
      }

      if (lowestIndex >= 0) {
        pool[lowestIndex].score = score;
        pool[lowestIndex].recordId = candidateOrUnitIndex;
        matchCount++;
      }
    }
  }

  // matchCount includes formations evicted from the pool.
  if (matchCount > 32)
    matchCount = 32;

  if (matchCount != 0) {
    struct FormationCandidate *chosen = &pool[(matchCount * Random16()) >> 16];

    result = chosen->recordId;
    *outMargin = targetLevel - chosen->score;
  } else {
    *outMargin = matchCount;
    result = 1;
  }

  Free(pool);
  return result;
}

/// Raises an enemy by levels, or lowers it for a negative count: each base
/// stat changes by a fixed amount a level, kept between 70 percent of what it
/// was and its cap.
/// A level is worth 9.7 HP, 1.5 PP, 12.3 Attack, 3.3 Defense, 5.1 Agility and
/// 15 power in each element; HP and PP are capped at 9999, the other stats at
/// 999 and power at 200. Changes the unit's base stats and level and works
/// out its stats again (Unit_Calculate). Called by
/// BattleFormation_BuildEnemyList for each enemy of a monster battle.
#ifdef UBFIX
void BattleEnemy_AdjustLevel(s32 owner, s32 levels) {
#else
s32 BattleEnemy_AdjustLevel(s32 owner, s32 levels) {
#endif
  struct BattleBaseStats *base;
  struct Unit *unit;
  s32 value;
  s32 minimumStat;
  s32 element;

  base = AllocEwram(sizeof(struct BattleBaseStats));
  unit = Unit_GetFar(owner);
  Iwram_CopyWords(base, &unit->baseHp, sizeof(struct BattleBaseStats));

  value = unit->baseHp;
  value += levels * 97 / 10;
  minimumStat = base->baseHp * 7 / 10;
  CLAMP(value, minimumStat, 9999);
  unit->baseHp = value;

  value = unit->basePp;
  value += levels * 15 / 10;
  minimumStat = base->basePp * 7 / 10;
  CLAMP(value, minimumStat, 9999);
  unit->basePp = value;

  minimumStat = levels * 123 / 10;
  value = unit->baseAttack;
  value += minimumStat;
  minimumStat = base->baseAttack * 7 / 10;
  CLAMP(value, minimumStat, 999);
  unit->baseAttack = value;

  minimumStat = levels * 33 / 10;
  value = unit->baseDefense;
  value += minimumStat;
  minimumStat = base->baseDefense * 7 / 10;
  CLAMP(value, minimumStat, 999);
  unit->baseDefense = value;

  minimumStat = levels * 51 / 10;
  value = unit->baseAgility;
  value += minimumStat;
  minimumStat = base->baseAgility * 7 / 10;
  CLAMP(value, minimumStat, 999);
  unit->baseAgility = value;

  for (element = 0; element < ELEMENT_COUNT; element++) {
    value = unit->baseElement[element].power + levels * 15;
    minimumStat = base->element[element].power * 7 / 10;
    CLAMP(value, minimumStat, 200);
    unit->baseElement[element].power = value;
  }

  unit->level += levels;
  Unit_CalculateFar(owner);
  Free(base);
}

/// Gives an enemy of the class its name letter, which tells it from others of
/// its kind, and returns the letter's number for BattleUnit_SetFar.
/// The letters are digits from 1 abroad and the alphabet in Japanese. The
/// class's first enemy gets ENEMY_NAME_SUFFIX_COUNT, which BattleUnit_SetFar
/// takes as no letter. Its second gets letter 1 with bit 15 (0x8001), telling
/// the caller to give the first letter 0 (BattleEnemy_LetterFirstOfKind);
/// later ones take the next free letter, wrapping. Returns -1 when five classes
/// are already listed. Called by BattleFormation_BuildEnemyList for each enemy
/// and by BattleAbility_ApplyToTarget for an enemy called into the battle.
/// Changes BattleWork's enemyKinds, the up to five classes listed, and fields
/// that despite their names hold the letters: lastNameLetter each class's last
/// letter given (-1 for none yet), nameLetterMasks its letters in use (bit n
/// for letter n), and enemyKindCount how many classes are listed.
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
      return 0x8001;
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
#ifdef UBFIX
void BattleEnemy_ReleaseNameLetter(s32 actorId) {
#else
s32 BattleEnemy_ReleaseNameLetter(s32 actorId) {
#endif
  struct BattleWork *battle = gBattleWork;
  struct Unit *unit;
  s32 count = battle->enemyKindCount;
  s32 index;
  s32 nameLength;
  s32 suffixIndex;
  s32 classId;

  unit = Unit_GetFar(actorId);
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
/// when BattleEnemy_ClaimNameLetter returns bit 15. Writes Unit.name.
#ifdef UBFIX
void BattleEnemy_LetterFirstOfKind(s32 classId) {
#else
s32 BattleEnemy_LetterFirstOfKind(s32 classId) {
#endif
  struct Unit *unit;
  s32 enemyIndex;
  s32 nameIndex;
  s32 terminatorIndex;
  u8 nameByte;
  u8 firstSuffix;

  enemyIndex = 0;
  firstSuffix = LIST_MARKER_CHAR;
  for (; enemyIndex <= 5; enemyIndex++) {
    unit = Unit_GetFar(enemyIndex + FIRST_ENEMY_UNIT);
    nameByte = unit->side;
    if (nameByte != BATTLE_SIDE_ENEMY)
      continue;
    if (unit->id != classId)
      continue;
    // nameByte still holds BATTLE_SIDE_ENEMY here, used as the index after
    // the letter; nameIndex holds the terminator, 0.
    nameIndex = 0;
    if (unit->name[0] == 0) {
      unit->name[0] = firstSuffix;
      unit->name[nameByte] = nameIndex;
      return;
    }
    // The name's end, then the letter and a new terminator after it.
  scan_slots:
    nameIndex++;
    if (nameIndex > 13)
      return;
    nameByte = unit->name[nameIndex];
    if (nameByte != 0)
      goto scan_slots;
    terminatorIndex = nameIndex + 1;
    unit->name[nameIndex] = firstSuffix;
    unit->name[terminatorIndex] = nameByte;
    return;
  }
}

/// Returns the class id of a random member of the formation, among those
/// that may appear (maximum count above 0); formation 1 stands in for an id
/// out of range.
/// Called by BattleAbility_ApplyToTarget when an enemy calls for help.
s32 BattleFormation_GetRandomMember(s32 recordId) {
  s32 choices[5];
  struct Formation *record;
  s32 *choicesBase;
  s32 *choiceCursor;
  u8 *maximumCount;
  u8 *memberId;
  u32 choiceCount;
  s32 remainingMembers;

  choiceCount = 0;
  if (recordId < 0 || recordId >= FORMATION_COUNT)
    recordId = 1;

  choicesBase = choices;
  record = &gFormations[recordId];
  memberId = record->memberIds;
  maximumCount = record->maximumCounts;
  choiceCursor = choicesBase;
  remainingMembers = 4;
  do {
    if (*maximumCount++ != 0) {
      *choiceCursor++ = *memberId + ENEMY_ID_FIRST;
      choiceCount++;
    }
    memberId++;
    remainingMembers--;
  } while (remainingMembers >= 0);

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
  u16 *enemyKinds;
  struct BattleWork *battle;
  struct Formation *record;
  s32 enemyCount;
  s32 levelAdjustment;
  u16 listBuffer[14];
  s32 extraCounts[5];
  s32 memberCounts[5];
  s32 memberOrder[5];
  s32 freeSpriteSlots;
  s32 changed;
  s32 additionalRoom;
  s32 spriteSlots;
  s32 fittingCount;
  s32 classId;
  s32 memberOrCopyIndex;
  s32 remainingKinds;
  s32 copies;
  u32 memberOrEnemyIndex;
  s32 memberIndex;

  battle = gBattleWork;
  levelAdjustment = 0;
  battle->enemyKindCount = 0;
  enemyKinds = listBuffer;
  if (GameFlag_TestFar(FLAG_MONSTER_BATTLE))
    recordId = BattleFormation_SelectLevelMatchedCandidate(&levelAdjustment);
  if (recordId < 0 || recordId >= FORMATION_COUNT)
    recordId = 1;
  record = &gFormations[recordId];
  for (memberIndex = 0;
       memberIndex <= 4 && record->minimumCounts[memberIndex] == 0;
       memberIndex++)
    ;
  if (memberIndex == 5)
    record = &gFormations[1];

  // The six sprite slots left once every member's minimum is placed; a
  // large enemy takes two.
  freeSpriteSlots = 6;
  enemyCount = 0;
  for (memberOrEnemyIndex = 0; memberOrEnemyIndex <= 4; memberOrEnemyIndex++) {
    if (record->minimumCounts[memberOrEnemyIndex] != 0) {
      s32 requiredSlots =
          2 - (BattleSprite_IsSmall(record->memberIds[memberOrEnemyIndex] +
                                    ENEMY_ID_FIRST) != 0);

      freeSpriteSlots -=
          requiredSlots * record->minimumCounts[memberOrEnemyIndex];
    }
  }

  // Each member's extra enemies: a random number from 0 to as many more as
  // its maximum allows and the free slots would hold on their own.
  for (memberOrEnemyIndex = 0; memberOrEnemyIndex <= 4; memberOrEnemyIndex++) {
    s32 minimum;
    s32 maximum;

    memberCounts[memberOrEnemyIndex] = minimum =
        record->minimumCounts[memberOrEnemyIndex];
    maximum = record->maximumCounts[memberOrEnemyIndex];
    additionalRoom = maximum - minimum;
    if (additionalRoom > 0) {
      s32 memberSlots =
          2 - (BattleSprite_IsSmall(record->memberIds[memberOrEnemyIndex] +
                                    ENEMY_ID_FIRST) != 0);

      fittingCount = freeSpriteSlots / memberSlots;
      if (fittingCount < additionalRoom)
        additionalRoom = fittingCount;
      extraCounts[memberOrEnemyIndex] =
          ((additionalRoom + 1) * Random16()) >> 16;
    } else {
      extraCounts[memberOrEnemyIndex] = 0;
    }
  }

  // The extras then take the shared slots one per member a pass, in member
  // order, until none fits.
  do {
    changed = 0;
    for (memberOrEnemyIndex = 0; memberOrEnemyIndex <= 4;
         memberOrEnemyIndex++) {
      classId = record->memberIds[memberOrEnemyIndex] + ENEMY_ID_FIRST;
      if (extraCounts[memberOrEnemyIndex] != 0) {
        spriteSlots = 2 - (BattleSprite_IsSmall(classId) != 0);
        if (spriteSlots > freeSpriteSlots) {
          extraCounts[memberOrEnemyIndex] = 0;
        } else {
          memberCounts[memberOrEnemyIndex]++;
          extraCounts[memberOrEnemyIndex]--;
          freeSpriteSlots -= spriteSlots;
          changed = 1;
        }
      }
    }
  } while (changed);

  // The list of class ids, in the order the battle type gives, ended by 0.
  battle->layout = record->layout;
  switch (battle->layout) {
  case FORMATION_LAYOUT_SHUFFLED:
    // The members' order shuffled by ten random swaps.
    for (memberOrEnemyIndex = 0; memberOrEnemyIndex <= 4; memberOrEnemyIndex++)
      memberOrder[memberOrEnemyIndex] = memberOrEnemyIndex;
    for (memberOrEnemyIndex = 0; memberOrEnemyIndex <= 9;
         memberOrEnemyIndex++) {
      s32 first = (Random16() * 5) >> 16;
      s32 second = (Random16() * 5) >> 16;
      s32 swap = memberOrder[first];

      memberOrder[first] = memberOrder[second];
      memberOrder[second] = swap;
    }
    for (memberOrEnemyIndex = 0; memberOrEnemyIndex <= 4;
         memberOrEnemyIndex++) {
      s32 copyIndex;
      s32 slot = memberOrder[memberOrEnemyIndex];

      for (copyIndex = 0; copyIndex < memberCounts[slot]; copyIndex++)
        enemyKinds[enemyCount++] = record->memberIds[slot] + ENEMY_ID_FIRST;
    }
    break;
  case FORMATION_LAYOUT_INTERLEAVED:
    // Each enemy drawn from a random member with any left.
    for (;;) {
      remainingKinds = 0;
      for (memberOrEnemyIndex = 0; memberOrEnemyIndex <= 4;
           memberOrEnemyIndex++) {
        if (memberCounts[memberOrEnemyIndex] != 0)
          memberOrder[remainingKinds++] = memberOrEnemyIndex;
      }
      if (remainingKinds == 0)
        break;
      memberOrCopyIndex = memberOrder[(Random16() * remainingKinds) >> 16];
      enemyKinds[enemyCount++] =
          record->memberIds[memberOrCopyIndex] + ENEMY_ID_FIRST;
      memberCounts[memberOrCopyIndex]--;
    }
    break;
  default:
    for (memberOrEnemyIndex = 0; memberOrEnemyIndex <= 4;
         memberOrEnemyIndex++) {
      copies = memberCounts[memberOrEnemyIndex];
      for (memberOrCopyIndex = 0; memberOrCopyIndex < copies;
           memberOrCopyIndex++)
        enemyKinds[enemyCount++] =
            record->memberIds[memberOrEnemyIndex] + ENEMY_ID_FIRST;
    }
    break;
  }
  // No class has fallen yet (a slot past the last), and the enemy units
  // start cleared.
  enemyKinds[enemyCount] = 0;
  battle->firstDefeated = BATTLE_ENEMY_CAPACITY;
  battle->defeatState = DEFEAT_ONE;
  for (memberOrEnemyIndex = FIRST_ENEMY_UNIT;
       memberOrEnemyIndex < FIRST_ENEMY_UNIT + ENEMY_BATTLE_SIZE;
       memberOrEnemyIndex++)
    Iwram_ClearWords(Unit_GetFar(memberOrEnemyIndex), BATTLE_UNIT_SIZE);

  memberOrEnemyIndex = 0;
  if (enemyKinds[0] == 0)
    goto assigned;
assign_enemy:
  // Each enemy unit, with its name letter (BattleEnemy_ClaimNameLetter) and,
  // in a monster battle, the level margin.
  {
    s32 suffixCode =
        BattleEnemy_ClaimNameLetter(enemyKinds[memberOrEnemyIndex], 1);

    if (suffixCode & 0x8000)
      BattleEnemy_LetterFirstOfKind(enemyKinds[memberOrEnemyIndex]);
    BattleUnit_SetFar(memberOrEnemyIndex + FIRST_ENEMY_UNIT,
                      enemyKinds[memberOrEnemyIndex], suffixCode & 0x7fff);
    Unit_GetFar(memberOrEnemyIndex + FIRST_ENEMY_UNIT);
    if (levelAdjustment != 0)
      BattleEnemy_AdjustLevel(memberOrEnemyIndex + FIRST_ENEMY_UNIT,
                              levelAdjustment);
  }
  memberOrEnemyIndex++;
  if (memberOrEnemyIndex > 5)
    goto assigned;
  if (enemyKinds[memberOrEnemyIndex] != 0)
    goto assign_enemy;
assigned:
  return memberOrEnemyIndex;
}
