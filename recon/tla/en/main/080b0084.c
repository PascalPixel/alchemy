// Under -mthumb-slow-loads (2026-10-26): differ 4 of 86 in tla-en, instruction
// order only. No empty asm before any one statement lowers it.
// Draft of Unit_CalculateBaseElements: differ 8 of 86 (tla-en, 2026-10-18).
// Ported from The Broken Seal's function of the same name
// (games/tbs/src/game/game.c): a unit's base power and resistance in each
// element, an enemy's copied from the preset its record names (48 here, the
// index at record byte 42), a character's from its element levels. The Lost
// Age divides the level by ten through a high multiply by 0x1999999a.
//
// Left: instruction order only (the preset table's load, one add of the
// tens scaling and the loop's pointer step each sit one or two places
// apart). Rewritten from the earlier port of The Broken Seal's file, which
// no longer compiled.
#include "types.h"
#include "battle_unit.h"
#include "owner_state.h"

// One of the 48 element presets an enemy record names (as in
// games/tla/src/game/character/resist.c).
struct ElementPreset {
  u32 element;
  u8 digits[4];
  s32 values[4];
};

struct ElementLevelBonus {
  u16 power;
  u16 res;
};

extern const struct ElementPreset gData_080c6684[];
extern const struct ElementLevelBonus Data_080c6644[16];

const u8 *Enemy_GetDefinition(s32 selector);
s32 Unit_GetElementLevels(s32 record, const u8 *source, s32 output[4]);
u32 Math_UnsignedMulHigh(u32 a, u32 b);

u32 Unit_CalculateBaseElements(s32 owner, s16 destination[4][2]) {
  struct BattleUnit *state = (struct BattleUnit *)Unit_Get(owner);
  u32 index;
  s32 values[4];
  s32 i;

  if (state->classIndex == 0) {
    index = Enemy_GetDefinition(state->enemyId)[42];
    if (index > 47)
      index = 0;

    i = 0;
    for (;;) {
      ((s32 *)destination)[i] = gData_080c6684[index].values[i];
      i++;
      if (i > 3)
        return index;
    }
  }

  Unit_GetElementLevels(state->enemyId, (u8 *)state->djinnAvailable, values);
  for (i = 0; i < 4; i++) {
    s32 ones;
    s32 tens;

    tens = Math_UnsignedMulHigh(values[i], 0x1999999a);
    ones = values[i] - tens * 10;

    if (tens > 15)
      tens = 15;
    if (tens < 0)
      tens = 0;

    destination[i][0] = Data_080c6644[tens].power + ones;
    destination[i][1] = Data_080c6644[tens].res + ones;
  }
}
