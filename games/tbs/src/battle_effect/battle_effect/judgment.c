/// @file
/// The Judgment summon.

#include "../battle_effect_internal.h"

/// The bytes of Judgment's work block before its dimmed mote copies.
#define JUDGMENT_SHEET_BYTES 10000

/// Judgment's work block, laid over struct BattleEffectWork from its start; the
/// sweep's wave picture later overwrites the gathering's part of it.
struct JudgmentWork {
  /// The gathering's pictures: the pink burst, the burst's quarters and the
  /// tornado sheet.
  u8 sheet[JUDGMENT_SHEET_BYTES];
  /// Eight copies of the mote sheet, copy i with every level capped at
  /// 64 - 7i; the gathering's ring of motes steps through them.
  u8 ramp[8][MOTE_SET_BYTES];
};

/// The gathering's 128 sparks, after its 128 motes in PARTICLES.
#define JUDGMENT_SPARKS (&PARTICLES[128])

#define JUDGMENT_GATHER_FRAMES 160 ///< frames of the gathering
#define JUDGMENT_SWEEP_FRAMES 320
#define JUDGMENT_FORMATION_OBJECTS 16 ///< sprites in the formation, 4x4
#define JUDGMENT_DEBRIS_OBJECTS 16    ///< debris sprites of the sweep
/// The burst's seven quarter pictures (OverlayImageB), after the pink burst's
/// 24x64 beam picture at the sheet's start.
#define JUDGMENT_SHEET_BURST (24 * 64)
/// The tornado sheet, after the burst's seven quarter pictures; its six glint
/// pictures are the burst's shards (gJudgment_ShardPictures counts from the
/// sheet's start).
#define JUDGMENT_SHEET_TORNADO                                                 \
  (JUDGMENT_SHEET_BURST + 2 * 4 + 3 * 6 + 4 * 8 + 6 * 12 + 10 * 20 + 11 * 22 + \
   12 * 24)
/// The explosion's four 32x54 pictures, loaded over the flares on the sweep's
/// frame 64.
#define JUDGMENT_SHEET_EXPLOSION 16384
/// The first 24x24 flare picture, from the crescent sheet the sweep loads.
#define JUDGMENT_SHEET_FLARE_A 16384
/// The second 24x24 flare picture.
#define JUDGMENT_SHEET_FLARE_B (JUDGMENT_SHEET_FLARE_A + 24 * 24)
// OverlayImageA's three flash pictures, a byte a pixel, one after another in
// the sheet: the 16x17 small flash, the 24x41 large flash and the 22x44 glow.
#define JUDGMENT_SHEET_FLASH_SMALL 24576
#define JUDGMENT_SHEET_FLASH_LARGE (JUDGMENT_SHEET_FLASH_SMALL + 16 * 17)
#define JUDGMENT_SHEET_FLASH_GLOW (JUDGMENT_SHEET_FLASH_LARGE + 24 * 41)
/// Each explosion picture is 32x54.
#define JUDGMENT_EXPLOSION_PICTURE_BYTES (32 * 54)
#define JUDGMENT_WAVE_SIZE 120 ///< the wave picture is 120x120
/// DMA word that fills the 128x128 canvas.
#define JUDGMENT_DMA_FILL_CANVAS DMA_FILL_WORDS(CANVAS_BYTES / 4)
/// DMA word that fills the 120x120 wave picture.
#define JUDGMENT_DMA_FILL_WAVE                                                 \
  DMA_FILL_WORDS((JUDGMENT_WAVE_SIZE * JUDGMENT_WAVE_SIZE) / 4)

/// Sets a ring's four mirrored points (left or right, top or bottom) of the
/// wave picture to level 20.
#define JUDGMENT_PLOT_RING_POINTS(left, right, top, bottom)                    \
  (work->sheet[(bottom) * JUDGMENT_WAVE_SIZE + (right)] = 20,                  \
   work->sheet[(top) * JUDGMENT_WAVE_SIZE + (right)] = 20,                     \
   work->sheet[(bottom) * JUDGMENT_WAVE_SIZE + (left)] = 20,                   \
   work->sheet[(top) * JUDGMENT_WAVE_SIZE + (left)] = 20)

/// The battle wipes away to a violet sky where Judgment flies forward while
/// motes gather into it; it bursts in a flash, and the blast falls on the land
/// below and spreads in rings of light.
///
/// Abilities: Judgment.
void BattleEffect_RunJudgment(struct BattleEffectParams *args) {
  static struct SpriteScale unitScale = {{{Q16_ONE, Q16_ONE}}};
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
  u8 *ramp;
  struct BattleCamera *camera;
  s32 depth;
  s32 zoom;
  s32 minimumDepth;
  s32 maximumDepth;
  void **heap;
  DrawRectangle *blitters;
  s32 cx;
  s32 cy;
  s32 squash;
  s32 sx = 8;
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
  ramp = HEAP_SLOT_FROM(heap, HEAP_SLOT_BATTLE_CANVAS, HEAP_SLOT_BATTLE_SHEET);
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
  // Setup: wipe the battle away, change the battle background behind it, wipe
  // back.
  BattleEffect_WipeCanvas(CANVAS_WIPE_DOWN, CANVAS_WIPE_FILL);
  BattleEffect_SelectLivingTargets(work->args);
  BattleEffect_SpawnObjects(JUDGMENT_FORMATION_OBJECTS, SPRITE_JUDGMENT, 1);
  gProjection.centerY = 240;
  wait(1);
  BattleBackground_LoadFar(BATTLE_BACKGROUND_FIXED,
                           (s32)&ResourceId_VioletSkyBackdrop, 0);
  view->busy = 1;
  gBgScroll[1].x = 0;
  BattleEffect_WipeCanvas(CANVAS_WIPE_DOWN, CANVAS_WIPE_CLEAR);
  EFFECT_SHOW_WIDE_CANVAS(16);
  depth = -px(64);
  zoom = px(8);
  minimumDepth = 0xffff;
  maximumDepth = 0;
  Resource_LoadAndDecompress((s32)&ResourceId_PinkBurstSheet, work->sheet, 1,
                             1);
  Resource_LoadAndDecompress((s32)&ResourceId_OverlayImageB,
                             work->sheet + JUDGMENT_SHEET_BURST, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_TornadoSheet,
                             work->sheet + JUDGMENT_SHEET_TORNADO, 1, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, ramp, 0, 0);

  for (i = 0; i != 8; i++) {
    u8 *src = ramp;
    for (j = 0; j != MOTE_SET_BYTES; j++) {
      s32 v = *src++;
      EFFECT_CAP_LEVEL(v, 64 - i * 7);
      ((struct JudgmentWork *)work)->ramp[i][j] = v;
    }
  }

  Iwram_CopyWords((void *)BG_PLTT,
                  Resource_GetTableEntry((s32)&ResourceId_BlueCrescentSheet),
                  128);

  for (i = 0; i != 128; i++) {
    PARTICLES[i].x = (Random16() & 15) + 72;
    PARTICLES[i].velocityX = Random16() & TRIG_ANGLE_MASK;
    PARTICLES[i].velocityY = Random16() & 0xffff;
    PARTICLES[i].velocityY = Random16() & TRIG_ANGLE_MASK;
  }

  for (i = 0; i != 128; i++) {
    s32 angle = Random16() & TRIG_ANGLE_MASK;
    s32 amp = 255;
    s32 speed = 0;
    JUDGMENT_SPARKS[i].x = (Trig_Sin(angle) * amp) >> 2;
    JUDGMENT_SPARKS[i].z = (Trig_Cos(angle) * amp) >> 2;
    JUDGMENT_SPARKS[i].y = px((Random16() & 63) + 32);
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
    if (frame == 143) {
      Iwram_FillWords(canvas, CANVAS_BYTES, CANVAS_FILL_WORD(42));
      Sound_PlayFar(SE_CRASH);
    }
    if (frame == 80) {
      Sound_PlayFar(SE_SWIRL);
    }

    // Unused: the sparks stage, never drawn, as frame starts at 0 and only
    // grows; the ROM keeps its signed test against 0 and the whole stage.
    if (frame < 0) {
      EFFECT_VIEW_FROM_CAMERA(camera);
      for (i = 0; i != 128; i++) {
        s32 z;
        s32 size;

        Particle_AdvanceWithGravity3D(&JUDGMENT_SPARKS[i], 60, -px(1) / 64);
        Projection_Point(&JUDGMENT_SPARKS[i], &projectedPosition);
        projectedPosition.x >>= 1;
        projectedPosition.y -= 120;
        if (projectedPosition.depth < 100) {
          projectedPosition.depth = 100;
        }
        if (projectedPosition.depth > 900) {
          projectedPosition.depth = 900;
        }
        z = projectedPosition.depth - 100;
        projectedPosition.depth = z;
        size = (z / 100) + 1;
        if (minimumDepth > z) {
          minimumDepth = z;
        }
        if (maximumDepth < z) {
          maximumDepth = z;
        }
        EFFECT_DRAW_MOTE(draw[0], canvas, ramp, projectedPosition.x,
                         projectedPosition.y, size);
        if (frame == 64) {
          JUDGMENT_SPARKS[i].velocityX +=
              (Trig_Sin(JUDGMENT_SPARKS[i].variant) * 255) >> 3;
          JUDGMENT_SPARKS[i].velocityY +=
              (Trig_Cos(JUDGMENT_SPARKS[i].variant) * 255) >> 3;
        }
      }
    }

    if (frame == 72) {
      work->fadeFrames = 24;
      work->fadeStep = 0;
    }

    anchor[2] = 0;
    anchor[1] = 0;
    for (i = 0; i != 64; i++) {
      if (frame > i / 4 + 80 && PARTICLES[i].x > 0) {
        s32 size;

        Transform_Reset();
        Transform_ApplyRoll(PARTICLES[i].velocityY);
        Transform_ApplyPitch(PARTICLES[i].velocityX);
        Transform_ApplyYaw(PARTICLES[i].velocityY + frame * (TRIG_TURN / 128));
        anchor[0] = PARTICLES[i].x;
        PARTICLES[i].x -= 2;
        if (PARTICLES[i].x < 0) {
          PARTICLES[i].x = 0;
        }
        Projection_Point(anchor, &projectedPosition);
        if (projectedPosition.depth < -60) {
          projectedPosition.depth = -60;
        }
        if (projectedPosition.depth > 60) {
          projectedPosition.depth = 60;
        }
        projectedPosition.depth += 60;
        size = projectedPosition.depth / 20 + 2;
        projectedPosition.x += 60;
        projectedPosition.y += 80;
        EFFECT_DRAW_MOTE(draw[0], canvas, ramp, projectedPosition.x,
                         projectedPosition.y, size);
      }
    }

    // The formation slows to a stop, jumps back and drifts forward again.
    pair = unitScale;
    place.groundHeight = 0;
    place.height = 0;
    for (i = 0; i != JUDGMENT_FORMATION_OBJECTS; i++) {
      place.x = px(i % 4 * 32) + px(76);
      place.y = px(i / 4 * 32) + depth;
      Actor_ApplyProjectedPlacementFar(work->sprites[i], &place, &pair, 0);
    }

    depth += zoom;
    if (frame <= 47) {
      zoom -= Q16_ONE / 4;
    }
    if (frame > 32) {
      zoom = zoom * 60 / 64;
    }
    if (frame == 144) {
      zoom = -px(8);
    }
    if (frame == 146) {
      zoom = Q16_ONE;
    }

    if (frame == 72) {
      for (i = 0; i != JUDGMENT_FORMATION_OBJECTS; i++) {
        Sprite_StartAnimationFar(work->sprites[i], i + 16);
      }
    }
    if (frame == 76) {
      for (i = 0; i != JUDGMENT_FORMATION_OBJECTS; i++) {
        Sprite_StartAnimationFar(work->sprites[i], i + 32);
      }
    }

    if (frame >= 116 && frame < 144) {
      s32 high;
      s32 picture;
      s32 n = (frame - 116) / 4;
      if (n > 6) {
        n = 6;
      }
      high = gJudgment_BurstHeights[n];
      picture = gJudgment_BurstPictures[n] + JUDGMENT_SHEET_BURST;

      EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2, EFFECT_CLIP_BOUNDS,
                          COMPOSITE_LIGHTEN);
      EFFECT_BLITTERS[HEAP_SLOT_RAM_CODE_2](canvas, work->sheet + picture,
                                            60 - high, 80 - high * 2, high,
                                            high * 2);
      HeapSlot_Release(HEAP_SLOT_RAM_CODE_2);
      EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2,
                          EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                          COMPOSITE_LIGHTEN);
      EFFECT_BLITTERS[HEAP_SLOT_RAM_CODE_2](canvas, work->sheet + picture, 60,
                                            80 - high * 2, high, high * 2);
      HeapSlot_Release(HEAP_SLOT_RAM_CODE_2);
      EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2,
                          EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_Y,
                          COMPOSITE_LIGHTEN);
      EFFECT_BLITTERS[HEAP_SLOT_RAM_CODE_2](canvas, work->sheet + picture,
                                            60 - high, 80, high, high * 2);
      HeapSlot_Release(HEAP_SLOT_RAM_CODE_2);
      EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2,
                          EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X |
                              COMPOSITE_FLIP_Y,
                          COMPOSITE_LIGHTEN);
      EFFECT_BLITTERS[HEAP_SLOT_RAM_CODE_2](canvas, work->sheet + picture, 60,
                                            80, high, high * 2);
      HeapSlot_Release(HEAP_SLOT_RAM_CODE_2);

      for (i = 0; i != 6; i++) {
        s32 k = i & 3;
        s32 angle = Random16() & TRIG_ANGLE_MASK;
        s32 x = ((Trig_Sin(angle) * 16) >> 16) - gJudgment_ShardWidths[k] / 2;
        s32 y = ((Trig_Cos(angle) * 16) >> 16) - gJudgment_ShardHeights[k] / 2;
        draw[0](canvas, work->sheet + gJudgment_ShardPictures[k], x + 60,
                y + 80, gJudgment_ShardWidths[k], gJudgment_ShardHeights[k]);
      }
    }

    if (frame > 143) {
      s32 top = (frame << 4) - 2272;
      s32 n;
      s32 r;
      EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2, EFFECT_CLIP_BOUNDS,
                          COMPOSITE_ADDITIVE);
      blitters[HEAP_SLOT_RAM_CODE_2](canvas, work->sheet, 36, top, 24, 64);
      HeapSlot_Release(HEAP_SLOT_RAM_CODE_2);
      EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2,
                          EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                          COMPOSITE_ADDITIVE);
      blitters[HEAP_SLOT_RAM_CODE_2](canvas, work->sheet, 60, top, 24, 64);
      HeapSlot_Release(HEAP_SLOT_RAM_CODE_2);

      n = (frame - 144) / 2;
      r = frame * 10 - 1424;
      work->transferValue = CANVAS_FADE_THREE_QUARTERS;
      if (n <= 6) {
        s32 picture = n * MOTE_SET_BYTES + gBattleEffect_MotePictures[7] +
                      JUDGMENT_SHEET_BYTES;
        s32 high = 8;
        EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2, EFFECT_CLIP_BOUNDS,
                            COMPOSITE_LIGHTEN);
        blit = blitters[HEAP_SLOT_RAM_CODE_2];
        for (i = 0; i != 128; i++) {
          s32 angle = i * (TRIG_TURN / 128);
          s32 x = ((r * Trig_Sin(angle)) >> 16) - high / 2;
          s32 y = (r * Trig_Cos(angle)) >> 17;
          blit(canvas, work->sheet + picture, x + 60, y + 72, high, high * 2);
        }
        HeapSlot_Release(HEAP_SLOT_RAM_CODE_2);
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
  Resource_LoadAndDecompress((s32)&ResourceId_BlueCrescentSheet,
                             work->sheet + JUDGMENT_SHEET_FLARE_A, 1, 1);

  {
    u8 *c = ramp;
    for (i = 0; i != MOTE_SET_BYTES; i++, c++) {
      if (*c > 32) {
        *c += 224;
      } else if (*c != 0) {
        *c = 1;
      }
    }
  }

  // Stop the battle background fade.
  work->fadeFrames = 0;
  work->fadeStep = 0;
  BattleBackground_LoadFar(BATTLE_BACKGROUND_FIXED,
                           (s32)&ResourceId_AerialViewBackdrop, 0);
  REG_BG2PA = Q_8_8(1);
  REG_BG2X = -(60 << 8);
  REG_BG2CNT = BGCNT_PRIORITY(0) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
               BGCNT_SCREEN_BASE(7);

  fill = 0;
  Dma_Set((const void *)&fill, canvas, JUDGMENT_DMA_FILL_CANVAS, REG_DMA3);

  cx = 60;
  cy = 44;
  squash = 6;
  shift = 0;
  ring = 0;
  rings = 2;
  EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);

  fill = shift;
  Dma_Set((const void *)&fill, work, JUDGMENT_DMA_FILL_WAVE, REG_DMA3);

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
    if (frame == 280) {
      Sound_PlayFar(SE_GATE_MOVE);
    }
    if (frame == 300) {
      EFFECT_SHOW_DAMAGE(SE_CRASH);
    }

    if (gKeysRepeat & (KEY_A | KEY_B)) {
      if (frame >= 5 && frame < 150) {
        frame = 150;
        IO_WRITE_QUEUE_PUSH_IME(ime, (u32)&REG_BG2PA, Q_8_8(0.5), IO_WRITE_16);
        IO_WRITE_QUEUE_PUSH_IME(ime, (u32)&REG_BG2X, 0, IO_WRITE_32);
        Resource_LoadAndDecompress((s32)&ResourceId_ExplosionSheet,
                                   work->sheet + JUDGMENT_SHEET_EXPLOSION, 1,
                                   0);
      } else if (frame >= 155 && frame < 214) {
        frame = 214;
      } else if (frame >= 219 && frame < 280) {
        frame = 280;
      }
    }

    if (frame == 64) {
      IO_WRITE_QUEUE_PUSH_IME(ime, (u32)&REG_BG2PA, Q_8_8(0.5), IO_WRITE_16);
      IO_WRITE_QUEUE_PUSH_IME(ime, (u32)&REG_BG2X, 0, IO_WRITE_32);
      fill = 0;
      Dma_Set((const void *)&fill, canvas, JUDGMENT_DMA_FILL_CANVAS, REG_DMA3);
      Resource_LoadAndDecompress((s32)&ResourceId_ExplosionSheet,
                                 work->sheet + JUDGMENT_SHEET_EXPLOSION, 1, 0);
      Resource_LoadAndDecompress((s32)&ResourceId_OverlayImageA,
                                 work->sheet + JUDGMENT_SHEET_FLASH_SMALL, 0,
                                 0);
    }

    if (frame == 66) {
      // Invert the battle background's colours, background palettes 6 to 13.
      u16 *pal = (u16 *)&BG_PLTT_COLOR(6, 0);
      for (i = 0; i != 128; i++, pal++) {
        *pal ^= RGB(31, 31, 31);
      }
    }

    if (frame == 69) {
      fill = CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST);
      Dma_Set((const void *)&fill, canvas, JUDGMENT_DMA_FILL_CANVAS, REG_DMA3);
    }

    // Undo the inversion, rewriting the battle background's palette from the
    // battle's copy with every channel 7 levels darker.
    if (frame == 70) {
      BattleBackground_SetModeFar(BATTLE_BACKGROUND_FIXED,
                                  (s32)&ResourceId_AerialViewBackdrop, 7);
    }

    // The beach: the rings restart, four a frame.
    if (frame == 150) {
      cx = 112;
      cy = 32;
      ring = 0;
      fill = 0;
      rings = 4;
      squash = 8;
      Dma_Set((const void *)&fill, work, JUDGMENT_DMA_FILL_WAVE, REG_DMA3);
      fill = CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST);
      Dma_Set((const void *)&fill, canvas, JUDGMENT_DMA_FILL_CANVAS, REG_DMA3);
      BattleBackground_LoadFar(BATTLE_BACKGROUND_FIXED,
                               (s32)&ResourceId_BeachBackdrop, 0);
    }

    // Frame 214, the forest.
    if (frame == 214) {
      fill = 0;
      Dma_Set((const void *)&fill, work, JUDGMENT_DMA_FILL_WAVE, REG_DMA3);
      fill = CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST);
      Dma_Set((const void *)&fill, canvas, JUDGMENT_DMA_FILL_CANVAS, REG_DMA3);
      BattleBackground_LoadFar(BATTLE_BACKGROUND_FIXED,
                               (s32)&ResourceId_ForestBackdrop, 0);
      for (i = 0; i != JUDGMENT_DEBRIS_OBJECTS; i++) {
        particle = &work->particles[i];
        particle->x = px((Random16() & 63) + 32);
        particle->y = px((Random16() & 31) + 96);
        particle->velocityY = ((Random16() & 15) + 16) << 15;
        particle->z = px(1) / 2 - (particle->x >> 7);
        particle->variant = 1;
      }
    }

    if (frame == 280) {
      // Back to the battle.
      BattleEffect_SetupBlendedDisplay();
      fill = 0;
      Dma_Set((const void *)&fill, work, JUDGMENT_DMA_FILL_WAVE, REG_DMA3);
      fill = CANVAS_FILL_WORD(1);
      Dma_Set((const void *)&fill, canvas, JUDGMENT_DMA_FILL_CANVAS, REG_DMA3);
      view->busy = 0;
      REG_BLDALPHA = BLEND_ALPHA(16, 16);
      for (i = 0; i != JUDGMENT_DEBRIS_OBJECTS; i++) {
        particle = &work->particles[i];
        particle->x = px((Random16() & 63) + 256);
        particle->y = px((Random16() & 31) + 96);
        particle->velocityY = ((Random16() & 15) + 16) << 15;
        particle->z = 0;
      }
      shift = 272;
      EFFECT_SET_TRANSFER(work, CANVAS_COPY, 0);
    }

    // The canvas's colours brighten to white.
    if (frame > 279) {
      volatile u16 *pal = &BG_PLTT_COLOR(0, 1);
      for (i = 0; i != 63; i++, pal++) {
        s32 c = *pal;
        s32 b = ((u16)c >> 10) & 31;
        s32 g = ((u16)c >> 5) & 31;
        s32 r = c & 31;
        b++;
        g++;
        r++;
        if (b > 31) {
          b = 31;
        }
        if (g > 31) {
          g = 31;
        }
        if (r > 31) {
          r = 31;
        }
        *pal = RGB(r, g, b);
      }
    }

    if (frame == 182) {
      fill = CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST);
      Dma_Set((const void *)&fill, canvas, JUDGMENT_DMA_FILL_CANVAS, REG_DMA3);
    }

    // A flare falls onto the aerial view, drawn as four mirrored quarters, its
    // picture flickering every 4 frames.
    if (frame <= 63) {
      u8 *sheet;
      s32 top = frame - 4;
      if ((frame & 7) < 4) {
        sheet = work->sheet + JUDGMENT_SHEET_FLARE_A;
      } else {
        sheet = work->sheet + JUDGMENT_SHEET_FLARE_B;
      }
      EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2, EFFECT_CLIP_BOUNDS,
                          COMPOSITE_LIGHTEN);
      EFFECT_BLITTERS[HEAP_SLOT_RAM_CODE_2](canvas, sheet, 36, top - 24, 24,
                                            24);
      HeapSlot_Release(HEAP_SLOT_RAM_CODE_2);
      EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2,
                          EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                          COMPOSITE_LIGHTEN);
      EFFECT_BLITTERS[HEAP_SLOT_RAM_CODE_2](canvas, sheet, 59, top - 24, 24,
                                            24);
      HeapSlot_Release(HEAP_SLOT_RAM_CODE_2);
      EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2,
                          EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_Y,
                          COMPOSITE_LIGHTEN);
      EFFECT_BLITTERS[HEAP_SLOT_RAM_CODE_2](canvas, sheet, 36, top - 1, 24, 24);
      HeapSlot_Release(HEAP_SLOT_RAM_CODE_2);
      EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2,
                          EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X |
                              COMPOSITE_FLIP_Y,
                          COMPOSITE_LIGHTEN);
      EFFECT_BLITTERS[HEAP_SLOT_RAM_CODE_2](canvas, sheet, 59, top - 1, 24, 24);
      HeapSlot_Release(HEAP_SLOT_RAM_CODE_2);
    }

    if (frame >= 64 && frame < 66) {
      draw[0](canvas, work->sheet + JUDGMENT_SHEET_FLASH_SMALL, 52, 51, 16, 17);
    }
    if (frame >= 66 && frame < 68) {
      draw[0](canvas, work->sheet + JUDGMENT_SHEET_FLASH_LARGE, 48, 40, 24, 41);
    }
    if (frame >= 68 && frame < 76) {
      s32 d = 76 - frame;
      draw[0](canvas, work->sheet + JUDGMENT_SHEET_FLASH_GLOW, 49 - d / 2,
              38 - d, 22, 44);
      draw[0](canvas, work->sheet + JUDGMENT_SHEET_FLASH_GLOW, 49 - d / 2,
              d + 38, 22, 44);
      draw[0](canvas, work->sheet + JUDGMENT_SHEET_FLASH_GLOW, d / 2 + 49,
              38 - d, 22, 44);
      draw[0](canvas, work->sheet + JUDGMENT_SHEET_FLASH_GLOW, d / 2 + 49,
              d + 38, 22, 44);
    }
    if (frame >= 78 && frame < 80) {
      draw[0](canvas, work->sheet + JUDGMENT_SHEET_FLASH_SMALL, 52, 51, 16, 17);
    }
    if (frame >= 80 && frame < 82) {
      draw[0](canvas, work->sheet + JUDGMENT_SHEET_FLASH_LARGE, 48, 40, 24, 41);
    }
    if (frame >= 82 && frame < 86) {
      s32 d = frame * 2 - 164;
      draw[0](canvas, work->sheet + JUDGMENT_SHEET_FLASH_GLOW, 49 - d / 4,
              38 - d, 22, 44);
      draw[0](canvas, work->sheet + JUDGMENT_SHEET_FLASH_GLOW, 49 - d / 4,
              d + 38, 22, 44);
      draw[0](canvas, work->sheet + JUDGMENT_SHEET_FLASH_GLOW, d / 4 + 49,
              38 - d, 22, 44);
      draw[0](canvas, work->sheet + JUDGMENT_SHEET_FLASH_GLOW, d / 4 + 49,
              d + 38, 22, 44);
    }

    if (frame >= 72 && frame < 76) {
      s32 r2 = (frame << 4) - 1136;
      s32 size = 10;
      for (i = 0; i != 256; i++) {
        s32 angle = i * (TRIG_TURN / 256);
        s32 x = ((Trig_Sin(angle) * r2) >> 16) - (size >> 1);
        s32 y = (Trig_Cos(angle) * r2) >> 17;
        draw[0](canvas, ramp + gBattleEffect_MotePictures[size - 1], x + 60,
                y + 50, size, size * 2);
      }
    }

    if (frame > 85) {
      // More rings each frame, each one pixel wider than the last.
      if (frame <= 213) {
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
            dx = (half * sx) / 8;
            right = cx + dx;
            /* FAKEMATCH(dead): a branch whose store the next line overwrites;
             * without it, 67 of 2455 instructions differ in tbs-en. */
            if (dx > cx)
              left = 0;
            left = cx - dx;
            bottom = cy + (slope * squash) / 8;
            top = cy - (slope * sx) / 8;
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
            dx = (halfSlope * sx) / 8;
            right = cx + dx;
            /* FAKEMATCH(dead): a branch whose store the next line overwrites;
             * without it, 1954 of 2455 instructions differ in tbs-en. */
            if (dx > cx)
              left = 0;
            left = cx - dx;
            bottom = cy + (radius * squash) / 8;
            top = cy - (radius * sx) / 8;
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

    if (frame >= 86 && frame < 150) {
      s32 angle = -TRIG_TURN / 4;
      for (i = 0; i != 9; i++, angle += TRIG_TURN / 16) {
        s32 x = ((frame - 86) * Trig_Sin(angle)) >> 16;
        s32 y = ((frame - 70) * Trig_Cos(angle)) >> 16;
        draw[0](canvas,
                work->sheet +
                    ((frame / 2 + i) % 4) * JUDGMENT_EXPLOSION_PICTURE_BYTES +
                    JUDGMENT_SHEET_EXPLOSION,
                x + 44, y + 17, 32, 54);
      }
    }

    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2, EFFECT_CLIP_BOUNDS,
                        COMPOSITE_ADDITIVE);
    draw[1] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_2);
    if (frame > 85) {
      draw[1](canvas, work->sheet, 0, 0, JUDGMENT_WAVE_SIZE,
              JUDGMENT_WAVE_SIZE);
    }

    if (frame > 279) {
      shift -= 8;
    }

    // Never true: the sweep ends at frame 319.
    if (frame > 319) {
      EFFECT_FOR_EACH_TARGET(i, work->args) {
        BattleEffect_TintBattler(work->args->targets[i], SPRITE_COLOR_BLACK,
                                 POSE_DAMAGE, TINT_KEEP, 0);
      }
    }

    if (frame == 238) {
      fill = CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST);
      Dma_Set((const void *)&fill, canvas, JUDGMENT_DMA_FILL_CANVAS, REG_DMA3);
    }

    if (frame >= 214 && frame < 280) {
      gBgScroll[1].x += 8;
    }

    if (frame >= 246 && frame < 280) {
      shift += 8;
      EFFECT_SET_TRANSFER(work, CANVAS_COPY, CANVAS_FADE_THREE_QUARTERS);
    }

    // Debris rises from the field.
    if (frame > 213) {
      debrisPlace.groundHeight = 0;
      debrisPlace.height = 0;
      for (i = 0; i != JUDGMENT_DEBRIS_OBJECTS; i++) {
        particle = &work->particles[i];
        if (particle->variant != 0) {
          scale.x = particle->z + (particle->y >> 7);
          if (scale.x < Q16_ONE / 32) {
            scale.x = Q16_ONE / 32;
          }
          scale.y = scale.x;
          debrisPlace.x = particle->x;
          debrisPlace.height = px(255);
          debrisPlace.y = particle->y + px(255);
          Actor_ApplyProjectedPlacementFar(work->sprites[i], &debrisPlace,
                                           &scale, 0);
          particle->y -= particle->velocityY;
          if (particle->y < px(16)) {
            if (frame <= 279) {
              particle->x = px((Random16() & 63) + shift + 32);
            } else {
              particle->x = px((Random16() & 63) + shift - 32);
            }
            if (frame > 301) {
              particle->variant = 0;
            } else if (frame > 245) {
              particle->y = px(96);
            } else {
              particle->y = px(192) - particle->x;
            }
          }
        }
      }

      if (frame <= 279) {
        for (i = 0; i != 8; i++) {
          s32 x =
              (i & 1) * 20 - (Random16() & 15) + (i / 2) * 4 + shift / 2 - 16;
          s32 y = (i / 2) * 32 - (Random16() & 15);
          draw[1](canvas,
                  work->sheet +
                      ((frame / 2 + i) % 4) * JUDGMENT_EXPLOSION_PICTURE_BYTES +
                      JUDGMENT_SHEET_EXPLOSION,
                  x, y, 32, 54);
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
                  x, y, 32, 54);
        }
      }
    }

    HeapSlot_Release(HEAP_SLOT_RAM_CODE_2);

    // The battle background shakes up and down.
    if (frame > 63) {
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
#undef JUDGMENT_DMA_FILL_CANVAS
#undef JUDGMENT_DMA_FILL_WAVE
