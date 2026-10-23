/// @file
/// Quake.
// Part of battle_anim.c.

#include "../battle_anim_internal.h"

/// The columns of a gQuake_Timings row.
enum {
  QUAKE_TIMING_FRAMES,      ///< frames in the whole animation
  QUAKE_TIMING_RISE_FRAMES, ///< frames each pillar stands
  QUAKE_TIMING_SHAKE        ///< how far the screen shakes
};

/// The peak heights of Quake's rock pillars: BattleAnim_RunQuake copies its
/// static table into a local of this type before the frame loop, and only
/// the first three columns are drawn.
struct RockWallHeights {
  /// Each pillar's peak height in pixels, -32: negative, but only its
  /// absolute value counts.
  s32 column[4];
};

#define QUAKE_PILLAR_WIDTH 40 ///< a pillar's width in pixels
/// The rock picture in the sheet; the sheet starts with the 40x16 strip of
/// ground cut from the background.
#define QUAKE_WALL_CELLS 0x1e00

/// Three rock pillars rise from the left, 4 frames apart, and sink again,
/// throwing the targets standing on them into the air.
///
/// Abilities: Quake, Earthquake, Quake Sphere (BATTLE_ANIM_QUAKE).
///
/// Each pillar is capped with a strip of ground cut from the background.
///
/// Writes REG_BG2PA, REG_BLDCNT and, on side 1, REG_BG2X; overwrites the
/// first 0x8000 bytes of Ram_MapGridCellBuffer with BG tiles; sets the targets'
/// vertical velocity and gravity; arms work->shakeFrames.
///
/// effect: the targets; variant picks the row of gQuake_Timings.
void BattleAnim_RunQuake(struct BattleAnimArgs *effect) {
  static s32 wallHeights[4] = {
      -32,
      -32,
      -32,
      -32,
  };
  void **slots;
  void **cursor;
  struct BattleAnimWork *work;
  void *canvas;
  s32 frame;
  s32 column;
  DrawRectangle draw;
  struct BattleCamera *camera;
  s32 xOffset;
  struct RockWallHeights heights;
  s32 point[3];
  struct ScreenPoint screen;
  s32 member;
  s32 row;
  s32 col;
  s32 y;

  slots = &gHeapSlots[HEAP_SLOT_BATTLE_ANIM];
  cursor = slots;
  ANIM_GET_WORK_AND_CANVAS(cursor, work, canvas);
  camera = ((struct BattleCamera *)HEAP_SLOT_BLOCK(HEAP_SLOT_CAMERA));
  work->effect = effect;
  BattleAnim_StartCanvasLayer(1);
  // The canvas at its own width, opaque: the pillars hide what is behind.
  REG_BG2PA = Q_8_8(1);
  REG_BLDCNT = 0;
  Resource_LoadAndDecompress((s32)&ResourceId_RockWallSheet,
                             work->sheet + QUAKE_WALL_CELLS, 1, 1);

  // Copy the background tiles (0x8000 bytes from VRAM 0x06008000) and cut
  // the strip at x 32 to 71, y 96 to 111 into the start of the sheet: the
  // ground that caps each pillar.
  Iwram_CopyWords(Ram_MapGridCellBuffer, BG_CHAR_BLOCK(2), 0x8000);
  for (row = 0; row != 16; row++) {
    y = row + 96;
    for (col = 0; col != QUAKE_PILLAR_WIDTH; col++) {
      // Pixel (col + 32, y) in 8bpp tiles laid out 32 tiles across.
      s32 pixel = (((col + 32) & 7) + (((col + 32) / 8) << 6) + ((y & 7) << 3) +
                   ((y / 8) << 11));

      work->sheet[row * QUAKE_PILLAR_WIDTH + col] =
          Ram_MapGridCellBuffer[pixel];
    }
  }

  // On side 1 the canvas moves 112 pixels right, and the targets' positions
  // with it.
  if (work->effect->side == 1) {
    REG_BG2X = -(112 << 8);
    xOffset = -112;
  } else {
    xOffset = 0;
  }
  ANIM_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, ANIM_CLIP_BOUNDS,
                    COMPOSITE_TRANSPARENT);
  draw = (DrawRectangle)gHeapSlots[HEAP_SLOT_RAM_CODE];
  // particles[member].variant becomes 1 once that target has landed.
  ANIM_FOR_EACH_TARGET(member, work->effect)
  work->particles[member].variant = 0;
  // Each transfer clears the canvas: the pillars are redrawn every frame.
  ANIM_START_TRANSFER(work, CANVAS_COPY_AND_CLEAR, 0);
  // The heights are negative; the sine below is taken as its absolute value,
  // so each pillar still rises up to 32 pixels.
  heights = (*(const struct RockWallHeights *)&wallHeights);
  // The screen shakes for 128 frames: all of Quake (80) and Earthquake (112),
  // and all but the last 16 of Quake Sphere (144).
  work->shakeFrames = 128;
  Sound_PlaySongFar(SE_RUMBLE);

  for (frame = 0;
       frame != gQuake_Timings[work->effect->variant][QUAKE_TIMING_FRAMES];
       frame++) {
    ANIM_VIEW_FROM_CAMERA(camera);
    // The damage shows 16 frames before the end.
    if (frame ==
        gQuake_Timings[work->effect->variant][QUAKE_TIMING_FRAMES] - 16)
      ANIM_SHOW_DAMAGE(SE_IMPACT);

    // The pillars rise from frames 16, 20 and 24.
    for (column = 0; column != 3; column++) {
      s32 riseFrame = column * 4 + 16;

      // Never true: the left side is 32 or more and riseFrame at most 24.
      if ((frame & 31) + 32 == riseFrame)
        heights.column[column] += 32;
      if (frame >= riseFrame &&
          frame < riseFrame + gQuake_Timings[work->effect->variant]
                                            [QUAKE_TIMING_RISE_FRAMES]) {
        s32 height;
        s32 top;

        // The height rises and falls along a sine, 1/64 of a turn a frame,
        // its absolute value taken: a pillar rises and sinks every 32 frames,
        // once for Quake, twice for Earthquake and three times for Quake
        // Sphere (it stands 32, 64 or 96 frames).
        height =
            (heights.column[column] * Trig_Sin((frame - riseFrame) << 10)) >>
            16;
        if (height < 0)
          height = -height;
        top = 112 - height;
        draw(canvas, work->sheet + QUAKE_WALL_CELLS,
             column * QUAKE_PILLAR_WIDTH + 8, top, QUAKE_PILLAR_WIDTH, height);
        draw(canvas, work->sheet, column * QUAKE_PILLAR_WIDTH + 8, 96 - height,
             QUAKE_PILLAR_WIDTH, 16);

        // Throw up the targets standing on the pillar: those whose projected
        // point, 16 pixels above the feet, lies in its 41 columns (both edges
        // count) and at or below its top. A target below the ground takes its
        // damage pose.
        ANIM_FOR_EACH_TARGET(member, work->effect) {
          struct MotionObject *object;

          object =
              GetBattleObjectSlotFar(work->effect->targets[member])->object;
          SET_VECTOR(point, object->x, object->y, object->z);
          Projection_Point(point, &screen);
          screen.x += xOffset;
          if (screen.x >= column * QUAKE_PILLAR_WIDTH + 8 &&
              screen.x <=
                  column * QUAKE_PILLAR_WIDTH + 8 + QUAKE_PILLAR_WIDTH &&
              screen.y >= top) {
            // 12 pixels a frame upwards, under the gravity the battle's own
            // motions use (BattleMotion_ReturnToSpot in battle/motion.c).
            object->velocityY = 12 << 16;
            object->verticalMotionStrength = 0xab85;
          }
          if (object->y < 0)
            ObjectGroup_UpdateMembers(work->effect->targets[member], 0,
                                      POSE_DAMAGE, -1, 0);
        }
      }
    }

    // A target that lands flashes white for 5 frames.
    ANIM_FOR_EACH_TARGET(member, work->effect) {
      struct MotionObject *object;
      struct Particle *state;

      object = GetBattleObjectSlotFar(work->effect->targets[member])->object;
      state = &work->particles[member];
      if (state->variant == 0 && object->y <= 0 && object->velocityY < 0) {
        state->variant = 1;
        ObjectGroup_UpdateMembers(work->effect->targets[member],
                                  SPRITE_COLOR_WHITE, POSE_DAMAGE, member, 5);
      }
    }
    // Shake ranges of 2, 4 or 8 pixels, by variant, centred on the view.
    BattleCamera_Shake(
        gQuake_Timings[work->effect->variant][QUAKE_TIMING_SHAKE],
        gQuake_Timings[work->effect->variant][QUAKE_TIMING_SHAKE]);
    ANIM_END_FRAME(work);
  }

  ANIM_FINISH_CANVAS_ONE_BLITTER();
}

#undef QUAKE_PILLAR_WIDTH
#undef QUAKE_WALL_CELLS
