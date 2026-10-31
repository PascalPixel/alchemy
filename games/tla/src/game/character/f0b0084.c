#include "types.h"
#include "battle_unit.h"
#include "enemy_definition.h"
#include "game.h"
#include "owner_state.h"

/// One of the 48 element presets an enemy record names (as in
/// games/tla/src/game/character/resist.c).
struct ElementPreset {
  u32 element;
  u8 digits[4];
  struct BattleElementStats values[4];
};

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
