// Under -mthumb-slow-loads (2026-10-26): differ 8 of 311 in tla-en, instruction
// order and register choice. The same temporary as Func_080d50f8's (ground)
// gives the native's registers (24 before). Left: as in Func_080d50f8, plus the
// gAdventure pool load before the action flags' store, which an empty asm
// before Actor_SetPositionAndCommit fixes (differ 6).
// Near miss, differ 34 of 311: The Broken Seal twin (Field_ClimbUp), with this
// game's checks. On the ground, when the leader's map cell and the one north
// of it are both climbable (attribute 0x40), the cell above is not a wall
// (255), the ground rises north and nothing is in the way, the leader steps
// to the middle of the tile, crouches, jumps and grabs the wall (pose 12).
// While climbing, the leader climbs over the top onto the ground if nothing
// stands there.
// Returns 0, or -1 when there is nothing to climb or the way is blocked.
// Sprite flags are s16 as in the twin: the native spills them as words and
// loads the shared 1 from the pool (s32 differs 95); the render mode is
// cleared before the action flags so its zero is the shared one, not a pool
// load (differ 82 the other way), and the count starts before the object
// table is loaded. Left: as in 080d50f8, the leader cell's address takes r2
// where the native has r3 and the bases of the cell tests swap, and a few
// one-slot moves.
#include "types.h"
#include "map.h"
#include "actor.h"
#include "adventure.h"
#include "ram_buffer.h"

struct ClimbSprite {
  u8 unk00[26];
  u8 flags;
};

struct Actor *Actor_GetById(s32 id);
void Event_Start(void);
void Event_End(void);
void Event_WaitFrames(s32 frames);
void WaitFrames(s32 frames);
s32 Map_GetTerrainHeightFar(s32 layer, s32 x, s32 z);
s32 Func_08020258(s32 *position);
s32 Field_CheckSpheresOverlapFar(s32 *position, s32 radius, s32 *other,
                                  s32 otherRadius);
void Actor_SetPositionAndCommit(s32 id, s32 x, s32 z);
void Actor_WaitForMoveById(s32 id);
void Actor_SetSingleChildFlagsFar(struct Actor *object,
                                           s32 flags);

s32 Func_080d53b8(void) {
  struct Actor *leader = Actor_GetById(gAdventure.currentOwner);
  struct MapWork *map = Ram_HeapSlots->mapWork;
  s16 spriteFlags = 1;
  struct MapCell *ground;
  s32 gridX = 8 + ((leader->x >> 16) & 0xfff0);
  s32 gridY = 8 + ((leader->y >> 16) & ~15);
  s32 gridZ = 8 + ((leader->z >> 16) & 0xfff0);
  s32 position[3];
  s32 result;

  Event_Start();
  if (leader->animationKind == 1)
    spriteFlags = ((struct ClimbSprite *)leader->animation)->flags;
  if (gAdventure.renderMode == 0) {
    struct MapCell *cell;
    struct MapCell *north;
    s32 cellX = gridX >> 4;
    s32 cellY = gridY >> 4;
    s32 cellZ = gridZ >> 4;

    ground = &map->layers[leader->terrainId].cells[cellX + cellZ * 128];
    cell = ground;
    north = cell - 128;
    if (!(cell->attributeB & 0x40) || !(north->attributeB & 0x40))
      goto failure;
    cell = &map->layers[2].cells[cellX + (cellZ - cellY) * 128];
    north = cell - 128;
    if (north->collisionCode == 255)
      goto failure;
    if (Map_GetTerrainHeightFar(leader->terrainId, gridX << 16, gridZ << 16) >=
        Map_GetTerrainHeightFar(leader->terrainId, gridX << 16,
                                (gridZ - 16) << 16))
      goto failure;
    position[0] = leader->x;
    position[1] = leader->y;
    position[2] = leader->z;
    result = Func_08020258(position);
    if (result != 0)
      goto failure;

    // Step to the middle of the tile, crouch, jump and grab the wall.
    leader->actionFlags = result;
    Actor_SetPositionAndCommit(gAdventure.currentOwner, gridX, gridZ);
    Actor_SetMode(leader, 6);
    WaitFrames(4);
    Actor_SetMode(leader, 7);
    leader->velocityY = 0x40000;
    WaitFrames(4);
    leader->motionFlags = result;
    spriteFlags &= 0xfe;
    Actor_SetSingleChildFlagsFar(leader, spriteFlags);
    leader->speedLimit = 0x10000;
    leader->velocityY = result;
    Actor_SetMode(leader, 12);
    WaitFrames(4);
    gAdventure.renderMode = 2;
    leader->actionFlags = 1;
    WaitFrames(8);
    leader->z = (leader->z & 0xfff00000) + 0x80000;
  } else {
    struct Actor *entry;
    s32 radius = leader->radius - 2;
    s32 i;

    position[0] = leader->x;
    position[1] = leader->y + 0x100000;
    position[2] = leader->z - 0x100000;
    i = 0;
    entry = Ram_HeapSlots->scriptObjects;
    for (; i < 64; i++, entry++) {
      if (entry->script != NULL && (entry->collisionFlags & 1) &&
          entry != leader &&
          Field_CheckSpheresOverlapFar(&entry->x, radius, position,
                                        entry->radius - 2) >= 0)
        goto failure;
    }

    // Climb over the top: up 8 pixels and a tile north.
    leader->motionFlags = 0;
    Actor_SetMode(leader, 11);
    Actor_SetTargetPosition(leader, gridX << 16, leader->y + 0x80000,
                       (gridZ << 16) - 0x100000);
    Actor_WaitForMoveById(gAdventure.currentOwner);
    leader->motionFlags = 3;
    leader->terrainHeight = leader->y;
    spriteFlags |= 1;
    Actor_SetSingleChildFlagsFar(leader, spriteFlags);
    Event_WaitFrames(4);
    gAdventure.renderMode = 0;
    leader->actionFlags = 1;
    leader->action = 0;
    leader->unk66 = 0;
  }
  Event_End();
  return 0;

failure:
  Event_End();
  return -1;
}
