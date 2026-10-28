// Twin: The Broken Seal's BattleEffect_StartCanvasLayer. The Lost Age's
// changes: Func_0813ba50 for the display set-up, DISPCNT 0x1741,
// Palette_StepFadeTransfer at 0x480, Func_0814355c for the display registers
// and one more work word cleared.
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

void Func_0813ba50(void);
void BattleEffect_ClearActorTimers(void);
void Palette_StepFadeTransfer(void);
void Func_0814355c(void);
void BattleStaging_ConfigurePaletteFadeFar(s32 mode, s32 background,
                                                s32 value);
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

  Func_0813ba50();
  BattleEffect_ClearActorTimers();
  display->secondMode = 1;
  WaitFrames(1);
  REG_BLDCNT = 0;
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT, 0x1741, 0x20000);
  gBgScroll[1].y = 32;
  WaitFrames(1);

  BattleStaging_ConfigurePaletteFadeFar(1, *(u16 *)battle->unk670, 0);
  work->fadeFrames = 0;
  work->fadeStep = 0;
  Task_Add(Palette_StepFadeTransfer, TASK_PASS_VBLANK);

  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT, 0x7341, 0x20000);
  WaitFrames(1);
  REG_BG2CNT = bgControl | 0x784;
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT, 0x7341, 0x20000);
  BattleCamera_SetViewFar(0, 0, 0, 100);
  display->secondMode = 0;
  WaitFrames(1);

  Func_0814355c();
  REG_WININ = 0x3537;
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT, 0x7741, 0x20000);

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
  WaitFrames(1);
}
