// Near miss, differ 2 of 60 (one reordered row): The Broken Seal's
// BattleEffect_ConfigureEffectDisplay. The native loads each register value from
// the pool before it starts the register's address; plainly every address
// starts first (differ 20). One-pass blocks around the BLDCNT, WIN0H and WININ
// values put those loads first without moving the literal pools (an empty asm
// counts 4 estimated bytes and moves both pools, differ 12). Left: the native
// loads WIN0V's value between the first two rows of the window address (movs
// r3; ldr r2; lsls r3), this draft after them; sched2 ranks the shift 21 and
// the load
// 20. Measured: the value assigned before the block, before or after the WIN0H
// store, unpinned, or behind a "+l" barrier (differ 2 to 30).
#include "types.h"
#include "io_reg.h"
#include "io_write_queue.h"
#include "system.h"

void Func_0813ba50(void) {
  {
    register u16 blend __asm__("r3");

    do {
      blend = 0;
    } while (0);
    REG_BLDCNT = blend;
  }
  REG_BLDALPHA = 0x100e;
  {
    register u16 h __asm__("r1");
    register u16 v __asm__("r2");

    do {
      h = 0x00f0;
    } while (0);
    REG_WIN0H = h;
    v = 0x1088;
    REG_WIN0V = v;
    REG_WIN1H = h;
    REG_WIN1V = v;
  }
  {
    register u16 in __asm__("r3");

    do {
      in = 0x3537;
    } while (0);
    REG_WININ = in;
  }
  REG_WINOUT = 0x3f21;
  IO_WRITE_QUEUE_PUSH(0x04000000, 0x7741, 0x20000);
  WaitFrames(1);
}
