// Part of effect.c, the battle effect object, compiled only through it.
// Quake.

// The columns of a gQuake_Timings row.
enum {
  TIMING_FRAMES,      // frames in the whole effect
  TIMING_RISE_FRAMES, // frames each pillar stands
  TIMING_SHAKE        // how far the screen shakes
};

struct RockWallHeights {
  s32 column[4]; // each pillar's height in pixels
};

#define ROCK_PILLAR_WIDTH 40 // a pillar's width in pixels
// The rock picture in the sheet; the sheet starts with the 40x16 strip of
// ground cut from the background.
#define ROCK_WALL_CELLS 0x1e00

// Abilities: Quake, Earthquake, Quake Sphere (kind 1).
//
// Rock wall: three rock pillars rise from the left, 4 frames apart, and
// sink again, throwing the targets standing on them into the air. Each
// pillar is capped with a strip of ground cut from the background.
//
// effect: the targets; variant picks the row of gQuake_Timings.
void BattleFx_RunQuake(struct BattleEffectArgument *effect) {
  static s32 wallHeights[4] = {
      -32,
      -32,
      -32,
      -32,
  };
  void **slots;
  void **cursor;
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  s32 column;
  DrawRectangle draw;
  struct BattleCamera *camera;
  s32 xOffset;
  struct RockWallHeights heights;
  s32 point[3];
  struct EffectPosition screen;
  s32 member;
  s32 row;
  s32 col;
  s32 y;

  slots = &gWorkSlot[HEAP_SLOT_BATTLE_EFFECT];
  cursor = slots;
  EFFECT_GET_WORK_AND_CANVAS(cursor, work, canvas);
  camera = *(struct BattleCamera **)((u8 *)gWorkSlot + HEAP_SLOT_CAMERA * 4);
  work->effect = effect;
  BattleFx_BeginCanvasLayer(1);
  REG_BG2PA = Q_8_8(1); // the canvas at its own width
  REG_BLDCNT = 0;       // no blending
  Resource_LoadAndDecompress((s32)&ResourceId_RockWallSheet,
                             work->sheet + ROCK_WALL_CELLS, 1, 1);

  // Copy the background tiles (0x8000 bytes from VRAM 0x06008000) and cut
  // the strip at x 32 to 71, y 96 to 111 into the start of the sheet: the
  // ground that caps each pillar.
  Iwram_CopyWords(Ram_MapCellBuffer, BG_CHAR_BLOCK(2), 0x8000);
  for (row = 0; row != 16; row++) {
    y = row + 96;
    for (col = 0; col != ROCK_PILLAR_WIDTH; col++) {
      // Pixel (col + 32, y) in 8bpp tiles laid out 32 tiles across.
      s32 pixel = (((col + 32) & 7) + (((col + 32) / 8) << 6) + ((y & 7) << 3) +
                   ((y / 8) << 11));

      work->sheet[row * ROCK_PILLAR_WIDTH + col] = Ram_MapCellBuffer[pixel];
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
  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, EFFECT_CLIP_BOUNDS,
                      COMPOSITE_TRANSPARENT);
  draw = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER];
  EFFECT_FOR_EACH_TARGET(member, work->effect)
    work->particles[member].variant = 0;      // 1 once the target has landed
  work->transferMode = CANVAS_COPY_AND_CLEAR; // each transfer clears the canvas
  work->transferValue = 0;
  Scheduler_AddOrUpdateCallback(BattleFx_ProcessPendingGraphicsTransfer, 0x480);
  heights = (*(const struct RockWallHeights *)&wallHeights);
  work->shakeFrames = 128;
  AudioCommand_PlayFar(141);

  for (frame = 0; frame != gQuake_Timings[work->effect->variant][TIMING_FRAMES];
       frame++) {
    EFFECT_VIEW_FROM_CAMERA(camera);
    if (frame == gQuake_Timings[work->effect->variant][TIMING_FRAMES] - 16)
      BattleEventRuntime_BeginPhaseFar(
          133); // 16 frames before the end, move on to the results

    // The pillars rise from frames 16, 20 and 24.
    for (column = 0; column != 3; column++) {
      s32 riseFrame = column * 4 + 16;

      if ((frame & 31) + 32 ==
          riseFrame) // never true: the left side is 32 or more
        heights.column[column] += 32;
      if (frame >= riseFrame &&
          frame <
              riseFrame +
                  gQuake_Timings[work->effect->variant][TIMING_RISE_FRAMES]) {
        s32 height;
        s32 top;

        // The height rises and falls along a sine, 1/64 of a turn a frame.
        height =
            (heights.column[column] * Trig_Sin((frame - riseFrame) << 10)) >>
            16;
        if (height < 0)
          height = -height;
        top = 112 - height;
        draw(canvas, work->sheet + ROCK_WALL_CELLS,
             column * ROCK_PILLAR_WIDTH + 8, top, ROCK_PILLAR_WIDTH, height);
        draw(canvas, work->sheet, column * ROCK_PILLAR_WIDTH + 8, 96 - height,
             ROCK_PILLAR_WIDTH, 16);

        // Throw up the targets standing on the pillar.
        EFFECT_FOR_EACH_TARGET(member, work->effect) {
          struct MotionObject *object;

          object = GetBattleObjectSlotFar(work->effect->actors[member])->object;
          SET_VECTOR(point, object->x, object->y, object->z);
          EffectPosition_ApplyBaseAndYOffset(point, &screen);
          screen.x += xOffset;
          if (screen.x >= column * ROCK_PILLAR_WIDTH + 8 &&
              screen.x <= column * ROCK_PILLAR_WIDTH + 8 + ROCK_PILLAR_WIDTH &&
              screen.y >= top) {
            object->velocityY = 12 << 16; // 12 pixels a frame upwards
            object->verticalMotionStrength = 0xab85;
          }
          if (object->y < 0)
            ObjectGroup_UpdateMembers(work->effect->actors[member], 0,
                                      POSE_DAMAGE, -1, 0);
        }
      }
    }

    // A target that lands flashes white for 5 frames.
    EFFECT_FOR_EACH_TARGET(member, work->effect) {
      struct MotionObject *object;
      struct EffectStep *state;

      object = GetBattleObjectSlotFar(work->effect->actors[member])->object;
      state = &work->particles[member];
      if (state->variant == 0 && object->y <= 0 && object->velocityY < 0) {
        state->variant = 1;
        ObjectGroup_UpdateMembers(work->effect->actors[member],
                                  SPRITE_COLOR_WHITE, POSE_DAMAGE, member, 5);
      }
    }
    Camera_ApplyShake(gQuake_Timings[work->effect->variant][TIMING_SHAKE],
                      gQuake_Timings[work->effect->variant][TIMING_SHAKE]);
    EFFECT_END_FRAME(work);
  }

  EFFECT_FINISH_CANVAS_ONE_BLITTER();
}
