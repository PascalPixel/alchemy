/// @file
/// Torch and Kite.
// Part of battle_effect.c.

#include "battle_effect_internal.h"

#define TORCH_FRAMES 96

/// The ring level drawn brightest: the levels either side of it dim toward 0
/// and toward the disc's edge.
#define TORCH_BRIGHTEST_LEVEL 32
/// The disc's colours are worked in eight-bit channels, each capped, then cut
/// to the five bits a colour holds; blue's lower cap cuts to the same level.
#define TORCH_CHANNEL_MAX 255
#define TORCH_BLUE_MAX 250

/// A fiery disc fades in, its rings running outward while every line of
/// the display ripples, then fades out.
///
/// Abilities: Torch.
void BattleEffect_RunTorch(struct BattleEffectParams *args) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 screenY;
  s32 screenX;
  DrawRectangle draw[2];
  s32 x;
  s32 y;
  s32 dx;
  s32 dy;
  s32 cy;
  s32 d;
  s32 i;
  s32 t;
  s32 r;
  s32 g;
  s32 b;
  s32 frame;
  s32 wave;
  s32 level;
  u16 *pal;
  s32 angle;
  s32 *line;

  work = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
  canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
  work->args = args;
  BattleEffect_StartCanvasLayer(BGCNT_WRAP);
  BattleEffect_RunDjinniEntrance(args, ELEMENT_MARS + DJINNI_ENTRANCE_LONG,
                                 work->args->side, DJINNI_ANCHOR_OVER_USER,
                                 &screenX, &screenY);
  REG_BG2CNT = BGCNT_PRIORITY(0) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
               BGCNT_SCREEN_BASE(7) | BGCNT_WRAP;
  REG_BLDALPHA = EFFECT_BLEND_CANVAS(0);
  REG_BG2PA = Q_8_8(1.0 / 1.5);
  BattleEffect_FetchRectangleBlitters(work->args->side, draw);
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);

  // The disc: each pixel's level is half its distance from the middle, its
  // rows drawn a little taller; one quarter is worked out and mirrored into
  // the other three.
  for (y = 0; y != CANVAS_SIZE / 2; y++) {
    for (x = 0; x != CANVAS_SIZE / 2; x++) {
      cy = y / 8 + CANVAS_SIZE / 2;
      dy = y - cy;
      dx = x - CANVAS_SIZE / 2;
      d = Iwram_Sqrt(dx * dx + dy * dy);
      d /= 2;
      if (d == 0)
        d = 1;
      if (d > CANVAS_LEVEL_BRIGHTEST)
        d = CANVAS_LEVEL_BRIGHTEST;
      work->sheet[y * CANVAS_SIZE + x] = d;
      work->sheet[y * CANVAS_SIZE + CANVAS_SIZE - 1 - x] = d;
      work->sheet[(CANVAS_SIZE - 1 - y) * CANVAS_SIZE + x] = d;
      work->sheet[(CANVAS_SIZE - 1 - y) * CANVAS_SIZE + CANVAS_SIZE - 1 - x] =
          d;
    }
  }

  // Its colours: red rises from the first level, green from the sixth and blue
  // from the eighth, so the rings run from dark red to pale orange at
  // TORCH_BRIGHTEST_LEVEL.
  pal = (u16 *)Ram_MapGridCellBuffer;
  for (i = 1; i != CANVAS_LEVEL_BRIGHTEST + 1; i++) {
    if (i >= TORCH_BRIGHTEST_LEVEL)
      t = 2 * TORCH_BRIGHTEST_LEVEL - i;
    else
      t = i;
    r = t * 9;
    g = (t - 6) * 7;
    b = (t - 8) * 7;
    if (r < 0)
      r = 0;
    if (g < 0)
      g = 0;
    if (b < 0)
      b = 0;
    if (r > TORCH_CHANNEL_MAX)
      r = TORCH_CHANNEL_MAX;
    if (g > TORCH_CHANNEL_MAX)
      g = TORCH_CHANNEL_MAX;
    if (b > TORCH_BLUE_MAX)
      b = TORCH_BLUE_MAX;
    r >>= 3;
    g >>= 3;
    b >>= 3;
    pal[i] = ((u16 *)BG_PLTT)[i] = RGB(r, g, b);
  }

  draw[0](canvas, work->sheet, 0, 0, CANVAS_SIZE, CANVAS_SIZE);
  work->transferPending = 1;
  Task_Add(BattleEffect_ArmBg2AffineHBlankDma, TASK_PASS_VBLANK);

  // Fade the disc in, ripple the display, then fade it out.
  for (frame = 0; frame != TORCH_FRAMES; frame++) {
    if (frame <= 8) {
      level = frame * 2;
      wave = level;
      REG_BLDALPHA = EFFECT_BLEND_CANVAS(level);
    } else {
      wave = frame * 2;
    }
    if (frame > 88)
      REG_BLDALPHA = EFFECT_BLEND_CANVAS(192 - frame * 2);

    // Each line starts one canvas pixel further along than the one above,
    // swayed by up to 32 either way.
    line = work->bg2xLines;
    for (i = 0, angle = -(wave << 9); i != DISPLAY_HEIGHT; i++) {
      *line++ = ((i << 18) - ((Trig_Sin(angle) * 128)) + toFIXED(4)) >> 10;
      angle += TRIG_TURN / 128;
    }

    // Rotate the colours so the rings run outward.
    // DEAD: never true, as frame stays below TORCH_FRAMES; the ROM keeps the
    // compare and the store.
    if (frame > 127) {
      work->transferPending = 1;
    } else {
      pal[1] = pal[CANVAS_LEVEL_BRIGHTEST];
      Dma_Set(pal + CANVAS_LEVEL_BRIGHTEST - 1, pal + CANVAS_LEVEL_BRIGHTEST,
              DMA_COPY_HALFWORDS(CANVAS_LEVEL_BRIGHTEST - 1) |
                  (DMA_SRC_DEC | DMA_DEST_DEC) << 16,
              REG_DMA3);
      IO_WRITE_QUEUE_PUSH((u32)&BG_PLTT[1], (u32)(pal + 1),
                          DMA_COPY_HALFWORDS(CANVAS_LEVEL_BRIGHTEST));
    }
    wait(1);
  }

  Task_Remove(BattleEffect_ArmBg2AffineHBlankDma);
  EFFECT_FINISH_CANVAS();
}

/// The long Jupiter Djinni entrance over the first target is the whole effect.
///
/// Abilities: Kite.
void BattleEffect_RunKite(struct BattleEffectParams *args) {
  struct BattleEffectWork *work;
  s32 originX;
  s32 originY;

  work = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
  work->args = args;
  BattleEffect_StartCanvasLayer(0);
  BattleEffect_RunDjinniEntrance(args, ELEMENT_JUPITER + DJINNI_ENTRANCE_LONG,
                                 work->args->side ^ 1,
                                 DJINNI_ANCHOR_OVER_TARGET, &originX, &originY);
  BattleEffect_EndCanvasLayer();
}

#undef TORCH_FRAMES
#undef TORCH_BRIGHTEST_LEVEL
#undef TORCH_CHANNEL_MAX
#undef TORCH_BLUE_MAX
