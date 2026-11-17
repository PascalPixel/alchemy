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
    if ((header & (SCENE_ITEMS_HEADER - 1)) == scene &&
        (flag == -1 || !GameFlag_Get(flag))) {
      found = p;
      break;
    }
    // Skip to the next header or the end.
    while ((value = p[0]) != 0 && !(value & SCENE_ITEMS_HEADER))
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
/// the scene has an event for, up to sixteen: a chest for a chest event, shown
/// open once its flag is set and gone once its mimic was beaten, and a Psynergy
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
  struct Actor *actor;

  if (rows == NULL)
    return;
  i = 0;
  type = *rows++;
  if (type == 0 || (type & SCENE_ITEMS_HEADER))
    return;
  do {
    id = rows[0];
    flag = rows[1];
    item = rows[2];
    rows += 3;
    if ((type == CONTAINER_CHEST || flag == -1 || !GameFlag_Get(flag)) &&
        !SceneItem_FindPosition(id, position)) {
      buried = 0;
      if ((u16)(type - CONTAINER_CHEST) <= CONTAINER_MIMIC - CONTAINER_CHEST)
        kind = SPRITE_CHEST;
      else if (type == CONTAINER_PSYNERGY_STONE)
        kind = SPRITE_PSYNERGY_STONE;
      else if (type == CONTAINER_STONE_TABLET)
        kind = SPRITE_STONE_TABLET;
      else if (type == CONTAINER_GROUND)
        kind = SPRITE_UNSEEN_MARKER;
      else {
        buried = 1;
        kind = SPRITE_UNSEEN_MARKER;
      }
      actor = Actor_CreateFar(kind, position[0], position[1], position[2]);
      if (actor != NULL) {
        if (work->mode == ACTOR_SYSTEM_WORLD_MAP) {
          actor->motionFlags = ACTOR_FALLS;
          actor->groundHeight = 0;
          actor->y.fixed = 0;
        } else {
          actor->groundHeight =
              MapGrid_GetTerrainHeightFar(0, actor->x.fixed, actor->z.fixed);
          actor->y.fixed += actor->groundHeight;
          if (buried) {
            u8 *motionFlags = &actor->motionFlags;
            struct MapWork *map;
            struct MapCell *cells;
            s32 x;
            s32 z;

            *motionFlags = ACTOR_FALLS;
            actor->groundHeight -= toFIXED(16);
            actor->y.fixed = actor->groundHeight;
            map = Ram_HeapSlots->mapGrid;
            x = FIELD_CELL(actor->x.fixed);
            z = FIELD_CELL(actor->z.fixed);
            cells = (struct MapCell *)gMapGridCellBuffer;
            if (map != NULL)
              cells = map->layers[0].cells;
            cells += x + z * MAPGRID_SIZE;
            if ((u32)(cells->collisionCode - CELL_BEHAVIOR_JAR) <=
                CELL_BEHAVIOR_WELL - CELL_BEHAVIOR_JAR)
              cells->collisionCode = 0;
          }
        }
        Actor_SelectAnimationFar(actor, 1);
        Actor_SetSpriteFlagsFar(actor, 0);
        if (GameFlag_Get(flag))
          Actor_SelectAnimationFar(actor, 2);
        Actor_StartIdleFar(actor);
        actor->homeX = actor->x.fixed / FIXED_ONE;
        actor->homeZ = actor->z.fixed / FIXED_ONE;
        actor->priorityFlags = ACTOR_PRIORITY_AUTOMATIC;
        actor->collisionFlags = ACTOR_SOLID;
        work->actors[FIELD_TREASURE_ACTOR + i] = actor;
        if (type == CONTAINER_GROUND)
          Actor_ShowItemIconById(FIELD_TREASURE_ACTOR + i, item);
        if (type == CONTAINER_POND)
          actor->collisionFlags = 0;
      }
    }
    i++;
    if (i > 15)
      break;
    type = *rows++;
    if (type == 0)
      break;
  } while (!(type & SCENE_ITEMS_HEADER));
}

/// Mode 0 moves the actor to x and z 0 and marks it with riseBaseY
/// TREASURE_SPOT_ON_CELL; any other mode draws its sprite wide (shape 1), 4
/// pixels lower.
void Actor_SetWideSprite(u32 objectId, s32 mode) {
  struct Actor *actor;
  struct Sprite *sprite;

  actor = Actor_Lookup(objectId);
  if (actor == 0)
    return;
  if (mode == 0) {
    actor->x.fixed = 0;
    actor->z.fixed = 0;
    actor->riseBaseY = TREASURE_SPOT_ON_CELL;
    return;
  }
  sprite = actor->sprite;
  if (sprite == 0)
    return;
  sprite->offsetY += 4;
  sprite->image.shape = 1;
  sprite->image.size = 0;
}

/// Draws the actor's sprite square (shape 0, size 1), 8 pixels down.
void Actor_SetSquareSprite(u32 objectId) {
  struct Actor *actor;
  struct Sprite *sprite;

  actor = Actor_Lookup(objectId);
  if (actor == 0)
    return;
  sprite = actor->sprite;
  if (sprite == 0)
    return;
  sprite->offsetY = 8;
  sprite->image.shape = 0;
  sprite->image.size = 1;
}

/// Returns the OAM shape of the actor's sprite (0 square, 1 wide, 2 tall), or
/// 0 without one.
s32 Actor_GetSpriteShape(u32 objectId) {
  struct Actor *actor;
  struct Sprite *sprite;
  s32 shape = 0;

  actor = Actor_Lookup(objectId);
  if (actor != 0) {
    sprite = actor->sprite;
    if (sprite != 0)
      shape = sprite->image.shape;
  }
  return shape;
}

/// Mode 1 moves the actor to z 0; mode 2 sets flag, draws a moved actor square
/// again and drops it back home (homeX, homeZ) at Scoop's anchor.
void Actor_ReturnHome(s32 mode, u32 objectId, s32 flag) {
  struct Actor *actor;

  actor = Actor_Lookup(objectId);
  if (actor == 0)
    return;
  if (mode == 1)
    actor->z.fixed = 0;
  if (mode == 2) {
    GameFlag_Set(flag);
    if (actor->x.fixed != 0)
      Actor_SetSquareSprite(objectId);
    actor->x.fixed = toFIXED(actor->homeX);
    actor->z.fixed = toFIXED(actor->homeZ);
    BattleEffect_DropObjectAtAnchor(objectId);
  }
}

/// Shakes the actor's facing at random for 16 frames, then turns it back.
void Actor_ShakeAngle(u32 objectId) {
  struct Actor *actor;
  s32 angle;
  s32 i;

  actor = Actor_Lookup(objectId);
  if (actor == 0)
    return;
  angle = actor->facing;
  for (i = 0; i < 16; i++) {
    u32 a = Random16();
    u32 b = Random16();
    u32 cur;

    cur = actor->facing;
    b >>= 4;
    a >>= 4;
    actor->facing = cur + (a - b);
    wait(1);
  }
  actor->facing = angle;
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
  struct Actor *actor;

  actor = Actor_Lookup(objectId);
  wait(18);
  Actor_SelectAnimationFar(actor, 7);
  Sound_PlayFar(SE_ENEMY_CRY);
  if (actor != 0) {
    actor->velocityY = toFIXED(8);
    Actor_SetSpriteFlagsFar(actor, 1);
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
void TreasureSpot_SetActorPosition(u32 objectId, FIXED x, FIXED z) {
  struct Actor *actor;

  actor = Actor_Lookup(objectId);
  if (actor == 0)
    return;
  if (x == -1)
    x = toFIXED(actor->homeX);
  if (z == -1)
    z = toFIXED(actor->homeZ);
  actor->x.fixed = x;
  actor->z.fixed = z;
  actor->y.fixed = actor->groundHeight = MapGrid_GetTerrainHeightFar(0, x, z);
}
