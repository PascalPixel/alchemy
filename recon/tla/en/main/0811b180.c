// Draft of Func_0811b180, The Broken Seal's Summon_LayoutPositions
// (games/tbs/src/battle/battle.c): differ 8 of 134 (tla-en, 2026-10-15).
// The Lost Age keys each actor by the enemy id at 0x14a, and nine enemies
// stand forward at -50 where The Broken Seal names two. The spacing is
// unsigned here: the native halves it with lsrs.

// Left: three order rows, each a load the native issues between the two
// halves of a constant: actorIds[index] inside 0xff02 (twice) and the enemy
// id inside 357. Here the constant is finished first, the residue the Djinn
// drafts record. Tried: the test as (u16)(id + 0xff02) (8), as a shifted
// u32 (83).

#include "types.h"
#include "battle_unit.h"

struct BattleUnit *UnitGet(s32 unit);
s32 Summon_IsEntryFlagged(s32 index);

// Lays out the actors of a summon in a column: each stands at x = -80
// (nearer, at -50, for a few large enemies) and z steps down by half the
// width of each actor on either side, centred on the column's middle.
// Actors 254 and 255 are 25 wide; others are 27 when their summon entry is
// flagged and 38 when not.
void Func_0811b180(u16 *actorIds, s32 count, s32 *xPositions, s32 *zPositions) {
  s32 totalSpacing = count <= 4 ? 30 : 27;
  s32 z = ((count - 1) * totalSpacing) / 2;
  s32 index;

  for (index = 0; index != count; index++) {
    u32 spacing = 0;

    xPositions[index] = -80;
    if (index != 0) {
      struct BattleUnit *actor;

      spacing = 25;
      if ((u16)(actorIds[index] - 254) > 1) {
        actor = UnitGet(actorIds[index]);
        spacing = Summon_IsEntryFlagged(actor->enemyId) ? 27 : 38;
        if (actor->enemyId == 357 || actor->enemyId == 348 ||
            actor->enemyId == 91 || actor->enemyId == 92 ||
            actor->enemyId == 93 || actor->enemyId == 94 ||
            actor->enemyId == 95 || actor->enemyId == 96 ||
            actor->enemyId == 97) {
          xPositions[index] = -50;
        }
      }
    }
    z -= spacing / 2;
    zPositions[index] = z;
    spacing = 25;
    if ((u16)(actorIds[index] - 254) > 1) {
      struct BattleUnit *actor = UnitGet(actorIds[index]);

      spacing = Summon_IsEntryFlagged(actor->enemyId) ? 27 : 38;
    }
    z -= spacing / 2;
  }
}
