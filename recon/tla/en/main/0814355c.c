// Near miss, differ 20 of 48: the same thirteen stores; four pool loads sit one
// row earlier in the native (ldr r3 before movs r2, #128 at the top; ldr r1
// and ldr r2 before the window address), which moves the pool words.
// Tried: each pool value pinned to its register behind a barrier that holds
// the address register, as in canvas.c (differ 16: the order then matches
// except the window height, which still loads after the address's shift,
// and the literal pool moves); alchemy permute for 4 minutes on that form.
#include "types.h"
#include "io_reg.h"

void Func_0814355c(void) {
  REG_BLDCNT = 0x3f44;
  REG_BLDALPHA = 0x100e;
  REG_BG2X = 0;
  REG_BG2Y = 0xfffff000;
  REG_BG2PA = 0x80;
  REG_BG2PB = 0;
  REG_BG2PC = 0;
  REG_BG2PD = 0x100;
  REG_WIN0H = 0xf0;
  REG_WIN0V = 0x1088;
  REG_WIN1H = 0xf0;
  REG_WIN1V = 0x1088;
  REG_WINOUT = 0x3f21;
}
