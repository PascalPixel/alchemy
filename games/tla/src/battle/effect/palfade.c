#include "types.h"
#include "ram_buffer.h"
#include "dma.h"
#include "io_reg.h"
#include "palbuf.h"

void BattleFx_ApplyColorToTargetBuffer(s32 colors, s32 mode);
void BattleFx_StartBufferInterpolation(s32 frames);
void GameFlag_ClearBit(s32 flag);

void GameFlag_SetBit(s32 flag);

/* The Spanish and Italian far-call tables reach this function under another
 * slot's name (Func_080db9c0); those two editions keep it on their scaffold
 * (S4). */
#if !defined(TLA_EDITION_ES) && !defined(TLA_EDITION_IT)
void Field_BeginColorTransition(s32 frames, s32 color) {
  u8 *state = Ram_HeapSlots->eventWork;
  struct BattleEffectBuffers *buffers = Ram_HeapSlots->block32;

  Dma_Set(buffers->target + 0x2a0, state + 0x762,
          DMA_ENABLE32 | DMA_32BIT | 0x150, REG_DMA3);
  if (*(s8 *)(state + 0xca4) == 0)
    Dma_Set(buffers->target, state + 0x222, DMA_ENABLE32 | DMA_32BIT | 0x150,
            REG_DMA3);
  Dma_Set(buffers->target, buffers->current, DMA_ENABLE32 | DMA_32BIT | 0x2a0,
          REG_DMA3);
  BattleFx_ApplyColorToTargetBuffer(0x200000 | color, 1);
  GameFlag_SetBit(0x108);
  BattleFx_StartBufferInterpolation(frames);
}
#endif

void Field_BeginPaletteTransition(s32 frames) {
  u8 *state = Ram_HeapSlots->eventWork;
  s32 colors = (s32)(state + 0x222);

  BattleFx_ApplyColorToTargetBuffer(colors, 2);
  if (*(s8 *)(state + 0xca4) != 0)
    BattleFx_ApplyColorToTargetBuffer(0x10001, 1);
  else
    BattleFx_ApplyColorToTargetBuffer(colors, 1);
  GameFlag_ClearBit(0x108);
  BattleFx_StartBufferInterpolation(frames);
}
