/// @file
/// Frost.
// Part of battle_effect.c.

#include "../battle_effect_internal.h"

/// One row of gFrost_Timings, for each variant.
enum {
  FROST_MOTES,  ///< how many motes spiral down
  FROST_MOUNDS, ///< how many ice mounds spring up
  FROST_FRAME   ///< the frame the motes burst
};

/// Motes of ice spiral down and in while mounds of ice spring up; at the burst
/// the motes spread and rise, chips fly up and sparkles rise.
///
/// Abilities: Frost, Tundra, Glacier.
void BattleEffect_RunFrost(struct BattleEffectParams *args) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  u8 *sparkleSheet;
  s32 xyz[3];
  struct ScreenPoint screen;
  s32 point[3];
  BlitterPair pair;
  struct Particle *mote;
  s32 i;

  work = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
  canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
  sparkleSheet = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_SHEET);
  work->args = args;
  EFFECT_START_NARROW_CANVAS(BGCNT_PRIORITY(1));
  Resource_LoadAndDecompress(RESOURCE_ICE_CHIP_SHEET, work->sheet, 1, 1);
  Resource_LoadAndDecompress(RESOURCE_SPARKLE_DOTS, sparkleSheet, 0, 0);
  BattleEffect_FetchRectangleBlitters(0, pair);

  for (i = 0; i != 64; i++) {
    struct Particle *seed = &work->particles[i];

    seed->x = Random16() & TRIG_ANGLE_MASK;
    seed->z = (Random16() & 63) + 56;
    seed->y = ((Random16() & 31) - 64) << 16;
  }

  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
  if (work->args->side == EFFECT_SIDE_PARTY)
    REG_BG2X = -(112 << 8);

  for (frame = 0;
       frame != gFrost_Timings[work->args->variant][FROST_FRAME] + 75;
       frame++) {
    s32 count;
    s32 lowest;
    s32 highest;

    lowest = px(120);
    highest = 0;
    if (frame == gFrost_Timings[work->args->variant][FROST_FRAME] + 11)
      EFFECT_SHOW_DAMAGE(SE_WATER);
    SET_VECTOR(point, 0, 0, px(512));
    Transform_Reset();
    Transform_ApplyPosition(point);
    if (frame >= 36 && frame < 64 && (frame & 3) == 0)
      Sound_PlayFar(SE_ICE);
    if (frame == 85)
      Sound_PlayFar(SE_WHOOSH);
    EFFECT_FOR_EACH_TARGET(i, work->args) {
      if (frame == i * 4 + 40)
        BattleEffect_TintBattler(work->args->targets[i], SPRITE_COLOR_BLUE_TINT,
                                 POSE_DAMAGE, TINT_KEEP, 0);
    }

    // Motes spiral in and sink.
    if (frame < gFrost_Timings[work->args->variant][FROST_FRAME])
      count = gFrost_Timings[work->args->variant][FROST_MOTES];
    else
      count = 16;
    if (frame < gFrost_Timings[work->args->variant][FROST_FRAME] + 35) {
      for (i = 0; i != count; i++) {
        if (frame > i) {
          s32 picture = i % 8;

          mote = &work->particles[i];
          if (mote->y < px(48 - i / 2) && mote->y > -px(48)) {
            u32 width;
            u32 height;

            SET_VECTOR(xyz, mote->z * Trig_Sin(mote->x), mote->y,
                       mote->z * Trig_Cos(mote->x));
            Projection_Point(xyz, &screen);
            screen.x = (screen.x >> 17) + 64;
            screen.y = Q_16_16_TO_INT(screen.y) + 60;
            // UB: width is assigned in one argument and read in another.
#ifdef UBFIX
            width = gFrost_MoteWidths[picture];
            height = gFrost_MoteHeights[picture];
            pair[1](canvas, work->sheet + gFrost_MotePictures[picture],
                    screen.x - (width >> 1), screen.y - (height >> 1), width,
                    height);
#else
            pair[1](canvas, work->sheet + gFrost_MotePictures[picture],
                    screen.x - ((width = gFrost_MoteWidths[picture]) >> 1),
                    screen.y - ((height = gFrost_MoteHeights[picture]) >> 1),
                    width, height);
#endif
          }
          if (frame < gFrost_Timings[work->args->variant][FROST_FRAME]) {
            if (frame > i + 16) {
              if (mote->z > 4)
                mote->z -= 2;
              if (mote->y < px(48))
                mote->y += px(5);
              mote->x += TRIG_TURN / 128;
            }
          } else {
            mote->z += 8;
            mote->y -= px(i % 5 + 2);
            if (lowest > mote->y)
              lowest = mote->y;
            if (highest < mote->y)
              highest = mote->y;
          }
        }
      }
    }
    lowest += px(64);
    highest += px(64);

    // Ice mounds spring up.
    if (frame < gFrost_Timings[work->args->variant][FROST_FRAME]) {
      for (i = 0; i != gFrost_Timings[work->args->variant][FROST_MOUNDS]; i++) {
        if (i < (frame - 36) / 3) {
          s32 picture = i % 3;
          u32 height;

          if (frame >= gFrost_Timings[work->args->variant][FROST_FRAME] - 7) {
#ifdef UBFIX
            height = gFrost_MoundHeights[picture];
            pair[1](canvas, work->sheet + gFrost_MoundPictures[picture],
                    gFrost_MoundSpots[i][FROST_MOUND_X],
                    gFrost_MoundSpots[i][FROST_MOUND_FOOT] - height, 32,
                    height);
#else
            pair[1](canvas, work->sheet + gFrost_MoundPictures[picture],
                    gFrost_MoundSpots[i][FROST_MOUND_X],
                    gFrost_MoundSpots[i][FROST_MOUND_FOOT] -
                        (height = gFrost_MoundHeights[picture]),
                    32, height);
#endif
          } else {
#ifdef UBFIX
            height = gFrost_MoundHeights[picture];
            pair[0](canvas, work->sheet + gFrost_MoundPictures[picture],
                    gFrost_MoundSpots[i][FROST_MOUND_X],
                    gFrost_MoundSpots[i][FROST_MOUND_FOOT] - height, 32,
                    height);
#else
            pair[0](canvas, work->sheet + gFrost_MoundPictures[picture],
                    gFrost_MoundSpots[i][FROST_MOUND_X],
                    gFrost_MoundSpots[i][FROST_MOUND_FOOT] -
                        (height = gFrost_MoundHeights[picture]),
                    32, height);
#endif
          }
        }
      }
    }

    // Chips fly up at the burst.
    // BUG: velocityX is never set, so a chip drifts sideways by whatever word
    // its PARTICLES slot last held. Nothing in a battle clears that buffer,
    // the map grid's cells, so the word is the field's cells or an earlier
    // effect's particles or pictures.
    if (frame == gFrost_Timings[work->args->variant][FROST_FRAME]) {
      for (i = 0; i != 32; i++) {
        struct Particle *chip = &PARTICLES[i];

        SET_POSITION(chip, (Random16() & 127) << 16,
                     ((Random16() & 15) + 80) << 16,
                     ((Random16() & 63) - 32) << 12);
#ifdef BUGFIX
        chip->velocityX = 0;
#endif
        chip->velocityY = ((-Random16() & 15) - 16) << 13;
        chip->variant = (Random16() & 15) + 16;
      }
    }
    if (frame >= gFrost_Timings[work->args->variant][FROST_FRAME]) {
      for (i = 0; i != 24; i++) {
        struct Particle *chip = &PARTICLES[i];

        if (chip->variant >= 0) {
          s32 picture = i % 8;

          pair[1](canvas, work->sheet + gFrost_ChipPictures[picture],
                  Q_16_16_TO_INT(chip->x), Q_16_16_TO_INT(chip->y),
                  gFrost_ChipWidths[picture], gFrost_ChipHeights[picture]);
          chip->x += chip->velocityX;
          chip->y += chip->velocityY;
          chip->variant--;
        }
        if (lowest > chip->y)
          lowest = chip->y;
        if (highest < chip->y)
          highest = chip->y;
      }
    }
    lowest >>= 16;
    highest >>= 16;
    if (highest <= lowest)
      highest = lowest + 1;

    // Sparkles rise over the motes and chips.
    if (frame == gFrost_Timings[work->args->variant][FROST_FRAME]) {
      for (i = 0; i != 32; i++) {
        struct Particle *sparkle = &work->particles[i];

        sparkle->velocityX = (Random16() & 127) << 16;
        if (highest == lowest)
          sparkle->velocityY = px(lowest);
        else
          sparkle->velocityY = (Random16() % (highest - lowest) + lowest) << 16;
        sparkle->variant = (Random16() & 15) + 20;
      }
    }
    if (frame >= gFrost_Timings[work->args->variant][FROST_FRAME]) {
      s32 rise = (frame - gFrost_Timings[work->args->variant][FROST_FRAME]) / 2;

      for (i = 0; i != 32; i++) {
        struct Particle *sparkle = &work->particles[i];

        if (sparkle->variant >= 0 && sparkle->variant <= 17) {
          s32 picture = (17 - sparkle->variant) / 2;
          u32 size;

#ifdef UBFIX
          size = gBattleEffect_SparkleSizes[picture];
          pair[1](canvas, sparkleSheet + gBattleEffect_SparklePictures[picture],
                  Q_16_16_TO_INT(sparkle->velocityX) - (size >> 1),
                  Q_16_16_TO_INT(sparkle->velocityY) - (size >> 1) - rise, size,
                  size);
#else
          pair[1](canvas, sparkleSheet + gBattleEffect_SparklePictures[picture],
                  Q_16_16_TO_INT(sparkle->velocityX) -
                      ((size = gBattleEffect_SparkleSizes[picture]) >> 1),
                  Q_16_16_TO_INT(sparkle->velocityY) - (size >> 1) - rise, size,
                  size);
#endif
        }
        sparkle->variant--;
        if ((sparkle->variant == -1 || sparkle->variant == 17) &&
            frame < gFrost_Timings[work->args->variant][FROST_FRAME] + 35) {
          sparkle->variant = 17;
          sparkle->velocityX = (Random16() & 127) << 16;
          sparkle->velocityY = (Random16() % (highest - lowest) + lowest) << 16;
        }
      }
    }
    EFFECT_PRESENT_CANVAS(work);
  }

  EFFECT_RELEASE_BLITTERS();
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}
