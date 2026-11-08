// Differ 2 of 104 (tla-en, 2026-11-08, -mtune=gba). Round two, no gain: the reset as its own
// once-only block before the loop (2), ++actors, an index or &list[0] spelling, map assigned late,
// cell assigned apart (all 2); the cursor a separate local from a renamed argument (6); an if on
// list->actor around a do-while (21); permute 8 min: nothing. Round one, no gain: a while loop,
// the reset in its own once-only block, the map read late, list assigned late or inside the for (4), a
// u16 flag, the 0xffff end as a local (23), the actor id as a local (34); permute 8 min: nothing better. The list is pinned to r9
// (tagged pin: unpinned, 25 of 104, re-measured 2026-11-08); an if around a do-while, a
// for with no init and a test of the list as well all measure 4 to 44.
// 2026-11-07: the argument itself is the cursor, reset from the list in a
// once-only block (6 -> 2); what remains is the first test, which the native
// reads from the list in r9 (`mov r2, r9`) and this from the cursor in r8,
// one row later. Permute 9 min from 2: nothing better. Spelt as an if around
// a do-while, a goto loop or an index, 14 to 28.
// Earlier: the list's start is fixed in r9, as the native keeps it for
// Func_080eaa14, which also puts the map work in r10 (25 -> 6). The terrain
// store order (y first, then the terrain height) is the native's.
// Puts a list of actors back on the map after a battle effect. For
// each (actor, flag) entry until 0xffff it clears the actor's motion flags
// and collision bit 0, shows its sprite, marks its map cell entry (slot from
// Func_080eab70 in the layer's cell table) with 0xff, and sets mode 0 (clearing
// the flag) when the flag is set, mode 1 otherwise. Func_080eaa14 then takes
// the same list, and the leader is lifted onto the terrain if below it.
#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
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

struct Actor *Actor_GetById(u32 actor);
s32 Func_080eab70(struct Actor *object);
void Actor_SetSingleChildFlagsFar(struct Actor *object, s32 flags);
s32 GameFlag_Test(s32 flag);
void GameFlag_ClearBit(s32 flag);
void Func_080eaa14(struct ActorFlag *list);
s32 Map_GetTerrainHeightFar(s32 layer, s32 x, s32 z);

void Func_080ea7c0(struct ActorFlag *actors) {
  /* FAKEMATCH(pin): the form kept; as a plain local, 25 of 104 instructions differ in tla-en. */
  register struct ActorFlag *list asm("r9") = actors;
  struct RestoreMap *map = (struct RestoreMap *)Ram_HeapSlots->mapWork;
  struct Actor *leader;
  s32 height;

  // A once-only block: as a plain loop, 4 of 104 differ.
  do {
    for (actors = list; actors->actor != 0xffff; actors++) {
      struct Actor *object = Actor_GetById(actors->actor);
      u16 flag = actors->flag;
      s32 slot = Func_080eab70(object);
      u8 *cell = map->layers[object->terrainId].cells[slot];

      object->motionFlags = 0;
      object->collisionFlags &= ~1;
      Actor_SetSingleChildFlagsFar(object, 0);
      cell[2] = 0xff;
      if (GameFlag_Test(flag)) {
        Actor_SetMode(object, 0);
        GameFlag_ClearBit(flag);
      } else {
        Actor_SetMode(object, 1);
      }
    }
  } while (0);
  Func_080eaa14(list);
  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  height = Map_GetTerrainHeightFar(leader->terrainId, leader->x, leader->z);
  if (leader->y < height)
    leader->y = leader->terrainHeight = height;
}
