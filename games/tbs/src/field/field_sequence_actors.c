/// @file
/// The party's sprites, placing a scene's actors, and the actor
/// table.
// Part of field_sequence.c.

#include "field_sequence_internal.h"

enum {
  /// 20 flags per element: the Djinn's caught flags are 48 to 127.
  ACTOR_DJINNI_FLAG_COUNT = ELEMENT_COUNT * 20
};

/// Returns the sprite of character, which is the character itself except that
/// FLAG_CHILDHOOD gives Isaac and Garet SPRITE_ISAAC_CHILD and
/// SPRITE_GARET_CHILD, and FLAG_ISAAC_WEARS_TUNIC gives grown Isaac sprite
/// SPRITE_ISAAC_TUNIC.
s32 Party_GetCharacterSprite(s32 character) {
  s32 sprite;

  sprite = character;
  if (sprite <= 8) {
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
s32 Party_RemapCharacterIdIfEnabled(s32 character, s32 childhood) {
  s32 sprite;

  sprite = character;
  if ((sprite <= 8) && (childhood != 0)) {
    if (sprite == ISAAC) {
      sprite = SPRITE_ISAAC_CHILD;
    }
    if (sprite == GARET) {
      sprite = SPRITE_GARET_CHILD;
    }
  }
  return sprite;
}

#define MAPGRID_CELLS ((struct MapGridCell *)Ram_MapGridCellBuffer)

/// Creates the actors of a scene's actor placement table (ended by sprite -1),
/// and records the table among the scene's tables.
void Event_SpawnActorPlacements(struct ActorPlacement *entry, s32 slot) {
  struct FieldWork *work;
  struct Actor *actor;
  struct Actor *previous;
  struct Sprite *sprite;
  s32 tableIndex;
  s32 actorIndex;
  s32 spriteId;
  s32 condition;
  u8 spriteSlot;
  u16 placementSprite;

  work = gFieldWork;
  // Remember the table once, before consuming slots for its actor
  // placements.
  for (tableIndex = 0; tableIndex < 4; tableIndex++) {
    if (work->tables[tableIndex] == entry)
      break;
    if (work->tables[tableIndex] == 0) {
      work->tables[tableIndex] = entry;
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
    else if (entry->sprite <= 9989)
      actorIndex = slot++;
    condition = entry->condition;
    if (!GameFlag_IsConditionActive(condition))
      continue;
    // Djinn_ResolvePending hides a Djinni after the party loses or flees.
    // Scene_Arrive clears these temporary flags on a normal entry.
    if ((u32)(condition - FLAG_DJINN_FIRST) <= ACTOR_DJINNI_FLAG_COUNT - 1 &&
        work->mode != ACTOR_SYSTEM_WORLD_MAP &&
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
      (actor)->homeX = actor->x.fixed / Q16_ONE;
      (actor)->homeZ = actor->z.fixed / Q16_ONE;
      // An actor placement with a height is lifted half a pixel more and takes
      // motion flags 4, floating: it neither follows the ground nor falls.
      if (actor->y.fixed != 0) {
        actor->motionFlags = ACTOR_FLOATS;
        actor->y.fixed += px(1) / 2;
      }
      if (work->mode == ACTOR_SYSTEM_WORLD_MAP) {
        // World Map actor placements use single sprites and do not follow the
        // ground.
        actor->motionFlags &= ~ACTOR_FOLLOWS_TERRAIN;
        if (!GameFlag_GetFar(FLAG_ISAAC_WEARS_TUNIC)) {
          // UB: sprite creation can fail while the actor slot stays live.
#ifdef UBFIX
          if (actor->spriteStorageKind == ACTOR_SPRITE_ONE &&
              (sprite = actor->sprite) != NULL)
#endif
            sprite->scale = Iwram_MulQ16(sprite->scale, 3 * Q16_ONE / 4);
        }
      } else {
        actor->groundHeight =
            MapGrid_GetTerrainHeightFar(0, actor->x.fixed, actor->z.fixed);
        actor->y.fixed += actor->groundHeight;
      }
      actor->priorityFlags = ACTOR_PRIORITY_AUTOMATIC;
    }
    work->actors[actorIndex] = actor;
  }
}

/// Releases the actor in slot actorIndex, if there is one, and
/// empties the slot.
void Actor_ReleaseAtIndex(s32 actorIndex) {
  struct FieldWork *work = gFieldWork;
  void *object = Actor_Lookup(actorIndex);

  if (object != NULL) {
    Actor_ReleaseFar(object);
#ifdef UBFIX
    {
      void *empty = NULL;

      // Get also accepts words past the actor array, within the field
      // work.
      memcpy((u8 *)work + offsetof(struct FieldWork, actors) +
                 (u32)actorIndex * sizeof(empty),
             &empty, sizeof(empty));
    }
#else
    work->actors[actorIndex] = NULL;
#endif
  }
}

/// Creates a scene's actors on entering it: the party leader where the main
/// routine's work puts it, then the scene's actor placement table from actor
/// slot 8, and the camera that follows the leader.
void Actor_ResetForLeader(struct ActorPlacement *table) {
  struct FieldWork *work;
  struct ActorPlacement *entry;
  struct Actor *actor;
  struct Actor *camera;
  struct MapGridCell *cell;
  struct MapGridCell *above;
  s32 leader;
  s32 pos;
  s32 height;

  work = gFieldWork;
  leader = gMainRoutineWork.leaderActorId;
  // Build the leader's temporary actor placement before the scene's placed
  // actors.
  entry = (struct ActorPlacement *)((u8 *)work + offsetof(struct FieldWork,
                                                          leaderPlacements));
  entry[0] = gField_DefaultActorPlacements[0];
  entry[1] = gField_DefaultActorPlacements[1];
  for (pos = 0; pos < 4; pos++)
    work->tables[pos] = 0;
  Actor_ClearAll();
  entry->condition = -1;
  entry->sprite = leader;
  SET_POSITION(entry, gMainRoutineWork.x, 0, gMainRoutineWork.z);
  entry->facing = gMainRoutineWork.heading;
  Event_SpawnActorPlacements(entry, leader);
  Event_SpawnActorPlacements(table, ACTOR_FIRST_PLACED);

  actor = work->actors[leader];
  // gMainRoutineWork.layer keeps the leader's map grid layer (FieldSequence_Run
  // saves its layer there).
  actor->layer = gMainRoutineWork.layer;
  pos = (actor->x.fixed / px(16)) + (actor->z.fixed / px(16)) * 128;
  cell = &MAPGRID_CELLS[pos];
  above = &MAPGRID_CELLS[pos - 128];
  if (gMainRoutineWork.y != 0 && cell->behavior == CELL_BEHAVIOR_CLIMB &&
      above->behavior == CELL_BEHAVIOR_CLIMB) {
    gMainRoutineWork.movementMode = LEADER_MOVEMENT_CLIMB;
    height = MapGrid_GetTerrainHeightFar(0, actor->x.fixed,
                                         actor->z.fixed - px(16)) -
             px(32);
    actor->y.fixed += height;
    actor->groundHeight = actor->y.fixed;
    actor->motionFlags = 0;
    Actor_SetSpriteFlagsFar(actor, 0);
    Actor_SelectAnimationFar(actor, ANIM_LEADER_CLING);
  } else {
    gMainRoutineWork.movementMode = LEADER_MOVEMENT_WALK;
  }

  // The camera follows this leader and starts at its resolved ground height;
  // the map scrolls by the camera's position. It takes no sprite.
  camera = Actor_CreateFar(ACTOR_CREATE_NO_SPRITE << 12, actor->x.fixed,
                           actor->y.fixed, actor->z.fixed);
  camera->groundHeight = actor->groundHeight;
  Actor_StartCameraFar(camera, (s32)actor);
  if (work->mode == ACTOR_SYSTEM_WORLD_MAP) {
    struct SpriteAnimation *meta =
        (struct SpriteAnimation *)Sprite_AddAnimationFar(actor->sprite,
                                                         SPRITE_BLANK_32);
    meta->colorMode = SPRITE_COLOR_HIDDEN;
    meta->priority = 9;
  }
  ((struct MapGridScrollWork *)gMapGridWork[0])->origin = &camera->x.fixed;
  work->camera = camera;
}

/// Returns the slot after the last placed actor (8 when there is none), or -1
/// when the last slot is occupied.
s32 Actor_FindLastActiveId(void) {
  struct FieldWork *work = gFieldWork;
  s32 lastActorIndex = ACTOR_FIRST_PLACED - 1;
  s32 actorIndex = ACTOR_FIRST_PLACED;
  struct Actor **slot = &work->actors[ACTOR_FIRST_PLACED];

  do {
    struct Actor *actor = *slot++;

    if (actor != 0) {
      lastActorIndex = actorIndex;
    }
    actorIndex++;
  } while (actorIndex <= ACTOR_ID_COUNT - 1);
  lastActorIndex++;
  if (lastActorIndex == ACTOR_ID_COUNT) {
    lastActorIndex = -1;
  }
  return lastActorIndex;
}

/// Unused: returns the address of actor slot index in the field work.
UNUSED void *Actor_GetSlotAddress(u32 actorIndex) {
  return (u8 *)gFieldWork + offsetof(struct FieldWork, actors) +
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
  struct MapGridScrollWork *view = gMapGridWork[0];

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

/// Releases the placed actors far from the view's centre: more than 160 pixels
/// to either side, 200 pixels north or 100 pixels south. Actors at (0, 0) stay.
void Actor_ClearOutOfBounds(void) {
  struct FieldWork *work = gFieldWork;
  struct Actor *camera;
  s32 centerX;
  s32 centerZ;
  s32 left;
  s32 right;
  s32 top;
  s32 bottom;
  struct Actor **actorSlot;
  s32 releaseCacheMask;
  s32 i;

  camera = work->camera;
  centerX = camera->x.fixed;
  left = centerX - px(160);
  right = centerX + px(160);
  centerZ = camera->z.fixed;
  top = centerZ - px(200);
  bottom = centerZ + px(100);
  actorSlot = &work->actors[ACTOR_FIRST_PLACED];
  releaseCacheMask = ~1;

  for (i = 0; i < PLACED_MAX; i++) {
    struct Actor *actor = *actorSlot;

    if (actor != NULL) {
      s32 actorX = actor->x.fixed;
      s32 actorZ = actor->z.fixed;

      if (actorX != 0 || actorZ != 0) {
        if (actorX < left || actorX > right || actorZ < top ||
            actorZ > bottom) {
          u8 *sprite;
          u8 *removed = &actor->spriteStorageKind;
          u32 flags;

          // ACTOR_SPRITE_ONE makes the release free the actor's one
          // sprite, and clearing retainCacheEntry (bit 0 of sprite byte 29)
          // frees its VRAM cache entry with it, even one shared with a
          // neighbour (ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE).
          *removed = ACTOR_SPRITE_ONE;
          sprite = (u8 *)actor->sprite;
          flags = sprite[29];
          sprite[29] = flags & releaseCacheMask;
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
  struct FieldWork *work = gFieldWork;
  struct ActorPlacement *placements;
  s32 slotIndex;

  for (slotIndex = 0; slotIndex < PLACED_MAX; slotIndex++) {
    struct Actor *actor = work->actors[ACTOR_FIRST_PLACED + slotIndex];

    if (actor != 0) {
      // As in Actor_ClearOutOfBounds: free the sprite and its
      // VRAM cache entry with the actor.
      actor->spriteStorageKind = ACTOR_SPRITE_ONE;
      actor->sprite->retainCacheEntry = 0;
      Actor_ReleaseFar(actor);
      work->actors[ACTOR_FIRST_PLACED + slotIndex] = 0;
    }
  }
  placements = work->tables[1];
  work->tables[1] = 0;
  work->tables[2] = 0;
  work->tables[3] = 0;
  if (placements != 0)
    Event_SpawnActorPlacements(placements, Actor_FindLastActiveId());
}

/// Empties all the actor slots, without releasing the actors.
void Actor_ClearAll(void) {
  struct FieldWork *work;
  struct Actor *emptyActor;
  s32 actorIndex;

  emptyActor = NULL;
  work = gFieldWork;
  for (actorIndex = ACTOR_ID_COUNT - 1; actorIndex >= 0; actorIndex--) {
    work->actors[actorIndex] = emptyActor;
  }
}

/// Returns the actor in slot index (below ACTOR_ID_COUNT), or NULL.
void *Actor_Lookup(u32 actorIndex) {
  u8 *workBytes = (u8 *)gFieldWork;
  u32 slotOffset;
#ifdef UBFIX
  void *object;
#endif
  if (actorIndex > 191)
    return 0;
  slotOffset = (actorIndex * 4) + offsetof(struct FieldWork, actors);
#ifdef UBFIX
  // The legacy range includes non-pointer fields. Copy their representation
  // without reading them through an incompatible pointer lvalue.
  memcpy(&object, workBytes + slotOffset, sizeof(object));
  return object;
#else
  return *(void **)(workBytes + slotOffset);
#endif
}

/// Saves up to 32 actors (only the party's 8 on the World Map) into
/// gActorSnapshot, with each one's slot, animation, sprite flags and
/// priority, so that Actor_Restore can put them back.
void Actor_Snapshot(void) {
  struct FieldWork *work;
  struct Actor *snapshot = gActorSnapshot.actors;
  u8 *animations = gActorSnapshot.animations;
  u8 *spriteFlags = gActorSnapshot.spriteFlags;
  u8 *priorities = gActorSnapshot.priorities;
  u8 *indices = gActorSnapshot.slots;
  u32 savedCount = 0;
  s32 actorLimit = sizeof(work->actors) / sizeof(work->actors[0]);
  struct Actor *actor;
  s32 actorIndex;
  u32 animation;
  u32 spriteFlagBits;
  u32 priority;

  work = gFieldWork;
  if (work->mode == ACTOR_SYSTEM_WORLD_MAP)
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
  s32 y;
  s32 *leaderId = &gMainRoutineWork.leaderActorId;
  struct FieldWork **work = &gFieldWork;
  struct MapGridScrollWork **grid = (struct MapGridScrollWork **)gMapGridWork;

  restoredCount = 0;
  actorIndex = *indices++;
  while (actorIndex != ACTOR_SNAPSHOT_EMPTY) {
    actor = Actor_Lookup(actorIndex);
    if (actor != NULL) {
      // The copy also brings back the sprite pointer saved with the actor.
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
      // Through the sprite union this store moves.
      *(struct Sprite **)&actor->sprite = sprite;
      // Re-anchor the camera only when restoring the selected leader.
      if (actorIndex == *leaderId) {
        camera = (*work)->camera;
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

#undef MAPGRID_CELLS
#undef FLAG_IN_VIEW
#undef FLAG_OUT_OF_VIEW
#undef PLACED_MAX
