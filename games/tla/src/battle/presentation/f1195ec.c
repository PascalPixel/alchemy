#include "types.h"
#include "battle.h"
#include "battle_work.h"
#include "battle_session.h"
#include "motobj.h"
#include "ram_buffer.h"
#include "main_routine_work.h"

s32 Func_0811a24c(s32 side, s32 value);

void Func_081195ec(void) {
  u16 actors[6];
  u16 reserves[6];
  s32 actorCount;
  s32 reserveCount;
  s32 count;
  struct BattleWork *work = Ram_HeapSlots->battleWork;
  s32 i;

  if (Func_0811a24c(1, 0) == 0)
    return;
  count = actorCount = BattleParty_ListActorIds(1, actors);
  reserveCount = BattleParty_PrepareReserveOwners(reserves);
  if (count > reserveCount)
    count = reserveCount;
  BattleHighlight_SetModes(actors, 4);
  wait(32);
  for (i = 0; i < count; i++) {
    s32 unit = work->partyUnits[i];
    s32 id = reserves[i];
    struct BattleObj *from = BattleObject_Get(unit);
    struct BattleObj *to = BattleObject_Get(id);
    struct MotionObject *object = to->object;
    struct MotionObject *place = from->object;

    to->anchorX = from->anchorX;
    to->anchorZ = from->anchorZ;
    Actor_SetPositionAndResetMotionFar((void *)object, place->x, place->y,
                                        place->z);
    gMainRoutineWork.activeOwners[i] = id;
    work->partyUnits[i] = id;
  }
  for (i = 0; i < count; i++)
    gMainRoutineWork.activeOwners[actorCount + i] = actors[i];
  PartyStatus_DrawFar(Ram_HeapSlots->battleWork->partyStatus);
  BattleObject_Put();
  BattleHighlight_SetModes(reserves, 6);
  wait(32);
}
