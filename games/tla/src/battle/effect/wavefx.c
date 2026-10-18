#include "types.h"
#include "callback_scheduler.h"
#include "mem.h"
#include "ram_buffer.h"

#define WAVE_FX_SLOT 88

struct ObjectRuntime;
struct ObjectRuntime *Object_GetById(s32 id);
void BattleFx_ApplyFlickerValue(struct ObjectRuntime *object, s32 value);
void BattleFx_PrepareBufferInterpolation(void);
void Func_080dcadc(void);
void Func_080dcb44(void);

void BattleFx_FinishSceneAndReleaseHeapBlock(void) {
  u8 *work = Ram_HeapSlots->unk4c[3];

  Func_080dcadc();
  Scheduler_RemoveCallback((SchedulerCallback)Func_080dcb44);
  BattleFx_ApplyFlickerValue(Object_GetById(*(u16 *)(work + 0x290)), 1);
  BattleFx_PrepareBufferInterpolation();
  Runtime_ReleaseHeapBlock(WAVE_FX_SLOT);
}

#define BG_PLTT_COLOR(bank, color)                                             \
  (*(volatile u16 *)(0x05000000 + ((bank) * 16 + (color)) * 2))
#define GREY28 0x739c

void BattleFx_ArmBg0HBlankDma(void);

// Gray BG palette 15 colors 1 and 3-12, then start scanline scrolling.
void Ui_FillBank15PaletteGrey(void) {
  volatile u16 *p;

  Ram_HeapSlots->windowTiles[4] = 1;
  p = &BG_PLTT_COLOR(15, 1);
  *p = GREY28;
  p += 2;
  *p = GREY28;
  p += 8;
  *p = GREY28;
  p += 1;
  *p = GREY28;
  p -= 8;
  *p = GREY28;
  p += 1;
  *p = GREY28;
  p += 1;
  *p = GREY28;
  p += 1;
  *p = GREY28;
  p += 1;
  *p = GREY28;
  p += 1;
  *p = GREY28;
  p += 1;
  *p = GREY28;
  Scheduler_AddOrUpdateCallback((SchedulerCallback)BattleFx_ArmBg0HBlankDma, 0xc6c);
}
