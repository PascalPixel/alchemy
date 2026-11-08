// Matches: differ 0 (tla-en, 2026-11-08)
// 8 November, last round: plain C with no device. The step is no variable:
// x + 0x100000 and z + 0x100000 are written as constants, and loop.c hoists
// the shared constant into r9 after the first test while x - 0x100000 stays
// a pool load in the loop, as the native has it. The map work is a plain
// local: as a pseudo it reaches fp and the cell load's base is reloaded late
// into r2, which the r11 pin (a copy made by expansion before the index,
// taking r1) could not give. A for loop, a while loop and an if around a
// do-while all match. Every edition matches (differ 0): ja and it at
// 080ea8d4, de at 080f78d4, es and fr at 080f38c8 and 080ef8c8, each measured
// at its own map's address.
// Older notes: the cell is marked through struct MapCell and the map work is
// struct MapWork from map.h; the owner's y is stored before its terrain
// height; a copy of the list's start is the value spilled to sp 0.
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
void Actor_SetSingleChildFlagsFar(struct Actor *object, s32 flags);
s32 GameFlag_Test(s32 flag);
void GameFlag_ClearBit(s32 flag);
void Func_080eaa14(struct ActorFlag *list);
s32 Map_GetTerrainHeightFar(s32 layer, s32 x, s32 z);
s32 Party_GetLeaderActor(void);
void Func_080ea8a8(s32 x, s32 z, s32 layer, s32 height);
s32 Func_08020310(s32 x, s32 z, s32 layer, s32 height);

void Func_080ea8d4(struct ActorFlag *list) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  struct ActorFlag *first = list;
  struct Actor *owner;

  for (; list->actor != 0xffff; list++) {
    struct Actor *object = Actor_GetById(list->actor);
    u16 flag = list->flag;
    s32 slot = Func_080eab70(object);
    struct MapCell *cell = &map->layers[object->terrainId].cells[slot];

    object->motionFlags = 0;
    object->collisionFlags &= ~1;
    Actor_SetSingleChildFlagsFar(object, 0);
    cell->collisionCode = 0xff;
    Func_080ea8a8(object->x + 0x100000, object->z, object->terrainId,
                  object->terrainHeight);
    Func_080ea8a8(object->x - 0x100000, object->z, object->terrainId,
                  object->terrainHeight);
    Func_080ea8a8(object->x, object->z + 0x100000, object->terrainId,
                  object->terrainHeight);
    Func_080ea8a8(object->x, object->z - 0x100000, object->terrainId,
                  object->terrainHeight);
    if (GameFlag_Test(flag)) {
      Actor_SetMode(object, 0);
      GameFlag_ClearBit(flag);
    } else {
      Actor_SetMode(object, 1);
    }
  }
  Func_080eaa14(first);
  owner = Actor_GetById(Party_GetLeaderActor());
  if (Func_08020310(owner->x, owner->z, owner->terrainId, owner->terrainHeight))
    owner->terrainHeight = owner->y =
        Map_GetTerrainHeightFar(owner->terrainId, owner->x, owner->z);
}
