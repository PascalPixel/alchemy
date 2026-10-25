// Draft, differ 43 of 145: a palette cycle for two 16-colour palettes (from
// 0x05000160 and 0x050001a0): unless the event work is busy or its byte
// 0xca4 is set, saves both palettes and working copies on the first frame,
// then every eight frames rotates their last four colours one place and
// queues both copies' transfer for the next frame. Left: the native keeps the busy byte through a sign-extending load in r8 and its first exit test uses r5 for the zero; the rotation loop is close.
#include "types.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "io_write_queue.h"
#include "system/io_reg.h"

extern s32 gData_020024c8;
extern u16 Data_020029d8[], Data_020029f8[], Data_02002a18[], Data_02002a38[];

void Func_02000540(void) {
  u8 *work = Ram_HeapSlots->eventWork;
  s32 busy, phase, k, j;

  if (*(s16 *)(work + 0x166) != 0)
    return;
  busy = *(s8 *)(work + 0xca4);
  if (busy != 0)
    return;
  if (gData_020024c8 == -1) {
    Iwram_CopyWords(Data_020029d8, (void *)0x05000160, 32);
    Iwram_CopyWords(Data_020029f8, Data_020029d8, 32);
    Iwram_CopyWords(Data_02002a18, (void *)0x050001a0, 32);
    Iwram_CopyWords(Data_02002a38, Data_02002a18, 32);
    gData_020024c8 = busy;
  }
  phase = gData_020024c8 / 8 % 4;
  for (k = 0; k <= 3; k++) {
    j = (phase + k) % 4;
    Data_020029f8[12 + j] = Data_020029d8[12 + k];
    Data_02002a38[12 + j] = Data_02002a18[12 + k];
  }
  IO_WRITE_QUEUE_PUSH(0x05000160, (u32)Data_020029f8, 0x84000008);
  IO_WRITE_QUEUE_PUSH(0x050001a0, (u32)Data_02002a38, 0x84000008);
  gData_020024c8++;
}
