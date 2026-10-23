/// @file
/// The Neptune summon.
// Part of effect.c.

#include "../effect_internal.h"

/// Sets BG2's horizontal scale back to one texel a pixel, for the lines above
/// Graphics_SetBg2AffineScaleHalf's interrupt. BattleEffect_RunNeptune
/// schedules it as a frame callback for its first stage.
void Graphics_ResetBg2Pa(void) { REG_BG2PA = Q_8_8(1); }

/// Stretches BG2 to twice its width from the current line down.
/// BattleEffect_RunNeptune installs it as the VCount interrupt handler, on line
/// 0 and from frame 152 on line 96, for its first stage.
void Graphics_SetBg2AffineScaleHalf(void) { REG_BG2PA = Q_8_8(0.5); }

#define NEPTUNE_FLAME_GROUND 112   ///< where the blue flames throw sparks
#define NEPTUNE_OBJECTS 8          ///< objects in the formation
#define NEPTUNE_PARTICLES 1024     ///< PARTICLES slots: spray, then sparks
#define NEPTUNE_FLARE_SHEET 0x4e20 ///< the flare pictures, after the spray
/// The 48x48 light pictures (9 * 256 bytes each), after the spray sheet's
/// 32x32 spray picture.
#define NEPTUNE_LIGHT_CELLS 0x400
/// The blue flame column's 34x65 head, after its 17x104 shaft.
#define NEPTUNE_FLAME_HEAD 0x6e8

/// Over dusk clouds a formation of objects flies in through rising spray,
/// gathers a ring of flares into a burst of light, then five blue flame columns
/// drop onto the targets, throwing sparks.
///
/// Abilities: Neptune (SUMMON_EFFECT_NEPTUNE).
///
/// The scene runs in two stages. The first (244 frames; A or B ends it after
/// frame 16) places NEPTUNE_OBJECTS objects at one and a half times their
/// size, opens the window with an edge table built every frame, sprays
/// particles under gravity, converges a ring of flares, then breaks into a
/// burst of light; meanwhile a frame callback and an interrupt change BG2's
/// horizontal scale. The second stage (144 frames) drops five blue flame
/// columns onto the targets, one every 16 frames; each throws sparks when it
/// reaches the ground (NEPTUNE_FLAME_GROUND). The battle shows the damage on
/// its frame 96.
///
/// Reached through BattleEffect_RunSummonEffect, whose sheet slot holds the
/// mote sheet here. The first stage projects with gProjection.centerY at 240
/// and holds BG1's scroll from the battle camera; the second gives BG1's x
/// back, removes the scale callback and interrupt, and
/// BattleEffect_SetupBlendedDisplay puts the centre back at 120, which its
/// Camera_ApplyShake then moves while the screen shakes. The formation's
/// objects are released before the scene returns.
///
/// In the first stage work->particles 0 to 31 are the converging flares, then
/// from frame 222 0 to 63 the burst points; PARTICLES holds the spray
/// (variant counts its frames, -1 when free). In the second stage
/// work->particles 0 to 4 are the flame columns (variant their frames on the
/// ground) and PARTICLES the sparks (variant their frames left, 0 when free).
/// The window's edge step, noise and scanline table use the map grid cell
/// buffer's first 450 bytes on frames 0 to 15, over spray particles 0 to 16,
/// which are already launched.
void BattleEffect_RunNeptune(struct BattleEffectArgument *effect) {
  static struct SpriteScale unitScale = {{{0x18000, 0x18000}}};
  struct SpritePlacement pos;
  struct EffectPosition targetPosition;
  void *canvas;
  u8 *particleSheet;
  s32 windowEdgeStep;
  s32 formationX;
  s32 formationZ;
  s32 formationVelocityX;
  s32 formationVelocityZ;
  s32 savedBg1ScrollX;
  struct BattleBackgroundView *view;
  s32 j;
  s32 emittedSparks;
  s32 sparkOriginX;
  u32 *cache;
  struct BattleEffectWork *work;
  s32 frame;
  s32 t;
  s32 i;
  s32 n;
  s32 s;
  struct EffectStep *flareSeed;
  struct EffectStep *flare;
  struct EffectStep *burstSeed;
  struct EffectStep *flameColumn;
  DrawRectangle blit[2];
  struct SpriteScale scale;
  s32 ground = NEPTUNE_FLAME_GROUND;

  cache = (u32 *)&gHeapSlots[HEAP_SLOT_BATTLE_CANVAS];
  canvas = (void *)cache[0];
  work = (struct BattleEffectWork *)CANVAS_SLOT(cache, HEAP_SLOT_BATTLE_ANIM);
  particleSheet = (u8 *)CANVAS_SLOT(cache, HEAP_SLOT_BATTLE_SHEET);
  work->effect = effect;
  BattleEffect_StartCanvasLayer(0);
  BattleEffect_ConfigureEffectDisplay();
  // The backdrop and colour 1, which the wipe fills the canvas with, are
  // black.
  PLTT_BACKDROP = 0;
  *(u16 *)&BG_PLTT[1] = 0;
  // BG2's scale per frame: one texel a pixel until the VCount line (0 here),
  // twice the width from it down.
  Task_Add(Graphics_ResetBg2Pa, EFFECT_CALLBACK_ORDER);
  Runtime_SetIrqHandler(IRQ_VCOUNT, 0, Graphics_SetBg2AffineScaleHalf);
  work->transferMode = CANVAS_COPY;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, EFFECT_CALLBACK_ORDER);
  // Setup: wipe the battle away and close window 0 (its edge opens over the
  // first 16 frames), swap in the dusk backdrop and load the pictures.
  REG_WININ = EFFECT_WIN0_LAYERS | WININ_WIN1_BG0 | WININ_WIN1_CLR;
  BattleEffect_WipeCanvas(CANVAS_WIPE_ACROSS, CANVAS_WIPE_FILL);
  REG_WIN0H = WIN_RANGE(240, 240);
  BattleEffect_SelectLivingTargets(work->effect);
  BattleEffect_SpawnObjects(NEPTUNE_OBJECTS, 0x17a, 1);
  gProjection.centerY = 240;
  WaitFrames(1);
  BattleBackground_LoadFar(BATTLE_BACKGROUND_FLAT,
                           (s32)&ResourceId_DuskCloudsBackdrop, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, particleSheet,
                             0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_WaterSpraySheet, work->sheet, 1,
                             1);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesD,
                             work->sheet + NEPTUNE_FLARE_SHEET, 0, 0);
  EFFECT_SHOW_WIDE_CANVAS(16);
  // The formation starts at x 280, 200 deep, and flies in at 4 pixels a
  // frame across and 8 forward (16.16), slowing as it comes.
  windowEdgeStep = 1;
  formationX = 140 << 17;
  formationZ = 200 << 16;
  formationVelocityX = -0x40000;
  formationVelocityZ = -0x80000;
  savedBg1ScrollX = gBgScroll[1].x;
  view = (struct BattleBackgroundView *)CANVAS_SLOT(
      cache, HEAP_SLOT_BATTLE_BACKGROUND);
  EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, 75);
  // The scene owns BG1's scroll until the second stage.
  view->busy = 1;
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                      COMPOSITE_ADDITIVE);
  blit[0] = (DrawRectangle)CANVAS_SLOT(cache, HEAP_SLOT_RAM_CODE);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2, EFFECT_CLIP_BOUNDS, COMPOSITE_MAX);
  blit[1] = (DrawRectangle)CANVAS_SLOT(cache, HEAP_SLOT_RAM_CODE_2);
  REG_BG2CNT = BGCNT_PRIORITY(0) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
               BGCNT_SCREEN_BASE(7);

  for (i = 0; i != NEPTUNE_PARTICLES; i++) {
    PARTICLES[i].variant = -1;
  }
  // Seed the flares that converge during the formation burst: each starts
  // 64 to 95 pixels out, in the quarter turn from 0x8000 (up and to the left
  // of the centre), and closes in over 32 frames.
  for (i = 0, flareSeed = work->particles; i != 32; i++, flareSeed++) {
    s32 a = (Random16() & 0x3fff) + 0x8000;
    s32 r = (Random16() & 127) + 255;
    flareSeed->x = (Trig_Sin(a) * r) >> 2;
    flareSeed->y = (Trig_Cos(a) * r) >> 2;
    flareSeed->velocityX = -flareSeed->x / 32;
    flareSeed->velocityY = -flareSeed->y / 32;
    flareSeed->variant = 0;
  }
  // Launch the first 712 spray particles from an oval around (128, 96),
  // reaching 32 pixels out across and 16 up and down, each moving left 1 to
  // 5 pixels a frame and up 3 to 5; the other slots stay free (variant -1).
  for (i = 0; i != 712; i++) {
    s32 r = Random16() & 255;
    s32 a = Random16() & 0xffff;
    PARTICLES[i].x = ((Trig_Sin(a) * r) >> 3) + 0x800000;
    PARTICLES[i].y = ((Trig_Cos(a) * r) >> 4) + (192 << 15);
    PARTICLES[i].velocityX = (-(Random16() & 0x1fff) - 0x800) << 5;
    PARTICLES[i].velocityY = (-(Random16() & 0xfff) - 0x1800) << 5;
    PARTICLES[i].variant = 0;
  }

  frame = 0;
  do {
    // The first stage: the window opens (frames 0 to 15), the formation flies
    // (22 to 170), the spray falls (from 28, a second burst on 76), the flares
    // converge (152 to 239), the beam of lines reaches the burst (from 176)
    // and the burst breaks into light (from 222).
    if (frame == 24) {
      Sound_PlaySongFar(SE_162);
    }
    if (frame == 76) {
      Sound_PlaySongFar(SE_164);
    }
    if (frame == 154) {
      Sound_PlaySongFar(SE_142);
    }
    if (frame == 222) {
      Sound_PlaySongFar(SE_145);
    }
    // Over the first 16 frames the window's right edge sweeps open by a
    // growing step, ragged by 128 random offsets: frame 1 starts the
    // callback that builds the edge table each frame, frame 15 stops it.
    if (frame >= 0 && frame < 16) {
      u16 *phase = (u16 *)Ram_MapGridCellBuffer;
      if (frame == 1) {
        u8 *noise = Ram_MapGridCellBuffer + 2;
        s32 k;
        for (k = 0; k != 128; k++) {
          noise[k] = Random16() & 63;
        }
        *phase = 0;
        Task_Add(BattleEffect_BuildWindowEdgeTable, EFFECT_CALLBACK_ORDER);
      }
      *phase += windowEdgeStep;
      windowEdgeStep += 3;
      if (frame == 15) {
        Task_Remove(BattleEffect_BuildWindowEdgeTable);
      }
    }

    // From frame 22 the formation flies in and settles around depth 132 by
    // frame 83, then drifts right while settling around depth 138, until
    // frame 170. Its objects stand at one and a half times their offsets.
    if (frame > 21) {
      scale = unitScale;
      if (frame <= 83) {
        if (frame > 48) {
          formationVelocityX = formationVelocityX * 60 / 64;
        }
        formationVelocityZ = formationVelocityZ * 60 / 64;
        if (formationZ > 0x840000) {
          formationVelocityZ -= 0x2000;
        } else {
          formationVelocityZ += 0x2000;
        }
        formationX += formationVelocityX;
        formationZ += formationVelocityZ;
      } else if (frame <= 170) {
        if (frame <= 107) {
          formationVelocityX += 0x2000;
        }
        formationVelocityX = formationVelocityX * 60 / 64;
        formationX += formationVelocityX;
        if (frame <= 117) {
          if (formationZ > 0x8a0000) {
            formationVelocityZ -= 0x2000;
          } else {
            formationVelocityZ += 0x2000;
          }
          formationVelocityZ = formationVelocityZ * 56 / 64;
          formationZ += formationVelocityZ;
        }
      }
      pos.groundHeight = 0;
      pos.height = 0;
      for (i = 0; i != NEPTUNE_OBJECTS; i++) {
        pos.x =
            (((gNeptune_ObjectX[i] * 3) >> 1) << 16) + formationX - 0x200000;
        pos.y =
            (((gNeptune_ObjectY[i] * 3) >> 1) << 16) + formationZ - 0x180000;
        Object_ApplyProjectedPlacementFar(work->animations[i], &pos, &scale, 0);
      }
    }

    if (frame == 76) {
      for (i = 0; i != NEPTUNE_PARTICLES; i++) {
        // Frame 76: a second spray from around (64, 96) out of every free
        // slot, up to 2 pixels a frame either way across and 2 to 6 up.
        if (PARTICLES[i].variant == -1) {
          s32 r = Random16() & 255;
          s32 a = Random16() & 0xffff;
          PARTICLES[i].x = ((Trig_Sin(a) * r) >> 3) + 0x400000;
          PARTICLES[i].y = ((Trig_Cos(a) * r) >> 4) + (192 << 15);
          PARTICLES[i].velocityX = (32 - (Random16() & 63)) << 12;
          PARTICLES[i].velocityY = (-(Random16() & 31) - 16) << 13;
          PARTICLES[i].variant = 0;
        }
      }
    }
    // Frame 152: only the lines from 96 down stay stretched, and the battle
    // backdrop's colours darken over 24 frames.
    if (frame == 152) {
      Runtime_SetIrqHandler(IRQ_VCOUNT, 96, Graphics_SetBg2AffineScaleHalf);
      work->fadeFrames = 24;
      work->fadeStep = 0;
    }

    // Frames 152 to 239: the 32 flares converge on (112, 62), four more
    // starting each frame, each shown for 32 frames.
    t = frame - 152;
    if (t >= 0 && t < 88) {
      for (i = 0, flare = work->particles; i != 32; i++, flare++) {
        if (frame >= i / 4 + 152 && frame < i / 4 + 152 + 32) {
          s32 fx = Q_16_16_TO_INT(flare->x) + 112;
          s32 fy = Q_16_16_TO_INT(flare->y) + 62;
          s32 r = (i & 3) + 5;
          s = r * 2;
          blit[0](canvas,
                  work->sheet + gBattleEffect_FlareCells[r - 1] +
                      NEPTUNE_FLARE_SHEET,
                  fx - r, fy - r, s, s);
          flare->x += flare->velocityX;
          flare->y += flare->velocityY;
        }
      }
    }

    if (frame == 222) {
      for (i = 0; i != 64; i++) {
        // Frame 222: 64 burst points around (100, 52), at least four pixels
        // out, each moving six pixels left a frame; z and velocityY keep the
        // start to restart from.
        burstSeed = &work->particles[i];
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
    if (frame > 221) {
      for (i = 0; i != 64; i++) {
        // Two more burst points start each frame, as the smallest flare; one
        // that leaves the top restarts.
        u16 *cells = gBattleEffect_FlareCells;
        s32 r = 1;
        if (frame >= i / 2 + 222) {
          blit[0](canvas, work->sheet + cells[r - 1] + NEPTUNE_FLARE_SHEET,
                  work->particles[i].x - r, work->particles[i].y - r, r * 2,
                  r * 2);
          work->particles[i].x += work->particles[i].velocityX;
          work->particles[i].y += work->particles[i].velocityY;
          if (work->particles[i].y < 0) {
            work->particles[i].y = work->particles[i].velocityY;
            work->particles[i].x = work->particles[i].z;
          }
        }
      }
    }

    // From frame 176 a beam of lines runs from the top of the canvas to the
    // burst: 5 lines, then from frame 222 40 lines, 60 on frames 223 and 224.
    // Line i lies d pixels left or right of the beam's centre by turns. Its
    // level, the line's last argument, is twice the line count less 4 for
    // each pixel of d, kept within 12 to 63: the 5-line beam is level 12
    // throughout, and the 40- and 60-line beams are brightest in the middle.
    if (frame > 175) {
      n = 5;
      if (frame > 221) {
        n = 60;
        if (frame != 223 && frame != 224) {
          n = 40;
        }
      }
      if (n > 60) {
        n = 60;
      }
      for (i = 0; i != n; i++) {
        s32 w;
        s32 d = (i + 1) / 2;
        w = n * 2 - d * 4;
        if (w < 12) {
          w = 12;
        }
        if (w > 63) {
          w = 63;
        }
        if (i & 1) {
          d = -d;
        }
        BattleEffect_DrawClippedCanvasLine(d + 48, 0, d / 2 + 111,
                                           63 - (d + 1) / 2, w);
      }
    }

    // The flare at the centre of the burst grows from frame 152, and from
    // frame 214 the light pictures cover it.
    if (t >= 0 && t < 40) {
      s32 z;
      z = t / 2 + 1;
      if (z > 4) {
        z = 4;
      }
      blit[1](canvas,
              work->sheet + gBattleEffect_FlareCells[z - 1] +
                  NEPTUNE_FLARE_SHEET,
              108 - z, 60 - z, z * 2, z * 2);
    }
    if (frame >= 192 && frame < 200) {
      s32 z;
      z = (frame - 192) / 2 + 3;
      if (z > 6) {
        z = 6;
      }
      blit[1](canvas,
              work->sheet + gBattleEffect_FlareCells[z - 1] +
                  NEPTUNE_FLARE_SHEET,
              108 - z, 60 - z, z * 2, z * 2);
    }
    if (frame > 199) {
      s32 z;
      s32 u = frame - 200;
      z = u / 4 + 5;
      if (z > 8) {
        z = 8;
      }
      blit[1](canvas,
              work->sheet + gBattleEffect_FlareCells[z - 1] +
                  NEPTUNE_FLARE_SHEET,
              108 - z, 60 - z, z * 2, z * 2);
      if (frame <= 213) {
        z = u / 4 + 1;
        if (z > 4) {
          z = 4;
        }
        blit[1](canvas,
                work->sheet + gBattleEffect_FlareCells[z - 1] +
                    NEPTUNE_FLARE_SHEET,
                100 - z, 52 - z, z * 2, z * 2);
      }
    }
    if (frame >= 214 && frame < 222) {
      s32 z;
      z = (frame - 214) / 4;
      if (z > 2) {
        z = 2;
      }
      blit[1](canvas, work->sheet + z * 9 * 256 + NEPTUNE_LIGHT_CELLS, 63, 18,
              48, 48);
    }
    if (frame > 221) {
      s32 z;
      z = (frame - 222) / 2 % 4 + 3;
      blit[0](canvas, work->sheet + z * 9 * 256 + NEPTUNE_LIGHT_CELLS, 72, 30,
              48, 48);
      if (z == 5) {
        blit[0](canvas, work->sheet + NEPTUNE_LIGHT_CELLS + 2 * (48 * 48), 66,
                22, 48, 48);
      }
    }

    // From frame 28 each live spray particle falls under its gravity
    // (gNeptune_Gravity by index); one that falls below y 104 is freed.
    if (frame > 27) {
      DrawRectangle draw = blit[1];
      for (i = 0; i != NEPTUNE_PARTICLES; i++) {
        if (PARTICLES[i].variant >= 0) {
          s32 k = i % 3 + 2;
          EFFECT_DRAW_MOTE(draw, canvas, particleSheet,
                           Q_16_16_TO_INT(PARTICLES[i].x),
                           Q_16_16_TO_INT(PARTICLES[i].y), k);
          EffectStep_AdvanceWithGravity2D(&PARTICLES[i], 62,
                                          gNeptune_Gravity[i & 3]);
          PARTICLES[i].variant++;
          if (PARTICLES[i].velocityY > 0 &&
              Q_16_16_TO_INT(PARTICLES[i].y) > 104) {
            PARTICLES[i].variant = -1;
          }
        }
      }
    }
    for (i = 0; i != 5; i++) {
      // The scrolling strip of five spray pictures along the bottom.
      blit[1](canvas, work->sheet, i * 32 + ((frame / 4) & 31) - 32, 88, 32,
              32);
    }
    EFFECT_PRESENT_FRAME(work);
  } while (++frame != 244 && (frame <= 16 || !(gKeysRepeat & (KEY_A | KEY_B))));

  // The second stage: restore the backdrop, the scale callbacks and the
  // display, free the spray as sparks (variant 0), and aim the five flame
  // columns: column i drops onto target i's x (halved for the wide canvas)
  // when there is one, otherwise onto a random x from 32 to 63.
  gBgScroll[1].x = savedBg1ScrollX;
  view->busy = 0;
  Runtime_SetIrqHandler(IRQ_VCOUNT, 0, 0);
  BattleEffect_SetupBlendedDisplay();
  Task_Remove(Graphics_ResetBg2Pa);
  REG_WIN0H = WIN_RANGE(0, 240);
  EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, 75);
  for (i = 0; i != NEPTUNE_PARTICLES; i++) {
    PARTICLES[i].variant = 0;
  }
  for (i = 0; i != 16; i++) {
    work->particles[i].x = (Random16() & 31) + 32;
    work->particles[i].y = 0;
    work->particles[i].variant = 0;
  }
  EFFECT_FOR_EACH_TARGET(i, work->effect) {
    EffectPosition_ProjectScaledBattler(work->effect->actors[i],
                                        &targetPosition);
    work->particles[i].x = targetPosition.x / 2;
  }
  Resource_LoadAndDecompress((s32)&ResourceId_BlueFlameSheet, work->sheet, 1,
                             1);
  Sound_PlaySongFar(AUDIO_CUE_FADE_SLOT3);
  REG_BG2PA = Q_8_8(0.5);
  REG_BLDALPHA = BLEND_ALPHA(16, 16);

  frame = 0;
  do {
    // The second stage, 144 frames: column i starts on frame 16i and lands
    // on frame 16i + 7, at y 112 (NEPTUNE_FLAME_GROUND); the battle shows
    // the damage on frame 96.
    if (frame == 96) {
      EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
    }
    for (i = 0; i != 5; i++) {
      flameColumn = &work->particles[i];
      if (frame == i * 16 + 7) {
        Sound_PlaySongFar(SE_CAST);
      }
      // A column, a scrolling shaft over a head, falls 16 pixels a frame.
      // Once on the ground it counts its frames there, shakes the screen for
      // the first eight and throws up to four sparks a frame for about 32
      // frames. A spark flies up in a half fan, up to 8 pixels a frame across
      // and 16 up, for 32 to 39 frames.
      if (frame >= i * 16 && flameColumn->variant <= 31) {
        s32 wobble = frame * 16 % 104;
        blit[1](canvas, work->sheet, flameColumn->x - 8,
                flameColumn->y + wobble - 216, 17, 104);
        blit[1](canvas, work->sheet, flameColumn->x - 8,
                flameColumn->y + wobble - 112, 17, 104 - wobble);
        blit[1](canvas, work->sheet + NEPTUNE_FLAME_HEAD, flameColumn->x - 17,
                flameColumn->y - 65, 34, 65);
        if (flameColumn->y <= 111) {
          flameColumn->y += 16;
        } else {
          flameColumn->variant++;
        }
        if (flameColumn->y > 111) {
          emittedSparks = 0;
          sparkOriginX = flameColumn->x;
          if (flameColumn->variant <= 7) {
            work->shakeFrames = 4;
          }
          for (j = 0; j != NEPTUNE_PARTICLES; j++) {
            if (PARTICLES[j].variant == 0) {
              s32 r = 0x3ff;
              s32 a;
              r &= Random16();
              a = (Random16() & 0x7fff) - 0x4000;
              PARTICLES[j].x = sparkOriginX << 16;
              PARTICLES[j].y = ground << 16;
              r += 32;
              PARTICLES[j].velocityX = (Trig_Sin(a) * r) >> 7;
              PARTICLES[j].velocityY = -(Trig_Cos(a) * r * 2) >> 7;
              PARTICLES[j].variant = (Random16() & 7) + 32;
              if (++emittedSparks == 4) {
                break;
              }
            }
          }
        }
      }
    }
    // Target i flashes and reels on frame 16i + 8, as its column lands.
    EFFECT_FOR_EACH_TARGET(i, work->effect) {
      if (frame == i * 16 + 8) {
        ObjectGroup_UpdateMembers(work->effect->actors[i], SPRITE_COLOR_WHITE,
                                  POSE_DAMAGE, i, 8);
        BattleMotion_ApplyVariantMotionFar(work->effect->actors[i],
                                           KNOCKBACK_LEAP);
      }
    }
    for (i = 0; i != NEPTUNE_PARTICLES; i++) {
      if (PARTICLES[i].variant > 0) {
        s32 x;
        s32 y;
        // Sparks fall under a quarter pixel of gravity and bounce at half
        // speed below y 120; one off the canvas is not drawn. A spark's mote
        // shrinks from size 6 or 7 down to 3 as its life runs out.
        PARTICLES[i].variant--;
        EffectStep_AdvanceWithGravity2D(&PARTICLES[i], 60, 0x4000);
        y = PARTICLES[i].y >> 16;
        if (y > 120) {
          PARTICLES[i].velocityY = -PARTICLES[i].velocityY / 2;
        } else if (PARTICLES[i].x >= 0 && (x = PARTICLES[i].x >> 16) <= 126 &&
                   PARTICLES[i].y >= 0) {
          s32 k = PARTICLES[i].variant / 8 + 3;
          EFFECT_DRAW_MOTE(blit[i & 1], canvas, particleSheet, x, y, k);
        }
      }
    }
    Camera_ApplyShake(4, 8);
    EFFECT_END_FRAME(work);
  } while (++frame != 144);

  // The finish: release the formation's objects, then the canvas.
  for (i = 0; i != NEPTUNE_OBJECTS; i++) {
    ResourceObject_ReleaseFar(work->animations[i]);
  }
  EFFECT_FINISH_CANVAS();
}

#undef NEPTUNE_FLAME_GROUND
#undef NEPTUNE_OBJECTS
#undef NEPTUNE_PARTICLES
#undef NEPTUNE_FLARE_SHEET
#undef NEPTUNE_LIGHT_CELLS
#undef NEPTUNE_FLAME_HEAD
