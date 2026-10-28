/// @file
/// Torch and Kite.
// Part of battle_effect.c.

#include "../battle_effect_internal.h"

#define TORCH_FRAMES 96

/// A fiery disc fades in, its rings running outward while every scanline
/// ripples, then fades out.
///
/// Abilities: Torch (BATTLE_EFFECT_TORCH).
///
/// The disc is a mirrored radial picture whose pixels hold their distance from
/// the middle, so rotating the palette moves the rings. It follows a Mars
/// Djinni's entrance over the user, fades in over the first 9 frames, ripples
/// throughout as BG2's x reference is fed line by line
/// (BattleEffect_ArmBg2AffineHBlankDma), and fades out over the last 7.
///
/// Opens the canvas layer for the entrance and closes it at the end. Writes
/// background colours 1 to 63 and keeps their working copy in the first 128
/// bytes of Ram_MapGridCellBuffer; adds and removes the HBlank DMA
/// task.
void BattleEffect_RunTorch(struct BattleEffectParams *args) {
  void **slots;
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

  slots = gBattleEffectWork;
  EFFECT_GET_WORK_AND_CANVAS(slots, work, canvas);
  work->args = args;
  BattleEffect_StartCanvasLayer(BGCNT_WRAP);
  // The Mars Djinni's long entrance, 84 frames, over the user.
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

  // Fill the 128x128 sheet with each pixel's distance from the middle,
  // halved and held to 1 to 63, so the distance picks the colour. Rows count
  // 7/8 of a pixel each, so the rings are a little taller than wide. Build
  // one quadrant and mirror it into the other three.
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

  // Colours 1 to 63: a fiery ramp that brightens to colour 32 and darkens
  // again, red first, then green and blue (t is the distance from either
  // end), written to the background palette and to a copy in
  // Ram_MapGridCellBuffer that the frame loop rotates.
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

  // Fade the radial picture in (frames 0 to 8), ripple each scanline, then
  // fade it out (frames 89 to 95, from 14 sixteenths down to 2).
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

    // BG2's x reference for each of the 160 scanlines, in 20.8 texels: line
    // i starts i + 1 texels in, slanting the picture a texel a line, swayed
    // up to 32 texels either way by a sine of one turn every 128 lines whose
    // phase moves 1/64 turn a frame. The HBlank DMA applies it.
    line = work->bg2xLines;
    for (i = 0, angle = -(wave << 9); i != 160; i++) {
      *line++ = ((i << 18) - ((Trig_Sin(angle) * 128)) + (4 << 16)) >> 10;
      angle += TRIG_TURN / 128;
    }

    // Rotate colours 2 to 63 up by one, so the rings run outward: colour 63
    // goes to colour 1 first, and the backward 62-halfword copy then moves 1
    // to 62 up to 2 to 63, leaving colour 1 equal to the new colour 2. Then
    // queue colours 1 to 63 for the background palette. The test against
    // frame 127 never holds within the 96 frames.
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
