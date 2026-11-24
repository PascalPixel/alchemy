/// @file
/// Enemy formations: the enemies chosen, levelled and told apart by name
/// suffixes, and each enemy class's sprite, palette, size and attack effect.
// Part of battle.c.

#include "battle_internal.h"
#include "field/flag_call.h"
#include "battle/presentation.h"

/// The most formations a monster battle chooses among.
#define FORMATION_POOL_SIZE 32
/// How far above the party's level a monster battle's formation may be.
#define FORMATION_LEVEL_MARGIN 3
/// Above any formation's average level: where the search for the pool's
/// lowest entry starts.
#define FORMATION_SCORE_ABOVE_ALL 999

/// Chooses a monster battle's formation for the party's level plus its winning
/// streak, at random among those at most FORMATION_LEVEL_MARGIN above it, and
/// returns it with the levels its enemies must gain in outMargin.
s32 BattleFormation_SelectLevelMatchedCandidate(s32 *outMargin) {
  s32 matchCount = 0;
  struct FormationCandidate *pool =
      Heap_AllocEwram(FORMATION_POOL_SIZE * sizeof *pool);
  u16 party[8];
  s32 targetLevel = 0;
  s32 unitCount = BattleParty_PrepareMembers(party);
  s32 i;
  s32 poolIndex;
  s32 result;

  if (unitCount > 0) {
    u16 *unitCursor = party;

    i = unitCount;
    do {
      u8 level = Unit_Get(*unitCursor)->level;

      i--;
      unitCursor++;
      targetLevel += level;
    } while (i != 0);
  }

  // The flag byte is -1 once a series has ended, which takes a level off.
  targetLevel = targetLevel / unitCount;
  targetLevel += (s8)GameFlag_GetByte(FLAG_MONSTER_BATTLE_WINS);
  if (targetLevel <= 0)
    targetLevel = 1;
  if (targetLevel > UNIT_LEVEL_MAX)
    targetLevel = UNIT_LEVEL_MAX;

  for (i = 0; i < FORMATION_POOL_SIZE; i++)
    pool[i].score = -1;

  // A boss beaten in another of its forms marks one form beaten: the
  // Serpent's four-beam one, the Knight's second, Moapa's third, and the
  // Doom Dragon's first only once its last has fallen.
  if (GameFlag_Get(ENEMY_DEFEATED_FLAG(ENEMY_SERPENT)))
    GameFlag_Set(ENEMY_DEFEATED_FLAG(ENEMY_SERPENT_FOUR_BEAMS));
  if (GameFlag_Get(ENEMY_DEFEATED_FLAG(ENEMY_SERPENT_ONE_BEAM)))
    GameFlag_Set(ENEMY_DEFEATED_FLAG(ENEMY_SERPENT_FOUR_BEAMS));
  if (GameFlag_Get(ENEMY_DEFEATED_FLAG(ENEMY_SERPENT_TWO_BEAMS)))
    GameFlag_Set(ENEMY_DEFEATED_FLAG(ENEMY_SERPENT_FOUR_BEAMS));
  if (GameFlag_Get(ENEMY_DEFEATED_FLAG(ENEMY_SERPENT_THREE_BEAMS)))
    GameFlag_Set(ENEMY_DEFEATED_FLAG(ENEMY_SERPENT_FOUR_BEAMS));
  if (GameFlag_Get(ENEMY_DEFEATED_FLAG(ENEMY_KNIGHT)))
    GameFlag_Set(ENEMY_DEFEATED_FLAG(ENEMY_KNIGHT_B));
  if (GameFlag_Get(ENEMY_DEFEATED_FLAG(ENEMY_MOAPA)))
    GameFlag_Set(ENEMY_DEFEATED_FLAG(ENEMY_MOAPA_C));
  if (GameFlag_Get(ENEMY_DEFEATED_FLAG(ENEMY_MOAPA_B)))
    GameFlag_Set(ENEMY_DEFEATED_FLAG(ENEMY_MOAPA_C));
  GameFlag_Clear(ENEMY_DEFEATED_FLAG(ENEMY_DOOM_DRAGON_A));
  if (GameFlag_Get(ENEMY_DEFEATED_FLAG(ENEMY_DOOM_DRAGON_H)))
    GameFlag_Set(ENEMY_DEFEATED_FLAG(ENEMY_DOOM_DRAGON_A));

  // Forget the defeated classes the monster battles exclude.
  for (i = 0; i < ARRAY_COUNT(gBattleFormation_LevelMatchedEnemies); i++)
    GameFlag_Clear(
        ENEMY_DEFEATED_FLAG(gBattleFormation_LevelMatchedEnemies[i]));
  // BUG: the three enemies formations 241 to 248 are made of are set by
  // their enemy ids, not their defeated flags, so they stay unavailable
  // unless beaten, and FLAG_DJINNI(DJINNI_SPRITZ), FLAG_DJINNI(DJINNI_ETHER)
  // and flag 0xe6 are set instead.
#ifdef BUGFIX
  GameFlag_Set(ENEMY_DEFEATED_FLAG(ENEMY_PUNCH_ANT));
  GameFlag_Set(ENEMY_DEFEATED_FLAG(ENEMY_GIANT_BAT));
  GameFlag_Set(ENEMY_DEFEATED_FLAG(ENEMY_WILD_WOLF));
#else
  GameFlag_Set(ENEMY_PUNCH_ANT);
  GameFlag_Set(ENEMY_GIANT_BAT);
  GameFlag_Set(ENEMY_WILD_WOLF);
#endif

  // Each qualifying formation replaces the pool's lowest entry; empty
  // entries score -1, so the pool fills first.
  for (i = 0; (u32)i < FORMATION_COUNT; i++) {
    s32 score = BattleFormation_AverageLevel(i);

    if (score >= 0 && score <= targetLevel + FORMATION_LEVEL_MARGIN) {
      s32 lowestIndex = -1;
      s32 lowestLevel;

      for (lowestLevel = FORMATION_SCORE_ABOVE_ALL, poolIndex = 0;
           poolIndex < FORMATION_POOL_SIZE; poolIndex++) {
        if (pool[poolIndex].score < lowestLevel) {
          lowestLevel = pool[poolIndex].score;
          lowestIndex = poolIndex;
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
    *outMargin = matchCount;
    result = 1;
  }

  Heap_Release(pool);
  return result;
}

/// The element power an enemy gains a level (BattleEnemy_AdjustLevel); its
/// other stats gain tenths of a point.
#define ELEMENT_POWER_GAIN 15

/// Raises an enemy by levels, or lowers it for a negative count, each stat by a
/// fixed amount a level and no lower than 70 percent of what it was.
UNREAD_RESULT BattleEnemy_AdjustLevel(s32 unitId, s32 levels) {
  struct BattleBaseStats *base = Heap_AllocEwram(sizeof *base);
  struct Unit *unit = Unit_Get(unitId);
  s32 value;
  s32 element;

  Iwram_CopyWordsEntry(base, &unit->baseHp, sizeof *base);

  value = unit->baseHp + levels * 97 / 10;
  CLAMP(value, base->baseHp * 7 / 10, UNIT_ENEMY_HP_MAX);
  unit->baseHp = value;

  value = unit->basePp + levels * 15 / 10;
  CLAMP(value, base->basePp * 7 / 10, UNIT_ENEMY_PP_MAX);
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
    value = unit->baseElement[element].power + levels * ELEMENT_POWER_GAIN;
    CLAMP(value, base->element[element].power * 7 / 10, UNIT_ELEMENT_MAX);
    unit->baseElement[element].power = value;
  }

  unit->level += levels;
  Unit_CalculateFar(unitId);
  Heap_Release(base);
}

/// Raises a unit's base HP by half and base Attack and Defense by a quarter, at
/// least 70 percent of the old values and at most UNIT_ENEMY_HP_MAX and
/// UNIT_STAT_MAX: an enemy on the HARD level (FLAG_HARD_LEVEL).
void BattleUnit_Strengthen(s32 unitId) {
  struct BattleBaseStats *saved = Heap_AllocEwram(sizeof *saved);
  struct Unit *unit = Unit_Get(unitId);
  s32 value;

  Iwram_CopyWordsEntry(saved, &unit->baseHp, sizeof *saved);
  value = unit->baseHp * 3 / 2;
  CLAMP(value, saved->baseHp * 7 / 10, UNIT_ENEMY_HP_MAX);
  unit->baseHp = value;
  value = unit->baseAttack * 5 / 4;
  CLAMP(value, saved->baseAttack * 7 / 10, UNIT_STAT_MAX);
  unit->baseAttack = value;
  value = unit->baseDefense * 5 / 4;
  CLAMP(value, saved->baseDefense * 7 / 10, UNIT_STAT_MAX);
  unit->baseDefense = value;
  Unit_CalculateFar(unitId);
  Heap_Release(saved);
}

/// The suffixes a class's mask holds, one bit each.
#define NAME_SUFFIX_MASK_BITS 32

/// Gives an enemy of the class its name suffix, which tells it from others of
/// its kind: digits abroad and the alphabet in Japanese. Returns -1 when five
/// classes are already listed.
s32 BattleEnemy_ClaimNameLetter(s32 classId, s32 unused) {
  struct BattleState *battle;
  s32 classCount;
  s32 classIndex;
  s32 attempt;

  battle = Ram_HeapSlots->battleState;
  classCount = battle->enemyKindCount;
  for (classIndex = 0; classIndex < classCount; classIndex++) {
    if (battle->enemyKinds[classIndex] == classId)
      break;
  }
  if (classIndex != classCount) {
    attempt = 0;
    // The second of its class: suffixes 0 and 1 both taken.
    if (battle->lastNameLetter[classIndex] < 0) {
      battle->lastNameLetter[classIndex] = 1;
      battle->nameLetterMasks[classIndex] = 3;
      return ENEMY_LETTER_FIRST_TOO | 1;
    }
    // The next suffix after the last given that is free; after 32 tries the
    // last one tried is reused.
    for (; attempt < NAME_SUFFIX_MASK_BITS; attempt++) {
      battle->lastNameLetter[classIndex] =
          (battle->lastNameLetter[classIndex] + 1) % NAME_SUFFIX_COUNT;
      if ((battle->nameLetterMasks[classIndex] &
           (1 << battle->lastNameLetter[classIndex])) == 0)
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
    return NAME_SUFFIX_COUNT;
  }
  return -1;
}

/// Frees a departing enemy's name suffix for the next of its class.
UNREAD_RESULT BattleEnemy_ReleaseNameLetter(s32 unitId) {
  struct BattleState *battle = Ram_HeapSlots->battleState;
  struct Unit *unit;
  s32 count = battle->enemyKindCount;
  s32 index;
  s32 nameLength;
  s32 suffix;
  s32 classId;

  unit = Unit_Get(unitId);
  if (unit->classId != CLASS_NONE)
    return;

  classId = unit->enemyId;
  for (index = 0; index < count; index++) {
    if (battle->enemyKinds[index] == classId)
      break;
  }
  if (index == count || battle->nameLetterMasks[index] == 0)
    return;

  for (nameLength = 0; nameLength < UNIT_NAME_SIZE - 1; nameLength++) {
    if (unit->name[nameLength] == 0)
      break;
  }

  // UB: an empty name shifts by NAME_SUFFIX_MASK_BITS, which the ARM turns
  // into 0, so no suffix is freed. A name with no suffix instead uses its
  // last character as one.
  suffix = NAME_SUFFIX_MASK_BITS;
#ifdef UBFIX
  if (nameLength == 0)
    return;
#endif
  if (nameLength > 0)
    suffix = unit->name[nameLength - 1] - LIST_MARKER_CHAR;
  battle->nameLetterMasks[index] &= ~(1 << suffix);
}

/// Gives the first enemy of the class on the field the first suffix (0),
/// appended to its name, once a second of its class arrives.
s32 BattleEnemy_LetterFirstOfKind(s32 enemyId) {
  struct Unit *unit;
  s32 enemyIndex;
  s32 nameIndex;
  s32 terminatorIndex;
  u8 nameByte;

  for (enemyIndex = 0; enemyIndex < ENEMY_BATTLE_SIZE; enemyIndex++) {
    unit = Unit_Get(enemyIndex + FIRST_ENEMY_UNIT);
    nameByte = unit->side;
    if (nameByte != BATTLE_SIDE_ENEMY)
      continue;
    if (unit->enemyId != enemyId)
      continue;
    // nameByte still holds BATTLE_SIDE_ENEMY here, used as the index after
    // the suffix; nameIndex holds the terminator, 0.
    nameIndex = 0;
    if (unit->name[0] == 0) {
      unit->name[0] = LIST_MARKER_CHAR;
      unit->name[nameByte] = nameIndex;
      return;
    }
    // The name's end, then the suffix and a new terminator after it.
  scan_slots:
    nameIndex++;
    if (nameIndex > UNIT_NAME_SIZE - 2)
      return;
    nameByte = unit->name[nameIndex];
    if (nameByte != 0)
      goto scan_slots;
    terminatorIndex = nameIndex + 1;
    unit->name[nameIndex] = LIST_MARKER_CHAR;
    unit->name[terminatorIndex] = nameByte;
    return;
  }
}

/// Returns the class of a random member of the formation that may appear.
s32 BattleFormation_GetRandomMember(s32 recordId) {
  s32 *choicesBase;
  s32 choices[FORMATION_MEMBERS];
  struct Formation *record;
  s32 *choiceCursor;
  u8 *maximumCount;
  u16 *memberId;
  s32 remaining;
  u32 count;

  count = 0;
  if ((u32)recordId >= FORMATION_COUNT)
    recordId = 1;

  record = &gFormations[recordId];
  choicesBase = choices;
  memberId = record->memberIds;
  maximumCount = record->maximumCounts;
  choiceCursor = choicesBase;
  remaining = FORMATION_MEMBERS - 1;
  do {
    if (*maximumCount++ != 0) {
      s32 id = *memberId;

      count++;
      *choiceCursor++ = id + ENEMY_ID_FIRST;
    }
    remaining--;
    memberId++;
  } while (remaining >= 0);

  return choices[(Random16() * count) >> 16];
}

/// The enemy sprite slots an enemy of the class takes: one for a small enemy,
/// two for any other.
#define ENEMY_SLOTS_TAKEN(classId) (2 - (BattleSprite_IsSmall(classId) != 0))

/// Builds the battle's enemies from a formation and returns how many there are:
/// each member its fewest times and at random up to its most while the sprite
/// slots hold them, laid out as the formation says.
s32 BattleFormation_BuildEnemyList(s32 recordId) {
  u16 *enemyClasses;
  struct BattleState *battle;
  struct Formation *record;
  s32 freeSpriteSlots;
  s32 enemyCount;
  s32 levelAdjustment;
  u16 listBuffer[UNIT_MAX];
  s32 extraCounts[FORMATION_MEMBERS];
  s32 memberCounts[FORMATION_MEMBERS];
  s32 memberOrder[FORMATION_MEMBERS];
  s32 changed;
  s32 additionalRoom;
  s32 spriteSlots;
  s32 fittingCount;
  s32 classId;
  s32 remainingKinds;
  s32 copies;
  u32 i;
  s32 memberIndex;

  battle = (struct BattleState *)Ram_HeapSlots->battleState;
  levelAdjustment = 0;
  battle->enemyKindCount = 0;
  enemyClasses = listBuffer;
  if (GameFlag_Get(FLAG_MONSTER_BATTLE))
    recordId = BattleFormation_SelectLevelMatchedCandidate(&levelAdjustment);
  if ((u32)recordId >= FORMATION_COUNT)
    recordId = 1;
  record = &gFormations[recordId];
  for (memberIndex = 0; memberIndex < FORMATION_MEMBERS &&
                        record->minimumCounts[memberIndex] == 0;
       memberIndex++)
    ;
  if (memberIndex == FORMATION_MEMBERS)
    record = &gFormations[1];

  freeSpriteSlots = ENEMY_SPRITE_SLOTS;
  enemyCount = 0;
  for (i = 0; i < FORMATION_MEMBERS; i++) {
    if (record->minimumCounts[i] != 0) {
      s32 requiredSlots =
          ENEMY_SLOTS_TAKEN(record->memberIds[i] + ENEMY_ID_FIRST);

      freeSpriteSlots -= requiredSlots * record->minimumCounts[i];
    }
  }

  for (i = 0; i < FORMATION_MEMBERS; i++) {
    s32 minimum;
    s32 maximum;

    memberCounts[i] = minimum = record->minimumCounts[i];
    maximum = record->maximumCounts[i];
    additionalRoom = maximum - minimum;
    if (additionalRoom > 0) {
      s32 memberSlots =
          ENEMY_SLOTS_TAKEN(record->memberIds[i] + ENEMY_ID_FIRST);

      fittingCount = freeSpriteSlots / memberSlots;
      if (fittingCount < additionalRoom)
        additionalRoom = fittingCount;
      extraCounts[i] = ((additionalRoom + 1) * Random16()) >> 16;
    } else {
      extraCounts[i] = 0;
    }
  }

  do {
    changed = 0;
    for (i = 0; i < FORMATION_MEMBERS; i++) {
      classId = record->memberIds[i] + ENEMY_ID_FIRST;
      if (extraCounts[i] != 0) {
        spriteSlots = ENEMY_SLOTS_TAKEN(classId);
        if (spriteSlots > freeSpriteSlots) {
          extraCounts[i] = 0;
        } else {
          memberCounts[i]++;
          extraCounts[i]--;
          freeSpriteSlots -= spriteSlots;
          changed = 1;
        }
      }
    }
  } while (changed);

  battle->layout = record->layout;
  switch (battle->layout) {
  case FORMATION_LAYOUT_SHUFFLED:
    for (i = 0; i < FORMATION_MEMBERS; i++)
      memberOrder[i] = i;
    for (i = 0; i <= 9; i++) {
      s32 first = Random_Below(FORMATION_MEMBERS);
      s32 second = Random_Below(FORMATION_MEMBERS);
      s32 swap = memberOrder[first];

      memberOrder[first] = memberOrder[second];
      memberOrder[second] = swap;
    }
    for (i = 0; i < FORMATION_MEMBERS; i++) {
      s32 copyIndex;
      s32 slot = memberOrder[i];

      for (copyIndex = 0; copyIndex < memberCounts[slot]; copyIndex++)
        enemyClasses[enemyCount++] = record->memberIds[slot] + ENEMY_ID_FIRST;
    }
    break;
  case FORMATION_LAYOUT_INTERLEAVED:
    for (;;) {
      remainingKinds = 0;
      for (i = 0; i < FORMATION_MEMBERS; i++) {
        if (memberCounts[i] != 0)
          memberOrder[remainingKinds++] = i;
      }
      if (remainingKinds == 0)
        break;
      spriteSlots = memberOrder[(Random16() * remainingKinds) >> 16];
      enemyClasses[enemyCount++] =
          record->memberIds[spriteSlots] + ENEMY_ID_FIRST;
      memberCounts[spriteSlots]--;
    }
    break;
  default:
    for (i = 0; i < FORMATION_MEMBERS; i++) {
      for (copies = 0; copies < memberCounts[i]; copies++)
        enemyClasses[enemyCount++] = record->memberIds[i] + ENEMY_ID_FIRST;
    }
    break;
  }
  enemyClasses[enemyCount] = 0;
  battle->firstDefeated = ENEMY_BATTLE_SIZE;
  battle->defeatState = DEFEAT_ONE;
  for (i = FIRST_ENEMY_UNIT; i < (FIRST_ENEMY_UNIT + ENEMY_BATTLE_SIZE); i++)
    Iwram_ClearWordsEntry(Unit_Get(i), BATTLE_UNIT_SIZE);

  i = 0;
  if (enemyClasses[0] != 0) {
    for (;;) {
      // Repeated enemy classes get distinct suffixes within this encounter.
      s32 suffix = BattleEnemy_ClaimNameLetter(enemyClasses[i], 1);
      struct Unit *unit;
      s32 enemyId;

      if (suffix & ENEMY_LETTER_FIRST_TOO)
        BattleEnemy_LetterFirstOfKind(enemyClasses[i]);
      BattleUnit_SetFar(i + FIRST_ENEMY_UNIT, enemyClasses[i],
                        suffix & ENEMY_LETTER_MASK);
      unit = Unit_Get(i + FIRST_ENEMY_UNIT);
      if (levelAdjustment != 0)
        BattleEnemy_AdjustLevel(i + FIRST_ENEMY_UNIT, levelAdjustment);
      else if (!GameFlag_Get(FLAG_MONSTER_BATTLE) &&
               GameFlag_Get(FLAG_HARD_LEVEL))
        BattleUnit_Strengthen(i + FIRST_ENEMY_UNIT);
      battle->forceField = 0;
      enemyId = unit->enemyId;
      switch (enemyId) {
      case ENEMY_POSEIDON:
      case ENEMY_POSEIDON_SECOND_ACTION:
      case ENEMY_POSEIDON_3:
        if (!GameFlag_Get(FLAG_MONSTER_BATTLE))
          battle->forceField = 1;
        break;
      }
      i++;
      if (i >= ENEMY_BATTLE_SIZE || enemyClasses[i] == 0)
        break;
    }
  }
  return i;
}

/// Returns the class's defeat-sound kind, 0 to 4, or -1 for none; a fallen
/// enemy's sound is SE_ENEMY_CRY plus it. Reads any index unchecked.
u32 BattleSprite_GetDefeatSound(s32 index) {
  u32 kind = gBattleSprites[index].defeatSound;

  if ((s32)kind > 4)
    kind = -1U;
  return kind;
}

/// The last enemy with a row in gBattleSprites (battle_sprites.c).
#define ENEMY_CLASS_LAST 386

/// Returns the class's battle sprite, row 0's for a class out of range.
s32 BattleSprite_GetSprite(s32 index) {
  if ((u32)index > ENEMY_CLASS_LAST)
    return gBattleSprites[0].animation;
  return gBattleSprites[index].animation;
}

/// Returns the class's sprite palette. For a class out of range it returns row
/// 0's sprite, not a palette.
s32 BattleSprite_GetPalette(s32 index) {
  if ((u32)index > ENEMY_CLASS_LAST)
    return gBattleSprites[0].animation;
  return gBattleSprites[index].palette;
}

/// Returns 1 for a small class, which takes one enemy sprite slot instead of
/// two and stands closer to its neighbours; 0 out of range.
s32 BattleSprite_IsSmall(s32 index) {

  s32 result;
  u32 flags;

  if ((u32)index > ENEMY_CLASS_LAST)
    return 0;
  result = 0;
  flags = gBattleSprites[index].isSmall;
  if (flags)
    result = 1;
  return result;
}

/// Returns the battle effect a class's plain attack shows, 1 for none or out of
/// range.
u32 BattleSprite_GetAttackEffect(u32 index) {
  u16 value;

  if (index > ENEMY_CLASS_LAST)
    return 1;
  value = gBattleSprites[index].attackEffect;
  return value ? value : 1;
}

/// Returns how high a class hops as it runs up to attack, in pixels; 0 out of
/// range.
u32 BattleSprite_GetAttackHop(u32 index) {
  u16 value;

  if (index > ENEMY_CLASS_LAST)
    return 0;
  value = gBattleSprites[index].attackHop;
  return value ? value : 0;
}

/// Returns 1 for a class whose blow lands with a heavier impact; 0 out of
/// range.
s32 BattleSprite_HasHeavyImpact(s32 index) {
  if ((u32)index > ENEMY_CLASS_LAST)
    return 0;
  return gBattleSprites[index].heavyImpact;
}

/// Returns the height an animation is drawn at over a class, in pixels, or 0
/// for its size's default; 0 out of range.
u32 BattleSprite_GetEffectHeight(u32 index) {
  if (index > ENEMY_CLASS_LAST)
    return 0;
  return gBattleSprites[index].effectHeight;
}
