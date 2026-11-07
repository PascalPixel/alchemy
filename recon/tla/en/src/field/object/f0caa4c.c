#include "types.h"
#include "ram_buffer.h"
#include "main_routine_work.h"
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
  leader = gMainRoutineWork.currentOwner;
  entry = placements->leader;
  entry[0] = gData_080f21d4[0];
  entry[1] = gData_080f21d4[1];
  for (i = 0; i < 4; i++)
    placements->tables[i] = 0;
  ObjectTable_Clear();
  entry->condition = -1;
  entry->sprite = leader;
  entry->x = gMainRoutineWork.x;
  entry->y = 0;
  entry->z = gMainRoutineWork.z;
  entry->facing = gMainRoutineWork.heading;
  Event_SpawnObjectTable((s32)entry, leader);
  {
    s32 y = gMainRoutineWork.y;

    if (y != 0) {
      object = work->objects[leader];
      object->y = y;
      object->terrainHeight = y;
    }
  }
  Event_SpawnObjectTable(table, 8);

  object = work->objects[leader];
  object->terrainId = gMainRoutineWork.turn;
  object->action = 0;
  object->unk66 = 0;
  if (work->controlMode == 3) {
    if (gMainRoutineWork.ownerOverride != 0)
      gMainRoutineWork.renderMode = 7;
    else
      gMainRoutineWork.renderMode = 1;
  } else if (gMainRoutineWork.ownerOverride != 0) {
    gMainRoutineWork.renderMode = 6;
  } else {
    // Stored through a byte pointer, which may alias the terrain layer's load
    // after it; as a plain field store that load moves above it (differ 26).
    *(u8 *)&gMainRoutineWork.renderMode = 0;
/* FAKEMATCH(barrier): an empty asm after `*(u8 *)&gMainRoutineWork.renderMode = 0;`; as plain statements, 2 of 292 instructions differ in tla-en. */
asm("");
    cell = Ram_HeapSlots->mapWork->layers[object->terrainId].cells +
           ((object->x / 0x100000) + ((object->z / 0x100000) << 7));
    above = cell - 128;
    if (gMainRoutineWork.y != 0 && (cell->attributeB & 0x40) &&
        (above->attributeB & 0x40)) {
      gMainRoutineWork.renderMode = 2;
      height = Map_GetTerrainHeightFar(0, object->x, object->z - 0x100000) -
               0x200000;
      object->y += height;
      object->terrainHeight = object->y;
      object->motionFlags = 0;
      Actor_SetSingleChildFlagsFar(object, 0);
      Actor_SetMode(object, 12);
    }
  }

  if (gMainRoutineWork.ownerOverride != 0) {
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
