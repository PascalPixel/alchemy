/// @file
/// Element levels and classes.
// Part of game.c.

#include "../game_internal.h"

/// One of the 48 element presets an enemy record names.
struct ElementPreset {
  u32 element;
  u8 digits[4];
  struct BattleElementStats values[4];
};

struct OwnerGrowthRecord;

extern const struct ElementPreset gData_080c6684[];

struct OwnerGrowthRecord *Character_GetDefinition(s32 index);

/// Each element's value, ten per digit: an enemy's from the preset its record
/// names, a character's from its djinn counts plus its growth record's bonus.
s32 Unit_GetElementLevels(s32 record, const u8 *source, s32 output[4]) {
  if (record > 7) {
    const struct ElementPreset *table;
    const struct ElementPreset *preset;
    u32 index;
    s32 i;

    index = Enemy_GetDefinition(record)[42];
    if (index > 47)
      index = 0;
    /* FAKEMATCH(temporary): the table in its own variable; as
     * &gData_080c6684[index], 6 of 58 instructions differ in tla-en. */
    table = gData_080c6684;
    preset = &table[index];
    for (i = 0; i <= 3; i++)
      output[i] = preset->digits[i] * 10;
  } else {
    s32 i;

    for (i = 0; i <= 3; i++)
      output[i] = source[36 + i] * 10;
    if (record <= 7) {
      const u8 *definition = (const u8 *)Character_GetDefinition(record);

      for (i = 0; i <= 3; i++)
        output[i] += definition[146 + i];
    }
  }
  return 0;
}

s32 Unit_GetElementLevel(s32 owner, s32 index) {
  struct OwnerInventoryState *state = Unit_Get(owner);
  s32 values[4];
  s32 result = 0;

  if (index <= 3) {
    Unit_GetElementLevels(
        state->classId,
        (const u8 *)&((struct OwnerDjinnState *)state)->flags.banks.owned,
        values);
    result = values[index] / 10;
  }
  return result;
}

/// The element of the preset the unit's enemy record names; an id past the
/// table's end reads the first preset.
s32 Enemy_GetAttackElement(void *state) {
  struct BattleUnit *unit = state;
  u8 preset = Enemy_GetDefinition(unit->enemyId)[42];
  const struct ElementPreset *table;
  u32 offset;

  if (preset > 47)
    preset = 0;
  table = gData_080c6684;
  /* FAKEMATCH: the barrier loads the table's address before the preset is
     scaled, and the offset local keeps the [r3, r2] address; as no barrier,
     2 instructions differ in tla-en. */
  asm("");
  offset = preset * sizeof(struct ElementPreset);
  return *(u32 *)((u8 *)table + offset);
}

struct ElementLevelBonus {
  u16 power;
  u16 res;
};

extern const struct ElementPreset gData_080c6684[];
extern struct ElementLevelBonus Data_080c6644[16];

u32 Math_UnsignedMulHigh(u32 a, u32 b);

void Unit_CalculateBaseElements(s32 owner,
                                struct BattleElementStats *destination) {
  struct BattleUnit *state = Unit_Get(owner);
  u32 index;
  s32 values[4];
  s32 i;

  if (state->classIndex == 0) {
    index = Enemy_GetDefinition(state->enemyId)[42];
    if (index > 47)
      index = 0;

    i = 0;
    for (;;) {
      destination[i] = gData_080c6684[index].values[i];
      i++;
      if (i > 3)
        return;
    }
  }

  Unit_GetElementLevels(state->enemyId, (const u8 *)state->djinnAvailable,
                        values);
  for (i = 0; i < 4; i++) {
    s32 ones;
    s32 tens;

    tens = Math_UnsignedMulHigh(values[i], 0x1999999a);
    ones = values[i] - tens * 10;

    if (tens > 15)
      tens = 15;
    if (tens < 0)
      tens = 0;

#ifdef UBFIX
    destination[i].power = Data_080c6644[tens].power + ones;
    destination[i].resistance = Data_080c6644[tens].res + ones;
#else
    // UB: the stats are stored through an s16 [4][2] view of the four
    // structs, the element pair table's spelling; as destination[i].power
    // and .resistance, 4 of 86 instructions differ in tla-en.
    ((s16 (*)[2])destination)[i][0] = Data_080c6644[tens].power + ones;
    ((s16 (*)[2])destination)[i][1] = Data_080c6644[tens].res + ones;
#endif
  }
}

extern const s32 Data_080c6604[4][4];

s32 Unit_GetElementLevels(s32 record, const u8 *source, s32 output[4]);

/// Chooses a character's class: the series comes from its two strongest
/// elements (or the item that picks one), and the class is the last of that
/// series whose element requirements it meets.
s32 Class_Determine(s32 character, u32 *djinn, s32 item) {
  s32 levels[4];
  s32 primary;
  s32 best;
  s32 series;
  s32 result;
  s32 i;
  s32 second;
  s32 j;
  s32 lastSeriesItem = 445; // One constant for its whole life.

  result = -1;
  if (character > 7)
    return 0;
  /* FAKEMATCH(signature): called as a void function, though it returns 0;
   * as a plain call, 2 of 156 instructions differ in tla-en. */
  ((void (*)(s32, const u8 *, s32 *))Unit_GetElementLevels)(
      character, (const u8 *)djinn, levels);
  best = result;
  primary = result;
  for (i = 0; i < 4; i++) {
    if (best < levels[i]) {
      best = levels[i];
      primary = i;
    }
  }
  second = -1;
  best = -1;
  for (i = 0; i < 4; i++) {
    if (i != primary && best < levels[i]) {
      best = levels[i];
      second = i;
    }
  }
  if (levels[second] <= 9)
    second = primary;
  series = Data_080c6604[primary][second];
  if (series == 2 && character == 5)
    series = 13;
  else if (series == 6 && character == 7)
    series = 14;
  if (item == 444)
    series = 16;
  else if (item == 443)
    series = 15;
  else if (item == lastSeriesItem)
    series = 17;
  for (i = 243; i >= 0; i--) {
    if (gClass_DefinitionTable[i].series != series)
      continue;
    for (j = 0; j < 4; j++) {
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

struct ClassDefinition *Class_GetDefinition(s32 classId) {
  return &gClass_DefinitionTable[classId];
}

void Unit_UpdateClass(s32 ownerId) {
  struct BattleUnit *owner = Unit_Get(ownerId);

  owner->classIndex = Class_Determine(owner->enemyId, owner->djinnAvailable,
                                      Inventory_GetEquippedItem(
                                          (struct OwnerInventoryState *)owner,
                                          10));
  Owner_UpdateClassActions(ownerId);
  Unit_CalculateBaseElements(ownerId, owner->baseElement);
}

void Unit_RefreshPsynergy(s32 owner_id) { Unit_UpdateClass(owner_id); }
