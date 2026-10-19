#include "types.h"
#include "io_reg.h"
#include "ram_buffer.h"

void BattleFx_BeginCanvasLayer(s32 mode);
void Func_08182898(s32 arg);

/// Opens the canvas layer, sets alpha blending and a 24-frame palette fade
/// counter (read by Func_08143488), then runs the effect body Func_08182898.
void Func_0818284c(s32 arg) {
  u8 *work = Ram_HeapSlots->battleFxWork[0];

  BattleFx_BeginCanvasLayer(0);
  {
    /* FAKEMATCH: the blend word pinned to r3 as a halfword loads from the pool
     * before the register address, as native; as a plain store, 2
     * instructions differ in tla-en. */
    register u16 blend __asm__("r3") = 0x3f44;

    /* FAKEMATCH: the barrier of the pin above: it holds r2 until the value is
     * loaded. */
    __asm__("" : "+l"(blend) : : "r2");
    REG_BLDCNT = blend;
  }
  REG_BLDALPHA = 0x1010;
  *(s32 *)(work + 0x77b4) = 24;
  *(s32 *)(work + 0x77b8) = 0;
  Func_08182898(arg);
}
