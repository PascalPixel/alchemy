// Draft, differ 14 of 122: swaps the party's front line for its reserves in
// battle: lists the actors and the reserve owners, sends the actors out
// (mode 4), puts each reserve's object where the actor it replaces stood,
// rewrites the active owners (reserves first, then the old actors) and the
// battle's party list, redraws the status and brings the reserves in (mode
// 6). No ☀️ twin. Left: the two spilled values trade stack slots (the battle
// work at sp+12 and the actor count at sp+8 here, the other way natively),
// four rows; the reserve id's load sits after the first BattleObjGet here and
// before it natively (the id read first in the source gives 77), and the
// mov r0, ip one slot later: register choice and order.
#include "types.h"
#include "battle.h"
#include "battle_work.h"
#include "battle_session.h"
#include "motobj.h"
#include "ram_buffer.h"
#include "party_state.h"

s32 Func_0811a24c(s32 side, s32 value);

void Func_081195ec(void) {
  struct BattleWork *work = Ram_HeapSlots->battleWork;
  u16 actors[6];
  u16 reserves[6];
  s32 actorCount;
  s32 reserveCount;
  s32 count;
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
    struct BattleObj *from = BattleObjGet(work->partyUnits[i]);
    s32 id = reserves[i];
    struct BattleObj *to = BattleObjGet(id);
    struct MotionObject *object = to->object;
    struct MotionObject *place = from->object;

    to->anchorX = from->anchorX;
    to->anchorZ = from->anchorZ;
    Object_SetPositionAndResetMotionFar((void *)object, place->x, place->y,
                                        place->z);
    gPartyState.activeOwners[i] = id;
    work->partyUnits[i] = id;
  }
  for (i = 0; i < count; i++)
    gPartyState.activeOwners[actorCount + i] = actors[i];
  PartyStatusDrawFar(Ram_HeapSlots->battleWork->partyStatus);
  BattleObjPut();
  BattlePres_SetActorModes(reserves, 6);
  WaitFrames(32);
}
