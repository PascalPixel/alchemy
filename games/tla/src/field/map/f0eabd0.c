#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "actor_view.h"

struct SwitchCell {
  u8 unk00[2];
  u8 block;
  u8 flags;
};

struct RestoreLayer {
  u8 unk00[0x138 - 0x108];
  struct SwitchCell *cells;
  u8 unk34[4];
};

struct RestoreMap {
  u8 unk000[0x108];
  struct RestoreLayer layers[3];
};

extern u8 gMapShapeGrid[];

struct Actor *Actor_GetById(u32 actor);
s32 Func_080eab70(struct Actor *object);
u32 Func_080202f0(s32 terrain, s32 x, s32 z);
#include "field.h"
s32 Func_080eab98(s32 unused, s32 key, s32 value);

void Func_080eabd0(u16 *actors) {
  struct RestoreMap *map = (struct RestoreMap *)Ram_HeapSlots->mapWork;
  struct SwitchCell *blocks = map->layers[2].cells;
  u8 *grid = gMapShapeGrid;
  struct Actor *leader;
  s32 height;

  for (; *actors != 0xffff; actors++) {
    struct Actor *object = Actor_GetById(*actors);
    u32 key = Func_080202f0(object->terrainId, object->x, object->z);
    s32 level =
        Map_GetTerrainHeightFar(object->terrainId, object->x, object->z) >> 19;
    s32 slot = Func_080eab70(object);
    struct SwitchCell *cells = map->layers[object->terrainId].cells;
    // The row's address as an integer sum, index first: &grid[row] adds the
    // grid first.
    u8 *shapes =
        (u8 *)((((s32)cells - (s32)Ram_MapCellBuffer) >> 2) + (s32)grid);
    struct SwitchCell *cell;
    s32 tag;
    u8 flags;

    object->motionFlags = 0;
    object->collisionFlags = 0;
    Actor_SetSingleChildFlagsFar(object, 0);
    shapes += slot;
    tag = Func_080eab98(object->terrainId, key, level + 4);
    cell = &cells[slot];
    flags = cell->flags;
    object->unk4c = flags;
    cell->flags = flags | 0x80;
    *shapes = tag;
    // Reused for the block in layer 2's table.
    cell = &blocks[slot];
    cell->block = 0xff;
  }
  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  height = Map_GetTerrainHeightFar(leader->terrainId, leader->x, leader->z);
  if (leader->y < height)
    leader->y = leader->terrainHeight = height;
}

// Only so the shape store has an alias set of its own; the grid holds bytes.
struct ShapeCell {
  u8 shape;
};

struct Actor *Func_080eaeb4(void);

void Func_080eace8(u16 *actors) {
  u8 *event = Ram_HeapSlots->eventWork;
  u8 *effect = Ram_HeapSlots->effectWork;
  struct Actor *target;

  if (effect != NULL && *(s16 *)(event + 358) != 0)
    target = *(struct Actor **)(effect + 20);
  else
    target = Func_080eaeb4();
  *(struct Actor **)(event + 0xcbc) = NULL;

  for (; *actors != 0xffff; actors++) {
    if (target == Actor_GetById(*actors)) {
      struct RestoreMap *map = (struct RestoreMap *)Ram_HeapSlots->mapWork;
      s32 zero = 0;
      u8 *grid = gMapShapeGrid;
      struct SwitchCell *cells = map->layers[target->terrainId].cells;
      struct SwitchCell *blocks = map->layers[2].cells;
      u8 *shapes = &grid[((s32)cells - (s32)Ram_MapCellBuffer) >> 2];
      u32 key = Func_080202f0(target->terrainId, target->x, target->z);
      s32 level =
          Map_GetTerrainHeightFar(target->terrainId, target->x, target->z);
      s32 slot = Func_080eab70(target);
      s32 tag;

      shapes += slot;
      tag = Func_080eab98(target->terrainId, key, (level >> 19) - 4);
      cells += slot;
      blocks += slot;
      ((struct ShapeCell *)shapes)->shape = tag;
      cells->flags = target->unk4c;
      blocks->block = zero;
      *(struct Actor **)(event + 0xcbc) = target;
    }
  }
}

#include "stddef.h"

// Only for the compiler: see the FAKEMATCH tag in Func_080eadfc.
static inline s32 ActorX(struct Actor *actor) { return actor->x; }

void Func_080eadfc(void) {
  struct Actor *target =
      *(struct Actor **)((u8 *)Ram_HeapSlots->eventWork + 0xcbc);

  if (target != NULL) {
    struct RestoreMap *map = (struct RestoreMap *)Ram_HeapSlots->mapWork;
    /* FAKEMATCH(temporary): the cell table read split into its byte offset
     * and the read, with the grid's address read between them; as
     * cells = map->layers[target->terrainId].cells, 35 of 84 instructions
     * differ in tla-en. */
    s32 offset = offsetof(struct RestoreMap, layers[target->terrainId].cells);
    u8 *grid = gMapShapeGrid;
    struct SwitchCell *cells = *(struct SwitchCell **)((u8 *)map + offset);
    struct SwitchCell *blocks = map->layers[2].cells;
    s32 index = ((s32)cells - (s32)Ram_MapCellBuffer) >> 2;
    u8 *shapes;
    u32 key;
    s32 level;
    s32 slot;
    s32 tag;
    u8 flags;

    /* FAKEMATCH(barrier): an empty asm on the grid index that claims to read
     * blocks, giving layer 2's table load a fourth dependent, so sched2 takes
     * it before the grid's pool load; without it, 2 of 84 instructions differ
     * in tla-en. */
    asm("" : "+l"(index) : "l"(blocks));
    shapes = &grid[index];
    key = Func_080202f0(target->terrainId, target->x, target->z);
    /* FAKEMATCH(wrapper): x read through ActorX for the height call; as
     * target->x, 12 of 84 instructions differ in tla-en. */
    level =
        Map_GetTerrainHeightFar(target->terrainId, ActorX(target), target->z);
    slot = Func_080eab70(target);
    shapes += slot;
    tag = Func_080eab98(target->terrainId, key, (level >> 19) + 4);
    ((struct ShapeCell *)shapes)->shape = tag;
    cells += slot;
    flags = cells->flags;
    target->unk4c = flags;
    cells->flags = flags | 0x80;
    blocks += slot;
    blocks->block = 0xff;
  }
}

void Vector_AddPolarOffset(s32 radius, s32 angle, s32 *position);
struct Actor *Func_080eaf28(s32 *position, s32 check_y);

struct Actor *Func_080eaeb4(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  s32 position[3];
  s32 angle = ((leader->angle >> 12) + 2) & 12;
  struct Actor *found;

  position[0] = leader->x;
  position[1] = leader->y;
  position[2] = leader->z;
  Vector_AddPolarOffset(0x100000, angle << 12, position);
  found = Func_080eaf28(position, 1);
  if (found != NULL)
    return found;
  position[0] = leader->x;
  position[1] = leader->y;
  position[2] = leader->z;
  Vector_AddPolarOffset(0x200000, angle << 12, position);
  found = Func_080eaf28(position, 1);
  if (found != NULL)
    return found;
  return NULL;
}

#include "fixed_math.h"

struct EventWork {
  u8 unk00[20];
  struct Actor *objects[64];
};

struct Actor *Func_080eaf28(s32 *position, s32 checkY) {
  u32 i;

  for (i = 8; i < 64; i++) {
    struct EventWork *event = Ram_HeapSlots->eventWork;
    struct Actor *object = event->objects[i];

    if (checkY) {
      if (position[0] >> 20 == object->x >> 20 &&
          position[1] / Q16_ONE == object->y / Q16_ONE &&
          position[2] >> 20 == object->z >> 20)
        return object;
    } else {
      if (position[0] >> 20 == object->x >> 20 &&
          position[2] >> 20 == object->z >> 20)
        return object;
    }
  }
  return NULL;
}

#include "iwram_call.h"
#include "effect_sprite.h"

/// The width and height of each OAM shape and size, in pixels.
extern const u8 gData_080f3bc4[][2];

/// Fills an effect sprite: OAM attributes 0 and 1 as given (no affine matrix,
/// 256 colours), the tiles, unscaled and unrotated at depth 255, and the
/// picture's size from its shape, pivoted at (pivotX, pivotY).
void Func_080eaf98(struct EffectSprite *sprite, s32 pivotX, s32 pivotY,
                   u32 attributes, s32 tile) {
  u32 *words;
  s32 shape;

  if (sprite == NULL)
    return;
  Iwram_FillWords(sprite, sizeof(*sprite), 0);
  sprite->scaleX = Q16_ONE;
  sprite->scaleY = Q16_ONE;
  sprite->depth = 255;
  sprite->pivotX = pivotX;
  sprite->pivotY = pivotY;
  shape = ((attributes >> 30) | (attributes >> 12)) & 15;
  sprite->width = gData_080f3bc4[shape][0];
  sprite->height = gData_080f3bc4[shape][1];
  words = (u32 *)&sprite->oam;
  *words++ = attributes;
  *words = 0;
  // Through the OAM's address: as sprite->oam.palette256, 2 of 57 differ.
  (&sprite->oam)->palette256 = 1;
  sprite->oam.index = tile;
}

#include <affine.h>

#define Q16_ONE 0x10000

void Func_080140d8(void *entry, s32 depth);

// Draws a field effect's sprite. An unscaled, unrotated sprite needs no
// affine matrix; any other takes one, and one scaled past 1 is drawn in a
// double-size box. The picture's pivot is turned and scaled with it, so the
// pivot stays on the sprite's position. A sprite wholly off screen is not
// queued; the rest join the OAM bucket of their depth.
void Func_080eb01c(struct EffectSprite *sprite) {
  s32 affineMode;
  s32 matrix;
  s32 x = sprite->x;
  s32 y = sprite->y;
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
  if (scaleX == Q16_ONE && scaleY == scaleX && sprite->angle == 0) {
    matrix = 0;
    affineMode = 0;
  } else {
    transform.angle = sprite->angle;
    transform.scaleX = scaleX >> 8;
    transform.scaleY = scaleY >> 8;
    matrix = AffineMatrix_BuildForEffect(&transform);
    affineMode = 1;
  }

  // Grown past its own size, it needs the double-size box.
  magnitude = scaleX;
  if (magnitude < 0)
    magnitude = -magnitude;
  if (magnitude > Q16_ONE ||
      (magnitude = scaleY < 0 ? -scaleY : scaleY, magnitude > Q16_ONE)) {
    affineMode = 3;
    width *= 2;
    height *= 2;
  }

  // Where the pivot sits in the drawn box, from the box's top left.
  if (sprite->angle == 0) {
    if (scaleX == Q16_ONE && scaleY == scaleX) {
      left = sprite->pivotX - (sprite->width >> 1) + width / 2;
      top = sprite->pivotY - (sprite->height >> 1) + height / 2;
    } else {
      left =
          ((scaleX * (sprite->pivotX - (sprite->width >> 1)) + 0xffff) >> 16) +
          width / 2;
      top =
          ((scaleY * (sprite->pivotY - (sprite->height >> 1)) + 0xffff) >> 16) +
          height / 2;
    }
  } else {
    s32 sin = Trig_Sin(sprite->angle);
    s32 cos = Trig_Cos(sprite->angle);
    s32 halfWidth = sprite->width * scaleX / 2;
    s32 halfHeight = sprite->height * scaleY / 2;
    s32 cornerX = Iwram_MulQ16(cos, halfWidth) - Iwram_MulQ16(sin, halfWidth);
    s32 cornerY = Iwram_MulQ16(cos, halfHeight) + Iwram_MulQ16(sin, halfHeight);
    s32 pivotX = scaleX * sprite->pivotX;
    s32 pivotY = sprite->pivotY * scaleY;
    s32 turnedX = Iwram_MulQ16(cos, pivotX) - Iwram_MulQ16(sin, pivotY);
    s32 turnedY = Iwram_MulQ16(cos, pivotY) + Iwram_MulQ16(sin, pivotX);

    left = width / 2 + ((turnedX - cornerX) >> 16);
    top = height / 2 + ((turnedY - cornerY) >> 16);
  }

  // The box on screen; skip it when none of it shows.
  screenX = (x >> 16) - left;
  screenY = (y >> 16) - top;
  if (screenX + width < 0 || screenX > 239 || screenY + height <= 0 ||
      screenY > 159)
    return;

  sprite->oam.x = screenX;
  sprite->oam.y = screenY;
  sprite->oam.affineMode = affineMode;
  sprite->oam.matrix = matrix;
  Func_080140d8(sprite, sprite->depth);
}

struct ScreenSprite {
  u8 unk00[0xc];
  s32 x;
  s32 y;
  u8 unk14[0x14];
};

void Camera_WorldToScreen(s32 *position);

/// Places sprite at the screen position of world position and draws it.
void EffectSprite_DrawAtWorldPosition(struct ScreenSprite *sprite,
                                      s32 *position) {
  s32 screen[3];

  if (sprite != 0) {
    s32 *p = screen;
    s32 *arg;

    p[0] = position[0];
    arg = p;
    p[1] = position[1];
    p[2] = position[2];
    Camera_WorldToScreen(arg);
    sprite->x = screen[0];
    sprite->y = screen[2];
    Func_080eb01c(sprite);
  }
}
