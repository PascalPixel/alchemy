#include "types.h"
#include "task.h"
#include "mem.h"
#include "ram_buffer.h"

#define WAVE_FX_SLOT 88

struct ObjectRuntime;
struct ObjectRuntime *Object_GetById(s32 id);
void BattleEffect_ApplyFlickerValue(struct ObjectRuntime *object, s32 value);
void BattleEffect_PrepareBufferInterpolation(void);
void Func_080dcadc(void);
void Func_080dcb44(void);

void BattleEffect_FinishSceneAndReleaseHeapBlock(void) {
  u8 *work = Ram_HeapSlots->unk4c[3];

  Func_080dcadc();
  Task_Remove((TaskFunc)Func_080dcb44);
  BattleEffect_ApplyFlickerValue(Object_GetById(*(u16 *)(work + 0x290)), 1);
  BattleEffect_PrepareBufferInterpolation();
  Runtime_ReleaseHeapBlock(WAVE_FX_SLOT);
}

#define BG_PLTT_COLOR(bank, color)                                             \
  (*(volatile u16 *)(0x05000000 + ((bank) * 16 + (color)) * 2))
#define GREY28 0x739c

void BattleEffect_ArmBg0HBlankDma(void);

/// Gray BG palette 15 colors 1 and 3-12, then start line scrolling.
void Palette_FillNumber15Grey(void) {
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
  Task_Add((TaskFunc)BattleEffect_ArmBg0HBlankDma, TASK_PASS_FRAME - 20);
}
