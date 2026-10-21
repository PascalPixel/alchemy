/// @file
/// Element levels and classes.
// Part of game.c.

#include "../game_internal.h"

/// The elemental power and resistance of one element level, in
/// gElement_PowerResistByLevel, which Unit_CalcBaseElements reads by whole
/// level, 0 to 15.
struct ElementLevelBonus {
  u16 power; ///< base power, before the tenths of a level
  u16 res;   ///< base resistance, likewise
};

extern struct ElementLevelBonus gElement_PowerResistByLevel[16];

/// Returns the class series of a character whose strongest element is
/// `primaryElement` and second `secondaryElement`, from the 4 by 4 table.
/// Called by Class_Determine.
s32 Class_GetSeries(s32 primaryElement, s32 secondaryElement) {
  return gOwner_FourColumnValues[(primaryElement * 4) + secondaryElement];
}

/// Writes ten times each element level to `output`: an enemy's from its
/// element preset, a character's from the Djinn set on it (`source` is its
/// Djinn bits) plus its own element bonus. Called by Class_Determine,
/// Unit_GetElementLevel and Unit_CalcBaseElements.
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
    // `source` points at the unit's Djinn bits; the counts of set Djinn by
    // element follow them in the unit.
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

/// Returns a unit's level in `element` (0 to 3), and 0 for `ELEMENT_NONE`.
/// Called by the two status screens to show the element levels, and by
/// Battle_IsReviveEffect.
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

/// Returns the element an enemy attacks with, from its element preset. Called
/// by Item_GetEquippedElement for an enemy, which has no weapon.
s32 Enemy_GetAttackElement(struct BattleUnit *unit) {
  const struct EnemyDefinition *enemy =
      (const struct EnemyDefinition *)Enemy_GetDefinition(unit->classId);
  u8 presetIndex = enemy->elementPreset;

  if (presetIndex > 43)
    presetIndex = 0;
  return gEnemy_ElementPresetTable[presetIndex].first;
}

// Writes a unit's base power and resistance in each element: an enemy's come
// from its preset, a character's from its element levels, as the value of
// gElement_PowerResistByLevel for the whole level (0 to 15) plus the level's
// tenths (Unit_GetElementLevels counts in tenths). Called by BattleUnitSet
// and Unit_UpdateClass, which both ignore the result.
#ifdef UBFIX
void Unit_CalcBaseElements(s32 owner, struct BattleElementStats *destination) {
#else
u32 Unit_CalcBaseElements(s32 owner, s16 destination[4][2]) {
#endif
  // Without UBFIX the return type is u32, which keeps r0 as the ROM does
  // (as void, the two return instructions differ: 4 in tbs-en), and the
  // destination is the array view, which keeps the table address added
  // after the first store (the typed fields differ by 2 in tbs-en).
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
/// fixed classes (200, 201 and 202). Called by Unit_UpdateClass. Returns 0
/// for a unit past the eight characters, and for a character that reaches
/// no class of its series; 0 is also the class that marks an enemy.
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
  // A second element below level 1 does not count: the series is then the
  // primary element's alone. Ties go to the lower element number, as each
  // search keeps the first strictly greater level.
  series = Class_GetSeries(primary, levels[j] > 9 ? j : primary);
  // The last class of the series that the element levels reach.
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

/// Returns class `classId`'s definition: its series, element requirements,
/// stat multipliers and the Psynergy it teaches by level.
struct ClassDefinition *Class_GetDefinition(s32 classId) {
  return &gClass_DefinitionTable[classId];
}

/// Works out a character's class again after its Djinn change, then its
/// Psynergy list and its base elements. Writes the unit's classIndex,
/// actionSlots and baseElement. Called when a Djinni is set or put on
/// standby, at the prologue's start and end, at a new game and around the
/// Colosso's inventory snapshot.
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
