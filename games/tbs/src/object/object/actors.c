/// @file
/// The party's sprites, placing a scene's actors, and the actor table.
/// Part of object.c, the field object, compiled only through it.

#include "../object_internal.h"

/// Returns the sprite of character, which is the character itself except that
/// FLAG_CHILDHOOD gives characters 0 and 1 sprites 0x12 and 0x13, and flag 0x21
/// gives character 0 sprite 0x11.
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
    } else if ((GameFlag_TestFar(0x21) != 0) && (sprite == 0)) {
      sprite = 0x11;
    }
  }
  return sprite;
}

/// Returns the sprite of character as Party_RemapCharacterIdByFlags would with
/// FLAG_CHILDHOOD set to childhood (as recorded in a save file's summary).
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
/// does not hold is skipped, and so, outside the world map, is one with a
/// condition from 48 to 127 whose condition + 80 does not hold. An actor that
/// already exists is only moved back to its place, unless the party keeps its
/// position.
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
    if (entry->sprite <= 7)
      actorIndex = entry->sprite;
    else if (entry->sprite <= 0x2705)
      actorIndex = slot++;
    condition = entry->condition;
    if (!GameFlag_IsConditionActive(condition))
      continue;
    if ((u32)(condition - 48) <= 79 && work->mode != MODE_WORLD_MAP &&
        !GameFlag_IsConditionActive(condition + 80))
      continue;
    spriteId = Party_RemapCharacterIdByFlags(entry->sprite);
    object = ObjectTable_Get(actorIndex);
    if (object == 0) {
      object = Object_CreateFar(spriteId, entry->x, entry->y, entry->z);
      // Placement flag 1: share the previous actor's sprite resource.
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
      if (GameFlag_TestFar(0x21) && (u32)(spriteId - 0x12) <= 1)
        ObjectDispatch_RegisterChildMetadataFar((struct DispatchObject *)object,
                                                226);
    } else if (!GameFlag_TestFar(FLAG_KEEP_PARTY_POSITION)) {
      Object_SetPositionAndResetMotionFar(object, entry->x, entry->y, entry->z);
    }

    if (object) {
      ObjectDispatch_SelectAnimationFar(object, 1);
      // Start each idle animation at a random frame.
      if (object->animationKind == 1 && (sprite = object->animation) != 0)
        sprite->lastNo = ((u32)(Random16()) % (u32)30);
      object->angle = entry->facing;
      object->unk59 = 1;
      ObjectMotion_SetActionCallback(object, entry->behavior);
      ObjectDispatch_SelectAnimationFar(object, 1);
      ((struct ScriptObjectRuntime *)object)->homeX = object->x / 0x10000;
      ((struct ScriptObjectRuntime *)object)->homeZ = object->z / 0x10000;
      if (object->y != 0) {
        object->flags = 4;
        object->y += 0x8000;
      }
      if (work->mode == MODE_WORLD_MAP) {
        // On the world map actors are drawn at three quarters size.
        object->flags &= 0xfe;
        if (!GameFlag_TestFar(0x21))
          sprite->scale = Iwram_MulQ16(sprite->scale, 0xc000);
      } else {
        object->terrainHeight =
            Map_GetTerrainHeightFar(0, object->x, object->z);
        object->y += object->terrainHeight;
      }
      object->unk23 = 1;
    }
    work->actors[actorIndex] = (struct FieldActor *)object;
  }
}

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
/// cell and the cell north of it have collision code 0xfd is raised by the
/// ground height of the cell north of it less 32 pixels, and takes movement
/// mode 1.
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
  // The two-row table lives in the event work at 0x200.
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

  // The camera follows this leader and starts at its resolved ground height.
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

          // Release the sprite's retained cache entry when destroying the
          // actor.
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
void BattleEffect_ClearAllObjects(void) {
  struct FieldWork *work = (struct FieldWork *)gEventWork;
  struct ScenePlacement *placements;
  s32 slotIndex;

  for (slotIndex = 0; slotIndex < PLACED_MAX; slotIndex++) {
    struct FieldActor *actor = work->actors[ACTOR_FIRST_PLACED + slotIndex];

    if (actor != 0) {
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
/// gObjectSnapshots, with each one's animation, sprite flags and priority, so
/// that ObjectTable_Restore can put them back. The 32 bytes before
/// gObjectSnapshots list the saved slots, ended by 255.
void ObjectTable_Snapshot(void) {
  struct ObjectRuntime *snapshot = gObjectSnapshots;
  u8 *animations = (u8 *)&gObjectSnapshots[32];
  u8 *spriteFlags = animations + 32;
  u8 *priorities = animations + 64;
  u8 *indices = (u8 *)gObjectSnapshots - 32;
  u32 savedCount = 0;
  s32 actorLimit = 66;
  struct ObjectRuntime *object;
  s32 actorIndex;
  u32 animation;
  u32 spriteFlagBits;
  u32 priority;

  if (((struct FieldWork *)gEventWork)->mode == MODE_WORLD_MAP)
    actorLimit = 8;
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
    if (savedCount > 31)
      break;
  }
  for (actorIndex = savedCount; actorIndex < 32; actorIndex++)
    *indices++ = 255;
}

/// Puts back the actors ObjectTable_Snapshot saved, and moves the camera to the
/// party leader's height.
void ObjectTable_Restore(void) {
  struct ObjectRuntime *snapshot = gObjectSnapshots;
  u8 *indices = (u8 *)gObjectSnapshots - 32;
  u8 *animations = (u8 *)&gObjectSnapshots[32];
  u8 *spriteFlags = animations + 32;
  u8 *priorities = animations + 64;
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
  while (actorIndex != 255) {
    object = ObjectTable_Get(actorIndex);
    if (object != NULL) {
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
    if (restoredCount > 31)
      break;
    actorIndex = *indices++;
  }
}

#undef MAP_CELLS
#undef FLAG_IN_VIEW
#undef FLAG_OUT_OF_VIEW
#undef PLACED_MAX
