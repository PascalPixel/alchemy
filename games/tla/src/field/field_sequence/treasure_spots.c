/// @file
/// The map's treasure spots: chests and Psynergy Stones.
// Part of field_sequence.c.

#include "../field_sequence_internal.h"
#include "module/field/field_sequence.h"

void Actor_SnapToTerrain(void *object) {
  struct Actor *state = object;
  s32 height;

  height = MapGrid_GetTerrainHeightFar(0, state->x, state->z);
  state->y = height;
  state->terrainHeight = height;
}

/// The current scene's rows of the scene item table (Data_080f2204): those
/// after the first header for this scene whose flag is -1 or not yet set, or
/// NULL.
s16 *Func_080ceba8(void) {
  s32 scene = gMainRoutineWork.scene;
  s16 *p = Data_080f2204;
  s16 *found = NULL;
  s32 header;
  s32 flag;
  s16 value;

  for (;;) {
    header = *p++;
    flag = *p++;
    if (header == 0)
      break;
    if ((header & 0x3fff) == scene && (flag == -1 || !GameFlag_Test(flag))) {
      found = p;
      break;
    }
    // Skip to the next header (bit 14) or the end.
    while ((value = p[0]) != 0 && !(value & 0x4000))
      p += 2;
  }
  return found;
}

/// Clears out[0..2], then fills it from Func_080ca9cc's table entry for
/// value; returns that lookup's result (0 found, -1 not).
s32 Func_080cec24(s32 value, s32 *out) {
  SET_VECTOR(out, 0, 0, 0);
  return Func_080ca9cc(value, out);
}

void Func_080d3b28(s32 id, s32 item);

void Func_080cec34(void) {
  s16 *rows = Func_080ceba8();
  struct SceneItemWork *work = Ram_HeapSlots->eventWork;
  s32 i;
  s16 type;
  s32 id;
  s32 flag;
  s32 item;
  s32 position[3];
  s32 kind;
  s32 buried;
  struct Actor *object;

  if (rows == NULL)
    return;
  i = 0;
  type = *rows++;
  if (type == 0 || (type & 0x4000))
    return;
  do {
    id = rows[0];
    flag = rows[1];
    item = rows[2];
    rows += 3;
    if ((type == 128 || flag == -1 || !GameFlag_Test(flag)) &&
        !Func_080cec24(id, position)) {
      buried = 0;
      if ((u16)(type - 128) <= 1)
        kind = 487;
      else if (type == 130)
        kind = 265;
      else if (type == 132)
        kind = 322;
      else if (type == 131)
        kind = 489;
      else {
        buried = 1;
        kind = 489;
      }
      object = Func_080200c0(kind, position[0], position[1], position[2]);
      if (object != NULL) {
        if (work->mode == 3) {
          object->motionFlags = 2;
          object->terrainHeight = 0;
          object->y = 0;
        } else {
          object->terrainHeight =
              MapGrid_GetTerrainHeightFar(0, object->x, object->z);
          object->y += object->terrainHeight;
          if (buried) {
            u8 *motionFlags = &object->motionFlags;
            struct MapWork *map;
            struct MapCell *cells;
            s32 x;
            s32 z;

            *motionFlags = 2;
            object->terrainHeight += 0xfff00000;
            object->y = object->terrainHeight;
            map = Ram_HeapSlots->mapWork;
            x = object->x >> 20;
            z = object->z >> 20;
            cells = (struct MapCell *)gMapCellBuffer;
            if (map != NULL)
              cells = map->layers[0].cells;
            cells += x + (z << 7);
            if ((u32)(cells->collisionCode - 242) <= 5)
              cells->collisionCode = 0;
          }
        }
        Actor_SetMode(object, 1);
        Actor_SetSingleChildFlagsFar(object, 0);
        if (GameFlag_Test(flag))
          Actor_SetMode(object, 2);
        Actor_ResetMotion(object);
        object->timer = object->x / 0x10000;
        object->unk66 = object->z / 0x10000;
        object->priorityFlags = 1;
        object->collisionFlags = 1;
        work->objects[i] = object;
        if (type == 131)
          Func_080d3b28(i + 64, item);
        if (type == 133)
          object->collisionFlags = 0;
      }
    }
    i++;
    if (i > 15)
      break;
    type = *rows++;
    if (type == 0)
      break;
  } while (!(type & 0x4000));
}

/* VIEW(field_sequence): tag struct ObjectSprite as struct ObjectSprite__field_sequence; module/field/field.h has another definition. */
struct ObjectSprite__field_sequence {
  u8 unk00[5];
  u8 attr0High;
  u8 unk06;
  u8 attr1High;
  u8 unk08[0xf];
  u8 yOffset;
};

void Actor_SetWideSprite(u32 objectId, s32 mode) {
  struct Actor *object;
  struct ObjectSprite__field_sequence *sprite;

  object = ObjectTable_Get(objectId);
  if (object == 0)
    return;
  if (mode == 0) {
    object->x = 0;
    object->z = 0;
    object->unk4c = 0x31415927;
    return;
  }
  sprite = object->sprite;
  if (sprite == 0)
    return;
  sprite->yOffset += 4;
  sprite->attr0High = (sprite->attr0High & 0x3f) | 0x40;
  sprite->attr1High &= 0x3f;
}

void Actor_SetSquareSprite(u32 objectId) {
  struct Actor *object;
  struct ObjectSprite__field_sequence *sprite;

  object = ObjectTable_Get(objectId);
  if (object == 0)
    return;
  sprite = object->sprite;
  if (sprite == 0)
    return;
  sprite->yOffset = 8;
  sprite->attr0High &= 0x3f;
  sprite->attr1High = (sprite->attr1High & 0x3f) | 0x40;
}

s32 Actor_GetSpriteShape(u32 objectId) {
  struct Actor *object;
  struct ObjectSprite__field_sequence *sprite;
  s32 shape = 0;

  object = ObjectTable_Get(objectId);
  if (object != 0) {
    sprite = object->sprite;
    if (sprite != 0)
      shape = sprite->attr0High >> 6;
  }
  return shape;
}

void Actor_ReturnHome(s32 mode, u32 objectId, s32 flag) {
  struct ActorScriptView *object;

  object = (struct ActorScriptView *)ObjectTable_Get(objectId);
  if (object == 0)
    return;
  if (mode == 1)
    object->z = 0;
  if (mode == 2) {
    GameFlag_SetBit(flag);
    if (object->x != 0)
      Actor_SetSquareSprite(objectId);
    object->x = object->homeX << 16;
    object->z = object->homeZ << 16;
    BattleEffect_DropObjectAtAnchor(objectId);
  }
}

void Actor_ShakeAngle(u32 objectId) {
  struct Actor *object;
  s32 angle;
  s32 i;

  object = ObjectTable_Get(objectId);
  if (object == 0)
    return;
  angle = object->facing;
  for (i = 15; i >= 0; i--) {
    u32 a = Random16();
    u32 b = Random16();
    u32 cur;

    cur = object->facing;
    b >>= 4;
    a >>= 4;
    object->facing = cur + (a - b);
    wait(1);
  }
  object->facing = angle;
}

struct Actor;

void ObjectEvent_SetMode5WithCue(u32 objectId) {
  Actor_SetMode(ObjectTable_Get(objectId), 5);
  Sound_PlayFar(125);
  wait(12);
}

void ObjectEvent_Spring(u32 objectId) {
  struct Actor *object;

  object = ObjectTable_Get(objectId);
  wait(18);
  Actor_SetMode(object, 7);
  Sound_PlayFar(146);
  if (object != 0) {
    object->velocityY = 0x80000;
    Actor_SetSingleChildFlagsFar(object, 1);
  }
}

void ObjectEvent_SetMode4WithCue(u32 objectId) {
  Actor_SetMode(ObjectTable_Get(objectId), 4);
  Sound_PlayFar(124);
  wait(12);
}

void ObjectEvent_SetMode2(u32 objectId) {
  Actor_SetMode(ObjectTable_Get(objectId), 2);
}

void ObjectEvent_SetMode6WithCue(u32 objectId) {
  Actor_SetMode(ObjectTable_Get(objectId), 6);
  Sound_PlayFar(124);
  wait(12);
}

struct Actor *ObjectEvent_GetObject(u32 objectId) {
  return ObjectTable_Get(objectId);
}

void ObjectEvent_Remove(u32 objectId) { Actor_ReleaseAtIndex(objectId); }

void ObjectEvent_PlaceOnGround(u32 objectId, s32 x, s32 z) {
  struct ActorScriptView *object;

  object = (struct ActorScriptView *)ObjectTable_Get(objectId);
  if (object == 0)
    return;
  if (x == -1)
    x = object->homeX << 16;
  if (z == -1)
    z = object->homeZ << 16;
  object->x = x;
  object->z = z;
  object->y = object->terrainHeight = MapGrid_GetTerrainHeightFar(0, x, z);
}
