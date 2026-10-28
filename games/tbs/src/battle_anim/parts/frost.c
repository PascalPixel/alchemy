/// @file
/// Frost.
// Part of battle_anim.c.

#include "../battle_anim_internal.h"

/// One row of gFrost_Timings, for each variant.
enum {
  FROST_MOTES,   ///< how many motes spiral down
  FROST_COLUMNS, ///< how many ice columns spring up
  FROST_FRAME    ///< the frame the motes burst
};

/// Motes spiral down and inwards while ice columns spring up one every 3
/// frames from frame 36; at the burst frame the motes spread and rise, chips
/// fly up and smoke rises over the heights they span.
///
/// Abilities: Frost, Tundra, Glacier (BATTLE_ANIM_FROST).
///
/// The targets take a blue tint 4 frames apart from frame 40, the battle shows
/// the damage 11 frames after the burst, and the animation ends 75 frames after
/// it.
///
/// work->particles holds the 64 motes (angle, height and radius in x, y and
/// z); after the burst the first 32 also hold the smoke puffs, in their
/// velocity and variant fields, while motes 0 to 15 keep moving in theirs.
/// PARTICLES holds the 32 chips. Writes REG_BG2PA and, on ANIM_SIDE_PARTY,
/// REG_BG2X; ends with the blitters freed, the transfer task removed and
/// the canvas layer closed.
///
/// args: the targets; variant picks the number of motes and columns and
///         the burst frame (gFrost_Timings): 32 motes, 8 columns and frame
///         85 for Frost, 48, 12 and 97 for Tundra, 64, 19 and 109 for
///         Glacier.
void BattleAnim_RunFrost(struct BattleAnimArgs *args) {
  void **slots;
  void **cursor;
  struct BattleAnimWork *work;
  void *canvas;
  s32 frame;
  u8 *puffSheet;
  DrawRectangle *draw;
  s32 count;
  s32 lowest;
  s32 highest;
  s32 xyz[3];
  struct ScreenPoint screen;
  s32 point[3];
  BlitterPair pair;
  struct Particle *mote;
  s32 i;

  slots = &gHeapSlots[HEAP_SLOT_BATTLE_ANIM];
  cursor = slots;
  ANIM_GET_WORK_AND_CANVAS(cursor, work, canvas);
  puffSheet = ANIM_SLOT(slots, HEAP_SLOT_BATTLE_SHEET);
  work->args = args;
  ANIM_START_NARROW_CANVAS(BGCNT_PRIORITY(1));
  Resource_LoadAndDecompress((s32)&ResourceId_IceChipSheet, work->sheet, 1, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_SparkleDots, puffSheet, 0, 0);
  /* FAKEMATCH(temporary): the motes, columns, chips and puffs draw through a
   * pointer to the pair, the columns' second picture through the pair itself;
   * as pair[1] everywhere, 36 of 782 instructions differ in tbs-en. */
  BattleAnim_FetchRectangleBlitters(0, draw = pair);

  // Motes: x holds the angle, z the radius (56 to 119), y the vertical
  // offset (-64 to -33 pixels, positive downwards on the canvas).
  for (i = 0; i != 64; i++) {
    struct Particle *seed = &work->particles[i];

    seed->x = Random16() & 0xffff;
    seed->z = (Random16() & 63) + 56;
    seed->y = ((Random16() & 31) - 64) << 16;
  }

  ANIM_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
  // On ANIM_SIDE_PARTY the canvas moves 112 pixels right, over the party.
  if (work->args->side == ANIM_SIDE_PARTY)
    REG_BG2X = -0x7000;

  for (frame = 0;
       frame != gFrost_Timings[work->args->variant][FROST_FRAME] + 75;
       frame++) {
    // The range of heights the smoke will rise over, in 16.16 pixels,
    // gathered from the motes and chips below.
    lowest = 120 << 16;
    highest = 0;
    if (frame == gFrost_Timings[work->args->variant][FROST_FRAME] + 11)
      ANIM_SHOW_DAMAGE(SE_WATER);
    // The motes turn about (0, 0, 512), not about a target. That point is
    // behind the eye, so they are never projected (see the motes below).
    SET_VECTOR(point, 0, 0, 512 << 16);
    Transform_Reset();
    Transform_ApplyPosition(point);
    // A chime every 4 frames while the columns spring up.
    if (frame >= 36 && frame < 64 && (frame & 3) == 0)
      Sound_PlaySongFar(SE_ICE);
    // Frame 85 is Frost's burst; Tundra and Glacier play this sound 12 and 24
    // frames before theirs.
    if (frame == 85)
      Sound_PlaySongFar(SE_WHOOSH);
    ANIM_FOR_EACH_TARGET(i, work->args) {
      if (frame == i * 4 + 40)
        BattleAnim_TintBattler(work->args->targets[i], SPRITE_COLOR_BLUE_TINT,
                               POSE_DAMAGE, -1, 0);
    }

    // Motes: mote i appears at frame i and, from 16 frames later, turns a
    // 128th of a circle a frame, closing in 2 pixels a frame and sinking 5
    // until y reaches 48. After the burst only the first 16 remain,
    // spreading out 8 pixels a frame and rising 2 to 6, and the range of
    // their heights is kept; none is drawn from 35 frames after it. A mote
    // is drawn only while y is between -48 and 48 - i / 2, so they go out at
    // staggered rows as they sink. The point lies behind the eye
    // (Transform_ProjectPoint looks down -z), so the projection is skipped and
    // position keeps the translated point in 16.16, with no perspective: y
    // grows down the canvas, and x is halved (>> 17) as on the wide canvas,
    // though this canvas is shown at its own width, so the spiral's
    // half-width is half its radius. It is centred on canvas (64, 60).
    if (frame < gFrost_Timings[work->args->variant][FROST_FRAME])
      count = gFrost_Timings[work->args->variant][FROST_MOTES];
    else
      count = 16;
    if (frame < gFrost_Timings[work->args->variant][FROST_FRAME] + 35) {
      for (i = 0; i != count; i++) {
        if (frame > i) {
          s32 cell = i % 8;

          mote = &work->particles[i];
          if (mote->y < (48 - i / 2) << 16 && mote->y > -(48 << 16)) {
            struct ScreenPoint *position;
            u32 width;
            u32 height;

            SET_VECTOR(xyz, mote->z * Trig_Sin(mote->x), mote->y,
                       mote->z * Trig_Cos(mote->x));
            Projection_Point(xyz, position = &screen);
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
          if (frame < gFrost_Timings[work->args->variant][FROST_FRAME]) {
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
    // Mote heights are relative to their centre: 64 rows down the canvas
    // puts them beside the chips' canvas rows.
    lowest += 64 << 16;
    highest += 64 << 16;

    // Columns (32 wide), standing at gFrost_ColumnSpots: one more every 3
    // frames from frame 36; over the last 7 frames before the burst they are
    // drawn additively (the pair's second blitter), a flash before they
    // break.
    if (frame < gFrost_Timings[work->args->variant][FROST_FRAME]) {
      for (i = 0; i != gFrost_Timings[work->args->variant][FROST_COLUMNS];
           i++) {
        if (i < (frame - 36) / 3) {
          s32 cell = i % 3;
          u32 height;

          if (frame >= gFrost_Timings[work->args->variant][FROST_FRAME] - 7) {
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

    // Chips: at the burst, 32 fly up from canvas rows 80 to 95 at 1/8 to 2
    // pixels a frame, for 16 to 31 frames each (24 of them are drawn).
    // BUG: velocityX is never set, so a chip drifts sideways by whatever word
    // its PARTICLES slot last held. Nothing in a battle clears that buffer,
    // the map grid's cells, so the word is the field's cells or an earlier
    // animation's particles or pictures.
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
      // The first 24 chips, drawn while they last; every one, live or not,
      // widens the range of heights the smoke rises over.
      for (i = 0; i != 24; i++) {
        struct Particle *chip = &PARTICLES[i];

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
    // half a pixel a frame. A puff keeps its x and y in velocityX and
    // velocityY, since x, y and z still hold the first 16 motes, and waits
    // 3 to 18 frames before its first picture (variant 20 to 35).
    if (frame == gFrost_Timings[work->args->variant][FROST_FRAME]) {
      for (i = 0; i != 32; i++) {
        struct Particle *puff = &work->particles[i];

        puff->velocityX = (Random16() & 127) << 16;
        if (highest == lowest)
          puff->velocityY = lowest << 16;
        else
          puff->velocityY = (Random16() % (highest - lowest) + lowest) << 16;
        puff->variant = (Random16() & 15) + 20;
      }
    }
    if (frame >= gFrost_Timings[work->args->variant][FROST_FRAME]) {
      // Each puff plays nine pictures, 2 frames each, and until 35 frames
      // after the burst starts again at a random spot when it goes out. The
      // spot chosen at the burst is replaced too, as the wait counts down to
      // 17, so every run of pictures starts at a fresh spot.
      s32 rise = (frame - gFrost_Timings[work->args->variant][FROST_FRAME]) / 2;

      for (i = 0; i != 32; i++) {
        struct Particle *puff = &work->particles[i];

        if (puff->variant >= 0 && puff->variant <= 17) {
          s32 cell = (17 - puff->variant) / 2;
          u32 size;

#ifdef UBFIX
          size = gBattleAnim_PuffSizes[cell];
          draw[1](canvas, puffSheet + gBattleAnim_PuffCells[cell],
                  Q_16_16_TO_INT(puff->velocityX) - (size >> 1),
                  Q_16_16_TO_INT(puff->velocityY) - (size >> 1) - rise, size,
                  size);
#else
          draw[1](canvas, puffSheet + gBattleAnim_PuffCells[cell],
                  Q_16_16_TO_INT(puff->velocityX) -
                      ((size = gBattleAnim_PuffSizes[cell]) >> 1),
                  Q_16_16_TO_INT(puff->velocityY) - (size >> 1) - rise, size,
                  size);
#endif
        }
        puff->variant--;
        if ((puff->variant == -1 || puff->variant == 17) &&
            frame < gFrost_Timings[work->args->variant][FROST_FRAME] + 35) {
          puff->variant = 17;
          puff->velocityX = (Random16() & 127) << 16;
          puff->velocityY = (Random16() % (highest - lowest) + lowest) << 16;
        }
      }
    }
    ANIM_PRESENT_CANVAS(work);
  }

  ANIM_RELEASE_BLITTERS();
  Task_Remove(BattleAnim_ProcessPendingGraphicsTransfer);
  BattleAnim_EndCanvasLayer();
}
