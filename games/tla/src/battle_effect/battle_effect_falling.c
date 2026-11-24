/// @file
/// Ice and Douse: shards and drops of water falling on the targets' side.
// Part of battle_effect.c.

#include "battle_effect_internal.h"
#include "sounds.h"
#include "gamepak/far_call/battle.h"
#include "resource_ids.h"

/// Fills window 0's per-line edges: from line 8 to line lastY, slant, the band
/// the shards and drops fall down, which moves along as the lines go down until
/// the line before it reaches span, the targets' side; span on the other lines
/// to the battle view's bottom; nothing below it.
#define FALLING_BUILD_SLANTED_WINDOW(line, i, lastY, slant, span)              \
  for ((i) = 0; (i) != DISPLAY_HEIGHT; (i)++) {                                \
    if ((i) >= 8 && (i) <= (lastY))                                            \
      (line)[i] = (slant);                                                     \
    else if ((i) < BATTLE_VIEW_BOTTOM)                                         \
      (line)[i] = (span);                                                      \
    else                                                                       \
      (line)[i] = WIN_RANGE(1, 0);                                             \
  }
/// The band from leftX, as wide as the canvas.
#define FALLING_BAND(leftX) WIN_RANGE(leftX, (leftX) + CANVAS_SIZE)

// The falling shards (Ice) and drops of water (Douse) rain diagonally onto the
// targets' side, seen only through a slanted band of window 0 whose edges an
// HBlank DMA feeds line by line; each lands just above line 80, shards
// shattering into sparkle and drops bursting in a splash that throws up
// droplets.

/// Each Ice variant's shards that fall, and its frames; the damage shows 16
/// frames before the end.
#define ICE_SHARDS(variant) gIce_Counts[(variant) * 2]
#define ICE_FRAMES(variant) gIce_Counts[(variant) * 2 + 1]

/// The shards or drops that can fall at once, and their puffs as they land.
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
  struct BattleEffectState *state;
  void *canvas;
  s32 frame;
  u8 *sparkles;
  void *draw[2];
  struct Particle *shard;
  struct Particle *puff;
  u16 *line;
  s32 i;
  s32 k;

  state = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  sparkles = Ram_HeapSlots->battleSheet;
  EFFECT_START_NARROW_CANVAS(BGCNT_WRAP | BGCNT_PRIORITY(1));
  Resource_LoadAndDecompress(RESOURCE_ICE_SHARD_SHEET, state->sheet, 1, 1);
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

      shard = &state->particles[i];
      x = (Random16() & 63) + (Random16() & 7) + 24;
      if (args->side == EFFECT_SIDE_PARTY)
        x = x + slant + 24;
      else
        x = x - slant + 80;
      shard->x = x << 3;
      shard->variant = -1; // still falling
      shard->y = y;
    }
  }
  for (i = 0; i != FALLING_SHARD_PUFF_COUNT; i++)
    state->particles[FALLING_SHARD_COUNT + i].variant = -1; // puff slot free

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

  if (args->side == EFFECT_SIDE_ENEMIES) {
    line = (u16 *)gMapGridCellBuffer;
    FALLING_BUILD_SLANTED_WINDOW(line, i, 103, FALLING_BAND(112 - i),
                                 FALLING_BAND(8));
  } else {
    line = (u16 *)gMapGridCellBuffer;
    FALLING_BUILD_SLANTED_WINDOW(line, i, 95, FALLING_BAND(i + 24),
                                 FALLING_BAND(120));
  }

  Task_Add(BattleEffect_ArmWin0HBlankDma, TASK_PASS_VBLANK);
  state->transferMode = CANVAS_COPY_AND_FADE;
  if (args->variant == EFFECT_VARIANT_STRONGER)
    state->transferValue = CANVAS_FADE_THREE_QUARTERS;
  else
    state->transferValue = CANVAS_FADE_HALF;
  Task_Add(BattleEffect_TransferCanvas, TASK_PASS_VBLANK);

  for (frame = 0; frame != ICE_FRAMES(args->variant); frame++) {
    if (frame == ICE_FRAMES(args->variant) - 16)
      EFFECT_SHOW_DAMAGE(SE_IMPACT);

    // Shards fall down the slant, 8 pixels down and 8 across a frame, and on
    // landing shatter, kicking up puffs.
    for (i = 0; i != ICE_SHARDS(args->variant); i++) {
      shard = &state->particles[i];
      if (shard->variant == -1) {
        s32 x = shard->x / 8;
        s32 y = shard->y / 8;

        ((DrawRectangle)
             draw[args->variant == EFFECT_VARIANT_STRONGEST ? 1 : 0])(
            canvas, state->sheet, x, y, ICE_SHARD_WIDTH, ICE_SHARD_HEIGHT);
        if (shard->y <= FALLING_LAND_Y) {
          if (args->side == EFFECT_SIDE_ENEMIES)
            shard->x -= 64;
          else
            shard->x += 64;
          shard->y += 64;
        } else {
          // Every target flashes blue as the shards land.
          if ((i & 3) == 0)
            Sound_PlayFar(SE_ICE);
          state->shakeFrames = 2;
          shard->variant = 0;
          EFFECT_FOR_EACH_TARGET(k, args)
            BattleEffect_TintBattler(args->targets[k], SPRITE_COLOR_BLUE_TINT,
                                     POSE_DAMAGE, k, 8);
        }
      }
      if (shard->variant != -1) {
        s32 x = shard->x / 8;
        s32 y = shard->y / 8;

        if (shard->variant >= 1 &&
            shard->variant <= FALLING_LANDED_LAST_FRAME) {
          ((DrawRectangle)
               draw[args->variant == EFFECT_VARIANT_STRONGEST ? 1 : 0])(
              canvas,
              state->sheet + PICTURE_BYTES(ICE_SHARD) +
                  shard->variant / ICE_SHATTER_PICTURE_FRAMES *
                      PICTURE_BYTES(ICE_SHARD),
              x, y, ICE_SHARD_WIDTH, ICE_SHARD_HEIGHT);
        }
        if (shard->variant >= ICE_PUFF_FIRST_FRAME &&
            shard->variant <= ICE_PUFF_LAST_FRAME) {
          for (k = 0; k != FALLING_SHARD_PUFF_COUNT; k++) {
            puff = &state->particles[FALLING_SHARD_COUNT + k];
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
      puff = &state->particles[FALLING_SHARD_COUNT + i];
      if (puff->variant != -1) {
        if (puff->variant < ICE_PUFF_FRAMES) {
          s32 picture = puff->variant / 2;
          s32 x = puff->x / 8 - (gBattleEffect_SparkleSizes[picture] >> 1);
          s32 y = puff->y / 8 - (gBattleEffect_SparkleSizes[picture] >> 1);

          ((DrawRectangle)
               draw[args->variant == EFFECT_VARIANT_STRONGEST ? 1 : 0])(
              canvas, sparkles + gBattleEffect_SparklePictures[picture], x, y,
              gBattleEffect_SparkleSizes[picture],
              gBattleEffect_SparkleSizes[picture]);
        }
        if (puff->variant > -1)
          puff->variant--;
      }
    }

    BattleEffect_CountDownBattlerTints();
    BattleCamera_Shake(4, 4);
    EFFECT_PRESENT_CANVAS(state);
  }

  Task_Remove(BattleEffect_TransferCanvas);
  Task_Remove(BattleEffect_ArmWin0HBlankDma);
  EFFECT_RELEASE_BLITTERS();
  BattleEffect_EndCanvasLayer();
  BattleEffect_ConfigureEffectDisplay();
}

/// Ram_MapCellBuffer as Douse uses it: window 0's table, then the drops'
/// droplets, each free while its variant is -1.
#define DOUSE_DROPLET_BUFFER ((struct DropletBuffer *)Ram_MapCellBuffer)

/// Each Douse variant: the drops that fall, of the 64 set out, the droplets
/// each throws up, the pixels between drops along the slant, and its frames;
/// the damage shows 64 frames before the end.
#define DOUSE_FALLING_DROPS(variant) gDouse_Counts[(variant) * 4]
#define DOUSE_DROPLETS_PER_DROP(variant) gDouse_Counts[(variant) * 4 + 1]
#define DOUSE_DROP_SPACING(variant) gDouse_Counts[(variant) * 4 + 2]
#define DOUSE_FRAMES(variant) gDouse_Counts[(variant) * 4 + 3]

/// BlueBeamSheet, after BlueGlowSheet's ten flare pictures: the falling
/// drop, a blue bubble; the small splash it lands in; then the large splash.
#define DOUSE_FALLING_PICTURE FLARE_SET_BYTES
#define DOUSE_SMALL_SPLASH_PICTURE                                             \
  (DOUSE_FALLING_PICTURE + PICTURE_BYTES(BLUE_BUBBLE))
#define DOUSE_LARGE_SPLASH_PICTURE                                             \
  (DOUSE_SMALL_SPLASH_PICTURE + PICTURE_BYTES(BLUE_BUBBLE))

/// Drops of water fall diagonally onto the targets' side of the field, 8 pixels
/// down and 4 across a frame, each bursting in a splash that throws up droplets
/// where it lands, while the targets flash; Deluge throws them higher.
///
/// Abilities: Douse, Drench, Deluge.
void BattleEffect_RunDouse(struct BattleEffectParams *args) {
  struct BattleEffectState *state;
  void *canvas;
  s32 frame;
  DrawRectangle draw[2];
  struct Particle *drop;
  struct Particle *droplet;
  u16 *line;
  s32 i;
  s32 k;

  state = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;

  EFFECT_START_NARROW_CANVAS(BGCNT_WRAP | BGCNT_PRIORITY(1));
  Resource_LoadAndDecompress(RESOURCE_BLUE_BEAM_SHEET,
                             state->sheet + DOUSE_FALLING_PICTURE, 1, 1);
  Resource_LoadAndDecompress(RESOURCE_BLUE_GLOW_SHEET, state->sheet, 0, 0);
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

  EFFECT_FREE_PARTICLES(i, DOUSE_DROPLET_BUFFER->droplets,
                        ARRAY_COUNT(DOUSE_DROPLET_BUFFER->droplets));

  // Line the drops up above the screen along the slant, each the variant's
  // spacing higher than the one before.
  for (i = 0; i != ARRAY_COUNT(state->particles); i++) {
    s32 x;
    s32 y;

    drop = &state->particles[i];
    x = Random16() & 63;
    y = -(i * DOUSE_DROP_SPACING(args->variant) + 16);
    if (args->side == EFFECT_SIDE_PARTY)
      x = x + y / 2 - 48;
    else
      x = x - y / 2 + 72;
    drop->x = x << 3;
    drop->y = y << 3;
    drop->variant = -1; // still falling
  }

  if (args->side == EFFECT_SIDE_ENEMIES) {
    line = (u16 *)gMapGridCellBuffer;
    FALLING_BUILD_SLANTED_WINDOW(line, i, 103, FALLING_BAND(52 - i / 2),
                                 FALLING_BAND(0));
  } else {
    line = (u16 *)gMapGridCellBuffer;
    FALLING_BUILD_SLANTED_WINDOW(line, i, 103, FALLING_BAND(i / 2 + 60),
                                 FALLING_BAND(112));
  }

  // Douse copies the canvas plainly; Drench and Deluge blend it in.
  Task_Add(BattleEffect_ArmWin0HBlankDma, TASK_PASS_VBLANK);
  if (args->variant == EFFECT_VARIANT_BASIC) {
    EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_CLEAR, 0);
  } else if (args->variant == EFFECT_VARIANT_STRONGER) {
    EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
  } else {
    EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_FADE,
                        CANVAS_FADE_THREE_QUARTERS);
  }
  Task_Add(BattleEffect_TransferCanvas, TASK_PASS_VBLANK);

  for (frame = 0; frame != DOUSE_FRAMES(args->variant); frame++) {
    if (frame == DOUSE_FRAMES(args->variant) - 64)
      EFFECT_SHOW_DAMAGE(SE_IMPACT);

    // Drops fall down the slant, then burst in a splash.
    for (i = 0; i != DOUSE_FALLING_DROPS(args->variant); i++) {
      s32 x;
      s32 y;

      drop = &state->particles[i];
      x = drop->x / 8;
      y = drop->y / 8;
      if (drop->variant == -1) {
        draw[0](canvas, state->sheet + DOUSE_FALLING_PICTURE, x, y,
                BLUE_BUBBLE_WIDTH, BLUE_BUBBLE_HEIGHT);
        if (drop->y <= FALLING_LAND_Y) {
          if (args->side == EFFECT_SIDE_ENEMIES)
            drop->x -= 32;
          else
            drop->x += 32;
          drop->y += 64;
        } else {
          // Droplets fly up from the middle of the landed drop.
          drop->variant = 0;
          for (k = 0; k != DOUSE_DROPLETS_PER_DROP(args->variant); k++) {
            droplet =
                &DOUSE_DROPLET_BUFFER
                     ->droplets[i * DOUSE_DROPLETS_PER_DROP(args->variant) + k];
            droplet->x = INTtoFIXED(x + BLUE_BUBBLE_WIDTH / 2);
            droplet->y = INTtoFIXED(y);
            droplet->velocityX = ((Random16() & 255) - 128) << 9;
            if (args->variant == EFFECT_VARIANT_STRONGEST)
              droplet->velocityY = ((Random16() & 0x1ff) - 384) << 10;
            else
              droplet->velocityY = ((Random16() & 255) - 255) << 10;
            droplet->variant = (Random16() & 15) + 16;
          }
          // Every target flashes white as the drops land.
          if ((i & 3) == 0)
            Sound_PlayFar(SE_WATER);
          EFFECT_FOR_EACH_TARGET(k, args)
            BattleEffect_TintBattler(args->targets[k], SPRITE_COLOR_WHITE,
                                     POSE_DAMAGE, k, 2);
        }
      } else {
        // Four frames of the small splash, four of the large one.
        if (drop->variant >= 0 && drop->variant <= 3)
          draw[0](canvas, state->sheet + DOUSE_SMALL_SPLASH_PICTURE, x, y,
                  BLUE_BUBBLE_WIDTH, BLUE_BUBBLE_HEIGHT);
        else if (drop->variant <= 7)
          draw[0](canvas, state->sheet + DOUSE_LARGE_SPLASH_PICTURE,
                  x - (BLUE_LARGE_SPLASH_WIDTH - BLUE_BUBBLE_WIDTH) / 2,
                  y - (BLUE_LARGE_SPLASH_HEIGHT - BLUE_BUBBLE_HEIGHT) / 2,
                  BLUE_LARGE_SPLASH_WIDTH, BLUE_LARGE_SPLASH_HEIGHT);
        if (drop->variant <= FALLING_LANDED_LAST_FRAME)
          drop->variant++;
      }
    }

    // Droplets fall and shrink as they die.
    for (i = 0; i != ARRAY_COUNT(DOUSE_DROPLET_BUFFER->droplets); i++) {
      droplet = &DOUSE_DROPLET_BUFFER->droplets[i];
      if (droplet->variant != -1) {
        s32 size = droplet->variant + 1;

        if (size > 6)
          size = 6;
        EFFECT_DRAW_FLARE(draw[1], canvas, state->sheet,
                          FIXED_INTEGER_PART(droplet->x),
                          FIXED_INTEGER_PART(droplet->y), size);
        Particle_AdvanceWithGravity2D(droplet, 60, toFIXED(1) / 8);
        droplet->variant--;
      }
    }

    EFFECT_END_FRAME(state);
  }

  Task_Remove(BattleEffect_ArmWin0HBlankDma);
  EFFECT_FINISH_CANVAS();
  BattleEffect_ConfigureEffectDisplay();
}

#undef FALLING_BUILD_SLANTED_WINDOW
#undef FALLING_BAND
#undef ICE_SHARDS
#undef ICE_FRAMES
#undef FALLING_LAND_Y
#undef ICE_SHARD_WIDTH
#undef ICE_SHARD_HEIGHT
#undef FALLING_LANDED_LAST_FRAME
#undef ICE_SHATTER_PICTURE_FRAMES
#undef ICE_PUFF_FIRST_FRAME
#undef ICE_PUFF_LAST_FRAME
#undef ICE_PUFF_FRAMES
#undef DOUSE_DROPLET_BUFFER
#undef DOUSE_FALLING_DROPS
#undef DOUSE_DROPLETS_PER_DROP
#undef DOUSE_DROP_SPACING
#undef DOUSE_FRAMES
#undef DOUSE_FALLING_PICTURE
#undef DOUSE_SMALL_SPLASH_PICTURE
#undef DOUSE_LARGE_SPLASH_PICTURE
