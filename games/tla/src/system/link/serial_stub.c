#include "serial_runtime.h"
#include "input.h"
#include "cartboot.h"

extern u8 gMapCellBuffer[];
extern const u8 Cartridge_Restart[];

void Audio_PlayCue(s32 cue);
void Func_08016180(void);

void Func_08016bd8(void) {}

// The serial link test: sends the cartridge's first bytes or those at 0x1000
// while A or B is held, and receives into the map cell buffer.
void SerialTest_Run(void) {
  u16 *tile;
  s32 value;
  s32 n;
  s32 tick;
  /* FAKEMATCH: each key test loads the held keys into the register it masks, as
   * ☀️'s volatile gKeysHeld does; read plainly, regmove copies the mask instead
   * (differ 16) */
  volatile u32 *held;

  Audio_PlayCue(3);
  Func_08016180();
  n = 19;
  tile = (u16 *)BG_SCREEN_BLOCK(4) + 16 * 32 + 19;
  value = (s16)0xf093;
  do {
    n--;
    *tile = value;
    tile--;
    value--;
  } while (n >= 0);
  Cpu_Fill32(0, gMapCellBuffer, 0x400);
  SerialRuntime_WaitForStatusMask(3);
restart:
  SerialRuntime_BeginReceive(gMapCellBuffer);
  held = &gInput.held;
  for (;;) {
    if (*held & 1)
      SerialRuntime_BeginSend((void *)Cart_Restart, 0x280);
    if (*held & 2)
      SerialRuntime_BeginSend((void *)(Cartridge_Restart + 0x1000), 0x280);
    if (*held & 8) {
      tick = 9999;
      do {
        tick--;
        Func_08016bd8();
      } while (tick >= 0);
    }
    if (gSerialReceiveDest == 0) {
      Dma_Set(gMapCellBuffer, (void *)0x06001000, 0x840000a0, REG_DMA3);
      goto restart;
    }
    WaitFrames(1);
  }
}
