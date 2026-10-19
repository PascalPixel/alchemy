// Near miss, differ 6 of 104 (2026-10-19; 25 before). The list's start is
// fixed in r9, as the native keeps it for Func_080eaa14, which also puts the
// map work in r10 (25 -> 6). What remains is the opening's copies: the
// native copies the argument to the cursor in r8, the cursor to r9 and r9
// to r2 for the first test; this copies it to r9, then r8, then r2. With
// the cursor fixed in r8 too and the list copied from it, 10 (the first
// test then reads r8); with an if around a do-while testing the list, 8.
// Permute 4 min: 6 at best. Iterating from an initialised entry pointer
// measured the same as the for loop. The terrain store order
// (y first, then the terrain height) is the native's.
// Puts a list of actors back on the map after a battle effect. For
// each (actor, flag) entry until 0xffff it clears the actor's motion flags
// and collision bit 0, shows its sprite, marks its map cell entry (slot from
// Func_080eab70 in the layer's cell table) with 0xff, and sets mode 0 (clearing
// the flag) when the flag is set, mode 1 otherwise. Func_080eaa14 then takes
// the same list, and the leader is lifted onto the terrain if below it.
#include "types.h"
#include "object_runtime.h"
#include "party_state.h"
#include "ram_buffer.h"

struct ActorFlag {
  u16 actor;
  s16 flag;
};

struct RestoreLayer {
  u8 unk00[0x138 - 0x108];
  u8 (*cells)[4];
  u8 unk34[4];
};

struct RestoreMap {
  u8 unk000[0x108];
  struct RestoreLayer layers[1];
};

struct ObjectRuntime *Object_GetById(u32 actor);
s32 Func_080eab70(struct ObjectRuntime *object);
void ObjectDispatch_SetSingleChildFlagsFar(struct ObjectRuntime *object,
                                           s32 flags);
s32 GameFlag_Test(s32 flag);
void GameFlag_ClearBit(s32 flag);
void Func_080eaa14(struct ActorFlag *list);
s32 Map_GetTerrainHeightFar(s32 layer, s32 x, s32 z);

void Func_080ea7c0(struct ActorFlag *actors) {
  register struct ActorFlag *list asm("r9") = actors;
  struct RestoreMap *map = (struct RestoreMap *)Ram_HeapSlots->mapWork;
  struct ActorFlag *entry;
  struct ObjectRuntime *leader;
  s32 height;

  for (entry = list; entry->actor != 0xffff; entry++) {
    struct ObjectRuntime *object = Object_GetById(entry->actor);
    u16 flag = entry->flag;
    s32 slot = Func_080eab70(object);
    u8 *cell = map->layers[object->terrainId].cells[slot];

    object->motionFlags = 0;
    object->collisionFlags &= ~1;
    ObjectDispatch_SetSingleChildFlagsFar(object, 0);
    cell[2] = 0xff;
    if (GameFlag_Test(flag)) {
      Object_SetMode(object, 0);
      GameFlag_ClearBit(flag);
    } else {
      Object_SetMode(object, 1);
    }
  }
  Func_080eaa14(list);
  leader = Object_GetById(gPartyState.currentOwner);
  height = Map_GetTerrainHeightFar(leader->terrainId, leader->x, leader->z);
  if (leader->y < height)
    leader->y = leader->terrainHeight = height;
}
