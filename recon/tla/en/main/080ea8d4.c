// Differ 4 of 141 (tla-en, 2026-11-08, -mtune=gba); the pins re-measured then.
// 8 November: the loop walks the list itself and a copy of its start, taken
// after the map load, is the value spilled to sp 0 (10 -> 4): the prologue
// order (sub sp, the spill after the map load) now matches. The map work is
// struct MapWork from map.h. Left: the map's copy, made before the terrain
// index is computed, takes r1 where the native's late reload takes r2; the
// index spellings tried (a layer local, a terrain local, the cell computed
// after the stores, + slot) give 4 or worse.
// 8 November, later: the map copy comes from expansion (insn 60 copies fp
// before the index). Spellings that use fp directly (map->layers + id,
// byte offsets) let combine fold it early (6 or 8); index casts, a map
// copy local, + slot keep 4; once-only block, early layers local (10); 10
// permuter minutes found nothing.
// Older notes: the cell is marked through struct MapCell; the step is set in
// an if around the loop (the plain for loop sets it before the test) with
// x - 0x100000 left to the pool as the native has it; the owner's y is stored
// before its terrain height.
// Func_080ea7c0's sibling that also touches the four cells around each
// actor. For each (actor, flag) entry until 0xffff it clears the actor's
// motion flags and collision bit 0, shows its sprite, marks its map cell
// entry with 0xff, runs Func_080ea8a8 on the cells one unit east, west, south
// and north of it, and sets mode 0 (clearing the flag) when the flag is set,
// mode 1 otherwise. Func_080eaa14 then takes the same list, and the
// controlled owner is set onto the terrain when Func_08020310 asks for it.
#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "map.h"

struct ActorFlag {
  u16 actor;
  s16 flag;
};

struct Actor *Actor_GetById(u32 actor);
s32 Func_080eab70(struct Actor *object);
void Actor_SetSingleChildFlagsFar(struct Actor *object,
                                           s32 flags);
s32 GameFlag_Test(s32 flag);
void GameFlag_ClearBit(s32 flag);
void Func_080eaa14(struct ActorFlag *list);
s32 Map_GetTerrainHeightFar(s32 layer, s32 x, s32 z);
s32 Party_GetLeaderActor(void);
void Func_080ea8a8(s32 x, s32 z, s32 layer, s32 height);
s32 Func_08020310(s32 x, s32 z, s32 layer, s32 height);

void Func_080ea8d4(struct ActorFlag *list) {
  /* FAKEMATCH(pin): the map work held in r11; as a plain local, 24 of 141
   * instructions differ in tla-en. */
  register struct MapWork *map asm("r11") = Ram_HeapSlots->mapWork;
  struct ActorFlag *first = list;
  struct Actor *owner;
  /* FAKEMATCH(pin): the step held in r9; as a plain local, 33 of 141
   * instructions differ in tla-en. */
  register s32 step asm("r9");

  if (list->actor != 0xffff) {
    step = 0x100000;
    do {
      struct Actor *object = Actor_GetById(list->actor);
      u16 flag = list->flag;
      s32 slot = Func_080eab70(object);
      struct MapCell *cell = &map->layers[object->terrainId].cells[slot];

      object->motionFlags = 0;
      object->collisionFlags &= ~1;
      Actor_SetSingleChildFlagsFar(object, 0);
      cell->collisionCode = 0xff;
      Func_080ea8a8(object->x + step, object->z, object->terrainId,
                    object->terrainHeight);
      Func_080ea8a8(object->x - 0x100000, object->z, object->terrainId,
                    object->terrainHeight);
      Func_080ea8a8(object->x, object->z + step, object->terrainId,
                    object->terrainHeight);
      Func_080ea8a8(object->x, object->z - 0x100000, object->terrainId,
                    object->terrainHeight);
      if (GameFlag_Test(flag)) {
        Actor_SetMode(object, 0);
        GameFlag_ClearBit(flag);
      } else {
        Actor_SetMode(object, 1);
      }
      list++;
    } while (list->actor != 0xffff);
  }
  Func_080eaa14(first);
  owner = Actor_GetById(Party_GetLeaderActor());
  if (Func_08020310(owner->x, owner->z, owner->terrainId, owner->terrainHeight))
    owner->terrainHeight = owner->y =
        Map_GetTerrainHeightFar(owner->terrainId, owner->x, owner->z);
}
