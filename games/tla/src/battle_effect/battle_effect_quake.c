/// @file
/// Quake.
// Part of battle_effect.c.

#include "battle_effect_internal.h"
#include "sounds.h"

#define QUAKE_PILLAR_WIDTH 40 ///< a pillar's width in pixels
/// The rock picture in the sprite sheet; the sprite sheet starts with the 40x16
/// strip of ground cut from the background.
#define QUAKE_WALL_PICTURE 7680

/// The byte of pixel (x, y) in the battle background's tiles, laid out 32 to a
/// row in 256 colours.
#define BACKGROUND_PIXEL_OFFSET(x, y)                                          \
  (((x) & 7) + (((x) / 8) << 6) + (((y) & 7) << 3) + (((y) / 8) << 11))

/// Three rock pillars rise from the left, 4 frames apart, and sink again,
/// throwing the targets standing on them into the air.
///
/// Abilities: Quake, Earthquake, Quake Sphere.
void BattleEffect_RunQuake(struct BattleEffectParams *args) {
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

  work = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  camera = Ram_HeapSlots->cameraWork;
  // The canvas at its own width, opaque: the pillars hide what is behind them.
  EFFECT_START_NARROW_CANVAS(BGCNT_PRIORITY(1));
  REG_BLDCNT = 0;
  Resource_LoadAndDecompress(RESOURCE_ROCK_WALL_SHEET,
                             work->sheet + QUAKE_WALL_PICTURE, 1, 1);

  // Cut the strip of ground that caps each pillar from the battle background.
  Iwram_CopyWords(Ram_MapCellBuffer, (void *)BG_CHAR_BLOCK(2),
                  2 * BG_CHAR_SIZE);
  for (row = 0; row != 16; row++) {
    y = row + 96;
    for (col = 0; col != QUAKE_PILLAR_WIDTH; col++) {
      s32 pixel = BACKGROUND_PIXEL_OFFSET(col + 32, y);

      work->sheet[row * QUAKE_PILLAR_WIDTH + col] = Ram_MapCellBuffer[pixel];
    }
  }

  if (args->side == EFFECT_SIDE_PARTY) {
    REG_BG2X = -(NARROW_CANVAS_PARTY_X << 8);
    xOffset = -NARROW_CANVAS_PARTY_X;
  } else {
    xOffset = 0;
  }
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                      BLITTER_TRANSPARENT);
  draw = (DrawRectangle)Ram_HeapSlots->ramCode;
  EFFECT_FOR_EACH_TARGET(member, args)
    work->particles[member].variant = 0;
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_CLEAR, 0);
  heights = gRockWall_Heights;
  work->shakeFrames = 128;
  Sound_PlayFar(SE_RUMBLE);

  for (frame = 0; frame != gQuake_Timings[args->variant][TIMING_FRAMES];
       frame++) {
    EFFECT_VIEW_FROM_CAMERA(camera);
    if (frame == gQuake_Timings[args->variant][TIMING_FRAMES] - 16)
      EFFECT_SHOW_DAMAGE(SE_IMPACT);

    for (column = 0; column != 3; column++) {
      s32 riseFrame = column * 4 + 16;

      // DEAD: never true, as the left side is 32 or more and riseFrame at
      // most 24; the ROM keeps the test and its store.
      if ((frame & 31) + 32 == riseFrame)
        heights.column[column] += 32;
      if (frame >= riseFrame &&
          frame <
              riseFrame + gQuake_Timings[args->variant][TIMING_RISE_FRAMES]) {
        s32 height;
        s32 top;

        // A pillar rises and sinks along a sine, once, twice or three times by
        // variant.
        height = FIXEDtoINT(heights.column[column] *
                            Trig_Sin((frame - riseFrame) * (TRIG_TURN / 64)));
        if (height < 0)
          height = -height;
        top = 112 - height;
        draw(canvas, work->sheet + QUAKE_WALL_PICTURE,
             column * QUAKE_PILLAR_WIDTH + 8, top, QUAKE_PILLAR_WIDTH, height);
        draw(canvas, work->sheet, column * QUAKE_PILLAR_WIDTH + 8, 96 - height,
             QUAKE_PILLAR_WIDTH, 16);

        // Throw up the targets standing on the pillar.
        EFFECT_FOR_EACH_TARGET(member, args) {
          struct MotionObject *object;

          object = BattleObject_GetFar(args->targets[member])->object;
          SET_VECTOR(point, object->x, object->y, object->z);
          EffectPosition_ProjectPoint(point, &screen);
          screen.x += xOffset;
          if (screen.x >= column * QUAKE_PILLAR_WIDTH + 8 &&
              screen.x <=
                  column * QUAKE_PILLAR_WIDTH + 8 + QUAKE_PILLAR_WIDTH &&
              screen.y >= top) {
            object->velocityY = toFIXED(12);
            object->verticalMotionStrength = BATTLE_UNIT_GRAVITY;
          }
          if (object->y < 0)
            BattleEffect_TintBattler(args->targets[member], SPRITE_COLOR_NORMAL,
                                     POSE_DAMAGE, TINT_KEEP, 0);
        }
      }
    }

    // Each target flashes white once, as it lands.
    EFFECT_FOR_EACH_TARGET(member, args) {
      struct MotionObject *object;
      struct Particle *state;

      object = BattleObject_GetFar(args->targets[member])->object;
      state = &work->particles[member];
      if (state->variant == 0 && object->y <= 0 && object->velocityY < 0) {
        state->variant = 1;
        BattleEffect_TintBattler(args->targets[member], SPRITE_COLOR_WHITE,
                                 POSE_DAMAGE, member, 5);
      }
    }
    BattleCamera_Shake(gQuake_Timings[args->variant][TIMING_SHAKE],
                       gQuake_Timings[args->variant][TIMING_SHAKE]);
    EFFECT_END_FRAME(work);
  }

  EFFECT_FINISH_CANVAS_ONE_BLITTER();
}

#undef QUAKE_PILLAR_WIDTH
#undef QUAKE_WALL_PICTURE
#undef BACKGROUND_PIXEL_OFFSET
