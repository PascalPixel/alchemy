#include "types.h"
#include "battle_unit.h"

struct BattleUnit *Unit_Get(s32 unit);
s32 BattleClass_IsSmall(s32 index);

/// Lays out the actors of a summon in a column: each stands at x = -80
/// (nearer, at -50, for a few large enemies) and z steps down by half the
/// width of each actor on either side, centred on the column's middle.
/// Actors 254 and 255 are 25 wide; others are 27 when their summon entry is
/// flagged and 38 when not.
// The Broken Seal's function of the same name, keyed by the enemy id, with
// nine enemies forward where it names two.
void Summon_LayoutPositions(u16 *actorIds, s32 count, s32 *xPositions,
                            s32 *zPositions) {
  s32 totalSpacing = count <= 4 ? 30 : 27;
  s32 z = ((count - 1) * totalSpacing) / 2;
  s32 index;

  for (index = 0; index != count; index++) {
    u32 spacing = 0;

    xPositions[index] = -80;
    if (index != 0) {
      struct BattleUnit *actor;
      u32 enemy;

      spacing = 25;
      if ((u16)(actorIds[index] - 254) > 1) {
        actor = Unit_Get(actorIds[index]);
        spacing = BattleClass_IsSmall(actor->enemyId) ? 27 : 38;
        enemy = actor->enemyId;
        if (enemy == 357 || enemy == 348 || enemy == 91 || enemy == 92 ||
            enemy == 93 || enemy == 94 || enemy == 95 || enemy == 96 ||
            enemy == 97) {
          xPositions[index] = -50;
        }
      }
    }
    z -= spacing / 2;
    zPositions[index] = z;
    spacing = 25;
    if ((u16)(actorIds[index] - 254) > 1) {
      struct BattleUnit *actor = Unit_Get(actorIds[index]);

      spacing = BattleClass_IsSmall(actor->enemyId) ? 27 : 38;
    }
    z -= spacing / 2;
  }
}
