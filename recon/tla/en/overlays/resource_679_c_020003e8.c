// Draft, differ 129 of 156: Func_02000540's twin for the palettes at 0x05000160
// and 0x05000180, with its own buffers. Left: as its twin's draft, and this draft loads both palette addresses from the literal pool where the native builds 0x05000160 and 0x05000180.
#include "types.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "io_write_queue.h"
#include "system/io_reg.h"

extern s32 gData_020024c4;
extern u16 Data_02002958[], Data_02002978[], gData_02002998[], gData_020029b8[];

void Func_020003e8(void) {
  u8 *work = Ram_HeapSlots->eventWork;
  s32 busy, phase, k, j;

  if (*(s16 *)(work + 0x166) != 0)
    return;
  busy = *(s8 *)(work + 0xca4);
  if (busy != 0)
    return;
  if (gData_020024c4 == -1) {
    Iwram_CopyWords(Data_02002958, (void *)0x05000160, 32);
    Iwram_CopyWords(Data_02002978, Data_02002958, 32);
    Iwram_CopyWords(gData_02002998, (void *)0x05000180, 32);
    Iwram_CopyWords(gData_020029b8, gData_02002998, 32);
    gData_020024c4 = busy;
  }
  phase = gData_020024c4 / 8 % 4;
  for (k = 0; k <= 3; k++) {
    j = (phase + k) % 4;
    Data_02002978[12 + j] = Data_02002958[12 + k];
    gData_020029b8[12 + j] = gData_02002998[12 + k];
  }
  IO_WRITE_QUEUE_PUSH(0x05000160, (u32)Data_02002978, 0x84000008);
  IO_WRITE_QUEUE_PUSH(0x05000180, (u32)gData_020029b8, 0x84000008);
  gData_020024c4++;
}
