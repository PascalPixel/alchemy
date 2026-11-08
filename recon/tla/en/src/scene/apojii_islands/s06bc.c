#include "../../../../games/tla/src/scene/apojii_islands/local.h"
#include "io_write_queue.h"

extern volatile s32 gData_020024cc[];
extern u16 gData_02002a58[16];
extern u16 gData_02002a78[16];

s32 __modsi3(s32 a, s32 b);

void Func_020006bc(void) {
  s32 base;
  s32 i;

  if (gData_020024cc[0] == -1) {
    Iwram_CopyWords(gData_02002a58, (void *)SPLIT_CONSTANT(0x5000000, 128), 32);
    gData_020024cc[0] = 0;
  }
  base = (gData_020024cc[0] / 2) % 15;
  for (i = 0; i < 15; i++)
    gData_02002a78[(base + i) % 15 + 1] = gData_02002a58[i + 1];
  IO_WRITE_QUEUE_PUSH(0x050003c0, (u32)gData_02002a78, 0x84000008);
  gData_020024cc[0]++;
}
