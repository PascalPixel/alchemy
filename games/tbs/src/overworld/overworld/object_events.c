/// @file
/// The party's sprites, placing a map's object events, and the object event
/// table.
// Part of overworld.c.

#include "../overworld_internal.h"

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
/// Called by FieldScript_SpawnObjectEventTemplates for every object event
/// template, and by PsynergyMenu_InitializeEntryObjects (through
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
/// Called by FileMenu_ShowPartySprites through
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
/// Called by ObjectEvent_ResetForObject (the leader, then the map's table),
/// ObjectEvent_Respawn and FieldScript_CallWithLastActiveObjectId. Records
/// the table in work->tables and each object event in work->objectEvents.
void FieldScript_SpawnObjectEventTemplates(struct ObjectEventTemplate *entry,
                                           s32 slot) {
  struct FieldWork *work;
  struct Object *object;
  struct Object *previous;
  struct Sprite *sprite;
  s32 tableIndex;
  s32 objectEventIndex;
  s32 spriteId;
  s32 condition;
  u8 spriteSlot;
  u16 templateSprite;

  work = gFieldWork;
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
    // Djinn_ResolvePendingEvent hides a Djinni after the party loses or flees.
    // Map_ResetFlagsOnEnter clears these temporary flags on a normal entry.
    if ((u32)(condition - FLAG_DJINN_FIRST) <=
            OBJ_EVENT_DJINNI_FLAG_COUNT - 1 &&
        work->mode != OBJECT_SYSTEM_WORLD_MAP &&
        !GameFlag_IsConditionActive(condition + OBJ_EVENT_DJINNI_FLAG_COUNT))
      continue;
    spriteId = Party_RemapCharacterIdByFlags(entry->sprite);
    object = ObjectEvent_GetByLocalId(objectEventIndex);
    if (object == 0) {
      object = Object_CreateFar(spriteId, entry->x, entry->y, entry->z);
      // OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE: share the previous object
      // event's sprite resource. The new sprite draws from the previous one's
      // VRAM cache entry, its own entry is freed, and both are marked to keep
      // the shared entry when either is released.
      if (entry->flags & OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE) {
        previous = ObjectEvent_GetByLocalId(objectEventIndex - 1);
        if (previous->spriteStorageKind == OBJECT_SPRITE_ONE &&
            object->spriteStorageKind == OBJECT_SPRITE_ONE) {
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
        Object_RegisterSpriteMetadataFar((struct Object *)object, SPRITE_TUNIC);
    } else if (!GameFlag_GetFar(FLAG_KEEP_PARTY_POSITION)) {
      Object_SetPositionAndResetMotionFar(object, entry->x, entry->y, entry->z);
    }

    if (object) {
      Object_SelectAnimationFar(object, 1);
      // lastNo is the animation last selected, which selecting again skips: a
      // random 0 to 29 makes the second selection below restart animation 1
      // from its first frame, unless it happens to be 1. XXX: likely meant to
      // start each idle at a random frame, because the value is random; as it
      // stands it only forces the restart.
      if (object->spriteStorageKind == OBJECT_SPRITE_ONE &&
          (sprite = object->sprite) != 0)
        sprite->lastNo = ((u32)(Random16()) % (u32)30);
      object->facing = entry->facing;
      // Solid.
      object->collisionFlags = 1;
      ObjectEvent_SetMovementType(object, entry->movement);
      Object_SelectAnimationFar(object, 1);
      ((struct Object *)object)->homeX = object->x / 0x10000;
      ((struct Object *)object)->homeZ = object->z / 0x10000;
      // An object event template with a height is lifted half a pixel more and
      // takes motion flags 4, floating: it neither follows the ground nor
      // falls. Outside the world map every height is then counted from the
      // ground below.
      if (object->y != 0) {
        object->motionFlags = 4;
        object->y += 0x8000;
      }
      if (work->mode == OBJECT_SYSTEM_WORLD_MAP) {
        // World-map object event templates use single sprites and do not follow
        // the ground. While the tunic flag is set, all keep their native
        // scale; otherwise they draw at 3/4 size.
        object->motionFlags &= 0xfe;
        if (!GameFlag_GetFar(FLAG_ISAAC_WEARS_TUNIC)) {
          // UB: sprite creation can fail while the object event slot stays
          // live. The native local is then indeterminate or from an earlier
          // row. Checking the sprite grows tbs-en from 608 to 628 bytes.
#ifdef UBFIX
          if (object->spriteStorageKind == OBJECT_SPRITE_ONE &&
              (sprite = object->sprite) != NULL)
#endif
            sprite->scale = Iwram_MulQ16(sprite->scale, 3 * Q16_ONE / 4);
        }
      } else {
        object->groundHeight =
            MapGrid_GetTerrainHeightFar(0, object->x, object->z);
        object->y += object->groundHeight;
      }
      object->priorityFlags = OBJ_EVENT_PRIORITY_AUTOMATIC;
    }
    work->objectEvents[objectEventIndex] = (struct ObjectEvent *)object;
  }
}

/// Destroys the object event in slot objectEventIndex, if there is one, and
/// empties the slot. No caller names it; its far-call veneer has no exported
/// name.
void ObjectEvent_DestroyAtIndex(s32 objectEventIndex) {
  struct FieldWork *work = gFieldWork;
  void *object = ObjectEvent_GetByLocalId(objectEventIndex);

  if (object != NULL) {
    Object_ReleaseFar(object);
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
/// save block puts it, then the map's object event template table from object
/// event slot 8, and the camera that follows the leader. A leader entering
/// above ground where its cell and the cell north of it are climbing spots
/// (MB_CLIMB) is raised by the ground height of the cell north of it less 32
/// pixels, and starts climbing (LEADER_MOVEMENT_CLIMB).
///
/// Called by Map_Run on entering a map, and by
/// FieldScript_RunObjectHookAndWait.
/// Empties every object event slot and table first, without destroying the
/// object events in them; sets work->camera, the map's origin and
/// gSaveBlock.movementMode.
void ObjectEvent_ResetForObject(struct ObjectEventTemplate *table) {
  struct FieldWork *work;
  struct ObjectEventTemplate *entry;
  struct Object *object;
  struct Object *camera;
  struct MapGridCell *cell;
  struct MapGridCell *above;
  s32 leader;
  s32 pos;
  s32 height;

  work = gFieldWork;
  leader = gSaveBlock.leaderLocalId;
  // Build the leader's temporary object event template before the map's placed
  // object events. The two-row table, the leader's row and an end row, lives in
  // the event work at 0x200 (FieldWork's leaderTemplates) and lasts with it.
  entry = (struct ObjectEventTemplate *)((u8 *)work + 0x200);
  entry[0] = gField_DefaultObjectEventTemplates[0];
  entry[1] = gField_DefaultObjectEventTemplates[1];
  for (pos = 0; pos < 4; pos++)
    work->tables[pos] = 0;
  ObjectEvent_ClearAll();
  entry->condition = -1;
  entry->sprite = leader;
  SET_POSITION(entry, gSaveBlock.x, 0, gSaveBlock.z);
  entry->facing = gSaveBlock.heading;
  FieldScript_SpawnObjectEventTemplates(entry, leader);
  FieldScript_SpawnObjectEventTemplates(table, LOCALID_FIRST_PLACED);

  object = (struct Object *)work->objectEvents[leader];
  // gSaveBlock.turn keeps the leader's map grid layer (Map_Run saves its
  // layer there). Its cell: 16 pixels square, 128 to a row, so above is
  // the cell north of it.
  object->layer = gSaveBlock.turn;
  pos = (object->x / 0x100000) + (object->z / 0x100000) * 128;
  cell = &MAPGRID_CELLS[pos];
  above = &MAPGRID_CELLS[pos - 128];
  if (gSaveBlock.y != 0 && cell->behavior == MB_CLIMB &&
      above->behavior == MB_CLIMB) {
    gSaveBlock.movementMode = LEADER_MOVEMENT_CLIMB;
    height = MapGrid_GetTerrainHeightFar(0, object->x, object->z - 0x100000) -
             0x200000;
    object->y += height;
    object->groundHeight = object->y;
    object->motionFlags = 0;
    Object_SetSpriteFlagsFar((struct Object *)object, 0);
    Object_SelectAnimationFar(object, 12);
  } else {
    gSaveBlock.movementMode = LEADER_MOVEMENT_WALK;
  }

  // The camera follows this leader and starts at its resolved ground height;
  // the map scrolls by the camera's position. It takes no sprite.
  camera = Object_CreateFar(OBJECT_CREATE_NO_SPRITE << 12, object->x, object->y,
                            object->z);
  camera->groundHeight = object->groundHeight;
  Object_StartCameraFar((struct Object *)camera, (s32)object);
  if (work->mode == OBJECT_SYSTEM_WORLD_MAP) {
    struct SpriteAnim *meta =
        (struct SpriteAnim *)Sprite_AddAnimFar(object->sprite, 23);
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
/// Called by ObjectEvent_Respawn and FieldScript_CallWithLastActiveObjectId
/// to choose where FieldScript_SpawnObjectEventTemplates starts.
s32 ObjectEvent_FindLastActiveId(void) {
  struct FieldWork *work = gFieldWork;
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
UNUSED void *ObjectEvent_GetSlotAddress(u32 objectEventIndex) {
  return (u8 *)gFieldWork + 0x14 + objectEventIndex * sizeof(void *);
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
/// Called by map overlays through Map_AssignViewFlagsFar. Writes the
/// object event templates' condition, and clears FLAG_IN_VIEW and sets
/// FLAG_OUT_OF_VIEW.
void Map_AssignViewFlags(struct ObjectEventTemplate *entry) {
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

/// Destroys the placed object events far from the view's centre: more than 160
/// pixels to either side, 200 pixels north or 100 pixels south. Object events
/// at (0, 0) stay.
///
/// Called on the world map before an effect plays, by Field_UsePsynergy and
/// Djinn_JoinParty; empties the slots it frees. ObjectEvent_Respawn
/// brings the object events back afterwards.
void ObjectEvent_ClearOutOfBounds(void) {
  struct FieldWork *work = gFieldWork;
  struct Object *camera;
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
          Object_ReleaseFar((struct Object *)objectEvent);
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
void ObjectEvent_Respawn(void) {
  struct FieldWork *work = gFieldWork;
  struct ObjectEventTemplate *templates;
  s32 slotIndex;

  for (slotIndex = 0; slotIndex < PLACED_MAX; slotIndex++) {
    struct ObjectEvent *objectEvent =
        work->objectEvents[LOCALID_FIRST_PLACED + slotIndex];

    if (objectEvent != 0) {
      // As in ObjectEvent_ClearOutOfBounds: free the sprite and its
      // VRAM cache entry with the object event.
      objectEvent->spriteStorageKind = OBJECT_SPRITE_ONE;
      objectEvent->sprite->retainCacheEntry = 0;
      Object_ReleaseFar((struct Object *)objectEvent);
      work->objectEvents[LOCALID_FIRST_PLACED + slotIndex] = 0;
    }
  }
  templates = work->tables[1];
  work->tables[1] = 0;
  work->tables[2] = 0;
  work->tables[3] = 0;
  if (templates != 0)
    FieldScript_SpawnObjectEventTemplates(templates,
                                          ObjectEvent_FindLastActiveId());
}

/// Empties all the object event slots, without destroying the object events.
///
/// Called by ObjectEvent_ResetForObject.
void ObjectEvent_ClearAll(void) {
  struct FieldWork *work;
  struct ObjectEvent *emptyObjectEvent;
  s32 objectEventIndex;

  emptyObjectEvent = NULL;
  objectEventIndex = OBJ_EVENT_SLOT_COUNT - 1;
  work = gFieldWork;
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
/// through ObjectEvent_GetByLocalIdFar, by map
/// overlays.
void *ObjectEvent_GetByLocalId(u32 objectEventIndex) {
  u8 *workBytes = (u8 *)gFieldWork;
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
/// gObjectEventSnapshot, with each one's slot, animation, sprite flags and
/// priority, so that ObjectEvent_Restore can put them back.
///
/// Called by Map_Run before a random battle, by saving the game through
/// Save_CaptureObjectEventsFar, and by two maps (Tolbi's spring and the
/// Venus Lighthouse aerie).
void ObjectEvent_Snapshot(void) {
  struct FieldWork *work;
  struct Object *snapshot = gObjectEventSnapshot.objectEvents;
  u8 *animations = gObjectEventSnapshot.animations;
  u8 *spriteFlags = gObjectEventSnapshot.spriteFlags;
  u8 *priorities = gObjectEventSnapshot.priorities;
  u8 *indices = gObjectEventSnapshot.slots;
  u32 savedCount = 0;
  s32 objectEventLimit =
      sizeof(work->objectEvents) / sizeof(work->objectEvents[0]);
  struct Object *object;
  s32 objectEventIndex;
  u32 animation;
  u32 spriteFlagBits;
  u32 priority;

  work = gFieldWork;
  if (work->mode == OBJECT_SYSTEM_WORLD_MAP)
    objectEventLimit = LOCALID_FIRST_PLACED;
  // Keep each object event and its three sprite-state bytes at matching save
  // indices.
  for (objectEventIndex = 0; objectEventIndex < objectEventLimit;
       objectEventIndex++) {
    object = ObjectEvent_GetByLocalId(objectEventIndex);
    if (object == NULL)
      continue;
    *indices++ = objectEventIndex;
    Dma_Set(object, snapshot,
            (DMA_ENABLE | DMA_32BIT) << 16 | (sizeof(struct Object) / 4),
            REG_DMA3);
    if (object->spriteStorageKind == OBJECT_SPRITE_ONE) {
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
    if (savedCount > OBJECT_EVENT_SNAPSHOT_CAPACITY - 1)
      break;
  }
  for (objectEventIndex = savedCount;
       objectEventIndex < OBJECT_EVENT_SNAPSHOT_CAPACITY; objectEventIndex++)
    *indices++ = OBJECT_EVENT_SNAPSHOT_EMPTY;
}

/// Puts back the object events ObjectEvent_Snapshot saved, and moves the camera
/// to the party leader's height.
///
/// Called by Map_Run when the party keeps its position
/// (FLAG_KEEP_PARTY_POSITION), and by the Venus Lighthouse aerie map through
/// ObjectEvent_RestoreFar. Only object events whose slots are filled again are
/// restored; the leader is set idle.
void ObjectEvent_Restore(void) {
  struct Object *snapshot = gObjectEventSnapshot.objectEvents;
  u8 *indices = gObjectEventSnapshot.slots;
  u8 *animations = gObjectEventSnapshot.animations;
  u8 *spriteFlags = gObjectEventSnapshot.spriteFlags;
  u8 *priorities = gObjectEventSnapshot.priorities;
  s32 restoredCount;
  struct Object *object;
  struct Sprite *sprite;
  struct Object *camera;
  s32 *cameraPosition;
  s32 objectEventIndex;
  s32 animation;
  s32 priority;
  s32 y;
  s32 *leaderId = &gSaveBlock.leaderLocalId;
  struct FieldWork **fieldWork = &gFieldWork;
  struct MapGridScrollWork **grid = (struct MapGridScrollWork **)gMapGridWork;

  restoredCount = 0;
  objectEventIndex = *indices++;
  while (objectEventIndex != OBJECT_EVENT_SNAPSHOT_EMPTY) {
    object = ObjectEvent_GetByLocalId(objectEventIndex);
    if (object != NULL) {
      // The copy also brings back the sprite pointer saved with the object
      // event. The animation and sprite flags below are set through that
      // pointer; the priorities go to the sprite the object event has now,
      // which is then put back.
      sprite = object->sprite;
      Dma_Set(snapshot, object,
              (DMA_ENABLE | DMA_32BIT) << 16 | (sizeof(struct Object) / 4),
              REG_DMA3);
      animation = *animations;
      if (animation != 0)
        Object_SelectAnimationFar(object, animation);
      Object_SetSpriteFlagsFar((struct Object *)object, *spriteFlags);
      priority = *priorities;
      sprite->image.priority = priority;
      sprite->shadow.priority = priority;
      object->sprite = sprite;
      // Re-anchor the camera only when restoring the selected leader.
      if (objectEventIndex == *leaderId) {
        camera = (*fieldWork)->camera;
        cameraPosition = (*grid)->origin;
        y = object->y;
        camera->groundHeight = y;
        camera->y = y;
        cameraPosition[1] = y;
        Object_StartIdleFar((struct Object *)object);
      }
    }
    snapshot++;
    animations++;
    spriteFlags++;
    priorities++;
    restoredCount++;
    if (restoredCount > OBJECT_EVENT_SNAPSHOT_CAPACITY - 1)
      break;
    objectEventIndex = *indices++;
  }
}

#undef MAPGRID_CELLS
#undef FLAG_IN_VIEW
#undef FLAG_OUT_OF_VIEW
#undef PLACED_MAX
