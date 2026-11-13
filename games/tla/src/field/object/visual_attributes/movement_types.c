/// @file
/// Actor scripts: moving, turning and the built-in movement types.
// Part of visual_attributes.c.

#include "../visual_attributes_internal.h"
#include "module/field/field.h"
#include "module/system/system.h"
#include "scene_call.h"

/* VIEW(f0d489c): routine Actor_CreateFar as struct LinkedEffectObject
 * *Actor_CreateFar(s32 kind, s32 x, s32 y, s32 z); visual_attributes has struct
 * BattleEffectWatchedObject *Actor_CreateFar(s32 kind, s32 x, s32 y, s32 z). */
extern struct LinkedEffectObject *
Func_080200c0__f0d489c(s32 kind, s32 x, s32 y,
                       s32 z) __asm__("Actor_CreateFar");
/* VIEW(f0d489c): routine Actor_ReleaseFar as void Actor_ReleaseFar(struct
 * LinkedEffectObject *object); battle.h has void Actor_ReleaseFar(struct Actor
 * *object). */
extern void Actor_Destroy__f0d489c(struct LinkedEffectObject *object) __asm__(
    "Actor_ReleaseFar");
/* VIEW(f0d489c): routine Actor_IsOutsideAnimation108 as s32
 * Actor_IsOutsideAnimation108(struct LinkedEffectObject *object); the
 * definition has s32 Actor_IsOutsideAnimation108(struct Actor *object). */
extern s32 Actor_IsOutsideAnimation108__f0d489c(
    struct LinkedEffectObject *object) __asm__("Actor_IsOutsideAnimation108");
/* VIEW(f0d489c): routine FieldEffect_CopyWatchedActorPosition as void
 * FieldEffect_CopyWatchedActorPosition(void); the definition has s32
 * FieldEffect_CopyWatchedActorPosition(struct Actor *object). */
extern void BattleEffect_CopyWatchedObjectPosition__f0d489c(void) __asm__(
    "FieldEffect_CopyWatchedActorPosition");
extern const u8 gActor_WaitScript[];

/// The Broken Seal's BattleEffect_ConfigureWatchedObject with this game's
/// calls: with flags & 3 zero it destroys the object's linked effect;
/// otherwise (mode 2, or when the linked effect is outside animation 0x108)
/// it creates effect object 264 at the object and links it, mode 1 keeping it
/// as the object's resource and mode 2 running gActor_WaitScript on it.
void FieldEffect_SetAttached(s32 actorId, s32 flags) {
  struct LinkedEffectObject *object = ObjectTable_Get(actorId);
  struct LinkedEffectObject *child;
  struct LinkedEffectVisual *visual;
  s32 mode;

  child = 0;
  visual = 0;

  if (object == 0)
    return;

  if ((flags & 3) != 0) {
    if ((flags & 3) == 2 ||
        Actor_IsOutsideAnimation108__f0d489c(object->resource)) {
      child = Func_080200c0__f0d489c(264, object->x, object->y, object->z);
    }
  } else {
    child = object->resource;
    if (child == 0)
      return;
    Actor_Destroy__f0d489c(child);
    object->resource = NULL;
    return;
  }

  if (child == 0)
    return;

  mode = flags & 3;
  switch (mode) {
  case 1:
    Actor_SelectAnimationFar(child, 1);
    object->resource = child;
    child->counter = 1;
    break;
  case 2:
    Actor_SelectAnimationFar(child, 2);
    Actor_StartScriptFar(child, gActor_WaitScript);
    child->counter = 1;
    break;
  }

  child->resourceId = actorId;
  child->value55 = 0;
  child->callback = BattleEffect_CopyWatchedObjectPosition__f0d489c;
  visual = child->visual;
  visual->value26 = 0;
  child->resource = object;

  if (flags & 0x100) {
    s32 mask = 13;
    u8 visualFlags = visual->flags;

    mask = -mask;
    mask &= visualFlags;
    mask |= 4;
    visual->flags = mask;
  } else {
    s32 copiedFlags = 12;
    u8 sourceFlags = object->visual->flags;
    u8 destinationFlags;
    s32 clearMask = 13;

    copiedFlags &= sourceFlags;
    destinationFlags = visual->flags;
    clearMask = -clearMask;
    clearMask &= destinationFlags;
    clearMask |= copiedFlags;
    visual->flags = clearMask;
  }
}

s32 __divsi3(s32, s32);

s32 Actor_ClearChildAndCollisionFlags(struct Actor *obj) {
  Actor_SetSpriteFlagsFar((struct Actor *)obj, 0);
  obj->collisionFlags = 0;
  return 0;
}

s32 Actor_ClearChildFlags(struct Actor *obj) {
  Actor_SetSpriteFlagsFar((struct Actor *)obj, 0);
  return 0;
}

s32 Actor_SetPriorityBit5(struct ObjectFlags23 *obj) {
  obj->flags23 |= 0x20;
  return 0;
}

s32 Actor_MoveTowardTarget(struct Actor *object) {
  struct Actor *target;
  s32 deltaX;
  s32 deltaY;
  s32 cellX;
  s32 cellY;
  s32 newX;
  s32 distance;
  s32 step;

  target = object->watchedObject;
  if (target != 0) {
    deltaX = target->x - object->x;
    if (deltaX < 0)
      deltaX += 0xffff;
    cellX = deltaX >> 16;
    deltaY = target->z - object->z;
    if (deltaY < 0)
      deltaY += 0xffff;
    cellY = deltaY >> 16;
    distance = Iwram_Sqrt(cellX * cellX + cellY * cellY);
    step = object->timer;
    if (distance >= step) {
      newX = object->x + __divsi3(cellX << 20, step);
      Actor_SetMoveTargetFar(object, newX, object->y,
                              object->z + __divsi3(cellY << 20, step));
      Actor_SelectAnimationFar(object, 2);
    } else {
      Actor_SelectAnimationFar(object, 1);
    }
  }
  return 1;
}

// Objects: turn toward the linked target.

/// Turns the object toward its facing target, if it has one, by at most
/// 0x1000 (22.5 degrees).
/// obj: object to turn.
s32 Actor_TurnTowardWatchedActor(struct FacingObject *obj) {
  s32 turn;
  u16 old;
  s32 dir;
  struct FacingObject *target;

  target = obj->facingTarget;
  if (target != NULL) {
    obj->facingFlags = 0xfe & obj->facingFlags;
    dir = (u16)ArcTan2(target->positionZ - obj->positionZ,
                       target->positionX - obj->positionX);
    old = obj->facing;
    turn = (s16)(dir - old);
    if (turn != 0) {
      if (turn > 0x1000)
        turn = 0x1000;
      if (turn < -0x1000)
        turn = -0x1000;
      obj->facing = old + turn;
    }
  }
  return 1;
}

/// Makes the object watch target and keeps it facing it.
void Func_080d4aa8(struct Actor *object, struct Actor *target) {
  object->watchedObject = target;
  object->update = Actor_TurnTowardWatchedActor;
}

/// Gives the object the party leader's display layer.
s32 Func_080d4ab4(struct Actor *object) {
  struct Actor *leader = ObjectTable_Get(gMainRoutineWork.currentOwner);
  s32 layer = ((struct ObjectLayerAnimation *)leader->sprite)->layer;

  if ((object->animationKind & 0xf) == 1) {
    struct ObjectLayerAnimation *animation = object->sprite;

    animation->layer = layer;
    animation->shadowLayer = layer;
    object->priorityFlags &= 0xfe;
  }
  return 0;
}

struct Actor *Actor_GetById(u32 objectId);

extern const u8 gMovementType_StandScript[];
extern const u8 gMovementType_WanderScript[];
extern const u8 gMovementType_WanderNearScript[];
extern const u8 gMovementType_WanderCloseScript[];
extern const u8 gMovementType_FollowWatchedScript[];
extern const u8 gMovementType_FaceLeaderScript[];
extern const u8 gData_080f34e8[];
extern const u8 gData_080f3614[];
extern const u8 gData_080f3764[];
extern const u8 gMovementType_ResetActionScript[];
extern const u8 gData_080f34f4[];

void Actor_SetActionCallback(struct Actor *object, s32 kind) {
  switch (kind) {
  case 1:
    kind = (s32)gMovementType_StandScript;
    break;
  case 2:
    kind = (s32)gMovementType_WanderScript;
    break;
  case 3:
    kind = (s32)gMovementType_WanderNearScript;
    break;
  case 4:
    kind = (s32)gMovementType_WanderCloseScript;
    break;
  case 5:
    kind = (s32)gMovementType_FollowWatchedScript;
    break;
  case 6:
    object->watchedObject = Actor_GetById(gMainRoutineWork.currentOwner);
    kind = (s32)gMovementType_FaceLeaderScript;
    break;
  case 7:
    kind = (s32)gData_080f34e8;
    break;
  case 8:
    kind = (s32)gData_080f3614;
    break;
  case 9:
    kind = (s32)gData_080f3764;
    break;
  case 10:
    kind = (s32)gMovementType_ResetActionScript;
    break;
  case 11:
    kind = (s32)gData_080f34f4;
    break;
  }
  Actor_StartScriptFar(object, (const void *)kind);
}

/// The Broken Seal's Actor_FindNearestFacingTarget: the nearest object
/// within 40 pixels in the direction self faces, if its first animation is id.
struct Actor *Actor_FindNearestFacingTarget(struct Actor *self, s32 id) {
  struct Actor *entry;
  struct Actor *found;
  s32 cnt;
  s32 best;
  s32 dy;
  s32 dx;
  s32 dz;
  s32 dist;
  s32 angle;
  s32 turn;

  found = NULL;
  best = 40;
  entry = Ram_HeapSlots->scriptObjects;
  for (cnt = 0; cnt < 64; cnt++, entry++) {
    if (entry->script == NULL)
      continue;
    if (entry == self)
      continue;
    if (entry->animationKind != 1)
      continue;
    dy = entry->y - self->y;
    if (dy >= 0) {
      if (dy > 0x2fffff)
        continue;
    } else {
      if (self->y - entry->y > 0x2fffff)
        continue;
    }
    dx = (entry->x - self->x) / Q16_ONE;
    dz = (entry->z - self->z) / Q16_ONE;
    dist = Iwram_Sqrt(dx * dx + dz * dz);
    if (dist >= best)
      continue;
    angle = (u16)ArcTan2(entry->z - self->z, entry->x - self->x);
    if (dist > 23) {
      turn = (s16)(angle - self->facing);
      if (turn < -0x2fff)
        continue;
      if (turn > 0x2fff)
        continue;
    }
    found = entry;
    best = dist;
  }
  if (found == NULL)
    return NULL;
  if (((struct ObjectAnimation *)found->sprite)->entries[0]->animId != id)
    return NULL;
  return found;
}

s32 Func_080d4ccc(struct Actor *object, s32 *position) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  s8 *event = Ram_HeapSlots->eventWork;

  if (map == 0 || event[0x18a] == 3 ||
      MapGrid_GetTerrainKindFar(object->terrainId, position[0], position[2]) <=
          2)
    return 0;
  return 1;
}

extern struct MapCell gMapCellBuffer[];

/// The leader jumps two tiles forward, over the tile in
/// front, when it faces a quarter turn, the map allows it (Func_080d4ccc and
/// the step check) or a platform object (flag 0x80) waits 16 pixels below the
/// landing, and no solid object stands there. A platform underfoot (sprite 258
/// or 259 with flag 2) dips 2 pixels with the leader. Each jump advances
/// walking distance, and the leader keeps jumping while it lands on a jump spot
/// (collision 254). Returns 0 after a jump, -1 when the leader cannot jump.
s32 Field_TryJumpForward(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  s32 result = -1;
  s32 angle = leader->facing;
  u8 *flags = &leader->motionFlags;
  u8 saved = *flags;
  struct JumpEventWork *work = Ram_HeapSlots->eventWork;
  s16 spriteFlags = 1;
  s32 position[3];
  s32 *positionWords;
  struct Actor *entry;
  struct Actor *platform;
  s32 canJump;
  s32 i;

  if (gMainRoutineWork.renderMode == 2)
    return result;
  if (gMainRoutineWork.renderMode == 3)
    return result;
  if ((angle + 0x1000) & 0x2000) // not facing a quarter turn
    return result;
  angle = (angle + 0x2000) & 0xc000;
  positionWords = position;
again:
  // The middle of the tile in front, then of the tile beyond.
  position[0] = (leader->x & 0xfff00000) + 0x80000;
  position[1] = leader->y;
  position[2] = (leader->z & 0xfff00000) + 0x80000;
  Vector_AddPolarOffset(0x100000, angle, position);
  if (MapGrid_CheckCollisionFar(leader, position) == 1)
    return -1;
  position[0] = (leader->x & 0xfff00000) + 0x80000;
  position[1] = leader->y;
  position[2] = (leader->z & 0xfff00000) + 0x80000;
  Vector_AddPolarOffset(0x200000, angle, position);
  canJump = 0;
  if (Func_080d4ccc(leader, position) == 0 &&
      MapGrid_CheckCollisionFar(leader, position) == 0)
    canJump = 1;

  // A platform 16 pixels below the landing makes it a jump spot.
  if (leader->priorityFlags & 0x40) {
    entry = Ram_HeapSlots->scriptObjects;
    for (i = 0; i < 64; i++, entry++) {
      s32 bound = 0x7ffff;
      s32 delta;
      s32 dy;

      if (entry->script == NULL)
        continue;
      if (!(entry->priorityFlags & 0x80))
        continue;
      if (entry->unk56 != 0)
        continue;
      if (entry == leader)
        continue;
      if ((position[0] - entry->x + 0x80000) >> 20 != 0)
        continue;
      if ((position[2] - entry->z + 0x80000) >> 20 != 0)
        continue;
      delta = position[1] - entry->y;
      dy = delta - 0x100000;
      if (dy >= 0) {
        if (dy <= bound)
          goto hit;
      } else {
        dy = 0x100000 - delta;
        if (dy <= bound) {
        hit:
          canJump = 1;
          break;
        }
      }
    }
  }

  // A solid object at the landing blocks it.
  entry = Ram_HeapSlots->scriptObjects;
  for (i = 0; i < 64; i++, entry++) {
    if (entry->script != NULL && !(entry->priorityFlags & 0x80) &&
        entry != leader && entry->collisionFlags != 0 &&
        (position[0] - entry->x + 0x80000) >> 20 == 0 &&
        (position[2] - entry->z + 0x80000) >> 20 == 0) {
      s32 d = position[1] - entry->y;
      if (d >= 0) {
        if (d <= 0x7ffff)
          goto hit2;
      } else if (entry->y - position[1] <= 0x7ffff) {
      hit2:
        canJump = 0;
        break;
      }
    }
  }

  if (canJump) {
    struct MapWork *map;
    u8 attr;

    // Crouch, then jump there.
    if (leader->animationKind == 1)
      spriteFlags = ((struct JumpSprite *)leader->sprite)->flags;
    Event_Start();
    Actor_SelectAnimationFar(leader, 6);
    wait(6);
    Sound_PlayFar(152);
    Actor_SelectAnimationFar(leader, 7);
    leader->speedLimit = 0x30000;
    leader->acceleration = 0x20000;
    leader->velocityY = 0x40000;
    *flags &= 0x7e;
    Actor_SetSpriteFlagsFar(leader, spriteFlags & 0xfe);
    Actor_MoveToAndWaitById(gMainRoutineWork.currentOwner,
                            ((s16 *)positionWords)[1],
                            ((s16 *)positionWords)[5]);
    Actor_SelectAnimationFar(leader, 6);
    Actor_SetSpriteFlagsFar(leader, spriteFlags);

    map = Ram_HeapSlots->mapWork;
    attr = map->unk017;
    if (attr & 12) {
      s32 cellX = leader->x >> 20;
      s32 cellZ = leader->z >> 20;
      struct MapCell *cells;

      if (map == NULL)
        cells = gMapCellBuffer;
      else
        cells = map->layers[leader->terrainId & 3].cells;
      cells += cellX + (cellZ << 7);
      if (cells->attributeB & 16)
        Func_080203a0(leader, (u32)(attr << 28) >> 30);
    }

    // A platform underfoot dips 2 pixels with the leader and comes back up.
    if (((platform = Actor_FindNearestFacingTarget(leader, 258)) != NULL ||
         (platform = Actor_FindNearestFacingTarget(leader, 259)) != NULL) &&
        (platform->priorityFlags & 2)) {
      Actor_SelectAnimationFar(platform, 7);
      leader->y -= 0x10000;
      leader->terrainHeight -= 0x10000;
      wait(2);
      leader->y -= 0x10000;
      leader->terrainHeight -= 0x10000;
      wait(10);
      leader->y += 0x10000;
      leader->terrainHeight += 0x10000;
      wait(4);
      leader->y += 0x10000;
      leader->terrainHeight += 0x10000;
    } else {
      wait(6);
    }
    *flags = saved;
    Event_End();
    if (work != NULL)
      work->walkingDistance +=
          Iwram_MulQ16(work->walkingDistanceScale, 0x200000);
    if (MapGrid_GetCellBehaviorAtFar(leader->terrainId, position[0],
                                     position[2]) == 254) {
      Actor_SelectAnimationFar(leader, 1);
      wait(6);
      goto again;
    }
    result = 0;
  }
  leader->timer = 8;
  leader->unk66 = 0;
  return result;
}

void Actor_WaitForMoveById(s32 id);

s32 Func_080d50f8(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
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
    spriteFlags = ((struct ClimbSprite *)leader->sprite)->flags;
  if (gMainRoutineWork.renderMode == 0) {
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
    if (MapGrid_GetTerrainHeightFar(leader->terrainId, gridX << 16,
                                    gridZ << 16) <=
        MapGrid_GetTerrainHeightFar(leader->terrainId, gridX << 16,
                                    (gridZ + 16) << 16))
      goto failure;
    position[0] = leader->x;
    position[1] = leader->y - 0x100000;
    position[2] = leader->z;
    if (MapGrid_IsWallAtFar(position) != 0)
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
    Actor_MoveToAndWaitById(gMainRoutineWork.currentOwner, gridX, gridZ);
    leader->speedLimit = 0x10000;
    Actor_ArmCallback(gMainRoutineWork.currentOwner, 0xc000, 0);
    Actor_RefreshSelectorById(gMainRoutineWork.currentOwner);
    leader->actionFlags = 1;
    leader->motionFlags = 0;
    spriteFlags &= 0xfe;
    Actor_SetSpriteFlagsFar(leader, spriteFlags);
    Actor_SelectAnimationFar(leader, 13);
    Actor_SetMoveTargetFar(leader, gridX << 16, leader->y - 0x100000,
                            (gridZ << 16) + 0x100000);
    Actor_WaitForMoveById(gMainRoutineWork.currentOwner);
    gMainRoutineWork.renderMode = 2;
    leader->z = (leader->z & 0xfff00000) + 0x80000;
  } else {
    // Let go of the wall and fall.
    Actor_SelectAnimationFar(leader, 10);
    leader->motionFlags = 3;
    leader->velocityY = 0x40000;
    leader->terrainHeight = leader->y;
    spriteFlags |= 1;
    Actor_SetSpriteFlagsFar(leader, spriteFlags);
    Event_Wait(6);
    gMainRoutineWork.renderMode = 0;
    leader->actionFlags = 1;
    leader->facing = 0xc000;
    leader->timer = 0;
    leader->unk66 = 0;
  }
  Event_End();
  return 0;

failure:
  Event_End();
  return -1;
}

s32 Func_080d53b8(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
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
    spriteFlags = ((struct ClimbSprite *)leader->sprite)->flags;
  if (gMainRoutineWork.renderMode == 0) {
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
    if (MapGrid_GetTerrainHeightFar(leader->terrainId, gridX << 16,
                                    gridZ << 16) >=
        MapGrid_GetTerrainHeightFar(leader->terrainId, gridX << 16,
                                    (gridZ - 16) << 16))
      goto failure;
    position[0] = leader->x;
    position[1] = leader->y;
    position[2] = leader->z;
    result = MapGrid_IsWallAtFar(position);
    if (result != 0)
      goto failure;

    // Step to the middle of the tile, crouch, jump and grab the wall.
    leader->actionFlags = result;
    Actor_MoveToAndWaitById(gMainRoutineWork.currentOwner, gridX, gridZ);
    Actor_SelectAnimationFar(leader, 6);
    wait(4);
    Actor_SelectAnimationFar(leader, 7);
    leader->velocityY = 0x40000;
    wait(4);
    leader->motionFlags = result;
    spriteFlags &= 0xfe;
    Actor_SetSpriteFlagsFar(leader, spriteFlags);
    leader->speedLimit = 0x10000;
    leader->velocityY = result;
    Actor_SelectAnimationFar(leader, 12);
    wait(4);
    gMainRoutineWork.renderMode = 2;
    leader->actionFlags = 1;
    wait(8);
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
    Actor_SelectAnimationFar(leader, 11);
    Actor_SetMoveTargetFar(leader, gridX << 16, leader->y + 0x80000,
                            (gridZ << 16) - 0x100000);
    Actor_WaitForMoveById(gMainRoutineWork.currentOwner);
    leader->motionFlags = 3;
    leader->terrainHeight = leader->y;
    spriteFlags |= 1;
    Actor_SetSpriteFlagsFar(leader, spriteFlags);
    Event_Wait(4);
    gMainRoutineWork.renderMode = 0;
    leader->actionFlags = 1;
    leader->timer = 0;
    leader->unk66 = 0;
  }
  Event_End();
  return 0;

failure:
  Event_End();
  return -1;
}

struct Work9cItem *Func_080d5670(s32 *position) {
  struct Work9cItem *found;
  struct Work9c *work;
  struct Work9cItem *item;
  s32 best;
  s32 i;

  found = NULL;
  work = WORK_9C;
  if (work == NULL)
    return NULL;
  /* FAKEMATCH(barrier): an empty asm keeps the loop's set-up (the item
   * pointer, the best distance, the count and the IWRAM entry) after the
   * workspace test, where if-conversion would hoist it; without it, 29 of 85
   * instructions differ in tla-en. */
  asm("");
  item = work->items;
  best = 0x10000000;
  for (i = 0; i < 128; i++, item++) {
    if (item->used == 1) {
      struct Work9cItemPosition *at = (struct Work9cItemPosition *)item;
      s32 dx = (position[0] - at->x) >> 8;
      s32 dy = (position[1] - at->y) >> 8;
      s32 dz = (position[2] - at->z) >> 8;
      s32 distance =
          Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dy, dy) + Iwram_MulQ16(dz, dz);

      if (distance <= 0x400 && best > distance) {
        best = distance;
        found = item;
      }
    }
  }
  return found;
}

s32 Func_080d5720(void) {
  struct Actor *leader = ObjectTable_Get(gMainRoutineWork.currentOwner);
  struct FieldSprite *sprite = leader->sprite;
  struct Work9cItemPosition *item = Func_080d5670(&leader->x);
  u8 *flags;

  ((struct EventWorkGrab *)Ram_HeapSlots->eventWork)->grabState = 0;
  if (item == NULL)
    return 0;
  Sound_PlayFar(152);
  leader->acceleration = 0x10000;
  leader->speedLimit = 0x20000;
  leader->velocityY = 0x40000;
  leader->watchedObject = (struct Actor *)item;
  Actor_SelectAnimationFar(leader, 2);
  Actor_SetMoveTargetFar(leader, item->x, item->y, item->z);
  Actor_WaitForMoveFar(leader);
  leader->timer = leader->motionFlags;
  flags = &leader->priorityFlags;
  leader->unk66 = sprite->nextFrame;
  ((struct GrabSave *)leader)->priority = sprite->priority;
  leader->priorityFlags = *flags & ~1;
  sprite->nextFrame = 0;
  sprite->priority = 1;
  gMainRoutineWork.renderMode = 3;
  return 1;
}

s32 Field_ClimbDown(void) {
  struct Actor *leader = ObjectTable_Get(gMainRoutineWork.currentOwner);
  struct FieldSprite *sprite = leader->sprite;
  s32 position[3];
  u32 angle;

  ((struct EventWorkGrab *)Ram_HeapSlots->eventWork)->grabState = 0;
  angle = Data_080f0890[(gInput.held >> 4) & 15];
  if (angle == 0xffff)
    return 0;
  leader->x = (leader->x & 0xfff00000) + 0x80000;
  leader->z = (leader->z & 0xfff00000) + 0x80000;
  position[0] = leader->x;
  position[1] = leader->y;
  position[2] = leader->z;
  Vector_AddPolarOffset(0x100000, angle, position);
  leader->terrainHeight = leader->y - 0x80000;
  if (MapGrid_GetCellAttributeLowNibbleFar(leader->terrainId, position[0],
                                           position[2]) ||
      MapGrid_CheckCollisionFar(leader, position))
    return 0;
  leader->facing = angle;
  leader->priorityFlags |= 1;
  leader->motionFlags = leader->timer;
  leader->scaleX = 0x10000;
  sprite->nextFrame = leader->unk66;
  sprite->rotation = 0;
  Sound_PlayFar(152);
  leader->speedLimit = 0x20000;
  leader->velocityY = 0x40000;
  leader->acceleration = 0x10000;
  Actor_SelectAnimationFar(leader, 2);
  Actor_SetMoveTargetFar(leader, position[0], position[1], position[2]);
  Actor_WaitForMoveFar(leader);
  sprite->priority = ((u8 *)leader)[98];
  gMainRoutineWork.renderMode = 0;
  return 1;
}

struct WorkA0Node *Func_080d5930(s32 *position) {
  struct WorkA0Node *found;
  struct WorkA0 *work;
  struct WorkA0Node *item;
  s32 best;
  s32 i;

  found = NULL;
  work = WORK_A0;
  if (work == NULL)
    return NULL;
  /* FAKEMATCH(barrier): an empty asm keeps the loop's set-up (the item
   * pointer, the best distance, the count and the IWRAM entry) after the
   * workspace test, where if-conversion would hoist it; without it, 29 of 84
   * instructions differ in tla-en. */
  asm("");
  item = work->items;
  best = 0x10000000;
  for (i = 0; i < 128; i++, item++) {
    if (item->used == 1) {
      s32 dx = (position[0] - item->x) >> 8;
      s32 dy = (position[1] - item->y) >> 8;
      s32 dz = (position[2] - item->z) >> 8;
      s32 distance =
          Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dy, dy) + Iwram_MulQ16(dz, dz);

      if (distance <= 0x400 && best > distance) {
        best = distance;
        found = item;
      }
    }
  }
  return found;
}

s32 Func_080d59dc(void) {
  struct MainRoutineWork *work = &gMainRoutineWork;
  struct Actor *leader = ObjectTable_Get(work->currentOwner);
  struct FieldSprite *sprite = leader->sprite;
  struct Work9cItemPosition *item = Func_080d5930(&leader->x);
  u8 *flags;

  ((struct EventWorkGrab *)Ram_HeapSlots->eventWork)->grabState = 0;
  if (item == NULL)
    return 0;
  Sound_PlayFar(152);
  leader->acceleration = 0x10000;
  leader->speedLimit = 0x20000;
  leader->velocityY = 0x40000;
  leader->watchedObject = (struct Actor *)item;
  Actor_SelectAnimationFar(leader, 2);
  Actor_SetMoveTargetFar(leader, item->x, item->y, item->z);
  Actor_WaitForMoveFar(leader);
  leader->timer = leader->motionFlags;
  leader->unk66 = sprite->nextFrame;
  ((struct GrabSave *)leader)->priority = sprite->priority;
  flags = &leader->priorityFlags;
  leader->priorityFlags = *flags & ~1;
  sprite->nextFrame = 0;
  sprite->priority = 1;
  work->renderMode = 4;
  return 1;
}

s32 Func_080d5aa0(void) {
  struct Actor *leader = ObjectTable_Get(gMainRoutineWork.currentOwner);
  struct FieldSprite *sprite = leader->sprite;
  s32 position[3];
  u32 angle;

  ((struct EventWorkGrab *)Ram_HeapSlots->eventWork)->grabState = 0;
  angle = Data_080f0890[(gInput.held >> 4) & 15];
  if (angle == 0xffff)
    return 0;
  leader->x = (leader->x & 0xfff00000) + 0x80000;
  leader->z = (leader->z & 0xfff00000) + 0x80000;
  position[0] = leader->x;
  position[1] = leader->y;
  position[2] = leader->z;
  Vector_AddPolarOffset(0x100000, angle, position);
  leader->terrainHeight = leader->y - 0x80000;
  if (MapGrid_GetCellAttributeLowNibbleFar(leader->terrainId, position[0],
                                           position[2]) ||
      MapGrid_CheckCollisionFar(leader, position))
    return 0;
  leader->facing = angle;
  leader->priorityFlags |= 1;
  leader->motionFlags = leader->timer;
  sprite->nextFrame = leader->unk66;
  sprite->rotation = 0;
  Sound_PlayFar(152);
  leader->acceleration = 0x10000;
  leader->speedLimit = 0x20000;
  leader->velocityY = 0x40000;
  Actor_SelectAnimationFar(leader, 2);
  Actor_SetMoveTargetFar(leader, position[0], position[1], position[2]);
  Actor_WaitForMoveFar(leader);
  sprite->priority = ((u8 *)leader)[98];
  gMainRoutineWork.renderMode = 0;
  return 1;
}
