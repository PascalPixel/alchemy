// Draft of Event_SpawnObjectTable: differ 321 of 347 (tla-en, 2026-10-18).
// The Broken Seal's form (the draft was ported from it): it records the
// placement table in the event work, then creates or moves each row's object
// whose condition holds, links paired sprites and sets each actor's home,
// facing and terrain height.
//
// Left: other. The Lost Age's function adds work the draft does not have
// yet: for conditions 48 to 127 it looks up a group byte in gData_02001000
// (condition - 48 divided and reduced by 20) before the second condition
// test, remaps the character through Func_080ca6a4, stops at slot 63, and
// has a loop over the sprite's parts before it sets the sprite's byte 25.

#include "resource.h"
#include "types.h"
#include "field_scene.h"
#include "map.h"
#include "iwram_call.h"
#include "ram_buffer.h"
#include "scene.h"
#include "object_runtime.h"
#include "script_object_runtime.h"
#include "animspr.h"
#include "object_dispatch.h"
#define MAP_CELLS ((struct MapCell *)Ram_MapCellBuffer)

// The field's event work (heap slot 0x6c): the placement tables spawned so
// far, the actors by index and the field mode.
struct FieldStepWork {
  struct ScenePlacement *tables[4];
  u8 unk10[4];
  struct ObjectRuntime *actors[66];
  u8 unk11c[0x6e];
  s8 mode;
};

// The Lost Age's sprite object as this function reads it.
struct SpawnSprite {
  u8 unk00[12];
  s32 scale;
  u8 slot;
  u8 displayFlags;
  u8 unk12[7];
  u8 lastNo;
};

void Event_SpawnObjectTable(struct ScenePlacement *entry, s32 slot) {
  struct FieldStepWork *work;
  struct ObjectRuntime *object;
  struct ObjectRuntime *previous;
  struct SpawnSprite *sprite;
  s32 i;
  s32 index;
  s32 character;
  s32 condition;
  u8 resource;
  u16 id;

  work = (struct FieldStepWork *)Ram_HeapSlots->eventWork;
  for (i = 0; i < 4; i++) {
    if (work->tables[i] == entry)
      break;
    if (work->tables[i] == 0) {
      work->tables[i] = entry;
      break;
    }
  }
  for (id = entry->sprite; (s16)id != -1 && slot <= 65;
       entry++, id = entry->sprite) {
    /* FAKEMATCH: the ROM reads the row id once per step and extends it
       again in the body; without this second read the compiler shares
       the shifted id between the test and the body. */
    id = entry->sprite;
    if ((s16)id <= 7)
      index = (s16)id;
    else if ((s16)id <= 0x2705)
      index = slot++;
    condition = entry->condition;
    if (!GameFlag_IsConditionActive(condition))
      continue;
    if ((u32)(condition - 48) <= 79 && work->mode != 3 &&
        !GameFlag_IsConditionActive(condition + 80))
      continue;
    character = Party_RemapCharacterIdByFlags(entry->sprite);
    object = ObjectTable_Get(index);
    if (object == 0) {
      object = Object_CreateFar(character, entry->x, entry->y, entry->z);
      if (entry->flags & 1) {
        previous = ObjectTable_Get(index - 1);
        if (previous->animationKind == 1 && object->animationKind == 1) {
          sprite = (struct SpawnSprite *)previous->animation;
          sprite->displayFlags |= 1;
          resource = sprite->slot;
          sprite = (struct SpawnSprite *)object->animation;
          sprite->displayFlags |= 1;
          Resource_ResetEntry(sprite->slot);
          sprite->slot = resource;
        }
      }
      if (GameFlag_TestFar(33) && (u32)(character - 18) <= 1)
        ObjectDispatch_RegisterChildMetadataFar((struct DispatchObject *)object,
                                                226);
    } else if (!GameFlag_TestFar(0x109)) {
      Object_SetPositionAndResetMotionFar(object, entry->x, entry->y, entry->z);
    }
    if (object) {
      Object_SetMode(object, 1);
      if (object->animationKind == 1 &&
          (sprite = (struct SpawnSprite *)object->animation) != 0)
        sprite->lastNo = __umodsi3(Random16(), 30);
      object->angle = entry->facing;
      object->collisionFlags = 1;
      ObjectMotion_SetActionCallback(object, entry->behavior);
      Object_SetMode(object, 1);
      ((struct ScriptObjectRuntime *)object)->homeX = object->x / 0x10000;
      ((struct ScriptObjectRuntime *)object)->homeZ = object->z / 0x10000;
      if (object->y != 0) {
        object->motionFlags = 4;
        object->y += 0x8000;
      }
      if (work->mode == 3) {
        object->motionFlags &= 0xfe;
        if (!GameFlag_TestFar(33))
          sprite->scale = Iwram_MulQ16(sprite->scale, 0xc000);
      } else {
        object->terrainHeight =
            Map_GetTerrainHeightFar(0, object->x, object->z);
        object->y += object->terrainHeight;
      }
      object->priorityFlags = 1;
    }
    work->actors[index] = object;
  }
}
