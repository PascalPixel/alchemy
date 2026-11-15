/// @file
/// Torch and Kite.
// Part of battle_effect.c.

#include "battle_effect_internal.h"

/// The disc: a 128 x 128 picture of each pixel's distance from the centre,
/// which the 63-colour ramp turns into rings.
#define DISC_SIZE 128
#define DISC_RINGS 63

#define RIPPLE_FRAMES 80
#define RIPPLE_FADE_IN_FRAMES 8
#define RIPPLE_FADE_OUT_FRAME 72
#define RIPPLE_HIT_FRAME 48

/// A disc of rings rises on BG2 over the target's side, its colour ramp
/// cycling outward one ring a frame while each scanline sways on a sine wave.
/// Abilities: Torch.
///
/// Setup: open the canvas, draw the distance map into the sheet and a warm
/// ramp into BG palette 1-63, blit the disc to the canvas, and arm the
/// per-scanline BG2 offsets. Each frame: fade in over the first 8 frames,
/// rebuild the 160 sway offsets, rotate the ramp by one colour, shake the
/// camera. At frame 48 the target is struck; from frame 72 the disc fades.
void BattleEffect_RunTorch(struct BattleEffectParams *args) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 screenX[3];
  s32 screenY[3];
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

  work = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;

  // Open the canvas on BG2 over the target's side.
  BattleEffect_StartCanvasLayer(BGCNT_WRAP);
  BattleEffect_RunDjinniEntrance(
      args,
      DJINNI_ENTRANCE(ELEMENT_MARS + DJINNI_ENTRANCE_LONG, args->side,
                      DJINNI_ANCHOR_OVER_USER),
      screenX, screenY);
  REG_BG2CNT =
      BGCNT_WRAP | BGCNT_SCREEN_BASE(7) | BGCNT_256_COLORS | BGCNT_CHAR_BASE(1);
  REG_BLDALPHA = EFFECT_BLEND_CANVAS(0);
  REG_BG2PA = 0xaa;
  BattleEffect_FetchRectangleBlitters(args->side, draw);
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);

  // The distance map, one quarter at a time mirrored into the other three,
  // the centre an eighth lower than the middle so the disc leans back.
  for (y = 0; y != DISC_SIZE / 2; y++) {
    for (x = 0; x != DISC_SIZE / 2; x++) {
      centerY = y / 8 + DISC_SIZE / 2;
      dy = y - centerY;
      dx = x - DISC_SIZE / 2;
      distance = Iwram_Sqrt(dx * dx + dy * dy);
      distance /= 2;
      if (distance == 0)
        distance = 1;
      if (distance > DISC_RINGS)
        distance = DISC_RINGS;
      work->sheet[y * DISC_SIZE + x] = distance;
      work->sheet[y * DISC_SIZE + DISC_SIZE - 1 - x] = distance;
      work->sheet[(DISC_SIZE - 1 - y) * DISC_SIZE + x] = distance;
      work->sheet[(DISC_SIZE - 1 - y) * DISC_SIZE + DISC_SIZE - 1 - x] =
          distance;
    }
  }

  // The ramp: dark red at the rim, rising to pale yellow at ring 31 and
  // back, kept in the map cell buffer and written to the BG palette.
  ramp = (u16 *)Ram_MapCellBuffer;
  for (i = 1; i != DISC_RINGS + 1; i++) {
    if (i > 31)
      shade = 64 - i;
    else
      shade = i;
    red = shade * 9;
    green = shade * 7 - 42;
    blue = shade * 7 - 56;
    if (red < 0)
      red = 0;
    if (green < 0)
      green = 0;
    if (blue < 0)
      blue = 0;
    if (red > 255)
      red = 255;
    if (green > 255)
      green = 255;
    if (blue > 250)
      blue = 250;
    red >>= 3;
    green >>= 3;
    blue >>= 3;
    ramp[i] = ((u16 *)BG_PLTT)[i] = RGB(red, green, blue);
  }

  // Blit the disc once; from here on only the palette and the scanline
  // offsets move it.
  draw[0](canvas, work->sheet, 0, 0, DISC_SIZE, DISC_SIZE);
  work->transferPending = 1;
  Task_Add(BattleEffect_ArmBg2AffineHBlankDma, TASK_PASS_FRAME);

  for (frame = 0; frame != RIPPLE_FRAMES; frame++) {
    // Fade in, and the sway grows with the frame.
    if (frame <= RIPPLE_FADE_IN_FRAMES) {
      level = frame * 2;
      wave = level;
      REG_BLDALPHA = EFFECT_BLEND_CANVAS(level);
    } else {
      wave = frame * 2;
    }
    if (frame > RIPPLE_FADE_OUT_FRAME)
      REG_BLDALPHA = BLEND_ALPHA(RIPPLE_FRAMES * 2 - frame * 2, 16);

    // The hit: the damage shown, a shake, and the target knocked back.
    if (frame == RIPPLE_HIT_FRAME) {
      EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
      work->shakeFrames = 8;
      BattleMotion_ApplyKnockbackFar(args->targets[0], KNOCKBACK_NUDGE);
    }

    // One BG2 X offset per scanline, a sine wave running down the screen.
    line = work->bg2xLines;
    for (i = 0, angle = -(wave << 9); i != DISPLAY_HEIGHT; i++) {
      *line++ = ((i << 18) - (Trig_Sin(angle) << 7) + 0x40000) >> 10;
      angle += 0x200;
    }

    // Rotate the ramp outward by one ring: the last colour wraps to the
    // first, DMA shifts the rest up, and the queue writes it at VBlank.
    if (frame > 127) {
      work->transferPending = 1;
    } else {
      ramp[1] = ramp[DISC_RINGS];
      Dma_Set(ramp + DISC_RINGS - 1, ramp + DISC_RINGS,
              toFIXED(DMA_ENABLE | DMA_DEST_DEC | DMA_SRC_DEC) | 62, REG_DMA3);
      IO_WRITE_QUEUE_PUSH((u32)&((u16 *)BG_PLTT)[1], (u32)(ramp + 1),
                          DMA_COPY_HALFWORDS(63));
    }
    BattleCamera_Shake(8, 8);
    BattleEffect_CountDownBattlerTints();
    wait(1);
  }

  Task_Remove(BattleEffect_ArmBg2AffineHBlankDma);
  EFFECT_FINISH_CANVAS();
}

/// Abilities: Kite.
void BattleEffect_RunKite(struct BattleEffectParams *args) {
  s32 b[3];
  s32 a[3];

  BattleEffect_StartCanvasLayer(0);
  BattleEffect_RunDjinniEntrance(
      args,
      DJINNI_ENTRANCE(ELEMENT_JUPITER + DJINNI_ENTRANCE_LONG, args->side ^ 1,
                      DJINNI_ANCHOR_OVER_TARGET),
      b, a);
  BattleEffect_EndCanvasLayer();
}
