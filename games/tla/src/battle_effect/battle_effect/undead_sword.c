/// @file
/// Undead Sword, Plume Edge and Diamond Berg, and the bodies of Dreamtide and
/// Watery Grave and of Waft and Aroma.
// Part of battle_effect.c.

#include "../battle_effect_internal.h"
#include "fixed_math.h"
#include "sounds.h"

#define SPARK_COUNT 16
#define SPARK_LIFE 28
#define BLAST_SHEET_OFFSET 0x460

#define BURST_FRAMES 54
#define BURST_HIT_FRAME 10
#define COLUMN_RISE_FRAME 8 // the columns break the ground
#define COLUMN_PEAK_FRAME 31
#define COLUMN_MAX_HEIGHT 80

#define PHASE_HIT 212

extern char ResourceId_LimeColumnSheet;
extern const u16 gData_08198afc[]; // ☀️ gUndeadSword_SparkCells
extern const u16 gData_08198b0a[]; // ☀️ gUndeadSword_SparkSizes

// Two lime columns burst out of the ground under the target and a spray of
// sparks flies up between them.
//
// Setup: open the canvas, load the column and blast sheets, line the canvas
// up with the target, and launch 16 sparks from (64, 112) in random
// directions. Each frame: from frame 8 the two columns (one drawn flipped)
// rise 12 pixels a frame to 80, held there with the brighter MAX blitter, and
// from frame 32 sink 6 a frame; each spark shows its blast picture for its
// age and falls under gravity. The target is struck at frame 10.
void BattleEffect_RunUndeadSword(struct BattleEffectArgument *effect) {
  struct BattleEffectWork *work;
  void *canvas;
  void *sprites;
  DrawRectangle draw[2];
  struct EffectPosition target;
  struct EffectStep *spark;
  struct EffectStep *particle;
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

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  sprites = Ram_HeapSlots->unk64[0];

  // Open the canvas and load the pictures.
  BattleEffect_StartCanvasLayer(0);
  {
    REG_BG2PA = 0x100;
  }
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, sprites, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_LimeColumnSheet, work->sheet, 1,
                             1);
  Resource_LoadAndDecompress((s32)&ResourceId_BlastSheet,
                             work->sheet + BLAST_SHEET_OFFSET, 1, 0);
  work->transferMode = 2;
  work->transferValue = 75;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);

  // Slide the canvas so that its x = 64 is under the target.
  EffectPosition_ProjectBattler(effect->actors[0], &target);
  REG_BG2X = (64 - target.x) << 8;

  // The sparks start together at the columns' foot and fly apart, upward
  // faster than sideways.
  for (i = 0, spark = work->particles; i != SPARK_COUNT; i++) {
    speed = (Random16() & 0x1ff) + 128;
    angle = Random16() & 0xffff;
    spark->x = 0x400000;
    spark->y = 0x700000;
    spark->velocityX = (Trig_Sin(angle) * speed) >> 8;
    spark->velocityY = (Trig_Cos(angle) * speed) >> 9;
    spark->variant = Random16() & 7;
    spark++;
  }
  work->shakeFrames = 8;

  for (frame = 0; frame != BURST_FRAMES; frame++) {
    // The hit: results phase, shake, knock-back and a flash.
    if (frame == BURST_HIT_FRAME) {
      work->shakeFrames = 8;
      BattlePlayback_StartFar(PHASE_HIT);
      BattleMotion_ApplyKnockbackFar(effect->actors[0], 0);
      BattleEffect_TintBattler(effect->actors[0], 7, 5, 0, 8);
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
          mode = BLIT_MAX;
        } else {
          mode = BLIT_ADDITIVE;
        }
        for (i = 0, x = 50; i != 2; i++, x += 14) {
          if (i == 0)
            BattleEffect_BuildBlitter(
                HEAP_BLITTER, 7, 7, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y, mode);
          else
            BattleEffect_BuildBlitter(
                HEAP_BLITTER, 7, 7,
                COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y | COMPOSITE_FLIP_X, mode);
          (draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1])(
              canvas, work->sheet, x, 112 - height, 14, height);
          HeapSlot_Release(HEAP_BLITTER);
        }
      }
    }

    // The sparks: a new blast picture every third frame, seven in all; the
    // pairs leave one frame apart from frame 8.
    BattleEffect_FetchRectangleBlitters(effect->side, draw);
    for (i = 0, particle = work->particles; i != SPARK_COUNT; i++) {
      if (frame >= i / 2 + 8 && particle->variant <= SPARK_LIFE) {
        s32 px;
        s32 py;

        cell = particle->variant / 3;
        px = HI(particle->x);
        py = HI(particle->y);
        if (cell > 6)
          cell = 6;
        picture = work->sheet + gData_08198afc[cell] + BLAST_SHEET_OFFSET;
        size = gData_08198b0a[cell];
        draw[0](canvas, picture, px - size / 2, py - size / 2, size, size);
        particle->variant++;
        BattleEffectKernels_IntegrateVector2(particle, 62, -0x2000);
      }
      particle++;
    }
    HeapSlot_Release(HEAP_BLITTER_ALTERNATE);
    HeapSlot_Release(HEAP_BLITTER);

    if (frame < COLUMN_RISE_FRAME)
      BattleCamera_Shake(4, 4);
    else
      BattleCamera_Shake(16, 16);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}

extern char ResourceId_Data0DA;
extern char ResourceId_MercuryDjinnSmallSheet;

void BattleEffect_SetApproachMotion(s32 actor, s32 target, s32 frames,
                                    s32 percent);

// Plume Edge: the user dashes at the target, which a fan of white flashes
// throws back; a pillar of light scrolls up through it for half a second and
// then bursts into 256 sparks with a white flash of the whole canvas.
//
// Setup: canvas layer 0 at half blend, the two rectangle blitters, the
// pillar and flash sheets in the work sheet, the spark sheet in the shared
// sheet and the light palette over the background's.
// Frame loop (80 frames): the fan of 64 flashes at frame 0, the user's dash
// at frames 18 and 21, the spark burst at frame 32, then the sparks fall.
void BattleEffect_RunPlumeEdge(struct BattleEffectArgument *effect) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  u8 *sheet;
  struct MotionObject *target;
  s32 pillarY;
  DrawRectangle draw[2];
  struct EffectPosition position;
  struct EffectStep *step;
  s32 i;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  sheet = Ram_HeapSlots->unk64[0];
  BattleEffect_StartCanvasLayer(0);
  REG_BLDALPHA = 0x1010;
  BattleEffect_FetchRectangleBlitters(0, draw);
  work->transferMode = 2;
  work->transferValue = 50;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);
  target = BattleObject_GetFar(effect->actors[0])->object;
  pillarY = 0;
  Resource_LoadAndDecompress((s32)&ResourceId_Data0DA, work->sheet + 0x4710, 0,
                             0);
  Iwram_CopyWordsEntry(
      (void *)BG_PLTT,
      Resource_GetTableEntry((s32)&ResourceId_MercuryDjinnSmallSheet), 128);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, sheet, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_FirePillarSheetA, work->sheet, 1,
                             0);
  Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet,
                             work->sheet + 0x2000, 1, 0);
  for (i = 0; i != 1024; i++)
    PARTICLES[i].variant = -1;

  for (frame = 0; frame != 80; frame++) {
    EffectPosition_ProjectBattler(effect->actors[0], &position);

    // Frame 0: 64 flashes fan out upward from the target, one more each
    // frame, and the target turns white and stops bouncing.
    if (frame == 0) {
      for (i = 0; i != 64; i++) {
        s32 speed = (Random16() & 0x3ff) + 32;
        s32 angle = (Random16() & 0x7fff) - TRIG_TURN / 4;

        step = &work->particles[i];
        step->x = (position.x / 2) << 16;
        step->y = position.y << 16;
        step->velocityX = (Trig_Sin(angle) * speed) >> 8;
        step->velocityY = -(Trig_Cos(angle) * speed * 2) >> 8;
        step->variant = -i;
      }
      target->verticalMotionStrength = 0;
      target->y = 0;
      pillarY = position.y;
      BattleEffect_TintBattler(effect->actors[0], -1, 5, 0, -1);
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
        step->x = (position.x / 2) << 16;
        step->y = (position.y - 16) << 16;
        step->velocityX = (Trig_Sin(angle) * speed) >> 7;
        step->velocityY = -(Trig_Cos(angle) * speed * 2) >> 7;
        step->variant = (Random16() & 7) + 48;
      }
      BattleMotion_ApplyKnockbackFar(effect->actors[0], 4);
      BattleEffect_TintBattler(effect->actors[0], 7, 5, 0, 16);
      work->shakeFrames = 8;
      BattlePlayback_StartFar(145);
      Iwram_FillWordsEntry(canvas, CANVAS_BYTES, CANVAS_FILL_WORD(63));
    }

    // The target is pushed down 2 pixels a frame for the first 24 frames,
    // while the user closes in on it.
    if (frame >= 0 && frame < 24)
      target->y += 2 << 16;
    if (frame == 18)
      BattleEffect_SetApproachMotion(effect->actor, effect->actors[0], 8, 80);
    if (frame == 21)
      BattleMotion_ApproachTargetFar(effect->actor, effect->actors[0], 12,
                                     8 << 16);

    // Sparks fall and shrink: a spark is drawn at its life / 6 pixels wide,
    // two pixels at the least.
    step = PARTICLES;
    for (i = 0; i != 512; i++, step++) {
      if (step->variant >= 0) {
        s32 size = step->variant / 6;

        if (size < 2)
          size = 2;
        step->variant--;
        BattleEffectKernels_IntegrateVector2(step, 60, 0x2000);
        draw[0](canvas, sheet + gBattleEffect_MoteCells[size - 1],
                HI(step->x) - size / 2, HI(step->y) - size, size, size * 2);
      }
    }

    // Each flash rises for 18 frames through the six 32x64 pictures of the
    // flash sheet, three frames apiece.
    for (i = 0; i != 32; i++) {
      step = &work->particles[i];
      step->variant++;
      if (step->variant >= 0 && step->variant < 18) {
        s32 picture = step->variant / 3;

        BattleEffectKernels_IntegrateVector2(step, 60, -0x2000);
        draw[0](canvas, work->sheet + 0x2000 + picture * 0x800,
                HI(step->x) - 16, HI(step->y) - 32, 32, 64);
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

    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }

  HeapSlot_Release(104);
  HeapSlot_Release(188);
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

void BattleEffect_RunWaterSheet(struct BattleEffectArgument *effect, s32 mode);

/// Abilities: Dreamtide (kind 106).
void BattleEffect_RunDreamtide(s32 a0) { BattleEffect_RunWaterSheet(a0, 0); }

/// Abilities: Watery Grave (kind 391).
void BattleEffect_RunWateryGrave(s32 a0) { BattleEffect_RunWaterSheet(a0, 1); }

struct FxPartyList;
struct Model;
struct StripQuad;

// A vertex of the water: three signed bytes, written as bit-fields (a store
// through a plain byte pointer is scheduled after every earlier load, a
// stack slot's included).
struct WaterVertex {
  s32 x : 8;
  s32 y : 8;
  s32 z : 8;
};

// One triangle of the water's mesh: three vertex indices, then a texture
// coordinate for each.
struct WaterTriangle {
  u32 index0 : 8;
  u32 index1 : 8;
  u32 index2 : 8;
  u32 : 8;
  u32 u0 : 8;
  u32 v0 : 8;
  u32 u1 : 8;
  u32 v1 : 8;
  u32 u2 : 8;
  u32 v2 : 8;
};

extern char ResourceId_CrackedTextureSheet;
extern char ResourceId_Data0B4;
extern char ResourceId_SmokeSheet;
extern char ResourceId_CrystalSheet;
extern const u8 gData_08199340[]; // the splash's mesh

#define WATER_COLUMNS 16
#define WATER_ROWS 8
#define WATER_QUADS ((WATER_ROWS - 1) * (WATER_COLUMNS - 1))
// The water's mesh as triangles, two to a quad.
#define WATER_MESH ((struct WaterTriangle *)gData_02014000)

/// The body of Dreamtide (mode 0) and Watery Grave (mode 1): a sheet of
/// water (a 16 by 8 grid of vertices, its mesh built here) rolls in under
/// the targets in waves while the camera turns; Dreamtide strikes the first
/// target with two splashes at frame 17, Watery Grave strikes every target
/// at frames 17 and 46 while bubbles rise from the deep.
void BattleEffect_RunWaterSheet(struct BattleEffectArgument *effect, s32 mode) {
  struct BattleEffectWork *work;
  s32 frame;
  DrawRectangle blit[2]; // Unused: [1] is the rest of its two-word pseudo.
  void *canvas;
  struct BattleCamera *camera;
  s16 savedYaw;
  s32 frames;
  struct EffectPosition screen;
  struct EffectPosition target;
  struct ModelTexture texture;
  struct RenderModel *model;
  struct EffectStep *bubble;
  s32 height;
  s32 phase;
  s32 row;
  s32 column;
  u8 *mesh;
  s32 i;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  BattleEffect_ConfigureEffectDisplay();
  BattleEffect_StartCanvasLayer(1);
  mesh = gData_02014000;
  if (mode == 0) {
    REG_BLDALPHA = 0xc10;
  } else {
    REG_BLDALPHA = 0x1010;
  }
  work->transferMode = 2;
  work->transferValue = 50;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);
  BattleEffect_LoadBlitter(
      SLOT_BLITTER,
      BLITTER(BLIT_ADDITIVE, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y));
  blit[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  camera = Ram_HeapSlots->cameraWork;
  savedYaw = (s16)camera->yaw;
  Resource_LoadAndDecompress((s32)&ResourceId_CrackedTextureSheet, work->sheet,
                             1, 1);
  Picture_MirrorIntoQuadrants(work->sheet, work->sheet + 0x400, 32, 32);
  Resource_LoadAndDecompress((s32)&ResourceId_Data0B4, work->sheet + 0x2000, 0,
                             0);
  if (mode == 1) {
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
          (struct WaterVertex *)((s8 *)gMapCellBuffer + (a + q - v) * 4);
      s32 u = column * 8;

      vertex->x = column * 11 - 82;
      vertex->y = 60;
      vertex->z = row * 24 - 84;
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
        quad[0].index0 = b;
        quad[0].index1 = d;
        quad[0].index2 = c;
        quad[1].index0 = a;
        quad[1].index1 = b;
        quad[1].index2 = c;
      }
    }
  }
  // A triangle of three zero indices ends the mesh.
  WATER_MESH[WATER_QUADS * 2].index0 = 0;
  WATER_MESH[WATER_QUADS * 2].index1 = 0;
  WATER_MESH[WATER_QUADS * 2].index2 = 0;
  frames = 48;
  if (mode != 0) {
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
    if (effect->side == 0) {
      if ((s16)camera->yaw <= 11999) {
        camera->yaw = (s16)camera->yaw + 128;
      }
    } else if ((s16)camera->yaw >= -11999) {
      camera->yaw = (s16)camera->yaw - 128;
    }

    if (mode == 0) {
      if (frame == 17) {
        BattlePlayback_StartFar(144);
        BattleEffect_StartActorMotion(effect->actors[0], 1, 0, 0, 0x80000, 100);
        BattleEffect_TintBattler(effect->actors[0], 7, 5, 0, 16);
        work->shakeFrames = 8;
      }
    } else {
      if (frame == 17) {
        Sound_PlayFar(SE_CRASH);
        for (i = 0; i != effect->count; i++) {
          BattleMotion_ApplyKnockbackFar(effect->actors[i], 0);
          BattleEffect_TintBattler(effect->actors[i], 7, 5, i, 16);
        }
        work->shakeFrames = 8;
      }
      if (frame == 46) {
        BattlePlayback_StartFar(134);
        for (i = 0; i != effect->count; i++) {
          BattleEffect_StartActorMotion(effect->actors[i], 1, 0, 0, 0x80000,
                                        100);
          BattleEffect_TintBattler(effect->actors[i], 7, 5, i, 16);
        }
        work->shakeFrames = 8;
      }
    }
    if (mode == 0) {
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
    if (mode == 0) {
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

      for (column = 0, vertex = (s8 *)gMapCellBuffer + offset + 1;
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
    if (mode != 0) {
      Transform_ApplyPitch(4000);
    }
    if (effect->side == 0) {
      Transform_ApplyYaw((s16)camera->yaw);
    } else {
      Transform_ApplyYaw((s16)camera->yaw + TRIG_TURN / 2);
    }
    {
      s32 shift = 128 - frame * 8;

      if (shift < -64) {
        shift = -64;
      }
      SceneTransform_ApplyPosition3(shift << 16, 0, 0);
    }
    model->kind = 7;
    model->texture = &texture;
    model->mesh = mesh;
    model->vertices = vertices;
    model->shade = shade;
    model->scrollU = (frame + phase) & 127;
    model->scrollV = 127 & ~frame;
    Render_TransformVertices(gMapCellBuffer, vertices, 128);
    Render_DrawModelList(model);

    if (frame == 0) {
      s32 bubbleZ = -0x3c0000;

      for (i = 0, bubble = work->particles; i != 20; i++, bubble++) {
        bubble->variant = Random16() % 6;
        bubble->z = bubbleZ;
        bubble->x = -0x3c0000;
        bubble->velocityX = 0;
        bubble->velocityY = 0;
        bubble->velocityZ = 0;
        bubbleZ += 0x60000;
      }
    }
    if (mode == 1) {
      for (i = 0; i != 20; i++) {
        struct EffectStep *step = &work->particles[i];
        s32 picture = step->variant;
        struct EffectPosition *position;

        if (picture < 0) {
          continue;
        }
        /* FAKEMATCH(temporary): position holds &screen. */
        position = &screen;
        if (EffectPosition_ProjectPoint(step, position) == 0 &&
            HI(position->depth) > 10) {
          s32 x = HI(position->x);
          s32 y;

          position->x = x + 64;
          y = HI(position->y);
          position->y = y + 64;
          blit[0](canvas, work->sheet + 0x3000 + picture * 0x480, x + 52,
                  y + 40, 24, 48);
          BattleEffectKernels_IntegrateVector3(step, 62, -0x2000);
        }
        if (++step->variant == 6) {
          step->variant = 0;
        }
      }
    } else {
      s32 t;

      EffectPosition_ProjectBattler(effect->actors[0], &target);
      texture.logWidth = 6;
      texture.logHeight = 6;
      model->mesh = gData_08199340;
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
        SceneTransform_ApplyPosition3((target.x / 2 - 64) << 16,
                                      (target.y - 64) << 16, 0);
        SceneTransform_ApplyScale3(Q16_ONE / 2, Q16_ONE, Q16_ONE / 2);
        Transform_ApplyRoll((i << 14) + TRIG_TURN / 8);
        Transform_ApplyPitch(TRIG_TURN / 16);
        Transform_ApplyYaw(angle);
        SceneTransform_ApplyUniformScale(t * 0x3000);
        Render_TransformVertices(gData_08199210, vertices, 4);
        Render_DrawModelList(model);
      }
    }
    Heap_Release(model);
    Heap_Release(vertices);
    BattleCamera_Shake(8, 8);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }
  HeapSlot_Release(104);
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}

void BattleEffect_RunWindFunnel(struct BattleEffectArgument *effect, s32 mode);

/// Abilities: Waft (kind 234).
void BattleEffect_RunWaft(s32 a0) { BattleEffect_RunWindFunnel(a0, 0); }

/// Abilities: Aroma (kind 247).
void BattleEffect_RunAroma(s32 a0) { BattleEffect_RunWindFunnel(a0, 1); }

extern char ResourceId_Data0D3;

extern char ResourceId_JupiterDjinnSmallSheet;
extern char ResourceId_CyanPalette;
extern char ResourceId_TanPalette;

extern u8 Data_02010108[];
extern u8 Data_02010318[];
extern const u8 Data_08198b24[]; // the funnels per mode

#define WAFT_SEGMENTS 33

/// The body of Waft (mode 0) and Aroma (mode 1): the djinni appears over the
/// caster, bobs and rises, while a funnel of wind (33 segments of two
/// vertices, each frame's head copied down the tail) twists up from it; Waft
/// strikes the first target at frame 28, Aroma touches each target in turn,
/// four frames apart.
void BattleEffect_RunWindFunnel(struct BattleEffectArgument *effect, s32 mode) {
  struct BattleEffectWork *work;
  void *canvas;
  u8 *sprites;
  // Unused: the second blitter, as in The Broken Seal's pair, gives the
  // stack frame its 84 bytes (the word at sp 40); as one DrawRectangle, 48
  // of 609 differ (2026-11-08).
  DrawRectangle blit[2];
  struct BattleCamera *camera;
  struct EffectPosition other;
  struct EffectPosition screen;
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

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  camera = Ram_HeapSlots->cameraWork;
  sprites = Ram_HeapSlots->unk64[0];
  // Read through funnel at the corner loop and the transform only; through it
  // at the head loop, 14 of 609 differ (2026-11-08).
  funnel = gMapCellBuffer;
  BattleEffect_StartCanvasLayer(0);
  if (mode == 0) {
    BattleEffect_RunDjinniEntrance(effect, effect->side << 4 | 3,
                                   (s32 *)&screen, (s32 *)&other);
    REG_BG2CNT = 0x785;
  } else {
    BattleEffect_RunDjinniEntrance(effect, effect->side << 4 | 35,
                                   (s32 *)&screen, (s32 *)&other);
  }
  if (effect->side == 0) {
    BattleEffect_LoadBlitter(
        SLOT_BLITTER, BLITTER(BLIT_MAX, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y));
  } else {
    BattleEffect_LoadBlitter(
        SLOT_BLITTER, BLITTER(BLIT_MAX, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y |
                                            COMPOSITE_FLIP_X));
  }
  blit[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  REG_BLDALPHA = 0x1010;
  work->transferMode = 2;
  work->transferValue = 50;
  transfer = BattleEffect_ProcessPendingGraphicsTransfer;
  Task_Add(transfer, TASK_PASS_FRAME);
  yaw = 0; // Default: set again at frame 0 before it is read.
  frames = 36;
  if (mode == 1 && effect->count > 4) {
    frames = effect->count * 6 + 12;
  }
  Resource_LoadAndDecompress((s32)&ResourceId_Data0D3, work->sheet, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, sprites, 0, 0);
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
      if (mode == 0) {
        x = 80;
        vertices = (u8 *)32;
      } else {
        x = 30;
        vertices = (u8 *)32;
      }
      for (i = 0; i != WAFT_SEGMENTS; i++) {
        row = i * 8;
        for (k = 0; k != 2; k++) {
          head = (s8 *)gMapCellBuffer + row + k * 4;
          head[1] = -(x + k * 24);
          head[0] = 0;
          head[2] = (s32)vertices;
        }
      }
      Sound_PlayFar(SE_179);
    }
    if (frame > 19) {
      if (mode == 0) {
        BattleEffect_StepPaletteToResource((s32)&ResourceId_CyanPalette);
      } else if (mode == 1) {
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
        corner = (s8 *)gMapCellBuffer + (i - 1) * 8;
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
    if (mode == 0) {
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

    if (mode == 0) {
      if (frame == 28) {
        work->shakeFrames = 6;
        BattlePlayback_StartFar(133);
        BattleEffect_TintBattler(effect->actors[0], 7, 5, 0, 8);
      }
    } else {
      if (frame == 12) {
        BattlePlayback_StartFar(-1);
      }
      for (k = 0; k != effect->count; k++) {
        if (frame == k * 4 + 12) {
          Sound_PlayFar(SE_RECOVERY);
          BattleEffect_TintBattler(effect->actors[k], 7, -1, k, 8);
        }
      }
    }

    Transform_Reset();
    Transform_SetLookAt(camera->eye, camera->pos);
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
    gCameraSceneParameters[4] = 104;
    model->vertices = vertices;
    model->shade = shade;
    for (k = 0; k != Data_08198b24[mode]; k++) {
      Transform_Reset();
      if (mode == 0) {
        s32 scale;

        SceneTransform_ApplyPosition3((screen.x / 2 - 64) << 16, 0x280000, 0);
        Transform_ApplyPitch(k << 11);
        Transform_ApplyYaw(k * 21000 + yaw);
        scale = 85536 - Trig_Sin(frame * 964);
        if (frame > 27) {
          scale = scale + (frame << 11) - 0xe000;
        }
        SceneTransform_ApplyScale3(scale * 3 / 2, Q16_ONE, scale * 3 / 2);
      } else {
        s32 scale;

        SceneTransform_ApplyPosition3((screen.x / 2 - 64) << 16,
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
    gCameraSceneParameters[3] = 120;
    gCameraSceneParameters[4] = 120;
    Heap_Release(model);
    BattleCamera_Shake(4, 4);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }
  HeapSlot_Release(SLOT_BLITTER);
  Task_Remove(transfer);
  BattleEffect_EndCanvasLayer();
}

extern char ResourceId_CrystalBitmapSheet;
extern char ResourceId_Data0C1;

extern u8 Data_02014e20[];         // gMapCellBuffer + 20000, the berg
extern s32 gData_08197834[];       // the shards' cells: offsets,
extern u8 gPrism_ShardWidths[];    // widths
extern u8 gData_08197826[];        // and heights
extern const u8 Data_08198ec4[];   // the ripple's mesh
extern const u8 Data_08198cac[];   // its corners
extern const u8 Data_08198b26[4];  // per ripple: its start,
extern const u8 Data_08198b2a[4];  // its growth
extern const s8 Data_08198b2e[4];  // and its height
extern const s16 Data_08198b32[8]; // per flash: its roll
extern const s16 Data_08198b42[8]; // and pitch

#define DIAMOND_BERG_FRAMES 148

// Abilities: Diamond Berg.
//
// An iceberg drops onto the target and is driven down in eight blows (every
// eighth frame from 31), each throwing eight shards; the last shatters it
// into a burst of light. Ripples of frost spread over the ground beneath.
void BattleEffect_RunDiamondBerg(struct BattleEffectArgument *effect) {
  struct BattleEffectWork *work;
  void *canvas;
  u8 *sprites;
  // Unused: gives the stack frame its 4 bytes. (blit[1], at sp 28, as The
  // Broken Seal's pair; as one DrawRectangle, 68 differ.)
  DrawRectangle blit[2];
  struct EffectPosition pos;
  struct EffectPosition target;
  struct ModelTexture texture;
  struct RenderModel *model;
  s32 centerX;
  s32 top;
  s32 frame;
  s32 i;
  s32 k;
  u8 *cells;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  sprites = Ram_HeapSlots->unk64[0];
  BattleEffect_StartCanvasLayer(0);
  REG_BLDALPHA = 0x1010;
  EffectPosition_ProjectBattler(effect->actors[0], &pos);
  centerX = pos.x / 2;
  BattleEffect_LoadBlitter(
      SLOT_BLITTER, BLITTER(BLIT_MAX, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y));
  blit[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  Resource_LoadAndDecompress((s32)&ResourceId_CrystalSheet, work->sheet, 1, 0);
  cells = gMapCellBuffer;
  Resource_LoadAndDecompress((s32)&ResourceId_CrystalBitmapSheet, cells + 20000,
                             1, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_Data0C1, cells, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data0DA, cells + 0x1000, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, sprites, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data0C2, work->sheet + 0x3080, 0,
                             0);
  work->transferMode = 2;
  work->transferValue = 50;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);
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
      EffectPosition_ProjectBattler(effect->actors[0], &target);
    }
    if (frame == 20) {
      BattleEffect_SetApproachMotion(effect->actor, effect->actors[0], 8, 60);
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
            BattleEffect_SetApproachMotion(effect->actor, effect->actors[0], 16,
                                           70);
          } else {
            BattleMotion_ApplyKnockbackFar(effect->actors[0], 4);
            BattlePlayback_StartFar(144);
            Iwram_FillWordsEntry(canvas, CANVAS_BYTES, CANVAS_FILL_WORD(63));
          }
          BattleEffect_TintBattler(effect->actors[0], 7, 5, 0, 6);
          top += 8;
          for (k = 0; k != 8; k++) {
            struct EffectStep *shard;

            shard = &work->particles[((t - 7) / 8 * 8 + k + 32) & 63];
            shard->y = 0x600000;
            shard->x = centerX << 16;
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
      struct EffectStep *shard;

      work->shakeFrames = 8;
      Sound_PlayFar(SE_CRASH);
      for (k = 0, shard = work->particles; k != 32; k++, shard++) {
        shard->y = 0x600000;
        shard->x = centerX << 16;
        shard->velocityX = ((Random16() & 127) - 64) << 12;
        shard->velocityY = -((Random16() & 127) + 64) << 12;
        shard->z = 32;
        shard->variant = 0;
      }
    }

    // The shards tumble and bounce; z is their spin.
    if (frame > 6) {
      struct EffectStep *shard;

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
        pixels = work->sheet + 0x800 + gData_08197834[cell];
        width = gPrism_ShardWidths[cell];
        x = HI(shard->x) - (width >> 1);
        y = HI(shard->y);
        // The heights table through a local: as gData_08197826[cell] the
        // width moves to the stack one place early.
        heights = gData_08197826;
        height = heights[cell];
        y -= height >> 1;
        blit[0](canvas, pixels, x, y, width, height);
        BattleEffectKernels_IntegrateVector2(shard, 64, 0x4000);
        if (HI(shard->y) > 104) {
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
      model->unk04 = 0;
      model->mesh = Data_08198ec4;
      model->vertices = vertices;
      model->scrollU = 0;
      model->scrollV = 0;

      // The frost ripples.
      for (k = 0; k != 4; k++) {
        s32 start = Data_08198b26[k] + 6;
        s32 scale;
        s32 shade;

        if (frame <= start) {
          continue;
        }
        scale = (frame - start) * Data_08198b2a[k] * 1400 + 16768;
        shade = (start - frame) * 8 + 56;
        if (shade > 0) {
          shade = 0;
        }
        if (shade <= -64) {
          continue;
        }
        model->shade = shade;
        Transform_Reset();
        SceneTransform_ApplyPosition3((centerX - 64) << 16,
                                      Data_08198b2e[k] << 16, 0);
        SceneTransform_ApplyScale3(scale, scale, scale);
        model->scrollU = ((frame + k * 5) & 7) << 4;
        Transform_ApplyPitch(3000);
        Render_TransformVertices(Data_08198cac, vertices, 32);
        Render_DrawModelList(model);
      }

      // A flash at each blow, at the target.
      texture.logWidth = 7;
      texture.logHeight = 7;
      texture.pixels = work->sheet + 0x3080;
      model->mesh = gData_08199364;
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
        SceneTransform_ApplyPosition3((target.x / 2 - 64) << 16,
                                      (target.y - 80) << 16, 0);
        SceneTransform_ApplyScale3(Q16_ONE / 2, Q16_ONE, Q16_ONE / 2);
        Transform_ApplyYaw((effect->side << 15) + TRIG_TURN / 2);
        Transform_ApplyRoll(Data_08198b32[k]);
        Transform_ApplyPitch(Data_08198b42[k]);
        SceneTransform_ApplyUniformScale(scale);
        Render_TransformVertices(gData_08199210, vertices, 4);
        Render_DrawModelList(model);
      }
      Heap_Release(model);
      Heap_Release(vertices);
    }
    BattleCamera_Shake(4, 4);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(SLOT_BLITTER);
  BattleEffect_EndCanvasLayer();
}
#undef HI
#undef EMBERS
#undef BLITTER
#undef SPARKS
#undef SPARK_COUNT
#undef SLOT_BLITTER
#undef SLOT_ALT
#undef OBJ_TILES
#undef PARTICLES
#undef SPARK_LIFE
#undef BLAST_SHEET_OFFSET
#undef BURST_FRAMES
#undef BURST_HIT_FRAME
#undef COLUMN_RISE_FRAME
#undef COLUMN_PEAK_FRAME
#undef COLUMN_MAX_HEIGHT
#undef PHASE_HIT
#undef WATER_COLUMNS
#undef WATER_ROWS
#undef WATER_QUADS
#undef WATER_MESH
#undef WAFT_SEGMENTS
#undef DIAMOND_BERG_FRAMES
