/// @file
/// Torch and Kite.
// Part of battle_effect.c.

#include "battle_effect_internal.h"

#define TORCH_FRAMES 80
#define TORCH_FADE_FRAMES 8   ///< the disc fades in, and later out, over these
#define TORCH_STRIKE_FRAME 48 ///< the target is struck

/// The ring level drawn brightest: the levels either side of it dim toward 0
/// and toward the disc's edge.
#define TORCH_BRIGHTEST_LEVEL 32
/// The disc's colours are worked in eight-bit channels, each capped, then cut
/// to the five bits a colour holds; blue's lower cap cuts to the same level.
#define TORCH_CHANNEL_MAX 255
#define TORCH_BLUE_MAX 250
/// How far a line sways either way: 32 canvas pixels, in a sway's quarter
/// pixels.
#define TORCH_SWAY (32 * 4)

/// A fiery disc fades in over the battle, its rings running outward while
/// every line of the display ripples; the target is struck, the disc fades
/// out.
///
/// Abilities: Torch.
void BattleEffect_RunTorch(struct BattleEffectParams *args) {
  struct BattleEffectState *state;
  void *canvas;
  s32 screen[3];
  s32 world[3];
  DrawRectangle draw[2];
  s32 x;
  s32 y;
  s32 dx;
  s32 dy;
  s32 centerY;
  s32 distance;
  s32 i;
  s32 shade;
  s32 red;
  s32 green;
  s32 blue;
  s32 frame;
  s32 wave;
  s32 level;
  u16 *ramp;
  s32 angle;
  s32 *line;

  state = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  BattleEffect_StartCanvasLayer(BGCNT_WRAP);
  BattleEffect_RunDjinniEntrance(
      args,
      DJINNI_ENTRANCE(ELEMENT_MARS + DJINNI_ENTRANCE_LONG, args->side,
                      DJINNI_ANCHOR_OVER_USER),
      screen, world);
  REG_BG2CNT =
      BGCNT_WRAP | BGCNT_SCREEN_BASE(7) | BGCNT_256_COLORS | BGCNT_CHAR_BASE(1);
  REG_BLDALPHA = EFFECT_BLEND_CANVAS(0);
  REG_BG2PA = Q_8_8(1.0 / 1.5);
  BattleEffect_FetchRectangleBlitters(args->side, draw);
  EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_FADE,
                        CANVAS_FADE_THREE_QUARTERS);

  // The disc: each pixel's level is half its distance from the middle, its
  // rows drawn a little taller; one quarter is worked out and mirrored into
  // the other three.
  for (y = 0; y != CANVAS_SIZE / 2; y++) {
    for (x = 0; x != CANVAS_SIZE / 2; x++) {
      centerY = y / 8 + CANVAS_SIZE / 2;
      dy = y - centerY;
      dx = x - CANVAS_SIZE / 2;
      distance = Iwram_Sqrt(dx * dx + dy * dy);
      distance /= 2;
      if (distance == 0)
        distance = 1;
      if (distance > CANVAS_LEVEL_BRIGHTEST)
        distance = CANVAS_LEVEL_BRIGHTEST;
      state->sheet[y * CANVAS_SIZE + x] = distance;
      state->sheet[y * CANVAS_SIZE + CANVAS_SIZE - 1 - x] = distance;
      state->sheet[(CANVAS_SIZE - 1 - y) * CANVAS_SIZE + x] = distance;
      state->sheet[(CANVAS_SIZE - 1 - y) * CANVAS_SIZE + CANVAS_SIZE - 1 - x] =
          distance;
    }
  }

  // Its colours: red rises from the first level, green from the sixth and blue
  // from the eighth, so the rings run from dark red to pale orange at
  // TORCH_BRIGHTEST_LEVEL.
  ramp = (u16 *)Ram_MapCellBuffer;
  for (i = 1; i != CANVAS_LEVEL_BRIGHTEST + 1; i++) {
    if (i >= TORCH_BRIGHTEST_LEVEL)
      shade = 2 * TORCH_BRIGHTEST_LEVEL - i;
    else
      shade = i;
    red = shade * 9;
    green = (shade - 6) * 7;
    blue = (shade - 8) * 7;
    if (red < 0)
      red = 0;
    if (green < 0)
      green = 0;
    if (blue < 0)
      blue = 0;
    if (red > TORCH_CHANNEL_MAX)
      red = TORCH_CHANNEL_MAX;
    if (green > TORCH_CHANNEL_MAX)
      green = TORCH_CHANNEL_MAX;
    if (blue > TORCH_BLUE_MAX)
      blue = TORCH_BLUE_MAX;
    red >>= 3;
    green >>= 3;
    blue >>= 3;
    ramp[i] = ((u16 *)BG_PLTT)[i] = RGB(red, green, blue);
  }

  draw[0](canvas, state->sheet, 0, 0, CANVAS_SIZE, CANVAS_SIZE);
  state->transferPending = 1;
  Task_Add(BattleEffect_ArmBg2AffineHBlankDma, TASK_PASS_VBLANK);

  // Fade the disc in, ripple the display, then fade it out.
  for (frame = 0; frame != TORCH_FRAMES; frame++) {
    if (frame <= TORCH_FADE_FRAMES) {
      level = frame * 2;
      wave = level;
      REG_BLDALPHA = EFFECT_BLEND_CANVAS(level);
    } else {
      wave = frame * 2;
    }
    if (frame > TORCH_FRAMES - TORCH_FADE_FRAMES)
      REG_BLDALPHA = EFFECT_BLEND_CANVAS(TORCH_FRAMES * 2 - frame * 2);

    if (frame == TORCH_STRIKE_FRAME) {
      EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
      state->shakeFrames = 8;
      BattleMotion_ApplyKnockbackFar(args->targets[0], KNOCKBACK_NUDGE);
    }

    // Each line starts one canvas pixel further along than the one above,
    // swayed either way by up to TORCH_SWAY.
    line = state->bg2xLines;
    for (i = 0, angle = -(wave << 9); i != DISPLAY_HEIGHT; i++) {
      *line++ = CANVAS_WAVE_BG2X(i * toFIXED(4) - Trig_Sin(angle) * TORCH_SWAY +
                                 toFIXED(4));
      angle += TRIG_TURN / 128;
    }

    // Rotate the colours so the rings run outward.
    // DEAD: never true, as frame stays below TORCH_FRAMES; the ROM keeps the
    // compare and the store.
    if (frame > 127) {
      state->transferPending = 1;
    } else {
      ramp[1] = ramp[CANVAS_LEVEL_BRIGHTEST];
      Dma_Set(ramp + CANVAS_LEVEL_BRIGHTEST - 1, ramp + CANVAS_LEVEL_BRIGHTEST,
              DMA_COPY_HALFWORDS_DOWN(CANVAS_LEVEL_BRIGHTEST - 1), REG_DMA3);
      IO_WRITE_QUEUE_PUSH((u32)&((u16 *)BG_PLTT)[1], (u32)(ramp + 1),
                          DMA_COPY_HALFWORDS(CANVAS_LEVEL_BRIGHTEST));
    }
    BattleCamera_Shake(8, 8);
    BattleEffect_CountDownBattlerTints();
    wait(1);
  }

  Task_Remove(BattleEffect_ArmBg2AffineHBlankDma);
  EFFECT_FINISH_CANVAS();
}

/// The long Jupiter Djinni entrance over the first target is the whole effect.
///
/// Abilities: Kite.
void BattleEffect_RunKite(struct BattleEffectParams *args) {
  s32 screen[3];
  s32 world[3];

  BattleEffect_StartCanvasLayer(0);
  BattleEffect_RunDjinniEntrance(
      args,
      DJINNI_ENTRANCE(ELEMENT_JUPITER + DJINNI_ENTRANCE_LONG, args->side ^ 1,
                      DJINNI_ANCHOR_OVER_TARGET),
      screen, world);
  BattleEffect_EndCanvasLayer();
}

#undef TORCH_FRAMES
#undef TORCH_FADE_FRAMES
#undef TORCH_STRIKE_FRAME
#undef TORCH_BRIGHTEST_LEVEL
#undef TORCH_CHANNEL_MAX
#undef TORCH_BLUE_MAX
#undef TORCH_SWAY
