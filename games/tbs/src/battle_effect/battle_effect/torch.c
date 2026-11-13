/// @file
/// Torch and Kite.
// Part of battle_effect.c.

#include "../battle_effect_internal.h"

#define TORCH_FRAMES 96

/// A fiery disc fades in, its rings running outward while every line of
/// the display ripples, then fades out.
///
/// Abilities: Torch (BATTLE_EFFECT_TORCH).
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
  // Invisible until the fade-in; the canvas is shown 1.5 times its width.
  REG_BLDALPHA = BLEND_ALPHA(0, 16);
  REG_BG2PA = Q_8_8(1.0 / 1.5);
  BattleEffect_FetchRectangleBlitters(work->args->side, draw);
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);

  // Fill the 128x128 sheet with each pixel's distance from the middle, halved
  // and held to 1 to 63, so the distance picks the colour.
  for (y = 0; y != 64; y++) {
    for (x = 0; x != 64; x++) {
      cy = y / 8 + 64;
      dy = y - cy;
      dx = x - 64;
      d = Iwram_Sqrt(dx * dx + dy * dy);
      d /= 2;
      if (d == 0)
        d = 1;
      if (d > 63)
        d = 63;
      work->sheet[y * 128 + x] = d;
      work->sheet[y * 128 + 127 - x] = d;
      work->sheet[(127 - y) * 128 + x] = d;
      work->sheet[(127 - y) * 128 + 127 - x] = d;
    }
  }

  // Colours 1 to 63.
  pal = (u16 *)Ram_MapGridCellBuffer;
  for (i = 1; i != 64; i++) {
    if (i > 31)
      t = 64 - i;
    else
      t = i;
    r = t * 9;
    g = t * 7 - 42;
    b = t * 7 - 56;
    if (r < 0)
      r = 0;
    if (g < 0)
      g = 0;
    if (b < 0)
      b = 0;
    if (r > 255)
      r = 255;
    if (g > 255)
      g = 255;
    if (b > 250)
      b = 250;
    r >>= 3;
    g >>= 3;
    b >>= 3;
    pal[i] = ((u16 *)BG_PLTT)[i] = RGB(r, g, b);
  }

  draw[0](canvas, work->sheet, 0, 0, 128, 128);
  work->transferPending = 1;
  Task_Add(BattleEffect_ArmBg2AffineHBlankDma, TASK_PASS_VBLANK);

  // Fade the radial picture in (frames 0 to 8), ripple each line of the
  // display, then fade it out (frames 89 to 95, from 14 sixteenths down to 2).
  for (frame = 0; frame != TORCH_FRAMES; frame++) {
    if (frame <= 8) {
      level = frame * 2;
      wave = level;
      REG_BLDALPHA = BLEND_ALPHA(level, 16);
    } else {
      wave = frame * 2;
    }
    if (frame > 88)
      REG_BLDALPHA = BLEND_ALPHA(192 - frame * 2, 16);

    // BG2's x reference for each line of the display, in 20.8 texels.
    line = work->bg2xLines;
    for (i = 0, angle = -(wave << 9); i != DISPLAY_HEIGHT; i++) {
      *line++ = ((i << 18) - ((Trig_Sin(angle) * 128)) + (4 << 16)) >> 10;
      angle += TRIG_TURN / 128;
    }

    // Rotate colours 2 to 63 up by one, so the rings run outward.
    if (frame > 127) {
      work->transferPending = 1;
    } else {
      pal[1] = pal[63];
      Dma_Set(pal + 62, pal + 63,
              (DMA_ENABLE | DMA_SRC_DEC | DMA_DEST_DEC) << 16 | 62, REG_DMA3);
      IO_WRITE_QUEUE_PUSH((u32)&BG_PLTT[1], (u32)(pal + 1),
                          DMA_COPY_HALFWORDS(63));
    }
    wait(1);
  }

  Task_Remove(BattleEffect_ArmBg2AffineHBlankDma);
  EFFECT_FINISH_CANVAS();
}

/// Shows the long Jupiter Djinni prelude (BattleEffect_RunDjinniEntrance with
/// DJINNI_ENTRANCE_LONG, 84 frames instead of 64) over the first
/// target, mirrored from the usual side; the Djinni's entrance is the whole
/// effect.
///
/// Abilities: Kite (BATTLE_EFFECT_KITE).
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
