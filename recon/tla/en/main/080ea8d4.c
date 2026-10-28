// Measured 2026-10-26 under the Makefile's options (-mthumb-slow-loads), differ
// 12 of 141 in tla-en. First difference: this issues `sub sp, #4` earlier (row
// 8) than the native does (row 11).
// Near miss, differ 14 of 141 (2026-10-19; 26 before). The native spills
// the list's start (sp 0) and keeps the map work in r11 and the 0x100000
// step in r9, set after the first test: both are fixed here, the step set
// in an if around the loop (26 -> 18; the plain for loop sets it before
// the test, 52), with x - 0x100000 left to the pool as the native has it
// (26 with the step). The owner's y is stored before its terrain height
// (16) and the actor's x is read before its cell is marked (14). What
// remains: the prologue (the native subtracts sp after its first movs and
// spills the list after the map load), the 0xffff's order around the first
// test, and the map's copy taking r1 where the native takes r2.
// Func_080ea7c0's sibling that also touches the four cells around each
// actor. For each (actor, flag) entry until 0xffff it clears the actor's
// motion flags and collision bit 0, shows its sprite, marks its map cell
// entry with 0xff, runs Func_080ea8a8 on the cells one unit east, west, south
// and north of it, and sets mode 0 (clearing the flag) when the flag is set,
// mode 1 otherwise. Func_080eaa14 then takes the same list, and the
// controlled owner is set onto the terrain when Func_08020310 asks for it.
#include "types.h"
#include "object_runtime.h"
#include "adventure.h"
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
s32 Party_GetLeaderActor(void);
void Func_080ea8a8(s32 x, s32 z, s32 layer, s32 height);
s32 Func_08020310(s32 x, s32 z, s32 layer, s32 height);

void Func_080ea8d4(struct ActorFlag *list) {
  register struct RestoreMap *map asm("r11") =
      (struct RestoreMap *)Ram_HeapSlots->mapWork;
  struct ActorFlag *entry;
  struct ObjectRuntime *owner;
  s32 x;
  register s32 step asm("r9");

  entry = list;
  if (entry->actor != 0xffff) {
    step = 0x100000;
    do {
      struct ObjectRuntime *object = Object_GetById(entry->actor);
      u16 flag = entry->flag;
      s32 slot = Func_080eab70(object);
      u8 *cell = map->layers[object->terrainId].cells[slot];

      object->motionFlags = 0;
      object->collisionFlags &= ~1;
      ObjectDispatch_SetSingleChildFlagsFar(object, 0);
      x = object->x;
      cell[2] = 0xff;
      Func_080ea8a8(x + step, object->z, object->terrainId,
                    object->terrainHeight);
      Func_080ea8a8(object->x - 0x100000, object->z, object->terrainId,
                    object->terrainHeight);
      Func_080ea8a8(object->x, object->z + step, object->terrainId,
                    object->terrainHeight);
      Func_080ea8a8(object->x, object->z - 0x100000, object->terrainId,
                    object->terrainHeight);
      if (GameFlag_Test(flag)) {
        Object_SetMode(object, 0);
        GameFlag_ClearBit(flag);
      } else {
        Object_SetMode(object, 1);
      }
      entry++;
    } while (entry->actor != 0xffff);
  }
  Func_080eaa14(list);
  owner = Object_GetById(Party_GetLeaderActor());
  if (Func_08020310(owner->x, owner->z, owner->terrainId, owner->terrainHeight))
    owner->terrainHeight = owner->y =
        Map_GetTerrainHeightFar(owner->terrainId, owner->x, owner->z);
}
