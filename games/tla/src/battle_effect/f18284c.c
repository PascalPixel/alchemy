#include "types.h"
#include "io_reg.h"
#include "ram_buffer.h"

struct BattleEffectArgument;

void BattleEffect_StartCanvasLayer(s32 mode);
void Func_08182898(struct BattleEffectArgument *effect);

/// Opens the canvas layer, sets alpha blending and a 24-frame palette fade
/// counter (read by Func_08143488), then runs the effect body Func_08182898.
void Func_0818284c(s32 arg) {
  u8 *work = Ram_HeapSlots->battleFxWork[0];

  BattleEffect_StartCanvasLayer(0);
  REG_BLDCNT = 0x3f44;
  REG_BLDALPHA = BLEND_ALPHA(16, 16);
  *(s32 *)(work + 0x77b4) = 24;
  *(s32 *)(work + 0x77b8) = 0;
  Func_08182898(arg);
}
