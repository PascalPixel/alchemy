/// @file
/// Moving actors by the direction pad: the party's keys, crossing a rope or a
/// tightrope, and moving as Sand, on a map and on the world map.
// Part of field.c.

#include "field_internal.h"
#include "fixed_math.h"
#include "sounds.h"
#include "gamepak/far_call/field_sequence.h"
#include "field/field.h"
#include "field/resource_slot.h"
#include "macros.h"
#include "field/hanging_vine.h"
#include "field/map.h"
#include "field/object.h"
#include "system/ram_buffer.h"

/// SPRITE_SAND_RIPPLE's animations, the leader as Sand: the mound of sand
/// shifting as it moves, and standing upright as it climbs a face.
#define SAND_ANIMATION_MOVE 1
#define SAND_ANIMATION_CLIMB 3
/// The puff of sand the leader leaves behind, thinning away over 64 frames
/// (SPRITE_SAND_PUFF), and a grain dropping off as he climbs, a cloud that
/// scatters into specks over 48 frames (SPRITE_DUST_CLOUD).
#define SAND_PUFF_ANIMATION_THIN 1
#define DUST_CLOUD_ANIMATION_SCATTER 2

/// Whether the actor can step 8 pixels toward angle: probe is set to the
/// step's place and tested against the map.
#define STEP_IS_CLEAR(actor, probe, angle)                                     \
  (SET_POSITION(probe, (actor)->x.fixed, (actor)->y.fixed, (actor)->z.fixed),  \
   Vector_AddPolarOffset(toFIXED(8), angle, probe),                            \
   !MapGrid_CheckCollision(actor, probe))
/// Sets probe to the place 8 pixels toward angle from an actor climbing a
/// wall, the step's depth turned into height.
#define SET_CLIMB_STEP(probe, actor, angle)                                    \
  (SET_POSITION(probe, (actor)->x.fixed, 0, 0),                                \
   Vector_AddPolarOffset(toFIXED(8), angle, probe),                            \
   (probe)->y = (actor)->y.fixed - (probe)->z, (probe)->z = (actor)->z.fixed)
/// Whether an actor climbing a wall can step 8 pixels toward angle.
#define CLIMB_STEP_IS_CLEAR(actor, probe, angle)                               \
  (SET_CLIMB_STEP(probe, actor, angle), !MapGrid_CheckCollision(actor, probe))

/// SC_KEYS (LEADER_MOVEMENT_KEYS), the leader's control on a climbable wall:
/// moves it 8 pixels at a time by the direction pad, up and down changing its
/// height rather than its depth, reaching east or west as it goes and clinging
/// while the pad is not held. Refuses a step into a wall, from or onto a cell
/// that is not MAP_CELL_CLIMBABLE, or onto ground too high (up) or too low
/// (down), and records the held keys in the field state. Returns 1.
s32 Actor_MoveByKeys(struct Actor *actor) {
  struct Point position;
  struct MapCell *sourceCell;
  struct MapCell *destinationCell;
  struct FieldState *state;
  u16 angle;
  s32 animation;
  s32 collisionFlags;
  s32 deltaZ;
  FIXED terrainHeight;
  struct MapState *map;

  actor->acceleration = toFIXED(1) / 4;
  animation = ANIM_LEADER_CLING;
  actor->speedLimit = toFIXED(1) / 2;
  // DEAD: a debug read of the held keys nothing uses; the ROM keeps the load.
  if (gDebugMode != 0)
    (void)gInput.held;
  angle = gWorldMap_KeyHeadings[DPAD_INDEX(gInput.held)];
  if (angle == WORLD_MAP_KEY_HEADING_NONE) {
    collisionFlags = PLAYER_STEP_STOPPED;
  } else {
    animation = ANIM_LEADER_CLIMB_EAST;
    if (FACING_SIXTEENTH(angle) != FACING_EAST) {
      animation = ANIM_LEADER_CLIMB_WEST;
      if (FACING_SIXTEENTH(angle) != FACING_WEST)
        animation = ANIM_LEADER_CLIMB;
    }
    collisionFlags = 0;
    SET_POSITION(&position, 0, 0, 0);
    Vector_AddPolarOffset(toFIXED(8), angle, &position);
    position.x += actor->x.fixed;
    deltaZ = position.z;
    if (deltaZ < 0)
      actor->facing = TRIG_TURN * 3 / 4;
    if (deltaZ > 0)
      actor->facing = DEGtoANG(90);
    position.y = actor->y.fixed - position.z;
    position.z = actor->z.fixed;
    map = Ram_HeapSlots->mapGrid;
    sourceCell = &map->layers[actor->layer].cells[Actor_GetCellIndexFar(actor)];
    destinationCell = &map->layers[actor->layer]
                           .cells[position.x / toFIXED(16) +
                                  (position.z / toFIXED(16)) * MAPGRID_SIZE];
    if (MapGrid_IsWallAt((s32 *)&position) != 0 ||
        !(sourceCell->attributeB & MAP_CELL_CLIMBABLE) ||
        !(destinationCell->attributeB & MAP_CELL_CLIMBABLE)) {
      collisionFlags = PLAYER_STEP_STOPPED;
      animation = ANIM_LEADER_CLING;
    } else if (gInput.held & KEY_UP) {
      terrainHeight = MapGrid_GetTerrainHeight(actor->layer, position.x,
                                               position.z - toFIXED(16));
      if (terrainHeight - actor->y.fixed < toFIXED(16))
        goto refused;
    } else if (gInput.held & KEY_DOWN) {
      terrainHeight =
          MapGrid_GetTerrainHeight(actor->layer, position.x, position.z);
      if (terrainHeight - actor->y.fixed > -toFIXED(8)) {
      refused:
        collisionFlags = PLAYER_STEP_BLOCKED_BY_MAP;
        animation = ANIM_LEADER_CLING;
      }
    }
  }
  state = Ram_HeapSlots->fieldState;
  if (state != NULL) {
    if (collisionFlags & PLAYER_STEP_BLOCKED)
      state->blockedSteps++;
    else
      state->blockedSteps = 0;
    state->heldKeys = gInput.held;
  }
  Actor_SelectAnimation(actor, animation);
  if (collisionFlags != 0) {
    actor->targetX = ACTOR_NO_TARGET;
    actor->targetY = ACTOR_NO_TARGET;
    actor->targetZ = ACTOR_NO_TARGET;
    actor->velocityX = 0;
    actor->velocityY = 0;
    actor->velocityZ = 0;
  } else {
    Actor_SetMoveTarget(actor, position.x, position.y, position.z);
  }
  actor->scriptCursor++;
  return 1;
}

/// SC_ROPE (LEADER_MOVEMENT_ROPE), the leader crossing a tied rope hand over
/// hand: the pad moves him from node to node along it, a half cell a step, and
/// sets the rope swinging.
s32 Actor_CrossRope(struct Actor *actor) {
  struct Point position;
  s32 collisionFlags;
  struct Sprite *sprite;
  struct RopeState *rope;
  s32 bestDistance;
  s32 animationSpeed;
  s32 speed;
  struct RopeNode *best;
  struct RopeNode *node;
  struct RopeSlot *slot;
  s32 count;
  u16 angle;
  s32 turn;
  struct Actor *first;
  struct Actor *second;
  struct FieldState *state;

  sprite = actor->sprite;
  // The rope slot, read by its number.
  rope = HEAP_SLOT(Ram_HeapSlots, HEAP_SLOT_ROPE);
  speed = FIXED_ONE;
  collisionFlags = 0;
  actor->speedLimit = toFIXED(1) / 2;
  actor->acceleration = toFIXED(1) / 2;
  actor->motionFlags = 0;
  angle = gWorldMap_KeyHeadings[DPAD_INDEX(gInput.held)];
  if (angle == WORLD_MAP_KEY_HEADING_NONE) {
    best = (struct RopeNode *)actor->watchedActor;
    animationSpeed = 0;
  } else {
    animationSpeed = ANIM_STEP_NORMAL;
    SET_POSITION(&position, 0, 0, 0);
    Vector_AddPolarOffset(toFIXED(8), angle, &position);
    position.x += actor->x.fixed;
    position.y = actor->y.fixed - position.z;
    position.z = actor->z.fixed;
    rope = HEAP_SLOT(Ram_HeapSlots, HEAP_SLOT_ROPE);
    node = rope->nodes;
    best = NULL;
    bestDistance = ROPE_NODE_DISTANCE_NONE;
    if (rope == NULL) {
      collisionFlags = PLAYER_STEP_BLOCKED_BY_MAP;
      state = Ram_HeapSlots->fieldState;
      goto report;
    }
    for (count = 0; count < ROPE_NODE_COUNT; count++, node++) {
      if (node->kind == ROPE_NODE_CROSSABLE) {
        s32 dx = (position.x - node->x) >> 8;
        s32 dy = (position.y - node->y) >> 8;
        s32 dz = (position.z - node->z) >> 8;
        s32 distance =
            Iwram_MulFX(dx, dx) + Iwram_MulFX(dy, dy) + Iwram_MulFX(dz, dz);

        if (distance <= 32 * 32 && bestDistance > distance) {
          bestDistance = distance;
          best = node;
        }
      }
    }
  }
  if (best == NULL) {
    collisionFlags |= PLAYER_STEP_BLOCKED_BY_MAP;
  } else {
    slot = &rope->slots[best->slot - 1];
    first = Actor_GetByIdFar(slot->first);
    second = Actor_GetByIdFar(slot->second);
    if (first->x.fixed > second->x.fixed)
      speed = -speed;
    first = Actor_GetByIdFar(slot->first);
    second = Actor_GetByIdFar(slot->second);
    if (first->y.fixed > second->y.fixed) {
      speed = -speed;
      turn = TRIG_TURN * 3 / 4;
    } else {
      turn = DEGtoANG(90);
    }
    actor->scaleX = speed;
    if (best->x == actor->x.fixed && best->y == actor->y.fixed &&
        best->z == actor->z.fixed) {
      collisionFlags = PLAYER_STEP_BLOCKED_BY_MAP;
    } else {
      slot->swinging = 1;
      slot->amplitude += toFIXED(1) / 2;
      if (slot->amplitude > toFIXED(2))
        slot->amplitude = toFIXED(2);
      actor->watchedActor = (union FieldObject *)best;
      SET_POSITION(&position, best->x, best->y, best->z);
      sprite->rotation = best->angle + turn;
    }
  }
  state = Ram_HeapSlots->fieldState;
report:
  if (state != NULL) {
    if (collisionFlags & PLAYER_STEP_BLOCKED)
      state->blockedSteps++;
    else
      state->blockedSteps = 0;
    state->heldKeys = gInput.held;
  }
  Actor_SelectAnimation(actor, ANIM_LEADER_CROSS_ROPE);
  Actor_SetAnimationSpeed(actor, animationSpeed);
  if (collisionFlags != 0) {
    actor->targetX = ACTOR_NO_TARGET;
    actor->targetY = ACTOR_NO_TARGET;
    actor->targetZ = ACTOR_NO_TARGET;
    actor->velocityX = 0;
    actor->velocityY = 0;
    actor->velocityZ = 0;
  } else {
    Actor_SetMoveTarget(actor, position.x, position.y, position.z);
  }
  actor->scriptCursor++;
  return 1;
}

/// Turns the actor toward angle, at most a sixteenth of a turn.
static inline void TurnToward(struct Actor *actor, u16 angle) {
  s16 delta = angle - actor->facing;

  if (delta > TRIG_TURN / 16)
    delta = TRIG_TURN / 16;
  if (delta < -(TRIG_TURN / 16))
    delta = -(TRIG_TURN / 16);
  actor->facing += delta;
}

/// SC_TIGHTROPE (LEADER_MOVEMENT_TIGHTROPE): Actor_CrossRope on a tightrope,
/// which the leader walks, the rope wobbling under him.
s32 Actor_CrossTightrope(struct Actor *actor) {
  struct Point position;
  s32 collisionFlags;
  struct Sprite *sprite;
  s32 animation;
  struct TightropeState *tightrope;
  struct TightropeNode *best;
  struct TightropeNode *point;
  struct TightropeSlot *slot;
  s32 bestDistance;
  u16 angle;
  s32 count;
  struct FieldState *state;

  sprite = actor->sprite;
  animation = ANIM_STAND;
  collisionFlags = 0;
  actor->speedLimit = toFIXED(1) / 2;
  actor->acceleration = toFIXED(1) / 2;
  actor->motionFlags = 0;
  angle = gWorldMap_KeyHeadings[DPAD_INDEX(gInput.held)];
  if (angle == WORLD_MAP_KEY_HEADING_NONE) {
    collisionFlags = PLAYER_STEP_STOPPED;
    best = (struct TightropeNode *)actor->watchedActor;
    SET_POSITION(&position, actor->x.fixed, best->y, actor->z.fixed);
    state = Ram_HeapSlots->fieldState;
  } else {
    SET_POSITION(&position, actor->x.fixed, actor->y.fixed, actor->z.fixed);
    Vector_AddPolarOffset(toFIXED(8), angle, &position);
    // The tightrope slot, read by its number.
    tightrope = HEAP_SLOT(Ram_HeapSlots, HEAP_SLOT_TIGHTROPE);
    point = tightrope->nodes;
    best = NULL;
    bestDistance = ROPE_NODE_DISTANCE_NONE;
    if (tightrope == NULL) {
      collisionFlags = PLAYER_STEP_BLOCKED_BY_MAP;
      state = Ram_HeapSlots->fieldState;
    } else {
      for (count = 0; count < ROPE_NODE_COUNT; count++, point++) {
        if (point->kind == ROPE_NODE_CROSSABLE) {
          s32 dx = (position.x - point->x) >> 8;
          s32 dy = (position.y - point->y) >> 8;
          s32 dz = (position.z - point->z) >> 8;
          s32 distance =
              Iwram_MulFX(dx, dx) + Iwram_MulFX(dy, dy) + Iwram_MulFX(dz, dz);

          if (distance <= 32 * 32 && bestDistance > distance) {
            bestDistance = distance;
            best = point;
          }
        }
      }
      if (best == NULL) {
        collisionFlags |= PLAYER_STEP_BLOCKED_BY_MAP;
      } else if (best->x == actor->x.fixed && best->y == actor->y.fixed &&
                 best->z == actor->z.fixed) {
        collisionFlags = PLAYER_STEP_BLOCKED_BY_MAP;
      } else {
        if (best->slot != 0) {
          slot = &tightrope->slots[best->slot - 1];
          slot->wobbling = 1;
          slot->amplitude += toFIXED(1) / 2;
          if (slot->amplitude > toFIXED(2))
            slot->amplitude = toFIXED(2);
          actor->watchedActor = (union FieldObject *)best;
        }
        /* FAKEMATCH(dead): position.x copied onto itself before the copies. */
        position.x = position.x;
        SET_POSITION(&position, best->x, best->y, best->z);
      }
      state = Ram_HeapSlots->fieldState;
    }
  }
  if (state != NULL) {
    if (collisionFlags & PLAYER_STEP_BLOCKED)
      state->blockedSteps++;
    else
      state->blockedSteps = 0;
    state->heldKeys = gInput.held;
  }
  if (collisionFlags == 0)
    animation = ANIM_WALK;
  Actor_SelectAnimation(actor, animation);
  Actor_SetAnimationSpeed(actor, ANIM_STEP_NORMAL / 2);
  sprite->offsetY = 4;
  if (collisionFlags != 0) {
    actor->targetX = ACTOR_NO_TARGET;
    actor->targetY = ACTOR_NO_TARGET;
    actor->targetZ = ACTOR_NO_TARGET;
    actor->velocityX = 0;
    actor->velocityY = 0;
    actor->velocityZ = 0;
    if (collisionFlags & PLAYER_STEP_BLOCKED)
      TurnToward(actor, angle);
    actor->y.fixed = position.y;
  } else {
    actor->y.fixed = position.y;
    Actor_SetMoveTarget(actor, position.x, position.y, position.z);
    if (actor->x.fixed == position.x && actor->z.fixed == position.z)
      TurnToward(actor, angle);
  }
  actor->scriptCursor++;
  return 1;
}

/// Update of a grain the Sand leaves as the leader moves: it turns and
/// jitters sideways at random.
void Sand_JitterGrain(struct Actor *actor) {
  u32 first;
  u32 second;
  FIXED x;

  ((struct Sprite *)actor->sprite)->rotation += TRIG_TURN / 64;
  first = Random16();
  second = Random16();
  x = actor->x.fixed;

  second >>= 1;
  first >>= 1;
  actor->x.fixed = x + (first - second);
}

/// Whether the player ends Sand: when the Select, A or Start keys are pressed,
/// plays SE_MENU_CANCEL where the leader is blocked (Actor_CheckBlockedFar),
/// and otherwise, while FLAG_SAND_CANNOT_END is clear, asks for Sand again
/// (ABILITY_SAND, packed) and returns 1.
s32 Sand_CheckEndKeys(struct Actor *actor) {
  struct FieldState *state = Ram_HeapSlots->fieldState;
  s32 pressed = 0;

  if (state == NULL)
    return 0;
  if (gInput.pressed & gMainRoutineState.selectKeys)
    pressed = 1;
  else if (gInput.pressed & gMainRoutineState.aKeys)
    pressed = 1;
  else if (gInput.pressed & gMainRoutineState.startKeys)
    pressed = 1;
  if (pressed) {
    if (Actor_CheckBlockedFar(actor)) {
      Sound_PlayFar(SE_MENU_CANCEL);
    } else if (!GameFlag_Get(FLAG_SAND_CANNOT_END)) {
      state->psynergy = ABILITY_SAND | ACT_PACKED;
      return 1;
    }
  }
  return 0;
}

/// SC_SAND (LEADER_MOVEMENT_SAND): the leader turned to sand by the Sand
/// Psynergy, moved by the direction pad on a field map, leaving puffs of sand;
/// up against a face, or down at a drop, he climbs it as sand.
s32 Actor_MoveAsSand(struct Actor *actor) {
  struct Point position;
  struct Point testPosition;
  s16 angleOffsets[10];
  struct Point spawn;
  s32 collision;
  s32 heading;
  FIXED groundHere;
  FIXED y;
  FIXED groundAhead;
  s32 angle;
  s32 cellX;
  s32 cellZ;
  s32 angleIndex;
  s32 count;
  /* FAKEMATCH(temporary): wakeTimer, a pointer to actor->wakeTimer; as the
   * field by name. */
  u8 *wakeTimer;
  struct Actor *effect;
  struct Sprite *sprite;

  heading = 0;
  count = 0;
  cellX = actor->x.fixed & FIELD_CELL_MASK;
  cellZ = actor->z.fixed & FIELD_CELL_MASK;
  y = actor->y.fixed;
  groundHere = MapGrid_GetTerrainHeight(actor->layer, cellX, cellZ);
  groundAhead =
      MapGrid_GetTerrainHeight(actor->layer, cellX, cellZ - toFIXED(16));
  if (actor->timer != 0) {
    if (actor->targetX != ACTOR_NO_TARGET)
      goto done;
    actor->timer = 0;
    actor->groundHeight = groundHere;
  }
  if (actor->userValue != 0) {
    if (groundAhead < y) {
      FIXED topZ = cellZ - toFIXED(8);

      if (MapGrid_CheckCollisionAt(actor->x.fixed, topZ, actor->layer,
                                   actor->groundHeight) == 2) {
        actor->y.fixed -= toFIXED(1);
      } else {
        Actor_SetMoveTarget(actor, actor->x.fixed, groundAhead, topZ);
        actor->timer = 1;
        actor->userValue = 0;
        goto done;
      }
    }
    if (groundHere > y) {
      actor->userValue = 0;
      actor->groundHeight = groundHere;
      actor->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
      goto done;
    }

    collision = 0;
    // DEAD: a debug read of the held keys nothing uses; the ROM keeps the load.
    if (gDebugMode != 0)
      (void)gInput.held;
    actor->speedLimit = toFIXED(1) / 2;
    actor->acceleration = toFIXED(1) / 4;
    angle = gWorldMap_KeyHeadings[DPAD_INDEX(gInput.held)];
    if ((u16)angle == WORLD_MAP_KEY_HEADING_NONE) {
      collision = PLAYER_STEP_STOPPED;
    } else {
      SET_CLIMB_STEP(&position, actor, (u16)angle);
      Actor_SelectAnimation(actor, SAND_ANIMATION_CLIMB);
      if (!(CLIMB_STEP_IS_CLEAR(actor, &testPosition, (u16)angle) &&
            CLIMB_STEP_IS_CLEAR(actor, &testPosition,
                                (u16)angle + FIELD_PROBE_ANGLE) &&
            CLIMB_STEP_IS_CLEAR(actor, &testPosition,
                                (u16)angle - FIELD_PROBE_ANGLE) &&
            CLIMB_STEP_IS_CLEAR(actor, &testPosition,
                                (u16)angle + DEGtoANG(45)) &&
            CLIMB_STEP_IS_CLEAR(actor, &testPosition,
                                (u16)angle - DEGtoANG(45)))) {
        SET_POSITION(&position, actor->x.fixed, actor->y.fixed, actor->z.fixed);
        collision |= PLAYER_STEP_BLOCKED_BY_MAP;
      }
    }
    if (collision != 0) {
      actor->targetX = ACTOR_NO_TARGET;
      actor->targetY = ACTOR_NO_TARGET;
      actor->targetZ = ACTOR_NO_TARGET;
      actor->velocityX = 0;
      actor->velocityZ = 0;
    } else {
      FIXED speed;

      heading = (s16)angle;
      Actor_SetMoveTarget(actor, position.x, position.y, position.z);
      speed = Math_SqrtFX(Iwram_MulFX(actor->velocityX, actor->velocityX) +
                          Iwram_MulFX(actor->velocityZ, actor->velocityZ));
      actor->velocityX = 0;
      actor->velocityZ = 0;
      Vector_AddPolarOffset(speed, (u16)heading,
                            (struct Point *)&actor->velocityX);
      actor->velocityY = -actor->velocityZ;
      actor->velocityZ = 0;
    }
    // Every 16th frame, a grain of sand drops beside the climber.
    if ((gFrameCount & 15) == 0) {
      SET_POSITION(&spawn, actor->x.fixed - Random16() * 6 + toFIXED(3),
                   actor->y.fixed, actor->z.fixed);
      effect = Actor_Create(SPRITE_DUST_CLOUD, spawn.x, spawn.y, spawn.z);
      if (effect != NULL) {
        effect->groundHeight = actor->groundHeight;
        sprite = effect->sprite;
        Actor_StartScript(effect, gSand_GrainScript);
        effect->update = Sand_JitterGrain;
        effect->motionFlags = ACTOR_FALLS;
        if (collision == 0)
          Sound_PlayFar(SE_SAND_MOVE);
        effect->gravity = toFIXED(1) / 25;
        effect->velocityY = 0;
        effect->scaleX = toFIXED(0.3);
        effect->scaleY = toFIXED(0.3);
        if (sprite != NULL) {
          Sprite_StartAnimation(sprite, DUST_CLOUD_ANIMATION_SCATTER);
          sprite->flags = 0;
          sprite->image.priority = 0;
        }
      }
    }
    Sand_CheckEndKeys(actor);
  } else {

    // On foot.
    collision = 0;
    actor->speedLimit = toFIXED(1);
    actor->acceleration = toFIXED(1) / 4;
    angle = gWorldMap_KeyHeadings[DPAD_INDEX(gInput.held)];
    if ((u16)angle == WORLD_MAP_KEY_HEADING_NONE) {
      collision = PLAYER_STEP_STOPPED;
    } else {
      SET_POSITION(&position, actor->x.fixed, actor->y.fixed, actor->z.fixed);
      Vector_AddPolarOffset(toFIXED(8), (u16)angle, &position);
      if (gDebugMode != 0 && (gInput.held & KEY_L)) {
        heading = (s16)angle;
      } else if (!MapGrid_CheckCollision(actor, &position) &&
                 STEP_IS_CLEAR(actor, &testPosition,
                               (u16)angle + FIELD_PROBE_ANGLE) &&
                 STEP_IS_CLEAR(actor, &testPosition,
                               (u16)angle - FIELD_PROBE_ANGLE) &&
                 STEP_IS_CLEAR(actor, &testPosition,
                               (u16)angle + DEGtoANG(45)) &&
                 STEP_IS_CLEAR(actor, &testPosition,
                               (u16)angle - DEGtoANG(45))) {
        heading = (s16)angle;
      } else {
        u16 direction = angle;

        angleOffsets[count++] = direction + FIELD_PROBE_ANGLE;
        angleOffsets[count++] = direction - FIELD_PROBE_ANGLE;
        angleOffsets[count++] = direction + DEGtoANG(45);
        angleOffsets[count++] = direction - DEGtoANG(45);
        angleOffsets[count++] = direction + FIELD_PROBE_ANGLE * 3;
        angleOffsets[count++] = direction - FIELD_PROBE_ANGLE * 3;
        for (angleIndex = 0; angleIndex < count; angleIndex++) {
          heading = angleOffsets[angleIndex];
          if (STEP_IS_CLEAR(actor, &position, (u16)heading) &&
              STEP_IS_CLEAR(actor, &testPosition,
                            (u16)heading + FIELD_PROBE_ANGLE) &&
              STEP_IS_CLEAR(actor, &testPosition,
                            (u16)heading - FIELD_PROBE_ANGLE) &&
              STEP_IS_CLEAR(actor, &testPosition,
                            (u16)heading + DEGtoANG(45)) &&
              STEP_IS_CLEAR(actor, &testPosition, (u16)heading - DEGtoANG(45)))
            goto counted;
        }
        SET_POSITION(&position, actor->x.fixed, actor->y.fixed, actor->z.fixed);
        collision |= PLAYER_STEP_BLOCKED_BY_MAP;
        // Up against a face more than 8 pixels high: start climbing it.
        if ((gInput.held & KEY_UP) && groundAhead > y + toFIXED(8)) {
          cellX = (position.x & FIELD_CELL_MASK) + toFIXED(8);
          cellZ = (position.z & FIELD_CELL_MASK) - toFIXED(8);

          if (MapGrid_IsClimbableCellFar(cellX, cellZ, 2)) {
            actor->motionFlags = 0;
            actor->y.fixed += toFIXED(1);
            actor->userValue = 1;
            actor->velocityX = 0;
            actor->velocityY = 0;
            actor->velocityZ = 0;
            Actor_SelectAnimation(actor, SAND_ANIMATION_CLIMB);
            goto done;
          }
        }
        // Down at a drop: climb down the face south of the cell.
        if (gInput.held & KEY_DOWN) {
          cellX = (actor->x.fixed & FIELD_CELL_MASK) + toFIXED(8);
          cellZ = (actor->z.fixed & FIELD_CELL_MASK) + toFIXED(24);

          if (MapGrid_GetTerrainHeight(actor->layer, cellX, cellZ) < y &&
              MapGrid_IsClimbableCellFar(cellX, cellZ, 2)) {
            actor->motionFlags = 0;
            Actor_SetMoveTarget(actor, actor->x.fixed, groundHere - toFIXED(2),
                                cellZ);
            actor->timer = 1;
            actor->userValue = 1;
            actor->velocityX = 0;
            actor->velocityY = 0;
            actor->velocityZ = 0;
            Actor_SelectAnimation(actor, SAND_ANIMATION_CLIMB);
            goto done;
          }
        }
      }
    }
  counted: {
    struct FieldState *state = Ram_HeapSlots->fieldState;

    if (state != NULL) {
      if (collision & PLAYER_STEP_BLOCKED)
        state->blockedSteps++;
      else
        state->blockedSteps = 0;
      state->heldKeys = gInput.held;
    }
  }
    Actor_SelectAnimation(actor, SAND_ANIMATION_MOVE);
    if (collision != 0) {
      actor->targetX = ACTOR_NO_TARGET;
      actor->targetY = ACTOR_NO_TARGET;
      actor->targetZ = ACTOR_NO_TARGET;
      actor->velocityX = 0;
      actor->velocityZ = 0;
      if (collision & PLAYER_STEP_BLOCKED) {
        s32 difference;
        u16 turn = angle;

        difference = (s16)(turn - actor->facing);
        if (difference > FIELD_BLOCKED_TURN_MAX)
          difference = FIELD_BLOCKED_TURN_MAX;
        if (difference < -FIELD_BLOCKED_TURN_MAX)
          difference = -FIELD_BLOCKED_TURN_MAX;
        actor->facing += difference;
      }
      wakeTimer = &actor->wakeTimer;
      *wakeTimer = 0;
    } else {
      FIXED speed;

      Actor_SetMoveTarget(actor, position.x, position.y, position.z);
      speed = Math_SqrtFX(Iwram_MulFX(actor->velocityX, actor->velocityX) +
                          Iwram_MulFX(actor->velocityZ, actor->velocityZ));
      actor->velocityX = 0;
      actor->velocityZ = 0;
      Vector_AddPolarOffset(speed, (u16)heading,
                            (struct Point *)&actor->velocityX);
      wakeTimer = &actor->wakeTimer;
      if (*wakeTimer != 0)
        (*wakeTimer)--;
    }
    // While the leader moves, a puff of sand is left behind every fifth
    // frame.
    if (actor->wakeTimer == 0 && collision == 0) {
      effect = Actor_Create(SPRITE_SAND_PUFF, actor->x.fixed, actor->y.fixed,
                            actor->z.fixed - toFIXED(2));
      if (effect != NULL) {
        effect->groundHeight = actor->groundHeight;
        sprite = effect->sprite;
        Actor_StartScript(effect, gActor_WakeScript);
        effect->priorityFlags = ACTOR_PRIORITY_OWN_DEPTH;
        effect->wakeFrames = 100;
        effect->motionFlags = 0;
        Sound_PlayFar(SE_SAND_MOVE);
        if (sprite != NULL) {
          struct FieldState *state = Ram_HeapSlots->fieldState;

          Sprite_StartAnimation(sprite, SAND_PUFF_ANIMATION_THIN);
          sprite->flags = 0;
          // DEAD: overwritten by the next line; the ROM keeps its mask and OR.
          sprite->image.priority = 2;
          sprite->image.priority = state->sandTrailPriority;
        }
        *wakeTimer = 5;
      }
    }
    Sand_CheckEndKeys(actor);
  }
done:
  actor->scriptCursor++;
  return 1;
}

/// SC_WORLD_SAND (LEADER_MOVEMENT_WORLD_SAND): the leader as sand on the world
/// map (the Sand Psynergy). Moves him on the world map by the direction pad
/// at a fixed pace (faster while L repeats), trying six turned headings when
/// the way is blocked, as Actor_MoveOnWorldMap does; records the held keys in
/// the event state and leaves a puff of sand (SPRITE_SAND_PUFF) behind it every
/// fifth frame it moves.
s32 Actor_MoveAsSandOnWorldMap(struct Actor *actor) {
  struct Point position;
  struct Point leftPosition;
  struct Point rightPosition;
  s16 angleOffsets[22];
  s32 collision;
  s32 angle;
  u16 direction;
  s32 angleIndex;
  s32 count;
  s32 difference;
  s32 terrainCollision;
  FIXED speed;
  s32 targetCameraYaw;
  struct WorldMapView *camera;
  s32 yawStep;
  struct PlayerSprite *playerSprite;
  struct Actor *ripple;
  struct PlayerSprite *rippleSprite;
  struct FieldState *state;

  collision = 0;
  count = 0;
  actor->userValue = 0;
  actor->timer = 1;
  actor->speedLimit = toFIXED(1) / 2;
  actor->acceleration = toFIXED(1) / 4;
  if (gInput.repeat & KEY_L)
    actor->speedLimit = toFIXED(4);

  angle = gWorldMap_KeyHeadings[DPAD_INDEX(gInput.held)];
  direction = angle;
  if (direction == WORLD_MAP_KEY_HEADING_NONE) {
    collision |= PLAYER_STEP_STOPPED;
  } else {
    SET_POSITION(&position, actor->x.fixed, actor->y.fixed, actor->z.fixed);
    Vector_AddPolarOffset(toFIXED(8), direction, &position);
    if (gDebugMode != 0 && (gInput.held & KEY_L)) {
      /* FAKEMATCH(dead): direction takes angle, which it already holds (set
       * just above), the debug branch's only statement; as the branch left
       * empty. */
      direction = angle;
    } else {
      SET_POSITION(&leftPosition, actor->x.fixed, actor->y.fixed,
                   actor->z.fixed);
      Vector_AddPolarOffset(toFIXED(8), direction + DEGtoANG(30),
                            &leftPosition);
      SET_POSITION(&rightPosition, actor->x.fixed, actor->y.fixed,
                   actor->z.fixed);
      Vector_AddPolarOffset(toFIXED(8), direction - DEGtoANG(30),
                            &rightPosition);
      if ((WorldMap_IsWalkable(actor, &position) |
           WorldMap_IsWalkable(actor, &leftPosition) |
           WorldMap_IsWalkable(actor, &rightPosition)) != 0) {
        angleOffsets[count++] = direction + FIELD_PROBE_ANGLE;
        angleOffsets[count++] = direction - FIELD_PROBE_ANGLE;
        angleOffsets[count++] = direction + DEGtoANG(45);
        angleOffsets[count++] = direction - DEGtoANG(45);
        angleOffsets[count++] = direction + FIELD_PROBE_ANGLE * 3;
        angleOffsets[count++] = direction - FIELD_PROBE_ANGLE * 3;
        for (angleIndex = 0; angleIndex < count; angleIndex++) {
          direction = angleOffsets[angleIndex];
          SET_POSITION(&position, actor->x.fixed, actor->y.fixed,
                       actor->z.fixed);
          Vector_AddPolarOffset(toFIXED(8), direction, &position);
          SET_POSITION(&leftPosition, actor->x.fixed, actor->y.fixed,
                       actor->z.fixed);
          Vector_AddPolarOffset(toFIXED(8), direction + DEGtoANG(30),
                                &leftPosition);
          SET_POSITION(&rightPosition, actor->x.fixed, actor->y.fixed,
                       actor->z.fixed);
          Vector_AddPolarOffset(toFIXED(8), direction - DEGtoANG(30),
                                &rightPosition);
          if ((WorldMap_IsWalkable(actor, &position) |
               WorldMap_IsWalkable(actor, &leftPosition) |
               WorldMap_IsWalkable(actor, &rightPosition)) == 0) {
            angle = (s16)direction;
            goto update_object;
          }
        }
        collision |= PLAYER_STEP_BLOCKED_BY_MAP;
      }
    }
  }

update_object:
  state = Ram_HeapSlots->fieldState;
  if (state != NULL) {
    if ((collision & PLAYER_STEP_BLOCKED) != 0)
      state->blockedSteps++;
    else
      state->blockedSteps = 0;
    state->heldKeys = gInput.held;
  }
  Actor_SelectAnimation(actor, SAND_ANIMATION_MOVE);

  if (collision != 0) {
    actor->targetX = ACTOR_NO_TARGET;
    actor->targetY = ACTOR_NO_TARGET;
    actor->targetZ = ACTOR_NO_TARGET;
    actor->velocityX = 0;
    actor->velocityZ = 0;
    if ((collision & PLAYER_STEP_BLOCKED) != 0) {
      direction = angle;
      difference = (s16)(direction - actor->facing);
      if (difference > FIELD_BLOCKED_TURN_MAX)
        difference = FIELD_BLOCKED_TURN_MAX;
      if (difference < -FIELD_BLOCKED_TURN_MAX)
        difference = -FIELD_BLOCKED_TURN_MAX;
      actor->facing += difference;
    }
    actor->wakeTimer = 0;
    actor->userValue = FOOTPRINT_BOTH_FEET;
  } else {
    Actor_SetMoveTarget(actor, position.x, position.y, position.z);
    speed = Math_SqrtFX(Iwram_MulFX(actor->velocityX, actor->velocityX) +
                        Iwram_MulFX(actor->velocityZ, actor->velocityZ));
    actor->velocityX = 0;
    actor->velocityZ = 0;
    Vector_AddPolarOffset(speed, (u16)angle, (struct Point *)&actor->velocityX);
    if (actor->wakeTimer != 0)
      actor->wakeTimer--;
  }

  camera = (struct WorldMapView *)Ram_HeapSlots->mapGrid;
  targetCameraYaw = gWorldMap_KeyYaws[DPAD_INDEX(gInput.held)];
  yawStep = (s16)(targetCameraYaw - camera->yaw) / 8;
  if (yawStep > WORLD_MAP_YAW_STEP_MAX)
    yawStep = WORLD_MAP_YAW_STEP_MAX;
  if (yawStep < -WORLD_MAP_YAW_STEP_MAX)
    yawStep = -WORLD_MAP_YAW_STEP_MAX;
  if (yawStep > -WORLD_MAP_YAW_SNAP && yawStep < WORLD_MAP_YAW_SNAP)
    yawStep = targetCameraYaw - camera->yaw;
  camera->yaw += yawStep;

  if (actor->spriteStorageKind == ACTOR_SPRITE_ONE) {
    playerSprite = actor->sprite;
    terrainCollision =
        WorldMap_GetCollision((struct WorldPosition *)&actor->x.fixed);
    if (terrainCollision == WORLD_MAP_COLLISION_FOREST)
      playerSprite->secondLayer->priority = 1;
    else
      playerSprite->secondLayer->priority = SPRITE_LAYER_NOT_DRAWN;
  }
  if (actor->wakeTimer == 0 && collision == 0) {
    ripple = Actor_Create(SPRITE_SAND_PUFF, actor->x.fixed, actor->y.fixed,
                          actor->z.fixed - toFIXED(2));
    if (ripple != NULL) {
      ripple->groundHeight = actor->groundHeight;
      rippleSprite = ripple->sprite;
      Actor_StartScript(ripple, gActor_WakeScript);
      ripple->priorityFlags = ACTOR_PRIORITY_OWN_DEPTH;
      ripple->wakeFrames = 100;
      ripple->motionFlags = 0;
      if (rippleSprite != NULL) {
        Sprite_StartAnimation(rippleSprite, SAND_PUFF_ANIMATION_THIN);
        rippleSprite->flags = 0;
        // DEAD: overwritten by the next line; the ROM keeps its mask and OR.
        rippleSprite->priority = 2;
        rippleSprite->priority = 1;
      }
      actor->wakeTimer = 5;
    }
  }
  Sand_CheckEndKeys(actor);
  actor->scriptCursor++;
  return 1;
}

#undef STEP_IS_CLEAR
#undef SET_CLIMB_STEP
#undef CLIMB_STEP_IS_CLEAR
