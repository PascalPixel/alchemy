#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "module/field/event_commands.h"
#include "module/field/object.h"
#include "module/system/system.h"
#include "gamepak/far_call/field.h"

void BattleEffect_StepVector2(s32 *vector, s32 damping, s32 gravity) {
  BattleEffect_IntegrateVector2(vector, damping, gravity);
}

void BattleEffect_StepVector3(s32 *vector, s32 damping, s32 gravity) {
  BattleEffect_IntegrateVector3(vector, damping, gravity);
}

#include "ram_buffer.h"

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

#include "iwram_call.h"
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

      fill = Iwram_FillWordsEntry;
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
    Iwram_FillWordsEntry(work, sizeof(*work), 0);
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
  Iwram_FillWordsEntry(canvas, CANVAS_SIZE, 0);
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
#include "actor.h"
#include "effect_sprite.h"
#include "actor_view.h"

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
  s32 angle = (u16)object->facing;
  struct Spark *spark;
  struct EffectSprite *sprite;
  s32 i;
  s32 next;
  s32 frame;
  s32 position[3];

  Decompress_TaggedLz(Resource_GetTableEntry((s32)&ResourceId_SparkleFade),
                      work);
  entry = VramCache_FindFreeEntry();
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
        object->facing -= 0x1000;
        object->scaleX = frame << 10;
        if (object->scaleX > 0x10000)
          object->scaleX = 0x10000;
      }
      if (frame == 64) {
        object->facing = angle;
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
  VramCache_FreeEntry(entry);
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
        FieldEffect_DrawScaledActor(effect);
    }
  }
}

void FieldEffect_DrawScaledActor(struct FieldParticle *object) {
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

#include "fixed_math.h"

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

void FieldEffect_ClearOwnedSlot(struct FieldParticle *slot) {
  if (slot->object)
    ResourceObject_ReleaseFar((struct ResourceObjectWork *)slot->object);
  Dma_Fill32(0, slot, sizeof *slot);
}
