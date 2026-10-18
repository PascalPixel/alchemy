// Near miss, differ 26 of 141, register choice: the native spills the
// list's start (sp 0) and keeps the map work in r11 and the 0x100000 step
// in r9; this spills the map work and keeps the step in r11, with a few
// rows of order around the cell lookup.
// Func_080ea7c0's sibling that also touches the four cells around each
// actor. For each (actor, flag) entry until 0xffff it clears the actor's
// motion flags and collision bit 0, shows its sprite, marks its map cell
// entry with 0xff, runs Func_080ea8a8 on the cells one unit east, west, south
// and north of it, and sets mode 0 (clearing the flag) when the flag is set,
// mode 1 otherwise. Func_080eaa14 then takes the same list, and the
// controlled owner is set onto the terrain when Func_08020310 asks for it.
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
s32 EventRuntime_GetControlledOwner(void);
void Func_080ea8a8(s32 x, s32 z, s32 layer, s32 height);
s32 Func_08020310(s32 x, s32 z, s32 layer, s32 height);

void Func_080ea8d4(struct ActorFlag *list) {
  struct RestoreMap *map = (struct RestoreMap *)Ram_HeapSlots->mapWork;
  struct ActorFlag *entry;
  struct ObjectRuntime *owner;

  for (entry = list; entry->actor != 0xffff; entry++) {
    struct ObjectRuntime *object = Object_GetById(entry->actor);
    u16 flag = entry->flag;
    s32 slot = Func_080eab70(object);
    u8 *cell = map->layers[object->terrainId].cells[slot];

    object->motionFlags = 0;
    object->collisionFlags &= ~1;
    ObjectDispatch_SetSingleChildFlagsFar(object, 0);
    cell[2] = 0xff;
    Func_080ea8a8(object->x + 0x100000, object->z, object->terrainId,
                  object->terrainHeight);
    Func_080ea8a8(object->x - 0x100000, object->z, object->terrainId,
                  object->terrainHeight);
    Func_080ea8a8(object->x, object->z + 0x100000, object->terrainId,
                  object->terrainHeight);
    Func_080ea8a8(object->x, object->z - 0x100000, object->terrainId,
                  object->terrainHeight);
    if (GameFlag_Test(flag)) {
      Object_SetMode(object, 0);
      GameFlag_ClearBit(flag);
    } else {
      Object_SetMode(object, 1);
    }
  }
  Func_080eaa14(list);
  owner = Object_GetById(EventRuntime_GetControlledOwner());
  if (Func_08020310(owner->x, owner->z, owner->terrainId,
                    owner->terrainHeight))
    owner->y = owner->terrainHeight =
        Map_GetTerrainHeightFar(owner->terrainId, owner->x, owner->z);
}
