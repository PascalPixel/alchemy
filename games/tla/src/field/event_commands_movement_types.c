/// @file
/// Actors' movement types and what they attach, and the leader's moves off the
/// ground: jumping a gap, climbing a wall up and down, and stepping onto and
/// off a rope or tightrope.
// Part of event_commands.c.

#include "event_commands_internal.h"
#include "fixed_math.h"
#include "sounds.h"
#include "field/field.h"
#include "system/system.h"
#include "field/scene_call.h"
#include "field/field_sequence.h"

extern const u8 gActor_WaitScript[];

/// The event command "actor set attached effect", by enum AttachedEffectFlag:
/// removes the actor attached to an actor, attaches one
/// (SPRITE_ATTACHED_EFFECT, animation 1) unless one already is, or shows a
/// one-off one (animation 2) that ends by itself (gActor_WaitScript).
void FieldEffect_SetAttached(s32 actorId, s32 flags) {
  struct Actor *actor = Actor_Lookup(actorId);
  struct Actor *effect;
  struct Sprite *visual;
  s32 mode;

  effect = 0;
  visual = 0;

  if (actor == 0)
    return;

  if ((flags & 3) != 0) {
    if ((flags & 3) == ATTACHED_EFFECT_ONCE ||
        Actor_IsNotAttachedEffect(actor->watchedActor)) {
      effect = Actor_CreateFar(SPRITE_ATTACHED_EFFECT, actor->x.fixed,
                               actor->y.fixed, actor->z.fixed);
    }
  } else {
    effect = actor->watchedActor;
    if (effect == 0)
      return;
    Actor_ReleaseFar(effect);
    actor->watchedActor = NULL;
    return;
  }

  if (effect == 0)
    return;

  mode = flags & 3;
  switch (mode) {
  case ATTACHED_EFFECT_ATTACH:
    Actor_SelectAnimationFar(effect, 1);
    actor->watchedActor = effect;
    effect->timer = 1;
    break;
  case ATTACHED_EFFECT_ONCE:
    Actor_SelectAnimationFar(effect, 2);
    Actor_StartScriptFar(effect, gActor_WaitScript);
    effect->timer = 1;
    break;
  }

  effect->userValue = actorId;
  effect->motionFlags = 0;
  effect->update = (ObjectUpdate)FieldEffect_CopyWatchedActorPosition;
  visual = effect->sprite;
  visual->flags = 0;
  effect->watchedActor = actor;

  // The effect takes sprite priority 1, or the actor's.
  if (flags & ATTACHED_EFFECT_IN_FRONT)
    ((struct PlacedSprite *)visual)->priority = 1;
  else
    ((struct PlacedSprite *)visual)->priority =
        ((struct PlacedSprite *)actor->sprite)->priority;
}

/// The SC_CALL routine of MOVEMENT_TYPE_CLEAR_COLLISION: clears an actor's
/// sprite flags and its collision flags, so nothing collides with it.
s32 Actor_ClearChildAndCollisionFlags(void *obj) {
  Actor_SetSpriteFlagsFar(obj, 0);
  ((struct Actor *)obj)->collisionFlags = 0;
  return 0;
}

/// Actor script hook: clears the actor's sprite flags (outline and shadow).
s32 Actor_ClearChildFlags(struct Actor *obj) {
  Actor_SetSpriteFlagsFar((struct Actor *)obj, 0);
  return 0;
}

/// Actor script hook: the actor takes its cells' priority even where the
/// ground in front rises.
s32 Actor_IgnoreRises(struct Actor *actor) {
  actor->priorityFlags |= ACTOR_PRIORITY_IGNORES_RISES;
  return 0;
}

/// The SC_CALL routine of MOVEMENT_TYPE_FOLLOW_WATCHED: walks toward the
/// watched actor, setting out each frame for 16/timer of the way there, and
/// stands once within its timer's pixels of it.
s32 Actor_MoveTowardTarget(struct Actor *actor) {
  s32 step;
  struct Actor *target;
  FIXED deltaX;
  FIXED deltaZ;
  s32 pixelsX;
  s32 pixelsZ;
  FIXED newX;
  FIXED newZ;
  s32 distance;

  target = actor->watchedActor;
  if (target != 0) {
    // Whole pixels, rounded toward zero as a division by FIXED_ONE would.
    deltaX = target->x.fixed - actor->x.fixed;
    if (deltaX < 0)
      deltaX += FIXED_ONE - 1;
    pixelsX = FIXEDtoINT(deltaX);
    deltaZ = target->z.fixed - actor->z.fixed;
    if (deltaZ < 0)
      deltaZ += FIXED_ONE - 1;
    pixelsZ = FIXEDtoINT(deltaZ);
    distance = Iwram_Sqrt(pixelsX * pixelsX + pixelsZ * pixelsZ);
    step = actor->timer;
    if (distance >= step) {
      // A target 16 pixels on for each pixel of distance, over the step.
      newX = actor->x.fixed + (s32)((u32)pixelsX * toFIXED(16)) / step;
      newZ = actor->z.fixed + (s32)((u32)pixelsZ * toFIXED(16)) / step;
      Actor_SetMoveTargetFar(actor, newX, actor->y.fixed, newZ);
      Actor_SelectAnimationFar(actor, ANIM_WALK);
    } else {
      Actor_SelectAnimationFar(actor, ANIM_STAND);
    }
  }
  return 1;
}

/// The SC_CALL routine of MOVEMENT_TYPE_FACE_LEADER: turns the actor toward its
/// watched actor, at most 1/16 turn a frame.
s32 Actor_TurnTowardWatchedActor(struct Actor *actor) {
  s16 turnStep;
  u16 currentAngle;
  s32 targetAngle;
  struct Actor *target;

  target = actor->watchedActor;
  if (target != NULL) {
    // Clear ACTOR_FACES_MOTION, so moving does not turn it away again.
    actor->steeringFlags &= ~ACTOR_FACES_MOTION;
    targetAngle = (u16)Trig_Atan(target->z.fixed - actor->z.fixed,
                                 target->x.fixed - actor->x.fixed);
    currentAngle = actor->facing;
    turnStep = targetAngle - currentAngle;
    if (turnStep != 0) {
      if (turnStep > FACING_STEP)
        turnStep = FACING_STEP;
      if (turnStep < -FACING_STEP)
        turnStep = -FACING_STEP;
      actor->facing = (currentAngle + turnStep);
    }
  }
  return 1;
}

/// Keeps an actor turning to face another.
void Actor_WatchActor(struct Actor *actor, struct Actor *target) {
  actor->watchedActor = target;
  actor->update = Actor_TurnTowardWatchedActor;
}

/// Gives an actor the leader's sprite priority, so it is drawn in front of
/// and behind what he is, and keeps it there.
s32 Actor_TakeLeaderLayer(struct Actor *actor) {
  struct Actor *leader = Actor_Lookup(gMainRoutineWork.leaderActorId);
  s32 layer = ((struct PlacedSprite *)leader->sprite)->priority;

  if ((actor->spriteStorageKind & 0xf) == ACTOR_SPRITE_ONE) {
    struct PlacedSprite *sprite = actor->sprite;

    sprite->priority = layer;
    sprite->shadowPriority = layer;
    actor->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
  }
  return 0;
}

extern const u8 gMovementType_StandScript[];
extern const u8 gMovementType_WanderScript[];
extern const u8 gMovementType_WanderNearScript[];
extern const u8 gMovementType_WanderCloseScript[];
extern const u8 gMovementType_FollowWatchedScript[];
extern const u8 gMovementType_FaceLeaderScript[];
extern const u8 gMovementType_ClearCollisionScript[];
extern const u8 gMovementType_SquashScript[];
extern const u8 gMovementType_TakeLeaderLayerScript[];
extern const u8 gMovementType_IgnoreRisesScript[];
extern const u8 gMovementType_ClearChildScript[];

/// Starts an actor's movement type: an enum MovementType, or any other value as
/// the actor script to run itself.
void Actor_SetMovementType(struct Actor *actor, s32 kind) {
  switch (kind) {
  case MOVEMENT_TYPE_STAND:
    kind = (s32)gMovementType_StandScript;
    break;
  case MOVEMENT_TYPE_WANDER:
    kind = (s32)gMovementType_WanderScript;
    break;
  case MOVEMENT_TYPE_WANDER_NEAR:
    kind = (s32)gMovementType_WanderNearScript;
    break;
  case MOVEMENT_TYPE_WANDER_CLOSE:
    kind = (s32)gMovementType_WanderCloseScript;
    break;
  case MOVEMENT_TYPE_FOLLOW_WATCHED:
    kind = (s32)gMovementType_FollowWatchedScript;
    break;
  case MOVEMENT_TYPE_FACE_LEADER:
    actor->watchedActor = Actor_GetById(gMainRoutineWork.leaderActorId);
    kind = (s32)gMovementType_FaceLeaderScript;
    break;
  case MOVEMENT_TYPE_CLEAR_COLLISION:
    kind = (s32)gMovementType_ClearCollisionScript;
    break;
  case MOVEMENT_TYPE_SQUASH:
    kind = (s32)gMovementType_SquashScript;
    break;
  case MOVEMENT_TYPE_TAKE_LEADER_LAYER:
    kind = (s32)gMovementType_TakeLeaderLayerScript;
    break;
  case MOVEMENT_TYPE_IGNORE_RISES:
    kind = (s32)gMovementType_IgnoreRisesScript;
    break;
  case MOVEMENT_TYPE_CLEAR_CHILD:
    kind = (s32)gMovementType_ClearChildScript;
    break;
  }
  Actor_StartScriptFar(actor, (const void *)kind);
}

/// Actor_FindNearestFacingTarget's reach in pixels, and the distance past
/// which an actor counts only within 3/16 turn of the facing.
#define FACING_TARGET_REACH 40
#define FACING_TARGET_NEAR 23

/// Finds the nearest animated actor to self within FACING_TARGET_REACH and 48
/// pixels of height, ignoring actors more than 3/16 turn to either side once
/// they are past FACING_TARGET_NEAR; returns it if its first sprite animation
/// is sprite id, as The Broken Seal's.
struct Actor *Actor_FindNearestFacingTarget(struct Actor *self, s32 id) {
  struct Actor *entry;
  struct Actor *found;
  s32 cnt;
  s32 best;
  FIXED dy;
  s32 dx;
  s32 dz;
  s32 dist;
  s32 angle;
  s32 turn;

  found = NULL;
  best = FACING_TARGET_REACH;
  entry = Ram_HeapSlots->actors;
  for (cnt = 0; cnt < ACTOR_COUNT; cnt++, entry++) {
    if (entry->script == NULL)
      continue;
    if (entry == self)
      continue;
    if (entry->spriteStorageKind != ACTOR_SPRITE_ONE)
      continue;
    dy = entry->y.fixed - self->y.fixed;
    if (dy >= 0) {
      if (dy >= toFIXED(48))
        continue;
    } else {
      if (self->y.fixed - entry->y.fixed >= toFIXED(48))
        continue;
    }
    dx = (entry->x.fixed - self->x.fixed) / FIXED_ONE;
    dz = (entry->z.fixed - self->z.fixed) / FIXED_ONE;
    dist = Iwram_Sqrt(dx * dx + dz * dz);
    if (dist >= best)
      continue;
    angle = (u16)Trig_Atan(entry->z.fixed - self->z.fixed,
                           entry->x.fixed - self->x.fixed);
    if (dist > FACING_TARGET_NEAR) {
      turn = (s16)(angle - self->facing);
      if (turn < -(TRIG_TURN * 3 / 16 - 1))
        continue;
      if (turn > TRIG_TURN * 3 / 16 - 1)
        continue;
    }
    found = entry;
    best = dist;
  }
  if (found == NULL)
    return NULL;
  if (((struct Sprite *)found->sprite)->entries[0]->animationId != id)
    return NULL;
  return found;
}

/// Whether the ground at position stops a jump: terrain kind above 2 on a
/// layered map; never on the world map or with no map.
s32 MapGrid_BlocksJump(struct Actor *actor, s32 *position) {
  struct MapWork *map = Ram_HeapSlots->mapGrid;
  struct FieldWork *event = Ram_HeapSlots->fieldWork;

  if (map == 0 || event->mode == ACTOR_SYSTEM_WORLD_MAP ||
      MapGrid_GetTerrainKindFar(actor->layer, position[0], position[2]) <= 2)
    return 0;
  return 1;
}

extern struct MapCell gMapGridCellBuffer[];

/// The middle pixel of the map cell a whole pixel lies in.
#define CELL_MIDDLE(pixels)                                                    \
  (MAPGRID_CELL_PIXELS / 2 + ((pixels) & MAPGRID_CELL_PIXEL_MASK))

/// The leader jumps two cells forward, over the cell in front, when it faces
/// a quarter turn, the map allows it (MapGrid_BlocksJump and the step check)
/// or a platform actor (ACTOR_PRIORITY_PLATFORM) waits 16 pixels below the
/// landing, and no solid actor stands there. A log underfoot (a
/// SPRITE_LOG_WEST_EAST or SPRITE_LOG_NORTH_SOUTH under the party) dips 2
/// pixels with the leader. Each jump adds to the walking distance, and the
/// leader keeps jumping while it lands on CELL_BEHAVIOR_JUMP_FORWARD. Returns
/// 0 after a jump, -1 when the leader cannot jump.
s32 Field_TryJumpForward(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  s32 result = -1;
  s32 angle = leader->facing;
  u8 *flags = &leader->motionFlags;
  u8 saved = *flags;
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  s16 spriteFlags = SPRITE_FLAG_SHADOW;
  FIXED position[3];
  s32 *positionWords;
  struct Actor *entry;
  struct Actor *platform;
  s32 canJump;
  s32 i;

  if (gMainRoutineWork.movementMode == LEADER_MOVEMENT_KEYS)
    return result;
  if (gMainRoutineWork.movementMode == LEADER_MOVEMENT_ROPE)
    return result;
  if ((angle + FACING_STEP) & DEGtoANG(45)) // not facing a quarter turn
    return result;
  angle = (angle + DEGtoANG(45)) & TRIG_TURN * 3 / 4;
  positionWords = position;
again:
  // The middle of the tile in front, then of the tile beyond.
  position[0] = (leader->x.fixed & FIELD_CELL_MASK) + toFIXED(8);
  position[1] = leader->y.fixed;
  position[2] = (leader->z.fixed & FIELD_CELL_MASK) + toFIXED(8);
  Vector_AddPolarOffset(toFIXED(16), angle, position);
  if (MapGrid_CheckCollisionFar(leader, position) == 1)
    return -1;
  position[0] = (leader->x.fixed & FIELD_CELL_MASK) + toFIXED(8);
  position[1] = leader->y.fixed;
  position[2] = (leader->z.fixed & FIELD_CELL_MASK) + toFIXED(8);
  Vector_AddPolarOffset(toFIXED(32), angle, position);
  canJump = 0;
  if (MapGrid_BlocksJump(leader, position) == 0 &&
      MapGrid_CheckCollisionFar(leader, position) == 0)
    canJump = 1;

  // A platform 16 pixels below the landing makes it a jump spot.
  if (leader->priorityFlags & ACTOR_PRIORITY_WALKS_ON_PLATFORMS) {
    entry = Ram_HeapSlots->actors;
    for (i = 0; i < ACTOR_COUNT; i++, entry++) {
      s32 bound = toFIXED(8) - 1;
      FIXED delta;
      FIXED dy;

      if (entry->script == NULL)
        continue;
      if (!(entry->priorityFlags & ACTOR_PRIORITY_PLATFORM))
        continue;
      if (entry->arrivalAxis != 0)
        continue;
      if (entry == leader)
        continue;
      if (FIELD_CELL(position[0] - entry->x.fixed + toFIXED(8)) != 0)
        continue;
      if (FIELD_CELL(position[2] - entry->z.fixed + toFIXED(8)) != 0)
        continue;
      delta = position[1] - entry->y.fixed;
      dy = delta - toFIXED(16);
      if (dy >= 0) {
        if (dy <= bound)
          goto hit;
      } else {
        dy = toFIXED(16) - delta;
        if (dy <= bound) {
        hit:
          canJump = 1;
          break;
        }
      }
    }
  }

  // A solid object at the landing blocks it.
  entry = Ram_HeapSlots->actors;
  for (i = 0; i < ACTOR_COUNT; i++, entry++) {
    if (entry->script != NULL &&
        !(entry->priorityFlags & ACTOR_PRIORITY_PLATFORM) && entry != leader &&
        entry->collisionFlags != 0 &&
        FIELD_CELL(position[0] - entry->x.fixed + toFIXED(8)) == 0 &&
        FIELD_CELL(position[2] - entry->z.fixed + toFIXED(8)) == 0) {
      FIXED d = position[1] - entry->y.fixed;
      if (d >= 0) {
        if (d < toFIXED(8))
          goto blocked;
      } else if (entry->y.fixed - position[1] < toFIXED(8)) {
      blocked:
        canJump = 0;
        break;
      }
    }
  }

  if (canJump) {
    struct MapWork *map;
    u8 attr;

    // Crouch, then jump there.
    if (leader->spriteStorageKind == ACTOR_SPRITE_ONE)
      spriteFlags = ((struct Sprite *)leader->sprite)->flags;
    Event_Start();
    Actor_SelectAnimationFar(leader, ANIM_LEADER_SPRING);
    wait(6);
    Sound_PlayFar(SE_JUMP);
    Actor_SelectAnimationFar(leader, ANIM_LEADER_LEAP);
    leader->speedLimit = toFIXED(3);
    leader->acceleration = toFIXED(2);
    leader->velocityY = toFIXED(4);
    *flags &= ACTOR_JUMP_KEEPS_MOTION;
    Actor_SetSpriteFlagsFar(leader, spriteFlags & (u8)~SPRITE_FLAG_SHADOW);
    Actor_MoveToAndWaitById(gMainRoutineWork.leaderActorId,
                            ((s16 *)positionWords)[1],
                            ((s16 *)positionWords)[5]);
    Actor_SelectAnimationFar(leader, ANIM_LEADER_SPRING);
    Actor_SetSpriteFlagsFar(leader, spriteFlags);

    map = Ram_HeapSlots->mapGrid;
    attr = map->groundEffects;
    if (attr & GROUND_EFFECTS_WADING_MASK) {
      s32 cellX = FIELD_CELL(leader->x.fixed);
      s32 cellZ = FIELD_CELL(leader->z.fixed);
      struct MapCell *cells;

      if (map == NULL)
        cells = gMapGridCellBuffer;
      else
        cells = map->layers[leader->layer & 3].cells;
      cells += cellX + cellZ * MAPGRID_SIZE;
      if (cells->attributeB & MAP_CELL_WATER)
        Actor_SpawnWaterSplashFar(leader, GROUND_EFFECTS_WADING(attr));
    }

    // A log underfoot dips 2 pixels with the leader and comes back up.
    if (((platform = Actor_FindNearestFacingTarget(
              leader, SPRITE_LOG_WEST_EAST)) != NULL ||
         (platform = Actor_FindNearestFacingTarget(
              leader, SPRITE_LOG_NORTH_SOUTH)) != NULL) &&
        (platform->priorityFlags & ACTOR_PRIORITY_UNDERFOOT)) {
      Actor_SelectAnimationFar(platform, 7);
      leader->y.fixed -= toFIXED(1);
      leader->groundHeight -= toFIXED(1);
      wait(2);
      leader->y.fixed -= toFIXED(1);
      leader->groundHeight -= toFIXED(1);
      wait(10);
      leader->y.fixed += toFIXED(1);
      leader->groundHeight += toFIXED(1);
      wait(4);
      leader->y.fixed += toFIXED(1);
      leader->groundHeight += toFIXED(1);
    } else {
      wait(6);
    }
    *flags = saved;
    Event_End();
    if (work != NULL)
      work->walkingDistance +=
          Iwram_MulFX(work->walkingDistanceScale, toFIXED(32));
    if (MapGrid_GetCellBehaviorAtFar(leader->layer, position[0], position[2]) ==
        CELL_BEHAVIOR_JUMP_FORWARD) {
      Actor_SelectAnimationFar(leader, ANIM_STAND);
      wait(6);
      goto again;
    }
    result = 0;
  }
  leader->timer = 8;
  leader->userValue = 0;
  return result;
}

void Actor_WaitForMoveById(s32 id);

/// Climbs down onto the wall below the leader, as The Broken Seal's
/// Field_ClimbDown does, or lets go and falls when it cannot.
s32 Field_ClimbDown(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  struct MapWork *map = Ram_HeapSlots->mapGrid;
  s32 gridX;
  s16 spriteFlags;
  struct MapCell *ground;
  s32 gridY;
  s32 gridZ;
  FIXED position[3];

  spriteFlags = SPRITE_FLAG_SHADOW;
  gridX = CELL_MIDDLE(FIXEDtoINT(leader->x.fixed));
  gridY = CELL_MIDDLE(FIXEDtoINT(leader->y.fixed));
  gridZ = CELL_MIDDLE(FIXEDtoINT(leader->z.fixed));
  Event_Start();
  if (leader->spriteStorageKind == ACTOR_SPRITE_ONE)
    spriteFlags = ((struct Sprite *)leader->sprite)->flags;
  if (gMainRoutineWork.movementMode == LEADER_MOVEMENT_WALK) {
    struct Actor *entry;
    struct MapCell *cell;
    struct MapCell *south;
    s32 cellX = gridX / 16;
    s32 cellY = gridY / 16;
    s32 cellZ = gridZ / 16;
    s32 radius;
    s32 i;

    ground = &map->layers[leader->layer].cells[cellX + cellZ * MAPGRID_SIZE];
    cell = ground;
    south = cell + MAPGRID_SIZE;
    if (!(cell->attributeB & MAP_CELL_CLIMBABLE) ||
        !(south->attributeB & MAP_CELL_CLIMBABLE))
      goto failure;
    cell = &map->layers[MAPGRID_LAYER_OVERHEAD]
                .cells[cellX + (cellZ - cellY) * MAPGRID_SIZE];
    south = cell + MAPGRID_SIZE;
    if (south->collisionCode == CELL_BEHAVIOR_WALL)
      goto failure;
    if (MapGrid_GetTerrainHeightFar(leader->layer, INTtoFIXED(gridX),
                                    INTtoFIXED(gridZ)) <=
        MapGrid_GetTerrainHeightFar(leader->layer, INTtoFIXED(gridX),
                                    INTtoFIXED(gridZ + 16)))
      goto failure;
    position[0] = leader->x.fixed;
    position[1] = leader->y.fixed - toFIXED(16);
    position[2] = leader->z.fixed;
    if (MapGrid_IsWallAtFar(position) != 0)
      goto failure;
    radius = leader->radius - 2;
    position[0] = leader->x.fixed;
    position[1] = leader->y.fixed - toFIXED(32);
    position[2] = leader->z.fixed + toFIXED(16);
    entry = Ram_HeapSlots->actors;
    for (i = 0; i < ACTOR_COUNT; i++, entry++) {
      if (entry->script != NULL && (entry->collisionFlags & ACTOR_SOLID) &&
          entry != leader &&
          Field_CheckSpheresOverlapFar(&entry->x.fixed, radius, position,
                                       entry->radius - 2) >= 0)
        goto failure;
    }

    // Step to the middle of the tile, face north and climb down a tile.
    Actor_MoveToAndWaitById(gMainRoutineWork.leaderActorId, gridX, gridZ);
    leader->speedLimit = toFIXED(1);
    Actor_FaceDirectionById(gMainRoutineWork.leaderActorId, FACING_NORTH, 0);
    Actor_WaitForScriptEndById(gMainRoutineWork.leaderActorId);
    leader->steeringFlags = ACTOR_FACES_MOTION;
    leader->motionFlags = 0;
    spriteFlags &= (u8)~SPRITE_FLAG_SHADOW;
    Actor_SetSpriteFlagsFar(leader, spriteFlags);
    Actor_SelectAnimationFar(leader, ANIM_LEADER_CLIMB_DOWN_FROM_TOP);
    Actor_SetMoveTargetFar(leader, INTtoFIXED(gridX),
                           leader->y.fixed - toFIXED(16),
                           INTtoFIXED(gridZ) + toFIXED(16));
    Actor_WaitForMoveById(gMainRoutineWork.leaderActorId);
    gMainRoutineWork.movementMode = LEADER_MOVEMENT_KEYS;
    leader->z.fixed = (leader->z.fixed & FIELD_CELL_MASK) + toFIXED(8);
  } else {
    // Let go of the wall and fall.
    Actor_SelectAnimationFar(leader, ANIM_LEADER_CLIMB);
    leader->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
    leader->velocityY = toFIXED(4);
    leader->groundHeight = leader->y.fixed;
    spriteFlags |= SPRITE_FLAG_SHADOW;
    Actor_SetSpriteFlagsFar(leader, spriteFlags);
    Event_Wait(6);
    gMainRoutineWork.movementMode = LEADER_MOVEMENT_WALK;
    leader->steeringFlags = ACTOR_FACES_MOTION;
    leader->facing = TRIG_TURN * 3 / 4;
    leader->timer = 0;
    leader->userValue = 0;
  }
  Event_End();
  return 0;

failure:
  Event_End();
  return -1;
}

/// Climbs up the wall in front of the leader, as The Broken Seal's
/// Field_ClimbUp does.
s32 Field_ClimbUp(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  struct MapWork *map = Ram_HeapSlots->mapGrid;
  s16 spriteFlags = SPRITE_FLAG_SHADOW;
  struct MapCell *ground;
  s32 gridX = CELL_MIDDLE(FIXEDtoINT(leader->x.fixed));
  s32 gridY = MAPGRID_CELL_PIXELS / 2 +
              (FIXEDtoINT(leader->y.fixed) & ~(MAPGRID_CELL_PIXELS - 1));
  s32 gridZ = CELL_MIDDLE(FIXEDtoINT(leader->z.fixed));
  FIXED position[3];
  s32 result;

  Event_Start();
  if (leader->spriteStorageKind == ACTOR_SPRITE_ONE)
    spriteFlags = ((struct Sprite *)leader->sprite)->flags;
  if (gMainRoutineWork.movementMode == LEADER_MOVEMENT_WALK) {
    struct MapCell *cell;
    struct MapCell *north;
    s32 cellX = gridX >> 4;
    s32 cellY = gridY >> 4;
    s32 cellZ = gridZ >> 4;

    ground = &map->layers[leader->layer].cells[cellX + cellZ * MAPGRID_SIZE];
    cell = ground;
    north = cell - MAPGRID_SIZE;
    if (!(cell->attributeB & MAP_CELL_CLIMBABLE) ||
        !(north->attributeB & MAP_CELL_CLIMBABLE))
      goto failure;
    cell = &map->layers[MAPGRID_LAYER_OVERHEAD]
                .cells[cellX + (cellZ - cellY) * MAPGRID_SIZE];
    north = cell - MAPGRID_SIZE;
    if (north->collisionCode == CELL_BEHAVIOR_WALL)
      goto failure;
    if (MapGrid_GetTerrainHeightFar(leader->layer, INTtoFIXED(gridX),
                                    INTtoFIXED(gridZ)) >=
        MapGrid_GetTerrainHeightFar(leader->layer, INTtoFIXED(gridX),
                                    INTtoFIXED(gridZ - 16)))
      goto failure;
    position[0] = leader->x.fixed;
    position[1] = leader->y.fixed;
    position[2] = leader->z.fixed;
    result = MapGrid_IsWallAtFar(position);
    if (result != 0)
      goto failure;

    // Step to the middle of the tile, crouch, jump and grab the wall.
    leader->steeringFlags = result;
    Actor_MoveToAndWaitById(gMainRoutineWork.leaderActorId, gridX, gridZ);
    Actor_SelectAnimationFar(leader, ANIM_LEADER_SPRING);
    wait(4);
    Actor_SelectAnimationFar(leader, ANIM_LEADER_LEAP);
    leader->velocityY = toFIXED(4);
    wait(4);
    leader->motionFlags = result;
    spriteFlags &= (u8)~SPRITE_FLAG_SHADOW;
    Actor_SetSpriteFlagsFar(leader, spriteFlags);
    leader->speedLimit = toFIXED(1);
    leader->velocityY = result;
    Actor_SelectAnimationFar(leader, ANIM_LEADER_CLING);
    wait(4);
    gMainRoutineWork.movementMode = LEADER_MOVEMENT_KEYS;
    leader->steeringFlags = ACTOR_FACES_MOTION;
    wait(8);
    leader->z.fixed = (leader->z.fixed & FIELD_CELL_MASK) + toFIXED(8);
  } else {
    struct Actor *entry;
    s32 radius = leader->radius - 2;
    s32 i;

    position[0] = leader->x.fixed;
    position[1] = leader->y.fixed + toFIXED(16);
    position[2] = leader->z.fixed - toFIXED(16);
    i = 0;
    entry = Ram_HeapSlots->actors;
    for (; i < ACTOR_COUNT; i++, entry++) {
      if (entry->script != NULL && (entry->collisionFlags & ACTOR_SOLID) &&
          entry != leader &&
          Field_CheckSpheresOverlapFar(&entry->x.fixed, radius, position,
                                       entry->radius - 2) >= 0)
        goto failure;
    }

    // Climb over the top: up 8 pixels and a tile north.
    leader->motionFlags = 0;
    Actor_SelectAnimationFar(leader, ANIM_LEADER_CLIMB_OVER_TOP);
    Actor_SetMoveTargetFar(leader, INTtoFIXED(gridX),
                           leader->y.fixed + toFIXED(8),
                           INTtoFIXED(gridZ) - toFIXED(16));
    Actor_WaitForMoveById(gMainRoutineWork.leaderActorId);
    leader->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
    leader->groundHeight = leader->y.fixed;
    spriteFlags |= SPRITE_FLAG_SHADOW;
    Actor_SetSpriteFlagsFar(leader, spriteFlags);
    Event_Wait(4);
    gMainRoutineWork.movementMode = LEADER_MOVEMENT_WALK;
    leader->steeringFlags = ACTOR_FACES_MOTION;
    leader->timer = 0;
    leader->userValue = 0;
  }
  Event_End();
  return 0;

failure:
  Event_End();
  return -1;
}

/// How near a rope or tightrope node must be for the leader to hop onto it,
/// in pixels.
#define ROPE_NODE_REACH 32

/// Returns the nearest crossable rope node within reach of position, or NULL.
struct RopeNode *Rope_FindNodeNear(s32 *position) {
  struct RopeNode *found;
  struct RopeNode *result;
  struct RopeWork *work;
  struct RopeNode *node;
  s32 best;
  s32 i;

  found = NULL;
  work = ROPE_WORK;
  if (work == NULL) {
    result = NULL;
  } else {
    node = work->nodes;
    best = ROPE_NODE_DISTANCE_NONE;
    for (i = 0; i < ROPE_NODE_COUNT; i++, node++) {
      if (node->kind == 1) {
        // In pixels, squared.
        s32 dx = (position[0] - node->x) >> 8;
        s32 dy = (position[1] - node->y) >> 8;
        s32 dz = (position[2] - node->z) >> 8;
        s32 distance =
            Iwram_MulFX(dx, dx) + Iwram_MulFX(dy, dy) + Iwram_MulFX(dz, dz);

        if (distance <= ROPE_NODE_REACH * ROPE_NODE_REACH && best > distance) {
          best = distance;
          found = node;
        }
      }
    }
    result = found;
  }
  return result;
}

/// Hops the leader onto the nearest crossable rope node, to cross the rope
/// (LEADER_MOVEMENT_ROPE); returns 0 when none is in reach.
s32 Leader_StepOntoRope(void) {
  struct Actor *leader = Actor_Lookup(gMainRoutineWork.leaderActorId);
  struct Sprite *sprite = leader->sprite;
  struct RopeNode *node = Rope_FindNodeNear(&leader->x.fixed);
  u8 *flags;

  gFieldWork->blockedSteps = 0;
  if (node == NULL)
    return 0;
  Sound_PlayFar(SE_JUMP);
  leader->acceleration = toFIXED(1);
  leader->speedLimit = toFIXED(2);
  leader->velocityY = toFIXED(4);
  leader->watchedActor = (struct Actor *)node;
  Actor_SelectAnimationFar(leader, ANIM_WALK);
  Actor_SetMoveTargetFar(leader, node->x, node->y, node->z);
  Actor_WaitForMoveFar(leader);
  leader->timer = leader->motionFlags;
  flags = &leader->priorityFlags;
  leader->userValue = sprite->flags;
  leader->savedSpritePriority = sprite->image.priority;
  leader->priorityFlags = *flags & ~ACTOR_PRIORITY_AUTOMATIC;
  sprite->flags = 0;
  sprite->image.priority = 1;
  gMainRoutineWork.movementMode = LEADER_MOVEMENT_ROPE;
  return 1;
}

/// Hops the leader off a tied rope a cell in the direction the pad holds,
/// when that cell is open; returns 0 otherwise.
s32 Leader_StepOffRope(void) {
  struct Actor *leader = Actor_Lookup(gMainRoutineWork.leaderActorId);
  struct Sprite *sprite = leader->sprite;
  FIXED position[3];
  u32 angle;

  gFieldWork->blockedSteps = 0;
  angle = gRope_KeyHeadings[DPAD_INDEX(gInput.held)];
  if (angle == DPAD_ANGLE_NONE)
    return 0;
  leader->x.fixed = (leader->x.fixed & FIELD_CELL_MASK) + toFIXED(8);
  leader->z.fixed = (leader->z.fixed & FIELD_CELL_MASK) + toFIXED(8);
  position[0] = leader->x.fixed;
  position[1] = leader->y.fixed;
  position[2] = leader->z.fixed;
  Vector_AddPolarOffset(toFIXED(16), angle, position);
  leader->groundHeight = leader->y.fixed - toFIXED(8);
  if (MapGrid_GetCellAttributeLowNibbleFar(leader->layer, position[0],
                                           position[2]) ||
      MapGrid_CheckCollisionFar(leader, position))
    return 0;
  leader->facing = angle;
  leader->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
  leader->motionFlags = leader->timer;
  leader->scaleX = FIXED_ONE;
  sprite->flags = leader->userValue;
  sprite->rotation = 0;
  Sound_PlayFar(SE_JUMP);
  leader->speedLimit = toFIXED(2);
  leader->velocityY = toFIXED(4);
  leader->acceleration = toFIXED(1);
  Actor_SelectAnimationFar(leader, ANIM_WALK);
  Actor_SetMoveTargetFar(leader, position[0], position[1], position[2]);
  Actor_WaitForMoveFar(leader);
  sprite->image.priority = leader->savedSpritePriority;
  gMainRoutineWork.movementMode = LEADER_MOVEMENT_WALK;
  return 1;
}

/// Returns the nearest crossable tightrope node within reach of position, or
/// NULL.
struct TightropeNode *Tightrope_FindNodeNear(s32 *position) {
  struct TightropeNode *found;
  struct TightropeNode *result;
  struct TightropeWork *work;
  struct TightropeNode *node;
  s32 best;
  s32 i;

  found = NULL;
  work = TIGHTROPE_WORK;
  if (work == NULL) {
    result = NULL;
  } else {
    node = work->nodes;
    best = ROPE_NODE_DISTANCE_NONE;
    for (i = 0; i < ROPE_NODE_COUNT; i++, node++) {
      if (node->kind == 1) {
        // In pixels, squared.
        s32 dx = (position[0] - node->x) >> 8;
        s32 dy = (position[1] - node->y) >> 8;
        s32 dz = (position[2] - node->z) >> 8;
        s32 distance =
            Iwram_MulFX(dx, dx) + Iwram_MulFX(dy, dy) + Iwram_MulFX(dz, dz);

        if (distance <= ROPE_NODE_REACH * ROPE_NODE_REACH && best > distance) {
          best = distance;
          found = node;
        }
      }
    }
    result = found;
  }
  return result;
}

/// Hops the leader onto the nearest crossable tightrope node
/// (LEADER_MOVEMENT_TIGHTROPE); returns 0 when none is in reach.
s32 Leader_StepOntoTightrope(void) {
  struct MainRoutineWork *work = &gMainRoutineWork;
  struct Actor *leader = Actor_Lookup(work->leaderActorId);
  struct Sprite *sprite = leader->sprite;
  struct TightropeNode *node = Tightrope_FindNodeNear(&leader->x.fixed);
  u8 *flags;

  gFieldWork->blockedSteps = 0;
  if (node == NULL)
    return 0;
  Sound_PlayFar(SE_JUMP);
  leader->acceleration = toFIXED(1);
  leader->speedLimit = toFIXED(2);
  leader->velocityY = toFIXED(4);
  leader->watchedActor = (struct Actor *)node;
  Actor_SelectAnimationFar(leader, ANIM_WALK);
  Actor_SetMoveTargetFar(leader, node->x, node->y, node->z);
  Actor_WaitForMoveFar(leader);
  leader->timer = leader->motionFlags;
  leader->userValue = sprite->flags;
  leader->savedSpritePriority = sprite->image.priority;
  flags = &leader->priorityFlags;
  leader->priorityFlags = *flags & ~ACTOR_PRIORITY_AUTOMATIC;
  sprite->flags = 0;
  sprite->image.priority = 1;
  work->movementMode = LEADER_MOVEMENT_TIGHTROPE;
  return 1;
}

/// Hops the leader off a tightrope a cell in the direction the pad holds, when
/// that cell is open; returns 0 otherwise.
s32 Leader_StepOffTightrope(void) {
  struct Actor *leader = Actor_Lookup(gMainRoutineWork.leaderActorId);
  struct Sprite *sprite = leader->sprite;
  FIXED position[3];
  u32 angle;

  gFieldWork->blockedSteps = 0;
  angle = gRope_KeyHeadings[DPAD_INDEX(gInput.held)];
  if (angle == DPAD_ANGLE_NONE)
    return 0;
  leader->x.fixed = (leader->x.fixed & FIELD_CELL_MASK) + toFIXED(8);
  leader->z.fixed = (leader->z.fixed & FIELD_CELL_MASK) + toFIXED(8);
  position[0] = leader->x.fixed;
  position[1] = leader->y.fixed;
  position[2] = leader->z.fixed;
  Vector_AddPolarOffset(toFIXED(16), angle, position);
  leader->groundHeight = leader->y.fixed - toFIXED(8);
  if (MapGrid_GetCellAttributeLowNibbleFar(leader->layer, position[0],
                                           position[2]) ||
      MapGrid_CheckCollisionFar(leader, position))
    return 0;
  leader->facing = angle;
  leader->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
  leader->motionFlags = leader->timer;
  sprite->flags = leader->userValue;
  sprite->rotation = 0;
  Sound_PlayFar(SE_JUMP);
  leader->acceleration = toFIXED(1);
  leader->speedLimit = toFIXED(2);
  leader->velocityY = toFIXED(4);
  Actor_SelectAnimationFar(leader, ANIM_WALK);
  Actor_SetMoveTargetFar(leader, position[0], position[1], position[2]);
  Actor_WaitForMoveFar(leader);
  sprite->image.priority = leader->savedSpritePriority;
  gMainRoutineWork.movementMode = LEADER_MOVEMENT_WALK;
  return 1;
}
