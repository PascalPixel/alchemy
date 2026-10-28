#include "types.h"
#include "task.h"
#include "mem.h"
#include "dma.h"
#include "ram_buffer.h"

#define WAVE_FX_SLOT 88

struct WaveFxWork {
  u8 unk000[0x28b];
  s8 red;
  s8 green;
  s8 blue;
  u16 phase;
  u16 id;
  u16 value;
  u8 steps;
  u8 unk295;
  u8 unk296[2];
};

extern u32 gFrameCount;

void BattleEffect_InitializeSharedScene(void);
void BattleEffect_AdvanceHueCycle(void);
void BattleEffect_ApplyColorToTargetBuffer(s32 colors, s32 mode);
void BattleEffect_StartBufferInterpolation(s32 frames);
void Ui_FillBank15PaletteGrey(void);
void Func_080dcb44(void);

/// Starts the wave effect's work in heap slot WAVE_FX_SLOT: a phase taken
/// from the frame count, id and value, then the shared scene, the first hue
/// step and its colour fade, with Func_080dcb44 run each frame.
void Func_080dc978(s32 id, s32 value) {
  struct WaveFxWork *work;

  work = Runtime_AllocateBlock(WAVE_FX_SLOT, sizeof(*work));
  Dma_Fill32(0, work, sizeof(*work));
  work->phase = (gFrameCount * 2) % 360;
  work->id = id;
  work->value = value;
  work->steps = 8;
  work->unk295 = 0;
  BattleEffect_InitializeSharedScene();
  BattleEffect_AdvanceHueCycle();
  BattleEffect_ApplyColorToTargetBuffer(
      (work->blue << 10) | (work->green << 5) | work->red | 0x200000, 1);
  BattleEffect_StartBufferInterpolation(8);
  Ui_FillBank15PaletteGrey();
  Task_Add((TaskFunc)Func_080dcb44, TASK_PASS_VBLANK);
}
