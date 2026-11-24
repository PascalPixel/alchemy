/// @file
/// Element levels and classes.
// Part of game.c.

#include "game_internal.h"
#include "game/game.h"
#include "system/mul_fx.h"

/// Element levels are counted in tenths: a Djinni set adds a whole level.
#define ELEMENT_LEVEL_SCALE 10
/// The highest whole element level, the last row of
/// gElement_PowerResistByLevel.
#define ELEMENT_LEVEL_MAX 15

/// Writes each element level in tenths (ELEMENT_LEVEL_SCALE) for the character
/// or enemy `enemyId`: an enemy's from its preset, a character's from the Djinn
/// set on it plus its own bonus. `source` points at the unit's Djinn bits
/// (djinnHeld); the counts of set Djinn by element follow them in the unit.
s32 Unit_GetElementLevels(s32 enemyId, const u8 *source,
                          s32 output[ELEMENT_COUNT]) {
  if (enemyId > PIERS) {
    const struct EnemyElementPreset *table;
    const struct EnemyElementPreset *preset;
    u32 index;
    s32 i;

    index = Enemy_GetDefinition(enemyId)->elementPreset;
    if (index >= ENEMY_ELEMENT_PRESET_COUNT)
      index = 0;
    table = gEnemy_ElementPresetTable;
    preset = &table[index];
    for (i = 0; i < ELEMENT_COUNT; i++)
      output[i] = preset->elementLevels[i] * ELEMENT_LEVEL_SCALE;
  } else {
    s32 i;

    for (i = 0; i < ELEMENT_COUNT; i++)
      output[i] = source[offsetof(struct Unit, djinnSetCounts) -
                         offsetof(struct Unit, djinnHeld) + i] *
                  ELEMENT_LEVEL_SCALE;
    if (enemyId <= PIERS) {
      const struct CharacterDefinition *definition =
          Character_GetDefinition(enemyId);

      for (i = 0; i < ELEMENT_COUNT; i++)
        output[i] += definition->elementBonus[i];
    }
  }
  return 0;
}

/// Returns how strongly a unit is attuned to `element`, its whole element level
/// (a tenth of what Unit_GetElementLevels counts), and 0 for `ELEMENT_NONE`.
s32 Unit_GetElementLevel(s32 unitId, s32 element) {
  struct Unit *unit = Unit_Get(unitId);
  s32 levels[ELEMENT_COUNT];
  s32 result = 0;

  if (element < ELEMENT_COUNT) {
    Unit_GetElementLevels(unit->enemyId, (const u8 *)unit->djinnHeld, levels);
    result = levels[element] / ELEMENT_LEVEL_SCALE;
  }
  return result;
}

/// Returns the element an enemy attacks with, from its element preset; an id
/// past the table's end reads the first preset.
s32 Enemy_GetAttackElement(void *state) {
  struct Unit *unit = state;
  u8 preset = Enemy_GetDefinition(unit->enemyId)->elementPreset;

  if (preset >= ENEMY_ELEMENT_PRESET_COUNT)
    preset = 0;
  return gEnemy_ElementPresetTable[preset].attackElement;
}

/// Writes a unit's base power and resistance in each element: an enemy's from
/// its preset, a character's from its element levels.
void Unit_CalculateBaseElements(s32 unitId,
                                struct BattleElementStats *destination) {
  struct Unit *unit = Unit_Get(unitId);
  u32 index;
  s32 levels[ELEMENT_COUNT];
  s32 i;
  extern const struct ElementLevelBonus
      gElement_PowerResistByLevel[ELEMENT_LEVEL_MAX + 1];

  if (unit->classId == CLASS_NONE) {
    index = Enemy_GetDefinition(unit->enemyId)->elementPreset;
    if (index >= ENEMY_ELEMENT_PRESET_COUNT)
      index = 0;

    // The copy returns from inside its loop, as the last element is written.
    i = 0;
    for (;;) {
      destination[i] = gEnemy_ElementPresetTable[index].stats[i];
      i++;
      if (i >= ELEMENT_COUNT)
        return;
    }
  }

  // Element levels in tenths: the whole level picks the table's row, the
  // tenths add to it.
  Unit_GetElementLevels(unit->enemyId, (const u8 *)unit->djinnHeld, levels);
  for (i = 0; i < ELEMENT_COUNT; i++) {
    s32 tenths;
    s32 level;

    // levels[i] / ELEMENT_LEVEL_SCALE, as a multiply by the reciprocal.
    level = Math_UnsignedMulHigh(levels[i], RECIPROCAL_OF(ELEMENT_LEVEL_SCALE));
    tenths = levels[i] - level * ELEMENT_LEVEL_SCALE;

    if (level > ELEMENT_LEVEL_MAX)
      level = ELEMENT_LEVEL_MAX;
    if (level < 0)
      level = 0;

#ifdef UBFIX
    destination[i].power = gElement_PowerResistByLevel[level].power + tenths;
    destination[i].resistance =
        gElement_PowerResistByLevel[level].resistance + tenths;
#else
    // UB: the stats are stored through an s16 [4][2] view of the four structs.
    ((s16 (*)[2])destination)[i][0] =
        gElement_PowerResistByLevel[level].power + tenths;
    ((s16 (*)[2])destination)[i][1] =
        (gElement_PowerResistByLevel + level)->resistance + tenths;
#endif
  }
}

/// Returns the class a character has with these Djinn: the series of its two
/// strongest elements, or the one its class-changing item (Mysterious Card,
/// Trainer's Whip, Tomegathericon) picks, and the last class of that series
/// whose element requirements it meets.
s32 Class_Determine(s32 character, u32 *djinn, s32 item) {
  s32 levels[ELEMENT_COUNT];
  s32 primary;
  s32 best;
  s32 series;
  s32 result;
  s32 i;
  s32 second;
  s32 j;
  s32 darkMageItem = ITEM_TOMEGATHERICON;

  result = -1;
  if (character > PIERS)
    return 0;
  // UB: Unit_GetElementLevels returns a value; this call goes through a void
  // function type.
#ifdef UBFIX
  Unit_GetElementLevels(character, (const u8 *)djinn, levels);
#else
  ((void (*)(s32, const u8 *, s32 *))Unit_GetElementLevels)(
      character, (const u8 *)djinn, levels);
#endif
  best = result;
  primary = result;
  for (i = 0; i < ELEMENT_COUNT; i++) {
    if (best < levels[i]) {
      best = levels[i];
      primary = i;
    }
  }
  second = -1;
  best = -1;
  for (i = 0; i < ELEMENT_COUNT; i++) {
    if (i != primary && best < levels[i]) {
      best = levels[i];
      second = i;
    }
  }
  // A second element below level 1 does not count, and ties go to the lower
  // element.
  if (levels[second] < ELEMENT_LEVEL_SCALE)
    second = primary;
  series = gClass_SeriesByElements[primary][second];
  // Jenna and Piers have series of their own where the others would be a
  // Guard or a Water Seer.
  if (series == CLASS_SERIES_GUARD && character == JENNA)
    series = CLASS_SERIES_FLAME_USER;
  else if (series == CLASS_SERIES_WATER_SEER && character == PIERS)
    series = CLASS_SERIES_MARINER;
  if (item == ITEM_TRAINERS_WHIP)
    series = CLASS_SERIES_TAMER;
  else if (item == ITEM_MYSTERIOUS_CARD)
    series = CLASS_SERIES_PIERROT;
  else if (item == darkMageItem)
    series = CLASS_SERIES_DARK_MAGE;
  // The last class of the series that the element levels reach.
  for (i = CLASS_COUNT - 1; i >= 0; i--) {
    if (gClass_DefinitionTable[i].series != series)
      continue;
    for (j = 0; j < ELEMENT_COUNT; j++) {
      if (levels[j] <
          gClass_DefinitionTable[i].required[j] * ELEMENT_LEVEL_SCALE)
        break;
    }
    if (j == ELEMENT_COUNT) {
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

/// Works out a character's class again after its Djinn or its class-changing
/// item change, then its Psynergy list and its base elements.
void Unit_UpdateClass(s32 unitId) {
  struct Unit *unit = Unit_Get(unitId);

  unit->classId =
      Class_Determine(unit->enemyId, unit->djinnHeld,
                      Inventory_GetEquippedItem(unit, ITEM_TYPE_CLASS_CHANGE));
  Unit_RebuildPsynergyList(unitId);
  Unit_CalculateBaseElements(unitId, unit->baseElement);
}

/// Works out a character's class, Psynergy and base elements again.
void Unit_RefreshPsynergy(s32 unitId) { Unit_UpdateClass(unitId); }
