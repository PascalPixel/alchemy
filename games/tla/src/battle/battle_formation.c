/// @file
/// Enemy formations: the enemies chosen, levelled and lettered, and each enemy
/// class's sprite, palette, size and attack effect.
// Part of battle.c.

#include "battle_internal.h"
#include "field/flag_call.h"
#include "battle/presentation.h"

/// The most formations a monster battle chooses among.
#define FORMATION_POOL_SIZE 32
/// How far above the party's level a monster battle's formation may be.
#define FORMATION_LEVEL_MARGIN 3

/// Keeps up to thirty-two eligible formations near the party's adjusted
/// average level, chooses one randomly and returns its level adjustment
/// through outMargin. It first sets flags from others, clears the enemy flags
/// of gBattleFormation_LevelMatchedEnemies, sets flags 71, 230 and 117, and
/// searches every formation.
s32 BattleFormation_SelectLevelMatchedCandidate(s32 *outMargin) {
  s32 matchCount = 0;
  struct FormationCandidate *pool =
      Heap_AllocEwram(FORMATION_POOL_SIZE * sizeof *pool);
  u16 party[8];
  s32 targetLevel = 0;
  s32 unitCount = BattleParty_PrepareMembers(party);
  s32 candidateOrUnitIndex;
  s32 poolIndex;
  s32 result;

  if (unitCount > 0) {
    u16 *unitCursor = party;

    candidateOrUnitIndex = unitCount;
    do {
      u8 level = Unit_Get(*unitCursor)->level;

      candidateOrUnitIndex--;
      unitCursor++;
      targetLevel += level;
    } while (candidateOrUnitIndex != 0);
  }

  targetLevel = targetLevel / unitCount;
  targetLevel += (s8)GameFlag_GetByte(0x3f8);
  if (targetLevel <= 0)
    targetLevel = 1;
  if (targetLevel > UNIT_LEVEL_MAX)
    targetLevel = UNIT_LEVEL_MAX;

  for (candidateOrUnitIndex = 0; candidateOrUnitIndex < FORMATION_POOL_SIZE;
       candidateOrUnitIndex++)
    pool[candidateOrUnitIndex].score = -1;

  if (GameFlag_Get(0x65b))
    GameFlag_Set(0x65f);
  if (GameFlag_Get(0x65c))
    GameFlag_Set(0x65f);
  if (GameFlag_Get(0x65d))
    GameFlag_Set(0x65f);
  if (GameFlag_Get(0x65e))
    GameFlag_Set(0x65f);
  if (GameFlag_Get(0x66b))
    GameFlag_Set(0x66c);
  if (GameFlag_Get(0x668))
    GameFlag_Set(0x66a);
  if (GameFlag_Get(0x669))
    GameFlag_Set(0x66a);
  GameFlag_Clear(0x771);
  if (GameFlag_Get(0x778))
    GameFlag_Set(0x771);

  for (candidateOrUnitIndex = 0; (u32)candidateOrUnitIndex <= 86;
       candidateOrUnitIndex++)
    GameFlag_Clear(ENEMY_DEFEATED_FLAG(
        gBattleFormation_LevelMatchedEnemies[candidateOrUnitIndex]));
  GameFlag_Set(71);
  GameFlag_Set(230);
  GameFlag_Set(117);

  for (candidateOrUnitIndex = 0; (u32)candidateOrUnitIndex < FORMATION_COUNT;
       candidateOrUnitIndex++) {
    s32 score = BattleFormation_AverageLevel(candidateOrUnitIndex);

    if (score >= 0 && score <= targetLevel + FORMATION_LEVEL_MARGIN) {
      s32 lowestIndex = -1;
      s32 lowestLevel;

      for (lowestLevel = 999, poolIndex = 0; poolIndex < FORMATION_POOL_SIZE;
           poolIndex++) {
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

/// A unit's calculated stats as BattleEnemy_AdjustLevel reads them.
struct OwnerStats {
  s16 maxHp;
  s16 maxPp;
  s16 hp;
  s16 pp;
  u16 atk;
  u16 def;
  u16 agi;
  u8 luck;
  u8 actionsPerTurn;
  u8 hpRegen;
  u8 ppRegen;
  u8 unused[2]; ///< no code names it
  struct OwnerElementStats element[4];
};

/// A unit as BattleEnemy_AdjustLevel reads it: its level and its stats.
struct OwnerState {
  u8 name[15]; ///< not read here
  u8 level;
  struct OwnerStats stats;
};

/// Raises an enemy by levels, The Broken Seal's BattleEnemy_AdjustLevel with
/// HP capped at 16383 instead of 9999. Returns nothing, though declared s32 as
/// there.
s32 BattleEnemy_AdjustLevel(s32 owner, s32 levels) {
  struct OwnerStats *base;
  struct OwnerState *state;
  struct OwnerStats *stats;
  s32 value;
  s32 floor;
  s32 i;

  base = Heap_AllocEwram(sizeof(struct OwnerStats));
  state = Unit_Get(owner);
  stats = &state->stats;
  Iwram_CopyWordsEntry(base, stats, sizeof(struct OwnerStats));

  value = stats->maxHp;
  value += __divsi3(levels * 97, 10);
  floor = __divsi3(base->maxHp * 7, 10);
  if (value < floor)
    value = floor;
  if (value > 16383)
    value = 16383;
  stats->maxHp = value;

  value = state->stats.maxPp;
  value += __divsi3(levels * 15, 10);
  floor = __divsi3(base->maxPp * 7, 10);
  if (value < floor)
    value = floor;
  if (value > 9999)
    value = 9999;
  state->stats.maxPp = value;

  floor = __divsi3(levels * 123, 10);
  value = state->stats.atk;
  value += floor;
  floor = __divsi3(base->atk * 7, 10);
  if (value < floor)
    value = floor;
  if (value > 999)
    value = 999;
  state->stats.atk = value;

  floor = __divsi3(levels * 33, 10);
  value = state->stats.def;
  value += floor;
  floor = __divsi3(base->def * 7, 10);
  if (value < floor)
    value = floor;
  if (value > 999)
    value = 999;
  state->stats.def = value;

  floor = __divsi3(levels * 51, 10);
  value = state->stats.agi;
  value += floor;
  floor = __divsi3(base->agi * 7, 10);
  if (value < floor)
    value = floor;
  if (value > 999)
    value = 999;
  state->stats.agi = value;

  for (i = 0; i < ELEMENT_COUNT; i++) {
    value = state->stats.element[i].power + levels * 15;
    floor = __divsi3(base->element[i].power * 7, 10);
    if (value < floor)
      value = floor;
    if (value > 200)
      value = 200;
    state->stats.element[i].power = value;
  }

  state->level += levels;
  Unit_CalculateFar(owner);
  Heap_Release(base);
}

/// Raises a unit's base HP by half and base Attack and Defense by a quarter (at
/// least 7/10 of the old values, at most 16383 and 999), from a copy of its
/// base block, then recalculates its stats.
void BattleUnit_Strengthen(s32 unitId) {
  struct UnitBaseStats *saved = Heap_AllocEwram(36);
  struct Unit *unit = Unit_Get(unitId);
  s32 value;
  s32 floor;

  Iwram_CopyWordsEntry(saved, &unit->baseHp, 36);
  value = unit->baseHp * 3 / 2;
  floor = saved->hp * 7 / 10;
  if (value < floor)
    value = floor;
  if (value > 16383)
    value = 16383;
  unit->baseHp = value;
  value = unit->baseAttack * 5 / 4;
  floor = saved->attack * 7 / 10;
  if (value < floor)
    value = floor;
  if (value > 999)
    value = 999;
  unit->baseAttack = value;
  value = unit->baseDefense * 5 / 4;
  floor = saved->defense * 7 / 10;
  if (value < floor)
    value = floor;
  if (value > 999)
    value = 999;
  unit->baseDefense = value;
  Unit_CalculateFar(unitId);
  Heap_Release(saved);
}

// The letters cycle through CH_CNT values: 26 in the Japanese release, 9
// elsewhere, as String_FindFirstDigit reads them back.
#if LANGUAGE == LANGUAGE_JAPANESE
#define CH_CNT 26
#else
#define CH_CNT 9
#endif

/// Gives an enemy of the class its name letter, which tells it from others of
/// its kind: digits abroad and the alphabet in Japanese. Returns -1 when five
/// classes are already listed.
s32 BattleEnemy_ClaimNameLetter(s32 no, s32 n) {
  struct SummonChargeState *w;
  s32 num;
  s32 i;
  s32 retry;

  w = (struct SummonChargeState *)Ram_HeapSlots->battleWork;
  num = w->count;
  for (i = 0; i < num; i++) {
    if (w->classIds[i] == no)
      break;
  }
  if (i != num) {
    retry = 0;
    if (w->channels[i] < 0) {
      w->channels[i] = 1;
      w->usedMasks[i] = 3;
      return ENEMY_LETTER_FIRST_TOO | 1;
    }
    for (; retry <= 31; retry++) {
      w->channels[i] = (w->channels[i] + 1) % CH_CNT;
      if ((w->usedMasks[i] & (1 << w->channels[i])) == 0)
        break;
    }
    w->usedMasks[i] |= 1 << w->channels[i];
    return w->channels[i];
  }
  if (num <= 4) {
    w->channels[num] = -1;
    w->classIds[num] = no;
    w->usedMasks[num] = 0;
    w->count = num + 1;
    return CH_CNT;
  }
  return -1;
}

#define LIST_MARKER_CHAR SUMMON_CHANNEL_MARKER

/// Frees a departing enemy's name letter for the next of its class.
s32 BattleEnemy_ReleaseNameLetter(s32 actorId) {
  struct SummonChargeState *state =
      (struct SummonChargeState *)Ram_HeapSlots->battleWork;
  struct BattleActorDefinition *actor;
  s32 count = state->count;
  s32 index;
  s32 nameLength;
  s32 bit;
  s32 classId;

  actor = Unit_Get(actorId);
  if (actor->unavailable != CLASS_NONE)
    return;

  classId = actor->classId;
  for (index = 0; index < count; index++) {
    if (state->classIds[index] == classId)
      break;
  }
  if (index == count || state->usedMasks[index] == 0)
    return;

  for (nameLength = 0; nameLength <= 13; nameLength++) {
    if (actor->name[nameLength] == 0)
      break;
  }

  bit = 32;
  if (nameLength > 0)
    bit = actor->name[nameLength - 1] - LIST_MARKER_CHAR;
  state->usedMasks[index] &= ~(1 << bit);
}

/// Gives the first enemy of the class on the field the first letter (0),
/// appended to its name, once a second of its class arrives.
s32 BattleEnemy_LetterFirstOfKind(s32 enemyId) {
  struct Unit *summon;
  s32 objectIndex;
  s32 slot;
  s32 nextSlot;
  u8 occupied;
  u8 marker;

  objectIndex = 0;
  marker = LIST_MARKER_CHAR;
scan_objects:
  summon = Unit_Get(objectIndex + FIRST_ENEMY_UNIT);
  occupied = summon->side;
  if (occupied != 1)
    goto next_object;
  if (summon->enemyId != enemyId)
    goto next_object;
  slot = 0;
  if (summon->name[0] != 0)
    goto scan_slots;
  summon->name[0] = marker;
  summon->name[occupied] = slot;
  return;
scan_slots:
  slot++;
  if (slot > 13)
    return;
  occupied = summon->name[slot];
  if (occupied != 0)
    goto scan_slots;
  nextSlot = slot + 1;
  summon->name[slot] = marker;
  summon->name[nextSlot] = occupied;
  return;
next_object:
  objectIndex++;
  if (objectIndex <= 5)
    goto scan_objects;
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
  remaining = 4;
  do {
    if (*maximumCount++ != 0) {
      s32 id = *memberId;

      count++;
      *choiceCursor++ = id + 8;
    }
    remaining--;
    memberId++;
  } while (remaining >= 0);

  return choices[(Random16() * count) >> 16];
}

/// Builds the battle's enemies from a formation and returns how many there are:
/// each member its fewest times and at random up to its most while the sprite
/// slots hold them, laid out as the formation says.
s32 BattleFormation_BuildEnemyList(s32 recordId) {
  u16 *enemyClasses;
  struct BattleWork *battle;
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
  u32 memberOrEnemyIndex;
  s32 memberIndex;

  battle = (struct BattleWork *)Ram_HeapSlots->battleWork;
  levelAdjustment = 0;
  // Through a byte pointer, which keeps the level adjustment stored first.
  *(u8 *)&battle->enemyKindCount = 0;
  enemyClasses = listBuffer;
  if (GameFlag_Get(FLAG_MONSTER_BATTLE))
    recordId = BattleFormation_SelectLevelMatchedCandidate(&levelAdjustment);
  if ((u32)recordId >= FORMATION_COUNT)
    recordId = 1;
  record = &gFormations[recordId];
  for (memberIndex = 0;
       memberIndex <= 4 && record->minimumCounts[memberIndex] == 0;
       memberIndex++)
    ;
  if (memberIndex == FORMATION_MEMBERS)
    record = &gFormations[1];

  freeSpriteSlots = 6;
  enemyCount = 0;
  for (memberOrEnemyIndex = 0; memberOrEnemyIndex <= 4; memberOrEnemyIndex++) {
    if (record->minimumCounts[memberOrEnemyIndex] != 0) {
      s32 requiredSlots =
          2 - (BattleSprite_IsSmall(record->memberIds[memberOrEnemyIndex] +
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
          2 - (BattleSprite_IsSmall(record->memberIds[memberOrEnemyIndex] +
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

  battle->layout = record->layout;
  switch (battle->layout) {
  case FORMATION_LAYOUT_SHUFFLED:
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
        enemyClasses[enemyCount++] = record->memberIds[slot] + ENEMY_ID_FIRST;
    }
    break;
  case FORMATION_LAYOUT_INTERLEAVED:
    for (;;) {
      remainingKinds = 0;
      for (memberOrEnemyIndex = 0; memberOrEnemyIndex <= 4;
           memberOrEnemyIndex++) {
        if (memberCounts[memberOrEnemyIndex] != 0)
          memberOrder[remainingKinds++] = memberOrEnemyIndex;
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
    for (memberOrEnemyIndex = 0; memberOrEnemyIndex <= 4;
         memberOrEnemyIndex++) {
      for (copies = 0; copies < memberCounts[memberOrEnemyIndex]; copies++)
        enemyClasses[enemyCount++] =
            record->memberIds[memberOrEnemyIndex] + ENEMY_ID_FIRST;
    }
    break;
  }
  enemyClasses[enemyCount] = 0;
  battle->firstDefeated = ENEMY_BATTLE_SIZE;
  battle->defeatState = DEFEAT_ONE;
  for (memberOrEnemyIndex = FIRST_ENEMY_UNIT;
       memberOrEnemyIndex < (FIRST_ENEMY_UNIT + ENEMY_BATTLE_SIZE);
       memberOrEnemyIndex++)
    Iwram_ClearWordsEntry(Unit_Get(memberOrEnemyIndex), BATTLE_UNIT_SIZE);

  memberOrEnemyIndex = 0;
  if (enemyClasses[0] == 0)
    goto assigned;
assign_enemy:
  // Repeated enemy classes get distinct suffixes within this encounter.
  {
    s32 suffixCode =
        BattleEnemy_ClaimNameLetter(enemyClasses[memberOrEnemyIndex], 1);
    struct Unit *unit;
    s32 enemyId;

    if (suffixCode & ENEMY_LETTER_FIRST_TOO)
      BattleEnemy_LetterFirstOfKind(enemyClasses[memberOrEnemyIndex]);
    BattleUnit_SetFar(memberOrEnemyIndex + FIRST_ENEMY_UNIT,
                      enemyClasses[memberOrEnemyIndex],
                      suffixCode & ENEMY_LETTER_MASK);
    unit = Unit_Get(memberOrEnemyIndex + FIRST_ENEMY_UNIT);
    if (levelAdjustment != 0)
      BattleEnemy_AdjustLevel(memberOrEnemyIndex + FIRST_ENEMY_UNIT,
                              levelAdjustment);
    else if (!GameFlag_Get(FLAG_MONSTER_BATTLE) && GameFlag_Get(0x2e))
      BattleUnit_Strengthen(memberOrEnemyIndex + FIRST_ENEMY_UNIT);
    battle->forceField = 0;
    enemyId = unit->enemyId;
    switch (enemyId) {
    case ENEMY_POSEIDON:
    case ENEMY_POSEIDON_SECOND_ACTION:
    case ENEMY_UNUSED_POSEIDON:
      if (!GameFlag_Get(FLAG_MONSTER_BATTLE))
        battle->forceField = 1;
      break;
    }
  }
  memberOrEnemyIndex++;
  if (memberOrEnemyIndex > 5)
    goto assigned;
  if (enemyClasses[memberOrEnemyIndex] != 0)
    goto assign_enemy;
assigned:
  return memberOrEnemyIndex;
}

/// Returns the class's defeat-sound kind, 0 to 4, or -1 for none; a fallen
/// enemy's sound is SE_ENEMY_CRY plus it. Reads any index unchecked.
u32 BattleSprite_GetDefeatSound(s32 index) {
  u32 kind = gSummon_EntryTable[index].defeatSound;

  if ((s32)kind > 4)
    kind = -1U;
  return kind;
}

/// Returns the class's battle sprite, row 0's for a class out of range.
s32 BattleSprite_GetSprite(s32 index) {
  if ((u32)index > 386)
    return gSummon_EntryTable[0].animation;
  return gSummon_EntryTable[index].animation;
}

/// Returns the class's sprite palette. For a class out of range it returns row
/// 0's sprite, not a palette.
s32 BattleSprite_GetPalette(s32 index) {
  if ((u32)index > 386)
    return gSummon_EntryTable[0].animation;
  return gSummon_EntryTable[index].palette;
}

/// Returns 1 for a small class, which takes one enemy sprite slot instead of
/// two and stands closer to its neighbours; 0 out of range.
s32 BattleSprite_IsSmall(s32 index) {

  s32 result;
  u32 flags;

  if ((u32)index > 386)
    return 0;
  result = 0;
  flags = gSummon_EntryTable[index].isSmall;
  if (flags)
    result = 1;
  return result;
}

/// Returns the battle effect a class's plain attack shows, 1 for none or out of
/// range.
u32 BattleSprite_GetAttackEffect(u32 index) {
  u16 value;

  if (index > 386)
    return 1;
  value = gSummon_EntryTable[index].attackEffect;
  return value ? value : 1;
}

/// Returns how high a class hops as it runs up to attack, in pixels; 0 out of
/// range.
u32 BattleSprite_GetAttackHop(u32 index) {
  u16 value;

  if (index > 386)
    return 0;
  value = gSummon_EntryTable[index].attackHop;
  return value ? value : 0;
}

/// Returns 1 for a class whose blow lands with a heavier impact; 0 out of
/// range.
s32 BattleSprite_HasHeavyImpact(s32 index) {
  if ((u32)index > 386)
    return 0;
  return gSummon_EntryTable[index].heavyImpact;
}

/// Returns the height an animation is drawn at over a class, in pixels, or 0
/// for its size's default; 0 out of range.
u32 BattleSprite_GetEffectHeight(u32 index) {
  u8 *table;
  u32 offset;

  if (index > 386)
    return 0;
  table = (u8 *)gSummon_EntryTable;
  offset = index * 8;
  offset += 4;
  return table[offset];
}
