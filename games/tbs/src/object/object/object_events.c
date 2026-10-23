/// @file
/// The party's sprites, placing a map's object events, and the object event
/// table.
// Part of object.c.

#include "../object_internal.h"

enum {
  /// 20 flags per element: the Djinn's caught flags are 48 to 127.
  OBJ_EVENT_DJINNI_FLAG_COUNT = ELEMENT_COUNT * 20,
  /// The field work's object event slots: the party's eight, then the placed
  /// object events.
  OBJ_EVENT_SLOT_COUNT = 66
};

/// Returns the sprite of character, which is the character itself except that
/// FLAG_CHILDHOOD gives Isaac and Garet ISAAC_CHILD and GARET_CHILD, and
/// FLAG_ISAAC_WEARS_TUNIC gives grown Isaac sprite 0x11.
///
/// Called by FieldScript_SpawnObjectTable for every object event template, and
/// by PsynergyMenu_InitializeEntryObjects (through
/// Party_RemapCharacterIdByFlagsFar) for the party's figures.
s32 Party_RemapCharacterIdByFlags(s32 character) {
  s32 sprite;

  sprite = character;
  if (sprite <= 8) {
    if (GameFlag_TestFar(FLAG_CHILDHOOD) != 0) {
      if (sprite == ISAAC) {
        sprite = ISAAC_CHILD;
      }
      if (sprite == GARET) {
        sprite = GARET_CHILD;
      }
    } else if ((GameFlag_TestFar(FLAG_ISAAC_WEARS_TUNIC) != 0) &&
               (sprite == ISAAC)) {
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

extern const struct ObjectEventTemplate gField_DefaultObjectEventTemplates[2];

/// Creates the object events of a map's object event template table (ended by
/// sprite -1), and records the table among the map's tables. Sprites 0 to 7 are
/// the party's characters and take their own object event slot; other object
/// event templates take the object event slots from slot on, one each, up to
/// the last. An object event template whose condition does not hold is skipped,
/// as is a Djinni hidden after a lost battle outside the world map, but it
/// still uses up its slot: object event slot + n is always row n, counted from
/// 0, of the table's rows of sprite 8 or more, as Map_FindObjectEventTemplate
/// counts them. An existing object event is moved back to its place unless the
/// party keeps its position. The table is kept by address, so it must last as
/// long as the map (the overlay's own tables do).
///
/// Called by ObjectTable_ResetForObject (the leader, then the map's table),
/// ObjectTable_Respawn and FieldScript_CallWithLastActiveObjectId. Records
/// the table in work->tables and each object event in work->objectEvents.
void FieldScript_SpawnObjectTable(struct ObjectEventTemplate *entry, s32 slot) {
  struct FieldWork *work;
  struct ObjectRuntime *object;
  struct ObjectRuntime *previous;
  struct AnimationObject *sprite;
  s32 tableIndex;
  s32 objectEventIndex;
  s32 spriteId;
  s32 condition;
  u8 spriteSlot;
  u16 templateSprite;

  work = (struct FieldWork *)gEventWork;
  // Remember the table once, before consuming slots for its object event
  // templates.
  for (tableIndex = 0; tableIndex < 4; tableIndex++) {
    if (work->tables[tableIndex] == entry)
      break;
    if (work->tables[tableIndex] == 0) {
      work->tables[tableIndex] = entry;
      break;
    }
  }
  for (templateSprite = entry->sprite;
       (s16)templateSprite != -1 && slot <= OBJ_EVENT_SLOT_COUNT - 1;
       entry++, templateSprite = entry->sprite) {
    // Current object event template tables use sprites 0 to 504, so every live
    // row sets objectEventIndex. XXX: what the 0x2705 cutoff was for; no table
    // reaches it.
    if (entry->sprite <= 7)
      objectEventIndex = entry->sprite;
    else if (entry->sprite <= 0x2705)
      objectEventIndex = slot++;
    condition = entry->condition;
    if (!GameFlag_IsConditionActive(condition))
      continue;
    // Djinn_ResolvePendingEvent hides a Djinni after a loss or escape.
    // Map_ResetFlagsOnEnter clears these temporary flags on a normal entry.
    if ((u32)(condition - FLAG_DJINNI_0) <= OBJ_EVENT_DJINNI_FLAG_COUNT - 1 &&
        work->mode != MODE_WORLD_MAP &&
        !GameFlag_IsConditionActive(condition + OBJ_EVENT_DJINNI_FLAG_COUNT))
      continue;
    spriteId = Party_RemapCharacterIdByFlags(entry->sprite);
    object = ObjectTable_Get(objectEventIndex);
    if (object == 0) {
      object = Object_CreateFar(spriteId, entry->x, entry->y, entry->z);
      // OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE: share the previous object
      // event's sprite resource. The new sprite draws from the previous one's
      // VRAM cache entry, its own entry is freed, and both are marked to keep
      // the shared entry when either is released.
      if (entry->flags & OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE) {
        previous = ObjectTable_Get(objectEventIndex - 1);
        if (previous->animationKind == OBJECT_SPRITE_ONE &&
            object->animationKind == OBJECT_SPRITE_ONE) {
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
          (u32)(spriteId - ISAAC_CHILD) <= GARET_CHILD - ISAAC_CHILD)
        ObjectDispatch_RegisterChildMetadataFar((struct DispatchObject *)object,
                                                SPRITE_TUNIC);
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
      if (object->animationKind == OBJECT_SPRITE_ONE &&
          (sprite = object->animation) != 0)
        sprite->lastNo = ((u32)(Random16()) % (u32)30);
      object->angle = entry->facing;
      // Solid.
      object->collisionFlags = 1;
      ObjectMotion_SetActionCallback(object, entry->behavior);
      ObjectDispatch_SelectAnimationFar(object, 1);
      ((struct ScriptObjectRuntime *)object)->homeX = object->x / 0x10000;
      ((struct ScriptObjectRuntime *)object)->homeZ = object->z / 0x10000;
      // An object event template with a height is lifted half a pixel more and
      // takes motion flags 4, floating: it neither follows the terrain nor
      // falls. Outside the world map every height is then counted from the
      // terrain below.
      if (object->y != 0) {
        object->flags = 4;
        object->y += 0x8000;
      }
      if (work->mode == MODE_WORLD_MAP) {
        // World-map object event templates use single sprites and do not follow
        // the terrain. While the tunic flag is set, all keep their native
        // scale; otherwise they draw at 3/4 size.
        object->flags &= 0xfe;
        if (!GameFlag_TestFar(FLAG_ISAAC_WEARS_TUNIC)) {
          // UB: sprite creation can fail while the object event slot stays
          // live. The native local is then indeterminate or from an earlier
          // row. Checking the sprite grows tbs-en from 608 to 628 bytes.
#ifdef UBFIX
          if (object->animationKind == OBJECT_SPRITE_ONE &&
              (sprite = object->animation) != NULL)
#endif
            sprite->scale = Iwram_MulQ16(sprite->scale, 3 * Q16_ONE / 4);
        }
      } else {
        object->terrainHeight =
            MapGrid_GetTerrainHeightFar(0, object->x, object->z);
        object->y += object->terrainHeight;
      }
      object->priorityFlags = OBJ_EVENT_PRIORITY_AUTOMATIC;
    }
    work->objectEvents[objectEventIndex] = (struct ObjectEvent *)object;
  }
}

/// Destroys the object event in slot objectEventIndex, if there is one, and
/// empties the slot. No caller names it; its far-call veneer has no exported
/// name.
void ObjectTable_DestroyAtIndex(s32 objectEventIndex) {
  struct FieldWork *work = (struct FieldWork *)gEventWork;
  void *object = ObjectTable_Get(objectEventIndex);

  if (object != NULL) {
    ObjectDispatch_ReleaseFar(object);
#ifdef UBFIX
    {
      void *empty = NULL;

      // Get also accepts words past the object event array, within the field
      // work.
      memcpy((u8 *)work + 0x14 + (u32)objectEventIndex * sizeof(empty), &empty,
             sizeof(empty));
    }
#else
    work->objectEvents[objectEventIndex] = NULL;
#endif
  }
}

/// Creates a map's object events on entering it: the party leader where the
/// game state puts it, then the map's object event template table from object
/// event slot 8, and the camera that follows the leader. A leader entering
/// above ground where its cell and the cell north of it are climbing spots
/// (MB_CLIMB) is raised by the ground height of the cell north of it less 32
/// pixels, and starts climbing (LEADER_MOVEMENT_CLIMB).
///
/// Called by Field_RunMap on entering a map, and by
/// FieldScript_RunObjectHookAndWait.
/// Empties every object event slot and table first, without destroying the
/// object events in them; sets work->camera, the map's origin and
/// gGameState.movementMode.
void ObjectTable_ResetForObject(struct ObjectEventTemplate *table) {
  struct FieldWork *work;
  struct ObjectEventTemplate *entry;
  struct ObjectRuntime *object;
  struct ObjectRuntime *camera;
  struct MapGridCell *cell;
  struct MapGridCell *above;
  s32 leader;
  s32 pos;
  s32 height;

  work = (struct FieldWork *)gEventWork;
  leader = gGameState.selectedObjectEvent;
  // Build the leader's temporary object event template before the map's placed
  // object events. The two-row table, the leader's row and an end row, lives in
  // the event work at 0x200 (FieldWork's leaderTemplates) and lasts with it.
  entry = (struct ObjectEventTemplate *)((u8 *)work + 0x200);
  entry[0] = gField_DefaultObjectEventTemplates[0];
  entry[1] = gField_DefaultObjectEventTemplates[1];
  for (pos = 0; pos < 4; pos++)
    work->tables[pos] = 0;
  ObjectTable_ClearAll();
  entry->condition = -1;
  entry->sprite = leader;
  SET_POSITION(entry, gGameState.x, 0, gGameState.z);
  entry->facing = gGameState.heading;
  FieldScript_SpawnObjectTable(entry, leader);
  FieldScript_SpawnObjectTable(table, LOCALID_FIRST_PLACED);

  object = (struct ObjectRuntime *)work->objectEvents[leader];
  // gGameState.turn keeps the leader's map grid layer (Field_RunMap saves its
  // terrainId there). Its cell: 16 pixels square, 128 to a row, so above is
  // the cell north of it.
  object->terrainId = gGameState.turn;
  pos = (object->x / 0x100000) + (object->z / 0x100000) * 128;
  cell = &MAPGRID_CELLS[pos];
  above = &MAPGRID_CELLS[pos - 128];
  if (gGameState.y != 0 && cell->collisionCode == MB_CLIMB &&
      above->collisionCode == MB_CLIMB) {
    gGameState.movementMode = LEADER_MOVEMENT_CLIMB;
    height = MapGrid_GetTerrainHeightFar(0, object->x, object->z - 0x100000) -
             0x200000;
    object->y += height;
    object->terrainHeight = object->y;
    object->flags = 0;
    ObjectDispatch_SetSingleChildFlagsFar((struct DispatchObject *)object, 0);
    ObjectDispatch_SelectAnimationFar(object, 12);
  } else {
    gGameState.movementMode = LEADER_MOVEMENT_WALK;
  }

  // The camera follows this leader and starts at its resolved ground height;
  // the map scrolls by the camera's position. It takes no sprite.
  camera = Object_CreateFar(OBJECT_CREATE_NO_SPRITE << 12, object->x, object->y,
                            object->z);
  camera->terrainHeight = object->terrainHeight;
  ObjectDispatch_StartCameraFar((struct DispatchObject *)camera, (s32)object);
  if (work->mode == MODE_WORLD_MAP) {
    struct AnimationEntry *meta =
        (struct AnimationEntry *)ResourceMetadata_RegisterFar(object->animation,
                                                              23);
    meta->colorMode = SPRITE_COLOR_HIDDEN;
    meta->priority = 9;
  }
  ((struct MapGridScrollWork *)gMapGridWork[0])->origin = &camera->x;
  work->camera = camera;
}

/// Returns the slot after the last placed object event (8 when there is none),
/// or -1 when the last slot is occupied. Earlier holes are not reused by this
/// append scan.
///
/// Called by ObjectTable_Respawn and FieldScript_CallWithLastActiveObjectId
/// to choose where FieldScript_SpawnObjectTable starts.
s32 ObjectTable_FindLastActiveId(void) {
  struct FieldWork *work = (struct FieldWork *)gEventWork;
  s32 lastObjectEventIndex = LOCALID_FIRST_PLACED - 1;
  s32 objectEventIndex = LOCALID_FIRST_PLACED;
  struct ObjectEvent **slot = &work->objectEvents[LOCALID_FIRST_PLACED];

  do {
    struct ObjectEvent *objectEvent = *slot++;

    if (objectEvent != 0) {
      lastObjectEventIndex = objectEventIndex;
    }
    objectEventIndex++;
  } while (objectEventIndex <= OBJ_EVENT_SLOT_COUNT - 1);
  lastObjectEventIndex++;
  if (lastObjectEventIndex == OBJ_EVENT_SLOT_COUNT) {
    lastObjectEventIndex = -1;
  }
  return lastObjectEventIndex;
}

/// Unused: returns the address of object event slot index in the event work.
/// Nothing calls it.
UNUSED void *ObjectTable_GetSlotAddress(u32 objectEventIndex) {
  return (u8 *)gEventWork + 0x14 + objectEventIndex * sizeof(void *);
}

// Object events and the view: object event template conditions from the map's
// view bounds, and clearing placed object events.

#define FLAG_IN_VIEW 0x164     ///< kept clear, so a template using it appears
#define FLAG_OUT_OF_VIEW 0x165 ///< kept set, so a template using it does not

#define PLACED_MAX                                                             \
  (OBJ_EVENT_SLOT_COUNT - LOCALID_FIRST_PLACED) ///< placed object events' slots

/// Gives each object event template with condition 0 a condition saying whether
/// it lies within the map's view bounds, so that only those inside appear.
///
/// Called by map overlays through Map_AssignViewFlagsFar, which they import
/// under four local names (Map_PrepareObjectEvents the most used). Writes the
/// object event templates' condition, and clears FLAG_IN_VIEW and sets
/// FLAG_OUT_OF_VIEW.
void Map_AssignViewFlags(struct ObjectEventTemplate *entry) {
  struct MapGridScrollWork *view = gMapGridWork[0];

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

/// Destroys the placed object events far from the view's centre: more than 160
/// pixels to either side, 200 pixels north or 100 pixels south. Object events
/// at (0, 0) stay.
///
/// Called on the world map before an effect plays, by Field_UsePsynergy and
/// Djinn_JoinParty; empties the slots it frees. ObjectTable_Respawn
/// brings the object events back afterwards.
void ObjectTable_ClearOutOfBounds(void) {
  struct FieldWork *work = (struct FieldWork *)gEventWork;
  struct ObjectRuntime *camera;
  s32 centerX;
  s32 centerZ;
  s32 left;
  s32 right;
  s32 top;
  s32 bottom;
  struct ObjectEvent **objectEventSlot;
  s32 releaseCacheMask;
  s32 remainingSlots;

  camera = work->camera;
  centerX = camera->x;
  left = centerX - 0xa00000;
  right = centerX + 0xa00000;
  centerZ = camera->z;
  top = centerZ - 0xc80000;
  bottom = centerZ + 0x640000;
  objectEventSlot = &work->objectEvents[LOCALID_FIRST_PLACED];
  releaseCacheMask = ~1;

  for (remainingSlots = PLACED_MAX - 1; remainingSlots >= 0; remainingSlots--) {
    struct ObjectEvent *objectEvent = *objectEventSlot;

    if (objectEvent != NULL) {
      s32 objectEventX = objectEvent->x.fixed;
      s32 objectEventZ = objectEvent->z.fixed;

      if (objectEventX != 0 || objectEventZ != 0) {
        if (objectEventX < left || objectEventX > right || objectEventZ < top ||
            objectEventZ > bottom) {
          u8 *sprite;
          u8 *removed = &objectEvent->spriteStorageKind;
          u32 flags;

          // OBJECT_SPRITE_ONE makes the release free the object event's one
          // sprite, and clearing retainCacheEntry (bit 0 of sprite byte 29)
          // frees its VRAM cache entry with it, even one shared with a
          // neighbour (OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE).
          *removed = OBJECT_SPRITE_ONE;
          sprite = (u8 *)objectEvent->sprite;
          flags = sprite[29];
          sprite[29] = flags & releaseCacheMask;
          ObjectDispatch_ReleaseFar((struct DispatchObject *)objectEvent);
          sprite = NULL;
          *objectEventSlot = (struct ObjectEvent *)sprite;
        }
      }
    }

    objectEventSlot++;
  }
}

/// Destroys every placed object event and places the map's object events again
/// from its first object event template table after the leader's.
///
/// Called on the world map after an effect, by Field_UsePsynergy and
/// Djinn_JoinParty. Clears work->tables 1 to 3 before the table is spawned
/// again.
void ObjectTable_Respawn(void) {
  struct FieldWork *work = (struct FieldWork *)gEventWork;
  struct ObjectEventTemplate *templates;
  s32 slotIndex;

  for (slotIndex = 0; slotIndex < PLACED_MAX; slotIndex++) {
    struct ObjectEvent *objectEvent =
        work->objectEvents[LOCALID_FIRST_PLACED + slotIndex];

    if (objectEvent != 0) {
      // As in ObjectTable_ClearOutOfBounds: free the sprite and its
      // VRAM cache entry with the object event.
      objectEvent->spriteStorageKind = OBJECT_SPRITE_ONE;
      objectEvent->sprite->retainCacheEntry = 0;
      ObjectDispatch_ReleaseFar((struct DispatchObject *)objectEvent);
      work->objectEvents[LOCALID_FIRST_PLACED + slotIndex] = 0;
    }
  }
  templates = work->tables[1];
  work->tables[1] = 0;
  work->tables[2] = 0;
  work->tables[3] = 0;
  if (templates != 0)
    FieldScript_SpawnObjectTable(templates, ObjectTable_FindLastActiveId());
}

/// Empties all the object event slots, without destroying the object events.
///
/// Called by ObjectTable_ResetForObject.
void ObjectTable_ClearAll(void) {
  struct FieldWork *work;
  struct ObjectEvent *emptyObjectEvent;
  s32 objectEventIndex;

  emptyObjectEvent = NULL;
  objectEventIndex = OBJ_EVENT_SLOT_COUNT - 1;
  work = (struct FieldWork *)gEventWork;
  do {
    work->objectEvents[objectEventIndex] = emptyObjectEvent;
    objectEventIndex--;
  } while (objectEventIndex >= 0);
}

/// Returns the object event in slot index (below OBJ_EVENT_SLOT_COUNT), or
/// NULL. Indices up to 191 read past the object event slots into the rest of
/// the event work; larger ones give NULL.
///
/// The field's object event lookup: called across the object folder and,
/// through ObjectTable_GetFar (Engine_ObjectEventLookup), by map overlays.
void *ObjectTable_Get(u32 objectEventIndex) {
  u8 *workBytes = (u8 *)gEventWork;
  u32 slotOffset;
#ifdef UBFIX
  void *object;
#endif
  if (objectEventIndex > 0xbf)
    return 0;
  slotOffset = (objectEventIndex * 4) + 0x14;
#ifdef UBFIX
  // The legacy range includes non-pointer fields. Copy their representation
  // without reading them through an incompatible pointer lvalue.
  memcpy(&object, workBytes + slotOffset, sizeof(object));
  return object;
#else
  return *(void **)(workBytes + slotOffset);
#endif
}

/// Saves up to 32 object events (only the party's 8 on the world map) into
/// gObjectSnapshots, with each one's slot, animation, sprite flags and
/// priority, so that ObjectTable_Restore can put them back.
///
/// Called by Field_RunMap before a random battle, by the save menu through
/// SaveState_CaptureObjectTableFar, and by two maps (Tolbi's spring and the
/// Venus Lighthouse aerie).
void ObjectTable_Snapshot(void) {
  struct FieldWork *work;
  struct ObjectRuntime *snapshot = gObjectSnapshots.objectEvents;
  u8 *animations = gObjectSnapshots.animations;
  u8 *spriteFlags = gObjectSnapshots.spriteFlags;
  u8 *priorities = gObjectSnapshots.priorities;
  u8 *indices = gObjectSnapshots.slots;
  u32 savedCount = 0;
  s32 objectEventLimit =
      sizeof(work->objectEvents) / sizeof(work->objectEvents[0]);
  struct ObjectRuntime *object;
  s32 objectEventIndex;
  u32 animation;
  u32 spriteFlagBits;
  u32 priority;

  work = (struct FieldWork *)gEventWork;
  if (work->mode == MODE_WORLD_MAP)
    objectEventLimit = LOCALID_FIRST_PLACED;
  // Keep each object event and its three sprite-state bytes at matching save
  // indices.
  for (objectEventIndex = 0; objectEventIndex < objectEventLimit;
       objectEventIndex++) {
    object = ObjectTable_Get(objectEventIndex);
    if (object == NULL)
      continue;
    *indices++ = objectEventIndex;
    Dma_Set(object, snapshot,
            (DMA_ENABLE | DMA_32BIT) << 16 | (sizeof(struct ObjectRuntime) / 4),
            REG_DMA3);
    if (object->animationKind == OBJECT_SPRITE_ONE) {
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
  for (objectEventIndex = savedCount;
       objectEventIndex < OBJECT_SNAPSHOT_CAPACITY; objectEventIndex++)
    *indices++ = OBJECT_SNAPSHOT_EMPTY;
}

/// Puts back the object events ObjectTable_Snapshot saved, and moves the camera
/// to the party leader's height.
///
/// Called by Field_RunMap when the party keeps its position
/// (FLAG_KEEP_PARTY_POSITION), and by the Venus Lighthouse aerie map through
/// ObjectTable_RestoreFar. Only object events whose slots are filled again are
/// restored; the leader is set idle.
void ObjectTable_Restore(void) {
  struct ObjectRuntime *snapshot = gObjectSnapshots.objectEvents;
  u8 *indices = gObjectSnapshots.slots;
  u8 *animations = gObjectSnapshots.animations;
  u8 *spriteFlags = gObjectSnapshots.spriteFlags;
  u8 *priorities = gObjectSnapshots.priorities;
  s32 restoredCount;
  struct ObjectRuntime *object;
  struct AnimationObject *sprite;
  struct ObjectRuntime *camera;
  s32 *cameraPosition;
  s32 objectEventIndex;
  s32 animation;
  s32 priority;
  s32 y;
  s32 *leaderId = &gGameState.selectedObjectEvent;
  struct EventWork **fieldWork = &gEventWork;
  struct MapGridScrollWork **grid = (struct MapGridScrollWork **)gMapGridWork;

  restoredCount = 0;
  objectEventIndex = *indices++;
  while (objectEventIndex != OBJECT_SNAPSHOT_EMPTY) {
    object = ObjectTable_Get(objectEventIndex);
    if (object != NULL) {
      // The copy also brings back the sprite pointer saved with the object
      // event. The animation and sprite flags below are set through that
      // pointer; the priorities go to the sprite the object event has now,
      // which is then put back.
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
      if (objectEventIndex == *leaderId) {
        camera = ((struct FieldWork *)*fieldWork)->camera;
        cameraPosition = (*grid)->origin;
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
    objectEventIndex = *indices++;
  }
}

#undef MAPGRID_CELLS
#undef FLAG_IN_VIEW
#undef FLAG_OUT_OF_VIEW
#undef PLACED_MAX
