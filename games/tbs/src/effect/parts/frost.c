/// @file
/// Frost.
/// Part of effect.c, the battle effect object, compiled only through it.

#include "../effect_internal.h"

/// One row of gFrost_Timings, for each variant.
enum {
  FROST_MOTES,   ///< the rising motes
  FROST_COLUMNS, ///< the ice columns
  FROST_FRAME    ///< the frame the motes burst
};

/// Rising motes: motes spiral upwards while ice columns spring up one every 3
/// frames from frame 36; at the burst frame the motes spread and fall, chips
/// fly and smoke rises over the heights they span.
///
/// Abilities: Frost, Tundra, Glacier (kind 6).
///
/// The targets take a blue tint 4 frames apart from frame 40, the battle shows
/// the damage 11 frames after the burst, and the effect ends 75 frames after
/// it.
///
/// effect: the targets; variant picks the number of motes and columns and
///         the burst frame (gFrost_Timings).
void BattleFx_RunFrost(struct BattleEffectArgument *effect) {
  void **slots;
  void **cursor;
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  u8 *puffSheet;
  DrawRectangle *draw;
  s32 count;
  s32 lowest;
  s32 highest;
  s32 xyz[3];
  struct EffectPosition screen;
  s32 point[3];
  BlitterPair pair;
  struct EffectStep *mote;
  s32 i;

  slots = &gWorkSlot[HEAP_SLOT_BATTLE_EFFECT];
  cursor = slots;
  EFFECT_GET_WORK_AND_CANVAS(cursor, work, canvas);
  puffSheet = EFFECT_SLOT(slots, HEAP_SLOT_BATTLE_SHEET);
  work->effect = effect;
  EFFECT_BEGIN_NARROW_CANVAS(1);
  Resource_LoadAndDecompress((s32)&ResourceId_IceChipSheet, work->sheet, 1, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_SparkleDots, puffSheet, 0, 0);
  /* FAKEMATCH: the motes, columns, chips and puffs draw through a pointer
   * to the pair, the columns' second picture through the pair itself; with
   * pair[1] everywhere, 72 lines of tbs-en effect.s differ. */
  BattleFx_FetchRectangleBlitters(0, draw = pair);

  // Motes: x holds the angle, z the radius (56 to 119), y the height (-64
  // to -33 pixels).
  for (i = 0; i != 64; i++) {
    struct EffectStep *seed = &work->particles[i];

    seed->x = Random16() & 0xffff;
    seed->z = (Random16() & 63) + 56;
    seed->y = ((Random16() & 31) - 64) << 16;
  }

  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, 50);
  if (work->effect->side == 1)
    REG_BG2X = -0x7000;

  for (frame = 0;
       frame != gFrost_Timings[work->effect->variant][FROST_FRAME] + 75;
       frame++) {
    lowest = 120 << 16;
    highest = 0;
    if (frame == gFrost_Timings[work->effect->variant][FROST_FRAME] + 11)
      EFFECT_SHOW_DAMAGE(SE_132);
    SET_VECTOR(point, 0, 0, 512 << 16);
    Render_ResetTransformState();
    SceneTransform_ApplyPosition(point);
    if (frame >= 36 && frame < 64 && (frame & 3) == 0)
      AudioCommand_PlayFar(SE_115);
    if (frame == 85)
      AudioCommand_PlayFar(SE_136);
    EFFECT_FOR_EACH_TARGET(i, work->effect) {
      if (frame == i * 4 + 40)
        ObjectGroup_UpdateMembers(work->effect->actors[i],
                                  SPRITE_COLOR_BLUE_TINT, POSE_DAMAGE, -1, 0);
    }

    // Motes: mote i appears at frame i and, from 16 frames later, turns,
    // closing in and rising 5 pixels a frame up to 48 pixels. After the
    // burst only the first 16 remain, spreading out and falling, and the
    // range of their heights is kept; none is drawn from 35 frames after it.
    if (frame < gFrost_Timings[work->effect->variant][FROST_FRAME])
      count = gFrost_Timings[work->effect->variant][FROST_MOTES];
    else
      count = 16;
    if (frame < gFrost_Timings[work->effect->variant][FROST_FRAME] + 35) {
      for (i = 0; i != count; i++) {
        if (frame > i) {
          s32 cell = i % 8;

          mote = &work->particles[i];
          if (mote->y < (48 - i / 2) << 16 && mote->y > -(48 << 16)) {
            struct EffectPosition *position;
            u32 width;
            u32 height;

            SET_VECTOR(xyz, mote->z * Trig_Sin(mote->x), mote->y,
                       mote->z * Trig_Cos(mote->x));
            EffectPosition_ProjectPoint(xyz, position = &screen);
            position->x = (position->x >> 17) + 64;
            position->y = Q_16_16_TO_INT(position->y) + 60;
            // UB: width is assigned in one argument and read in another.
#ifdef UBFIX
            width = gFrost_MoteWidths[cell];
            height = gFrost_MoteHeights[cell];
            draw[1](canvas, work->sheet + gFrost_MoteCells[cell],
                    position->x - (width >> 1), position->y - (height >> 1),
                    width, height);
#else
            draw[1](canvas, work->sheet + gFrost_MoteCells[cell],
                    position->x - ((width = gFrost_MoteWidths[cell]) >> 1),
                    position->y - ((height = gFrost_MoteHeights[cell]) >> 1),
                    width, height);
#endif
          }
          if (frame < gFrost_Timings[work->effect->variant][FROST_FRAME]) {
            if (frame > i + 16) {
              if (mote->z > 4)
                mote->z -= 2;
              if (mote->y < 48 << 16)
                mote->y += 5 << 16;
              mote->x += 0x200;
            }
          } else {
            mote->z += 8;
            mote->y -= (i % 5 + 2) << 16;
            if (lowest > mote->y)
              lowest = mote->y;
            if (highest < mote->y)
              highest = mote->y;
          }
        }
      }
    }
    lowest += 64 << 16;
    highest += 64 << 16;

    // Columns (32 wide): one more every 3 frames from frame 36; over the
    // last 7 frames before the burst they use the second blitter.
    if (frame < gFrost_Timings[work->effect->variant][FROST_FRAME]) {
      for (i = 0; i != gFrost_Timings[work->effect->variant][FROST_COLUMNS];
           i++) {
        if (i < (frame - 36) / 3) {
          s32 cell = i % 3;
          u32 height;

          if (frame >= gFrost_Timings[work->effect->variant][FROST_FRAME] - 7) {
#ifdef UBFIX
            height = gFrost_ColumnHeights[cell];
            draw[1](canvas, work->sheet + gFrost_ColumnCells[cell],
                    gFrost_ColumnSpots[i][RISING_MOTES_COLUMN_X],
                    gFrost_ColumnSpots[i][RISING_MOTES_COLUMN_FOOT] - height,
                    32, height);
#else
            draw[1](canvas, work->sheet + gFrost_ColumnCells[cell],
                    gFrost_ColumnSpots[i][RISING_MOTES_COLUMN_X],
                    gFrost_ColumnSpots[i][RISING_MOTES_COLUMN_FOOT] -
                        (height = gFrost_ColumnHeights[cell]),
                    32, height);
#endif
          } else {
#ifdef UBFIX
            height = gFrost_ColumnHeights[cell];
            pair[0](canvas, work->sheet + gFrost_ColumnCells[cell],
                    gFrost_ColumnSpots[i][RISING_MOTES_COLUMN_X],
                    gFrost_ColumnSpots[i][RISING_MOTES_COLUMN_FOOT] - height,
                    32, height);
#else
            pair[0](canvas, work->sheet + gFrost_ColumnCells[cell],
                    gFrost_ColumnSpots[i][RISING_MOTES_COLUMN_X],
                    gFrost_ColumnSpots[i][RISING_MOTES_COLUMN_FOOT] -
                        (height = gFrost_ColumnHeights[cell]),
                    32, height);
#endif
          }
        }
      }
    }

    // Chips: at the burst, 32 fly up from low down (24 of them are drawn).
    if (frame == gFrost_Timings[work->effect->variant][FROST_FRAME]) {
      for (i = 0; i != 32; i++) {
        struct EffectStep *chip = &PARTICLES[i];

        SET_POSITION(chip, (Random16() & 127) << 16,
                     ((Random16() & 15) + 80) << 16,
                     ((Random16() & 63) - 32) << 12);
        chip->velocityY = ((-Random16() & 15) - 16) << 13;
        chip->variant = (Random16() & 15) + 16;
      }
    }
    if (frame >= gFrost_Timings[work->effect->variant][FROST_FRAME]) {
      // The first 24 chips, drawn while they last; every one, live or not,
      // widens the range of heights the smoke rises over.
      for (i = 0; i != 24; i++) {
        struct EffectStep *chip = &PARTICLES[i];

        if (chip->variant >= 0) {
          s32 cell = i % 8;

          draw[1](canvas, work->sheet + gFrost_ChipCells[cell],
                  Q_16_16_TO_INT(chip->x), Q_16_16_TO_INT(chip->y),
                  gFrost_ChipWidths[cell], gFrost_ChipHeights[cell]);
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

    // Smoke: puffs appear over the heights of the motes and chips and rise
    // half a pixel a frame.
    if (frame == gFrost_Timings[work->effect->variant][FROST_FRAME]) {
      for (i = 0; i != 32; i++) {
        struct EffectStep *puff = &work->particles[i];

        puff->velocityX = (Random16() & 127) << 16;
        if (highest == lowest)
          puff->velocityY = lowest << 16;
        else
          puff->velocityY = (Random16() % (highest - lowest) + lowest) << 16;
        puff->variant = (Random16() & 15) + 20;
      }
    }
    if (frame >= gFrost_Timings[work->effect->variant][FROST_FRAME]) {
      // Each puff plays nine pictures, 2 frames each, and until 35 frames
      // after the burst starts again at a random spot when it goes out.
      s32 rise =
          (frame - gFrost_Timings[work->effect->variant][FROST_FRAME]) / 2;

      for (i = 0; i != 32; i++) {
        struct EffectStep *puff = &work->particles[i];

        if (puff->variant >= 0 && puff->variant <= 17) {
          s32 cell = (17 - puff->variant) / 2;
          u32 size;

#ifdef UBFIX
          size = gBattleFx_PuffSizes[cell];
          draw[1](canvas, puffSheet + gBattleFx_PuffCells[cell],
                  Q_16_16_TO_INT(puff->velocityX) - (size >> 1),
                  Q_16_16_TO_INT(puff->velocityY) - (size >> 1) - rise, size,
                  size);
#else
          draw[1](canvas, puffSheet + gBattleFx_PuffCells[cell],
                  Q_16_16_TO_INT(puff->velocityX) -
                      ((size = gBattleFx_PuffSizes[cell]) >> 1),
                  Q_16_16_TO_INT(puff->velocityY) - (size >> 1) - rise, size,
                  size);
#endif
        }
        puff->variant--;
        if ((puff->variant == -1 || puff->variant == 17) &&
            frame < gFrost_Timings[work->effect->variant][FROST_FRAME] + 35) {
          puff->variant = 17;
          puff->velocityX = (Random16() & 127) << 16;
          puff->velocityY = (Random16() % (highest - lowest) + lowest) << 16;
        }
      }
    }
    EFFECT_PRESENT_FRAME(work);
  }

  EFFECT_RELEASE_BLITTERS();
  Scheduler_RemoveCallback(BattleFx_ProcessPendingGraphicsTransfer);
  BattleFx_EndCanvasLayer();
}
