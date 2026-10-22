#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "owner_state.h"
#include "battle_unit.h"
#include "runtime_interfaces.h"

/// One of the 48 element presets an enemy record names.
struct ElementPreset {
  u32 element;
  u8 digits[4];
  u8 unk08[16];
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
    table = gData_080c6684;
    preset = &table[index];
    /* FAKEMATCH: an empty asm reading the table puts its load between the
       index's two steps; as no asm, 2 instructions differ in tla-en. */
    asm("" : "+r"(preset) : "r"(table));
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
