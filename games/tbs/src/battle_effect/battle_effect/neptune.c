/// @file
/// The Neptune summon.
// Part of battle_effect.c.

#include "../battle_effect_internal.h"

/// Sets BG2's horizontal scale back to one texel a pixel, for the lines above
/// BattleEffect_SetBg2AffineScaleHalf's interrupt. BattleEffect_RunNeptune
/// adds it as a task for its first stage.
void BattleEffect_ResetBg2Pa(void) { REG_BG2PA = Q_8_8(1); }

/// Stretches BG2 to twice its width from the current line down.
/// BattleEffect_RunNeptune installs it as the VCount interrupt handler, on line
/// 0 and from frame 152 on line 96, for its first stage.
void BattleEffect_SetBg2AffineScaleHalf(void) { REG_BG2PA = Q_8_8(0.5); }

#define NEPTUNE_FLARE_SHEET 20000 ///< the flare pictures, after the spray
/// The seven 48x48 pictures of the splash where the beam meets the head,
/// growing and then curling, after the spray sheet's 32x32 picture of the sea.
#define NEPTUNE_SPLASH (32 * 32)
/// A water column's 34x65 head, after its 17x104 shaft.
#define NEPTUNE_COLUMN_HEAD (17 * 104)

/// Under storm clouds Neptune leaps out of the sea in a burst of spray, light
/// gathers at its head and a beam of light joins its head to the sky, with a
/// splash there; then five columns of water crash down on the targets,
/// throwing up drops.
///
/// Abilities: Neptune (SUMMON_EFFECT_NEPTUNE).
///
/// The effect runs in two stages. The first (244 frames; A or B ends it
/// after frame 16) places Neptune, eight sprites of SPRITE_NEPTUNE, at one
/// and a half times their size, opens the window with an edge table built
/// every frame, sprays particles under gravity over the sea along the bottom,
/// converges a ring of flares on Neptune's head, then joins it to the sky with
/// a beam and a splash; meanwhile a frame task and an interrupt change BG2's
/// horizontal scale. The second stage (144 frames) drops five columns of water
/// onto the targets, one every 16 frames; each throws up drops when it
/// reaches the ground at y 112. The battle shows the damage on its frame 96.
///
/// Reached through BattleEffect_RunSummon, whose sheet slot holds the
/// mote sheet here. The first stage projects with gProjection.centerY at 240
/// and holds BG1's scroll from the battle camera; the second gives BG1's x
/// back, removes the scale task and interrupt, and
/// BattleEffect_SetupBlendedDisplay puts the centre back at 120, which its
/// BattleCamera_Shake then moves while the screen shakes. Neptune's
/// sprites are released before the effect returns.
///
/// In the first stage work->particles 0 to 31 are the converging flares, then
/// from frame 222 0 to 63 the burst points; PARTICLES 0 to 1023 hold the spray
/// (variant counts its frames, -1 when free). In the second stage
/// work->particles 0 to 4 are the water columns (variant their frames on the
/// ground) and PARTICLES the drops (variant their frames left, 0 when free).
/// The window's edge step, noise and line table use the map grid cell
/// buffer's first 450 bytes on frames 0 to 15, over spray particles 0 to 16,
/// which are already launched.
void BattleEffect_RunNeptune(struct BattleEffectParams *args) {
  static struct SpriteScale unitScale = {{{Q16_ONE * 3 / 2, Q16_ONE * 3 / 2}}};
  struct SpritePlacement pos;
  struct ScreenPoint targetPosition;
  void *canvas;
  u8 *particleSheet;
  s32 windowEdgeStep;
  s32 neptuneX;
  s32 neptuneZ;
  s32 neptuneVelocityX;
  s32 neptuneVelocityZ;
  s32 savedBg1ScrollX;
  struct BattleBackgroundWork *view;
  s32 j;
  s32 emittedDrops;
  s32 dropOriginX;
  u32 *cache;
  struct BattleEffectWork *work;
  s32 frame;
  s32 t;
  s32 i;
  s32 n;
  s32 s;
  struct Particle *flareSeed;
  struct Particle *flare;
  struct Particle *burstSeed;
  struct Particle *column;
  DrawRectangle blit[2];
  struct SpriteScale scale;
  s32 ground = 112; // where the columns land

  cache = (u32 *)&gHeapSlots[HEAP_SLOT_BATTLE_CANVAS];
  canvas = (void *)cache[0];
  work = (struct BattleEffectWork *)HEAP_SLOT_FROM(
      cache, HEAP_SLOT_BATTLE_CANVAS, HEAP_SLOT_BATTLE_EFFECT);
  particleSheet = (u8 *)HEAP_SLOT_FROM(cache, HEAP_SLOT_BATTLE_CANVAS,
                                       HEAP_SLOT_BATTLE_SHEET);
  work->args = args;
  BattleEffect_StartCanvasLayer(0);
  BattleEffect_ConfigureEffectDisplay();
  // The backdrop and colour 1, which the wipe fills the canvas with, are
  // black.
  PLTT_BACKDROP = 0;
  *(u16 *)&BG_PLTT[1] = 0;
  // BG2's scale per frame: one texel a pixel until the VCount line (0 here),
  // twice the width from it down.
  Task_Add(BattleEffect_ResetBg2Pa, TASK_PASS_VBLANK);
  Irq_SetHandler(IRQ_VCOUNT, 0, BattleEffect_SetBg2AffineScaleHalf);
  work->transferMode = CANVAS_COPY;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_VBLANK);
  // Setup: wipe the battle away and close window 0 (its edge opens over the
  // first 16 frames), swap in the storm clouds and load the pictures.
  REG_WININ = EFFECT_WIN0_LAYERS | WININ_WIN1_BG0 | WININ_WIN1_CLR;
  BattleEffect_WipeCanvas(CANVAS_WIPE_ACROSS, CANVAS_WIPE_FILL);
  REG_WIN0H = WIN_RANGE(240, 240);
  BattleEffect_SelectLivingTargets(work->args);
  BattleEffect_SpawnObjects(8, SPRITE_NEPTUNE, 1);
  gProjection.centerY = 240;
  wait(1);
  BattleBackground_LoadFar(BATTLE_BACKGROUND_FIXED,
                           (s32)&ResourceId_DuskCloudsBackdrop, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, particleSheet,
                             0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_WaterSpraySheet, work->sheet, 1,
                             1);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesD,
                             work->sheet + NEPTUNE_FLARE_SHEET, 0, 0);
  EFFECT_SHOW_WIDE_CANVAS(16);
  // Neptune starts at x 280, 200 deep, and leaps in at 4 pixels a frame
  // across and 8 forward, slowing as it comes.
  windowEdgeStep = 1;
  neptuneX = px(280);
  neptuneZ = px(200);
  neptuneVelocityX = -px(4);
  neptuneVelocityZ = -px(8);
  savedBg1ScrollX = gBgScroll[1].x;
  view = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_BACKGROUND);
  EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);
  // The effect owns BG1's scroll until the second stage.
  view->busy = 1;
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                      COMPOSITE_ADDITIVE);
  blit[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2, EFFECT_CLIP_BOUNDS,
                      COMPOSITE_LIGHTEN);
  blit[1] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_2);
  REG_BG2CNT = BGCNT_PRIORITY(0) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
               BGCNT_SCREEN_BASE(7);

  for (i = 0; i != 1024; i++) {
    PARTICLES[i].variant = -1;
  }
  // Seed the flares that converge on Neptune's head: each starts
  // 64 to 95 pixels out, in the quarter turn from TRIG_TURN / 2 (up and to
  // the left of the centre), and closes in over 32 frames.
  for (i = 0, flareSeed = work->particles; i != 32; i++, flareSeed++) {
    s32 a = (Random16() & 0x3fff) + TRIG_TURN / 2;
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
    s32 a = Random16() & TRIG_ANGLE_MASK;
    PARTICLES[i].x = ((Trig_Sin(a) * r) >> 3) + px(128);
    PARTICLES[i].y = ((Trig_Cos(a) * r) >> 4) + px(96);
    PARTICLES[i].velocityX = (-(Random16() & 0x1fff) - 0x800) << 5;
    PARTICLES[i].velocityY = (-(Random16() & 0xfff) - 0x1800) << 5;
    PARTICLES[i].variant = 0;
  }

  frame = 0;
  do {
    // The first stage: the window opens (frames 0 to 15), Neptune leaps in
    // (22 to 170), the spray falls (from 28, a second burst on 76), the flares
    // converge (152 to 239), the beam of lines reaches the burst (from 176)
    // and the beam widens with the splash (from 222).
    if (frame == 24) {
      Sound_PlayFar(SE_WAVE);
    }
    if (frame == 76) {
      Sound_PlayFar(SE_SCATTER);
    }
    if (frame == 154) {
      Sound_PlayFar(SE_SWIRL);
    }
    if (frame == 222) {
      Sound_PlayFar(SE_CRASH);
    }
    // Over the first 16 frames the window's right edge sweeps open by a
    // growing step, ragged by 128 random offsets: frame 1 starts the
    // task that builds the edge table each frame, frame 15 stops it.
    if (frame >= 0 && frame < 16) {
      u16 *phase = (u16 *)Ram_MapGridCellBuffer;
      if (frame == 1) {
        u8 *noise = Ram_MapGridCellBuffer + 2;
        s32 k;
        for (k = 0; k != 128; k++) {
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

    // From frame 22 Neptune leaps in and settles around depth 132 by frame
    // 83, then drifts right while settling around depth 138, until frame 170.
    // Its sprites stand at one and a half times their offsets.
    if (frame > 21) {
      scale = unitScale;
      if (frame <= 83) {
        if (frame > 48) {
          neptuneVelocityX = neptuneVelocityX * 60 / 64;
        }
        neptuneVelocityZ = neptuneVelocityZ * 60 / 64;
        if (neptuneZ > px(132)) {
          neptuneVelocityZ -= px(1) / 8;
        } else {
          neptuneVelocityZ += px(1) / 8;
        }
        neptuneX += neptuneVelocityX;
        neptuneZ += neptuneVelocityZ;
      } else if (frame <= 170) {
        if (frame <= 107) {
          neptuneVelocityX += px(1) / 8;
        }
        neptuneVelocityX = neptuneVelocityX * 60 / 64;
        neptuneX += neptuneVelocityX;
        if (frame <= 117) {
          if (neptuneZ > px(138)) {
            neptuneVelocityZ -= px(1) / 8;
          } else {
            neptuneVelocityZ += px(1) / 8;
          }
          neptuneVelocityZ = neptuneVelocityZ * 56 / 64;
          neptuneZ += neptuneVelocityZ;
        }
      }
      pos.groundHeight = 0;
      pos.height = 0;
      for (i = 0; i != 8; i++) {
        pos.x = px((gNeptune_ObjectX[i] * 3) >> 1) + neptuneX - px(32);
        pos.y = px((gNeptune_ObjectY[i] * 3) >> 1) + neptuneZ - px(24);
        Actor_ApplyProjectedPlacementFar(work->sprites[i], &pos, &scale, 0);
      }
    }

    if (frame == 76) {
      for (i = 0; i != 1024; i++) {
        // Frame 76: a second spray from around (64, 96) out of every free
        // slot, up to 2 pixels a frame either way across and 2 to 6 up.
        if (PARTICLES[i].variant == -1) {
          s32 r = Random16() & 255;
          s32 a = Random16() & TRIG_ANGLE_MASK;
          PARTICLES[i].x = ((Trig_Sin(a) * r) >> 3) + px(64);
          PARTICLES[i].y = ((Trig_Cos(a) * r) >> 4) + px(96);
          PARTICLES[i].velocityX = (32 - (Random16() & 63)) << 12;
          PARTICLES[i].velocityY = (-(Random16() & 31) - 16) << 13;
          PARTICLES[i].variant = 0;
        }
      }
    }
    // Frame 152: only the lines from 96 down stay stretched, and the battle
    // background's colours darken over 24 frames.
    if (frame == 152) {
      Irq_SetHandler(IRQ_VCOUNT, 96, BattleEffect_SetBg2AffineScaleHalf);
      work->fadeFrames = 24;
      work->fadeStep = 0;
    }

    // Frames 152 to 239: the 32 flares converge on Neptune's head at
    // (112, 62), four more starting each frame, each shown for 32 frames.
    t = frame - 152;
    if (t >= 0 && t < 88) {
      for (i = 0, flare = work->particles; i != 32; i++, flare++) {
        if (frame >= i / 4 + 152 && frame < i / 4 + 152 + 32) {
          s32 fx = Q_16_16_TO_INT(flare->x) + 112;
          s32 fy = Q_16_16_TO_INT(flare->y) + 62;
          s32 r = (i & 3) + 5;
          s = r * 2;
          blit[0](canvas,
                  work->sheet + gBattleEffect_FlarePictures[r - 1] +
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
        /* FAKEMATCH(temporary): pictures holds gBattleEffect_FlarePictures; as
         * gBattleEffect_FlarePictures[r - 1], 104 of 1474 instructions differ
         * in tbs-en. */
        u16 *pictures = gBattleEffect_FlarePictures;
        /* LEFTOVER(template): r holds 1, the smallest flare's size; the
         * sibling loop in BattleEffect_RunNeptune, the flares from frame 152
         * above, runs the same draw with a size that varies, (i & 3) + 5; as
         * 0, 1 and 2 written out, 62 of 1474 instructions differ in tbs-en. */
        s32 r = 1;
        if (frame >= i / 2 + 222) {
          blit[0](canvas, work->sheet + pictures[r - 1] + NEPTUNE_FLARE_SHEET,
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
      // DEAD: never true, as n is 5, 40 or 60 here; the ROM keeps the compare
      // and the store.
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
    // frame 214 the splash pictures cover it.
    if (t >= 0 && t < 40) {
      s32 z;
      z = t / 2 + 1;
      if (z > 4) {
        z = 4;
      }
      blit[1](canvas,
              work->sheet + gBattleEffect_FlarePictures[z - 1] +
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
              work->sheet + gBattleEffect_FlarePictures[z - 1] +
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
              work->sheet + gBattleEffect_FlarePictures[z - 1] +
                  NEPTUNE_FLARE_SHEET,
              108 - z, 60 - z, z * 2, z * 2);
      if (frame <= 213) {
        z = u / 4 + 1;
        if (z > 4) {
          z = 4;
        }
        blit[1](canvas,
                work->sheet + gBattleEffect_FlarePictures[z - 1] +
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
      blit[1](canvas, work->sheet + NEPTUNE_SPLASH + z * (48 * 48), 63, 18, 48,
              48);
    }
    if (frame > 221) {
      s32 z;
      z = (frame - 222) / 2 % 4 + 3;
      blit[0](canvas, work->sheet + NEPTUNE_SPLASH + z * (48 * 48), 72, 30, 48,
              48);
      if (z == 5) {
        blit[0](canvas, work->sheet + NEPTUNE_SPLASH + 2 * (48 * 48), 66, 22,
                48, 48);
      }
    }

    // From frame 28 each live spray particle falls under its gravity
    // (gNeptune_Gravity by index); one that falls below y 104 is freed.
    if (frame > 27) {
      DrawRectangle draw = blit[1];
      for (i = 0; i != 1024; i++) {
        if (PARTICLES[i].variant >= 0) {
          s32 k = i % 3 + 2;
          EFFECT_DRAW_MOTE(draw, canvas, particleSheet,
                           Q_16_16_TO_INT(PARTICLES[i].x),
                           Q_16_16_TO_INT(PARTICLES[i].y), k);
          Particle_AdvanceWithGravity2D(&PARTICLES[i], 62,
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
      // The sea: five copies of its 32x32 picture along the bottom, scrolling.
      blit[1](canvas, work->sheet, i * 32 + ((frame / 4) & 31) - 32, 88, 32,
              32);
    }
    EFFECT_PRESENT_CANVAS(work);
  } while (++frame != 244 && (frame <= 16 || !(gKeysRepeat & (KEY_A | KEY_B))));

  // The second stage: restore the battle background, the scale tasks and
  // the display, free the spray as drops (variant 0), and aim the five water
  // columns: column i drops onto target i's x (halved for the wide canvas)
  // when there is one, otherwise onto a random x from 32 to 63.
  gBgScroll[1].x = savedBg1ScrollX;
  view->busy = 0;
  Irq_SetHandler(IRQ_VCOUNT, 0, 0);
  BattleEffect_SetupBlendedDisplay();
  Task_Remove(BattleEffect_ResetBg2Pa);
  REG_WIN0H = WIN_RANGE(0, 240);
  EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);
  for (i = 0; i != 1024; i++) {
    PARTICLES[i].variant = 0;
  }
  for (i = 0; i != 16; i++) {
    work->particles[i].x = (Random16() & 31) + 32;
    work->particles[i].y = 0;
    work->particles[i].variant = 0;
  }
  EFFECT_FOR_EACH_TARGET(i, work->args) {
    Projection_ScaledUnit(work->args->targets[i], &targetPosition);
    work->particles[i].x = targetPosition.x / 2;
  }
  Resource_LoadAndDecompress((s32)&ResourceId_BlueFlameSheet, work->sheet, 1,
                             1);
  Sound_PlayFar(SOUND_FADE_OUT_ACTION_SE);
  REG_BG2PA = Q_8_8(0.5);
  REG_BLDALPHA = BLEND_ALPHA(16, 16);

  frame = 0;
  do {
    // The second stage, 144 frames: column i starts on frame 16i and lands
    // on frame 16i + 7, at y 112; the battle shows
    // the damage on frame 96.
    if (frame == 96) {
      EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
    }
    for (i = 0; i != 5; i++) {
      column = &work->particles[i];
      if (frame == i * 16 + 7) {
        Sound_PlayFar(SE_CAST);
      }
      // A column, a scrolling shaft over a head, falls 16 pixels a frame.
      // Once on the ground it counts its frames there, shakes the screen for
      // the first eight and throws up to four drops a frame for about 32
      // frames. A drop flies up in a half fan, up to 8 pixels a frame across
      // and 16 up, for 32 to 39 frames.
      if (frame >= i * 16 && column->variant <= 31) {
        s32 wobble = frame * 16 % 104;
        blit[1](canvas, work->sheet, column->x - 8, column->y + wobble - 216,
                17, 104);
        blit[1](canvas, work->sheet, column->x - 8, column->y + wobble - 112,
                17, 104 - wobble);
        blit[1](canvas, work->sheet + NEPTUNE_COLUMN_HEAD, column->x - 17,
                column->y - 65, 34, 65);
        if (column->y <= 111) {
          column->y += 16;
        } else {
          column->variant++;
        }
        if (column->y > 111) {
          emittedDrops = 0;
          dropOriginX = column->x;
          if (column->variant <= 7) {
            work->shakeFrames = 4;
          }
          for (j = 0; j != 1024; j++) {
            if (PARTICLES[j].variant == 0) {
              s32 r = (Random16() & 0x3ff) + 32;
              s32 a;

              a = (Random16() & 0x7fff) - TRIG_TURN / 4;
              PARTICLES[j].x = px(dropOriginX);
              PARTICLES[j].y = px(ground);
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
    // Target i flashes and reels on frame 16i + 8, as its column lands.
    EFFECT_FOR_EACH_TARGET(i, work->args) {
      if (frame == i * 16 + 8) {
        BattleEffect_TintBattler(work->args->targets[i], SPRITE_COLOR_WHITE,
                                 POSE_DAMAGE, i, 8);
        BattleMotion_ApplyKnockbackFar(work->args->targets[i], KNOCKBACK_LEAP);
      }
    }
    for (i = 0; i != 1024; i++) {
      if (PARTICLES[i].variant > 0) {
        s32 x;
        s32 y;
        // Drops fall under a quarter pixel of gravity and bounce at half
        // speed below y 120; one off the canvas is not drawn. A drop's mote
        // shrinks from size 6 or 7 down to 3 as its life runs out.
        PARTICLES[i].variant--;
        Particle_AdvanceWithGravity2D(&PARTICLES[i], 60, px(1) / 4);
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
    BattleCamera_Shake(4, 8);
    EFFECT_END_FRAME(work);
  } while (++frame != 144);

  // The finish: release Neptune's sprites, then the canvas.
  for (i = 0; i != 8; i++) {
    Sprite_DestroyFar(work->sprites[i]);
  }
  EFFECT_FINISH_CANVAS();
}

#undef NEPTUNE_FLARE_SHEET
#undef NEPTUNE_SPLASH
#undef NEPTUNE_COLUMN_HEAD
