#include "types.h"
#include "battle.h"
#include "battle_work.h"
#include "battle_session.h"
#include "motobj.h"
#include "ram_buffer.h"
#include "adventure.h"

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
  BattlePres_SetActorModes(actors, 4);
  WaitFrames(32);
  for (i = 0; i < count; i++) {
    s32 unit = work->partyUnits[i];
    s32 id = reserves[i];
    struct BattleObj *from = BattleObject_Get(unit);
    struct BattleObj *to = BattleObject_Get(id);
    struct MotionObject *object = to->object;
    struct MotionObject *place = from->object;

    to->anchorX = from->anchorX;
    to->anchorZ = from->anchorZ;
    Object_SetPositionAndResetMotionFar((void *)object, place->x, place->y,
                                        place->z);
    gAdventure.activeOwners[i] = id;
    work->partyUnits[i] = id;
  }
  for (i = 0; i < count; i++)
    gAdventure.activeOwners[actorCount + i] = actors[i];
  PartyStatus_DrawFar(Ram_HeapSlots->battleWork->partyStatus);
  BattleObject_Put();
  BattlePres_SetActorModes(reserves, 6);
  WaitFrames(32);
}
