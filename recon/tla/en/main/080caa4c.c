// Draft, differ 38 of 292, written from the listing in the form of The
// Broken Seal's ObjectEvent_ResetForObject: the leader's placement row from
// gPartyState (and its height when it enters above ground), then the
// scene's table from slot 8, the leader's movement mode (world map, the
// stand-in actor 8 under ownerOverride, climbing where its cell and the one
// north of it have bit 6 of attributeB set), the camera that follows the
// leader or the stand-in, and the world map's sprite metadata. Left,
// instruction order only, about 19 pairs: the native issues pool and memory
// loads and the renderMode store earlier (the gPartyState pool load before
// the currentOwner offset, the placement offset's halves around the load,
// the store of renderMode 0 before the layer is read again), and sets r0
// before r1 for Event_SpawnObjectTable(table, 8).
#include "types.h"
#include "ram_buffer.h"
#include "party_state.h"
#include "event_work.h"
#include "object_runtime.h"
#include "field_scene.h"
#include "map.h"

// The event work as this function reads it: the four event tables it clears
// and the leader's two-row placement table at 0x1ec.
struct EventWorkPlacements {
  const struct ScenePlacement *tables[4];
  u8 unk010[0x1dc];
  struct ScenePlacement leader[2];
};

extern const struct ScenePlacement gData_080f21d4[];

void ObjectTable_Clear(void);
s32 Event_SpawnObjectTable(s32 table, s32 first);
s32 Map_GetTerrainHeightFar(u8 kind, s32 x, s32 z);
void ObjectDispatch_SetSingleChildFlagsFar(struct ObjectRuntime *object,
                                           u32 flags);
struct ObjectRuntime *Func_080200c0(s32 kind, s32 x, s32 y, s32 z);
void ObjectDispatch_StartCameraFar(void *object, void *effect);
u8 *ResourceMetadata_RegisterFar(void *resource, s32 id);

void Func_080caa4c(s32 table) {
  struct EventWork *work;
  struct EventWorkPlacements *placements;
  struct ScenePlacement *entry;
  struct ObjectRuntime *object;
  struct ObjectRuntime *camera;
  struct MapCell *cell;
  struct MapCell *above;
  s32 leader;
  s32 i;
  s32 height;

  work = Ram_HeapSlots->eventWork;
  placements = (struct EventWorkPlacements *)work;
  leader = gPartyState.currentOwner;
  entry = placements->leader;
  entry[0] = gData_080f21d4[0];
  entry[1] = gData_080f21d4[1];
  for (i = 0; i < 4; i++)
    placements->tables[i] = 0;
  ObjectTable_Clear();
  entry->condition = -1;
  entry->sprite = leader;
  entry->x = gPartyState.x;
  entry->y = 0;
  entry->z = gPartyState.z;
  entry->facing = gPartyState.heading;
  Event_SpawnObjectTable((s32)entry, leader);
  {
    s32 y = gPartyState.y;

    if (y != 0) {
      object = work->objects[leader];
      object->y = y;
      object->terrainHeight = y;
    }
  }
  Event_SpawnObjectTable(table, 8);

  object = work->objects[leader];
  object->terrainId = gPartyState.turn;
  object->action = 0;
  object->unk66 = 0;
  if (work->controlMode == 3) {
    if (gPartyState.ownerOverride != 0)
      gPartyState.renderMode = 7;
    else
      gPartyState.renderMode = 1;
  } else if (gPartyState.ownerOverride != 0) {
    gPartyState.renderMode = 6;
  } else {
    gPartyState.renderMode = 0;
    cell = Ram_HeapSlots->mapWork->layers[object->terrainId].cells +
           ((object->x / 0x100000) + ((object->z / 0x100000) << 7));
    above = cell - 128;
    if (gPartyState.y != 0 && (cell->attributeB & 0x40) &&
        (above->attributeB & 0x40)) {
      gPartyState.renderMode = 2;
      height = Map_GetTerrainHeightFar(0, object->x, object->z - 0x100000) -
               0x200000;
      object->y += height;
      object->terrainHeight = object->y;
      object->motionFlags = 0;
      ObjectDispatch_SetSingleChildFlagsFar(object, 0);
      Object_SetMode(object, 12);
    }
  }

  if (gPartyState.ownerOverride != 0) {
    struct ObjectRuntime *stand = work->objects[8];

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
  ObjectDispatch_StartCameraFar(camera, object);
  if (work->controlMode == 3) {
    u8 *effect = ResourceMetadata_RegisterFar(object->animation, 266);

    effect[5] = 15;
    effect[6] = 9;
  }
  Ram_HeapSlots->mapWork->origin = &camera->x;
  work->viewCenter = camera;
}
