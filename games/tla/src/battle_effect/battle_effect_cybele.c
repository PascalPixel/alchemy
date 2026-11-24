/// @file
/// The Cybele summon.
// Part of battle_effect.c.

#include "battle_effect_internal.h"
#include "sounds.h"

#include "gamepak/far_call/battle.h"
#include "field/object.h"
#include "system/system.h"
#include "gamepak/far_call/field.h"
#include "window/screens.h"
#include "resource_ids.h"

/// Writes the canvas's tile map for BG2, then closes the canvas layer.
UNUSED void BattleEffect_InitializeTilemapAndFlushQueue(void) {
  BattleEffect_StartTiledCanvas(BGCNT_PRIORITY(1));
  BattleEffect_EndCanvasLayer();
}

#define CYBELE_FRAMES 366
/// The first target lands on its spreading ring, sixteen frames after it is
/// dropped; the others follow.
#define CYBELE_LANDING_FRAME 302

/// Where the blast pictures start in state->sheet, after the yellow ring's.
#define CYBELE_BLAST_PICTURES 5760
#define CYBELE_VINES 16 ///< vines, from state->particles[0]
#define CYBELE_SPARKS 16
#define CYBELE_THORNS 16
#define CYBELE_CHIPS 48
#define CYBELE_FIRST_SPARK 16 ///< the first spark in state->particles
#define CYBELE_FIRST_THORN 32 ///< the first thorn in state->particles
#define CYBELE_FIRST_CHIP 16  ///< the first chip in state->particles
#define CYBELE_DUST_PICTURE 6 ///< the dust cloud in gCybele_BlastPictures
#define CYBELE_SPRITES 8
/// How far right of the screen's left edge the canvas shows until the chips
/// fly, in pixels.
#define CYBELE_CANVAS_X 32
/// The order the canvas dissolves in: shuffled runs of the canvas's columns,
/// row y taking run y % CYBELE_DISSOLVE_RUNS.
#define CYBELE_DISSOLVE_RUNS 8
/// The dissolve clears CYBELE_DISSOLVE_PIXELS pixels a frame from each of the
/// CYBELE_DISSOLVE_BAND rows it moves down the canvas.
#define CYBELE_DISSOLVE_BAND 32
#define CYBELE_DISSOLVE_PIXELS 4
/// The vines are drawn for CYBELE_VINE_FRAMES frames from the chips' first,
/// growing for the first CYBELE_VINE_GROW_FRAMES.
#define CYBELE_VINE_FRAMES 96
#define CYBELE_VINE_GROW_FRAMES 80
/// The vines drawn, of the CYBELE_VINES set out.
#define CYBELE_VINES_DRAWN 10
/// The chips that fly, from the first after the sparks.
#define CYBELE_FLYING_CHIPS 32
#define CYBELE_FIRST_FLYING_CHIP 32
/// The sparks round each target, from CYBELE_FIRST_SPARK, flashing from the
/// vines' first frame to CYBELE_SPARKS_END frames after it.
#define CYBELE_TARGET_SPARKS 6
#define CYBELE_SPARKS_END 172
/// Each target's dust puffs, from state->particles[target *
/// CYBELE_TARGET_DUST]: set out a fifth of a turn apart (rounded up), the first
/// CYBELE_TARGET_DUST_DRAWN drawn until their CYBELE_DUST_LAST_FRAME.
#define CYBELE_TARGET_DUST 10
#define CYBELE_TARGET_DUST_DRAWN 5
#define CYBELE_DUST_SPREAD ((TRIG_TURN + 4) / 5)
#define CYBELE_DUST_LAST_FRAME 26

/// The vine pictures in the thorn vine sprite sheet, below a tip at 0; the
/// flowers, thorns and sparks follow.
#define CYBELE_VINE_WIDTH 24
#define CYBELE_VINE_TIP_HEIGHT 8
#define CYBELE_VINE_STEM_HEIGHT 64
/// YellowRingSheet's ring, drawn as two mirrored halves.
#define CYBELE_RING_HALF_WIDTH 20
#define CYBELE_RING_HALF_HEIGHT 24
/// Where each picture starts in ThornVineSheet, in bytes: a vine's tip, then
/// its stem.
enum ThornVineSheetPicture {
  CYBELE_VINE_STEM = CYBELE_VINE_WIDTH * CYBELE_VINE_TIP_HEIGHT,
  /// a thinner vine: its tip, then its stem
  CYBELE_THIN_VINE = CYBELE_VINE_STEM +
      CYBELE_VINE_WIDTH * CYBELE_VINE_STEM_HEIGHT
};

/// Cybele drops in at the right and thorns fly from her mouth; flowering
/// vines grow out of the ground, lifting the targets while sparks flash around
/// them; then the canvas greys and dissolves pixel by pixel, the targets
/// drop, each landing on a spreading ring in a cloud of dust, and Cybele bursts
/// away in a flash. A or B skips to the drop.
///
/// Abilities: Cybele.
void BattleEffect_RunCybele(struct BattleEffectParams *args) {
  struct BattleEffectState *state;
  u8 *canvas;
  s32 frame;
  FIXED cybeleX = toFIXED(128);
  FIXED height;
  FIXED speed;
  s32 phase;
  s32 rise;
  s32 i;
  s32 j;
  u8 *order;
  s32 point[3];
  struct ScreenPoint screen;
  DrawRectangle draw[2];

  state = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  BattleEffect_SpawnObjects(CYBELE_SPRITES, SPRITE_CYBELE, 1);

  order = Ram_MapCellBuffer;
  for (i = 0; i != CYBELE_DISSOLVE_RUNS * CANVAS_SIZE; i++)
    order[i] = i & (CANVAS_SIZE - 1);
  for (i = 0; i != CYBELE_DISSOLVE_RUNS; i++) {
    for (j = 0; j != CANVAS_SIZE; j++) {
      s32 a = Random16() & (CANVAS_SIZE - 1);
      s32 b = Random16() & (CANVAS_SIZE - 1);
      u8 *to = &order[i * CANVAS_SIZE + b];
      u8 *from = &order[i * CANVAS_SIZE + a];
      u8 t = *to;

      *to = *from;
      *from = t;
    }
  }

  EFFECT_START_NARROW_CANVAS(0);
  REG_BLDCNT = 0;
  Resource_LoadAndDecompress(RESOURCE_THORN_VINE_SHEET, state->sheet, 1, 1);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                      BLITTER_TRANSPARENT);
  draw[0] = (DrawRectangle)Ram_HeapSlots->ramCode;
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
                      EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                      BLITTER_TRANSPARENT);
  draw[1] = (DrawRectangle)Ram_HeapSlots->ramCodePaired;
  EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_CLEAR, 0);
  REG_BG2X = -(CYBELE_CANVAS_X << 8);

  for (i = 0; i != CYBELE_VINES; i++) {
    struct Particle *vine = &state->particles[i];

    if (i <= 4) {
      vine->x = i * 20;
      vine->y = (Random16() & 7) + 104;
    } else {
      vine->x = i * 20 - 90;
      vine->y = (Random16() & 7) + 108;
    }
    vine->velocityY = (Random16() & 7) + 4;
    vine->variant = (Random16() & 15) + 16;
  }
  for (i = 0; i != CYBELE_SPARKS; i++) {
    struct Particle *spark = &state->particles[CYBELE_FIRST_SPARK + i];

    spark->x = (Random16() & 63) + 32;
    spark->y = (Random16() & 31) + 64;
    spark->variant = -(Random16() & 7) - 8;
  }
  for (i = 0; i != CYBELE_THORNS; i++) {
    struct Particle *thorn = &state->particles[CYBELE_FIRST_THORN + i];

    thorn->x = toFIXED(128);
    thorn->y = toFIXED(64);
    thorn->velocityX = -((Random16() & 255) + 200) << 9;
    thorn->velocityY = 0;
    thorn->variant = 0;
  }
  BattleEffect_SelectLivingTargets(state->args);

  height = -toFIXED(64);
  speed = 0;
  for (frame = 0; frame != CYBELE_FRAMES; frame++) {
    struct BattleCamera *camera = Ram_HeapSlots->cameraState;

    // A or B skips the vines straight to the drop.
    if ((gInput.repeat & (KEY_A | KEY_B)) != 0 && frame > 190 && frame <= 285) {
      Iwram_ClearWordsEntry(canvas, CANVAS_BYTES);
      frame = 286;
    }
    if (frame == 224)
      state->transferMode = CANVAS_COPY;
    EFFECT_VIEW_FROM_CAMERA(camera);
    // Cybele lands; the screen shakes and the targets flinch.
    if (frame == 31) {
      state->shakeFrames = 8;
      Sound_PlayFar(SE_GATE_MOVE);
      EFFECT_FOR_EACH_TARGET(i, args)
        BattleMotion_ApplyKnockbackFar(args->targets[i], KNOCKBACK_BOUNCE);
    }
    if (frame == 72)
      Sound_PlayFar(SE_WHOOSH);
    if (frame == 140)
      Sound_PlayFar(SE_SWITCH_MECHANISM);

    // Cybele settles, and her mouth opens as the thorns fly and shuts again.
    speed += toFIXED(1) / 4;
    height += speed;
    if (height > toFIXED(64))
      height = toFIXED(64);
    BattleEffect_PlaceFormationObjects(SUMMON_FORMATION_CYBELE, cybeleX,
                                       height);
    if (frame >= 48 && frame <= 96) {
      s32 pose = (frame - 48) / 24 % 3;

      Sprite_StartAnimationFar(state->sprites[3],
                               gCybele_ObjectPoses[pose * 2]);
      Sprite_StartAnimationFar(state->sprites[4],
                               gCybele_ObjectPoses[pose * 2 + 1]);
    }

    // Thorns fly from her mouth to the ground.
    if (frame >= 72 && frame <= 127) {
      for (i = 0; i != CYBELE_THORNS; i++) {
        struct Particle *thorn = &state->particles[CYBELE_FIRST_THORN + i];

        if (frame >= i + 72 && thorn->y < toFIXED(104)) {
          s32 pictureIndex = (frame + i) / 4 % 5;
          s32 x = FIXED_INTEGER_PART(thorn->x);
          s32 y = FIXEDtoINT(thorn->y);
          u8 *picture = state->sheet + gCybele_ThornPictures[pictureIndex];
          u32 width = gCybele_ThornWidths[pictureIndex];
          u32 tall;

          x -= width >> 1;
          tall = gCybele_ThornHeights[pictureIndex];
          y -= tall >> 1;
          draw[0](canvas, picture, x, y, width, tall);
          Particle_AdvanceWithGravity2D(thorn, PARTICLE_DAMPING_NONE,
                                        toFIXED(1) / 16);
        }
      }
    }
    if (frame == 128) {
      for (i = 0; i != CYBELE_CHIPS; i++) {
        struct Particle *chip = &state->particles[CYBELE_FIRST_CHIP + i];

        chip->x = INTtoFIXED(Random16() % 96);
        chip->y = INTtoFIXED((Random16() & 7) + 88);
        chip->velocityX = ((Random16() & 255) - 128) << 11;
        chip->velocityY = -(Random16() & 255) << 11;
        chip->variant = -(Random16() & 15) - 16;
      }
      REG_BG2X = 0;
    }

    // Grow the vines, shaking the screen while they grow.
    phase = frame - 128;
    if (phase >= 0 && phase <= CYBELE_VINE_FRAMES) {
      rise = phase;
      if (rise > CYBELE_VINE_GROW_FRAMES)
        rise = CYBELE_VINE_GROW_FRAMES;
      else
        state->shakeFrames = 2;
      for (i = 0; i != CYBELE_VINES_DRAWN; i++) {
        struct Particle *vine = &state->particles[i];

        if (rise > vine->variant) {
          u8 *sheet = state->sheet;
          s32 top;
          s32 wrap;
          s32 k;

          if (i > 5)
            sheet = state->sheet + CYBELE_THIN_VINE;
          top = (rise - vine->variant) * vine->velocityY;
          // A vine taller than the battle view has lost its tip off the top;
          // past a stem more, it is wound back a stem, so its stems keep
          // rising.
          for (wrap = top; wrap > BATTLE_VIEW_HEIGHT + CYBELE_VINE_STEM_HEIGHT;
               wrap -= CYBELE_VINE_STEM_HEIGHT)
            ;
          if (wrap < BATTLE_VIEW_HEIGHT)
            draw[i & 1](canvas, sheet, vine->x,
                        vine->y - wrap - CYBELE_VINE_TIP_HEIGHT,
                        CYBELE_VINE_WIDTH, CYBELE_VINE_TIP_HEIGHT);
          for (k = 0; k != 3; k++) {
            s32 y = vine->y - wrap + k * CYBELE_VINE_STEM_HEIGHT;
            s32 skip = 0;
            s32 rows = CYBELE_VINE_STEM_HEIGHT;

            if (y >= -CYBELE_VINE_STEM_HEIGHT) {
              if (y < 0) {
                skip = -y * CYBELE_VINE_WIDTH;
                rows = y + CYBELE_VINE_STEM_HEIGHT;
                y = 0;
              }
              if (y + rows > vine->y)
                rows -= y + rows - vine->y;
              draw[i & 1](canvas, sheet + skip + CYBELE_VINE_STEM, vine->x, y,
                          CYBELE_VINE_WIDTH, rows);
            }
          }
          if ((i & 1) != 0) {
            s32 y = ((vine->y - top) & 127) - 16;
            s32 kind = i % 3;
            s32 full = gCybele_FlowerSizes[kind];
            s32 rows = full;

            if (y + rows > vine->y)
              rows -= y + rows - vine->y;
            if (rows > 0)
              draw[i & 1](canvas, state->sheet + gCybele_FlowerPictures[kind],
                          vine->x + 8, y, full, rows);
          }
        }
      }
    }
    // Chips fly up.
    if (phase >= 0 && phase < CYBELE_VINE_FRAMES) {
      for (i = 0; i != CYBELE_FLYING_CHIPS; i++) {
        struct Particle *chip = &state->particles[CYBELE_FIRST_FLYING_CHIP + i];

        if (chip->variant >= 0) {
          s32 pictureIndex = i % 5;

          draw[0](canvas, state->sheet + gCybele_ChipPictures[pictureIndex],
                  FIXED_INTEGER_PART(chip->x), FIXED_INTEGER_PART(chip->y),
                  gCybele_ChipWidths[pictureIndex],
                  gCybele_ChipHeights[pictureIndex]);
          chip->x += chip->velocityX;
          chip->y += chip->velocityY;
          chip->velocityY += toFIXED(1) / 4;
          if ((chip->y < 0 || chip->y > toFIXED(120)) && frame <= 159) {
            chip->x = INTtoFIXED(Random16() % 96);
            chip->y = INTtoFIXED((Random16() & 7) + 88);
            chip->velocityX = ((Random16() & 255) - 128) << 11;
            chip->velocityY = -(Random16() & 255) << 11;
          }
        }
        chip->variant++;
      }
    }

    // The colours fade to grey.
    if (frame >= 224 && frame <= 247 && (frame & 3) == 0) {
      u16 *color = (u16 *)BG_PLTT;

      for (i = 0; i != SHEET_PALETTE_COLORS; i++) {
        s32 red = RGB_RED(*color);
        s32 green = RGB_GREEN(*color);
        s32 blue = RGB_BLUE(*color);
        s32 grey = (red + green + blue) / 3;

        if (red > grey)
          red--;
        if (red < grey)
          red++;
        if (green > grey)
          green--;
        if (green < grey)
          green++;
        if (blue > grey)
          blue--;
        if (blue < grey)
          blue++;
        *color++ = RGB(red, green, blue);
      }
    }

    // Six sparks keep flashing around each target's body.
    if (phase >= 0 && phase <= CYBELE_SPARKS_END) {
      EFFECT_FOR_EACH_TARGET(i, args) {
        struct MotionObject *target =
            BattleObject_GetFar(args->targets[i])->object;

        point[0] = target->x;
        point[1] = target->y;
        point[2] = target->z;
        EffectPosition_ProjectPoint(point, &screen);
        for (j = 0; j != CYBELE_TARGET_SPARKS; j++) {
          struct Particle *spark =
              &state->particles[CYBELE_FIRST_SPARK + i * CYBELE_TARGET_SPARKS +
                                j];

          if (spark->variant == 0) {
            spark->x = screen.x + (Random16() & 15) - 8;
            spark->y = screen.y + (Random16() & 15) - 40;
          }
          if (spark->variant >= 0 && spark->variant <= 4) {
            u8 *picture = state->sheet + gCybele_SparkPictures[spark->variant];
            u32 size = gCybele_SparkSizes[spark->variant];
            u32 half = size >> 1;

            draw[0](canvas, picture, spark->x - half, spark->y - half, size,
                    size);
          }
          spark->variant++;
          if (frame <= 199 && spark->variant == 5)
            spark->variant = -(Random16() & 7);
        }
      }
    }

    // The canvas dissolves pixel by pixel from the top.
    if (frame > 232) {
      s32 row = frame * 2 - 496;

      for (i = 0; i != CYBELE_DISSOLVE_BAND; i++) {
        for (j = 0; j != CYBELE_DISSOLVE_PIXELS; j++) {
          s32 y = row + i;

          if (y >= 0 && y < CANVAS_SIZE) {
            s32 x = order[(y & (CYBELE_DISSOLVE_RUNS - 1)) * CANVAS_SIZE +
                          i * CYBELE_DISSOLVE_PIXELS + j];

            canvas[CANVAS_PIXEL_OFFSET(x, y)] = 0;
          }
        }
      }
      row++;
      for (i = 0; i != CYBELE_DISSOLVE_BAND; i++) {
        for (j = 0; j != CYBELE_DISSOLVE_PIXELS; j++) {
          s32 y = row + i;

          if (y >= 0 && y < CANVAS_SIZE) {
            s32 x = order[(y & (CYBELE_DISSOLVE_RUNS - 1)) * CANVAS_SIZE +
                          i * CYBELE_DISSOLVE_PIXELS + j];

            canvas[CANVAS_PIXEL_OFFSET(x, y)] = 0;
          }
        }
      }
    }

    // The vines lift the targets.
    if (frame >= 161 && frame <= 223) {
      EFFECT_FOR_EACH_TARGET(i, args) {
        if (frame > i * 8 + 160) {
          struct BattleObject *slot = BattleObject_GetFar(args->targets[i]);
          struct MotionObject *target = slot->object;
          void *record;
          s32 n;

          target->y += toFIXED(8);
          if (target->y > toFIXED(128))
            target->y = toFIXED(128);
          target->verticalMotionStrength = 0;
          for (n = 0; (record = GetMotionRecordFar(slot->object, n)) != 0; n++)
            Sprite_StartAnimationFar(record, POSE_DAMAGE);
        }
      }
    }
    // The targets drop.
    EFFECT_FOR_EACH_TARGET(i, args) {
      if (frame == i * 5 + CYBELE_LANDING_FRAME - 16) {
        struct MotionObject *target =
            BattleObject_GetFar(args->targets[i])->object;

        target->y = toFIXED(96);
        target->verticalMotionStrength = BATTLE_UNIT_GRAVITY;
      }
      if (frame == i * 5 + CYBELE_LANDING_FRAME) {
        BattleEffect_TintBattler(args->targets[i], SPRITE_COLOR_WHITE,
                                 TINT_KEEP, i, 8);
        Sound_PlayFar(SE_HEAVY_IMPACT);
        state->shakeFrames = 8;
      }
    }

    if (frame == CYBELE_LANDING_FRAME) {
      EFFECT_RELEASE_BLITTERS();
      Resource_LoadAndDecompress(RESOURCE_YELLOW_RING_SHEET, state->sheet, 1,
                                 0);
      Resource_LoadAndDecompress(RESOURCE_BLAST_SHEET,
                                 state->sheet + CYBELE_BLAST_PICTURES, 1, 1);
      EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
      draw[0] = (DrawRectangle)Ram_HeapSlots->ramCode;
      EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
                          EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, BLITTER_MAX);
      // ramCodePaired, reached from the slot before it.
      draw[1] = (DrawRectangle) * (&Ram_HeapSlots->unusedAfterCanvasMap + 1);
      REG_BLDCNT = BLDCNT_TGT1_BG1 | BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND |
                   BLDCNT_TGT2_ALL;
      REG_BG2PA = Q_8_8(0.5);
      REG_BG2X = 0;
      EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_FADE,
                            CANVAS_FADE_THREE_QUARTERS);
      EFFECT_FOR_EACH_TARGET(i, args) {
        struct MotionObject *target =
            BattleObject_GetFar(args->targets[i])->object;

        for (j = 0; j != CYBELE_TARGET_DUST; j++) {
          struct Particle *dust = &state->particles[i * CYBELE_TARGET_DUST + j];

          SET_POSITION(dust, target->x, toFIXED(20), target->z);
          dust->velocityX = Trig_Sin(j * CYBELE_DUST_SPREAD) * 4;
          dust->velocityY = (Random16() & 0x7fff) + toFIXED(1);
          dust->velocityZ = Trig_Cos(j * CYBELE_DUST_SPREAD) * 4;
          dust->variant = 0;
        }
      }
    }
    // Each target lands on a spreading ring.
    if (frame >= CYBELE_LANDING_FRAME) {
      EFFECT_FOR_EACH_TARGET(i, args) {
        s32 start = i * 4 + CYBELE_LANDING_FRAME;

        if (frame >= start && frame < i * 4 + CYBELE_LANDING_FRAME + 12) {
          s32 pictureIndex = (frame - start) / 2;
          struct MotionObject *target =
              BattleObject_GetFar(args->targets[i])->object;

          point[0] = target->x;
          point[1] = 0;
          point[2] = target->z;
          EffectPosition_ProjectPoint(point, &screen);
          screen.x >>= 1;
          draw[0](canvas,
                  state->sheet + pictureIndex * PICTURE_BYTES(CYBELE_RING_HALF),
                  screen.x - CYBELE_RING_HALF_WIDTH,
                  screen.y - CYBELE_RING_HALF_HEIGHT, CYBELE_RING_HALF_WIDTH,
                  CYBELE_RING_HALF_HEIGHT);
          draw[1](canvas,
                  state->sheet + pictureIndex * PICTURE_BYTES(CYBELE_RING_HALF),
                  screen.x, screen.y - CYBELE_RING_HALF_HEIGHT,
                  CYBELE_RING_HALF_WIDTH, CYBELE_RING_HALF_HEIGHT);
        }
        if (frame >= start + 6) {
          for (j = 0; j != CYBELE_TARGET_DUST_DRAWN; j++) {
            struct Particle *dust =
                &state->particles[i * CYBELE_TARGET_DUST + j];

            EffectPosition_ProjectPoint(&dust->x, &screen);
            screen.x >>= 1;
            if (dust->variant >= 0 && dust->variant <= CYBELE_DUST_LAST_FRAME) {
              s32 pictureIndex = CYBELE_DUST_PICTURE;
              u8 *source = state->sheet + gCybele_BlastPictures[pictureIndex];
              u32 size = gCybele_BlastSizes[pictureIndex];

              draw[0](canvas, source, screen.x - (size >> 1),
                      screen.y - (size >> 1), size, size);
            }
            Particle_AdvanceWithGravity2D(dust, 60, toFIXED(1) / 16);
            dust->variant++;
          }
        }
      }
    }

    if (frame <= 127)
      BattleCamera_Shake(4, 16);
    else if (frame < CYBELE_LANDING_FRAME)
      BattleCamera_Shake(4, 4);
    else
      BattleCamera_Shake(4, 8);
    EFFECT_END_FRAME(state);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  EFFECT_RELEASE_BLITTERS();
  EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
  BattleEffect_RunImpactBurst(SUMMON_FORMATION_CYBELE, cybeleX, height);
  for (i = 0; i != CYBELE_SPRITES; i++)
    Sprite_DestroyFar(state->sprites[i]);
  BattleEffect_EndCanvasLayer();
}

#undef CYBELE_FRAMES
#undef CYBELE_LANDING_FRAME
#undef CYBELE_BLAST_PICTURES
#undef CYBELE_VINES
#undef CYBELE_SPARKS
#undef CYBELE_THORNS
#undef CYBELE_CHIPS
#undef CYBELE_FIRST_SPARK
#undef CYBELE_FIRST_THORN
#undef CYBELE_FIRST_CHIP
#undef CYBELE_DUST_PICTURE
#undef CYBELE_SPRITES
#undef CYBELE_CANVAS_X
#undef CYBELE_DISSOLVE_RUNS
#undef CYBELE_DISSOLVE_BAND
#undef CYBELE_DISSOLVE_PIXELS
#undef CYBELE_VINE_FRAMES
#undef CYBELE_VINE_GROW_FRAMES
#undef CYBELE_VINES_DRAWN
#undef CYBELE_FLYING_CHIPS
#undef CYBELE_FIRST_FLYING_CHIP
#undef CYBELE_TARGET_SPARKS
#undef CYBELE_SPARKS_END
#undef CYBELE_TARGET_DUST
#undef CYBELE_TARGET_DUST_DRAWN
#undef CYBELE_DUST_SPREAD
#undef CYBELE_DUST_LAST_FRAME
#undef CYBELE_VINE_WIDTH
#undef CYBELE_VINE_TIP_HEIGHT
#undef CYBELE_VINE_STEM_HEIGHT
#undef CYBELE_RING_HALF_WIDTH
#undef CYBELE_RING_HALF_HEIGHT
