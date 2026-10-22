#include "mem.h"
#include "palbuf.h"
#include "dma.h"
#include "types.h"
#include "callback_scheduler.h"
#include "ram_buffer.h"
#include "io_reg.h"

void PaletteFade_Initialize(void) {
  struct PaletteFade *buffer;
  s32 operation;

  buffer = Runtime_AllocateBlock(128, sizeof(*buffer));
  Dma_Fill32(0, buffer, sizeof(*buffer));
  Dma_Set((void *)0x05000000, buffer, 0x84000080, REG_DMA3);
  Dma_Set((void *)0x05000200, &buffer->palette[256], 0x84000080, REG_DMA3);
  Palette_Transform(0x10000, buffer->palette, buffer->target, 0);
  operation = 1152;
  Scheduler_AddOrUpdateCallback((s32)(PaletteFade_Update), operation);
}

void PaletteFade_Release(void) {
  Scheduler_RemoveCallback((u32)((s32)&PaletteFade_Update));
  Runtime_ReleaseHeapBlock(0x80);
}

void PaletteFade_SetTarget(s32 index, s32 transform) {
  struct PaletteFade *target = Ram_HeapSlots->block32;

  if (target != NULL)
    Palette_Transform(index, target->palette, target->target, transform);
}

void PaletteFade_SetCurrent(s32 index, s32 transform) {
  struct PaletteFade *target = Ram_HeapSlots->block32;

  if (target != NULL)
    Palette_Transform(index, (u16 *)target, target->current, transform);
}

void PaletteFade_SetBackdrop(s32 value) {
  struct PaletteFade *target = Ram_HeapSlots->block32;

  if (target != NULL)
    target->palette[0] = value;
}

void PaletteFade_Start(s32 value) {
  struct PaletteFade *state = Ram_HeapSlots->block32;

  if (state != NULL) {
    state->duration = value;
    state->step = 0;
    PaletteFade_ComputeSteps((s16 *)state->current, (s16 *)state->target,
                             (s16 *)state->delta, value);
  }
}
