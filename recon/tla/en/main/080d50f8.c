// Under -mthumb-slow-loads (2026-10-26): differ 6 of 324 in tla-en, instruction
// order only. The leader's map cell goes through a function-scope temporary
// (ground), declared between the sprite flags and gridY, which gives the
// native's registers (24 before; found by permute; a FAKEMATCH(temporary) to
// tag on adoption). Left: the leader's copy (adds r7, r0) comes after the two
// stack spills in the native, the 8 of gridX is built earlier, and for
// Actor_SetSingleChildFlagsFar the native reloads the sprite flags
// into r1 before copying the leader to r0. No single empty asm lowers it.
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
#include "actor.h"
#include "sequence_parent_work.h"
#include "ram_buffer.h"

struct ClimbSprite {
  u8 unk00[26];
  u8 flags;
};

struct Actor *Actor_GetById(s32 id);
void Event_Start(void);
void Event_End(void);
void Event_Wait(s32 frames);
s32 Map_GetTerrainHeightFar(s32 layer, s32 x, s32 z);
s32 Func_08020258(s32 *position);
s32 Field_CheckSpheresOverlapFar(s32 *position, s32 radius, s32 *other,
                                  s32 otherRadius);
void Actor_SetPositionAndCommit(s32 id, s32 x, s32 z);
void Actor_ArmCallback(s32 id, s32 angle, s32 frames);
void Actor_WaitForMoveById(s32 id);
void Actor_SetSingleChildFlagsFar(struct Actor *object,
                                           s32 flags);

s32 Func_080d50f8(void) {
  struct Actor *leader = Actor_GetById(gSequenceParentWork.currentOwner);
  struct MapWork *map = Ram_HeapSlots->mapWork;
  s32 gridX;
  s16 spriteFlags;
  struct MapCell *ground;
  s32 gridY;
  s32 gridZ;
  s32 position[3];

  spriteFlags = 1;
  gridX = 8 + ((leader->x >> 16) & 0xfff0);
  gridY = 8 + ((leader->y >> 16) & 0xfff0);
  gridZ = 8 + ((leader->z >> 16) & 0xfff0);
  Event_Start();
  if (leader->animationKind == 1)
    spriteFlags = ((struct ClimbSprite *)leader->animation)->flags;
  if (gSequenceParentWork.renderMode == 0) {
    struct Actor *entry;
    struct MapCell *cell;
    struct MapCell *south;
    s32 cellX = gridX / 16;
    s32 cellY = gridY / 16;
    s32 cellZ = gridZ / 16;
    s32 radius;
    s32 i;

    ground = &map->layers[leader->terrainId].cells[cellX + cellZ * 128];
    cell = ground;
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
          Field_CheckSpheresOverlapFar(&entry->x, radius, position,
                                        entry->radius - 2) >= 0)
        goto failure;
    }

    // Step to the middle of the tile, face north and climb down a tile.
    Actor_SetPositionAndCommit(gSequenceParentWork.currentOwner, gridX, gridZ);
    leader->speedLimit = 0x10000;
    Actor_ArmCallback(gSequenceParentWork.currentOwner, 0xc000, 0);
    Actor_RefreshSelectorById(gSequenceParentWork.currentOwner);
    leader->actionFlags = 1;
    leader->motionFlags = 0;
    spriteFlags &= 0xfe;
    Actor_SetSingleChildFlagsFar(leader, spriteFlags);
    Actor_SetMode(leader, 13);
    Actor_SetTargetPosition(leader, gridX << 16, leader->y - 0x100000,
                       (gridZ << 16) + 0x100000);
    Actor_WaitForMoveById(gSequenceParentWork.currentOwner);
    gSequenceParentWork.renderMode = 2;
    leader->z = (leader->z & 0xfff00000) + 0x80000;
  } else {
    // Let go of the wall and fall.
    Actor_SetMode(leader, 10);
    leader->motionFlags = 3;
    leader->velocityY = 0x40000;
    leader->terrainHeight = leader->y;
    spriteFlags |= 1;
    Actor_SetSingleChildFlagsFar(leader, spriteFlags);
    Event_Wait(6);
    gSequenceParentWork.renderMode = 0;
    leader->actionFlags = 1;
    leader->angle = 0xc000;
    leader->action = 0;
    leader->unk66 = 0;
  }
  Event_End();
  return 0;

failure:
  Event_End();
  return -1;
}
