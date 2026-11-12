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

#include "battle.h"
#include "debug.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"

void BattleEffect_StepVector2(s32 *vector, s32 damping, s32 gravity) {
  BattleEffect_IntegrateVector2(vector, damping, gravity);
}

void BattleEffect_StepVector3(s32 *vector, s32 damping, s32 gravity) {
  BattleEffect_IntegrateVector3(vector, damping, gravity);
}

void BattleEffect_PlotHalfCanvasPixel(s32 x, s32 y, s32 color) {
  u16 *map = Ram_HeapSlots->canvasMap;
  u8 *canvas;

  canvas = Ram_HeapSlots->battleFxWork[1];
  canvas[map[(y << 6) + x]] = color;
}

void BattleEffect_PlotCanvasPixel(s32 x, s32 y, s32 color) {
  u16 *map = Ram_HeapSlots->canvasMap;
  u8 *canvas = Ram_HeapSlots->battleFxWork[1];

  if (x > 63) {
    canvas += 0x1000;
    x -= 64;
  }
  canvas[map[(y << 6) + x]] = color;
}

/// Draws a line of half canvas pixels from x0, y0 to x1, y1, Bresenham's
/// way: one pixel per step along the longer axis.
void BattleEffect_DrawHalfCanvasLine(s32 x0, s32 y0, s32 x1, s32 y1,
                                     s32 color) {
  s32 dx = x1 - x0;
  s32 dy;
  s32 sx;
  s32 sy;
  s32 err = 0;
  s32 x;
  s32 y;
  s32 i;

  if (dx < 0) {
    sx = -1;
    dx = -dx;
  } else {
    sx = 1;
  }
  dy = y1 - y0;
  if (dy < 0) {
    sy = -1;
    dy = -dy;
  } else {
    sy = 1;
  }
  x = x0;
  y = y0;
  if (dx > dy) {
    for (i = 0; i < dx; i++) {
      BattleEffect_PlotHalfCanvasPixel(x, y, color);
      err += dy;
      if (err >= dx) {
        err -= dx;
        y += sy;
      }
      x += sx;
    }
  } else {
    for (i = 0; i < dy; i++) {
      BattleEffect_PlotHalfCanvasPixel(x, y, color);
      err += dx;
      if (err >= dy) {
        err -= dy;
        x += sx;
      }
      y += sy;
    }
  }
}

/// Draws a circle of the given radius round (cx, cy) on the half canvas, by
/// the midpoint method: each step plots the eight symmetric points.
void BattleEffect_DrawHalfCanvasCircle(s32 cx, s32 cy, s32 radius, s32 color) {
  s32 x = radius;
  s32 y = 0;

  while (x >= y) {
    BattleEffect_PlotHalfCanvasPixel(cx + x, cy + y, color);
    BattleEffect_PlotHalfCanvasPixel(cx + x, cy - y, color);
    BattleEffect_PlotHalfCanvasPixel(cx - x, cy + y, color);
    BattleEffect_PlotHalfCanvasPixel(cx - x, cy - y, color);
    BattleEffect_PlotHalfCanvasPixel(cx + y, cy + x, color);
    BattleEffect_PlotHalfCanvasPixel(cx + y, cy - x, color);
    BattleEffect_PlotHalfCanvasPixel(cx - y, cy + x, color);
    BattleEffect_PlotHalfCanvasPixel(cx - y, cy - x, color);
    radius = radius - y * 2 + 1;
    if (radius < 0) {
      s32 next = x - 1;

      radius += next * 2;
      x = next;
    }
    y++;
  }
}

/// Copies a width by height block of colour indices onto the battle canvas
/// through its cell map at x, y, skipping zeros; nothing is drawn when the
/// block starts off the map or its far edge passes cell 63.
void BattleEffect_DrawCanvasBlock(const u8 *src, s32 x, s32 y, s32 width,
                                  s32 height) {
  u16 *map = Ram_HeapSlots->canvasMap;
  u8 *canvas = Ram_HeapSlots->battleFxWork[1];
  s32 row;
  s32 n;

  if (x < 0 || x + width > 63 || y < 0 || y + height > 63)
    return;
  for (row = 0; row < height; row++) {
    for (n = 0; n < width; n++) {
      if (*src != 0)
        canvas[map[((y + row) << 6) + x + n]] = *src;
      src++;
    }
  }
}

void ColorBuffer_MapToFadeStep1(u8 *buffer, s32 count) {
  if (count > 0) {
    const u8 *table = gColorBuffer_FadeStep1Map;

    do {
      if (*buffer != 0)
        *buffer = table[*buffer & 31];
      count--;
      buffer++;
    } while (count != 0);
  }
}

void ColorBuffer_MapToFadeStep2(u8 *buffer, s32 count) {
  if (count > 0) {
    const u8 *table = gColorBuffer_FadeStep2Map;

    do {
      if (*buffer != 0)
        *buffer = table[*buffer & 31];
      count--;
      buffer++;
    } while (count != 0);
  }
}

void ColorBuffer_MapToFadeStep3(u8 *buffer, s32 count) {
  if (count > 0) {
    const u8 *table = gColorBuffer_FadeStep3Map;

    do {
      if (*buffer != 0)
        *buffer = table[*buffer & 31];
      count--;
      buffer++;
    } while (count != 0);
  }
}

#include "dma.h"
#include "io_reg.h"
#include "canvas_sprite.h"

#define OBJ_TILES ((u8 *)0x06010000)
#define BG_VRAM ((void *)0x06000000)
#define TILE_SIZE 32

/// The canvas in words, as one 32-bit DMA.
#define CANVAS_DMA DMA_COPY_WORDS(CANVAS_SIZE / 4)

/// The canvas's frame callback: counts the frame and carries out the command
/// the effect left for it, copying the canvas into the sprite's tiles (or to
/// the start of BG VRAM), and clearing or fading it after the copy. Case 2
/// comes before case 1: its body comes first in the ROM.
void Func_080eb594(void) {
  struct CanvasSpriteWork *work = Ram_HeapSlots->unkec[1];
  u8 *canvas = Ram_HeapSlots->battleFxWork[1];
  u8 *tiles = OBJ_TILES + work->tile * TILE_SIZE;

  work->frame++;
  switch (work->command) {
  case CANVAS_UPLOAD:
    Dma_Set(canvas, tiles, CANVAS_DMA, REG_DMA3);
    break;
  case CANVAS_UPLOAD_AND_CLEAR:
    Dma_Set(canvas, tiles, CANVAS_DMA, REG_DMA3);
    {
      s32 size = 128;
      /* FAKEMATCH(pin): the canvas pinned to the r0 it is passed in, so its
       * copy there comes before the size's shift; as a plain u8 *buffer, 2
       * of 98 instructions differ in tla-en. */
      register u8 *buffer asm("r0");
      s32 (*fill)(void *, s32, u32);

      fill = Iwram_FillWords;
      buffer = canvas;
      fill(buffer, size << 5, 0);
    }
    break;
  case CANVAS_UPLOAD_AND_FADE1:
    Dma_Set(canvas, tiles, CANVAS_DMA, REG_DMA3);
    ColorBuffer_MapToFadeStep1(canvas, CANVAS_SIZE);
    break;
  case CANVAS_UPLOAD_AND_FADE2:
    Dma_Set(canvas, tiles, CANVAS_DMA, REG_DMA3);
    ColorBuffer_MapToFadeStep2(canvas, CANVAS_SIZE);
    break;
  case CANVAS_UPLOAD_AND_FADE3:
    Dma_Set(canvas, tiles, CANVAS_DMA, REG_DMA3);
    ColorBuffer_MapToFadeStep3(canvas, CANVAS_SIZE);
    break;
  case CANVAS_UPLOAD_TO_BG:
    Dma_Set(canvas, BG_VRAM, DMA_COPY_WORDS(CANVAS_SIZE * 2 / 4), REG_DMA3);
    break;
  }
  work->command = 0;
}

// OAM attributes 0 and 1 of a 64x64 square sprite.
#define OAM_SQUARE_64X64 0xc0000000

// The canvas sprite's frame callback: places the sprite at the work's
// position (whole pixels), scale and angle, and draws it. A sprite scaled to
// nothing is not drawn.
void Func_080eb6a0(void) {
  struct CanvasSpriteWork *work = Ram_HeapSlots->unkec[1];
  s32 x = work->x << 16;
  s32 y = work->y << 16;
  s32 scaleX = work->scaleX;
  s32 scaleY = work->scaleY;

  if (scaleX != 0 && scaleY != 0) {
    if (work->mode == CANVAS_SPRITE_16_COLORS) {
      Func_080eaf98(&work->sprite, 32, 32, OAM_SQUARE_64X64, work->tile);
      work->sprite.oam.palette256 = 0;
      work->sprite.oam.palette = 14;
      work->sprite.oam.priority = work->priority;
      work->sprite.depth = work->depth;
      work->sprite.x = x;
      work->sprite.y = y;
      work->sprite.scaleX = scaleX;
      work->sprite.scaleY = scaleY;
      work->sprite.angle = work->angle;
      Func_080eb01c(&work->sprite);
    }
    if (work->mode == CANVAS_SPRITE_256_COLORS) {
      Func_080eaf98(&work->sprite, 32, 32, OAM_SQUARE_64X64, work->tile);
      work->sprite.oam.palette256 = 1;
      work->sprite.oam.palette = 0;
      work->sprite.oam.priority = work->priority;
      work->sprite.depth = work->depth;
      work->sprite.x = x;
      work->sprite.y = y;
      work->sprite.scaleX = scaleX;
      work->sprite.scaleY = scaleY;
      work->sprite.angle = work->angle;
      Func_080eb01c(&work->sprite);
    }
    if (work->mode == CANVAS_SPRITE_KEPT) {
      work->sprite.oam.priority = work->priority;
      work->sprite.depth = work->depth;
      work->sprite.x = x;
      work->sprite.y = y;
      work->sprite.scaleX = scaleX;
      work->sprite.scaleY = scaleY;
      work->sprite.angle = work->angle;
      Func_080eb01c(&work->sprite);
    }
  }
  work->drawnFrames++;
}

// The canvas sprite of the field Psynergy scenes.

#include "heap_release.h"
#include "task.h"
#include "window.h"

/// Starts the canvas sprite of the field Psynergy scenes: its work, a cleared
/// 64x64 canvas copied into the sprite's tiles, and a map from each canvas
/// pixel (row by row) to its place in the tiles. Func_080eb6a0 draws the
/// sprite and Func_080eb594 uploads the canvas each frame;
/// BattleEffect_EndCanvasSprite ends it.
void Func_080eb824(void) {
  struct WindowWork *windowWork =
      (struct WindowWork *)Ram_HeapSlots->windowTiles;
  struct CanvasSpriteWork *work =
      HeapSlot_Alloc(HEAP_SLOT_CANVAS_SPRITE, sizeof(*work));
  u8 *canvas = HeapSlot_Alloc(HEAP_SLOT_CANVAS, CANVAS_SIZE);
  u16 *map;
  s32 y;
  s32 x;

  do {
    Iwram_FillWords(work, sizeof(*work), 0);
  } while (0);
  work->tile = windowWork->glyphTiles;
  work->mode = CANVAS_SPRITE_16_COLORS;
  work->command = 0;
  work->x = 16;
  work->y = 0;
  work->scaleX = 0x10000;
  work->scaleY = 0x10000;
  work->priority = 1;
  work->depth = 250;
  Iwram_FillWords(canvas, CANVAS_SIZE, 0);
  Dma_Set(canvas, OBJ_TILES + work->tile * TILE_SIZE,
          0x84000000 | CANVAS_SIZE / 4, REG_DMA3);

  // Where each pixel of the canvas sits in its 8x8 tiles.
  map = HeapSlot_AllocEwram(HEAP_SLOT_CANVAS_MAP,
                            CANVAS_WIDTH * CANVAS_WIDTH * sizeof(u16));
  for (y = 0; y < CANVAS_WIDTH; y++) {
    for (x = 0; x < CANVAS_WIDTH; x++) {
      s32 row = y & 7;
      s32 column = x & 7;
      *map++ = ((((y >> 3) * 8 + (x >> 3)) * 8) + row) * 8 + column;
    }
  }

  Task_Add(Func_080eb6a0, TASK_PASS_VBLANK);
  Task_Add(Func_080eb594, TASK_PASS_FRAME - 20);
}

/// Ends the canvas sprite: stops its draw and upload callbacks, waits a
/// frame and frees its sprite work, canvas and pixel map.
void BattleEffect_EndCanvasSprite(void) {
  Task_Remove(Func_080eb6a0);
  Task_Remove(Func_080eb594);
  wait(1);
  HeapSlot_Release(180);
  HeapSlot_Release(96);
  HeapSlot_Release(240);
}

#include "resource.h"

// One spark: where it circles round, its angle and radius, and its age in
// frames (-1: unused).
struct Spark {
  s32 centre[3];
  s32 angle;
  s32 unk10;
  s32 radius;
  s32 age;
};

// The heap block of the effect: the picture, then the sparks and their
// sprites.
struct SparkWork {
  u8 picture[0x400];
  struct Spark sparks[64];
  struct EffectSprite sprites[64];
};

#define HEAP_SLOT_SPARKS 92
#define SPARK_LIFE 50
#define PHASE_DONE 999

s32 BattleEffect_GetObjectDepth(struct Actor *object);
s32 BattleEffect_GetObjectPriority(struct Actor *object);

extern char ResourceId_SparkleFade;
/// An object vanishes in a whirl of sparks: 64 sprites of picture 0x1ea circle
/// out from it, each spark rising and closing in for 50 frames. Ten frames in
/// the sparks stop coming and the object spins and widens from nothing back to
/// full width over 64 frames, when its sprite and its child flags come back;
/// 74 frames later it ends.
void Func_080eb960(s32 objectId) {
  struct SparkWork *work =
      HeapSlot_Alloc(HEAP_SLOT_SPARKS, sizeof(struct SparkWork));
  s32 phase;
  s32 entry;
  s32 tiles;
  s32 spawning;
  struct Actor *object = ObjectTable_Get(objectId);
  s32 depth = BattleEffect_GetObjectDepth(object);
  s32 angle = (u16)object->angle;
  struct Spark *spark;
  struct EffectSprite *sprite;
  s32 i;
  s32 next;
  s32 frame;
  s32 position[3];

  Resource_DecodeType01(Resource_GetTableEntry((s32)&ResourceId_SparkleFade),
                        work);
  entry = Resource_FindFreeEntry();
  tiles = VramCache_Load(entry, sizeof(work->picture), work);

  i = 0;
  spark = work->sparks;
  for (; i < 64; i++, spark++) {
    sprite = &work->sprites[i];
    Func_080eaf98(sprite, 4, 4, 0, tiles + (i & 15) * 2);
    sprite->oam.palette256 = 1;
    sprite->oam.palette = 0;
    sprite->depth = BattleEffect_GetObjectDepth(object);
    sprite->oam.priority = BattleEffect_GetObjectPriority(object);
    spark->age = -1;
  }
  object->scaleX = 0;
  object->scaleY = 0x10000;
  Actor_SetSingleChildFlagsFar(object, 0);
  spawning = 1;
  phase = 0;
  next = 0;
  frame = 0;
  do {
    if (spawning != 0) {
      for (i = spawning; i != 0; i--) {
        s32 sparkAngle = Random16();
        s32 radius = Random16() * 6 + 0xc0000;

        spark = &work->sparks[next];

        spark->age = 0;
        spark->centre[0] = object->x;
        spark->centre[1] = object->y + 0x140000;
        spark->centre[2] = object->z;
        Vector_AddPolarOffset(Random16(), sparkAngle, spark->centre);
        next++;
        spark->angle = sparkAngle;
        spark->radius = radius;
        next %= 64;
      }
    }
    spark = work->sparks;
    sprite = work->sprites;
    i = 63;
    for (; i >= 0; i--, sprite++, spark++) {
      if (spark->age >= 0) {
        struct EffectSprite *drawn = sprite;
        /* FAKEMATCH(barrier): sprite copied to drawn through an empty asm,
           which keeps the two in registers of their own; as sprite itself (or
           the plain copy), 83 of 325 instructions differ in tla-en. */
        asm("" : "=r"(drawn) : "0"(drawn));

        if ((u32)spark->age < SPARK_LIFE) {
          s32 *pos = position;
          s32 sparkAngle = spark->angle;
          s32 radius = spark->radius;

          pos[0] = spark->centre[0];
          pos[1] = spark->centre[1];
          pos[2] = spark->centre[2];
          Vector_AddPolarOffset(radius, sparkAngle, pos);
          if (pos[2] > object->z)
            sprite->depth = depth + 2;
          else
            sprite->depth = depth - 2;
          Camera_WorldToScreen(pos);
          drawn->x = pos[0];
          drawn->y = pos[2];
          drawn->oam.index = tiles + (spark->age & 3) * 2;
          Func_080eb01c(drawn);
          spark->angle += 0x800;
          spark->radius -= 0x3333;
          spark->centre[1] -= 0x4000;
        }
        if (++spark->age == SPARK_LIFE)
          spark->age = -1;
      }
    }
    switch (phase) {
    case 0:
      if (frame == 10) {
        phase = 1;
        frame = -1;
      }
      break;
    case 1:
      if (frame < 64) {
        object->angle -= 0x1000;
        object->scaleX = frame << 10;
        if (object->scaleX > 0x10000)
          object->scaleX = 0x10000;
      }
      if (frame == 64) {
        object->angle = angle;
        Actor_SetSingleChildFlagsFar(object, 1);
      }
      if (frame == 10)
        spawning = 0;
      if (frame == 74)
        phase = PHASE_DONE;
      break;
    }
    wait(1);
    frame++;
  } while (phase != PHASE_DONE);
  Resource_ResetEntry(entry);
  HeapSlot_Release(HEAP_SLOT_SPARKS);
}

#include "field_particle.h"
#include "macros.h"

void FieldParticle_Update(struct FieldParticle *effect) {
  if (effect->active != 0) {
    effect->age++;
    if (effect->callbackDelay != 0)
      effect->callbackDelay--;
    else if (effect->callback != 0)
      effect->callback(effect);
    if (effect->active != 0) {
      if (effect->updateMotion != 0)
        FieldParticle_UpdateMotion(effect);
      if (effect->render != 0)
        BattleEffect_DrawScaledObject(effect);
    }
  }
}

struct ScaledSprite {
  u8 unk00[0xc];
  s32 scale;
};

void BattleEffect_DrawScaledObject(struct FieldParticle *object) {
  struct ScaledSprite *sprite;
  s32 offset;
  s32 scale[2];
  s32 position[4];

  sprite = object->object;
  offset = 0;
  if (object->flags & 4)
    offset = 0x1fc0000 - object->z;
  scale[0] = Iwram_MulQ16(object->scaleX, sprite->scale);
  scale[1] = Iwram_MulQ16(object->scaleY, sprite->scale);
  SET_VECTOR(position, object->x, offset, object->z + offset);
  position[3] = 0;
  if (object->x > -0x200000 && object->x < 0x1100000 && object->z > -0x200000 &&
      object->z < 0xe00000)
    Render_ApplyProjectedPlacementFar(sprite, position, scale, 0);
}

void FieldParticle_UpdateMotion(struct FieldParticle *effect) {
  s32 dx;
  s32 dz;
  s32 distance;
  s32 ix;
  s32 iz;
  s32 speed;
  s16 angle;
  s32 heading;
  s32 facing;
  s16 turn;

  if (effect->targetX == EFFECT_NO_TARGET)
    return;
  dx = effect->targetX - effect->x;
  dz = effect->targetZ - effect->z;
  if (effect->stopAtTarget != 0) {
    ix = dx / Q16_ONE;
    iz = dz / Q16_ONE;
    distance = Iwram_Sqrt(ix * ix + iz * iz) << 16;
    if (distance < 0x800000)
      distance = FixedSqrt(Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dz, dz));
    if (distance <= 0x80000) {
      FieldParticle_SetPosition(effect, effect->targetX, effect->targetZ);
      return;
    }
  }
  angle = ArcTan2(dz, dx);
  if (effect->limitTurnStep != 0) {
    heading = effect->heading;
    turn = angle - heading;
    if ((turn >= 0 ? turn : -turn) >= effect->maxTurnStep) {
      if (turn < 0) {
        if (-turn > effect->maxTurnStep)
          turn = -effect->maxTurnStep;
      } else if (turn > effect->maxTurnStep) {
        turn = effect->maxTurnStep;
      }
      angle = turn + heading;
    }
  }
  facing = (u16)angle;
  effect->heading = facing;
  speed = effect->speed + effect->acceleration;
  if (speed > effect->maxSpeed)
    speed = effect->maxSpeed;
  angle = (s16)facing;
  effect->speed = speed;
  effect->x += Iwram_MulQ16(Trig_Cos(angle), speed);
  effect->z += Iwram_MulQ16(Trig_Sin(angle), speed);
}

/// Reports an unfinished target only for effects that stop on arrival.
/// A plain != adds two bytes to the English function.
u32 FieldParticle_HasPendingTarget(struct FieldParticle *effect) {
  u32 value;

  if (effect->stopAtTarget == 0) {
    return 0;
  }
  value = (u32)effect->targetX ^ (u32)EFFECT_NO_TARGET;
  return ((0u - value) | value) >> 31;
}

#include "field_sprite.h"

void FieldParticle_SetPosition(struct FieldParticle *effect, s32 x, s32 z) {
  effect->targetX = EFFECT_NO_TARGET;
  effect->targetZ = EFFECT_NO_TARGET;
  effect->x = x;
  effect->z = z;
  effect->speed = 0;
}

void Animation_ApplyChildArgumentFar(void *, s32);

void FieldParticle_SetActorMode(struct FieldParticle *effect, s32 mode) {
  Animation_ApplyChildArgumentFar(effect->object, mode);
}

void FieldParticle_SetCallback(struct FieldParticle *effect,
                               EffectCallback callback) {
  effect->callback = callback;
  effect->callbackDelay = 0;
  effect->age = 0;
  effect->state = 0;
}

/// Starts slot effect as a particle of resource object kind at (x, z): clears
/// it, creates its sprite in front, and sets it moving at up to 2 pixels a
/// frame toward its target at full size, turning a limited step at a time and
/// stopping on arrival, with a random value and z lifting the sprite.
void FieldParticle_Initialize(struct FieldParticle *effect, s32 kind, s32 x,
                              s32 z) {
  s32 unit;
  struct FieldSprite *object;
  volatile u32 zero;

  Dma_Fill32Cell(zero, 0, effect, sizeof *effect);
  object = (struct FieldSprite *)ResourceObject_CreateFar(kind);
  effect->object = object;
  if (object != NULL)
    object->priority = 0;
  FieldParticle_SetPosition(effect, x, z);
  unit = 0x10000;
  effect->maxSpeed = 0x20000;
  effect->acceleration = effect->scaleY = effect->scaleX = unit;
  effect->originX = x;
  effect->originZ = z;
  // The sprite flags (nextFrame), through a byte pointer: as a field of
  // struct FieldSprite the origin stores move after it.
  ((s8 *)effect->object)[26] = 0;
  effect->stopAtTarget = 1;
  effect->limitTurnStep = 1;
  effect->updateMotion = 1;
  effect->render = 1;
  effect->active = 1;
  effect->randomValue = Random16();
  effect->flags = 4;
  FieldParticle_SetActorMode(effect, 1);
}

void BattleEffect_ClearOwnedSlot(struct FieldParticle *slot) {
  if (slot->object)
    ResourceObject_ReleaseFar((struct ResourceObjectWork *)slot->object);
  Dma_Fill32(0, slot, sizeof *slot);
}
