// Draft, differ 167 of 463 (179 before statements reordered 2026-10-26): The Broken Seal twin (Field_TryJumpForward), with
// this game's checks: the leader jumps two tiles forward, over the tile in front, when
// it faces a quarter turn, the map allows it (Func_080d4ccc and the step
// check) or a platform object (flag 0x80) waits 16 pixels below the landing,
// and no solid object stands there. A platform underfoot (sprite 258 or 259
// with flag 2) dips 2 pixels with the leader. Each jump advances walking
// distance, and the leader keeps jumping while it lands on a jump spot
// (collision 254). Returns 0 after a jump, -1 when the leader cannot jump.
// The two mode tests are two statements (joined, they fold into one range
// check: differ 192). Left: the platform scan takes its height difference's
// absolute value and compares once, where this draft threads the compare
// into both arms, and the solid scan's arms are laid out the other way round
// (written with an explicit absolute value: 202); the half tile 0x80000 is
// not shared between the x and z tests; the angle takes r4 on its way to
// r10 at the start.
#include "types.h"
#include "iwram_call.h"
#include "macros.h"
#include "map.h"
#include "object_runtime.h"
#include "adventure.h"
#include "ram_buffer.h"

struct JumpSprite {
  u8 unk00[26];
  u8 flags;
};

struct JumpEventWork {
  u8 unk000[0x19c];
  s32 walkingDistanceScale;
  s32 walkingDistance;
};

extern struct MapCell gMapCellBuffer[];

struct ObjectRuntime *Object_GetById(s32 id);
void EventRuntime_Start(void);
void EventRuntime_End(void);
void WaitFrames(s32 frames);
void Vector_AddPolarOffset(s32 magnitude, s32 angle, s32 *position);
s32 Func_08020210(struct ObjectRuntime *object, s32 *position);
s32 Func_080d4ccc(struct ObjectRuntime *object, s32 *position);
void ObjectDispatch_SetSingleChildFlagsFar(struct ObjectRuntime *object,
                                           s32 flags);
void ObjectMotion_SetPositionAndCommit(s32 id, s32 x, s32 z);
void Func_080203a0(struct ObjectRuntime *object, s32 kind);
struct ObjectRuntime *Object_FindNearestFacingTarget(struct ObjectRuntime *self,
                                                     s32 id);
s32 GetMapCellCollisionFar(s32 layer, s32 x, s32 z);

s32 Func_080d4d08(void) {
  struct ObjectRuntime *leader = Object_GetById(gAdventure.currentOwner);
  u8 *flags = &leader->motionFlags;
  s32 result = -1;
  u8 saved = *flags;
  struct JumpEventWork *work = Ram_HeapSlots->eventWork;
  s32 angle = leader->angle;
  s16 spriteFlags = 1;
  s32 position[3];
  struct ObjectRuntime *entry;
  struct ObjectRuntime *platform;
  s32 canJump;
  s32 i;

  if (gAdventure.renderMode == 2)
    return result;
  if (gAdventure.renderMode == 3)
    return result;
  if ((angle + 0x1000) & 0x2000) // not facing a quarter turn
    return result;
  angle = (angle + 0x2000) & 0xc000;
again:
  // The middle of the tile in front, then of the tile beyond.
  position[0] = (leader->x & 0xfff00000) + 0x80000;
  position[1] = leader->y;
  position[2] = (leader->z & 0xfff00000) + 0x80000;
  Vector_AddPolarOffset(0x100000, angle, position);
  if (Func_08020210(leader, position) == 1)
    return -1;
  position[0] = (leader->x & 0xfff00000) + 0x80000;
  position[1] = leader->y;
  position[2] = (leader->z & 0xfff00000) + 0x80000;
  Vector_AddPolarOffset(0x200000, angle, position);
  canJump = 0;
  if (Func_080d4ccc(leader, position) == 0 &&
      Func_08020210(leader, position) == 0)
    canJump = 1;

  // A platform 16 pixels below the landing makes it a jump spot.
  if (leader->priorityFlags & 0x40) {
    entry = Ram_HeapSlots->scriptObjects;
    for (i = 0; i < 64; i++, entry++) {
      if (entry->script != NULL && (entry->priorityFlags & 0x80) &&
          entry->unk56 == 0 && entry != leader &&
          (position[0] - entry->x + 0x80000) >> 20 == 0 &&
          (position[2] - entry->z + 0x80000) >> 20 == 0 &&
          ABS(position[1] - entry->y - 0x100000) <= 0x7ffff) {
        canJump = 1;
        break;
      }
    }
  }

  // A solid object at the landing blocks it.
  entry = Ram_HeapSlots->scriptObjects;
  for (i = 0; i < 64; i++, entry++) {
    if (entry->script != NULL && !(entry->priorityFlags & 0x80) &&
        entry != leader && entry->collisionFlags != 0 &&
        (position[0] - entry->x + 0x80000) >> 20 == 0 &&
        (position[2] - entry->z + 0x80000) >> 20 == 0 &&
        ABS(position[1] - entry->y) <= 0x7ffff) {
      canJump = 0;
      break;
    }
  }

  if (canJump) {
    struct MapWork *map;

    // Crouch, then jump there.
    if (leader->animationKind == 1)
      spriteFlags = ((struct JumpSprite *)leader->animation)->flags;
    EventRuntime_Start();
    Object_SetMode(leader, 6);
    WaitFrames(6);
    Sound_PlayFar(152);
    Object_SetMode(leader, 7);
    leader->speedLimit = 0x30000;
    leader->acceleration = 0x20000;
    leader->velocityY = 0x40000;
    *flags &= 0x7e;
    ObjectDispatch_SetSingleChildFlagsFar(leader, spriteFlags & 0xfe);
    ObjectMotion_SetPositionAndCommit(gAdventure.currentOwner,
                                      ((s16 *)&position[0])[1],
                                      ((s16 *)&position[2])[1]);
    Object_SetMode(leader, 6);
    ObjectDispatch_SetSingleChildFlagsFar(leader, spriteFlags);

    map = Ram_HeapSlots->mapWork;
    if (map->unk017 & 12) {
      s32 cellX = leader->x >> 20;
      s32 cellZ = leader->z >> 20;
      struct MapCell *cells;

      if (map == NULL)
        cells = gMapCellBuffer;
      else
        cells = map->layers[leader->terrainId & 3].cells;
      if (cells[cellX + cellZ * 128].attributeB & 16)
        Func_080203a0(leader, (map->unk017 >> 2) & 3);
    }

    // A platform underfoot dips 2 pixels with the leader and comes back up.
    if (((platform = Object_FindNearestFacingTarget(leader, 258)) != NULL ||
         (platform = Object_FindNearestFacingTarget(leader, 259)) != NULL) &&
        (platform->priorityFlags & 2)) {
      Object_SetMode(platform, 7);
      leader->y -= 0x10000;
      leader->terrainHeight -= 0x10000;
      WaitFrames(2);
      leader->y -= 0x10000;
      leader->terrainHeight -= 0x10000;
      WaitFrames(10);
      leader->y += 0x10000;
      leader->terrainHeight += 0x10000;
      WaitFrames(4);
      leader->y += 0x10000;
      leader->terrainHeight += 0x10000;
    } else {
      WaitFrames(6);
    }
    *flags = saved;
    EventRuntime_End();
    if (work != NULL)
      work->walkingDistance +=
          Iwram_MulQ16(work->walkingDistanceScale, 0x200000);
    if (GetMapCellCollisionFar(leader->terrainId, position[0], position[2]) ==
        254) {
      Object_SetMode(leader, 1);
      WaitFrames(6);
      goto again;
    }
    result = 0;
  }
  leader->action = 8;
  leader->unk66 = 0;
  return result;
}
