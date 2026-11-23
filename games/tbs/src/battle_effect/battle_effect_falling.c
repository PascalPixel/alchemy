/// @file
/// Ice and Douse: shards and drops of water falling on the targets' side.
// Part of battle_effect.c.

#include "battle_effect_internal.h"

/// Fills window 0's per-line edges: from line 8 to line last, slant, the band
/// the shards and drops fall down, which moves along as the lines go down until
/// the line before it reaches span, the targets' side; span on the other lines
/// to the battle view's bottom; nothing below it.
#define FALLING_BUILD_SLANTED_WINDOW(line, i, last, slant, span)               \
  for ((i) = 0; (i) != DISPLAY_HEIGHT; (i)++) {                                \
    if ((i) >= 8 && (i) <= (last))                                             \
      (line)[i] = (slant);                                                     \
    else if ((i) < BATTLE_VIEW_BOTTOM)                                         \
      (line)[i] = (span);                                                      \
    else                                                                       \
      (line)[i] = WINDOW_SPAN_INVERTED;                                        \
  }
/// The band from left, as wide as the canvas.
#define FALLING_BAND(left) WIN_RANGE(left, (left) + CANVAS_SIZE)

// The falling shards (Ice) and drops of water (Douse) rain diagonally onto the
// targets' side, seen only through a slanted band of window 0 whose edges an
// HBlank DMA feeds line by line; each lands just above line 80, shards
// shattering into sparkle and drops bursting in a splash that throws up
// droplets.

/// The shards or drops that fall, and the slots for the sparkle or splash
/// they burst into, which follow them in PARTICLES.
enum { FALLING_SHARD_COUNT = 32, FALLING_SHARD_PUFF_COUNT = 32 };

/// A falling shard or drop keeps falling while its y, in eighths of a pixel, is
/// at most this.
#define FALLING_LAND_Y (80 * 8 - 1)
/// IceShardSheet: the shard, then its shatter, each picture 32x32.
#define ICE_SHARD_WIDTH 32
#define ICE_SHARD_HEIGHT 32
/// A landed shard or drop counts its frames since landing up to one past this.
#define FALLING_LANDED_LAST_FRAME 14
/// A landed shard shows its shatter's pictures from its first frame to
/// FALLING_LANDED_LAST_FRAME, ICE_SHATTER_PICTURE_FRAMES each, and kicks up a
/// puff on frames ICE_PUFF_FIRST_FRAME to ICE_PUFF_LAST_FRAME.
#define ICE_SHATTER_PICTURE_FRAMES 3
#define ICE_PUFF_FIRST_FRAME 9
#define ICE_PUFF_LAST_FRAME 11
/// A puff's frames: the nine sparkle pictures, two frames each.
#define ICE_PUFF_FRAMES 18

/// Ice shards rain down diagonally onto the targets' side of the field and
/// shatter into puffs of sparkle, flashing and shaking each target as they
/// land.
///
/// Abilities: Ice, Ice Horn, Ice Missile.
void BattleEffect_RunIce(struct BattleEffectParams *args) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  u8 *sparkles;
  BlitterPair draw;
  struct Particle *shard;
  struct Particle *puff;
  u16 *line;
  s32 i;
  s32 k;

  work = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
  canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
  sparkles = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_SHEET);
  work->args = args;
  EFFECT_START_NARROW_CANVAS(BGCNT_WRAP | BGCNT_PRIORITY(1));
  Resource_LoadAndDecompress(RESOURCE_ICE_SHARD_SHEET, work->sheet, 1, 1);
  Resource_LoadAndDecompress(RESOURCE_SPARKLE_DOTS, sparkles, 0, 0);
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
      if (work->args->side == EFFECT_SIDE_PARTY)
        x = x + slant + 24;
      else
        x = x - slant + 80;
      shard->y = y;
      shard->x = x << 3;
      shard->variant = -1; // still falling
    }
  }
  for (i = 0; i != FALLING_SHARD_PUFF_COUNT; i++)
    work->particles[FALLING_SHARD_COUNT + i].variant = -1; // puff slot free

  // Falling shards draw at their maximum; the puffs add their light. Both
  // mirror for the other side.
  if (work->args->side == EFFECT_SIDE_ENEMIES) {
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, BLITTER_CLIP_Y, COMPOSITE_LIGHTEN);
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, BLITTER_CLIP_Y,
                        COMPOSITE_ADDITIVE);
  } else {
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, BLITTER_CLIP_Y | COMPOSITE_FLIP_X,
                        COMPOSITE_LIGHTEN);
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
                        BLITTER_CLIP_Y | COMPOSITE_FLIP_X, COMPOSITE_ADDITIVE);
  }
  draw[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
  draw[1] = EFFECT_BLITTERS[HEAP_SLOT_RAM_CODE_PAIRED];

  if (work->args->side == EFFECT_SIDE_ENEMIES) {
    line = (u16 *)Ram_MapGridCellBuffer;
    FALLING_BUILD_SLANTED_WINDOW(line, i, 103, FALLING_BAND(112 - i),
                                 FALLING_BAND(8));
  } else {
    line = (u16 *)Ram_MapGridCellBuffer;
    FALLING_BUILD_SLANTED_WINDOW(line, i, 95, FALLING_BAND(i + 24),
                                 FALLING_BAND(120));
  }

  Task_Add(BattleEffect_ArmWin0HBlankDma, TASK_PASS_VBLANK);
  work->transferMode = CANVAS_COPY_AND_FADE;
  if (work->args->variant == EFFECT_VARIANT_STRONGER)
    work->transferValue = CANVAS_FADE_THREE_QUARTERS;
  else
    work->transferValue = CANVAS_FADE_HALF;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_VBLANK);

  for (frame = 0; frame != gFalling_IceCounts[work->args->variant][ICE_FRAMES];
       frame++) {
    if (frame == gFalling_IceCounts[work->args->variant][ICE_FRAMES] - 16)
      EFFECT_SHOW_DAMAGE(SE_WATER);

    // Shards fall down the slant, and on landing shatter, kicking up puffs.
    for (i = 0; i != gFalling_IceCounts[work->args->variant][ICE_SHARD_COUNT];
         i++) {
      shard = &work->particles[i];
      if (shard->variant == -1) {
        s32 x = shard->x / 8;
        s32 y = shard->y / 8;

        draw[work->args->variant == EFFECT_VARIANT_STRONGEST ? 1 : 0](
            canvas, work->sheet, x, y, ICE_SHARD_WIDTH, ICE_SHARD_HEIGHT);
        if (shard->y <= FALLING_LAND_Y) { // above y 80
          if (work->args->side == EFFECT_SIDE_ENEMIES)
            shard->x -= 64;
          else
            shard->x += 64;
          shard->y += 64;
        } else {
          // Every target flashes blue as the shards land.
          if ((i & 3) == 0)
            Sound_PlayFar(SE_ICE);
          work->shakeFrames = 2;
          shard->variant = 0;
          EFFECT_FOR_EACH_TARGET(k, work->args)
            BattleEffect_TintBattler(work->args->targets[k],
                                     SPRITE_COLOR_BLUE_TINT, POSE_DAMAGE, k, 8);
        }
      }
      if (shard->variant != -1) {
        s32 x = shard->x / 8;
        s32 y = shard->y / 8;

        if (shard->variant >= 1 &&
            shard->variant <= FALLING_LANDED_LAST_FRAME) {
          draw[work->args->variant == EFFECT_VARIANT_STRONGEST ? 1 : 0](
              canvas,
              work->sheet + PICTURE_BYTES(ICE_SHARD) +
                  shard->variant / ICE_SHATTER_PICTURE_FRAMES *
                      PICTURE_BYTES(ICE_SHARD),
              x, y, ICE_SHARD_WIDTH, ICE_SHARD_HEIGHT);
        }
        if (shard->variant >= ICE_PUFF_FIRST_FRAME &&
            shard->variant <= ICE_PUFF_LAST_FRAME) {
          for (k = 0; k != FALLING_SHARD_PUFF_COUNT; k++) {
            puff = &work->particles[FALLING_SHARD_COUNT + k];
            if (puff->variant == -1) {
              puff->variant = ICE_PUFF_FRAMES;
              puff->x = (((Random16() & 31) + shard->x / 8) << 3) + 8;
              puff->y = ((Random16() & 15) + shard->y / 8 - 15) << 3;
              break;
            }
          }
        }
        if (shard->variant <= FALLING_LANDED_LAST_FRAME)
          shard->variant++;
      }
    }

    // Puffs twinkle out.
    for (i = 0; i != FALLING_SHARD_PUFF_COUNT; i++) {
      puff = &work->particles[FALLING_SHARD_COUNT + i];
      if (puff->variant != -1) {
        if (puff->variant < ICE_PUFF_FRAMES) {
          s32 image = puff->variant / 2;
          s32 x = puff->x / 8 - (gBattleEffect_SparkleSizes[image] >> 1);
          s32 y = puff->y / 8 - (gBattleEffect_SparkleSizes[image] >> 1);

          draw[work->args->variant == EFFECT_VARIANT_STRONGEST ? 1 : 0](
              canvas, sparkles + gBattleEffect_SparklePictures[image], x, y,
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

/// Ram_MapGridCellBuffer as Douse uses it: window 0's table, then the drops'
/// droplets.
struct DropSplashBuffer {
  u16 lines[DISPLAY_HEIGHT]; ///< WIN0H for each line, read by the HBlank DMA
  struct Particle droplets[512]; ///< free while -1
};

/// Ram_MapGridCellBuffer seen as Douse's struct DropSplashBuffer.
#define DOUSE_SPLASH_BUFFER ((struct DropSplashBuffer *)Ram_MapGridCellBuffer)

/// BlueBeamSheet, after ParticleSpritesD's ten flare pictures (as many bytes as
/// two sets of motes): the falling drop, a blue bubble; the small splash it
/// lands in, as large as the drop; then the large splash.
#define DOUSE_DROP_WIDTH 24
#define DOUSE_DROP_HEIGHT 24
#define DOUSE_LARGE_SPLASH_WIDTH 42
#define DOUSE_LARGE_SPLASH_HEIGHT 42
#define DOUSE_DROP_PICTURE (2 * MOTE_SET_BYTES)
#define DOUSE_SMALL_SPLASH_PICTURE                                             \
  (DOUSE_DROP_PICTURE + PICTURE_BYTES(DOUSE_DROP))
#define DOUSE_LARGE_SPLASH_PICTURE                                             \
  (DOUSE_SMALL_SPLASH_PICTURE + PICTURE_BYTES(DOUSE_DROP))

/// Drops of water fall diagonally onto the targets' side of the field, 8 pixels
/// down and 4 across a frame, each bursting in a splash that throws up droplets
/// where it lands, while the targets flash; Deluge throws them higher.
///
/// Abilities: Douse, Drench, Deluge.
void BattleEffect_RunDouse(struct BattleEffectParams *args) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  DrawRectangle drawDroplet;
  DrawRectangle drawDrop;
  struct Particle *drop;
  struct Particle *droplet;
  u16 *line;
  s32 i;
  s32 k;

  work = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
  canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
  work->args = args;
  EFFECT_START_NARROW_CANVAS(BGCNT_WRAP | BGCNT_PRIORITY(1));
  Resource_LoadAndDecompress(RESOURCE_BLUE_BEAM_SHEET,
                             work->sheet + DOUSE_DROP_PICTURE, 1, 1);
  Resource_LoadAndDecompress(RESOURCE_PARTICLE_SPRITES_D, work->sheet, 0, 0);
  BattleEffect_ConfigureEffectDisplay();
  REG_BLDCNT = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  REG_WININ = EFFECT_WIN0_LAYERS | WININ_WIN1_BG0 | WININ_WIN1_BG1 |
              WININ_WIN1_OBJ | WININ_WIN1_CLR;

  // Drops draw at their maximum; droplets add their light.
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, BLITTER_CLIP_Y, COMPOSITE_LIGHTEN);
  drawDrop = HEAP_SLOT_BLOCK(HEAP_SLOT_RAM_CODE);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, BLITTER_CLIP_Y,
                      COMPOSITE_ADDITIVE);
  drawDroplet = HEAP_SLOT_BLOCK(HEAP_SLOT_RAM_CODE_PAIRED);

  EFFECT_FREE_PARTICLES(i, DOUSE_SPLASH_BUFFER->droplets,
                        ARRAY_COUNT(DOUSE_SPLASH_BUFFER->droplets));

  // Line the drops up above the screen along the slant, each the variant's
  // spacing higher than the one before.
  for (i = 0; i != ARRAY_COUNT(work->particles); i++) {
    s32 x;
    s32 y;

    drop = &work->particles[i];
    x = Random16() & 63;
    y = -(i * gFalling_Counts[work->args->variant][DOUSE_DROP_SPACING] + 16);
    if (work->args->side == EFFECT_SIDE_PARTY)
      x = x + y / 2 - 48;
    else
      x = x - y / 2 + 72;
    drop->x = x << 3;
    drop->y = y << 3;
    drop->variant = -1; // still falling
  }

  if (work->args->side == EFFECT_SIDE_ENEMIES) {
    line = (u16 *)Ram_MapGridCellBuffer;
    FALLING_BUILD_SLANTED_WINDOW(line, i, 103, FALLING_BAND(52 - i / 2),
                                 FALLING_BAND(0));
  } else {
    line = (u16 *)Ram_MapGridCellBuffer;
    FALLING_BUILD_SLANTED_WINDOW(line, i, 103, FALLING_BAND(i / 2 + 60),
                                 FALLING_BAND(112));
  }

  // Douse copies the canvas plainly; Drench and Deluge blend it in.
  Task_Add(BattleEffect_ArmWin0HBlankDma, TASK_PASS_VBLANK);
  if (work->args->variant == EFFECT_VARIANT_BASIC) {
    EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_CLEAR, 0);
  } else if (work->args->variant == EFFECT_VARIANT_STRONGER) {
    EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
  } else {
    EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);
  }
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_VBLANK);

  for (frame = 0; frame != gFalling_Counts[work->args->variant][DOUSE_FRAMES];
       frame++) {
    if (frame == gFalling_Counts[work->args->variant][DOUSE_FRAMES] - 64)
      EFFECT_SHOW_DAMAGE(SE_WATER);

    // Drops fall down the slant, then splash.
    for (i = 0; i != gFalling_Counts[work->args->variant][DOUSE_DROP_COUNT];
         i++) {
      s32 x;
      s32 y;

      drop = &work->particles[i];
      x = drop->x / 8;
      y = drop->y / 8;
      if (drop->variant == -1) {
        drawDrop(canvas, work->sheet + DOUSE_DROP_PICTURE, x, y,
                 DOUSE_DROP_WIDTH, DOUSE_DROP_HEIGHT);
        if (drop->y <= FALLING_LAND_Y) {
          if (work->args->side == EFFECT_SIDE_ENEMIES)
            drop->x -= 32;
          else
            drop->x += 32;
          drop->y += 64;
        } else {
          // Droplets fly up from the middle of the landed drop.
          drop->variant = 0;
          for (k = 0;
               k !=
               gFalling_Counts[work->args->variant][DOUSE_DROPLETS_PER_DROP];
               k++) {
            droplet =
                &DOUSE_SPLASH_BUFFER
                     ->droplets[i * gFalling_Counts[work->args->variant]
                                                   [DOUSE_DROPLETS_PER_DROP] +
                                k];
            droplet->x = INTtoFIXED(x + DOUSE_DROP_WIDTH / 2);
            droplet->y = INTtoFIXED(y);
            droplet->velocityX = ((Random16() & 255) - 128) << 9;
            if (work->args->variant == EFFECT_VARIANT_STRONGEST)
              droplet->velocityY = ((Random16() & 0x1ff) - 384) << 10;
            else
              droplet->velocityY = ((Random16() & 255) - 255) << 10;
            droplet->variant = (Random16() & 15) + 16;
          }
          // Every target flashes white as the drops land.
          if ((i & 3) == 0)
            Sound_PlayFar(SE_WATER);
          EFFECT_FOR_EACH_TARGET(k, work->args)
            BattleEffect_TintBattler(work->args->targets[k], SPRITE_COLOR_WHITE,
                                     POSE_DAMAGE, k, 2);
        }
      } else {
        // Four frames of the small splash, four of the large one.
        if (drop->variant >= 0 && drop->variant <= 3)
          drawDrop(canvas, work->sheet + DOUSE_SMALL_SPLASH_PICTURE, x, y,
                   DOUSE_DROP_WIDTH, DOUSE_DROP_HEIGHT);
        else if (drop->variant <= 7)
          drawDrop(canvas, work->sheet + DOUSE_LARGE_SPLASH_PICTURE,
                   x - (DOUSE_LARGE_SPLASH_WIDTH - DOUSE_DROP_WIDTH) / 2,
                   y - (DOUSE_LARGE_SPLASH_HEIGHT - DOUSE_DROP_HEIGHT) / 2,
                   DOUSE_LARGE_SPLASH_WIDTH, DOUSE_LARGE_SPLASH_HEIGHT);
        if (drop->variant <= FALLING_LANDED_LAST_FRAME)
          drop->variant++;
      }
    }
    // Droplets fall and shrink as they die.
    for (i = 0; i != ARRAY_COUNT(DOUSE_SPLASH_BUFFER->droplets); i++) {
      droplet = &DOUSE_SPLASH_BUFFER->droplets[i];
      if (droplet->variant != -1) {
        s32 size = droplet->variant + 1;

        if (size > 6)
          size = 6;
        EFFECT_DRAW_FLARE(drawDroplet, canvas, work->sheet,
                          FIXEDtoINT(droplet->x), FIXEDtoINT(droplet->y), size);
        Particle_AdvanceWithGravity2D(droplet, 60, toFIXED(1) / 8);
        droplet->variant--;
      }
    }
    EFFECT_END_FRAME(work);
  }

  Task_Remove(BattleEffect_ArmWin0HBlankDma);
  EFFECT_FINISH_CANVAS();
  BattleEffect_ConfigureEffectDisplay();
}

#undef FALLING_BUILD_SLANTED_WINDOW
#undef FALLING_BAND
#undef FALLING_LANDED_LAST_FRAME
#undef ICE_SHATTER_PICTURE_FRAMES
#undef ICE_PUFF_FIRST_FRAME
#undef ICE_PUFF_LAST_FRAME
#undef ICE_PUFF_FRAMES
#undef FALLING_LAND_Y
#undef DOUSE_SPLASH_BUFFER
#undef DOUSE_DROP_WIDTH
#undef DOUSE_DROP_HEIGHT
#undef DOUSE_LARGE_SPLASH_WIDTH
#undef DOUSE_LARGE_SPLASH_HEIGHT
#undef DOUSE_DROP_PICTURE
#undef DOUSE_SMALL_SPLASH_PICTURE
#undef DOUSE_LARGE_SPLASH_PICTURE
