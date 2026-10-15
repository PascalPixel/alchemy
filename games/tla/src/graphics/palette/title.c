#include "mem.h"
#include "palbuf.h"
#include "dma.h"
#include "types.h"
#include "callback_scheduler.h"
#include "ram_buffer.h"
#include "io_reg.h"

void TitlePalette_InitializeBuffers(void) {
  struct TitlePaletteWork *buffer;
  s32 operation;

  buffer = Runtime_AllocateBlock(128, sizeof(*buffer));
  Dma_Fill32(0, buffer, sizeof(*buffer));
  Dma_Set((void *)0x05000000, buffer, 0x84000080, REG_DMA3);
  Dma_Set((void *)0x05000200, &buffer->palette[256], 0x84000080, REG_DMA3);
  Graphics_TransformPaletteBuffer(0x10000, buffer->palette, buffer->target, 0);
  operation = 1152;
  Scheduler_AddOrUpdateCallback((s32)(TitlePalette_UpdateFade), operation);
}

void Runtime_ScheduleCallbackAndReleaseBlock32A(void) {
  Scheduler_RemoveCallback((u32)((s32)&TitlePalette_UpdateFade));
  Runtime_ReleaseHeapBlock(0x80);
}

void Graphics_TransformLargePalette(s32 index, s32 transform) {
  struct TitlePaletteWork *target = Ram_HeapSlots->block32;

  if (target != NULL)
    Graphics_TransformPaletteBuffer(index, target->palette, target->target,
                                    transform);
}

void Graphics_TransformSmallPalette(s32 index, s32 transform) {
  struct TitlePaletteWork *target = Ram_HeapSlots->block32;

  if (target != NULL)
    Graphics_TransformPaletteBuffer(index, (u16 *)target, target->current,
                                    transform);
}

void Graphics_SetPaletteTransformValue(s32 value) {
  struct TitlePaletteWork *target = Ram_HeapSlots->block32;

  if (target != NULL)
    target->palette[0] = value;
}

void Graphics_UpdatePaletteInterpolation(s32 value) {
  struct TitlePaletteWork *state = Ram_HeapSlots->block32;

  if (state != NULL) {
    state->duration = value;
    state->step = 0;
    Graphics_InterpolatePaletteBuffers((s16 *)state->current,
                                       (s16 *)state->target,
                                       (s16 *)state->delta, value);
  }
}
