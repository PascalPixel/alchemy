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

extern u32 gCameraSceneParameters[];

void Sound_PlayFar(s32 cue);
void BattleEffect_ClearActorTimers(void);
void Palette_StepFadeTransfer(void);
void Func_0814355c(void);
void BattleEffect_StartCanvasLayer(s32 bg_control);
void BattleStaging_ConfigurePaletteFadeFar(s32 mode, s32 background,
                                                s32 value);
void BattleBackground_SetPaletteLevelFar(s32 background, s32 level);
void BattleCamera_SetViewFar(s32 a, s32 b, s32 c, s32 d);

static __inline__ s32 ClearWords_(void *destination, s32 size) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants
   * and the routine in a callee-saved register; called directly, 24
   * instructions of BattleEffect_OpenCanvasLayer differ in tla-en. */
  return Iwram_ClearWords(destination, size);
}

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
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT, 0x1741, 0x20000);
  gBgScroll[1].y = 32;
  wait(1);
  BattleStaging_ConfigurePaletteFadeFar(1, *(u16 *)battle->unk670, 128);
  work->fadeFrames = 24;
  work->fadeStep = 0;
  Task_Add(Palette_StepFadeTransfer, TASK_PASS_VBLANK);
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT, 0x1341, 0x20000);
  wait(1);
  REG_BG2CNT = bgControl | 0x784;
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT, 0x1341, 0x20000);
  BattleCamera_SetViewFar(0, 0, 0, 100);
  display->secondMode = 0;
  wait(1);

  Func_0814355c();
  REG_WININ = 0x3537;
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT, 0x7741, 0x20000);
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
  REG_BG2CNT = bg_control | 0x4784;
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
  REG_BG2CNT = bg_control | 0x4784;
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
  REG_BG2CNT = 0x787;
  Iwram_ClearWords(BG_CHAR_BLOCK(1), 0x4000);
  Task_Remove(Palette_StepFadeTransfer);
  gBgScroll[1].y = 32;
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT, 0x7341, 0x20000);
  REG_BLDCNT = 0;
  wait(1);

  BattleStaging_ConfigurePaletteFadeFar(2, *(u16 *)battle->unk670, 7);
  wait(1);
  for (i = 0; i != 8; i++) {
    BattleBackground_SetPaletteLevelFar(*(u16 *)battle->unk670, 21 - i * 3);
    wait(1);
  }

  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT, 0x7541, 0x20000);
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
  QueueIoWriteDelay2((u32)&REG_DISPCNT, 0x1541);
  wait(1);
  BattleStaging_ConfigurePaletteFadeFar(2, *(u16 *)battle->unk670, 0);
  IO_WRITE_QUEUE_PUSH((u32)&REG_BG1CNT, 0x1f83, 0x20000);
  wait(1);
}
