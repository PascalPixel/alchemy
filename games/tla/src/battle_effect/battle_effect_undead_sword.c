/// @file
/// Undead Sword, Plume Edge and Diamond Berg, and the bodies of Dreamtide and
/// Watery Grave and of Waft and Aroma.
// Part of battle_effect.c.

#include "battle_effect_internal.h"
#include "sounds.h"

/// The water sheet's modes (BattleEffect_RunWaterSheet).
enum WaterSheetMode { WATER_SHEET_DREAMTIDE, WATER_SHEET_WATERY_GRAVE };

/// The wind funnel's modes (BattleEffect_RunWindFunnel).
enum WindFunnelMode { WIND_FUNNEL_WAFT, WIND_FUNNEL_AROMA };

#define SPARK_COUNT 16
#define SPARK_LIFE 28
#define BLAST_SHEET_OFFSET 0x460

#define BURST_FRAMES 54
#define BURST_HIT_FRAME 10
#define COLUMN_RISE_FRAME 8 ///< the columns break the ground
#define COLUMN_PEAK_FRAME 31
#define COLUMN_MAX_HEIGHT 80

/// Two mirrored columns of light rise at the first target's feet and sink
/// again, scattering sparks that drift upward.
///
/// Abilities: Undead Sword.
void BattleEffect_RunUndeadSword(struct BattleEffectParams *args) {
  struct BattleEffectWork *work;
  void *canvas;
  void *sprites;
  DrawRectangle draw[2];
  struct ScreenPoint target;
  struct Particle *spark;
  struct Particle *particle;
  s32 frame;
  s32 i;
  s32 height;
  s32 mode;
  s32 x;
  u8 *picture;
  s32 speed;
  s32 angle;
  s32 cell;
  s32 size;

  work = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  sprites = Ram_HeapSlots->battleSheet;

  // Open the canvas and load the pictures.
  BattleEffect_StartCanvasLayer(0);
  {
    REG_BG2PA = Q_8_8(1);
  }
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sprites, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_LimeColumnSheet, work->sheet, 1,
                             1);
  Resource_LoadAndDecompress((s32)&ResourceId_BlastSheet,
                             work->sheet + BLAST_SHEET_OFFSET, 1, 0);
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);

  // Slide the canvas so that its x = 64 is under the target.
  EffectPosition_ProjectBattler(args->targets[0], &target);
  REG_BG2X = (64 - target.x) << 8;

  // The sparks start together at the columns' foot and fly apart, upward
  // faster than sideways.
  for (i = 0, spark = work->particles; i != SPARK_COUNT; i++) {
    speed = (Random16() & 0x1ff) + 128;
    angle = Random16() & 0xffff;
    spark->x = px(64);
    spark->y = px(112);
    spark->velocityX = (Trig_Sin(angle) * speed) >> 8;
    spark->velocityY = (Trig_Cos(angle) * speed) >> 9;
    spark->variant = Random16() & 7;
    spark++;
  }
  work->shakeFrames = 8;

  for (frame = 0; frame != BURST_FRAMES; frame++) {
    // The hit: the damage shown, a shake, knock-back and a flash.
    if (frame == BURST_HIT_FRAME) {
      work->shakeFrames = 8;
      EFFECT_SHOW_DAMAGE(SE_POWER_UP);
      BattleMotion_ApplyKnockbackFar(args->targets[0], KNOCKBACK_NUDGE);
      BattleEffect_TintBattler(args->targets[0], SPRITE_COLOR_WHITE,
                               POSE_DAMAGE, 0, 8);
    }

    // The columns: up 12 pixels a frame until frame 31, then down 6.
    if (frame >= COLUMN_RISE_FRAME) {
      if (frame <= COLUMN_PEAK_FRAME)
        height = frame * 12 - 96;
      else
        height = 272 - frame * 6;
      if (height > 0) {
        if (height > COLUMN_MAX_HEIGHT) {
          height = COLUMN_MAX_HEIGHT;
          mode = BLITTER_MAX;
        } else {
          mode = BLITTER_ADDITIVE;
        }
        for (i = 0, x = 50; i != 2; i++, x += 14) {
          if (i == 0)
            BattleEffect_BuildBlitter(HEAP_SLOT_RAM_CODE, 7, 7,
                                      COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y,
                                      mode);
          else
            BattleEffect_BuildBlitter(
                HEAP_SLOT_RAM_CODE, 7, 7,
                COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y | COMPOSITE_FLIP_X, mode);
          (draw[0] = (DrawRectangle)Ram_HeapSlots->ramCode)(
              canvas, work->sheet, x, 112 - height, 14, height);
          HeapSlot_Release(HEAP_SLOT_RAM_CODE);
        }
      }
    }

    // The sparks: a new blast picture every third frame, seven in all; the
    // pairs leave one frame apart from frame 8.
    BattleEffect_FetchRectangleBlitters(args->side, draw);
    for (i = 0, particle = work->particles; i != SPARK_COUNT; i++) {
      if (frame >= i / 2 + 8 && particle->variant <= SPARK_LIFE) {
        s32 px;
        s32 py;

        cell = particle->variant / 3;
        px = Q_16_16_UPPER_HALF(particle->x);
        py = Q_16_16_UPPER_HALF(particle->y);
        if (cell > 6)
          cell = 6;
        picture =
            work->sheet + gUndeadSword_SparkPictures[cell] + BLAST_SHEET_OFFSET;
        size = gUndeadSword_SparkSizes[cell];
        draw[0](canvas, picture, px - size / 2, py - size / 2, size, size);
        particle->variant++;
        BattleEffectKernels_IntegrateVector2(particle, 62, -px(1) / 8);
      }
      particle++;
    }
    EFFECT_RELEASE_BLITTERS();

    if (frame < COLUMN_RISE_FRAME)
      BattleCamera_Shake(4, 4);
    else
      BattleCamera_Shake(16, 16);
    EFFECT_END_FRAME(work);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}

/// Plume Edge: the user dashes at the target, which a fan of white flashes
/// throws back; a pillar of light scrolls up through it for half a second and
/// then bursts into 256 sparks with a white flash of the whole canvas.
///
/// Setup: canvas layer 0 at half blend, the two rectangle blitters, the
/// pillar and flash sheets in the work sheet, the spark sheet in the shared
/// sheet and the light palette over the background's.
/// Frame loop (80 frames): the fan of 64 flashes at frame 0, the user's dash
/// at frames 18 and 21, the spark burst at frame 32, then the sparks fall.
///
/// Abilities: Plume Edge.
void BattleEffect_RunPlumeEdge(struct BattleEffectParams *args) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  u8 *sheet;
  struct MotionObject *target;
  s32 pillarY;
  DrawRectangle draw[2];
  struct ScreenPoint position;
  struct Particle *step;
  s32 i;

  work = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  sheet = Ram_HeapSlots->battleSheet;
  BattleEffect_StartCanvasLayer(0);
  REG_BLDALPHA = BLEND_ALPHA(16, 16);
  BattleEffect_FetchRectangleBlitters(0, draw);
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
  target = BattleObject_GetFar(args->targets[0])->object;
  pillarY = 0;
  Resource_LoadAndDecompress((s32)&ResourceId_Data0DA, work->sheet + 0x4710, 0,
                             0);
  Iwram_CopyWordsEntry(
      (void *)BG_PLTT,
      Resource_GetTableEntry((s32)&ResourceId_MercuryDjinnSmallSheet), 128);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_FirePillarSheetA, work->sheet, 1,
                             0);
  Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet,
                             work->sheet + 0x2000, 1, 0);
  for (i = 0; i != 1024; i++)
    PARTICLES[i].variant = -1;

  for (frame = 0; frame != 80; frame++) {
    EffectPosition_ProjectBattler(args->targets[0], &position);

    // Frame 0: 64 flashes fan out upward from the target, one more each
    // frame, and the target turns white and stops bouncing.
    if (frame == 0) {
      for (i = 0; i != 64; i++) {
        s32 speed = (Random16() & 0x3ff) + 32;
        s32 angle = (Random16() & 0x7fff) - TRIG_TURN / 4;

        step = &work->particles[i];
        step->x = px(position.x / 2);
        step->y = px(position.y);
        step->velocityX = (Trig_Sin(angle) * speed) >> 8;
        step->velocityY = -(Trig_Cos(angle) * speed * 2) >> 8;
        step->variant = -i;
      }
      target->verticalMotionStrength = 0;
      target->y = 0;
      pillarY = position.y;
      BattleEffect_TintBattler(args->targets[0], TINT_KEEP, POSE_DAMAGE, 0, -1);
      work->shakeFrames = 4;
      Sound_PlayFar(SE_CLATTER);
    }

    // Frame 32: the pillar bursts into 256 sparks, the target is knocked
    // back and flashes, and the whole canvas flashes white.
    if (frame == 32) {
      for (i = 0; i != 256; i++) {
        s32 speed = (Random16() & 0x3ff) + 32;
        s32 angle = Random16();

        step = &PARTICLES[256 + i];
        step->x = px(position.x / 2);
        step->y = px(position.y - 16);
        step->velocityX = (Trig_Sin(angle) * speed) >> 7;
        step->velocityY = -(Trig_Cos(angle) * speed * 2) >> 7;
        step->variant = (Random16() & 7) + 48;
      }
      BattleMotion_ApplyKnockbackFar(args->targets[0], KNOCKBACK_HURL);
      BattleEffect_TintBattler(args->targets[0], SPRITE_COLOR_WHITE,
                               POSE_DAMAGE, 0, 16);
      work->shakeFrames = 8;
      EFFECT_SHOW_DAMAGE(SE_CRASH);
      Iwram_FillWordsEntry(canvas, CANVAS_BYTES, CANVAS_FILL_WORD(63));
    }

    // The target is pushed down 2 pixels a frame for the first 24 frames,
    // while the user closes in on it.
    if (frame >= 0 && frame < 24)
      target->y += px(2);
    if (frame == 18)
      BattleEffect_SetApproachMotion(args->user, args->targets[0], 8, 80);
    if (frame == 21)
      BattleMotion_ApproachTargetFar(args->user, args->targets[0], 12, px(8));

    // Sparks fall and shrink: a spark is drawn at its life / 6 pixels wide,
    // two pixels at the least.
    step = PARTICLES;
    for (i = 0; i != 512; i++, step++) {
      if (step->variant >= 0) {
        s32 size = step->variant / 6;

        if (size < 2)
          size = 2;
        step->variant--;
        BattleEffectKernels_IntegrateVector2(step, 60, px(1) / 8);
        draw[0](canvas, sheet + gBattleEffect_MotePictures[size - 1],
                Q_16_16_UPPER_HALF(step->x) - size / 2,
                Q_16_16_UPPER_HALF(step->y) - size, size, size * 2);
      }
    }

    // Each flash rises for 18 frames through the six 32x64 pictures of the
    // flash sheet, three frames apiece.
    for (i = 0; i != 32; i++) {
      step = &work->particles[i];
      step->variant++;
      if (step->variant >= 0 && step->variant < 18) {
        s32 picture = step->variant / 3;

        BattleEffectKernels_IntegrateVector2(step, 60, -px(1) / 8);
        draw[0](canvas, work->sheet + 0x2000 + picture * 0x800,
                Q_16_16_UPPER_HALF(step->x) - 16,
                Q_16_16_UPPER_HALF(step->y) - 32, 32, 64);
      }
    }

    // The pillar scrolls up 8 pixels a frame over the target, drawn in
    // two pieces of its 104-pixel picture.
    if (frame >= 0 && frame < 32) {
      s32 scroll = (frame * 8) % 104;

      draw[0](canvas, work->sheet, position.x / 2 - 17, pillarY - scroll - 96,
              34, 104);
      draw[0](canvas, work->sheet, position.x / 2 - 17, pillarY - scroll + 8,
              34, scroll);
    }

    EFFECT_END_FRAME(work);
  }

  HeapSlot_Release(HEAP_SLOT_RAM_CODE);
  HeapSlot_Release(HEAP_SLOT_RAM_CODE_2);
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}

/// Copies a width by height picture into a picture twice its size, mirrored
/// into all four quadrants.
void Picture_MirrorIntoQuadrants(u8 *src, u8 *dst, s32 width, s32 height) {
  s32 x;
  s32 y;

  for (y = 0; y != height; y++) {
    for (x = 0; x != width; x++) {
      u8 c = src[y * width + x];

      dst[y * width * 2 + x] = c;
      dst[y * 2 * width + (width * 2 - 1 - x)] = c;
      dst[(height * 2 - 1 - y) * 2 * width + x] = c;
      dst[(height * 2 - 1 - y) * 2 * width + (width * 2 - 1 - x)] = c;
    }
  }
}

/// Abilities: Dreamtide.
void BattleEffect_RunDreamtide(struct BattleEffectParams *args) {
  BattleEffect_RunWaterSheet(args, WATER_SHEET_DREAMTIDE);
}

/// Abilities: Watery Grave.
void BattleEffect_RunWateryGrave(struct BattleEffectParams *args) {
  BattleEffect_RunWaterSheet(args, WATER_SHEET_WATERY_GRAVE);
}

struct StripQuad;

/// A corner of the water sheet: x, y and depth, a byte each.
struct WaterVertex {
  s32 x : 8;
  s32 y : 8;
  s32 z : 8;
};

/// One triangle of the water's mesh: three vertex indices, then a texture
/// coordinate for each.
struct WaterTriangle {
  u32 vertexA : 8;
  u32 vertexB : 8;
  u32 vertexC : 8;
  u32 : 8;
  u32 u0 : 8;
  u32 v0 : 8;
  u32 u1 : 8;
  u32 v1 : 8;
  u32 u2 : 8;
  u32 v2 : 8;
};

#define WATER_COLUMNS 16
#define WATER_ROWS 8
#define WATER_QUADS ((WATER_ROWS - 1) * (WATER_COLUMNS - 1))
/// The water's mesh as triangles, two to a quad.
#define WATER_MESH ((struct WaterTriangle *)gData_02014000)

/// The body of Dreamtide (mode 0) and Watery Grave (mode 1): a sheet of
/// water (a 16 by 8 grid of vertices, its mesh built here) rolls in under
/// the targets in waves while the camera turns; Dreamtide strikes the first
/// target with two splashes at frame 17, Watery Grave strikes every target
/// at frames 17 and 46 while bubbles rise from the deep.
void BattleEffect_RunWaterSheet(struct BattleEffectParams *args, s32 mode) {
  struct BattleEffectWork *work;
  s32 frame;
  DrawRectangle blit[2]; // Unused: [1] is the rest of its two-word pseudo.
  void *canvas;
  struct BattleCamera *camera;
  s16 savedYaw;
  s32 frames;
  struct ScreenPoint screen;
  struct ScreenPoint target;
  struct ModelTexture texture;
  struct RenderModel *model;
  struct Particle *bubble;
  s32 height;
  s32 phase;
  s32 row;
  s32 column;
  u8 *mesh;
  s32 i;

  work = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  BattleEffect_ConfigureEffectDisplay();
  BattleEffect_StartCanvasLayer(1);
  mesh = gData_02014000;
  if (mode == WATER_SHEET_DREAMTIDE) {
    REG_BLDALPHA = BLEND_ALPHA(16, 12);
  } else {
    REG_BLDALPHA = BLEND_ALPHA(16, 16);
  }
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_ADDITIVE);
  blit[0] = (DrawRectangle)Ram_HeapSlots->ramCode;
  camera = Ram_HeapSlots->cameraWork;
  savedYaw = (s16)camera->yaw;
  Resource_LoadAndDecompress((s32)&ResourceId_CrackedTextureSheet, work->sheet,
                             1, 1);
  Picture_MirrorIntoQuadrants(work->sheet, work->sheet + 0x400, 32, 32);
  Resource_LoadAndDecompress((s32)&ResourceId_Data0B4, work->sheet + 0x2000, 0,
                             0);
  if (mode == WATER_SHEET_WATERY_GRAVE) {
    u8 *pixel;

    /* FAKEMATCH(temporary): pixel set inside the call that takes the sheet's
     * address. */
    Resource_LoadAndDecompress((s32)&ResourceId_SmokeSheet,
                               pixel = work->sheet + 0x3000, 1, 0);
    for (i = 0; i != 0x1b00; i++, pixel++) {
      if (*pixel != 0) {
        *pixel += 8;
        if (*pixel > 63) {
          *pixel = 63;
        }
      }
    }
    {
      const void *palette =
          Resource_GetTableEntry((s32)&ResourceId_CrystalSheet);
      /* FAKEMATCH(temporary): the copy routine's address in a local. */
      s32 (*copy)(void *, const void *, s32) = Iwram_CopyWordsEntry;

      copy((void *)BG_PLTT, palette, 128);
    }
  }

  // The water's grid: 16 by 8 vertices, and a quad (two triangles) for
  // each cell but the last row and column.
  for (row = 0; row != WATER_ROWS; row++) {
    s32 v = row * 16;
    /* FAKEMATCH(dead): the row's first vertex index with the row added and
     * taken away again. */
    s32 q = v + row - row;

    for (column = 0; column != WATER_COLUMNS; column++) {
      s32 a = row * 16 + column;
      /* FAKEMATCH(dead): the vertex's index with q added and v, its equal,
       * taken away. */
      struct WaterVertex *vertex =
          (struct WaterVertex *)((s8 *)gMapGridCellBuffer + (a + q - v) * 4);
      s32 u = column * 8;

      SET_POSITION(vertex, column * 11 - 82, 60, row * 24 - 84);
      if (row <= 6 && column <= 14) {
        struct WaterTriangle *quad =
            (struct WaterTriangle *)(mesh + (row * 15 + column) * 24);
        s32 b = q + column + 1;
        s32 d = q + column + 17;
        s32 c = q + column + 16;

        /* FAKEMATCH(dead): a store the frame count below overwrites. Its column
         * * 8 - 60 is a counter of the column that the u + 7 below is then
         * counted from (- 60. */
        frames = (column * 8 - 60) & 1;
        quad[0].u0 = u + 7;
        quad[0].v0 = q;
        quad[0].u1 = u + 7;
        quad[0].v1 = v + 15;
        quad[0].u2 = u;
        quad[0].v2 = v + 15;
        quad[1].u0 = u;
        quad[1].v0 = q;
        quad[1].u1 = u + 7;
        quad[1].v1 = q;
        quad[1].u2 = u;
        quad[1].v2 = v + 15;
        quad[0].vertexA = b;
        quad[0].vertexB = d;
        quad[0].vertexC = c;
        quad[1].vertexA = a;
        quad[1].vertexB = b;
        quad[1].vertexC = c;
      }
    }
  }
  // A triangle of three zero indices ends the mesh.
  WATER_MESH[WATER_QUADS * 2].vertexA = 0;
  WATER_MESH[WATER_QUADS * 2].vertexB = 0;
  WATER_MESH[WATER_QUADS * 2].vertexC = 0;
  frames = 48;
  if (mode != WATER_SHEET_DREAMTIDE) {
    frames = 81;
  }

  for (frame = 0; frame != frames; frame++) {
    void *vertices;
    s32 shade;
    s32 angle;
    s32 offset;

    if (frame == 0) {
      camera->yaw = savedYaw;
      for (i = 0; i != 64; i++) {
        work->particles[i].variant = -1;
      }
    }
    if (args->side == EFFECT_SIDE_ENEMIES) {
      if ((s16)camera->yaw <= 11999) {
        camera->yaw = (s16)camera->yaw + 128;
      }
    } else if ((s16)camera->yaw >= -11999) {
      camera->yaw = (s16)camera->yaw - 128;
    }

    if (mode == WATER_SHEET_DREAMTIDE) {
      if (frame == 17) {
        EFFECT_SHOW_DAMAGE(SE_CLATTER);
        BattleEffect_StartActorMotion(args->targets[0], 1, 0, 0, 0x80000, 100);
        BattleEffect_TintBattler(args->targets[0], SPRITE_COLOR_WHITE,
                                 POSE_DAMAGE, 0, 16);
        work->shakeFrames = 8;
      }
    } else {
      if (frame == 17) {
        Sound_PlayFar(SE_CRASH);
        for (i = 0; i != args->targetCount; i++) {
          BattleMotion_ApplyKnockbackFar(args->targets[i], KNOCKBACK_NUDGE);
          BattleEffect_TintBattler(args->targets[i], SPRITE_COLOR_WHITE,
                                   POSE_DAMAGE, i, 16);
        }
        work->shakeFrames = 8;
      }
      if (frame == 46) {
        EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
        for (i = 0; i != args->targetCount; i++) {
          BattleEffect_StartActorMotion(args->targets[i], 1, 0, 0, 0x80000,
                                        100);
          BattleEffect_TintBattler(args->targets[i], SPRITE_COLOR_WHITE,
                                   POSE_DAMAGE, i, 16);
        }
        work->shakeFrames = 8;
      }
    }
    if (mode == WATER_SHEET_DREAMTIDE) {
      if ((u32)(frame - 33) <= 14) {
        REG_BLDALPHA = (48 - frame) | 0x1000;
      }
    } else if ((u32)(frame - 65) <= 14) {
      REG_BLDALPHA = (80 - frame) | 0x1000;
    }

    vertices = Heap_AllocEwram(0x400);
    model = Render_AllocateModelList(1);
    shade = 0;

    // The waves rise, then ebb.
    if (mode == WATER_SHEET_DREAMTIDE) {
      if (frame <= 31) {
        height = frame * 4;
        if (height > 64) {
          height = 64;
        }
      } else {
        height = 320 - frame * 8;
        if (height < 0) {
          height = 0;
        }
      }
    } else if (frame <= 63) {
      height = frame * 2;
      if (height > 64) {
        height = 64;
      }
    } else {
      height = 576 - frame * 8;
      if (height < 0) {
        height = 0;
      }
    }
    phase = frame * 2;
    row = 1;
    angle = frame << 11;
    for (offset = 64; row != 7; row++, offset += 64) {
      s8 *vertex;

      for (column = 0, vertex = (s8 *)gMapGridCellBuffer + offset + 1;
           column != WATER_COLUMNS; column++, vertex += 4) {
        s32 amplitude = (16 - column) * height / 32;
        s32 y = ((Trig_Sin(angle + column * 5000 + Trig_Sin(row * 3553)) *
                  amplitude) >>
                 16) -
                amplitude;

        /* FAKEMATCH(temporary): the height stored through the vertex it is part
         * of. */
        ((struct WaterVertex *)(vertex - 1))->y = y + 64;
      }
    }
    i = 0;
    height /= 2;
    for (bubble = work->particles; i != 20; i++, bubble++) {
      if (bubble->variant == 0) {
        bubble->y =
            (((Trig_Sin(angle + Trig_Sin(3552 + i * 888)) * height) >> 16) -
             height + 64)
            << 16;
      }
    }

    texture.logWidth = 6;
    texture.logHeight = 6;
    texture.pixels = work->sheet + 0x400;
    Transform_Reset();
    if (mode != WATER_SHEET_DREAMTIDE) {
      Transform_ApplyPitch(4000);
    }
    if (args->side == EFFECT_SIDE_ENEMIES) {
      Transform_ApplyYaw((s16)camera->yaw);
    } else {
      Transform_ApplyYaw((s16)camera->yaw + TRIG_TURN / 2);
    }
    {
      s32 shift = 128 - frame * 8;

      if (shift < -64) {
        shift = -64;
      }
      SceneTransform_ApplyPosition3(px(shift), 0, 0);
    }
    model->kind = 7;
    model->texture = &texture;
    model->mesh = mesh;
    model->vertices = vertices;
    model->shade = shade;
    model->scrollU = (frame + phase) & 127;
    model->scrollV = 127 & ~frame;
    Render_TransformVertices(gMapGridCellBuffer, vertices, 128);
    Render_DrawModelList(model);

    if (frame == 0) {
      s32 bubbleZ = -0x3c0000;

      for (i = 0, bubble = work->particles; i != 20; i++, bubble++) {
        bubble->variant = Random16() % 6;
        bubble->z = bubbleZ;
        bubble->x = -px(60);
        bubble->velocityX = 0;
        bubble->velocityY = 0;
        bubble->velocityZ = 0;
        bubbleZ += 0x60000;
      }
    }
    if (mode == WATER_SHEET_WATERY_GRAVE) {
      for (i = 0; i != 20; i++) {
        struct Particle *step = &work->particles[i];
        s32 picture = step->variant;
        struct ScreenPoint *position;

        if (picture < 0) {
          continue;
        }
        /* FAKEMATCH(temporary): position holds &screen. */
        position = &screen;
        if (EffectPosition_ProjectPoint(step, position) == 0 &&
            Q_16_16_UPPER_HALF(position->depth) > 10) {
          s32 x = Q_16_16_UPPER_HALF(position->x);
          s32 y;

          position->x = x + 64;
          y = Q_16_16_UPPER_HALF(position->y);
          position->y = y + 64;
          blit[0](canvas, work->sheet + 0x3000 + picture * 0x480, x + 52,
                  y + 40, 24, 48);
          BattleEffectKernels_IntegrateVector3(step, 62, -px(1) / 8);
        }
        if (++step->variant == 6) {
          step->variant = 0;
        }
      }
    } else {
      s32 t;

      EffectPosition_ProjectBattler(args->targets[0], &target);
      texture.logWidth = 6;
      texture.logHeight = 6;
      model->mesh = gBattleEffect_SquareMesh;
      model->vertices = vertices;
      model->scrollU = 0;
      model->scrollV = 0;
      for (i = 0; i != 2; i++) {
        t = frame - 16 - i * 4;
        if (t < 0) {
          continue;
        }
        texture.pixels = work->sheet + 0x2000;
        if (t > 7) {
          shade = (8 - t) * 4;
        }
        model->shade = shade;
        if (shade <= -64) {
          continue;
        }
        Transform_Reset();
        SceneTransform_ApplyPosition3(px(target.x / 2 - 64), px(target.y - 64),
                                      0);
        SceneTransform_ApplyScale3(Q16_ONE / 2, Q16_ONE, Q16_ONE / 2);
        Transform_ApplyRoll((i << 14) + TRIG_TURN / 8);
        Transform_ApplyPitch(TRIG_TURN / 16);
        Transform_ApplyYaw(angle);
        SceneTransform_ApplyUniformScale(t * 0x3000);
        Render_TransformVertices(gBattleEffect_GroundSquareCorners, vertices,
                                 4);
        Render_DrawModelList(model);
      }
    }
    Heap_Release(model);
    Heap_Release(vertices);
    BattleCamera_Shake(8, 8);
    EFFECT_END_FRAME(work);
  }
  HeapSlot_Release(HEAP_SLOT_RAM_CODE);
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}

/// Abilities: Waft.
void BattleEffect_RunWaft(struct BattleEffectParams *args) {
  BattleEffect_RunWindFunnel(args, WIND_FUNNEL_WAFT);
}

/// Abilities: Aroma.
void BattleEffect_RunAroma(struct BattleEffectParams *args) {
  BattleEffect_RunWindFunnel(args, WIND_FUNNEL_AROMA);
}

#define WAFT_SEGMENTS 33

/// The body of Waft (mode 0) and Aroma (mode 1): the djinni appears over the
/// caster, bobs and rises, while a funnel of wind (33 segments of two
/// vertices, each frame's head copied down the tail) twists up from it; Waft
/// strikes the first target at frame 28, Aroma touches each target in turn,
/// four frames apart.
void BattleEffect_RunWindFunnel(struct BattleEffectParams *args, s32 mode) {
  struct BattleEffectWork *work;
  void *canvas;
  u8 *sprites;
  // Unused: the second blitter, the idle half of the pair, gives the stack
  // frame its 84 bytes.
  DrawRectangle blit[2];
  struct BattleCamera *camera;
  struct ScreenPoint other;
  struct ScreenPoint screen;
  struct ModelTexture texture;
  struct RenderModel *model;
  s32 yaw;
  s32 frame;
  s32 frames;
  s32 i;
  s32 k;
  u8 *vertices;
  u8 *funnel;
  void (*transfer)(void);

  work = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  camera = Ram_HeapSlots->cameraWork;
  sprites = Ram_HeapSlots->battleSheet;
  // Read through funnel at the corner loop and the transform only.
  funnel = gMapGridCellBuffer;
  BattleEffect_StartCanvasLayer(0);
  if (mode == WIND_FUNNEL_WAFT) {
    BattleEffect_RunDjinniEntrance(
        args,
        DJINNI_ENTRANCE(ELEMENT_JUPITER, args->side, DJINNI_ANCHOR_OVER_TARGET),
        (s32 *)&screen, (s32 *)&other);
    REG_BG2CNT = BGCNT_PRIORITY(1) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
                 BGCNT_SCREEN_BASE(7);
  } else {
    BattleEffect_RunDjinniEntrance(
        args,
        DJINNI_ENTRANCE(ELEMENT_JUPITER, args->side, DJINNI_ANCHOR_TARGET_X),
        (s32 *)&screen, (s32 *)&other);
  }
  if (args->side == EFFECT_SIDE_ENEMIES) {
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
  } else {
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
                        EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, BLITTER_MAX);
  }
  blit[0] = (DrawRectangle)Ram_HeapSlots->ramCode;
  REG_BLDALPHA = BLEND_ALPHA(16, 16);
  EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
  transfer = BattleEffect_ProcessPendingGraphicsTransfer;
  Task_Add(transfer, TASK_PASS_FRAME);
  yaw = 0; // Default: set again at frame 0 before it is read.
  frames = 36;
  if (mode == WIND_FUNNEL_AROMA && args->targetCount > 4) {
    frames = args->targetCount * 6 + 12;
  }
  Resource_LoadAndDecompress((s32)&ResourceId_Data0D3, work->sheet, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sprites, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_JupiterDjinnSmallSheet,
                             work->sheet + 0x2000, 1, 1);
  Render_BuildQuadStrip(Data_02010318, 4, 32, 32);

  for (frame = 0; frame != frames; frame++) {
    s32 shade;
    s32 height;
    s32 y;
    s32 angle;
    s8 *corner;
    s32 x; // The djinni's x; at frame 0, the funnel's length.

    if (frame == 0) {
      s8 *head;
      s32 row;

      yaw = 0;
      // vertices is reused for the radius, 32, through casts; an s32 of its own
      // differs.
      /* FAKEMATCH(dead): both arms set the radius alike. */
      if (mode == WIND_FUNNEL_WAFT) {
        x = 80;
        vertices = (u8 *)32;
      } else {
        x = 30;
        vertices = (u8 *)32;
      }
      for (i = 0; i != WAFT_SEGMENTS; i++) {
        row = i * 8;
        for (k = 0; k != 2; k++) {
          head = (s8 *)gMapGridCellBuffer + row + k * 4;
          head[1] = -(x + k * 24);
          head[0] = 0;
          head[2] = (s32)vertices;
        }
      }
      Sound_PlayFar(SE_179);
    }
    if (frame > 19) {
      if (mode == WIND_FUNNEL_WAFT) {
        BattleEffect_StepPaletteToResource((s32)&ResourceId_CyanPalette);
      } else if (mode == WIND_FUNNEL_AROMA) {
        BattleEffect_StepPaletteToResource((s32)&ResourceId_TanPalette);
      }
    }

    // The djinni, bobbing over the caster and rising from frame 8.
    if (frame <= 79) {
      s32 y;

      x = ((Trig_Sin(frame << 11) << 3) >> 16) + screen.x / 2 - 10;
      y = ((Trig_Cos(frame << 11) << 2) >> 16) + screen.y - 24;

      if (frame > 7) {
        y = y - frame * 4 + 32;
      }
      blit[0](canvas, work->sheet + 0x2000, x, y, 20, 40);
    }

    // The funnel's tail trails its head.
    if ((frame & 1) || frame > 47 || frame <= 31) {
      s8 *corner;
      s32 i;
      for (i = 32; i != 0; i--) {
        k = 0;
        corner = (s8 *)gMapGridCellBuffer + (i - 1) * 8;
        for (; k != 2; k++) {
          corner[8] = corner[0];
          corner[9] = corner[1];
          corner[10] = corner[2];
          corner += 4;
        }
      }
    }
    if (frame > 23) {
      yaw = yaw - frame * 16 + 2432;
    } else {
      yaw += 2048;
    }
    if (mode == WIND_FUNNEL_WAFT) {
      angle = frame << 12;
      height = 80 - ((Trig_Sin(frame * 820) * 80) >> 16);
    } else {
      angle = yaw;
      height = 30 - ((Trig_Sin(frame * 820) * 20) >> 16);
    }
    k = 0;
    corner = (s8 *)funnel;
    y = -height;
    for (; k != 2; k++) {
      // The (u32) casts are an equivalent spelling: the stored byte is the
      // same.
      corner[0] = (u32)Trig_Sin(angle) >> 11;
      corner[1] = y;
      corner[2] = (u32)Trig_Cos(angle) >> 11;
      y -= 24;
      corner += 4;
    }

    if (mode == WIND_FUNNEL_WAFT) {
      if (frame == 28) {
        work->shakeFrames = 6;
        EFFECT_SHOW_DAMAGE(SE_IMPACT);
        BattleEffect_TintBattler(args->targets[0], SPRITE_COLOR_WHITE,
                                 POSE_DAMAGE, 0, 8);
      }
    } else {
      if (frame == 12) {
        EFFECT_SHOW_DAMAGE(-1);
      }
      for (k = 0; k != args->targetCount; k++) {
        if (frame == k * 4 + 12) {
          Sound_PlayFar(SE_RECOVERY);
          BattleEffect_TintBattler(args->targets[k], SPRITE_COLOR_WHITE,
                                   TINT_KEEP, k, 8);
        }
      }
    }

    Transform_Reset();
    Transform_SetLookAt(camera->eye, camera->focus);
    model = Render_AllocateModelList(1);
    vertices = Data_02010108;
    shade = 0;
    if (frame <= 15) {
      shade = frame * 4 - 64;
      if (shade > 0) {
        shade = 0;
      }
    } else if (frame > 27) {
      shade = (28 - frame) * 8;
      if (shade < -64) {
        shade = -64;
      }
    }
    texture.logWidth = 7;
    texture.logHeight = 5;
    texture.pixels = work->sheet;
    model->kind = 6;
    model->texture = &texture;
    model->mesh = Data_02010318;
    model->scrollU = 0;
    model->scrollV = 0;
    gProjection.centerY = 104;
    model->vertices = vertices;
    model->shade = shade;
    for (k = 0; k != gWindFunnel_Counts[mode]; k++) {
      Transform_Reset();
      if (mode == WIND_FUNNEL_WAFT) {
        s32 scale;

        SceneTransform_ApplyPosition3(px(screen.x / 2 - 64), px(40), 0);
        Transform_ApplyPitch(k << 11);
        Transform_ApplyYaw(k * 21000 + yaw);
        scale = 85536 - Trig_Sin(frame * 964);
        if (frame > 27) {
          scale = scale + (frame << 11) - 0xe000;
        }
        SceneTransform_ApplyScale3(scale * 3 / 2, Q16_ONE, scale * 3 / 2);
      } else {
        s32 scale;

        SceneTransform_ApplyPosition3(px(screen.x / 2 - 64),
                                      (k << 20) + 0x100000, 0);
        Transform_ApplyPitch((k << 12) - TRIG_TURN / 32);
        Transform_ApplyYaw(k * 21000 + yaw);
        scale = TRIG_TURN - Trig_Sin(frame * 682) / 4;
        if (frame > 27) {
          scale = scale + (frame << 11) - 0xe000;
        }
        SceneTransform_ApplyScale3(scale * 2, Q16_ONE, scale * 2);
      }
      Render_TransformVertices(funnel, Data_02010108, 66);
      Render_DrawModelList(model);
    }
    gProjection.centerX = 120;
    gProjection.centerY = 120;
    Heap_Release(model);
    BattleCamera_Shake(4, 4);
    EFFECT_END_FRAME(work);
  }
  HeapSlot_Release(HEAP_SLOT_RAM_CODE);
  Task_Remove(transfer);
  BattleEffect_EndCanvasLayer();
}

/* VIEW(undead_sword): tag struct RenderModel as struct
 * RenderModel__undead_sword; render_model.h has another definition; tried with
 * the definition the rest of the object has, BattleEffect_RunDiamondBerg moves:
 * 1 of 655 instructions in tla-en. */
struct RenderModel__undead_sword {
  s32 kind;
  s32 flags;
  const u16 *mesh;
  u8 *vertices;
  struct ModelTexture *texture;
  s32 shade;
  u8 scrollU;
  u8 scrollV;
};

#define DIAMOND_BERG_FRAMES 148

/// An iceberg drops onto the target and is driven down in eight blows (every
/// eighth frame from 31), each throwing eight shards; the last shatters it
/// into a burst of light. Ripples of frost spread over the ground beneath.
///
/// Abilities: Diamond Berg.
void BattleEffect_RunDiamondBerg(struct BattleEffectParams *args) {
  struct BattleEffectWork *work;
  void *canvas;
  u8 *sprites;
  // Unused: blit[1], the idle half of the pair, gives the stack frame its 4
  // bytes.
  DrawRectangle blit[2];
  struct ScreenPoint pos;
  struct ScreenPoint target;
  struct ModelTexture texture;
  struct RenderModel__undead_sword *model;
  s32 centerX;
  s32 top;
  s32 frame;
  s32 i;
  s32 k;
  u8 *cells;

  work = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  sprites = Ram_HeapSlots->battleSheet;
  BattleEffect_StartCanvasLayer(0);
  REG_BLDALPHA = BLEND_ALPHA(16, 16);
  EffectPosition_ProjectBattler(args->targets[0], &pos);
  centerX = pos.x / 2;
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
  blit[0] = (DrawRectangle)Ram_HeapSlots->ramCode;
  Resource_LoadAndDecompress((s32)&ResourceId_CrystalSheet, work->sheet, 1, 0);
  cells = gMapGridCellBuffer;
  Resource_LoadAndDecompress((s32)&ResourceId_CrystalBitmapSheet, cells + 20000,
                             1, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_Data0C1, cells, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data0DA, cells + 0x1000, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sprites, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data0C2, work->sheet + 0x3080, 0,
                             0);
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
  // DEAD: frame 0 sets top to -104 before anything reads it.
  top = -128;

  for (frame = 0; frame != DIAMOND_BERG_FRAMES; frame++) {
    if (frame == 0) {
      top = -104;
      for (i = 0; i != 64; i++) {
        work->particles[i].variant = -1;
      }
    }
    if (frame <= 63) {
      EffectPosition_ProjectBattler(args->targets[0], &target);
    }
    if (frame == 20) {
      BattleEffect_SetApproachMotion(args->user, args->targets[0], 8, 60);
    }
    if (frame == 2) {
      Sound_PlayFar(SE_POWER_UP);
    }

    // The berg: it drops, then each blow drives it eight lines down.
    if (frame <= 127) {
      s32 height = 104;

      if (frame <= 8) {
        top += 16;
      } else if ((u32)(frame - 24) <= 64) {
        s32 t = frame - 24;
        // t % 8, spelled so the quotient is its own register.
        s32 beat = t - t / 8 * 8;

        if (beat == 4) {
          Sound_PlayFar(SE_POWER_UP);
        }
        if (beat == 7) {
          work->shakeFrames = 4;
          if (frame <= 79) {
            Sound_PlayFar(SE_CLATTER);
            BattleEffect_SetApproachMotion(args->user, args->targets[0], 16,
                                           70);
          } else {
            BattleMotion_ApplyKnockbackFar(args->targets[0], KNOCKBACK_HURL);
            EFFECT_SHOW_DAMAGE(SE_CLATTER);
            Iwram_FillWordsEntry(canvas, CANVAS_BYTES, CANVAS_FILL_WORD(63));
          }
          BattleEffect_TintBattler(args->targets[0], SPRITE_COLOR_WHITE,
                                   POSE_DAMAGE, 0, 6);
          top += 8;
          for (k = 0; k != 8; k++) {
            struct Particle *shard;

            shard = &work->particles[((t - 7) / 8 * 8 + k + 32) & 63];
            shard->y = px(96);
            shard->x = px(centerX);
            shard->velocityX = ((Random16() & 127) - 64) << 11;
            shard->velocityY = -((Random16() & 127) + 64) << 11;
            shard->z = 32;
            shard->variant = 0;
          }
        }
      }
      // The berg's bottom (top + 104) past line 108; only this spelling, and
      // height - top + 4 below, match (as top > 4, 20 differ; 108 - top, 33).
      if (top + 104 > 108) {
        height = height - top + 4;
      }
      if (height > 0) {
        blit[0](canvas, Data_02014e20, centerX - 24, top, 48, height);
      }
    }
    if (frame == 7) {
      struct Particle *shard;

      work->shakeFrames = 8;
      Sound_PlayFar(SE_CRASH);
      for (k = 0, shard = work->particles; k != 32; k++, shard++) {
        shard->y = px(96);
        shard->x = px(centerX);
        shard->velocityX = ((Random16() & 127) - 64) << 12;
        shard->velocityY = -((Random16() & 127) + 64) << 12;
        shard->z = 32;
        shard->variant = 0;
      }
    }

    // The shards tumble and bounce; z is their spin.
    if (frame > 6) {
      struct Particle *shard;

      for (i = 0, shard = work->particles; i != 64; i++, shard++) {
        s32 cell;
        u8 *pixels;
        s32 x;
        s32 y;
        u32 width;
        u32 height;
        const u8 *heights;

        if ((u32)shard->variant > 1023) {
          continue;
        }
        cell = i % 5 * 3 + shard->variant / 96 % 3;
        pixels = work->sheet + 0x800 + gPrism_ShardPictures[cell];
        width = gPrism_ShardWidths[cell];
        x = Q_16_16_UPPER_HALF(shard->x) - (width >> 1);
        y = Q_16_16_UPPER_HALF(shard->y);
        // The heights table through a local: as gPrism_ShardHeights[cell] the
        // width moves to the stack one place early.
        heights = gPrism_ShardHeights;
        height = heights[cell];
        y -= height >> 1;
        blit[0](canvas, pixels, x, y, width, height);
        BattleEffectKernels_IntegrateVector2(shard, 64, px(1) / 4);
        if (Q_16_16_UPPER_HALF(shard->y) > 104) {
          shard->velocityY = -shard->velocityY / 2;
        }
        shard->variant += shard->z;
        if (shard->z > 1 && (frame & 1)) {
          shard->z--;
        }
      }
    }

    if (frame > 5) {
      void *vertices;

      vertices = Heap_AllocEwram(0x400);
      model = Render_AllocateModelList(1);
      texture.logWidth = 7;
      texture.logHeight = 7;
      model->kind = 7;
      model->texture = &texture;
      texture.pixels = cells;
      model->flags = 0;
      model->mesh = gBattleEffect_CylinderMesh;
      model->vertices = vertices;
      model->scrollU = 0;
      model->scrollV = 0;

      // The frost ripples.
      for (k = 0; k != 4; k++) {
        s32 start = gDiamondBerg_RingStarts[k] + 6;
        s32 scale;
        s32 shade;

        if (frame <= start) {
          continue;
        }
        scale = (frame - start) * gDiamondBerg_RingGrowth[k] * 1400 + 16768;
        shade = (start - frame) * 8 + 56;
        if (shade > 0) {
          shade = 0;
        }
        if (shade <= -64) {
          continue;
        }
        model->shade = shade;
        Transform_Reset();
        SceneTransform_ApplyPosition3(px(centerX - 64),
                                      px(gDiamondBerg_RingHeights[k]), 0);
        SceneTransform_ApplyScale3(scale, scale, scale);
        model->scrollU = ((frame + k * 5) & 7) << 4;
        Transform_ApplyPitch(3000);
        Render_TransformVertices(gBattleEffect_CylinderCorners, vertices, 32);
        Render_DrawModelList(model);
      }

      // A flash at each blow, at the target.
      texture.logWidth = 7;
      texture.logHeight = 7;
      texture.pixels = work->sheet + 0x3080;
      model->mesh = gBattleEffect_LargeSquareMesh;
      model->scrollU = 0;
      model->scrollV = 0;
      for (k = 0; k != 8; k++) {
        s32 t = frame - 31 - k * 8;
        s32 scale;
        s32 shade;

        if (t < 0) {
          continue;
        }
        scale = ((k << 11) + Q16_ONE / 8) * t + Q16_ONE / 2;
        shade = 0;
        if (t > 3) {
          shade = (4 - t) * 16;
        }
        model->shade = shade;
        if (shade <= -64) {
          continue;
        }
        Transform_Reset();
        SceneTransform_ApplyPosition3(px(target.x / 2 - 64), px(target.y - 80),
                                      0);
        SceneTransform_ApplyScale3(Q16_ONE / 2, Q16_ONE, Q16_ONE / 2);
        Transform_ApplyYaw((args->side << 15) + TRIG_TURN / 2);
        Transform_ApplyRoll(gDiamondBerg_ShardRolls[k]);
        Transform_ApplyPitch(gDiamondBerg_ShardPitches[k]);
        SceneTransform_ApplyUniformScale(scale);
        Render_TransformVertices(gBattleEffect_GroundSquareCorners, vertices,
                                 4);
        Render_DrawModelList(model);
      }
      Heap_Release(model);
      Heap_Release(vertices);
    }
    BattleCamera_Shake(4, 4);
    EFFECT_END_FRAME(work);
  }
  EFFECT_FINISH_CANVAS_ONE_BLITTER();
}
#undef EMBERS
#undef BLITTER
#undef SPARKS
#undef SPARK_COUNT
#undef OBJ_TILES
#undef PARTICLES
#undef SPARK_LIFE
#undef BLAST_SHEET_OFFSET
#undef BURST_FRAMES
#undef BURST_HIT_FRAME
#undef COLUMN_RISE_FRAME
#undef COLUMN_PEAK_FRAME
#undef COLUMN_MAX_HEIGHT
#undef WATER_COLUMNS
#undef WATER_ROWS
#undef WATER_QUADS
#undef WATER_MESH
#undef WAFT_SEGMENTS
#undef DIAMOND_BERG_FRAMES
