/// @file
/// Element levels and classes.
// Part of game.c.

#include "../game_internal.h"

/// Returns the class series of a character whose strongest element is
/// `primaryElement` and second `secondaryElement`, from the 4 by 4 table.
s32 Class_GetSeries(s32 primaryElement, s32 secondaryElement) {
  return gClass_SeriesByElements[(primaryElement * 4) + secondaryElement];
}

/// Writes ten times each element level to `output`: an enemy's from its
/// element preset, a character's from the Djinn set on it (`source` is its
/// Djinn bits) plus its own element bonus.
s32 Unit_GetElementLevels(s32 record, const u8 *source, s32 output[4]) {
  s32 i;
  s32 *cursor;

  if (record > PIERS) {
    u32 index;

    index = Enemy_GetDefinition(record)->elementPreset;
    if (index > 43)
      index = 0;

    i = 0;
    cursor = output;
    for (; i < ELEMENT_COUNT; i++)
      *cursor++ = gEnemy_ElementPresetTable[index].elementLevels[i] * 10;
  } else {
    cursor = output;
    // `source` points at the unit's Djinn bits; the counts of set Djinn by
    // element follow them in the unit.
    source += (u8 *)&((struct Unit *)0)->djinnSetCounts -
              (u8 *)&((struct Unit *)0)->djinnHeld;
    for (i = 0; i < ELEMENT_COUNT; i++) {
      u32 count = *source++;

      *cursor++ = count * 10;
    }

    if (record <= PIERS) {
      for (i = 0; i < ELEMENT_COUNT; i++) {
        *output += Character_GetDefinition(record)->elementBonus[i];
        output++;
      }
    }
  }

  return 0;
}

/// Returns how strongly a unit is attuned to `element`, its whole element
/// level (a tenth of what Unit_GetElementLevels counts), and 0 for
/// `ELEMENT_NONE`.
/// Called by the two status screens to show the element levels, and by
/// Battle_IsReviveEffect.
s32 Unit_GetElementLevel(s32 unitId, s32 element) {
  struct Unit *unit = Unit_Get(unitId);
  s32 summonBoost[4];
  s32 result = 0;

  if (element < ELEMENT_COUNT) {
    Unit_GetElementLevels(unit->id, (u8 *)unit->djinnHeld, summonBoost);
    result = summonBoost[element] / 10;
  }
  return result;
}

/// Returns the element an enemy attacks with, from its element preset. Called
/// by Item_GetEquippedElement for an enemy, which has no weapon.
s32 Enemy_GetAttackElement(struct Unit *unit) {
  const struct EnemyDefinition *enemy = Enemy_GetDefinition(unit->id);
  u8 presetIndex = enemy->elementPreset;

  if (presetIndex > 43)
    presetIndex = 0;
  return gEnemy_ElementPresetTable[presetIndex].first;
}

// Writes a unit's base power and resistance in each element: an enemy's come
// from its preset, a character's from its element levels, as the value of
// gElement_PowerResistByLevel for the whole level (0 to 15) plus the level's
// tenths (Unit_GetElementLevels counts in tenths).
UNREAD_RESULT
Unit_CalculateBaseElements(s32 unitId, struct BattleElementStats *destination) {
  struct Unit *state = Unit_Get(unitId);
  s32 levels[ELEMENT_COUNT];
  s32 i;

  if (state->classId == CLASS_NONE) {
    u32 preset = Enemy_GetDefinition(state->id)->elementPreset;

    if (preset > 43)
      preset = 0;
    // The copy returns from inside its loop, as the last element is written.
    for (i = 0;;) {
      destination[i] = gEnemy_ElementPresetTable[preset].stats[i];
      if (++i >= ELEMENT_COUNT)
        RETURN_UNREAD(preset);
    }
  }

  // Element levels in tenths: the whole level picks the table's row, the
  // tenths add to it.
  Unit_GetElementLevels(state->id, (u8 *)state->djinnHeld, levels);
  for (i = 0; i < ELEMENT_COUNT; i++) {
    s32 tenths = levels[i] % 10;
    s32 level = levels[i] / 10;

    if (level > 15)
      level = 15;
    if (level < 0)
      level = 0;
    // The pair is written as two halfwords in a row, power then resistance.
    ((s16 *)destination)[i * 2] =
        gElement_PowerResistByLevel[level].power + tenths;
    ((s16 *)destination)[i * 2 + 1] =
        gElement_PowerResistByLevel[level].res + tenths;
  }
}

/// Returns the class a character has with `djinn` set: the series is chosen by
/// its two strongest elements, and the class is the last of the series whose
/// element requirements it meets. Isaac and Garet as children and Jenna have
/// fixed classes (200, 201 and 202). Returns 0 for a unit past the eight
/// characters, and for a character that reaches no class of its series; 0 is
/// also the class that marks an enemy.
s32 Class_Determine(s32 character, const u8 *djinn) {
  s32 levels[4];
  s32 primary;
  s32 best;
  s32 series;
  s32 result;
  s32 j;
  s32 i;

  result = -1;
  if (character > PIERS)
    return 0;
  Unit_GetElementLevels(character, djinn, levels);
  if (GameFlag_Get(FLAG_CHILDHOOD)) {
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
  for (i = 0; i < ELEMENT_COUNT; i++) {
    if (best < levels[i]) {
      best = levels[i];
      primary = i;
    }
  }
  j = -1;
  best = -1;
  for (i = 0; i < ELEMENT_COUNT; i++) {
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
    for (j = 0; j < ELEMENT_COUNT; j++) {
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
  return (struct ClassDefinition *)&gClass_DefinitionTable[classId];
}

/// Works out a character's class again after its Djinn change, then its
/// Psynergy list and its base elements. Writes the unit's classIndex,
/// psynergy and baseElement. Called when a Djinni is set or put on
/// standby, at the prologue's start and end, at a new game and around the
/// Colosso's inventory snapshot.
void Unit_UpdateClass(s32 unitId) {
  struct Unit *unit = Unit_Get(unitId);

  unit->classId = Class_Determine(unit->id, (u8 *)unit->djinnHeld);
  Unit_RefreshPsynergy(unitId);
  Unit_CalculateBaseElements(unitId, unit->baseElement);
}
