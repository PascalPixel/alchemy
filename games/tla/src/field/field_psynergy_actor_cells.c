/// @file
/// Actors standing as blocks in the map: their cells tagged and the tags
/// cleared, the actor at a position, and effect sprites filled and queued.
// Part of field_psynergy.c.

#include "field_psynergy_internal.h"
#include "stddef.h"

/// A 16.16 height in the 8-pixel steps a collision tag's height counts.
#define HEIGHT_STEPS(height) (FIXEDtoINT(height) >> 3)
/// A block's height, 32 pixels, in those steps.
#define BLOCK_HEIGHT_STEPS 4

/// Makes each listed actor (ids, ended by ACTOR_LIST_END) a block in the map:
/// its cell's shape is tagged BLOCK_HEIGHT_STEPS above the ground, so the party
/// can stand on it, its cell is marked MAP_CELL_JUMP, a gap to jump onto from
/// another block (the old mark kept in the actor), and the overhead layer's
/// cell is a wall. A
/// leader below the new ground is lifted onto it. Scenes call it as they
/// start, for the blocks Move pushes.
void Actor_PlaceBlocks(u16 *actors) {
  struct MapWork *map = Ram_HeapSlots->mapGrid;
  struct MapCell *blocks = map->layers[MAPGRID_LAYER_OVERHEAD].cells;
  u8 *grid = gMapShapeGrid;
  struct Actor *leader;
  s32 height;

  for (; *actors != ACTOR_LIST_END; actors++) {
    struct Actor *actor = Actor_GetById(*actors);
    u32 key =
        MapGrid_GetTerrainKindFar(actor->layer, actor->x.fixed, actor->z.fixed);
    s32 level = HEIGHT_STEPS(MapGrid_GetTerrainHeightFar(
        actor->layer, actor->x.fixed, actor->z.fixed));
    s32 slot = Actor_GetCellIndex(actor);
    struct MapCell *cells = map->layers[actor->layer].cells;
    // The layer's row of the shape grid, as an integer sum with its index
    // first.
    u8 *shapes =
        (u8 *)((((s32)cells - (s32)Ram_MapCellBuffer) >> 2) + (s32)grid);
    struct MapCell *cell;
    s32 tag;
    u8 flags;

    actor->motionFlags = 0;
    actor->collisionFlags = 0;
    Actor_SetSpriteFlagsFar(actor, 0);
    shapes += slot;
    tag = MapCollision_FindOrAddTag(actor->layer, key,
                                    level + BLOCK_HEIGHT_STEPS);
    cell = &cells[slot];
    flags = cell->attributeB;
    actor->riseBaseY = flags;
    cell->attributeB = flags | MAP_CELL_JUMP;
    *shapes = tag;
    cell = &blocks[slot];
    cell->collisionCode = CELL_BEHAVIOR_WALL;
  }
  leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  height = MapGrid_GetTerrainHeightFar(leader->layer, leader->x.fixed,
                                       leader->z.fixed);
  if (leader->y.fixed < height)
    leader->y.fixed = leader->groundHeight = height;
}

struct Actor *Leader_FindActorAhead(void);

/// Takes the block about to move out of the map: the field effect's target
/// while one runs, else the actor ahead of the leader, if it is one of the
/// listed actors. Its cell goes back to the ground and its old mark, and the
/// event work remembers it for Actor_PlaceMovedBlock.
void Actor_LiftTargetBlock(u16 *actors) {
  struct FieldWork *event = Ram_HeapSlots->fieldWork;
  struct FieldEffectWork *work = Ram_HeapSlots->fieldEffectWork;
  struct Actor *target;

  if (work != NULL && event->psynergy != 0)
    target = work->child;
  else
    target = Leader_FindActorAhead();
  event->movedBlock = NULL;

  for (; *actors != ACTOR_LIST_END; actors++) {
    if (target == Actor_GetById(*actors)) {
      struct MapWork *map = Ram_HeapSlots->mapGrid;
      u8 *grid = gMapShapeGrid;
      struct MapCell *cells = map->layers[target->layer].cells;
      struct MapCell *blocks = map->layers[MAPGRID_LAYER_OVERHEAD].cells;
      u8 *shapes = &grid[((s32)cells - (s32)Ram_MapCellBuffer) >> 2];
      u32 key = MapGrid_GetTerrainKindFar(target->layer, target->x.fixed,
                                          target->z.fixed);
      s32 level = MapGrid_GetTerrainHeightFar(target->layer, target->x.fixed,
                                              target->z.fixed);
      s32 slot = Actor_GetCellIndex(target);
      s32 tag;

      shapes += slot;
      tag = MapCollision_FindOrAddTag(target->layer, key,
                                      HEIGHT_STEPS(level) - BLOCK_HEIGHT_STEPS);
      cells += slot;
      blocks += slot;
      ((struct ShapeCell *)shapes)->shape = tag;
      cells->attributeB = target->riseBaseY;
      blocks->collisionCode = 0;
      event->movedBlock = target;
    }
  }
}

/// Puts the block Actor_LiftTargetBlock remembered back into the map at the
/// cell it has moved to, as Actor_PlaceBlocks places each.
void Actor_PlaceMovedBlock(void) {
  struct Actor *target = gFieldWork->movedBlock;

  if (target != NULL) {
    struct MapWork *map = Ram_HeapSlots->mapGrid;
    u8 *grid = Ram_MapShapeGrid;
    struct MapCell *cells = map->layers[target->layer].cells;
    struct MapCell *blocks = map->layers[MAPGRID_LAYER_OVERHEAD].cells;
    u8 *shapes = &grid[((s32)cells - (s32)Ram_MapCellBuffer) >> 2];
    u32 key = MapGrid_GetTerrainKindFar(target->layer, target->x.fixed,
                                        target->z.fixed);
    s32 level = MapGrid_GetTerrainHeightFar(target->layer, target->x.fixed,
                                            target->z.fixed);
    s32 slot = Actor_GetCellIndex(target);
    s32 tag;

    shapes += slot;
    tag = MapCollision_FindOrAddTag(target->layer, key,
                                    HEIGHT_STEPS(level) + BLOCK_HEIGHT_STEPS);
    ((struct ShapeCell *)shapes)->shape = tag;
    cells += slot;
    target->riseBaseY = cells->attributeB;
    cells->attributeB |= MAP_CELL_JUMP;
    blocks += slot;
    blocks->collisionCode = CELL_BEHAVIOR_WALL;
  }
}

struct Actor *Actor_FindAtCell(s32 *position, s32 checkY);

/// Returns the actor one cell, or failing that two cells, ahead of the leader
/// at the leader's height, or NULL.
struct Actor *Leader_FindActorAhead(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  FIXED position[3];
  // The leader's facing rounded to the nearest quarter turn, in sixteenths.
  s32 angle = (leader->facing / FACING_STEP + 2) & 0xc;
  struct Actor *found;

  position[0] = leader->x.fixed;
  position[1] = leader->y.fixed;
  position[2] = leader->z.fixed;
  Vector_AddPolarOffset(toFIXED(16), angle * FACING_STEP, position);
  found = Actor_FindAtCell(position, 1);
  if (found != NULL)
    return found;
  position[0] = leader->x.fixed;
  position[1] = leader->y.fixed;
  position[2] = leader->z.fixed;
  Vector_AddPolarOffset(toFIXED(32), angle * FACING_STEP, position);
  found = Actor_FindAtCell(position, 1);
  if (found != NULL)
    return found;
  return NULL;
}

/// Returns the first of the placed actors (not the treasure spots) on
/// position's cell, at its height too when checkY is set, or NULL.
struct Actor *Actor_FindAtCell(s32 *position, s32 checkY) {
  u32 i;

  for (i = ACTOR_FIRST_PLACED; i < FIELD_TREASURE_ACTOR; i++) {
    struct FieldWork *event = Ram_HeapSlots->fieldWork;
    struct Actor *actor = event->actors[i];

    if (checkY) {
      if (FIELD_CELL(position[0]) == FIELD_CELL(actor->x.fixed) &&
          position[1] / FIXED_ONE == actor->y.fixed / FIXED_ONE &&
          FIELD_CELL(position[2]) == FIELD_CELL(actor->z.fixed))
        return actor;
    } else {
      if (FIELD_CELL(position[0]) == FIELD_CELL(actor->x.fixed) &&
          FIELD_CELL(position[2]) == FIELD_CELL(actor->z.fixed))
        return actor;
    }
  }
  return NULL;
}

/// The width and height of each OAM shape and size, in pixels, by the shape
/// times 4 plus the size.
extern const u8 gOam_ShapeSizes[][2];
/// Where an attr01 word holds a sprite's size (attribute 1's top two bits) and
/// shape (attribute 0's).
#define OBJ_SIZE_SHIFT 30
#define OBJ_SHAPE_SHIFT 14

/// Fills an effect sprite: OAM attributes 0 and 1 as given (no affine matrix,
/// 256 colours), the tiles, unscaled and unrotated at depth 255, and the
/// picture's size from its shape, pivoted at (pivotX, pivotY).
void EffectSprite_Init(struct EffectSprite *sprite, s32 pivotX, s32 pivotY,
                       u32 attributes, s32 tile) {
  u32 *words;
  s32 shape;

  if (sprite == NULL)
    return;
  Iwram_FillWordsEntry(sprite, sizeof(*sprite), 0);
  sprite->scaleX = FIXED_ONE;
  sprite->scaleY = FIXED_ONE;
  sprite->depth = OAM_DEPTH_FRONT;
  sprite->pivotX = pivotX;
  sprite->pivotY = pivotY;
  shape =
      ((attributes >> OBJ_SIZE_SHIFT) | (attributes >> (OBJ_SHAPE_SHIFT - 2))) &
      0xf;
  sprite->width = gOam_ShapeSizes[shape][0];
  sprite->height = gOam_ShapeSizes[shape][1];
  words = (u32 *)&sprite->oam;
  *words++ = attributes;
  *words = 0;
  // Spelt through the OAM's address, as the two words above are written.
  (&sprite->oam)->fullColor = 1;
  sprite->oam.index = tile;
}

#include "system/affine.h"

/// An OAM entry's affine mode (attribute 0's two bits).
enum OamAffineMode {
  OAM_AFFINE_OFF = 0,    ///< drawn as it is
  OAM_AFFINE_NORMAL = 1, ///< through its matrix
  OAM_AFFINE_DOUBLE = 3  ///< through its matrix, in a box twice the size
};

/// Draws a field effect's sprite. An unscaled, unrotated sprite needs no
/// affine matrix; any other takes one, and one scaled past 1 is drawn in a
/// double-size box. The picture's pivot is turned and scaled with it, so the
/// pivot stays on the sprite's position. A sprite wholly off screen is not
/// queued; the rest join the OAM bucket of their depth.
void EffectSprite_Draw(struct EffectSprite *sprite) {
  s32 affineMode;
  s32 matrix;
  FIXED x = sprite->x;
  FIXED y = sprite->y;
  s32 scaleX = sprite->scaleX;
  s32 scaleY = sprite->scaleY;
  s32 width = sprite->width;
  s32 height = sprite->height;
  s32 left;
  s32 top;
  s32 screenX;
  s32 screenY;
  s32 magnitude;
  struct AffineTransform transform;

  if (sprite->depth == EFFECT_SPRITE_HIDDEN || scaleX == 0 || scaleY == 0)
    return;

  // The affine matrix, unless the sprite is drawn as it is.
  if (scaleX == FIXED_ONE && scaleY == scaleX && sprite->angle == 0) {
    matrix = 0;
    affineMode = OAM_AFFINE_OFF;
  } else {
    transform.angle = sprite->angle;
    transform.scaleX = scaleX >> 8;
    transform.scaleY = scaleY >> 8;
    matrix = OamMatrix_AllocFromTransform(&transform);
    affineMode = OAM_AFFINE_NORMAL;
  }

  // Grown past its own size, it needs the double-size box.
  magnitude = scaleX;
  if (magnitude < 0)
    magnitude = -magnitude;
  if (magnitude > FIXED_ONE ||
      (magnitude = scaleY < 0 ? -scaleY : scaleY, magnitude > FIXED_ONE)) {
    affineMode = OAM_AFFINE_DOUBLE;
    width *= 2;
    height *= 2;
  }

  // Where the pivot sits in the drawn box, from the box's top left.
  if (sprite->angle == 0) {
    if (scaleX == FIXED_ONE && scaleY == scaleX) {
      left = sprite->pivotX - (sprite->width >> 1) + width / 2;
      top = sprite->pivotY - (sprite->height >> 1) + height / 2;
    } else {
      left = FIXEDtoINT(scaleX * (sprite->pivotX - (sprite->width >> 1)) +
                        (FIXED_ONE - 1)) +
             width / 2;
      top = FIXEDtoINT(scaleY * (sprite->pivotY - (sprite->height >> 1)) +
                       (FIXED_ONE - 1)) +
            height / 2;
    }
  } else {
    FIXED sin = Trig_Sin(sprite->angle);
    FIXED cos = Trig_Cos(sprite->angle);
    s32 halfWidth = sprite->width * scaleX / 2;
    s32 halfHeight = sprite->height * scaleY / 2;
    FIXED cornerX = Iwram_MulFX(cos, halfWidth) - Iwram_MulFX(sin, halfWidth);
    FIXED cornerY = Iwram_MulFX(cos, halfHeight) + Iwram_MulFX(sin, halfHeight);
    s32 pivotX = scaleX * sprite->pivotX;
    s32 pivotY = sprite->pivotY * scaleY;
    s32 turnedX = Iwram_MulFX(cos, pivotX) - Iwram_MulFX(sin, pivotY);
    s32 turnedY = Iwram_MulFX(cos, pivotY) + Iwram_MulFX(sin, pivotX);

    left = width / 2 + FIXEDtoINT(turnedX - cornerX);
    top = height / 2 + FIXEDtoINT(turnedY - cornerY);
  }

  // The box on screen; skip it when none of it shows.
  screenX = FIXEDtoINT(x) - left;
  screenY = FIXEDtoINT(y) - top;
  if (screenX + width < 0 || screenX >= DISPLAY_WIDTH ||
      screenY + height <= 0 || screenY >= DISPLAY_HEIGHT)
    return;

  sprite->oam.x = screenX;
  sprite->oam.y = screenY;
  sprite->oam.affineMode = affineMode;
  sprite->oam.matrix = matrix;
  Oam_AddSpriteAtDepthClamped(sprite, sprite->depth);
}

/// Places sprite at the screen position of world position and draws it.
void EffectSprite_DrawAtWorldPosition(struct EffectSprite *sprite,
                                      s32 *position) {
  s32 screen[3];

  if (sprite != 0) {

    screen[0] = position[0];
    screen[1] = position[1];
    screen[2] = position[2];
    Camera_WorldToScreen(screen);
    sprite->x = screen[0];
    sprite->y = screen[2];
    EffectSprite_Draw(sprite);
  }
}

#undef HEIGHT_STEPS
#undef BLOCK_HEIGHT_STEPS
#undef OBJ_SIZE_SHIFT
#undef OBJ_SHAPE_SHIFT
