// Twin: The Broken Seal's BattleEffect_ConfigureEffectDisplay.
#include "types.h"
#include "io_reg.h"
#include "io_write_queue.h"
#include "system.h"

void Func_0813ba50(void) {
  REG_BLDCNT = 0;
  REG_BLDALPHA = BLEND_ALPHA(14, 16);
  REG_WIN0H = WIN_RANGE(0, 240);
  REG_WIN0V = WIN_RANGE(16, 136);
  REG_WIN1H = WIN_RANGE(0, 240);
  REG_WIN1V = WIN_RANGE(16, 136);
  REG_WININ = WININ_WIN1_CLR | WININ_WIN1_OBJ | WININ_WIN1_BG2 |
              WININ_WIN1_BG0 | WININ_WIN0_CLR | WININ_WIN0_OBJ |
              WININ_WIN0_BG2 | WININ_WIN0_BG1 | WININ_WIN0_BG0;
  REG_WINOUT = WINOUT_WINOBJ_ALL | WINOUT_WIN01_CLR | WINOUT_WIN01_BG0;
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT,
                      DISPCNT_WIN1_ON | DISPCNT_WIN0_ON | DISPCNT_OBJ_ON |
                          DISPCNT_BG2_ON | DISPCNT_BG1_ON | DISPCNT_BG0_ON |
                          DISPCNT_OBJ_1D_MAP | DISPCNT_MODE_1,
                      IO_WRITE_16);
  wait(1);
}
