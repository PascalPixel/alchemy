/// @file
/// The party's sprites, placing a scene's actors, and the actor
/// table.
// Part of field_sequence.c.

#include "../field_sequence_internal.h"
#include "flags.h"
#include "fixed_math.h"
#include "actor_view.h"
#include "module/field/field_sequence.h"
#include "module/field/object.h"
#include "gamepak/far_call/field.h"
#include "system/ram_buffer.h"
#include "module/tla.h"
#include "macros.h"

/// Picks the sprite a scene placement's character id shows: character 8
/// becomes 9 on the world map and 11 elsewhere, one more once flag 0x8df is
/// set.
s32 Party_GetCharacterSprite(s32 character) {
  s32 sprite;

  sprite = character;
  if (sprite <= 8) {
    if (sprite == 8) {
      sprite = 9;
      if (gMainRoutineWork.mapId != SCENE_CLEAR)
        sprite = 11;
      if (GameFlag_Get(FLAG_SHIP_FLIES))
        sprite++;
    }
  }
  return sprite;
}

/// Does nothing; nothing calls Actor_ReservedNoOpFar.
void Actor_ReservedNoOp(void) {}

void Event_SpawnObjectTable(struct ActorPlacement *entry, s32 slot) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  s32 index = 0;
  s32 i;
  u16 id;

  for (i = 0; i < 4; i++) {
    if (work->tables[i] == entry)
      break;
    if (work->tables[i] == NULL) {
      work->tables[i] = entry;
      break;
    }
  }
  for (id = entry->sprite; (s16)id != -1 && slot <= 63;
       entry++, id = entry->sprite) {
    struct Actor *object;
    struct SpawnSprite *sprite;
    s32 condition;
    s32 character;

    if (entry->sprite <= 7)
      index = entry->sprite;
    else if (entry->sprite <= 0x2705)
      index = slot++;
    condition = entry->condition;
    if (!GameFlag_IsConditionActive(condition))
      continue;
    if ((u32)(condition - 48) <= 79) {
      s32 group = (condition - 48) / 20;

      if ((condition - 48) % 20 == 19) {
        s32 member = gSceneLocalState[group + 0x1950];

        if (member == 0)
          continue;
        condition = group * 20 + member + 47;
        if (!GameFlag_IsConditionActive(condition))
          continue;
      }
      if (work->mode != 3 && !GameFlag_IsConditionActive(condition + 80))
        continue;
    }
    character = Party_GetCharacterSprite(entry->sprite);
    object = ObjectTable_Get(index);
    if (object == NULL) {
      if (entry->flags & 2) {
        s32 position[3];

        position[0] = (s32)object;
        position[1] = (s32)object;
        position[2] = (s32)object;
        ScenePoint_GetPosition(entry->x, position);
        object = Actor_CreateFar(character, position[0], 0, position[2]);
      } else {
        object = Actor_CreateFar(character, entry->x, entry->y, entry->z);
      }
      if (entry->flags & 1) {
        struct Actor *previous = ObjectTable_Get(index - 1);

        if (previous->spriteStorageKind == 1 &&
            object->spriteStorageKind == 1) {
          u8 resource;

          sprite = previous->sprite;
          sprite->displayFlags |= 1;
          resource = sprite->slot;
          sprite = object->sprite;
          sprite->displayFlags |= 1;
          VramCache_FreeEntry(sprite->slot);
          sprite->slot = resource;
        }
      }
    } else {
      if (entry->sprite == gMainRoutineWork.leaderActorId)
        continue;
      Actor_SetPositionAndResetMotionFar(object, entry->x, entry->y, entry->z);
    }
    if (object != NULL) {
      sprite = object->sprite;
      Actor_SelectAnimationFar(object, 1);
      if (object->spriteStorageKind == 1 && sprite != NULL) {
        for (i = 0; i < sprite->partCount; i++) {
          struct SpawnSpritePart *part = sprite->parts[i];

          if (part != NULL)
            part->frame = (Random16() * 5) >> 15;
        }
        sprite->unk19 = 1;
      }
      object->facing = entry->facing;
      object->collisionFlags = 1;
      Actor_SetActionCallback(object, entry->behavior);
      ((struct SpawnActor *)object)->homeX = object->x.fixed / Q16_ONE;
      ((struct SpawnActor *)object)->homeZ = object->z.fixed / Q16_ONE;
      if (object->y.fixed != 0) {
        object->motionFlags = 4;
        object->y.fixed += px(1) / 2;
      }
      if (work->mode == 3) {
        object->motionFlags &= 0xfe;
        if (!GameFlag_Get(33))
          sprite->scale = Iwram_MulQ16(sprite->scale, Q16_ONE * 3 / 4);
      } else {
        object->groundHeight =
            MapGrid_GetTerrainHeightFar(0, object->x.fixed, object->z.fixed);
        object->y.fixed += object->groundHeight;
      }
      object->priorityFlags = 1;
    }
    work->actors[index] = object;
  }
}

/// Looks point id up in the map's scene points (its scene_points.tsv, ended by
/// id 0xff) and writes its x and z, in 16.16 pixels, to position; returns 0,
/// or -1 when the map has no such point.
s32 ScenePoint_GetPosition(s32 id, s32 *position) {
  u8 *cursor = Ram_Data_0202e008;
  s8 key;

  key = *cursor++;
  while (key != -1) {
    if (key == id) {
      position[0] = Stream_ReadU16(&cursor) << 16;
      position[2] = Stream_ReadU16(&cursor) << 16;
      return 0;
    }
    cursor += 4;
    key = *cursor++;
  }
  return -1;
}

/// Releases the actor in slot actorIndex of the event work's table, if there
/// is one, and empties the slot.
void Actor_ReleaseAtIndex(u32 actorIndex) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  struct Actor *actor = ObjectTable_Get(actorIndex);

  if (actor != NULL) {
    Actor_ReleaseFar(actor);
    work->actors[actorIndex] = NULL;
  }
}

u8 *ResourceMetadata_RegisterFar(void *resource, s32 id);

/// Creates a scene's actors on entering it, as The Broken Seal's
/// Actor_ResetForLeader does: the party leader where the main routine's work
/// puts it, the scene's placement table from actor slot 8, and the camera
/// that follows the leader; it also chooses the leader's control mode.
void Actor_ResetForLeader(s32 table) {
  struct FieldWork *work;
  struct EventWorkPlacements *placements;
  struct ActorPlacement *entry;
  struct Actor *object;
  struct Actor *camera;
  struct MapCell *cell;
  struct MapCell *above;
  s32 leader;
  s32 i;
  s32 height;

  work = Ram_HeapSlots->fieldWork;
  placements = (struct EventWorkPlacements *)work;
  leader = gMainRoutineWork.leaderActorId;
  entry = placements->leader;
  entry[0] = gField_DefaultPlacements[0];
  entry[1] = gField_DefaultPlacements[1];
  for (i = 0; i < 4; i++)
    placements->tables[i] = 0;
  ObjectTable_Clear();
  entry->condition = -1;
  entry->sprite = leader;
  SET_POSITION(entry, gMainRoutineWork.x, 0, gMainRoutineWork.z);
  entry->facing = gMainRoutineWork.heading;
  Event_SpawnObjectTable(entry, leader);
  {
    s32 y = gMainRoutineWork.y;

    if (y != 0) {
      object = work->actors[leader];
      object->y.fixed = y;
      object->groundHeight = y;
    }
  }
  Event_SpawnObjectTable((struct ActorPlacement *)table, 8);

  object = work->actors[leader];
  object->layer = gMainRoutineWork.layer;
  object->timer = 0;
  object->userValue = 0;
  if (work->mode == 3) {
    if (gMainRoutineWork.ownerOverride != 0)
      gMainRoutineWork.movementMode = 7;
    else
      gMainRoutineWork.movementMode = 1;
  } else if (gMainRoutineWork.ownerOverride != 0) {
    gMainRoutineWork.movementMode = 6;
  } else {
    gMainRoutineWork.movementMode = 0;
    /* FAKEMATCH(barrier): an empty asm after the store. */
    asm("");
    cell = Ram_HeapSlots->mapGrid->layers[object->layer].cells +
           ((object->x.fixed / 0x100000) + ((object->z.fixed / 0x100000) << 7));
    above = cell - 128;
    if (gMainRoutineWork.y != 0 && (cell->attributeB & 0x40) &&
        (above->attributeB & 0x40)) {
      gMainRoutineWork.movementMode = 2;
      height = MapGrid_GetTerrainHeightFar(0, object->x.fixed,
                                           object->z.fixed - 0x100000) -
               0x200000;
      object->y.fixed += height;
      object->groundHeight = object->y.fixed;
      object->motionFlags = 0;
      Actor_SetSpriteFlagsFar(object, 0);
      Actor_SelectAnimationFar(object, 12);
    }
  }

  if (gMainRoutineWork.ownerOverride != 0) {
    struct Actor *stand = work->actors[8];

    SET_ACTOR_POSITION(stand, object->x.fixed, object->y.fixed,
                       object->z.fixed);
    stand->groundHeight = object->groundHeight;
    stand->facing = object->facing;
    object->x.fixed = 0;
    object->z.fixed = 0;
    object = stand;
  }

  camera = Actor_CreateFar(0x8000, object->x.fixed, object->y.fixed,
                           object->z.fixed);
  camera->groundHeight = object->groundHeight;
  Actor_StartCameraFar(camera, object);
  if (work->mode == 3) {
    u8 *effect = ResourceMetadata_RegisterFar(object->sprite, 266);

    effect[5] = 15;
    effect[6] = 9;
  }
  Ram_HeapSlots->mapGrid->origin = &camera->x.fixed;
  work->camera = camera;
}

/// Returns the slot after the last placed actor (8 when there is none), or -1
/// when the last slot is occupied.
s32 Actor_FindLastActiveId(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  s32 last = 7;
  s32 i;

  for (i = 8; i <= 63; i++) {
    if (work->actors[i] != NULL)
      last = i;
  }
  last++;
  if (last == 64)
    last = -1;
  return last;
}

/// Gives each actor placement with condition 0 a condition saying whether
/// it lies within the map's view bounds, so that only those inside appear.
void Scene_AssignViewFlags(struct SceneRegionEntry *entry) {
  struct MapWork *view = Ram_HeapSlots->mapGrid;

  GameFlag_Clear(0x164);
  GameFlag_Set(0x165);
  while (entry->id != -1) {
    s32 x;
    s32 z;

    // A placement with a flag of its own keeps it.
    if (entry->flag != 0) {
      entry++;
      continue;
    }
    x = entry->x;
    z = entry->z;
    if (view->minX <= x && x <= view->maxX && view->minZ <= z &&
        z <= view->maxZ)
      entry->flag = 0x164;
    else
      entry->flag = 0x165;
    entry++;
  }
}

void ObjectTable_Clear(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  s32 i;

  for (i = 0; i < 80; i++)
    work->actors[i] = NULL;
}

struct Actor *ObjectTable_Get(u32 objectId) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;

  if (objectId > 0xbf)
    return NULL;
  return work->actors[objectId];
}

void ObjectTable_Snapshot(void) {
  struct Actor *copy = gSnapshotObjects;
  u8 *indices = (u8 *)gSnapshotObjects - 32;
  u8 *frames = (u8 *)&gSnapshotObjects[32];
  u8 *nextFrames = frames + 32;
  u8 *priorities = frames + 64;
  u32 count = 0;
  struct Actor *object;
  struct FieldSprite *sprite;
  s32 i;
  u32 frame;
  u32 next;
  u32 priority;
  s32 limit = 80;

  for (i = 0; i < limit; i++) {
    object = ObjectTable_Get(i);
    if (object == NULL)
      continue;
    *indices++ = i;
    Dma_Set(object, copy, DMA_COPY_WORDS(sizeof(struct Actor) / 4), REG_DMA3);
    if (object->spriteStorageKind == 1) {
      sprite = object->sprite;
      frame = sprite->frame;
      next = sprite->nextFrame;
      priority = sprite->priority;
    } else {
      frame = 0;
      next = 0;
      priority = 0;
    }
    *frames++ = frame;
    *nextFrames++ = next;
    *priorities++ = priority;
    count++;
    copy++;
    if (count > 31)
      break;
  }
  for (i = count; i < 32; i++)
    *indices++ = 255;
}

/// Puts back the actors the snapshot saved, and moves the camera to the party
/// leader's height.
void Actor_Restore(void) {
  struct Actor *copy = gSnapshotObjects;
  u8 *indices = (u8 *)gSnapshotObjects - 32;
  u8 *frames = (u8 *)&gSnapshotObjects[32];
  u8 *nextFrames = frames + 32;
  u8 *priorities = frames + 64;
  s32 count;
  struct Actor *object;
  struct FieldSprite *sprite;
  s32 *view;
  struct Actor *camera;
  s32 index;
  s32 priority;
  s32 y;

  count = 0;
  index = *indices++;
  while (index != 255) {
    object = ObjectTable_Get(index);
    if (object != NULL) {
      sprite = object->sprite;
      Dma_Set(copy, object, DMA_COPY_WORDS(sizeof(struct Actor) / 4), REG_DMA3);
      object->sprite = sprite;
      Actor_SelectAnimationFar(object, *frames);
      Actor_SetSpriteFlagsFar(object, *nextFrames);
      priority = *priorities;
      sprite->priority = priority;
      sprite->partPriority = priority;
      if (index == Party_GetLeaderActor()) {
        camera = gFieldWork->camera;
        view = Ram_HeapSlots->mapGrid->origin;
        y = object->y.fixed;
        camera->x.fixed = object->x.fixed;
        camera->groundHeight = y;
        camera->y.fixed = y;
        camera->z.fixed = object->z.fixed;
        view[1] = y;
        Actor_StartIdleFar(object);
      }
    }
    copy++;
    frames++;
    nextFrames++;
    count++;
    priorities++;
    if (count > 31)
      break;
    index = *indices++;
  }
}
