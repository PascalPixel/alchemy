// Draft of Event_SpawnObjectTable, differ 43 of 347 (321 before), rewritten
// from the listing (2026-10-26):
// it records the placement table in the event work, then creates or moves
// each row's object whose condition holds, links paired sprites and sets each
// actor's home, facing and terrain height.
// Left: the row loop. The native tests the row id at the loop foot only,
// entering by a jump (its two copies of the test cross-jumped), keeping the
// id zero-extended in r2 and sign-extending it in the test; here the test is
// copied before the loop and the shifted id is shared with the body.
#include "types.h"
#include "ram_buffer.h"
#include "field_scene.h"
#include "object_runtime.h"
#include "party_state.h"
#include "iwram_call.h"

// The field's event work (heap slot 0x6c): the placement tables spawned so
// far, the actors by index and the field mode.
struct FieldStepWork {
  struct ScenePlacement *tables[4];
  u8 unk10[4];
  struct ObjectRuntime *actors[66];
  u8 unk11c[0x6e];
  s8 mode;
};

struct SpawnSpritePart {
  u8 unk00[2];
  u16 frame;
};

// The Lost Age's sprite object as this function reads it.
struct SpawnSprite {
  u8 unk00[12];
  s32 scale;
  u8 slot;
  u8 displayFlags;
  u8 unk12[7];
  u8 unk19;
  u8 unk1a;
  u8 partCount;
  u8 unk1c[12];
  struct SpawnSpritePart *parts[1];
};

// The object fields this function sets as an actor's home cell.
struct SpawnActor {
  u8 unk00[100];
  s16 homeX;
  s16 homeZ;
};

extern s8 gData_02001000[];

s32 GameFlag_IsConditionActive(s32 condition);
s32 GameFlag_Test(s32 flag);
s32 Party_RemapCharacterIdByFlags(s32 sprite);
struct ObjectRuntime *Func_080200c0(s32 kind, s32 x, s32 y, s32 z);
void Func_080ca9cc(s32 value, s32 *out);
s32 Resource_ResetEntry(u32 slot);
void Object_SetPositionAndResetMotionFar(struct ObjectRuntime *object, s32 x,
                                         s32 y, s32 z);
s32 Map_GetTerrainHeightFar(u8 layer, s32 x, s32 z);
u32 Random16(void);

void Event_SpawnObjectTable(struct ScenePlacement *entry, s32 slot) {
  struct FieldStepWork *work = Ram_HeapSlots->eventWork;
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
    struct ObjectRuntime *object;
    struct SpawnSprite *sprite;
    s32 condition;
    s32 character;

    if ((s16)id <= 7)
      index = (s16)id;
    else if ((s16)id <= 0x2705)
      index = slot++;
    condition = entry->condition;
    if (!GameFlag_IsConditionActive(condition))
      continue;
    if ((u32)(condition - 48) <= 79) {
      s32 group = (condition - 48) / 20;

      if ((condition - 48) % 20 == 19) {
        s32 member = gData_02001000[group + 0x1950];

        if (member == 0)
          continue;
        condition = group * 20 + member + 47;
        if (!GameFlag_IsConditionActive(condition))
          continue;
      }
      if (work->mode != 3 && !GameFlag_IsConditionActive(condition + 80))
        continue;
    }
    character = Party_RemapCharacterIdByFlags(entry->sprite);
    object = ObjectTable_Get(index);
    if (object == NULL) {
      if (entry->flags & 2) {
        s32 position[3];

        position[0] = (s32)object;
        position[1] = (s32)object;
        position[2] = (s32)object;
        Func_080ca9cc(entry->x, position);
        object = Func_080200c0(character, position[0], 0, position[2]);
      } else {
        object = Func_080200c0(character, entry->x, entry->y, entry->z);
      }
      if (entry->flags & 1) {
        struct ObjectRuntime *previous = ObjectTable_Get(index - 1);

        if (previous->animationKind == 1 && object->animationKind == 1) {
          u8 resource;

          sprite = previous->animation;
          sprite->displayFlags |= 1;
          resource = sprite->slot;
          sprite = object->animation;
          sprite->displayFlags |= 1;
          Resource_ResetEntry(sprite->slot);
          sprite->slot = resource;
        }
      }
    } else {
      if (entry->sprite == gPartyState.currentOwner)
        continue;
      Object_SetPositionAndResetMotionFar(object, entry->x, entry->y, entry->z);
    }
    if (object != NULL) {
      sprite = object->animation;
      Object_SetMode(object, 1);
      if (object->animationKind == 1 && sprite != NULL) {
        for (i = 0; i < sprite->partCount; i++) {
          struct SpawnSpritePart *part = sprite->parts[i];

          if (part != NULL)
            part->frame = (Random16() * 5) >> 15;
        }
        sprite->unk19 = 1;
      }
      object->angle = entry->facing;
      object->collisionFlags = 1;
      Object_SetActionCallback(object, entry->behavior);
      ((struct SpawnActor *)object)->homeX = object->x / 0x10000;
      ((struct SpawnActor *)object)->homeZ = object->z / 0x10000;
      if (object->y != 0) {
        object->motionFlags = 4;
        object->y += 0x8000;
      }
      if (work->mode == 3) {
        object->motionFlags &= 0xfe;
        if (!GameFlag_Test(33))
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
