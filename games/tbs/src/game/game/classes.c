/// @file
/// Element levels and classes.
/// Part of game.c, the game object, compiled only through it.

#include "../game_internal.h"

/// The elemental power and resistance of one element level.
struct ElementLevelBonus {
  u16 power;
  u16 res;
};

extern struct ElementLevelBonus gElement_PowerResistByLevel[16];

/// Returns the class series selected by the primary and secondary elements.
s32 Class_GetSeries(s32 primaryElement, s32 secondaryElement) {
  return gOwner_FourColumnValues[(primaryElement * 4) + secondaryElement];
}

/// Writes ten times each element level to `output`: an enemy's from its
/// element preset, a character's from the Djinn set on it (`source` is its
/// Djinn bits) plus its own element bonus.
s32 Unit_GetElementLevels(s32 record, const u8 *source, s32 output[4]) {
  s32 i;
  s32 *cursor;

  if (record > 7) {
    u32 index;

    index = ((const struct EnemyDefinition *)Enemy_GetDefinition(record))
                ->elementPreset;
    if (index > 43)
      index = 0;

    i = 0;
    cursor = output;
    for (; i < NUM_ELEMENTS; i++)
      *cursor++ = gEnemy_ElementPresetTable[index].digits[i] * 10;
  } else {
    cursor = output;
    source += (u8 *)&((struct BattleUnit *)0)->djinnActiveCounts -
              (u8 *)&((struct BattleUnit *)0)->djinnAvailable;
    for (i = 3; i >= 0; i--) {
      u32 value = *source;
      source++;
      *cursor++ = value * 10;
    }

    if (record <= 7) {
      for (i = 0; i < NUM_ELEMENTS; i++) {
        *output += Character_GetDefinition(record)->elementBonus[i];
        output++;
      }
    }
  }

  return 0;
}

/// Returns a unit's level in `element` (0 to 3).
s32 Unit_GetElementLevel(s32 owner, s32 element) {
  struct BattleUnit *unit = UnitGet(owner);
  s32 elementLevels[4];
  s32 result = 0;

  if (element < NUM_ELEMENTS) {
    Unit_GetElementLevels(unit->classId, (u8 *)unit->djinnAvailable,
                          elementLevels);
    result = elementLevels[element] / 10;
  }
  return result;
}

/// Returns the element an enemy attacks with, from its element preset.
s32 Enemy_GetAttackElement(struct BattleUnit *unit) {
  const struct EnemyDefinition *enemy =
      (const struct EnemyDefinition *)Enemy_GetDefinition(unit->classId);
  u8 presetIndex = enemy->elementPreset;

  if (presetIndex > 43)
    presetIndex = 0;
  return gEnemy_ElementPresetTable[presetIndex].first;
}

// Writes a unit's base power and resistance in each element: an enemy's come
// from its preset, a character's from its element levels.
// Both callers ignore the result. The matching return type preserves r0;
// plain void changes the two return instructions (tbs-en: differ 4).
// Its array view keeps the table-address addition after the first store;
// using the typed fields in that build differs 2 in tbs-en.
#ifdef UBFIX
void Unit_CalcBaseElements(s32 owner, struct BattleElementStats *destination) {
#else
u32 Unit_CalcBaseElements(s32 owner, s16 destination[4][2]) {
#endif
  struct BattleUnit *state = UnitGet(owner);
  u32 index;
  s32 values[4];
  s32 i;

  if (state->classIndex == 0) {
    index =
        ((const struct EnemyDefinition *)Enemy_GetDefinition(state->classId))
            ->elementPreset;
    if (index > 43)
      index = 0;

#ifdef UBFIX
    // Copy the packed pairs without accessing halfword members as words.
    memcpy(destination, gEnemy_ElementPresetTable[index].values,
           sizeof(gEnemy_ElementPresetTable[index].values));
    return;
#else
    i = 0;
    for (;;) {
      ((s32 *)destination)[i] = gEnemy_ElementPresetTable[index].values[i];
      i++;
      if (i > 3) {
        return index;
      }
    }
#endif
  }

  Unit_GetElementLevels(state->classId, (u8 *)state->djinnAvailable, values);
  for (i = 0; i < NUM_ELEMENTS; i++) {
    s32 value;
    s32 ones;
    s32 tens;

    value = values[i];
    ones = value % 10;
    tens = value / 10;

    if (tens > 15)
      tens = 15;
    if (tens < 0)
      tens = 0;

#ifdef UBFIX
    destination[i].power = gElement_PowerResistByLevel[tens].power + ones;
    destination[i].resistance = gElement_PowerResistByLevel[tens].res + ones;
#else
    destination[i][0] = gElement_PowerResistByLevel[tens].power + ones;
    destination[i][1] = gElement_PowerResistByLevel[tens].res + ones;
#endif
  }
}

/// Returns the class a character has with `djinn` set: the series is chosen by
/// its two strongest elements, and the class is the last of the series whose
/// element requirements it meets. Isaac and Garet as children and Jenna have
/// fixed classes.
s32 Class_Determine(s32 character, const u8 *djinn) {
  s32 levels[4];
  s32 primary;
  s32 best;
  s32 series;
  s32 result;
  s32 j;
  s32 i;

  result = -1;
  if (character > 7)
    return 0;
  Unit_GetElementLevels(character, djinn, levels);
  if (GameFlag_Test(FLAG_CHILDHOOD)) {
    if (character == ISAAC)
      return 200;
    if (character == GARET)
      return 201;
  }
  if (character == JENNA)
    return 202;
  if (result != -1)
    return result;
  best = result;
  primary = result;
  for (i = 0; i < NUM_ELEMENTS; i++) {
    if (best < levels[i]) {
      best = levels[i];
      primary = i;
    }
  }
  j = -1;
  best = -1;
  for (i = 0; i < NUM_ELEMENTS; i++) {
    if (i != primary && best < levels[i]) {
      best = levels[i];
      j = i;
    }
  }
  series = Class_GetSeries(primary, levels[j] > 9 ? j : primary);
  for (i = 202; i >= 0; i--) {
    if (gClass_DefinitionTable[i].series != series)
      continue;
    for (j = 0; j < NUM_ELEMENTS; j++) {
      if (levels[j] < gClass_DefinitionTable[i].required[j] * 10)
        break;
    }
    if (j == 4) {
      result = i;
      break;
    }
  }
  if (result == -1)
    result = 0;
  return result;
}

/// Returns the definition of class `classId`.
struct ClassDefinition *Class_GetDefinition(s32 classId) {
  return &gClass_DefinitionTable[classId];
}

/// Works out a character's class again after its Djinn change, then its
/// Psynergy list and its base elements.
void Unit_UpdateClass(s32 ownerId) {
  struct BattleUnit *owner = UnitGet(ownerId);

  owner->classIndex =
      Class_Determine(owner->classId, (u8 *)owner->djinnAvailable);
  Unit_RefreshPsynergy(ownerId);
#ifdef UBFIX
  Unit_CalcBaseElements(ownerId, owner->baseElement);
#else
  Unit_CalcBaseElements(ownerId, (s16 (*)[2])owner->baseElement);
#endif
}
