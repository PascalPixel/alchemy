/// @file
/// Stepping effect vectors, the canvas a field effect draws into, the
/// colour buffer fades and the field particles.
// Part of field_psynergy.c.

#include "field_psynergy_internal.h"
#include "fixed_math.h"
#include "system/dma.h"
#include "window/window.h"
#include "macros.h"
#include "field/field_sprite.h"

/// Moves a 2D point by its velocity, damped and pulled by gravity
/// (Field_AdvanceWithGravity2D).
void BattleEffect_StepVector2(s32 *vector, s32 damping, s32 gravity) {
  Field_AdvanceWithGravity2D(vector, damping, gravity);
}

/// Moves a 3D point by its velocity, damped and pulled by gravity
/// (Field_AdvanceWithGravity3D).
void BattleEffect_StepVector3(s32 *vector, s32 damping, s32 gravity) {
  Field_AdvanceWithGravity3D(vector, damping, gravity);
}

/// Plots a pixel of colour color at (x, y) on the 64-pixel canvas.
void BattleEffect_PlotHalfCanvasPixel(s32 x, s32 y, s32 color) {
  u16 *map = Ram_HeapSlots->canvasMap;
  u8 *canvas;

  canvas = Ram_HeapSlots->battleCanvas;
  canvas[map[(y << 6) + x]] = color;
}

/// Plots a pixel of colour color at (x, y) on the 128-pixel-wide canvas pair:
/// x from 64 lands on the second canvas.
void BattleEffect_PlotCanvasPixel(s32 x, s32 y, s32 color) {
  u16 *map = Ram_HeapSlots->canvasMap;
  u8 *canvas = Ram_HeapSlots->battleCanvas;

  if (x > 63) {
    canvas += CANVAS_SIZE;
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
  u8 *canvas = Ram_HeapSlots->battleCanvas;
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

/// Maps each drawn (nonzero) canvas pixel of buffer, count of them, through
/// gColorBuffer_DarkenOneLevelMap, one level darker down the ramp of colours
/// 0xe0 to 0xfe.
void ColorBuffer_DarkenOneLevel(u8 *buffer, s32 count) {
  if (count > 0) {
    const u8 *table = gColorBuffer_DarkenOneLevelMap;

    do {
      if (*buffer != 0)
        *buffer = table[*buffer & 31];
      count--;
      buffer++;
    } while (count != 0);
  }
}

/// Maps each drawn (nonzero) canvas pixel of buffer, count of them, through
/// gColorBuffer_DarkenTwoLevelsMap, two levels darker.
void ColorBuffer_DarkenTwoLevels(u8 *buffer, s32 count) {
  if (count > 0) {
    const u8 *table = gColorBuffer_DarkenTwoLevelsMap;

    do {
      if (*buffer != 0)
        *buffer = table[*buffer & 31];
      count--;
      buffer++;
    } while (count != 0);
  }
}

/// Maps each drawn (nonzero) canvas pixel of buffer, count of them, through
/// gColorBuffer_DarkenFourLevelsMap, four levels darker.
void ColorBuffer_DarkenFourLevels(u8 *buffer, s32 count) {
  if (count > 0) {
    const u8 *table = gColorBuffer_DarkenFourLevelsMap;

    do {
      if (*buffer != 0)
        *buffer = table[*buffer & 31];
      count--;
      buffer++;
    } while (count != 0);
  }
}

#define OBJ_TILES ((u8 *)OBJ_VRAM0)
#define BG_VRAM ((void *)BG_CHAR_BLOCK(0))
#define TILE_SIZE 32

/// The canvas in words, as one 32-bit DMA.
#define CANVAS_DMA DMA_COPY_WORDS(CANVAS_SIZE / 4)

/// The canvas's frame callback: counts the frame and carries out the command
/// the effect left for it, copying the canvas into the sprite's tiles (or to
/// the start of BG VRAM), and clearing or fading it after the copy. Case 2
/// comes before case 1: its body comes first in the ROM.
void BattleEffect_UploadCanvas(void) {
  struct CanvasSpriteWork *work = Ram_HeapSlots->canvasSprite;
  u8 *canvas = Ram_HeapSlots->battleCanvas;
  u8 *tiles = OBJ_TILES + work->tile * TILE_SIZE;

  work->frame++;
  switch (work->command) {
  case CANVAS_UPLOAD:
    Dma_Set(canvas, tiles, CANVAS_DMA, REG_DMA3);
    break;
  case CANVAS_UPLOAD_AND_CLEAR:
    Dma_Set(canvas, tiles, CANVAS_DMA, REG_DMA3);
    Iwram_FillWords(canvas, CANVAS_SIZE, 0);
    break;
  case CANVAS_UPLOAD_AND_DARKEN_ONE_LEVEL:
    Dma_Set(canvas, tiles, CANVAS_DMA, REG_DMA3);
    ColorBuffer_DarkenOneLevel(canvas, CANVAS_SIZE);
    break;
  case CANVAS_UPLOAD_AND_DARKEN_TWO_LEVELS:
    Dma_Set(canvas, tiles, CANVAS_DMA, REG_DMA3);
    ColorBuffer_DarkenTwoLevels(canvas, CANVAS_SIZE);
    break;
  case CANVAS_UPLOAD_AND_DARKEN_FOUR_LEVELS:
    Dma_Set(canvas, tiles, CANVAS_DMA, REG_DMA3);
    ColorBuffer_DarkenFourLevels(canvas, CANVAS_SIZE);
    break;
  case CANVAS_UPLOAD_TO_BG:
    Dma_Set(canvas, BG_VRAM, DMA_COPY_WORDS(CANVAS_SIZE * 2 / 4), REG_DMA3);
    break;
  }
  work->command = 0;
}

// OAM attributes 0 and 1 of a 64x64 square sprite.
#define OAM_SQUARE_64X64 0xc0000000

/// The canvas sprite's frame callback: places the sprite at the work's
/// position (whole pixels), scale and angle, and draws it. A sprite scaled to
/// nothing is not drawn.
void BattleEffect_DrawCanvasSprite(void) {
  struct CanvasSpriteWork *work = Ram_HeapSlots->canvasSprite;
  s32 x = toFIXED(work->x);
  s32 y = toFIXED(work->y);
  s32 scaleX = work->scaleX;
  s32 scaleY = work->scaleY;

  if (scaleX != 0 && scaleY != 0) {
    if (work->mode == CANVAS_SPRITE_16_COLORS) {
      EffectSprite_Init(&work->sprite, 32, 32, OAM_SQUARE_64X64, work->tile);
      work->sprite.oam.fullColor = 0;
      work->sprite.oam.palette = CANVAS_PALETTE;
      work->sprite.oam.priority = work->priority;
      work->sprite.depth = work->depth;
      work->sprite.x = x;
      work->sprite.y = y;
      work->sprite.scaleX = scaleX;
      work->sprite.scaleY = scaleY;
      work->sprite.angle = work->angle;
      EffectSprite_Draw(&work->sprite);
    }
    if (work->mode == CANVAS_SPRITE_256_COLORS) {
      EffectSprite_Init(&work->sprite, 32, 32, OAM_SQUARE_64X64, work->tile);
      work->sprite.oam.fullColor = 1;
      work->sprite.oam.palette = 0;
      work->sprite.oam.priority = work->priority;
      work->sprite.depth = work->depth;
      work->sprite.x = x;
      work->sprite.y = y;
      work->sprite.scaleX = scaleX;
      work->sprite.scaleY = scaleY;
      work->sprite.angle = work->angle;
      EffectSprite_Draw(&work->sprite);
    }
    if (work->mode == CANVAS_SPRITE_KEPT) {
      work->sprite.oam.priority = work->priority;
      work->sprite.depth = work->depth;
      work->sprite.x = x;
      work->sprite.y = y;
      work->sprite.scaleX = scaleX;
      work->sprite.scaleY = scaleY;
      work->sprite.angle = work->angle;
      EffectSprite_Draw(&work->sprite);
    }
  }
  work->drawnFrames++;
}

// The canvas sprite of the field Psynergy scenes.

/// Starts the canvas sprite of the field Psynergy scenes: its work, a cleared
/// 64x64 canvas copied into the sprite's tiles, and a map from each canvas
/// pixel (row by row) to its place in the tiles. BattleEffect_DrawCanvasSprite
/// draws the sprite and BattleEffect_UploadCanvas uploads the canvas each
/// frame; BattleEffect_EndCanvasSprite ends it.
void BattleEffect_StartCanvasSprite(void) {
  struct WindowWork *windowWork =
      (struct WindowWork *)Ram_HeapSlots->windowWork;
  struct CanvasSpriteWork *work =
      HeapSlot_Alloc(HEAP_SLOT_CANVAS_SPRITE, sizeof(*work));
  u8 *canvas = HeapSlot_Alloc(HEAP_SLOT_BATTLE_CANVAS, CANVAS_SIZE);
  u16 *map;
  s32 y;
  s32 x;

  IWRAM_FILL_WORDS(work, sizeof(*work), 0);
  work->tile = windowWork->glyphTiles;
  work->mode = CANVAS_SPRITE_16_COLORS;
  work->command = 0;
  work->x = 16;
  work->y = 0;
  work->scaleX = FIXED_ONE;
  work->scaleY = FIXED_ONE;
  work->priority = 1;
  work->depth = 250;
  Iwram_FillWordsEntry(canvas, CANVAS_SIZE, 0);
  Dma_Set(canvas, OBJ_TILES + work->tile * TILE_SIZE,
          DMA_COPY_WORDS(CANVAS_SIZE / 4), REG_DMA3);

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

  Task_Add(BattleEffect_DrawCanvasSprite, TASK_PASS_FRAME);
  Task_Add(BattleEffect_UploadCanvas, TASK_PASS_VBLANK - 20);
}

/// Ends the canvas sprite: stops its draw and upload callbacks, waits a
/// frame and frees its sprite work, canvas and pixel map.
void BattleEffect_EndCanvasSprite(void) {
  Task_Remove(BattleEffect_DrawCanvasSprite);
  Task_Remove(BattleEffect_UploadCanvas);
  wait(1);
  HeapSlot_Release(HEAP_SLOT_CANVAS_MAP);
  HeapSlot_Release(HEAP_SLOT_BATTLE_CANVAS);
  HeapSlot_Release(HEAP_SLOT_CANVAS_SPRITE);
}

/// The sparks that circle an actor as it vanishes.
#define VANISH_SPARK_COUNT 64

/// Actor_VanishInSparks's work (heap slot HEAP_SLOT_BATTLE_EFFECT): the spark
/// picture, and the sparks that circle the actor and their sprites.
struct VanishSparksWork {
  u8 picture[1024];
  struct WhirlSpark sparks[VANISH_SPARK_COUNT];
  struct EffectSprite sprites[VANISH_SPARK_COUNT];
};

#define SPARK_LIFE 50
#define PHASE_DONE 999

/// An object vanishes in a whirl of sparks: 64 sprites of picture 0x1ea circle
/// out from it, each spark rising and closing in for 50 frames. Ten frames in
/// the sparks stop coming and the object spins and widens from nothing back to
/// full width over 64 frames, when its sprite and its child flags come back;
/// 74 frames later it ends.
void Actor_VanishInSparks(s32 objectId) {
  struct VanishSparksWork *work =
      HeapSlot_Alloc(HEAP_SLOT_BATTLE_EFFECT, sizeof(struct VanishSparksWork));
  s32 phase;
  s32 entry;
  s32 tiles;
  s32 spawning;
  struct Actor *actor = Actor_Lookup(objectId);
  s32 depth = BattleEffect_GetObjectDepth(actor);
  s32 angle = (u16)actor->facing;
  struct WhirlSpark *spark;
  struct EffectSprite *sprite;
  s32 i;
  s32 next;
  s32 frame;
  s32 position[3];

  Decompress_TaggedLz(Resource_GetTableEntry(RESOURCE_SPARKLE_FADE), work);
  entry = VramCache_FindFreeEntry();
  tiles = VramCache_Load(entry, sizeof(work->picture), work);

  i = 0;
  spark = work->sparks;
  for (; i < VANISH_SPARK_COUNT; i++, spark++) {
    sprite = &work->sprites[i];
    EffectSprite_Init(sprite, 4, 4, 0, tiles + (i & 15) * 2);
    sprite->oam.fullColor = 1;
    sprite->oam.palette = 0;
    sprite->depth = BattleEffect_GetObjectDepth(actor);
    sprite->oam.priority = BattleEffect_GetObjectPriority(actor);
    spark->age = -1;
  }
  actor->scaleX = 0;
  actor->scaleY = FIXED_ONE;
  Actor_SetSpriteFlagsFar(actor, 0);
  spawning = 1;
  phase = 0;
  next = 0;
  frame = 0;
  do {
    if (spawning != 0) {
      for (i = spawning; i != 0; i--) {
        s32 sparkAngle = Random16();
        s32 radius = Random16() * 6 + toFIXED(12);

        spark = &work->sparks[next];

        spark->age = 0;
        spark->x = actor->x.fixed;
        spark->y = actor->y.fixed + toFIXED(20);
        spark->z = actor->z.fixed;
        Vector_AddPolarOffset(Random16(), sparkAngle, &spark->x);
        next++;
        spark->angle = sparkAngle;
        spark->radius = radius;
        next %= VANISH_SPARK_COUNT;
      }
    }
    spark = work->sparks;
    sprite = work->sprites;
    i = VANISH_SPARK_COUNT - 1;
    for (; i >= 0; i--, sprite++, spark++) {
      if (spark->age >= 0) {
        struct EffectSprite *drawn = sprite;
        /* FAKEMATCH(barrier): sprite copied to drawn through an empty asm. */
        asm("" : "=r"(drawn) : "0"(drawn));

        if ((u32)spark->age < SPARK_LIFE) {
          s32 *pos = position;
          s32 sparkAngle = spark->angle;
          FIXED radius = spark->radius;

          pos[0] = spark->x;
          pos[1] = spark->y;
          pos[2] = spark->z;
          Vector_AddPolarOffset(radius, sparkAngle, pos);
          if (pos[2] > actor->z.fixed)
            sprite->depth = depth + 2;
          else
            sprite->depth = depth - 2;
          Camera_WorldToScreen(pos);
          drawn->x = pos[0];
          drawn->y = pos[2];
          drawn->oam.index = tiles + (spark->age & 3) * 2;
          EffectSprite_Draw(drawn);
          spark->angle += TRIG_TURN / 32;
          spark->radius -= toFIXED(1) / 5;
          spark->y -= toFIXED(1) / 4;
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
        actor->facing -= FACING_STEP;
        actor->scaleX = frame << 10;
        if (actor->scaleX > FIXED_ONE)
          actor->scaleX = FIXED_ONE;
      }
      if (frame == 64) {
        actor->facing = angle;
        Actor_SetSpriteFlagsFar(actor, 1);
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
  HeapSlot_Release(HEAP_SLOT_BATTLE_EFFECT);
}

/// Updates one field effect sprite: counts its age, runs its callback (after
/// any delay), moves it and draws it.
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

/// Draws a field effect sprite at its screen position and scale, when it is
/// near the screen: x from -32 to 272 pixels and z from -32 to 224, both
/// exclusive.
void FieldEffect_DrawScaledActor(struct FieldParticle *object) {
  struct Sprite *sprite;
  s32 offset;
  FIXED scale[2];
  s32 position[4];

  sprite = object->actor;
  offset = 0;
  if (object->flags & FIELD_PARTICLE_LIFTED_BY_Z)
    offset = toFIXED(508) - object->z;
  scale[0] = Iwram_MulFX(object->scaleX, sprite->scale);
  scale[1] = Iwram_MulFX(object->scaleY, sprite->scale);
  SET_VECTOR(position, object->x, offset, object->z + offset);
  position[3] = 0;
  if (object->x > -toFIXED(32) && object->x < toFIXED(272) &&
      object->z > -toFIXED(32) && object->z < toFIXED(224))
    Render_ApplyProjectedPlacementFar(sprite, position, scale, 0);
}

/// Moves a field effect sprite toward its target, turning at most turnStepMax
/// a frame when turnStepLimited is set and speeding up by acceleration to
/// speedMax. One that stops at its target lands on it once within 8 pixels.
void FieldParticle_UpdateMotion(struct FieldParticle *effect) {
  s32 dx;
  s32 dz;
  FIXED distance;
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
    ix = dx / FIXED_ONE;
    iz = dz / FIXED_ONE;
    distance = toFIXED(Iwram_Sqrt(ix * ix + iz * iz));
    if (distance < toFIXED(128))
      distance = Math_SqrtFX(Iwram_MulFX(dx, dx) + Iwram_MulFX(dz, dz));
    if (distance <= toFIXED(8)) {
      FieldParticle_SetPosition(effect, effect->targetX, effect->targetZ);
      return;
    }
  }
  angle = Trig_Atan(dz, dx);
  if (effect->turnStepLimited != 0) {
    heading = effect->heading;
    turn = angle - heading;
    if ((turn >= 0 ? turn : -turn) >= effect->turnStepMax) {
      if (turn < 0) {
        if (-turn > effect->turnStepMax)
          turn = -effect->turnStepMax;
      } else if (turn > effect->turnStepMax) {
        turn = effect->turnStepMax;
      }
      angle = turn + heading;
    }
  }
  facing = (u16)angle;
  effect->heading = facing;
  speed = effect->speed + effect->acceleration;
  if (speed > effect->speedMax)
    speed = effect->speedMax;
  angle = (s16)facing;
  effect->speed = speed;
  effect->x += Iwram_MulFX(Trig_Cos(angle), speed);
  effect->z += Iwram_MulFX(Trig_Sin(angle), speed);
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

/// Places a field effect sprite at (x, z), at rest with no target.
void FieldParticle_SetPosition(struct FieldParticle *effect, s32 x, s32 z) {
  effect->targetX = EFFECT_NO_TARGET;
  effect->targetZ = EFFECT_NO_TARGET;
  effect->x = x;
  effect->z = z;
  effect->speed = 0;
}

/// Sets the animation of a field effect sprite.
void FieldParticle_SetActorMode(struct FieldParticle *effect, s32 mode) {
  Sprite_StartAnimationFar(effect->actor, mode);
}

/// Gives a field effect sprite its per-frame callback, starting at state 0.
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
  struct Sprite *animation;

  Dma_Fill32(0, effect, sizeof *effect);
  animation = Sprite_CreateFar(kind);
  effect->actor = animation;
  if (animation != NULL)
    animation->image.priority = 0;
  FieldParticle_SetPosition(effect, x, z);
  effect->scaleX = FIXED_ONE;
  effect->speedMax = toFIXED(2);
  effect->scaleY = FIXED_ONE;
  effect->acceleration = FIXED_ONE;
  effect->originX = x;
  effect->originZ = z;
  // BUG: Sprite_CreateFar can return NULL, but this and the animation
  // setup below still require a sprite. Callers also immediately use it, so
  // guarding this store alone would not establish allocation-failure recovery.
  {
    struct Sprite *sprite = effect->actor;
    sprite->flags = 0;
  }
  effect->stopAtTarget = 1;
  effect->turnStepLimited = 1;
  effect->updateMotion = 1;
  effect->render = 1;
  effect->active = 1;
  effect->randomValue = Random16();
  effect->flags = 4;
  FieldParticle_SetActorMode(effect, 1);
}

/// Frees a field effect sprite and its sprite, clearing it to zero (inactive).
void FieldEffect_ClearOwnedSlot(struct FieldParticle *effect) {
  if (effect->actor)
    Sprite_DestroyFar(effect->actor);
  Dma_Fill32(0, effect, sizeof *effect);
}
