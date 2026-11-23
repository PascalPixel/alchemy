/// @file
/// Frost.
// Part of battle_effect.c.

#include "battle_effect_internal.h"

/// One row of gFrost_Timings, for each variant.
enum {
  FROST_MOTES,  ///< how many motes spiral down
  FROST_MOUNDS, ///< how many ice mounds spring up
  FROST_FRAME   ///< the frame the motes burst
};

/// The ice mounds' pictures in IceChipSheet are all as wide; their heights are
/// gFrost_MoundHeights.
#define FROST_MOUND_WIDTH 32

/// The motes still drawn once they burst, of the variant's FROST_MOTES.
#define FROST_BURST_MOTES 16
/// How far below the centre the motes sink before the burst, in pixels; they
/// are drawn only between this depth and as high above.
#define FROST_MOTE_DEPTH 48
/// The ice chips thrown up at the burst; only the first FROST_CHIPS_DRAWN are
/// drawn.
#define FROST_CHIPS 32
#define FROST_CHIPS_DRAWN 24
/// The sparkles that rise over the burst.
#define FROST_SPARKLES 32
/// A sparkle's frames: two for each of gBattleEffect_SparklePictures' nine,
/// counted down.
#define FROST_SPARKLE_LIFE 18

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
  BlitterPair draw;
  struct Particle *mote;
  s32 i;

  work = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
  canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
  sparkleSheet = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_SHEET);
  work->args = args;
  EFFECT_START_NARROW_CANVAS(BGCNT_PRIORITY(1));
  Resource_LoadAndDecompress(RESOURCE_ICE_CHIP_SHEET, work->sheet, 1, 1);
  Resource_LoadAndDecompress(RESOURCE_SPARKLE_DOTS, sparkleSheet, 0, 0);
  BattleEffect_FetchRectangleBlitters(0, draw);

  for (i = 0; i != ARRAY_COUNT(work->particles); i++) {
    struct Particle *seed = &work->particles[i];

    seed->x = Random16() & TRIG_ANGLE_MASK;
    seed->z = (Random16() & 63) + 56;
    seed->y = INTtoFIXED((Random16() & 31) - 64);
  }

  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
  if (work->args->side == EFFECT_SIDE_PARTY)
    REG_BG2X = -(NARROW_CANVAS_PARTY_X << 8);

  for (frame = 0;
       frame != gFrost_Timings[work->args->variant][FROST_FRAME] + 75;
       frame++) {
    s32 count;
    s32 lowest;
    s32 highest;

    lowest = toFIXED(120);
    highest = 0;
    if (frame == gFrost_Timings[work->args->variant][FROST_FRAME] + 11)
      EFFECT_SHOW_DAMAGE(SE_WATER);
    SET_VECTOR(point, 0, 0, toFIXED(512));
    Matrix_Initialize();
    Matrix_TranslateVector(point);
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
      count = FROST_BURST_MOTES;
    if (frame < gFrost_Timings[work->args->variant][FROST_FRAME] + 35) {
      for (i = 0; i != count; i++) {
        if (frame > i) {
          s32 picture = i % 8;

          mote = &work->particles[i];
          if (mote->y < INTtoFIXED(FROST_MOTE_DEPTH - i / 2) &&
              mote->y > -toFIXED(FROST_MOTE_DEPTH)) {
            u32 width;
            u32 height;

            SET_VECTOR(xyz, mote->z * Trig_Sin(mote->x), mote->y,
                       mote->z * Trig_Cos(mote->x));
            Projection_Point(xyz, &screen);
            screen.x = (FIXEDtoINT(screen.x) >> 1) + 64;
            screen.y = FIXEDtoINT(screen.y) + 60;
            // UB: width is assigned in one argument and read in another.
#ifdef UBFIX
            width = gFrost_MoteWidths[picture];
            height = gFrost_MoteHeights[picture];
            draw[1](canvas, work->sheet + gFrost_MotePictures[picture],
                    screen.x - (width >> 1), screen.y - (height >> 1), width,
                    height);
#else
            draw[1](canvas, work->sheet + gFrost_MotePictures[picture],
                    screen.x - ((width = gFrost_MoteWidths[picture]) >> 1),
                    screen.y - ((height = gFrost_MoteHeights[picture]) >> 1),
                    width, height);
#endif
          }
          if (frame < gFrost_Timings[work->args->variant][FROST_FRAME]) {
            if (frame > i + 16) {
              if (mote->z > 4)
                mote->z -= 2;
              if (mote->y < toFIXED(FROST_MOTE_DEPTH))
                mote->y += toFIXED(5);
              mote->x += TRIG_TURN / 128;
            }
          } else {
            mote->z += 8;
            mote->y -= INTtoFIXED(i % 5 + 2);
            if (lowest > mote->y)
              lowest = mote->y;
            if (highest < mote->y)
              highest = mote->y;
          }
        }
      }
    }
    lowest += toFIXED(64);
    highest += toFIXED(64);

    // Ice mounds spring up.
    if (frame < gFrost_Timings[work->args->variant][FROST_FRAME]) {
      for (i = 0; i != gFrost_Timings[work->args->variant][FROST_MOUNDS]; i++) {
        if (i < (frame - 36) / 3) {
          s32 picture = i % 3;
          u32 height;

          if (frame >= gFrost_Timings[work->args->variant][FROST_FRAME] - 7) {
#ifdef UBFIX
            height = gFrost_MoundHeights[picture];
            draw[1](canvas, work->sheet + gFrost_MoundPictures[picture],
                    gFrost_MoundSpots[i][FROST_MOUND_X],
                    gFrost_MoundSpots[i][FROST_MOUND_FOOT] - height,
                    FROST_MOUND_WIDTH, height);
#else
            draw[1](canvas, work->sheet + gFrost_MoundPictures[picture],
                    gFrost_MoundSpots[i][FROST_MOUND_X],
                    gFrost_MoundSpots[i][FROST_MOUND_FOOT] -
                        (height = gFrost_MoundHeights[picture]),
                    FROST_MOUND_WIDTH, height);
#endif
          } else {
#ifdef UBFIX
            height = gFrost_MoundHeights[picture];
            draw[0](canvas, work->sheet + gFrost_MoundPictures[picture],
                    gFrost_MoundSpots[i][FROST_MOUND_X],
                    gFrost_MoundSpots[i][FROST_MOUND_FOOT] - height,
                    FROST_MOUND_WIDTH, height);
#else
            draw[0](canvas, work->sheet + gFrost_MoundPictures[picture],
                    gFrost_MoundSpots[i][FROST_MOUND_X],
                    gFrost_MoundSpots[i][FROST_MOUND_FOOT] -
                        (height = gFrost_MoundHeights[picture]),
                    FROST_MOUND_WIDTH, height);
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
      for (i = 0; i != FROST_CHIPS; i++) {
        struct Particle *chip = &PARTICLES[i];

        SET_POSITION(chip, INTtoFIXED(Random16() & 127),
                     INTtoFIXED((Random16() & 15) + 80),
                     ((Random16() & 63) - 32) << 12);
#ifdef BUGFIX
        chip->velocityX = 0;
#endif
        chip->velocityY = ((-Random16() & 15) - 16) << 13;
        chip->variant = (Random16() & 15) + 16;
      }
    }
    if (frame >= gFrost_Timings[work->args->variant][FROST_FRAME]) {
      for (i = 0; i != FROST_CHIPS_DRAWN; i++) {
        struct Particle *chip = &PARTICLES[i];

        if (chip->variant >= 0) {
          s32 picture = i % 8;

          draw[1](canvas, work->sheet + gFrost_ChipPictures[picture],
                  FIXEDtoINT(chip->x), FIXEDtoINT(chip->y),
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
    lowest = FIXEDtoINT(lowest);
    highest = FIXEDtoINT(highest);
    if (highest <= lowest)
      highest = lowest + 1;

    // Sparkles rise over the motes and chips.
    if (frame == gFrost_Timings[work->args->variant][FROST_FRAME]) {
      for (i = 0; i != FROST_SPARKLES; i++) {
        struct Particle *sparkle = &work->particles[i];

        // A sparkle keeps its place in its velocity, as it never moves.
        sparkle->velocityX = INTtoFIXED(Random16() & 127);
        if (highest == lowest)
          sparkle->velocityY = INTtoFIXED(lowest);
        else
          sparkle->velocityY =
              INTtoFIXED(Random16() % (highest - lowest) + lowest);
        sparkle->variant = (Random16() & 15) + 20;
      }
    }
    if (frame >= gFrost_Timings[work->args->variant][FROST_FRAME]) {
      s32 rise = (frame - gFrost_Timings[work->args->variant][FROST_FRAME]) / 2;

      for (i = 0; i != FROST_SPARKLES; i++) {
        struct Particle *sparkle = &work->particles[i];

        if (sparkle->variant >= 0 && sparkle->variant < FROST_SPARKLE_LIFE) {
          s32 picture = (FROST_SPARKLE_LIFE - 1 - sparkle->variant) / 2;
          u32 size;

#ifdef UBFIX
          size = gBattleEffect_SparkleSizes[picture];
          draw[1](canvas, sparkleSheet + gBattleEffect_SparklePictures[picture],
                  FIXEDtoINT(sparkle->velocityX) - (size >> 1),
                  FIXEDtoINT(sparkle->velocityY) - (size >> 1) - rise, size,
                  size);
#else
          draw[1](canvas, sparkleSheet + gBattleEffect_SparklePictures[picture],
                  FIXEDtoINT(sparkle->velocityX) -
                      ((size = gBattleEffect_SparkleSizes[picture]) >> 1),
                  FIXEDtoINT(sparkle->velocityY) - (size >> 1) - rise, size,
                  size);
#endif
        }
        sparkle->variant--;
        if ((sparkle->variant == -1 ||
             sparkle->variant == FROST_SPARKLE_LIFE - 1) &&
            frame < gFrost_Timings[work->args->variant][FROST_FRAME] + 35) {
          sparkle->variant = FROST_SPARKLE_LIFE - 1;
          sparkle->velocityX = INTtoFIXED(Random16() & 127);
          sparkle->velocityY =
              INTtoFIXED(Random16() % (highest - lowest) + lowest);
        }
      }
    }
    EFFECT_PRESENT_CANVAS(work);
  }

  EFFECT_RELEASE_BLITTERS();
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}

#undef FROST_MOUND_WIDTH
#undef FROST_BURST_MOTES
#undef FROST_MOTE_DEPTH
#undef FROST_CHIPS
#undef FROST_CHIPS_DRAWN
#undef FROST_SPARKLES
#undef FROST_SPARKLE_LIFE
