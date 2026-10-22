// Near miss, differ 4 of 122 (two reordered rows): swaps the party's front
// line for its reserves in battle: lists the actors and the reserve owners,
// sends the actors out (mode 4), puts each reserve's object where the actor
// it replaces stood, rewrites the active owners (reserves first, then the
// old actors) and the battle's party list, redraws the status and brings the
// reserves in (mode 6). No ☀️ twin. The battle work declared after the
// counts puts the spills in the native's slots (permute), and both ids read
// before the lookups load the reserve id before the first BattleObject_Get
// (14 to 6 to 4). Left: the native copies the object into r0 (mov r0, ip)
// before it loads the position's three words, and builds the owner step 1
// before it reloads the party list's offset (ldr r1, [sp, #8]). Tried: the
// position in locals (4), a barrier after the call (6), four minutes of
// permute (no gain).
#include "types.h"
#include "battle.h"
#include "battle_work.h"
#include "battle_session.h"
#include "motobj.h"
#include "ram_buffer.h"
#include "party_state.h"

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
    gPartyState.activeOwners[i] = id;
    work->partyUnits[i] = id;
  }
  for (i = 0; i < count; i++)
    gPartyState.activeOwners[actorCount + i] = actors[i];
  PartyStatusDrawFar(Ram_HeapSlots->battleWork->partyStatus);
  BattleObject_Put();
  BattlePres_SetActorModes(reserves, 6);
  WaitFrames(32);
}
