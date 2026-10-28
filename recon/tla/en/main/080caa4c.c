// Draft, differ 16 of 292 (2026-10-26, 26 before): renderMode 0 stored through
// a byte pointer, so the layer's read stays after it as in the native. Left:
// the opening table's offset halves around its stores, and two pool and
// field loads one row apart. Earlier notes follow.
// Under -mthumb-slow-loads (2026-10-26): differ 26 of 292 in tla-en,
// instruction order and register choice. Empty asms before `entry->sprite =
// leader;` and the cell's load lower it to 14.
// Draft, differ 36 of 292 (38 before Event_SpawnObjectTable was declared void,
// as its listing returns nothing), written from the listing in the form of The
// Broken Seal's ObjectEvent_ResetForObject: the leader's placement row from
// gSequenceParentWork (and its height when it enters above ground), then the
// scene's table from slot 8, the leader's movement mode (world map, the
// stand-in actor 8 under ownerOverride, climbing where its cell and the one
// north of it have bit 6 of attributeB set), the camera that follows the
// leader or the stand-in, and the world map's sprite metadata. Left,
// instruction order only, about 19 pairs: the native issues pool and memory
// loads and the renderMode store earlier (the gSequenceParentWork pool load before
// the currentOwner offset, the placement offset's halves around the load,
// the store of renderMode 0 before the layer is read again), and sets r0
// before r1 for Event_SpawnObjectTable(table, 8).
#include "types.h"
#include "ram_buffer.h"
#include "sequence_parent_work.h"
#include "event_work.h"
#include "actor.h"
#include "scene_tables.h"
#include "map.h"

// The event work as this function reads it: the four event tables it clears
// and the leader's two-row placement table at 0x1ec.
struct EventWorkPlacements {
  const struct ActorPlacement *tables[4];
  u8 unk010[0x1dc];
  struct ActorPlacement leader[2];
};

extern const struct ActorPlacement gData_080f21d4[];

void ObjectTable_Clear(void);
void Event_SpawnObjectTable(s32 table, s32 first);
s32 Map_GetTerrainHeightFar(u8 kind, s32 x, s32 z);
void Actor_SetSingleChildFlagsFar(struct Actor *object,
                                           u32 flags);
struct Actor *Func_080200c0(s32 kind, s32 x, s32 y, s32 z);
void Actor_StartCameraFar(void *object, void *effect);
u8 *ResourceMetadata_RegisterFar(void *resource, s32 id);

void Func_080caa4c(s32 table) {
  struct EventWork *work;
  struct EventWorkPlacements *placements;
  struct ActorPlacement *entry;
  struct Actor *object;
  struct Actor *camera;
  struct MapCell *cell;
  struct MapCell *above;
  s32 leader;
  s32 i;
  s32 height;

  work = Ram_HeapSlots->eventWork;
  placements = (struct EventWorkPlacements *)work;
  leader = gSequenceParentWork.currentOwner;
  entry = placements->leader;
  entry[0] = gData_080f21d4[0];
  entry[1] = gData_080f21d4[1];
  for (i = 0; i < 4; i++)
    placements->tables[i] = 0;
  ObjectTable_Clear();
  entry->condition = -1;
  entry->sprite = leader;
  entry->x = gSequenceParentWork.x;
  entry->y = 0;
  entry->z = gSequenceParentWork.z;
  entry->facing = gSequenceParentWork.heading;
  Event_SpawnObjectTable((s32)entry, leader);
  {
    s32 y = gSequenceParentWork.y;

    if (y != 0) {
      object = work->objects[leader];
      object->y = y;
      object->terrainHeight = y;
    }
  }
  Event_SpawnObjectTable(table, 8);

  object = work->objects[leader];
  object->terrainId = gSequenceParentWork.turn;
  object->action = 0;
  object->unk66 = 0;
  if (work->controlMode == 3) {
    if (gSequenceParentWork.ownerOverride != 0)
      gSequenceParentWork.renderMode = 7;
    else
      gSequenceParentWork.renderMode = 1;
  } else if (gSequenceParentWork.ownerOverride != 0) {
    gSequenceParentWork.renderMode = 6;
  } else {
    // Stored through a byte pointer, which may alias the terrain layer's load
    // after it; as a plain field store that load moves above it (differ 26).
    *(u8 *)&gSequenceParentWork.renderMode = 0;
    cell = Ram_HeapSlots->mapWork->layers[object->terrainId].cells +
           ((object->x / 0x100000) + ((object->z / 0x100000) << 7));
    above = cell - 128;
    if (gSequenceParentWork.y != 0 && (cell->attributeB & 0x40) &&
        (above->attributeB & 0x40)) {
      gSequenceParentWork.renderMode = 2;
      height = Map_GetTerrainHeightFar(0, object->x, object->z - 0x100000) -
               0x200000;
      object->y += height;
      object->terrainHeight = object->y;
      object->motionFlags = 0;
      Actor_SetSingleChildFlagsFar(object, 0);
      Actor_SetMode(object, 12);
    }
  }

  if (gSequenceParentWork.ownerOverride != 0) {
    struct Actor *stand = work->objects[8];

    stand->x = object->x;
    stand->y = object->y;
    stand->z = object->z;
    stand->terrainHeight = object->terrainHeight;
    stand->angle = object->angle;
    object->x = 0;
    object->z = 0;
    object = stand;
  }

  camera = Func_080200c0(0x8000, object->x, object->y, object->z);
  camera->terrainHeight = object->terrainHeight;
  Actor_StartCameraFar(camera, object);
  if (work->controlMode == 3) {
    u8 *effect = ResourceMetadata_RegisterFar(object->animation, 266);

    effect[5] = 15;
    effect[6] = 9;
  }
  Ram_HeapSlots->mapWork->origin = &camera->x;
  work->viewCenter = camera;
}
