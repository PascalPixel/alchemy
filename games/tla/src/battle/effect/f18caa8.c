#include "types.h"
#include "ram_buffer.h"

typedef s32 (*CanvasBlitFunc)(u8 *canvas, void *source, s32 x, s32 y, s32 width,
                              s32 height);

s32 BattleEffect_LoadBlitter(s32 slot, s32 value);
void Runtime_ReleaseHeapBlock(s32 slot);

void Func_0818caa8(void *source, s32 x, s32 y, s32 size) {
  u8 *canvas = Ram_HeapSlots->battleFxWork[1];
  s32 half;

  BattleEffect_LoadBlitter(188, 19);
  half = size / 2;
  ((CanvasBlitFunc)Ram_HeapSlots->unkb8[1])(canvas, source, x - half, y - size,
                                            half, size);
  Runtime_ReleaseHeapBlock(188);
  BattleEffect_LoadBlitter(188, 23);
  ((CanvasBlitFunc)Ram_HeapSlots->unkb8[1])(canvas, source, x, y - size, half,
                                            size);
  Runtime_ReleaseHeapBlock(188);
  BattleEffect_LoadBlitter(188, 27);
  ((CanvasBlitFunc)Ram_HeapSlots->unkb8[1])(canvas, source, x - half, y, half,
                                            size);
  Runtime_ReleaseHeapBlock(188);
  BattleEffect_LoadBlitter(188, 31);
  ((CanvasBlitFunc)Ram_HeapSlots->unkb8[1])(canvas, source, x, y, half, size);
  Runtime_ReleaseHeapBlock(188);
}
