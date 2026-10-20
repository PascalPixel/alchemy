/// @file
/// The party's sprites, placing a scene's actors, and the actor table.
// Part of object.c.

#include "../object_internal.h"

enum {
  /// 20 flags per element: the Djinn's caught flags are 48 to 127.
  ACTOR_DJINNI_FLAG_COUNT = NUM_ELEMENTS * 20,
  /// The tunic's child part, drawn over a child sprite while
  /// FLAG_ISAAC_WEARS_TUNIC is set.
  ACTOR_TUNIC_ANIMATION = 226
};

/// Returns the sprite of character, which is the character itself except that
/// FLAG_CHILDHOOD gives characters 0 and 1 sprites 0x12 and 0x13 (ISAAC_CHILD
/// and GARET_CHILD), and FLAG_ISAAC_WEARS_TUNIC gives grown Isaac sprite 0x11.
///
/// Called by Event_SpawnObjectTable for every placement, and by
/// PsynergyMenu_InitializeEntryObjects (through
/// Party_RemapCharacterIdByFlagsFar) for the party's figures.
s32 Party_RemapCharacterIdByFlags(s32 character) {
  s32 sprite;

  sprite = character;
  if (sprite <= 8) {
    if (GameFlag_TestFar(FLAG_CHILDHOOD) != 0) {
      if (sprite == 0) {
        sprite = 0x12;
      }
      if (sprite == 1) {
        sprite = 0x13;
      }
    } else if ((GameFlag_TestFar(FLAG_ISAAC_WEARS_TUNIC) != 0) &&
               (sprite == 0)) {
      sprite = 0x11;
    }
  }
  return sprite;
}

/// Returns the sprite of character as Party_RemapCharacterIdByFlags would with
/// FLAG_CHILDHOOD set to childhood (as recorded in a save file's summary).
///
/// Called by SaveMenu_ShowPartySprites through
/// Party_RemapCharacterIdIfEnabledFar. Save summaries omit the tunic flag,
/// so this never gives sprite 0x11.
s32 Party_RemapCharacterIdIfEnabled(s32 character, s32 childhood) {
  s32 sprite;

  sprite = character;
  if ((sprite <= 8) && (childhood != 0)) {
    if (sprite == 0) {
      sprite = 0x12;
    }
    if (sprite == 1) {
      sprite = 0x13;
    }
  }
  return sprite;
}

#define MAP_CELLS ((struct MapCell *)Ram_MapCellBuffer)

extern const struct ScenePlacement gField_DefaultPlacements[2];

/// Creates the actors of a scene's placement table (ended by sprite -1), and
/// records the table among the scene's tables. Sprites 0 to 7 are the party's
/// characters and take their own actor slot; other placements take the actor
/// slots from slot on, one each, up to slot 65. A placement whose condition
/// does not hold is skipped, as is a Djinni hidden after a lost battle outside
/// the world map, but it still uses up its slot: actor slot + n is always row
/// n, counted from 0, of the table's rows of sprite 8 or more, as
/// Scene_FindActorPlacement counts them. An existing actor is moved back to
/// its place unless the party keeps its position. The table is kept by
/// address, so it must last as long as the scene (the overlay's own tables
/// do).
///
/// Called by ObjectTable_ResetForObject (the leader, then the scene's table),
/// BattleEffect_ClearAllObjects and Event_CallWithLastActiveObjectId. Records
/// the table in work->tables and each actor in work->actors.
void Event_SpawnObjectTable(struct ScenePlacement *entry, s32 slot) {
  struct FieldWork *work;
  struct ObjectRuntime *object;
  struct ObjectRuntime *previous;
  struct AnimationObject *sprite;
  s32 tableIndex;
  s32 actorIndex;
  s32 spriteId;
  s32 condition;
  u8 spriteSlot;
  u16 placementSprite;

  work = (struct FieldWork *)gEventWork;
  // Remember the table once, before consuming slots for its placements.
  for (tableIndex = 0; tableIndex < 4; tableIndex++) {
    if (work->tables[tableIndex] == entry)
      break;
    if (work->tables[tableIndex] == 0) {
      work->tables[tableIndex] = entry;
      break;
    }
  }
  for (placementSprite = entry->sprite;
       (s16)placementSprite != -1 && slot <= 65;
       entry++, placementSprite = entry->sprite) {
    // Current placement tables use sprites 0 to 504, so every live row sets
    // actorIndex. XXX: what the 0x2705 cutoff was for; no table reaches it.
    if (entry->sprite <= 7)
      actorIndex = entry->sprite;
    else if (entry->sprite <= 0x2705)
      actorIndex = slot++;
    condition = entry->condition;
    if (!GameFlag_IsConditionActive(condition))
      continue;
    // Djinn_ResolvePendingEvent hides a Djinni after a loss or escape.
    // Scene_ResetFlagsOnEnter clears these temporary flags on a normal entry.
    if ((u32)(condition - FLAG_DJINNI_0) <= ACTOR_DJINNI_FLAG_COUNT - 1 &&
        work->mode != MODE_WORLD_MAP &&
        !GameFlag_IsConditionActive(condition + ACTOR_DJINNI_FLAG_COUNT))
      continue;
    spriteId = Party_RemapCharacterIdByFlags(entry->sprite);
    object = ObjectTable_Get(actorIndex);
    if (object == 0) {
      object = Object_CreateFar(spriteId, entry->x, entry->y, entry->z);
      // Placement flag 1: share the previous actor's sprite resource. The new
      // sprite draws from the previous one's VRAM cache entry, its own entry
      // is freed, and both are marked to keep the shared entry when either
      // is released.
      if (entry->flags & PLACEMENT_SHARE_PREVIOUS_SPRITE) {
        previous = ObjectTable_Get(actorIndex - 1);
        if (previous->animationKind == 1 && object->animationKind == 1) {
          sprite = previous->animation;
          sprite->retainCacheEntry = 1;
          spriteSlot = sprite->slot;
          sprite = object->animation;
          sprite->retainCacheEntry = 1;
          Resource_ResetEntry(sprite->slot);
          sprite->slot = spriteSlot;
        }
      }
      // Dora's tunic overlays the young sprites with its child part.
      if (GameFlag_TestFar(FLAG_ISAAC_WEARS_TUNIC) &&
          (u32)(spriteId - 0x12) <= 1)
        ObjectDispatch_RegisterChildMetadataFar((struct DispatchObject *)object,
                                                ACTOR_TUNIC_ANIMATION);
    } else if (!GameFlag_TestFar(FLAG_KEEP_PARTY_POSITION)) {
      Object_SetPositionAndResetMotionFar(object, entry->x, entry->y, entry->z);
    }

    if (object) {
      ObjectDispatch_SelectAnimationFar(object, 1);
      // lastNo is the animation last selected, which selecting again skips: a
      // random 0 to 29 makes the second selection below restart animation 1
      // from its first frame, unless it happens to be 1. XXX: likely meant to
      // start each idle at a random frame, because the value is random; as it
      // stands it only forces the restart.
      if (object->animationKind == 1 && (sprite = object->animation) != 0)
        sprite->lastNo = ((u32)(Random16()) % (u32)30);
      object->angle = entry->facing;
      // Solid (collision bit 0).
      object->collisionFlags = 1;
      ObjectMotion_SetActionCallback(object, entry->behavior);
      ObjectDispatch_SelectAnimationFar(object, 1);
      ((struct ScriptObjectRuntime *)object)->homeX = object->x / 0x10000;
      ((struct ScriptObjectRuntime *)object)->homeZ = object->z / 0x10000;
      // A placement with a height is lifted half a pixel more and takes motion
      // flags 4, floating: it neither follows the terrain nor falls (bits 0 and
      // 1, field_obj.h). Outside the world map every height is then counted
      // from the terrain below.
      if (object->y != 0) {
        object->flags = 4;
        object->y += 0x8000;
      }
      if (work->mode == MODE_WORLD_MAP) {
        // World-map placements use single sprites and do not follow the
        // terrain (motion bit 0). While the tunic flag is set, all keep their
        // native scale; otherwise they draw at 3/4 size.
        object->flags &= 0xfe;
        if (!GameFlag_TestFar(FLAG_ISAAC_WEARS_TUNIC)) {
          // UB: sprite creation can fail while the actor slot stays live.
          // The native local is then indeterminate or from an earlier row.
          // Checking the sprite grows tbs-en from 608 to 628 bytes.
#ifdef UBFIX
          if (object->animationKind == 1 &&
              (sprite = object->animation) != NULL)
#endif
            sprite->scale = Iwram_MulQ16(sprite->scale, 3 * Q16_ONE / 4);
        }
      } else {
        object->terrainHeight =
            Map_GetTerrainHeightFar(0, object->x, object->z);
        object->y += object->terrainHeight;
      }
      object->priorityFlags = 1;
    }
    work->actors[actorIndex] = (struct FieldActor *)object;
  }
}

/// Destroys the actor in slot actorIndex, if there is one, and empties the
/// slot. No caller names it; its far-call veneer has no exported name.
void ObjectTable_DestroyAtIndex(s32 actorIndex) {
  struct FieldWork *work = (struct FieldWork *)gEventWork;
  void *object = ObjectTable_Get(actorIndex);

  if (object != NULL) {
    ObjectDispatch_ReleaseFar(object);
#ifdef UBFIX
    {
      void *empty = NULL;

      // Get also accepts words past the actor array, within the field work.
      memcpy((u8 *)work + 0x14 + (u32)actorIndex * sizeof(empty), &empty,
             sizeof(empty));
    }
#else
    work->actors[actorIndex] = NULL;
#endif
  }
}

/// Creates a scene's actors on entering it: the party leader where the game
/// state puts it, then the scene's placement table from actor slot 8, and the
/// camera that follows the leader. A leader entering above ground where its
/// cell and the cell north of it are climbing spots (collision code 0xfd) is
/// raised by the ground height of the cell north of it less 32 pixels, and
/// starts climbing (movement mode 1).
///
/// Called by Field_RunScene on entering a scene, and by
/// Event_RunObjectHookAndWait.
/// Empties every actor slot and table first, without destroying the actors in
/// them; sets work->camera, the map's origin and gGameState.movementMode.
void ObjectTable_ResetForObject(struct ScenePlacement *table) {
  struct FieldWork *work;
  struct ScenePlacement *entry;
  struct ObjectRuntime *object;
  struct ObjectRuntime *camera;
  struct MapCell *cell;
  struct MapCell *above;
  s32 leader;
  s32 pos;
  s32 height;

  work = (struct FieldWork *)gEventWork;
  leader = gGameState.selectedActor;
  // Build the leader's temporary placement before the scene's placed actors.
  // The two-row table, the leader's row and an end row, lives in the event
  // work at 0x200 (FieldWork's unk200) and lasts with it.
  entry = (struct ScenePlacement *)((u8 *)work + 0x200);
  entry[0] = gField_DefaultPlacements[0];
  entry[1] = gField_DefaultPlacements[1];
  for (pos = 0; pos < 4; pos++)
    work->tables[pos] = 0;
  ObjectTable_ClearAll();
  entry->condition = -1;
  entry->sprite = leader;
  SET_POSITION(entry, gGameState.x, 0, gGameState.z);
  entry->facing = gGameState.heading;
  Event_SpawnObjectTable(entry, leader);
  Event_SpawnObjectTable(table, ACTOR_FIRST_PLACED);

  object = (struct ObjectRuntime *)work->actors[leader];
  // gGameState.turn keeps the leader's map layer (Field_RunScene saves its
  // terrainId there). Its cell: 16 pixels square, 128 to a row, so above is
  // the cell north of it.
  object->terrainId = gGameState.turn;
  pos = (object->x / 0x100000) + (object->z / 0x100000) * 128;
  cell = &MAP_CELLS[pos];
  above = &MAP_CELLS[pos - 128];
  if (gGameState.y != 0 && cell->collisionCode == 0xfd &&
      above->collisionCode == 0xfd) {
    gGameState.movementMode = 1;
    height =
        Map_GetTerrainHeightFar(0, object->x, object->z - 0x100000) - 0x200000;
    object->y += height;
    object->terrainHeight = object->y;
    object->flags = 0;
    ObjectDispatch_SetSingleChildFlagsFar((struct DispatchObject *)object, 0);
    ObjectDispatch_SelectAnimationFar(object, 12);
  } else {
    gGameState.movementMode = 0;
  }

  // The camera follows this leader and starts at its resolved ground height;
  // the map scrolls by the camera's position. Id 0x8000 is object kind 8
  // (id / 4096, FieldObject_Create), which takes no sprite.
  camera = Object_CreateFar(0x8000, object->x, object->y, object->z);
  camera->terrainHeight = object->terrainHeight;
  ObjectDispatch_StartCameraFar((struct DispatchObject *)camera, (s32)object);
  if (work->mode == MODE_WORLD_MAP) {
    struct AnimationEntry *meta =
        (struct AnimationEntry *)ResourceMetadata_RegisterFar(object->animation,
                                                              23);
    meta->param = 15;
    meta->priority = 9;
  }
  ((struct MapScrollWork *)gMapWork[0])->origin = &camera->x;
  work->camera = camera;
}

/// Returns the slot after the last placed actor (8 when there is none), or -1
/// when slot 65 is occupied. Earlier holes are not reused by this append scan.
///
/// Called by BattleEffect_ClearAllObjects and Event_CallWithLastActiveObjectId
/// to choose where Event_SpawnObjectTable starts.
s32 ObjectTable_FindLastActiveId(void) {
  struct FieldWork *work = (struct FieldWork *)gEventWork;
  s32 lastActorIndex = ACTOR_FIRST_PLACED - 1;
  s32 actorIndex = ACTOR_FIRST_PLACED;
  struct FieldActor **slot = &work->actors[ACTOR_FIRST_PLACED];

  do {
    struct FieldActor *object = *slot++;

    if (object != 0) {
      lastActorIndex = actorIndex;
    }
    actorIndex++;
  } while (actorIndex <= 65);
  lastActorIndex++;
  if (lastActorIndex == 66) {
    lastActorIndex = -1;
  }
  return lastActorIndex;
}

/// Returns the address of actor slot index in the event work.
UNUSED void *ObjectTable_GetSlotAddress(u32 actorIndex) {
  return (u8 *)gEventWork + 0x14 + actorIndex * sizeof(void *);
}

// Actors and the view: placement conditions from the map's view bounds, and
// clearing placed actors.

#define FLAG_IN_VIEW 0x164     ///< kept clear, so a placement using it appears
#define FLAG_OUT_OF_VIEW 0x165 ///< kept set, so a placement using it does not

#define PLACED_MAX 58 ///< actor slots 8 to 65

/// Gives each placement with condition 0 a condition saying whether it lies
/// within the map's view bounds, so that only those inside appear.
///
/// Called by scene overlays through Scene_AssignViewFlagsFar, which they import
/// under four local names (FieldScene_PrepareActors the most used). Writes the
/// placements' condition, and clears FLAG_IN_VIEW and sets FLAG_OUT_OF_VIEW.
void Scene_AssignViewFlags(struct ScenePlacement *entry) {
  struct MapScrollWork *view = gMapWork[0];

  GameFlag_ClearBitFar(FLAG_IN_VIEW);
  GameFlag_SetBitFar(FLAG_OUT_OF_VIEW);
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

/// Destroys the placed actors far from the view's centre: more than 160 pixels
/// to either side, 200 pixels north or 100 pixels south. Actors at (0, 0) stay.
///
/// Called on the world map before an effect plays, by Field_UsePsynergy and
/// Djinn_JoinParty; empties the slots it frees. BattleEffect_ClearAllObjects
/// brings the actors back afterwards.
void BattleEffect_ClearOutOfBoundsObjects(void) {
  struct FieldWork *work = (struct FieldWork *)gEventWork;
  struct ObjectRuntime *camera;
  s32 centerX;
  s32 centerZ;
  s32 left;
  s32 right;
  s32 top;
  s32 bottom;
  struct FieldActor **actorSlot;
  s32 releaseCacheMask;
  s32 remainingSlots;

  camera = work->camera;
  centerX = camera->x;
  left = centerX - 0xa00000;
  right = centerX + 0xa00000;
  centerZ = camera->z;
  top = centerZ - 0xc80000;
  bottom = centerZ + 0x640000;
  actorSlot = &work->actors[ACTOR_FIRST_PLACED];
  releaseCacheMask = ~1;

  for (remainingSlots = PLACED_MAX - 1; remainingSlots >= 0; remainingSlots--) {
    struct FieldActor *actor = *actorSlot;

    if (actor != NULL) {
      s32 actorX = actor->x.fixed;
      s32 actorZ = actor->z.fixed;

      if (actorX != 0 || actorZ != 0) {
        if (actorX < left || actorX > right || actorZ < top ||
            actorZ > bottom) {
          u8 *sprite;
          u8 *removed = &actor->spriteStorageKind;
          u32 flags;

          // Kind 1 makes the release free the actor's one sprite, and clearing
          // retainCacheEntry (bit 0 of sprite byte 29) frees its VRAM cache
          // entry with it, even one shared with a neighbour
          // (PLACEMENT_SHARE_PREVIOUS_SPRITE).
          *removed = 1;
          sprite = (u8 *)actor->sprite;
          flags = sprite[29];
          sprite[29] = flags & releaseCacheMask;
          ObjectDispatch_ReleaseFar((struct DispatchObject *)actor);
          sprite = NULL;
          *actorSlot = (struct FieldActor *)sprite;
        }
      }
    }

    actorSlot++;
  }
}

/// Destroys every placed actor and places the scene's actors again from its
/// first placement table after the leader's.
///
/// Called on the world map after an effect, by Field_UsePsynergy and
/// Djinn_JoinParty. Clears work->tables 1 to 3 before the table is spawned
/// again.
void BattleEffect_ClearAllObjects(void) {
  struct FieldWork *work = (struct FieldWork *)gEventWork;
  struct ScenePlacement *placements;
  s32 slotIndex;

  for (slotIndex = 0; slotIndex < PLACED_MAX; slotIndex++) {
    struct FieldActor *actor = work->actors[ACTOR_FIRST_PLACED + slotIndex];

    if (actor != 0) {
      // As in BattleEffect_ClearOutOfBoundsObjects: free the sprite and its
      // VRAM cache entry with the actor.
      actor->spriteStorageKind = 1;
      actor->sprite->retainCacheEntry = 0;
      ObjectDispatch_ReleaseFar((struct DispatchObject *)actor);
      work->actors[ACTOR_FIRST_PLACED + slotIndex] = 0;
    }
  }
  placements = work->tables[1];
  work->tables[1] = 0;
  work->tables[2] = 0;
  work->tables[3] = 0;
  if (placements != 0)
    Event_SpawnObjectTable(placements, ObjectTable_FindLastActiveId());
}

/// Empties all 66 actor slots, without destroying the actors.
///
/// Called by ObjectTable_ResetForObject.
void ObjectTable_ClearAll(void) {
  struct FieldWork *work;
  struct FieldActor *emptyActor;
  s32 actorIndex;

  emptyActor = NULL;
  actorIndex = 65;
  work = (struct FieldWork *)gEventWork;
  do {
    work->actors[actorIndex] = emptyActor;
    actorIndex--;
  } while (actorIndex >= 0);
}

/// Returns the actor in slot index (0 to 65), or NULL. Indices up to 191 read
/// past the actor slots into the rest of the event work; larger ones give NULL.
///
/// The field's actor lookup: called across the object folder and, through
/// ObjectTable_GetFar (Engine_ActorLookup), by scene overlays.
void *ObjectTable_Get(u32 actorIndex) {
  u8 *workBytes = (u8 *)gEventWork;
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

/// Saves up to 32 actors (only the party's 8 on the world map) into
/// gObjectSnapshots, with each one's slot, animation, sprite flags and
/// priority, so that ObjectTable_Restore can put them back.
///
/// Called by Field_RunScene before a random battle, by the save menu through
/// SaveState_CaptureObjectTableFar, and by two scenes (Tolbi's spring and the
/// Venus Lighthouse aerie).
void ObjectTable_Snapshot(void) {
  struct FieldWork *work;
  struct ObjectRuntime *snapshot = gObjectSnapshots.actors;
  u8 *animations = gObjectSnapshots.animations;
  u8 *spriteFlags = gObjectSnapshots.spriteFlags;
  u8 *priorities = gObjectSnapshots.priorities;
  u8 *indices = gObjectSnapshots.slots;
  u32 savedCount = 0;
  s32 actorLimit = sizeof(work->actors) / sizeof(work->actors[0]);
  struct ObjectRuntime *object;
  s32 actorIndex;
  u32 animation;
  u32 spriteFlagBits;
  u32 priority;

  work = (struct FieldWork *)gEventWork;
  if (work->mode == MODE_WORLD_MAP)
    actorLimit = ACTOR_FIRST_PLACED;
  // Keep each actor and its three sprite-state bytes at matching save indices.
  for (actorIndex = 0; actorIndex < actorLimit; actorIndex++) {
    object = ObjectTable_Get(actorIndex);
    if (object == NULL)
      continue;
    *indices++ = actorIndex;
    Dma_Set(object, snapshot,
            (DMA_ENABLE | DMA_32BIT) << 16 | (sizeof(struct ObjectRuntime) / 4),
            REG_DMA3);
    if (object->animationKind == 1) {
      struct AnimationObject *sprite = object->animation;
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
    if (savedCount > OBJECT_SNAPSHOT_CAPACITY - 1)
      break;
  }
  for (actorIndex = savedCount; actorIndex < OBJECT_SNAPSHOT_CAPACITY;
       actorIndex++)
    *indices++ = OBJECT_SNAPSHOT_EMPTY;
}

/// Puts back the actors ObjectTable_Snapshot saved, and moves the camera to the
/// party leader's height.
///
/// Called by Field_RunScene when the party keeps its position
/// (FLAG_KEEP_PARTY_POSITION), and by the Venus Lighthouse aerie scene through
/// ObjectTable_RestoreFar. Only actors whose slots are filled again are
/// restored; the leader is set idle.
void ObjectTable_Restore(void) {
  struct ObjectRuntime *snapshot = gObjectSnapshots.actors;
  u8 *indices = gObjectSnapshots.slots;
  u8 *animations = gObjectSnapshots.animations;
  u8 *spriteFlags = gObjectSnapshots.spriteFlags;
  u8 *priorities = gObjectSnapshots.priorities;
  s32 restoredCount;
  struct ObjectRuntime *object;
  struct AnimationObject *sprite;
  struct ObjectRuntime *camera;
  s32 *cameraPosition;
  s32 actorIndex;
  s32 animation;
  s32 priority;
  s32 y;
  s32 *leaderId = &gGameState.selectedActor;
  struct EventWork **fieldWork = &gEventWork;
  struct MapScrollWork **mapWork = (struct MapScrollWork **)gMapWork;

  restoredCount = 0;
  actorIndex = *indices++;
  while (actorIndex != OBJECT_SNAPSHOT_EMPTY) {
    object = ObjectTable_Get(actorIndex);
    if (object != NULL) {
      // The copy also brings back the sprite pointer saved with the actor.
      // The animation and sprite flags below are set through that pointer;
      // the priorities go to the sprite the actor has now, which is then put
      // back.
      sprite = object->animation;
      Dma_Set(snapshot, object,
              (DMA_ENABLE | DMA_32BIT) << 16 |
                  (sizeof(struct ObjectRuntime) / 4),
              REG_DMA3);
      animation = *animations;
      if (animation != 0)
        ObjectDispatch_SelectAnimationFar(object, animation);
      ObjectDispatch_SetSingleChildFlagsFar((struct DispatchObject *)object,
                                            *spriteFlags);
      priority = *priorities;
      sprite->image.priority = priority;
      sprite->shadow.priority = priority;
      object->animation = sprite;
      // Re-anchor the camera only when restoring the selected leader.
      if (actorIndex == *leaderId) {
        camera = ((struct FieldWork *)*fieldWork)->camera;
        cameraPosition = (*mapWork)->origin;
        y = object->y;
        camera->terrainHeight = y;
        camera->y = y;
        cameraPosition[1] = y;
        ObjectDispatch_StartIdleFar((struct DispatchObject *)object);
      }
    }
    snapshot++;
    animations++;
    spriteFlags++;
    priorities++;
    restoredCount++;
    if (restoredCount > OBJECT_SNAPSHOT_CAPACITY - 1)
      break;
    actorIndex = *indices++;
  }
}

#undef MAP_CELLS
#undef FLAG_IN_VIEW
#undef FLAG_OUT_OF_VIEW
#undef PLACED_MAX
