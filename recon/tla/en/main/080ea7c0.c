// Matches: differ 0 (tla-en, 2026-11-08)
// Plain C, no device, read through map.h's MapWork, MapLayer and MapCell and
// the headers' prototypes: the map is read before the list's start is saved, so
// CSE keeps the argument as the cursor and reads the first test from the start,
// as the native does. With the copy straight after the argument's, CSE swaps
// the two registers (25); with the start pinned to r9, the old form, the first
// test read the cursor (2). All six editions differ 0; de, es and fr place it
// elsewhere, so measure them with
// --symbol Func_080ea7c0.
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
#include "map.h"
#include "actor_view.h"
#include "field/field.h"

struct ActorFlag {
  u16 actor;
  s16 flag;
};

struct Actor *Actor_GetById(u32 actor);
s32 Func_080eab70(struct Actor *object);
s32 GameFlag_Test(s32 flag);
void GameFlag_ClearBit(s32 flag);
void Func_080eaa14(struct ActorFlag *list);

void Func_080ea7c0(struct ActorFlag *actors) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  struct ActorFlag *list = actors;
  struct Actor *leader;
  s32 height;

  for (; actors->actor != 0xffff; actors++) {
    struct Actor *object = Actor_GetById(actors->actor);
    u16 flag = actors->flag;
    s32 slot = Func_080eab70(object);
    struct MapCell *cell = &map->layers[object->terrainId].cells[slot];

    object->motionFlags = 0;
    object->collisionFlags &= ~1;
    Actor_SetSingleChildFlagsFar((struct ActorView *)object, 0);
    cell->collisionCode = 0xff;
    if (GameFlag_Test(flag)) {
      Actor_SetMode(object, 0);
      GameFlag_ClearBit(flag);
    } else {
      Actor_SetMode(object, 1);
    }
  }
  Func_080eaa14(list);
  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  height = Map_GetTerrainHeightFar(leader->terrainId, leader->x, leader->z);
  if (leader->y < height)
    leader->y = leader->terrainHeight = height;
}
