// Twin: The Broken Seal's BattleEffect_StartCanvasLayer. The Lost Age's
// changes: BattleEffect_ConfigureEffectDisplay for the display set-up, DISPCNT
// 0x1741, Palette_StepFadeTransfer at 0x480, Func_0814355c for the display
// registers and one more work word cleared.
#include "types.h"
#include "system.h"
#include "io_reg.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "io_write_queue.h"
#include "task.h"
#include "battle_work.h"
#include "battle_display.h"
#include "bg_scroll.h"

/// The battle effect work, as far as the canvas layer reads it.
struct CanvasWork {
  u8 unk0000[0x77a0];
  s32 savedBg1X;
  s32 savedBg1Y;
  s32 shakeFrames;
  u8 unk77ac[8];
  s32 fadeFrames;
  s32 fadeStep;
  u8 unk77bc[0x12c];
  s32 unk78e8;
};

static __inline__ s32 ClearWords_(void *destination, s32 size) {
  return Iwram_ClearWords(destination, size);
}

void BattleEffect_ConfigureEffectDisplay(void);
void BattleEffect_ClearActorTimers(void);
void Func_0814355c(void);
void BattleStaging_ConfigurePaletteFadeFar(s32 mode, s32 background, s32 value);
void BattleCamera_SetViewFar(s32 a, s32 b, s32 c, s32 d);

void BattleEffect_StartCanvasLayer(s32 bgControl) {
  struct CanvasWork *work = (struct CanvasWork *)Ram_HeapSlots->battleFxWork[0];
  struct BattleWork *battle = Ram_HeapSlots->battleWork;
  void *canvas = Ram_HeapSlots->battleFxWork[1];
  struct BattleBackgroundView *display =
      (struct BattleBackgroundView *)Ram_HeapSlots->transitionWork;
  s32 row;
  s32 col;
  s32 offset;

  BattleEffect_ConfigureEffectDisplay();
  BattleEffect_ClearActorTimers();
  display->secondMode = 1;
  wait(1);
  REG_BLDCNT = 0;
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT,
                      DISPCNT_OBJ_ON | DISPCNT_BG2_ON | DISPCNT_BG1_ON |
                          DISPCNT_BG0_ON | DISPCNT_OBJ_1D_MAP | DISPCNT_MODE_1,
                      IO_WRITE_16);
  gBgScroll[1].y = 32;
  wait(1);

  BattleStaging_ConfigurePaletteFadeFar(1, *(u16 *)battle->unk670, 0);
  work->fadeFrames = 0;
  work->fadeStep = 0;
  Task_Add(Palette_StepFadeTransfer, TASK_PASS_VBLANK);

  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT,
                      DISPCNT_WIN1_ON | DISPCNT_WIN0_ON | DISPCNT_OBJ_ON |
                          DISPCNT_BG1_ON | DISPCNT_BG0_ON | DISPCNT_OBJ_1D_MAP |
                          DISPCNT_MODE_1,
                      IO_WRITE_16);
  wait(1);
  REG_BG2CNT =
      bgControl | BGCNT_SCREEN_BASE(7) | BGCNT_256_COLORS | BGCNT_CHAR_BASE(1);
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT,
                      DISPCNT_WIN1_ON | DISPCNT_WIN0_ON | DISPCNT_OBJ_ON |
                          DISPCNT_BG1_ON | DISPCNT_BG0_ON | DISPCNT_OBJ_1D_MAP |
                          DISPCNT_MODE_1,
                      IO_WRITE_16);
  BattleCamera_SetViewFar(0, 0, 0, 100);
  display->secondMode = 0;
  wait(1);

  Func_0814355c();
  REG_WININ = WININ_WIN1_CLR | WININ_WIN1_OBJ | WININ_WIN1_BG2 |
              WININ_WIN1_BG0 | WININ_WIN0_CLR | WININ_WIN0_OBJ |
              WININ_WIN0_BG2 | WININ_WIN0_BG1 | WININ_WIN0_BG0;
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT,
                      DISPCNT_WIN1_ON | DISPCNT_WIN0_ON | DISPCNT_OBJ_ON |
                          DISPCNT_BG2_ON | DISPCNT_BG1_ON | DISPCNT_BG0_ON |
                          DISPCNT_OBJ_1D_MAP | DISPCNT_MODE_1,
                      IO_WRITE_16);

  offset = 0;
  for (row = 0; row != 16; row++) {
    for (col = 0; col != 8; col++, offset += 2) {
      s32 chr;
      s16 pair;

      chr = row * 16 + col * 2;
      pair = ((chr + 1) << 8) | chr;
      *(volatile s16 *)((u8 *)BG_CANVAS_MAP + offset) = pair;
    }
  }

  ClearWords_(canvas, 0x4000);
  ClearWords_(BG_CHAR_BLOCK(1), 0x4000);
  work->shakeFrames = 0;
  work->unk78e8 = 0;
  work->savedBg1X = gBgScroll[1].x;
  work->savedBg1Y = gBgScroll[1].y;
  wait(1);
}

extern u32 gCameraSceneParameters[];

void Sound_PlayFar(s32 cue);

void BattleBackground_SetPaletteLevelFar(s32 background, s32 level);

/// Opens the effect canvas layer: BG2 becomes a 2x-wide 128x128 canvas of
/// 16x16 characters, blended over the battle background, whose palette
/// starts fading.
void BattleEffect_OpenCanvasLayer(s32 bgControl) {
  struct CanvasWork *work = (struct CanvasWork *)Ram_HeapSlots->battleFxWork[0];
  struct BattleWork *battle = Ram_HeapSlots->battleWork;
  void *canvas = Ram_HeapSlots->battleFxWork[1];
  struct BattleBackgroundView *display =
      (struct BattleBackgroundView *)Ram_HeapSlots->transitionWork;
  s32 row;
  s32 col;
  s32 offset;

  BattleEffect_ClearActorTimers();
  display->secondMode = 1;
  wait(1);
  REG_BLDCNT = 0;
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT,
                      DISPCNT_OBJ_ON | DISPCNT_BG2_ON | DISPCNT_BG1_ON |
                          DISPCNT_BG0_ON | DISPCNT_OBJ_1D_MAP | DISPCNT_MODE_1,
                      IO_WRITE_16);
  gBgScroll[1].y = 32;
  wait(1);
  BattleStaging_ConfigurePaletteFadeFar(1, *(u16 *)battle->unk670, 128);
  work->fadeFrames = 24;
  work->fadeStep = 0;
  Task_Add(Palette_StepFadeTransfer, TASK_PASS_VBLANK);
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT,
                      DISPCNT_OBJ_ON | DISPCNT_BG1_ON | DISPCNT_BG0_ON |
                          DISPCNT_OBJ_1D_MAP | DISPCNT_MODE_1,
                      IO_WRITE_16);
  wait(1);
  REG_BG2CNT =
      bgControl | BGCNT_SCREEN_BASE(7) | BGCNT_256_COLORS | BGCNT_CHAR_BASE(1);
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT,
                      DISPCNT_OBJ_ON | DISPCNT_BG1_ON | DISPCNT_BG0_ON |
                          DISPCNT_OBJ_1D_MAP | DISPCNT_MODE_1,
                      IO_WRITE_16);
  BattleCamera_SetViewFar(0, 0, 0, 100);
  display->secondMode = 0;
  wait(1);

  Func_0814355c();
  REG_WININ = WININ_WIN1_CLR | WININ_WIN1_OBJ | WININ_WIN1_BG2 |
              WININ_WIN1_BG0 | WININ_WIN0_CLR | WININ_WIN0_OBJ |
              WININ_WIN0_BG2 | WININ_WIN0_BG1 | WININ_WIN0_BG0;
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT,
                      DISPCNT_WIN1_ON | DISPCNT_WIN0_ON | DISPCNT_OBJ_ON |
                          DISPCNT_BG2_ON | DISPCNT_BG1_ON | DISPCNT_BG0_ON |
                          DISPCNT_OBJ_1D_MAP | DISPCNT_MODE_1,
                      IO_WRITE_16);
  wait(1);

  // The canvas map: 16 rows of 8 character pairs.
  offset = 0;
  for (row = 0; row != 16; row++) {
    for (col = 0; col != 8; col++, offset += 2) {
      s32 chr;
      s16 pair;

      chr = row * 16 + col * 2;
      pair = ((chr + 1) << 8) | chr;
      *(volatile s16 *)((u8 *)BG_CANVAS_MAP + offset) = pair;
    }
  }

  /* FAKEMATCH(wrapper): the clear forwarded through an inline, which keeps each
   * call's own constants and the routine in a callee-saved register; as direct
   * Iwram_ClearWords calls, 24 of 259 instructions of
   * BattleEffect_OpenCanvasLayer differ in tla-en. */
  ClearWords_(canvas, 0x4000);
  ClearWords_(BG_CHAR_BLOCK(1), 0x4000);
  work->shakeFrames = 0;
  work->unk78e8 = 0;
  wait(1);
}

void BattleEffect_StartTiledCanvas(s32 bg_control) {
  s32 row;
  s32 col;
  s32 offset;

  BattleEffect_StartCanvasLayer(bg_control);
  REG_BG2CNT = bg_control | BGCNT_SIZE(1) | BGCNT_SCREEN_BASE(7) |
               BGCNT_256_COLORS | BGCNT_CHAR_BASE(1);
  offset = 0;
  for (row = 0; row != 16; row++) {
    for (col = 0; col != 8; col++, offset += 2) {
      s32 tile;
      s16 pair;

      tile = row * 16 + col * 2;
      pair = ((tile + 1) << 8) | tile;
      *(volatile s16 *)(0x06003800 + offset) = pair;
    }
    for (col = 0; col != 8; col++, offset += 2) {
      *(volatile u16 *)(0x06003800 + offset) = 0;
    }
  }
}

void BattleEffect_StartTiledCanvasFilled(s32 bg_control) {
  s32 row;
  s32 col;
  s32 offset;

  BattleEffect_StartCanvasLayer(bg_control);
  REG_BG2CNT = bg_control | BGCNT_SIZE(1) | BGCNT_SCREEN_BASE(7) |
               BGCNT_256_COLORS | BGCNT_CHAR_BASE(1);
  offset = 0;
  for (row = 0; row != 16; row++) {
    for (col = 0; col != 8; col++, offset += 2) {
      s32 tile;
      s16 pair;

      tile = row * 16 + col * 2;
      pair = ((tile + 1) << 8) | tile;
      *(volatile s16 *)(0x06003800 + offset) = pair;
    }
    for (col = 0; col != 8; col++, offset += 2) {
      *(volatile u16 *)(0x06003800 + offset) = 0xf0f;
    }
  }
}

/// Closes the effect canvas layer: BG1 back where it was, BG2 and its
/// characters cleared, then the background palette faded back in.
void BattleEffect_EndCanvasLayer(void) {
  struct CanvasWork *work = (struct CanvasWork *)Ram_HeapSlots->battleFxWork[0];
  struct BattleWork *battle = Ram_HeapSlots->battleWork;
  s32 i;

  Sound_PlayFar(390);
  gBgScroll[1].x = work->savedBg1X;
  gBgScroll[1].y = work->savedBg1Y;
  gCameraSceneParameters[3] = 120;
  gCameraSceneParameters[4] = 120;
  REG_BG2CNT = BGCNT_SCREEN_BASE(7) | BGCNT_256_COLORS | BGCNT_CHAR_BASE(1) |
               BGCNT_PRIORITY(3);
  Iwram_ClearWords(BG_CHAR_BLOCK(1), 0x4000);
  Task_Remove(Palette_StepFadeTransfer);
  gBgScroll[1].y = 32;
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT,
                      DISPCNT_WIN1_ON | DISPCNT_WIN0_ON | DISPCNT_OBJ_ON |
                          DISPCNT_BG1_ON | DISPCNT_BG0_ON | DISPCNT_OBJ_1D_MAP |
                          DISPCNT_MODE_1,
                      IO_WRITE_16);
  REG_BLDCNT = 0;
  wait(1);

  BattleStaging_ConfigurePaletteFadeFar(2, *(u16 *)battle->unk670, 7);
  wait(1);
  for (i = 0; i != 8; i++) {
    BattleBackground_SetPaletteLevelFar(*(u16 *)battle->unk670, 21 - i * 3);
    wait(1);
  }

  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT,
                      DISPCNT_WIN1_ON | DISPCNT_WIN0_ON | DISPCNT_OBJ_ON |
                          DISPCNT_BG2_ON | DISPCNT_BG0_ON | DISPCNT_OBJ_1D_MAP |
                          DISPCNT_MODE_1,
                      IO_WRITE_16);
  wait(1);
}

/// Hides the canvas layer: marks the background view, queues DISPCNT 0x1541,
/// fades the background palette back and queues BG1's control word.
void BattleEffect_HideBg1AndRestoreBackdrop(void) {
  struct BattleWork *battle;
  s32 *flag;

  flag = &((struct BattleBackgroundView *)Ram_HeapSlots->transitionWork)
              ->secondMode;
  battle = Ram_HeapSlots->battleWork;
  *flag = 1;
  QueueIoWriteDelay2((u32)&REG_DISPCNT,
                     DISPCNT_OBJ_ON | DISPCNT_BG2_ON | DISPCNT_BG0_ON |
                         DISPCNT_OBJ_1D_MAP | DISPCNT_MODE_1);
  wait(1);
  BattleStaging_ConfigurePaletteFadeFar(2, *(u16 *)battle->unk670, 0);
  IO_WRITE_QUEUE_PUSH((u32)&REG_BG1CNT,
                      BGCNT_SCREEN_BASE(31) | BGCNT_256_COLORS |
                          BGCNT_PRIORITY(3),
                      IO_WRITE_16);
  wait(1);
}

static __inline__ s32 FillWords_(void *destination, s32 size, u32 value) {
  return Iwram_FillWords(destination, size, value);
}

void Func_08143d80(void) {
  void *canvas = Ram_HeapSlots->battleFxWork[1];
  struct BattleWork *screen = Ram_HeapSlots->battleWork;
  struct BattleBackgroundView *layer =
      (struct BattleBackgroundView *)Ram_HeapSlots->transitionWork;
  s32 row;
  s32 col;
  s32 off;

  BattleEffect_ClearActorTimers();
  BattleStaging_ConfigurePaletteFadeFar(2, *(u16 *)screen->unk670, 0);
  gBgScroll[1].y = 32;
  layer->secondMode = 0;
  ClearWords_((void *)0x06003fc0, 64);
  FillWords_((void *)0x0600f900, 0x200, -1);
  off = 0;
  for (row = 0; row != 16; row++) {
    for (col = 0; col != 32; col++, off += 2) {
      if (col > 15)
        *(volatile u16 *)(0x0600fb00 + off) = 0xff;
      else
        *(volatile u16 *)(0x0600fb00 + off) = row * 16 + 0x100 + col;
    }
  }
  REG_DISPCNT = DISPCNT_WIN1_ON | DISPCNT_WIN0_ON | DISPCNT_OBJ_ON |
                DISPCNT_BG2_ON | DISPCNT_BG1_ON | DISPCNT_BG0_ON |
                DISPCNT_OBJ_1D_MAP | DISPCNT_MODE_1;
  REG_BG1CNT = BGCNT_SCREEN_BASE(31) | BGCNT_256_COLORS | BGCNT_PRIORITY(1);
  REG_BLDCNT = BLDCNT_TGT1_BG1 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  REG_WIN0H = WIN_RANGE(0, 240);
  REG_WIN0V = WIN_RANGE(16, 136);
  REG_WIN1H = WIN_RANGE(0, 240);
  REG_WIN1V = WIN_RANGE(16, 136);
  REG_WININ = WININ_WIN1_CLR | WININ_WIN1_OBJ | WININ_WIN1_BG2 |
              WININ_WIN1_BG0 | WININ_WIN0_CLR | WININ_WIN0_OBJ |
              WININ_WIN0_BG2 | WININ_WIN0_BG1 | WININ_WIN0_BG0;
  REG_WINOUT = WINOUT_WINOBJ_ALL | WINOUT_WIN01_CLR | WINOUT_WIN01_BG0;
  REG_BLDALPHA = BLEND_ALPHA(14, 16);
  ClearWords_(canvas, 0x4000);
  ClearWords_(BG_CHAR_BLOCK(1), 0x4000);
  wait(1);
}

#define ABS(v) ((v) < 0 ? -(v) : (v))

/// The Broken Seal's BattleEffect_DrawClippedCanvasLine, with the fraction in
/// bit 31 (started at 0x80 << 23, carried when negative and cleared with a
/// shift pair) and dx and dy reused as the step direction.
void Func_08143eb4(s32 x0, s32 y0, s32 x1, s32 y1, s32 color) {
  s32 dx = x1 - x0;
  s32 dy = y1 - y0;
  s32 frac = 0x80 << 23;
  u8 *canvas = Ram_HeapSlots->battleFxWork[1];
  s32 step;
  s32 i;
  s32 x;
  s32 y;
  s32 tmp;
  u32 offset;

  if (y0 < 0)
    y0 = 0;
  if (y0 > 127)
    y0 = 127;
  if (y1 < 0)
    y1 = 0;
  if (y1 > 127)
    y1 = 127;

  if (ABS(dx) < ABS(dy)) {
    if (dy < 0) {
      tmp = x0;
      x0 = x1;
      x1 = tmp;
      tmp = y0;
      y0 = y1;
      y1 = tmp;
      dx = x1 - x0;
      dy = y1 - y0;
    }
    step = (ABS(x1 - x0) << 8) / ABS(y1 - y0);
    step <<= 23;
    x = x0;
    if (dx < 0)
      dx = -1;
    else
      dx = 1;
    for (i = y0; i != y1; i++) {
      offset = (((((u32)i >> 3) << 4) + ((u32)x >> 3)) << 3) + (i & 7);
      offset = (offset << 3) + (x & 7);
      if (canvas[offset] < color)
        canvas[offset] = color;
      frac += step;
      if (frac < 0) {
        x += dx;
        frac = (u32)(frac << 1) >> 1;
      }
    }
  } else {
    if (dx < 0) {
      tmp = x0;
      x0 = x1;
      x1 = tmp;
      tmp = y0;
      y0 = y1;
      y1 = tmp;
      dx = x1 - x0;
      dy = y1 - y0;
    }
    step = (ABS(y1 - y0) << 8) / ABS(x1 - x0);
    step <<= 23;
    y = y0;
    if (dy < 0)
      dy = -1;
    else
      dy = 1;
    for (i = x0; i != x1; i++) {
      offset = (((((u32)y >> 3) << 4) + ((u32)i >> 3)) << 3) + (y & 7);
      offset = (offset << 3) + (i & 7);
      if (canvas[offset] < color)
        canvas[offset] = color;
      frac += step;
      if (frac < 0) {
        y += dy;
        frac = (u32)(frac << 1) >> 1;
      }
    }
  }
}

#include "battle.h"
#include "graphics.h"
#include "lib.h"
#include "resource.h"
#include "sound.h"

#include "battle_effect.h"
#include "battle_effect_work.h"
#include "effect_step.h"

#include "motobj.h"

// The skulls live in the map buffer, 64 for each target.
#define PARTICLES ((struct EffectStep *)Ram_MapCellBuffer)

// The canvas routines every effect shares, with their Broken Seal names.
void BattleEffect_StartCanvasLayer(s32 bgControl);
void BattleEffect_ProcessPendingGraphicsTransfer(
    void); // ..ProcessPendingGraphicsTransfer
void BattleEffect_CountDownBattlerTints(void);
void Func_081180e8(s32 phase); // BattlePlayback_StartFar
void Func_0814cd48(s32 actor, s32 palette, s32 duration, s32 timer,
                   s32 frames); // BattleEffect_TintBattler
void BattleEffect_FetchRectangleBlitters(
    s32 flip, void **out); // ..FetchRectangleBlitters
void SceneTransform_ApplyPosition(const s32 *pos);
void SceneTransform_ApplyPitch(s32 angle);

extern char ResourceId_SkullSheet;
extern char ResourceId_PinkBurstSheet;

#define SKULLS 8 // per target
#define SKULL_SIZE 24
#define SKULL_CELL (24 * 24)

// Closing skulls: for each target in turn, 32 frames apart, eight skulls
// appear one every four frames from all around, up to 64 pixels away, and
// close in on a point 20 pixels above the target while the view of them
// turns; the background waves line by line throughout.
void BattleEffect_RunHaunt(struct BattleEffectArgument *effect) {
  s32 k;
  struct EffectPosition pos;
  s32 point[3];
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  DrawRectangle draw[2];
  s32 *camera;
  s32 i;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];

  // Set up: the skull sheet with its palette, the pink burst palette, the
  // max and additive blitters, the wave's HBlank DMA and a transfer that
  // fades the canvas by 4 each frame.
  BattleEffect_StartCanvasLayer(0);
  Resource_LoadAndDecompress((s32)&ResourceId_SkullSheet, work->sheet, 1, 1);
  Iwram_CopyWords((void *)BG_PLTT,
                  Resource_GetTableEntry((s32)&ResourceId_PinkBurstSheet), 128);
  BattleEffect_FetchRectangleBlitters(0, draw);
  Task_Add(BattleEffect_ArmBg2AffineHBlankDma, 0xc80);
  work->transferMode = 3;
  work->transferValue = 0x04040404;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);

  // Scatter every skull up to 64 pixels from its centre.
  for (i = 0; i != 512; i++) {
    PARTICLES[i].x = ((Random16() & 255) - 127) << 15;
    PARTICLES[i].y = ((Random16() & 255) - 127) << 15;
    PARTICLES[i].z = ((Random16() & 255) - 127) << 15;
  }
  Sound_PlayFar(142);

  for (frame = 0; frame != effect->count * 32 + 64; frame++) {
    s32 *row;

    camera = Ram_HeapSlots->cameraWork;
    if (frame == 92)
      Func_081180e8(0);

    // The background waves sideways, one sine step per line.
    row = work->bg2xLines;
    if (effect->side == 0) {
      for (i = 0; i != 160; i++)
        *row++ = (0x60000 - Trig_Sin((frame + i) << 11) * 6) >> 10;
    } else {
      for (i = 0; i != 160; i++)
        *row++ = Trig_Sin((frame + i) << 11) * 6 >> 10;
    }

    for (k = 0; k != effect->count; k++) {
      struct MotionObject *target =
          GetBattleObjectSlotFar(effect->actors[k])->object;

      // Each target's skulls turn about a point 20 pixels above it.
      Render_ResetTransformState();
      // The look-at call in a once-only block, so the eye loads before the
      // point it looks at; as the plain call, 2 of 355 instructions differ in
      // tla-en.
      do {
        Transform_SetLookAt(camera, camera + 3);
      } while (0);
      point[0] = target->x;
      point[1] = 20 << 16;
      point[2] = target->z;
      SceneTransform_ApplyPosition(point);
      if (frame > k * 32) {
        SceneTransform_ApplyPitch(frame << 9);
        if (frame == k * 32 + 32)
          Func_0814cd48(effect->actors[k], 7, 5, k, 32);

        // A skull moves one part in its distance toward the centre each
        // frame, and is gone once it arrives.
        for (i = 0; i != SKULLS; i++) {
          struct EffectStep *skull = &PARTICLES[k * 64 + i];

          if (frame > (k * 8 + i) * 4) {
            s32 x = (skull->x >> 8) * (skull->x >> 8);
            s32 y = (skull->y >> 8) * (skull->y >> 8);
            s32 z = (skull->z >> 8) * (skull->z >> 8);
            s32 dist = Iwram_Sqrt(x + y + z) >> 8;

            if (dist != 0) {
              EffectPosition_ProjectPoint((s32 *)skull, &pos);
              pos.x >>= 1;
              draw[0](canvas, work->sheet + i % 3 * SKULL_CELL,
                      pos.x - SKULL_SIZE / 2, pos.y - SKULL_SIZE / 2,
                      SKULL_SIZE, SKULL_SIZE);
              skull->x -= skull->x / dist;
              skull->y -= skull->y / dist;
              skull->z -= skull->z / dist;
              skull->variant++;
            }
          }
        }
      }
    }
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  Task_Remove(BattleEffect_ArmBg2AffineHBlankDma);
  HeapSlot_Release(188);
  HeapSlot_Release(104);
  BattleEffect_EndCanvasLayer();
}

// Heap blocks are named by their byte offset in Ram_HeapSlots.
#define HEAP_OFFSET(field) ((s32)&((struct HeapSlots *)0)->field)
#define HEAP_BLITTER HEAP_OFFSET(unk64[1])
#define HEAP_BLITTER_ALTERNATE HEAP_OFFSET(unkb8[1])

// The blitter generator's modes, The Broken Seal's in reverse order.
enum { BLIT_ADDITIVE, BLIT_MAX, BLIT_TRANSPARENT, BLIT_COPY };
#define BLITTER(mode, flags) ((mode) << 4 | (flags))
#define CLIP (COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y)

s32 BattleEffect_LoadBlitter(s32 slot,
                             s32 modeAndFlags); // BattleEffect_BuildBlitter

extern char ResourceId_SpiralSheet;

#define SPIRAL_SIZE 32
#define SPIRAL_CELS 4

// Member orbit: over each target in turn, sixteen frames apart, four
// spirals circle a point 40 pixels above it for 60 frames, the blitter
// flipping every sixteen frames; the background waves line by line.
void BattleEffect_RunOrbitingSpirals(struct BattleEffectArgument *effect) {
  struct BattleEffectWork *work;
  void *canvas;
  DrawRectangle draw[2];
  s32 point[3];
  struct EffectPosition screen;
  s32 member;
  s32 offsetX;
  s32 frame;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];

  // Set up: the spiral sheet with its palette, the two blitters, the wave's
  // HBlank DMA and the transfer at half strength.
  BattleEffect_StartCanvasLayer(0);
  REG_BG2PA = 0x100;
  Resource_LoadAndDecompress((s32)&ResourceId_SpiralSheet, work->sheet, 1, 1);
  BattleEffect_LoadBlitter(HEAP_BLITTER, BLITTER(BLIT_MAX, CLIP));
  draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  BattleEffect_LoadBlitter(HEAP_BLITTER_ALTERNATE,
                           BLITTER(BLIT_MAX, CLIP | COMPOSITE_FLIP));
  draw[1] = (DrawRectangle)Ram_HeapSlots->unkb8[1];
  Task_Add(BattleEffect_ArmBg2AffineHBlankDma, 0xc80);
  work->transferMode = 2;
  work->transferValue = 50;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);

  // On the far side the background and the spirals move 112 pixels left.
  if (effect->side == 1) {
    REG_BG2X = -0x6800;
    offsetX = -112;
  } else {
    offsetX = 0;
  }

  for (frame = 0; frame != (effect->count * 16) + 48; frame++) {
    s32 *camera;
    s32 *row;
    s32 i;

    // The background waves sideways line by line.
    camera = Ram_HeapSlots->cameraWork;
    row = work->bg2xLines;
    if (effect->side == 0) {
      s32 angle;
      s32 base;

      for (i = 0, base = 0x80000, angle = frame << 10; i != 160; i++) {
        *row++ = (base - (Trig_Sin(angle) << 3)) >> 10;
        angle += 1024;
      }
    } else {
      s32 angle;

      for (i = 0, angle = frame << 10; i != 160; i++) {
        *row++ = ((Trig_Sin(angle) << 3) >> 10) - 0x7000;
        angle += 1024;
      }
    }

    Render_ResetTransformState();
    Transform_SetLookAt(camera, camera + 3);
    member = 0;
    if (effect->count != 0) {
      while (member != effect->count) {
        struct MotionObject *target;

        target = GetBattleObjectSlotFar(effect->actors[member])->object;
        if (frame > member * 16 && frame < (member * 16) + 60) {
          s32 spin;

          // Halfway through, the target's flash stops.
          if (frame == (member * 16) + 32)
            Func_0814cd48(effect->actors[member], 0, 5, -1, 0);
          point[0] = target->x;
          point[1] = 40 << 16;
          point[2] = target->z;
          EffectPosition_ProjectPoint(point, &screen);

          // The four spirals, a quarter turn apart, each a 32 x 32 cel of
          // the sheet's four-frame animation.
          for (i = 0; i != SPIRAL_CELS; i++) {
            s32 x;
            s32 y;
            s32 half;

            spin = (frame + i * 32) << 9;
            x = (screen.x + ((Trig_Sin(spin) << 4) >> 16)) + offsetX;
            y = screen.y + ((Trig_Cos(spin) << 4) >> 16);
            half = frame / 16;
            draw[half & 1](canvas,
                           work->sheet + (((frame / 4) - (half * 4)) << 10),
                           x - SPIRAL_SIZE / 2, y - SPIRAL_SIZE / 2,
                           SPIRAL_SIZE, SPIRAL_SIZE);
          }
        }
        member++;
      }
    }
    work->transferPending = 1;
    wait(1);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  Task_Remove(BattleEffect_ArmBg2AffineHBlankDma);
  HeapSlot_Release(HEAP_BLITTER_ALTERNATE);
  HeapSlot_Release(HEAP_BLITTER);
  BattleEffect_EndCanvasLayer();
}

#include "battle_effect_argument.h"

void Func_08144628(struct BattleEffectArgument *effect, s32 mode);

/// Abilities: Demon Night (kind 50).
DEFINE_EFFECT_KIND(BattleEffect_RunDemonNight, Func_08144628, 0)

/// Abilities: Rumble (kind 308).
DEFINE_EFFECT_KIND(BattleEffect_RunRumble, Func_08144628, 1)

/// Abilities: Haunting (kind 351).
DEFINE_EFFECT_KIND(BattleEffect_RunHaunting, Func_08144628, 2)

/// Abilities: Thorny Grave (kind 127).
DEFINE_EFFECT_KIND(BattleEffect_RunThornyGrave, Func_08144628, 3)

void BattleEffect_ArmBg1HBlankDma(void); // arms the BG1 scroll's HBlank DMA
void Func_0815b634(void);                // mode 3 only
void BattleEffectKernels_IntegrateVector3(struct EffectStep *step, s32 damping,
                                          s32 gravity);
s32 BattleUnit_GetEffectHeightFar(s32 unit);
void BattleEffect_StartActorMotion(s32 id, s32 force, s32 acceleration,
                                   s32 speedLimit, s32 velocityY, s32 percent);

extern char ResourceId_MemberBurstImage;

extern char ResourceId_MarsDjinnSheet;
extern char ResourceId_MercuryDjinnSheet;
// Per mode: how many motes each target gets, and the effect's frame count
// before the targets' share.
extern u8 Data_08197864[];

#define MOTES_PER_TARGET 128
#define MOTE_CELL (20 * 32)

// Member burst (kinds through four wrappers; mode picks the colours): from
// the caster's body, 128 motes per target scatter every way, and from 30
// frames on each is pulled toward the middle of its target's body, vanishing
// once it has crossed it. The background sways sideways, swelling and dying
// away over the effect. In mode 3 every mote that arrives also pushes its
// target back.
void Func_08144628(struct BattleEffectArgument *effect, s32 mode) {
  struct EffectPosition pos;
  DrawRectangle draw[2];
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  s32 k;
  s32 *camera;
  s32 palette;
  s32 i;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  camera = Ram_HeapSlots->cameraWork;

  // Set up: the mote picture, the mode's palette, and the max and additive
  // blitters for the caster's side.
  BattleEffect_StartCanvasLayer(1);
  Resource_LoadAndDecompress((s32)&ResourceId_MemberBurstImage, work->sheet, 0,
                             0);
  if (mode == 0 || mode == 3)
    palette = (s32)&ResourceId_PinkBurstSheet;
  else if (mode == 1)
    palette = (s32)&ResourceId_MarsDjinnSheet;
  else
    palette = (s32)&ResourceId_MercuryDjinnSheet;
  // The entry held in a local loads before the arguments; as a plain
  // Iwram_CopyWords call, 2 of 511 instructions differ in tla-en.
  {
    const void *source_ = Resource_GetTableEntry(palette);
    __typeof__(Iwram_CopyWords) routine = Iwram_CopyWords;
    routine((void *)BG_PLTT, source_, 128);
  }
  if (mode == 3)
    Func_0815b634();
  BattleEffect_FetchRectangleBlitters(effect->side, draw);

  // The motes start at the caster's body height and scatter every way at up
  // to two pixels a frame.
  for (i = 0; i != 1024; i++)
    PARTICLES[i].variant = -1;
  for (k = 0; k != effect->count; k++) {
    struct MotionObject *source = GetBattleObjectSlotFar(effect->actor)->object;
    s32 height = BattleUnit_GetEffectHeightFar(effect->actor);

    for (i = 0; i != MOTES_PER_TARGET; i++) {
      struct EffectStep *mote = &PARTICLES[k * 128 + i];

      mote->x = source->x;
      mote->y = height;
      mote->z = source->z;
      mote->velocityX = ((Random16() & 255) - 128) << 10;
      mote->velocityY = ((Random16() & 255) - 128) << 10;
      mote->velocityZ = ((Random16() & 255) - 128) << 10;
      mote->variant = 0;
    }
  }

  Task_Add(BattleEffect_ArmBg1HBlankDma, 0xc80);
  work->transferMode = 2;
  work->transferValue = 75;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);
  Sound_PlayFar(146);

  for (frame = 0; frame != Data_08197864[mode * 2 + 1] + effect->count * 20;
       frame++) {
    s32 *row;
    s32 sway;
    s32 scrollX;

    if (frame == 80) {
      if (mode == 0)
        Func_081180e8(134);
      else if (mode == 3)
        Func_081180e8(134);
      else
        Func_081180e8(133);
    }
    Render_ResetTransformState();
    Transform_SetLookAt(camera, camera + 3);

    // BG1 sways line by line, by up to 16 pixels at the effect's midpoint.
    // A negative scroll wraps into the halfword one short of 0x10000.
    /* FAKEMATCH(barrier): an empty asm after the look-at call keeps its
     * arguments loaded in order, r0 before r1; without it, 2 of 511
     * instructions differ in tla-en. */
    __asm__("");
    row = work->bg2xLines;
    sway = Trig_Sin((frame << 15) /
                    (Data_08197864[mode * 2 + 1] + effect->count * 20)) *
               16 >>
           16;
    scrollX = gBgScroll[1].x;
    for (i = 0; i != 160; i++) {
      s32 x = (Trig_Sin((frame + i) << 10) * sway >> 16) + scrollX;

      if (x < 0)
        x += 0xffff;
      *row++ = x | 32 << 16;
    }

    for (k = 0; k != effect->count; k++) {
      struct MotionObject *target =
          GetBattleObjectSlotFar(effect->actors[k])->object;
      s32 half = BattleUnit_GetEffectHeightFar(effect->actors[k]) / 2;

      if (frame == k * 20 + 71) {
        if (mode == 0)
          Sound_PlayFar(134);
        else if (mode != 3)
          Sound_PlayFar(133);
      }
      if (frame == k * 20 + 70)
        Func_0814cd48(effect->actors[k], 7, 5, k, 26);
      if (frame > k * 20) {
        for (i = 0; i != Data_08197864[mode * 2]; i++) {
          struct EffectStep *mote = &PARTICLES[k * 128 + i];

          if (frame > (k * 10 + i) * 2 && mote->variant >= 0) {
            EffectPosition_ProjectPoint((s32 *)mote, &pos);
            pos.x >>= 1;
            draw[0](canvas, work->sheet + i % 3 * MOTE_CELL, pos.x - 10,
                    pos.y - 16, 20, 32);
            BattleEffectKernels_IntegrateVector3(mote, 62, 0);

            // Thirty frames on, the mote is pulled toward the middle of its
            // target's body.
            if (frame > k * 20 + i + 30) {
              s32 x = mote->x;
              s32 dx = (target->x - x) >> 8;
              s32 dy = (target->y + half - mote->y) >> 9;
              s32 dz = (target->z - mote->z) >> 8;
              s32 targetDistance;
              s32 moteDistance;
              s32 crossed;

              mote->velocityX += dx;
              mote->velocityY += dy;
              mote->velocityZ += dz;
              // It has crossed once it is as far out from the middle as its
              // target is, on the target's side.
              targetDistance = target->x < 0 ? -target->x : target->x;
              moteDistance = x;
              /* FAKEMATCH(barrier): an empty asm through moteDistance keeps it
               * a copy apart from x, so the sign test and the negation read the
               * copy; as x < 0 ? -x : x, 4 of 511 instructions differ in
               * tla-en. */
              __asm__("" : "=r"(moteDistance) : "0"(moteDistance));
              if (moteDistance < 0)
                moteDistance = -moteDistance;
              /* FAKEMATCH(dead): each arm stores its test in crossed, never
               * read; as the tests without the stores, 81 of 511 instructions
               * differ in tla-en. */
              if (targetDistance <= moteDistance &&
                  ((x < 0 && (crossed = target->x < 0)) ||
                   (x >= 0 && (crossed = target->x >= 0)))) {
                mote->variant = -1;
                if (mode == 3) {
                  BattleEffect_StartActorMotion(effect->actors[k], 1, 0x20000,
                                                0x20000, 0x20000, i * 8 + 100);
                  if ((i & 1) && frame != 80)
                    Sound_PlayFar(134);
                }
              }
            }
          }
        }
      }
    }
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(188);
  HeapSlot_Release(104);
  Task_Remove(BattleEffect_ArmBg1HBlankDma);
  BattleEffect_EndCanvasLayer();
}

/// Builds the two usual routines that draw onto the 128x128 canvas, clipped,
/// mirrored when flip is nonzero. out[0] keeps the higher colour index (heap
/// slot 26), out[1] adds the colours (slot 47). BattleEffect_LoadBlitter takes
/// each slot's heap offset and the blitter flags.
// The Broken Seal's BattleEffect_FetchRectangleBlitters.
void BattleEffect_FetchRectangleBlitters(s32 flip, void **out) {
  // Unused: gives the stack frame its 4 bytes.
  UNUSED u8 unused[4];

  if (flip == 0) {
    u32 heap = 0xc0;
    s32 flags = 19;
    void **slots;

    /* FAKEMATCH(barrier): an empty asm on heap and flags, so both are set
     * before the heap base is shifted and the slot offset built; without it,
     * 2 of 30 instructions differ in tla-en. */
    __asm__("" : "+l"(heap), "+l"(flags));
    slots = (void **)(heap << 18);
    BattleEffect_LoadBlitter(104, flags);
    out[0] = slots[26];
    BattleEffect_LoadBlitter(188, 3);
    out[1] = slots[47];
  } else {
    u32 heap = 0xc0;
    s32 flags = 23;
    void **slots;

    /* FAKEMATCH(barrier): the same asm for the mirrored blitters; without it,
     * 2 of 30 instructions differ in tla-en. */
    __asm__("" : "+l"(heap), "+l"(flags));
    slots = (void **)(heap << 18);
    BattleEffect_LoadBlitter(104, flags);
    out[0] = slots[26];
    BattleEffect_LoadBlitter(188, 7);
    out[1] = slots[47];
  }
}

void Func_08144b14(struct BattleEffectArgument *effect, s32 mode);

/// Abilities: Revive, Water of Life, Lich (kind 25); Quartz (kind 201).
DEFINE_EFFECT_KIND(BattleEffect_RunRevive, Func_08144b14, 0)

/// Abilities: Dew (kind 209).
DEFINE_EFFECT_KIND(BattleEffect_RunDew, Func_08144b14, 1)

/// Abilities: Spark (kind 255).
DEFINE_EFFECT_KIND(BattleEffect_RunSpark, Func_08144b14, 2)

#include "fixed_math.h"

#include "transform.h"

void Func_0815585c(struct BattleEffectArgument *effect, u32 packed, s32 *screen,
                   s32 *world); // BattleEffect_RunDjinniEntrance

extern char ResourceId_EarthWallSheet;
extern char ResourceId_CrescentMoonSheet;
extern char ResourceId_VenusDjinnSheet;

extern u8 Data_0819786c[];  // the mote's picture for each step of its sway
extern u16 Data_08197874[]; // the beam's head: three pictures, offsets
extern u8 Data_0819787a[];  // ... their widths
extern u8 Data_0819787d[];  // ... their heights

// The pictures in the work sheet, after the beam (EarthWallSheet).
#define PIC_MOTE 0x2710  // the crescent motes, 24 x 24
#define PIC_DJINN 0x65c0 // the Djinn, 40 x 40
#define MOTES 8          // per target

// Member beam (kinds through three wrappers; the mode picks the Djinn:
// 0 Venus, 1 Mercury, 2 Mars): with a prelude, the Djinn appears, sways and
// rises away; for each target in turn, sixteen frames apart, eight motes
// drift down swaying; from frame 72 a beam of light falls on each target,
// flashing it, and narrows from frame 88.
void Func_08144b14(struct BattleEffectArgument *effect, s32 mode) {
  s32 djinn[3];
  s32 djinnSize[3];
  struct EffectPosition one;
  struct EffectPosition pos;
  s32 ground[3];
  s32 point[3];
  DrawRectangle draw[2];
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  s32 offsetX;
  s32 i;
  s32 k;
  struct HeapSlots *heap;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  // heap holds the heap slots from here for the second blitter's read, so the
  // 0x03000000 base is built again there; as Ram_HeapSlots at that read, 102
  // of 652 instructions differ in tla-en.
  heap = Ram_HeapSlots;

  // Set up: the canvas for the Djinn when there is a prelude, then the beam,
  // the motes and the mode's Djinn, each with its palette.
  BattleEffect_StartCanvasLayer(0);
  if (effect->prelude == 1)
    Func_0815585c(effect, (effect->side ^ effect->prelude) << 4 | mode, djinn,
                  djinnSize);
  Resource_LoadAndDecompress((s32)&ResourceId_EarthWallSheet, work->sheet, 1,
                             1);
  Resource_LoadAndDecompress((s32)&ResourceId_CrescentMoonSheet,
                             work->sheet + PIC_MOTE, 1, 1);
  if (mode == 0)
    Resource_LoadAndDecompress((s32)&ResourceId_VenusDjinnSheet,
                               work->sheet + PIC_DJINN, 1, 1);
  else if (mode == 1)
    Resource_LoadAndDecompress((s32)&ResourceId_MercuryDjinnSheet,
                               work->sheet + PIC_DJINN, 1, 1);
  else
    Resource_LoadAndDecompress((s32)&ResourceId_MarsDjinnSheet,
                               work->sheet + PIC_DJINN, 1, 1);

  // The motes start 80 pixels up, each at its own phase of the sway.
  for (i = 0; i != 512; i++) {
    PARTICLES[i].x = 0;
    PARTICLES[i].y = 80 << 16;
    PARTICLES[i].z = (Random16() | -32) << 14;
    PARTICLES[i].variant = Random16() & 255;
  }

  // With one target, BG2 is shifted so that it stands at x = 64.
  REG_BG2PA = 0x100;
  if (effect->count == 1) {
    EffectPosition_ProjectScaledBattler(effect->actors[0], &one);
    offsetX = 64 - one.x;
  } else {
    if (effect->side == 1)
      offsetX = -112;
    else
      offsetX = 0;
  }
  REG_BG2X = offsetX << 8;
  work->transferMode = 2;
  work->transferValue = 50;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);

  for (frame = 0; frame != effect->count * 16 + 116; frame++) {
    struct BattleCamera *camera = Ram_HeapSlots->cameraWork;
    s32 prelude;

    if (frame == 64)
      Sound_PlayFar(212);
    if (frame == 80)
      Func_081180e8(0);

    // The Djinn sways 20 pixels either way and rises from frame 32.
    prelude = effect->prelude;
    if (prelude == 1) {
      s32 angle = frame << 11;
      s32 x = (-Trig_Sin(angle) * 20 >> 16) + djinn[0] + offsetX - 20;
      s32 y = (Trig_Cos(angle) * 4 >> 16) + djinn[1] - 24;

      BattleEffect_FetchRectangleBlitters(effect->side ^ prelude, draw);
      if (frame > 32)
        y = y - frame * 2 + 64;
      draw[1](canvas, work->sheet + PIC_DJINN, x, y, 40, 40);
      if (frame <= 3)
        draw[1](canvas, work->sheet + PIC_DJINN, x, y, 40, 40);
      HeapSlot_Release(HEAP_BLITTER_ALTERNATE);
      HeapSlot_Release(HEAP_BLITTER);
    }
    BattleEffect_LoadBlitter(HEAP_BLITTER, BLITTER(BLIT_MAX, CLIP));
    draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
    BattleEffect_LoadBlitter(HEAP_BLITTER_ALTERNATE,
                             BLITTER(BLIT_MAX, CLIP | COMPOSITE_FLIP_X));
    draw[1] = (DrawRectangle)heap->unkb8[1];

    // A mote every eight frames drifts down a pixel a frame, swaying 16
    // pixels, until it is 8 pixels from the ground.
    for (k = 0; k != effect->count; k++) {
      struct MotionObject *target =
          GetBattleObjectSlotFar(effect->actors[k])->object;
      s32 start = k * 16;

      Render_ResetTransformState();
      Transform_SetLookAt(camera->eye, camera->pos);
      point[0] = target->x;
      point[1] = 0;
      point[2] = target->z;
      SceneTransform_ApplyPosition(point);
      if (frame > start) {
        for (i = 0; i != MOTES; i++) {
          struct EffectStep *mote = &PARTICLES[k * 64 + i];

          if (frame > i * 8 + start && mote->y > 8 << 16) {
            s32 sway;

            EffectPosition_ProjectPoint((s32 *)mote, &pos);
            pos.x += offsetX;
            sway = Trig_Sin(mote->variant << 10) * 16 >> 16;
            if (i & 1)
              pos.x -= sway;
            else
              pos.x += sway;
            draw[i & 1](canvas,
                        work->sheet +
                            Data_0819786c[mote->variant / 8 & 7] * 576 +
                            PIC_MOTE,
                        pos.x - 12, pos.y - 12, 24, 24);
            mote->y -= 1 << 16;
            mote->variant++;
          }
        }
      }
    }

    // The beam: from the top of the screen to the target's feet, drawn in
    // two mirrored halves, six pixels wide until frame 88, then narrowing a
    // pixel every three frames; its head flickers at the feet.
    for (i = 0; i != effect->count; i++) {
      s32 start = i * 16;

      if (frame >= start + 72) {
        struct MotionObject *target =
            GetBattleObjectSlotFar(effect->actors[i])->object;

        Render_ResetTransformState();
        Transform_SetLookAt(camera->eye, camera->pos);
        if (frame == start + 72)
          Func_0814cd48(effect->actors[i], 1, -1, -1, 0);
        if (frame == start + 88)
          Func_0814cd48(effect->actors[i], 0, -1, -1, 0);
        ground[0] = target->x;
        ground[1] = 0;
        ground[2] = target->z;
        EffectPosition_ProjectPoint(ground, &pos);
        pos.x += offsetX;
        if (frame < start + 104) {
          s32 pic = frame / 4;
          s32 width = 6;

          if (frame > start + 88)
            width = 6 - (frame - start - 88) / 3;
          if (pic > 2)
            pic = (pic & 1) + 1;
          if (frame < start + 100) {
            draw[0](canvas, work->sheet + Data_08197874[pic],
                    pos.x - Data_0819787a[pic], pos.y - Data_0819787d[pic] + 8,
                    Data_0819787a[pic], Data_0819787d[pic]);
            draw[1](canvas, work->sheet + Data_08197874[pic], pos.x,
                    pos.y - Data_0819787d[pic] + 8, Data_0819787a[pic],
                    Data_0819787d[pic]);
          }
          for (k = 0; k != pos.y; k++) {
            draw[0](canvas, work->sheet + 5, pos.x - width, k, width, 1);
            draw[1](canvas, work->sheet + 5, pos.x, k, width, 1);
          }
        }
      }
    }
    HeapSlot_Release(HEAP_BLITTER_ALTERNATE);
    HeapSlot_Release(HEAP_BLITTER);
    work->transferPending = 1;
    wait(1);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}

void Func_0814512c(struct BattleEffectArgument *effect, s32 mode);

/// Abilities: Mad Spatter (kind 347).
DEFINE_EFFECT_KIND(BattleEffect_RunMadSpatter, Func_0814512c, 4)

/// Abilities: Mud (kind 237).
DEFINE_EFFECT_KIND(BattleEffect_RunMud, Func_0814512c, 7)

/// Abilities: Decompose (kind 350).
DEFINE_EFFECT_KIND(BattleEffect_RunDecompose, Func_0814512c, 5)

/// Abilities: Bone Chiller (kind 309).
DEFINE_EFFECT_KIND(BattleEffect_RunBoneChiller, Func_0814512c, 0)

/// Abilities: Sticky Goo (kind 315).
DEFINE_EFFECT_KIND(BattleEffect_RunStickyGoo, Func_0814512c, 1)

/// Abilities: Poison Ink (kind 340).
DEFINE_EFFECT_KIND(BattleEffect_RunPoisonInk, Func_0814512c, 6)

/// Abilities: Sticky Poison (kind 322).
DEFINE_EFFECT_KIND(BattleEffect_RunStickyPoison, Func_0814512c, 2)

/// Abilities: Rotten Blood (kind 330).
DEFINE_EFFECT_KIND(BattleEffect_RunRottenBlood, Func_0814512c, 3)

/// Abilities: Rising Venom (kind 430).
DEFINE_EFFECT_KIND(BattleEffect_RunRisingVenom, Func_0814512c, 8)

/// Abilities: Formic Acid (kind 453).
DEFINE_EFFECT_KIND(BattleEffect_RunFormicAcid, Func_0814512c, 9)

// The shots live in the map buffer, 16 for each target, and the bursts
// after them, 32 for each target.

#define BURSTS ((struct EffectStep *)(Ram_MapCellBuffer + 0x1c00))

void BattleCamera_Shake(s32 rangeX, u32 rangeY);

void SummonEffect_RunCanvasMode1(void *action);
void SummonEffect_RunCanvasMode3(void *action);

extern char ResourceId_GoldShellSheet;
extern char ResourceId_SmokeSheet;
extern char ResourceId_BluePalette;
extern char ResourceId_SpiderWebSheet;
extern char ResourceId_LimePalette;
extern char ResourceId_GreenPalette;

// Each mode's record: whether its shots burst into shards (else into smoke),
// the shots' blitter, shots per target, and frames before the targets'
// share.
extern s8 Data_08197880[];
#define RECORD_SHARDS 0
#define RECORD_SHOT_BLITTER 1
#define RECORD_SHOTS 2
#define RECORD_FRAMES 3
extern u8 Data_081978a8[];  // shard widths
extern u8 Data_081978ba[];  // shard heights
extern u16 Data_081978cc[]; // shard cell offsets

#define SMOKE_SHEET 0x3e80 // the smoke, after the gold shells
#define SHARD_SHEET 0x1e00 // the shards, among the gold shells

// Shell shot (the body of ten kinds; the mode picks the colours and the
// record above the rest): shots arc from the caster to each target, one or
// four per target eight frames apart, turning as they fly; each bursts where
// it lands into smoke or shards, flashing the target. Mode 7 prepares a
// canvas for its prelude first; modes 3 and 5 hand the canvas to a summon
// canvas effect at the end.
void Func_0814512c(struct BattleEffectArgument *effect, s32 mode) {
  struct EffectPosition position;
  s32 preludeSize[3];
  s32 preludeOrigin[3];
  DrawRectangle draw[2];
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  s32 i;
  s32 k;
  s32 total;
  struct MotionObject *source;
  s32 *camera;
  u8 *palette;
  s32 height;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  camera = Ram_HeapSlots->cameraWork;

  // Set up: the canvas (mode 7 with its prelude), the gold shells and the
  // smoke with their palettes, then the mode's palette.
  if (mode == 7) {
    BattleEffect_StartCanvasLayer(0);
    Func_0815585c(effect, effect->side << 4 | 0x44, preludeOrigin, preludeSize);
    REG_BG2CNT = 0x785;
  } else {
    BattleEffect_StartCanvasLayer(1);
    REG_BLDALPHA = 0x1010;
  }
  Resource_LoadAndDecompress((s32)&ResourceId_GoldShellSheet, work->sheet, 1,
                             1);
  Resource_LoadAndDecompress((s32)&ResourceId_SmokeSheet,
                             work->sheet + SMOKE_SHEET, 1, 1);
  if (mode == 0)
    palette = Resource_GetTableEntry((s32)&ResourceId_BluePalette);
  else if (mode == 1)
    palette = Resource_GetTableEntry((s32)&ResourceId_SpiderWebSheet);
  else if (mode == 2)
    palette = Resource_GetTableEntry((s32)&ResourceId_LimePalette);
  else if (mode == 8)
    palette = Resource_GetTableEntry((s32)&ResourceId_LimePalette);
  else if (mode == 3)
    palette = Resource_GetTableEntry((s32)&ResourceId_GreenPalette);
  else if (mode == 4 || mode == 7)
    palette = Resource_GetTableEntry((s32)&ResourceId_GoldShellSheet);
  else if (mode == 6)
    palette = Resource_GetTableEntry((s32)&ResourceId_MarsDjinnSheet);
  else if (mode == 9)
    palette = Resource_GetTableEntry((s32)&ResourceId_PinkBurstSheet);
  else
    palette = Resource_GetTableEntry((s32)&ResourceId_GreenPalette);
  Iwram_CopyWords((void *)BG_PLTT, palette, 128);

  // The shots leave the caster at the mode's height, each aimed at its
  // target with a little spread in depth.
  for (i = 0; i != 1024; i++)
    PARTICLES[i].variant = -1;
  source = GetBattleObjectSlotFar(effect->actor)->object;
  if (mode == 7)
    height = 0x280000;
  else if (mode == 8)
    height = 0x140000;
  else
    height = 0x40000;
  for (k = 0; k != effect->count; k++) {
    struct MotionObject *target =
        GetBattleObjectSlotFar(effect->actors[k % effect->count])->object;

    for (i = 0; i != 16; i++) {
      struct EffectStep *shot = &PARTICLES[k * 16 + i];
      s32 aimZ;

      shot->x = source->x;
      shot->y = height;
      shot->z = source->z;
      aimZ = target->z + (((Random16() & 31) - 16) << 17);
      shot->velocityX = (target->x - source->x) >> 4;
      shot->velocityY = 0x40000;
      shot->velocityZ = (aimZ - source->z) >> 4;
      shot->variant = 0;
    }
  }
  for (i = 0; i != 256; i++)
    BURSTS[i].variant = -1;

  // The blitter pair for the caster's side; modes 4 and 7 clear the canvas
  // each frame and turn blending off, the others fade it to three quarters.
  BattleEffect_FetchRectangleBlitters(effect->side, draw);
  if (mode == 4 || mode == 7) {
    work->transferMode = 1;
    work->transferValue = 0;
    REG_BLDCNT = 0;
  } else {
    work->transferMode = 2;
    work->transferValue = 75;
  }
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);
  total = Data_08197880[mode * 4 + RECORD_FRAMES] + effect->count * 8;
  Sound_PlayFar(103);

  for (frame = 0; frame != total; frame++) {
    struct EffectStep *burst;
    struct EffectStep *piece;

    Render_ResetTransformState();
    Transform_SetLookAt(camera, camera + 3);

    // The shots: each starts four frames after the last, flies under
    // gravity and turns through ten pictures; at the ground it bursts.
    for (k = 0; k != effect->count; k++) {
      for (i = 0; i != Data_08197880[mode * 4 + RECORD_SHOTS]; i++) {
        s32 start = k * 8 + i * 4;

        if (frame >= start) {
          struct EffectStep *shot = &PARTICLES[k * 16 + i];

          if (frame == start + 17) {
            Func_0814cd48(effect->actors[k % effect->count], 7, 5, k, 16);
            Func_081180e8(133);
          }
          if (shot->variant >= 0) {
            s32 step = (frame - start) / 3;

            if (step > 9)
              step = 9;
            EffectPosition_ProjectPoint((s32 *)shot, &position);
            position.x >>= 1;
            position.y -= 8;
            if (step > 4)
              draw[Data_08197880[mode * 4 + RECORD_SHOT_BLITTER]](
                  canvas, work->sheet + step * 768, position.x - 16,
                  position.y - 12, 32, 24);
            else
              draw[Data_08197880[mode * 4 + RECORD_SHOT_BLITTER]](
                  canvas, work->sheet + step * 768, position.x - 12,
                  position.y - 16, 24, 32);
            if (shot->variant == 0)
              BattleEffectKernels_IntegrateVector3(shot, 63, -0x8000);
            if (shot->y < 0) {
              s32 count;
              s32 n;

              shot->y = 0;
              shot->variant = 1;
              if (Data_08197880[mode * 4 + RECORD_SHARDS] == 0)
                count = 4;
              else
                count = 16;
              for (n = 0; n != count; n++) {
                piece = &BURSTS[(k * 4 + i) * 8 + n];
                piece->x = shot->x;
                piece->y = shot->y;
                piece->z = shot->z;
                if (Data_08197880[mode * 4 + RECORD_SHARDS] == 0) {
                  piece->velocityX = ((Random16() & 63) - 32) << 11;
                  piece->velocityY = 0;
                  piece->velocityZ = ((Random16() & 63) - 32) << 11;
                } else {
                  piece->velocityX = ((Random16() & 63) - 32) << 13;
                  piece->velocityY = ((Random16() & 31) + 32) << 12;
                  piece->velocityZ = ((Random16() & 63) - 32) << 13;
                }
                piece->variant = 0;
              }
            }
          }
        }
      }
    }

    // The bursts: smoke rises slowly and spreads; shards fly up, fall and
    // tumble, facing the way they move. Each lives 45 frames.
    for (i = 0, burst = BURSTS; i != 256; burst++, i++) {
      if ((u32)burst->variant <= 44 && burst->y >= 0) {
        EffectPosition_ProjectPoint((s32 *)burst, &position);
        position.x >>= 1;
        if (Data_08197880[mode * 4 + RECORD_SHARDS] == 0) {
          draw[1](canvas, work->sheet + burst->variant / 8 * 1152 + SMOKE_SHEET,
                  position.x - 12, position.y - 24, 24, 48);
        } else {
          s32 picture = burst->variant / 5;
          s32 flip;

          if (i & 1)
            picture += 9;
          flip = effect->side;
          draw[burst->velocityX > 0 ? flip ^ 1 : flip](
              canvas, work->sheet + Data_081978cc[picture] + SHARD_SHEET,
              position.x - Data_081978a8[picture] / 2,
              position.y - Data_081978ba[picture] / 2, Data_081978a8[picture],
              Data_081978ba[picture]);
        }
        if (Data_08197880[mode * 4 + RECORD_SHARDS] == 0)
          BattleEffectKernels_IntegrateVector3(burst, 62, 0x800);
        else
          BattleEffectKernels_IntegrateVector3(burst, 62, -0x8000);
        burst->variant++;
      }
    }
    BattleCamera_Shake(4, 4);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(188);
  HeapSlot_Release(104);

  // Modes 3 and 5 clear both canvases and go on into a summon canvas effect.
  if (mode == 3 || mode == 5) {
    /* FAKEMATCH(wrapper): both clears through ClearWords_, which builds the
     * size anew for each; as direct Iwram_ClearWords calls, 16 of 718
     * instructions differ in tla-en. */
    ClearWords_((void *)0x06004000, 0x3c00);
    ClearWords_(canvas, 0x3c00);
    Task_Remove(Palette_StepFadeTransfer);
    if (mode == 3)
      SummonEffect_RunCanvasMode1(effect);
    else
      SummonEffect_RunCanvasMode3(effect);
  } else {
    BattleEffect_EndCanvasLayer();
  }
}

// The canvas routines every effect shares, with their Broken Seal names.
void BattleEffect_StartTiledCanvas(s32 bgControl);

void Func_08118088(s32 actor, s32 variant); // ..ApplyVariantMotionFar

void BattleBackground_LoadFar(s32 layer, s32 background, s32 frames);

extern char ResourceId_CounterRevealSheet;
extern char ResourceId_RedCrescentSheetA;
extern u8 Data_081978f0[]; // each panel's x, by side (7 a side)
extern u8 Data_081978fe[]; // each panel's y
extern u8 gMapCellBuffer[];

// The seven panels of the counter picture: the first two in the work sheet,
// the rest in the map buffer.
#define PANEL_B 0x15d2
#define PANEL_C 0x3081
#define PANEL_D 0x0000
#define PANEL_E 0x2d80
#define PANEL_F 0x4b00
#define PANEL_G 0x58d2

// Counter reveal (kind 40): a shimmering image circles in, fades out and in
// again twice as it closes on the target, then splits into its panels one
// after another with a white flash at frame 72, when the target is thrown
// and the screen shakes. The background waves while the image fades.
void BattleEffect_RunCondemn(struct BattleEffectArgument *effect) {
  struct BattleEffectWork *work;
  void *canvas;
  DrawRectangle draw[2];
  // Unused: the second position gives the stack frame its 72 bytes, as in
  // The Broken Seal's twin (one position: 4 of 549 differ, 2026-11-08).
  struct EffectPosition pos[2];
  s32 curve[2];
  s32 visible;
  s32 column;
  s32 row;
  s32 zero;
  u8 *cells;
  s32 frame;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];

  // The zero is held in r8 through the setup calls, as in The Broken Seal's
  // twin (the literal 0 stored directly: 81 of 549 differ, 2026-11-08).
  zero = 0;

  // Set up: half-and-half blending, the image with its palette, the crescent
  // panels into the map buffer, two transparent blitters (the second
  // flipped), the wave's HBlank DMA and a transfer that clears the canvas.
  BattleEffect_StartTiledCanvas(0x2000);
  REG_BG2PA = 0x100;
  cells = gMapCellBuffer;
  REG_BLDALPHA = 0x1010;
  Resource_LoadAndDecompress((s32)&ResourceId_CounterRevealSheet, work->sheet,
                             1, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_RedCrescentSheetA, cells, 1, 0);
  BattleEffect_LoadBlitter(HEAP_BLITTER, BLITTER(BLIT_TRANSPARENT, CLIP));
  draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  BattleEffect_LoadBlitter(HEAP_BLITTER_ALTERNATE,
                           BLITTER(BLIT_TRANSPARENT, CLIP | COMPOSITE_FLIP_X));
  draw[1] = (DrawRectangle)Ram_HeapSlots->unkb8[1];
  Task_Add(BattleEffect_ArmBg2AffineHBlankDma, 0xc80);
  work->transferMode = 1;
  work->transferValue = zero;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);

  // The image starts off to the side, above the field.
  visible = 1;
  if (effect->side == 1)
    curve[0] = -0x500000;
  else
    curve[0] = 0x700000;
  curve[1] = -0x200000;

  for (frame = 0; frame != 104; frame++) {
    s32 spin;
    s32 screenX;
    s32 screenY;
    s32 amp;
    s32 rowBase;
    s32 angle;
    s32 i;
    s32 *scanline;

    spin = frame << 9;
    screenX = (curve[0] >> 16) + ((Trig_Sin(spin) << 4) >> 16) + 48;
    screenY = (curve[1] >> 16) + ((Trig_Cos(spin) << 2) >> 16) + 16;

    // It jumps closer at frame 24 and onto the target at frame 48, hidden
    // for the frame of each jump.
    if (frame == 24) {
      if (effect->side == 1)
        curve[0] = -0x200000;
      else
        curve[0] = 0x480000;
      curve[1] = 0x180000;
      visible = 0;
    }
    if (frame == 25) {
      REG_BLDALPHA = 0x1010;
      visible = 1;
    }
    if (frame == 48) {
      EffectPosition_ProjectScaledBattler(effect->actors[0], pos);
      if (effect->side == 1)
        curve[0] = (pos[0].x - 128) << 16;
      else
        curve[0] = (pos[0].x - 64) << 16;
      curve[1] = 0;
      visible = 0;
    }
    if (frame == 49) {
      REG_BLDALPHA = 0x1010;
      visible = 1;
    }

    // Before each jump the image fades out over eight frames while the
    // background waves harder.
    scanline = work->bg2xLines;
    amp = 0;
    if (frame <= 23) {
      if (frame > 15) {
        amp = (frame * 2) - 32;
        REG_BLDALPHA = (31 - frame) | 0x1000;
      }
    } else if (frame <= 47) {
      if (frame > 31) {
        amp = (frame * 2) - 64;
        REG_BLDALPHA = (47 - frame) | 0x1000;
      }
    }
    if (amp < 0)
      amp = 0;

    i = 0;
    rowBase = (6 - screenX) << 8;
    angle = frame << 11;
    for (; i != 160; i++) {
      *scanline++ = rowBase - ((Trig_Sin(angle) * amp) >> 10);
      angle += 0x800;
    }

    // The image, then from frame 72 its panels, each shown for two frames.
    if (visible != 0) {
      s32 side = effect->side;

      if (side == 0) {
        column = 0;
        row = 0;
      } else {
        column = 1;
        /* FAKEMATCH(dead): i is 160 here, so i >> 31 folds to 0 and only
         * changes allocation; as row = 0, 477 of 549 instructions differ in
         * tla-en. */
        row = i >> 31;
        /* FAKEMATCH(dead): side = 0, never read, keeps the side's copy from
         * replacing the draws' index, so the test and the draws share one
         * register; without it, 31 of 549 instructions differ in tla-en. */
        side = 0;
      }
      if (frame <= 71) {
        draw[effect->side](canvas, work->sheet, Data_081978f0[column * 7],
                           Data_081978fe[row * 7] + screenY, 57, 98);
      } else {
        if (frame <= 75)
          draw[effect->side](canvas, work->sheet, Data_081978f0[column * 7],
                             Data_081978fe[row * 7] + screenY, 57, 98);
        draw[effect->side](canvas, work->sheet + PANEL_B,
                           Data_081978f0[column * 7 + 1],
                           Data_081978fe[row * 7 + 1] + screenY, 99, 69);
        if ((u32)(frame - 72) <= 1)
          Iwram_FillWords(canvas, 0x4000, 0x3f3f3f3f);
        if ((u32)(frame - 74) <= 1)
          draw[effect->side](canvas, work->sheet + PANEL_C,
                             Data_081978f0[column * 7 + 2],
                             Data_081978fe[row * 7 + 2] + screenY, 128, 91);
        if ((u32)(frame - 76) <= 1)
          draw[effect->side](canvas, gMapCellBuffer + PANEL_D,
                             Data_081978f0[column * 7 + 3],
                             Data_081978fe[row * 7 + 3] + screenY, 128, 91);
        if ((u32)(frame - 78) <= 1)
          draw[effect->side](canvas, gMapCellBuffer + PANEL_E,
                             Data_081978f0[column * 7 + 4],
                             Data_081978fe[row * 7 + 4] + screenY, 128, 59);
        if ((u32)(frame - 80) <= 1)
          draw[effect->side](canvas, gMapCellBuffer + PANEL_F,
                             Data_081978f0[column * 7 + 5],
                             Data_081978fe[row * 7 + 5] + screenY, 122, 29);
        if ((u32)(frame - 82) <= 1)
          draw[effect->side](canvas, gMapCellBuffer + PANEL_G,
                             Data_081978f0[column * 7 + 6],
                             Data_081978fe[row * 7 + 6] + screenY, 76, 25);
      }
    }

    // The strike: a cue at 68; at 72 the target is thrown, the screen
    // shakes and the results begin.
    if (frame == 68)
      Sound_PlayFar(212);
    if (frame == 72) {
      Func_08118088(effect->actors[0], 0);
      work->shakeFrames = 8;
      Func_081180e8(144);
    }
    BattleCamera_Shake(8, 8);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  Task_Remove(BattleEffect_ArmBg2AffineHBlankDma);
  // The battle's own background (its id leads unk670) comes back.
  BattleBackground_LoadFar(1, *(u16 *)Ram_HeapSlots->battleWork->unk670, 24);
  HeapSlot_Release(HEAP_BLITTER_ALTERNATE);
  HeapSlot_Release(HEAP_BLITTER);
  BattleEffect_EndCanvasLayer();
}

#define PIXEL_BUFFER Ram_MapCellBuffer

static __inline__ void CopyWords_(void *destination, const void *source,
                                  s32 size) {
  /* FAKEMATCH(wrapper): the copy forwarded through an inline, which loads the
   * routine's entry before the arguments, as the native does at all three
   * copies; as direct Iwram_CopyWords calls, 8 of 181 instructions differ in
   * tla-en. */
  Iwram_CopyWords(destination, source, size);
}

/// Turns the background canvas into a blue ramp, row by row from the top, each
/// column starting after its own random delay.
void Graphics_ConvertBackgroundToBlueRamp(void) {
  s32 i, x, y, row, end, speed;
  s32 red, green, blue;
  u16 color;
  u8 *delays, *p, *pixel;
  u16 *palette;
  u32 delayMask;

  palette = (u16 *)0x05000040;
  i = 0;
  do {
    s32 half;

    half = i / 2;
    *palette++ = RGB(half, half, i);
    i++;
  } while (i != 32);
  delays = Ram_HeapSlots->unk64[0];
  end = 0;
  speed = 16;
  CopyWords_(PIXEL_BUFFER, (void *)0x06008000, 0x7800);
  delayMask = 63;
  p = delays;
  do {
    *p++ = Random16() & delayMask;
  } while (p != delays + 256);
  row = 0;
  i = 0;
  do {
    end += speed / 4;
    speed++;
    while (row != end) {
      x = 0;
      do {
        y = row - delays[x];
        if (y >= 0) {
          if (y <= 119) {
            pixel = PIXEL_BUFFER + ((x & 7) + ((x / 8) << 6) + ((y & 7) << 3) +
                                    ((y / 8) << 11));
            color = ((s16 *)0x05000000)[*pixel];
            red = color & 31;
            green = (color >> 5) & 31;
            blue = (color >> 10) & 31;
            if (red < green)
              red = green;
            if (red < blue)
              red = blue;
            *pixel = 63 - red;
          }
        }
        x++;
      } while (x != 256);
      row++;
    }
    CopyWords_((void *)0x06008000, PIXEL_BUFFER, 0x7800);
    wait(1);
    if (end > 248)
      break;
    i++;
  } while (i != 27);
  palette = (u16 *)0x050000c0;
  i = 0;
  do {
    s32 half;

    half = i / 2;
    *palette++ = RGB(half, half, i);
    i++;
  } while (i != 32);
  {
    u8 *buffer = PIXEL_BUFFER;

    i = 0;
    do {
      *buffer++ += 64;
      i++;
    } while (i != 0x7800);
  }
  CopyWords_((void *)0x06008000, PIXEL_BUFFER, 0x7800);
  wait(1);
}

void Func_081496c8(s32 frame, s32 redPhase, s32 greenPhase,
                   s32 bluePhase); // Graphics_UpdatePhasePalette
void Func_080150e4(s32 angle);     // SceneTransform_ApplyRoll
void Func_080151ac(s32 *scale);    // SceneTransform_ApplyScale
void SceneTransform_ApplyYaw(s32 angle);

extern char ResourceId_RuneSheet;
extern char ResourceId_Data134;
extern char ResourceId_ParticleSpritesD;
extern char ResourceId_JupiterDjinnSheet;
extern s32 Data_0819790c[]; // the triangle's first corner: (0, 24, 0)
extern u16 Data_08197424[]; // flare sizes 1 to 5: cell offsets
#define FLARE_SHEET 0x1000  // the flares, after the rune picture
#define DJINN_SHEET 0x2000  // the Jupiter Djinn, after the flares
#define DJINN_SIZE 40

#define CORNERS 3
#define EDGE_STEPS 24 // flares drawn along each edge

// Spinning triangle (kinds 38 and 218): as BattleEffect_RunTriangleStrike, a
// triangle of flares spins down onto the target and leaves the rune picture
// where it lands; when the effect has a prelude, the Jupiter Djinn also circles
// above its place for the first frames and rises away after frame 32.
void BattleEffect_RunBind(struct BattleEffectArgument *effect) {
  s32 origin[3];
  s32 originSize[3];
  struct EffectPosition anchor;
  s32 vector[3];
  struct EffectPosition screen;
  s32 scale[3];
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  DrawRectangle draw[2];
  s32 member;
  s32 *camera;
  s32 shift;
  u8 *graphics;
  struct MotionObject *motion;
  s32 memberFrame;
  struct EffectStep *point;
  s32 size;
  s32 j;
  s32 k;
  struct HeapSlots *heap;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  graphics = Ram_HeapSlots->unk64[0];
  camera = Ram_HeapSlots->cameraWork;
  // heap holds the heap slots from here for the second blitter's read, so the
  // 0x03000000 base is built again there; as Ram_HeapSlots at that read, 28
  // of 518 instructions differ in tla-en.
  heap = Ram_HeapSlots;

  // Set up: the rune picture with its palette, the dots, the flares, the
  // Djinn with its palette, and a transfer that fades the canvas by 4 each
  // frame.
  BattleEffect_StartCanvasLayer(1);
  if (effect->prelude == 1)
    Func_0815585c(effect, effect->side << 4 | 3, origin, originSize);
  REG_BG2PA = 0x100;
  Resource_LoadAndDecompress((s32)&ResourceId_RuneSheet, work->sheet, 1, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, graphics, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesD,
                             work->sheet + FLARE_SHEET, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_JupiterDjinnSheet,
                             work->sheet + DJINN_SHEET, 1, 0);
  work->transferMode = 3;
  work->transferValue = 0x04040404;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);

  // The background is shifted so that the target stands at x = 64.
  EffectPosition_ProjectScaledBattler(effect->actors[0], &anchor);
  shift = 64 - anchor.x;
  REG_BG2X = shift << 8;
  Sound_PlayFar(142);

  for (frame = 0; frame != effect->count * 20 + 72; frame++) {
    if (frame == 64)
      Func_081180e8(0);
    Func_081496c8(frame, 0xaaab, 0x5555, 0);

    // The prelude: the Djinn circles where the effect was prepared, and
    // rises away after frame 32; its blitters live for this frame only.
    if (effect->prelude == 1) {
      s32 x;
      s32 y;

      x = ((Trig_Sin(frame << 11) * 20) >> 16) + origin[0] + shift - 20;
      y = ((Trig_Cos(frame << 11) << 2) >> 16) + origin[1] - 24;
      BattleEffect_FetchRectangleBlitters(effect->side, draw);
      if (frame > 32)
        y = y - frame * 2 + 64;
      draw[0](canvas, work->sheet + DJINN_SHEET, x, y, DJINN_SIZE, DJINN_SIZE);
      if (frame <= 3)
        draw[1](canvas, work->sheet + DJINN_SHEET, x, y, DJINN_SIZE,
                DJINN_SIZE);
      HeapSlot_Release(HEAP_BLITTER_ALTERNATE);
      HeapSlot_Release(HEAP_BLITTER);
    }

    // A fresh pair of max blitters each frame, the second flipped.
    BattleEffect_LoadBlitter(HEAP_BLITTER, BLITTER(BLIT_MAX, CLIP));
    draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
    BattleEffect_LoadBlitter(HEAP_BLITTER_ALTERNATE,
                             BLITTER(BLIT_MAX, CLIP | COMPOSITE_FLIP_X));
    draw[1] = (DrawRectangle)heap->unkb8[1];
    if (frame > 16 && (frame & 15) == 0)
      work->transferValue += 0x01010101;

    for (member = 0; member != 1; member++) {
      motion = GetBattleObjectSlotFar(effect->actors[member])->object;
      memberFrame = frame - member * 8;
      if (memberFrame >= 0 && memberFrame < 96) {
        Render_ResetTransformState();
        Transform_SetLookAt(camera, camera + 3);
        vector[0] = motion->x;
        vector[1] = motion->y;
        vector[2] = motion->z;
        EffectPosition_ProjectPoint(vector, &screen);
        screen.x = anchor.x + shift;
        screen.y -= 24;

        // The three corners turn and shrink toward the target until frame
        // 64, then hold; flares are drawn along the edges between them.
        if (memberFrame <= 67) {
          for (j = 0; j != CORNERS; j++) {
            point = &work->particles[member * 32 + j];
            Render_ResetTransformState();
            if (memberFrame <= 63) {
              scale[0] = 0x2a000 - (memberFrame * 3 << 9);
              scale[1] = 0x2a000 - (memberFrame * 3 << 9);
              scale[2] = 0x2a000 - (memberFrame * 3 << 9);
              Func_080151ac(scale);
              Func_080150e4((64 - memberFrame) << 9);
              SceneTransform_ApplyYaw((64 - memberFrame) << 9);
            }
            Func_080150e4(j * 0x5555);
            EffectPosition_ProjectPoint(Data_0819790c,
                                        (struct EffectPosition *)vector);
            point->velocityX = vector[0] + screen.x;
            point->velocityY = vector[1] + screen.y + 16;
          }
          for (j = 0; j != CORNERS; j++) {
            struct EffectStep *from = &work->particles[j + member * 32];
            struct EffectStep *to = &work->particles[(j + 1) % 3 + member * 32];

            size = 5 - memberFrame / 16;
            for (k = 0; k != EDGE_STEPS; k++) {
              s32 x =
                  from->velocityX + k * (to->velocityX - from->velocityX) / 24;
              s32 y =
                  from->velocityY + k * (to->velocityY - from->velocityY) / 24;

              draw[0](canvas,
                      work->sheet + Data_08197424[size - 1] + FLARE_SHEET,
                      x - size, y - size, size * 2, size * 2);
            }
          }
        }

        // Landed: the rune picture, its right half mirrored from the left.
        if (memberFrame > 63) {
          draw[0](canvas, work->sheet, screen.x - 24, screen.y - 24, 24, 48);
          draw[1](canvas, work->sheet, screen.x, screen.y - 24, 24, 48);
        }
      }
    }
    HeapSlot_Release(HEAP_BLITTER_ALTERNATE);
    HeapSlot_Release(HEAP_BLITTER);
    work->transferPending = 1;
    wait(1);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}

extern s32 Data_08197918[]; // the triangle's first corner: (0, 24, 0)

// Triangle strike (kind 67): a triangle of flares spins down onto the
// target, shrinking, for 64 frames; then the rune picture is stamped in two
// halves where it landed. The caster approaches the target at frame 46, the
// palette pulses throughout and the trail fades a step every 16 frames.
void BattleEffect_RunTriangleStrike(struct BattleEffectArgument *effect) {
  struct EffectPosition anchor;
  s32 vector[3];
  struct EffectPosition screen;
  s32 scale[3];
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  DrawRectangle draw[2];
  s32 member;
  s32 *camera;
  s32 shift;
  u8 *graphics;
  struct MotionObject *object;
  s32 memberFrame;
  struct EffectStep *point;
  s32 size;
  s32 j;
  s32 k;
  struct HeapSlots *heap;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  graphics = Ram_HeapSlots->unk64[0];
  camera = Ram_HeapSlots->cameraWork;
  // heap holds the heap slots from here for the second blitter's read, so the
  // 0x03000000 base is built again there; as Ram_HeapSlots at that read, 32
  // of 428 instructions differ in tla-en.
  heap = Ram_HeapSlots;

  // Set up: the rune picture with its palette, the dots, the flares, and a
  // transfer that fades the canvas by 4 each frame.
  BattleEffect_StartCanvasLayer(0);
  REG_BG2PA = 0x100;
  Resource_LoadAndDecompress((s32)&ResourceId_RuneSheet, work->sheet, 1, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, graphics, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesD,
                             work->sheet + FLARE_SHEET, 0, 0);
  work->transferMode = 3;
  work->transferValue = 0x04040404;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);

  // The background is shifted so that the target stands at x = 64.
  EffectPosition_ProjectScaledBattler(effect->actors[0], &anchor);
  shift = 64 - anchor.x;
  REG_BG2X = shift << 8;
  Sound_PlayFar(142);

  for (frame = 0; frame != effect->count * 20 + 72; frame++) {
    if (frame == 64)
      Func_081180e8(0);
    if (frame == 46)
      BattleMotion_ApproachTargetFar(effect->actor, effect->actors[0], 16, 0);
    Func_081496c8(frame, 0xaaab, 0x5555, 0);

    // A fresh pair of max blitters each frame, the second flipped.
    BattleEffect_LoadBlitter(HEAP_BLITTER, BLITTER(BLIT_MAX, CLIP));
    draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
    BattleEffect_LoadBlitter(HEAP_BLITTER_ALTERNATE,
                             BLITTER(BLIT_MAX, CLIP | COMPOSITE_FLIP_X));
    draw[1] = (DrawRectangle)heap->unkb8[1];
    if (frame > 16 && (frame & 15) == 0)
      work->transferValue += 0x01010101;

    for (member = 0; member != 1; member++) {
      object = GetBattleObjectSlotFar(effect->actors[member])->object;
      memberFrame = frame - member * 8;
      if (memberFrame >= 0 && memberFrame < 96) {
        Render_ResetTransformState();
        Transform_SetLookAt(camera, camera + 3);
        vector[0] = object->x;
        vector[1] = object->y;
        vector[2] = object->z;
        EffectPosition_ProjectPoint(vector, &screen);
        screen.x = anchor.x + shift;
        screen.y -= 24;

        // The three corners turn and shrink toward the target until frame
        // 64, then hold; flares are drawn along the edges between them.
        if (memberFrame <= 67) {
          for (j = 0; j != CORNERS; j++) {
            point = &work->particles[member * 32 + j];
            Render_ResetTransformState();
            if (memberFrame <= 63) {
              scale[0] = 0x2a000 - (memberFrame * 3 << 9);
              scale[1] = 0x2a000 - (memberFrame * 3 << 9);
              scale[2] = 0x2a000 - (memberFrame * 3 << 9);
              Func_080151ac(scale);
              Func_080150e4((64 - memberFrame) << 9);
              SceneTransform_ApplyYaw((64 - memberFrame) << 9);
            }
            Func_080150e4(j * 0x5555);
            EffectPosition_ProjectPoint(Data_08197918,
                                        (struct EffectPosition *)vector);
            point->velocityX = vector[0] + screen.x;
            point->velocityY = vector[1] + screen.y + 16;
          }
          for (j = 0; j != CORNERS; j++) {
            struct EffectStep *from = &work->particles[j + member * 32];
            struct EffectStep *to = &work->particles[(j + 1) % 3 + member * 32];

            size = 5 - memberFrame / 16;
            for (k = 0; k != EDGE_STEPS; k++) {
              s32 x =
                  from->velocityX + k * (to->velocityX - from->velocityX) / 24;
              s32 y =
                  from->velocityY + k * (to->velocityY - from->velocityY) / 24;

              draw[0](canvas,
                      work->sheet + Data_08197424[size - 1] + FLARE_SHEET,
                      x - size, y - size, size * 2, size * 2);
            }
          }
        }

        // Landed: the rune picture, its right half mirrored from the left.
        if (memberFrame > 63) {
          draw[0](canvas, work->sheet, screen.x - 24, screen.y - 24, 24, 48);
          draw[1](canvas, work->sheet, screen.x, screen.y - 24, 24, 48);
        }
      }
    }
    HeapSlot_Release(HEAP_BLITTER_ALTERNATE);
    HeapSlot_Release(HEAP_BLITTER);
    work->transferPending = 1;
    wait(1);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}

#include "actor.h"

void Transform_Pop(void);

// The camera's heading, as the camera work holds it.
struct CameraView {
  u8 unk00[0x36];
  u16 yaw;
};

extern s32 gData_08197924[]; // the two ring points, alternating
extern u16 gData_08197410[]; // dot sizes 1 to 10: cell offsets in the sheet

#define BOLT_COUNT 3
#define RING_POINTS 10
#define RING_STEPS 16 // dots drawn between two ring points
#define FAR_DEPTH 399999

#define ACTOR(id) ((struct Actor *)GetBattleObjectSlotFar(id)->object)

// Three rings of light fly one after another from the caster to the target,
// each growing as it spins, while the camera swings round; each ring that
// lands throws the target back, and the last begins the results.
void BattleEffect_RunAstralBlast(struct BattleEffectArgument *effect) {
  struct EffectPosition pos;
  struct EffectPosition base;
  s32 scale[3];
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  DrawRectangle draw[2];
  s32 i;
  u8 *graphics;
  s32 *camera;
  struct Actor *caster;
  struct Actor *source;
  struct Actor *target;
  struct EffectStep *point;
  struct EffectStep *bolt;
  s32 start;
  s32 nearest;
  s32 size;
  s32 j;
  s32 k;
  struct EffectStep *ring;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  graphics = Ram_HeapSlots->unk64[0];
  camera = Ram_HeapSlots->cameraWork;
  caster = ACTOR(effect->actor);

  // Set up: the rune palette, the dot sheet, the caster's casting pose, one
  // max blitter and the transfer.
  BattleEffect_StartCanvasLayer(1);
  Iwram_CopyWords((void *)BG_PLTT,
                  Resource_GetTableEntry((s32)&ResourceId_RuneSheet), 128);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, graphics, 0, 0);
  Actor_SetMode(caster, 2);
  Actor_SetAnimationSpeedFar((struct ActorView *)caster, 48);
  BattleEffect_LoadBlitter(
      HEAP_BLITTER, BLITTER(BLIT_MAX, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y));
  work->transferMode = 2;
  work->transferValue = 75;
  draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);

  // The rings leave from above the caster and reach the target in twelve
  // frames; the second and third aim past it.
  source = ACTOR(effect->actor);
  target = ACTOR(effect->actors[0]);
  for (i = 0, point = work->particles; i != BOLT_COUNT; i++) {
    point->x = source->x;
    point->y = source->y + 0x280000;
    point->z = source->z;
    if (i == 0)
      point->velocityX = (target->x - point->x) / 12;
    else
      point->velocityX = (target->x * 2 - point->x) / 12;
    point->velocityY = (target->y - point->y + 0x280000) / 12;
    point->velocityZ = (target->z - point->z) / 12;
    point->variant = 0;
    point++;
  }

  ring = (struct EffectStep *)gMapCellBuffer;
  for (frame = 0; frame != 60; frame++) {
    // The camera turns toward the caster's side, slowing from frame 40.
    if (frame <= 47) {
      struct CameraView *view = (struct CameraView *)camera;
      s32 speed;

      if (frame <= 39)
        speed = 128;
      else
        speed = 0x300 - frame * 16;
      if (effect->side == 0)
        view->yaw -= speed;
      else
        view->yaw += speed;
    }

    // A new ring every twelve frames: it spins and grows from a dot to
    // full size, and is drawn while it is near enough to the camera.
    for (i = 0; i != BOLT_COUNT; i++) {
      start = i * 12;
      if (frame >= start) {
        bolt = &work->particles[i];
        size = (frame - start) / 4 + 2;
        if (size > 10)
          size = 10;
        Render_ResetTransformState();
        Transform_SetLookAt(camera, camera + 3);
        SceneTransform_ApplyPosition(bolt);
        nearest = 0;
        for (j = 0; j != RING_POINTS; j++) {
          s32 depth;

          Transform_Push();
          Func_080150e4((frame - start) << 10);
          SceneTransform_ApplyYaw(0x4000);
          scale[0] = ((frame - start) << 12) + 0x1000;
          if (scale[0] > 0x10000)
            scale[0] = 0x10000;
          scale[1] = scale[0];
          scale[2] = scale[0];
          Func_080151ac(scale);
          Func_080150e4(j * 0x199a);
          depth =
              EffectPosition_ProjectPoint(&gData_08197924[(j & 1) * 3], &pos);
          if (nearest < depth)
            nearest = depth;
          pos.x >>= 1;
          // The first two stores read base, which nothing sets, and are
          // overwritten at once; both games' code keeps them.
          ring[i * 10 + j].velocityX = pos.x + base.x;
          ring[i * 10 + j].velocityY = pos.y + base.y;
          ring[i * 10 + j].velocityX = pos.x;
          ring[i * 10 + j].velocityY = pos.y;
          Transform_Pop();
        }
        if (nearest <= FAR_DEPTH) {
          for (j = 0; j != RING_POINTS; j++) {
            struct EffectStep *from =
                &((struct EffectStep *)gMapCellBuffer)[j + i * 10];
            struct EffectStep *to =
                &((struct EffectStep *)gMapCellBuffer)[(j + 1) % 10 + i * 10];

            for (k = 0; k != RING_STEPS; k++) {
              s32 x =
                  from->velocityX + (to->velocityX - from->velocityX) * k / 16;
              s32 y =
                  from->velocityY + (to->velocityY - from->velocityY) * k / 16;

              draw[0](canvas, graphics + gData_08197410[size - 1], x - size / 2,
                      y - size, size, size * 2);
            }
          }
        }
        bolt->x += bolt->velocityX;
        bolt->y += bolt->velocityY;
        bolt->z += bolt->velocityZ;

        // Ten frames after it starts (plus one per ring) the ring lands: the
        // target is thrown back 40 pixels and up, and flashes; the third ring
        // begins the results instead.
        if (frame == start + i + 10) {
          target->acceleration = 0x20000;
          target->speedLimit = 0x80000;
          target->velocityY = 0x50000;
          target->gravity = 0xab85;
          target->actionFlags = 0;
          Actor_ResetMotion(target);
          if (target->x < 0)
            Actor_SetTargetPosition(target, target->x - 0x280000, 0, target->z);
          else
            Actor_SetTargetPosition(target, target->x + 0x280000, 0, target->z);
          if (i == 2) {
            Func_081180e8(134);
          } else {
            Sound_PlayFar(134);
            Func_0814cd48(effect->actors[0], 7, 5, 0, 8);
          }
          work->shakeFrames = 4;
        }
      }
    }
    BattleCamera_Shake(8, 8);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(HEAP_BLITTER_ALTERNATE);
  HeapSlot_Release(HEAP_BLITTER);
  BattleEffect_EndCanvasLayer();
}

#undef RING_STEPS

// Shining Star's rings of stars.

extern u8 gData_0819793c[]; // ring radii, alternating: 32, 16

#define STAR_COUNT 8

#define RING_STEPS 12 // dots drawn between two ring points
#define DOT_SIZE 2
#define GROUND_Y 0x1dffff

/// Eight rings of stars fly from the caster to the target, one every eight
/// frames and spinning as they go; each bounces where it meets the ground,
/// shaking the screen and flashing the targets.
void BattleEffect_RunShiningStar(struct BattleEffectArgument *effect) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  // Unused: draw[1] gives the stack frame its 4 bytes.
  DrawRectangle draw[2];
  s32 i;
  u8 *graphics;
  s32 *camera;
  struct MotionObject *source;
  struct MotionObject *target;
  struct EffectStep *point;
  struct EffectStep *star;
  struct EffectPosition pos;
  s32 size;
  s32 j;
  s32 k;
  struct EffectStep *ring;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  graphics = Ram_HeapSlots->unk64[0];
  camera = Ram_HeapSlots->cameraWork;

  // Set up: the rune palette, the dot sheet, one max blitter and the transfer.
  BattleEffect_StartCanvasLayer(1);
  Iwram_CopyWords((void *)BG_PLTT,
                  Resource_GetTableEntry((s32)&ResourceId_RuneSheet), 128);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, graphics, 0, 0);
  BattleEffect_LoadBlitter(
      HEAP_BLITTER, BLITTER(BLIT_MAX, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y));
  draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  work->transferMode = 2;
  work->transferValue = 50;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);

  // Each star leaves from above the caster on its own course to the target,
  // the later ones after a longer delay (held in variant).
  source = GetBattleObjectSlotFar(effect->actor)->object;
  target = GetBattleObjectSlotFar(effect->actors[0])->object;
  for (i = 0, point = work->particles; i != STAR_COUNT; i++) {
    s32 delay = i * 8;

    point->x = source->x / 2;
    point->y = source->y + 0x780000;
    point->z = source->z;
    point->velocityX =
        (target->x + ((s32)((Random16() & 0x7f) - 64) << 16) - point->x) / 12;
    point->velocityY = (target->y - point->y + 0x140000) / 12;
    point->velocityZ = (target->z - point->z) / 12;
    point->variant = (Random16() & 15) + delay;
    point++;
  }

  size = DOT_SIZE;
  ring = (struct EffectStep *)gMapCellBuffer;
  for (frame = 0; frame != 128; frame++) {
    Func_081496c8(frame, 0xaaab, 0x5555, 0);
    if (frame == 96)
      Func_081180e8(134);
    for (i = 0; i != STAR_COUNT; i++) {
      star = &work->particles[i];
      if (frame >= star->variant) {
        // Project the star and, while it is on screen, lay out its ring of
        // ten points (the velocity fields of the map buffer's steps), then
        // join each point to the next with a row of dots.
        Render_ResetTransformState();
        Transform_SetLookAt(camera, camera + 3);
        EffectPosition_ProjectPoint((s32 *)star, &pos);
        pos.x >>= 1;
        if (pos.x >= -8 && pos.x <= 127) {
          if (pos.y <= 127) {
            if (pos.y >= -8) {
              for (j = 0; j != RING_POINTS; j++) {
                ring[i * 10 + j].velocityX =
                    pos.x +
                    ((Trig_Sin(j * 0x199a - ((frame - star->variant) << 11)) *
                      gData_0819793c[j & 1]) /
                         2 >>
                     16);
                ring[i * 10 + j].velocityY =
                    pos.y -
                    (Trig_Cos(j * 0x199a - ((frame - star->variant) << 11)) *
                         gData_0819793c[j & 1] >>
                     16);
              }
              for (j = 0; j != RING_POINTS; j++) {
                struct EffectStep *from = &ring[j + i * 10];
                struct EffectStep *to = &ring[i * 10 + (j + 1) % 10];

                for (k = 0; k != RING_STEPS; k++) {
                  s32 x = from->velocityX +
                          (to->velocityX - from->velocityX) * k / 12;
                  s32 y = from->velocityY +
                          (to->velocityY - from->velocityY) * k / 12;

                  draw[0](canvas, graphics + gData_08197410[size - 1],
                          x - size / 2, y - size, size, size * 2);
                }
              }
            }
          }
        }

        // At the ground the star bounces up at half its sideways speed.
        if (star->y <= GROUND_Y) {
          star->velocityY = -star->velocityY;
          star->velocityX /= 2;
          star->velocityZ /= 2;
          work->shakeFrames = 4;
          Sound_PlayFar(134);
          for (j = 0; j != effect->count; j++)
            Func_0814cd48(effect->actors[j], 7, 5, j, 8);
        }
        star->x += star->velocityX;
        star->y += star->velocityY;
        star->z += star->velocityZ;
      }
    }
    BattleCamera_Shake(4, 4);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(HEAP_BLITTER_ALTERNATE);
  HeapSlot_Release(HEAP_BLITTER);
  BattleEffect_EndCanvasLayer();
}

// Summon 8 (BattleEffect_RunSceneEffect's case 8). Twin: The Broken Seal's
// BattleEffect_RunProcne, ported by trial 2 and rewritten here. The
// Lost Age's changes: BattleEffect_FetchRectangleBlitters for the pair; the
// whirlwind unpacked with its palette by Resource_LoadAndDecompress into the
// work sheet, the comets into work->sheet + 0x3600; frame / 4 % 3 written
// through Math_UnsignedMulHighAdd(x, 0x55555556, -3) and the depth divisions by
// 36 and 90 through Math_UnsignedMulHigh with their reciprocals (calls the
// source makes: agscc emits __modsi3 and __divsi3 for the operators);
// Iwram_TransformVector at the comet sparks; formation resource 646; the effect
// argument for the actor loops; gCameraSceneParameters declared unsized (with
// the shared u32[3], word 3 folds into the address).

// What fixed the frame and slots: screen.y >> 16 and screen.depth >>= 16 as the
// twin writes them (combine makes the ldrsh off the pointer to screen; a cast
// to s16 * gave screen + 6 its own register and two spills); the twin's
// declaration order; wave and drift in a block opened after the blitter fetch,
// whose spill slots fall below the pointer to draw.

#include "input.h"

struct FxPartyList;

void BattleEffect_SelectLivingTargets(
    struct FxPartyList *party); // ..SelectLivingTargets
void BattleEffect_SpawnObjects(s32 entryCount, s32 kind,
                               u32 variant); // ..SpawnObjects
void Func_0814cca8(void);                    // BattleEffect_SetupBlendedDisplay

s32 ArcTan2(s32 x, s32 y);
u32 Math_UnsignedMulHigh(u32 value, u32 magic);
s32 Math_UnsignedMulHighAdd(s32 value, u32 magic, s32 multiplier);

struct Point2D {
  s32 x;
  s32 y;
};

struct Scale {
  s32 x;
  s32 y;
};

extern char ResourceId_WhirlwindSheet;

extern char ResourceId_VioletSkyBackdrop;
extern char ResourceId_VioletCometSheet;

extern struct Scale Data_08196df8; // the comets' unit scale
extern u8 Data_0819793e[];         // the nine comets' grid: x
extern u8 Data_08197947[];         // ... and depth
extern u16 Data_08197950[];        // the four comet strikes' frames
extern u8 Data_08197958[];         // a strike's three pictures: widths
extern u8 Data_0819795b[];         // ... heights
extern u8 Data_0819795e[];         // ... raise above the mark
extern u16 Data_08197962[];        // ... cell offsets

// frame / 4 % 3, as the source writes it: the multiply-high routine with the
// reciprocal of 3, where a plain % calls __modsi3.
#define MOD3(v) Math_UnsignedMulHighAdd((v), 0x55555556, -3)

#define WHIRLWIND_CEL (48 * 64) // one of the whirlwind's three frames
#define DUST_SHEET 0x2400       // the dust motes, after the whirlwind
#define COMET_SHEET 0x3600      // the violet comets
#define COMETS 9
#define FORMATION_RESOURCE 646

// Summon 8: a whirlwind rises under the enemies and lifts them, spinning,
// while dust motes swirl up through it (frames 0 to 159; A or B skips to
// 160). Then the sky turns violet and a formation of nine comets wheels
// in; a cloud of sparks gathers on its centre, and from frame 330 four
// comets strike one after another, each with a burst of sparks, throwing
// the enemies down. A or B skips frames 161 to 394.
void BattleEffect_RunProcne(struct BattleEffectArgument *effect) {
  void *canvas;
  void *camera;
  struct BattleEffectWork *work;
  DrawRectangle draw[2];
  s32 frame;
  u8 *graphics;
  struct MotionObject *object;
  struct BattleObj *context;
  struct EffectStep *point;
  struct EffectStep *spark;
  struct EffectStep *mark;
  s32 index;
  s32 slot;
  s32 size;
  s32 span;
  s32 dist;
  s32 alive;
  s32 heading[8];
  s32 radius[8];
  s32 spin[8];
  struct Point2D home[8];
  s32 baseAngle[8];
  s32 vector[3];
  struct EffectPosition screen;
  s32 shake[3];
  s32 place[4];
  struct Scale scale;

  canvas = Ram_HeapSlots->battleFxWork[1];
  camera = Ram_HeapSlots->cameraWork;
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  graphics = Ram_HeapSlots->unk64[0];

  // Set up: the max and additive pair, the whirlwind with its palette, the
  // dots, and the transfer at half strength.
  BattleEffect_StartCanvasLayer(0);
  BattleEffect_FetchRectangleBlitters(0, draw);
  {
    struct Point2D wave;
    struct Point2D drift;
    Resource_LoadAndDecompress((s32)&ResourceId_WhirlwindSheet, work->sheet, 1,
                               1);
    Resource_LoadAndDecompress((s32)&ResourceId_Data134, graphics, 0, 0);
    work->transferMode = 2;
    work->transferValue = 50;
    Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);
    wave.x = 0x800000;
    wave.y = 0x280000;
    drift.x = 0;
    drift.y = 0;

    // Each enemy's place, heading, distance and facing, to restore at the
    // end; its bobbing stops.
    for (index = 0; index != effect->count; index++) {
      context = GetBattleObjectSlotFar(effect->actors[index]);
      object = context->object;
      home[index].x = object->x;
      home[index].y = object->z;
      baseAngle[index] = object->angle;
      heading[index] = (u16)ArcTan2(object->x, object->z);
      radius[index] = Iwram_Sqrt((object->x >> 8) * (object->x >> 8) +
                                 (object->z >> 8) * (object->z >> 8)) >>
                      7;
      spin[index] = 0;
      object->verticalMotionStrength = 0;
    }

    // The dust motes start at the whirlwind's foot, each after its own wait.
    index = 0;
    do {
      point = &work->particles[index];
      point->z = 120;
      point->y = 0;
      Random16();
      point->velocityY = 0;
      point->velocityX = 0;
      point->variant = Random16() & 63;
      index++;
    } while (index != 64);

    frame = 0;
    do {
      if ((gInput.repeat & 3) != 0) {
        if (frame <= 159)
          frame = 160;
        else if (frame <= 394)
          frame = 395;
      }

      Render_ResetTransformState();
      Transform_SetLookAt(camera, (s32 *)((u8 *)camera + 12));

      if (frame == 16)
        Sound_PlayFar(141);
      if (frame == 256)
        Sound_PlayFar(140);
      if (frame == 334)
        Sound_PlayFar(212);
      if (frame == 347)
        Sound_PlayFar(212);
      if (frame == 359)
        Sound_PlayFar(212);
      if (frame == 372)
        Sound_PlayFar(212);

      // The enemies, one every 16 frames, spiral in toward the centre and
      // rise, spinning faster; each comet strike knocks them down.
      for (index = 0; index != effect->count; index++) {
        if (frame > index * 16) {
          context = GetBattleObjectSlotFar(effect->actors[index]);
          object = context->object;
          object->x = (radius[index] * Trig_Sin(heading[index])) >> 1;
          object->z = (radius[index] * Trig_Cos(heading[index])) >> 1;

          if (frame <= 159) {
            if (frame > index * 16 + 16)
              spin[index] += 48;
            if (radius[index] <= 31)
              object->y += 0x60000;
            else
              object->y += 0x8000;
            if (object->y > 0x7c0000)
              object->y = 0x7c0000;
          }

          if (frame <= 490) {
            heading[index] += spin[index];
            if (heading[index] > 0x10000)
              heading[index] -= 0x10000;
            object->angle += spin[index] / 4;
          }

          if (frame == 395)
            object->verticalMotionStrength = 0xab85;

          slot = 0;
          do {
            if (frame == Data_08197950[slot]) {
              object->velocityY = 0;
              Func_0814cd48(effect->actors[index], 7, 5, index, 8);
            }
            slot++;
          } while (slot != 4);

          if (radius[index] > 16)
            radius[index] -= 2;
        }
      }

      // The whirlwind: its foot grows from frame 16, its top from 48, and
      // from 160 it stands whole, two cels high.
      if ((u32)(frame - 16) <= 143) {
        size = frame * 2 - 32;
        if (size > 48)
          size = 48;
        draw[0](
            canvas,
            &work->sheet[MOD3(frame / 4) * WHIRLWIND_CEL + (48 - size) * 48],
            32, 112 - size, 48, size);
      }
      if ((u32)(frame - 48) <= 111) {
        size = frame * 2 - 96;
        if (size > 64)
          size = 64;
        draw[0](
            canvas,
            &work->sheet[MOD3(frame / 4) * WHIRLWIND_CEL + (64 - size) * 48],
            32, 64 - size, 48, size);
      }
      if ((u32)(frame - 160) <= 239) {
        draw[0](canvas, &work->sheet[MOD3(frame / 4) * WHIRLWIND_CEL], 8, 0, 48,
                64);
        draw[0](canvas, &work->sheet[MOD3(frame / 4) * WHIRLWIND_CEL], 8, 64,
                48, 64);
      }

      // The dust motes swirl up the whirlwind, speeding up, and start again
      // from the foot when they pass the top.
      if (frame <= 159) {
        index = 0;
        point = work->particles;
        do {
          if (point[index].variant == 0) {
            vector[0] = point[index].z * Trig_Sin(point[index].velocityY);
            vector[1] = point[index].y;
            vector[2] = point[index].z * Trig_Cos(point[index].velocityY);
            EffectPosition_ProjectPoint(vector, &screen);
            screen.x >>= 1;
            if (point[index].y <= 0x3fffff)
              draw[0](canvas, work->sheet + DUST_SHEET, screen.x - 12,
                      screen.y - 12, 16, 16);
            if (point[index].z > 24)
              point[index].z -= 4;
            point[index].velocityY += point[index].velocityX * 2;
            if (point[index].velocityY > 0x10000)
              point[index].velocityY -= 0x10000;
            point[index].velocityX += 50;
            if (point[index].velocityX > 4096)
              point[index].velocityX = 4096;
            point[index].velocityY += 2048;
            point[index].y += point[index].velocityY * 2;
            if (point[index].y > 0x300000) {
              point[index].y = 0;
              point[index].z = 100;
              point[index].velocityY = 0;
              point[index].velocityX = 0;
            }
          } else {
            point[index].variant--;
          }
          index++;
        } while (index != 24);
      }

      // Frame 160: the violet sky, the formation of comets, sparks scattered
      // round its centre, and the enemies dropped back at random heights.
      if (frame == 160) {
        s32 speed;
        s32 angle;

        BattleBackground_LoadFar(1, (s32)&ResourceId_VioletSkyBackdrop, 0);
        BattleStaging_ConfigurePaletteFadeFar(
            1, (s32)&ResourceId_VioletSkyBackdrop, 8);
        BattleEffect_SelectLivingTargets(work->effect);
        BattleEffect_SpawnObjects(COMETS, FORMATION_RESOURCE, 2);
        Resource_LoadAndDecompress((s32)&ResourceId_VioletCometSheet,
                                   work->sheet + COMET_SHEET, 1, 1);

        index = 0;
        do {
          PARTICLES[index].variant = 0;
          index++;
        } while (index != 512);

        for (index = 0; index != effect->count; index++) {
          context = GetBattleObjectSlotFar(effect->actors[index]);
          object = context->object;
          object->y = ((Random16() & 15) + 8) << 16;
        }

        gCameraSceneParameters[3] = 72;

        index = 0;
        do {
          speed = Random16() & 127;
          angle = (Random16() & 0x7fff) + 0x9fff;
          work->particles[index].x = Trig_Sin(angle) * speed;
          work->particles[index].y = Trig_Cos(angle) * speed;
          work->particles[index].z = (Random16() % 200 - 100) << 16;
          work->particles[index].variant = 0;
          index++;
        } while (index != 32);

        for (index = 0; index != effect->count; index++) {
          radius[index] = 16;
          spin[index] = 6000;
        }
      }

      if (frame > 159) {
        // The formation sways on a slow wave, pushed back by each strike.
        scale = Data_08196df8;
        size = 16;
        if (frame - 160 <= 64)
          size = 32;
        wave.x -= (Trig_Sin((frame - 160) << 7) * 32) >> 6;
        wave.y += (Trig_Sin((frame - 160) << 9) * size) >> 6;
        wave.x += drift.x;
        wave.y += drift.y;
        drift.x /= 2;
        drift.y /= 2;

        scale.x = 0x10000;
        scale.y = 0x10000;
        place[3] = 0;
        index = 0;
        do {
          place[0] = (Data_0819793e[index] << 16) + wave.x + 0x280000;
          place[1] = 0xff0000 + index * 0x400000;
          place[2] = ((Data_08197947[index] + index * 64) << 16) + wave.y;
          Render_ApplyProjectedPlacementFar(
              (struct ScaledSprite *)work->objects[index], place, &scale.x, 0);
          index++;
        } while (index != COMETS);

        if (frame > 255) {
          s32 threshold;

          shake[2] = 0x1000000;
          alive = 0;
          shake[0] = 0;
          shake[1] = 0;
          Render_ResetTransformState();
          SceneTransform_ApplyPosition(shake);

          // Until frame 330 the scattered sparks gather on the centre, each
          // a part of its distance a frame; those that have arrived make one
          // spark that grows with their number.
          if (frame <= 329) {
            index = 0;
            do {
              s32 xx;
              s32 yy;
              s32 zz;

              point = &work->particles[index];
              xx = (point->x >> 8) * (point->x >> 8);
              yy = (point->y >> 8) * (point->y >> 8);
              zz = (point->z >> 8) * (point->z >> 8);
              dist = Iwram_Sqrt(xx + yy + zz) >> 9;
              if (dist != 0) {
                EffectPosition_ProjectPoint(&point->x, &screen);
                screen.x = (screen.x >> 17) + (wave.x >> 17) + 32;
                screen.y = (screen.y >> 16) + (wave.y >> 16) - 4;
                screen.depth >>= 16;
                if (screen.depth <= 169)
                  screen.depth = 170;
                if (screen.depth > 350)
                  screen.depth = 350;
                span = 6 - Math_UnsignedMulHigh(screen.depth - 170, 0x071c71c8);
                draw[1](canvas, graphics + gData_08197410[span - 1],
                        screen.x - span / 2, screen.y - span, span, span * 2);
                point->x -= point->x / dist;
                point->y -= point->y / dist;
                point->z -= point->z / dist;
              } else {
                alive++;
              }
              index++;
            } while (index != 32);

            if (alive > 0) {
              span = alive / 10 + 1;
              draw[1](canvas, graphics + gData_08197410[span - 1],
                      (wave.x >> 17) - span / 2 + 32, (wave.y >> 16) - span - 4,
                      span, span * 2);
            }
          }

          // Each strike throws out 96 sparks that fly and fade.
          slot = 0;
          do {
            if (frame == Data_08197950[slot]) {
              s32 speed;
              s32 angle;

              index = 0;
              do {
                speed = (Random16() & 127) + 16;
                angle = Random16() & 0xffff;
                spark = &PARTICLES[slot * 128 + index];
                spark->velocityX = (Trig_Sin(angle) * speed) >> 6;
                spark->velocityY = -(Trig_Cos(angle) * speed) >> 6;
                spark->velocityY = ((Random16() & 255) - 128) << 10;
                spark->x = 0;
                spark->y = 0;
                spark->z = 0;
                spark->variant = (Random16() & 15) + 64;
                index++;
              } while (index != 96);
            }

            if (frame >= Data_08197950[0]) {
              spark = PARTICLES;
              index = 0;
              do {
                if (spark[index].variant > 0) {
                  // The transform in a once-only block, which ends CSE's block
                  // before it; as a plain block, 6 of 1493 instructions differ
                  // in tla-en.
                  do {
                    void (*transform)(const void *, s32 *) =
                        Iwram_TransformVector;
                    /* FAKEMATCH(pin): the result pointer in r1, set before the
                     * point in r0; as a plain argument, 2 of 1493 instructions
                     * differ in tla-en. */
                    register s32 *result __asm__("r1") = &screen.x;
                    transform(&spark[index].x, result);
                  } while (0);
                  screen.x = (screen.x >> 17) + 32;
                  screen.y = (screen.y >> 16) + 56;
                  screen.depth >>= 16;
                  if (screen.depth <= 169)
                    screen.depth = 170;
                  if (screen.depth > 350)
                    screen.depth = 350;
                  span =
                      3 - Math_UnsignedMulHigh(screen.depth - 170, 0x02d82d83);
                  draw[0](canvas, graphics + gData_08197410[span - 1],
                          screen.x - span / 2, screen.y - span, span, span * 2);
                  spark[index].x += spark[index].velocityX;
                  spark[index].y += spark[index].velocityY;
                  spark[index].z += spark[index].velocityY;
                  spark[index].variant--;
                }
                index++;
              } while (index != 512);
            }
            slot++;
          } while (slot != 4);

          // From frame 330, a comet every twelve frames: it strikes at the
          // formation's centre, the screen shakes and the formation is
          // pushed back; a ring of 28 sparks opens round the mark.
          index = 0;
          do {
            mark = &work->particles[index + 60];
            threshold = index * 12 + 330;
            if (frame == threshold) {
              mark->x = (wave.x >> 17) + 32;
              mark->y = (wave.y >> 16) - 4;
              mark->velocityX = (wave.x >> 17) + 32;
              mark->velocityY = (wave.y >> 16) - 4;

              slot = 0;
              do {
                work->particles[slot + 32].variant = 4;
                slot++;
              } while (slot != 28);

              drift.x = 0x80000;
              drift.y = -0x20000;
              work->shakeFrames = 8;
            }

            if (frame >= threshold) {
              s32 cel;

              cel = (frame - threshold) / 2;
              if (cel > 2)
                cel = 2;
              draw[1](canvas, work->sheet + Data_08197962[cel], mark->x,
                      mark->y - Data_0819795e[cel], Data_08197958[cel],
                      Data_0819795b[cel]);
              mark->x -= 8;
              mark->y += 2;

              if (frame < threshold + 8) {
                Func_080150e4(-2048);
                SceneTransform_ApplyYaw(-4096);

                slot = 0;
                spark = &work->particles[32];
                do {
                  struct EffectStep *ring;

                  ring = &spark[slot];
                  vector[0] = 0;
                  vector[1] = ring->variant * Trig_Cos(slot * 2340);
                  vector[2] = ring->variant * Trig_Sin(slot * 2340);
                  ring->variant += 2;
                  EffectPosition_ProjectPoint(vector, &screen);
                  screen.x =
                      (screen.x >> 17) + work->particles[index + 60].velocityX;
                  screen.y =
                      (screen.y >> 16) + work->particles[index + 60].velocityY;
                  screen.depth >>= 16;
                  if (screen.depth <= 169)
                    screen.depth = 170;
                  if (screen.depth > 350)
                    screen.depth = 350;
                  span =
                      3 - Math_UnsignedMulHigh(screen.depth - 170, 0x02d82d83);
                  draw[0](canvas, graphics + gData_08197410[span - 1],
                          screen.x - span / 2, screen.y - span, span, span * 2);
                  slot++;
                } while (slot != 28);
              }
            }
            index++;
          } while (index != 4);
        }
      }

      BattleEffect_CountDownBattlerTints();
      work->transferPending = 1;
      wait(1);
      frame++;
    } while (frame != 400);

    // The results; the enemies go back where they stood, facing as before.
    Func_081180e8(134);
    for (index = 0; index != effect->count; index++) {
      struct MotionObject *enemy;

      context = GetBattleObjectSlotFar(effect->actors[index]);
      enemy = context->object;
      enemy->x = home[index].x;
      enemy->z = home[index].y;
      enemy->angle = baseAngle[index];
    }
    gCameraSceneParameters[3] = 120;
    Func_0814cca8();

    index = 0;
    do {
      ResourceObject_ReleaseFar(work->objects[index]);
      index++;
    } while (index != COMETS);

    Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
    HeapSlot_Release(188);
    HeapSlot_Release(104);
    BattleEffect_EndCanvasLayer();
  }
}

void Func_08147aec(struct BattleEffectArgument *effect, s32 mode);

/// Abilities: Dragon Driver (kind 385).
DEFINE_EFFECT_KIND(BattleEffect_RunDragonDriver, Func_08147aec, 1)

/// Abilities: Tiamat (scene kind 7).
DEFINE_EFFECT_KIND(BattleEffect_RunTiamat, Func_08147aec, 0)

#undef FORMATION_RESOURCE

// The rising grains live in the map buffer.

// The high halfword of a 16.16 value: its whole pixels.
#define HI(v) (((s16 *)&(v))[1])

void Func_0816442c(s32 formation, s32 x, s32 y);  // ..PlaceFormationObjects
void Func_0816467c(s32 formation, s32 x, s32 y);  // ..RunImpactBurst
void Func_08164a4c(s32 blue, s32 green, s32 red); // Palette_BrightenBgEntries
void Animation_ApplyChildArgumentFar(void *object, s32 animation);

extern char ResourceId_FlashBurstSheet;
extern char ResourceId_EmberStreakSheet;

extern char ResourceId_LightningBoltSheet;
extern u8 Data_08197968[];  // a grain's added speed, by grain
extern u8 Data_0819796c[];  // a spark's two pixels
extern u8 Data_0819796e[];  // a grain's size, by grain
extern u8 Data_08197972[];  // the streak's six pictures: widths
extern u8 Data_08197978[];  // ... heights
extern u8 Data_0819797e[];  // ... drops below the streak
extern u16 Data_08197984[]; // ... cell offsets

#define STREAK_SHEET 0x3000 // the ember streaks, after the flashes
#define FORMATION_RESOURCE 647

// Summon 7 (mode 0; mode 1 is the same scene cast from the caster's side,
// without the formation): streaks rain down and splash, sparks and grains
// rise from where the formation stands, a burst of flashes at frame 128,
// then the screen whitens and the canvas fades away. In mode 0 a formation
// of eight drops in at frame 40 and bursts at the end. A or B skips frames
// 49 to 159.
void Func_08147aec(struct BattleEffectArgument *effect, s32 mode) {
  struct EffectPosition origin;
  DrawRectangle draw[2];
  void *canvas;
  s32 *camera;
  struct BattleEffectWork *work;
  s32 frame;
  u8 *sheet;
  s32 i;

  canvas = Ram_HeapSlots->battleFxWork[1];
  camera = Ram_HeapSlots->cameraWork;
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  sheet = Ram_HeapSlots->unk64[0];

  // Set up: half-and-half blending, the max and additive pair, the flashes
  // with their palette, the streaks, the dots, and in mode 1 the lightning
  // palette.
  BattleEffect_StartCanvasLayer(0);
  REG_BLDALPHA = 0x1010;
  REG_BG2CNT = 0x784;
  BattleEffect_FetchRectangleBlitters(0, draw);
  Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet, work->sheet, 1,
                             1);
  Resource_LoadAndDecompress((s32)&ResourceId_EmberStreakSheet,
                             work->sheet + STREAK_SHEET, 1, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, sheet, 0, 0);
  if (mode == 1) {
    const void *lightning =
        Resource_GetTableEntry((s32)&ResourceId_LightningBoltSheet);
    s32 (*copy)(void *destination, const void *source, s32 size) =
        Iwram_CopyWords;

    // The entry held in a local loads ahead of the destination's shift; as a
    // plain Iwram_CopyWords call, 2 of 1072 instructions differ in tla-en.
    copy((void *)BG_PLTT, lightning, 128);
  }
  work->transferMode = 2;
  work->transferValue = 75;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);
  {
    struct Point2D focus;
    s32 dir;
    s32 alternate;

    // When the first target's animation kind is 2 the formation stands 8
    // pixels further right and lower, with its sparks and flashes.
    alternate = 0;

    if (((struct Actor *)GetBattleObjectSlotFar(effect->actors[0])->object)
            ->animationKind == 2)
      alternate = 1;

    // Mode 1 on the far side mirrors everything sideways, and starts from
    // the caster.
    if (mode == 0)
      dir = 1;
    else if (effect->side == 1)
      dir = -1;
    else
      dir = 1;
    if (mode == 1) {
      EffectPosition_ProjectScaledBattler(effect->actor, &origin);
      origin.x /= 2;
      origin.y = 66;
      if (effect->side == 1)
        origin.x = 76;
      else
        origin.x = 44;
    }

    // The formation enters from above the right edge.
    if (alternate == 1) {
      focus.x = 220 << 16;
      focus.y = -(52 << 16);
    } else {
      focus.x = 212 << 16;
      focus.y = -(60 << 16);
    }
    for (i = 0; i != 64; i++)
      work->particles[i].variant = -1;
    for (i = 0; i != 16; i++) {
      struct EffectStep *streak = &work->particles[24 + i];

      if (dir == 1)
        streak->x = (Random16() & 127) + 128;
      else
        streak->x = (Random16() & 127) - 128;
      streak->y = (Random16() & 7) - 72;
      streak->variant = -(Random16() & 31); // frames before it falls
    }
    for (i = 0; i != 512; i++)
      PARTICLES[i].variant = -1;
    if (mode == 0) {
      BattleEffect_SelectLivingTargets(work->effect);
      wait(1);
      BattleEffect_SpawnObjects(8, FORMATION_RESOURCE, 2);
    }

    for (frame = 0; frame != 208; frame++) {
      if ((gInput.repeat & 3) && frame > 48 && frame <= 159) {
        if (mode == 0) {
          Animation_ApplyChildArgumentFar(work->objects[0], 8);
          Animation_ApplyChildArgumentFar(work->objects[1], 9);
          Animation_ApplyChildArgumentFar(work->objects[3], 10);
          Animation_ApplyChildArgumentFar(work->objects[4], 11);
        }
        for (i = 0; i != effect->count; i++) {
          Func_0814cd48(effect->actors[i], 10, 5, -1, 0);
          Func_08118088(effect->actors[i], 4);
        }
        frame = 160;
      }
      Render_ResetTransformState();
      Transform_SetLookAt(camera, camera + 3);
      if (frame == 178)
        Func_081180e8(134);

      // From frame 128 the trails halve; from 160 to 175 the canvas is filled
      // and the palette brightens; from 176 the canvas fades by 2 a frame.
      if (frame == 128)
        work->transferValue = 50;
      if (frame == 176) {
        work->transferMode = 3;
        work->transferValue = 0x02020202;
        Iwram_CopyWords(
            (void *)BG_PLTT,
            Resource_GetTableEntry((s32)&ResourceId_EmberStreakSheet), 128);
      } else if (frame >= 160 && frame <= 175) {
        work->transferMode = 1;
        work->transferValue = 0x10101010;
        if (frame > 173)
          work->transferValue = 0x3f3f3f3f;
        Func_08164a4c(2, 2, 2);
      }

      // Grains rise from y = 112: one a frame from 33, eight from 104.
      if (frame >= 33 && frame <= 175) {
        s32 spawned = 0;
        s32 limit = 1;

        if (frame > 103)
          limit = 8;
        for (i = 0; i != 512; i++) {
          struct EffectStep *grain = &PARTICLES[i];

          if (grain->variant == -1) {
            grain->x = ((Random16() & 255) - 32) << 16;
            grain->y = 112 << 16;
            grain->velocityX = ((Random16() & 127) + Data_08197968[i & 3]) << 9;
            grain->velocityY = -((Random16() & 127) + Data_08197968[i & 3])
                               << 11;
            grain->variant = 0;
            spawned++;
            if (spawned == limit)
              break;
          }
        }
      }

      // Sparks fly up and out from the formation, one every other frame.
      if (frame >= 41 && frame <= 127) {
        s32 spawned = 0;

        if (frame & 1) {
          for (i = 0; i != 24; i++) {
            struct EffectStep *spark = &work->particles[40 + i];

            if (spark->variant == -1) {
              s32 speed = (Random16() & 255) + 128;
              s32 angle = (Random16() & 0x1fff) + 20000;

              if (mode == 0) {
                if (alternate == 1) {
                  spark->x = ((Random16() & 7) + 78) << 16;
                  spark->y = 78 << 16;
                } else {
                  spark->x = ((Random16() & 7) + 78) << 16;
                  spark->y = 70 << 16;
                }
              } else {
                spark->x = ((Random16() & 7) + origin.x - 8) << 16;
                spark->y = origin.y << 16;
              }
              spark->velocityX = speed * Trig_Sin(angle) >> 9;
              spark->velocityY = speed * Trig_Cos(angle) >> 9;
              spark->variant = 0;
              spawned++;
              if (spawned == 1)
                break;
            }
          }
        }
      }
      if (frame == 48)
        Sound_PlayFar(141);
      if (frame == 128)
        Sound_PlayFar(145);

      // Flashes fly out sideways, one a frame from 129.
      if (frame >= 129 && frame <= 175) {
        s32 spawned = 0;

        for (i = 0; i != 24; i++) {
          struct EffectStep *flash = &work->particles[i];

          if (flash->variant == -1) {
            s32 speed = (Random16() & 255) + 128;
            s32 angle = (Random16() & 0x1fff) - 20000;

            if (mode == 0) {
              if (alternate == 1) {
                flash->x = 72 << 16;
                flash->y = 72 << 16;
              } else {
                flash->x = 68 << 16;
                flash->y = 64 << 16;
              }
            } else {
              flash->x = origin.x << 16;
              flash->y = origin.y << 16;
            }
            flash->velocityX = speed * Trig_Sin(angle) >> 6;
            flash->velocityY = speed * Trig_Cos(angle) >> 6;
            flash->variant = 0;
            spawned++;
            if (spawned == 1)
              break;
          }
        }
      }

      // A flash steps its picture every four frames and lives 24.
      if (frame <= 175) {
        for (i = 0; i != 24; i++) {
          struct EffectStep *flash = &work->particles[i];

          if (flash->variant >= 0) {
            draw[0](canvas, work->sheet + ((flash->variant >> 2) << 11),
                    HI(flash->x) - 16, HI(flash->y) - 32, 32, 64);
            flash->x += flash->velocityX * dir;
            flash->y += flash->velocityY;
            flash->variant++;
            if (flash->variant == 24)
              flash->variant = -1;
          }
        }
      }

      // A spark is two pixels, speeding upward, and lives 48 frames.
      for (i = 0; i != 24; i++) {
        struct EffectStep *spark = &work->particles[40 + i];
        s32 size = 1;

        if (spark->variant >= 0) {
          draw[1](canvas, Data_0819796c, HI(spark->x), HI(spark->y) - size,
                  size, size * 2);
          spark->x += spark->velocityX * dir;
          spark->y += spark->velocityY;
          spark->velocityY += -0x400;
          spark->variant++;
          if (spark->variant == 48)
            spark->variant = -1;
        }
      }

      // Grains speed upward until frame 128, then bend away sideways (the odd
      // ones harder); each lives 256 frames.
      if (frame <= 175) {
        for (i = 0; i != 512; i++) {
          struct EffectStep *grain = &PARTICLES[i];

          if (grain->variant >= 0) {
            u8 size = Data_0819796e[i & 3];
            s32 height = size * 2;

            draw[0](canvas, sheet + gData_08197410[size - 1],
                    HI(grain->x) - size / 2, HI(grain->y) - size, size, height);
            grain->x += grain->velocityX * dir;
            grain->y += grain->velocityY;
            if (frame > 128) {
              if (i & 1)
                grain->velocityX += -0x8000;
              else
                grain->velocityX += -0x2000;
            } else {
              grain->velocityY += -0x400;
            }
            grain->variant++;
            if (grain->variant == 256)
              grain->variant = -1;
          }
        }
      }

      // The shakes, and the formation: it drops in over frames 40 to 47 and
      // turns to face the targets from 128 to 175.
      if (frame == 128)
        work->shakeFrames = 48;
      if (mode == 0 && frame == 48)
        work->shakeFrames = 8;
      if (frame >= 40 && frame <= 47) {
        focus.x += -(8 << 16);
        focus.y += 16 << 16;
      }
      if (mode == 0) {
        if (frame == 128) {
          Animation_ApplyChildArgumentFar(work->objects[0], 8);
          Animation_ApplyChildArgumentFar(work->objects[1], 9);
          Animation_ApplyChildArgumentFar(work->objects[3], 10);
          Animation_ApplyChildArgumentFar(work->objects[4], 11);
        }
        if (frame == 176) {
          Animation_ApplyChildArgumentFar(work->objects[0], 0);
          Animation_ApplyChildArgumentFar(work->objects[1], 1);
          Animation_ApplyChildArgumentFar(work->objects[3], 3);
          Animation_ApplyChildArgumentFar(work->objects[4], 4);
        }
        if (alternate == 1)
          Func_0816442c(6, focus.x, focus.y);
        else
          Func_0816442c(3, focus.x, focus.y);
      }
      if (frame == 138) {
        for (i = 0; i != effect->count; i++) {
          Func_0814cd48(effect->actors[i], 10, 5, -1, 0);
          Func_08118088(effect->actors[i], 4);
        }
      }

      // A streak falls six pixels a frame after its wait, then below y = 55
      // splashes through six pictures, two frames each, over and over.
      if (frame <= 175) {
        for (i = 0; i != 16; i++) {
          struct EffectStep *streak = &work->particles[24 + i];

          if (streak->y > 55) {
            if (streak->variant >= 0 && streak->variant <= 11) {
              s32 cel = streak->variant / 2;

              draw[0](canvas, work->sheet + Data_08197984[cel] + STREAK_SHEET,
                      streak->x - Data_08197972[cel] / 2,
                      streak->y + Data_0819797e[cel], Data_08197972[cel],
                      Data_08197978[cel]);
            }
            streak->variant++;
            if (streak->variant == 12)
              streak->variant = 0;
          } else if (streak->variant == 0) {
            s32 size = 10;

            streak->x -= dir * 6;
            streak->y += 6;
            if (frame <= 47 && streak->y > 55)
              Sound_PlayFar(136);
            draw[0](canvas, sheet + gData_08197410[size - 1],
                    streak->x - size / 2, streak->y + 30, size, size * 2);
          } else {
            streak->variant++;
          }
        }
      }
      BattleCamera_Shake(8, 8);
      BattleEffect_CountDownBattlerTints();
      work->transferPending = 1;
      wait(1);
    }

    // Tear down: the formation bursts where it stands and is released.
    Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
    HeapSlot_Release(188);
    HeapSlot_Release(104);
    if (mode == 0) {
      if (alternate == 1)
        Func_0816467c(6, focus.x, focus.y);
      else
        Func_0816467c(3, focus.x, focus.y);
    }
    if (mode == 0) {
      for (i = 0; i != 8; i++)
        ResourceObject_ReleaseFar(work->objects[i]);
    }
  }
  BattleEffect_EndCanvasLayer();
}

#undef FORMATION_RESOURCE

// Nereid, summon 2.

extern char ResourceId_WaterSpraySheet;
extern char ResourceId_FirePillarSheetA;

extern u8 Data_08197990[];  // the three columns' feet, x and y
extern s32 Data_08197998[]; // a falling spark's gravity, by spark

#define SPARK_COUNT 1024
#define FORMATION_SIZE 12
#define FORMATION_RESOURCE 649
#define COLUMN_SHEET 0x6e4 // the fire pillar, after the water spray band
#define COLUMN_WIDTH 34
#define COLUMN_HEIGHT 104

/// Summon 2: a formation of twelve sweeps in from the right and settles over
/// the enemies while a band of spray rises from below. At frame 28, and every
/// frame from 32 to 79, sparks burst up from beside the formation and fall
/// back; three columns of fire stream up meanwhile, and every twelve frames
/// the targets flash and jump. After frame 80 the sparks rise away. A or B
/// skips frames 33 to 97.
/// Nereid (summon 2, BattleEffect_RunSceneEffect's case 2), ported call for
/// call from The Broken Seal's BattleEffect_RunNereid. This game reads its heap
/// blocks canvas, work, sheet; loads three resources where The Broken Seal
/// copies a palette and decodes two sheets; takes its blitter pair from
/// BattleEffect_FetchRectangleBlitters and its formation from resource 649; and
/// reads the effect argument in the hit loop.
void BattleEffect_RunNereid(struct BattleEffectArgument *effect) {
  DrawRectangle draw[2];
  void *canvas;
  struct BattleEffectWork *work;
  u8 *graphics;

  canvas = Ram_HeapSlots->battleFxWork[1];
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  graphics = Ram_HeapSlots->unk64[0];

  // Set up: half-and-half blending, the max and additive blitters, the
  // spray band and the fire column into the work sheet, the sparks into the
  // sheet slot, and the transfer at three quarters.
  BattleEffect_StartCanvasLayer(0);
  REG_BLDALPHA = 0x1010;
  BattleEffect_FetchRectangleBlitters(0, draw);
  Resource_LoadAndDecompress((s32)&ResourceId_WaterSpraySheet, work->sheet, 1,
                             1);
  Resource_LoadAndDecompress((s32)&ResourceId_FirePillarSheetA,
                             work->sheet + COLUMN_SHEET, 1, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, graphics, 0, 0);
  work->transferMode = 2;
  work->transferValue = 75;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);
  {
    struct Point2D focus;
    struct Point2D velocity;
    s32 rise;
    s32 frame;
    s32 k;
    struct EffectStep *point;
    struct EffectStep *spark;
    struct EffectStep *sparks;
    s32 *life;

    // The formation enters from (256, 88), moving left and up.
    rise = 0;
    focus.x = 256 << 16;
    focus.y = 88 << 16;
    velocity.x = -(16 << 16);
    velocity.y = -(4 << 16);
    for (k = 0; k != 64; k++)
      work->particles[k].variant = -1;
    for (k = 0; k != 16; k++) {
      point = &work->particles[24 + k];
      point->x = Random16() & 127;
      point->y = (Random16() & 7) + 56;
      point->variant = -(Random16() & 15);
    }

    // Every spark in the map buffer starts free (variant -1).
    life = &((struct EffectStep *)Ram_MapCellBuffer)->variant;
    for (k = 0; k != SPARK_COUNT; k++) {
      *life = -1;
      life += sizeof(struct EffectStep) / 4;
    }
    sparks = (struct EffectStep *)Ram_MapCellBuffer;
    BattleEffect_SelectLivingTargets(work->effect);
    wait(1);
    BattleEffect_SpawnObjects(FORMATION_SIZE, FORMATION_RESOURCE, 2);

    for (frame = 0; frame != 124; frame++) {
      if ((gInput.repeat & 3) && frame > 32 && frame <= 97)
        frame = 98;
      if (frame == 120)
        Func_081180e8(134);
      if (frame <= 15)
        rise += 2;

      // The formation slows (x 58/64, y 56/64 each frame) and is pushed
      // back right once it passes x = 120.
      if (frame <= 99) {
        focus.x += velocity.x;
        focus.y += velocity.y;
        velocity.x = velocity.x * 58 / 64;
        velocity.y = velocity.y * 56 / 64;
        if (focus.x < 120 << 16)
          velocity.x += 0x8000;
      }
      Func_0816442c(1, focus.x, focus.y);

      // Sparks burst up from around (32, 96): every free one at frame 28,
      // then sixteen a frame from 32 to 79.
      if (frame == 28) {
        for (k = 0; k != 256; k++) {
          spark = &sparks[k];
          if (spark->variant == -1) {
            s32 radius = Random16() & 63;
            s32 angle = Random16() & 0xffff;

            spark->x = ((Trig_Sin(angle) * radius) >> 3) + (32 << 16);
            spark->y = ((Trig_Cos(angle) * radius) >> 2) + (96 << 16);
            spark->velocityX = ((Random16() & 63) - 32) << 14;
            spark->velocityY = (-(Random16() & 63) - 8) << 13;
            spark->variant = 0;
          }
        }
      }
      if (frame >= 32 && frame < 80) {
        s32 spawned = 0;

        for (k = 0; k != SPARK_COUNT; k++) {
          spark = &sparks[k];
          if (spark->variant == -1) {
            s32 radius = Random16() & 63;
            s32 angle = Random16() & 0xffff;

            spark->x = ((Trig_Sin(angle) * radius) >> 3) + (32 << 16);
            spark->y = ((Trig_Cos(angle) * radius) >> 2) + (96 << 16);
            spark->velocityX = ((Random16() & 63) - 32) << 14;
            spark->velocityY = (-(Random16() & 63) - 8) << 13;
            spark->variant = 0;
            spawned++;
            if (spawned == 16)
              break;
          }
        }
      }
      if (frame == 0)
        Sound_PlayFar(164);
      if (frame == 32)
        Sound_PlayFar(145);
      if (frame == 80)
        Sound_PlayFar(144);

      // Three columns of fire scroll up while the sparks burst.
      if (frame >= 32 && frame < 80) {
        for (k = 0; k != 3; k++) {
          s32 span = (frame * 16 - 256 + k * 25) % COLUMN_HEIGHT;

          draw[0](canvas, work->sheet + COLUMN_SHEET,
                  Data_08197990[k * 2] - COLUMN_WIDTH / 2,
                  Data_08197990[k * 2 + 1] - span - COLUMN_HEIGHT, COLUMN_WIDTH,
                  COLUMN_HEIGHT);
          draw[0](canvas, work->sheet + COLUMN_SHEET,
                  Data_08197990[k * 2] - COLUMN_WIDTH / 2,
                  Data_08197990[k * 2 + 1] - span, COLUMN_WIDTH, span);
        }
      }

      // The spray band rises sixteen pixels and drifts right.
      if (frame <= 95) {
        for (k = 0; k != 5; k++)
          draw[0](canvas, work->sheet, k * 32 + ((frame / 4) & 31) - 32,
                  120 - rise, 32, 32);
      }

      // Each live spark: rising ones are drawn larger and by the max
      // blitter, falling ones by the additive one; all grow from frame 68.
      // They fall under gravity until frame 80, then accelerate upward, and
      // a falling spark that reaches y = 104 is freed.
      for (k = 0; k != SPARK_COUNT; k++) {
        spark = &sparks[k];
        if (spark->variant >= 0) {
          s32 size = k % 3 + 2;

          if (spark->velocityY > 0)
            size += 2;
          if (frame > 68 && size <= 5)
            size = 6;
          if (frame > 70 && size <= 6)
            size = 7;
          if (frame > 72 && size <= 7)
            size = 8;
          if (frame > 74 && size <= 8)
            size = 9;
          if (frame > 76)
            size = 10;
          draw[spark->velocityY > 0](
              canvas, graphics + gData_08197410[size - 1],
              HI(spark->x) - size / 2, HI(spark->y) - size, size, size * 2);
          spark->x += spark->velocityX;
          spark->y += spark->velocityY;
          if (frame > 80)
            spark->velocityY += -0x8000;
          else
            spark->velocityY += Data_08197998[k & 3];
          spark->velocityX = spark->velocityX * 62 / 64;
          spark->velocityY = spark->velocityY * 62 / 64;
          spark->variant++;
          if (spark->velocityY > 0 && HI(spark->y) > 104)
            spark->variant = -1;
        }
      }

      // From frame 30 to 79 the targets flash and are knocked upward every
      // twelve frames, and stop flashing six frames later.
      if (frame <= 79) {
        for (k = 0; k != effect->count; k++) {
          if (frame > 29) {
            s32 phase = frame % 12;

            if (phase == 0) {
              struct MotionObject *target =
                  GetBattleObjectSlotFar(effect->actors[k])->object;

              Func_0814cd48(effect->actors[k], 7, 5, -1, 0);
              target->velocityY = 0x48000;
              target->verticalMotionStrength = 0xab85;
            }
            if (phase == 6)
              Func_0814cd48(effect->actors[k], 0, 5, -1, 0);
          }
        }
      }
      work->transferPending = 1;
      wait(1);
    }

    // Tear down: the formation bursts where it stands and is released.
    Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
    HeapSlot_Release(HEAP_BLITTER_ALTERNATE);
    HeapSlot_Release(HEAP_BLITTER);
    Func_0816467c(1, focus.x, focus.y);
    for (k = 0; k != FORMATION_SIZE; k++)
      ResourceObject_ReleaseFar(work->objects[k]);
  }
  BattleEffect_EndCanvasLayer();
}

#undef HEAP_BLITTER
#undef HEAP_BLITTER_ALTERNATE

#include "heap_release.h"

// Heap slots The Lost Age addresses by byte offset in Ram_HeapSlots.
#define HEAP_BLITTER 104           // ☀️ HEAP_SLOT_BLITTER
#define HEAP_BLITTER_ALTERNATE 188 // ☀️ HEAP_SLOT_BLITTER_ALTERNATE

// The Lost Age numbers its blitter modes in the reverse of The Broken Seal's
// order: Func_08196404 builds additive for 0, max for 1, transparent for 2.
enum { BLITTER_ADDITIVE, BLITTER_MAX, BLITTER_TRANSPARENT, BLITTER_COPY };

#define MOTE_COUNT 30
#define SEAT_FIRST 32 // the particles that hold each target's seat

extern char ResourceId_VortexSheet;

// ☀️ gVortices_Counts: motes, then frames, for each variant.
extern u8 gData_081979a8[];

void Camera_ApplyPhasedDelta(void);

// Thirty motes rise from the caster, swirl, and home in on the targets; each
// one that reaches the ground flashes and knocks back its target and plays a
// three-cel splash where it landed.
void BattleEffect_RunFroth(struct BattleEffectArgument *effect) {
  struct EffectPosition pos;
  struct BattleEffectWork *work;
  void *canvas;
  DrawRectangle draw[2];
  struct MotionObject *source;
  struct MotionObject *object;
  struct EffectStep *point;
  struct EffectStep *seat;
  struct EffectStep *target;
  struct BattleCamera *camera;
  s32 frame;
  s32 y;
  s32 i;
  s8 *landed;

  // Set up: the canvas layer, the vortex sheet and two blitters, the one on
  // the right mirrored.
  landed = (s8 *)Ram_MapCellBuffer;
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  BattleEffect_StartCanvasLayer(1);
  Resource_LoadAndDecompress((s32)&ResourceId_VortexSheet, work->sheet, 1, 1);
  BattleEffect_LoadBlitter(HEAP_BLITTER, BLITTER_MAX << 4 | COMPOSITE_CLIP_X |
                                             COMPOSITE_CLIP_Y);
  draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  BattleEffect_LoadBlitter(HEAP_BLITTER_ALTERNATE,
                           BLITTER_MAX << 4 | COMPOSITE_CLIP_X |
                               COMPOSITE_CLIP_Y | COMPOSITE_FLIP);
  REG_BLDALPHA = BLEND_ALPHA(15, 15);
  draw[1] = (DrawRectangle)Ram_HeapSlots->unkb8[1];

  // The motes start at the caster's chest with random velocities.
  source = GetBattleObjectSlotFar(effect->actor)->object;
  y = source->y + BattleUnit_GetEffectHeightFar(effect->actor);
  for (i = 0; i != MOTE_COUNT; i++) {
    point = &work->particles[i];
    point->x = source->x;
    point->y = y;
    point->z = source->z;
    point->velocityX = ((s32)((Random16() & 255) - 127) << 16) >> 5;
    point->velocityY = ((s32)((Random16() & 127) - 16) << 16) >> 6;
    point->velocityZ = ((s32)((Random16() & 255) - 127) << 16) >> 5;
    point->variant = -1;
    landed[i] = 0;
  }

  // Each target's seat on the ground.
  for (i = 0; i != effect->count; i++) {
    seat = &work->particles[SEAT_FIRST + i];
    object = GetBattleObjectSlotFar(effect->actors[i])->object;
    seat->x = object->x;
    seat->y = 0;
    seat->z = object->z;
  }

  work->cameraDelta = 0;
  work->cameraPhase = 0;
  Task_Add(Camera_ApplyPhasedDelta, TASK_PASS_FRAME);
  work->transferMode = 2;
  work->transferValue = 75;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);
  Sound_PlayFar(164);

  for (frame = 0; frame != gData_081979a8[effect->variant * 2 + 1]; frame++) {
    // The camera turns between frames 17 and 63.
    camera = Ram_HeapSlots->cameraWork;
    if (frame >= 17 && frame < 64)
      work->cameraDelta = 0x180;
    else
      work->cameraDelta = 0;
    if (frame == gData_081979a8[effect->variant * 2 + 1] - 16)
      Func_081180e8(133);
    Render_ResetTransformState();
    Transform_SetLookAt(camera->eye, camera->pos);

    for (i = 0; i != gData_081979a8[effect->variant * 2]; i++) {
      point = &work->particles[i];

      // A mote leaves the caster two frames after the one before it.
      if (frame > i * 2 && landed[i] == 0) {
        EffectPosition_ProjectPoint((s32 *)point, &pos);
        pos.x >>= 1;
        if (pos.depth < 160)
          pos.depth = 160;
        if (pos.depth > 0x31f)
          pos.depth = 0x31f;
        draw[0](canvas, work->sheet + 0xc00, pos.x - 6, pos.y - 12, 12, 24);
        point->x += point->velocityX;
        point->y += point->velocityY;
        point->z += point->velocityZ;
      }

      // After 48 frames it steers toward its target's seat, damped for the
      // first 37 of them, and lands.
      if (frame > i * 2 + 48 && landed[i] == 0) {
        target = &work->particles[SEAT_FIRST + i % effect->count];
        point->velocityX += (target->x - point->x) >> 9;
        point->velocityY += (target->y - point->y) >> 9;
        point->velocityZ += (target->z - point->z) >> 9;
        if (frame < i * 2 + 85) {
          point->velocityX = point->velocityX * 60 / 64;
          point->velocityY = point->velocityY * 60 / 64;
          point->velocityZ = point->velocityZ * 60 / 64;
        }
        if (point->y < 0) {
          landed[i] = 1;
          point->variant = 0;
          point->x = pos.x;
          point->y = pos.y + (Random16() & 31) - 16;
          Func_0814cd48(effect->actors[i % effect->count], 7, 5,
                        i % effect->count, 4);
          Func_08118088(effect->actors[i % effect->count], 0);
          work->shakeFrames = 4;
          Sound_PlayFar(132);
        }
      }

      // The splash: three cels, two frames each, drawn as mirrored halves.
      if (point->variant >= 0 && point->variant < 16) {
        draw[0](canvas, work->sheet + ((point->variant / 2) % 3 << 10),
                point->x - 16, point->y - 56, 16, 64);
        draw[1](canvas, work->sheet + ((point->variant / 2) % 3 << 10),
                point->x, point->y - 56, 16, 64);
        point->variant++;
      }
    }

    BattleCamera_Shake(effect->variant * 2 + 2, effect->variant * 2 + 2);
    if (work->cameraPhase == 0)
      work->cameraPhase = 1;
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }

  // Tear down.
  Task_Remove(Camera_ApplyPhasedDelta);
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(HEAP_BLITTER_ALTERNATE);
  HeapSlot_Release(HEAP_BLITTER);
  BattleEffect_EndCanvasLayer();
}

#define TORNADO_FRAMES 80
#define GLINT_COUNT 16 // four glints for each of up to four funnels
#define GLINT_CELS 6   // three cels of two frames each

extern char ResourceId_TornadoSheet;

// ☀️ gVortices_Shapes: funnels, sway radius and shake, for each variant.
extern u8 gData_081979ae[];
// ☀️ gBattleEffect_GlintCells, Widths and Heights.
extern u16 gData_08197486[];
extern u8 gData_08197492[];
extern u8 gData_08197498[];

// One to four funnels of wind sway across the targets for 80 frames, each
// drawn as three stacked segments with glints swirling around it; the canvas
// is wiped every eight frames, and each target is struck seven times.
void BattleEffect_RunWhirlwind(struct BattleEffectArgument *effect) {
  DrawRectangle draw[2];
  struct BattleEffectWork *work;
  void *canvas;
  s32 wave;
  struct EffectStep *glint;
  s32 frame;
  s32 x;
  s32 y;
  s32 segment;
  s32 cell;
  s32 i;
  s32 j;

  // Set up: the canvas layer, the tornado sheet and two max blitters, the
  // second mirrored.
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  BattleEffect_StartCanvasLayer(1);
  REG_BLDALPHA = 0x1010;
  Resource_LoadAndDecompress((s32)&ResourceId_TornadoSheet, work->sheet, 1, 1);
  BattleEffect_LoadBlitter(HEAP_BLITTER, BLITTER_MAX << 4 | COMPOSITE_CLIP_X |
                                             COMPOSITE_CLIP_Y);
  draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  BattleEffect_LoadBlitter(HEAP_BLITTER_ALTERNATE,
                           BLITTER_MAX << 4 | COMPOSITE_CLIP_X |
                               COMPOSITE_CLIP_Y | COMPOSITE_FLIP_X);
  draw[1] = (DrawRectangle)Ram_HeapSlots->unkb8[1];

  // The glints start at random places in their funnel, staggered in time.
  for (j = 0; j != GLINT_COUNT; j++) {
    glint = &work->particles[j];
    glint->x = Random16() & 31;
    glint->y = (Random16() & 63) + 16;
    glint->variant = -(Random16() & 15);
  }
  work->transferMode = 2;
  work->transferValue = 50;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);
  REG_BLDALPHA = 0x1000;
  wait(1);
  Sound_PlayFar(141);

  for (frame = 0; frame != TORNADO_FRAMES; frame++) {
    wave = Trig_Sin(frame << 10) << 4;
    if (frame == 32)
      Func_081180e8(133);

    // Wipe the canvas every eight frames from frame 16, leaving a trail.
    for (i = 0; i != 7; i++) {
      if (frame == i * 8 + 16)
        Iwram_FillWords(canvas, 0x4000, 0x08080808);
    }

    // Sway toward the targets' side; fade in over 16 frames and out over the
    // last 16.
    if (effect->side == 1)
      wave += 0x200000;
    else
      wave -= 0x200000;
    if (frame <= 16)
      REG_BLDALPHA = frame | 0x1000;
    if (frame > 63)
      REG_BLDALPHA = (0x4f - frame) | 0x1000;

    for (i = 0; i != gData_081979ae[effect->variant * 3]; i++) {
      // Each funnel circles its own phase, a quarter turn from the next.
      x = ((gData_081979ae[effect->variant * 3 + 1] *
                Trig_Sin((frame << 11) + i * 0x4000) +
            wave) >>
           16) +
          40;
      y = (Trig_Cos((frame << 11) + i * 0x4000) * 2) >> 16;
      segment = (frame / 2) % 3;
      draw[0](canvas, work->sheet + segment * 0xa00 + 0xc56, x, y + 16, 40, 32);
      draw[0](canvas, work->sheet + segment * 0x500 + 0x2a56, x, y + 48, 40,
              32);
      draw[0](canvas, work->sheet + segment * 0xa00 + 0x1156, x, y + 80, 40,
              32);

      // Its four glints, the second of each pair mirrored; each restarts at
      // a new place when its cels run out.
      for (j = 0; j != 4; j++) {
        glint = &work->particles[i * 4 + j];
        if (glint->variant >= 0) {
          cell = glint->variant / 2 + (j / 2) * 3;
          draw[j & 1](canvas, work->sheet + gData_08197486[cell], glint->x + x,
                      glint->y + y, gData_08197492[cell], gData_08197498[cell]);
        }
        glint->variant++;
        if (glint->variant == GLINT_CELS) {
          glint->x = Random16() & 31;
          glint->y = (Random16() & 63) + 16;
          glint->variant = 0;
        }
      }
    }

    // Strike each target seven times, every eight frames, three frames
    // after the target before it.
    for (j = 0; j != effect->count; j++) {
      for (i = 0; i != 7; i++) {
        if (frame == i * 8 + j * 3 + 16) {
          Func_0814cd48(effect->actors[j], 7, 5, j, 4);
          Func_08118088(effect->actors[j], 6);
        }
      }
    }

    work->shakeFrames = 1;
    BattleCamera_Shake(gData_081979ae[effect->variant * 3 + 2],
                       gData_081979ae[effect->variant * 3 + 2] * 2);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }

  // Tear down.
  HeapSlot_Release(HEAP_BLITTER_ALTERNATE);
  HeapSlot_Release(HEAP_BLITTER);
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}

#undef GROUND_Y

#define SHARDS_PER_CRYSTAL 16
#define GROUND_Y (56 << 16)
#define SHATTERED 1 // a crystal's z once it has hit the ground

extern char ResourceId_BoulderSheet; // directory row 0x14d
extern char ResourceId_CrystalSheet; // directory row 0x193

// ☀️ gPrism_Counts, with a third byte: crystals, frames and the frame
// that begins phase 134, for each variant.
extern u8 Data_081979b7[];
extern s32 gData_08197834[]; // ☀️ gPrism_ShardOffsets
extern u8 gData_0819781a[];  // ☀️ gPrism_ShardWidths
extern u8 gData_08197826[];  // ☀️ gPrism_ShardHeights
extern u8 gData_081977f8[];  // ☀️ gPrism_ShardStarts: x, y for each shard

void BattleEffectKernels_IntegrateVector2(struct EffectStep *step, s32 damping,
                                          s32 gravity);

// Crystals fall one after another, eight frames apart, onto the targets'
// ground and shatter into sixteen spinning shards each; the screen darkens
// over the last eight frames.
void BattleEffect_RunPrism(struct BattleEffectArgument *effect) {
  DrawRectangle draw[2];
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  struct EffectStep *seed;
  struct EffectStep *cry;
  struct EffectStep *shards;
  struct EffectStep *shard;
  s32 x;
  s32 cell;
  s32 i;
  s32 j;
  s32 k;

  // Set up: the canvas layer, the two sheets and the blitter pair.
  shards = (struct EffectStep *)Ram_MapCellBuffer;
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  BattleEffect_StartCanvasLayer(0);
  REG_BLDALPHA = 0x1010;
  Resource_LoadAndDecompress((s32)&ResourceId_BoulderSheet, work->sheet, 1, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_CrystalSheet, work->sheet, 1, 1);
  BattleEffect_FetchRectangleBlitters(effect->side, draw);
  work->transferMode = 2;
  work->transferValue = 50;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);

  // The crystals wait above the screen, each eight frames behind the last,
  // and drift sideways so that they land over the targets.
  for (k = 0; k != Data_081979b7[effect->variant * 3]; k++) {
    seed = &work->particles[k];
    x = Random16() & 31;
    seed->y = -0x400000;
    if (effect->side == 1) {
      x = ((Random16() & 31) + 80) << 16;
      seed->velocityX = (Random16() & 63) << 12;
    } else {
      x = ((Random16() & 31) + 8) << 16;
      seed->velocityX = -(Random16() & 63) << 12;
    }
    seed->x = x - seed->velocityX * 18;
    seed->velocityY = 0;
    seed->z = 0;
    seed->variant = k * 8;
  }

  for (frame = 0; frame != Data_081979b7[effect->variant * 3 + 1]; frame++) {
    // The strongest variant turns the camera, slowing from frame 96.
    if (effect->variant == 2 && frame <= 103) {
      struct BattleCamera *camera = Ram_HeapSlots->cameraWork;
      s32 speed;

      if (frame <= 95)
        speed = 192;
      else
        speed = 2496 - frame * 24;
      if (effect->side == 0)
        camera->yaw -= speed;
      else
        camera->yaw += speed;
    }
    if (frame == Data_081979b7[effect->variant * 3 + 2])
      Func_081180e8(134);

    // Darken over the last eight frames.
    if (frame == Data_081979b7[effect->variant * 3 + 1] - 8) {
      work->transferMode = 3;
      work->transferValue = 0x06060606;
    }

    if (frame <= Data_081979b7[effect->variant * 3 + 1] - 8) {
      for (i = 0; i != Data_081979b7[effect->variant * 3]; i++) {
        cry = &work->particles[i];
        if (cry->z == SHATTERED) {
          // A shattered crystal: its shards fly and spin, slowing down.
          for (j = 0; j != SHARDS_PER_CRYSTAL; j++) {
            cell = (j % 5) * 3 +
                   (shards[i * SHARDS_PER_CRYSTAL + j].variant / 96) % 3;
            shard = &shards[i * SHARDS_PER_CRYSTAL + j];
            draw[j <= 2](canvas, work->sheet + gData_08197834[cell] + 0x800,
                         HI(shard->x) - gData_0819781a[cell] / 2,
                         HI(shard->y) - gData_08197826[cell] / 2,
                         gData_0819781a[cell], gData_08197826[cell]);
            BattleEffectKernels_IntegrateVector2(shard, 64, 0x2000);
            shard->variant += shard->z;
            if (shard->z > 1 && (frame & 1))
              shard->z--;
          }
        } else if (frame >= cry->variant) {
          // A falling crystal (32x64), until it reaches the ground.
          draw[i & 1](canvas, work->sheet, HI(cry->x) - 16, HI(cry->y), 32, 64);
          BattleEffectKernels_IntegrateVector2(cry, 64, 0x10000);
          if (cry->y > GROUND_Y) {
            cry->z = SHATTERED;
            cry->y = GROUND_Y;
            for (j = 0; j != SHARDS_PER_CRYSTAL; j++) {
              shard = &shards[i * SHARDS_PER_CRYSTAL + j];
              shard->x = ((gData_081977f8[j * 2] - 40) << 16) + cry->x;
              shard->y = gData_081977f8[j * 2 + 1] << 16;
              shard->velocityX = ((Random16() & 127) - 64) << 11;
              shard->velocityY = -(Random16() & 127) << 11;
              if (i & 1) {
                shard->velocityX *= 2;
                shard->velocityY *= 2;
              }
              shard->z = 32;
              shard->variant = 0;
            }
            work->shakeFrames = 8;
            Sound_PlayFar(144);
            for (j = 0; j != effect->count; j++)
              Func_0814cd48(effect->actors[j], 7, 5, j, 4);
          }
        }
      }
    }

    BattleCamera_Shake(effect->variant * 2 + 4, effect->variant * 4 + 8);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }

  // Tear down.
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(HEAP_BLITTER_ALTERNATE);
  HeapSlot_Release(HEAP_BLITTER);
  BattleEffect_EndCanvasLayer();
}
