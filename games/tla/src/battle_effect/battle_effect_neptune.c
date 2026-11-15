/// @file
/// The Neptune summon.
// Part of battle_effect.c.

#include "battle_effect_internal.h"

/// Where ParticleSpritesD is decoded in the effect's pictures.
#define NEPTUNE_PARTICLE_SPRITES_D_PICTURES 0x4e20

/// The first stage's flares converging on Neptune's head, and the burst at
/// its end.
#define NEPTUNE_FLARES_FRAME 152
#define NEPTUNE_BURST_FRAME 222
/// WaterSpraySheet: the wave, then the splash's pictures.
#define NEPTUNE_SPLASH PICTURE_BYTES(WATER_WAVE)
#define NEPTUNE_COLUMN_FRAMES 144 ///< frames of the columns of water
#define NEPTUNE_FLARE_SHEET 20000 ///< the flare pictures, after the spray

/// A water column's head, after its shaft: BlueFlameSheet's pictures.
#define NEPTUNE_COLUMN_HEAD PICTURE_BYTES(BLUE_FLAME_COLUMN)

/// Under storm clouds Neptune leaps out of the sea in a burst of spray, light
/// gathers at its head and a beam of light joins its head to the sky, with a
/// splash there; then columns of water crash down on the targets, throwing up
/// drops.
///
/// Act one (244 frames): Neptune is built from eight 32x32 sprites and 712
/// drops of spray wait; the window opens in 16 frames over a ragged edge;
/// Neptune rises from frame 22, a second burst of spray at 76, light gathers
/// at its head from 152, the beam from 176 and the splash from 222, the sea
/// rolling along the bottom all the while. Act two (144 frames): a column of
/// water falls on each target 16 frames apart, throwing up drops where it
/// lands and striking that target; the damage shows at frame 96.
///
/// Abilities: Neptune.
void BattleEffect_RunNeptune(struct BattleEffectParams *args) {
  s32 position[4];
  s32 target[3];
  void *canvas;
  struct BattleEffectWork *work;
  u8 *cells;
  s32 openStep;
  s32 serpentX;
  s32 serpentY;
  s32 serpentVX;
  s32 serpentVY;
  s32 savedScrollX;
  s32 *transition;
  s32 j;
  s32 splashes;
  s32 frame;
  s32 beamFrame;
  s32 i;
  s32 rays;
  s32 glintSize;
  struct Particle *glintSeed;
  struct Particle *glint;
  struct Particle *spark;
  struct Particle *pillar;
  DrawRectangle draw[2];
  struct Scale scale;
  s32 ground = 112;
  u8 *tiles;

  canvas = Ram_HeapSlots->battleCanvas;
  work = Ram_HeapSlots->battleEffect;
  cells = Ram_HeapSlots->battleSheet;
  tiles = gMapGridCellBuffer;

  // Open the canvas over a dark backdrop; BG2 is drawn at half scale
  // below the line the IRQ handler picks.
  BattleEffect_StartCanvasLayer(0);
  BattleEffect_ConfigureEffectDisplay();
  PLTT_BACKDROP = 0;
  BG_PLTT_COLOR(0, 1) = 0;
  Task_Add(BattleEffect_ResetBg2Pa, TASK_PASS_FRAME);
  Irq_SetHandler(2, 0, BattleEffect_SetBg2AffineScaleHalf);
  work->transferMode = CANVAS_COPY;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);
  REG_WININ = EFFECT_WIN0_LAYERS | WININ_WIN1_BG0 | WININ_WIN1_CLR;
  BattleEffect_WipeCanvas(CANVAS_WIPE_ACROSS, CANVAS_WIPE_FILL);
  REG_WIN0H = WIN_RANGE(240, 240);
  BattleEffect_SelectLivingTargets(work->args);
  // Build the serpent: its palette, its tiles, and eight sprites.
  Ram_HeapSlots->battleWork->keepBackdropPalette = 1;
  {
    u8 *sheet = Resource_GetTableEntry((s32)&ResourceId_SeaSerpentTiles);
    Iwram_CopyWords((void *)OBJ_PLTT, sheet, OBJ_PLTT_SIZE);
    sheet += OBJ_PLTT_SIZE;
    Decompress_TaggedLz(sheet, tiles);
    for (i = 0; i != 8; i++) {
      struct Sprite *object = EffectSprite_CreateWithTiles(
          32, 32, OBJ_SIZE_32X32 | OBJ_256_COLORS, 0);

      object->image.priority = 1;
      work->sprites[i] = object;
      Iwram_CopyWords(
          (void *)(OBJ_VRAM0 + gVramCacheEntries[object->slot].offset),
          tiles + i * OBJ_32X32_8BPP_BYTES, OBJ_32X32_8BPP_BYTES);
    }
  }
  gProjection.centerY = SUMMON_PROJECTION_CENTER_Y;
  wait(1);
  BattleBackground_LoadFar(BATTLE_BACKGROUND_FIXED,
                           (s32)&ResourceId_DuskCloudsBackdrop, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, cells, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_WaterSpraySheet, work->sheet, 1,
                             1);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesD,
                             work->sheet + NEPTUNE_PARTICLE_SPRITES_D_PICTURES,
                             0, 0);
  REG_DISPCNT = EFFECT_DISPCNT;
  REG_BG2PA = Q_8_8(0.5);
  REG_BLDALPHA = EFFECT_BLEND_ADD;
  // The serpent starts low on the right, rising to the left.
  REG_BLDCNT = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  openStep = 1;
  serpentX = px(280);
  serpentY = px(200);
  serpentVX = -px(4);
  serpentVY = -px(8);
  savedScrollX = gBgScroll[1].x;
  transition = (s32 *)Ram_HeapSlots->battleBackground;
  EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);
  transition[4] = 1;
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_ADDITIVE);
  draw[0] = (DrawRectangle)Ram_HeapSlots->ramCode;
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS,
                      BLITTER_MAX);
  draw[1] = (DrawRectangle)Ram_HeapSlots->ramCodePaired;
  REG_BG2CNT = BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(7);

  // The glints wait on a wide arc above; the first spray fountains up from
  // the bottom.
  for (i = 0; i != PARTICLE_POOL_COUNT; i++)
    PARTICLES[i].variant = -1;
  for (i = 0, glintSeed = work->particles; i != 32; i++, glintSeed++) {
    s32 a = (Random16() & 0x3fff) + 0x8000;
    s32 r = (Random16() & 127) + 255;

    glintSeed->x = (Trig_Sin(a) * r) >> 2;
    glintSeed->y = (Trig_Cos(a) * r) >> 2;
    glintSeed->velocityX = -glintSeed->x / 32;
    glintSeed->velocityY = -glintSeed->y / 32;
    glintSeed->variant = 0;
  }
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
    if (frame == 24)
      Sound_PlayFar(SE_WAVE);
    if (frame == 76)
      Sound_PlayFar(SE_SCATTER);
    if (frame == 154)
      Sound_PlayFar(SE_SWIRL);
    if (frame == NEPTUNE_BURST_FRAME)
      Sound_PlayFar(SE_CRASH);

    // The window opens: each column's edge from its own noise, the edge
    // moving faster each frame.
    if (frame >= 0 && frame < 16) {
      u16 *phase = (u16 *)Ram_MapCellBuffer;

      if (frame == 1) {
        u8 *noise = Ram_MapCellBuffer + 2;
        s32 k;

        for (k = 0; k != WINDOW_EDGE_INSET_LINES; k++)
          noise[k] = Random16() & 63;
        *phase = 0;
        Task_Add(BattleEffect_BuildWindowEdgeTable, TASK_PASS_FRAME);
      }
      *phase += openStep;
      openStep += 3;
      if (frame == 15)
        Task_Remove(BattleEffect_BuildWindowEdgeTable);
    }

    // The serpent rises, slows, then glides back right and settles.
    if (frame > 21) {
      scale = gNeptune_UnitScale;
      if (frame <= 83) {
        if (frame > 48)
          serpentVX = serpentVX * 60 / DAMPING_SCALE;
        serpentVY = serpentVY * 60 / DAMPING_SCALE;
        if (serpentY > px(132))
          serpentVY -= px(1) / 8;
        else
          serpentVY += px(1) / 8;
        serpentX += serpentVX;
        serpentY += serpentVY;
      } else if (frame <= 170) {
        if (frame <= 107)
          serpentVX += px(1) / 8;
        serpentVX = serpentVX * 60 / DAMPING_SCALE;
        serpentX += serpentVX;
        if (frame <= 117) {
          if (serpentY > px(138))
            serpentVY -= px(1) / 8;
          else
            serpentVY += px(1) / 8;
          serpentVY = serpentVY * 56 / DAMPING_SCALE;
          serpentY += serpentVY;
        }
      }
      position[3] = 0;
      position[1] = 0;
      for (i = 0; i != 8; i++) {
        position[0] = px((i & 3) * 48) + serpentX - px(32);
        position[2] = px((i / 4) * 48) + serpentY - px(24);
        Render_ApplyProjectedPlacementFar(work->sprites[i], position, &scale.x,
                                          0);
      }
    }

    // The second fountain fills the free spray.
    if (frame == 76) {
      for (i = 0; i != PARTICLE_POOL_COUNT; i++) {
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
    if (frame == NEPTUNE_FLARES_FRAME) {
      Irq_SetHandler(2, 96, BattleEffect_SetBg2AffineScaleHalf);
      work->fadeFrames = BACKGROUND_DARKEN_FRAMES;
      work->fadeStep = 0;
    }

    // The glints converge on the mouth.
    beamFrame = frame - NEPTUNE_FLARES_FRAME;
    if (beamFrame >= 0 && beamFrame < 88) {
      for (i = 0, glint = work->particles; i != 32; i++, glint++) {
        if (frame >= i / 4 + NEPTUNE_FLARES_FRAME &&
            frame < i / 4 + NEPTUNE_FLARES_FRAME + 32) {
          s32 fx = Q_16_16_UPPER_HALF(glint->x) + 112;
          s32 fy = Q_16_16_UPPER_HALF(glint->y) + 62;
          s32 r = (i & 3) + 5;

          glintSize = r * 2;
          draw[0](canvas,
                  work->sheet + gBattleEffect_FlarePictures[r - 1] + 0x4e20,
                  fx - r, fy - r, glintSize, glintSize);
          glint->x += glint->velocityX;
          glint->y += glint->velocityY;
        }
      }
    }

    // The breath: a ring of sparks streams from the mouth.
    if (frame == NEPTUNE_BURST_FRAME) {
      for (i = 0; i != ARRAY_COUNT(work->particles); i++) {
        spark = &work->particles[i];
        spark->x = (Random16() & 15) - 8;
        spark->y = (Random16() & 15) - 8;
        if (spark->x < 0)
          spark->x -= 4;
        else
          spark->x += 4;
        if (spark->y < 0)
          spark->y -= 4;
        else
          spark->y += 4;
        spark->x += 100;
        spark->y += 52;
        spark->velocityX = -6;
        spark->z = spark->x;
        spark->velocityY = spark->y;
        spark->variant = 0;
      }
    }
    if (frame >= NEPTUNE_BURST_FRAME) {
      for (i = 0; i != 64; i++) {
        u16 *sparkCells = gBattleEffect_FlarePictures;
        s32 r = 1;

        if (frame >= i / 2 + NEPTUNE_BURST_FRAME) {
          draw[0](canvas, work->sheet + sparkCells[r - 1] + NEPTUNE_FLARE_SHEET,
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

    // The rays: lines fanning out from the mouth, widest at the breath.
    if (frame > 175) {
      rays = 5;
      if (frame > 221) {
        rays = 60;
        if (frame != 223 && frame != 224)
          rays = 40;
      }
      if (rays > 60)
        rays = 60;
      for (i = 0; i != rays; i++) {
        s32 w;
        s32 d = (i + 1) / 2;

        w = rays * 2 - d * 4;
        if (w < 12)
          w = 12;
        if (w > 63)
          w = 63;
        if (i & 1)
          d = -d;
        BattleEffect_DrawClippedCanvasLine(d + 48, 0, d / 2 + 111,
                                           63 - (d + 1) / 2, w);
      }
    }

    // The glow at the mouth grows, then the breath's own pictures.
    if (beamFrame >= 0 && beamFrame < 40) {
      s32 z;

      z = beamFrame / 2 + 1;
      if (z > 4)
        z = 4;
      draw[1](canvas,
              work->sheet + gBattleEffect_FlarePictures[z - 1] +
                  NEPTUNE_FLARE_SHEET,
              108 - z, 60 - z, z * 2, z * 2);
    }
    if (frame >= 192 && frame < 200) {
      s32 z;

      z = (frame - 192) / 2 + 3;
      if (z > 6)
        z = 6;
      draw[1](canvas,
              work->sheet + gBattleEffect_FlarePictures[z - 1] +
                  NEPTUNE_FLARE_SHEET,
              108 - z, 60 - z, z * 2, z * 2);
    }
    if (frame > 199) {
      s32 z;
      s32 u = frame - 200;

      z = u / 4 + 5;
      if (z > 8)
        z = 8;
      draw[1](canvas,
              work->sheet + gBattleEffect_FlarePictures[z - 1] +
                  NEPTUNE_FLARE_SHEET,
              108 - z, 60 - z, z * 2, z * 2);
      if (frame <= 213) {
        z = u / 4 + 1;
        if (z > 4)
          z = 4;
        draw[1](canvas,
                work->sheet + gBattleEffect_FlarePictures[z - 1] +
                    NEPTUNE_FLARE_SHEET,
                100 - z, 52 - z, z * 2, z * 2);
      }
    }
    if (frame >= NEPTUNE_BURST_FRAME - 8 && frame < NEPTUNE_BURST_FRAME) {
      s32 z;

      z = (frame - (NEPTUNE_BURST_FRAME - 8)) / 4;
      if (z > 2)
        z = 2;
      draw[1](canvas,
              work->sheet + z * PICTURE_BYTES(WATER_SPLASH) + NEPTUNE_SPLASH,
              63, 18, WATER_SPLASH_WIDTH, WATER_SPLASH_HEIGHT);
    }
    if (frame > 221) {
      s32 z;

      z = (frame - NEPTUNE_BURST_FRAME) / 2 % 4 + 3;
      draw[0](canvas,
              work->sheet + z * PICTURE_BYTES(WATER_SPLASH) + NEPTUNE_SPLASH,
              72, 30, WATER_SPLASH_WIDTH, WATER_SPLASH_HEIGHT);
      if (z == 5)
        draw[0](canvas,
                work->sheet + NEPTUNE_SPLASH + 2 * PICTURE_BYTES(WATER_SPLASH),
                66, 22, WATER_SPLASH_WIDTH, WATER_SPLASH_HEIGHT);
    }

    // The spray, rising and falling back into the sea.
    if (frame > 27) {
      DrawRectangle drawSpray = draw[1];

      for (i = 0; i != PARTICLE_POOL_COUNT; i++) {
        s32 m = 3;

        if (PARTICLES[i].variant >= 0) {
          s32 k = i % 3 + 2;

          drawSpray(canvas, cells + gBattleEffect_MotePictures[k - 1],
                    Q_16_16_UPPER_HALF(PARTICLES[i].x) - k / 2,
                    Q_16_16_UPPER_HALF(PARTICLES[i].y) - k, k, k * 2);
          BattleEffectKernels_IntegrateVector2(&PARTICLES[i], 62,
                                               gNeptune_Gravity[i & m]);
          PARTICLES[i].variant++;
          if (PARTICLES[i].velocityY > 0 &&
              Q_16_16_UPPER_HALF(PARTICLES[i].y) > 104)
            PARTICLES[i].variant = -1;
        }
      }
    }

    // The sea rolls along the bottom.
    for (i = 0; i != 5; i++)
      draw[1](canvas, work->sheet,
              i * WATER_WAVE_WIDTH + ((frame / 4) & 31) - WATER_WAVE_WIDTH, 88,
              WATER_WAVE_WIDTH, WATER_WAVE_HEIGHT);
    EFFECT_PRESENT_CANVAS(work);
  } while (++frame != 244 &&
           (frame <= 16 || !(gInput.repeat & (KEY_A | KEY_B))));

  // Act two: restore the display; a pillar waits above each target.
  gBgScroll[1].x = savedScrollX;
  transition[4] = 0;
  Irq_SetHandler(2, 0, 0);
  BattleEffect_SetupBlendedDisplay();
  Palette_LoadBasicSpriteColors();
  Dma_Set((const void *)OBJ_PLTT, (void *)&BG_PLTT_COLOR(5, 0),
          DMA_COPY_HALFWORDS(PALETTE_COLORS), REG_DMA3);
  BG_PLTT_COLOR(5, 14) = BG_PLTT_COLOR(15, 4);
  Task_Remove(BattleEffect_ResetBg2Pa);
  REG_WIN0H = WIN_RANGE(0, DISPLAY_WIDTH);
  EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);
  for (i = 0; i != PARTICLE_POOL_COUNT; i++)
    PARTICLES[i].variant = 0;
  for (i = 0; i != 16; i++) {
    work->particles[i].x = (Random16() & 31) + 32;
    work->particles[i].y = 0;
    work->particles[i].variant = 0;
  }
  for (i = 0; i != args->targetCount; i++) {
    EffectPosition_ProjectScaledBattler(args->targets[i],
                                        (struct ScreenPoint *)target);
    work->particles[i].x = target[0] / 2;
  }
  Resource_LoadAndDecompress((s32)&ResourceId_BlueFlameSheet, work->sheet, 1,
                             1);
  Sound_PlayFar(SOUND_FADE_OUT_ACTION_SE);
  REG_BG2PA = Q_8_8(0.5);
  REG_BLDALPHA = EFFECT_BLEND_ADD;

  frame = 0;
  do {
    if (frame == 96)
      EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
    // Each pillar falls 16 pixels a frame and splashes where it lands.
    for (i = 0; i != 5; i++) {
      pillar = &work->particles[i];
      if (frame == i * 16 + 7)
        Sound_PlayFar(SE_CAST);
      if (frame >= i * 16 && pillar->variant <= 31) {
        s32 wobble = frame * 16 % BLUE_FLAME_COLUMN_HEIGHT;

        draw[1](canvas, work->sheet, pillar->x - 8, pillar->y + wobble - 216,
                BLUE_FLAME_COLUMN_WIDTH, BLUE_FLAME_COLUMN_HEIGHT);
        draw[1](canvas, work->sheet, pillar->x - 8, pillar->y + wobble - 112,
                BLUE_FLAME_COLUMN_WIDTH, BLUE_FLAME_COLUMN_HEIGHT - wobble);
        draw[1](canvas, work->sheet + NEPTUNE_COLUMN_HEAD, pillar->x - 17,
                pillar->y - BLUE_FLAME_SPLASH_HEIGHT, BLUE_FLAME_SPLASH_WIDTH,
                BLUE_FLAME_SPLASH_HEIGHT);
        if (pillar->y <= 111)
          pillar->y += 16;
        else
          pillar->variant++;
        if (pillar->y > 111) {
          s32 pillarX;

          splashes = 0;
          pillarX = pillar->x;
          if (pillar->variant <= 7)
            work->shakeFrames = 4;
          for (j = 0; j != PARTICLE_POOL_COUNT; j++) {
            if (PARTICLES[j].variant == 0) {
              s32 r = 0x3ff;
              s32 a;

              r &= Random16();
              a = (Random16() & 0x7fff) - 0x4000;
              PARTICLES[j].x = px(pillarX);
              PARTICLES[j].y = px(ground);
              r += 32;
              PARTICLES[j].velocityX = (Trig_Sin(a) * r) >> 7;
              PARTICLES[j].velocityY = -(Trig_Cos(a) * r * 2) >> 7;
              PARTICLES[j].variant = (Random16() & 7) + 32;
              if (++splashes == 4)
                break;
            }
          }
        }
      }
    }
    for (i = 0; i != args->targetCount; i++) {
      if (frame == i * 16 + 8) {
        EFFECT_STRIKE_TARGET(args, i, 8, KNOCKBACK_LEAP);
      }
    }
    for (i = 0; i != PARTICLE_POOL_COUNT; i++) {
      if (PARTICLES[i].variant > 0) {
        s32 x;
        s32 y;

        PARTICLES[i].variant--;
        BattleEffectKernels_IntegrateVector2(&PARTICLES[i], 60, px(1) / 4);
        y = Q_16_16_TO_INT(PARTICLES[i].y);
        if (y > 120) {
          PARTICLES[i].velocityY = -PARTICLES[i].velocityY / 2;
        } else if (PARTICLES[i].x >= 0 &&
                   (x = Q_16_16_TO_INT(PARTICLES[i].x)) <= 126 &&
                   PARTICLES[i].y >= 0) {
          s32 k = PARTICLES[i].variant / 8 + 3;

          draw[i & 1](canvas, cells + gBattleEffect_MotePictures[k - 1],
                      x - k / 2, y - k, k, k * 2);
        }
      }
    }
    BattleCamera_Shake(4, 8);
    EFFECT_END_FRAME(work);
  } while (++frame != NEPTUNE_COLUMN_FRAMES);

  for (i = 0; i != 8; i++)
    Sprite_DestroyFar(work->sprites[i]);
  EFFECT_FINISH_CANVAS();
}
