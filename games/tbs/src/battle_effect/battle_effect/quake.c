/// @file
/// Quake.
// Part of battle_effect.c.

#include "../battle_effect_internal.h"

/// The columns of a gQuake_Timings row.
enum {
  QUAKE_TIMING_FRAMES,      ///< frames in the whole effect
  QUAKE_TIMING_RISE_FRAMES, ///< frames each pillar stands
  QUAKE_TIMING_SHAKE        ///< how far the screen shakes
};

/// The peak heights of Quake's rock pillars; only the first three are drawn.
struct RockWallHeights {
  /// Each pillar's peak in pixels, negative, of which only the size counts.
  s32 column[4];
};

#define QUAKE_PILLAR_WIDTH 40 ///< a pillar's width in pixels
/// The rock picture in the sheet; the sheet starts with the 40x16 strip of
/// ground cut from the background.
#define QUAKE_WALL_PICTURE 7680

/// Three rock pillars rise from the left, 4 frames apart, and sink again,
/// throwing the targets standing on them into the air.
///
/// Abilities: Quake, Earthquake, Quake Sphere.
void BattleEffect_RunQuake(struct BattleEffectParams *args) {
  static s32 wallHeights[4] = {
      -32,
      -32,
      -32,
      -32,
  };
  struct BattleEffectWork *work;
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

  work = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
  canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
  camera = ((struct BattleCamera *)HEAP_SLOT_BLOCK(HEAP_SLOT_CAMERA));
  work->args = args;
  BattleEffect_StartCanvasLayer(BGCNT_PRIORITY(1));
  // The canvas at its own width, opaque: the pillars hide what is behind them.
  REG_BG2PA = Q_8_8(1);
  REG_BLDCNT = 0;
  Resource_LoadAndDecompress(RESOURCE_ROCK_WALL_SHEET,
                             work->sheet + QUAKE_WALL_PICTURE, 1, 1);

  // Cut the strip of ground that caps each pillar from the battle background.
  Iwram_CopyWords(Ram_MapGridCellBuffer, BG_CHAR_BLOCK(2), 2 * BG_CHAR_SIZE);
  for (row = 0; row != 16; row++) {
    y = row + 96;
    for (col = 0; col != QUAKE_PILLAR_WIDTH; col++) {
      s32 pixel = (((col + 32) & 7) + (((col + 32) / 8) << 6) + ((y & 7) << 3) +
                   ((y / 8) << 11));

      work->sheet[row * QUAKE_PILLAR_WIDTH + col] =
          Ram_MapGridCellBuffer[pixel];
    }
  }

  if (work->args->side == EFFECT_SIDE_PARTY) {
    REG_BG2X = -(112 << 8);
    xOffset = -112;
  } else {
    xOffset = 0;
  }
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                      COMPOSITE_TRANSPARENT);
  draw = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
  EFFECT_FOR_EACH_TARGET(member, work->args)
    work->particles[member].variant = 0;
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_CLEAR, 0);
  heights = (*(const struct RockWallHeights *)&wallHeights);
  work->shakeFrames = 128;
  Sound_PlayFar(SE_RUMBLE);

  for (frame = 0;
       frame != gQuake_Timings[work->args->variant][QUAKE_TIMING_FRAMES];
       frame++) {
    EFFECT_VIEW_FROM_CAMERA(camera);
    if (frame == gQuake_Timings[work->args->variant][QUAKE_TIMING_FRAMES] - 16)
      EFFECT_SHOW_DAMAGE(SE_IMPACT);

    for (column = 0; column != 3; column++) {
      s32 riseFrame = column * 4 + 16;

      // DEAD: never true, as the left side is 32 or more and riseFrame at
      // most 24; the ROM keeps the test and its store.
      if ((frame & 31) + 32 == riseFrame)
        heights.column[column] += 32;
      if (frame >= riseFrame &&
          frame < riseFrame + gQuake_Timings[work->args->variant]
                                            [QUAKE_TIMING_RISE_FRAMES]) {
        s32 height;
        s32 top;

        // A pillar rises and sinks along a sine, once, twice or three times by
        // variant.
        height =
            (heights.column[column] * Trig_Sin((frame - riseFrame) << 10)) >>
            16;
        if (height < 0)
          height = -height;
        top = 112 - height;
        draw(canvas, work->sheet + QUAKE_WALL_PICTURE,
             column * QUAKE_PILLAR_WIDTH + 8, top, QUAKE_PILLAR_WIDTH, height);
        draw(canvas, work->sheet, column * QUAKE_PILLAR_WIDTH + 8, 96 - height,
             QUAKE_PILLAR_WIDTH, 16);

        // Throw up the targets standing on the pillar.
        EFFECT_FOR_EACH_TARGET(member, work->args) {
          struct Actor *actor;

          actor = GetBattleObjectSlotFar(work->args->targets[member])->actor;
          SET_VECTOR(point, actor->x.fixed, actor->y.fixed, actor->z.fixed);
          Projection_Point(point, &screen);
          screen.x += xOffset;
          if (screen.x >= column * QUAKE_PILLAR_WIDTH + 8 &&
              screen.x <=
                  column * QUAKE_PILLAR_WIDTH + 8 + QUAKE_PILLAR_WIDTH &&
              screen.y >= top) {
            actor->velocityY = px(12);
            actor->gravity = BATTLE_UNIT_GRAVITY;
          }
          if (actor->y.fixed < 0)
            BattleEffect_TintBattler(work->args->targets[member],
                                     SPRITE_COLOR_NORMAL, POSE_DAMAGE,
                                     TINT_KEEP, 0);
        }
      }
    }

    EFFECT_FOR_EACH_TARGET(member, work->args) {
      struct Actor *actor;
      struct Particle *state;

      actor = GetBattleObjectSlotFar(work->args->targets[member])->actor;
      state = &work->particles[member];
      if (state->variant == 0 && actor->y.fixed <= 0 && actor->velocityY < 0) {
        state->variant = 1;
        BattleEffect_TintBattler(work->args->targets[member],
                                 SPRITE_COLOR_WHITE, POSE_DAMAGE, member, 5);
      }
    }
    BattleCamera_Shake(gQuake_Timings[work->args->variant][QUAKE_TIMING_SHAKE],
                       gQuake_Timings[work->args->variant][QUAKE_TIMING_SHAKE]);
    EFFECT_END_FRAME(work);
  }

  EFFECT_FINISH_CANVAS_ONE_BLITTER();
}

#undef QUAKE_PILLAR_WIDTH
#undef QUAKE_WALL_PICTURE
