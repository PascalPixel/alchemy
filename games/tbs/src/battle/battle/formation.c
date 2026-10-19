// Part of battle.c, the battle object, compiled only through it.
// Enemy formations: the enemies chosen, levelled and lettered.

#include "../battle_internal.h"

// The rows of gBattleFormation_Records.
#define FORMATION_COUNT 380

// How many name letters tell the enemies of one class apart: 26 in Japanese,
// 9 in the localizations.
#if (LANGUAGE != LANGUAGE_JAPANESE)
#define ENEMY_NAME_SUFFIX_COUNT 9
#else
#define ENEMY_NAME_SUFFIX_COUNT 26
#endif

// Returns the formation's average enemy level, or -1 for an empty minimum list,
// -2 for an ineligible unseen enemy, or -3 when no member has a maximum count.
s32 BattleFormation_AverageLevel(s32 recordId) {
  struct BattleFormationRecord *record;
  struct EnemyDefinition *enemy;
  u32 memberIndex;
  s32 enemyId;
  s32 levelTotal;
  s32 enemyCount;
  /* FAKEMATCH(dead): an unused array for the frame's 28 bytes; without it, 2
   * of 94 instructions differ in tbs-en. */
  s32 levels[7];

  enemyCount = 0;
  levelTotal = 0;
  record = &gBattleFormation_Records[recordId];
  for (memberIndex = 0; memberIndex < 5; memberIndex++) {
    if (record->minimumCounts[memberIndex] != 0)
      break;
  }
  if (memberIndex == 5)
    return -1;
  for (memberIndex = 0; memberIndex < 5; memberIndex++) {
    if (record->maximumCounts[memberIndex] == 0)
      continue;
    enemyId = record->memberIds[memberIndex];
    enemy = Enemy_GetDefinitionFar(enemyId + 8);
    if (enemy == 0)
      continue;
    if (enemy->level > 3 && !GameFlag_TestFar(0x174) &&
        !GameFlag_TestFar(enemyId + 0x608))
      return -2;
    levelTotal += enemy->level;
    enemyCount++;
  }
  if (enemyCount == 0)
    return -3;
  return levelTotal / enemyCount;
}

struct FormationCandidate {
  s16 recordId;
  s16 score;
};

struct BattleBaseStats {
  s16 baseHp;
  s16 basePp;
  s16 hpGauge;
  s16 ppGauge;
  u16 baseAttack;
  u16 baseDefense;
  u16 baseAgility;
  u8 baseLuck;
  u8 actionCount;
  u8 hpRegen;
  u8 ppRegen;
  u8 unk12[2];
  struct BattleElementStats element[4];
};

// Keeps up to thirty-two eligible formations near the party's adjusted average
// level, chooses one randomly and returns its level adjustment through
// outMargin.
s32 BattleFormation_SelectLevelMatchedCandidate(s32 *outMargin) {
  s32 matchCount = 0;
  struct FormationCandidate *pool = Runtime_BumpAllocateAlternatePool(128);
  u16 party[8];
  s32 levelTotal = 0;
  s32 unitCount = BattleParty_PrepareActiveOwners(party);
  s32 candidateOrUnitIndex;
  s32 poolIndex;
  s32 targetLevel;
  s32 result;

  if (unitCount > 0) {
    u16 *unitCursor = party;

    candidateOrUnitIndex = unitCount;
    do {
      u8 level = UnitGetFar(*unitCursor)->level;

      candidateOrUnitIndex--;
      unitCursor++;
      levelTotal += level;
    } while (candidateOrUnitIndex != 0);
  }

  targetLevel = levelTotal / unitCount;
  targetLevel += (s8)GameFlag_GetByteFar(FLAG_MONSTER_BATTLE_WINS);
  if (targetLevel <= 0)
    targetLevel = 1;
  if (targetLevel > 99)
    targetLevel = 99;

  for (candidateOrUnitIndex = 0; candidateOrUnitIndex <= 31;
       candidateOrUnitIndex++)
    pool[candidateOrUnitIndex].score = -1;

  for (candidateOrUnitIndex = 0; (u32)candidateOrUnitIndex <= 19;
       candidateOrUnitIndex++) {
    Enemy_GetDefinitionFar(
        gBattleFormation_LevelMatchedEnemies[candidateOrUnitIndex]);
    GameFlag_ClearBitFar(
        gBattleFormation_LevelMatchedEnemies[candidateOrUnitIndex] + 0x600);
  }

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

  MemFree(pool);
  return result;
}

// Adjusts enemy base stats by the level margin, retaining 70-percent floors and
// per-stat caps, then updates the level and derived stats.
s32 Owner_ApplyLevelGains(s32 owner, s32 levels) {
  struct BattleBaseStats *base;
  struct BattleUnit *unit;
  s32 value;
  s32 minimumStat;
  s32 element;

  base = Runtime_BumpAllocateAlternatePool(sizeof(struct BattleBaseStats));
  unit = UnitGetFar(owner);
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

  for (element = 0; element < 4; element++) {
    value = unit->baseElement[element].power + levels * 15;
    minimumStat = base->element[element].power * 7 / 10;
    CLAMP(value, minimumStat, 200);
    unit->baseElement[element].power = value;
  }

  unit->level += levels;
  UnitCalcFar(owner);
  MemFree(base);
}

// Claims the next displayed name suffix for an enemy class. Bit 15 requests
// a suffix for the first existing duplicate; a new class returns the
// suffix-count sentinel.
s32 EnemyCountDecrement(s32 classId, s32 unused) {
  struct BattleWork *battle;
  s32 classCount;
  s32 classIndex;
  s32 attempt;
  s32 suffix;

  battle = gBattleWork;
  classCount = battle->summonCount;
  for (classIndex = 0; classIndex < classCount; classIndex++) {
    if (battle->enemyClasses[classIndex] == classId)
      break;
  }
  if (classIndex != classCount) {
    attempt = 0;
    if (battle->summonChannels[classIndex] < 0) {
      battle->summonChannels[classIndex] = 1;
      battle->summonMasks[classIndex] = 3;
      return 0x8001;
    }
    for (; attempt <= 31; attempt++) {
      suffix =
          (battle->summonChannels[classIndex] + 1) % ENEMY_NAME_SUFFIX_COUNT;
      battle->summonChannels[classIndex] = suffix;
      if ((battle->summonMasks[classIndex] & (1 << (s8)suffix)) == 0)
        break;
    }
    battle->summonMasks[classIndex] |= 1 << battle->summonChannels[classIndex];
    return battle->summonChannels[classIndex];
  }
  if (classCount <= 4) {
    battle->summonChannels[classCount] = -1;
    battle->enemyClasses[classCount] = classId;
    battle->summonMasks[classCount] = 0;
    battle->summonCount = classCount + 1;
    return ENEMY_NAME_SUFFIX_COUNT;
  }
  return -1;
}

s32 EnemyCountRelease(s32 actorId) {
  struct BattleWork *battle = gBattleWork;
  struct BattleUnit *unit;
  s32 count = battle->summonCount;
  s32 index;
  s32 nameLength;
  s32 suffixIndex;
  s32 classId;

  unit = UnitGetFar(actorId);
  if (unit->classIndex != 0)
    return;

  classId = unit->classId;
  for (index = 0; index < count; index++) {
    if (battle->enemyClasses[index] == classId)
      break;
  }
  if (index == count || battle->summonMasks[index] == 0)
    return;

  for (nameLength = 0; nameLength <= 13; nameLength++) {
    if (unit->name[nameLength] == 0)
      break;
  }

  suffixIndex = 32;
  if (nameLength > 0)
    suffixIndex = unit->name[nameLength - 1] - LIST_MARKER_CHAR;
  battle->summonMasks[index] &= ~(1 << suffixIndex);
}

// Appends the first suffix to the first present enemy of the duplicated class.
s32 EnemyCountClear(s32 classId) {
  struct BattleUnit *unit;
  s32 enemyIndex;
  s32 nameIndex;
  s32 terminatorIndex;
  u8 nameByte;
  u8 firstSuffix;

  enemyIndex = 0;
  firstSuffix = LIST_MARKER_CHAR;
  for (; enemyIndex <= 5; enemyIndex++) {
    unit = UnitGetFar(enemyIndex + FIRST_ENEMY_UNIT);
    nameByte = unit->side;
    if (nameByte != BATTLE_UNIT_ENEMY)
      continue;
    if (unit->classId != classId)
      continue;
    nameIndex = 0;
    if (unit->name[0] == 0) {
      unit->name[0] = firstSuffix;
      unit->name[nameByte] = nameIndex;
      return;
    }
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

// Chooses an enemy class from the formation's members whose maximum count is
// nonzero.
s32 BattleGroupRandom(s32 recordId) {
  s32 choices[5];
  struct BattleFormationRecord *record;
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
  record = &gBattleFormation_Records[recordId];
  memberId = record->memberIds;
  maximumCount = record->maximumCounts;
  choiceCursor = choicesBase;
  remainingMembers = 4;
  do {
    if (*maximumCount++ != 0) {
      *choiceCursor++ = *memberId + 8;
      choiceCount++;
    }
    memberId++;
    remainingMembers--;
  } while (remainingMembers >= 0);

  return choices[(Random16() * choiceCount) >> 16];
}

// Chooses extra enemies within the six sprite slots, orders them according to
// the formation type and initializes each enemy with its suffix and level
// adjustment.
s32 BattleFormation_BuildEnemyList(s32 recordId) {
  u16 *enemyClasses;
  struct BattleWork *battle;
  struct BattleFormationRecord *record;
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
  battle->summonCount = 0;
  enemyClasses = listBuffer;
  if (GameFlag_TestFar(FLAG_MONSTER_BATTLE))
    recordId = BattleFormation_SelectLevelMatchedCandidate(&levelAdjustment);
  if (recordId < 0 || recordId >= FORMATION_COUNT)
    recordId = 1;
  record = &gBattleFormation_Records[recordId];
  for (memberIndex = 0;
       memberIndex <= 4 && record->minimumCounts[memberIndex] == 0;
       memberIndex++)
    ;
  if (memberIndex == 5)
    record = &gBattleFormation_Records[1];

  freeSpriteSlots = 6;
  enemyCount = 0;
  for (memberOrEnemyIndex = 0; memberOrEnemyIndex <= 4; memberOrEnemyIndex++) {
    if (record->minimumCounts[memberOrEnemyIndex] != 0) {
      s32 requiredSlots =
          2 - (Summon_IsEntryFlagged(record->memberIds[memberOrEnemyIndex] +
                                     8) != 0);

      freeSpriteSlots -=
          requiredSlots * record->minimumCounts[memberOrEnemyIndex];
    }
  }

  for (memberOrEnemyIndex = 0; memberOrEnemyIndex <= 4; memberOrEnemyIndex++) {
    s32 minimum;
    s32 maximum;

    memberCounts[memberOrEnemyIndex] = minimum =
        record->minimumCounts[memberOrEnemyIndex];
    maximum = record->maximumCounts[memberOrEnemyIndex];
    additionalRoom = maximum - minimum;
    if (additionalRoom > 0) {
      s32 memberSlots =
          2 - (Summon_IsEntryFlagged(record->memberIds[memberOrEnemyIndex] +
                                     8) != 0);

      fittingCount = freeSpriteSlots / memberSlots;
      if (fittingCount < additionalRoom)
        additionalRoom = fittingCount;
      extraCounts[memberOrEnemyIndex] =
          ((additionalRoom + 1) * Random16()) >> 16;
    } else {
      extraCounts[memberOrEnemyIndex] = 0;
    }
  }

  do {
    changed = 0;
    for (memberOrEnemyIndex = 0; memberOrEnemyIndex <= 4;
         memberOrEnemyIndex++) {
      classId = record->memberIds[memberOrEnemyIndex] + 8;
      if (extraCounts[memberOrEnemyIndex] != 0) {
        spriteSlots = 2 - (Summon_IsEntryFlagged(classId) != 0);
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

  battle->battleType = record->battleType;
  switch (battle->battleType) {
  case 0:
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
        enemyClasses[enemyCount++] = record->memberIds[slot] + 8;
    }
    break;
  case 1:
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
      enemyClasses[enemyCount++] = record->memberIds[memberOrCopyIndex] + 8;
      memberCounts[memberOrCopyIndex]--;
    }
    break;
  default:
    for (memberOrEnemyIndex = 0; memberOrEnemyIndex <= 4;
         memberOrEnemyIndex++) {
      copies = memberCounts[memberOrEnemyIndex];
      for (memberOrCopyIndex = 0; memberOrCopyIndex < copies;
           memberOrCopyIndex++)
        enemyClasses[enemyCount++] = record->memberIds[memberOrEnemyIndex] + 8;
    }
    break;
  }
  enemyClasses[enemyCount] = 0;
  battle->firstDefeated = 6;
  battle->defeatState = 0;
  for (memberOrEnemyIndex = FIRST_ENEMY_UNIT;
       memberOrEnemyIndex < FIRST_ENEMY_UNIT + ENEMY_BATTLE_SIZE;
       memberOrEnemyIndex++)
    Iwram_ClearWords(UnitGetFar(memberOrEnemyIndex), BATTLE_UNIT_SIZE);

  memberOrEnemyIndex = 0;
  if (enemyClasses[0] == 0)
    goto assigned;
assign_enemy:
  // Repeated enemy classes get distinct suffixes within this encounter.
  {
    s32 suffixCode = EnemyCountDecrement(enemyClasses[memberOrEnemyIndex], 1);

    if (suffixCode & 0x8000)
      EnemyCountClear(enemyClasses[memberOrEnemyIndex]);
    BattleUnitSetFar(memberOrEnemyIndex + FIRST_ENEMY_UNIT,
                     enemyClasses[memberOrEnemyIndex], suffixCode & 0x7fff);
    UnitGetFar(memberOrEnemyIndex + FIRST_ENEMY_UNIT);
    if (levelAdjustment != 0)
      Owner_ApplyLevelGains(memberOrEnemyIndex + FIRST_ENEMY_UNIT,
                            levelAdjustment);
  }
  memberOrEnemyIndex++;
  if (memberOrEnemyIndex > 5)
    goto assigned;
  if (enemyClasses[memberOrEnemyIndex] != 0)
    goto assign_enemy;
assigned:
  return memberOrEnemyIndex;
}
