/// @file
/// The map's treasure spots: chests and Psynergy Stones.
// Part of field_sequence.c.

#include "field_sequence_internal.h"
#include "fixed_math.h"
#include "sounds.h"
#include "field/field_sequence.h"

#include "sprite_ids.h"

/// Sets an actor's y and groundHeight to the map's height at its x and z.
void Actor_SnapToTerrain(void *actorPointer) {
  struct Actor *actor = actorPointer;
  s32 groundHeight;

  groundHeight = MapGrid_GetTerrainHeightFar(0, actor->x.fixed, actor->z.fixed);
  actor->y.fixed = groundHeight;
  actor->groundHeight = groundHeight;
}

/// The current scene's rows of the scene item table (gSceneItems): those
/// after the first header for this scene whose flag is -1 or not yet set, or
/// NULL.
s16 *SceneItem_GetRows(void) {
  s32 scene = gMainRoutineState.next;
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

/// Clears out, then fills it with the position of scene point value; returns
/// 0 when the point was found, else -1.
s32 SceneItem_FindPosition(s32 value, s32 *out) {
  SET_VECTOR(out, 0, 0, 0);
  return ScenePoint_GetPosition(value, out);
}

void Actor_ShowItemIconById(s32 id, s32 item);

/// The chest sprite's animations.
enum {
  TREASURE_SPOT_CHEST_OPEN = 2, ///< standing open
  TREASURE_SPOT_CHEST_OPENING = 4,
  TREASURE_SPOT_CHEST_CLOSING = 5, ///< closing again on what nobody could carry
  TREASURE_SPOT_CHEST_OPENING_EMPTY = 6, ///< its lid lifting on nothing inside
  TREASURE_SPOT_CHEST_MIMIC = 7          ///< jumping up as a mimic
};

/// Places an actor at each of the map's treasure spots (ids 100 to 239) that
/// the scene has an event for, up to sixteen: a chest for a chest event, shown
/// open once its flag is set and gone once its mimic was beaten, and a Psynergy
/// Stone for an examine trigger giving one, until it is taken.
void Field_PlaceTreasureSpots(void) {
  s16 *rows = SceneItem_GetRows();
  struct FieldState *state = Ram_HeapSlots->fieldState;
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
        if (state->mode == ACTOR_SYSTEM_WORLD_MAP) {
          actor->motionFlags = ACTOR_FALLS;
          actor->groundHeight = 0;
          actor->y.fixed = 0;
        } else {
          actor->groundHeight =
              MapGrid_GetTerrainHeightFar(0, actor->x.fixed, actor->z.fixed);
          actor->y.fixed += actor->groundHeight;
          if (buried) {
            u8 *motionFlags = &actor->motionFlags;
            struct MapState *map;
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
              cells->collisionCode = CELL_BEHAVIOR_NORMAL;
          }
        }
        Actor_SelectAnimationFar(actor, ANIM_STAND);
        Actor_SetSpriteFlagsFar(actor, 0);
        if (GameFlag_Get(flag))
          Actor_SelectAnimationFar(actor, TREASURE_SPOT_CHEST_OPEN);
        Actor_StartIdleFar(actor);
        actor->homeX = actor->x.fixed / FIXED_ONE;
        actor->homeZ = actor->z.fixed / FIXED_ONE;
        actor->priorityFlags = ACTOR_PRIORITY_AUTOMATIC;
        actor->collisionFlags = ACTOR_SOLID;
        state->actors[FIELD_TREASURE_ACTOR + i] = actor;
        if (type == CONTAINER_GROUND)
          Actor_ShowItemIconById(FIELD_TREASURE_ACTOR + i, item);
        if (type == CONTAINER_POND)
          actor->collisionFlags = 0;
      }
    }
    i++;
    if (i >= TREASURE_SPOT_MAX)
      break;
    type = *rows++;
    if (type == 0)
      break;
  } while (!(type & SCENE_ITEMS_HEADER));
}

/// Mode 0 moves the actor to x and z 0 and marks it with sceneWord
/// TREASURE_SPOT_ON_CELL; any other mode draws its sprite wide (shape 1), 4
/// pixels lower.
void Actor_SetWideSprite(u32 actorId, s32 mode) {
  struct Actor *actor;
  struct Sprite *sprite;

  actor = Actor_Lookup(actorId);
  if (actor == 0)
    return;
  if (mode == 0) {
    actor->x.fixed = 0;
    actor->z.fixed = 0;
    actor->sceneWord = TREASURE_SPOT_ON_CELL;
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
void Actor_SetSquareSprite(u32 actorId) {
  struct Actor *actor;
  struct Sprite *sprite;

  actor = Actor_Lookup(actorId);
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
s32 Actor_GetSpriteShape(u32 actorId) {
  struct Actor *actor;
  struct Sprite *sprite;
  s32 shape = 0;

  actor = Actor_Lookup(actorId);
  if (actor != 0) {
    sprite = actor->sprite;
    if (sprite != 0)
      shape = sprite->image.shape;
  }
  return shape;
}

/// Mode 1 moves the actor to z 0; mode 2 sets flag, draws a moved actor square
/// again and drops it back home (homeX, homeZ) at Scoop's anchor.
void Actor_ReturnHome(s32 mode, u32 actorId, s32 flag) {
  struct Actor *actor;

  actor = Actor_Lookup(actorId);
  if (actor == 0)
    return;
  if (mode == 1)
    actor->z.fixed = 0;
  if (mode == 2) {
    GameFlag_Set(flag);
    if (actor->x.fixed != 0)
      Actor_SetSquareSprite(actorId);
    actor->x.fixed = toFIXED(actor->homeX);
    actor->z.fixed = toFIXED(actor->homeZ);
    Scoop_DropFromHand(actorId);
  }
}

/// Shakes the actor's facing at random for 16 frames, then turns it back.
UNUSED void Actor_ShakeAngle(u32 actorId) {
  struct Actor *actor;
  s32 angle;
  s32 i;

  actor = Actor_Lookup(actorId);
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
void TreasureSpot_CloseChest(u32 actorId) {
  Actor_SelectAnimationFar(Actor_Lookup(actorId), TREASURE_SPOT_CHEST_CLOSING);
  Sound_PlayFar(SE_CHEST_CLOSE);
  wait(12);
}

/// Makes the chest of the actor jump up as a mimic: after 18 frames it takes
/// TREASURE_SPOT_CHEST_MIMIC, plays SE_ENEMY_CRY and gets an upward velocity of
/// 8 pixels a frame, its shadow shown.
void TreasureSpot_LaunchMimic(u32 actorId) {
  struct Actor *actor;

  actor = Actor_Lookup(actorId);
  wait(18);
  Actor_SelectAnimationFar(actor, TREASURE_SPOT_CHEST_MIMIC);
  Sound_PlayFar(SE_ENEMY_CRY);
  if (actor != 0) {
    actor->velocityY = toFIXED(8);
    Actor_SetSpriteFlagsFar(actor, SPRITE_FLAG_SHADOW);
  }
}

/// Opens the chest of the actor with SE_CHEST_OPEN, and waits 12 frames.
void TreasureSpot_OpenChest(u32 actorId) {
  Actor_SelectAnimationFar(Actor_Lookup(actorId), TREASURE_SPOT_CHEST_OPENING);
  Sound_PlayFar(SE_CHEST_OPEN);
  wait(12);
}

/// Shows the chest of the actor open, with no sound.
void TreasureSpot_ShowOpened(u32 actorId) {
  Actor_SelectAnimationFar(Actor_Lookup(actorId), TREASURE_SPOT_CHEST_OPEN);
}

/// Opens the empty chest of the actor with SE_CHEST_OPEN, and waits 12 frames.
void TreasureSpot_OpenEmptyChest(u32 actorId) {
  Actor_SelectAnimationFar(Actor_Lookup(actorId),
                           TREASURE_SPOT_CHEST_OPENING_EMPTY);
  Sound_PlayFar(SE_CHEST_OPEN);
  wait(12);
}

/// Returns the actor, or NULL.
struct Actor *TreasureSpot_GetActor(u32 actorId) {
  return Actor_Lookup(actorId);
}

/// Releases the actor and empties its slot.
void TreasureSpot_Remove(u32 actorId) { Actor_ReleaseAtIndex(actorId); }

/// Moves the actor to (x, z) on the ground, 16.16 pixels; -1 keeps its home
/// (homeX, homeZ) on that axis.
void TreasureSpot_SetActorPosition(u32 actorId, FIXED x, FIXED z) {
  struct Actor *actor;

  actor = Actor_Lookup(actorId);
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
