/// @file
/// The party's sprites, placing a scene's actors, and the actor slots.
// Part of field_sequence.c.

#include "field_sequence_internal.h"

enum {
  /// 20 flags per element: the Djinn's caught flags are 0x30 to 0x7f.
  ACTOR_DJINNI_FLAG_COUNT = ELEMENT_COUNT * 20
};

/// A placement row's sprite: 0 to 7 is a party member, who keeps that actor id;
/// one up to this takes the next free slot, and one above it, which no table
/// names, takes the slot of the row before.
#define PLACEMENT_SPRITE_MAX 9989

/// The ids Actor_Lookup answers: more than the ACTOR_ID_COUNT slots, so one
/// past them reads the field state's members after the slots.
#define LOOKUP_ID_COUNT 192

/// The last sprite the tests below take as a party member's: one past PIERS,
/// though only Isaac's and Garet's sprites are ever swapped.
#define PARTY_SPRITE_LAST (PIERS + 1)

/// Returns the sprite of character, which is the character itself except that
/// FLAG_CHILDHOOD gives Isaac and Garet SPRITE_ISAAC_CHILD and
/// SPRITE_GARET_CHILD, and FLAG_ISAAC_WEARS_TUNIC gives grown Isaac sprite
/// SPRITE_ISAAC_TUNIC.
s32 Party_GetCharacterSprite(s32 character) {
  s32 sprite;

  sprite = character;
  if (sprite <= PARTY_SPRITE_LAST) {
    if (GameFlag_GetFar(FLAG_CHILDHOOD) != 0) {
      if (sprite == ISAAC) {
        sprite = SPRITE_ISAAC_CHILD;
      }
      if (sprite == GARET) {
        sprite = SPRITE_GARET_CHILD;
      }
    } else if ((GameFlag_GetFar(FLAG_ISAAC_WEARS_TUNIC) != 0) &&
               (sprite == ISAAC)) {
      sprite = SPRITE_ISAAC_TUNIC;
    }
  }
  return sprite;
}

/// Returns the sprite of character as Party_GetCharacterSprite would with
/// FLAG_CHILDHOOD set to childhood (as recorded in a save file's summary).
s32 Party_GetCharacterSpriteForChildhood(s32 character, s32 childhood) {
  s32 sprite;

  sprite = character;
  if ((sprite <= PARTY_SPRITE_LAST) && (childhood != 0)) {
    if (sprite == ISAAC) {
      sprite = SPRITE_ISAAC_CHILD;
    }
    if (sprite == GARET) {
      sprite = SPRITE_GARET_CHILD;
    }
  }
  return sprite;
}

/// Creates the actors of a scene's actor placement table (ended by sprite -1),
/// and records the table among the scene's tables.
void Event_SpawnActorPlacements(struct ActorPlacement *entry, s32 slot) {
  struct FieldState *state;
  struct Actor *actor;
  struct Actor *previous;
  struct Sprite *sprite;
  s32 tableIndex;
  s32 actorIndex;
  s32 spriteId;
  s32 condition;
  u8 spriteSlot;
  u16 placementSprite;

  state = gFieldState;
  // Remember the table once, before consuming slots for its actor
  // placements.
  for (tableIndex = 0; tableIndex < (s32)ARRAY_COUNT(state->tables);
       tableIndex++) {
    if (state->tables[tableIndex] == entry)
      break;
    if (state->tables[tableIndex] == 0) {
      state->tables[tableIndex] = entry;
      break;
    }
  }
  for (placementSprite = entry->sprite;
       (s16)placementSprite != -1 && slot <= ACTOR_ID_COUNT - 1;
       entry++, placementSprite = entry->sprite) {
    // Current actor placement tables use sprites 0 to 504, so every live
    // row sets actorIndex.
    if (entry->sprite <= 7)
      actorIndex = entry->sprite;
    else if (entry->sprite <= PLACEMENT_SPRITE_MAX)
      actorIndex = slot++;
    condition = entry->condition;
    if (!GameFlag_IsConditionActive(condition))
      continue;
    // Djinn_ResolvePending hides a Djinni after the party loses or flees.
    // Scene_Arrive clears these temporary flags on a normal entry.
    if (condition >= FLAG_DJINN_FIRST &&
        condition < FLAG_DJINN_FIRST + ACTOR_DJINNI_FLAG_COUNT &&
        state->mode != ACTOR_SYSTEM_WORLD_MAP &&
        !GameFlag_IsConditionActive(condition + ACTOR_DJINNI_FLAG_COUNT))
      continue;
    spriteId = Party_GetCharacterSprite(entry->sprite);
    actor = Actor_Lookup(actorIndex);
    if (actor == 0) {
      actor = Actor_CreateFar(spriteId, entry->x, entry->y, entry->z);
      // ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE: share the previous actor's
      // sprite resource.
      if (entry->flags & ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE) {
        previous = Actor_Lookup(actorIndex - 1);
        if (previous->spriteStorageKind == ACTOR_SPRITE_ONE &&
            actor->spriteStorageKind == ACTOR_SPRITE_ONE) {
          sprite = previous->sprite;
          sprite->retainCacheEntry = 1;
          spriteSlot = sprite->slot;
          sprite = actor->sprite;
          sprite->retainCacheEntry = 1;
          VramCache_FreeEntry(sprite->slot);
          sprite->slot = spriteSlot;
        }
      }
      // Dora's tunic overlays the young sprites with its child part.
      if (GameFlag_GetFar(FLAG_ISAAC_WEARS_TUNIC) &&
          (u32)(spriteId - SPRITE_ISAAC_CHILD) <=
              SPRITE_GARET_CHILD - SPRITE_ISAAC_CHILD)
        Actor_RegisterSpriteMetadataFar(actor, SPRITE_TUNIC);
    } else if (!GameFlag_GetFar(FLAG_KEEP_PARTY_POSITION)) {
      Actor_SetPositionAndResetMotionFar(actor, entry->x, entry->y, entry->z);
    }

    if (actor) {
      Actor_SelectAnimationFar(actor, ANIM_STAND);
      // lastNo is the animation last selected, which selecting again skips: a
      // random 0 to 29 makes the second selection below restart ANIM_STAND from
      // its first frame, unless it happens to be ANIM_STAND.
      if (actor->spriteStorageKind == ACTOR_SPRITE_ONE &&
          (sprite = actor->sprite) != 0)
        sprite->lastNo = ((u32)(Random16()) % (u32)30);
      actor->facing = entry->facing;
      actor->collisionFlags = ACTOR_SOLID;
      Actor_SetMovementType(actor, entry->movement);
      Actor_SelectAnimationFar(actor, ANIM_STAND);
      actor->homeX = actor->x.fixed / FIXED_ONE;
      actor->homeZ = actor->z.fixed / FIXED_ONE;
      // An actor placement with a height is lifted half a pixel more and
      // floats (ACTOR_FLOATS): it neither follows the ground nor falls.
      if (actor->y.fixed != 0) {
        actor->motionFlags = ACTOR_FLOATS;
        actor->y.fixed += toFIXED(1) / 2;
      }
      if (state->mode == ACTOR_SYSTEM_WORLD_MAP) {
        // World Map actor placements use single sprites and do not follow the
        // ground.
        actor->motionFlags &= ~ACTOR_FOLLOWS_TERRAIN;
        if (!GameFlag_GetFar(FLAG_ISAAC_WEARS_TUNIC)) {
          // UB: sprite creation can fail while the actor slot stays live.
#ifdef UBFIX
          if (actor->spriteStorageKind == ACTOR_SPRITE_ONE &&
              (sprite = actor->sprite) != NULL)
#endif
            sprite->scale = Iwram_MulFX(sprite->scale, 3 * FIXED_ONE / 4);
        }
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

/// Releases the actor in slot actorIndex, if there is one, and
/// empties the slot.
void Actor_ReleaseAtIndex(s32 actorIndex) {
  struct FieldState *state = gFieldState;
  void *object = Actor_Lookup(actorIndex);

  if (object != NULL) {
    Actor_ReleaseFar(object);
#ifdef UBFIX
    {
      void *empty = NULL;

      // Actor_Lookup also accepts ids past the actor slots, within the
      // field state.
      memcpy((u8 *)state + offsetof(struct FieldState, actors) +
                 (u32)actorIndex * sizeof(empty),
             &empty, sizeof(empty));
    }
#else
    state->actors[actorIndex] = NULL;
#endif
  }
}

/// Creates a scene's actors on entering it: the party leader where the main
/// routine's state puts it, then the scene's actor placement table from actor
/// slot 8, and the camera that follows the leader.
void Actor_ResetForLeader(struct ActorPlacement *table) {
  struct FieldState *state;
  struct ActorPlacement *entry;
  struct Actor *actor;
  struct Actor *camera;
  struct MapGridCell *cell;
  struct MapGridCell *above;
  s32 leader;
  s32 pos;
  FIXED height;

  state = gFieldState;
  leader = gMainRoutineState.leaderActorId;
  // Build the leader's temporary actor placement before the scene's placed
  // actors.
  entry = (struct ActorPlacement *)((u8 *)state + offsetof(struct FieldState,
                                                           leaderPlacements));
  entry[0] = gField_DefaultActorPlacements[0];
  entry[1] = gField_DefaultActorPlacements[1];
  for (pos = 0; pos < (s32)ARRAY_COUNT(state->tables); pos++)
    state->tables[pos] = 0;
  Actor_ClearAll();
  entry->condition = -1;
  entry->sprite = leader;
  SET_POSITION(entry, gMainRoutineState.x, 0, gMainRoutineState.z);
  entry->facing = gMainRoutineState.heading;
  Event_SpawnActorPlacements(entry, leader);
  Event_SpawnActorPlacements(table, ACTOR_FIRST_PLACED);

  actor = state->actors[leader];
  // gMainRoutineState.layer keeps the leader's map grid layer
  // (FieldSequence_Run saves its layer there).
  actor->layer = gMainRoutineState.layer;
  pos = actor->x.fixed / toFIXED(MAPGRID_CELL_PIXELS) +
        (actor->z.fixed / toFIXED(MAPGRID_CELL_PIXELS)) * MAPGRID_SIZE;
  cell = &MAPGRID_CELLS[pos];
  above = &MAPGRID_CELLS[pos - MAPGRID_SIZE];
  if (gMainRoutineState.y != 0 && cell->behavior == CELL_BEHAVIOR_CLIMB &&
      above->behavior == CELL_BEHAVIOR_CLIMB) {
    gMainRoutineState.movementMode = LEADER_MOVEMENT_CLIMB;
    height = MapGrid_GetTerrainHeightFar(0, actor->x.fixed,
                                         actor->z.fixed - toFIXED(16)) -
             toFIXED(32);
    actor->y.fixed += height;
    actor->groundHeight = actor->y.fixed;
    actor->motionFlags = 0;
    Actor_SetSpriteFlagsFar(actor, 0);
    Actor_SelectAnimationFar(actor, ANIM_LEADER_CLING);
  } else {
    gMainRoutineState.movementMode = LEADER_MOVEMENT_WALK;
  }

  // The camera follows this leader and starts at its resolved ground height;
  // the map scrolls by the camera's position. It takes no sprite.
  camera = Actor_CreateFar(ACTOR_CREATE_KIND(ACTOR_CREATE_NO_SPRITE),
                           actor->x.fixed, actor->y.fixed, actor->z.fixed);
  camera->groundHeight = actor->groundHeight;
  Actor_StartCameraFar(camera, (s32)actor);
  if (state->mode == ACTOR_SYSTEM_WORLD_MAP) {
    struct SpriteAnimation *meta =
        (struct SpriteAnimation *)Sprite_AddAnimationFar(actor->sprite,
                                                         SPRITE_BLANK_32);
    meta->colorMode = SPRITE_COLOR_HIDDEN;
    meta->priority = 9;
  }
  ((struct MapGridScrollState *)gMapGridState[0])->origin = &camera->x.fixed;
  state->camera = camera;
}

/// Returns the slot after the last placed actor (8 when there is none), or -1
/// when the last slot is occupied.
s32 Actor_FindLastActiveId(void) {
  struct FieldState *state = gFieldState;
  s32 lastActorIndex = ACTOR_FIRST_PLACED - 1;
  s32 actorIndex = ACTOR_FIRST_PLACED;
  struct Actor **slot = &state->actors[ACTOR_FIRST_PLACED];

  for (; actorIndex < ACTOR_ID_COUNT; actorIndex++) {
    struct Actor *actor = *slot++;

    if (actor != 0) {
      lastActorIndex = actorIndex;
    }
  }
  lastActorIndex++;
  if (lastActorIndex == ACTOR_ID_COUNT) {
    lastActorIndex = -1;
  }
  return lastActorIndex;
}

/// Unused: returns the address of actor slot index in the field state.
UNUSED void *Actor_GetSlotAddress(u32 actorIndex) {
  return (u8 *)gFieldState + offsetof(struct FieldState, actors) +
         actorIndex * sizeof(void *);
}

// Actors and the view: actor placement conditions from the map's
// view bounds, and clearing placed actors.

#define FLAG_IN_VIEW 0x164     ///< kept clear, so a placement using it appears
#define FLAG_OUT_OF_VIEW 0x165 ///< kept set, so a placement using it does not

#define PLACED_MAX                                                             \
  (ACTOR_ID_COUNT - ACTOR_FIRST_PLACED) ///< placed actors' slots

/// Gives each actor placement with condition 0 a condition saying whether
/// it lies within the map's view bounds, so that only those inside appear.
void Scene_AssignViewFlags(struct ActorPlacement *entry) {
  struct MapGridScrollState *view = gMapGridState[0];

  GameFlag_ClearFar(FLAG_IN_VIEW);
  GameFlag_SetFar(FLAG_OUT_OF_VIEW);
  while (entry->sprite != -1) {
    s32 x;
    s32 z;

    // A placement with a condition of its own keeps it.
    if (entry->condition != 0) {
      entry++;
      continue;
    }
    x = entry->x;
    z = entry->z;
    if (view->minX <= x && x <= view->maxX && view->minY <= z &&
        z <= view->maxY)
      entry->condition = FLAG_IN_VIEW;
    else
      entry->condition = FLAG_OUT_OF_VIEW;
    entry++;
  }
}

/// The byte of struct Sprite after its slot, whose bit 0 is retainCacheEntry.
#define SPRITE_CACHE_FLAGS (offsetof(struct Sprite, slot) + 1)

/// Releases the placed actors far from the view's centre: more than 160 pixels
/// to either side, 200 pixels north or 100 pixels south. Actors at (0, 0) stay.
void Actor_ClearOutOfBounds(void) {
  struct FieldState *state = gFieldState;
  struct Actor *camera;
  FIXED centerX;
  FIXED centerZ;
  FIXED left;
  FIXED right;
  FIXED top;
  FIXED bottom;
  struct Actor **actorSlot;
  s32 releaseCacheMask;
  s32 i;

  camera = state->camera;
  centerX = camera->x.fixed;
  left = centerX - toFIXED(160);
  right = centerX + toFIXED(160);
  centerZ = camera->z.fixed;
  top = centerZ - toFIXED(200);
  bottom = centerZ + toFIXED(100);
  actorSlot = &state->actors[ACTOR_FIRST_PLACED];
  // Every bit of SPRITE_CACHE_FLAGS but retainCacheEntry.
  releaseCacheMask = ~1;

  for (i = 0; i < PLACED_MAX; i++) {
    struct Actor *actor = *actorSlot;

    if (actor != NULL) {
      FIXED actorX = actor->x.fixed;
      FIXED actorZ = actor->z.fixed;

      if (actorX != 0 || actorZ != 0) {
        if (actorX < left || actorX > right || actorZ < top ||
            actorZ > bottom) {
          u8 *sprite;
          u8 *removed = &actor->spriteStorageKind;
          u32 flags;

          // ACTOR_SPRITE_ONE makes the release free the actor's one
          // sprite, and clearing retainCacheEntry frees its VRAM cache entry
          // with it, even one shared with a neighbour
          // (ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE).
          *removed = ACTOR_SPRITE_ONE;
          sprite = (u8 *)actor->sprite;
          flags = sprite[SPRITE_CACHE_FLAGS];
          sprite[SPRITE_CACHE_FLAGS] = flags & releaseCacheMask;
          Actor_ReleaseFar(actor);
          sprite = NULL;
          *actorSlot = (struct Actor *)sprite;
        }
      }
    }

    actorSlot++;
  }
}

/// Releases every placed actor and places the scene's actors again from its
/// first actor placement table after the leader's.
void Actor_Respawn(void) {
  struct FieldState *state = gFieldState;
  struct ActorPlacement *placements;
  s32 slotIndex;

  for (slotIndex = 0; slotIndex < PLACED_MAX; slotIndex++) {
    struct Actor *actor = state->actors[ACTOR_FIRST_PLACED + slotIndex];

    if (actor != 0) {
      // As in Actor_ClearOutOfBounds: free the sprite and its
      // VRAM cache entry with the actor.
      actor->spriteStorageKind = ACTOR_SPRITE_ONE;
      actor->sprite->retainCacheEntry = 0;
      Actor_ReleaseFar(actor);
      state->actors[ACTOR_FIRST_PLACED + slotIndex] = 0;
    }
  }
  placements = state->tables[1];
  state->tables[1] = 0;
  state->tables[2] = 0;
  state->tables[3] = 0;
  if (placements != 0)
    Event_SpawnActorPlacements(placements, Actor_FindLastActiveId());
}

/// Empties all the actor slots, without releasing the actors.
void Actor_ClearAll(void) {
  struct FieldState *state;
  struct Actor *emptyActor;
  s32 actorIndex;

  emptyActor = NULL;
  state = gFieldState;
  for (actorIndex = 0; actorIndex < ACTOR_ID_COUNT; actorIndex++) {
    state->actors[actorIndex] = emptyActor;
  }
}

/// Returns the actor in slot actorIndex (below LOOKUP_ID_COUNT), or NULL.
void *Actor_Lookup(u32 actorIndex) {
  u8 *stateBytes = (u8 *)gFieldState;
  u32 slotOffset;
#ifdef UBFIX
  void *object;
#endif
  if (actorIndex > LOOKUP_ID_COUNT - 1)
    return 0;
  slotOffset = (actorIndex * 4) + offsetof(struct FieldState, actors);
#ifdef UBFIX
  // The legacy range includes non-pointer fields. Copy their representation
  // without reading them through an incompatible pointer lvalue.
  memcpy(&object, stateBytes + slotOffset, sizeof(object));
  return object;
#else
  return *(void **)(stateBytes + slotOffset);
#endif
}

/// Saves up to 32 actors (only the party's 8 on the World Map) into
/// gActorSnapshot, with each one's slot, animation, sprite flags and
/// priority, so that Actor_Restore can put them back.
void Actor_Snapshot(void) {
  struct FieldState *state;
  struct Actor *snapshot = gActorSnapshot.actors;
  u8 *animations = gActorSnapshot.animations;
  u8 *spriteFlags = gActorSnapshot.spriteFlags;
  u8 *priorities = gActorSnapshot.priorities;
  u8 *indices = gActorSnapshot.slots;
  u32 savedCount = 0;
  s32 actorLimit = sizeof(state->actors) / sizeof(state->actors[0]);
  struct Actor *actor;
  s32 actorIndex;
  u32 animation;
  u32 spriteFlagBits;
  u32 priority;

  state = gFieldState;
  if (state->mode == ACTOR_SYSTEM_WORLD_MAP)
    actorLimit = ACTOR_FIRST_PLACED;
  // Keep each actor and its three sprite-state bytes at matching save
  // indices.
  for (actorIndex = 0; actorIndex < actorLimit; actorIndex++) {
    actor = Actor_Lookup(actorIndex);
    if (actor == NULL)
      continue;
    *indices++ = actorIndex;
    Dma_Set(actor, snapshot, DMA_COPY_WORDS(sizeof(struct Actor) / 4),
            REG_DMA3);
    if (actor->spriteStorageKind == ACTOR_SPRITE_ONE) {
      struct Sprite *sprite = actor->sprite;
      animation = sprite->lastNo;
      spriteFlagBits = sprite->flags;
      priority = sprite->image.priority;
    } else {
      animation = 0;
      spriteFlagBits = 0;
      priority = 0;
    }
    *animations++ = animation;
    *spriteFlags++ = spriteFlagBits;
    *priorities++ = priority;
    savedCount++;
    snapshot++;
    if (savedCount > ACTOR_SNAPSHOT_CAPACITY - 1)
      break;
  }
  for (actorIndex = savedCount; actorIndex < ACTOR_SNAPSHOT_CAPACITY;
       actorIndex++)
    *indices++ = ACTOR_SNAPSHOT_EMPTY;
}

/// Puts back the actors Actor_Snapshot saved, and moves the camera to the party
/// leader's height.
void Actor_Restore(void) {
  struct Actor *snapshot = gActorSnapshot.actors;
  u8 *indices = gActorSnapshot.slots;
  u8 *animations = gActorSnapshot.animations;
  u8 *spriteFlags = gActorSnapshot.spriteFlags;
  u8 *priorities = gActorSnapshot.priorities;
  s32 restoredCount;
  struct Actor *actor;
  struct Sprite *sprite;
  struct Actor *camera;
  s32 *cameraPosition;
  s32 actorIndex;
  s32 animation;
  s32 priority;
  FIXED y;
  struct FieldState **state = &gFieldState;
  struct MapGridScrollState **grid =
      (struct MapGridScrollState **)gMapGridState;

  restoredCount = 0;
  actorIndex = *indices++;
  while (actorIndex != ACTOR_SNAPSHOT_EMPTY) {
    actor = Actor_Lookup(actorIndex);
    if (actor != NULL) {
      // The actor keeps the sprite it has now, not the one saved with it.
      sprite = actor->sprite;
      Dma_Set(snapshot, actor, DMA_COPY_WORDS(sizeof(struct Actor) / 4),
              REG_DMA3);
      animation = *animations;
      if (animation != 0)
        Actor_SelectAnimationFar(actor, animation);
      Actor_SetSpriteFlagsFar(actor, *spriteFlags);
      priority = *priorities;
      sprite->image.priority = priority;
      sprite->shadow.priority = priority;
      *(struct Sprite **)&actor->sprite = sprite;
      // Re-anchor the camera only when restoring the selected leader.
      if (actorIndex == gMainRoutineState.leaderActorId) {
        camera = (*state)->camera;
        cameraPosition = (*grid)->origin;
        y = actor->y.fixed;
        camera->groundHeight = y;
        camera->y.fixed = y;
        cameraPosition[1] = y;
        Actor_StartIdleFar(actor);
      }
    }
    snapshot++;
    animations++;
    spriteFlags++;
    priorities++;
    restoredCount++;
    if (restoredCount > ACTOR_SNAPSHOT_CAPACITY - 1)
      break;
    actorIndex = *indices++;
  }
}

#undef FLAG_IN_VIEW
#undef FLAG_OUT_OF_VIEW
#undef PLACED_MAX
#undef PLACEMENT_SPRITE_MAX
#undef LOOKUP_ID_COUNT
#undef PARTY_SPRITE_LAST
#undef SPRITE_CACHE_FLAGS
