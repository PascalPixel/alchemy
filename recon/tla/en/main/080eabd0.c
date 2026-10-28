// Draft, differ 66 of 125: register choice and order. The native keeps the
// layer's cell table in r10 and the shape row (offset already added) in r5,
// and builds the slot's offset into r8 after Func_080eab98; this keeps the
// shape row in r8 and the cell pointer in r5, so the tail's stores read
// differently. Byte 3 read once into a local measured 66; read for the
// save and again for the |= 0x80 (the store to the word at 76 may alias
// it), 77.
// Raises the map under a list of actors (ids until 0xffff). For each
// it reads the actor's cell key and height, clears its motion and collision
// flags, shows its sprite, gives the cell's shape in gMapShapeGrid
// Func_080eab98's tag four half-units higher, saves the cell entry's byte 3
// in the actor's word at 76 before setting its raised bit, and blocks the
// same slot in layer 2's cell table. The leader is then lifted onto the
// terrain if below it, as in Func_080ea7c0.
#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"

struct SwitchCell {
  u8 unk00[2];
  u8 block;
  u8 unk03_0 : 7;
  u8 raised : 1;
};

struct RestoreLayer {
  u8 unk00[0x138 - 0x108];
  struct SwitchCell *cells;
  u8 unk34[4];
};

struct RestoreMap {
  u8 unk000[0x108];
  struct RestoreLayer layers[3];
};

extern u8 gMapShapeGrid[];

struct Actor *Actor_GetById(u32 actor);
s32 Func_080eab70(struct Actor *object);
u32 Func_080202f0(s32 terrain, s32 x, s32 z);
s32 Map_GetTerrainHeightFar(s32 layer, s32 x, s32 z);
s32 Func_080eab98(s32 unused, s32 key, s32 value);
void Actor_SetSingleChildFlagsFar(struct Actor *object,
                                           s32 flags);

void Func_080eabd0(u16 *actors) {
  struct RestoreMap *map = (struct RestoreMap *)Ram_HeapSlots->mapWork;
  struct SwitchCell *blocks = map->layers[2].cells;
  struct Actor *leader;
  s32 height;

  for (; *actors != 0xffff; actors++) {
    struct Actor *object = Actor_GetById(*actors);
    u32 key = Func_080202f0(object->terrainId, object->x, object->z);
    s32 level =
        Map_GetTerrainHeightFar(object->terrainId, object->x, object->z) >> 19;
    s32 slot = Func_080eab70(object);
    struct SwitchCell *cells = map->layers[object->terrainId].cells;
    u8 *shapes = &gMapShapeGrid[((s32)cells - 0x02010000) >> 2];
    s32 tag;
    u8 flags;

    object->motionFlags = 0;
    object->collisionFlags = 0;
    Actor_SetSingleChildFlagsFar(object, 0);
    tag = Func_080eab98(object->terrainId, key, level + 4);
    flags = ((u8 *)&cells[slot])[3];
    object->unk4c = flags;
    ((u8 *)&cells[slot])[3] = flags | 0x80;
    shapes[slot] = tag;
    blocks[slot].block = 0xff;
  }
  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  height = Map_GetTerrainHeightFar(leader->terrainId, leader->x, leader->z);
  if (leader->y < height)
    leader->y = leader->terrainHeight = height;
}
