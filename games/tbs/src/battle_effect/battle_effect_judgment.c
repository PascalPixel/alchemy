/// @file
/// The Judgment summon.
// Part of battle_effect.c.

#include "battle_effect_internal.h"

/// The bytes of Judgment's work block before its dimmed mote copies.
#define JUDGMENT_SHEET_BYTES 10000

/// Judgment's work block, laid over struct BattleEffectWork from its start; the
/// sweep's wave picture later overwrites the gathering's part of it.
struct JudgmentWork {
  /// The gathering's pictures: the pink burst, the burst's quarters and the
  /// tornado sheet.
  u8 sheet[JUDGMENT_SHEET_BYTES];
  /// The dimmed motes, which the gathering's ring of motes steps through as
  /// it fades.
  u8 dimmedMotes[DIMMED_COPIES][MOTE_SET_BYTES];
};

/// The gathering's motes in PARTICLES, then its sparks.
#define JUDGMENT_MOTES 128
#define JUDGMENT_SPARK_COUNT 128
#define JUDGMENT_SPARKS (&PARTICLES[JUDGMENT_MOTES])
/// The motes in the ring that spreads from the burst, and in the flash's ring.
#define JUDGMENT_RING_MOTES 128
#define JUDGMENT_FLASH_RING_MOTES 256

#define JUDGMENT_GATHER_FRAMES 160 ///< frames of the gathering
#define JUDGMENT_SWEEP_FRAMES 320
/// The gathering's crash: the canvas fills to JUDGMENT_CRASH_LEVEL the frame
/// before, then the beams fall.
#define JUDGMENT_BEAM_FRAME 144
#define JUDGMENT_CRASH_LEVEL 42
#define JUDGMENT_FORMATION_OBJECTS 16 ///< sprites in the formation, 4x4
#define JUDGMENT_FORMATION_COLUMNS 4
#define JUDGMENT_FORMATION_SPRITE_SIZE 32 ///< each sprite is 32 pixels square
/// The formation's animations, one a sprite for each of Judgment's poses:
/// its lion shield at its side, swinging round, then held before it.
#define JUDGMENT_POSE_SHIELD_SWINGING JUDGMENT_FORMATION_OBJECTS
#define JUDGMENT_POSE_SHIELD_FORWARD (2 * JUDGMENT_FORMATION_OBJECTS)
#define JUDGMENT_DEBRIS_OBJECTS 16 ///< debris sprites of the sweep

/// Where the motes gather on the canvas, at Judgment's shield: the burst's
/// quarters and shards and the falling beams are drawn about it.
#define JUDGMENT_BURST_X 60
#define JUDGMENT_BURST_Y 80
/// How far from the burst's centre its shards flicker.
#define JUDGMENT_SHARD_RADIUS 16
/// The gathering motes' depths either side of the burst; a mote is drawn a size
/// larger for each JUDGMENT_MOTE_SIZE_DEPTH nearer, from size 2.
#define JUDGMENT_MOTE_DEPTH 60
#define JUDGMENT_MOTE_SIZE_DEPTH 20
/// The unused sparks stage's depths, and the depth it starts its nearest
/// from, above any a spark can have. The sparks start on a ring a quarter of
/// JUDGMENT_SPARK_AMPLITUDE out, and at frame 64 burst outward by an eighth of
/// it.
#define JUDGMENT_SPARK_DEPTH_NEAR 100
#define JUDGMENT_SPARK_DEPTH_FAR 900
#define JUDGMENT_SPARK_DEPTH_NONE 65535
#define JUDGMENT_SPARK_AMPLITUDE 255
/// The pink burst's beam picture, at the sheet's start.
#define JUDGMENT_BEAM_WIDTH 24
#define JUDGMENT_BEAM_HEIGHT 64
/// The burst's seven quarter pictures (OverlayImageB), after the beam.
#define JUDGMENT_SHEET_BURST PICTURE_BYTES(JUDGMENT_BEAM)
/// The tornado sheet, after the burst's seven quarter pictures; its six glint
/// pictures are the burst's shards (gJudgment_ShardPictures counts from the
/// sheet's start).
#define JUDGMENT_SHEET_TORNADO                                                 \
  (JUDGMENT_SHEET_BURST + 2 * 4 + 3 * 6 + 4 * 8 + 6 * 12 + 10 * 20 + 11 * 22 + \
   12 * 24)
/// The explosion's four pictures, loaded over the flares on the sweep's
/// frame 64.
#define JUDGMENT_SHEET_EXPLOSION 16384
/// The two flare pictures, from the crescent sheet the sweep loads.
#define JUDGMENT_FLARE_WIDTH 24
#define JUDGMENT_FLARE_HEIGHT 24
#define JUDGMENT_SHEET_FLARE_A 16384
#define JUDGMENT_SHEET_FLARE_B                                                 \
  (JUDGMENT_SHEET_FLARE_A + PICTURE_BYTES(JUDGMENT_FLARE))
/// OverlayImageA's three flash pictures, a byte a pixel, one after another in
/// the sheet: the small flash, the large flash and the glow.
#define JUDGMENT_SHEET_FLASH_SMALL_WIDTH 16
#define JUDGMENT_SHEET_FLASH_SMALL_HEIGHT 17
#define JUDGMENT_SHEET_FLASH_LARGE_WIDTH 24
#define JUDGMENT_SHEET_FLASH_LARGE_HEIGHT 41
#define JUDGMENT_SHEET_FLASH_GLOW_WIDTH 22
#define JUDGMENT_SHEET_FLASH_GLOW_HEIGHT 44
#define JUDGMENT_SHEET_FLASH_SMALL 24576
#define JUDGMENT_SHEET_FLASH_LARGE                                             \
  (JUDGMENT_SHEET_FLASH_SMALL + PICTURE_BYTES(JUDGMENT_SHEET_FLASH_SMALL))
#define JUDGMENT_SHEET_FLASH_GLOW                                              \
  (JUDGMENT_SHEET_FLASH_LARGE + PICTURE_BYTES(JUDGMENT_SHEET_FLASH_LARGE))
#define JUDGMENT_EXPLOSION_WIDTH 32
#define JUDGMENT_EXPLOSION_HEIGHT 54
#define JUDGMENT_EXPLOSION_PICTURE_BYTES PICTURE_BYTES(JUDGMENT_EXPLOSION)

/// The sweep's scenes, by the frame each opens on: the flare lands on the
/// aerial view and it explodes, the wave crosses the beach, the debris rises
/// in the forest, and the battle returns. A or B skips to the next scene from
/// five frames into one.
#define JUDGMENT_SWEEP_EXPLOSION 64
#define JUDGMENT_SWEEP_BEACH 150
#define JUDGMENT_SWEEP_FOREST 214
#define JUDGMENT_SWEEP_RETURN 280
#define JUDGMENT_WAVE_SIZE 120 ///< the wave picture is 120x120
/// The level the wave's rings are drawn at.
#define JUDGMENT_RING_LEVEL 20
/// Where the canvas sits on the screen in the sweep: the wave picture
/// centred.
#define JUDGMENT_SWEEP_CANVAS_X ((DISPLAY_WIDTH - JUDGMENT_WAVE_SIZE) / 2)
/// The column of the wave picture the flare falls down and the aerial view's
/// rings spread from, at JUDGMENT_AERIAL_RINGS_Y; the beach's rings spread from
/// further right and higher.
#define JUDGMENT_FLARE_X (JUDGMENT_WAVE_SIZE / 2)
#define JUDGMENT_AERIAL_RINGS_Y 44
#define JUDGMENT_BEACH_RINGS_X 112
#define JUDGMENT_BEACH_RINGS_Y 32
/// Where the blast and its debris sweep back in from as the battle returns,
/// beyond the screen's right edge.
#define JUDGMENT_RETURN_BLAST_X (DISPLAY_WIDTH + 32)

/// Sets a ring's four mirrored points (left or right, top or bottom) of the
/// wave picture to JUDGMENT_RING_LEVEL.
#define JUDGMENT_PLOT_RING_POINTS(left, right, top, bottom)                    \
  (work->sheet[(bottom) * JUDGMENT_WAVE_SIZE + (right)] = JUDGMENT_RING_LEVEL, \
   work->sheet[(top) * JUDGMENT_WAVE_SIZE + (right)] = JUDGMENT_RING_LEVEL,    \
   work->sheet[(bottom) * JUDGMENT_WAVE_SIZE + (left)] = JUDGMENT_RING_LEVEL,  \
   work->sheet[(top) * JUDGMENT_WAVE_SIZE + (left)] = JUDGMENT_RING_LEVEL)

/// The battle wipes away to a violet sky where Judgment, a winged knight with a
/// lion shield, flies forward while motes gather into its shield; they burst in
/// a flash, a flare falls on the land below and explodes in rings of light, and
/// the blast sweeps over a beach and through a forest, throwing up rocks,
/// before the battle returns.
///
/// Abilities: Judgment.
void BattleEffect_RunJudgment(struct BattleEffectParams *args) {
  static struct SpriteScale unitScale = {{{FIXED_ONE, FIXED_ONE}}};
  s32 anchor[3];
  struct ScreenPoint projectedPosition;
  struct SpritePlacement place;
  volatile u32 fill;
  struct SpritePlacement debrisPlace;
  struct SpriteScale pair;
  struct SpriteScale scale;
  void *canvas;
  struct BattleEffectWork *work;
  struct BattleBackgroundWork *view;
  DrawRectangle draw[2];
  s32 frame;
  u8 *motes;
  struct BattleCamera *camera;
  FIXED depth;
  s32 zoom;
  s32 minimumDepth;
  s32 maximumDepth;
  void **heap;
  DrawRectangle *blitters;
  s32 cx;
  s32 cy;
  s32 squash;
  s32 stretch = 8; // the rings' width and upper half, in eighths
  s32 ring;
  s32 rings;
  s32 shift;
  s32 i;
  s32 j;
  struct Particle *particle;
  DrawRectangle blit;
  volatile u16 *ime;

  ime = (u16 *)&REG_IME;
  heap = &gHeapSlots[HEAP_SLOT_BATTLE_CANVAS];
  canvas = heap[0];
  work = HEAP_SLOT_FROM(heap, HEAP_SLOT_BATTLE_CANVAS, HEAP_SLOT_BATTLE_EFFECT);
  view = HEAP_SLOT_FROM(heap, HEAP_SLOT_BATTLE_CANVAS,
                        HEAP_SLOT_BATTLE_BACKGROUND);
  motes = HEAP_SLOT_FROM(heap, HEAP_SLOT_BATTLE_CANVAS, HEAP_SLOT_BATTLE_SHEET);
  camera = HEAP_SLOT_FROM(heap, HEAP_SLOT_BATTLE_CANVAS, HEAP_SLOT_CAMERA);
  work->args = args;
  EFFECT_START_NARROW_CANVAS(BGCNT_WRAP);
  BattleEffect_ConfigureEffectDisplay();
  PLTT_BACKDROP = 0;
  BG_PLTT_COLOR(0, 1) = 0;
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                      COMPOSITE_ADDITIVE);
  draw[0] = (DrawRectangle)HEAP_SLOT_FROM(heap, HEAP_SLOT_BATTLE_CANVAS,
                                          HEAP_SLOT_RAM_CODE);
  work->transferMode = CANVAS_COPY;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_VBLANK);
  // Wipe the battle away, change the battle background behind it, wipe
  // back.
  BattleEffect_WipeCanvas(CANVAS_WIPE_DOWN, CANVAS_WIPE_FILL);
  BattleEffect_SelectLivingTargets(work->args);
  BattleEffect_SpawnObjects(JUDGMENT_FORMATION_OBJECTS, SPRITE_JUDGMENT, 1);
  gProjection.centerY = SUMMON_PROJECTION_CENTER_Y;
  wait(1);
  BattleBackground_LoadFar(BATTLE_BACKGROUND_FIXED,
                           RESOURCE_VIOLET_SKY_BACKDROP, 0);
  view->busy = 1;
  gBgScroll[1].x = 0;
  BattleEffect_WipeCanvas(CANVAS_WIPE_DOWN, CANVAS_WIPE_CLEAR);
  EFFECT_SHOW_WIDE_CANVAS(BLEND_WEIGHT_MAX);
  depth = -toFIXED(64);
  zoom = toFIXED(8);
  minimumDepth = JUDGMENT_SPARK_DEPTH_NONE;
  maximumDepth = 0;
  Resource_LoadAndDecompress(RESOURCE_PINK_BURST_SHEET, work->sheet, 1, 1);
  Resource_LoadAndDecompress(RESOURCE_OVERLAY_IMAGE_B,
                             work->sheet + JUDGMENT_SHEET_BURST, 0, 0);
  Resource_LoadAndDecompress(RESOURCE_TORNADO_SHEET,
                             work->sheet + JUDGMENT_SHEET_TORNADO, 1, 0);
  Resource_LoadAndDecompress(RESOURCE_PARTICLE_SPRITES_A, motes, 0, 0);

  for (i = 0; i != DIMMED_COPIES; i++) {
    u8 *src = motes;
    for (j = 0; j != MOTE_SET_BYTES; j++) {
      s32 v = *src++;
      EFFECT_CAP_LEVEL(v, DIMMED_COPY_CAP(i));
      ((struct JudgmentWork *)work)->dimmedMotes[i][j] = v;
    }
  }

  Iwram_CopyWords(BG_PLTT, Resource_GetTableEntry(RESOURCE_BLUE_CRESCENT_SHEET),
                  SHEET_PALETTE_BYTES);

  // Each mote starts 72 to 87 pixels from the shield, its two velocities
  // holding the tilt and turn of its path; the first turn drawn is thrown
  // away.
  for (i = 0; i != JUDGMENT_MOTES; i++) {
    PARTICLES[i].x = (Random16() & 15) + 72;
    PARTICLES[i].velocityX = Random16() & TRIG_ANGLE_MASK;
    PARTICLES[i].velocityY = Random16() & 0xffff;
    PARTICLES[i].velocityY = Random16() & TRIG_ANGLE_MASK;
  }

  for (i = 0; i != JUDGMENT_SPARK_COUNT; i++) {
    s32 angle = Random16() & TRIG_ANGLE_MASK;
    s32 amplitude = JUDGMENT_SPARK_AMPLITUDE;
    s32 speed = 0;
    JUDGMENT_SPARKS[i].x = (Trig_Sin(angle) * amplitude) >> 2;
    JUDGMENT_SPARKS[i].z = (Trig_Cos(angle) * amplitude) >> 2;
    JUDGMENT_SPARKS[i].y = INTtoFIXED((Random16() & 63) + 32);
    JUDGMENT_SPARKS[i].velocityX = speed;
    JUDGMENT_SPARKS[i].velocityY = speed;
    JUDGMENT_SPARKS[i].variant = angle;
  }

  EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
  REG_BG2CNT = BGCNT_PRIORITY(0) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
               BGCNT_SCREEN_BASE(7);

  // The gathering.
  for (frame = 0; frame != JUDGMENT_GATHER_FRAMES &&
                  (frame <= 4 || !(gKeysRepeat & (KEY_A | KEY_B)));
       frame++) {
    blitters = EFFECT_BLITTERS;
    if (frame == JUDGMENT_BEAM_FRAME - 1) {
      Iwram_FillWords(canvas, CANVAS_BYTES,
                      CANVAS_FILL_WORD(JUDGMENT_CRASH_LEVEL));
      Sound_PlayFar(SE_CRASH);
    }
    if (frame == 80) {
      Sound_PlayFar(SE_SWIRL);
    }

    // DEAD: the sparks stage never runs, as frame starts at 0 and only grows;
    // the ROM keeps its signed test against 0 and the whole stage.
    if (frame < 0) {
      EFFECT_VIEW_FROM_CAMERA(camera);
      for (i = 0; i != JUDGMENT_SPARK_COUNT; i++) {
        s32 z;
        s32 size;

        Particle_AdvanceWithGravity3D(&JUDGMENT_SPARKS[i], 60,
                                      -toFIXED(1) / 64);
        Projection_Point(&JUDGMENT_SPARKS[i], &projectedPosition);
        projectedPosition.x >>= 1;
        projectedPosition.y -= 120;
        CLAMP(projectedPosition.depth, JUDGMENT_SPARK_DEPTH_NEAR,
              JUDGMENT_SPARK_DEPTH_FAR);
        z = projectedPosition.depth - JUDGMENT_SPARK_DEPTH_NEAR;
        projectedPosition.depth = z;
        size = (z / 100) + 1;
        if (minimumDepth > z) {
          minimumDepth = z;
        }
        if (maximumDepth < z) {
          maximumDepth = z;
        }
        EFFECT_DRAW_MOTE(draw[0], canvas, motes, projectedPosition.x,
                         projectedPosition.y, size);
        if (frame == 64) {
          JUDGMENT_SPARKS[i].velocityX +=
              (Trig_Sin(JUDGMENT_SPARKS[i].variant) *
               JUDGMENT_SPARK_AMPLITUDE) >>
              3;
          JUDGMENT_SPARKS[i].velocityY +=
              (Trig_Cos(JUDGMENT_SPARKS[i].variant) *
               JUDGMENT_SPARK_AMPLITUDE) >>
              3;
        }
      }
    }

    if (frame == 72) {
      work->fadeFrames = BACKGROUND_DARKEN_FRAMES;
      work->fadeStep = 0;
    }

    // The first half of the motes gather into Judgment's shield, four more
    // each frame from frame 80.
    anchor[2] = 0;
    anchor[1] = 0;
    for (i = 0; i != JUDGMENT_MOTES / 2; i++) {
      if (frame > i / 4 + 80 && PARTICLES[i].x > 0) {
        s32 size;

        Matrix_Initialize();
        Matrix_RotateZ(PARTICLES[i].velocityY);
        Matrix_RotateX(PARTICLES[i].velocityX);
        Matrix_RotateY(PARTICLES[i].velocityY + frame * (TRIG_TURN / 128));
        anchor[0] = PARTICLES[i].x;
        PARTICLES[i].x -= 2;
        if (PARTICLES[i].x < 0) {
          PARTICLES[i].x = 0;
        }
        Projection_Point(anchor, &projectedPosition);
        CLAMP(projectedPosition.depth, -JUDGMENT_MOTE_DEPTH,
              JUDGMENT_MOTE_DEPTH);
        projectedPosition.depth += JUDGMENT_MOTE_DEPTH;
        size = projectedPosition.depth / JUDGMENT_MOTE_SIZE_DEPTH + 2;
        projectedPosition.x += JUDGMENT_BURST_X;
        projectedPosition.y += JUDGMENT_BURST_Y;
        EFFECT_DRAW_MOTE(draw[0], canvas, motes, projectedPosition.x,
                         projectedPosition.y, size);
      }
    }

    // The formation slows to a stop, jumps back and drifts forward again.
    pair = unitScale;
    place.groundHeight = 0;
    place.height = 0;
    for (i = 0; i != JUDGMENT_FORMATION_OBJECTS; i++) {
      place.x = INTtoFIXED(i % JUDGMENT_FORMATION_COLUMNS *
                           JUDGMENT_FORMATION_SPRITE_SIZE) +
                toFIXED(76);
      place.y = INTtoFIXED(i / JUDGMENT_FORMATION_COLUMNS *
                           JUDGMENT_FORMATION_SPRITE_SIZE) +
                depth;
      Actor_ApplyProjectedPlacementFar(work->sprites[i], &place, &pair, 0);
    }

    depth += zoom;
    if (frame <= 47) {
      zoom -= FIXED_ONE / 4;
    }
    if (frame > 32) {
      zoom = zoom * 60 / DAMPING_SCALE;
    }
    if (frame == JUDGMENT_BEAM_FRAME) {
      zoom = -toFIXED(8);
    }
    if (frame == 146) {
      zoom = FIXED_ONE;
    }

    if (frame == 72) {
      for (i = 0; i != JUDGMENT_FORMATION_OBJECTS; i++) {
        Sprite_StartAnimationFar(work->sprites[i],
                                 i + JUDGMENT_POSE_SHIELD_SWINGING);
      }
    }
    if (frame == 76) {
      for (i = 0; i != JUDGMENT_FORMATION_OBJECTS; i++) {
        Sprite_StartAnimationFar(work->sprites[i],
                                 i + JUDGMENT_POSE_SHIELD_FORWARD);
      }
    }

    if (frame >= 116 && frame < JUDGMENT_BEAM_FRAME) {
      s32 high;
      s32 picture;
      s32 n = (frame - 116) / 4;
      if (n > 6) {
        n = 6;
      }
      high = gJudgment_BurstHeights[n];
      picture = gJudgment_BurstPictures[n] + JUDGMENT_SHEET_BURST;

      EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS,
                          COMPOSITE_LIGHTEN);
      EFFECT_BLITTERS[HEAP_SLOT_RAM_CODE_PAIRED](
          canvas, work->sheet + picture, JUDGMENT_BURST_X - high,
          JUDGMENT_BURST_Y - high * 2, high, high * 2);
      HeapSlot_Release(HEAP_SLOT_RAM_CODE_PAIRED);
      EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
                          EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                          COMPOSITE_LIGHTEN);
      EFFECT_BLITTERS[HEAP_SLOT_RAM_CODE_PAIRED](
          canvas, work->sheet + picture, JUDGMENT_BURST_X,
          JUDGMENT_BURST_Y - high * 2, high, high * 2);
      HeapSlot_Release(HEAP_SLOT_RAM_CODE_PAIRED);
      EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
                          EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_Y,
                          COMPOSITE_LIGHTEN);
      EFFECT_BLITTERS[HEAP_SLOT_RAM_CODE_PAIRED](
          canvas, work->sheet + picture, JUDGMENT_BURST_X - high,
          JUDGMENT_BURST_Y, high, high * 2);
      HeapSlot_Release(HEAP_SLOT_RAM_CODE_PAIRED);
      EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
                          EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X |
                              COMPOSITE_FLIP_Y,
                          COMPOSITE_LIGHTEN);
      EFFECT_BLITTERS[HEAP_SLOT_RAM_CODE_PAIRED](
          canvas, work->sheet + picture, JUDGMENT_BURST_X, JUDGMENT_BURST_Y,
          high, high * 2);
      HeapSlot_Release(HEAP_SLOT_RAM_CODE_PAIRED);

      for (i = 0; i != 6; i++) {
        s32 k = i & 3;
        s32 angle = Random16() & TRIG_ANGLE_MASK;
        s32 x = FIXEDtoINT(Trig_Sin(angle) * JUDGMENT_SHARD_RADIUS) -
                gJudgment_ShardWidths[k] / 2;
        s32 y = FIXEDtoINT(Trig_Cos(angle) * JUDGMENT_SHARD_RADIUS) -
                gJudgment_ShardHeights[k] / 2;
        draw[0](canvas, work->sheet + gJudgment_ShardPictures[k],
                x + JUDGMENT_BURST_X, y + JUDGMENT_BURST_Y,
                gJudgment_ShardWidths[k], gJudgment_ShardHeights[k]);
      }
    }

    if (frame >= JUDGMENT_BEAM_FRAME) {
      // Two beams fall 16 pixels a frame onto the shield, and a ring of
      // motes spreads from it 10 pixels a frame.
      s32 top = (frame << 4) - 2272;
      s32 n;
      s32 r;
      EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS,
                          COMPOSITE_ADDITIVE);
      blitters[HEAP_SLOT_RAM_CODE_PAIRED](
          canvas, work->sheet, JUDGMENT_BURST_X - JUDGMENT_BEAM_WIDTH, top,
          JUDGMENT_BEAM_WIDTH, JUDGMENT_BEAM_HEIGHT);
      HeapSlot_Release(HEAP_SLOT_RAM_CODE_PAIRED);
      EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
                          EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                          COMPOSITE_ADDITIVE);
      blitters[HEAP_SLOT_RAM_CODE_PAIRED](canvas, work->sheet, JUDGMENT_BURST_X,
                                          top, JUDGMENT_BEAM_WIDTH,
                                          JUDGMENT_BEAM_HEIGHT);
      HeapSlot_Release(HEAP_SLOT_RAM_CODE_PAIRED);

      n = (frame - JUDGMENT_BEAM_FRAME) / 2;
      r = frame * 10 - 1424;
      work->transferValue = CANVAS_FADE_THREE_QUARTERS;
      if (n <= 6) {
        s32 picture = n * MOTE_SET_BYTES + gBattleEffect_MotePictures[7] +
                      JUDGMENT_SHEET_BYTES;
        s32 high = 8;
        EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS,
                            COMPOSITE_LIGHTEN);
        blit = blitters[HEAP_SLOT_RAM_CODE_PAIRED];
        for (i = 0; i != JUDGMENT_RING_MOTES; i++) {
          s32 angle = i * (TRIG_TURN / JUDGMENT_RING_MOTES);
          // The ring lies flat, half as tall as it is wide.
          s32 x = FIXEDtoINT(r * Trig_Sin(angle)) - high / 2;
          s32 y = FIXEDtoINT(r * Trig_Cos(angle)) >> 1;
          blit(canvas, work->sheet + picture, x + JUDGMENT_BURST_X,
               y + JUDGMENT_BURST_Y - high, high, high * 2);
        }
        HeapSlot_Release(HEAP_SLOT_RAM_CODE_PAIRED);
      }
    }

    EFFECT_PRESENT_CANVAS(work);
  }

  // The sweep's setup.
  Iwram_FillWords(canvas, CANVAS_BYTES, 0);

  for (i = 0; i != JUDGMENT_FORMATION_OBJECTS; i++) {
    Sprite_DestroyFar(work->sprites[i]);
  }

  for (i = 0; i != JUDGMENT_DEBRIS_OBJECTS; i++) {
    struct Sprite *sprite = Sprite_CreateFar(SPRITE_BATTLE_ROCKS);
    work->sprites[i] = sprite;
    if (sprite != 0) {
      sprite->flags = 0;
      Sprite_StartAnimationFar(sprite, (i % 3));
      work->sprites[i]->image.priority = 0;
    }
  }

  HeapSlot_Release(HEAP_SLOT_RAM_CODE);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                      COMPOSITE_LIGHTEN);
  draw[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
  Resource_LoadAndDecompress(RESOURCE_BLUE_CRESCENT_SHEET,
                             work->sheet + JUDGMENT_SHEET_FLARE_A, 1, 1);

  for (i = 0; i != MOTE_SET_BYTES; i++) {
    // The sweep's motes are dimmer: a level in the brighter half drops by
    // half the levels, and any other but 0 becomes 1.
    if (motes[i] > (CANVAS_LEVEL_BRIGHTEST + 1) / 2) {
      motes[i] -= (CANVAS_LEVEL_BRIGHTEST + 1) / 2;
    } else if (motes[i] != 0) {
      motes[i] = 1;
    }
  }

  // Stop the battle background fade.
  work->fadeFrames = 0;
  work->fadeStep = 0;
  BattleBackground_LoadFar(BATTLE_BACKGROUND_FIXED,
                           RESOURCE_AERIAL_VIEW_BACKDROP, 0);
  REG_BG2PA = Q_8_8(1);
  REG_BG2X = -(JUDGMENT_SWEEP_CANVAS_X << 8);
  REG_BG2CNT = BGCNT_PRIORITY(0) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
               BGCNT_SCREEN_BASE(7);

  Dma_Fill32Cell(fill, 0, canvas, CANVAS_BYTES);

  cx = JUDGMENT_FLARE_X;
  cy = JUDGMENT_AERIAL_RINGS_Y;
  squash = 6;
  shift = 0;
  ring = 0;
  rings = 2;
  EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);

  Dma_Fill32Cell(fill, shift, work, JUDGMENT_WAVE_SIZE * JUDGMENT_WAVE_SIZE);

  // The sweep, on its own frame clock.
  for (frame = 0; frame != JUDGMENT_SWEEP_FRAMES; frame++) {
    if (frame == 66) {
      Sound_PlayFar(SE_CRASH);
      Sound_PlayFar(SE_RUMBLE);
    }
    if (frame == 155) {
      Sound_PlayFar(SE_WAVE);
    }
    if (frame == 217) {
      Sound_PlayFar(SE_SWITCH_MECHANISM);
    }
    if (frame == JUDGMENT_SWEEP_RETURN) {
      Sound_PlayFar(SE_GATE_MOVE);
    }
    if (frame == 300) {
      EFFECT_SHOW_DAMAGE(SE_CRASH);
    }

    if (gKeysRepeat & (KEY_A | KEY_B)) {
      if (frame >= 5 && frame < JUDGMENT_SWEEP_BEACH) {
        frame = JUDGMENT_SWEEP_BEACH;
        IO_WRITE_QUEUE_PUSH_IME(ime, (u32)&REG_BG2PA, Q_8_8(0.5),
                                IO_WRITE_HALFWORD);
        IO_WRITE_QUEUE_PUSH_IME(ime, (u32)&REG_BG2X, 0, IO_WRITE_WORD);
        Resource_LoadAndDecompress(RESOURCE_EXPLOSION_SHEET,
                                   work->sheet + JUDGMENT_SHEET_EXPLOSION, 1,
                                   0);
      } else if (frame >= JUDGMENT_SWEEP_BEACH + 5 &&
                 frame < JUDGMENT_SWEEP_FOREST) {
        frame = JUDGMENT_SWEEP_FOREST;
      } else if (frame >= JUDGMENT_SWEEP_FOREST + 5 &&
                 frame < JUDGMENT_SWEEP_RETURN) {
        frame = JUDGMENT_SWEEP_RETURN;
      }
    }

    if (frame == JUDGMENT_SWEEP_EXPLOSION) {
      IO_WRITE_QUEUE_PUSH_IME(ime, (u32)&REG_BG2PA, Q_8_8(0.5),
                              IO_WRITE_HALFWORD);
      IO_WRITE_QUEUE_PUSH_IME(ime, (u32)&REG_BG2X, 0, IO_WRITE_WORD);
      Dma_Fill32Cell(fill, 0, canvas, CANVAS_BYTES);
      Resource_LoadAndDecompress(RESOURCE_EXPLOSION_SHEET,
                                 work->sheet + JUDGMENT_SHEET_EXPLOSION, 1, 0);
      Resource_LoadAndDecompress(RESOURCE_OVERLAY_IMAGE_A,
                                 work->sheet + JUDGMENT_SHEET_FLASH_SMALL, 0,
                                 0);
    }

    if (frame == 66) {
      // Invert the battle background's colours, background palettes 6 to 13.
      u16 *pal = (u16 *)&BG_PLTT_COLOR(6, 0);
      for (i = 0; i != BATTLE_BACKGROUND_COLORS; i++, pal++) {
        *pal ^= RGB_WHITE;
      }
    }

    if (frame == 69) {
      Dma_Fill32Cell(fill, CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST), canvas,
                     CANVAS_BYTES);
    }

    // Undo the inversion, rewriting the battle background's palette from the
    // battle's copy with every channel 7 levels darker.
    if (frame == 70) {
      BattleBackground_SetModeFar(BATTLE_BACKGROUND_FIXED,
                                  RESOURCE_AERIAL_VIEW_BACKDROP, 7);
    }

    // The beach: the rings restart, four a frame.
    if (frame == JUDGMENT_SWEEP_BEACH) {
      cx = JUDGMENT_BEACH_RINGS_X;
      cy = JUDGMENT_BEACH_RINGS_Y;
      ring = 0;
      rings = 4;
      squash = 8;
      Dma_Fill32Cell(fill, 0, work, JUDGMENT_WAVE_SIZE * JUDGMENT_WAVE_SIZE);
      Dma_Fill32Cell(fill, CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST), canvas,
                     CANVAS_BYTES);
      BattleBackground_LoadFar(BATTLE_BACKGROUND_FIXED, RESOURCE_BEACH_BACKDROP,
                               0);
    }

    // The forest.
    if (frame == JUDGMENT_SWEEP_FOREST) {
      Dma_Fill32Cell(fill, 0, work, JUDGMENT_WAVE_SIZE * JUDGMENT_WAVE_SIZE);
      Dma_Fill32Cell(fill, CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST), canvas,
                     CANVAS_BYTES);
      BattleBackground_LoadFar(BATTLE_BACKGROUND_FIXED,
                               RESOURCE_FOREST_BACKDROP, 0);
      for (i = 0; i != JUDGMENT_DEBRIS_OBJECTS; i++) {
        particle = &work->particles[i];
        particle->x = INTtoFIXED((Random16() & 63) + 32);
        particle->y = INTtoFIXED((Random16() & 31) + 96);
        particle->velocityY = ((Random16() & 15) + 16) << 15;
        particle->z = toFIXED(1) / 2 - (particle->x >> 7);
        particle->variant = 1;
      }
    }

    if (frame == JUDGMENT_SWEEP_RETURN) {
      // Back to the battle.
      BattleEffect_SetupBlendedDisplay();
      Dma_Fill32Cell(fill, 0, work, JUDGMENT_WAVE_SIZE * JUDGMENT_WAVE_SIZE);
      Dma_Fill32Cell(fill, CANVAS_FILL_WORD(1), canvas, CANVAS_BYTES);
      view->busy = 0;
      REG_BLDALPHA = EFFECT_BLEND_ADD;
      for (i = 0; i != JUDGMENT_DEBRIS_OBJECTS; i++) {
        particle = &work->particles[i];
        particle->x = INTtoFIXED((Random16() & 63) + 256);
        particle->y = INTtoFIXED((Random16() & 31) + 96);
        particle->velocityY = ((Random16() & 15) + 16) << 15;
        particle->z = 0;
      }
      shift = JUDGMENT_RETURN_BLAST_X;
      EFFECT_SET_TRANSFER(work, CANVAS_COPY, 0);
    }

    // The canvas's colours brighten to white.
    if (frame >= JUDGMENT_SWEEP_RETURN) {
      volatile u16 *pal = &BG_PLTT_COLOR(0, 1);
      for (i = 0; i != SHEET_PALETTE_COLORS - 1; i++, pal++) {
        s32 c = *pal;
        s32 b = RGB_BLUE((u16)c);
        s32 g = RGB_GREEN((u16)c);
        s32 r = RGB_RED(c);
        b++;
        g++;
        r++;
        if (b > RGB_LEVEL_MAX) {
          b = RGB_LEVEL_MAX;
        }
        if (g > RGB_LEVEL_MAX) {
          g = RGB_LEVEL_MAX;
        }
        if (r > RGB_LEVEL_MAX) {
          r = RGB_LEVEL_MAX;
        }
        *pal = RGB(r, g, b);
      }
    }

    if (frame == 182) {
      Dma_Fill32Cell(fill, CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST), canvas,
                     CANVAS_BYTES);
    }

    // A flare falls onto the aerial view, drawn as four mirrored quarters, its
    // picture flickering every 4 frames.
    if (frame < JUDGMENT_SWEEP_EXPLOSION) {
      u8 *sheet;
      s32 top = frame - 4;
      if ((frame & 7) < 4) {
        sheet = work->sheet + JUDGMENT_SHEET_FLARE_A;
      } else {
        sheet = work->sheet + JUDGMENT_SHEET_FLARE_B;
      }
      EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS,
                          COMPOSITE_LIGHTEN);
      EFFECT_BLITTERS[HEAP_SLOT_RAM_CODE_PAIRED](
          canvas, sheet, JUDGMENT_FLARE_X - JUDGMENT_FLARE_WIDTH,
          top - JUDGMENT_FLARE_HEIGHT, JUDGMENT_FLARE_WIDTH,
          JUDGMENT_FLARE_HEIGHT);
      HeapSlot_Release(HEAP_SLOT_RAM_CODE_PAIRED);
      EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
                          EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                          COMPOSITE_LIGHTEN);
      EFFECT_BLITTERS[HEAP_SLOT_RAM_CODE_PAIRED](
          canvas, sheet, JUDGMENT_FLARE_X - 1, top - JUDGMENT_FLARE_HEIGHT,
          JUDGMENT_FLARE_WIDTH, JUDGMENT_FLARE_HEIGHT);
      HeapSlot_Release(HEAP_SLOT_RAM_CODE_PAIRED);
      EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
                          EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_Y,
                          COMPOSITE_LIGHTEN);
      EFFECT_BLITTERS[HEAP_SLOT_RAM_CODE_PAIRED](
          canvas, sheet, JUDGMENT_FLARE_X - JUDGMENT_FLARE_WIDTH, top - 1,
          JUDGMENT_FLARE_WIDTH, JUDGMENT_FLARE_HEIGHT);
      HeapSlot_Release(HEAP_SLOT_RAM_CODE_PAIRED);
      EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
                          EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X |
                              COMPOSITE_FLIP_Y,
                          COMPOSITE_LIGHTEN);
      EFFECT_BLITTERS[HEAP_SLOT_RAM_CODE_PAIRED](
          canvas, sheet, JUDGMENT_FLARE_X - 1, top - 1, JUDGMENT_FLARE_WIDTH,
          JUDGMENT_FLARE_HEIGHT);
      HeapSlot_Release(HEAP_SLOT_RAM_CODE_PAIRED);
    }

    if (frame >= 64 && frame < 66) {
      EFFECT_DRAW_PICTURE(draw[0], canvas, work->sheet,
                          JUDGMENT_SHEET_FLASH_SMALL, 52, 51);
    }
    if (frame >= 66 && frame < 68) {
      EFFECT_DRAW_PICTURE(draw[0], canvas, work->sheet,
                          JUDGMENT_SHEET_FLASH_LARGE, 48, 40);
    }
    if (frame >= 68 && frame < 76) {
      s32 d = 76 - frame;
      EFFECT_DRAW_PICTURE(draw[0], canvas, work->sheet,
                          JUDGMENT_SHEET_FLASH_GLOW, 49 - d / 2, 38 - d);
      EFFECT_DRAW_PICTURE(draw[0], canvas, work->sheet,
                          JUDGMENT_SHEET_FLASH_GLOW, 49 - d / 2, d + 38);
      EFFECT_DRAW_PICTURE(draw[0], canvas, work->sheet,
                          JUDGMENT_SHEET_FLASH_GLOW, d / 2 + 49, 38 - d);
      EFFECT_DRAW_PICTURE(draw[0], canvas, work->sheet,
                          JUDGMENT_SHEET_FLASH_GLOW, d / 2 + 49, d + 38);
    }
    if (frame >= 78 && frame < 80) {
      EFFECT_DRAW_PICTURE(draw[0], canvas, work->sheet,
                          JUDGMENT_SHEET_FLASH_SMALL, 52, 51);
    }
    if (frame >= 80 && frame < 82) {
      EFFECT_DRAW_PICTURE(draw[0], canvas, work->sheet,
                          JUDGMENT_SHEET_FLASH_LARGE, 48, 40);
    }
    if (frame >= 82 && frame < 86) {
      s32 d = frame * 2 - 164;
      EFFECT_DRAW_PICTURE(draw[0], canvas, work->sheet,
                          JUDGMENT_SHEET_FLASH_GLOW, 49 - d / 4, 38 - d);
      EFFECT_DRAW_PICTURE(draw[0], canvas, work->sheet,
                          JUDGMENT_SHEET_FLASH_GLOW, 49 - d / 4, d + 38);
      EFFECT_DRAW_PICTURE(draw[0], canvas, work->sheet,
                          JUDGMENT_SHEET_FLASH_GLOW, d / 4 + 49, 38 - d);
      EFFECT_DRAW_PICTURE(draw[0], canvas, work->sheet,
                          JUDGMENT_SHEET_FLASH_GLOW, d / 4 + 49, d + 38);
    }

    if (frame >= 72 && frame < 76) {
      s32 radius = (frame - 71) * 16;
      s32 size = MOTE_SIZE_MAX;
      for (i = 0; i != JUDGMENT_FLASH_RING_MOTES; i++) {
        s32 angle = i * (TRIG_TURN / JUDGMENT_FLASH_RING_MOTES);
        s32 x = FIXEDtoINT(Trig_Sin(angle) * radius) - (size >> 1);
        s32 y = FIXEDtoINT(Trig_Cos(angle) * radius) >> 1;
        draw[0](canvas, motes + gBattleEffect_MotePictures[size - 1], x + 60,
                y + 50, size, size * 2);
      }
    }

    if (frame > 85) {
      // More rings each frame, each one pixel wider than the last.
      if (frame < JUDGMENT_SWEEP_FOREST) {
        for (i = 0; i != rings; i++, ring++) {
          s32 radius = ring;
          s32 slope = 0;
          s32 err = ring;
          while (radius >= slope) {
            s32 half;
            s32 halfSlope;
            s32 left;
            s32 right;
            s32 top;
            s32 bottom;
            s32 dx;

            half = radius / 2;
            dx = (half * stretch) / 8;
            right = cx + dx;
            /* FAKEMATCH(dead): a branch whose store the next line overwrites.
             */
            if (dx > cx)
              left = 0;
            left = cx - dx;
            bottom = cy + (slope * squash) / 8;
            top = cy - (slope * stretch) / 8;
            if (top < 0)
              top = 0;
            if (bottom > JUDGMENT_WAVE_SIZE - 1)
              bottom = JUDGMENT_WAVE_SIZE - 1;
            if (left < 0)
              left = 0;
            if (right > JUDGMENT_WAVE_SIZE - 1)
              right = JUDGMENT_WAVE_SIZE - 1;
            JUDGMENT_PLOT_RING_POINTS(left, right, top, bottom);
            right = cx + half + 1;
            left = cx - half + 1;
            if (left < 0)
              left = 0;
            if (right > JUDGMENT_WAVE_SIZE - 1)
              right = JUDGMENT_WAVE_SIZE - 1;
            JUDGMENT_PLOT_RING_POINTS(left, right, top, bottom);

            halfSlope = slope / 2;
            dx = (halfSlope * stretch) / 8;
            right = cx + dx;
            /* FAKEMATCH(dead): a branch whose store the next line overwrites.
             */
            if (dx > cx)
              left = 0;
            left = cx - dx;
            bottom = cy + (radius * squash) / 8;
            top = cy - (radius * stretch) / 8;
            if (left < 0)
              left = 0;
            if (right > JUDGMENT_WAVE_SIZE - 1)
              right = JUDGMENT_WAVE_SIZE - 1;
            if (top < 0)
              top = 0;
            if (bottom > JUDGMENT_WAVE_SIZE - 1)
              bottom = JUDGMENT_WAVE_SIZE - 1;
            JUDGMENT_PLOT_RING_POINTS(left, right, top, bottom);
            right = cx + halfSlope + 1;
            left = cx - halfSlope + 1;
            if (left < 0)
              left = 0;
            if (right > JUDGMENT_WAVE_SIZE - 1)
              right = JUDGMENT_WAVE_SIZE - 1;
            JUDGMENT_PLOT_RING_POINTS(left, right, top, bottom);

            err = err - slope * 2 - 1;
            if (err < 0) {
              err = err + radius * 2 - 2;
              radius--;
            }
            slope++;
          }
        }
      }
    }

    // The explosion: nine blasts fan out below where the flare struck.
    if (frame >= 86 && frame < JUDGMENT_SWEEP_BEACH) {
      s32 angle = -DEGtoANG(90);
      for (i = 0; i != 9; i++, angle += TRIG_TURN / 16) {
        s32 x = FIXEDtoINT((frame - 86) * Trig_Sin(angle));
        s32 y = FIXEDtoINT((frame - 70) * Trig_Cos(angle));
        draw[0](canvas,
                work->sheet +
                    ((frame / 2 + i) % 4) * JUDGMENT_EXPLOSION_PICTURE_BYTES +
                    JUDGMENT_SHEET_EXPLOSION,
                x + 44, y + 17, JUDGMENT_EXPLOSION_WIDTH,
                JUDGMENT_EXPLOSION_HEIGHT);
      }
    }

    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS,
                        COMPOSITE_ADDITIVE);
    draw[1] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED);
    if (frame > 85) {
      draw[1](canvas, work->sheet, 0, 0, JUDGMENT_WAVE_SIZE,
              JUDGMENT_WAVE_SIZE);
    }

    if (frame >= JUDGMENT_SWEEP_RETURN) {
      shift -= 8;
    }

    // DEAD: never true, as the sweep's last frame is JUDGMENT_SWEEP_FRAMES - 1;
    // the ROM keeps the test and the tint.
    if (frame >= JUDGMENT_SWEEP_FRAMES) {
      EFFECT_FOR_EACH_TARGET(i, work->args) {
        BattleEffect_TintBattler(work->args->targets[i], SPRITE_COLOR_BLACK,
                                 POSE_DAMAGE, TINT_KEEP, 0);
      }
    }

    if (frame == 238) {
      Dma_Fill32Cell(fill, CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST), canvas,
                     CANVAS_BYTES);
    }

    if (frame >= JUDGMENT_SWEEP_FOREST && frame < JUDGMENT_SWEEP_RETURN) {
      gBgScroll[1].x += 8;
    }

    if (frame >= 246 && frame < JUDGMENT_SWEEP_RETURN) {
      shift += 8;
      EFFECT_SET_TRANSFER(work, CANVAS_COPY, CANVAS_FADE_THREE_QUARTERS);
    }

    // Debris rises from the field.
    if (frame >= JUDGMENT_SWEEP_FOREST) {
      debrisPlace.groundHeight = 0;
      debrisPlace.height = 0;
      for (i = 0; i != JUDGMENT_DEBRIS_OBJECTS; i++) {
        particle = &work->particles[i];
        if (particle->variant != 0) {
          scale.x = particle->z + (particle->y >> 7);
          if (scale.x < FIXED_ONE / 32) {
            scale.x = FIXED_ONE / 32;
          }
          scale.y = scale.x;
          debrisPlace.x = particle->x;
          debrisPlace.height = toFIXED(255);
          debrisPlace.y = particle->y + toFIXED(255);
          Actor_ApplyProjectedPlacementFar(work->sprites[i], &debrisPlace,
                                           &scale, 0);
          particle->y -= particle->velocityY;
          if (particle->y < toFIXED(16)) {
            if (frame < JUDGMENT_SWEEP_RETURN) {
              particle->x = INTtoFIXED((Random16() & 63) + shift + 32);
            } else {
              particle->x = INTtoFIXED((Random16() & 63) + shift - 32);
            }
            if (frame > 301) {
              particle->variant = 0;
            } else if (frame > 245) {
              particle->y = toFIXED(96);
            } else {
              particle->y = toFIXED(192) - particle->x;
            }
          }
        }
      }

      if (frame < JUDGMENT_SWEEP_RETURN) {
        for (i = 0; i != 8; i++) {
          s32 x =
              (i & 1) * 20 - (Random16() & 15) + (i / 2) * 4 + shift / 2 - 16;
          s32 y = (i / 2) * 32 - (Random16() & 15);
          draw[1](canvas,
                  work->sheet +
                      ((frame / 2 + i) % 4) * JUDGMENT_EXPLOSION_PICTURE_BYTES +
                      JUDGMENT_SHEET_EXPLOSION,
                  x, y, JUDGMENT_EXPLOSION_WIDTH, JUDGMENT_EXPLOSION_HEIGHT);
        }
      } else {
        for (i = 0; i != 8; i++) {
          s32 x =
              (i & 1) * 20 + (Random16() & 15) - (i / 2) * 4 + shift / 2 - 16;
          s32 y = (i / 2) * 32 - (Random16() & 15);
          draw[1](canvas,
                  work->sheet +
                      ((frame / 2 + i) % 4) * JUDGMENT_EXPLOSION_PICTURE_BYTES +
                      JUDGMENT_SHEET_EXPLOSION,
                  x, y, JUDGMENT_EXPLOSION_WIDTH, JUDGMENT_EXPLOSION_HEIGHT);
        }
      }
    }

    HeapSlot_Release(HEAP_SLOT_RAM_CODE_PAIRED);

    // The battle background shakes up and down.
    if (frame >= JUDGMENT_SWEEP_EXPLOSION) {
      s32 shake = (Random16() & 3) + 30;

      gBgScroll[1].y = shake;
    }

    EFFECT_PRESENT_CANVAS(work);
  }

  for (i = 0; i != JUDGMENT_DEBRIS_OBJECTS; i++) {
    Sprite_DestroyFar(work->sprites[i]);
  }

  EFFECT_FINISH_CANVAS_ONE_BLITTER();
}

#undef JUDGMENT_SHEET_BYTES
#undef JUDGMENT_SPARKS
#undef JUDGMENT_CRASH_LEVEL
#undef JUDGMENT_MOTE_SIZE_DEPTH
#undef JUDGMENT_SPARK_AMPLITUDE
#undef JUDGMENT_AERIAL_RINGS_Y
#undef JUDGMENT_BEACH_RINGS_X
#undef JUDGMENT_BEACH_RINGS_Y
#undef JUDGMENT_RETURN_BLAST_X
#undef JUDGMENT_SHEET_TORNADO
#undef JUDGMENT_GATHER_FRAMES
#undef JUDGMENT_SWEEP_FRAMES
#undef JUDGMENT_SHEET_FLARE_B
#undef JUDGMENT_FORMATION_OBJECTS
#undef JUDGMENT_DEBRIS_OBJECTS
#undef JUDGMENT_SHEET_BURST
#undef JUDGMENT_SHEET_EXPLOSION
#undef JUDGMENT_SHEET_FLARE_A
#undef JUDGMENT_SHEET_FLASH_SMALL
#undef JUDGMENT_SHEET_FLASH_LARGE
#undef JUDGMENT_SHEET_FLASH_GLOW
#undef JUDGMENT_EXPLOSION_PICTURE_BYTES
#undef JUDGMENT_WAVE_SIZE
#undef JUDGMENT_FORMATION_COLUMNS
#undef JUDGMENT_FORMATION_SPRITE_SIZE
#undef JUDGMENT_POSE_SHIELD_SWINGING
#undef JUDGMENT_POSE_SHIELD_FORWARD
#undef JUDGMENT_BURST_X
#undef JUDGMENT_BURST_Y
#undef JUDGMENT_SHARD_RADIUS
#undef JUDGMENT_MOTE_DEPTH
#undef JUDGMENT_SPARK_DEPTH_NEAR
#undef JUDGMENT_SPARK_DEPTH_FAR
#undef JUDGMENT_SPARK_DEPTH_NONE
#undef JUDGMENT_RING_LEVEL
#undef JUDGMENT_SWEEP_CANVAS_X
#undef JUDGMENT_FLARE_X
