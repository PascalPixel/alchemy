/// @file
/// The party's sprites, placing a scene's actors, and the actor
/// table.
// Part of field_sequence.c.

#include "field_sequence_internal.h"
#include "djinn_ids.h"
#include "flags.h"
#include "fixed_math.h"
#include "field/actor_view.h"
#include "field/field_sequence.h"
#include "field/object.h"
#include "gamepak/far_call/field.h"
#include "system/ram_buffer.h"
#include "macros.h"

enum {
  /// DJINN_PER_ELEMENT flags per element: the Djinn flags are 48 to 127, their
  /// hiding flags the next 80.
  DJINN_FLAG_COUNT = ELEMENT_COUNT * DJINN_PER_ELEMENT
};

/// A placement row's sprite: below PLACEMENT_PARTY_COUNT a party member, who
/// keeps that actor id; one up to PLACEMENT_SPRITE_MAX takes the next free
/// slot, and one above it, which no table names, takes the slot of the row
/// before.
#define PLACEMENT_PARTY_COUNT 8
#define PLACEMENT_SPRITE_MAX 9989
/// A placement's character after the party: the ship.
#define PLACEMENT_SHIP 8

/// The ids Actor_Lookup answers: more than the FIELD_ACTOR_COUNT slots.
#define LOOKUP_ID_COUNT 192

/// Picks the sprite a scene placement's character id shows: the ship
/// (PLACEMENT_SHIP) is SPRITE_WINGED_SHIP_SMALL on the world map and
/// SPRITE_WINGED_SHIP elsewhere, each one's flying picture once FLAG_SHIP_FLIES
/// is set.
s32 Party_GetCharacterSprite(s32 character) {
  s32 sprite;

  sprite = character;
  if (sprite <= PLACEMENT_SHIP) {
    if (sprite == PLACEMENT_SHIP) {
      sprite = SPRITE_WINGED_SHIP_SMALL;
      if (gMainRoutineState.mapId != SCENE_CLEAR)
        sprite = SPRITE_WINGED_SHIP;
      if (GameFlag_Get(FLAG_SHIP_FLIES))
        sprite++;
    }
  }
  return sprite;
}

/// Does nothing; nothing calls Actor_ReservedNoOpFar.
UNUSED void Actor_ReservedNoOp(void) {}

/// Creates the actors of a scene's actor placement table (ended by sprite -1),
/// and records the table among the scene's tables.
void Event_SpawnActorTable(struct ActorPlacement *entry, s32 slot) {
  struct FieldState *state = Ram_HeapSlots->fieldState;
  s32 actorIndex = 0;
  s32 i;
  u16 placementSprite;

  // Remember the table once, before consuming slots for its actor placements.
  for (i = 0; i < 4; i++) {
    if (state->tables[i] == entry)
      break;
    if (state->tables[i] == NULL) {
      state->tables[i] = entry;
      break;
    }
  }
  for (placementSprite = entry->sprite;
       (s16)placementSprite != -1 && slot <= FIELD_TREASURE_ACTOR - 1;
       entry++, placementSprite = entry->sprite) {
    struct Actor *actor;
    struct Sprite *sprite;
    s32 condition;
    s32 spriteId;

    if (entry->sprite <= PLACEMENT_PARTY_COUNT - 1)
      actorIndex = entry->sprite;
    else if (entry->sprite <= PLACEMENT_SPRITE_MAX)
      actorIndex = slot++;
    condition = entry->condition;
    if (!GameFlag_IsConditionActive(condition))
      continue;
    if ((u32)(condition - FLAG_DJINN_FIRST) <= DJINN_FLAG_COUNT - 1) {
      s32 element = (condition - FLAG_DJINN_FIRST) / DJINN_PER_ELEMENT;

      // The last flag of an element stands for the Djinni the scene chose
      // for it, numbered from 1; with none chosen the actor is left out.
      if ((condition - FLAG_DJINN_FIRST) % DJINN_PER_ELEMENT ==
          DJINN_PER_ELEMENT - 1) {
        s32 member = gSceneLocalState[element + SCENE_STATE_CHOSEN_DJINN];

        if (member == 0)
          continue;
        condition = element * DJINN_PER_ELEMENT + member + FLAG_DJINN_FIRST - 1;
        if (!GameFlag_IsConditionActive(condition))
          continue;
      }
      // Djinn_ResolvePending hides a Djinni after the party loses or flees.
      if (state->mode != ACTOR_SYSTEM_WORLD_MAP &&
          !GameFlag_IsConditionActive(condition + FLAG_DJINN_HIDING_FIRST -
                                      FLAG_DJINN_FIRST))
        continue;
    }
    spriteId = Party_GetCharacterSprite(entry->sprite);
    actor = Actor_Lookup(actorIndex);
    if (actor == NULL) {
      // ACTOR_PLACEMENT_AT_SCENE_POINT: x is the number of a scene point to
      // stand on.
      if (entry->flags & ACTOR_PLACEMENT_AT_SCENE_POINT) {
        s32 position[3];

        position[0] = (s32)actor;
        position[1] = (s32)actor;
        position[2] = (s32)actor;
        ScenePoint_GetPosition(entry->x, position);
        actor = Actor_CreateFar(spriteId, position[0], 0, position[2]);
      } else {
        actor = Actor_CreateFar(spriteId, entry->x, entry->y, entry->z);
      }
      // ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE: share the previous actor's
      // sprite resource.
      if (entry->flags & ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE) {
        struct Actor *previous = Actor_Lookup(actorIndex - 1);

        if (previous->spriteStorageKind == ACTOR_SPRITE_ONE &&
            actor->spriteStorageKind == ACTOR_SPRITE_ONE) {
          u8 spriteSlot;

          sprite = previous->sprite;
          sprite->retainCacheEntry = 1;
          spriteSlot = sprite->slot;
          sprite = actor->sprite;
          sprite->retainCacheEntry = 1;
          VramCache_FreeEntry(sprite->slot);
          sprite->slot = spriteSlot;
        }
      }
    } else {
      if (entry->sprite == gMainRoutineState.leaderActorId)
        continue;
      Actor_SetPositionAndResetMotionFar(actor, entry->x, entry->y, entry->z);
    }
    if (actor != NULL) {
      sprite = actor->sprite;
      Actor_SelectAnimationFar(actor, ANIM_STAND);
      // Each layer starts its first picture 0 to 4 sixteenths of a frame
      // apart, so actors placed together do not move in step.
      if (actor->spriteStorageKind == ACTOR_SPRITE_ONE && sprite != NULL) {
        for (i = 0; i < sprite->count; i++) {
          struct SpriteAnimation *layer = sprite->entries[i];

          if (layer != NULL)
            layer->timer = (Random16() * 5) >> 15;
        }
        sprite->dirty = 1;
      }
      actor->facing = entry->facing;
      actor->collisionFlags = ACTOR_SOLID;
      Actor_SetMovementType(actor, entry->behavior);
      actor->homeX = actor->x.fixed / FIXED_ONE;
      actor->homeZ = actor->z.fixed / FIXED_ONE;
      // An actor placement with a height is lifted half a pixel more and
      // floats: it neither follows the ground nor falls.
      if (actor->y.fixed != 0) {
        actor->motionFlags = ACTOR_FLOATS;
        actor->y.fixed += toFIXED(1) / 2;
      }
      if (state->mode == ACTOR_SYSTEM_WORLD_MAP) {
        actor->motionFlags &= ~ACTOR_FOLLOWS_TERRAIN;
        if (!GameFlag_Get(FLAG_ISAAC_WEARS_TUNIC))
          sprite->scale = Iwram_MulFX(sprite->scale, FIXED_ONE * 3 / 4);
      } else {
        actor->groundHeight =
            MapGrid_GetTerrainHeightFar(0, actor->x.fixed, actor->z.fixed);
        actor->y.fixed += actor->groundHeight;
      }
      actor->priorityFlags = ACTOR_PRIORITY_AUTOMATIC;
    }
    state->actors[actorIndex] = actor;
  }
}

/// Looks point id up in the map's scene points (its scene_points.tsv, ended by
/// id 0xff) and writes its x and z, in 16.16 pixels, to position; returns 0,
/// or -1 when the map has no such point.
s32 ScenePoint_GetPosition(s32 id, FIXED *position) {
  u8 *cursor = Ram_MapScenePoints;
  s8 key;

  key = *cursor++;
  while (key != -1) {
    if (key == id) {
      position[0] = toFIXED(Stream_ReadU16(&cursor));
      position[2] = toFIXED(Stream_ReadU16(&cursor));
      return 0;
    }
    cursor += 4;
    key = *cursor++;
  }
  return -1;
}

/// Releases the actor in slot actorIndex of the field state's table, if there
/// is one, and empties the slot.
void Actor_ReleaseAtIndex(u32 actorIndex) {
  struct FieldState *state = gFieldState;
  struct Actor *actor = Actor_Lookup(actorIndex);

  if (actor != NULL) {
    Actor_ReleaseFar(actor);
    state->actors[actorIndex] = NULL;
  }
}

u8 *Sprite_AddAnimationFar(void *resource, s32 id);

/// Creates a scene's actors on entering it, as The Broken Seal's
/// Actor_ResetForLeader does: the party leader where the main routine's state
/// puts it, the scene's placement table from actor slot 8, and the camera
/// that follows the leader; it also chooses the leader's control mode.
void Actor_ResetForLeader(s32 table) {
  struct FieldState *state;
  struct ActorPlacement *entry;
  struct Actor *actor;
  struct Actor *camera;
  struct MapCell *cell;
  struct MapCell *above;
  s32 leader;
  s32 i;
  s32 height;

  state = gFieldState;
  leader = gMainRoutineState.leaderActorId;
  entry = state->leaderPlacements;
  entry[0] = gField_DefaultPlacements[0];
  entry[1] = gField_DefaultPlacements[1];
  for (i = 0; i < 4; i++)
    state->tables[i] = 0;
  ActorTable_Clear();
  entry->condition = -1;
  entry->sprite = leader;
  SET_POSITION(entry, gMainRoutineState.x, 0, gMainRoutineState.z);
  entry->facing = gMainRoutineState.heading;
  Event_SpawnActorTable(entry, leader);
  {
    FIXED y = gMainRoutineState.y;

    if (y != 0) {
      actor = state->actors[leader];
      actor->y.fixed = y;
      actor->groundHeight = y;
    }
  }
  Event_SpawnActorTable((struct ActorPlacement *)table, ACTOR_FIRST_PLACED);

  actor = state->actors[leader];
  actor->layer = gMainRoutineState.layer;
  actor->timer = 0;
  actor->userValue = 0;
  if (state->mode == ACTOR_SYSTEM_WORLD_MAP) {
    if (gMainRoutineState.ownerOverride != 0)
      gMainRoutineState.movementMode = LEADER_MOVEMENT_SHIP;
    else
      gMainRoutineState.movementMode = LEADER_MOVEMENT_WORLD_MAP;
  } else if (gMainRoutineState.ownerOverride != 0) {
    gMainRoutineState.movementMode = LEADER_MOVEMENT_SHIP_ON_MAP;
  } else {
    gMainRoutineState.movementMode = LEADER_MOVEMENT_WALK;
    cell = ((struct MapState *)Ram_HeapSlots->mapGrid)
               ->layers[actor->layer]
               .cells +
           ((actor->x.fixed / toFIXED(MAPGRID_CELL_PIXELS)) +
            ((actor->z.fixed / toFIXED(MAPGRID_CELL_PIXELS)) * MAPGRID_SIZE));
    above = cell - MAPGRID_SIZE;
    if (gMainRoutineState.y != 0 && (cell->attributeB & MAP_CELL_CLIMBABLE) &&
        (above->attributeB & MAP_CELL_CLIMBABLE)) {
      gMainRoutineState.movementMode = LEADER_MOVEMENT_KEYS;
      height = MapGrid_GetTerrainHeightFar(0, actor->x.fixed,
                                           actor->z.fixed - toFIXED(16)) -
               toFIXED(32);
      actor->y.fixed += height;
      actor->groundHeight = actor->y.fixed;
      actor->motionFlags = 0;
      Actor_SetSpriteFlagsFar(actor, 0);
      Actor_SelectAnimationFar(actor, ANIM_LEADER_CLING);
    }
  }

  if (gMainRoutineState.ownerOverride != 0) {
    struct Actor *stand = state->actors[ACTOR_FIRST_PLACED];

    SET_ACTOR_POSITION(stand, actor->x.fixed, actor->y.fixed, actor->z.fixed);
    stand->groundHeight = actor->groundHeight;
    stand->facing = actor->facing;
    actor->x.fixed = 0;
    actor->z.fixed = 0;
    actor = stand;
  }

  camera = Actor_CreateFar(ACTOR_CREATE_KIND(ACTOR_CREATE_NO_SPRITE),
                           actor->x.fixed, actor->y.fixed, actor->z.fixed);
  camera->groundHeight = actor->groundHeight;
  Actor_StartCameraFar(camera, actor);
  // On the World Map the leader takes a second layer, kept undrawn until
  // Actor_MoveOnWorldMap draws it in a forest.
  if (state->mode == ACTOR_SYSTEM_WORLD_MAP) {
    struct SpriteAnimation *secondLayer =
        (struct SpriteAnimation *)Sprite_AddAnimationFar(actor->sprite,
                                                         SPRITE_BLANK_32);

    secondLayer->colorMode = SPRITE_COLOR_HIDDEN;
    secondLayer->priority = SPRITE_LAYER_NOT_DRAWN;
  }
  ((struct MapState *)Ram_HeapSlots->mapGrid)->origin = &camera->x.fixed;
  state->camera = camera;
}

/// Returns the slot after the last placed actor (8 when there is none), or -1
/// when the last slot is occupied.
s32 Actor_FindLastActiveId(void) {
  struct FieldState *state = Ram_HeapSlots->fieldState;
  s32 last = ACTOR_FIRST_PLACED - 1;
  s32 i;

  for (i = ACTOR_FIRST_PLACED; i <= FIELD_TREASURE_ACTOR - 1; i++) {
    if (state->actors[i] != NULL)
      last = i;
  }
  last++;
  if (last == FIELD_TREASURE_ACTOR)
    last = -1;
  return last;
}

#define FLAG_IN_VIEW 0x164     ///< kept clear, so a placement using it appears
#define FLAG_OUT_OF_VIEW 0x165 ///< kept set, so a placement using it does not

/// Gives each actor placement with condition 0 a condition saying whether
/// it lies within the map's view bounds, so that only those inside appear.
void Scene_AssignViewFlags(struct ViewedPlacement *entry) {
  struct MapState *view = Ram_HeapSlots->mapGrid;

  GameFlag_Clear(FLAG_IN_VIEW);
  GameFlag_Set(FLAG_OUT_OF_VIEW);
  while (entry->sprite != -1) {
    s32 x;
    s32 z;

    // A placement with a flag of its own keeps it.
    if (entry->condition != 0) {
      entry++;
      continue;
    }
    x = entry->x;
    z = entry->z;
    if (view->minX <= x && x <= view->maxX && view->minZ <= z &&
        z <= view->maxZ)
      entry->condition = FLAG_IN_VIEW;
    else
      entry->condition = FLAG_OUT_OF_VIEW;
    entry++;
  }
}

/// Empties all the actor slots, without releasing the actors.
void ActorTable_Clear(void) {
  struct FieldState *state = Ram_HeapSlots->fieldState;
  s32 i;

  for (i = 0; i < FIELD_ACTOR_COUNT; i++)
    state->actors[i] = NULL;
}

/// Returns the actor in slot actorId (below LOOKUP_ID_COUNT), or NULL.
struct Actor *Actor_Lookup(u32 actorId) {
  struct FieldState *state = Ram_HeapSlots->fieldState;

  if (actorId > LOOKUP_ID_COUNT - 1)
    return NULL;
  // UB: ids from FIELD_ACTOR_COUNT to LOOKUP_ID_COUNT - 1 read past actors[],
  // into the field state's requests; UBFIX returns NULL for them.
#ifdef UBFIX
  if (actorId > FIELD_ACTOR_COUNT - 1)
    return NULL;
#endif
  return state->actors[actorId];
}

/// Saves up to 32 actors into gSnapshotObjects, with each one's slot,
/// animation, sprite flags and priority, so that they can be put back.
void ActorTable_Snapshot(void) {
  struct Actor *copy = gSnapshotObjects;
  u8 *indices = (u8 *)gSnapshotObjects - ACTOR_SNAPSHOT_CAPACITY;
  u8 *animations = (u8 *)&gSnapshotObjects[ACTOR_SNAPSHOT_CAPACITY];
  u8 *spriteFlags = animations + ACTOR_SNAPSHOT_CAPACITY;
  u8 *priorities = animations + 2 * ACTOR_SNAPSHOT_CAPACITY;
  u32 count = 0;
  struct Actor *actor;
  struct Sprite *sprite;
  s32 i;
  u32 animation;
  u32 flags;
  u32 priority;
  s32 limit = FIELD_ACTOR_COUNT;

  for (i = 0; i < limit; i++) {
    actor = Actor_Lookup(i);
    if (actor == NULL)
      continue;
    *indices++ = i;
    Dma_Set(actor, copy, DMA_COPY_WORDS(sizeof(struct Actor) / 4), REG_DMA3);
    if (actor->spriteStorageKind == ACTOR_SPRITE_ONE) {
      sprite = actor->sprite;
      animation = sprite->lastNo;
      flags = sprite->flags;
      priority = sprite->image.priority;
    } else {
      animation = 0;
      flags = 0;
      priority = 0;
    }
    *animations++ = animation;
    *spriteFlags++ = flags;
    *priorities++ = priority;
    count++;
    copy++;
    if (count > ACTOR_SNAPSHOT_CAPACITY - 1)
      break;
  }
  for (i = count; i < ACTOR_SNAPSHOT_CAPACITY; i++)
    *indices++ = ACTOR_SNAPSHOT_EMPTY;
}

/// Puts back the actors the snapshot saved, and moves the camera to the party
/// leader's height.
void Actor_Restore(void) {
  struct Actor *copy = gSnapshotObjects;
  u8 *indices = (u8 *)gSnapshotObjects - ACTOR_SNAPSHOT_CAPACITY;
  u8 *animations = (u8 *)&gSnapshotObjects[ACTOR_SNAPSHOT_CAPACITY];
  u8 *spriteFlags = animations + ACTOR_SNAPSHOT_CAPACITY;
  u8 *priorities = animations + 2 * ACTOR_SNAPSHOT_CAPACITY;
  s32 count;
  struct Actor *actor;
  struct Sprite *sprite;
  s32 *view;
  struct Actor *camera;
  s32 index;
  s32 priority;
  FIXED y;

  count = 0;
  index = *indices++;
  while (index != ACTOR_SNAPSHOT_EMPTY) {
    actor = Actor_Lookup(index);
    if (actor != NULL) {
      sprite = actor->sprite;
      Dma_Set(copy, actor, DMA_COPY_WORDS(sizeof(struct Actor) / 4), REG_DMA3);
      actor->sprite = sprite;
      Actor_SelectAnimationFar(actor, *animations);
      Actor_SetSpriteFlagsFar(actor, *spriteFlags);
      priority = *priorities;
      sprite->image.priority = priority;
      sprite->shadow.priority = priority;
      if (index == Party_GetLeaderActor()) {
        camera = gFieldState->camera;
        view = ((struct MapState *)Ram_HeapSlots->mapGrid)->origin;
        y = actor->y.fixed;
        camera->x.fixed = actor->x.fixed;
        camera->groundHeight = y;
        camera->y.fixed = y;
        camera->z.fixed = actor->z.fixed;
        view[1] = y;
        Actor_StartIdleFar(actor);
      }
    }
    copy++;
    animations++;
    spriteFlags++;
    count++;
    priorities++;
    if (count > ACTOR_SNAPSHOT_CAPACITY - 1)
      break;
    index = *indices++;
  }
}

#undef PLACEMENT_PARTY_COUNT
#undef PLACEMENT_SPRITE_MAX
#undef PLACEMENT_SHIP
#undef LOOKUP_ID_COUNT
