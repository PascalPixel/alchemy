#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "mem.h"
#include "dma.h"
#include "task.h"
#include "types.h"
#include "palbuf.h"
#include "ram_buffer.h"
#include "io_reg.h"

void BattleEffect_InitializeBuffers(void) {
  volatile s32 zero;
  struct BattleEffectBuffers *buffer;

  buffer = HeapSlot_AllocEwram(128, sizeof(*buffer));
  zero = 0;
  Dma_Set(&zero, buffer, 0x85000a81, REG_DMA3);
  Dma_Set((void *)0x05000000, buffer, 0x84000070, REG_DMA3);
  Dma_Set((void *)0x05000200, (u8 *)buffer->palette + 0x1c0, 0x84000070,
          REG_DMA3);
  BattleEffect_BuildBuffer(0x10000, buffer, buffer->target, 0);
  Task_Add((s32)BattlePalette_UpdateBlend, TASK_PASS_VBLANK + 15);
}

void BattlePalette_StopBlendAndReleaseBuffer(void) {
  Task_Remove((s32)&BattlePalette_UpdateBlend);
  HeapSlot_Release(128);
}

void BattleEffect_ApplyColorToTargetBuffer(s32 value, s32 mode) {
  struct BattleEffectBuffers *buffers = Ram_HeapSlots->block32;

  if (buffers != NULL) {
    BattleEffect_BuildBuffer(value, buffers, buffers->target, mode);
  }
}

void BattleEffect_ApplyColorToSourceBuffer(s32 value, s32 mode) {
  struct BattleEffectBuffers *buffers = Ram_HeapSlots->block32;

  if (buffers != NULL) {
    BattleEffect_BuildBuffer(value, buffers, buffers->current, mode);
  }
}

void BattleEffect_SetPrimaryBufferValue(u32 value) {
  struct BattleEffectBuffers *buffers = Ram_HeapSlots->block32;

  if (buffers != NULL) {
    buffers->palette[0] = value;
  }
}
