// Near miss: listing 94 instructions, draft 94, differ 12; all registers match,
// six pairs are in another order. The same source compiled with -mtune=arm7tdmi
// in place of K2's -mtune=arm9tdmi matches all 94 instructions (analysis only,
// outside the build: K2 sets the tuning for every file). Under arm9tdmi the
// native stores the source and y arguments before it copies size, stores both
// stack arguments before it copies the slot address for each blit, and
// subtracts size from y before storing size. Measured: the four callees' return
// types decide which argument register loads first after each call (all void:
// differ 20; Func_081963ec s32: 14; it and the blit s32: 12;
// Runtime_ReleaseHeapBlock's type changes nothing); the blit through a local
// pointer reloaded per call: 102 instructions, differ 90; the canvas read after
// the first call: 20. permute (6 min): nothing below 12.
#include "types.h"
#include "ram_buffer.h"

typedef s32 (*CanvasBlitFunc)(u8 *canvas, void *source, s32 x, s32 y, s32 width,
                              s32 height);

s32 Func_081963ec(s32 slot, s32 value);
void Runtime_ReleaseHeapBlock(s32 slot);

void Func_0818caa8(void *source, s32 x, s32 y, s32 size) {
  u8 *canvas = Ram_HeapSlots->battleFxWork[1];
  s32 half;

  Func_081963ec(188, 19);
  half = size / 2;
  ((CanvasBlitFunc)Ram_HeapSlots->unkb8[1])(canvas, source, x - half, y - size,
                                            half, size);
  Runtime_ReleaseHeapBlock(188);
  Func_081963ec(188, 23);
  ((CanvasBlitFunc)Ram_HeapSlots->unkb8[1])(canvas, source, x, y - size, half,
                                            size);
  Runtime_ReleaseHeapBlock(188);
  Func_081963ec(188, 27);
  ((CanvasBlitFunc)Ram_HeapSlots->unkb8[1])(canvas, source, x - half, y, half,
                                            size);
  Runtime_ReleaseHeapBlock(188);
  Func_081963ec(188, 31);
  ((CanvasBlitFunc)Ram_HeapSlots->unkb8[1])(canvas, source, x, y, half, size);
  Runtime_ReleaseHeapBlock(188);
}
