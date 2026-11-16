/// @file
/// Ice and Douse: shards and bolts falling on the targets' side.
// Part of battle_effect.c.

#include "battle_effect_internal.h"
#include "sounds.h"
#include "gamepak/far_call/battle.h"
#include "resource_ids.h"

#define SHARD_COUNT(variant) gFalling_IceCounts[(variant) * 2]
#define FRAME_COUNT(variant) gFalling_IceCounts[(variant) * 2 + 1]

enum { FALLING_SHARD_COUNT = 32, FALLING_SHARD_PUFF_COUNT = 32 };

// Shards of ice fall in a slanting column onto the targets' side. Each one
// flies down at 8 pixels a frame until it reaches the ground line, then
// flashes every target, shakes the screen and shatters: fourteen frames of
// breaking pictures, with a burst of sparkle puffs in the middle of them.
// Window 0 is narrowed line by line through an HBlank DMA table into a
// slanting band, so the shards appear only over the targets.

/// A falling shard or bolt keeps falling while its y, in eighths of a pixel, is
/// at most this.
#define FALLING_LAND_Y (80 * 8 - 1)
/// IceShardSheet: the shard, then its shatter, each picture 32x32.
#define ICE_SHARD_WIDTH 32
#define ICE_SHARD_HEIGHT 32

/// Ice shards rain down diagonally onto the targets' side of the field and
/// shatter into puffs of sparkle, flashing and shaking each target as they
/// land.
///
/// Abilities: Ice, Ice Horn, Ice Missile.
void BattleEffect_RunIce(struct BattleEffectParams *args) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  u8 *graphics;
  void *draw[2];
  struct Particle *shard;
  struct Particle *puff;
  u16 *line;
  s32 i;
  s32 k;

  work = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  graphics = Ram_HeapSlots->battleSheet;

  // Set up the canvas, the shard pictures in the sheet and the puffs.
  EFFECT_START_NARROW_CANVAS(BGCNT_WRAP | BGCNT_PRIORITY(1));
  Resource_LoadAndDecompress(RESOURCE_ICE_SHARD_SHEET, work->sheet, 1, 1);
  Resource_LoadAndDecompress(RESOURCE_SPARKLE_DOTS, graphics, 0, 0);
  BattleEffect_ConfigureEffectDisplay();
  REG_BLDCNT = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  REG_WININ = EFFECT_WIN0_LAYERS | WININ_WIN1_BG0 | WININ_WIN1_BG1 |
              WININ_WIN1_OBJ | WININ_WIN1_CLR;

  // Line the shards up above the screen, each 8 pixels higher and further
  // out than the one before, with a little scatter.
  {
    s32 y;
    s32 slant;

    for (i = 0, y = -128, slant = -16; i != FALLING_SHARD_COUNT;
         y -= 64, i++, slant -= 8) {
      s32 x;

      shard = &work->particles[i];
      x = (Random16() & 63) + (Random16() & 7) + 24;
      if (args->side == EFFECT_SIDE_PARTY)
        x = x + slant + 24;
      else
        x = x - slant + 80;
      shard->x = x << 3;
      shard->variant = -1;
      shard->y = y;
    }
  }
  for (i = 0; i != FALLING_SHARD_PUFF_COUNT; i++)
    work->particles[FALLING_SHARD_COUNT + i].variant = -1;

  // Falling shards draw at their maximum; the puffs add their light. Both
  // mirror for the other side.
  if (args->side == EFFECT_SIDE_ENEMIES) {
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, COMPOSITE_CLIP_Y, BLITTER_MAX);
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, COMPOSITE_CLIP_Y,
                        BLITTER_ADDITIVE);
  } else {
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, COMPOSITE_CLIP_Y | COMPOSITE_FLIP_X,
                        BLITTER_MAX);
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
                        COMPOSITE_CLIP_Y | COMPOSITE_FLIP_X, BLITTER_ADDITIVE);
  }
  draw[0] = Ram_HeapSlots->ramCode;
  draw[1] = Ram_HeapSlots->ramCodePaired;

  // Window 0's left and right edge for each line: a band slanting with the
  // shards' fall, then the targets' full width, then nothing.
  if (args->side == EFFECT_SIDE_ENEMIES) {
    line = gMapGridCellBuffer;
    for (i = 0; i != DISPLAY_HEIGHT; i++) {
      if (i >= 8 && i <= 103)
        line[i] = WIN_RANGE(112 - i, 240 - i);
      else if (i <= 135)
        line[i] = WIN_RANGE(8, 136);
      else
        line[i] = WIN_RANGE(1, 0);
    }
  } else {
    line = gMapGridCellBuffer;
    for (i = 0; i != DISPLAY_HEIGHT; i++) {
      if (i >= 8 && i <= 95)
        line[i] = WIN_RANGE(i + 24, i - 8 + 160);
      else if (i <= 135)
        line[i] = WIN_RANGE(120, 248);
      else
        line[i] = WIN_RANGE(1, 0);
    }
  }

  Task_Add(BattleEffect_ArmWin0HBlankDma, TASK_PASS_FRAME);
  work->transferMode = CANVAS_COPY_AND_FADE;
  if (args->variant == EFFECT_VARIANT_STRONGER)
    work->transferValue = CANVAS_FADE_THREE_QUARTERS;
  else
    work->transferValue = CANVAS_FADE_HALF;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);

  for (frame = 0; frame != FRAME_COUNT(args->variant); frame++) {
    if (frame == FRAME_COUNT(args->variant) - 16)
      EFFECT_SHOW_DAMAGE(SE_IMPACT);

    for (i = 0; i != SHARD_COUNT(args->variant); i++) {
      shard = &work->particles[i];

      // A falling shard moves 8 pixels down and 8 across; at the ground
      // line it lands.
      if (shard->variant == -1) {
        s32 x = shard->x / 8;
        s32 y = shard->y / 8;

        ((DrawRectangle)
             draw[args->variant == EFFECT_VARIANT_STRONGEST ? 1 : 0])(
            canvas, work->sheet, x, y, ICE_SHARD_WIDTH, ICE_SHARD_HEIGHT);
        if (shard->y <= FALLING_LAND_Y) {
          if (args->side == EFFECT_SIDE_ENEMIES)
            shard->x -= 64;
          else
            shard->x += 64;
          shard->y += 64;
        } else {
          if ((i & 3) == 0)
            Sound_PlayFar(SE_ICE);
          work->shakeFrames = 2;
          shard->variant = 0;
          for (k = 0; k != args->targetCount; k++)
            BattleEffect_TintBattler(args->targets[k], SPRITE_COLOR_BLUE_TINT,
                                     POSE_DAMAGE, k, 8);
        }
      }

      // A landed shard breaks over fourteen frames, three to a picture,
      // and throws a sparkle puff on each of frames 9 to 11.
      if (shard->variant != -1) {
        s32 x = shard->x / 8;
        s32 y = shard->y / 8;

        if (shard->variant >= 1 && shard->variant <= 14) {
          ((DrawRectangle)
               draw[args->variant == EFFECT_VARIANT_STRONGEST ? 1 : 0])(
              canvas,
              work->sheet + PICTURE_BYTES(ICE_SHARD) +
                  ((shard->variant / 3) << 10),
              x, y, ICE_SHARD_WIDTH, ICE_SHARD_HEIGHT);
        }
        if (shard->variant >= 9 && shard->variant <= 11) {
          for (k = 0; k != FALLING_SHARD_PUFF_COUNT; k++) {
            puff = &work->particles[FALLING_SHARD_COUNT + k];
            if (puff->variant == -1) {
              puff->variant = 18;
              puff->x = (((Random16() & 31) + shard->x / 8) << 3) + 8;
              puff->y = ((Random16() & 15) + shard->y / 8 - 15) << 3;
              break;
            }
          }
        }
        if (shard->variant <= 14)
          shard->variant++;
      }
    }

    // Each puff counts down from 18 through its nine pictures, two frames
    // apiece, centred on its point.
    for (i = 0; i != FALLING_SHARD_PUFF_COUNT; i++) {
      puff = &work->particles[FALLING_SHARD_COUNT + i];
      if (puff->variant != -1) {
        if (puff->variant <= 17) {
          s32 image = puff->variant / 2;
          s32 x = puff->x / 8 - (gBattleEffect_SparkleSizes[image] >> 1);
          s32 y = puff->y / 8 - (gBattleEffect_SparkleSizes[image] >> 1);

          ((DrawRectangle)
               draw[args->variant == EFFECT_VARIANT_STRONGEST ? 1 : 0])(
              canvas, graphics + gBattleEffect_SparklePictures[image], x, y,
              gBattleEffect_SparkleSizes[image],
              gBattleEffect_SparkleSizes[image]);
        }
        if (puff->variant > -1)
          puff->variant--;
      }
    }

    BattleEffect_CountDownBattlerTints();
    BattleCamera_Shake(4, 4);
    EFFECT_PRESENT_CANVAS(work);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  Task_Remove(BattleEffect_ArmWin0HBlankDma);
  EFFECT_RELEASE_BLITTERS();
  BattleEffect_EndCanvasLayer();
  BattleEffect_ConfigureEffectDisplay();
}

#undef FRAME_COUNT

#define gDropSplashes ((struct DropSplashBuffer *)Ram_MapCellBuffer)

#define DROP_COUNT(variant) gFalling_Counts[(variant) * 4]
#define SPLASH_COUNT(variant) gFalling_Counts[(variant) * 4 + 1]
#define DROP_SPACING(variant) gFalling_Counts[(variant) * 4 + 2]
#define FRAME_COUNT(variant) gFalling_Counts[(variant) * 4 + 3]

// Drops of water fall in a slanting column onto the targets' side, 8 pixels
// down and 4 across a frame. At the ground line each one flashes every
// target and bursts: its splash pictures play for eight frames while a spray
// of droplets is thrown up and falls back under gravity. Window 0 is narrowed
// line by line through an HBlank DMA table into a slanting band over the
// targets. Deluge throws its droplets higher.

/// Where BlueBeamSheet is decoded in the effect's pictures.
#define DOUSE_BLUE_BEAM_PICTURES 0x604

#define DOUSE_DROP_WIDTH 24
#define DOUSE_DROP_HEIGHT 24
/// The small and the large splash in ParticleSpritesD, decoded at the start
/// of the effect's pictures.
#define DOUSE_SPLASH 0x844
#define DOUSE_BURST 0xa84
#define DOUSE_BURST_PICTURE_WIDTH 42
#define DOUSE_BURST_PICTURE_HEIGHT 42

/// Blue bolts streak down diagonally onto the targets' side of the field, each
/// bursting into a fountain of falling sparks where it lands, while the targets
/// flash.
///
/// Abilities: Douse, Drench, Deluge.
void BattleEffect_RunDouse(struct BattleEffectParams *args) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  DrawRectangle draw[2];
  struct Particle *drop;
  struct Particle *splash;
  u16 *line;
  s32 i;
  s32 k;

  work = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;

  // Set up the canvas, the drop pictures and the droplets.
  EFFECT_START_NARROW_CANVAS(BGCNT_WRAP | BGCNT_PRIORITY(1));
  Resource_LoadAndDecompress(RESOURCE_BLUE_BEAM_SHEET,
                             work->sheet + DOUSE_BLUE_BEAM_PICTURES, 1, 1);
  Resource_LoadAndDecompress(RESOURCE_PARTICLE_SPRITES_D, work->sheet, 0, 0);
  BattleEffect_ConfigureEffectDisplay();
  REG_BLDCNT = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  REG_WININ = EFFECT_WIN0_LAYERS | WININ_WIN1_BG0 | WININ_WIN1_BG1 |
              WININ_WIN1_OBJ | WININ_WIN1_CLR;

  // Drops draw at their maximum; droplets add their light.
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, COMPOSITE_CLIP_Y, BLITTER_MAX);
  draw[0] = Ram_HeapSlots->ramCode;
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, COMPOSITE_CLIP_Y,
                      BLITTER_ADDITIVE);
  draw[1] = Ram_HeapSlots->ramCodePaired;

  for (i = 0; i != ARRAY_COUNT(gDropSplashes->splashes); i++)
    gDropSplashes->splashes[i].variant = -1;

  // Line the drops up above the screen along the slant, each the variant's
  // spacing higher than the one before.
  for (i = 0; i != ARRAY_COUNT(work->particles); i++) {
    s32 x;
    s32 y;

    drop = &work->particles[i];
    x = Random16() & 63;
    y = -(i * DROP_SPACING(args->variant) + 16);
    if (args->side == EFFECT_SIDE_PARTY)
      x = x + y / 2 - 48;
    else
      x = x - y / 2 + 72;
    drop->x = x << 3;
    drop->y = y << 3;
    drop->variant = -1;
  }

  // Window 0's left and right edge for each line: a band slanting with the
  // drops' fall, then the targets' width, then nothing.
  if (args->side == EFFECT_SIDE_ENEMIES) {
    line = gMapGridCellBuffer;
    for (i = 0; i != DISPLAY_HEIGHT; i++) {
      if (i >= 8 && i <= 103)
        line[i] = WIN_RANGE(52 - i / 2, 180 - i / 2);
      else if (i <= 135)
        line[i] = WIN_RANGE(0, 128);
      else
        line[i] = WIN_RANGE(1, 0);
    }
  } else {
    line = gMapGridCellBuffer;
    for (i = 0; i != DISPLAY_HEIGHT; i++) {
      if (i >= 8 && i <= 103)
        line[i] = WIN_RANGE(i / 2 + 60, i / 2 + 188);
      else if (i <= 135)
        line[i] = WIN_RANGE(112, 240);
      else
        line[i] = WIN_RANGE(1, 0);
    }
  }

  // Douse copies the canvas plainly; Drench and Deluge blend it in.
  Task_Add(BattleEffect_ArmWin0HBlankDma, TASK_PASS_FRAME);
  if (args->variant == EFFECT_VARIANT_BASIC) {
    EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_CLEAR, 0);
  } else if (args->variant == EFFECT_VARIANT_STRONGER) {
    EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
  } else {
    EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);
  }
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);

  for (frame = 0; frame != FRAME_COUNT(args->variant); frame++) {
    if (frame == FRAME_COUNT(args->variant) - 64)
      EFFECT_SHOW_DAMAGE(SE_IMPACT);

    for (i = 0; i != DROP_COUNT(args->variant); i++) {
      s32 x;
      s32 y;

      drop = &work->particles[i];
      x = drop->x / 8;
      y = drop->y / 8;
      if (drop->variant == -1) {
        // Falling: draw the drop and move it on, or land it.
        draw[0](canvas, work->sheet + DOUSE_BLUE_BEAM_PICTURES, x, y, 24,
                DOUSE_DROP_HEIGHT);
        if (drop->y <= FALLING_LAND_Y) {
          if (args->side == EFFECT_SIDE_ENEMIES)
            drop->x -= 32;
          else
            drop->x += 32;
          drop->y += 64;
        } else {
          // Landed: throw this drop's droplets up from its middle in 16.16
          // fixed point, sound every fourth landing and flash the targets.
          drop->variant = 0;
          for (k = 0; k != SPLASH_COUNT(args->variant); k++) {
            splash =
                &gDropSplashes->splashes[i * SPLASH_COUNT(args->variant) + k];
            splash->x = INTtoFIXED(x + 12);
            splash->y = INTtoFIXED(y);
            splash->velocityX = ((Random16() & 255) - 128) << 9;
            if (args->variant == EFFECT_VARIANT_STRONGEST)
              splash->velocityY = ((Random16() & 0x1ff) - 0x180) << 10;
            else
              splash->velocityY = ((Random16() & 255) - 255) << 10;
            splash->variant = (Random16() & 15) + 16;
          }
          if ((i & 3) == 0)
            Sound_PlayFar(SE_WATER);
          for (k = 0; k != args->targetCount; k++)
            BattleEffect_TintBattler(args->targets[k], SPRITE_COLOR_WHITE,
                                     POSE_DAMAGE, k, 2);
        }
      } else {
        // Landed: four frames of the small splash, four of the large one.
        if (drop->variant >= 0 && drop->variant <= 3)
          draw[0](canvas, work->sheet + DOUSE_SPLASH, x, y, DOUSE_DROP_WIDTH,
                  DOUSE_DROP_HEIGHT);
        else if (drop->variant <= 7)
          draw[0](canvas, work->sheet + DOUSE_BURST, x - 9, y - 9,
                  DOUSE_BURST_PICTURE_WIDTH, DOUSE_BURST_PICTURE_HEIGHT);
        if (drop->variant <= 14)
          drop->variant++;
      }
    }

    // Each droplet shrinks as its life runs out, centred on its point.
    for (i = 0; i != ARRAY_COUNT(gDropSplashes->splashes); i++) {
      splash = &gDropSplashes->splashes[i];
      if (splash->variant != -1) {
        s32 size = splash->variant + 1;

        if (size > 6)
          size = 6;
        draw[1](canvas, work->sheet + gBattleEffect_FlarePictures[size - 1],
                ((s16 *)&splash->x)[1] - size, ((s16 *)&splash->y)[1] - size,
                size * 2, size * 2);
        BattleEffectKernels_IntegrateVector2(splash, 60, toFIXED(1) / 8);
        splash->variant--;
      }
    }

    EFFECT_END_FRAME(work);
  }

  Task_Remove(BattleEffect_ArmWin0HBlankDma);
  EFFECT_FINISH_CANVAS();
  BattleEffect_ConfigureEffectDisplay();
}
#undef SHARD_COUNT
#undef DROP_COUNT
