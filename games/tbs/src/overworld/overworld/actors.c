/// @file
/// The party's sprites, placing a scene's actors, and the actor
/// table.
// Part of overworld.c.

#include "../overworld_internal.h"

enum {
  /// 20 flags per element: the Djinn's caught flags are 48 to 127.
  ACTOR_DJINNI_FLAG_COUNT = ELEMENT_COUNT * 20
};

/// Returns the sprite of character, which is the character itself except that
/// FLAG_CHILDHOOD gives Isaac and Garet ISAAC_CHILD and GARET_CHILD, and
/// FLAG_ISAAC_WEARS_TUNIC gives grown Isaac sprite 0x11.
///
/// Called by Event_SpawnActorPlacements for every actor
/// placement, and by PsynergyMenu_InitializeEntryActors (through
/// Party_RemapCharacterIdByFlagsFar) for the party's figures.
s32 Party_RemapCharacterIdByFlags(s32 character) {
  s32 sprite;

  sprite = character;
  if (sprite <= 8) {
    if (GameFlag_GetFar(FLAG_CHILDHOOD) != 0) {
      if (sprite == ISAAC) {
        sprite = ISAAC_CHILD;
      }
      if (sprite == GARET) {
        sprite = GARET_CHILD;
      }
    } else if ((GameFlag_GetFar(FLAG_ISAAC_WEARS_TUNIC) != 0) &&
               (sprite == ISAAC)) {
      sprite = 0x11;
    }
  }
  return sprite;
}

/// Returns the sprite of character as Party_RemapCharacterIdByFlags would with
/// FLAG_CHILDHOOD set to childhood (as recorded in a save file's summary).
///
/// Called by FileList_ShowPartySprites through
/// Party_RemapCharacterIdIfEnabledFar. Save summaries omit the tunic flag,
/// so this never gives sprite 0x11.
s32 Party_RemapCharacterIdIfEnabled(s32 character, s32 childhood) {
  s32 sprite;

  sprite = character;
  if ((sprite <= 8) && (childhood != 0)) {
    if (sprite == ISAAC) {
      sprite = ISAAC_CHILD;
    }
    if (sprite == GARET) {
      sprite = GARET_CHILD;
    }
  }
  return sprite;
}

#define MAPGRID_CELLS ((struct MapGridCell *)Ram_MapGridCellBuffer)

extern const struct ActorPlacement gField_DefaultActorPlacements[2];

/// Creates the actors of a scene's actor placement table (ended by
/// sprite -1), and records the table among the scene's tables. Sprites 0 to 7
/// are the party's characters and take their own actor slot; other actor
/// placements take the actor slots from slot on, one each, up to the last. An
/// actor placement whose condition does not hold is skipped, as is a Djinni
/// hidden after a lost battle outside the World Map, but it still uses up its
/// slot: actor slot + n is always row n, counted from 0, of the table's rows of
/// sprite 8 or more, as Scene_FindActorPlacement counts them. An existing actor
/// is moved back to its place unless the party keeps its position. The table is
/// kept by address, so it must last as long as the scene (the overlay's own
/// tables do).
///
/// Called by Actor_ResetForLeader (the leader, then the scene's table),
/// Actor_Respawn and Event_CallWithLastActiveActorId. Records
/// the table in work->tables and each actor in work->actors.
void Event_SpawnActorPlacements(struct ActorPlacement *entry, s32 slot) {
  struct FieldWork *work;
  struct Actor *object;
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
    // XXX (no evidence): what the 0x2705 cutoff was for; no table reaches it.
    if (entry->sprite <= 7)
      actorIndex = entry->sprite;
    else if (entry->sprite <= 0x2705)
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
    spriteId = Party_RemapCharacterIdByFlags(entry->sprite);
    object = Actor_Lookup(actorIndex);
    if (object == 0) {
      object = Actor_CreateFar(spriteId, entry->x, entry->y, entry->z);
      // ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE: share the previous
      // actor's sprite resource. The new sprite draws from the previous one's
      // VRAM cache entry, its own entry is freed, and both are marked to keep
      // the shared entry when either is released.
      if (entry->flags & ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE) {
        previous = Actor_Lookup(actorIndex - 1);
        if (previous->spriteStorageKind == ACTOR_SPRITE_ONE &&
            object->spriteStorageKind == ACTOR_SPRITE_ONE) {
          sprite = previous->sprite;
          sprite->retainCacheEntry = 1;
          spriteSlot = sprite->slot;
          sprite = object->sprite;
          sprite->retainCacheEntry = 1;
          VramCache_FreeEntry(sprite->slot);
          sprite->slot = spriteSlot;
        }
      }
      // Dora's tunic overlays the young sprites with its child part.
      if (GameFlag_GetFar(FLAG_ISAAC_WEARS_TUNIC) &&
          (u32)(spriteId - ISAAC_CHILD) <= GARET_CHILD - ISAAC_CHILD)
        Actor_RegisterSpriteMetadataFar(object, SPRITE_TUNIC);
    } else if (!GameFlag_GetFar(FLAG_KEEP_PARTY_POSITION)) {
      Actor_SetPositionAndResetMotionFar(object, entry->x, entry->y, entry->z);
    }

    if (object) {
      Actor_SelectAnimationFar(object, 1);
      // lastNo is the animation last selected, which selecting again skips: a
      // random 0 to 29 makes the second selection below restart animation 1
      // from its first frame, unless it happens to be 1. The first selection
      // has just started it from that frame, so the idle looks the same
      // either way.
      // XXX (no evidence): why the value is random when only whether it is 1
      // matters; nothing else reads lastNo before the selection.
      if (object->spriteStorageKind == ACTOR_SPRITE_ONE &&
          (sprite = object->sprite) != 0)
        sprite->lastNo = ((u32)(Random16()) % (u32)30);
      object->facing = entry->facing;
      // Solid.
      object->collisionFlags = 1;
      Actor_SetMovementType(object, entry->movement);
      Actor_SelectAnimationFar(object, 1);
      (object)->homeX = object->x.fixed / Q16_ONE;
      (object)->homeZ = object->z.fixed / Q16_ONE;
      // An actor placement with a height is lifted half a pixel more and
      // takes motion flags 4, floating: it neither follows the ground nor
      // falls. Outside the World Map every height is then counted from the
      // ground below.
      if (object->y.fixed != 0) {
        object->motionFlags = 4;
        object->y.fixed += 0x8000;
      }
      if (work->mode == ACTOR_SYSTEM_WORLD_MAP) {
        // World Map actor placements use single sprites and do not follow
        // the ground. While the tunic flag is set, all keep their native
        // scale; otherwise they draw at 3/4 size.
        object->motionFlags &= 0xfe;
        if (!GameFlag_GetFar(FLAG_ISAAC_WEARS_TUNIC)) {
          // UB: sprite creation can fail while the actor slot stays
          // live. The native local is then indeterminate or from an earlier
          // row. Checking the sprite grows tbs-en from 608 to 628 bytes.
#ifdef UBFIX
          if (object->spriteStorageKind == ACTOR_SPRITE_ONE &&
              (sprite = object->sprite) != NULL)
#endif
            sprite->scale = Iwram_MulQ16(sprite->scale, 3 * Q16_ONE / 4);
        }
      } else {
        object->groundHeight =
            MapGrid_GetTerrainHeightFar(0, object->x.fixed, object->z.fixed);
        object->y.fixed += object->groundHeight;
      }
      object->priorityFlags = ACTOR_PRIORITY_AUTOMATIC;
    }
    work->actors[actorIndex] = object;
  }
}

/// Releases the actor in slot actorIndex, if there is one, and
/// empties the slot. No caller names it; its far-call veneer has no exported
/// name.
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
      memcpy((u8 *)work + 0x14 + (u32)actorIndex * sizeof(empty), &empty,
             sizeof(empty));
    }
#else
    work->actors[actorIndex] = NULL;
#endif
  }
}

/// Creates a scene's actors on entering it: the party leader where the
/// adventure puts it, then the scene's actor placement table from
/// actor slot 8, and the camera that follows the leader. A leader entering
/// above ground where its cell and the cell north of it are climbing spots
/// (CELL_BEHAVIOR_CLIMB) is raised by the ground height of the cell north of it
/// less 32 pixels, and starts climbing (LEADER_MOVEMENT_CLIMB).
///
/// Called by SceneSequence_Run on entering a scene, and by
/// Event_RunActorUpdateAndWait.
/// Empties every actor slot and table first, without releasing the
/// actors in them; sets work->camera, the map's origin and
/// gAdventure.movementMode.
void Actor_ResetForLeader(struct ActorPlacement *table) {
  struct FieldWork *work;
  struct ActorPlacement *entry;
  struct Actor *object;
  struct Actor *camera;
  struct MapGridCell *cell;
  struct MapGridCell *above;
  s32 leader;
  s32 pos;
  s32 height;

  work = gFieldWork;
  leader = gAdventure.leaderActorId;
  // Build the leader's temporary actor placement before the scene's placed
  // actors. The two-row table, the leader's row and an end row, lives in
  // the field work at 0x200 (FieldWork's leaderPlacements) and lasts with it.
  entry = (struct ActorPlacement *)((u8 *)work + 0x200);
  entry[0] = gField_DefaultActorPlacements[0];
  entry[1] = gField_DefaultActorPlacements[1];
  for (pos = 0; pos < 4; pos++)
    work->tables[pos] = 0;
  Actor_ClearAll();
  entry->condition = -1;
  entry->sprite = leader;
  SET_POSITION(entry, gAdventure.x, 0, gAdventure.z);
  entry->facing = gAdventure.heading;
  Event_SpawnActorPlacements(entry, leader);
  Event_SpawnActorPlacements(table, ACTOR_FIRST_PLACED);

  object = work->actors[leader];
  // gAdventure.layer keeps the leader's map grid layer (SceneSequence_Run
  // saves its layer there). Its cell: 16 pixels square, 128 to a row, so above
  // is the cell north of it.
  object->layer = gAdventure.layer;
  pos = (object->x.fixed / 0x100000) + (object->z.fixed / 0x100000) * 128;
  cell = &MAPGRID_CELLS[pos];
  above = &MAPGRID_CELLS[pos - 128];
  if (gAdventure.y != 0 && cell->behavior == CELL_BEHAVIOR_CLIMB &&
      above->behavior == CELL_BEHAVIOR_CLIMB) {
    gAdventure.movementMode = LEADER_MOVEMENT_CLIMB;
    height = MapGrid_GetTerrainHeightFar(0, object->x.fixed,
                                         object->z.fixed - 0x100000) -
             0x200000;
    object->y.fixed += height;
    object->groundHeight = object->y.fixed;
    object->motionFlags = 0;
    Actor_SetSpriteFlagsFar(object, 0);
    Actor_SelectAnimationFar(object, ANIM_LEADER_CLING);
  } else {
    gAdventure.movementMode = LEADER_MOVEMENT_WALK;
  }

  // The camera follows this leader and starts at its resolved ground height;
  // the map scrolls by the camera's position. It takes no sprite.
  camera = Actor_CreateFar(ACTOR_CREATE_NO_SPRITE << 12, object->x.fixed,
                           object->y.fixed, object->z.fixed);
  camera->groundHeight = object->groundHeight;
  Actor_StartCameraFar(camera, (s32)object);
  if (work->mode == ACTOR_SYSTEM_WORLD_MAP) {
    struct SpriteAnimation *meta =
        (struct SpriteAnimation *)Sprite_AddAnimationFar(object->sprite, 23);
    meta->colorMode = SPRITE_COLOR_HIDDEN;
    meta->priority = 9;
  }
  ((struct MapGridScrollWork *)gMapGridWork[0])->origin = &camera->x.fixed;
  work->camera = camera;
}

/// Returns the slot after the last placed actor (8 when there is none),
/// or -1 when the last slot is occupied. Earlier holes are not reused by this
/// append scan.
///
/// Called by Actor_Respawn and Event_CallWithLastActiveActorId
/// to choose where Event_SpawnActorPlacements starts.
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
  return (u8 *)gFieldWork + 0x14 + actorIndex * sizeof(void *);
}

// Actors and the view: actor placement conditions from the map's
// view bounds, and clearing placed actors.

#define FLAG_IN_VIEW 0x164     ///< kept clear, so a placement using it appears
#define FLAG_OUT_OF_VIEW 0x165 ///< kept set, so a placement using it does not

#define PLACED_MAX                                                             \
  (ACTOR_ID_COUNT - ACTOR_FIRST_PLACED) ///< placed actors' slots

/// Gives each actor placement with condition 0 a condition saying whether
/// it lies within the map's view bounds, so that only those inside appear.
///
/// Called by scene overlays through Scene_AssignViewFlagsFar. Writes the
/// actor placements' condition, and clears FLAG_IN_VIEW and sets
/// FLAG_OUT_OF_VIEW.
void Scene_AssignViewFlags(struct ActorPlacement *entry) {
  struct MapGridScrollWork *view = gMapGridWork[0];

  GameFlag_ClearFar(FLAG_IN_VIEW);
  GameFlag_SetFar(FLAG_OUT_OF_VIEW);
  while (entry->sprite != -1) {
    /* FAKEMATCH(temporary): the condition read into a u16 local and then an
     * s16 one, which keeps loop.c to one hoisted bound address; as
     * entry->condition == 0, 18 of 57 instructions differ in tbs-en. */
    u16 rawCondition = entry->condition;
    s16 condition = rawCondition;

    if (condition == 0) {
      s32 x = entry->x;
      s32 z = entry->z;

      if (view->minX <= x && x <= view->maxX && view->minY <= z &&
          z <= view->maxY)
        entry->condition = FLAG_IN_VIEW;
      else
        entry->condition = FLAG_OUT_OF_VIEW;
    }
    entry++;
  }
}

/// Releases the placed actors far from the view's centre: more than 160
/// pixels to either side, 200 pixels north or 100 pixels south. Actors
/// at (0, 0) stay.
///
/// Called on the World Map before a Psynergy or a Djinni's joining plays, by
/// Field_UsePsynergy and Djinn_JoinParty; empties the slots it releases.
/// Actor_Respawn brings the actors back afterwards.
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
  s32 remainingSlots;

  camera = work->camera;
  centerX = camera->x.fixed;
  left = centerX - 0xa00000;
  right = centerX + 0xa00000;
  centerZ = camera->z.fixed;
  top = centerZ - 0xc80000;
  bottom = centerZ + 0x640000;
  actorSlot = &work->actors[ACTOR_FIRST_PLACED];
  releaseCacheMask = ~1;

  for (remainingSlots = PLACED_MAX - 1; remainingSlots >= 0; remainingSlots--) {
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

/// Releases every placed actor and places the scene's actors again
/// from its first actor placement table after the leader's.
///
/// Called on the World Map after one, by Field_UsePsynergy and
/// Djinn_JoinParty. Clears work->tables 1 to 3 before the table is spawned
/// again.
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
///
/// Called by Actor_ResetForLeader.
void Actor_ClearAll(void) {
  struct FieldWork *work;
  struct Actor *emptyActor;
  s32 actorIndex;

  emptyActor = NULL;
  actorIndex = ACTOR_ID_COUNT - 1;
  work = gFieldWork;
  do {
    work->actors[actorIndex] = emptyActor;
    actorIndex--;
  } while (actorIndex >= 0);
}

/// Returns the actor in slot index (below ACTOR_ID_COUNT), or
/// NULL. Indices up to 191 read past the actor slots into the rest of
/// the field work; larger ones give NULL.
///
/// The field's actor lookup: called across this folder and,
/// through Actor_LookupFar, by scene
/// overlays.
void *Actor_Lookup(u32 actorIndex) {
  u8 *workBytes = (u8 *)gFieldWork;
  u32 slotOffset;
#ifdef UBFIX
  void *object;
#endif
  if (actorIndex > 0xbf)
    return 0;
  slotOffset = (actorIndex * 4) + 0x14;
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
///
/// Called by SceneSequence_Run before a random battle, by saving the game
/// through Save_CaptureActorsFar, and by two scenes (Tolbi's spring and the
/// Venus Lighthouse aerie).
void Actor_Snapshot(void) {
  struct FieldWork *work;
  struct Actor *snapshot = gActorSnapshot.actors;
  u8 *animations = gActorSnapshot.animations;
  u8 *spriteFlags = gActorSnapshot.spriteFlags;
  u8 *priorities = gActorSnapshot.priorities;
  u8 *indices = gActorSnapshot.slots;
  u32 savedCount = 0;
  s32 actorLimit = sizeof(work->actors) / sizeof(work->actors[0]);
  struct Actor *object;
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
    object = Actor_Lookup(actorIndex);
    if (object == NULL)
      continue;
    *indices++ = actorIndex;
    Dma_Set(object, snapshot,
            (DMA_ENABLE | DMA_32BIT) << 16 | (sizeof(struct Actor) / 4),
            REG_DMA3);
    if (object->spriteStorageKind == ACTOR_SPRITE_ONE) {
      struct Sprite *sprite = object->sprite;
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

/// Puts back the actors Actor_Snapshot saved, and moves the camera
/// to the party leader's height.
///
/// Called by SceneSequence_Run when the party keeps its position
/// (FLAG_KEEP_PARTY_POSITION), and by the Venus Lighthouse aerie scene through
/// Actor_RestoreFar. Only actors whose slots are filled again are
/// restored; the leader is set idle.
void Actor_Restore(void) {
  struct Actor *snapshot = gActorSnapshot.actors;
  u8 *indices = gActorSnapshot.slots;
  u8 *animations = gActorSnapshot.animations;
  u8 *spriteFlags = gActorSnapshot.spriteFlags;
  u8 *priorities = gActorSnapshot.priorities;
  s32 restoredCount;
  struct Actor *object;
  struct Sprite *sprite;
  struct Actor *camera;
  s32 *cameraPosition;
  s32 actorIndex;
  s32 animation;
  s32 priority;
  s32 y;
  s32 *leaderId = &gAdventure.leaderActorId;
  struct FieldWork **fieldWork = &gFieldWork;
  struct MapGridScrollWork **grid = (struct MapGridScrollWork **)gMapGridWork;

  restoredCount = 0;
  actorIndex = *indices++;
  while (actorIndex != ACTOR_SNAPSHOT_EMPTY) {
    object = Actor_Lookup(actorIndex);
    if (object != NULL) {
      // The copy also brings back the sprite pointer saved with the
      // actor. The animation and sprite flags below are set through that
      // pointer; the priorities go to the sprite the actor has now,
      // which is then put back.
      sprite = object->sprite;
      Dma_Set(snapshot, object,
              (DMA_ENABLE | DMA_32BIT) << 16 | (sizeof(struct Actor) / 4),
              REG_DMA3);
      animation = *animations;
      if (animation != 0)
        Actor_SelectAnimationFar(object, animation);
      Actor_SetSpriteFlagsFar(object, *spriteFlags);
      priority = *priorities;
      sprite->image.priority = priority;
      sprite->shadow.priority = priority;
      // Through the sprite union this store moves.
      *(struct Sprite **)&object->sprite = sprite;
      // Re-anchor the camera only when restoring the selected leader.
      if (actorIndex == *leaderId) {
        camera = (*fieldWork)->camera;
        cameraPosition = (*grid)->origin;
        y = object->y.fixed;
        camera->groundHeight = y;
        camera->y.fixed = y;
        cameraPosition[1] = y;
        Actor_StartIdleFar(object);
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
