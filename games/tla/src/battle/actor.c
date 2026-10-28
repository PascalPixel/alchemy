/// @file
/// Battle: clean up defeated units' graphics and formation entries.

#include "types.h"
#include "edition.h"
#include "battle.h"
#include "system.h"
#include "battle_party.h"
#include "battle_system.h"
#include "battle_type.h"
#include "battle_work.h"
#include "battle_summon.h"
#include "motobj.h"
#include "animspr.h"

struct BattleUnit *Unit_Get(s32);

void Animation_ApplyChildArgumentFar(void *, s32);

/// Removes the unit from the ally and enemy formations and the planned
/// actions. Returns 0 on removal, or -1 if absent from both formations.
/// id: unit ID.
s32 BattleActor_RemoveFromLists(s32 id) {
  struct BattleWork *work;
  s32 i;
  u32 j;
  s32 unit;

  // Ally formation.
  work = Ram_HeapSlots->battleWork;
  Unit_Get(id)->side = BATTLE_UNIT_ABSENT;
  for (i = 0;; i++) {
    unit = work->partyUnits[i];
    if (unit == id) {
      work->partyUnits[i] = BATTLE_UNIT_REMOVED;
      goto removed;
    }
    if (unit == BATTLE_UNIT_LIST_END)
      break;
  }

  // Enemy formation.
  for (j = 0;; j++) {
    unit = work->enemyIds[j];
    if (unit == id) {
      work->enemyIds[j] = BATTLE_UNIT_REMOVED;
      goto removed;
    }
    if (unit == BATTLE_UNIT_LIST_END)
      break;
  }
  return -1;

removed:
  BattleEnemy_ReleaseNameLetter(id);
  for (j = 0; j < 20; j++) {
    if (work->actions[j].unitId == id)
      work->actions[j].unitId = BATTLE_UNIT_LIST_END;
  }
  return 0;
}

/// Defeated unit graphics: animation 4 for allies, 5 for enemies.
/// Enemies wait four more frames, leave the formation, then erase pixel by
/// pixel. id: unit ID.
void BattleMotion_ShowDowned(s32 id) {
  void *recs[4];
  struct BattleUnit *unit;
  struct AnimationObject *rec;
  struct AnimationEntry *ent;
  s32 n;

  unit = Unit_Get(id);
  n = 0;
  while ((rec = GetMotionRecord(BattleObject_Get(id)->object, n)) != 0) {
    if (unit->side != BATTLE_UNIT_ENEMY)
      Animation_ApplyChildArgumentFar(rec, 4);
    else
      Animation_ApplyChildArgumentFar(rec, 5);
    n++;
  }

  if (unit->side == BATTLE_UNIT_ENEMY) {
    n = 0;
    while ((rec = GetMotionRecord(BattleObject_Get(id)->object, n)) != 0) {
      /* FAKEMATCH: the native CSE block ends after the child lookup; a one-pass
         block keeps its load before the index shift, which a straight
         assignment reverses. */
      do {
        ent = rec->entries[0];
      } while (0);
      recs[n] = rec;
      ent->param = 6;
      ent->frame = 0xff;
      n++;
    }
    WaitFrames(4);
    BattleActor_RemoveFromLists(id);
    Map_RenderAllAnimatedTilePicturesFar(recs, n); // Erase pixel by pixel.
    BattleObject_MarkForRelease(id);
  }
}
