/// @file
/// The Neptune summon.
// Part of battle_effect.c.

#include "battle_effect_internal.h"

/// Frames of the first stage, Neptune's leap, which A or B cuts short after
/// its first 16, and of the second, the columns of water.
#define NEPTUNE_LEAP_FRAMES 244
#define NEPTUNE_COLUMN_FRAMES 144
/// The first stage's cues: flares converge on Neptune's head from
/// NEPTUNE_FLARES_FRAME, looked at for NEPTUNE_FLARES_WINDOW frames, while the
/// flare on the head grows and from NEPTUNE_BEAM_FRAME the beam joins it to the
/// sky; the flare swells from NEPTUNE_SWELL_FRAME, and again from
/// NEPTUNE_SECOND_SWELL_FRAME with a second flare beside it, until the splash
/// curls up from NEPTUNE_SPLASH_FRAME and it bursts at NEPTUNE_BURST_FRAME.
#define NEPTUNE_FLARES_FRAME 152
#define NEPTUNE_FLARES_WINDOW 88
#define NEPTUNE_BEAM_FRAME 176
#define NEPTUNE_SWELL_FRAME 192
#define NEPTUNE_SECOND_SWELL_FRAME 200
#define NEPTUNE_SPLASH_FRAME 214
#define NEPTUNE_BURST_FRAME 222

/// Sets BG2's horizontal scale back to one texel a pixel above the stretched
/// lines.
void BattleEffect_ResetBg2Pa(void) { REG_BG2PA = Q_8_8(1); }

/// Stretches BG2 to twice its width from the current line down.
void BattleEffect_SetBg2AffineScaleHalf(void) { REG_BG2PA = Q_8_8(0.5); }

#define NEPTUNE_FLARE_SHEET 20000 ///< the flare pictures, after the spray
/// The splash where the beam meets the head, growing and curling.
#define NEPTUNE_SPLASH PICTURE_BYTES(WATER_WAVE)
/// A water column's head, after its shaft: BlueFlameSheet's pictures.
#define NEPTUNE_COLUMN_HEAD PICTURE_BYTES(BLUE_FLAME_COLUMN)

/// Neptune is drawn by eight 32x32 sprites (gNeptune_ObjectX and _ObjectY).
#define NEPTUNE_SPRITES 8
/// The drops of the first spray, as Neptune leaps out of the sea.
#define NEPTUNE_FIRST_SPRAY_DROPS 712
/// The sea's waves along the canvas, from this row down; the spray is gone
/// once it falls halfway into them.
#define NEPTUNE_SEA_Y 88
/// From NEPTUNE_FLARES_FRAME the canvas is drawn at its own width above this
/// screen line and twice as wide below it, rather than all twice as wide.
#define NEPTUNE_NARROW_LINES 96
/// The flares that gather on Neptune's head, each closing in for
/// NEPTUNE_FLARE_FRAMES frames on the canvas point NEPTUNE_FLARES_X,
/// NEPTUNE_FLARES_Y.
#define NEPTUNE_FLARE_COUNT 32
#define NEPTUNE_FLARE_FRAMES 32
#define NEPTUNE_FLARES_X 112
#define NEPTUNE_FLARES_Y 62
/// The sparks of the burst, falling from it.
#define NEPTUNE_BURST_SPARKS 64
/// The beam's width in lines: thin as the flares gather, wide from the burst,
/// and wider still on the burst's second and third frames. Its middle line is
/// the brightest, each line out four levels dimmer, none below
/// NEPTUNE_BEAM_LEVEL_MIN.
#define NEPTUNE_BEAM_THIN 5
#define NEPTUNE_BEAM_WIDE 40
#define NEPTUNE_BEAM_FLASH 60
#define NEPTUNE_BEAM_LEVEL_MIN 12
/// The columns of water: NEPTUNE_COLUMN_SLOTS set out, the first
/// NEPTUNE_COLUMNS falling NEPTUNE_COLUMN_INTERVAL frames apart at
/// NEPTUNE_COLUMN_SPEED pixels a frame until they reach the canvas row
/// NEPTUNE_COLUMN_GROUND, where each stands NEPTUNE_COLUMN_LANDED_FRAMES
/// frames, shaking the battle for the first NEPTUNE_COLUMN_SHAKE_FRAMES.
#define NEPTUNE_COLUMN_SLOTS 16
#define NEPTUNE_COLUMNS 5
#define NEPTUNE_COLUMN_INTERVAL 16
#define NEPTUNE_COLUMN_SPEED 16
#define NEPTUNE_COLUMN_GROUND 112
#define NEPTUNE_COLUMN_LANDED_FRAMES 32
#define NEPTUNE_COLUMN_SHAKE_FRAMES 8

/// Under storm clouds Neptune leaps out of the sea in a burst of spray, light
/// gathers at its head and a beam of light joins its head to the sky, with a
/// splash there; then five columns of water crash down on the targets,
/// throwing up drops.
///
/// Abilities: Neptune.
void BattleEffect_RunNeptune(struct BattleEffectParams *args) {
  static struct SpriteScale unitScale = {
      {{FIXED_ONE * 3 / 2, FIXED_ONE * 3 / 2}}};
  struct SpritePlacement place;
  struct ScreenPoint targetPosition;
  void *canvas;
  u8 *motes;
  s32 windowEdgeStep;
  FIXED neptuneX;
  FIXED neptuneZ;
  s32 neptuneVelocityX;
  s32 neptuneVelocityZ;
  s32 savedBg1ScrollX;
  struct BattleBackgroundState *view;
  s32 j;
  s32 emittedDrops;
  s32 dropOriginX;
  u32 *heap;
  struct BattleEffectState *state;
  s32 frame;
  s32 flareFrame;
  s32 beamLines;
  s32 span;
  s32 i;
  struct Particle *flareSeed;
  struct Particle *flare;
  struct Particle *burstSeed;
  struct Particle *column;
  DrawRectangle blit[2];
  struct SpriteScale scale;
  s32 ground = NEPTUNE_COLUMN_GROUND;

  heap = (u32 *)&gHeapSlots[HEAP_SLOT_BATTLE_CANVAS];
  canvas = (void *)heap[0];
  state = (struct BattleEffectState *)HEAP_SLOT_FROM(
      heap, HEAP_SLOT_BATTLE_CANVAS, HEAP_SLOT_BATTLE_EFFECT);
  motes = (u8 *)HEAP_SLOT_FROM(heap, HEAP_SLOT_BATTLE_CANVAS,
                               HEAP_SLOT_BATTLE_SHEET);
  state->args = args;
  BattleEffect_StartCanvasLayer(0);
  BattleEffect_ConfigureEffectDisplay();
  PLTT_BACKDROP = 0;
  BG_PLTT_COLOR(0, 1) = 0;
  Task_Add(BattleEffect_ResetBg2Pa, TASK_PASS_VBLANK);
  Irq_SetHandler(IRQ_VCOUNT, 0, BattleEffect_SetBg2AffineScaleHalf);
  state->transferMode = CANVAS_COPY;
  Task_Add(BattleEffect_TransferCanvas, TASK_PASS_VBLANK);
  // Setup: wipe the battle away, swap in the storm clouds and load the
  // pictures.
  REG_WININ = EFFECT_WIN0_LAYERS | WININ_WIN1_BG0 | WININ_WIN1_CLR;
  BattleEffect_WipeCanvas(CANVAS_WIPE_ACROSS, CANVAS_WIPE_FILL);
  REG_WIN0H = WIN_RANGE(240, 240);
  BattleEffect_ShowLivingTargets(state->args);
  BattleEffect_CreateSprites(NEPTUNE_SPRITES, SPRITE_NEPTUNE, 1);
  gProjection.centerY = SUMMON_PROJECTION_CENTER_Y;
  wait(1);
  BattleBackground_LoadFar(BATTLE_BACKGROUND_FIXED,
                           RESOURCE_DUSK_CLOUDS_BACKDROP, 0);
  Resource_LoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, motes, 0, 0);
  Resource_LoadAndDecompress(RESOURCE_WATER_SPRAY_SHEET, state->sheet, 1, 1);
  Resource_LoadAndDecompress(RESOURCE_BLUE_GLOW_SHEET,
                             state->sheet + NEPTUNE_FLARE_SHEET, 0, 0);
  EFFECT_SHOW_WIDE_CANVAS(BLEND_WEIGHT_MAX);
  windowEdgeStep = 1;
  neptuneX = toFIXED(280);
  neptuneZ = toFIXED(200);
  neptuneVelocityX = -toFIXED(4);
  neptuneVelocityZ = -toFIXED(8);
  savedBg1ScrollX = gBgScroll[1].x;
  view = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_BACKGROUND);
  EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);
  view->busy = 1;
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                      COMPOSITE_ADDITIVE);
  blit[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS,
                      COMPOSITE_LIGHTEN);
  blit[1] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED);
  REG_BG2CNT = BGCNT_PRIORITY(0) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
               BGCNT_SCREEN_BASE(7);

  for (i = 0; i != PARTICLE_POOL_COUNT; i++) {
    PARTICLES[i].variant = -1;
  }
  for (i = 0; i != NEPTUNE_FLARE_COUNT; i++) {
    s32 a = (Random16() & 0x3fff) + DEGtoANG(180);
    s32 r = (Random16() & 127) + 255;

    flareSeed = &state->particles[i];
    flareSeed->x = (Trig_Sin(a) * r) >> 2;
    flareSeed->y = (Trig_Cos(a) * r) >> 2;
    flareSeed->velocityX = -flareSeed->x / NEPTUNE_FLARE_FRAMES;
    flareSeed->velocityY = -flareSeed->y / NEPTUNE_FLARE_FRAMES;
    flareSeed->variant = 0;
  }
  // The first spray around Neptune.
  for (i = 0; i != NEPTUNE_FIRST_SPRAY_DROPS; i++) {
    s32 r = Random16() & 255;
    s32 a = Random16() & TRIG_ANGLE_MASK;
    PARTICLES[i].x = ((Trig_Sin(a) * r) >> 3) + toFIXED(128);
    PARTICLES[i].y = ((Trig_Cos(a) * r) >> 4) + toFIXED(96);
    PARTICLES[i].velocityX = (-(Random16() & 0x1fff) - toFIXED(1) / 32) << 5;
    PARTICLES[i].velocityY = (-(Random16() & 0xfff) - toFIXED(3) / 32) << 5;
    PARTICLES[i].variant = 0;
  }

  for (frame = 0; frame != NEPTUNE_LEAP_FRAMES &&
                  (frame <= 16 || !(gKeysRepeat & (KEY_A | KEY_B)));
       frame++) {
    // The first stage: Neptune leaps from the sea and the beam joins it to the
    // sky.
    if (frame == 24) {
      Sound_PlayFar(SE_WAVE);
    }
    if (frame == 76) {
      Sound_PlayFar(SE_SCATTER);
    }
    if (frame == 154) {
      Sound_PlayFar(SE_SWIRL);
    }
    if (frame == NEPTUNE_BURST_FRAME) {
      Sound_PlayFar(SE_CRASH);
    }
    // The window opens with a ragged edge.
    if (frame >= 0 && frame < 16) {
      u16 *phase = WINDOW_EDGE_RIGHT;
      if (frame == 1) {
        u8 *noise = WINDOW_EDGE_INSETS;
        s32 k;
        for (k = 0; k != WINDOW_EDGE_INSET_LINES; k++) {
          noise[k] = Random16() & 63;
        }
        *phase = 0;
        Task_Add(BattleEffect_BuildWindowEdgeTable, TASK_PASS_VBLANK);
      }
      *phase += windowEdgeStep;
      windowEdgeStep += 3;
      if (frame == 15) {
        Task_Remove(BattleEffect_BuildWindowEdgeTable);
      }
    }

    // Neptune leaps in and settles.
    if (frame > 21) {
      scale = unitScale;
      if (frame <= 83) {
        if (frame > 48) {
          neptuneVelocityX = neptuneVelocityX * 60 / DAMPING_SCALE;
        }
        neptuneVelocityZ = neptuneVelocityZ * 60 / DAMPING_SCALE;
        if (neptuneZ > toFIXED(132)) {
          neptuneVelocityZ -= toFIXED(1) / 8;
        } else {
          neptuneVelocityZ += toFIXED(1) / 8;
        }
        neptuneX += neptuneVelocityX;
        neptuneZ += neptuneVelocityZ;
      } else if (frame <= 170) {
        if (frame <= 107) {
          neptuneVelocityX += toFIXED(1) / 8;
        }
        neptuneVelocityX = neptuneVelocityX * 60 / DAMPING_SCALE;
        neptuneX += neptuneVelocityX;
        if (frame <= 117) {
          if (neptuneZ > toFIXED(138)) {
            neptuneVelocityZ -= toFIXED(1) / 8;
          } else {
            neptuneVelocityZ += toFIXED(1) / 8;
          }
          neptuneVelocityZ = neptuneVelocityZ * 56 / DAMPING_SCALE;
          neptuneZ += neptuneVelocityZ;
        }
      }
      place.groundHeight = 0;
      place.height = 0;
      for (i = 0; i != NEPTUNE_SPRITES; i++) {
        place.x =
            INTtoFIXED((gNeptune_ObjectX[i] * 3) >> 1) + neptuneX - toFIXED(32);
        place.y =
            INTtoFIXED((gNeptune_ObjectY[i] * 3) >> 1) + neptuneZ - toFIXED(24);
        Render_ApplyProjectedPlacementFar(state->sprites[i], &place, &scale, 0);
      }
    }

    if (frame == 76) {
      for (i = 0; i != PARTICLE_POOL_COUNT; i++) {
        // A second spray.
        if (PARTICLES[i].variant == -1) {
          s32 r = Random16() & 255;
          s32 a = Random16() & TRIG_ANGLE_MASK;
          PARTICLES[i].x = ((Trig_Sin(a) * r) >> 3) + toFIXED(64);
          PARTICLES[i].y = ((Trig_Cos(a) * r) >> 4) + toFIXED(96);
          PARTICLES[i].velocityX = (32 - (Random16() & 63)) << 12;
          PARTICLES[i].velocityY = (-(Random16() & 31) - 16) << 13;
          PARTICLES[i].variant = 0;
        }
      }
    }
    if (frame == NEPTUNE_FLARES_FRAME) {
      Irq_SetHandler(IRQ_VCOUNT, NEPTUNE_NARROW_LINES,
                     BattleEffect_SetBg2AffineScaleHalf);
      state->fadeFrames = BACKGROUND_DARKEN_FRAMES;
      state->fadeStep = 0;
    }

    // Flares converge on Neptune's head.
    flareFrame = frame - NEPTUNE_FLARES_FRAME;
    if (flareFrame >= 0 && flareFrame < NEPTUNE_FLARES_WINDOW) {
      for (i = 0; i != NEPTUNE_FLARE_COUNT; i++) {
        flare = &state->particles[i];
        if (frame >= i / 4 + NEPTUNE_FLARES_FRAME &&
            frame < i / 4 + NEPTUNE_FLARES_FRAME + NEPTUNE_FLARE_FRAMES) {
          s32 fx = FIXEDtoINT(flare->x) + NEPTUNE_FLARES_X;
          s32 fy = FIXEDtoINT(flare->y) + NEPTUNE_FLARES_Y;
          s32 size = (i & 3) + 5;
          span = size * 2;
          blit[0](canvas,
                  state->sheet + gBattleEffect_FlarePictures[size - 1] +
                      NEPTUNE_FLARE_SHEET,
                  fx - size, fy - size, span, span);
          flare->x += flare->velocityX;
          flare->y += flare->velocityY;
        }
      }
    }

    // The burst's sparks, one more every two frames. Each keeps its start in z
    // and velocityY, to start again from there once it rises past the top, but
    // velocityY is also its fall: it drops by its start height a frame, so it
    // shows at its start and once or twice lower and is off the canvas's foot
    // within three frames, never to start again. In a recording they are too
    // brief to pick out under the burst's flash.
    if (frame == NEPTUNE_BURST_FRAME) {
      for (i = 0; i != NEPTUNE_BURST_SPARKS; i++) {
        burstSeed = &state->particles[i];
        burstSeed->x = (Random16() & 15) - 8;
        burstSeed->y = (Random16() & 15) - 8;
        if (burstSeed->x < 0) {
          burstSeed->x -= 4;
        } else {
          burstSeed->x += 4;
        }
        if (burstSeed->y < 0) {
          burstSeed->y -= 4;
        } else {
          burstSeed->y += 4;
        }
        burstSeed->x += 100;
        burstSeed->y += 52;
        burstSeed->velocityX = -6;
        burstSeed->z = burstSeed->x;
        burstSeed->velocityY = burstSeed->y;
        burstSeed->variant = 0;
      }
    }
    if (frame >= NEPTUNE_BURST_FRAME) {
      u16 *pictures = gBattleEffect_FlarePictures;

      for (i = 0; i != NEPTUNE_BURST_SPARKS; i++) {
        // The smallest flare's size.
        s32 r = 1;
        if (frame >= i / 2 + NEPTUNE_BURST_FRAME) {
          blit[0](canvas, state->sheet + pictures[r - 1] + NEPTUNE_FLARE_SHEET,
                  state->particles[i].x - r, state->particles[i].y - r, r * 2,
                  r * 2);
          state->particles[i].x += state->particles[i].velocityX;
          state->particles[i].y += state->particles[i].velocityY;
          if (state->particles[i].y < 0) {
            state->particles[i].y = state->particles[i].velocityY;
            state->particles[i].x = state->particles[i].z;
          }
        }
      }
    }

    // The beam runs from the top of the canvas to the burst.
    if (frame >= NEPTUNE_BEAM_FRAME) {
      beamLines = NEPTUNE_BEAM_THIN;
      if (frame >= NEPTUNE_BURST_FRAME) {
        beamLines = NEPTUNE_BEAM_FLASH;
        if (frame != NEPTUNE_BURST_FRAME + 1 &&
            frame != NEPTUNE_BURST_FRAME + 2) {
          beamLines = NEPTUNE_BEAM_WIDE;
        }
      }
      // DEAD: never true, as beamLines is one of the three widths here; the ROM
      // keeps the compare and the store.
      if (beamLines > NEPTUNE_BEAM_FLASH) {
        beamLines = NEPTUNE_BEAM_FLASH;
      }
      for (i = 0; i != beamLines; i++) {
        s32 level;
        s32 offset = (i + 1) / 2;
        level = beamLines * 2 - offset * 4;
        if (level < NEPTUNE_BEAM_LEVEL_MIN) {
          level = NEPTUNE_BEAM_LEVEL_MIN;
        }
        if (level > CANVAS_LEVEL_BRIGHTEST) {
          level = CANVAS_LEVEL_BRIGHTEST;
        }
        if (i & 1) {
          offset = -offset;
        }
        BattleEffect_DrawClippedCanvasLine(offset + 48, 0, offset / 2 + 111,
                                           63 - (offset + 1) / 2, level);
      }
    }

    // The flare at the burst grows, then the splash covers it.
    if (flareFrame >= 0 &&
        flareFrame < NEPTUNE_SWELL_FRAME - NEPTUNE_FLARES_FRAME) {
      s32 size;
      size = flareFrame / 2 + 1;
      if (size > 4) {
        size = 4;
      }
      EFFECT_DRAW_FLARE(blit[1], canvas, state->sheet + NEPTUNE_FLARE_SHEET,
                        108, 60, size);
    }
    if (frame >= NEPTUNE_SWELL_FRAME && frame < NEPTUNE_SECOND_SWELL_FRAME) {
      s32 size;
      size = (frame - NEPTUNE_SWELL_FRAME) / 2 + 3;
      if (size > 6) {
        size = 6;
      }
      EFFECT_DRAW_FLARE(blit[1], canvas, state->sheet + NEPTUNE_FLARE_SHEET,
                        108, 60, size);
    }
    if (frame >= NEPTUNE_SECOND_SWELL_FRAME) {
      s32 size;
      s32 swellFrame = frame - NEPTUNE_SECOND_SWELL_FRAME;
      size = swellFrame / 4 + 5;
      if (size > 8) {
        size = 8;
      }
      EFFECT_DRAW_FLARE(blit[1], canvas, state->sheet + NEPTUNE_FLARE_SHEET,
                        108, 60, size);
      if (frame < NEPTUNE_SPLASH_FRAME) {
        size = swellFrame / 4 + 1;
        if (size > 4) {
          size = 4;
        }
        EFFECT_DRAW_FLARE(blit[1], canvas, state->sheet + NEPTUNE_FLARE_SHEET,
                          100, 52, size);
      }
    }
    if (frame >= NEPTUNE_SPLASH_FRAME && frame < NEPTUNE_BURST_FRAME) {
      s32 picture;
      picture = (frame - NEPTUNE_SPLASH_FRAME) / 4;
      if (picture > 2) {
        picture = 2;
      }
      blit[1](canvas,
              state->sheet + NEPTUNE_SPLASH +
                  picture * PICTURE_BYTES(WATER_SPLASH),
              63, 18, WATER_SPLASH_WIDTH, WATER_SPLASH_HEIGHT);
    }
    if (frame >= NEPTUNE_BURST_FRAME) {
      s32 picture;
      picture = (frame - NEPTUNE_BURST_FRAME) / 2 % 4 + 3;
      blit[0](canvas,
              state->sheet + NEPTUNE_SPLASH +
                  picture * PICTURE_BYTES(WATER_SPLASH),
              72, 30, WATER_SPLASH_WIDTH, WATER_SPLASH_HEIGHT);
      if (picture == 5) {
        blit[0](canvas,
                state->sheet + NEPTUNE_SPLASH + 2 * PICTURE_BYTES(WATER_SPLASH),
                66, 22, WATER_SPLASH_WIDTH, WATER_SPLASH_HEIGHT);
      }
    }

    // The spray falls.
    if (frame > 27) {
      DrawRectangle draw = blit[1];
      for (i = 0; i != PARTICLE_POOL_COUNT; i++) {
        if (PARTICLES[i].variant >= 0) {
          s32 k = i % 3 + 2;
          EFFECT_DRAW_MOTE(draw, canvas, motes, FIXEDtoINT(PARTICLES[i].x),
                           FIXEDtoINT(PARTICLES[i].y), k);
          Particle_AdvanceWithGravity2D(&PARTICLES[i], 62,
                                        gNeptune_Gravity[i & 3]);
          PARTICLES[i].variant++;
          if (PARTICLES[i].velocityY > 0 &&
              FIXEDtoINT(PARTICLES[i].y) >
                  NEPTUNE_SEA_Y + WATER_WAVE_HEIGHT / 2) {
            PARTICLES[i].variant = -1;
          }
        }
      }
    }
    for (i = 0; i != 5; i++) {
      // The sea scrolls along the bottom.
      blit[1](canvas, state->sheet,
              i * WATER_WAVE_WIDTH + ((frame / 4) & 31) - WATER_WAVE_WIDTH,
              NEPTUNE_SEA_Y, WATER_WAVE_WIDTH, WATER_WAVE_HEIGHT);
    }
    EFFECT_PRESENT_CANVAS(state);
  }

  // The second stage: five columns of water crash down on the targets.
  gBgScroll[1].x = savedBg1ScrollX;
  view->busy = 0;
  Irq_SetHandler(IRQ_VCOUNT, 0, 0);
  BattleEffect_SetupBlendedDisplay();
  Task_Remove(BattleEffect_ResetBg2Pa);
  REG_WIN0H = WIN_RANGE(0, 240);
  EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);
  for (i = 0; i != PARTICLE_POOL_COUNT; i++) {
    PARTICLES[i].variant = 0;
  }
  for (i = 0; i != NEPTUNE_COLUMN_SLOTS; i++) {
    state->particles[i].x = (Random16() & 31) + 32;
    state->particles[i].y = 0;
    state->particles[i].variant = 0;
  }
  EFFECT_FOR_EACH_TARGET(i, state->args) {
    Projection_ScaledUnit(state->args->targets[i], &targetPosition);
    state->particles[i].x = targetPosition.x / 2;
  }
  Resource_LoadAndDecompress(RESOURCE_BLUE_FLAME_SHEET, state->sheet, 1, 1);
  Sound_PlayFar(SOUND_FADE_OUT_ACTION_SE);
  REG_BG2PA = Q_8_8(0.5);
  REG_BLDALPHA = EFFECT_BLEND_ADD;

  for (frame = 0; frame != NEPTUNE_COLUMN_FRAMES; frame++) {
    if (frame == 96) {
      EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
    }
    for (i = 0; i != NEPTUNE_COLUMNS; i++) {
      column = &state->particles[i];
      if (frame == i * NEPTUNE_COLUMN_INTERVAL + 7) {
        Sound_PlayFar(SE_CAST);
      }
      // A column falls.
      if (frame >= i * NEPTUNE_COLUMN_INTERVAL &&
          column->variant < NEPTUNE_COLUMN_LANDED_FRAMES) {
        s32 wobble = frame * NEPTUNE_COLUMN_SPEED % BLUE_FLAME_COLUMN_HEIGHT;
        blit[1](canvas, state->sheet, column->x - BLUE_FLAME_COLUMN_WIDTH / 2,
                column->y + wobble - 216, BLUE_FLAME_COLUMN_WIDTH,
                BLUE_FLAME_COLUMN_HEIGHT);
        blit[1](canvas, state->sheet, column->x - BLUE_FLAME_COLUMN_WIDTH / 2,
                column->y + wobble - 112, BLUE_FLAME_COLUMN_WIDTH,
                BLUE_FLAME_COLUMN_HEIGHT - wobble);
        blit[1](canvas, state->sheet + NEPTUNE_COLUMN_HEAD,
                column->x - BLUE_FLAME_SPLASH_WIDTH / 2,
                column->y - BLUE_FLAME_SPLASH_HEIGHT, BLUE_FLAME_SPLASH_WIDTH,
                BLUE_FLAME_SPLASH_HEIGHT);
        if (column->y < NEPTUNE_COLUMN_GROUND) {
          column->y += NEPTUNE_COLUMN_SPEED;
        } else {
          column->variant++;
        }
        if (column->y >= NEPTUNE_COLUMN_GROUND) {
          emittedDrops = 0;
          dropOriginX = column->x;
          if (column->variant < NEPTUNE_COLUMN_SHAKE_FRAMES) {
            state->shakeFrames = 4;
          }
          for (j = 0; j != PARTICLE_POOL_COUNT; j++) {
            if (PARTICLES[j].variant == 0) {
              s32 r = (Random16() & 0x3ff) + 32;
              s32 a;

              a = (Random16() & 0x7fff) - DEGtoANG(90);
              PARTICLES[j].x = INTtoFIXED(dropOriginX);
              PARTICLES[j].y = INTtoFIXED(ground);
              PARTICLES[j].velocityX = (Trig_Sin(a) * r) >> 7;
              PARTICLES[j].velocityY = -(Trig_Cos(a) * r * 2) >> 7;
              PARTICLES[j].variant = (Random16() & 7) + 32;
              if (++emittedDrops == 4) {
                break;
              }
            }
          }
        }
      }
    }
    EFFECT_FOR_EACH_TARGET(i, state->args) {
      if (frame == i * NEPTUNE_COLUMN_INTERVAL + 8) {
        BattleEffect_TintBattler(state->args->targets[i], SPRITE_COLOR_WHITE,
                                 POSE_DAMAGE, i, 8);
        BattleMotion_ApplyKnockbackFar(state->args->targets[i], KNOCKBACK_LEAP);
      }
    }
    for (i = 0; i != PARTICLE_POOL_COUNT; i++) {
      if (PARTICLES[i].variant > 0) {
        s32 x;
        s32 y;
        // Drops fall and bounce.
        PARTICLES[i].variant--;
        Particle_AdvanceWithGravity2D(&PARTICLES[i], 60, toFIXED(1) / 4);
        y = FIXEDtoINT(PARTICLES[i].y);
        if (y > 120) {
          PARTICLES[i].velocityY = -PARTICLES[i].velocityY / 2;
        } else if (PARTICLES[i].x >= 0 &&
                   (x = FIXEDtoINT(PARTICLES[i].x)) <= 126 &&
                   PARTICLES[i].y >= 0) {
          s32 k = PARTICLES[i].variant / 8 + 3;
          EFFECT_DRAW_MOTE(blit[i & 1], canvas, motes, x, y, k);
        }
      }
    }
    BattleCamera_Shake(4, 8);
    EFFECT_END_FRAME(state);
  }

  for (i = 0; i != NEPTUNE_SPRITES; i++) {
    Sprite_DestroyFar(state->sprites[i]);
  }
  EFFECT_FINISH_CANVAS();
}

#undef NEPTUNE_LEAP_FRAMES
#undef NEPTUNE_COLUMN_FRAMES
#undef NEPTUNE_FLARES_FRAME
#undef NEPTUNE_FLARES_WINDOW
#undef NEPTUNE_BEAM_FRAME
#undef NEPTUNE_SWELL_FRAME
#undef NEPTUNE_SECOND_SWELL_FRAME
#undef NEPTUNE_SPLASH_FRAME
#undef NEPTUNE_BURST_FRAME
#undef NEPTUNE_FLARE_SHEET
#undef NEPTUNE_SPLASH
#undef NEPTUNE_COLUMN_HEAD
#undef NEPTUNE_SPRITES
#undef NEPTUNE_FIRST_SPRAY_DROPS
#undef NEPTUNE_SEA_Y
#undef NEPTUNE_NARROW_LINES
#undef NEPTUNE_FLARE_COUNT
#undef NEPTUNE_FLARE_FRAMES
#undef NEPTUNE_FLARES_X
#undef NEPTUNE_FLARES_Y
#undef NEPTUNE_BURST_SPARKS
#undef NEPTUNE_BEAM_THIN
#undef NEPTUNE_BEAM_WIDE
#undef NEPTUNE_BEAM_FLASH
#undef NEPTUNE_BEAM_LEVEL_MIN
#undef NEPTUNE_COLUMN_SLOTS
#undef NEPTUNE_COLUMNS
#undef NEPTUNE_COLUMN_INTERVAL
#undef NEPTUNE_COLUMN_SPEED
#undef NEPTUNE_COLUMN_GROUND
#undef NEPTUNE_COLUMN_LANDED_FRAMES
#undef NEPTUNE_COLUMN_SHAKE_FRAMES
