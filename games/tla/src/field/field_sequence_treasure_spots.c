/// @file
/// The map's treasure spots: chests and Psynergy Stones.
// Part of field_sequence.c.

#include "field_sequence_internal.h"
#include "fixed_math.h"
#include "sounds.h"
#include "field/field_sequence.h"

#include "sprite_ids.h"

/// Sets an actor's y and groundHeight to the map's height at its x and z.
void Actor_SnapToTerrain(void *object) {
  struct Actor *actor = object;
  s32 groundHeight;

  groundHeight = MapGrid_GetTerrainHeightFar(0, actor->x.fixed, actor->z.fixed);
  actor->y.fixed = groundHeight;
  actor->groundHeight = groundHeight;
}

/// The current scene's rows of the scene item table (gSceneItems): those
/// after the first header for this scene whose flag is -1 or not yet set, or
/// NULL.
s16 *SceneItem_GetRows(void) {
  s32 scene = gMainRoutineWork.next;
  s16 *p = gSceneItems;
  s16 *found = NULL;
  s32 header;
  s32 flag;
  s16 value;

  for (;;) {
    header = *p++;
    flag = *p++;
    if (header == 0)
      break;
    if ((header & 0x3fff) == scene && (flag == -1 || !GameFlag_Get(flag))) {
      found = p;
      break;
    }
    // Skip to the next header (bit 14) or the end.
    while ((value = p[0]) != 0 && !(value & 0x4000))
      p += 2;
  }
  return found;
}

/// Clears out[0..2], then fills it from ScenePoint_GetPosition's table entry
/// for value; returns that lookup's result (0 found, -1 not).
s32 SceneItem_FindPosition(s32 value, s32 *out) {
  SET_VECTOR(out, 0, 0, 0);
  return ScenePoint_GetPosition(value, out);
}

void Actor_ShowItemIconById(s32 id, s32 item);

/// Places an actor at each of the map's treasure spots (ids 100 to 239) that
/// the scene has an event for, up to ten: a chest for a chest event, shown open
/// once its flag is set and gone once its mimic was beaten, and a Psynergy
/// Stone for an examine trigger giving one, until it is taken.
void Field_PlaceTreasureSpots(void) {
  s16 *rows = SceneItem_GetRows();
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
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
    if ((type == 128 || flag == -1 || !GameFlag_Get(flag)) &&
        !SceneItem_FindPosition(id, position)) {
      buried = 0;
      if ((u16)(type - 128) <= 1)
        kind = 487;
      else if (type == 130)
        kind = 265;
      else if (type == 132)
        kind = SPRITE_STONE_TABLET;
      else if (type == 131)
        kind = 489;
      else {
        buried = 1;
        kind = 489;
      }
      object = Actor_CreateFar(kind, position[0], position[1], position[2]);
      if (object != NULL) {
        if (work->mode == 3) {
          object->motionFlags = 2;
          object->groundHeight = 0;
          object->y.fixed = 0;
        } else {
          object->groundHeight =
              MapGrid_GetTerrainHeightFar(0, object->x.fixed, object->z.fixed);
          object->y.fixed += object->groundHeight;
          if (buried) {
            u8 *motionFlags = &object->motionFlags;
            struct MapWork *map;
            struct MapCell *cells;
            s32 x;
            s32 z;

            *motionFlags = 2;
            object->groundHeight -= px(16);
            object->y.fixed = object->groundHeight;
            map = Ram_HeapSlots->mapGrid;
            x = FIELD_CELL(object->x.fixed);
            z = FIELD_CELL(object->z.fixed);
            cells = (struct MapCell *)gMapGridCellBuffer;
            if (map != NULL)
              cells = map->layers[0].cells;
            cells += x + z * MAPGRID_SIZE;
            if ((u32)(cells->collisionCode - 242) <= 5)
              cells->collisionCode = 0;
          }
        }
        Actor_SelectAnimationFar(object, 1);
        Actor_SetSpriteFlagsFar(object, 0);
        if (GameFlag_Get(flag))
          Actor_SelectAnimationFar(object, 2);
        Actor_StartIdleFar(object);
        object->homeX = object->x.fixed / Q16_ONE;
        object->homeZ = object->z.fixed / Q16_ONE;
        object->priorityFlags = 1;
        object->collisionFlags = 1;
        work->actors[FIELD_TREASURE_ACTOR + i] = object;
        if (type == 131)
          Actor_ShowItemIconById(FIELD_TREASURE_ACTOR + i, item);
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

/// Mode 0 moves the actor to x and z 0 and marks it with riseBaseY 0x31415927;
/// any other mode draws its sprite wide (shape 1), 4 pixels lower.
void Actor_SetWideSprite(u32 objectId, s32 mode) {
  struct Actor *object;
  struct Sprite *sprite;

  object = Actor_Lookup(objectId);
  if (object == 0)
    return;
  if (mode == 0) {
    object->x.fixed = 0;
    object->z.fixed = 0;
    object->riseBaseY = 0x31415927;
    return;
  }
  sprite = object->sprite;
  if (sprite == 0)
    return;
  sprite->offsetY += 4;
  sprite->image.shape = 1;
  sprite->image.size = 0;
}

/// Draws the actor's sprite square (shape 0, size 1), 8 pixels down.
void Actor_SetSquareSprite(u32 objectId) {
  struct Actor *object;
  struct Sprite *sprite;

  object = Actor_Lookup(objectId);
  if (object == 0)
    return;
  sprite = object->sprite;
  if (sprite == 0)
    return;
  sprite->offsetY = 8;
  sprite->image.shape = 0;
  sprite->image.size = 1;
}

/// Returns the OAM shape of the actor's sprite (0 square, 1 wide, 2 tall), or
/// 0 without one.
s32 Actor_GetSpriteShape(u32 objectId) {
  struct Actor *object;
  struct Sprite *sprite;
  s32 shape = 0;

  object = Actor_Lookup(objectId);
  if (object != 0) {
    sprite = object->sprite;
    if (sprite != 0)
      shape = sprite->image.shape;
  }
  return shape;
}

/// Mode 1 moves the actor to z 0; mode 2 sets flag, draws a moved actor square
/// again and drops it back home (homeX, homeZ) at Scoop's anchor.
void Actor_ReturnHome(s32 mode, u32 objectId, s32 flag) {
  struct Actor *object;

  object = Actor_Lookup(objectId);
  if (object == 0)
    return;
  if (mode == 1)
    object->z.fixed = 0;
  if (mode == 2) {
    GameFlag_Set(flag);
    if (object->x.fixed != 0)
      Actor_SetSquareSprite(objectId);
    object->x.fixed = px(object->homeX);
    object->z.fixed = px(object->homeZ);
    BattleEffect_DropObjectAtAnchor(objectId);
  }
}

/// Shakes the actor's facing at random for 16 frames, then turns it back.
void Actor_ShakeAngle(u32 objectId) {
  struct Actor *object;
  s32 angle;
  s32 i;

  object = Actor_Lookup(objectId);
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

/// Closes the chest of the actor again (its item could not be carried), with
/// SE_CHEST_CLOSE, and waits 12 frames.
void TreasureSpot_CloseChest(u32 objectId) {
  Actor_SelectAnimationFar(Actor_Lookup(objectId), 5);
  Sound_PlayFar(SE_CHEST_CLOSE);
  wait(12);
}

/// Makes the chest of the actor jump up as a mimic: after 18 frames it takes
/// animation 7, plays SE_ENEMY_CRY and gets an upward velocity of 8 pixels a
/// frame, its shadow shown.
void TreasureSpot_LaunchMimic(u32 objectId) {
  struct Actor *object;

  object = Actor_Lookup(objectId);
  wait(18);
  Actor_SelectAnimationFar(object, 7);
  Sound_PlayFar(SE_ENEMY_CRY);
  if (object != 0) {
    object->velocityY = px(8);
    Actor_SetSpriteFlagsFar(object, 1);
  }
}

/// Opens the chest of the actor with SE_CHEST_OPEN, and waits 12 frames.
void TreasureSpot_OpenChest(u32 objectId) {
  Actor_SelectAnimationFar(Actor_Lookup(objectId), 4);
  Sound_PlayFar(SE_CHEST_OPEN);
  wait(12);
}

/// Shows the chest of the actor open, with no sound.
void TreasureSpot_ShowOpened(u32 objectId) {
  Actor_SelectAnimationFar(Actor_Lookup(objectId), 2);
}

/// Gives the actor animation 6 with SE_CHEST_OPEN, and waits 12 frames.
void ObjectEvent_SetMode6WithCue(u32 objectId) {
  Actor_SelectAnimationFar(Actor_Lookup(objectId), 6);
  Sound_PlayFar(SE_CHEST_OPEN);
  wait(12);
}

/// Returns the actor, or NULL.
struct Actor *TreasureSpot_GetActor(u32 objectId) {
  return Actor_Lookup(objectId);
}

/// Releases the actor and empties its slot.
void ObjectEvent_Remove(u32 objectId) { Actor_ReleaseAtIndex(objectId); }

/// Moves the actor to (x, z) on the ground, 16.16 pixels; -1 keeps its home
/// (homeX, homeZ) on that axis.
void TreasureSpot_SetActorPosition(u32 objectId, s32 x, s32 z) {
  struct Actor *object;

  object = Actor_Lookup(objectId);
  if (object == 0)
    return;
  if (x == -1)
    x = px(object->homeX);
  if (z == -1)
    z = px(object->homeZ);
  object->x.fixed = x;
  object->z.fixed = z;
  object->y.fixed = object->groundHeight = MapGrid_GetTerrainHeightFar(0, x, z);
}
