// Near miss, differ 30 of 324: The Broken Seal twin (Field_ClimbDown), with
// this game's checks. On the ground, when the leader's map cell and the one
// south of it are both climbable (attribute 0x40), the cell below is not a
// wall (255), the ground drops away south and nothing stands in the way, the
// leader steps to the middle of the tile, turns round and climbs down onto
// the wall (pose 13). While climbing, the leader lets go and falls. Returns
// 0, or -1 when there is nothing to climb or the way is blocked. Sprite flags
// are s16 as in the twin (the native spills them as words and loads the
// shared 1 from the pool), declared after the x grid so they take the lower
// stack slot. Left: the leader cell's address takes r2 where the native has
// r3 (and the bases of the two cell tests swap r1 and r3), and a few
// one-slot moves.
#include "types.h"
#include "map.h"
#include "object_runtime.h"
#include "party_state.h"
#include "ram_buffer.h"

struct ClimbSprite {
  u8 unk00[26];
  u8 flags;
};

struct ObjectRuntime *Object_GetById(s32 id);
void EventRuntime_Start(void);
void EventRuntime_End(void);
void EventRuntime_Wait(s32 frames);
s32 Map_GetTerrainHeightFar(s32 layer, s32 x, s32 z);
s32 Func_08020258(s32 *position);
s32 Runtime_CheckRadiusOverlapFar(s32 *position, s32 radius, s32 *other,
                                  s32 otherRadius);
void ObjectMotion_SetPositionAndCommit(s32 id, s32 x, s32 z);
void ObjectMotion_ArmCallback(s32 id, s32 angle, s32 frames);
void ObjectMotion_CommitCurrentPositionAndActivate(s32 id);
void ObjectDispatch_SetSingleChildFlagsFar(struct ObjectRuntime *object,
                                           s32 flags);

s32 Func_080d50f8(void) {
  struct ObjectRuntime *leader = Object_GetById(gPartyState.currentOwner);
  struct MapWork *map = Ram_HeapSlots->mapWork;
  s32 gridX;
  s16 spriteFlags;
  s32 gridY;
  s32 gridZ;
  s32 position[3];

  spriteFlags = 1;
  gridX = 8 + ((leader->x >> 16) & 0xfff0);
  gridY = 8 + ((leader->y >> 16) & 0xfff0);
  gridZ = 8 + ((leader->z >> 16) & 0xfff0);
  EventRuntime_Start();
  if (leader->animationKind == 1)
    spriteFlags = ((struct ClimbSprite *)leader->animation)->flags;
  if (gPartyState.renderMode == 0) {
    struct ObjectRuntime *entry;
    struct MapCell *cell;
    struct MapCell *south;
    s32 cellX = gridX / 16;
    s32 cellY = gridY / 16;
    s32 cellZ = gridZ / 16;
    s32 radius;
    s32 i;

    cell = &map->layers[leader->terrainId].cells[cellX + cellZ * 128];
    south = cell + 128;
    if (!(cell->attributeB & 0x40) || !(south->attributeB & 0x40))
      goto failure;
    cell = &map->layers[2].cells[cellX + (cellZ - cellY) * 128];
    south = cell + 128;
    if (south->collisionCode == 255)
      goto failure;
    if (Map_GetTerrainHeightFar(leader->terrainId, gridX << 16, gridZ << 16) <=
        Map_GetTerrainHeightFar(leader->terrainId, gridX << 16,
                                (gridZ + 16) << 16))
      goto failure;
    position[0] = leader->x;
    position[1] = leader->y - 0x100000;
    position[2] = leader->z;
    if (Func_08020258(position) != 0)
      goto failure;
    radius = leader->radius - 2;
    position[0] = leader->x;
    position[1] = leader->y - 0x200000;
    position[2] = leader->z + 0x100000;
    entry = Ram_HeapSlots->scriptObjects;
    for (i = 0; i < 64; i++, entry++) {
      if (entry->script != NULL && (entry->collisionFlags & 1) &&
          entry != leader &&
          Runtime_CheckRadiusOverlapFar(&entry->x, radius, position,
                                        entry->radius - 2) >= 0)
        goto failure;
    }

    // Step to the middle of the tile, face north and climb down a tile.
    ObjectMotion_SetPositionAndCommit(gPartyState.currentOwner, gridX, gridZ);
    leader->speedLimit = 0x10000;
    ObjectMotion_ArmCallback(gPartyState.currentOwner, 0xc000, 0);
    Object_RefreshSelectorById(gPartyState.currentOwner);
    leader->actionFlags = 1;
    leader->motionFlags = 0;
    spriteFlags &= 0xfe;
    ObjectDispatch_SetSingleChildFlagsFar(leader, spriteFlags);
    Object_SetMode(leader, 13);
    Object_SetPosition(leader, gridX << 16, leader->y - 0x100000,
                       (gridZ << 16) + 0x100000);
    ObjectMotion_CommitCurrentPositionAndActivate(gPartyState.currentOwner);
    gPartyState.renderMode = 2;
    leader->z = (leader->z & 0xfff00000) + 0x80000;
  } else {
    // Let go of the wall and fall.
    Object_SetMode(leader, 10);
    leader->motionFlags = 3;
    leader->velocityY = 0x40000;
    leader->terrainHeight = leader->y;
    spriteFlags |= 1;
    ObjectDispatch_SetSingleChildFlagsFar(leader, spriteFlags);
    EventRuntime_Wait(6);
    gPartyState.renderMode = 0;
    leader->actionFlags = 1;
    leader->angle = 0xc000;
    leader->action = 0;
    leader->unk66 = 0;
  }
  EventRuntime_End();
  return 0;

failure:
  EventRuntime_End();
  return -1;
}
