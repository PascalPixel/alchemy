//--------------------------------------------------------------------------
// advance_owner_count_to_target.c
// Party: raise a character to a specified level.
//--------------------------------------------------------------------------

#include "owner_state.h"
#include "party_state.h"
#include "battle_unit.h"

struct LevelUpResult;
struct LevelUpResult *Owner_LevelUp(s32 owner, struct LevelUpResult *result);
void UnitCalc(s32);
u32 Unit_GetExperienceForLevel(s32 owner, s32 level);

// Levels the owner up once its experience reaches the next level's threshold;
// returns the result record then, and 0 otherwise.
struct LevelUpResult *Func_080afb80(s32 owner_id, struct LevelUpResult *value) {
  struct BattleUnit *owner;

  owner = (struct BattleUnit *)UnitGet(owner_id);
  /* FAKEMATCH: the barrier loads the level before the experience field's offset
   * is started; plain, the offset comes first (one reordered pair, differ 2).
   */
  asm("");
  if ((owner->experience >=
       Unit_GetExperienceForLevel(owner_id, owner->level + 1)) &&
      (Owner_LevelUp(owner_id, value) != NULL)) {
    return value;
  }
  return 0;
}

//--------------------------------------------------------------------------
// Level up one level at a time until the target, recalculating stats each time.
// unit: character ID.
// lv: target level.
//--------------------------------------------------------------------------
void Unit_LevelUpTo(s32 unit, s32 lv) {
  u8 res[16]; // Receives struct LevelUpResult (unused).
  s32 cur = ((struct BattleUnit *)UnitGet(unit))->level;

  while (cur < lv) {
    Owner_LevelUp(unit, (struct LevelUpResult *)res);
    cur++;
  }
  UnitCalc(unit);
}
